#ifndef E2K_CPU_H
#define E2K_CPU_H

#include "qemu/bswap.h"
#include "cpu-qom.h"
#include "exec/cpu-defs.h"
#include "fpu/softfloat.h"

void e2k_tcg_initialize(void);

/* a1ba: fsqrtid, fxsqrtixx, fxsqrtuxx are instructions that
 * calculate intermediate square root
 * I don't think the results of these instructions are meant to be used
 * somewhere else than final square root instruction (fsqrttd, fxsqrttxx)
 * Correctly emulating that instruction may impact performance
 *
 * Uncomment this macro to have correct implementation,
 * or left as is to just copy values
 */

/* #define TARGET_E2K_PRECISE_FSQRTID */


/* FIXME: nfx window mode
 * numas13: I don't know why elbrus always spill/fill in fx mode.
 * If we enable 'nfx' mode It will break "finish" command in gdb because
 * it will try to read registers from hell.
 */
#define E2K_FORCE_FX true

#define GEN_MASK(start, len) (((1ULL << (len)) - 1) << (start))
#define GET_BIT(v, index) (((v) >> (index)) & 1)

#define MMU_USER_IDX 1
#define E2K_VA_SIZE 48
#define E2K_VA_MSB (E2K_VA_SIZE - 1)

#define CPU_RESOLVING_TYPE TYPE_E2K_CPU

#define E2K_REG_LEN sizeof(uint64_t)
#define E2K_REG_SIZE (E2K_REG_LEN * 8)

#define E2K_TAG_MASK_32 0x03
#define E2K_TAG_MASK_64 0x0f
#define E2K_TAG_MASK_80 E2K_TAG_MASK_64
#define E2K_TAG_MASK_128 0xff

#define E2K_WR_COUNT 64     /* %rN      [0, 64) */
#define E2K_BR_COUNT 128    /* %b[N]    [0, 128) */
#define E2K_NR_COUNT 224
#define E2K_GR_COUNT 32     /* %gN      [0, 32) */
#define E2K_BGR_COUNT 8     /* %gN      [24, 32) */
#define E2K_REG_COUNT (E2K_NR_COUNT + E2K_GR_COUNT)
#define E2K_TLS_REG (E2K_NR_COUNT + 13)

#define E2K_PR_COUNT 32     /* %predN   [0, 32) */

typedef enum {
    E2K_TAG_NUMBER32 = 0,
    E2K_TAG_NUMBER64 = 0,
    E2K_TAG_NUMBER80 = 0,
    E2K_TAG_NUMBER128 = 0,

    E2K_TAG_NON_NUMBER32 = 0x01,
    E2K_TAG_NON_NUMBER64 = 0x05,
    E2K_TAG_NON_NUMBER80 = 0x05,
    E2K_TAG_NON_NUMBER128 = 0x55,
} E2kRegisterTag;

#define E2K_MOVA_RESULT_INVALID 0xeaed0f70eaed0f70
#define E2K_LD_RESULT_INVALID 0x4afafafa4afafafa

#define CRS_SIZE (sizeof(E2KCrs))

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

typedef enum {
    CTPR_TAG_NONE = 0x0,
    CTPR_TAG_RETURN = 0x2,
    CTPR_TAG_DISP = 0x3,
    CTPR_TAG_SDISP = 0x5,
} CtprTag;

typedef enum {
    CTPR_OPC_DISP = 0x0,
    CTPR_OPC_LDISP = 0x1,
    CTPR_OPC_SIGRET = 0x3,
} CtprOpc;

#ifdef CONFIG_USER_ONLY
# define E2K_SYSCALL_MAX_ARGS 10
/* fake kernel addresses */
# if TARGET_LONG_BITS == 64
#  define E2K_FAKE_KERN_START 0xe20000000000
#  define E2K_FAKE_KERN_END 0xe30000000000
# else /* TARGET_LONG_BITS == 32 */
#  define E2K_FAKE_KERN_START 0xe0000000
#  define E2K_FAKE_KERN_END 0xe3000000
# endif
# define E2K_SYSCALL_ADDR1 (E2K_FAKE_KERN_START + 0x800 * 1)
# define E2K_SYSCALL_ADDR3 (E2K_FAKE_KERN_START + 0x800 * 3)
# define E2K_SYSCALL_ADDR4 (E2K_FAKE_KERN_START + 0x800 * 4)
# define E2K_SYSCALL_ADDR6 (E2K_FAKE_KERN_START + 0x800 * 6)
# define E2K_SYSRET_ADDR (E2K_FAKE_KERN_START + 0x15700)
# define E2K_SIGRET_ADDR (E2K_FAKE_KERN_START + 0x15800)
# define E2K_SYSRET_BACKTRACE_ADDR (E2K_FAKE_KERN_START + 0x15900)
#endif

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

#define USD_LO_BASE_OFF 0
#define USD_LO_BASE_LEN 48
#define USD_LO_PROTECTED_OFF 58
#define USD_LO_PROTECTED_BIT (1ULL << USD_LO_PROTECTED_OFF)
#define USD_LO_READ_OFF 59
#define USD_LO_READ_BIT (1ULL << USD_LO_READ_OFF)
#define USD_LO_WRITE_OFF 60
#define USD_LO_WRITE_BIT (1ULL << USD_LO_WRITE_OFF)

#define USD_HI_CURPTR_OFF 0
#define USD_HI_CURPTR_LEN 32
#define USD_HI_SIZE_OFF 32
#define USD_HI_SIZE_LEN 32

#define CR1_HI_BR_OFF 0
#define CR1_HI_BR_LEN 28
#define CR1_HI_WDBL_OFF 35
#define CR1_HI_WDBL_BIT (1ULL << CR1_HI_WDBL_OFF)
#define CR1_HI_USSZ_OFF 36
#define CR1_HI_USSZ_LEN 28

#define CR1_LO_TR_OFF 0
#define CR1_LO_TR_LEN 15
#define CR1_LO_EIN_OFF 16
#define CR1_LO_EIN_LEN 8
#define CR1_LO_SS_OFF 24
#define CR1_LO_SS_BIT (1ULL << CR1_LO_SS_OFF)
#define CR1_LO_WFX_OFF 25
#define CR1_LO_WFX_BIT (1ULL << CR1_LO_WFX_OFF)
#define CR1_LO_WPSZ_OFF 26
#define CR1_LO_WPSZ_LEN 7
#define CR1_LO_WBS_OFF 33
#define CR1_LO_WBS_LEN 7
#define CR1_LO_CUIR_OFF 40
#define CR1_LO_CUIR_LEN 17
#define CR1_LO_PSR_OFF 57
#define CR1_LO_PSR_LEN 7

#define BR_RBS_OFF 0        /* based regs window offset */
#define BR_RBS_LEN 6
#define BR_RSZ_OFF 6        /* based regs window size */
#define BR_RSZ_LEN 6
#define BR_RCUR_OFF 12      /* based regs current index */
#define BR_RCUR_LEN 6
#define BR_BN_OFF 0
#define BR_BN_LEN 18
#define BR_PSZ_OFF 18       /* based pregs window size */
#define BR_PSZ_LEN 5
#define BR_PCUR_OFF 23      /* based pregs current index */
#define BR_PCUR_LEN 5
#define BR_BP_OFF 18
#define BR_BP_LEN 10

#define BGR_VAL_OFF 0
#define BGR_VAL_LEN 8
#define BGR_CUR_OFF 8
#define BGR_CUR_LEN 3
#define BGR_MASK 0x7ff

#define LSR_LCNT_OFF 0      /* loop counter */
#define LSR_LCNT_LEN 32
#define LSR_ECNT_OFF 32     /* epilogue counter */
#define LSR_ECNT_LEN 5
#define LSR_VLC_OFF 37      /* loop count valid bit */
#define LSR_VLC_BIT (1UL << LSR_VLC_OFF)
#define LSR_OVER_OFF 38     /* loop count overflow */
#define LSR_LDMC_OFF 39     /* loads manual control */
#define LSR_LDOVL_OFF 40    /* load overlap */
#define LSR_LDOVL_LEN 8
#define LSR_PCNT_OFF 48     /* prologue counter */
#define LSR_PCNT_LEN 5
#define LSR_STRMD_OFF 53    /* store remainder counter */
#define LSR_STRMD_LEN 7
#define LSR_SEMC_OFF 60     /* side effects manual control */
#define ILCR_MASK 0x1f001f

#define PSR_PM      0x01U   /* privileged mode */
#define PSR_IE      0x02U
#define PSR_SGE     0x04U
#define PSR_LW      0x08U
#define PSR_UIE     0x10U
#define PSR_NMIE    0x20U
#define PSR_UNMIE   0x40U
#define PSR_MASK    0x7fU

#define UPSR_FE     0x01U   /* floating point enable */
#define UPSR_SE     0x02U   /* supervisor mode enable (only for Intel) */
#define UPSR_AC     0x04U   /* not-aligned access control */
#define UPSR_DI     0x08U   /* delayed interrupt (only for Intel) */
#define UPSR_WP     0x10U   /* write protection (only for Intel) */
#define UPSR_IE     0x20U   /* interrupt enable */
#define UPSR_A20    0x40U   /* emulation of 1 Mb memory (only for Intel) */
#define UPSR_NMIE   0x80U   /* not masked interrupt enable */
#define UPSR_FSM    0x100U  /* only on E3S/ES2/E2S/E8C/E1C+ CPUs */
                            /* floating comparison mode flag */
                            /* 1 - compatible with x86/x87 */
#define UPSR_IMPT   0x200U  /* ignore Memory Protection Table flag */
#define UPSR_IUC    0x400U  /* ignore access right for uncached pages */
#define UPSR_MASK   0x7ff

#define CUIR_MASK 0x1ffff

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
    EXCP_ILLEGAL_OPCODE = 0,
    EXCP_PRIV_ACTION = 1,
    EXCP_FP_DISABLED = 2,
    EXCP_FP_STACK_U = 3,
    EXCP_D_INTERRUPT = 4,
    EXCP_DIAG_CT_COND = 5,
    EXCP_DIAG_INSTR_ADDR = 6,
    EXCP_ILLEGAL_INSTR_ADDR = 7,
    EXCP_INSTR_DEBUG = 8,
    EXCP_WINDOW_BOUNDS = 9,
    EXCP_USER_STACK_BOUNDS = 10,
    EXCP_PROC_STACK_BOUNDS = 11,
    EXCP_CHAIN_STACK_BOUNDS = 12,
    EXCP_FP_STACK_O = 13,
    EXCP_DIAG_COND = 14,
    EXCP_DIAG_OPERAND = 15,
    EXCP_ILLEGAL_OPERAND = 16,
    EXCP_ARRAY_BOUNDS = 17,
    EXCP_ACCESS_RIGHTS = 18,
    EXCP_ADDR_NOT_ALIGNED = 19,
    EXCP_INSTR_PAGE_MISS = 20,
    EXCP_INSTR_PAGE_PROT = 21,
    EXCP_AINSTR_PAGE_MISS = 22,
    EXCP_AINSTR_PAGE_PROT = 23,
    EXCP_LAST_WISH = 24,
    EXCP_BASE_NOT_ALIGNED = 25,

    EXCP_DATA_DEBUG = 28,
    EXCP_DATA_PAGE = 29,

    EXCP_RECOVERY_POINT = 31,
    EXCP_INTERRUPT = 32,
    EXCP_NM_INTERRUPT = 33,
    EXCP_DIV = 34,
    EXCP_FP = 35,
    EXCP_MEM_LOCK = 36,
    EXCP_MEM_LOCK_AS = 37,
    EXCP_MEM_ERROR_OUT_CPU = 38,
    EXCP_MEM_ERROR_MAU = 39,
    EXCP_MEM_ERROR_L2 = 40,
    EXCP_MEM_ERROR_L1_35 = 41,
    EXCP_MEM_ERROR_L1_02 = 42,
    EXCP_MEM_ERROR_ICACHE = 43,

    EXCP_MAX = 43,

#ifdef CONFIG_USER_ONLY
    EXCP_SYSCALL = 100,
#endif
} Exception;

typedef enum {
    SR_PSR          = 0x00,
    SR_WD           = 0x01,
    SR_CORE_MODE    = 0x04,
    SR_CWD          = 0x06,
    SR_PSP_HI       = 0x07,
    SR_PSP_LO       = 0x09,
    SR_PSHTP        = 0x0b,
    SR_PCSP_HI      = 0x0d,
    SR_PCSP_LO      = 0x0f,
    SR_PCSHTP       = 0x13,
    SR_CTPR1        = 0x15,
    SR_CTPR2        = 0x16,
    SR_CTPR3        = 0x17,
    SR_SBR          = 0x1e,
    SR_CUTD         = 0x21,
    SR_EIR          = 0x23,
    SR_TSD          = 0x24,
    SR_CUIR         = 0x25,
    SR_OSCUD_HI     = 0x26,
    SR_OSCUD_LO     = 0x27,
    SR_OSGD_HI      = 0x28,
    SR_OSGD_LO      = 0x29,
    SR_OSEM         = 0x2a,
    SR_USD_HI       = 0x2c,
    SR_USD_LO       = 0x2d,
    SR_TR           = 0x2e,
    SR_OSR0         = 0x2f,
    SR_CUD_HI       = 0x30,
    SR_CUD_LO       = 0x31,
    SR_GD_HI        = 0x32,
    SR_GD_LO        = 0x33,
    SR_CS_HI        = 0x34,
    SR_CS_LO        = 0x35,
    SR_DS_HI        = 0x36,
    SR_DS_LO        = 0x37,
    SR_ES_HI        = 0x38,
    SR_ES_LO        = 0x39,
    SR_FS_HI        = 0x3a,
    SR_FS_LO        = 0x3b,
    SR_GS_HI        = 0x3c,
    SR_GS_LO        = 0x3d,
    SR_SS_HI        = 0x3e,
    SR_SS_LO        = 0x3f,
    SR_DIBCR        = 0x40,
    SR_DIMCR        = 0x41,
    SR_DIBSR        = 0x42,
    SR_DTCR         = 0x43,
    SR_DIBAR0       = 0x48,
    SR_DIBAR1       = 0x49,
    SR_DIBAR2       = 0x4a,
    SR_DIBAR3       = 0x4b,
    SR_DIMAR0       = 0x4c,
    SR_DIMAR1       = 0x4d,
    SR_DTRAF        = 0x4e,
    SR_DTART        = 0x4f,
    SR_CR0_HI       = 0x51,
    SR_CR0_LO       = 0x53,
    SR_CR1_HI       = 0x55,
    SR_CR1_LO       = 0x57,
    SR_SCLKM1       = 0x70,
    SR_SCLKM2       = 0x71,
    SR_CU_HW0       = 0x78,
    SR_UPSR         = 0x80,
    SR_IP           = 0x81,
    SR_NIP          = 0x82,
    SR_LSR          = 0x83,
    SR_PFPFR        = 0x84,
    SR_FPCR         = 0x85,
    SR_FPSR         = 0x86,
    SR_ILCR         = 0x87,
    SR_BR           = 0x88,
    SR_BGR          = 0x89,
    SR_IDR          = 0x8A,
    SR_CLKR         = 0x90,
    SR_RNDPR        = 0x91,
    SR_SCLKR        = 0x92,
    SR_TIR_HI       = 0x9C,
    SR_TIR_LO       = 0x9D,
    SR_RPR          = 0xA0,
    SR_SBBP         = 0xA1,
    SR_RPR_HI       = 0xA2,
    SR_UPSRM        = 0xC0,
    SR_LSR1         = 0xC3,
    SR_ILCR1        = 0xC7,
} StateReg;

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
    union {
        struct {
            uint64_t tr: 15;
            uint64_t unused1: 1;
            uint64_t ein: 8;
            uint64_t ss: 1;
            uint64_t wfx: 1;
            uint64_t wpsz: 7;
            uint64_t wbs: 7;
            uint64_t cuir: 17;
            uint64_t psr: 7;
        };
        struct {
            uint64_t unused2: 40;
            uint64_t cui: 16;
            uint64_t ic: 1;
            uint64_t pm: 1;         /* privileged mode */
            uint64_t ie: 1;
            uint64_t sge: 1;
            uint64_t lw: 1;
            uint64_t uie: 1;
            uint64_t nmie: 1;
            uint64_t unmie: 1;
        };
        uint64_t lo;
    };
    union {
        struct {
            uint64_t br: 28;
            uint64_t unused3: 7;
            uint64_t wdbl: 1;
            /* user stack size */
            uint64_t ussz: 28;
        };
        struct {
            uint64_t rbs: 6;
            uint64_t rsz: 6;
            uint64_t rcur: 6;
            uint64_t psz: 5;
            uint64_t pcur: 5;
            uint64_t unused4: 36;
        };
        uint64_t hi;
    };
} E2KCr1;

typedef struct {
    uint64_t cr0_lo;
    uint64_t cr0_hi;
    E2KCr1 cr1;
} E2KCrs;

typedef struct {
    union {
        struct {
            uint64_t base: 48;          /* 47:0 */
            uint64_t unused1: 7;        /* 55:48 */
            uint64_t stub3: 1;          /* 56 */
            uint64_t stub2: 1;          /* 57 */
            uint64_t stub1: 1;          /* 58 */
            uint64_t is_readable: 1;    /* 59 */
            uint64_t is_writable: 1;    /* 60 */
            uint64_t itag: 3;           /* 63:61 */
        };
        uint64_t lo;
    };
    union {
        struct {
            uint64_t index: 32;         /* 31:0 */
            uint64_t size: 32;          /* 63:32 */
        };
        uint64_t hi;
    };
    /* FIXME: HACK: pointer to tag storage */
    target_ulong base_tag;
} E2KPsp;

#define E2K_PSP_LO_MASK (GEN_MASK(56, 8) | GEN_MASK(0, E2K_VA_SIZE))

typedef struct {
    int32_t base;
    int32_t size;
    int32_t psize;
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

/* AAU prefetch instruction */
typedef union {
    struct {
        union {
            struct {
                uint32_t abs: 5;
                uint32_t asz: 3;

                /* version >= 2 || si == 0 */
                uint32_t ind: 4;
                uint32_t incr: 3;

                uint32_t aad: 5;
                uint32_t mrng: 5;
                uint32_t fmt: 3;
                uint32_t dcd: 2;
                uint32_t si: 1;
                uint32_t ct: 1;
            };
            struct {
                uint32_t unused1: 8;

                /* version <= 1 && si == 1 */
                uint32_t area: 5;
                uint32_t am: 1;
                uint32_t be: 1;

                uint32_t unused2: 16;
                uint32_t dpl: 1;
            };
            uint32_t lo;
        };
        union {
            uint32_t disp;
            uint32_t hi;
        };
    };
    uint64_t raw;
} E2KAauPrefInstr;

typedef struct {
    E2KAauPrefInstr pi;     /* prefetch instr */
    uint32_t cdi;   /* current data index */
    uint32_t ldi;   /* loaded data index */
    target_ulong last_page;
    bool last_page_valid;
} E2KAauAreaState;

typedef struct {
    E2KAauAreaState area[32];
} E2KAauPrefState;

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
    E2KAalda lda[64];
    E2KAauPrefState pl, pr;
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

/* E2K FPU regs are compatible with x87 regs */
#define FP_IE (1U << 0) /* invalid operation */
#define FP_DE (1U << 1) /* denormalized operand */
#define FP_ZE (1U << 2) /* zero divide */
#define FP_OE (1U << 3) /* overflow */
#define FP_UE (1U << 4) /* underflow */
#define FP_PE (1U << 5) /* precision */

typedef union {
    struct {
        uint32_t ef : 6; /* exception flags */
        uint32_t sf : 1; /* stack fault, unused */
        uint32_t es : 1; /* error summary status */
        uint32_t _c0 : 1; /* condition code 0, unused */
        uint32_t c1 : 1; /* condition code 1 */
        uint32_t _c2 : 1; /* condition code 2, unused */
        uint32_t top: 3; /* stack top, unused */
        uint32_t _c3 : 1; /* condition code 3, unused */
        uint32_t b  : 1; /* fpu busy */
    };
    uint32_t raw;
} E2KFpsrState;

#define FP_EM (FP_IE|FP_DE|FP_ZE|FP_OE|FP_UE|FP_PE)

#define FPCR_PC_SP       0 /* single precision (32 bits) */
#define FPCR_PC_RESERVED 1 /* reserved */
#define FPCR_PC_DP       2 /* double precision (64 bits) */
#define FPCR_PC_XP       3 /* extended precision (80 bits) */

#define FP_RC_NEAR 0 /* round to nearest */
#define FP_RC_DOWN 1 /* round down */
#define FP_RC_UP   2 /* round up */
#define FP_RC_CHOP 3 /* round toward zero (truncate) */

typedef union {
    struct {
        uint32_t em : 6; /* masks flags */
        uint32_t _one : 1; /* reserved, always 1 (?) */
        uint32_t _zero0 : 1; /* reserved, always 0 */
        uint32_t pc : 2; /* precision control */
        uint32_t rc : 2; /* rounding control */
        uint32_t ic : 1; /* infinity control */
        uint32_t _zero1 : 3; /* reserved */             
    };
    uint32_t raw;
} E2KFpcrState;

/*
 *  Packed Floating Point Flag Register (PFPFR)
 */
typedef union {
    struct {
        uint32_t ef: 6;         /* 5:0 */
        uint32_t zero1: 1;      /* 6 */
        uint32_t em: 6;         /* 12:7 */
        uint32_t rc: 2;         /* 14:13 */
        uint32_t fz: 1;         /* 15 */
        uint32_t zero2: 10;     /* 25:16 */
        uint32_t dm: 6;         /* 31:26 */
    };
    uint32_t raw;
} E2KPfpfr;

typedef struct {
    union {
        uint64_t lo;
        struct {
            uint64_t base      : E2K_VA_SIZE;       /* [47: 0] */
            uint64_t unused1   : 57 - E2K_VA_MSB;   /* [57:48] */
            uint64_t protected :  1;                /* [58] */
            uint64_t read      :  1;                /* [59] */
            uint64_t write     :  1;                /* [60] */
            uint64_t unsued2   :  3;                /* [63:61] */
        };
    };
    union {
        uint64_t hi;
        struct {
            uint64_t curptr    : 32; /* [31: 0] */
            uint64_t size      : 32; /* [63:32] */
        };
    };
} E2KRwap;

#define E2K_RWAP_LO_MASK (GEN_MASK(58, 3) | GEN_MASK(0, E2K_VA_SIZE))

typedef union {
    struct {
        uint64_t l1col: 2;  /* 1:0 */
        uint64_t l1msb: 2;  /* 3:2 */
        uint64_t word: 9;   /* 12:4 */
        uint64_t page: 28;  /* 40:13 */
        uint64_t mask: 8;   /* 48:41 */
        uint64_t rg: 8;     /* 56:49 */
        uint64_t quadro: 1; /* 57 */
        uint64_t val: 1;    /* 58 */
        uint64_t unused: 5; /* 63:59 */
    };
    uint64_t raw;
} E2KDamEntry;

typedef union {
    uint32_t u32;
    uint64_t u64;
    floatx80 f80;

    uint8_t ub[16];
    uint16_t uh[8];
    uint32_t uw[4];
    uint64_t ud[2];

    int8_t sb[16];
    int16_t sh[8];
    int32_t sw[4];
    int64_t sd[2];

    struct {
        uint64_t lo;
        uint64_t hi;
    };
} E2KReg;

typedef struct {
    /* Registers Tags File */
    uint8_t tags[E2K_REG_COUNT];
    /* Registers File */
    E2KReg regs[E2K_REG_COUNT];
    /* Predicate Registers File */
    uint64_t pregs;
    /* Instruction Address */
    target_ulong ip;
    /* Next Instruction Address */
    target_ulong nip;

    /* Procedure Chain Stack Pointer (control registers) */
    E2KPsp pcsp;
    /* Procedure Stack Pointer (window registers)  */
    E2KPsp psp;
    /* Non-Protected User Stack Pointer */
    E2KRwap usd;
    /* Non-Protected User Stack Base address */
    uint64_t sbr;
    /* Procedure Registers Window state */
    E2KWdState wd;
    /* Flag to clear the upper half of the register for 32-bit operations */
    uint32_t wdbl;
    /* Based Registers Window state */
    E2KBnState bn;
    /* Based Predicate Registers Window state */
    E2KBpState bp;
    /* Based Global Registers Window state */
    uint32_t bgr;

    /* Loop Status Register */
    uint64_t lsr;
    uint64_t lsr_lcnt;
    uint32_t lsr_ecnt;
    uint32_t lsr_vlc;
    uint32_t lsr_over;
    uint32_t lsr_pcnt;
    uint32_t lsr_strmd;

    /* Initial Loop Counter Register */
    uint32_t ilcr;
    uint64_t ilcr_lcnt;

    /* Control Transfer Preparation Register (CTPR) */
    E2KCtpr ctprs[3];
    /* Control Transfer Condition */
    target_ulong ct_cond;
    
    uint32_t psr;
    uint32_t upsr;
    uint32_t cuir;
    /* System Register */
    uint64_t osr0;
    uint64_t idr;
    uint64_t core_mode;

    /* Packed Floating Point Flag Register (PFPFR) */
    E2KPfpfr pfpfr;
    /* Floating point state register (FPSR) */
    E2KFpsrState fpsr;
    /* Floating point control register (FPCR) */
    E2KFpcrState fpcr;

    /* OS Compilation Unit Descriptor */
    E2KRwap oscud;
    /* OS Compilation Unit Globals Descriptor */
    E2KRwap osgd;

    /* User Compilation Unit Descriptor */
    E2KRwap cud;
    /* User Compilation Unit Globals Descriptor */
    E2KRwap gd;

    /* Segment Registers */
    E2KRwap cs;
    E2KRwap ds;
    E2KRwap es;
    E2KRwap fs;
    E2KRwap gs;
    E2KRwap ss;

    /* Array Access Unit State */
    E2KAauState aau;

    /* DAM */
    E2KDamEntry dam[32];

    /*
     * Internal use
     */

    /* TODO */
    int interrupt_index;

    /* 32/64-bit scalar/packed float status */
    float_status fp_status;
    /* 80-bit float status */
    float_status fx_status;

    /* Temporary storage for big constant values */
    E2KReg tmp[12];
    /* Temporary storage for pre saved registers */
    E2KReg tmp_saved[6];
    /* Temporary storage for empty and temporary results of alops */
    E2KReg al_result[12];

    /* breakpoint flag */
    uint32_t is_bp;

    /* FIXME: DAM? ld mas=7 + st mas=2 */
    uint64_t last_val0;
    uint64_t last_val1;

    /* Fields up to this point are cleared by a CPU reset */
    struct {} end_reset_fields;

    /* ISA version */
    uint32_t version;
    /* Force alop to preserve the destination register before writing to it.
     * Default: false */
    bool force_save_alc_dst;
    /* Enable tags handling.
     * Default: false */
    bool enable_tags;

    struct e2k_def_t def;

    uint32_t elf_flags;
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

void e2k_cpu_dump_state(CPUState *cs, FILE *f, int flags);
void e2k_cpu_do_interrupt(CPUState *cs);
void e2k_cpu_list(void);
int e2k_cpu_signal_handler(int host_signum, void *pinfo, void *puc);
int e2k_cpu_gdb_read_register(CPUState *cs, GByteArray *mem_buf, int n);
int e2k_cpu_gdb_write_register(CPUState *cs, uint8_t *mem_buf, int n);
void e2k_cpu_register_gdb_regs_for_features(CPUState *cs);
bool e2k_cpu_tlb_fill(CPUState *cpu, vaddr address, int size,
                 MMUAccessType access_type, int mmu_idx,
                 bool probe, uintptr_t retaddr);
void e2k_update_fp_status(CPUE2KState *env);
void e2k_update_fx_status(CPUE2KState *env);
#ifdef CONFIG_USER_ONLY
void e2k_psp_new(E2KPsp *psp, unsigned int size, bool tags);
#endif
void e2k_proc_call(CPUE2KState *env, int base, target_ulong ret_ip,
    bool force_fx);
void e2k_proc_return(CPUE2KState *env, bool force_fx);

#define cpu_signal_handler e2k_cpu_signal_handler
#define cpu_list e2k_cpu_list

typedef CPUE2KState CPUArchState;
typedef E2KCPU ArchCPU;

#include "exec/cpu-all.h"

#endif
