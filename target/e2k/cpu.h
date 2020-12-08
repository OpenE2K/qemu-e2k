#ifndef E2K_CPU_H
#define E2K_CPU_H

#include "qemu/bswap.h"
#include "cpu-qom.h"
#include "exec/cpu-defs.h"

void e2k_tcg_initialize(void);

#define GEN_MASK(start, len) (((1UL << (len)) - 1) << (start))
#define GET_BIT(v, index) (((v) >> (index)) & 1)
#define GET_FIELD(v, s, l) (((v) >> (s)) & GEN_MASK(0, l))
#define SET_FIELD(v, f, s, l) \
    ( \
        ((v) & ~GEN_MASK((s), (l))) | \
        ((((typeof((v))) (f)) << (s)) & GEN_MASK((s), (l))) \
    )

#define MMU_USER_IDX 1
#define CPU_RESOLVING_TYPE TYPE_E2K_CPU

#define E2K_TAG_SIZE 2 /* 2-bit tag for 32-bit value */
#define E2K_REG_LEN sizeof(uint64_t)
#define E2K_REG_SIZE (E2K_REG_LEN * 8)
#define E2K_REG_TAGS_SIZE (E2K_TAG_SIZE * 2) /* two tags for 32-bit halves */

#define E2K_WR_COUNT 64     /* %rN      [0, 64) */
#define E2K_BR_COUNT 128    /* %b[N]    [0, 128) */
#define E2K_NR_COUNT (E2K_WR_COUNT + E2K_BR_COUNT)
#define E2K_GR_COUNT 32     /* %gN      [0, 32) */
#define E2K_BGR_COUNT 8     /* %gN      [24, 32) */
#define E2K_REG_COUNT (E2K_NR_COUNT + E2K_GR_COUNT)

/* how many tags can be packed into a register */
#define E2K_TAGS_PER_REG (E2K_REG_LEN * 8 / E2K_REG_TAGS_SIZE)
/* packed tags registers count */
#define E2K_TAGS_REG_COUNT (E2K_REG_COUNT / E2K_TAGS_PER_REG)

#define E2K_PR_COUNT 32     /* %predN   [0, 32) */

typedef enum {
    E2K_TAG_NUMBER32 = 0,
    E2K_TAG_NUMBER64 = 0,
    E2K_TAG_NON_NUMBER32 = 1,
    E2K_TAG_NON_NUMBER64 = 5,
} E2kRegisterTag;

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

#define DESC_HI_IND_OFF 0       /* index for SPILL */
#define DESC_HI_IND_END 31
#define DESC_HI_IND_LEN (DESC_HI_IND_END - DESC_HI_IND_OFF + 1)
#define DESC_HI_SIZE_OFF 32     /* stack size */
#define DESC_HI_SIZE_END 63
#define DESC_HI_SIZE_LEN (DESC_HI_SIZE_END - DESC_HI_SIZE_OFF + 1)

#define DESC_LO_BASE_OFF 0      /* stack address */
#define DESC_LO_BASE_END 47
#define DESC_LO_BASE_LEN (DESC_LO_BASE_END - DESC_LO_BASE_OFF + 1)
#define DESC_LO_READ_OFF 59
#define DESC_LO_READ_BIT (1UL << DESC_LO_READ_OFF)
#define DESC_LO_WRITE_OFF 60
#define DESC_LO_WRITE_BIT (1UL << DESC_LO_WRITE_OFF)

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

#define UPSR_FE_OFF 0 /* floating point enable */
#define UPSR_FE_BIT 1
#define UPSR_SE_OFF 1 /* supervisor mode enable (only for Intel) */
#define UPSR_SE_BIT (1 << UPSR_SE_OFF)
#define UPSR_AC_OFF 2 /* not-aligned access control */
#define UPSR_AC_BIT (1 << UPSR_AC_OFF)
#define UPSR_DI_OFF 3 /* delayed interrupt (only for Intel) */
#define UPSR_DI_BIT (1 << UPSR_DI_OFF)
#define UPSR_WP_OFF 4 /* write protection (only for Intel) */
#define UPSR_WP_BIT (1 << UPSR_WP_OFF)
#define UPSR_IE_OFF 5 /* interrupt enable */
#define UPSR_IE_BIT (1 << UPSR_IE_OFF)
#define UPSR_A20_OFF 6 /* emulation of 1 Mb memory (only for Intel) */
#define UPSR_A20_BIT (1 << UPSR_A20_OFF)
#define UPSR_NMIE_OFF 7 /* not masked interrupt enable */
#define UPSR_NMIE_BIT (1 << UPSR_NMIE_OFF)
/* next field of register exist only on E3S/ES2/E2S/E8C/E1C+ CPUs */
#define UPSR_FSM_OFF 8 /* floating comparison mode flag */
                       /* 1 - compatible with x86/x87 */
#define UPSR_FSM_BIT (1 << UPSR_FSM_OFF)
#define UPSR_IMPT_OFF 9 /* ignore Memory Protection Table flag */
#define UPSR_IMPT_BIT (1 << UPSR_IMPT_OFF)
#define UPSR_IUC_OFF 10 /* ignore access right for uncached pages */
#define UPSR_IUC_BIT (1 << UPSR_IUC_OFF)

#define IDR_MDL_OFF 0 /* CPU model number */
#define IDR_MDL_END 7
#define IDR_MDL_LEN (IDR_MDL_END - IDR_MDL_OFF + 1)
#define IDR_REV_OFF 8 /* revision number */
#define IDR_REV_END 11
#define IDR_REV_LEN (IDR_REV_END - IDR_REV_OFF + 1)
#define IDR_WBL_OFF 12 /* write-back length of L2 */
#define IDR_WBL_END 14
#define IDR_WBL_LEN (IDR_WBL_END - IDR_WBL_OFF + 1)
#define IDR_MS_OFF 15 /* model specific info */
#define IDR_MS_END 63
#define IDR_MS_LEN (IDR_MS_END - IDR_MS_OFF + 1)

/* Cache write-back length */
#define IDR_WBL_0   0x0 /* none CPU internal cache */
#define IDR_WBL_32  0x1
#define IDR_WBL_64  0x2
#define IDR_WBL_128 0x3
#define IDR_WBL_256 0x4
#define IDR_WBL_TO_BYTES(wbl) ((wbl) ? (1 << ((wbs) + 4)) : 1)

typedef enum {
    E2K_EXCP_SYSCALL = 0x02,
    E2K_EXCP_ILLOPC = 0x03,
    E2K_EXCP_ILLOPN = 0x04,
    E2K_EXCP_MAPERR = 0x05,
} Exception;

struct e2k_def_t {
    const char *name;
    const char *canonical_name;
    const char *gdb_arch;
    uint32_t isa_version;
};

typedef struct {
    uint32_t base;
    uint32_t size;
    uint32_t cur;
} E2KBnState;

typedef struct {
    uint32_t size;
    uint32_t cur;
} E2KBpState;

typedef struct {
    uint32_t br;
    uint32_t cuir;
    uint32_t ussz;
    uint16_t tr;
    uint8_t ein;
    bool ss;
    bool wfx;
    uint8_t wpsz;
    uint8_t wbs;
    uint8_t psr;
    bool wdbl;
} E2KCr1State;

typedef struct {
    void *base;
    uint32_t index;
    uint32_t size;
    bool is_readable;
    bool is_writable;
} E2KDescState, E2KPsState, E2KPcsState;

typedef struct {
    int16_t index;
    uint16_t fx_index;
    uint16_t t_index;
    bool fx;
} E2KPshtpState;

typedef struct {
    uint32_t base;
    uint32_t size;
    uint32_t psize;
    bool fx;
} E2KWdState;

typedef enum {
    AASR_NULL = 0,
    AASR_READY = 1,
    AASR_ACTIVE = 3,
    AASR_STOPPED = 5,
} E2KAasrState;

typedef union {
    struct {
        uint32_t unused     : 5;
        uint32_t stb        : 1;
        uint32_t iab        : 1;
        uint32_t lds        : 3;
    };
    uint32_t raw;
} E2KAasr;

typedef enum {
    AAD_TAG_UNV = 0,
    AAD_TAG_UDT = 1,
    AAD_TAG_UET = 2,
    AAD_TAG_UAP = 4,
    AAD_TAG_USAP = 5,
    AAD_TAG_UDS = 6,
} E2KAadTag;

typedef struct {
    union {
        struct {
            uint64_t base       : 48;   /* 48:0 */
            uint64_t unused1    : 6;    /* 53:48 */
            uint64_t tag        : 3;    /* 56:54 */
            uint64_t mb         : 1;    /* 57 */
            uint64_t ed         : 1;    /* 58 */
            uint64_t rw         : 2;    /* 60:59 */
            uint64_t unused2    : 3;    /* 63:60 */
        };
        uint64_t lo;
    };
    union {
        struct {
            uint64_t unused3    : 32;
            uint64_t size       : 32;
        };
        uint64_t hi;
    };
} E2KAad;

typedef enum {
    AALDA_EXC_EIO = 1,
    AALDA_EXC_EPM = 2,
    AALDA_EXC_EPMSI = 3,
} E2KAaldaExc;

typedef union {
    struct {
        uint8_t exc:        2;
        uint8_t cincr:      1;
        uint8_t unused1:    1;
        uint8_t root:       1;
        uint8_t unused2:    3;
    };
    uint8_t raw;
} E2KAalda;

typedef struct {
    E2KAasr sr;
    uint32_t fstr;
    uint64_t ldm;
    uint64_t ldv;
    uint32_t stis[16];
    uint32_t sti_tags;
    uint32_t incrs[8];
    uint32_t incr_tags;
    uint32_t inds[16];
    uint32_t ind_tags;
    E2KAad ds[32];
    uint32_t ldi[64];
    E2KAalda lda[64];
    uint64_t pib[64];
} E2KAauState;

typedef union {
    struct {
        uint64_t base: 48;      /* 47:0 */
        uint64_t unused1: 6;    /* 53:48 */
        uint64_t tag: 3;        /* 56:54 */
        uint64_t opc: 2;        /* 58:57 */
        uint64_t ipd: 2;        /* 60:59 */
        uint64_t unused2: 3;    /* 63:61 */
    };
    uint64_t raw;
} E2KCtpr;

typedef struct {
    /* register file */
    uint64_t regs[E2K_REG_COUNT]; /* registers */
    uint64_t tags[E2K_TAGS_REG_COUNT]; /* registers tags */
    uint64_t *rptr; /* pointer to regs */
    uint64_t *tptr; /* pointer to tags */

    E2KCr1State cr1;

    /* Procedure chain info = cr0_lo, cr0_hi, cr1_lo, cr1_hi */
    E2KPcsState pcsp;
    uint64_t pcshtp;

    /* Procedure stack pointer (for regs)  */
    E2KPsState psp;
    E2KPshtpState pshtp;

    E2KWdState wd;
    E2KBnState bn;
    E2KBpState bp;

    uint64_t lsr; /* loop status register */

    uint64_t usd_lo;
    uint64_t usd_hi;

    /* control registers */
    E2KCtpr ctprs[3]; // Control Transfer Preparation Register (CTPR)
    target_ulong ct_cond;
    
    union {
        uint64_t pregs; /* predicate file */
        uint64_t cr0_lo;
    };
    union {
        target_ulong ip; /* instruction address */
        uint64_t cr0_hi;
    };

    target_ulong nip; /* next instruction address */
    
    uint64_t upsr;
    uint64_t idr;

    uint32_t pfpfr; // Packed Floating Point Flag Register (PFPFR)
    uint32_t fpcr; // Floating point control register (FPCR)
    uint32_t fpsr; // Floating point state register (FPSR)

    E2KAauState aau;

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
void e2k_cpu_list(void);
int e2k_cpu_signal_handler(int host_signum, void *pinfo, void *puc);
int e2k_cpu_gdb_read_register(CPUState *cs, GByteArray *mem_buf, int n);
int e2k_cpu_gdb_write_register(CPUState *cs, uint8_t *mem_buf, int n);
void e2k_break_save_state(CPUE2KState *env);

#define cpu_signal_handler e2k_cpu_signal_handler
#define cpu_list e2k_cpu_list

static inline uint64_t e2k_state_desc_lo(E2KDescState *desc)
{
    uint64_t lo = 0;

    lo = deposit64(lo, DESC_LO_BASE_OFF, DESC_LO_BASE_LEN,
        (uint64_t) desc->base);
    lo = deposit64(lo, DESC_LO_READ_OFF, 1, desc->is_readable);
    lo = deposit64(lo, DESC_LO_WRITE_OFF, 1, desc->is_writable);

    return lo;
}

static inline uint64_t e2k_state_desc_hi(E2KDescState *env)
{
    uint64_t hi = 0;

    hi = deposit64(hi, DESC_HI_IND_OFF, DESC_HI_IND_LEN, env->index);
    hi = deposit64(hi, DESC_HI_SIZE_OFF, DESC_HI_SIZE_OFF, env->size);

    return hi;
}

#define e2k_state_pcsp_lo(env) e2k_state_desc_lo(&(env)->pcsp)
#define e2k_state_pcsp_hi(env) e2k_state_desc_hi(&(env)->pcsp)
#define e2k_state_psp_lo(env) e2k_state_desc_lo(&(env)->psp)
#define e2k_state_psp_hi(env) e2k_state_desc_hi(&(env)->psp)

static inline uint64_t e2k_state_pshtp(CPUE2KState *env)
{
    E2KPshtpState *s = &env->pshtp;
    uint64_t ret = 0;

    ret = deposit64(ret, PSHTP_IND_OFF, PSHTP_IND_LEN, s->index);
    ret = deposit64(ret, PSHTP_FXIND_OFF, PSHTP_FXIND_LEN, s->fx_index);
    ret = deposit64(ret, PSHTP_TIND_OFF, PSHTP_TIND_LEN, s->t_index);
    ret = deposit64(ret, PSHTP_FX_OFF, 1, s->fx);

    return ret;
}

static inline uint64_t e2k_state_wd(CPUE2KState *env)
{
    E2KWdState *wd = &env->wd;
    uint64_t ret = 0;

    ret = deposit64(ret, WD_BASE_OFF, WD_BASE_LEN, wd->base * 8);
    ret = deposit64(ret, WD_SIZE_OFF, WD_SIZE_LEN, wd->size * 8);
    ret = deposit64(ret, WD_PSIZE_OFF, WD_PSIZE_LEN, wd->psize * 8);
    ret = deposit64(ret, WD_FX_OFF, 1, wd->fx);

    return ret;
}

static inline uint32_t e2k_state_br(CPUE2KState *env)
{
    E2KBnState *bn = &env->bn;
    E2KBpState *bp = &env->bp;
    uint32_t ret = 0;

    ret = deposit32(ret, BR_RBS_OFF, BR_RBS_LEN, bn->base / 2);
    ret = deposit32(ret, BR_RSZ_OFF, BR_RSZ_LEN, bn->size / 2 - 1);
    ret = deposit32(ret, BR_RCUR_OFF, BR_RCUR_LEN, bn->cur / 2);

    ret = deposit32(ret, BR_PSZ_OFF, BR_PSZ_LEN, bp->size);
    ret = deposit32(ret, BR_PCUR_OFF, BR_PCUR_LEN, bp->cur);

    return ret;
}

static inline void e2k_state_br_set(CPUE2KState *env, uint32_t br)
{
    E2KBnState *bn = &env->bn;
    E2KBpState *bp = &env->bp;

    bn->base = extract32(br, BR_RBS_OFF, BR_RBS_LEN) * 2;
    bn->size = extract32(br, BR_RSZ_OFF, BR_RSZ_LEN) * 2 + 2;
    bn->cur = extract32(br, BR_RCUR_OFF, BR_RCUR_LEN) * 2;

    bp->size = extract32(br, BR_PSZ_OFF, BR_PSZ_LEN);
    bp->cur = extract32(br, BR_PCUR_OFF, BR_PCUR_LEN);
}

static inline uint64_t e2k_state_cr1_lo(CPUE2KState *env)
{
    E2KCr1State *cr1 = &env->cr1;
    uint64_t ret = 0;

    ret = deposit64(ret, CR1_LO_TR_OFF, CR1_LO_TR_LEN, cr1->tr);
    ret = deposit64(ret, CR1_LO_EIN_OFF, CR1_LO_EIN_LEN, cr1->ein);
    ret = deposit64(ret, CR1_LO_SS_OFF, 1, cr1->ss);
    ret = deposit64(ret, CR1_LO_WFX_OFF, 1, cr1->wfx);
    ret = deposit64(ret, CR1_LO_WPSZ_OFF, CR1_LO_WPSZ_LEN, cr1->wpsz);
    ret = deposit64(ret, CR1_LO_WBS_OFF, CR1_LO_WBS_LEN, cr1->wbs);
    ret = deposit64(ret, CR1_LO_CUIR_OFF, CR1_LO_CUIR_LEN, cr1->cuir);
    ret = deposit64(ret, CR1_LO_PSR_OFF, CR1_LO_PSR_LEN, cr1->psr);

    return ret;
}

static inline void e2k_state_cr1_lo_set(CPUE2KState *env, uint64_t lo)
{
    E2KCr1State *cr1 = &env->cr1;

    cr1->tr = extract64(lo, CR1_LO_TR_OFF, CR1_LO_TR_LEN);
    cr1->ein = extract64(lo, CR1_LO_EIN_OFF, CR1_LO_EIN_LEN);
    cr1->ss = extract64(lo, CR1_LO_SS_OFF, 1);
    cr1->wfx = extract64(lo, CR1_LO_WFX_OFF, 1);
    cr1->wpsz = extract64(lo, CR1_LO_WPSZ_OFF, CR1_LO_WPSZ_LEN);
    cr1->wbs = extract64(lo, CR1_LO_WBS_OFF, CR1_LO_WBS_LEN);
    cr1->cuir = extract64(lo, CR1_LO_CUIR_OFF, CR1_LO_CUIR_LEN);
    cr1->psr = extract64(lo, CR1_LO_PSR_OFF, CR1_LO_PSR_LEN);
}

static inline uint64_t e2k_state_cr1_hi(CPUE2KState *env)
{
    E2KCr1State *cr1 = &env->cr1;
    uint64_t ret = 0;

    ret = deposit64(ret, CR1_HI_BR_OFF, CR1_HI_BR_LEN, cr1->br);
    ret = deposit64(ret, CR1_HI_WDBL_OFF, 1, cr1->wdbl);
    ret = deposit64(ret, CR1_HI_USSZ_OFF, CR1_HI_USSZ_LEN, cr1->ussz);

    return ret;
}

static inline void e2k_state_cr1_hi_set(CPUE2KState *env, uint64_t hi)
{
    E2KCr1State *cr1 = &env->cr1;

    cr1->br = extract64(hi, CR1_HI_BR_OFF, CR1_HI_BR_LEN);
    cr1->wdbl = extract64(hi, CR1_HI_WDBL_OFF, 1);
    cr1->ussz = extract64(hi, CR1_HI_USSZ_OFF, CR1_HI_USSZ_LEN);
}

typedef CPUE2KState CPUArchState;
typedef E2KCPU ArchCPU;

#include "exec/cpu-all.h"

#endif
