/*
 * Misc E2K helpers
 *
 * SPDX-License-Identifier: GPL-2.0-only
 */

#include "helper-tcg.h"
#include "qemu/log.h"
#include "qemu/host-utils.h"
#include "exec/helper-proto.h"
#include "exec/translation-block.h"

#define FRAME_SIZE(NREGS) ((NREGS) * 16)

static inline void reset_ctprs(CPUE2KState *env)
{
    unsigned int i;

    for (i = 0; i < 3; i++) {
        env->ctprs[i].tag = CTPR_TAG_NONE;
    }
}

static inline void ps_write(CPUE2KState *env, uint64_t value, uint8_t tag, int32_t index)
{
    cpu_stq_le_data(env, env->psp.base + index, value);

    if (env->enable_tags) {
        cpu_stb_data(env, env->psp.base_tag + index / 8, tag);
    }
}

static inline uint64_t ps_read(CPUE2KState *env, uint8_t *ret_tag, int32_t index)
{
    if (ret_tag != NULL) {
        if (env->enable_tags) {
            *ret_tag = cpu_ldub_data(env, env->psp.base_tag + index / 8);
        } else {
            *ret_tag = 0;
        }
    }

    return cpu_ldq_le_data(env, env->psp.base + index);
}

static void ps_spill(CPUE2KState *env, int n, bool fx)
{
    int32_t index = env->psp.index;

    // XXX: psp.size is set to some random value and does not show real stack size
#ifndef CONFIG_USER_ONLY
    if (env->psp.index + FRAME_SIZE(n) > env->psp.size) {
        raise_exception(env, E2K_EXCP_PROC_STACK_BOUNDS);
    }
#endif

    if (env->def.isa >= 5) {
        for (int i = 0; i < n; i++, index += 16) {
            ps_write(env, env->regs[i].lo, env->tags[i], index);

            if (fx) {
                ps_write(env, env->regs[i].hi, 0, index + 8);
            }
        }
    } else{
        for (int i = 0; i < n; i += 2, index += 32) {
            E2KReg r0 = env->regs[i + 0];
            E2KReg r1 = env->regs[i + 1];

            ps_write(env, r0.lo, env->tags[i], index);
            ps_write(env, r1.lo, env->tags[i + 1], index + 8);

            if (fx) {
                ps_write(env, r0.hi, 0, index + 16);
                ps_write(env, r1.hi, 0, index + 24);
            }
        }
    }

    env->psp.index = index;
}

static void ps_fill(CPUE2KState *env, int n, bool fx)
{
    int32_t index = env->psp.index, frame_size = FRAME_SIZE(n);

    if (index < frame_size) {
        raise_exception(env, E2K_EXCP_PROC_STACK_BOUNDS);
    }

    if (env->def.isa >= 5) {
        for (int i = n; i-- > 0; index -= 16) {
            if (fx) {
                env->regs[i].hi = ps_read(env, NULL, index - 8);
            }

            env->regs[i].lo = ps_read(env, &env->tags[i], index - 16);
        }
    } else {
        for (int i = n; i > 0; i -= 2, index -= 32) {
            E2KReg *r0 = &env->regs[i - 1];
            E2KReg *r1 = &env->regs[i - 2];

            if (fx) {
                r0->hi = ps_read(env, NULL, index - 8);
                r1->hi = ps_read(env, NULL, index - 16);
            }

            r0->lo = ps_read(env, &env->tags[i - 1], index - 24);
            r1->lo = ps_read(env, &env->tags[i - 2], index - 32);
        }
    }

    env->psp.index = index;
}

static void move_regs(CPUE2KState *env, int dst, int src, int n)
{
    memmove(&env->regs[dst], &env->regs[src], n * sizeof(env->regs[0]));
    if (env->enable_tags) {
        memmove(&env->tags[dst], &env->tags[src], n * sizeof(env->tags[0]));
    }
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

static void pcs_pop(CPUE2KState *env, E2KCrs *crs)
{
    crs_read(env, env->pcsp.base + env->pcsp.index, crs);

    if (env->pcsp.index < sizeof(E2KCrs)) {
        raise_exception(env, E2K_EXCP_CHAIN_STACK_BOUNDS);
    } else {
        env->pcsp.index -= sizeof(E2KCrs);
    }
}

void e2k_proc_call(CPUE2KState *env, int base, target_ulong ret_ip,
    bool force_fx)
{
    E2KCrs crs;

    crs.cr0_lo = env->pregs;
    crs.cr0_hi = ret_ip & ~7;
    crs.cr1.wbs = base / 2;
    crs.cr1.wpsz = env->wd.psize / 2;
    crs.cr1.wfx = env->wd.fx;
    crs.cr1.wdbl = env->wdbl;
    crs.cr1.psr = env->psr;
    crs.cr1.cuir = env->cuir;
    crs.cr1.br = env_br_get(env);
    crs.cr1.ussz = env->usd.size >> 4;

    env->pcsp.index += sizeof(E2KCrs);
    crs_write(env, env->pcsp.base + env->pcsp.index, &crs);

    callee_window(env, base, env->wd.size, env->wd.fx || force_fx);

    env->wd.fx = true;
    env->wd.size -= base;
    env->wd.psize = env->wd.size;
}

void e2k_proc_return(CPUE2KState *env, bool force_fx)
{
    E2KCrs crs;
    int base;

    pcs_pop(env, &crs);
    base = crs.cr1.wbs * 2;

    caller_window(env, base, env->wd.psize, crs.cr1.wfx || force_fx);

    env->pregs = crs.cr0_lo;
    env->ip = crs.cr0_hi & ~7;
    env_br_set(env, crs.cr1.br);
    env->cuir = crs.cr1.cuir;
    env->psr = crs.cr1.psr;
    env->wd.size = env->wd.psize + base;
    env->wd.psize = crs.cr1.wpsz * 2;
    env->wd.fx = crs.cr1.wfx;
    env->wdbl = crs.cr1.wdbl;
    env->usd.size = crs.cr1.ussz << 4;
    env->usd.base = env->sbr - env->usd.size;
}

static inline void do_call(CPUE2KState *env, int wbs, target_ulong ret_ip)
{
    e2k_proc_call(env, wbs * 2, ret_ip, false);
    reset_ctprs(env);
}

#ifdef CONFIG_USER_ONLY
void HELPER(expand_proc_chain_stack)(CPUE2KState *env)
{
    if ((env->pcsp.size - env->pcsp.index) <= (sizeof(E2KCrs) * 2)) {
        raise_exception_ra(env, E2K_EXCP_CHAIN_STACK_BOUNDS, GETPC());
    }
}
#endif /* CONFIG_USER_ONLY */

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
        raise_exception(env, E2K_EXCP_ILLEGAL_OPCODE);
        break;
    }
}

void HELPER(icalld)(CPUE2KState *env, target_ulong target, int call_wbs,
        target_ulong pc_next)
{
    do_call(env, call_wbs, pc_next);
    env->ip = target;
}

void HELPER(iret)(CPUE2KState *env)
{
    e2k_proc_return(env, false);
    reset_ctprs(env);
}

uint64_t HELPER(prep_return)(CPUE2KState *env, int ipd)
{
    E2KCtpr ret = { 0 };
    target_ulong addr = env->pcsp.base + env->pcsp.index + offsetof(E2KCrs, cr0_hi);
    uint64_t cr0_hi = cpu_ldq_le_data(env, addr) & ~7;

    ret.ipd = ipd;
    ret.base = cr0_hi;
    ret.tag = CTPR_TAG_RETURN;
#ifdef CONFIG_USER_ONLY
    ret.opc = cr0_hi == E2K_SIGRET_ADDR ? CTPR_OPC_SIGRET : 0;
#else
    // TODO: set ctpr.opc
    ret.opc = 0;
#endif

    return ret.raw;
}

void HELPER(return)(CPUE2KState *env)
{
    CtprOpc opc = env->ctprs[2].opc;

#ifdef CONFIG_USER_ONLY
    if (opc == CTPR_OPC_SIGRET) {
        CPUState *cs = env_cpu(env);
        env->wd.psize = 2;
        env->wreg[0].lo = 119; /* TARGET_NR_sigreturn */
        if (env->enable_tags) {
            env->wtag[0] = E2K_TAG_NUMBER64;
        }
        cs->exception_index = E2K_EXCP_SYSCALL;
        cpu_loop_exit(cs);
    }
#endif

    if (opc != 0) {
        qemu_log(TARGET_FMT_lx ": unknown return ctpr opc %d\n", env->ip, opc);
    }

    e2k_proc_return(env, false);
    reset_ctprs(env);
}

void G_NORETURN raise_exception(CPUE2KState *env, int exception_index)
{
    raise_exception_ra(env, exception_index, 0);
}

void G_NORETURN raise_exception_ra(CPUE2KState *env, int exception_index,
    uintptr_t retaddr)
{
    CPUState *cs = env_cpu(env);
    switch (exception_index) {
#ifdef CONFIG_USER_ONLY
    case E2K_EXCP_SYSCALL:
    case E2K_EXCP_SYSCALL_FAST:
#endif
    case E2K_EXCP_PROC_STACK_BOUNDS:
    case E2K_EXCP_CHAIN_STACK_BOUNDS:
        /* ignore */
        break;
    default:
        e2k_proc_call(env, env->wd.size, env->ip, true);
        break;
    }
    cs->exception_index = exception_index;
    cpu_loop_exit_restore(cs, retaddr);
}

void HELPER(raise_exception)(CPUE2KState *env, int exception_index)
{
    raise_exception(env, exception_index);
}

void HELPER(setwd)(CPUE2KState *env, int size, int nfx, int dbl)
{
    if (size < env->wd.psize) {
        raise_exception(env, E2K_EXCP_ILLEGAL_OPCODE);
    }

    if ((env->psp.size - env->psp.index) <= (E2K_REG_LEN * E2K_NR_COUNT * 4)) {
        raise_exception_ra(env, E2K_EXCP_PROC_STACK_BOUNDS, GETPC());
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

    e2k_proc_call(env, env->wd.size, env->ip, true);
    cs->exception_index = E2K_EXCP_DATA_PAGE;
    cpu_loop_exit_restore(cs, retaddr);
}
