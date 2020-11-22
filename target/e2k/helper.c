#include "qemu/osdep.h"
#include "qemu/log.h"
#include "cpu.h"
#include "exec/exec-all.h"
#include "qemu/host-utils.h"
#include "exec/helper-proto.h"
#include "translate.h"

static inline void reset_ctprs(CPUE2KState *env)
{
    unsigned int i;

    for (i = 0; i < 3; i++) {
        env->ctprs[i] = 0;
    }
}

void helper_save_cpu_state(CPUE2KState *env)
{
    uint32_t br = 0;

    br = SET_FIELD(br, env->boff / 2, BR_RBS_OFF, BR_RBS_LEN);
    br = SET_FIELD(br, (env->bsize - 2) / 2, BR_RSZ_OFF, BR_RSZ_LEN);
    br = SET_FIELD(br, env->bcur / 2, BR_RCUR_OFF, BR_RCUR_LEN);
    br = SET_FIELD(br, env->psize, BR_PSZ_OFF, BR_PSZ_LEN);
    br = SET_FIELD(br, env->pcur, BR_PCUR_OFF, BR_PCUR_LEN);

    env->cr1_hi = SET_FIELD(env->cr1_hi, br, CR1_HI_BR_OFF + BR_BN_OFF,
        BR_BN_LEN);

    env->cr1_lo = SET_FIELD(env->cr1_lo, env->wsize / 2,
        CR1_LO_WPSZ_OFF, CR1_LO_WPSZ_LEN);
}

static inline void restore_state(CPUE2KState *env)
{
    uint32_t br;
    int rbs, rsz, rcur, psz, pcur, wsz, wbs;

    br = GET_FIELD(env->cr1_hi, CR1_HI_BR_OFF, CR1_HI_BR_LEN);
    rbs = GET_FIELD(br, BR_RBS_OFF, BR_RBS_END);
    rsz = GET_FIELD(br, BR_RSZ_OFF, BR_RSZ_END);
    rcur = GET_FIELD(br, BR_RCUR_OFF, BR_RCUR_END);
    psz = GET_FIELD(br, BR_PSZ_OFF, BR_PSZ_END);
    pcur = GET_FIELD(br, BR_PCUR_OFF, BR_PCUR_END);

    wbs = GET_FIELD(env->cr1_lo, CR1_LO_WBS_OFF, CR1_LO_WBS_END);
    wsz = GET_FIELD(env->cr1_lo, CR1_LO_WPSZ_OFF, CR1_LO_WPSZ_END);

    env->boff = rbs * 2;
    env->bsize = rsz * 2 + 2;
    env->bcur = rcur * 2;
    env->psize = psz;
    env->pcur = pcur;

    env->woff = wbs * 2;
    env->wsize = wsz * 2;
}

void helper_unimpl(CPUE2KState *env)
{
    CPUState *cs = env_cpu(env);

    cs->exception_index = E2K_EXCP_UNIMPL;
    cpu_loop_exit(cs);
}

void helper_raise_exception(CPUE2KState *env, int tt)
{
    CPUState *cs = env_cpu(env);
    helper_save_cpu_state(env);
    cs->exception_index = tt;
    cpu_loop_exit(cs);
}

void helper_debug(CPUE2KState *env)
{
    CPUState *cs = env_cpu(env);
    helper_save_cpu_state(env);
    cs->exception_index = EXCP_DEBUG;
    cpu_loop_exit(cs);
}

static void pcs_push(CPUE2KState *env)
{
    size_t size = e2k_state_pcs_size_get(env);
    size_t offset = e2k_state_pcs_index_get(env);
    uint64_t *pcsp = (uint64_t *) (e2k_state_pcs_base_get(env) + offset);

    if (offset + 32 > size) {
        /* TODO: allocate more memory */
        abort();
    }

    memcpy(pcsp, &env->pf, 8);
    memcpy(pcsp + 1, &env->nip, 8);
    memcpy(pcsp + 2, &env->cr1_lo, 8);
    memcpy(pcsp + 3, &env->cr1_hi, 8);

    e2k_state_pcs_index_set(env, offset + 32);
}

static uint64_t pcs_pop(CPUE2KState *env)
{
    size_t offset = e2k_state_pcs_index_get(env);
    uint64_t *pcsp = (uint64_t *) (e2k_state_pcs_base_get(env) + offset - 32);
    uint64_t tgt;

    if (offset < 32) {
        /* TODO: SIGKILL */
        abort();
    }

    memcpy(&env->pf, pcsp, 8);
    memcpy(&tgt, pcsp + 1, 8);
    memcpy(&env->cr1_lo, pcsp + 2, 8);
    memcpy(&env->cr1_hi, pcsp + 3, 8);

    e2k_state_pcs_index_set(env, offset - 32);

    return tgt;
}

static void ps_push(CPUE2KState *env, unsigned int wbs, size_t len)
{
    unsigned int i;
    size_t index = e2k_state_ps_ind_get(env);
    uint64_t *p = (uint64_t*) (e2k_state_ps_base_get(env) + index);

    /* TODO: push FX registers */
    /* TODO: stack overflow */

    for (i = 0; i < len; i++) {
        uint64_t reg = env->wregs[(wbs + i) % WREGS_SIZE];
        memcpy(p + i, &reg, sizeof(uint64_t));
    }

    e2k_state_ps_ind_set(env, index + len * sizeof(uint64_t));
}

static void ps_pop(CPUE2KState *env, unsigned int wbs, size_t len)
{
    unsigned int i;
    size_t index = e2k_state_ps_ind_get(env) - len * sizeof(uint64_t);
    uint64_t *p = (uint64_t*) (e2k_state_ps_base_get(env) + index);

    /* TODO: pop FX registers */
    /* TODO: stack overflow */

    for (i = 0; i < len; i++) {
        uint64_t *reg = &env->wregs[(wbs + i) % WREGS_SIZE];
        memcpy(reg, p + i, sizeof(uint64_t));
    }

    e2k_state_ps_ind_set(env, index);
}

static inline void do_call(CPUE2KState *env, int call_wbs)
{
    int wbs, wsz, new_wbs, new_wsz;

    wbs = e2k_state_wbs_get(env);
    wsz = e2k_state_wsz_get(env);
    new_wbs = (wbs + call_wbs) % (WREGS_SIZE / 2);
    new_wsz = wsz - call_wbs;

    if (new_wsz < 0) {
        /* TODO: SIGSEGV */
        abort();
    }

    /* save procedure chain info */
    pcs_push(env);
    /* save regs */
    ps_push(env, wbs * 2, call_wbs * 2);

    e2k_state_wbs_set(env, new_wbs);
    e2k_state_wsz_set(env, new_wsz);

    /* restore woff, wsize, etc */
    restore_state(env);
    reset_ctprs(env);
}

target_ulong helper_return(CPUE2KState *env)
{
    int new_wbs, old_wbs;
    target_ulong tgt;

    old_wbs = e2k_state_wbs_get(env) * 2;

    /* restore procedure chain info */
    tgt = pcs_pop(env);

    new_wbs = e2k_state_wbs_get(env) * 2;

    if (old_wbs < new_wbs) {
        old_wbs += WREGS_SIZE;
    }

    ps_pop(env, new_wbs, old_wbs - new_wbs);

    /* restore woff, wsize, etc */
    restore_state(env);
    reset_ctprs(env);

    return tgt;
}

static inline void do_syscall(CPUE2KState *env, int call_wbs)
{
    do_call(env, call_wbs);
    helper_raise_exception(env, E2K_EXCP_SYSCALL);
    helper_return(env);
    reset_ctprs(env);
}

target_ulong helper_call(CPUE2KState *env, uint64_t ctpr,
    int call_wbs)
{
    int ctpr_tag = GET_FIELD(ctpr, CTPR_TAG_OFF, CTPR_TAG_END);

    helper_save_cpu_state(env);

    switch (ctpr_tag) {
    case CTPR_TAG_DISP:
        do_call(env, call_wbs);
        return GET_FIELD(ctpr, CTPR_BASE_OFF, CTPR_BASE_END);
    case CTPR_TAG_SDISP:
        do_syscall(env, call_wbs);
        return env->nip;
    default:
        helper_raise_exception(env, E2K_EXCP_UNIMPL);
        return env->nip;
    }
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

uint64_t helper_getsp(CPUE2KState *env, uint64_t src2) {
    uint64_t base = GET_FIELD(env->usd_lo, USD_LO_BASE_OFF, USD_LO_BASE_END);

    base += src2;

    /* TODO: stack overflow */
    env->usd_lo = SET_FIELD(env->usd_lo, base, USD_LO_BASE_OFF,
        USD_LO_BASE_LEN);

    return base;
}

uint32_t helper_cur_dec(CPUE2KState *env, uint32_t cur, uint32_t n,
    uint32_t size)
{
    if (size == 0) {
        helper_raise_exception(env, E2K_EXCP_MAPERR);
    }

    return size - (size + n - cur) % size;
}
