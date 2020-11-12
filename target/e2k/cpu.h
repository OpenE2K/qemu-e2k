#ifndef E2K_CPU_H
#define E2K_CPU_H

#include "qemu/bswap.h"
#include "cpu-qom.h"
#include "exec/cpu-defs.h"

void e2k_tcg_initialize(void);

#define MMU_USER_IDX 1
#define CPU_RESOLVING_TYPE TYPE_E2K_CPU
#define WREGS_SIZE 192
// size of regular reg in bytes
#define REG_SIZE (sizeof(target_ulong))

typedef enum {
    E2K_EXCP_SYSCALL = 0x01,
} Exception;

struct e2k_def_t {
	const char *name;
	uint32_t isa_version;
};

typedef struct {
    
    /* register file */
    target_ulong gregs[32]; // global regs
    target_ulong wregs[WREGS_SIZE]; // window regs
    target_ulong *win_ptr;
    uint64_t pregs;
    uint32_t wbs; // window regs offset (* 2)
    uint32_t wsz; // window regs size (* 2)
    uint32_t nfx; // TODO
    uint32_t dbl; // TODO
    uint32_t rbs; // based regs offset (* 2)
    uint32_t rsz; // based regs window size (* 2 + 2)
    uint32_t rcur; // based regs current index (* 2)
    uint32_t psz; // pred regs window size
    
    /* control registers */
    target_ulong ctprs[3]; // Control Transfer Preparation Register (CTPR)
    
    /* special registers */
    target_ulong ip; /* instruction address, next instruction address */
    
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
    *cs_base = 0;
    *pflags = MMU_USER_IDX;
}

int e2k_cpu_signal_handler(int host_signum, void *pinfo, void *puc);

#define cpu_signal_handler e2k_cpu_signal_handler

typedef CPUE2KState CPUArchState;
typedef E2KCPU ArchCPU;

#include "exec/cpu-all.h"

#endif
