#ifndef TARGET_SIGNAL_H
#define TARGET_SIGNAL_H

/*
 * sigaltstack controls
 */
#define TARGET_SS_ONSTACK      1
#define TARGET_SS_DISABLE      2

#define TARGET_MINSIGSTKSZ     4096
#define TARGET_SIGSTKSZ        8192
#define TARGET_MCL_CURRENT 1
#define TARGET_MCL_FUTURE 2
// TODO: check
#define TARGET_MCL_ONFAULT 4

#define TARGET_ARCH_HAS_SETUP_FRAME

typedef struct target_sigaltstack {
    abi_ulong ss_sp;
    abi_int   ss_flags;
    abi_ulong ss_size;
} target_stack_t;

#include "../generic/signal.h"

#endif /* TARGET_SIGNAL_H */ 
