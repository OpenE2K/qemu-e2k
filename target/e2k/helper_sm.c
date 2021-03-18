#include "qemu/osdep.h"
#include "qemu/log.h"
#include "cpu.h"
#include "exec/exec-all.h"
#include "qemu/host-utils.h"
#include "exec/helper-proto.h"

static int e2k_probe_access(CPUE2KState *env, target_ulong addr, int size,
    MMUAccessType access_type, int mmu_idx)
{
    target_ulong addr_end = addr + size - 1;
    int flags;
    void *ignore;

    flags = probe_access_flags(env, addr, access_type, mmu_idx,
        true, &ignore, 0);

    if (flags & TLB_INVALID_MASK) {
        return 0;
    } else if ((addr & TARGET_PAGE_MASK) != (addr_end & TARGET_PAGE_MASK)) {
        flags = probe_access_flags(env, addr_end, access_type, mmu_idx, true,
            &ignore, 0);
        return !(flags & TLB_INVALID_MASK);
    }

    return 1;
}

int HELPER(probe_read_access)(CPUE2KState *env, target_ulong addr,
    int size, int mmu_idx)
{
    return e2k_probe_access(env, addr, size, MMU_DATA_LOAD, mmu_idx);
}

int HELPER(probe_write_access)(CPUE2KState *env, target_ulong addr,
    int size, int mmu_idx)
{
    return e2k_probe_access(env, addr, size, MMU_DATA_STORE, mmu_idx);
}
