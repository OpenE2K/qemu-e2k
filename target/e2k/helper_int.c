#include "qemu/osdep.h"
#include "qemu/log.h"
#include "cpu.h"
#include "exec/exec-all.h"
#include "qemu/host-utils.h"
#include "exec/helper-proto.h"
#include "translate.h"

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
        abort();
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
