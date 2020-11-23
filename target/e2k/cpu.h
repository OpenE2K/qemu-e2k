#ifndef E2K_CPU_H
#define E2K_CPU_H

#include "qemu/bswap.h"
#include "cpu-qom.h"
#include "exec/cpu-defs.h"

void e2k_tcg_initialize(void);

#define GEN_MASK(start, end) \
    GEN_MASK_LEN((start), (end) - (start) + 1)
#define GEN_MASK_LEN(start, len) \
    (((1UL << (len)) - 1) << (start))
#define GET_BIT(v, index) (((v) >> (index)) & 1)
#define GET_FIELD(v, start, end) \
    (((v) >> (start)) & ((1UL << ((end) - (start) + 1)) - 1))
#define GET_FIELD_LEN(v, s, l) \
	(((v) >> (s)) & GEN_MASK_LEN(0, l))
#define SET_FIELD(v, f, s, l) \
    ( \
        ((v) & ~GEN_MASK_LEN((s), (l))) | \
        ((((typeof((v))) (f)) << (s)) & GEN_MASK_LEN((s), (l))) \
    )

#define MMU_USER_IDX 1
#define CPU_RESOLVING_TYPE TYPE_E2K_CPU

#define REG_SIZE (sizeof(uint64_t))
#define WREGS_SIZE 192
#define GREGS_SIZE 32
#define WREGS_MAX 64
#define BREGS_MAX 128
#define GREGS_MAX 24
#define BGREGS_MAX 8
#define PF_SIZE 32

#define CTPR_BASE_OFF 0
#define CTPR_BASE_END 47
#define CTPR_BASE_LEN (CTPR_BASE_END - CTPR_BASE_OFF + 1)
#define CTPR_TAG_OFF 54
#define CTPR_TAG_END 56
#define CTPR_TAG_LEN (CTPR_TAG_END - CTPR_TAG_OFF + 1)
#define CTPR_OPC_OFF 57
#define CTPR_OPC_END 58
#define CTPR_OPC_LEN (CTPR_OPC_END - CTPR_OPC_OFF + 1)
#define CTPR_IPD_OFF 59
#define CTPR_IPD_END 60
#define CTPR_IPD_LEN (CTPR_IPD_END - CTPR_IPD_OFF + 1)

#define WD_BASE_OFF 0
#define WD_BASE_END 10
#define WD_BASE_LEN (WD_BASE_END - WD_BASE_OFF + 1)
#define WD_SIZE_OFF 16
#define WD_SIZE_END 26
#define WD_SIZE_LEN (WD_SIZE_END - WD_SIZE_OFF + 1)
#define WD_PSIZE_OFF 32
#define WD_PSIZE_END 42
#define WD_PSIZE_LEN (WD_PSIZE_END - WD_PSIZE_OFF + 1)
#define WD_FX_OFF 48
#define WD_FX_BIT (1UL << WD_FX_OFF)

#define PCSP_HI_IND_OFF 0       /* index for SPILL */
#define PCSP_HI_IND_END 31
#define PCSP_HI_IND_LEN (PCSP_HI_IND_END - PCSP_HI_IND_OFF + 1)
#define PCSP_HI_SIZE_OFF 32
#define PCSP_HI_SIZE_END 63     /* procedure stack chain size */
#define PCSP_HI_SIZE_LEN (PCSP_HI_SIZE_END - PCSP_HI_SIZE_OFF + 1)

#define PCSP_LO_BASE_OFF 0      /* procedure stack chain address */
#define PCSP_LO_BASE_END 47
#define PCSP_LO_BASE_LEN (PCSP_LO_BASE_END - PCSP_LO_BASE_OFF + 1)
#define PCSP_LO_READ_OFF 59
#define PCSP_LO_READ_BIT (1UL << PCSP_LO_READ_OFF)
#define PCSP_LO_WRITE_OFF 60
#define PCSP_LO_WRITE_BIT (1UL << PCSP_LO_WRITE_OFF)

#define PSP_HI_IND_OFF 0       /* index for SPILL */
#define PSP_HI_IND_END 31
#define PSP_HI_IND_LEN (PSP_HI_IND_END - PSP_HI_IND_OFF + 1)
#define PSP_HI_SIZE_OFF 32
#define PSP_HI_SIZE_END 63     /* procedure stack size */
#define PSP_HI_SIZE_LEN (PSP_HI_SIZE_END - PSP_HI_SIZE_OFF + 1)

#define PSP_LO_BASE_OFF 0      /* procedure stack address */
#define PSP_LO_BASE_END 47
#define PSP_LO_BASE_LEN (PSP_LO_BASE_END - PSP_LO_BASE_OFF + 1)
#define PSP_LO_READ_OFF 59
#define PSP_LO_READ_BIT (1UL << PSP_LO_READ_OFF)
#define PSP_LO_WRITE_OFF 60
#define PSP_LO_WRITE_BIT (1UL << PSP_LO_WRITE_OFF)

#define PSHTP_IND_OFF 0
#define PSHTP_IND_END 11
#define PSHTP_IND_LEN (PSHTP_IND_END - PSHTP_IND_OFF + 1)
#define PSHTP_FXIND_OFF 16
#define PSHTP_FXIND_END 26
#define PSHTP_FXIND_LEN (PSHTP_FXIND_END - PSHTP_FXIND_OFF + 1)
#define PSHTP_TIND_OFF 32
#define PSHTP_TIND_END 42
#define PSHTP_TIND_LEN (PSHTP_TIND_END - PSHTP_TIND_OFF + 1)
#define PSHTP_FX_OFF 48
#define PSHTP_FX_BIT (1UL << PSHTP_FX_OFF)

#define USD_LO_BASE_OFF 0
#define USD_LO_BASE_END 47
#define USD_LO_BASE_LEN (USD_LO_BASE_END - USD_LO_BASE_OFF + 1)
#define USD_LO_PROTECTED_OFF 58
#define USD_LO_PROTECTED_BIT (1UL << USD_LO_PROTECTED_OFF)
#define USD_LO_READ_OFF 59
#define USD_LO_READ_BIT (1UL << USD_LO_READ_OFF)
#define USD_LO_WRITE_OFF 60
#define USD_LO_WRITE_BIT (1UL << USD_LO_WRITE_OFF)

#define USD_HI_CURPTR_OFF 0
#define USD_HI_CURPTR_END 31
#define USD_HI_CURPTR_LEN (USD_HI_CURPTR_END - USD_HI_CURPTR_OFF + 1)
#define USD_HI_SIZE_OFF 32
#define USD_HI_SIZE_END 63
#define USD_HI_SIZE_LEN (USD_HI_SIZE_END - USD_HI_SIZE_OFF + 1)

#define CR1_HI_BR_OFF 0
#define CR1_HI_BR_END 27
#define CR1_HI_BR_LEN (CR1_HI_BR_END - CR1_HI_BR_OFF + 1)
#define CR1_HI_WDBL_OFF 35
#define CR1_HI_WDBL_BIT (1UL << CR1_HI_WDBL_OFF)
#define CR1_HI_USSZ_OFF 36
#define CR1_HI_USSZ_END 63
#define CR1_HI_USSZ_LEN (CR1_HI_USSZ_END - CR1_HI_USSZ_OFF + 1)

#define CR1_LO_TR_OFF 0
#define CR1_LO_TR_END 14
#define CR1_LO_TR_LEN (CR1_LO_TR_END - CR1_LO_TR_OFF + 1)
#define CR1_LO_EIN_OFF 16
#define CR1_LO_EIN_END 23
#define CR1_LO_EIN_LEN (CR1_LO_EIN_END - CR1_LO_EIN_OFF + 1)
#define CR1_LO_SS_OFF 24
#define CR1_LO_SS_BIT (1UL << CR1_LO_SS_OFF)
#define CR1_LO_WFX_OFF 25
#define CR1_LO_WFX_BIT (1UL << CR1_LO_WFX_OFF)
#define CR1_LO_WPSZ_OFF 26
#define CR1_LO_WPSZ_END 32
#define CR1_LO_WPSZ_LEN (CR1_LO_WPSZ_END - CR1_LO_WPSZ_OFF + 1)
#define CR1_LO_WBS_OFF 33
#define CR1_LO_WBS_END 39
#define CR1_LO_WBS_LEN (CR1_LO_WBS_END - CR1_LO_WBS_OFF + 1)
#define CR1_LO_CUIR_OFF 40
#define CR1_LO_CUIR_END 56
#define CR1_LO_CUIR_LEN (CR1_LO_CUIR_END - CR1_LO_CUIR_OFF + 1)
#define CR1_LO_PSR_OFF 57
#define CR1_LO_PSR_END 63
#define CR1_LO_PSR_LEN (CR1_LO_PSR_END - CR1_LO_PSR_OFF + 1)

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
    E2K_EXCP_ILLOPC = 0x03,
    E2K_EXCP_ILLOPN = 0x04,
    E2K_EXCP_MAPERR = 0x05,
} Exception;

struct e2k_def_t {
	const char *name;
	uint32_t isa_version;
};

typedef struct {
    /* register file */
    uint64_t gregs[GREGS_SIZE]; /* global registers */
    uint64_t wregs[WREGS_SIZE]; /* window registers */
    uint64_t *wptr;

    /* Procedure chain info = cr0_lo, cr0_hi, cr1_lo, cr1_hi */
    uint64_t pcsp_lo;
    uint64_t pcsp_hi;
    uint64_t pcshtp;
    uint32_t br;

    /* Procedure stack pointer (for regs)  */
    uint64_t psp_lo;
    uint64_t psp_hi;
    uint64_t pshtp;

    uint64_t lsr; /* loop status register */

    uint32_t wd_base;
    uint32_t wd_size;
    uint32_t wd_psize;
    uint32_t boff; /* holds rbs * 2 */
    uint32_t bsize; /* holds rsz * 2 + 2 */
    uint32_t bcur; /* holds rcur * 2 */
    uint32_t psize; /* holds psz */
    uint32_t pcur; /* holds pcur */

    uint64_t usd_lo;
    uint64_t usd_hi;

    /* control registers */
    target_ulong ctprs[3]; // Control Transfer Preparation Register (CTPR)
    target_ulong ct_cond;
    
    union {
        struct {
            union {
                uint64_t pf; /* predicate file */
                uint64_t cr0_lo;
            };
            union {
                target_ulong ip; /* instruction address */
                uint64_t cr0_hi;
            };
            uint64_t cr1_lo;
            uint64_t cr1_hi;
        };
        uint64_t proc_chain[4];
    };

    target_ulong nip; /* next instruction address */
    
    uint32_t pfpfr; // Packed Floating Point Flag Register (PFPFR)
    uint32_t fpcr; // Floating point control register (FPCR)
    uint32_t fpsr; // Floating point state register (FPSR)

    int interrupt_index;
    uint32_t is_bp; /* breakpoint flag */
    int syscall_wbs; // FIXME: temp for syscall

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

void e2k_cpu_do_interrupt(CPUState *cs);
int e2k_cpu_signal_handler(int host_signum, void *pinfo, void *puc);
int e2k_cpu_gdb_read_register(CPUState *cs, GByteArray *mem_buf, int n);
int e2k_cpu_gdb_write_register(CPUState *cs, uint8_t *mem_buf, int n);

#define cpu_signal_handler e2k_cpu_signal_handler

static inline target_ulong e2k_state_pcs_base_get(CPUE2KState *env)
{
    return GET_FIELD(env->pcsp_lo, PCSP_LO_BASE_OFF, PCSP_LO_BASE_END);
}

static inline void e2k_state_pcs_base_set(CPUE2KState *env, target_ulong pcsp)
{
    env->pcsp_lo = SET_FIELD(env->pcsp_lo, pcsp, PCSP_LO_BASE_OFF,
        PCSP_LO_BASE_LEN);
}

static inline size_t e2k_state_pcs_index_get(CPUE2KState *env)
{
    return GET_FIELD(env->pcsp_hi, PCSP_HI_IND_OFF, PCSP_HI_IND_END);
}

static inline void e2k_state_pcs_index_set(CPUE2KState *env, size_t ind)
{
    env->pcsp_hi = SET_FIELD(env->pcsp_hi, ind, PCSP_HI_IND_OFF,
        PCSP_HI_IND_LEN);
}

static inline size_t e2k_state_pcs_size_get(CPUE2KState *env)
{
    return GET_FIELD(env->pcsp_hi, PCSP_HI_SIZE_OFF, PCSP_HI_SIZE_END);
}

static inline void e2k_state_pcs_size_set(CPUE2KState *env, size_t size)
{
    env->pcsp_hi = SET_FIELD(env->pcsp_hi, size, PCSP_HI_SIZE_OFF,
        PCSP_HI_SIZE_LEN);
}

static inline target_ulong e2k_state_ps_base_get(CPUE2KState *env)
{
    return GET_FIELD(env->psp_lo, PSP_LO_BASE_OFF, PSP_LO_BASE_END);
}

static inline size_t e2k_state_ps_ind_get(CPUE2KState *env)
{
    return GET_FIELD(env->psp_hi, PSP_HI_IND_OFF, PSP_HI_IND_LEN);
}

static inline void e2k_state_ps_ind_set(CPUE2KState *env, size_t ind)
{
    env->psp_hi = SET_FIELD(env->psp_hi, ind, PSP_HI_IND_OFF, PSP_HI_IND_LEN);
}

static inline int e2k_state_cr1_wbs_get(CPUE2KState *env)
{
    return GET_FIELD(env->cr1_lo, CR1_LO_WBS_OFF, CR1_LO_WBS_END);
}

static inline void e2k_state_cr1_wbs_set(CPUE2KState *env, int wbs)
{
    env->cr1_lo = SET_FIELD(env->cr1_lo, wbs, CR1_LO_WBS_OFF, CR1_LO_WBS_LEN);
}

static inline int e2k_state_cr1_wpsz_get(CPUE2KState *env)
{
    return GET_FIELD(env->cr1_lo, CR1_LO_WPSZ_OFF, CR1_LO_WPSZ_END);
}

static inline void e2k_state_cr1_wpsz_set(CPUE2KState *env, int wpsz)
{
    env->cr1_lo = SET_FIELD(env->cr1_lo, wpsz, CR1_LO_WPSZ_OFF,
        CR1_LO_WPSZ_LEN);
}

static inline uint32_t e2k_state_cr1_br_get(CPUE2KState *env)
{
    return GET_FIELD(env->cr1_hi, CR1_HI_BR_OFF, CR1_HI_BR_END);
}

static inline void e2k_state_cr1_br_set(CPUE2KState *env, uint32_t br)
{
    env->cr1_hi = SET_FIELD(env->cr1_hi, br, CR1_HI_BR_OFF, CR1_HI_BR_LEN);
}

typedef CPUE2KState CPUArchState;
typedef E2KCPU ArchCPU;

#include "exec/cpu-all.h"

#endif
