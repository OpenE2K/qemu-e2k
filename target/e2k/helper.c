#include "qemu/osdep.h"
#include "qemu/log.h"
#include "cpu.h"
#include "exec/exec-all.h"
#include "qemu/host-utils.h"
#include "exec/helper-proto.h"
#include "translate.h"

#define PS_FORCE_FX true

static inline void reset_ctprs(CPUE2KState *env)
{
    unsigned int i;

    for (i = 0; i < 3; i++) {
        env->ctprs[i].tag = CTPR_TAG_NONE;
    }
}

static inline void stack_push(CPUE2KState *env, E2KStackState *s, uint64_t value)
{
    if ((s->index + 8) > s->size) {
        helper_raise_exception(env, E2K_EXCP_MAPERR);
        return;
    }

    cpu_stq_le_data_ra(env, s->base + s->index, value, GETPC());
    s->index += 8;
}

static inline uint64_t stack_pop(CPUE2KState *env, E2KStackState *s)
{
    if (s->index < 8) {
        helper_raise_exception(env, E2K_EXCP_MAPERR);
        return 0;
    }

    s->index -= 8;
    return cpu_ldq_le_data(env, s->base + s->index);
}

#define pcs_push(env, value) stack_push(env, &env->pcsp, (value))
#define pcs_pop(env) stack_pop(env, &env->pcsp)
#define ps_push(env, value) stack_push(env, &env->psp, (value))
#define ps_pop(env) stack_pop(env, &env->psp)

static void proc_chain_save(CPUE2KState *env, int wbs)
{
    env->pshtp.index += wbs * 2;

    env->cr1.wbs = wbs;
    env->cr1.wpsz = env->wd.psize / 2;
    env->cr1.wfx = env->wd.fx;
    env->cr1.br = e2k_state_br(env);

    pcs_push(env, env->cr0_lo);
    pcs_push(env, env->cr0_hi);
    pcs_push(env, e2k_state_cr1_lo(env));
    pcs_push(env, e2k_state_cr1_hi(env));

    env->wd.fx = true;
    env->wd.base = (E2K_NR_COUNT + env->wd.base + wbs * 2) % E2K_NR_COUNT;
    env->wd.size -= wbs * 2;
    env->wd.psize = env->wd.size;
}

static inline void proc_chain_restore(CPUE2KState *env)
{
    int wbs;

    e2k_state_cr1_hi_set(env, pcs_pop(env));
    e2k_state_cr1_lo_set(env, pcs_pop(env));
    env->cr0_hi = pcs_pop(env); // FIXME: is it necessary to restore ip?
    env->cr0_lo = pcs_pop(env);

    wbs = env->cr1.wbs;
    e2k_state_br_set(env, env->cr1.br);
    env->wd.size = env->wd.psize + wbs * 2;
    env->wd.psize = env->cr1.wpsz * 2;
    env->wd.base = (E2K_NR_COUNT + env->wd.base - wbs * 2) % E2K_NR_COUNT;
    env->wd.fx = env->cr1.wfx;

    env->pshtp.index -= wbs * 2;
}

static inline void ps_spill(CPUE2KState *env, bool force, bool force_fx)
{
    while (E2K_NR_COUNT < env->pshtp.index + env->wd.size ||
        (force && env->pshtp.index))
    {
        int i = (E2K_NR_COUNT + env->wd.base - env->pshtp.index) % E2K_NR_COUNT;
        // TODO: push tags
        ps_push(env, env->regs[i]);
        ps_push(env, env->regs[i + 1]);
        env->regs[i] = 0;
        env->regs[i + 1] = 0;
        // TODO: set invalid reg tags
//        e2k_state_reg_tag_set_i64(env, E2K_TAG_NON_NUMBER64, i);
//        e2k_state_reg_tag_set_i64(env, E2K_TAG_NON_NUMBER64, i + 1);
        // TODO: push fx
        if (force_fx) {
            ps_push(env, env->xregs[i]);
            ps_push(env, env->xregs[i + 1]);
        }
        env->pshtp.index -= 2;
    }
}

static inline void ps_fill(CPUE2KState *env, bool force_fx)
{
    while(env->pshtp.index < 0) {
        env->pshtp.index += 2;
        int i = (E2K_NR_COUNT + env->wd.base - env->pshtp.index) % E2K_NR_COUNT;
        // TODO: pop fx
        if (force_fx) {
            env->xregs[i + 1] = ps_pop(env);
            env->xregs[i] = ps_pop(env);
        }
        // TODO: restore tags
        env->regs[i + 1] = ps_pop(env);
        env->regs[i] = ps_pop(env);
    }
}

static inline void do_call(CPUE2KState *env, int wbs, target_ulong pc_next)
{
    env->ip = pc_next;
    proc_chain_save(env, wbs);
    reset_ctprs(env);
}

void helper_setwd(CPUE2KState *env, uint32_t lts)
{
    env->wd.size = extract32(lts, 5, 7) * 2;
    env->wd.fx = extract32(lts, 4, 1) == 0;

    if (env->version >= 3) {
        bool dbl = extract32(lts, 3, 1);
        // TODO: set dbl
        if (dbl != false) {
            qemu_log_mask(LOG_UNIMP, "0x%lx: dbl is not implemented!\n",
                env->ip);
        }
        env->cr1.wdbl = dbl;
    }

    ps_spill(env, false, PS_FORCE_FX);
}

uint64_t helper_prep_return(CPUE2KState *env, int ipd)
{
    E2KCtpr ret = { 0 };

    if (env->pcsp.index < 32) {
        helper_raise_exception(env, E2K_EXCP_MAPERR);
        return 0;
    }

    ret.base = cpu_ldq_le_data(env, env->pcsp.base + env->pcsp.index - 24);
    ret.tag = CTPR_TAG_RETURN;
    ret.ipd = ipd;

    return ret.raw;
}

void helper_return(CPUE2KState *env)
{
    proc_chain_restore(env);
    ps_fill(env, PS_FORCE_FX);
    reset_ctprs(env);
}

static inline void do_syscall(CPUE2KState *env, int call_wbs,
    target_ulong pc_next)
{
    env->ip = pc_next;
    env->syscall_wbs = call_wbs;
    reset_ctprs(env);
    helper_raise_exception(env, E2K_EXCP_SYSCALL);
}

void helper_call(CPUE2KState *env, uint64_t ctpr_raw, int call_wbs,
    target_ulong pc_next)
{
    E2KCtpr ctpr = { .raw = ctpr_raw };

    switch (ctpr.tag) {
    case CTPR_TAG_DISP:
        do_call(env, call_wbs, pc_next);
        env->ip = ctpr.base;
        break;
    case CTPR_TAG_SDISP:
        do_syscall(env, call_wbs, pc_next);
        break;
    default:
        abort();
        break;
    }
}

void helper_raise_exception(CPUE2KState *env, int tt)
{
    CPUState *cs = env_cpu(env);
    cs->exception_index = tt;
    cpu_loop_exit(cs);
}

void e2k_break_save_state(CPUE2KState *env)
{
    env->is_bp = true;
    proc_chain_save(env, env->wd.size / 2);
    env->wd.size = 0;
    env->wd.psize = 0;
    ps_spill(env, true, true);
}

void helper_break_restore_state(CPUE2KState *env)
{
    env->is_bp = false;
    proc_chain_restore(env);
    ps_fill(env, true);
}

void helper_debug(CPUE2KState *env)
{
    CPUState *cs = env_cpu(env);
    e2k_break_save_state(env);
    cs->exception_index = EXCP_DEBUG;
    cpu_loop_exit(cs);
}

void helper_debug_i32(uint32_t x)
{
    qemu_log_mask(LOG_UNIMP, "log %#x\n", x);
}

void helper_debug_i64(uint64_t x)
{
    qemu_log_mask(LOG_UNIMP, "log %#lx\n", x);
}

void helper_debug_ptr(void *x)
{
    qemu_log_mask(LOG_UNIMP, "log %#lx\n", (uint64_t) x);
}
