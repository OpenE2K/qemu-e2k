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

static inline void ps_push(CPUE2KState *env, uint64_t value, uint8_t tag)
{
    cpu_stb_data(env, env->psp.base_tag + env->psp.index / 8, tag);
    stack_push(env, &env->psp, value);
}

static inline uint64_t ps_pop(CPUE2KState *env, uint8_t *ret_tag)
{
    uint64_t ret = stack_pop(env, &env->psp);
    if (ret_tag != NULL) {
        *ret_tag = cpu_ldub_data(env, env->psp.base_tag + env->psp.index / 8);
    }
    return ret;
}

static void proc_chain_save(CPUE2KState *env, int wd_base, target_ulong ret_ip)
{
    env->pshtp.index += wd_base;

    pcs_push(env, env->crs.cr0_lo);
    pcs_push(env, env->crs.cr0_hi);
    pcs_push(env, env->crs.cr1.lo);
    pcs_push(env, env->crs.cr1.hi);

    env->crs.cr0_lo = env->pregs;
    env->crs.cr0_hi = ret_ip;
    env->crs.cr1.wbs = wd_base / 2;
    env->crs.cr1.wpsz = env->wd.psize / 2;
    env->crs.cr1.wfx = env->wd.fx;
    env->crs.cr1.wdbl = env->wdbl;
    env->crs.cr1.br = e2k_state_br(env);
    env->crs.cr1.ussz = env->usd.size >> 4;

    env->wd.fx = true;
    env->wd.base = e2k_wrap_reg_index(env->wd.base + wd_base);
    env->wd.size -= wd_base;
    env->wd.psize = env->wd.size;
}

static void proc_chain_restore(CPUE2KState *env)
{
    int wd_base;

    env->pregs = env->crs.cr0_lo;
    env->ip = env->crs.cr0_hi;
    wd_base = env->crs.cr1.wbs * 2;
    e2k_state_br_set(env, env->crs.cr1.br);
    env->wd.size = env->wd.psize + wd_base;
    env->wd.psize = env->crs.cr1.wpsz * 2;
    env->wd.base = e2k_wrap_reg_index(env->wd.base - wd_base);
    env->wd.fx = env->crs.cr1.wfx;
    env->wdbl = env->crs.cr1.wdbl;
    env->usd.size = env->crs.cr1.ussz << 4;
    env->usd.base = env->sbr - env->usd.size;

    env->crs.cr1.hi = pcs_pop(env);
    env->crs.cr1.lo = pcs_pop(env);
    env->crs.cr0_hi = pcs_pop(env);
    env->crs.cr0_lo = pcs_pop(env);

    env->pshtp.index -= wd_base;
}

/* Stores pair of registers to stack */
static inline void ps_spill_round(CPUE2KState *env, bool force_fx)
{
    int i = e2k_wrap_reg_index(env->wd.base - env->pshtp.index);
    ps_push(env, env->regs[i], env->tags[i]);
    ps_push(env, env->regs[i + 1], env->tags[i + 1]);

    // TODO: push fx
    if (force_fx) {
        ps_push(env, env->xregs[i], 0);
        ps_push(env, env->xregs[i + 1], 0);
    }

    env->regs[i] = 0;
    env->tags[i] = E2K_TAG_NON_NUMBER64;
    env->regs[i + 1] = 0;
    env->tags[i + 1] = E2K_TAG_NON_NUMBER64;

    env->pshtp.index -= 2;
}

/* Stores all windows to stack */
static void ps_spill_all(CPUE2KState *env, bool force_fx)
{
    while (env->wd.size + env->pshtp.index) {
        ps_spill_round(env, force_fx);
    }
}

/* Stores bottom windows to stack */
static void ps_spill_bottom(CPUE2KState *env, bool force_fx)
{
    while (env->pshtp.index > 0) {
        ps_spill_round(env, force_fx);
    }
}

/* Stores registers to stack till be enough space to hold current window */
static void ps_spill(CPUE2KState *env, bool force_fx)
{
    while (E2K_NR_COUNT < env->pshtp.index + env->wd.size) {
        ps_spill_round(env, force_fx);
    }
}

/* Restores current window from stack if needed */
static void ps_fill(CPUE2KState *env, bool force_fx)
{
    while(env->pshtp.index < 0) {
        env->pshtp.index += 2;
        int i = e2k_wrap_reg_index(env->wd.base - env->pshtp.index);
        if (force_fx) {
            env->xregs[i + 1] = ps_pop(env, NULL);
            env->xregs[i] = ps_pop(env, NULL);
        }
        env->regs[i + 1] = ps_pop(env, &env->tags[i + 1]);
        env->regs[i] = ps_pop(env, &env->tags[i]);
    }
}

static inline void do_call(CPUE2KState *env, int wbs, target_ulong ret_ip)
{
    proc_chain_save(env, wbs * 2, ret_ip);
    reset_ctprs(env);
}

static inline void do_syscall(CPUE2KState *env, int wbs, target_ulong ret_ip)
{
    CPUState *cs = env_cpu(env);
    do_call(env, wbs, ret_ip);
    ps_spill_bottom(env, PS_FORCE_FX);
    cs->exception_index = E2K_EXCP_SYSCALL;
    cpu_loop_exit(cs);
}

void HELPER(call)(CPUE2KState *env, uint64_t ctpr_raw, int call_wbs,
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
        env->ip = ctpr.base;
        break;
    default:
        helper_raise_exception(env, E2K_EXCP_ILLOPC);
        break;
    }
}

uint64_t HELPER(prep_return)(CPUE2KState *env, int ipd)
{
    E2KCtpr ret = { 0 };

    if (env->pcsp.index < 32) {
        helper_raise_exception(env, E2K_EXCP_MAPERR);
        return 0;
    }

    ret.base = env->crs.cr0_hi;
    ret.tag = CTPR_TAG_RETURN;
    ret.ipd = ipd;

    return ret.raw;
}

void HELPER(return)(CPUE2KState *env)
{
    proc_chain_restore(env);
    ps_fill(env, PS_FORCE_FX);
    reset_ctprs(env);
}

void HELPER(raise_exception)(CPUE2KState *env, int tt)
{
    CPUState *cs = env_cpu(env);
    cs->exception_index = tt;
    proc_chain_save(env, env->wd.size, env->ip);
    ps_spill_all(env, PS_FORCE_FX);
    cpu_loop_exit(cs);
}

void HELPER(raise_exception_no_spill)(CPUE2KState *env, int tt)
{
    CPUState *cs = env_cpu(env);
    cs->exception_index = tt;
    cpu_loop_exit(cs);
}

void HELPER(setwd)(CPUE2KState *env, uint32_t lts)
{
    int old_size = env->wd.size, size = extract32(lts, 5, 7) * 2;

    if (size < env->wd.psize) {
        helper_raise_exception(env, E2K_EXCP_ILLOPN);
        return;
    }

    env->wd.size = size;
    env->wd.fx = extract32(lts, 4, 1) == 0;

    if (env->version >= 3) {
        bool dbl = extract32(lts, 3, 1);
        env->wdbl = dbl;
    }

    ps_spill(env, PS_FORCE_FX);

    if (old_size < size) {
        unsigned int i, offset = env->wd.base + old_size;

        for (i = 0; i < size - old_size; i++) {
            int idx = e2k_wrap_reg_index(offset + i);
            env->regs[idx] = 0;
            env->tags[idx] = E2K_TAG_NON_NUMBER64;
        }
    }
}

bool e2k_cpu_tlb_fill(CPUState *cs, vaddr address, int size,
     MMUAccessType access_type, int mmu_idx, bool probe, uintptr_t retaddr)
{
    E2KCPU *cpu = E2K_CPU(cs);
    CPUE2KState *env = &cpu->env;

    proc_chain_save(env, env->wd.size, env->ip);
    ps_spill_all(env, PS_FORCE_FX);

    cs->exception_index = E2K_EXCP_MAPERR;
    cpu_loop_exit_restore(cs, retaddr);
}

void e2k_break_save_state(CPUE2KState *env)
{
    env->is_bp = true;
    proc_chain_save(env, env->wd.size, env->ip);
    ps_spill_all(env, PS_FORCE_FX);
}

void HELPER(break_restore_state)(CPUE2KState *env)
{
    env->is_bp = false;
    proc_chain_restore(env);
    ps_fill(env, true);
}

void HELPER(debug)(CPUE2KState *env)
{
    CPUState *cs = env_cpu(env);
    e2k_break_save_state(env);
    cs->exception_index = EXCP_DEBUG;
    cpu_loop_exit(cs);
}

void HELPER(debug_i32)(uint32_t x)
{
    qemu_log_mask(LOG_UNIMP, "log %#x\n", x);
}

void HELPER(debug_i64)(uint64_t x)
{
    qemu_log_mask(LOG_UNIMP, "log %#lx\n", x);
}

void HELPER(debug_ptr)(void *x)
{
    qemu_log_mask(LOG_UNIMP, "log %p\n", x);
}
