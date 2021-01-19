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
    env->wd.fx = env->crs.cr1.wfx;
    env->wdbl = env->crs.cr1.wdbl;
    env->usd.size = env->crs.cr1.ussz << 4;
    env->usd.base = env->sbr - env->usd.size;

    env->crs.cr1.hi = pcs_pop(env);
    env->crs.cr1.lo = pcs_pop(env);
    env->crs.cr0_hi = pcs_pop(env);
    env->crs.cr0_lo = pcs_pop(env);
}

static void ps_spill(CPUE2KState *env, int n, bool fx)
{
    int i;
    for (i = 0; i < n; i += 2) {
        ps_push(env, env->regs[i], env->tags[i]);
        ps_push(env, env->regs[i + 1], env->tags[i + 1]);
        if (fx) {
            ps_push(env, env->xregs[i + 0], 0);
            ps_push(env, env->xregs[i + 1], 0);
        }
    }
}

static void ps_fill(CPUE2KState *env, int n, bool fx)
{
    int i;
    for (i = n; i > 0; i -= 2) {
        if (fx) {
            env->xregs[i - 1] = ps_pop(env, NULL);
            env->xregs[i - 2] = ps_pop(env, NULL);
        }
        env->regs[i - 1] = ps_pop(env, &env->tags[i - 1]);
        env->regs[i - 2] = ps_pop(env, &env->tags[i - 2]);
    }
}

static inline void ps_spill_all(CPUE2KState *env)
{
    ps_spill(env, env->wd.size, env->wd.fx);
}

static void move_regs(CPUE2KState *env, int dst, int src, int n)
{
    memmove(&env->regs[dst], &env->regs[src], n * sizeof(env->regs[0]));
    memmove(&env->tags[dst], &env->tags[src], n * sizeof(env->tags[0]));
    memmove(&env->xregs[dst], &env->xregs[src], n * sizeof(env->xregs[0]));
}

static void callee_window(CPUE2KState *env, int wbs)
{
    int s = wbs * 2;
    ps_spill(env, s, env->wd.fx);
    move_regs(env, 0, s, env->wd.size - s);
}

static void caller_window(CPUE2KState *env)
{
    int s = env->crs.cr1.wbs * 2;
    move_regs(env, s, 0, env->wd.psize);
    ps_fill(env, s, env->crs.cr1.wfx);
}

static inline void do_call(CPUE2KState *env, int wbs, target_ulong ret_ip)
{
    callee_window(env, wbs);
    proc_chain_save(env, wbs * 2, ret_ip);
    reset_ctprs(env);
}

static inline void do_syscall(CPUE2KState *env, int wbs, target_ulong ret_ip)
{
    CPUState *cs = env_cpu(env);
    do_call(env, wbs, ret_ip);
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
    caller_window(env);
    proc_chain_restore(env);
    reset_ctprs(env);
}

void HELPER(raise_exception)(CPUE2KState *env, int tt)
{
    CPUState *cs = env_cpu(env);
    cs->exception_index = tt;
    ps_spill_all(env);
    proc_chain_save(env, env->wd.size, env->ip);
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
    int i, old_size = env->wd.size, size = extract32(lts, 5, 7) * 2;

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

    for (i = 0; i < size - old_size; i++) {
        env->tags[old_size + i] = E2K_TAG_NON_NUMBER64;
    }
}

bool e2k_cpu_tlb_fill(CPUState *cs, vaddr address, int size,
     MMUAccessType access_type, int mmu_idx, bool probe, uintptr_t retaddr)
{
    E2KCPU *cpu = E2K_CPU(cs);
    CPUE2KState *env = &cpu->env;

    ps_spill_all(env);
    proc_chain_save(env, env->wd.size, env->ip);

    cs->exception_index = E2K_EXCP_MAPERR;
    cpu_loop_exit_restore(cs, retaddr);
}

void e2k_break_save_state(CPUE2KState *env)
{
    env->is_bp = true;
    ps_spill_all(env);
    proc_chain_save(env, env->wd.size, env->ip);
}

void HELPER(break_restore_state)(CPUE2KState *env)
{
    proc_chain_restore(env);
    ps_fill(env, env->wd.size, env->wd.fx);
    env->is_bp = false;
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
