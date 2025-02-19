/*
 * E2K cpu parameters for qemu.
 *
 * SPDX-License-Identifier: GPL-2.0-only
 */

#ifndef TARGET_E2K_CPU_PARAM_H
#define TARGET_E2K_CPU_PARAM_H

#ifdef TARGET_E2K32
# define TARGET_VIRT_ADDR_SPACE_BITS 32
#else
# define TARGET_VIRT_ADDR_SPACE_BITS 48
#endif
#define TARGET_PAGE_BITS 12 /* 4k */
#define TARGET_PHYS_ADDR_SPACE_BITS 40

#endif /* TARGET_E2K_CPU_PARAM_H */
