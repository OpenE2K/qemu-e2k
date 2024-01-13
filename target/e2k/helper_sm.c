#include "qemu/osdep.h"
#include "qemu/log.h"
#include "cpu.h"
#include "exec/exec-all.h"
#include "qemu/host-utils.h"
#include "exec/helper-proto.h"

#if defined(CONFIG_USER_ONLY)
#if PAGE_READ > 0xff || PAGE_WRITE_ORG > 0xff
# error
#endif

static bool e2k_probe_access_cached(CPUE2KState *env, target_ulong page, int flags)
{
    int index = (page >> TARGET_PAGE_BITS) & (TARGET_PROBE_CACHE_SIZE - 1);

    if (env->probe_cache_page[index] == page) {
        return (env->probe_cache_flags[index] & flags) == flags;
    }

    env->probe_cache_page[index] = page;
    env->probe_cache_flags[index] = page_get_flags(page);

    return (env->probe_cache_flags[index] & flags) == flags;
}

static bool e2k_probe_access(CPUE2KState *env, target_ulong addr, int size, int flags)
{
    target_ulong start = addr & TARGET_PAGE_MASK;
    target_ulong last = (addr + size - 1) & TARGET_PAGE_MASK;

    if (start == last) {
        return guest_addr_valid_untagged(addr) &&
            e2k_probe_access_cached(env, start, flags);
    } else {
        return guest_range_valid_untagged(addr, size) &&
            e2k_probe_access_cached(env, start, flags) &&
            e2k_probe_access_cached(env, last, flags);
    }
}

int HELPER(probe_read_access)(CPUE2KState *env, target_ulong addr, int size)
{
    return e2k_probe_access(env, addr, size, PAGE_READ);
}

int HELPER(probe_write_access)(CPUE2KState *env, target_ulong addr, int size)
{
    return e2k_probe_access(env, addr, size, PAGE_WRITE_ORG);
}

int HELPER(probe_rw_access)(CPUE2KState *env, target_ulong addr, int size)
{
    return e2k_probe_access(env, addr, size, PAGE_READ | PAGE_WRITE_ORG);
}
#endif /* CONFIG_USER_ONLY */
