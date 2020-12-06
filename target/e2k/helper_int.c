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
    /* FIXME: user cannot write */
    case 0x2c: return &env->usd_hi; /* %usd.hi */
    /* FIXME: user cannot write */
    case 0x2d: return &env->usd_lo; /* %usd.lo */
    case 0x51: return &env->ip; /* %cr1.hi */
    case 0x53: return &env->pregs; /* %cr1.lo */
    case 0x80: return &env->upsr; /* %upsr */
    case 0x83: return &env->lsr; /* %lsr */
    default: return NULL;
    }
}

uint64_t helper_state_reg_read_i64(CPUE2KState *env, int idx)
{
    switch (idx) {
    case 0x01:
        return e2k_state_wd(env); /* %wd */
    case 0x0f:
        return e2k_state_pcsp_lo(env); /* %pcsp.lo */
    case 0x0d:
        return e2k_state_pcsp_hi(env); /* %pcsp.hi */
    case 0x13:
        return env->pcshtp; /* %pcshtp */
    case 0x55:
        return e2k_state_cr1_hi(env); /* %cr1.hi */
    case 0x57:
        return e2k_state_cr1_lo(env); /* %cr1.lo */
    case 0x81: /* %ip */
        return env->ip;
    case 0x8a: /* %idr */
        return env->idr;
    case 0x90: /* %clkr */
        return cpu_get_host_ticks();
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
    uint64_t *p = state_reg_ptr(env, idx);

    if (p != NULL) {
        *p = val;
    } else {
        qemu_log_mask(LOG_UNIMP, "unknown state register 0x%x\n", idx);
        abort();
    }
}

void helper_state_reg_write_i32(CPUE2KState *env, int idx, uint32_t val)
{
    uint32_t *p = (uint32_t*) state_reg_ptr(env, idx);

    if (p != NULL) {
        *p = val;
    } else {
        qemu_log_mask(LOG_UNIMP, "unknown state register 0x%x\n", idx);
        abort();
    }
}

uint64_t helper_getsp(CPUE2KState *env, uint32_t src2) {
    uint64_t base = GET_FIELD(env->usd_lo, USD_LO_BASE_OFF, USD_LO_BASE_LEN);

    base += (int32_t) src2;

    /* TODO: stack overflow */
    env->usd_lo = SET_FIELD(env->usd_lo, base, USD_LO_BASE_OFF,
        USD_LO_BASE_LEN);

    return base;
}
