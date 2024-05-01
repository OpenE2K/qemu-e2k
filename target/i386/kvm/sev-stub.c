#include "qemu/osdep.h"
#include "system/kvm.h"
#include "../sev.h"

uint32_t sev_get_cbit_position(void) {
    g_assert_not_reached();
}

uint32_t sev_get_reduced_phys_bits(void) {
    g_assert_not_reached();
}

#if !defined(CONFIG_USER_ONLY)
bool sev_add_kernel_loader_hashes(SevKernelLoaderContext *ctx, Error **errp)
{
    g_assert_not_reached();
}
#endif
