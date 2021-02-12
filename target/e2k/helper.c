#include "qemu/osdep.h"
#include "qemu/log.h"
#include "cpu.h"
#include "exec/exec-all.h"
#include "qemu/host-utils.h"
#include "exec/helper-proto.h"
#include "translate.h"

#define PS_FORCE_FX true

void helper_signal_frame(CPUE2KState *env, int wbs, target_ulong ret_ip);

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

static void move_regs(CPUE2KState *env, int dst, int src, int n)
{
    memmove(&env->regs[dst], &env->regs[src], n * sizeof(env->regs[0]));
    memmove(&env->tags[dst], &env->tags[src], n * sizeof(env->tags[0]));
    memmove(&env->xregs[dst], &env->xregs[src], n * sizeof(env->xregs[0]));
}

static void callee_window(CPUE2KState *env, int base, int size, bool fx)
{
    ps_spill(env, base, fx);
    move_regs(env, 0, base, size - base);
}

static void caller_window(CPUE2KState *env, int base, int params, bool fx)
{
    move_regs(env, base, 0, params);
    ps_fill(env, base, fx);
}

static void crs_write(CPUE2KState *env, target_ulong addr, E2KCrs *crs)
{
    cpu_stq_le_data(env, addr + offsetof(E2KCrs, cr0_lo), crs->cr0_lo);
    cpu_stq_le_data(env, addr + offsetof(E2KCrs, cr0_hi), crs->cr0_hi);
    cpu_stq_le_data(env, addr + offsetof(E2KCrs, cr1.lo), crs->cr1.lo);
    cpu_stq_le_data(env, addr + offsetof(E2KCrs, cr1.hi), crs->cr1.hi);
}

static void crs_read(CPUE2KState *env, target_ulong addr, E2KCrs *crs)
{
    crs->cr0_lo = cpu_ldq_le_data(env, addr + offsetof(E2KCrs, cr0_lo));
    crs->cr0_hi = cpu_ldq_le_data(env, addr + offsetof(E2KCrs, cr0_hi));
    crs->cr1.lo = cpu_ldq_le_data(env, addr + offsetof(E2KCrs, cr1.lo));
    crs->cr1.hi = cpu_ldq_le_data(env, addr + offsetof(E2KCrs, cr1.hi));
}

static void pcs_push(CPUE2KState *env, E2KCrs *crs)
{
    if ((env->pcsp.index + sizeof(E2KCrs) * 2) > env->pcsp.size) {
        // TODO: stack expand (switch to next stack)
        qemu_log_mask(LOG_UNIMP, "e2k stack expand\n");
        helper_raise_exception(env, E2K_EXCP_MAPERR);
    }

    env->pcsp.index += sizeof(E2KCrs);
    crs_write(env, env->pcsp.base + env->pcsp.index, crs);
}

static void pcs_pop(CPUE2KState *env, E2KCrs *crs)
{
    crs_read(env, env->pcsp.base + env->pcsp.index, crs);

    if (env->pcsp.index < sizeof(E2KCrs)) {
        // TODO: stack shrink (switch to previous stack)
        qemu_log_mask(LOG_UNIMP, "e2k stack shrink\n");
    } else {
        env->pcsp.index -= sizeof(E2KCrs);
    }
}

static void proc_call(CPUE2KState *env, int base, target_ulong ret_ip,
    bool force_fx)
{
    E2KCrs crs;

    crs.cr0_lo = env->pregs;
    crs.cr0_hi = ret_ip & ~7;
    crs.cr1.wbs = base / 2;
    crs.cr1.wpsz = env->wd.psize / 2;
    crs.cr1.wfx = env->wd.fx;
    crs.cr1.wdbl = env->wdbl;
    crs.cr1.br = e2k_state_br(env);
    crs.cr1.ussz = env->usd.size >> 4;

    pcs_push(env, &crs);
    callee_window(env, base, env->wd.size, env->wd.fx || force_fx);

    env->wd.fx = true;
    env->wd.size -= base;
    env->wd.psize = env->wd.size;
}

static void proc_return(CPUE2KState *env, bool force_fx)
{
    E2KCrs crs;
    int base;

    pcs_pop(env, &crs);
    base = crs.cr1.wbs * 2;

    caller_window(env, base, env->wd.psize, crs.cr1.wfx || force_fx);

    env->pregs = crs.cr0_lo;
    env->ip = crs.cr0_hi & ~7;
    e2k_state_br_set(env, crs.cr1.br);
    env->wd.size = env->wd.psize + base;
    env->wd.psize = crs.cr1.wpsz * 2;
    env->wd.fx = crs.cr1.wfx;
    env->wdbl = crs.cr1.wdbl;
    env->usd.size = crs.cr1.ussz << 4;
    env->usd.base = env->sbr - env->usd.size;
}

void HELPER(signal_frame)(CPUE2KState *env, int wd_size, target_ulong ret_ip)
{
    proc_call(env, wd_size, ret_ip, false);
}

void HELPER(signal_return)(CPUE2KState *env)
{
    proc_return(env, false);
}

static inline void do_call(CPUE2KState *env, int wbs, target_ulong ret_ip)
{
    proc_call(env, wbs * 2, ret_ip, false);
    reset_ctprs(env);
}

void HELPER(syscall)(CPUE2KState *env)
{
    CPUState *cs = env_cpu(env);
    cs->exception_index = E2K_EXCP_SYSCALL;
    cpu_loop_exit(cs);
}

void HELPER(call)(CPUE2KState *env, uint64_t ctpr_raw, int call_wbs,
    target_ulong pc_next)
{
    E2KCtpr ctpr = { .raw = ctpr_raw };

    switch (ctpr.tag) {
    case CTPR_TAG_DISP:
    case CTPR_TAG_SDISP:
        do_call(env, call_wbs, pc_next);
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
    target_ulong addr = env->pcsp.base + env->pcsp.index + offsetof(E2KCrs, cr0_hi);
    uint64_t cr0_hi = cpu_ldq_le_data(env, addr) & ~7;

    if (cr0_hi == E2K_SYSRET_ADDR_CTPR) {
        ret.base = E2K_SIGRET_ADDR;
        ret.opc = CTPR_OPC_SIGRET;
    } else {
        ret.base = cr0_hi;
    }

    ret.tag = CTPR_TAG_RETURN;
    ret.ipd = ipd;

    return ret.raw;
}

void HELPER(return)(CPUE2KState *env)
{
    CtprOpc opc = env->ctprs[2].opc;

    if (opc == CTPR_OPC_SIGRET) {
        CPUState *cs = env_cpu(env);
        env->wd.psize = 2;
        env->regs[0] = 119; /* TARGET_NR_sigreturn */
        env->tags[0] = E2K_TAG_NUMBER64;
        cs->exception_index = E2K_EXCP_SYSCALL;
        cpu_loop_exit(cs);
    } else {
        if (opc != 0) {
            qemu_log("%#lx: unknown return ctpr opc %d\n", env->ip, opc);
        }

        proc_return(env, false);
        reset_ctprs(env);
    }
}

void HELPER(raise_exception)(CPUE2KState *env, int tt)
{
    CPUState *cs = env_cpu(env);
    cs->exception_index = tt;
    proc_call(env, env->wd.size, env->ip, true);
    cpu_loop_exit(cs);
}

void HELPER(raise_exception_no_spill)(CPUE2KState *env, int tt)
{
    CPUState *cs = env_cpu(env);
    cs->exception_index = tt;
    cpu_loop_exit(cs);
}

void HELPER(setwd)(CPUE2KState *env, int wsz, int nfx, int dbl)
{
    int i, size = wsz * 2;

    if (size < env->wd.psize) {
        helper_raise_exception(env, E2K_EXCP_ILLOPN);
    }

    for (i = env->wd.size; i < size; i++) {
        env->tags[i] = E2K_TAG_NON_NUMBER64;
    }

    env->wd.size = size;
    env->wd.fx = nfx == 0;
    env->wdbl = dbl;
}

bool e2k_cpu_tlb_fill(CPUState *cs, vaddr address, int size,
     MMUAccessType access_type, int mmu_idx, bool probe, uintptr_t retaddr)
{
    E2KCPU *cpu = E2K_CPU(cs);
    CPUE2KState *env = &cpu->env;

    proc_call(env, env->wd.size, env->ip, true);

    cs->exception_index = E2K_EXCP_MAPERR;
    cpu_loop_exit_restore(cs, retaddr);
}

void HELPER(break_restore_state)(CPUE2KState *env)
{
    proc_return(env, true);
    env->is_bp = false;
}

void HELPER(debug)(CPUE2KState *env)
{
    CPUState *cs = env_cpu(env);
    env->is_bp = true;
    proc_call(env, env->wd.size, env->ip, true);
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
