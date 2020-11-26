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

static inline void save_proc_chain_info(CPUE2KState *env, uint64_t buf[4])
{
    env->cr1.br = e2k_state_br(env);
    env->cr1.wpsz = env->wd.psize / 2;

    buf[0] = env->cr0_lo;
    buf[1] = env->cr0_hi;
    buf[2] = e2k_state_cr1_lo(env);
    buf[3] = e2k_state_cr1_hi(env);
}

static inline void restore_proc_chain_info(CPUE2KState *env, uint64_t buf[4])
{
    e2k_state_cr1_hi_set(env, buf[3]);
    e2k_state_cr1_lo_set(env, buf[2]);
    env->cr0_hi = buf[1];
    env->cr0_lo = buf[0];

    env->wd.psize = env->cr1.wpsz * 2;
    e2k_state_br_set(env, env->cr1.br);
}

static void pcs_push(CPUE2KState *env, int wbs)
{
    uint64_t buf[4];

    if (env->pcsp.size < (env->pcsp.index + 32)) {
        helper_raise_exception(env, E2K_EXCP_MAPERR);
        return;
    }

    // save proc chain
    save_proc_chain_info(env, buf);
    memcpy(env->pcsp.base + env->pcsp.index, buf, 32);

    env->cr1.wbs = wbs;
    env->pcsp.index += 32;
}

static void pcs_pop(CPUE2KState *env)
{
    uint64_t buf[4];

    if (env->pcsp.index < 32) {
        helper_raise_exception(env, E2K_EXCP_MAPERR);
        return;
    }

    env->pcsp.index -= 32;
    memcpy(buf, env->pcsp.base + env->pcsp.index, 32);
    restore_proc_chain_info(env, buf);
}

static void ps_push_nfx(CPUE2KState *env, unsigned int base, size_t len)
{
    unsigned int i;
    size_t size = len * sizeof(uint64_t);
    uint64_t *p;

    if (env->psp.size < (env->psp.index + size)) {
        helper_raise_exception(env, E2K_EXCP_MAPERR);
        return;
    }

    p = (uint64_t *) (env->psp.base + env->psp.index);
    for (i = 0; i < len; i++) {
        int idx = (base + i) % WREGS_SIZE;
        memcpy(p + i, &env->wregs[idx], sizeof(uint64_t));
    }

    env->psp.index += size;
}

static void ps_pop_nfx(CPUE2KState *env, unsigned int base, size_t len)
{
    unsigned int i;
    size_t size = len * sizeof(uint64_t);
    uint64_t *p;

    if (env->psp.index < size) {
        // TODO: check where to raise exception
        helper_raise_exception(env, E2K_EXCP_MAPERR);
        return;
    }

    env->psp.index -= size;
    p = (uint64_t *) (env->psp.base + env->psp.index);
    for (i = 0; i < len; i++) {
        int idx = (base + i) % WREGS_SIZE;
        memcpy(&env->wregs[idx], p + i, sizeof(uint64_t));
    }
}

static void ps_push_fx(CPUE2KState *env, unsigned int base, size_t len)
{
    unsigned int i;
    size_t size = len * 2 * sizeof(uint64_t);
    uint64_t *p, zeros[2] = { 0 };

    if (env->psp.size < (env->psp.index + size)) {
        helper_raise_exception(env, E2K_EXCP_MAPERR);
        return;
    }

    p = (uint64_t *) (env->psp.base + env->psp.index);
    for (i = 0; i < len; i += 2) {
        int idx = (base + i) % WREGS_SIZE;
        memcpy(p + i * 2, &env->wregs[idx], 2 * sizeof(uint64_t));
        // TODO: save fx part
        memcpy(p + i * 2 + 2, zeros, 2 * sizeof(uint64_t));
    }

    env->psp.index += size;
}

static void ps_pop_fx(CPUE2KState *env, unsigned int base, size_t len)
{
    unsigned int i;
    size_t size = len * 2 * sizeof(uint64_t);
    uint64_t *p;

    if (env->psp.index < size) {
        // TODO: check where to raise exception
        helper_raise_exception(env, E2K_EXCP_MAPERR);
        return;
    }

    env->psp.index -= size;
    p = (uint64_t *) (env->psp.base + env->psp.index);
    for (i = 0; i < len; i += 2) {
        int idx = (base + i) % WREGS_SIZE;
        memcpy(&env->wregs[idx], p + i * 2, sizeof(uint64_t));
        // TODO: restore fx part
    }
}

static inline void do_call(CPUE2KState *env, int call_wbs)
{
    int call_wpsz = env->wd.size / 2 - call_wbs;

    env->ip = env->nip;
    pcs_push(env, call_wbs);
    ps_push_nfx(env, env->wd.base, call_wbs * 2);

    env->wd.base = (env->wd.base + call_wbs * 2) % WREGS_SIZE;
    env->wd.size = env->wd.psize = call_wpsz * 2;

    reset_ctprs(env);
}

void helper_return(CPUE2KState *env)
{
    uint32_t new_wd_size, new_wd_base, wbs;

    wbs = env->cr1.wbs;
    new_wd_size = env->wd.psize + wbs * 2;
    new_wd_base = (env->wd.base - wbs * 2) % WREGS_SIZE;

    if (env->wd.base < new_wd_base) {
        env->wd.base += WREGS_SIZE;
    }

    ps_pop_nfx(env, new_wd_base, env->wd.base - new_wd_base);
    pcs_pop(env);

    env->wd.base = new_wd_base;
    env->wd.size = new_wd_size;

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
    int ctpr_tag = GET_FIELD(ctpr, CTPR_TAG_OFF, CTPR_TAG_LEN);

    switch (ctpr_tag) {
    case CTPR_TAG_DISP:
        do_call(env, call_wbs);
        return GET_FIELD(ctpr, CTPR_BASE_OFF, CTPR_BASE_LEN);
    case CTPR_TAG_SDISP:
        do_syscall(env, call_wbs);
        return env->ip;
    default:
        abort();
        break;
    }

    return 0;
}

void helper_raise_exception(CPUE2KState *env, int tt)
{
    CPUState *cs = env_cpu(env);
    cs->exception_index = tt;
    cpu_loop_exit(cs);
}

static void break_save_state(CPUE2KState *env)
{
    int wbs;

    wbs = env->wd.size / 2;
    ps_push_fx(env, env->wd.base, env->wd.size);
    pcs_push(env, wbs);

    env->wd.base = (env->wd.base + env->wd.size) % WREGS_SIZE;
    env->wd.size = 0;
    env->wd.psize = 0;

    env->is_bp = true;
}

void helper_break_restore_state(CPUE2KState *env)
{
    int wbs = env->cr1.wbs;

    pcs_pop(env);
    env->wd.size = wbs * 2;
    env->wd.base = (env->wd.base - env->wd.size) % WREGS_SIZE;
    ps_pop_fx(env, env->wd.base, env->wd.size);

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
    case 0x2d: /* %usd.lo */
        return env->usd_lo;
    case 0x80: /* %upsr */
        return env->upsr;
    case 0x81: /* %ip */
        return env->ip;
    case 0x83: /* %lsr */
        return env->lsr;
    case 0x8a: /* %idr */
        return env->idr;
    case 0x90: /* %clkr */
        return cpu_get_host_ticks();
    default:
        /* TODO: exception */
        qemu_log_mask(LOG_UNIMP, "unknown register 0x%x\n", reg);
        abort();
        return 0; /* unreachable */
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
    case 0x80: /* %upsr */
        env->upsr = val;
        break;
    case 0x83: /* %lsr */
        env->lsr = val;
        break;
    default:
        qemu_log_mask(LOG_UNIMP, "unknown register 0x%x\n", reg);
        helper_raise_exception(env, E2K_EXCP_ILLOPC);
        break;
    }
}

uint64_t helper_getsp(CPUE2KState *env, uint64_t src2) {
    uint64_t base = GET_FIELD(env->usd_lo, USD_LO_BASE_OFF, USD_LO_BASE_LEN);

    base += src2;

    /* TODO: stack overflow */
    env->usd_lo = SET_FIELD(env->usd_lo, base, USD_LO_BASE_OFF,
        USD_LO_BASE_LEN);

    return base;
}
