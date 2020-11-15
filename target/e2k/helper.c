#include "qemu/osdep.h"
#include "qemu/log.h"
#include "cpu.h"
#include "exec/exec-all.h"
#include "qemu/host-utils.h"
#include "exec/helper-proto.h"
#include "translate.h"

void helper_raise_exception(CPUE2KState *env, int tt)
{
    CPUState *cs = env_cpu(env);

    cs->exception_index = tt;
    cpu_loop_exit(cs);
}

static inline void do_call(CPUE2KState *env, target_ulong ctpr)
{
    unsigned int offset, wbs, wsz, new_wbs, new_wsz;
    uint64_t *pcsp = (uint64_t *)
        GET_FIELD(env->pcsp_lo, PCSP_LO_BASE_OFF, PCSP_LO_BASE_END);

    wbs = e2k_state_wbs_get(env);
    wsz = e2k_state_wsz_get(env);
    new_wbs = (wbs + env->call_wbs) % WREGS_SIZE;
    new_wsz = wsz - env->call_wbs;

    /* save procedure chain */
    offset = (env->pcsp_hi & GEN_MASK(0, 31)) / 8;
    pcsp[offset + 0] = env->pf;
    pcsp[offset + 1] = env->nip;
    pcsp[offset + 2] = env->cr1_lo;
    pcsp[offset + 3] = env->cr1_hi;
    /* TODO: check stack overflow */
    env->pcsp_hi = env->pcsp_hi + 32;

    /* TODO: check if call_wbs is less or equal then wsz */
    e2k_state_wbs_set(env, new_wbs);
    e2k_state_wsz_set(env, new_wsz);
    /* TODO: check rbs/rsz after call */
    env->ip = GET_FIELD(ctpr, CTPR_BASE_OFF, CTPR_BASE_END);
}

static void do_return(CPUE2KState *env)
{
    unsigned int offset = env->pcsp_hi & GEN_MASK(0, 31);
    uint64_t *pcsp = (uint64_t *) (GET_FIELD(env->pcsp_lo, PCSP_LO_BASE_OFF,
        PCSP_LO_BASE_END) + offset - 32);
//    uint64_t *psp = (uint64_t *) GET_FIELD(env->psp_lo, PSP_LO_BASE_OFF,
//        PSP_LO_BASE_END);

    env->pf = pcsp[0];
    env->ip = pcsp[1];
    env->nip = env->ip + 8;
    env->cr1_lo = pcsp[2];
    env->cr1_hi = pcsp[3];
    /* TODO: check stack overflow */
    env->pcsp_hi = env->pcsp_hi - 32;

    // TODO: restore regs
}

void helper_call(CPUE2KState *env, target_ulong cond, target_ulong ctpr)
{
    unsigned int i = 0;

    if (!cond) {
        env->ip = env->nip;
        return;
    }

    for (i = 0; i < 4; i++) {
        env->ctprs[i] = 0;
    }

    switch (GET_FIELD(ctpr, CTPR_TAG_OFF, CTPR_TAG_END)) {
    case CTPR_TAG_SDISP: {
        CPUState *cs = env_cpu(env);

        env->ip = env->nip;
        cs->exception_index = E2K_EXCP_SYSCALL;
        cpu_loop_exit(cs);
        break;
    }
    case CTPR_TAG_DISP: {
        do_call(env, ctpr);
        break;
    }
    default:
        /* TODO: exception */
        abort();
        break;
    }
}

/* FIXME: combine with helper_call? */
void helper_jump(CPUE2KState *env, target_ulong cond, target_ulong ctpr)
{
    if (!cond) {
        env->ip = env->nip;
        return;
    }

    switch (GET_FIELD(ctpr, CTPR_TAG_OFF, CTPR_TAG_END)) {
    case CTPR_TAG_DISP: {
        env->ip = GET_FIELD(ctpr, CTPR_BASE_OFF, CTPR_BASE_END);
        break;
    }
    case CTPR_TAG_RETURN:
        do_return(env);
        break;
    default:
        /* TODO: exception */
        abort();
        break;
    }
}

void helper_setwd(CPUE2KState *env, uint32_t lts0)
{
    int wsz = GET_FIELD(lts0, 5, 11);
    /* TODO: store nfx */
//    int nfx = GET_BIT(lts0, 4);

    /* TODO: push wregs */

    e2k_state_wsz_set(env, wsz);

    if (env->version >= 3) {
//        int dbl = GET_BIT(lts0, 3);
        /* TODO: store dbl */
    }
}

void helper_setbn(CPUE2KState *env, uint32_t cs1)
{
    env->cr1_hi = SET_FIELD(
        env->cr1_hi,
        GET_FIELD(cs1, 0, 17),
        CR1_HI_BR_OFF + BR_BN_OFF,
        BR_BN_LEN
    );
}

uint64_t helper_sxt(uint64_t x, uint64_t y)
{
    int size;

    switch (x & 3) {
    case 0:
        size = 8;
        break;
    case 1:
        size = 16;
        break;
    default:
        size = 32;
        break;
    }

    if (x & 4) {
        return y & GEN_MASK(0, size);
    } else {
        return (((int64_t) y) << (64 - size) >> (64 - size));
    }
}

void helper_debug_i32(uint32_t x)
{
    qemu_log_mask(LOG_UNIMP, "log %#x\n", x);
}

void helper_debug_i64(uint64_t x)
{
    qemu_log_mask(LOG_UNIMP, "log %#lx\n", x);
}

uint64_t helper_state_reg_get(CPUE2KState *env, int reg)
{
    switch (reg) {
    case 0x2c: /* %usd.hi */
        return env->usd_hi;
        break;
    case 0x2d: /* %usd.lo */
        return env->usd_lo;
        break;
    case 0x83: /* %lsr */
        return env->lsr;
        break;
    default:
        /* TODO: exception */
        abort();
        break;
    }
}

void helper_state_reg_set(CPUE2KState *env, int reg, uint64_t val)
{
    switch (reg) {
    case 0x2c: /* %usd.hi */
        /* FIXME: user cannot write */
        env->usd_hi = val;
        break;
    case 0x2d: /* %usd.lo */
        /* FIXME: user cannot write */
        env->usd_lo = val;
        break;
    case 0x83: /* %lsr */
        env->lsr = val;
        break;
    default:
        /* TODO: exception */
        abort();
        break;
    }
}

void helper_save_rcur(CPUE2KState *env, int rcur)
{
    env->cr1_hi = SET_FIELD(
        env->cr1_hi,
        rcur / 2,
        CR1_HI_BR_OFF + BR_RCUR_OFF,
        BR_RCUR_LEN
    );
}

void helper_save_pcur(CPUE2KState *env, int pcur)
{
    env->cr1_hi = SET_FIELD(
        env->cr1_hi,
        pcur,
        CR1_HI_BR_OFF + BR_PCUR_OFF,
        BR_PCUR_LEN
    );
}
