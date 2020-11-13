#include "qemu/osdep.h"
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

void helper_call(CPUE2KState *env, uint64_t ctpr, uint64_t cond)
{
    int tag = GET_FIELD(ctpr, CTPR_TAG_OFF, CTPR_TAG_END);
    if (!cond) {
        return;
    }
    if (tag == CTPR_TAG_SDISP) {
        CPUState *cs = env_cpu(env);

        cs->exception_index = E2K_EXCP_SYSCALL;
        cpu_loop_exit(cs);
    } else {
        /* TODO: call */
        abort();
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
