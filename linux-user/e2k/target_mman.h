#ifndef LINUX_USER_E2K_TARGET_MMAN_H
#define LINUX_USER_E2K_TARGET_MMAN_H

#ifdef TARGET_E2K64
# define TARGET_TASK_SIZE   0xd00000000000UL
#else
# define TARGET_TASK_SIZE   0xf0000000UL
#endif

#define TASK_UNMAPPED_BASE  TARGET_PAGE_ALIGN(TARGET_TASK_SIZE / 3)
#define ELF_ET_DYN_BASE     (2 * TARGET_TASK_SIZE / 3)

#define TARGET_MAP_ANONYMOUS           0x000010 /* don't use a file */
#define TARGET_MAP_FIXED               0x000100 /* Interpret addr exactly */
#define TARGET_MAP_DENYWRITE           0x000800 /* ETXTBSY */
#define TARGET_MAP_GROWSDOWN           0x001000 /* stack-like segment */
#define TARGET_MAP_GROWSUP             0x002000 /* register stack-like segment */
#define TARGET_MAP_EXECUTABLE          0x004000 /* mark it as an executable */
#define TARGET_MAP_LOCKED              0x008000 /* pages are locked */
#define TARGET_MAP_NORESERVE           0x010000 /* don't check for reservations */
#define TARGET_MAP_POPULATE            0x020000 /* populate (prefault) pagetables */
#define TARGET_MAP_NONBLOCK            0x040000 /* do not block on IO */
#define TARGET_MAP_FIRST32             0x080000 /* in protected mode map in  */
/* first 2 ** 32 area */
#define TARGET_MAP_WRITECOMBINED       0x100000 /* Write combine */
#define TARGET_MAP_HUGETLB             0x200000 /* create a huge page mapping */
#define TARGET_MAP_FIXED_NOREPLACE     0x400000 /* MAP_FIXED which doesn't unmap */
/* underlying mapping */
#define TARGET_MAP_STACK TARGET_MAP_GROWSDOWN

#include "../generic/target_mman.h"

#endif /* LINUX_USER_E2K_TARGET_MMAN_H */
