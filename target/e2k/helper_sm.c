#include "qemu/osdep.h"
#include "qemu/log.h"
#include "cpu.h"
#include "exec/exec-all.h"
#include "qemu/host-utils.h"
#include "exec/helper-proto.h"
#include "translate.h"

int HELPER(probe_read_access)(CPUE2KState *env, target_ulong addr)
{
    int flags;
    void *ignore;

    flags = probe_access_flags(env, addr, MMU_DATA_LOAD, 0, true, &ignore, 0);

    return (flags & TLB_INVALID_MASK) == 0;
}

int HELPER(probe_write_access)(CPUE2KState *env, target_ulong addr)
{
    int flags;
    void *ignore;

    flags = probe_access_flags(env, addr, MMU_DATA_STORE, 0, true, &ignore, 0);

    return (flags & TLB_INVALID_MASK) == 0;
}
