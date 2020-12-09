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
        env->ctprs[i].tag = CTPR_TAG_NONE;
    }
}

static inline void save_proc_chain_info(CPUE2KState *env, uint64_t buf[4],
    int wbs)
{
    env->cr1.br = e2k_state_br(env);
    env->cr1.wpsz = env->wd.psize / 2;

    buf[0] = env->cr0_lo;
    buf[1] = env->cr0_hi;
    buf[2] = e2k_state_cr1_lo(env);
    buf[3] = e2k_state_cr1_hi(env);

    env->cr1.wfx = env->wd.fx;
    env->cr1.wbs = wbs;
    env->wd.base = (env->wd.base + wbs * 2) % E2K_NR_COUNT;
    env->wd.psize = env->wd.size -= wbs * 2;
}

static inline void restore_proc_chain_info(CPUE2KState *env, uint64_t buf[4])
{
    env->wd.fx = env->cr1.wfx;

    e2k_state_cr1_hi_set(env, buf[3]);
    e2k_state_cr1_lo_set(env, buf[2]);
    env->cr0_hi = buf[1]; // FIXME: is it necessary to restore ip?
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

    save_proc_chain_info(env, buf, wbs);
    memcpy(env->pcsp.base + env->pcsp.index, buf, 32);
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
        int idx = (base + i) % E2K_NR_COUNT;
        memcpy(p + i, &env->regs[idx], sizeof(uint64_t));
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
        int idx = (base + i) % E2K_NR_COUNT;
        memcpy(&env->regs[idx], p + i, sizeof(uint64_t));
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
        int idx = (base + i) % E2K_NR_COUNT;
        memcpy(p + i * 2, &env->regs[idx], 2 * sizeof(uint64_t));
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
        int idx = (base + i) % E2K_NR_COUNT;
        memcpy(&env->regs[idx], p + i * 2, sizeof(uint64_t));
        // TODO: restore fx part
    }
}

static inline void ps_push(CPUE2KState *env)
{
    int occupied = env->wd.size + env->pshtp.index;
    if (E2K_NR_COUNT < occupied) {
        int len = occupied - E2K_NR_COUNT;
        int base = (int) env->wd.base - env->pshtp.index;
        if (base < 0) {
            base += E2K_NR_COUNT;
        }
        // TODO: ps_push_fx
        ps_push_nfx(env, base, len);
        env->pshtp.index -= len;
    }
}

static inline void ps_pop(CPUE2KState *env, int base, int required)
{
    if (env->pshtp.index < required) {
        int len = required - env->pshtp.index;
        // TODO: ps_pop_fx
        ps_pop_nfx(env, base, len);
        env->pshtp.index = 0;
    } else {
        env->pshtp.index -= required;
    }
}

static inline void do_call(CPUE2KState *env, int wbs, target_ulong pc_next)
{
    env->ip = pc_next;
    env->pshtp.index += wbs * 2; // add old window to allocated registers
    pcs_push(env, wbs);
    reset_ctprs(env);
}

uint64_t helper_prep_return(CPUE2KState *env, int ipd)
{
    uint64_t pc;
    E2KCtpr ret = { 0 };
    void *p;

    if (env->pcsp.index < 32) {
        helper_raise_exception(env, E2K_EXCP_MAPERR);
        return 0;
    }

    p = (void *) env->pcsp.base + env->pcsp.index - 24;
    memcpy(&pc, p, 8);

    ret.base = pc;
    ret.tag = CTPR_TAG_RETURN;
    ret.ipd = ipd;

    return ret.raw;
}

void helper_return(CPUE2KState *env)
{
    uint32_t new_wd_size, new_wd_base, offset;

    offset = env->cr1.wbs * 2;
    new_wd_size = env->wd.psize + offset;
    new_wd_base = env->wd.base;
    if (new_wd_base < offset) {
        new_wd_base += E2K_NR_COUNT;
    }
    new_wd_base = (new_wd_base - offset) % E2K_NR_COUNT;

    ps_pop(env, new_wd_base, offset);
    pcs_pop(env);
    env->wd.base = new_wd_base;
    env->wd.size = new_wd_size;

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
    int wbs;

    wbs = env->wd.size / 2;
    ps_push_fx(env, env->wd.base, env->wd.size);
    pcs_push(env, wbs);

    env->wd.base = (env->wd.base + env->wd.size) % E2K_NR_COUNT;
    env->wd.size = 0;
    env->wd.psize = 0;

    env->is_bp = true;
}

void helper_break_restore_state(CPUE2KState *env)
{
    int wbs = env->cr1.wbs;

    pcs_pop(env);
    env->wd.size = wbs * 2;
    env->wd.base = (env->wd.base - env->wd.size) % E2K_NR_COUNT;
    ps_pop_fx(env, env->wd.base, env->wd.size);

    env->is_bp = false;
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

void helper_setwd(CPUE2KState *env, uint32_t lts)
{
    env->wd.size = extract32(lts, 5, 7) * 2;
    env->wd.fx = extract32(lts, 4, 1) == 0;

    if (env->wd.fx) {
        qemu_log_mask(LOG_UNIMP, "0x%lx: wd.fx is not implemented!\n",
            env->ip);
    }

    if (env->version >= 3) {
        bool dbl = extract32(lts, 3, 1);
        // TODO: set dbl
        if (dbl != false) {
            qemu_log_mask(LOG_UNIMP, "0x%lx: dbl is not implemented!\n",
                env->ip);
        }
        env->cr1.wdbl = dbl;
    }

    ps_push(env);
}
