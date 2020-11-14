#ifndef E2K_CPU_H
#define E2K_CPU_H

#include "qemu/bswap.h"
#include "cpu-qom.h"
#include "exec/cpu-defs.h"

void e2k_tcg_initialize(void);

#define GEN_MASK(start, end) \
    (((1UL << ((end) - (start) + 1)) - 1) << start)
#define GET_BIT(v, index) (((v) >> (index)) & 1)
#define GET_FIELD(v, start, end) \
    (((v) >> (start)) & ((1UL << ((end) - (start) + 1)) - 1))

#define MMU_USER_IDX 1
#define CPU_RESOLVING_TYPE TYPE_E2K_CPU
#define WREGS_SIZE 192
// size of regular reg in bytes
#define REG_SIZE (sizeof(target_ulong))

#define CTPR_BASE_OFF 0
#define CTPR_BASE_END 47
#define CTPR_TAG_OFF 54
#define CTPR_TAG_END 56
#define CTPR_OPC_OFF 57
#define CTPR_OPC_END 58
#define CTPR_IPD_OFF 59
#define CTPR_IPD_END 60

#define BR_RBS_OFF 0        /* based regs window offset */
#define BR_RBS_END 5
#define BR_RBS_LEN (BR_RBS_END - BR_RBS_OFF + 1)
#define BR_RSZ_OFF 6        /* based regs window size */
#define BR_RSZ_END 11
#define BR_RSZ_LEN (BR_RSZ_END - BR_RSZ_OFF + 1)
#define BR_RCUR_OFF 12      /* based regs current index */
#define BR_RCUR_END 17
#define BR_RCUR_LEN (BR_RCUR_END - BR_RCUR_OFF + 1)
#define BR_BN_OFF BR_RBS_OFF
#define BR_BN_END BR_RCUR_END
#define BR_BN_LEN (BR_BN_END - BR_BN_OFF + 1)
#define BR_PSZ_OFF 18       /* based pregs window size */
#define BR_PSZ_END 22
#define BR_PSZ_LEN (BR_PSZ_END - BR_PSZ_OFF + 1)
#define BR_PCUR_OFF 23      /* based pregs current index */
#define BR_PCUR_END 27
#define BR_PCUR_LEN (BR_PCUR_END - BR_PCUR_OFF + 1)
#define BR_BP_OFF BR_PSZ_OFF
#define BR_BP_END BR_PCUR_END
#define BR_BP_LEN (BR_BP_END - BR_BP_OFF + 1)

#define LSR_LCNT_OFF 0      /* loop counter */
#define LSR_LCNT_END 31
#define LSR_LCNT_LEN (LSR_LCNT_END - LSR_LCNT_OFF + 1)
#define LSR_ECNT_OFF 32     /* epilogue counter */
#define LSR_ECNT_END 36
#define LSR_ECNT_LEN (LSR_ECNT_END - LSR_ECNT_OFF + 1)
#define LSR_VLC_OFF 37      /* loop count valid bit */
#define LSR_OVER_OFF 38     /* loop count overflow */
#define LSR_LDMC_OFF 39     /* loads manual control */
#define LSR_LDOVL_OFF 40    /* load overlap */
#define LSR_LDOVL_END 47
#define LSR_LDOVL_SIZE (LSR_LDOVL_END - LSR_LDOVL_OFF + 1)
#define LSR_PCNT_OFF 48     /* prologue counter */
#define LSR_PCNT_END 52
#define LSR_PCNT_LEN (LSR_PCNT_END - LSR_PCNT_OFF + 1)
#define LSR_STRMD_OFF 53    /* store remainder counter */
#define LSR_STRMD_END 59
#define LSR_STRMD_LEN (LSR_STRMD_END - LSR_STRMD_OFF + 1)
#define LSR_SEMC_OFF        /* side effects manual control */

typedef enum {
    E2K_EXCP_UNIMPL = 0x01,
    E2K_EXCP_SYSCALL = 0x02,
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

    uint32_t wbs; // window regs offset
    uint32_t wsz; // window regs size
    uint32_t nfx; // TODO
    uint32_t dbl; // TODO

    uint32_t br; /* based regs and pregs window registers */
    uint64_t lsr; /* loop status register */

    uint32_t syscall_wbs;
    
    uint64_t usd_lo;
    uint64_t usd_hi;

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