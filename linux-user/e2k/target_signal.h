#ifndef TARGET_SIGNAL_H
#define TARGET_SIGNAL_H

/*
 * sigaltstack controls
 */
#define SS_ONSTACK      1
#define SS_DISABLE      2

#define MINSIGSTKSZ     4096
#define SIGSTKSZ        8192

typedef struct sigaltstack {
    abi_ulong ss_sp;
    abi_int   ss_flags;
    abi_ulong ss_size;
} target_stack_t;

#include "../generic/signal.h"

#endif /* TARGET_SIGNAL_H */ 
