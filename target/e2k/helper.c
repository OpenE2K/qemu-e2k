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
