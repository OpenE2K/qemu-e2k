#ifndef E2K_CPU_H
#define E2K_CPU_H

#include "qemu/bswap.h"
#include "cpu-qom.h"
#include "exec/cpu-defs.h"

void e2k_tcg_initialize(void);

#define MMU_USER_IDX 1
#define CPU_RESOLVING_TYPE TYPE_E2K_CPU

struct e2k_def_t {
	const char *name;
	uint32_t isa_version;
};

typedef struct {
    
    /* register file */ 
    CPU_QuadU gregs[32]; /* general registers */
    CPU_QuadU pregs[224]; /* procedure regs, access through wreg */
    uint8_t greg_tags[32];
    uint8_t preg_tags[224];
    
    /* control registers */
    target_ulong ctpr1; // Control Transfer Preparation Register (CTPR)
    target_ulong ctpr2;
    target_ulong ctpr3;
    
    /* special registers */
    target_ulong *wreg; /* pointer to current window */
    uint32_t br;    /* base rsegister offset, max 128 */
    target_ulong ip, nip; /* instruction address, next instruction address */
    
    uint32_t pfpfr; // Packed Floating Point Flag Register (PFPFR)
    uint32_t fpcr; // Floating point control register (FPCR)
    uint32_t fpsr; // Floating point state register (FPSR)

    int interrupt_index;

    /* Fields up to this point are cleared by a CPU reset */
    struct {} end_reset_fields;

    uint32_t version;

    struct e2k_def_t def;
} CPUE2KState;

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

static inline void cpu_get_tb_cpu_state(CPUE2KState *env, target_ulong *pc,
                                        target_ulong *cs_base, uint32_t *pflags)
{
    *pc = env->ip;
    *cs_base = env->nip;
    *pflags = MMU_USER_IDX;
}

int e2k_cpu_signal_handler(int host_signum, void *pinfo, void *puc);

#define cpu_signal_handler e2k_cpu_signal_handler

typedef CPUE2KState CPUArchState;
typedef E2KCPU ArchCPU;

#include "exec/cpu-all.h"

#endif
