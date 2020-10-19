#ifndef E2K_CPU_H
#define E2K_CPU_H

#include "qemu/bswap.h"
#include "cpu-qom.h"
#include "exec/cpu-defs.h"

typedef struct {
    
    /* register file */ 
    uint64_t gregs[32]; /* general registers */
    uint64_t pregs[224]; /* procedure regs, access through wreg */
    
    /* control registers */
    uint64_t ctpr1; // Control Transfer Preparation Register (CTPR)
    uint64_t ctpr2;
    uint64_t ctpr3;
    
    /* special registers */
    uint64_t *wreg; /* pointer to current window */
    uint32_t br;    /* base rsegister offset, max 128 */
    uint64_t ip, nip; /* instruction address, next instruction address */
    
    uint32_t pfpfr; // Packed Floating Point Flag Register (PFPFR)
    uint32_t fpcr; // Floating point control register (FPCR)
    uint32_t fpsr; // Floating point state register (FPSR)
} CPUE2KState;

typedef CPUE2KState CPUArchState;

/**
 * E2KCPU:
 * @env: #CPUE2KState
 *
 * An Elbrus CPU.
 */
struct E2KCPU {
    /*< private >*/
    CPUState parent_obj;
    /*< public >*/

    CPUNegativeOffsetState neg;
    CPUE2KState env;
};

#endif
