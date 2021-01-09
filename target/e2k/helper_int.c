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

static uint64_t* state_reg_ptr(CPUE2KState *env, int idx)
{
    switch (idx) {
    case 0x80: return &env->upsr; /* %upsr */
    case 0x83: return &env->lsr; /* %lsr */
    default: return NULL;
    }
}

uint64_t helper_state_reg_read_i64(CPUE2KState *env, int idx)
{
    switch (idx) {
    case 0x01: return e2k_state_wd(env); /* %wd */
    case 0x0f: return e2k_state_pcsp_lo(env); /* %pcsp.lo */
    case 0x0d: return e2k_state_pcsp_hi(env); /* %pcsp.hi */
    case 0x13: return env->pcshtp; /* %pcshtp */
    case 0x2c: return env->usd.hi; /* %usd.hi */
    case 0x2d: return env->usd.lo; /* %usd.lo */
    case 0x51: return env->crs.cr0_hi; /* %cr0.hi */
    case 0x53: return env->crs.cr0_lo; /* %cr0.lo */
    case 0x55: return env->crs.cr1.hi; /* %cr1.hi */
    case 0x57: return env->crs.cr1.lo; /* %cr1.lo */
    case 0x81: return env->ip; /* %ip */
    case 0x85: return env->fpcr.raw; /* %fpcr */
    case 0x86: return env->fpsr.raw; /* %fpsr */
    case 0x8a: return env->idr; /* %idr */
    case 0x90: return cpu_get_host_ticks(); /* %clkr */
    default: {
        uint64_t *p = state_reg_ptr(env, idx);

        if (p != NULL) {
            return *p;
        } else {
            qemu_log_mask(LOG_UNIMP, "read unknown state register 0x%x\n", idx);
            return 0;
        }
    }
    }
}

uint32_t helper_state_reg_read_i32(CPUE2KState *env, int idx)
{
    return helper_state_reg_read_i64(env, idx);
}

void helper_state_reg_write_i64(CPUE2KState *env, int idx, uint64_t val)
{
    switch(idx) {
    case 0x85: /* %fpcr */
        env->fpcr.raw = val;
        e2k_update_fp_status(env);
        break;
    case 0x86: env->fpsr.raw = val; break; /* %fpsr */
    default: {
        uint64_t *p = state_reg_ptr(env, idx);

        if (p != NULL) {
            *p = val;
        } else {
            qemu_log_mask(LOG_UNIMP, "unknown state register 0x%x\n", idx);
            abort();
        }
        break;
    }
    }
}

void helper_state_reg_write_i32(CPUE2KState *env, int idx, uint32_t val)
{
    switch (idx) {
    case 0x85: /* %fpcr */
        env->fpcr.raw = val;
        e2k_update_fp_status(env);
        break;
    case 0x86: env->fpsr.raw = val; break; /* %fpsr */
    default: {
        uint32_t *p = (uint32_t*) state_reg_ptr(env, idx);
        
        if (p != NULL) {
            *p = val;
        } else {
            qemu_log_mask(LOG_UNIMP, "unknown state register 0x%x\n", idx);
            abort();
        }
        break;
    }
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
