#include "qemu/osdep.h"
#include "qemu/log.h"
#include "cpu.h"
#include "exec/exec-all.h"
#include "qemu/host-utils.h"
#include "exec/helper-proto.h"
#include "translate.h"

uint64_t helper_sxt(uint64_t x, uint32_t y)
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

static uint64_t cr_read(CPUE2KState *env, size_t offset)
{
    target_ulong addr = env->pcsp.base + env->pcsp.index + offset;
    return cpu_ldq_le_data(env, addr);
}

uint64_t helper_state_reg_read_i64(CPUE2KState *env, int idx)
{
    switch (idx) {
    case 0x01: return e2k_state_wd(env); /* %wd */
    case 0x0f: return e2k_state_pcsp_lo(env); /* %pcsp.lo */
    case 0x0d: return e2k_state_pcsp_hi(env); /* %pcsp.hi */
    case 0x13: return 0; /* %pcshtp */
    case 0x2c: return env->usd.hi; /* %usd.hi */
    case 0x2d: return env->usd.lo; /* %usd.lo */
    case 0x51: return cr_read(env, offsetof(E2KCrs, cr0_hi)); /* %cr0.hi */
    case 0x53: return cr_read(env, offsetof(E2KCrs, cr0_lo)); /* %cr0.lo */
    case 0x55: return cr_read(env, offsetof(E2KCrs, cr1.hi)); /* %cr1.hi */
    case 0x57: return cr_read(env, offsetof(E2KCrs, cr1.lo)); /* %cr1.lo */
    case 0x80: return env->upsr; /* %upsr */
    case 0x81: return env->ip; /* %ip */
    case 0x83: /* %lsr */
    {
        uint64_t lsr = env->lsr;
        lsr = deposit64(lsr, LSR_LCNT_OFF, LSR_LCNT_LEN, env->lsr_lcnt);
        lsr = deposit64(lsr, LSR_ECNT_OFF, LSR_ECNT_LEN, env->lsr_ecnt);
        lsr = deposit64(lsr, LSR_VLC_OFF, 1, env->lsr_vlc);
        lsr = deposit64(lsr, LSR_OVER_OFF, 1, env->lsr_over);
        lsr = deposit64(lsr, LSR_PCNT_OFF, LSR_PCNT_LEN, env->lsr_pcnt);
        lsr = deposit64(lsr, LSR_STRMD_OFF, LSR_STRMD_LEN, env->lsr_strmd);
        return lsr;
    }
    case 0x84: return env->pfpfr; /* %pfpfr */
    case 0x85: return env->fpcr.raw; /* %fpcr */
    case 0x86: return env->fpsr.raw; /* %fpsr */
    case 0x8a: return env->idr; /* %idr */
    case 0x90: return cpu_get_host_ticks(); /* %clkr */
    default:
        qemu_log_mask(LOG_UNIMP, "read unknown state register 0x%x\n", idx);
        return 0;
    }
}

uint32_t helper_state_reg_read_i32(CPUE2KState *env, int idx)
{
    return helper_state_reg_read_i64(env, idx);
}

void helper_state_reg_write_i64(CPUE2KState *env, int idx, uint64_t val)
{
    switch(idx) {
    case 0x80: /* %upsr */
        env->upsr = val;
        break;
    case 0x83: /* %lsr */
        env->lsr = val;
        env->lsr_lcnt = extract64(val, LSR_LCNT_OFF, LSR_LCNT_LEN);
        env->lsr_ecnt = extract64(val, LSR_ECNT_OFF, LSR_ECNT_LEN);
        env->lsr_vlc = extract64(val, LSR_VLC_OFF, 1);
        env->lsr_over = extract64(val, LSR_OVER_OFF, 1);
        env->lsr_pcnt = extract64(val, LSR_PCNT_OFF, LSR_PCNT_LEN);
        env->lsr_strmd = extract64(val, LSR_STRMD_OFF, LSR_STRMD_LEN);
        break;
    case 0x84: /* %pfpfr */
        env->pfpfr = val;
        break;
    case 0x85: /* %fpcr */
        env->fpcr.raw = val;
        e2k_update_fp_status(env);
        break;
    case 0x86: env->fpsr.raw = val; break; /* %fpsr */
    default:
        qemu_log_mask(LOG_UNIMP, "rwd unknown state register 0x%x\n", idx);
        break;
    }
}

void helper_state_reg_write_i32(CPUE2KState *env, int idx, uint32_t val)
{
    switch (idx) {
    case 0x80: /* %upsr */
        env->upsr = deposit64(env->upsr, 0, 32, val);
        break;
    case 0x83: /* %lsr */
        env->lsr_lcnt = val;
        break;
    case 0x84: /* %pfpfr */
        env->pfpfr = deposit64(env->pfpfr, 0, 32, val);
        break;
    case 0x85: /* %fpcr */
        env->fpcr.raw = val;
        e2k_update_fp_status(env);
        break;
    case 0x86: env->fpsr.raw = val; break; /* %fpsr */
    default:
        qemu_log_mask(LOG_UNIMP, "rws unknown state register 0x%x\n", idx);
        break;
    }
}

uint64_t helper_getsp(CPUE2KState *env, uint32_t src2)
{
    int32_t s2 = src2 & ~0xf;

    env->usd.base += s2;
    env->usd.size -= s2;

    return env->usd.base;
}

void HELPER(dam_lock_addr)(CPUE2KState *env, uint64_t addr, int size, int reg)
{
    // TODO: helper_dam_lock_addr
}

int HELPER(dam_unlock_addr)(CPUE2KState *env, uint64_t addr, int size, int reg)
{
    // TODO: helper_dam_unlock_addr
    return 1;
}
