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
        env->ctprs[i] = SET_FIELD(env->ctprs[i], CTPR_TAG_NONE,
            CTPR_TAG_OFF, CTPR_TAG_LEN);
    }
}

static inline void save_br_state(CPUE2KState *env)
{
    int rbs, rsz, rcur;

    rbs = env->boff / 2;
    rsz = (env->bsize - 2) / 2;
    rcur = env->bcur / 2;

    env->br = SET_FIELD(env->br, rbs, BR_RBS_OFF, BR_RBS_LEN);
    env->br = SET_FIELD(env->br, rsz, BR_RSZ_OFF, BR_RSZ_LEN);
    env->br = SET_FIELD(env->br, rcur, BR_RCUR_OFF, BR_RCUR_LEN);
    env->br = SET_FIELD(env->br, env->psize, BR_PSZ_OFF, BR_PSZ_LEN);
    env->br = SET_FIELD(env->br, env->pcur, BR_PCUR_OFF, BR_PCUR_LEN);

    e2k_state_cr1_br_set(env, env->br);
}

static inline void restore_br_state(CPUE2KState *env)
{
    int rbs, rsz, rcur;

    env->br = e2k_state_cr1_br_get(env);
    rbs = GET_FIELD(env->br, BR_RBS_OFF, BR_RBS_END);
    rsz = GET_FIELD(env->br, BR_RSZ_OFF, BR_RSZ_END);
    rcur = GET_FIELD(env->br, BR_RCUR_OFF, BR_RCUR_END);

    env->boff = rbs * 2;
    env->bsize = rsz * 2 + 2;
    env->bcur = rcur * 2;
    env->psize = GET_FIELD(env->br, BR_PSZ_OFF, BR_PSZ_END);
    env->pcur = GET_FIELD(env->br, BR_PCUR_OFF, BR_PCUR_END);
}

void helper_unimpl(CPUE2KState *env)
{
    CPUState *cs = env_cpu(env);

    cs->exception_index = E2K_EXCP_UNIMPL;
    cpu_loop_exit(cs);
}

static void pcs_push(CPUE2KState *env, int wbs)
{
    size_t size = e2k_state_pcs_size_get(env);
    size_t offset = e2k_state_pcs_index_get(env);
    void *pcsp = (void*) e2k_state_pcs_base_get(env) + offset;

    if (offset + 32 > size) {
        /* TODO: allocate more memory */
        abort();
    }

    save_br_state(env);
    e2k_state_cr1_wpsz_set(env, env->wd_psize / 2);
    memcpy(pcsp, &env->proc_chain[0], 32);
    e2k_state_cr1_wbs_set(env, wbs);

    e2k_state_pcs_index_set(env, offset + 32);
}

static void pcs_pop(CPUE2KState *env)
{
    size_t offset = e2k_state_pcs_index_get(env);
    void *pcsp = (void*) e2k_state_pcs_base_get(env) + offset - 32;

    if (offset < 32) {
        /* TODO: SIGKILL */
        abort();
    }

    memcpy(&env->proc_chain[0], pcsp, 32);

    // TODO: restore wbs (after pshtp implemented)
    env->wd_psize = e2k_state_cr1_wpsz_get(env) * 2;
    restore_br_state(env);

    e2k_state_pcs_index_set(env, offset - 32);
}

static void ps_push(CPUE2KState *env, unsigned int wd_base, size_t len)
{
    unsigned int i;
    size_t index = e2k_state_ps_ind_get(env);
    uint64_t *p = (uint64_t*) (e2k_state_ps_base_get(env) + index);

    /* TODO: push FX registers */
    /* TODO: stack overflow */

    for (i = 0; i < len; i++) {
        uint64_t reg = env->wregs[(wd_base + i) % WREGS_SIZE];
        memcpy(p + i, &reg, sizeof(uint64_t));
    }

    e2k_state_ps_ind_set(env, index + len * sizeof(uint64_t));
}

static void ps_pop(CPUE2KState *env, unsigned int wd_base, size_t len)
{
    unsigned int i;
    size_t index = e2k_state_ps_ind_get(env) - len * sizeof(uint64_t);
    uint64_t *p = (uint64_t*) (e2k_state_ps_base_get(env) + index);

    /* TODO: pop FX registers */
    /* TODO: stack overflow */

    for (i = 0; i < len; i++) {
        uint64_t *reg = &env->wregs[(wd_base + i) % WREGS_SIZE];
        memcpy(reg, p + i, sizeof(uint64_t));
    }

    e2k_state_ps_ind_set(env, index);
}

static inline void do_call(CPUE2KState *env, int call_wbs)
{
    int call_wpsz = env->wd_size / 2 - call_wbs;

    env->ip = env->nip;
    pcs_push(env, call_wbs);
    ps_push(env, env->wd_base, call_wbs * 2);

    env->wd_base = (env->wd_base + call_wbs * 2) % WREGS_SIZE;
    env->wd_size = env->wd_psize = call_wpsz * 2;

    reset_ctprs(env);
}

void helper_return(CPUE2KState *env)
{
    uint32_t new_wd_size, new_wd_base, wbs;

    wbs = e2k_state_cr1_wbs_get(env);
    new_wd_size = env->wd_psize + wbs * 2;
    new_wd_base = (env->wd_base - wbs * 2) % WREGS_SIZE;

    if (env->wd_base < new_wd_base) {
        env->wd_base += WREGS_SIZE;
    }

    ps_pop(env, new_wd_base, env->wd_base - new_wd_base);
    pcs_pop(env);

    env->wd_base = new_wd_base;
    env->wd_size = new_wd_size;

    reset_ctprs(env);
}

static inline void do_syscall(CPUE2KState *env, int call_wbs)
{
    env->syscall_wbs = call_wbs;
    reset_ctprs(env);
    helper_raise_exception(env, E2K_EXCP_SYSCALL);
}

target_ulong helper_call(CPUE2KState *env, uint64_t ctpr,
    int call_wbs)
{
    int ctpr_tag = GET_FIELD(ctpr, CTPR_TAG_OFF, CTPR_TAG_END);

    switch (ctpr_tag) {
    case CTPR_TAG_DISP:
        do_call(env, call_wbs);
        return GET_FIELD(ctpr, CTPR_BASE_OFF, CTPR_BASE_END);
    case CTPR_TAG_SDISP:
        do_syscall(env, call_wbs);
        return env->ip;
    default:
        helper_raise_exception(env, E2K_EXCP_UNIMPL);
        return env->ip;
    }
}

void helper_raise_exception(CPUE2KState *env, int tt)
{
    CPUState *cs = env_cpu(env);
    save_br_state(env);
    cs->exception_index = tt;
    cpu_loop_exit(cs);
}

static void break_save_ps(CPUE2KState *env)
{
    unsigned int i;
    unsigned int wd_base = env->wd_base;
    unsigned int len = env->wd_size;
    size_t index = e2k_state_ps_ind_get(env);
    uint64_t *p = (uint64_t*) (e2k_state_ps_base_get(env) + index);

    /* TODO: stack overflow */

    for (i = 0; i < len; i += 2) {
        uint64_t t[2] = { 0 };
        uint64_t *reg = &env->wregs[(wd_base + i) % WREGS_SIZE];
        memcpy(p + i * 2, reg, 2 * sizeof(uint64_t));
        memcpy(p + i * 2 + 2, t, 2 * sizeof(uint64_t)); // TODO: fx part
    }

    e2k_state_ps_ind_set(env, index + len * 2 * sizeof(uint64_t));
}

static void break_save_state(CPUE2KState *env)
{
    break_save_ps(env);
    pcs_push(env, env->wd_size / 2);

    env->wd_base = (env->wd_base + env->wd_size) % WREGS_SIZE;
    env->wd_size = 0;
    env->wd_psize = 0;

    env->is_bp = true;
}

void helper_break_restore_state(CPUE2KState *env)
{
    size_t index;
    int wbs;

    wbs = e2k_state_cr1_wbs_get(env);
    pcs_pop(env);
    env->wd_size = wbs * 2;
    env->wd_base = (env->wd_base - env->wd_size) % WREGS_SIZE;

    index = e2k_state_ps_ind_get(env);
    e2k_state_ps_ind_set(env, index - env->wd_size * 2 * sizeof(uint64_t));

    env->is_bp = false;
}

void helper_debug(CPUE2KState *env)
{
    CPUState *cs = env_cpu(env);
    break_save_state(env);
    cs->exception_index = EXCP_DEBUG;
    cpu_loop_exit(cs);
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
        helper_raise_exception(env, E2K_EXCP_ILLOPC);
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
