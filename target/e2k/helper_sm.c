#include "qemu/osdep.h"
#include "qemu/log.h"
#include "cpu.h"
#include "exec/exec-all.h"
#include "qemu/host-utils.h"
#include "exec/helper-proto.h"

static bool e2k_probe_access(target_ulong addr, int size, int flags)
{
    target_ulong start = addr & TARGET_PAGE_MASK;
    target_ulong last = (addr + size - 1) & TARGET_PAGE_MASK;

    if (start == last) {
        return guest_addr_valid_untagged(addr) &&
            (page_get_flags(start) & flags) == flags;
    } else {
        return guest_range_valid_untagged(addr, size) &&
            page_check_range(addr, size, flags);
    }
}

int HELPER(probe_read_access)(target_ulong addr, int size)
{
    return e2k_probe_access(addr, size, PAGE_READ);
}

int HELPER(probe_write_access)(target_ulong addr, int size)
{
    return e2k_probe_access(addr, size, PAGE_WRITE_ORG);
}

int HELPER(probe_rw_access)(target_ulong addr, int size)
{
    return e2k_probe_access(addr, size, PAGE_READ | PAGE_WRITE_ORG);
}
