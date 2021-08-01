#include "qemu/osdep.h"
#include "exec/log.h"
#include "exec/translator.h"
#include "tcg/tcg-op.h"
#include "tcg/tcg-op-gvec.h"

//#define FORCE_SAVE_PLU_PREG
//#define FORCE_SAVE_ALC_PREG

#define glue3(a, b, c) glue(glue(a, b), c)
#define glue4(a, b, c, d) glue(glue(a, b), glue(c, d))

#define DYNAMIC -1

#define IS_BASED(i) (((i) & 0x80) == 0)
#define IS_REGULAR(i) (((i) & 0xc0) == 0x80)
#define IS_IMM5(i) (((i) & 0xe0) == 0xc0)
#define IS_IMM4(i) (((i) & 0xf0) == 0xc0)
#define IS_LIT(i) (((i) & 0xf0) == 0xd0)
#define IS_LIT16_LO(i) (((i) & 0x0e) == 0x00)
#define IS_LIT16_HI(i) (((i) & 0x0e) == 0x04)
#define IS_LIT32(i) (((i) & 0x0c) == 0x08)
#define IS_LIT64(i) (((i) & 0x0c) == 0x0c)
#define IS_GLOBAL(i) (((i) & 0xe0) == 0xe0)
#define IS_REG(i) ((i) < 192 || (i) >= 224)
#if 0
/* these registers are safe not to keep the old value %g[0..32] */
# define IS_REG_SAVE_SAFE(i) ((i) >= 224)
#else
/* this could potentially create an RAW conflict for next ops in a bundle */
#define IS_REG_SAVE_SAFE(i) (true)
#endif
#define IS_CTPR(i) (((i) & 0xfc) == 0xd0 && ((i) & 3) != 0)

#define GET_BASED(i) ((i) & 0x7f)
#define GET_REGULAR(i) ((i) & 0x3f)
#define GET_IMM5(i) ((i) & 0x1f)
#define GET_IMM4(i) ((i) & 0x0f)
#define GET_LIT(i) ((i) & 0x03)
#define GET_GLOBAL(i) ((i) & 0x1f)
#define GET_CTPR(i) ((i) & 3)

#define type_name_q ptr
#define type_name_x ptr
#define type_name_d i64
#define type_name_s i32
#define type_name(s) glue(type_name_, s)

#define call(s, name, ...) glue3(name, _, type_name(s))(__VA_ARGS__)

#define tagged(s) glue(Tagged_, type_name(s))
#define tagged_new(s) glue(tagged_new_, type_name(s))()
#define tagged_local_new(s) glue(tagged_local_new_, type_name(s))()
#define tagged_free(s, t) glue(tagged_free_, type_name(s))(t)

#define temp(s) glue(TCGv_, type_name(s))
#define temp_new(s) glue(tcg_temp_new_, type_name(s))()
#define temp_local_new(s) glue(tcg_temp_local_new_, type_name(S))()
#define temp_free(s, t) glue(tcg_temp_free_, type_name(s))(t)

#define gen_tagged_src(i, s, instr, ret) \
    glue4(gen_tagged_src, i, _, s)(instr, ret)

#define gen_tag3q(r, a, b, c) gen_tag3_i128(r.tag, a.tag, b.tag, c.tag)
#define gen_tag3x(r, a, b, c) gen_tag3_i64(r.tag, a.tag, b.tag, c.tag)
#define gen_tag3d(r, a, b, c) gen_tag3_i64(r.tag, a.tag, b.tag, c.tag)
#define gen_tag3s(r, a, b, c) gen_tag3_i32(r.tag, a.tag, b.tag, c.tag)
#define gen_tag3(s, r, a, b, c) glue(gen_tag3, s)(r, a, b, c)

#define gen_tag2q(r, a, b) gen_tag2_i128(r.tag, a.tag, b.tag)
#define gen_tag2x(r, a, b) gen_tag2_i64(r.tag, a.tag, b.tag)
#define gen_tag2d(r, a, b) gen_tag2_i64(r.tag, a.tag, b.tag)
#define gen_tag2s(r, a, b) gen_tag2_i32(r.tag, a.tag, b.tag)
#define gen_tag2(s, r, a, b) glue(gen_tag2, s)(r, a, b)

#define gen_tag1q(r, a) gen_tag1_i128(r.tag, a.tag)
#define gen_tag1x(r, a) gen_tag1_i64(r.tag, a.tag)
#define gen_tag1d(r, a) gen_tag1_i64(r.tag, a.tag)
#define gen_tag1s(r, a) gen_tag1_i32(r.tag, a.tag)
#define gen_tag1(s, r, a) glue(gen_tag1, s)(r, a)

#define gen_result_init_s(i, r)
#define gen_result_init_d(i, r)
#define gen_result_init_x(i, r) gen_result_reg_ptr(i->ctx, r.val, i->als.dst, i->chan)
#define gen_result_init_q(i, r) gen_result_reg_ptr(i->ctx, r.val, i->als.dst, i->chan)
#define gen_result_init(R, i, r) glue(gen_result_init_, R)(i, r)

#define gen_result_init2_s(i, r)
#define gen_result_init2_d(i, r)
#define gen_result_init2_x(i, r) gen_temp_result_ptr(r.val, i->chan)
#define gen_result_init2_q(i, r) gen_temp_result_ptr(r.val, i->chan)
#define gen_result_init2(R, i, r) glue(gen_result_init2_, R)(i, r)

#define gen_al_result(R, instr, r) glue(gen_al_result_, R)(instr, r)

#define gen_extrl_i32 tcg_gen_mov_i32
#define gen_extrl_i64 tcg_gen_extrl_i64_i32

typedef enum {
    ALES_NONE = 0x00,
    ALES_PRESENT = 0x01,
    ALES_ALLOCATED = 0x02,
} AlesFlag;

static TCGv cpu_pc;
static TCGv cpu_npc;
static TCGv_i64 cpu_ctprs[3];
static TCGv_i32 cpu_ct_cond;
static TCGv_i32 cpu_is_bp; /* breakpoint flag */
static TCGv_i32 cpu_wdbl;
static TCGv_i32 cpu_wd_base; /* holds wbs * 2 */
static TCGv_i32 cpu_wd_size; /* holds wsz * 2 */
static TCGv_i32 cpu_boff; /* holds rbs * 2 */
static TCGv_i32 cpu_bsize; /* holds rsz * 2 + 2 */
static TCGv_i32 cpu_bcur; /* holds rcur * 2 */
static TCGv_i64 cpu_pregs;
static TCGv_i32 cpu_psize; /* holds psz */
static TCGv_i32 cpu_pcur; /* holds pcur */
static TCGv_i64 cpu_last_val0;
static TCGv_i64 cpu_last_val1;
/* lsr */
static TCGv_i64 cpu_lsr_lcnt;
static TCGv_i32 cpu_lsr_ecnt;
static TCGv_i32 cpu_lsr_vlc;
static TCGv_i32 cpu_lsr_over;
static TCGv_i32 cpu_lsr_pcnt;
static TCGv_i32 cpu_lsr_strmd;
/* AAU */
static TCGv_i32 cpu_aasti[16];
static TCGv_i32 cpu_aasti_tags;
static TCGv_i32 cpu_aaind[16];
static TCGv_i32 cpu_aaind_tags;
static TCGv_i32 cpu_aaincr[8];
static TCGv_i32 cpu_aaincr_tags;
static TCGv_i64 cpu_aad_lo[32];
static TCGv_i64 cpu_aad_hi[32];

typedef struct {
  uint32_t hs;
  uint32_t ss;
  uint32_t als[6];
  uint32_t cs0;
  uint16_t ales[6];
  uint32_t cs1;
  uint16_t aas[6];
  uint32_t lts[4];
  uint32_t pls[3];
  uint32_t cds[3];

  bool ss_present;
  bool als_present[6];
  bool cs0_present;
  AlesFlag ales_present[6];
  bool cs1_present;
  bool aas_present[6];
  bool lts_present[4];
  bool pls_present[3];
  bool cds_present[3];
} UnpackedBundle;

typedef struct {
    int32_t sdisp;   /* CS0 28:0 */
} Cs0IBranch, Cs0Puttsd;

typedef struct {
    int32_t sdisp;   /* CS0 28:0 */
    uint8_t ipd;    /* SS 31:30 */
    uint8_t ctpr;   /* CS0 31:30 */
    uint8_t opc;
} Cs0Disp;

typedef struct {
    uint32_t disp;  /* CS0 28:0 */
    uint8_t ipd;    /* SS 31:30 */
    uint8_t ctpr;   /* CS0 31:30 */
} Cs0SDisp;

typedef struct {
    uint8_t ipd;
} Cs0Return;

typedef struct {
    uint32_t disp;  /* 28:4 */
    uint8_t prefr;  /* 2:0 */
    uint8_t ipd;    /* 3 */
} Cs0Pref;

typedef enum {
    CS0_NONE,
    CS0_IBRANCH,
    CS0_PREF,
    CS0_PUTTSD,
    CS0_DONE,
    CS0_HRET,
    CS0_GLAUNCH,
    CS0_DISP,
    CS0_SDISP,
    CS0_GETTSD,
    CS0_RETURN,
} Cs0Type;

typedef struct {
    Cs0Type type;
    union {
        Cs0IBranch ibranch;
        Cs0Puttsd puttsd;
        Cs0Disp disp;
        Cs0SDisp sdisp;
        Cs0Pref pref;
        Cs0Return ret;
    };
} Cs0;

typedef enum {
    SETR_VFRPSZ = 0x01,
    SETR_WD = 0x02,
    SETR_BN = 0x04,
    SETR_BP = 0x08,
} SetrType;

typedef struct {
    SetrType type;
    uint8_t rpsz;
    uint8_t wsz;
    bool nfx;
    bool dbl;
    uint8_t rbs;
    uint8_t rsz;
    uint8_t rcur;
    uint8_t psz;
} Cs1Setr;

typedef struct {
    bool ma_c;
    bool fl_c;
    bool ld_c;
    bool st_c;
    bool all_e;
    bool all_c;
    /* v2+ */
    bool trap;
    /* v5+ */
    bool sal;
    bool sas;
} Cs1Wait;

typedef struct {
    uint8_t wbs;
    uint8_t disp;
} Cs1HCall;

typedef struct {
    bool flushr;
    bool flushc;
} Cs1Flush;

typedef struct {
    bool chkm4;
    uint8_t dmask;
    uint8_t umask;
} Cs1Vfbg;

typedef enum {
    CS1_NONE,
    CS1_SETR,
    CS1_SETEI,
    CS1_SETSFT,
    CS1_WAIT,
    CS1_CALL,
    CS1_HCALL,
    CS1_MAS,
    CS1_FLUSH,
    CS1_VFBG,
} Cs1Type;

typedef struct {
    Cs1Type type;
    union {
        Cs1Setr setr;
        uint8_t ei;
        Cs1Wait wait;
        uint8_t call_wbs;
        Cs1HCall hcall;
        uint8_t mas[6];
        Cs1Flush flush;
        Cs1Vfbg vfbg;
    };
} Cs1;

typedef enum {
    ALOPF_NONE,
    ALOPF1,
    ALOPF1_MAS,
    ALOPF1_MERGE,
    ALOPF2,
    ALOPF3,
    ALOPF7,
    ALOPF8,
    ALOPF10,
    ALOPF11,
    ALOPF11_MAS,
    ALOPF11_MERGE,
    ALOPF11_LIT8,
    ALOPF12,
    ALOPF12_PSHUFH,
    ALOPF12_IBRANCHD,
    ALOPF12_ICALLD,
    ALOPF13,
    ALOPF15,
    ALOPF16,
    ALOPF17,
    ALOPF21,
    ALOPF21_ICOMB,
    ALOPF21_FCOMB,
    ALOPF21_PFCOMB,
    ALOPF21_QPFCOMB,
    ALOPF22,
} Alopf;

#include "alops.inc"

typedef struct {
    TCGv_i32 tag;
    TCGv_ptr val;
} Tagged_ptr;

typedef struct {
    TCGv_i32 tag;
    TCGv_i32 val;
} Tagged_i32;

typedef struct {
    TCGv_i32 tag;
    TCGv_i64 val;
} Tagged_i64;

typedef enum {
    TAGGED_S,
    TAGGED_D,
    TAGGED_X,
    TAGGED_Q,
} TaggedKind;

typedef struct {
    TaggedKind kind;
    union {
        Tagged_i32 t32;
        Tagged_i64 t64;
        Tagged_ptr tptr;
    };
} Tagged;

typedef struct DisasContext DisasContext;

typedef struct {
    DisasContext *ctx;
    Alopf format;
    AlopArgs args;
    uint32_t op;
    const char *name;
    int chan;
    uint8_t mas;
    union {
        struct {
            uint32_t src4: 8;
            uint32_t src2: 8;
            uint32_t src1: 8;
            uint32_t opc1: 7;
            uint32_t sm: 1;
        };
        struct {
            uint32_t dst: 8;
            uint32_t opce2: 8;
            uint32_t opce1: 8;
            uint32_t unused1: 8;
        };
        struct {
            uint32_t dst_preg: 5;
            uint32_t opc_cmp: 3;
            uint32_t unused2: 24;
        };
        /* staa/ldaa/aaurw/aaurr */
        struct {
            uint32_t unused3: 8;
            uint32_t aalit: 2;
            uint32_t aaopc: 2;
            uint32_t aaincr: 3;
            /* aaind/aasti/aaincr for aaurw/aaurr */
            uint32_t aaind: 4;
            uint32_t aad: 5;
            uint32_t unused4: 8;
        };
        uint32_t raw;
    } als;
    union {
        struct {
            uint16_t src3: 8;
            uint16_t opc2: 8;
        };
        struct {
            uint16_t opce3: 8;
            uint16_t unused5: 8;
        };
        uint16_t raw;
    } ales;
} Alop;

typedef struct {
    int reg; // -1 means do not write
    TCGv_i32 value;
} PlResult;

typedef enum {
    CT_NONE,
    CT_IBRANCH,
    CT_JUMP,
    CT_CALL,
} ControlTransferType;

typedef struct {
    ControlTransferType type;
    union {
        target_ulong target;
        TCGv_i64 ctpr;
    } u;
    int wbs;
    uint8_t cond_type;
    uint8_t psrc;
} ControlTransfer;

typedef enum {
    /* mandatory group */

    /* Cache(s) flush operations */
    MAS_OPC_CACHE_FLUSH = 0,
    /* Data cache(s) line flush operations */
    MAS_OPC_DCACHE_LINE_FLUSH = 1,
    /* Instruction cache(s) line flush operations */
    MAS_OPC_ICACHE_LINE_FLUSH = 2,
    /* TLB page flush operations */
    MAS_OPC_TLB_PAGE_FLUSH = 2,
    MAS_OPC_ST_UNKNOWN3 = 3,
    /* Instruction cache(s) flush operations */
    MAS_OPC_ICACHE_FLUSH = 4,
    /* TLB flush operations */
    MAS_OPC_TLB_FLUSH = 4,
    /* TLB address probe operations */
    MAS_OPC_TLB_ADDR_PROBE = 5,
    /* TLB entry probe operations */
    MAS_OPC_TLB_ENTRY_PROBE = 6,
    /* AAU registers access */
    MAS_OPC_AAU_REG = 7,

    /* optional group */

    /* MMU registers access */
    MAS_OPC_MMU_REG = 8,
    /* DTLB registers access */
    MAS_OPC_DTLB_REG = 9,
    /* L1 cache registers access */
    MAS_OPC_L1_REG = 10,
    /* L2 cache registers access */
    MAS_OPC_L2_REG = 11,
    /* ICACHE registers access */
    MAS_OPC_ICACHE_REG = 12,
    /* ITLB registers access */
    MAS_OPC_ITLB_REG = 12,
    /* DAM register(s) access */
    MAS_OPC_DAM_REG = 13,
    /* MLT register(s) access */
    MAS_OPC_MLT_REG = 13,
    /* CLW register(s) access */
    MAS_OPC_CLW_REG = 13,
    /* SNOOP register(s) access */
    MAS_OPC_SNOOP_REG = 13,
    /* PCS (Procedure Chain Stack) registers operations */
    MAS_OPC_PCS_REG = 14,
    MAS_OPC_RESERVED2 = 15,
} MasSpecialOpc;

typedef struct {
    Tagged tagged;
    uint8_t dst;
    uint8_t chan;
} SavedReg;

typedef struct {
    int index;
    TCGv_i32 val;
} SavedPreg;

typedef struct DisasContext {
    DisasContextBase base;
    UnpackedBundle bundle;
    Cs0 cs0;
    Cs1 cs1;
    Alop alops[6];
    target_ulong pc;
    int mmuidx;
    bool loop_mode;

    /* optional, can be NULL */
    TCGv_i32 mlock;
    TCGv_i32 loop_end;
    TCGv_i32 delayed_illop;

    int version;
    bool enable_tags;
    bool force_save_alc_dst;
    /* Force ILLOP for bad instruction format for cases where real CPU
       do not generate it. */
    bool strict;

    // Temporary values.
    TCGv_i32 t32[64];
    TCGv_i64 t64[32];
    TCGv_ptr tptr[32];
    // Allocated temporary values count.
    int t32_len;
    int t64_len;
    int tptr_len;

    /* Delayed window bounds check */
    int wd_size;
    int max_r;
    int bsize;
    int max_b;

    SavedReg saved_reg[6];
    SavedPreg saved_preg[8];
    int saved_reg_len;
    int saved_preg_len;

    int aau_am[4];
    ControlTransfer ct;
} DisasContext;

typedef struct {
    int chan;
    union {
        struct {
            uint16_t am: 1;
            uint16_t ind: 5;
            uint16_t area: 6;
            uint16_t opc: 3;
            uint16_t be: 1;
        };
        uint16_t aas;
    };
    uint8_t dst;
} Mova;

static int16_t alops_map[4][128][6];

static inline void gen_save_pc(target_ulong pc)
{
    tcg_gen_movi_tl(cpu_pc, pc);
}

static inline void gen_save_cpu_state(DisasContext *ctx)
{
    gen_save_pc(ctx->pc);
}

static inline void gen_tr_exception(DisasContext *ctx, int exception_index)
{
    TCGv_i32 t0 = tcg_const_i32(exception_index);

    ctx->base.is_jmp = DISAS_NORETURN;
    gen_save_cpu_state(ctx);
    gen_helper_raise_exception(cpu_env, t0);
    tcg_temp_free_i32(t0);
}

#define IMPL_GEN_TR_EXCP(name, excp) \
    static inline void name(DisasContext *ctx) \
    { \
        gen_tr_exception(ctx, excp); \
    }

IMPL_GEN_TR_EXCP(gen_tr_excp_illopc, EXCP_ILLEGAL_OPCODE)
IMPL_GEN_TR_EXCP(gen_tr_excp_illopn, EXCP_ILLEGAL_OPERAND)
IMPL_GEN_TR_EXCP(gen_tr_excp_window_bounds, EXCP_WINDOW_BOUNDS)
#ifndef TARGET_E2K32
IMPL_GEN_TR_EXCP(gen_tr_excp_array_bounds, EXCP_ARRAY_BOUNDS)
#endif

static inline void gen_exception(int excp)
{
    TCGv_i32 t0 = tcg_const_i32(excp);

    // TODO: check if need to save state
    gen_helper_raise_exception(cpu_env, t0);
    tcg_temp_free_i32(t0);
}

#define IMPL_GEN_EXCP(name, excp) \
    static inline void name(void) \
    { \
        gen_exception(excp); \
    }

IMPL_GEN_EXCP(gen_excp_illopc, EXCP_ILLEGAL_OPCODE)
IMPL_GEN_EXCP(gen_excp_window_bounds, EXCP_WINDOW_BOUNDS)

#define e2k_todo(ctx, fmt, ...) \
    qemu_log(TARGET_FMT_lx ": " fmt " (%s:%d)\n", ctx->pc, \
        ## __VA_ARGS__, __FILE__, __LINE__)

#define e2k_todo_illop(ctx, fmt, ...) \
    e2k_todo(ctx, fmt, ## __VA_ARGS__); \
    gen_tr_excp_illopc(ctx)

static inline TCGv_i32 e2k_get_temp_i32(DisasContext *ctx)
{
    assert(ctx->t32_len < ARRAY_SIZE(ctx->t32));
    return ctx->t32[ctx->t32_len++] = tcg_temp_local_new_i32();
}

static inline TCGv_i64 e2k_get_temp_i64(DisasContext *ctx)
{
    assert(ctx->t64_len < ARRAY_SIZE(ctx->t64));
    return ctx->t64[ctx->t64_len++] = tcg_temp_local_new_i64();
}

static inline TCGv_ptr e2k_get_temp_ptr(DisasContext *ctx)
{
    assert(ctx->tptr_len < ARRAY_SIZE(ctx->tptr));
    return ctx->tptr[ctx->tptr_len++] = tcg_temp_local_new_ptr();
}

#define IMPL_TAGGED_FNS(Self, S) \
    static inline Self glue(tagged_new_, S)(void) \
    { \
        Self r; \
        r.tag = tcg_temp_new_i32(); \
        r.val = glue(tcg_temp_new_, S)(); \
        return r; \
    } \
    \
    static inline Self glue(tagged_local_new_, S)(void) \
    { \
        Self r; \
        r.tag = tcg_temp_local_new_i32(); \
        r.val = glue(tcg_temp_local_new_, S)(); \
        return r; \
    } \
    \
    static inline void glue(tagged_free_, S)(Self t) \
    { \
        tcg_temp_free_i32(t.tag); \
        glue(tcg_temp_free_, S)(t.val); \
    } \
    static inline Self glue(e2k_get_tagged_temp_, S)(DisasContext *ctx) \
    { \
        Self r; \
        r.tag = e2k_get_temp_i32(ctx); \
        r.val = glue(e2k_get_temp_, S)(ctx); \
        return r; \
    }


IMPL_TAGGED_FNS(Tagged_i32, i32)
IMPL_TAGGED_FNS(Tagged_i64, i64)
IMPL_TAGGED_FNS(Tagged_ptr, ptr)

static inline uint64_t ctpr_new(uint8_t tag, uint8_t opc, uint8_t ipd,
    target_ulong base)
{
    uint64_t ctpr = 0;
    ctpr = deposit64(ctpr, CTPR_BASE_OFF, CTPR_BASE_LEN, base);
    ctpr = deposit64(ctpr, CTPR_TAG_OFF, CTPR_TAG_LEN, tag);
    ctpr = deposit64(ctpr, CTPR_IPD_OFF, CTPR_IPD_LEN, ipd);
    ctpr = deposit64(ctpr, CTPR_OPC_OFF, CTPR_OPC_LEN, opc);
    return ctpr;
}

static inline uint64_t ctpr_new_disp(DisasContext *ctx, Cs0Disp *disp)
{
    target_ulong base = ctx->pc + disp->sdisp;
    return ctpr_new(CTPR_TAG_DISP, disp->opc, disp->ipd, base);
}

static void gen_goto_tb(DisasContext *ctx, int tb_num, target_ulong pc)
{
    if (translator_use_goto_tb(&ctx->base, pc))  {
        /* jump to same page: we can use a direct jump */
        tcg_gen_goto_tb(tb_num);
        tcg_gen_movi_tl(cpu_pc, pc);
        tcg_gen_exit_tb(ctx->base.tb, tb_num);
    } else {
        /* jump to another page: currently not optimized */
        tcg_gen_movi_tl(cpu_pc, pc);
        tcg_gen_exit_tb(NULL, 0);
    }
}

/* returns zero if bundle is invalid */
static size_t unpack_bundle(CPUE2KState *env,
    target_ulong pc, UnpackedBundle *bundle)
{
    unsigned int gap;
    unsigned int pos = 0;
    unsigned int mdl;
    unsigned int hsyll_cntr = 0;
    unsigned int i;
    uint32_t hs;

    memset(bundle, 0, sizeof(UnpackedBundle));

    bundle->hs = hs = translator_ldl(env, pc + pos);
    pos += 4;

    /* Check for SS.  */
    if (GET_BIT(hs, 12)) {
        bundle->ss_present = true;
        bundle->ss = translator_ldl(env, pc + pos);
        pos += 4;
    }

    /* Check for available ALS syllables.  */
    for (i = 0; i < 6; i++) {
        if (GET_BIT(hs, 26 + i)) {
            bundle->als_present[i] = true;
            bundle->als[i] = translator_ldl(env, pc + pos);
            pos += 4;
        }
    }

    /* Check for CS0.  */
    if (GET_BIT(hs, 14)) {
        bundle->cs0_present = true;
        bundle->cs0 = translator_ldl(env, pc + pos);
        pos += 4;
    }

    if (GET_BIT(hs, 25)) {
        bundle->ales_present[5] = ALES_PRESENT;
        bundle->ales[5] = 0x01c0;
    }

    if (GET_BIT(hs, 22)) {
        bundle->ales_present[2] = ALES_PRESENT;
        bundle->ales[2] = 0x01c0;
    }

    /* Calculate the size of f1 fragment in bytes. For a valid bundle it
       should be equal to either of `pos', `pos + 4' or `pos + 8'. What should I
       do if it's not?  */
    /* TODO: exception */
    mdl = ((hs & 0xf) + 1) * 4;

    /* The following condition means that ALES{2,5} are physically present within
       the wide instruction. However, they should be probably taken into account
       only if HS.ale{2,5} are set. Should I disassemble them if these bits are
       not set but the syllables physically exist?  */
    if ((GET_BIT(hs, 15) && mdl == pos + 8) ||
        (!GET_BIT(hs, 15) && mdl == pos + 4)) {
        /* Fill in ALES5 and ALES2 syllables even if none of them is specified in
           HS as present. This will let me output this syllable into disassembly
           whichever case takes place. */
        bundle->ales[5] = translator_lduw(env, pc + pos);
        bundle->ales[2] = translator_lduw(env, pc + pos + 2);

        /* Adjust `ALES_PRESENT[{5,2}]' as proposed above now that we know that
           they are allocated.  */
        bundle->ales_present[5] |= ALES_ALLOCATED;
        bundle->ales_present[2] |= ALES_ALLOCATED;

        pos += 4;
    }

    /* Check for CS1.  */
    if (GET_BIT(hs, 15)) {
        bundle->cs1_present = 1;
        bundle->cs1 = translator_ldl(env, pc + pos);
        pos += 4;
    }

    /* A primitive control just for a moment.  */
    if (mdl != pos) {
        /* This is either an APB instruction or an invalid one. Let's stupidly
           believe that the former takes place and signalize our caller about
           that by returning 0.  */
        return 0;
    }

    /* Check for ALES{0,1,3,4}.  */
    for (i = 0; i < 5; i++) {
        if (i == 2)
            continue;

        if (GET_BIT(hs, 20 + i)) {
            unsigned int offset = 2 * ((hsyll_cntr & ~0x1) + 1 - (hsyll_cntr & 0x1));
            bundle->ales_present[i] = ALES_PRESENT;

            /* Recall the idiotic order of half-syllables in the packed wide
               instruction.  */
            bundle->ales[i] = translator_lduw(env, pc + pos + offset);
            hsyll_cntr++;
        }
    }

    /* Check for AASj half-syllables. To encode them SS syllable of SF1 type
       should be present.  */
    if (bundle->ss_present && !GET_BIT(bundle->ss, 20)) {
        for (i = 0; i < 4; i++) {
            if (GET_BIT(bundle->ss, 12 + i)) {
                bundle->aas_present[i >> 1] = true;
                bundle->aas_present[i + 2] = true;
            }
        }

        for (i = 0; i < 6; i++) {
            if (bundle->aas_present[i]) {
                unsigned int offset = 2 * ((hsyll_cntr & ~0x1) + 1 - (hsyll_cntr & 0x1));
                /* Recall the idiotic order of half-syllables in the packed wide
                   instruction. Note that the first AAS half-syllable may share a
                   syllable with the last ALES.  */
                bundle->aas[i] = translator_lduw(env, pc + pos + offset);
                hsyll_cntr++;
            }
        }
    }

    /* align half-syllables */
    hsyll_cntr += hsyll_cntr & 1;

    /* Calculate the next 32-bit syllable's position. It may be the uppermost LTS
       syllable. Note that I don't consider the case when LTS syllables reuse the
       values encoded in the preceding ones, though according to `iset-v5.single'
       this is quite legal. GAS doesn't produce such a code. Hopefully neither LAS
       has ever done that . . .  */
    gap = pos + 2 * hsyll_cntr;

    /* Set POS to point to the last syllable in the current wide instruction and
       extract CDSj and PLSj syllables if any.  */
    pos = ((extract32(hs, 4, 3) + 1) << 3) - 4;

    /* Check for CDSj syllables.  */
    for (i = 0; i < extract32(hs, 16, 2); i++) {
        bundle->cds_present[i] = true;
        bundle->cds[i] = translator_ldl(env, pc + pos);
        pos -= 4;
    }

    /* Check for PLSj syllables.  */
    for (i = 0; i < extract32(hs, 18, 2); i++) {
        bundle->pls_present[i] = true;
        bundle->pls[i] = translator_ldl(env, pc + pos);
        pos -= 4;
    }

    /* Now POS should point to the lowermost LTS0 syllable if any. If there are
       no LTSj syllables in this instruction, POS should point to the last
       syllable consisting of half-syllables.

       If neither of these conditions holds true, believe that it's not a valid
       synchronous instruction by analogy with the middle point test above.
       Engineers are said to customize instructions with references to missing
       literal syllables occasionally, but the lack of space for more substantial
       syllables should not be allowed for.  */
    if (pos < gap && pos != gap - 4) {
        return 0;
    }

    /* Extract available LTSj syllables.  */
    for (i = 0; i < 4 && pos >= gap; i++) {
        bundle->lts_present[i] = true;
        bundle->lts[i] = translator_ldl(env, pc + pos);
        pos -= 4;
    }

    return 8 + extract32(hs, 4, 3) * 8;
}

static inline uint8_t ss_extract_ipd(const UnpackedBundle *raw)
{
    return raw->ss_present ? extract32(raw->ss, 30, 2) : 3;
}

static inline int32_t cs0_extract_sdisp(const UnpackedBundle *raw)
{
    return sextract32(raw->cs0, 0, 28) << 3;
}

static inline void decode_cs1(DisasContext *ctx, const UnpackedBundle *raw)
{
    enum {
        SETR0,
        SETR1,
        SETEI,
        WAIT,
        SETBR,
        CALL,
        MAS_OPC,
        FLUSHR,
        BG
    };

    Cs1 *ret = &ctx->cs1;
    uint32_t cs1 = raw->cs1;
    int opc = extract32(cs1, 28, 4);

    ret->type = CS1_NONE;
    if (!raw->cs1_present) {
        return;
    }

    if (opc == SETR0 || opc == SETR1 || opc == SETBR) {
        Cs1Setr *setr = &ret->setr;
        setr->type = 0;
        if (opc == SETR0 || opc == SETR1) {
            uint32_t lts0 = raw->lts[0];
            if (!raw->lts_present[0]) {
                gen_tr_excp_illopc(ctx);
                return;
            }
            setr->type |= opc == SETR1 ? SETR_VFRPSZ : 0;
            setr->type |= SETR_WD;
            setr->wsz = extract32(lts0, 5, 7);
            setr->nfx = extract32(lts0, 4, 1);
            if (ctx->version >= 3) {
                setr->dbl = extract32(lts0, 3, 1);
            }
            if (setr->wsz > 112) {
                gen_tr_excp_illopc(ctx);
            }
        }
        if (extract32(cs1, 26, 1)) {
            setr->type |= SETR_BN;
            setr->rbs = extract32(cs1, BR_RBS_OFF, BR_RBS_LEN);
            setr->rsz = extract32(cs1, BR_RSZ_OFF, BR_RSZ_LEN);
            setr->rcur = extract32(cs1, BR_RCUR_OFF, BR_RCUR_LEN);
        }
        if (extract32(cs1, 27, 1)) {
            setr->type |= SETR_BP;
            setr->psz = extract32(cs1, BR_PSZ_OFF, BR_PSZ_LEN);
        }
        ret->type = CS1_SETR;
    } else if (opc == SETEI) {
        if (extract32(cs1, 27, 1)) {
            if (ctx->version < 2) {
                gen_tr_excp_illopc(ctx);
                return;
            }
            ret->type = CS1_SETSFT;
        } else {
            ret->type = CS1_SETEI;
            ret->ei = extract32(cs1, 0, 8);
        }
    } else if (opc == WAIT) {
        ret->type = CS1_WAIT;
        ret->wait.all_c = extract32(cs1, 0, 1);
        ret->wait.all_e = extract32(cs1, 1, 1);
        ret->wait.st_c = extract32(cs1, 2, 1);
        ret->wait.ld_c = extract32(cs1, 3, 1);
        ret->wait.fl_c = extract32(cs1, 4, 1);
        ret->wait.ma_c = extract32(cs1, 5, 1);

        if (ctx->version >= 2) {
            ret->wait.trap = extract32(cs1, 6, 1);
        }

        if (ctx->version >= 5) {
            if (ret->wait.st_c) {
                ret->wait.sas = extract32(cs1, 7, 1);
            }
            if (ret->wait.ld_c) {
                ret->wait.sal = extract32(cs1, 8, 1);
            }
        }
    } else if (opc == CALL) {
        int ctop = extract32(raw->ss, 10, 2);
        int wbs = extract32(cs1, 0, 7);
        if (ctop) {
            ret->type = CS1_CALL;
            ret->call_wbs = wbs;
        } else {
            int cs1_ctopc = extract32(cs1, 7, 3);
            int cs0_opc = extract32(raw->cs0, 28, 4);
            int disp = extract32(raw->cs0, 1, 5);
            if (cs1_ctopc != 2 || cs0_opc != 0 || !raw->cs0_present) {
                gen_tr_excp_illopc(ctx);
                return;
            }
            ret->type = CS1_HCALL;
            ret->hcall.disp = disp;
            ret->hcall.wbs = wbs;
        }
    } else if (opc == MAS_OPC) {
        ret->type = CS1_MAS;
        ret->mas[0] = extract32(cs1, 21, 7);
        ret->mas[2] = extract32(cs1, 14, 7);
        ret->mas[3] = extract32(cs1, 7, 7);
        ret->mas[5] = extract32(cs1, 0, 7);
    } else if (opc == FLUSHR) {
        ret->type = CS1_FLUSH;
        ret->flush.flushr = (cs1 & 1) != 0;
        ret->flush.flushc = (cs1 & 2) != 0;
    } else if (opc == BG) {
        ret->type = CS1_VFBG;
        ret->vfbg.umask = extract32(cs1, 0, 8);
        ret->vfbg.dmask = extract32(cs1, 8, 8);
        ret->vfbg.chkm4 = extract32(cs1, 16, 1);
    } else {
        gen_tr_excp_illopc(ctx);
    }
}

static inline void decode_cs0(DisasContext *ctx, const UnpackedBundle *raw)
{
    Cs0 *ret = &ctx->cs0;
    uint32_t cs0 = raw->cs0;
    int ctpr = extract32(cs0, 30, 2);
    int ctp_opc = extract32(cs0, 28, 2);
    int param_type = extract32(cs0, 0, 3);

    if (!raw->cs0_present) {
        ret->type = CS0_NONE;
        return;
    } else if (ctpr > 0) {
        switch(ctp_opc) {
        case 0: // disp
        case 1: // ldisp
            if (ctp_opc == 1 && ctpr != 2) {
                gen_tr_excp_illopc(ctx);
                return;
            }
            ret->type = CS0_DISP;
            ret->disp.opc = ctp_opc;
            ret->disp.ctpr = ctpr;
            ret->disp.sdisp = cs0_extract_sdisp(raw);
            ret->disp.ipd = ss_extract_ipd(raw);
            break;
        case 2:
            ret->type = CS0_SDISP;
            ret->sdisp.ctpr = ctpr;
            ret->sdisp.disp = extract32(cs0, 0, 28);
            ret->sdisp.ipd = ss_extract_ipd(raw);
            break;
        case 3:
            if (param_type == 0 && ctpr == 3) {
                ret->type = CS0_RETURN;
            } else if (param_type == 1) {
                ret->type = CS0_GETTSD;
            }
            break;
        }
    } else {
        switch(ctp_opc) {
        case 0:
            ret->type = CS0_IBRANCH;
            ret->ibranch.sdisp = cs0_extract_sdisp(raw);
            break;
        case 1:
            ret->type = CS0_PREF;
            ret->pref.prefr = extract32(cs0, 0, 2);
            ret->pref.ipd = extract32(cs0, 3, 1);
            ret->pref.disp = extract32(cs0, 4, 24);
            break;
        case 2:
            ret->type = CS0_PUTTSD;
            break;
        case 3:
            if (param_type == 0) {
                ret->type = CS0_DONE;
            } else if (param_type == 3) {
                ret->type = CS0_HRET;
            } else if (param_type == 4) {
                ret->type = CS0_GLAUNCH;
            }
            break;
        }
    }

    switch(ret->type) {
    case CS0_NONE:
        gen_tr_excp_illopc(ctx);
        break;
    case CS0_IBRANCH:
    case CS0_DONE:
    case CS0_HRET:
    case CS0_GLAUNCH:
        if (!raw->ss_present
            || extract32(raw->ss, 10, 2) != 0
            || extract32(raw->ss, 0, 9) == 0
            || ctx->cs1.type == CS1_CALL
            || ctx->cs1.type == CS1_HCALL)
        {
            ret->type = CS0_NONE;
            gen_tr_excp_illopc(ctx);
        }
        break;
    default:
        break;
    }
}

static inline void decode_ct_cond(DisasContext *ctx, const UnpackedBundle *raw)
{
    ctx->ct.type = CT_NONE;
    ctx->ct.cond_type = 0;
    int ctpr = extract32(raw->ss, 10, 2);
    if (ctpr != 0) {
        if (ctx->ct.type == CT_NONE) {
            ctx->ct.type = CT_JUMP;
        }
        ctx->ct.u.ctpr = cpu_ctprs[ctpr - 1];
    }
    ctx->ct.psrc = extract32(raw->ss, 0, 5);
    ctx->ct.cond_type = extract32(raw->ss, 5, 4);
}

static inline void gen_lcntex(TCGv_i32 ret)
{
    TCGv_i64 t0 = tcg_temp_new_i64();

    tcg_gen_setcondi_i64(TCG_COND_EQ, t0, cpu_lsr_lcnt, 0);
    tcg_gen_extrl_i64_i32(ret, t0);

    tcg_temp_free_i64(t0);
}

static inline bool is_chan_03(int c)
{
    return c == 0 || c == 3;
}

static inline bool is_chan_14(int c)
{
    return c == 1 || c == 4;
}

static inline bool is_chan_25(int c)
{
    return c == 2 || c == 5;
}

static inline bool is_chan_0134(int c)
{
    return is_chan_03(c) || is_chan_14(c);
}

static void gen_ptr_from_index(TCGv_ptr ret, TCGv_ptr ptr, TCGv_i32 idx,
    int size)
{
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_ptr t1 = tcg_temp_new_ptr();

    tcg_gen_muli_i32(t0, idx, size);
    tcg_gen_ext_i32_ptr(t1, t0);
    tcg_gen_add_ptr(ret, ptr, t1);

    tcg_temp_free_ptr(t1);
    tcg_temp_free_i32(t0);
}

static void gen_wrap_i32(TCGv_i32 ret, TCGv_i32 value, TCGv_i32 size)
{
    TCGv_i32 t0 = tcg_temp_new_i32();

    tcg_gen_sub_i32(t0, value, size);
    tcg_gen_movcond_i32(TCG_COND_LEU, ret, size, value, t0, value);

    tcg_temp_free_i32(t0);
}

static void gen_preg_index(TCGv_i32 ret, int index)
{
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i32 t1 = tcg_temp_new_i32();
    TCGv_i32 t2 = tcg_const_i32(index);

    tcg_gen_addi_i32(t0, cpu_pcur, index);
    gen_wrap_i32(t1, t0, cpu_psize);
    tcg_gen_movcond_i32(TCG_COND_LTU, ret, t2, cpu_psize, t1, t2);

    tcg_temp_free_i32(t2);
    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
}

static void gen_preg_offset(TCGv_i64 ret, int idx)
{
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i32 t1 = tcg_temp_new_i32();

    gen_preg_index(t0, idx);
    tcg_gen_shli_i32(t1, t0, 1);
    tcg_gen_extu_i32_i64(ret, t1);

    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
}

static void gen_preg_raw_i64(TCGv_i64 ret, int reg)
{
    TCGv_i64 one = tcg_const_i64(1);
    TCGv_i64 t0 = tcg_temp_new_i64();
    TCGv_i64 t1 = tcg_temp_new_i64();

    gen_preg_offset(t0, reg);
    tcg_gen_shr_i64(t1, cpu_pregs, t0);
    tcg_gen_andi_i64(ret, t1, 1);

    tcg_temp_free_i64(t1);
    tcg_temp_free_i64(t0);
    tcg_temp_free_i64(one);
}

static void gen_preg_raw_i32(TCGv_i32 ret, int reg)
{
    TCGv_i64 t0 = tcg_temp_new_i64();

    gen_preg_raw_i64(t0, reg);
    tcg_gen_extrl_i64_i32(ret, t0);
    tcg_temp_free_i64(t0);
}

static bool gen_saved_preg_i32(DisasContext *ctx, TCGv_i32 ret, int index)
{
    int i;

    for (i = 0; i < ctx->saved_preg_len; i++) {
        if (ctx->saved_preg[i].index == index) {
            tcg_gen_mov_i32(ret, ctx->saved_preg[i].val);
            return true;
        }
    }

    return false;
}

static void gen_preg_i32(DisasContext *ctx, TCGv_i32 ret, int index)
{
    if (!gen_saved_preg_i32(ctx, ret, index)) {
        gen_preg_raw_i32(ret, index);
    }
}

static void gen_preg_set_i32(int index, TCGv_i32 val)
{
    TCGv_i64 t0 = tcg_temp_new_i64();
    TCGv_i64 t1 = tcg_temp_new_i64();
    TCGv_i64 t2 = tcg_temp_new_i64();
    TCGv_i64 t3 = tcg_temp_new_i64();
    TCGv_i64 t4 = tcg_temp_new_i64();

    gen_preg_offset(t0, index);
    tcg_gen_rotr_i64(t1, cpu_pregs, t0);
    tcg_gen_andi_i64(t2, t1, ~3);
    tcg_gen_extu_i32_i64(t3, val);
    tcg_gen_or_i64(t4, t2, t3);
    tcg_gen_rotl_i64(cpu_pregs, t4, t0);

    tcg_temp_free_i64(t4);
    tcg_temp_free_i64(t3);
    tcg_temp_free_i64(t2);
    tcg_temp_free_i64(t1);
    tcg_temp_free_i64(t0);
}

static void gen_reg_tag_ptr(TCGv_ptr ret, TCGv_i32 idx)
{
    TCGv_ptr t0 = tcg_temp_new_ptr();
    TCGv_ptr t1 = tcg_temp_new_ptr();

    tcg_gen_ext_i32_ptr(t0, idx);
    tcg_gen_addi_ptr(t1, cpu_env, offsetof(CPUE2KState, tags));
    tcg_gen_add_ptr(ret, t1, t0);

    tcg_temp_free_ptr(t1);
    tcg_temp_free_ptr(t0);
}

static inline bool is_tag_mask_all(DisasContext *ctx, uint8_t mask)
{
    if (ctx->version >= 5) {
        return mask == E2K_TAG_MASK_128;
    } else {
        return mask == E2K_TAG_MASK_80;
    }
}

static void gen_reg_tag_mask(DisasContext *ctx, TCGv_i32 ret,
    TCGv_i32 index, uint8_t mask)
{
    if (ctx->enable_tags) {
        TCGv_ptr t0 = tcg_temp_new_ptr();


        gen_reg_tag_ptr(t0, index);

        if (is_tag_mask_all(ctx, mask)) {
            tcg_gen_ld8u_i32(ret, t0, 0);
        } else {
            TCGv_i32 t1 = tcg_temp_new_i32();

            tcg_gen_ld8u_i32(t1, t0, 0);
            tcg_gen_andi_i32(ret, t1, mask);

            tcg_temp_free_i32(t1);
        }

        tcg_temp_free_ptr(t0);
    } else {
        tcg_gen_movi_i32(ret, 0);
    }
}

#define gen_reg_tag_i32(c, r, i) \
    gen_reg_tag_mask(c, r, i, E2K_TAG_MASK_32)

#define gen_reg_tag_i64(c, r, i) \
    gen_reg_tag_mask(c, r, i, E2K_TAG_MASK_64)

#define gen_reg_tag_i80(c, r, i) \
    gen_reg_tag_mask(c, r, i, E2K_TAG_MASK_80)

#define gen_reg_tag_i128(c, r, i) \
    gen_reg_tag_mask(c, r, i, E2K_TAG_MASK_128)

static void gen_reg_tag_mask_set(DisasContext *ctx, TCGv_i32 tag,
    TCGv_i32 index, uint8_t mask)
{
    TCGv_ptr t0;

    if (!ctx->enable_tags) {
        return;
    }

    t0 = tcg_temp_new_ptr();
    gen_reg_tag_ptr(t0, index);

    if (mask == E2K_TAG_MASK_128) {
        tcg_gen_st8_i32(tag, t0, 0);
    } else if (mask == E2K_TAG_MASK_80) {
        TCGv_i32 t1 = tcg_temp_new_i32();
        
        tcg_gen_andi_i32(t1, tag, E2K_TAG_MASK_80);
        tcg_gen_st8_i32(t1, t0, 0);
        
        tcg_temp_free_i32(t1);
    } else {
        TCGv_i32 t1 = tcg_temp_new_i32();
        TCGv_i32 t2 = tcg_temp_new_i32();
        TCGv_i32 t3 = tcg_temp_new_i32();
        TCGv_i32 t4 = tcg_temp_new_i32();

        tcg_gen_ld8u_i32(t1, t0, 0);
        tcg_gen_andi_i32(t2, t1, ~mask);
        tcg_gen_andi_i32(t3, tag, mask);
        tcg_gen_or_i32(t4, t2, t3);
        tcg_gen_st8_i32(t4, t0, 0);

        tcg_temp_free_i32(t4);
        tcg_temp_free_i32(t3);
        tcg_temp_free_i32(t2);
        tcg_temp_free_i32(t1);
    }

    tcg_temp_free_ptr(t0);
}

#define gen_reg_tag_set_i32(c, t, i) \
    gen_reg_tag_mask_set(c, t, i, E2K_TAG_MASK_32)

#define gen_reg_tag_set_i64(c, t, i) \
    gen_reg_tag_mask_set(c, t, i, E2K_TAG_MASK_64)

#define gen_reg_tag_set_i80(c, t, i) \
    gen_reg_tag_mask_set(c, t, i, E2K_TAG_MASK_80)

#define gen_reg_tag_set_i128(c, t, i) \
    gen_reg_tag_mask_set(c, t, i, E2K_TAG_MASK_128)

static void gen_breg_index_i32(TCGv_i32 ret, int index)
{
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i32 t1 = tcg_temp_new_i32();

    tcg_gen_addi_i32(t0, cpu_bcur, index);
    gen_wrap_i32(t1, t0, cpu_bsize);
    tcg_gen_add_i32(ret, cpu_boff, t1);

    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
}

static inline void gen_wreg_index_i32(TCGv_i32 ret, int index)
{
    tcg_gen_movi_i32(ret, index);
}

static inline void gen_greg_index_i32(TCGv_i32 ret, int index)
{
    tcg_gen_movi_i32(ret, E2K_NR_COUNT + index);
}

static void gen_reg_index(DisasContext *ctx, TCGv_i32 ret, uint8_t arg)
{
    if (IS_BASED(arg)) {
        gen_breg_index_i32(ret, GET_BASED(arg));
    } else if (IS_REGULAR(arg)) {
        gen_wreg_index_i32(ret, GET_REGULAR(arg));
    } else if (IS_GLOBAL(arg)) {
        gen_greg_index_i32(ret, GET_GLOBAL(arg));
    } else {
        gen_tr_excp_illopc(ctx);
    }
}

static void gen_reg_lo_ptr(TCGv_ptr ret, TCGv_i32 idx)
{
    TCGv_ptr t0 = tcg_temp_new_ptr();

    tcg_gen_addi_ptr(t0, cpu_env, offsetof(CPUE2KState, regs));
    gen_ptr_from_index(ret, t0, idx, sizeof(E2KReg));
    tcg_temp_free_ptr(t0);
}

static int64_t get_literal(DisasContext *ctx, uint8_t arg)
{
    int i = GET_LIT(arg);
    int64_t lit = 0;

    if (!ctx->bundle.lts_present[i]) {
        gen_tr_excp_illopc(ctx);
    } else if (IS_LIT16_LO(arg) && i < 2) {
        lit = sextract32(ctx->bundle.lts[i], 0, 16);
    } else if (IS_LIT16_HI(arg) && i < 2) {
        lit = ((int64_t) lit << 32) >> 48;
        lit = sextract32(ctx->bundle.lts[i], 16, 16);
    } else if (IS_LIT32(arg)) {
        lit = (int32_t) ctx->bundle.lts[i];
    } else if (IS_LIT64(arg) && i < 3) {
        if (!ctx->bundle.lts_present[i + 1]) {
            gen_tr_excp_illopc(ctx);
        }
        lit = ((uint64_t) ctx->bundle.lts[i + 1] << 32) | ctx->bundle.lts[i];
    } else {
        gen_tr_excp_illopc(ctx);
    }

    return lit;
}

static void gen_tagged_temp_ptr(TCGv_ptr ret, int i)
{
    tcg_gen_addi_ptr(ret, cpu_env, offsetof(CPUE2KState, tmp[i]));
}

static void gen_result_reg_ptr(DisasContext *ctx, TCGv_ptr ret, uint8_t dst,
    int temp_index)
{
    if (dst == 0xdf) {
        /* %empty */
        intptr_t offset = offsetof(CPUE2KState, al_result[temp_index]);
        tcg_gen_addi_ptr(ret, cpu_env, offset);
    } else if (IS_REG(dst)) {
        TCGv_i32 t0 = tcg_temp_new_i32();

        gen_reg_index(ctx, t0, dst);
        gen_reg_lo_ptr(ret, t0);

        tcg_temp_free_i32(t0);
    } else {
        gen_tr_excp_illopc(ctx);
    }
}

static void gen_temp_result_ptr(TCGv_ptr ret, int i)
{
    tcg_gen_addi_ptr(ret, cpu_env, offsetof(CPUE2KState, al_result[i + 6]));
}

static void gen_tagged_const_ptr(Tagged_ptr ret, uint8_t tag, int64_t val)
{
    TCGv_i64 lo = tcg_const_i64(val);
    TCGv_i64 hi = tcg_const_i64(0);

    tcg_gen_movi_i32(ret.tag, tag);
    tcg_gen_st_i64(lo, ret.val, offsetof(E2KReg, lo));
    tcg_gen_st_i64(hi, ret.val, offsetof(E2KReg, hi));

    tcg_temp_free_i64(hi);
    tcg_temp_free_i64(lo);
}

static void gen_tagged_const_q(Tagged_ptr ret, uint8_t tag, int64_t val, int i)
{
    gen_tagged_temp_ptr(ret.val, i);
    gen_tagged_const_ptr(ret, tag, val);
}

#define gen_tagged_const_x gen_tagged_const_q

static void gen_tagged_const_s(Tagged_i32 ret, uint8_t tag, int64_t val, int i)
{
    tcg_gen_movi_i32(ret.tag, tag);
    tcg_gen_movi_i32(ret.val, val);
}

static void gen_tagged_const_d(Tagged_i64 ret, uint8_t tag, int64_t val, int i)
{
    tcg_gen_movi_i32(ret.tag, tag);
    tcg_gen_movi_i64(ret.val, val);
}

static void gen_tagged_reg_ptr(DisasContext *ctx, Tagged_ptr ret,
    uint8_t arg, uint8_t mask)
{
    TCGv_i32 t0 = tcg_temp_new_i32();

    if (IS_BASED(arg)) {
        TCGv_i32 t1 = tcg_temp_new_i32();

        tcg_gen_addi_i32(t0, cpu_bcur, GET_BASED(arg));
        tcg_gen_sub_i32(t1, t0, cpu_bsize);
        tcg_gen_movcond_i32(TCG_COND_LT, t0, t0, cpu_bsize, t0, t1);
        tcg_gen_add_i32(t0, t0, cpu_boff);

        tcg_temp_free_i32(t1);
    } else if (IS_REGULAR(arg)) {
        tcg_gen_movi_i32(t0, GET_REGULAR(arg));
    } else if (IS_GLOBAL(arg)) {
        tcg_gen_movi_i32(t0, E2K_NR_COUNT + GET_GLOBAL(arg));
    } else {
        gen_tr_excp_illopc(ctx);
    }

    gen_reg_tag_mask(ctx, ret.tag, t0, mask);
    gen_reg_lo_ptr(ret.val, t0);

    tcg_temp_free_i32(t0);
}

static Tagged* get_saved_reg(DisasContext *ctx, uint8_t arg, int chan)
{
    int i;

    for (i = 0; i < ctx->saved_reg_len; i++) {
        SavedReg *r = &ctx->saved_reg[i];

        switch (r->tagged.kind) {
        case TAGGED_S:
        case TAGGED_D:
            if (chan > r->chan && r->dst == arg) {
                return &r->tagged;
            }
            break;
        case TAGGED_X:
        case TAGGED_Q:
            if (chan >= r->chan && r->dst == arg) {
                return &r->tagged;
            }
            break;
        }
    }

    return NULL;
}

static void gen_tagged_reg_q(DisasContext *ctx, Tagged_ptr ret, uint8_t arg,
    int chan)
{
    Tagged *r = get_saved_reg(ctx, arg, chan);

    if (r) {
        switch (r->kind) {
        case TAGGED_Q:
            tcg_gen_mov_i32(ret.tag, r->tptr.tag);
            tcg_gen_addi_ptr(ret.val, r->tptr.val, 0);
            break;
        default:
            g_assert_not_reached();
            break;
        }
    } else {
        gen_tagged_reg_ptr(ctx, ret, arg, E2K_TAG_MASK_128);
    }
}

static void gen_tagged_reg_x(DisasContext *ctx, Tagged_ptr ret, uint8_t arg,
    int chan)
{
    Tagged *r = get_saved_reg(ctx, arg, chan);

    if (r) {
        switch (r->kind) {
        case TAGGED_X:
            tcg_gen_mov_i32(ret.tag, r->tptr.tag);
            tcg_gen_addi_ptr(ret.val, r->tptr.val, 0);
            break;
        case TAGGED_Q:
            tcg_gen_andi_i32(ret.tag, r->tptr.tag, E2K_TAG_MASK_80);
            tcg_gen_addi_ptr(ret.val, r->tptr.val, 0);
            break;
        default:
            g_assert_not_reached();
            break;
        }
    } else {
        gen_tagged_reg_ptr(ctx, ret, arg, E2K_TAG_MASK_80);
    }
}

static void gen_tagged_reg_d(DisasContext *ctx, Tagged_i64 ret, uint8_t arg,
    int chan)
{
    Tagged *r = get_saved_reg(ctx, arg, chan);

    if (r) {
        switch (r->kind) {
        case TAGGED_D:
            tcg_gen_mov_i32(ret.tag, r->t64.tag);
            tcg_gen_mov_i64(ret.val, r->t64.val);
            break;
        case TAGGED_X:
        case TAGGED_Q:
            tcg_gen_andi_i32(ret.tag, r->tptr.tag, E2K_TAG_MASK_64);
            tcg_gen_ld_i64(ret.val, r->tptr.val, offsetof(E2KReg, lo));
            break;
        default:
            g_assert_not_reached();
            break;
        }
    } else {
        Tagged_ptr t0 = tagged_new_ptr();

        gen_tagged_reg_ptr(ctx, t0, arg, E2K_TAG_MASK_64);
        tcg_gen_mov_i32(ret.tag, t0.tag);
        tcg_gen_ld_i64(ret.val, t0.val, offsetof(E2KReg, lo));

        tagged_free_ptr(t0);
    }
}

static void gen_tagged_reg_s(DisasContext *ctx, Tagged_i32 ret, uint8_t arg,
    int chan)
{
    Tagged *r = get_saved_reg(ctx, arg, chan);

    if (r) {
        switch (r->kind) {
        case TAGGED_S:
            tcg_gen_mov_i32(ret.tag, r->t32.tag);
            tcg_gen_mov_i32(ret.val, r->t32.val);
            break;
        case TAGGED_D:
            tcg_gen_andi_i32(ret.tag, r->t64.tag, E2K_TAG_MASK_32);
            tcg_gen_extrl_i64_i32(ret.val, r->t64.val);
            break;
        case TAGGED_X:
        case TAGGED_Q:
            tcg_gen_andi_i32(ret.tag, r->tptr.tag, E2K_TAG_MASK_32);
            tcg_gen_ld_i32(ret.val, r->tptr.val, offsetof(E2KReg, lo));
            break;
        default:
            g_assert_not_reached();
            break;
        }
    } else {
        Tagged_ptr t0 = tagged_new_ptr();

        gen_tagged_reg_ptr(ctx, t0, arg, E2K_TAG_MASK_32);
        tcg_gen_mov_i32(ret.tag, t0.tag);
        tcg_gen_ld_i32(ret.val, t0.val, offsetof(E2KReg, lo));

        tagged_free_ptr(t0);
    }
}

#define IMPL_GEN_TAGGED_SRC1(S, T) \
    static void glue(gen_tagged_src1_, S)(Alop *alop, T ret) \
    { \
        if (IS_IMM5(alop->als.src1)) { \
            glue(gen_tagged_const_, S)(ret, 0, GET_IMM5(alop->als.src1), 1); \
        } else { \
            glue(gen_tagged_reg_, S)(alop->ctx, ret, alop->als.src1, alop->chan); \
        } \
    }

IMPL_GEN_TAGGED_SRC1(q, Tagged_ptr)
IMPL_GEN_TAGGED_SRC1(x, Tagged_ptr)
IMPL_GEN_TAGGED_SRC1(d, Tagged_i64)
IMPL_GEN_TAGGED_SRC1(s, Tagged_i32)

#define IMPL_GEN_TAGGED_SRC2(S, T) \
    static void glue(gen_tagged_src2_, S)(Alop *alop, T ret) \
    { \
        if (IS_IMM4(alop->als.src2)) { \
            glue(gen_tagged_const_, S)(ret, 0, GET_IMM4(alop->als.src2), 2); \
        } else if (IS_LIT(alop->als.src2)) { \
            int64_t lit = get_literal(alop->ctx, alop->als.src2); \
            \
            glue(gen_tagged_const_, S)(ret, 0, lit, 2); \
        } else { \
            glue(gen_tagged_reg_, S)(alop->ctx, ret, alop->als.src2, alop->chan); \
        } \
    }

IMPL_GEN_TAGGED_SRC2(q, Tagged_ptr)
IMPL_GEN_TAGGED_SRC2(x, Tagged_ptr)
IMPL_GEN_TAGGED_SRC2(d, Tagged_i64)
IMPL_GEN_TAGGED_SRC2(s, Tagged_i32)

#define IMPL_GEN_TAGGED_SRC3(S, T) \
    static void glue(gen_tagged_src3_, S)(Alop *alop, T ret) \
    { \
        glue(gen_tagged_reg_, S)(alop->ctx, ret, alop->ales.src3, alop->chan); \
    }

IMPL_GEN_TAGGED_SRC3(q, Tagged_ptr)
IMPL_GEN_TAGGED_SRC3(d, Tagged_i64)
IMPL_GEN_TAGGED_SRC3(s, Tagged_i32)

#define IMPL_GEN_TAGGED_SRC4(S, T) \
    static void glue(gen_tagged_src4_, S)(Alop *alop, T ret) \
    { \
        glue(gen_tagged_reg_, S)(alop->ctx, ret, alop->als.src4, alop->chan); \
    }

IMPL_GEN_TAGGED_SRC4(q, Tagged_ptr)
IMPL_GEN_TAGGED_SRC4(d, Tagged_i64)
IMPL_GEN_TAGGED_SRC4(s, Tagged_i32)

/*
 * Returns zero if all @args is zero otherwise returns @tag.
 */
static inline void gen_tag3_raw(TCGv_i32 ret, int tag, TCGv_i32 arg1, TCGv_i32 arg2,
    TCGv_i32 arg3)
{
    TCGv_i32 zero = tcg_const_i32(0);
    TCGv_i32 fail = tcg_const_i32(tag);
    TCGv_i32 t0 = tcg_temp_new_i32();

    if (arg1 != NULL) {
        tcg_gen_mov_i32(t0, arg1);
    } else {
        tcg_gen_movi_i32(t0, 0);
    }
    if (arg2 != NULL) {
        tcg_gen_or_i32(t0, t0, arg2);
    }
    if (arg3 != NULL) {
        tcg_gen_or_i32(t0, t0, arg3);
    }
    tcg_gen_movcond_i32(TCG_COND_NE, ret, t0, zero, fail, zero);

    tcg_temp_free_i32(t0);
    tcg_temp_free_i32(fail);
    tcg_temp_free_i32(zero);
}

#define gen_tag3_i128(ret, a1, a2, a3) \
    gen_tag3_raw(ret, E2K_TAG_NON_NUMBER128, a1, a2, a3)
#define gen_tag2_i128(ret, a1, a2) gen_tag3_i128(ret, a1, a2, NULL)
#define gen_tag1_i128(ret, a1) gen_tag2_i128(ret, a1, NULL)

#define gen_tag3_i64(ret, a1, a2, a3) \
    gen_tag3_raw(ret, E2K_TAG_NON_NUMBER64, a1, a2, a3)
#define gen_tag2_i64(ret, a1, a2) gen_tag3_i64(ret, a1, a2, NULL)
#define gen_tag1_i64(ret, a1) gen_tag2_i64(ret, a1, NULL)

#define gen_tag3_i32(ret, a1, a2, a3) \
    gen_tag3_raw((ret), E2K_TAG_NON_NUMBER32, (a1), (a2), (a3))
#define gen_tag2_i32(ret, a1, a2) gen_tag3_i32((ret), (a1), (a2), NULL)
#define gen_tag1_i32(ret, a1) gen_tag2_i32((ret), (a1), NULL)

#if !defined(FORCE_SAVE_ALC_PREG) && !defined(FORCE_SAVE_PLU_PREG)
static bool is_preg_used_by_rlp(DisasContext *ctx, int chan, int index)
{
    int i, j;

    for (i = 0; i < 3; i++) {
        if (!ctx->bundle.cds_present[i]) {
            continue;
        }

        for (j = 0; j < 2; j++) {
            uint16_t rlp = extract32(ctx->bundle.cds[i], j * 16, 16);
            int cluster = extract16(rlp, 14, 1);
            int am = extract16(rlp, 13, 1);
            int mask = extract16(rlp, 10, 3) << (cluster ? 3 : 0);
            int psrc = extract16(rlp, 0, 5);

            // FIXME: forced save preg if am is set
            if (((1 << chan) < mask || am) && index == psrc) {
                return true;
            }
        }
    }

    return false;
}

static bool is_preg_used_by_pls(DisasContext *ctx, int offset, int index)
{
    int i, j;

    for (i = offset; i < 2; i++) {
        uint32_t pls = ctx->bundle.pls[i];

        if (!ctx->bundle.pls_present[i]) {
            continue;
        }

        for (j = 0; j < 2; j++) {
            uint8_t psrc = extract32(pls, 16 + j * 8, 7);

            if ((psrc & 0xe0) == 0x60) {
                int index2 = extract8(psrc, 0, 5);

                if (index == index2) {
                    return true;
                }
            }
        }
    }

    return false;
}

static bool is_preg_used_by_ct(DisasContext *ctx, int index)
{
    if (ctx->ct.type == CT_NONE) {
        return false;
    }

    switch (ctx->ct.cond_type) {
    case 0x2:
    case 0x6:
    case 0xf:
    case 0x3:
    case 0x7:
    case 0xe:
        return index == ctx->ct.psrc;
    default:
        return false;
    }
}
#endif

static bool is_preg_saved(DisasContext *ctx, int index)
{
    int i;

    for (i = 0; i < ctx->saved_preg_len; i++) {
        if (ctx->saved_preg[i].index == index) {
            return true;
        }
    }

    return false;
}

static bool is_alop_affected_by_dbl(Alop *alop)
{
    if (alop->format == ALOPF2) {
        switch (alop->op) {
        case OP_MOVTS:
        case OP_MOVTCS:
        case OP_MOVTRS:
        case OP_MOVTRCS:
            return false;
        default:
            break;
        }
    }

    return true;
}

static bool is_alop_check_tag(Alop *alop)
{
    if (alop->format == ALOPF2) {
        switch (alop->op) {
        case OP_PUTTAGS:
        case OP_PUTTAGD:
        case OP_MOVTS:
        case OP_MOVTRS:
        case OP_MOVTD:
        case OP_MOVTRD:
        case OP_MOVTQ:
        case OP_MOVTRQ:
            return false;
        default:
            break;
        }
    }

    return true;
}

#if 0
static bool is_alop_poison_result(Alop *alop)
{
    if (alop->format == ALOPF2) {
        switch (alop->op) {
        case OP_PUTTAGS:
        case OP_PUTTAGD:
        case OP_MOVTS:
        case OP_MOVTRS:
        case OP_MOVTD:
        case OP_MOVTRD:
        case OP_MOVTQ:
        case OP_MOVTRQ:
            return false;
        default:
            break;
        }
    }

    return true;
}
#endif

static bool is_alop_store(Alop *alop)
{
    switch (alop->format) {
    case ALOPF3:
    case ALOPF10:
    case ALOPF13:
        return true;
    default:
        return false;
    }
}

static void gen_save_preg(DisasContext *ctx, int index)
{
    SavedPreg *r = &ctx->saved_preg[ctx->saved_preg_len++];

    r->index = index;
    r->val = e2k_get_temp_i32(ctx);
    gen_preg_raw_i32(r->val, r->index);
}

static void gen_get_lp(TCGv_i32 ret, uint16_t clp, int offset, TCGv_i32 lp[7])
{
    int p = extract32(clp, offset, 3);
    int neg = GET_BIT(clp, offset + 3);

    tcg_gen_xori_i32(ret, lp[p], neg);
}

static void gen_cond_i32(DisasContext *ctx, TCGv_i32 ret, uint8_t psrc)
{
    if (psrc & 0x80) {
        if (psrc == 0xc0) {
            // %bgrpred
            e2k_todo_illop(ctx, "%%bgrpred");
        } else if ((psrc & 0xe0) == 0xc0) {
            // %rndpred
            e2k_todo_illop(ctx, "%%rndpred");
        } else {
            gen_tr_excp_illopn(ctx);
        }
    } else {
        int idx = extract8(psrc, 0, 5);
        if (psrc == 0) {
            // %lcntex
            gen_lcntex(ret);
        } else if ((psrc & 0x40) == 0) {
            // %spredMASK
            e2k_todo_illop(ctx, "%%spred");
        } else if ((psrc & 0x60) == 0x60) {
            // %predN
            gen_preg_i32(ctx, ret, idx);
        } else {
            // %pcntN
            tcg_gen_setcondi_i32(TCG_COND_LEU, ret, cpu_lsr_pcnt, idx);
        }
    }
}

static inline void scan_needed(const UnpackedBundle *bundle, int need[7])
{
    bool once_more = true;
    unsigned int i;

    for (i = 0; i < 3; i++) {
        if (bundle->pls_present[i] && GET_BIT(bundle->pls[i], 5)) {
            need[4 + i] = 1;
        }
    }

    while (once_more) {
        once_more = false;

        for (i = 0; i < 3; i++) {
            int p0, p1;

            if (need[4 + i] != 1) {
                continue;
            }

            p0 = extract32(bundle->pls[i], 10, 3);
            p1 = extract32(bundle->pls[i], 6, 3);

            if (p0 < 7 && need[p0] == 0) {
                need[p0] = 1;

                if (p0 >= 4) {
                    once_more = true;
                }
            }

            if (p1 < 7 && need[p1] == 0) {
                need[p1] = 1;

                if (p1 >= 4) {
                    once_more = true;
                }
            }

            need[4 + i] = 2;
        }
    }
}

static void gen_plu(DisasContext *ctx)
{
    const UnpackedBundle *bundle = &ctx->bundle;
    int i, need[7] = { 0 };
    TCGv_i32 lp[7];

    scan_needed(bundle, need);

    for (i = 0; i < 7; i++) {
        if (need[i]) {
            lp[i] = tcg_temp_new_i32();
        }
    }

    for (i = 0; i < 3; i++) {
        if (!bundle->pls_present[i]) {
            continue;
        }

        if (i < 2) {
            if (need[i * 2]) {
                int elp = extract32(bundle->pls[i], 24, 7);
                gen_cond_i32(ctx, lp[i * 2], elp);
            }

            if (need[i * 2 + 1]) {
                int elp = extract32(bundle->pls[i], 16, 7);
                gen_cond_i32(ctx, lp[i * 2 + 1], elp);
            }
        }

        if (need[4 + i]) {
            uint16_t clp = extract32(bundle->pls[i], 0, 16);
            int opc = extract32(clp, 14, 2);
            TCGv_i32 p0 = tcg_temp_new_i32();
            TCGv_i32 p1 = tcg_temp_new_i32();
            int vdst = extract32(clp, 5, 1);
            int pdst = extract32(clp, 0, 5);

            // TODO: check clp arg
            // {C/M}LP0 0, 1             => 4
            // {C/M}LP1 0, 1, 2, 3, 4    => 5
            // {C/M}LP2 0, 1, 2, 3, 4, 5 => 6
            // maximal cascading is 2

            if (vdst && !is_preg_saved(ctx, pdst)
#ifndef FORCE_SAVE_PLU_PREG
                && (is_preg_used_by_pls(ctx, i + 1, pdst)
                    || is_preg_used_by_ct(ctx, pdst))
#endif
            ) {
                gen_save_preg(ctx, pdst);
            }

            gen_get_lp(p0, clp, 10, lp);
            gen_get_lp(p1, clp, 6, lp);

            switch (opc) {
            case 0: /* andp */
                // FIXME: what is the difference between `andp` and `landp`?
            case 1: /* landp */
                tcg_gen_and_i32(lp[4 + i], p0, p1);
                if (vdst) {
                    gen_preg_set_i32(pdst, lp[4 + i]);
                }
                break;
            case 3: { /* movep */
                // FIXME: clp cannot read result of movep???
                tcg_gen_and_i32(lp[4 + i], p0, p1);

                if (vdst) {
                    TCGv_i32 t0 = tcg_const_i32(0);
                    TCGv_i32 t1 = tcg_temp_new_i32();
                    TCGv_i32 t2 = tcg_temp_new_i32();

                    gen_preg_i32(ctx, t1, pdst);
                    tcg_gen_movcond_i32(TCG_COND_NE, t2, p0, t0, p1, t1);
                    gen_preg_set_i32(pdst, t2);

                    tcg_temp_free_i32(t2);
                    tcg_temp_free_i32(t1);
                    tcg_temp_free_i32(t0);
                }
                break;
            }
            default:
                gen_tr_excp_illopc(ctx);
                break;
            }
        }
    }

    for (i = 0; i < 7; i++) {
        if (need[i]) {
            tcg_temp_free_i32(lp[i]);
        }
    }
}

static inline ArgSize alop_reg_max_size(uint8_t src1, ArgSize size1,
    uint8_t src2, ArgSize size2)
{
    if (IS_REG(src2) && src1 == src2) {
        return MAX(size1, size2);
    } else {
        return size1;
    }
}

static ArgSize alop_opn_max_size(Alop *alop, uint8_t src)
{
    ArgSize r = ARG_SIZE_NONE;

    switch (alop->format) {
    case ALOPF_NONE:
        break;
    case ALOPF1:
    case ALOPF1_MAS:
    case ALOPF1_MERGE:
    case ALOPF7:
    case ALOPF11:
    case ALOPF11_MAS:
    case ALOPF11_MERGE:
    case ALOPF11_LIT8:
    case ALOPF17:
        r = alop_reg_max_size(src, r, alop->als.src1, args_src1(alop->args));
        r = alop_reg_max_size(src, r, alop->als.src2, args_src2(alop->args));
        break;
    case ALOPF2:
    case ALOPF8:
    case ALOPF12:
    case ALOPF12_PSHUFH:
    case ALOPF15:
    case ALOPF22:
        r = alop_reg_max_size(src, r, alop->als.src2, args_src2(alop->args));
        break;
    case ALOPF3:
    case ALOPF13:
        r = alop_reg_max_size(src, r, alop->als.src1, args_src1(alop->args));
        r = alop_reg_max_size(src, r, alop->als.src2, args_src2(alop->args));
        r = alop_reg_max_size(src, r, alop->als.src4, args_src4(alop->args));
        break;
    case ALOPF10:
        r = alop_reg_max_size(src, r, alop->als.src4, args_src4(alop->args));
        break;
    case ALOPF16:
        break;
    case ALOPF21:
    case ALOPF21_ICOMB:
    case ALOPF21_FCOMB:
    case ALOPF21_PFCOMB:
    case ALOPF21_QPFCOMB:
        r = alop_reg_max_size(src, r, alop->als.src1, args_src1(alop->args));
        r = alop_reg_max_size(src, r, alop->als.src2, args_src2(alop->args));
        r = alop_reg_max_size(src, r, alop->ales.src3, args_src3(alop->args));
        break;
    default:
        g_assert_not_reached();
        break;
    }

    return r;
}

static bool is_reg_saved(DisasContext *ctx, uint8_t dst)
{
    int i;

    for (i = 0; i < ctx->saved_reg_len; i++) {
        if (ctx->saved_reg[i].dst == dst) {
            return true;
        }
    }

    return false;
}

static void gen_save_reg(DisasContext *ctx, int chan, ArgSize size,
    uint8_t dst)
{
    SavedReg *r;
    Tagged_ptr t0;

    if (size == ARG_SIZE_NONE) {
        return;
    }

    r = &ctx->saved_reg[ctx->saved_reg_len++];
    r->dst = dst;
    r->chan = chan;
    t0 = tagged_new_ptr();

    switch (size) {
    case ARG_SIZE_S:
        r->tagged.kind = TAGGED_S;
        r->tagged.t32 = e2k_get_tagged_temp_i32(ctx);
        gen_tagged_reg_ptr(ctx, t0, r->dst, E2K_TAG_MASK_32);
        tcg_gen_mov_i32(r->tagged.t32.tag, t0.tag);
        tcg_gen_ld_i32(r->tagged.t32.val, t0.val, offsetof(E2KReg, lo));
        break;
    case ARG_SIZE_D:
    case ARG_SIZE_Q:
        r->tagged.kind = TAGGED_D;
        r->tagged.t64 = e2k_get_tagged_temp_i64(ctx);
        gen_tagged_reg_ptr(ctx, t0, r->dst, E2K_TAG_MASK_64);
        tcg_gen_mov_i32(r->tagged.t64.tag, t0.tag);
        tcg_gen_ld_i64(r->tagged.t64.val, t0.val, offsetof(E2KReg, lo));
        break;
    case ARG_SIZE_X:
    {
        Tagged_ptr *t = &r->tagged.tptr;
        TCGv_i64 t1 = tcg_temp_new_i64();
        TCGv_i32 t2 = tcg_temp_new_i32();

        r->tagged.kind = TAGGED_X;
        *t = e2k_get_tagged_temp_ptr(ctx);
        tcg_gen_addi_ptr(t->val, cpu_env, offsetof(CPUE2KState, tmp_saved[chan]));
        gen_tagged_reg_ptr(ctx, t0, r->dst, E2K_TAG_MASK_80);
        tcg_gen_mov_i32(t->tag, t0.tag);
        tcg_gen_ld_i64(t1, t0.val, offsetof(E2KReg, lo));
        tcg_gen_ld16u_i32(t2, t0.val, offsetof(E2KReg, hi));
        tcg_gen_st_i64(t1, t->val, offsetof(E2KReg, lo));
        tcg_gen_st_i32(t2, t->val, offsetof(E2KReg, hi));

        tcg_temp_free_i32(t2);
        tcg_temp_free_i64(t1);
        break;
    }
    case ARG_SIZE_P:
    {
        Tagged_ptr *t = &r->tagged.tptr;
        TCGv_i64 t1 = tcg_temp_new_i64();
        TCGv_i64 t2 = tcg_temp_new_i64();

        r->tagged.kind = TAGGED_Q;
        *t = e2k_get_tagged_temp_ptr(ctx);
        tcg_gen_addi_ptr(t->val, cpu_env, offsetof(CPUE2KState, tmp_saved[chan]));
        gen_tagged_reg_ptr(ctx, t0, r->dst, E2K_TAG_MASK_128);
        tcg_gen_mov_i32(t->tag, t0.tag);
        tcg_gen_ld_i64(t1, t0.val, offsetof(E2KReg, lo));
        tcg_gen_ld_i64(t2, t0.val, offsetof(E2KReg, hi));
        tcg_gen_st_i64(t1, t->val, offsetof(E2KReg, lo));
        tcg_gen_st_i64(t2, t->val, offsetof(E2KReg, hi));

        tcg_temp_free_i64(t2);
        tcg_temp_free_i64(t1);
        break;
    }
    default:
        g_assert_not_reached();
        break;
    }

    tagged_free_ptr(t0);
}

static void gen_alop_save_dst(Alop *alop)
{
    DisasContext *ctx = alop->ctx;
    uint8_t dst;

    switch (alop->format) {
    case ALOPF_NONE:
    case ALOPF3:
    case ALOPF10:
    case ALOPF13:
    case ALOPF15:
        /* no dst reg */
        break;
    case ALOPF1:
    case ALOPF1_MAS:
    case ALOPF1_MERGE:
    case ALOPF2:
    case ALOPF11:
    case ALOPF11_MAS:
    case ALOPF11_MERGE:
    case ALOPF11_LIT8:
    case ALOPF12:
    case ALOPF12_PSHUFH:
    case ALOPF16:
    case ALOPF21:
    case ALOPF21_ICOMB:
    case ALOPF21_FCOMB:
    case ALOPF21_PFCOMB:
    case ALOPF21_QPFCOMB:
    case ALOPF22:
        dst = alop->als.dst;

        if (IS_REG(dst) && !is_reg_saved(ctx, dst)) {
            int chan = alop->chan;
            ArgSize size;

            if (!ctx->force_save_alc_dst && IS_REG_SAVE_SAFE(dst)) {
                int i;

                size = alop_opn_max_size(alop, dst);
                size = size >= ARG_SIZE_X ? size : ARG_SIZE_NONE;

                for (i = alop->chan + 1; i < 6; i++) {
                    Alop *next_alop = &alop->ctx->alops[i];

                    size = MAX(size, alop_opn_max_size(next_alop, dst));
                }
            } else {
                size = ctx->version >= 5 ? ARG_SIZE_P : ARG_SIZE_X;
            }
            gen_save_reg(ctx, chan, size, dst);
        }
        break;
    case ALOPF7:
    case ALOPF8:
    case ALOPF17:
        dst = alop->als.dst_preg;

#ifndef FORCE_SAVE_ALC_PREG
        if (!is_preg_saved(ctx, dst)
            && (is_preg_used_by_rlp(ctx, alop->chan, dst)
                || is_preg_used_by_pls(ctx, 0, dst)
                || is_preg_used_by_ct(ctx, dst)))
        {
            gen_save_preg(ctx, dst);
        }
#else
        if (!is_preg_saved(ctx, dst)) {
            gen_save_preg(ctx, dst);
        }
#endif
        break;
    default:
        g_assert_not_reached();
        break;
    }
}

static void gen_qppackdl(TCGv_ptr ret, TCGv_i64 hi, TCGv_i64 lo)
{
    tcg_gen_st_i64(lo, ret, offsetof(E2KReg, lo));
    tcg_gen_st_i64(hi, ret, offsetof(E2KReg, hi));
}

static void gen_qpunpackdl(TCGv_i64 hi, TCGv_i64 lo, TCGv_ptr arg)
{
    tcg_gen_ld_i64(lo, arg, offsetof(E2KReg, lo));
    tcg_gen_ld_i64(hi, arg, offsetof(E2KReg, hi));
}

static void gen_movqp(TCGv_ptr ret, TCGv_ptr arg)
{
    TCGv_i64 t0 = tcg_temp_new_i64();
    TCGv_i64 t1 = tcg_temp_new_i64();

    gen_qpunpackdl(t0, t1, arg);
    gen_qppackdl(ret, t0, t1);

    tcg_temp_free_i64(t1);
    tcg_temp_free_i64(t0);
}

static void gen_reg_set_q(DisasContext *ctx, TCGv_i32 tag, TCGv_i64 hi,
    TCGv_i64 lo, uint8_t dst)
{
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_ptr t1 = tcg_temp_new_ptr();

    gen_reg_index(ctx, t0, dst);
    gen_reg_lo_ptr(t1, t0);
    gen_reg_tag_mask_set(ctx, tag, t0, E2K_TAG_MASK_128);
    gen_qppackdl(t1, hi, lo);

    tcg_temp_free_ptr(t1);
    tcg_temp_free_i32(t0);
}

static void gen_reg_set_d(DisasContext *ctx,
    TCGv_i32 index, TCGv_i32 tag, TCGv_i64 value)
{
    TCGv_ptr t0 = tcg_temp_new_ptr();

    // TODO: poison value
    gen_reg_tag_set_i64(ctx, tag, index);
    gen_reg_lo_ptr(t0, index);
    tcg_gen_st_i64(value, t0, 0);

    tcg_temp_free_ptr(t0);
}

static void gen_reg_set_s(DisasContext *ctx, bool is_dbl,
    TCGv_i32 index, TCGv_i32 tag, TCGv_i64 val)
{
    if (is_dbl) {
        Cs1 *cs1 = &ctx->cs1;
        Cs1Setr *setr = &cs1->setr;

        if (cs1->type == CS1_SETR && (setr->type & SETR_WD) && setr->dbl) {
            TCGv_ptr t0 = tcg_temp_new_ptr();
            TCGv_i32 t1 = tcg_temp_new_i32();

            gen_reg_lo_ptr(t0, index);
            gen_tag1_i64(t1, tag);
            gen_reg_tag_set_i64(ctx, t1, index);
            tcg_gen_st_i64(val, t0, 0);

            tcg_temp_free_i32(t1);
            tcg_temp_free_ptr(t0);
        } else {
            TCGLabel *l0 = gen_new_label();
            TCGLabel *l1 = gen_new_label();
            TCGv_ptr t0 = tcg_temp_local_new_ptr();
            TCGv_i32 t1 = tcg_temp_local_new_i32();
            TCGv_i64 t2 = tcg_temp_local_new_i64();
            TCGv_i32 t3 = tcg_temp_local_new_i32();

            gen_reg_lo_ptr(t0, index);
            tcg_gen_mov_i32(t1, tag);
            tcg_gen_mov_i64(t2, val);
            tcg_gen_mov_i32(t3, index);
            tcg_gen_brcondi_i32(TCG_COND_NE, cpu_wdbl, 0, l0);

            /* wdbl is not set */
            gen_reg_tag_set_i32(ctx, t1, t3);
            tcg_gen_st32_i64(t2, t0, 0);
            tcg_gen_br(l1);

            /* wdbl is set */
            gen_set_label(l0);
            gen_tag1_i64(t1, t1);
            gen_reg_tag_set_i64(ctx, t1, t3);
            tcg_gen_st_i64(t2, t0, 0);

            gen_set_label(l1);

            tcg_temp_free_i32(t3);
            tcg_temp_free_i64(t2);
            tcg_temp_free_i32(t1);
            tcg_temp_free_ptr(t0);
        }
    } else {
        TCGv_ptr t0 = tcg_temp_new_ptr();

        gen_reg_lo_ptr(t0, index);
        gen_reg_tag_set_i32(ctx, tag, index);
        tcg_gen_st32_i64(val, t0, 0);

        tcg_temp_free_ptr(t0);
    }
}

static void gen_delayed_tag_check(DisasContext *ctx, TCGv_i32 tag)
{
    TCGv_i32 t0 = tcg_temp_new_i32();

    assert(ctx->delayed_illop);
    tcg_gen_setcondi_i32(TCG_COND_NE, t0, tag, 0);
    tcg_gen_or_i32(ctx->delayed_illop, ctx->delayed_illop, t0);
    tcg_temp_free_i32(t0);
}

static inline void gen_delayed_alop_tag_check(Alop *alop, TCGv_i32 tag)
{
    if (alop->ctx->enable_tags && !alop->als.sm && is_alop_check_tag(alop)) {
        gen_delayed_tag_check(alop->ctx, tag);
    }
}

static void gen_al_result_q(Alop *alop, Tagged_ptr arg)
{
    uint8_t dst = alop->als.dst;

    gen_delayed_alop_tag_check(alop, arg.tag);

    if (dst == 0xdf) {
        /* %empty */
    } else if (IS_REG(dst)) {
        TCGv_i32 t0 = tcg_temp_new_i32();

        gen_reg_index(alop->ctx, t0, dst);
        gen_reg_tag_mask_set(alop->ctx, arg.tag, t0, E2K_TAG_MASK_128);

        tcg_temp_free_i32(t0);
    } else {
        gen_tr_excp_illopc(alop->ctx);
    }
}

static void gen_al_result_x(Alop *alop, Tagged_ptr arg)
{
    uint8_t dst = alop->als.dst;

    gen_delayed_alop_tag_check(alop, arg.tag);

    if (dst == 0xdf) {
        /* %empty */
    } else if (IS_REG(dst)) {
        TCGv_i32 t0 = tcg_temp_new_i32();

        gen_reg_index(alop->ctx, t0, dst);
        gen_reg_tag_mask_set(alop->ctx, arg.tag, t0, E2K_TAG_MASK_80);

        tcg_temp_free_i32(t0);
    } else {
        gen_tr_excp_illopc(alop->ctx);
    }
}

static void gen_al_result_d(Alop *alop, Tagged_i64 arg)
{
    uint8_t dst = alop->als.dst;

    gen_delayed_alop_tag_check(alop, arg.tag);

    if (dst == 0xdf) {
        /* %empty */
    } else if (IS_REG(dst)) {
        TCGv_i32 t0 = tcg_temp_new_i32();

        gen_reg_index(alop->ctx, t0, dst);

        if (alop->format == ALOPF16 && alop->op == OP_RRS) {
            gen_reg_set_s(alop->ctx, true, t0, arg.tag, arg.val);
        } else {
            gen_reg_set_d(alop->ctx, t0, arg.tag, arg.val);
        }

        tcg_temp_free_i32(t0);
    } else if (IS_CTPR(dst)) {
        TCGv_i64 ctpr = cpu_ctprs[GET_CTPR(dst) - 1];
        TCGv_i64 t0 = tcg_const_i64(CTPR_TAG_DISP);

        tcg_gen_deposit_i64(ctpr, ctpr, arg.val, CTPR_BASE_OFF, CTPR_BASE_LEN);
        tcg_gen_deposit_i64(ctpr, ctpr, t0, CTPR_TAG_OFF, CTPR_TAG_LEN);

        tcg_temp_free_i64(t0);
    } else {
        gen_tr_excp_illopc(alop->ctx);
    }
}

static void gen_al_result_s(Alop *alop, Tagged_i32 arg)
{
    uint8_t dst = alop->als.dst;

    gen_delayed_alop_tag_check(alop, arg.tag);

    if (dst == 0xdf) {
        /* %empty */
    } else if (IS_REG(dst)) {
        bool dbl = is_alop_affected_by_dbl(alop);
        TCGv_i32 t0 = tcg_temp_new_i32();
        TCGv_i64 t1 = tcg_temp_new_i64();

        gen_reg_index(alop->ctx, t0, dst);
        tcg_gen_extu_i32_i64(t1, arg.val);
        gen_reg_set_s(alop->ctx, dbl, t0, arg.tag, t1);

        tcg_temp_free_i64(t1);
        tcg_temp_free_i32(t0);
    } else if (IS_CTPR(dst)) {
        TCGv_i64 ctpr = cpu_ctprs[GET_CTPR(dst) - 1];
        TCGv_i64 t0 = tcg_temp_new_i64();
        TCGv_i64 t1 = tcg_const_i64(CTPR_TAG_DISP);

        tcg_gen_extu_i32_i64(t0, arg.val);
        tcg_gen_deposit_i64(ctpr, ctpr, t0, CTPR_BASE_OFF, CTPR_BASE_LEN);
        tcg_gen_deposit_i64(ctpr, ctpr, t1, CTPR_TAG_OFF, CTPR_TAG_LEN);

        tcg_temp_free_i64(t1);
        tcg_temp_free_i64(t0);
    } else {
        gen_tr_excp_illopc(alop->ctx);
    }
}

static void gen_al_result_b(Alop *alop, Tagged_i32 arg)
{
    gen_preg_set_i32(alop->als.dst_preg, arg.val);
}

static inline bool check_qr(uint8_t src, int chan)
{
    int index = 0, o = chan & 1;
    if (IS_REGULAR(src)) {
        index = GET_REGULAR(src);
    } else if (IS_BASED(src)) {
        index = GET_BASED(src);
    } else if (IS_GLOBAL(src)) {
        index = GET_GLOBAL(src);
    } else {
        return true;
    }
    return (index & 1) == (chan < 2 ? o : o ^ 1);
}

static inline bool is_mrgc(uint16_t rlp, int chan)
{
    int is_mrgc = GET_BIT(rlp, 15);
    int cluster = GET_BIT(rlp, 14);
    int alc_mask = extract32(rlp, 10, 3);
    int alc = GET_BIT(alc_mask, chan % 3);

    return is_mrgc && (cluster == (chan > 2)) && (alc != 0);
}

static uint16_t find_mrgc(DisasContext *dc, int chan)
{
    unsigned int i;

    for (i = 0; dc->bundle.cds_present[i]; i++) {
        uint32_t cds = dc->bundle.cds[i];
        uint16_t rlp0 = cds >> 16;
        uint16_t rlp1 = cds & 0xffff;

        if (is_mrgc(rlp0, chan)) {
            return rlp0;
        }
        if (is_mrgc(rlp1, chan)) {
            return rlp1;
        }
    }

    return 0;
}

static uint16_t find_am_cond(DisasContext *ctx, int chan)
{
    unsigned int i, j;

    for (i = 0; i < ctx->bundle.cds_present[i]; i++) {
        uint16_t *cds = (uint16_t *) &ctx->bundle.cds[i];

        for (j = 0; j < 2; j++) {
            int opc = extract16(cds[j], 13, 3);
            int req = chan <= 2 ? 1 : 3;

            if (opc == req) {
                return cds[j];
            }
        }
    }

    return 0;
}

static inline void gen_am_cond_i32(DisasContext *ctx, TCGv_i32 ret, int chan,
    uint16_t rlp)
{
    TCGv_i32 t0 = tcg_temp_new_i32();

    gen_cond_i32(ctx, t0, extract16(rlp, 0, 7));
    // FIXME: It isn't clear if am can be the only one cond in RLP.
    tcg_gen_xori_i32(ret, t0, GET_BIT(rlp, 7 + chan % 3));

    tcg_temp_free_i32(t0);
}

static inline void gen_mrgc_i32(DisasContext *ctx, int chan, TCGv_i32 ret)
{
    uint16_t rlp = find_mrgc(ctx, chan);

    if (rlp) {
        int psrc = extract16(rlp, 0, 7);

        if (GET_BIT(rlp, 7 + chan % 3)) {
            TCGv_i32 t0 = tcg_temp_new_i32();

            gen_cond_i32(ctx, t0, psrc);
            tcg_gen_setcondi_i32(TCG_COND_EQ, ret, t0, 0);

            tcg_temp_free_i32(t0);
        } else {
            gen_cond_i32(ctx, ret, psrc);
        }
    } else {
        /* Undefined behavior if MRGC is not provided but CPU returns src2. */
        tcg_gen_movi_i32(ret, 0);
    }
}

static void gen_is_loop_end_i32(DisasContext *ctx, TCGv_i32 ret)
{
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i64 t1 = tcg_temp_new_i64();
    TCGv_i32 t2 = tcg_temp_new_i32();
    TCGv_i32 t3 = tcg_temp_new_i32();

    tcg_gen_setcondi_i32(TCG_COND_EQ, t0, cpu_lsr_ecnt, 0);
    tcg_gen_setcondi_i64(TCG_COND_LTU, t1, cpu_lsr_lcnt, 2);
    tcg_gen_extrl_i64_i32(t2, t1);
    tcg_gen_and_i32(t3, t0, t2);
    tcg_gen_and_i32(ret, t3, cpu_lsr_vlc);

    tcg_temp_free_i32(t3);
    tcg_temp_free_i32(t2);
    tcg_temp_free_i64(t1);
    tcg_temp_free_i32(t0);
}

#define IMPL_GEN_OPN(name, S, op) \
    static inline void name(temp(S) ret, temp(S) src1, temp(S) src2) \
    { \
        temp(S) t0 = temp_new(S); \
        \
        call(S, tcg_gen_not, t0, src2); \
        op(ret, src1, t0); \
        \
        temp_free(S, t0); \
    }

IMPL_GEN_OPN(gen_andn_i32, s, tcg_gen_and_i32)
IMPL_GEN_OPN(gen_andn_i64, d, tcg_gen_and_i64)
IMPL_GEN_OPN(gen_orn_i32,  s, tcg_gen_or_i32)
IMPL_GEN_OPN(gen_orn_i64,  d, tcg_gen_or_i64)
IMPL_GEN_OPN(gen_xorn_i32, s, tcg_gen_xor_i32)
IMPL_GEN_OPN(gen_xorn_i64, d, tcg_gen_xor_i64)

#define IMPL_GEN_MASK(S, T, L) \
    static inline void glue(gen_mask_, S)(T ret, T size) \
    { \
        T t0 = glue(tcg_const_, S)(1); \
        T t1 = glue(tcg_temp_new_, S)(); \
        glue(tcg_gen_shl_, S)(t1, t0, size); \
        glue(tcg_gen_subi_, S)(ret, t1, 1); \
        glue(tcg_temp_free_, S)(t1); \
        glue(tcg_temp_free_, S)(t0); \
    } \

IMPL_GEN_MASK(i64, TCGv_i64, 64)
IMPL_GEN_MASK(i32, TCGv_i32, 32)

#define IMPL_GEN_GETF_SIGN(S, T) \
    static inline void glue(gen_getf_sign_, S)(T ret, T val, T len, \
        T offset, T byte) \
    { \
       T z = glue(tcg_const_, S)(0); \
       T ones = glue(tcg_const_, S)(-1); \
       T t0 = glue(tcg_temp_new_, S)(); \
       T t1 = glue(tcg_temp_new_, S)(); \
       T t2 = glue(tcg_temp_new_, S)(); \
       T t3 = glue(tcg_temp_new_, S)(); \
       T t4 = glue(tcg_temp_new_, S)(); \
       T t5 = glue(tcg_temp_new_, S)(); \
       T t6 = glue(tcg_temp_new_, S)(); \
       T t7 = glue(tcg_temp_new_, S)(); \
       /* sign = (x >> (byte * 8 + ((offset + len - 1) & 7))) & 1 */ \
       glue(tcg_gen_add_, S)(t0, offset, len); \
       glue(tcg_gen_subi_, S)(t1, t0, 1); \
       glue(tcg_gen_andi_, S)(t2, t1, 7); \
       glue(tcg_gen_muli_, S)(t3, byte, 8); \
       glue(tcg_gen_add_, S)(t4, t3, t2); \
       glue(tcg_gen_shr_, S)(t5, val, t4); \
       glue(tcg_gen_andi_, S)(t6, t5, 1); \
       glue(tcg_gen_shl_, S)(t7, ones, len); \
       glue(tcg_gen_movcond_, S)(TCG_COND_NE, ret, t6, z, t7, z); \
       glue(tcg_temp_free_, S)(t7); \
       glue(tcg_temp_free_, S)(t6); \
       glue(tcg_temp_free_, S)(t5); \
       glue(tcg_temp_free_, S)(t4); \
       glue(tcg_temp_free_, S)(t3); \
       glue(tcg_temp_free_, S)(t2); \
       glue(tcg_temp_free_, S)(t1); \
       glue(tcg_temp_free_, S)(t0); \
       glue(tcg_temp_free_, S)(ones); \
       glue(tcg_temp_free_, S)(z); \
    }

IMPL_GEN_GETF_SIGN(i64, TCGv_i64)
IMPL_GEN_GETF_SIGN(i32, TCGv_i32)

#define IMPL_GEN_GETF(NAME, S, T, OFFSET, LEN, BYTE, N) \
    static inline void NAME(T ret, T src1, T src2) \
    { \
        T z = glue(tcg_const_, S)(0); \
        T offset = glue(tcg_temp_new_, S)(); \
        T len = glue(tcg_temp_new_, S)(); \
        T sign = glue(tcg_temp_new_, S)(); \
        T byte = glue(tcg_temp_new_, S)(); \
        T t0 = glue(tcg_temp_new_, S)(); \
        T t1 = glue(tcg_temp_new_, S)(); \
        T t2 = glue(tcg_temp_new_, S)(); \
        T t3 = glue(tcg_temp_new_, S)(); \
        T t4 = glue(tcg_temp_new_, S)(); \
        glue(tcg_gen_extract_, S)(offset, src2, 0, OFFSET); \
        glue(tcg_gen_extract_, S)(len, src2, 6, LEN); \
        glue(tcg_gen_extract_, S)(sign, src2, 12, 1); \
        glue(tcg_gen_extract_, S)(byte, src2, 13, BYTE); \
        glue(tcg_gen_rotr_, S)(t0, src1, offset); \
        glue(gen_mask_, S)(t1, len); \
        glue(tcg_gen_and_, S)(t2, t0, t1); \
        glue(gen_getf_sign_, S)(t3, src1, len, offset, byte); \
        glue(tcg_gen_or_, S)(t4, t3, t2); \
        glue(tcg_gen_movcond_, S)(TCG_COND_NE, ret, sign, z, t4, t2); \
        glue(tcg_temp_free_, S)(t4); \
        glue(tcg_temp_free_, S)(t3); \
        glue(tcg_temp_free_, S)(t2); \
        glue(tcg_temp_free_, S)(t1); \
        glue(tcg_temp_free_, S)(t0); \
        glue(tcg_temp_free_, S)(byte); \
        glue(tcg_temp_free_, S)(sign); \
        glue(tcg_temp_free_, S)(len); \
        glue(tcg_temp_free_, S)(offset); \
        glue(tcg_temp_free_, S)(z); \
    }

IMPL_GEN_GETF(gen_getfd, i64, TCGv_i64, 6, 6, 3, 64)
IMPL_GEN_GETF(gen_getfs, i32, TCGv_i32, 5, 5, 2, 32)

#define gen_getf_i64 gen_getfd
#define gen_getf_i32 gen_getfs

#define IMPL_GEN_EXTRACT_SIGN(name, S, LEN) \
    static void name(temp(S) ret, temp(S) val, temp(S) len) \
    { \
        temp(S) t0 = temp_new(S); \
        temp(S) t1 = temp_new(S); \
        \
        call(S, tcg_gen_rotr, t0, val, len); \
        call(S, tcg_gen_sari, t1, t0, LEN); \
        call(S, tcg_gen_shl, ret, t1, len); \
        \
        temp_free(S, t1); \
        temp_free(S, t0); \
    }

IMPL_GEN_EXTRACT_SIGN(gen_extract_sign_i32, s, 31)
IMPL_GEN_EXTRACT_SIGN(gen_extract_sign_i64, d, 63)

#define IMPL_GEN_GETFZ(name, S, OFFSET, LEN) \
    static void name(temp(S) ret, temp(S) s1, temp(S) s2) \
    { \
        temp(S) z = call(S, tcg_const, 0); \
        temp(S) off = temp_new(S); \
        temp(S) len = temp_new(S); \
        temp(S) sign = temp_new(S); \
        temp(S) zlen = temp_new(S); \
        temp(S) t0 = temp_new(S); \
        temp(S) t1 = temp_new(S); \
        temp(S) t2 = temp_new(S); \
        \
        call(S, tcg_gen_extract, off, s2, 0, OFFSET); \
        call(S, tcg_gen_extract, len, s2, 6, LEN); \
        call(S, tcg_gen_extract, sign, s2, 12, 1); \
        call(S, tcg_gen_extract, zlen, s2, 13, 3); \
        \
        call(S, tcg_gen_rotr, t0, s1, off); \
        call(S, gen_mask, t1, len); \
        call(S, tcg_gen_and, t1, t0, t1); \
        call(S, gen_extract_sign, t2, t0, len); \
        call(S, tcg_gen_or, t2, t1, t2); \
        call(S, tcg_gen_movcond, TCG_COND_NE, ret, sign, z, t2, t1); \
        call(S, tcg_gen_shr, ret, ret, zlen); \
        call(S, tcg_gen_shl, ret, ret, zlen); \
        \
        temp_free(S, t2); \
        temp_free(S, t1); \
        temp_free(S, t0); \
        temp_free(S, zlen); \
        temp_free(S, sign); \
        temp_free(S, len); \
        temp_free(S, off); \
        temp_free(S, z); \
    }

IMPL_GEN_GETFZ(gen_getfzs, s, 5, 5)
IMPL_GEN_GETFZ(gen_getfzd, d, 6, 6)

#define IMPL_GEN_BSWAP8_PARTIAL(name, S) \
    static void name(temp(S) ret, temp(S) arg, int n) \
    { \
        temp(S) t0 = temp_new(S); \
        temp(S) t1 = temp_new(S); \
        uint8_t c = (0x0f3355 >> ((n >> 1) * 8)) & 0xff; \
        uint64_t mask = c * 0x0101010101010101UL; \
        \
        call(S, tcg_gen_shri, t0, arg, n); \
        call(S, tcg_gen_andi, t0, t0,  mask); \
        call(S, tcg_gen_andi, t1, arg, mask); \
        call(S, tcg_gen_shli, t1, t1,  n); \
        call(S, tcg_gen_or, ret, t0, t1); \
        \
        temp_free(S, t1); \
        temp_free(S, t0); \
    }

IMPL_GEN_BSWAP8_PARTIAL(gen_bswap8_partial_i64, d)
IMPL_GEN_BSWAP8_PARTIAL(gen_bswap8_partial_i32, s)

static void gen_bitrevs(TCGv_i32 ret, TCGv_i32 src1) {
    tcg_gen_bswap32_i32(ret, src1);
    gen_bswap8_partial_i32(ret, ret, 4);
    gen_bswap8_partial_i32(ret, ret, 2);
    gen_bswap8_partial_i32(ret, ret, 1);
}

static void gen_bitrevd(TCGv_i64 ret, TCGv_i64 src1) {
    tcg_gen_bswap64_i64(ret, src1);
    gen_bswap8_partial_i64(ret, ret, 4);
    gen_bswap8_partial_i64(ret, ret, 2);
    gen_bswap8_partial_i64(ret, ret, 1);
}

static void gen_lzcnts(TCGv_i32 ret, TCGv_i32 src1) {
    tcg_gen_clzi_i32(ret, src1, 32);
}

static void gen_lzcntd(TCGv_i64 ret, TCGv_i64 src1) {
    tcg_gen_clzi_i64(ret, src1, 64);
}

#define IMPL_GEN_OP_C(name, op, cond) \
    static void name(TCGv_i64 ret, TCGv_i64 s1, TCGv_i64 s2, TCGv_i32 s3) \
    { \
        TCGv_i32 t0 = tcg_temp_new_i32(); \
        TCGv_i64 t1 = tcg_temp_new_i64(); \
        \
        tcg_gen_andi_i32(t0, s3, 1); \
        tcg_gen_extu_i32_i64(t1, t0); \
        op(ret, s1, s2); \
        op(ret, ret, t1); \
        \
        tcg_temp_free_i64(t1); \
        tcg_temp_free_i32(t0); \
    } \
    \
    static void glue(name, _c)(TCGv_i64 ret, TCGv_i64 s1, TCGv_i64 s2, \
        TCGv_i32 s3) \
    { \
        TCGv_i64 t0 = tcg_temp_new_i64(); \
        \
        name(t0, s1, s2, s3); \
        tcg_gen_setcond_i64(cond, ret, t0, s1); \
        \
        tcg_temp_free_i64(t0); \
    }

IMPL_GEN_OP_C(gen_addcd, tcg_gen_add_i64, TCG_COND_LTU)
IMPL_GEN_OP_C(gen_subcd, tcg_gen_sub_i64, TCG_COND_GTU)

#define IMPL_GEN_PSHIFT(name, op) \
    static void name(TCGv_i64 ret, TCGv_i64 src1, TCGv_i64 src2) \
    { \
        TCGv_i64 t0 = tcg_const_i64(64); \
        TCGv_i64 t1 = tcg_const_i64(0); \
        \
        op(ret, src1, src2); \
        tcg_gen_movcond_i64(TCG_COND_LTU, ret, src2, t0, ret, t1); \
        \
        tcg_temp_free_i64(t1); \
        tcg_temp_free_i64(t0); \
    }

IMPL_GEN_PSHIFT(gen_pslld, tcg_gen_shl_i64)
IMPL_GEN_PSHIFT(gen_psrld, tcg_gen_shr_i64)

#define gen_psrcd tcg_gen_rotr_i64

static void gen_psrcw(TCGv_i64 ret, TCGv_i64 src1, TCGv_i64 src2)
{
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i32 t1 = tcg_temp_new_i32();
    TCGv_i32 t2 = tcg_temp_new_i32();

    tcg_gen_extr_i64_i32(t0, t1, src1);
    tcg_gen_extrl_i64_i32(t2, src2);
    tcg_gen_rotr_i32(t0, t0, t2);
    tcg_gen_rotr_i32(t1, t1, t2);
    tcg_gen_concat_i32_i64(ret, t0, t1);

    tcg_temp_free_i32(t2);
    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
}

static void gen_pmullw(TCGv_i64 ret, TCGv_i64 src1, TCGv_i64 src2)
{
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i32 t1 = tcg_temp_new_i32();
    TCGv_i32 t2 = tcg_temp_new_i32();
    TCGv_i32 t3 = tcg_temp_new_i32();

    tcg_gen_extr_i64_i32(t0, t1, src1);
    tcg_gen_extr_i64_i32(t2, t3, src2);
    tcg_gen_mul_i32(t0, t0, t2);
    tcg_gen_mul_i32(t1, t1, t3);
    tcg_gen_concat_i32_i64(ret, t0, t1);

    tcg_temp_free_i32(t3);
    tcg_temp_free_i32(t2);
    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
}

static void gen_sm_i32(bool sm, TCGv_i32 ret, TCGv_i32 ret_tag, Exception excp)
{
    if (sm) {
        tcg_gen_movi_i32(ret_tag, E2K_TAG_NON_NUMBER32);
        tcg_gen_movi_i32(ret, 0);
    } else {
        gen_exception(excp);
    }
}

static void gen_brchecki_i32(bool sm, TCGv_i32 ret, TCGv_i32 ret_tag,
    TCGCond cond, TCGv_i32 arg0, uint32_t arg1, TCGLabel *l, Exception excp)
{
    TCGLabel *l0 = gen_new_label();
    tcg_gen_brcondi_i32(cond, arg0, arg1, l0);
    gen_sm_i32(sm, ret, ret_tag, excp);
    tcg_gen_br(l);
    gen_set_label(l0);
}

static void gen_brchecki_i32_i64(bool sm, TCGv_i32 ret, TCGv_i32 ret_tag,
    TCGCond cond, TCGv_i64 arg0, uint64_t arg1, TCGLabel *l, Exception excp)
{
    TCGLabel *l0 = gen_new_label();
    tcg_gen_brcondi_i64(cond, arg0, arg1, l0);
    gen_sm_i32(sm, ret, ret_tag, excp);
    tcg_gen_br(l);
    gen_set_label(l0);
}

#define GEN_OP_DIVX(name, op, checks) \
    static void glue(gen_, name)(TCGv_i32 ret, TCGv_i32 ret_tag, TCGv_i32 tag, \
        TCGv_i64 src1, TCGv_i32 src2, bool sm) { \
        TCGLabel *l0 = gen_new_label(); \
        TCGv_i64 t0 = tcg_temp_new_i64(); \
        TCGv_i64 t1 = tcg_temp_local_new_i64(); \
        gen_brchecki_i32(sm, ret, ret_tag, \
            TCG_COND_NE, src2, 0, l0, EXCP_DIV); \
        tcg_gen_extu_i32_i64(t0, src2); \
        op(t1, src1, t0); \
        checks \
        tcg_gen_extrl_i64_i32(ret, t1); \
        gen_set_label(l0); \
        tcg_temp_free_i64(t1); \
        tcg_temp_free_i64(t0); \
    }

GEN_OP_DIVX(udivx, tcg_gen_divu_i64, { \
    gen_brchecki_i32_i64(sm, ret, ret_tag, \
        TCG_COND_LEU, t1, UINT32_MAX, l0, EXCP_DIV); \
})
GEN_OP_DIVX(sdivx, tcg_gen_div_i64, { \
    gen_brchecki_i32_i64(sm, ret, ret_tag, \
        TCG_COND_LE, t1, INT32_MAX, l0, EXCP_DIV); \
    gen_brchecki_i32_i64(sm, ret, ret_tag, \
        TCG_COND_GE, t1, INT32_MIN, l0, EXCP_DIV); \
})

#define GEN_OP_MODX(name, op) \
    static void glue(gen_, name)(TCGv_i32 ret, TCGv_i32 tag, TCGv_i32 ret_tag, \
        TCGv_i64 src1, TCGv_i32 src2, bool sm) { \
        TCGLabel *l0 = gen_new_label(); \
        TCGv_i64 t0 = tcg_temp_new_i64(); \
        TCGv_i64 t1 = tcg_temp_new_i64(); \
        gen_brchecki_i32(sm, ret, ret_tag, \
            TCG_COND_NE, src2, 0, l0, EXCP_DIV); \
        tcg_gen_extu_i32_i64(t0, src2); \
        /* FIXME: must gen exception/tag on overflow */ \
        op(t1, src1, t0); \
        tcg_gen_extrl_i64_i32(ret, t1); \
        gen_set_label(l0); \
        tcg_temp_free_i64(t1); \
        tcg_temp_free_i64(t0); \
    }

GEN_OP_MODX(umodx, tcg_gen_remu_i64)
GEN_OP_MODX(smodx, tcg_gen_rem_i64)

#define IMPL_GEN_PARITY8(name, S) \
    static void name(temp(S) ret, temp(S) arg) \
    { \
        call(S, tcg_gen_andi, ret, arg, 0xff); \
        call(S, tcg_gen_ctpop, ret, ret); \
        call(S, tcg_gen_andi, ret, ret, 1); \
        call(S, tcg_gen_setcondi, TCG_COND_EQ, ret, ret, 0); \
    }

IMPL_GEN_PARITY8(gen_parity8_i32, s)
IMPL_GEN_PARITY8(gen_parity8_i64, d)

#define IMPL_CMPO(name, S) \
    static void name(temp(S) ret, temp(S) src1, temp(S) src2) \
    { \
        temp(S) t0 = temp_new(S); \
        temp(S) t1 = temp_new(S); \
        temp(S) t2 = temp_new(S); \
        \
        call(S, tcg_gen_sub, t0, src1, src2); \
        call(S, tcg_gen_xor, t1, src1, t0); \
        call(S, gen_xorn, t2, t0, src2); \
        call(S, tcg_gen_and, ret, t1, t2); \
        call(S, tcg_gen_setcondi, TCG_COND_LT, ret, ret, 0); \
        \
        temp_free(S, t2); \
        temp_free(S, t1); \
        temp_free(S, t0); \
    }

IMPL_CMPO(gen_cmposb, s)
IMPL_CMPO(gen_cmpodb, d)

#define IMPL_CMPS(name, S) \
    static void name(temp(S) ret, temp(S) src1, temp(S) src2) \
    { \
        call(S, tcg_gen_sub, ret, src1, src2); \
        call(S, tcg_gen_setcondi, TCG_COND_LT, ret, ret, 0); \
    }

IMPL_CMPS(gen_cmpssb, s)
IMPL_CMPS(gen_cmpsdb, d)

#define IMPL_CMPP(name, S) \
    static void name(temp(S) ret, temp(S) src1, temp(S) src2) \
    { \
        call(S, tcg_gen_sub, ret, src1, src2); \
        call(S, gen_parity8, ret, ret); \
    }

IMPL_CMPP(gen_cmppsb, s)
IMPL_CMPP(gen_cmppdb, d)

#define IMPL_CMP(name, S, cond) \
    static void name(temp(S) ret, temp(S) src1, temp(S) src2) \
    { \
        call(S, tcg_gen_setcond, cond, ret, src1, src2); \
    }

IMPL_CMP(gen_cmpbsb,  s, TCG_COND_LTU)
IMPL_CMP(gen_cmpesb,  s, TCG_COND_EQ)
IMPL_CMP(gen_cmpbesb, s, TCG_COND_LEU)
IMPL_CMP(gen_cmplsb,  s, TCG_COND_LT)
IMPL_CMP(gen_cmplesb, s, TCG_COND_LE)

IMPL_CMP(gen_cmpbdb,  d, TCG_COND_LTU)
IMPL_CMP(gen_cmpedb,  d, TCG_COND_EQ)
IMPL_CMP(gen_cmpbedb, d, TCG_COND_LEU)
IMPL_CMP(gen_cmpldb,  d, TCG_COND_LT)
IMPL_CMP(gen_cmpledb, d, TCG_COND_LE)

#define IMPL_GEN_CMPANDP(name, S) \
    static void name(temp(S) ret, temp(S) src1, temp(S) src2) \
    { \
        call(S, tcg_gen_and, ret, src1, src2); \
        call(S, gen_parity8, ret, ret); \
    }

IMPL_GEN_CMPANDP(gen_cmpandpsb, s)
IMPL_GEN_CMPANDP(gen_cmpandpdb, d)

#define IMPL_GEN_CMPAND(name, S, cond) \
    static void name(temp(S) ret, temp(S) src1, temp(S) src2) \
    { \
        call(S, tcg_gen_and, ret, src1, src2); \
        call(S, tcg_gen_setcondi, cond, ret, ret, 0); \
    }

IMPL_GEN_CMPAND(gen_cmpandesb,  s, TCG_COND_EQ)
IMPL_GEN_CMPAND(gen_cmpandssb,  s, TCG_COND_LT)
IMPL_GEN_CMPAND(gen_cmpandlesb, s, TCG_COND_LE)

IMPL_GEN_CMPAND(gen_cmpandedb,  d, TCG_COND_EQ)
IMPL_GEN_CMPAND(gen_cmpandsdb,  d, TCG_COND_LT)
IMPL_GEN_CMPAND(gen_cmpandledb, d, TCG_COND_LE)

#define glue4(a, b, c, d) glue(glue(a, b), glue(c, d))

static inline void gen_merge_i32(TCGv_i32 ret, TCGv_i32 src1, TCGv_i32 src2,
    TCGv_i32 cond)
{
    TCGv_i32 zero = tcg_const_i32(0);
    tcg_gen_movcond_i32(TCG_COND_EQ, ret, cond, zero, src1, src2);
    tcg_temp_free_i32(zero);
}

static inline void gen_merge_i64(TCGv_i64 ret, TCGv_i64 src1, TCGv_i64 src2,
    TCGv_i32 cond)
{
    TCGv_i64 zero = tcg_const_i64(0);
    TCGv_i64 t0 = tcg_temp_new_i64();

    tcg_gen_extu_i32_i64(t0, cond);
    tcg_gen_movcond_i64(TCG_COND_EQ, ret, t0, zero, src1, src2);

    tcg_temp_free_i64(t0);
    tcg_temp_free_i64(zero);
}

#define IMPL_MERGE(name, S) \
    static void name(Alop *alop) \
    { \
        tagged(S) r = tagged_new(S); \
        tagged(S) a = tagged_new(S); \
        tagged(S) b = tagged_new(S); \
        TCGv_i32 t0 = tcg_temp_new_i32(); \
        \
        gen_tagged_src(1, S, alop, a); \
        gen_tagged_src(2, S, alop, b); \
        gen_result_init(S, alop, r); \
        \
        gen_mrgc_i32(alop->ctx, alop->chan, t0); \
        gen_merge_i32(r.tag, a.tag, b.tag, t0); \
        gen_tag1(S, r, r); \
        call(S, gen_merge, r.val, a.val, b.val, t0); \
        gen_al_result(S, alop, r); \
        \
        tagged_free(S, b); \
        tagged_free(S, a); \
        tagged_free(S, r); \
    }

IMPL_MERGE(gen_merges, s)
IMPL_MERGE(gen_merged, d)

static inline void gen_udivd(TCGv_i64 ret, TCGv_i32 ret_tag, TCGv_i32 tag,
    TCGv_i64 src1, TCGv_i64 src2, bool sm)
{
    if (sm) {
        TCGLabel *l0 = gen_new_label();
        TCGLabel *l1 = gen_new_label();

        tcg_gen_brcondi_i64(TCG_COND_NE, src2, 0, l1);
        tcg_gen_movi_i32(ret_tag, 5);
        tcg_gen_movi_i64(ret, 0);
        tcg_gen_br(l0);
        gen_set_label(l1);
        tcg_gen_mov_i32(ret_tag, tag);
        tcg_gen_divu_i64(ret, src1, src2);
        gen_set_label(l0);
    } else {
        tcg_gen_mov_i32(ret_tag, tag);
        tcg_gen_divu_i64(ret, src1, src2);
    }
}

static inline void gen_udivs(TCGv_i32 ret, TCGv_i32 ret_tag, TCGv_i32 tag,
    TCGv_i32 src1, TCGv_i32 src2, bool sm)
{
    if (sm) {
        TCGLabel *l0 = gen_new_label();
        TCGLabel *l1 = gen_new_label();

        tcg_gen_brcondi_i32(TCG_COND_NE, src2, 0, l1);
        tcg_gen_movi_i32(ret_tag, 5);
        tcg_gen_movi_i32(ret, 0);
        tcg_gen_br(l0);
        gen_set_label(l1);
        tcg_gen_mov_i32(ret_tag, tag);
        tcg_gen_divu_i32(ret, src1, src2);
        gen_set_label(l0);
    } else {
        tcg_gen_mov_i32(ret_tag, tag);
        tcg_gen_divu_i32(ret, src1, src2);
    }
}

#define IMPL_GEN_SDIV(name, S, T, bad_tag) \
    static inline void name(T ret, TCGv_i32 ret_tag, \
        TCGv_i32 tag, T src1, T src2, bool sm) \
    { \
        TCGLabel *l0 = gen_new_label(); \
        TCGLabel *l1 = gen_new_label(); \
        T t0 = glue(tcg_temp_new_i, S)(); \
        T t1 = glue(tcg_temp_new_i, S)(); \
        T t2 = glue(tcg_temp_new_i, S)(); \
        T t3 = glue(tcg_temp_new_i, S)(); \
        T t4 = glue(tcg_temp_new_i, S)(); \
        \
        glue(tcg_gen_brcondi_i, S)(TCG_COND_NE, src2, 0, l1); \
        /* src2 is zero */ \
        tcg_gen_movi_i32(ret_tag, bad_tag); \
        glue(tcg_gen_movi_i, S)(ret, 0); \
        tcg_gen_br(l0); \
        \
        /* src2 is not zero */ \
        gen_set_label(l1); \
        glue(tcg_gen_setcondi_i, S)(TCG_COND_EQ, t0, src1, 1ULL << (S - 1)); \
        glue(tcg_gen_setcondi_i, S)(TCG_COND_EQ, t1, src2, -1); \
        glue(tcg_gen_and_i, S)(t2, t0, t1); \
        glue(tcg_gen_movi_i, S)(t3, 0); \
        glue(tcg_gen_movcond_i, S)(TCG_COND_NE, t4, t2, t3, t2, src2); \
        \
        tcg_gen_mov_i32(ret_tag, tag); \
        glue(tcg_gen_div_i, S)(ret, src1, t4); \
        \
        gen_set_label(l0); \
        \
        glue(tcg_temp_free_i, S)(t4); \
        glue(tcg_temp_free_i, S)(t3); \
        glue(tcg_temp_free_i, S)(t2); \
        glue(tcg_temp_free_i, S)(t1); \
        glue(tcg_temp_free_i, S)(t0); \
    }

IMPL_GEN_SDIV(gen_sdivd, 64, TCGv_i64, E2K_TAG_NON_NUMBER64)
IMPL_GEN_SDIV(gen_sdivs, 32, TCGv_i32, E2K_TAG_NON_NUMBER32)

#define IMPL_GEN_GETTAG(name, S, ext) \
    static void name(Alop *alop) \
    { \
        tagged(S) r = tagged_new(S); \
        tagged(S) b = tagged_new(S); \
        \
        gen_tagged_src(2, S, alop, b); \
        tcg_gen_movi_i32(r.tag, 0); \
        ext(r.val, b.tag); \
        gen_al_result(S, alop, r); \
        \
        tagged_free(S, b); \
        tagged_free(S, r); \
    }

IMPL_GEN_GETTAG(gen_gettagd, d, tcg_gen_extu_i32_i64)
IMPL_GEN_GETTAG(gen_gettags, s, tcg_gen_mov_i32)

#define IMPL_GEN_PUTTAG(name, S1, S2, R, mov) \
    static void name(Alop *alop) \
    { \
        TCGLabel *l0 = gen_new_label(); \
        TCGLabel *l1 = gen_new_label(); \
        tagged(R) r = tagged_local_new(R); \
        tagged(S1) s1 = tagged_local_new(S1); \
        tagged(S2) s2 = tagged_local_new(S2); \
        \
        gen_tagged_src(1, S1, alop, s1); \
        gen_tagged_src(2, S2, alop, s2); \
        gen_result_init(R, alop, r); \
        mov(r.val, s1.val); \
        tcg_gen_brcondi_i32(TCG_COND_EQ, s2.val, 0, l0); \
        gen_tag2(R, r, s1, s2); \
        tcg_gen_brcondi_i32(TCG_COND_EQ, r.tag, 0, l0); \
        /* FIXME: call(R, gen_dst_poison, r.val, r.val, r.tag); */ \
        tcg_gen_br(l1); \
        gen_set_label(l0); \
        tcg_gen_mov_i32(r.tag, s2.val); \
        gen_set_label(l1); \
        gen_al_result(R, alop, r); \
        \
        tagged_free(S2, s2); \
        tagged_free(S1, s1); \
        tagged_free(R, r); \
    }

IMPL_GEN_PUTTAG(gen_puttagqp, q, s, q, gen_movqp)
IMPL_GEN_PUTTAG(gen_puttagd,  d, s, d, tcg_gen_mov_i64)
IMPL_GEN_PUTTAG(gen_puttags,  s, s, s, tcg_gen_mov_i32)

static void gen_insfd_tag_mask(TCGv_i32 ret, TCGv_i32 flags,
    TCGv_i32 tag, int offset)
{
    TCGv_i32 z = tcg_const_i32(0);
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i32 t1 = tcg_temp_new_i32();
    tcg_gen_andi_i32(t0, flags, 1 << offset);
    tcg_gen_andi_i32(t1, tag, 0x3);
    tcg_gen_movcond_i32(TCG_COND_NE, ret, t0, z, t1, tag);
    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
    tcg_temp_free_i32(z);
}

static void gen_insfd_tag(TCGv_i32 ret, TCGv_i64 value,
    TCGv_i32 s1_tag, TCGv_i32 s3_tag)
{
    TCGv_i32 z = tcg_const_i32(0);
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i32 t1 = tcg_temp_new_i32();
    TCGv_i32 t2 = tcg_temp_new_i32();

    tcg_gen_extrl_i64_i32(t0, value);
    gen_insfd_tag_mask(t1, t0, s1_tag, 13);
    gen_insfd_tag_mask(t2, t0, s3_tag, 15);
    tcg_gen_or_i32(ret, t1, t2);

    tcg_temp_free_i32(t2);
    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
    tcg_temp_free_i32(z);
}

static void gen_insfd(Alop *alop)
{
    Tagged_i64 r = tagged_new_i64();
    Tagged_i64 a = tagged_new_i64();
    Tagged_i64 b = tagged_new_i64();
    Tagged_i64 c = tagged_new_i64();
    TCGv_i64 one = tcg_const_i64(1);
    TCGv_i64 offset = tcg_temp_new_i64();
    TCGv_i64 len = tcg_temp_new_i64();
    TCGv_i64 t0 = tcg_temp_new_i64();
    TCGv_i64 t1 = tcg_temp_new_i64();
    TCGv_i64 t2 = tcg_temp_new_i64();
    TCGv_i64 t3 = tcg_temp_new_i64();

    gen_tagged_src1_d(alop, a);
    gen_tagged_src2_d(alop, b);
    gen_tagged_src3_d(alop, c);
    tcg_gen_extract_i64(offset, b.val, 0, 6);
    tcg_gen_extract_i64(len, b.val, 6, 6);
    gen_mask_i64(t0, len);
    tcg_gen_rotr_i64(t1, a.val, offset);
    gen_andn_i64(t2, t1, t0);
    tcg_gen_and_i64(t3, c.val, t0);
    tcg_gen_or_i64(r.val, t2, t3);

    gen_insfd_tag(r.tag, b.val, a.tag, c.tag);
    gen_tag2_i64(r.tag, b.tag, r.tag);
    gen_al_result_d(alop, r);

    tcg_temp_free_i64(t3);
    tcg_temp_free_i64(t2);
    tcg_temp_free_i64(t1);
    tcg_temp_free_i64(t0);
    tcg_temp_free_i64(len);
    tcg_temp_free_i64(offset);
    tcg_temp_free_i64(one);
    tagged_free_i64(c);
    tagged_free_i64(b);
    tagged_free_i64(a);
    tagged_free_i64(r);
}

static void gen_insfs(TCGv_i32 ret, TCGv_i32 src1,
    TCGv_i32 src2, TCGv_i32 src3)
{
    TCGv_i32 one = tcg_const_i32(1);
    TCGv_i32 offset = tcg_temp_new_i32();
    TCGv_i32 len = tcg_temp_new_i32();
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i32 t1 = tcg_temp_new_i32();
    TCGv_i32 t2 = tcg_temp_new_i32();
    TCGv_i32 t3 = tcg_temp_new_i32();
    TCGv_i32 t4 = tcg_temp_new_i32();
    TCGv_i32 t5 = tcg_temp_new_i32();

    tcg_gen_extract_i32(offset, src2, 0, 6);
    tcg_gen_extract_i32(len, src2, 6, 6);
    tcg_gen_shl_i32(t0, one, len);
    tcg_gen_subi_i32(t1, t0, 1);
    tcg_gen_rotr_i32(t2, src1, offset);
    tcg_gen_not_i32(t3, t1);
    tcg_gen_and_i32(t4, t2, t3);
    tcg_gen_and_i32(t5, src3, t1);
    tcg_gen_or_i32(ret, t4, t5);

    tcg_temp_free_i32(t5);
    tcg_temp_free_i32(t4);
    tcg_temp_free_i32(t3);
    tcg_temp_free_i32(t2);
    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
    tcg_temp_free_i32(len);
    tcg_temp_free_i32(offset);
    tcg_temp_free_i32(one);
}

static inline void gen_umulx(TCGv_i64 ret, TCGv_i32 src1, TCGv_i32 src2)
{
    TCGv_i32 l = tcg_temp_new_i32();
    TCGv_i32 h = tcg_temp_new_i32();

    tcg_gen_mulu2_i32(l, h, src1, src2);
    tcg_gen_concat_i32_i64(ret, l, h);

    tcg_temp_free_i32(h);
    tcg_temp_free_i32(l);
}

static inline void gen_smulx(TCGv_i64 ret, TCGv_i32 src1, TCGv_i32 src2)
{
    TCGv_i32 l = tcg_temp_new_i32();
    TCGv_i32 h = tcg_temp_new_i32();

    tcg_gen_muls2_i32(l, h, src1, src2);
    tcg_gen_concat_i32_i64(ret, l, h);

    tcg_temp_free_i32(h);
    tcg_temp_free_i32(l);
}

static inline void gen_umulhd(TCGv_i64 ret, TCGv_i64 src1, TCGv_i64 src2)
{
    TCGv_i64 t0 = tcg_temp_new_i64();
    tcg_gen_mulu2_i64(t0, ret, src1, src2);
    tcg_temp_free_i64(t0);
}

static inline void gen_smulhd(TCGv_i64 ret, TCGv_i64 src1, TCGv_i64 src2)
{
    TCGv_i64 t0 = tcg_temp_new_i64();
    tcg_gen_muls2_i64(t0, ret, src1, src2);
    tcg_temp_free_i64(t0);
}

static void gen_rrd(Alop *alop)
{
    Tagged_i64 r = tagged_new_i64();
    TCGv_i32 t0 = tcg_const_i32(alop->als.src1);

    gen_save_cpu_state(alop->ctx);
    gen_helper_state_reg_get(r.val, cpu_env, t0);
    tcg_gen_movi_i32(r.tag, 0);
    gen_al_result_d(alop, r);

    tcg_temp_free_i32(t0);
    tagged_free_i64(r);
}

static inline void gen_state_reg_write(Alop *alop, TCGv_i64 value)
{
    TCGv_i32 t0 = tcg_const_i32(alop->als.dst);

    gen_helper_state_reg_set(cpu_env, value, t0);
    tcg_temp_free_i32(t0);
}

static void gen_rws(Alop *alop)
{
    Tagged_i32 s2 = tagged_new_i32();
    TCGv_i64 t0 = tcg_temp_new_i64();

    gen_tagged_src2_s(alop, s2);
    gen_delayed_alop_tag_check(alop, s2.tag);
    tcg_gen_extu_i32_i64(t0, s2.val);
    gen_state_reg_write(alop, t0);
    tcg_temp_free_i64(t0);
    tagged_free_i32(s2);
}

static void gen_rwd(Alop *alop)
{
    Tagged_i64 s2 = tagged_new_i64();

    gen_tagged_src2_d(alop, s2);
    gen_delayed_alop_tag_check(alop, s2.tag);
    gen_state_reg_write(alop, s2.val);
    tagged_free_i64(s2);
}

#define IMPL_GEN_MOV(name, S, code) \
    static void name(Alop *alop) \
    { \
        tagged(S) b = tagged_new(S); \
        \
        gen_tagged_src(2, S, alop, b); \
        { code; } \
        gen_al_result(S, alop, b); \
        \
        tagged_free(S, b); \
    }

static void gen_qpair_check(Alop *alop)
{
    if (!check_qr(alop->als.src2, alop->chan)
        || !check_qr(alop->als.dst, alop->chan))
    {
        gen_tr_excp_illopc(alop->ctx);
    }
}

IMPL_GEN_MOV(gen_movts,  s, { })
IMPL_GEN_MOV(gen_movtd,  d, { })
IMPL_GEN_MOV(gen_movtq,  d, { gen_qpair_check(alop); })
IMPL_GEN_MOV(gen_movtcs, s, { })
IMPL_GEN_MOV(gen_movtcd, d, { })
IMPL_GEN_MOV(gen_movtcq, d, { gen_qpair_check(alop); })

static void gen_getpl(Alop *alop)
{
#ifdef TARGET_E2K32
    Tagged_i64 r = tagged_new_i64();
    Tagged_i32 b = tagged_new_i32();

    // TODO: CUD
    gen_tagged_src2_s(alop, b);
    gen_tag1_i64(r.tag, b.tag);
    tcg_gen_extu_i32_i64(r.val, b.val);
    gen_al_result_d(alop, r);

    tagged_free_i32(b);
    tagged_free_i64(r);
#else /* !TARGET_E2K32 */
    // TODO: getpl 64-bit
    e2k_todo_illop(alop->ctx, "getpl");
#endif
}

static void gen_movif(TCGv_ptr ret, TCGv_i64 lo, TCGv_i32 hi)
{
    TCGv_i64 t0 = tcg_temp_new_i64();

    tcg_gen_extu_i32_i64(t0, hi);
    tcg_gen_st_i64(lo, ret, offsetof(E2KReg, lo));
    tcg_gen_st16_i64(t0, ret, offsetof(E2KReg, hi));

    tcg_temp_free_i64(t0);
}

static void gen_movfi(TCGv_i32 ret, TCGv_ptr arg)
{
    tcg_gen_ld16u_i32(ret, arg, offsetof(E2KReg, hi));
}

#define IMPL_MOVX(name, op) \
    static void name(TCGv_ptr ret, TCGv_ptr arg) \
    { \
        TCGv_i64 lo = tcg_temp_new_i64(); \
        TCGv_i64 hi = tcg_temp_new_i64(); \
        \
        tcg_gen_ld_i64(lo, arg, offsetof(E2KReg, lo)); \
        tcg_gen_ld16u_i64(hi, arg, offsetof(E2KReg, hi)); \
        op(hi, hi); \
        tcg_gen_st_i64(lo, ret, offsetof(E2KReg, lo)); \
        tcg_gen_st_i64(hi, ret, offsetof(E2KReg, hi)); \
    }

#define movxa(r, t) tcg_gen_andi_i64(r, t, 0x7fff)
#define movxc(r, t) tcg_gen_xori_i64(r, t, 0x8000)

IMPL_MOVX(gen_movx,  tcg_gen_mov_i64);
IMPL_MOVX(gen_movxa, movxa);
IMPL_MOVX(gen_movxc, movxc);

static inline void gen_shli2_i64(TCGv_i64 rl, TCGv_i64 rh, TCGv_i64 l,
    TCGv_i64 h, int i)
{
    if (i < 64) {
        TCGv_i64 t0 = tcg_temp_new_i64();
        TCGv_i64 t1 = tcg_temp_new_i64();
        tcg_gen_shri_i64(t0, l, 64 - i);
        tcg_gen_shli_i64(t1, h, i);
        tcg_gen_shli_i64(rl, l, i);
        tcg_gen_or_i64(rh, t1, t0);
        tcg_temp_free_i64(t1);
        tcg_temp_free_i64(t0);
    } else if (i < 128) {
        tcg_gen_movi_i64(rl, 0);
        tcg_gen_shli_i64(rh, l, i - 64);
    } else {
        tcg_gen_movi_i64(rl, 0);
        tcg_gen_movi_i64(rh, 0);
    }
}

static inline void gen_shri2_i64(TCGv_i64 rl, TCGv_i64 rh, TCGv_i64 l,
    TCGv_i64 h, int i)
{
    if (i < 64) {
        TCGv_i64 t0 = tcg_temp_new_i64();
        TCGv_i64 t1 = tcg_temp_new_i64();
        tcg_gen_shli_i64(t0, h, 64 - i);
        tcg_gen_shri_i64(t1, l, i);
        tcg_gen_or_i64(rl, t1, t0);
        tcg_gen_shri_i64(rh, h, i);
        tcg_temp_free_i64(t1);
        tcg_temp_free_i64(t0);
    } else if (i < 128) {
        tcg_gen_shri_i64(rl, h, i - 64);
        tcg_gen_movi_i64(rh, 0);
    } else {
        tcg_gen_movi_i64(rl, 0);
        tcg_gen_movi_i64(rh, 0);
    }
}

static inline void gen_psllql(TCGv_i64 ret, TCGv_i64 src1,
    TCGv_i64 src2, int i)
{
    tcg_gen_shli_i64(ret, src2, i * 8);
}

static inline void gen_psllqh(TCGv_i64 ret, TCGv_i64 src1,
    TCGv_i64 src2, int i)
{
    TCGv_i64 t0 = tcg_temp_new_i64();
    gen_shli2_i64(t0, ret, src2, src1, i * 8);
    tcg_temp_free_i64(t0);
}

static inline void gen_psrlql(TCGv_i64 ret, TCGv_i64 src1,
    TCGv_i64 src2, int i)
{
    TCGv_i64 t0 = tcg_temp_new_i64();
    gen_shri2_i64(ret, t0, src2, src1, i * 8);
    tcg_temp_free_i64(t0);
}

static inline void gen_psrlqh(TCGv_i64 ret, TCGv_i64 src1,
    TCGv_i64 src2, int i)
{
    tcg_gen_shri_i64(ret, src1, i * 8);
}

static inline void gen_pinsh(TCGv_i64 ret, TCGv_i64 src1,
    TCGv_i64 src2, int i)
{
    if (i < 4) {
        tcg_gen_deposit_i64(ret, src1, src2, i * 16, 16);
    } else {
        tcg_gen_mov_i64(ret, src1);
    }
}

static inline void gen_pextrh(TCGv_i64 ret, TCGv_i64 src1,
    TCGv_i64 src2, int i)
{
    if (i < 4) {
        tcg_gen_extract_i64(ret, src2, i * 16, 16);
    } else if (i < 8) {
        tcg_gen_extract_i64(ret, src1, (i - 4) * 16, 16);
    } else {
        tcg_gen_movi_i64(ret, 0);
    }
}

static inline void gen_pshufw(TCGv_i64 ret, TCGv_i64 src1,
    TCGv_i64 src2, int i)
{
    TCGv_i32 imm8 = tcg_const_i32(i);
    gen_helper_pshufw(ret, src1, src2, imm8);
    tcg_temp_free_i32(imm8);
}

#define IMPL_GEN_PCMPP(name, op, c, i) \
    static void name(TCGv_i64 ret, TCGv_i64 s1, TCGv_i64 s2) \
    { \
        TCGv_i64 t0 = tcg_temp_new_i64(); \
        \
        op(t0, s1, s2); \
        tcg_gen_setcondi_i64(c, ret, t0, i); \
        \
        tcg_temp_free_i64(t0); \
    }

IMPL_GEN_PCMPP(gen_pcmpeqbop, gen_helper_pcmpeqb, TCG_COND_NE, 0)
IMPL_GEN_PCMPP(gen_pcmpeqhop, gen_helper_pcmpeqh, TCG_COND_NE, 0)
IMPL_GEN_PCMPP(gen_pcmpeqwop, gen_helper_pcmpeqw, TCG_COND_NE, 0)
IMPL_GEN_PCMPP(gen_pcmpgtbop, gen_helper_pcmpgtb, TCG_COND_NE, 0)
IMPL_GEN_PCMPP(gen_pcmpgthop, gen_helper_pcmpgth, TCG_COND_NE, 0)
IMPL_GEN_PCMPP(gen_pcmpgtwop, gen_helper_pcmpgtw, TCG_COND_NE, 0)
IMPL_GEN_PCMPP(gen_pcmpeqbap, gen_helper_pcmpeqb, TCG_COND_EQ, -1)
IMPL_GEN_PCMPP(gen_pcmpeqhap, gen_helper_pcmpeqh, TCG_COND_EQ, -1)
IMPL_GEN_PCMPP(gen_pcmpeqwap, gen_helper_pcmpeqw, TCG_COND_EQ, -1)
IMPL_GEN_PCMPP(gen_pcmpgtbap, gen_helper_pcmpgtb, TCG_COND_EQ, -1)
IMPL_GEN_PCMPP(gen_pcmpgthap, gen_helper_pcmpgth, TCG_COND_EQ, -1)
IMPL_GEN_PCMPP(gen_pcmpgtwap, gen_helper_pcmpgtw, TCG_COND_EQ, -1)

#define gen_pcmpeqdop gen_cmpedb
#define gen_pcmpeqdap gen_cmpedb

IMPL_CMP(gen_pcmpgtdop, d, TCG_COND_GT)
IMPL_CMP(gen_pcmpgtdap, d, TCG_COND_GT)

static void gen_qpswitchd(TCGv_ptr ret, TCGv_ptr src2)
{
    TCGv_i64 t0 = tcg_temp_new_i64();
    TCGv_i64 t1 = tcg_temp_new_i64();

    gen_qpunpackdl(t1, t0, src2);
    gen_qppackdl(ret, t0, t1);

    tcg_temp_free_i64(t1);
    tcg_temp_free_i64(t0);
}

static void gen_pswitchw(TCGv_i64 ret, TCGv_i64 val)
{
    TCGv_i64 t0 = tcg_temp_new_i64();

    tcg_gen_shri_i64(t0, val, 32);
    tcg_gen_concat32_i64(ret, t0, val);

    tcg_temp_free_i64(t0);
}

static void gen_qpswitchw(TCGv_ptr ret, TCGv_ptr src2)
{
    TCGv_i64 t0 = tcg_temp_new_i64();
    TCGv_i64 t1 = tcg_temp_new_i64();

    gen_qpunpackdl(t1, t0, src2);
    gen_pswitchw(t0, t0);
    gen_pswitchw(t1, t1);
    gen_qppackdl(ret, t1, t0);

    tcg_temp_free_i64(t1);
    tcg_temp_free_i64(t0);
}

#define IMPL_GEN_ALOPF1_QQQ(name, op) \
    static void name(TCGv_ptr ret, TCGv_ptr s1, TCGv_ptr s2) \
    { \
        TCGv_i64 t0 = tcg_temp_new_i64(); \
        TCGv_i64 t1 = tcg_temp_new_i64(); \
        TCGv_i64 t2 = tcg_temp_new_i64(); \
        TCGv_i64 t3 = tcg_temp_new_i64(); \
        TCGv_i64 t4 = tcg_temp_new_i64(); \
        TCGv_i64 t5 = tcg_temp_new_i64(); \
        \
        gen_qpunpackdl(t0, t1, s1); \
        gen_qpunpackdl(t2, t3, s2); \
        op(t4, t0, t2); \
        op(t5, t1, t3); \
        gen_qppackdl(ret, t4, t5); \
        \
        tcg_temp_free_i64(t5); \
        tcg_temp_free_i64(t4); \
        tcg_temp_free_i64(t3); \
        tcg_temp_free_i64(t2); \
        tcg_temp_free_i64(t1); \
        tcg_temp_free_i64(t0); \
    }

IMPL_GEN_ALOPF1_QQQ(gen_qpand,  tcg_gen_and_i64)
IMPL_GEN_ALOPF1_QQQ(gen_qpandn, gen_andn_i64)
IMPL_GEN_ALOPF1_QQQ(gen_qpor,   tcg_gen_or_i64)
IMPL_GEN_ALOPF1_QQQ(gen_qpxor,  tcg_gen_xor_i64)

IMPL_GEN_ALOPF1_QQQ(gen_qpmaxsb, gen_helper_pmaxsb)
IMPL_GEN_ALOPF1_QQQ(gen_qpmaxsh, gen_helper_pmaxsh)
IMPL_GEN_ALOPF1_QQQ(gen_qpmaxsw, gen_helper_pmaxsw)
IMPL_GEN_ALOPF1_QQQ(gen_qpmaxub, gen_helper_pmaxub)
IMPL_GEN_ALOPF1_QQQ(gen_qpmaxuh, gen_helper_pmaxuh)
IMPL_GEN_ALOPF1_QQQ(gen_qpmaxuw, gen_helper_pmaxuw)
IMPL_GEN_ALOPF1_QQQ(gen_qpminsb, gen_helper_pminsb)
IMPL_GEN_ALOPF1_QQQ(gen_qpminsh, gen_helper_pminsh)
IMPL_GEN_ALOPF1_QQQ(gen_qpminsw, gen_helper_pminsw)
IMPL_GEN_ALOPF1_QQQ(gen_qpminub, gen_helper_pminub)
IMPL_GEN_ALOPF1_QQQ(gen_qpminuh, gen_helper_pminuh)
IMPL_GEN_ALOPF1_QQQ(gen_qpminuw, gen_helper_pminuw)

IMPL_GEN_ALOPF1_QQQ(gen_qpcmpeqb, gen_helper_pcmpeqb)
IMPL_GEN_ALOPF1_QQQ(gen_qpcmpeqd, gen_helper_pcmpeqd)
IMPL_GEN_ALOPF1_QQQ(gen_qpcmpeqh, gen_helper_pcmpeqh)
IMPL_GEN_ALOPF1_QQQ(gen_qpcmpeqw, gen_helper_pcmpeqw)
IMPL_GEN_ALOPF1_QQQ(gen_qpcmpgtb, gen_helper_pcmpgtb)
IMPL_GEN_ALOPF1_QQQ(gen_qpcmpgtd, gen_helper_pcmpgtd)
IMPL_GEN_ALOPF1_QQQ(gen_qpcmpgth, gen_helper_pcmpgth)
IMPL_GEN_ALOPF1_QQQ(gen_qpcmpgtw, gen_helper_pcmpgtw)

IMPL_GEN_ALOPF1_QQQ(gen_qpaddb, tcg_gen_vec_add8_i64)
IMPL_GEN_ALOPF1_QQQ(gen_qpaddh, tcg_gen_vec_add16_i64)
IMPL_GEN_ALOPF1_QQQ(gen_qpaddw, tcg_gen_vec_add32_i64)
IMPL_GEN_ALOPF1_QQQ(gen_qpaddd, tcg_gen_add_i64)

IMPL_GEN_ALOPF1_QQQ(gen_qpsubb, tcg_gen_vec_sub8_i64)
IMPL_GEN_ALOPF1_QQQ(gen_qpsubh, tcg_gen_vec_sub16_i64)
IMPL_GEN_ALOPF1_QQQ(gen_qpsubw, tcg_gen_vec_sub32_i64)
IMPL_GEN_ALOPF1_QQQ(gen_qpsubd, tcg_gen_sub_i64)

IMPL_GEN_ALOPF1_QQQ(gen_qpmullw, gen_pmullw)

IMPL_GEN_ALOPF1_QQQ(gen_qpaddsb,  gen_helper_paddsb)
IMPL_GEN_ALOPF1_QQQ(gen_qpaddsh,  gen_helper_paddsh)
IMPL_GEN_ALOPF1_QQQ(gen_qpaddusb, gen_helper_paddusb)
IMPL_GEN_ALOPF1_QQQ(gen_qpaddush, gen_helper_paddush)
IMPL_GEN_ALOPF1_QQQ(gen_qpsubsb,  gen_helper_psubsb)
IMPL_GEN_ALOPF1_QQQ(gen_qpsubsh,  gen_helper_psubsh)
IMPL_GEN_ALOPF1_QQQ(gen_qpsubusb, gen_helper_psubusb)
IMPL_GEN_ALOPF1_QQQ(gen_qpsubush, gen_helper_psubush)

IMPL_GEN_ALOPF1_QQQ(gen_qpavgusb, gen_helper_pavgusb)
IMPL_GEN_ALOPF1_QQQ(gen_qpavgush, gen_helper_pavgush)

IMPL_GEN_ALOPF1_QQQ(gen_qpmulhh,    gen_helper_pmulhh)
IMPL_GEN_ALOPF1_QQQ(gen_qpmulhrsh,  gen_helper_pmulhrsh)
IMPL_GEN_ALOPF1_QQQ(gen_qpmulhuh,   gen_helper_pmulhuh)
IMPL_GEN_ALOPF1_QQQ(gen_qpmullh,    gen_helper_pmullh)
IMPL_GEN_ALOPF1_QQQ(gen_qpsignb,    gen_helper_psignb)
IMPL_GEN_ALOPF1_QQQ(gen_qpsignh,    gen_helper_psignh)
IMPL_GEN_ALOPF1_QQQ(gen_qpsignw,    gen_helper_psignw)
IMPL_GEN_ALOPF1_QQQ(gen_qpmaddh,    gen_helper_pmaddh)
IMPL_GEN_ALOPF1_QQQ(gen_qpmaddubsh, gen_helper_pmaddubsh)
IMPL_GEN_ALOPF1_QQQ(gen_qpsadbw,    gen_helper_psadbw)

#define IMPL_GEN_ALOPF1_ALT_QQQ(name, op) \
    static void name(TCGv_ptr ret, TCGv_ptr s1, TCGv_ptr s2) \
    { \
        TCGv_i64 t0 = tcg_temp_new_i64(); \
        TCGv_i64 t1 = tcg_temp_new_i64(); \
        TCGv_i64 t2 = tcg_temp_new_i64(); \
        TCGv_i64 t3 = tcg_temp_new_i64(); \
        TCGv_i64 t4 = tcg_temp_new_i64(); \
        TCGv_i64 t5 = tcg_temp_new_i64(); \
        \
        gen_qpunpackdl(t0, t1, s1); \
        gen_qpunpackdl(t2, t3, s2); \
        op(t4, t0, t1); \
        op(t5, t2, t3); \
        gen_qppackdl(ret, t4, t5); \
        \
        tcg_temp_free_i64(t5); \
        tcg_temp_free_i64(t4); \
        tcg_temp_free_i64(t3); \
        tcg_temp_free_i64(t2); \
        tcg_temp_free_i64(t1); \
        tcg_temp_free_i64(t0); \
    }

IMPL_GEN_ALOPF1_ALT_QQQ(gen_qpacksshb, gen_helper_packsshb)
IMPL_GEN_ALOPF1_ALT_QQQ(gen_qpacksswh, gen_helper_packsswh)
IMPL_GEN_ALOPF1_ALT_QQQ(gen_qpackushb, gen_helper_packushb)
IMPL_GEN_ALOPF1_ALT_QQQ(gen_qpackuswh, gen_helper_packuswh)

#define IMPL_GEN_ALOPF1_QDQ(name, op) \
    static void name(TCGv_ptr ret, TCGv_ptr s1, TCGv_i64 s2) \
    { \
        TCGv_i64 t0 = tcg_temp_new_i64(); \
        TCGv_i64 t1 = tcg_temp_new_i64(); \
        \
        gen_qpunpackdl(t0, t1, s1); \
        op(t0, t0, s2); \
        op(t1, t1, s2); \
        gen_qppackdl(ret, t0, t1); \
        \
        tcg_temp_free_i64(t1); \
        tcg_temp_free_i64(t0); \
    }

IMPL_GEN_ALOPF1_QDQ(gen_qpsllh, gen_helper_psllh)
IMPL_GEN_ALOPF1_QDQ(gen_qpsllw, gen_helper_psllw)
IMPL_GEN_ALOPF1_QDQ(gen_qpslld, gen_pslld)
IMPL_GEN_ALOPF1_QDQ(gen_qpsrlh, gen_helper_psrlh)
IMPL_GEN_ALOPF1_QDQ(gen_qpsrlw, gen_helper_psrlw)
IMPL_GEN_ALOPF1_QDQ(gen_qpsrld, gen_psrld)
IMPL_GEN_ALOPF1_QDQ(gen_qpsrah, gen_helper_psrah)
IMPL_GEN_ALOPF1_QDQ(gen_qpsraw, gen_helper_psraw)
IMPL_GEN_ALOPF1_QDQ(gen_qpsrcw, gen_psrcw)
IMPL_GEN_ALOPF1_QDQ(gen_qpsrcd, gen_psrcd)

static void gen_qpsrad_helper(TCGv_i64 ret, TCGv_i64 s1, TCGv_i64 s2)
{
    TCGv_i64 t0 = tcg_const_i64(63);
    TCGv_i64 t1 = tcg_temp_new_i64();

    tcg_gen_movcond_i64(TCG_COND_LTU, t1, s2, t0, s2, t0);
    tcg_gen_sar_i64(ret, s1, t1);

    tcg_temp_free_i64(t1);
    tcg_temp_free_i64(t0);
}

IMPL_GEN_ALOPF1_QDQ(gen_qpsrad, gen_qpsrad_helper)

#define IMPL_GEN_ALOPF21_QQQQ(name, op) \
    static void name(TCGv_ptr ret, TCGv_ptr s1, TCGv_ptr s2, TCGv_ptr s3) \
    { \
        TCGv_i64 t0 = tcg_temp_new_i64(); \
        TCGv_i64 t1 = tcg_temp_new_i64(); \
        TCGv_i64 t2 = tcg_temp_new_i64(); \
        TCGv_i64 t3 = tcg_temp_new_i64(); \
        TCGv_i64 t4 = tcg_temp_new_i64(); \
        TCGv_i64 t5 = tcg_temp_new_i64(); \
        \
        gen_qpunpackdl(t0, t1, s1); \
        gen_qpunpackdl(t2, t3, s2); \
        gen_qpunpackdl(t4, t5, s3); \
        op(t0, t0, t2, t4); \
        op(t1, t1, t3, t5); \
        gen_qppackdl(ret, t0, t1); \
        \
        tcg_temp_free_i64(t5); \
        tcg_temp_free_i64(t4); \
        tcg_temp_free_i64(t3); \
        tcg_temp_free_i64(t2); \
        tcg_temp_free_i64(t1); \
        tcg_temp_free_i64(t0); \
    }

IMPL_GEN_ALOPF21_QQQQ(gen_qpmerge,  gen_helper_pmerge)

#define IMPL_GEN_ALOPF1_ENV_QQQ_2(name, op1, op2) \
    static void name(TCGv_ptr ret, TCGv_env env, TCGv_ptr s1, TCGv_ptr s2) \
    { \
        TCGv_i64 t0 = tcg_temp_new_i64(); \
        TCGv_i64 t1 = tcg_temp_new_i64(); \
        TCGv_i64 t2 = tcg_temp_new_i64(); \
        TCGv_i64 t3 = tcg_temp_new_i64(); \
        \
        gen_qpunpackdl(t0, t1, s1); \
        gen_qpunpackdl(t2, t3, s2); \
        op1(t0, env, t0, t2); \
        op2(t1, env, t1, t3); \
        gen_qppackdl(ret, t0, t1); \
        \
        tcg_temp_free_i64(t3); \
        tcg_temp_free_i64(t2); \
        tcg_temp_free_i64(t1); \
        tcg_temp_free_i64(t0); \
    }

#define IMPL_GEN_ALOPF1_ENV_QQQ(name, op) \
    IMPL_GEN_ALOPF1_ENV_QQQ_2(name, op, op)

IMPL_GEN_ALOPF1_ENV_QQQ(gen_qpfadds, gen_helper_pfadds)
IMPL_GEN_ALOPF1_ENV_QQQ(gen_qpfaddd, gen_helper_faddd)
IMPL_GEN_ALOPF1_ENV_QQQ(gen_qpfsubs, gen_helper_pfsubs)
IMPL_GEN_ALOPF1_ENV_QQQ(gen_qpfsubd, gen_helper_fsubd)
IMPL_GEN_ALOPF1_ENV_QQQ(gen_qpfmuls, gen_helper_pfmuls)
IMPL_GEN_ALOPF1_ENV_QQQ(gen_qpfmuld, gen_helper_fmuld)
IMPL_GEN_ALOPF1_ENV_QQQ(gen_qpfaddsubs, gen_helper_pfaddsubs)
IMPL_GEN_ALOPF1_ENV_QQQ_2(gen_qpfaddsubd, gen_helper_faddd, gen_helper_fsubd)
IMPL_GEN_ALOPF1_ENV_QQQ(gen_qpfmins, gen_helper_pfmins)
IMPL_GEN_ALOPF1_ENV_QQQ(gen_qpfmind, gen_helper_fmind)
IMPL_GEN_ALOPF1_ENV_QQQ(gen_qpfmaxs, gen_helper_pfmaxs)
IMPL_GEN_ALOPF1_ENV_QQQ(gen_qpfmaxd, gen_helper_fmaxd)

IMPL_GEN_ALOPF1_ENV_QQQ(gen_qpfcmpeqs,  gen_helper_pfcmpeqs)
IMPL_GEN_ALOPF1_ENV_QQQ(gen_qpfcmplts,  gen_helper_pfcmplts)
IMPL_GEN_ALOPF1_ENV_QQQ(gen_qpfcmples,  gen_helper_pfcmples)
IMPL_GEN_ALOPF1_ENV_QQQ(gen_qpfcmpuods, gen_helper_pfcmpuods)
IMPL_GEN_ALOPF1_ENV_QQQ(gen_qpfcmpneqs, gen_helper_pfcmpneqs)
IMPL_GEN_ALOPF1_ENV_QQQ(gen_qpfcmpnlts, gen_helper_pfcmpnlts)
IMPL_GEN_ALOPF1_ENV_QQQ(gen_qpfcmpnles, gen_helper_pfcmpnles)
IMPL_GEN_ALOPF1_ENV_QQQ(gen_qpfcmpods,  gen_helper_pfcmpods)
IMPL_GEN_ALOPF1_ENV_QQQ(gen_qpfcmpeqd,  gen_helper_fcmpeqd)
IMPL_GEN_ALOPF1_ENV_QQQ(gen_qpfcmpltd,  gen_helper_fcmpltd)
IMPL_GEN_ALOPF1_ENV_QQQ(gen_qpfcmpled,  gen_helper_fcmpled)
IMPL_GEN_ALOPF1_ENV_QQQ(gen_qpfcmpuodd, gen_helper_fcmpuodd)
IMPL_GEN_ALOPF1_ENV_QQQ(gen_qpfcmpneqd, gen_helper_fcmpneqd)
IMPL_GEN_ALOPF1_ENV_QQQ(gen_qpfcmpnltd, gen_helper_fcmpnltd)
IMPL_GEN_ALOPF1_ENV_QQQ(gen_qpfcmpnled, gen_helper_fcmpnled)
IMPL_GEN_ALOPF1_ENV_QQQ(gen_qpfcmpodd,  gen_helper_fcmpodd)

#define IMPL_GEN_ALOPF1_ENV_DQQ(name, op) \
    static void name(TCGv_ptr ret, TCGv_env env, TCGv_i64 s1, TCGv_ptr s2) \
    { \
        TCGv_i64 t0 = tcg_temp_new_i64(); \
        TCGv_i64 t1 = tcg_temp_new_i64(); \
        \
        gen_qpunpackdl(t0, t1, s2); \
        op(t0, env, s1, t0); \
        op(t1, env, s1, t1); \
        gen_qppackdl(ret, t0, t1); \
        \
        tcg_temp_free_i64(t1); \
        tcg_temp_free_i64(t0); \
    }

IMPL_GEN_ALOPF1_ENV_DQQ(gen_qpfstoifs, gen_helper_pfstoifs)
IMPL_GEN_ALOPF1_ENV_DQQ(gen_qpfdtoifd, gen_helper_fdtoifd)

#define IMPL_GEN_ALOPF2_ENV_QQ(name, op) \
    static void name(TCGv_ptr ret, TCGv_env env, TCGv_ptr s2) \
    { \
        TCGv_i64 t0 = tcg_temp_new_i64(); \
        TCGv_i64 t1 = tcg_temp_new_i64(); \
        \
        gen_qpunpackdl(t0, t1, s2); \
        op(t0, env, t0); \
        op(t1, env, t1); \
        gen_qppackdl(ret, t0, t1); \
        \
        tcg_temp_free_i64(t1); \
        tcg_temp_free_i64(t0); \
    }

IMPL_GEN_ALOPF2_ENV_QQ(gen_qpfstois,   gen_helper_pfstois)
IMPL_GEN_ALOPF2_ENV_QQ(gen_qpfstoistr, gen_helper_pfstoistr)
IMPL_GEN_ALOPF2_ENV_QQ(gen_qpistofs,   gen_helper_pistofs)
IMPL_GEN_ALOPF2_ENV_QQ(gen_qpfdtoid,   gen_helper_fdtoid)
IMPL_GEN_ALOPF2_ENV_QQ(gen_qpfdtoidtr, gen_helper_fdtoidtr)
IMPL_GEN_ALOPF2_ENV_QQ(gen_qpidtofd,   gen_helper_idtofd)

#define IMPL_GEN_PLOG(name, base) \
    static void name(TCGv_i64 ret, uint32_t table, TCGv_i64 s1, \
        TCGv_i64 s2, TCGv_i64 s3) \
    { \
        TCGv_i32 t0 = tcg_const_i32(base + table); \
        gen_helper_plog(ret, t0, s1, s2, s3); \
        tcg_temp_free_i32(t0); \
    }

IMPL_GEN_PLOG(gen_plog_0x00, 0x00)
IMPL_GEN_PLOG(gen_plog_0x80, 0x80)

#define IMPL_GEN_ALOPF21_LOG_QQQQ(name, op) \
    static void name(TCGv_ptr ret, uint32_t opc, TCGv_ptr s1, \
        TCGv_ptr s2, TCGv_ptr s3) \
    { \
        TCGv_i64 t0 = tcg_temp_new_i64(); \
        TCGv_i64 t1 = tcg_temp_new_i64(); \
        TCGv_i64 t2 = tcg_temp_new_i64(); \
        TCGv_i64 t3 = tcg_temp_new_i64(); \
        TCGv_i64 t4 = tcg_temp_new_i64(); \
        TCGv_i64 t5 = tcg_temp_new_i64(); \
        \
        gen_qpunpackdl(t0, t1, s1); \
        gen_qpunpackdl(t2, t3, s2); \
        gen_qpunpackdl(t4, t5, s3); \
        op(t0, opc, t0, t2, t4); \
        op(t1, opc, t1, t3, t5); \
        gen_qppackdl(ret, t0, t1); \
        \
        tcg_temp_free_i64(t5); \
        tcg_temp_free_i64(t4); \
        tcg_temp_free_i64(t3); \
        tcg_temp_free_i64(t2); \
        tcg_temp_free_i64(t1); \
        tcg_temp_free_i64(t0); \
    }

IMPL_GEN_ALOPF21_LOG_QQQQ(gen_qplog_0x00, gen_plog_0x00)
IMPL_GEN_ALOPF21_LOG_QQQQ(gen_qplog_0x80, gen_plog_0x80)

#define IMPL_GEN_ALOPF7_QQB(name, op1, op2) \
    static void name(TCGv_i64 ret, TCGv_ptr s1, TCGv_ptr s2) \
    { \
        TCGv_i64 t0 = tcg_temp_new_i64(); \
        TCGv_i64 t1 = tcg_temp_new_i64(); \
        TCGv_i64 t2 = tcg_temp_new_i64(); \
        TCGv_i64 t3 = tcg_temp_new_i64(); \
        TCGv_i64 t4 = tcg_temp_new_i64(); \
        TCGv_i64 t5 = tcg_temp_new_i64(); \
        \
        gen_qpunpackdl(t0, t1, s1); \
        gen_qpunpackdl(t2, t3, s2); \
        op1(t4, t0, t2); \
        op1(t5, t1, t3); \
        op2(ret, t4, t5); \
        \
        tcg_temp_free_i64(t5); \
        tcg_temp_free_i64(t4); \
        tcg_temp_free_i64(t3); \
        tcg_temp_free_i64(t2); \
        tcg_temp_free_i64(t1); \
        tcg_temp_free_i64(t0); \
    }

IMPL_GEN_ALOPF7_QQB(gen_qpcmpeqbop, gen_pcmpeqbop, tcg_gen_or_i64)
IMPL_GEN_ALOPF7_QQB(gen_qpcmpeqhop, gen_pcmpeqhop, tcg_gen_or_i64)
IMPL_GEN_ALOPF7_QQB(gen_qpcmpeqwop, gen_pcmpeqwop, tcg_gen_or_i64)
IMPL_GEN_ALOPF7_QQB(gen_qpcmpeqdop, gen_pcmpeqdop, tcg_gen_or_i64)
IMPL_GEN_ALOPF7_QQB(gen_qpcmpgtbop, gen_pcmpgtbop, tcg_gen_or_i64)
IMPL_GEN_ALOPF7_QQB(gen_qpcmpgthop, gen_pcmpgthop, tcg_gen_or_i64)
IMPL_GEN_ALOPF7_QQB(gen_qpcmpgtwop, gen_pcmpgtwop, tcg_gen_or_i64)
IMPL_GEN_ALOPF7_QQB(gen_qpcmpgtdop, gen_pcmpgtdop, tcg_gen_or_i64)
IMPL_GEN_ALOPF7_QQB(gen_qpcmpeqbap, gen_pcmpeqbap, tcg_gen_and_i64)
IMPL_GEN_ALOPF7_QQB(gen_qpcmpeqhap, gen_pcmpeqhap, tcg_gen_and_i64)
IMPL_GEN_ALOPF7_QQB(gen_qpcmpeqwap, gen_pcmpeqwap, tcg_gen_and_i64)
IMPL_GEN_ALOPF7_QQB(gen_qpcmpeqdap, gen_pcmpeqdap, tcg_gen_and_i64)
IMPL_GEN_ALOPF7_QQB(gen_qpcmpgtbap, gen_pcmpgtbap, tcg_gen_and_i64)
IMPL_GEN_ALOPF7_QQB(gen_qpcmpgthap, gen_pcmpgthap, tcg_gen_and_i64)
IMPL_GEN_ALOPF7_QQB(gen_qpcmpgtwap, gen_pcmpgtwap, tcg_gen_and_i64)
IMPL_GEN_ALOPF7_QQB(gen_qpcmpgtdap, gen_pcmpgtdap, tcg_gen_and_i64)

static void gen_merge_ptr(TCGv_ptr ret, TCGv_ptr s1, TCGv_ptr s2,
    TCGv_i32 cond)
{
    TCGv_i64 t0 = tcg_const_i64(0);
    TCGv_i64 t1 = tcg_temp_new_i64();
    TCGv_i64 t2 = tcg_temp_new_i64();
    TCGv_i64 t3 = tcg_temp_new_i64();
    TCGv_i64 t4 = tcg_temp_new_i64();
    TCGv_i64 t5 = tcg_temp_new_i64();

    tcg_gen_extu_i32_i64(t1, cond);
    gen_qpunpackdl(t2, t3, s1);
    gen_qpunpackdl(t4, t5, s2);
    tcg_gen_movcond_i64(TCG_COND_EQ, t2, t0, t1, t2, t4);
    tcg_gen_movcond_i64(TCG_COND_EQ, t3, t0, t1, t3, t5);
    gen_qppackdl(ret, t2, t3);

    tcg_temp_free_i64(t5);
    tcg_temp_free_i64(t4);
    tcg_temp_free_i64(t3);
    tcg_temp_free_i64(t2);
    tcg_temp_free_i64(t1);
    tcg_temp_free_i64(t0);
}

IMPL_MERGE(gen_qpmrgp, q)

#define IMPL_GEN_QPCEXT(name, CONST) \
    static void name(TCGv_ptr ret, TCGv_i64 s2) \
    { \
        TCGv_i64 t0 = tcg_const_i64(0x0101010101010101ULL * CONST); \
        \
        gen_qppackdl(ret, t0, s2); \
        \
        tcg_temp_free_i64(t0); \
    }

IMPL_GEN_QPCEXT(gen_qpcext_0x00, 0x00)
IMPL_GEN_QPCEXT(gen_qpcext_0x7f, 0x7f)
IMPL_GEN_QPCEXT(gen_qpcext_0x80, 0x80)
IMPL_GEN_QPCEXT(gen_qpcext_0xff, 0xff)

static MemOp memop_from_mas(uint8_t mas)
{
    MemOp r = 0;
//    int disable_cache = extract(mas, 5, 2);

    assert((mas & 0x7) != 0x7);

    if (mas != 0) {
        r |= extract8(mas, 3, 1) ? MO_BE : MO_LE;
        r |= extract8(mas, 4, 1) ? MO_UNALN : MO_ALIGN;
    } else {
        r = MO_LE | MO_ALIGN;
    }

    return r;
}

static MemOp scan_ld_mas(Alop *alop, bool *skip, bool *save)
{
    int mod = alop->mas & 0x7;
    MemOp memop = 0;

    if (mod == 0x7) {
        int opc = alop->mas >> 3;
        bool warn = false;

        memop |= MO_LE;

        switch (opc) {
        case MAS_OPC_CACHE_FLUSH:
            if (alop->chan == 0) {
                *save = true;
            } else {
                warn = true;
            }
            break;
        default:
            warn = true;
            break;
        }

        if (warn) {
            e2k_todo(alop->ctx, "ld special opc %d on channel %d", opc,
                alop->chan);
        }
    } else {
        bool warn = false;

        memop |= memop_from_mas(alop->mas);

        switch (mod) {
        case 0:
            /* normal load */
            break;
        case 3:
            if (is_chan_25(alop->chan)) {
                /* TODO: DAM */
                /* always go to fixing code */
                if (alop->ctx->mlock) {
                    tcg_gen_movi_i32(alop->ctx->mlock, 1);
                }
            } else {
                warn = true;
            }
            break;
        case 4:
            if (alop->als.sm && is_chan_03(alop->chan)) {
                /* TODO: DAM */
                /* always ignore lock load */
                *skip = true;
            } else if (!alop->als.sm) {
                /* TODO */
            } else {
                warn = true;
            }
            break;
        default:
            warn = true;
            break;
        }

        if (warn) {
            e2k_todo(alop->ctx, "ld mod %d on channel %d", mod, alop->chan);
        }
    }

    return memop;
}

static MemOp scan_st_mas(Alop *alop, bool *skip, bool *check)
{
    int mod = alop->mas & 0x7;
    MemOp memop = 0;

    if (mod == 0x7) {
        int opc = alop->mas >> 3;
        bool warn = false;

        memop |= MO_LE;

        switch (opc) {
        case MAS_OPC_DCACHE_LINE_FLUSH:
        case MAS_OPC_ICACHE_LINE_FLUSH:
            *skip = true;
            break;
        case MAS_OPC_ST_UNKNOWN3:
            if (!alop->als.sm) {
                // TODO: unknown store special opc 3
                *skip = true;
            } else {
                warn = true;
            }
            break;
        default:
            warn = true;
            break;
        }

        if (warn) {
            e2k_todo(alop->ctx, "st special opc %d on channel %d", opc,
                alop->chan);
        }
    } else {
        bool warn = false;

        memop |= memop_from_mas(alop->mas);

        switch (mod) {
        case 0:
            /* normal store */
            break;
        case 2:
            *check = true;
            break;
        default:
            warn = true;
            break;
        }

        if (warn) {
            e2k_todo(alop->ctx, "st mod %d on channel %d", mod, alop->chan);
        }
    }

    return memop;
}

static void gen_probe_read_access(TCGv_i32 ret, TCGv addr, int size,
    int mmu_idx)
{
    TCGv_i32 t0 = tcg_const_i32(size);
    TCGv_i32 t1 = tcg_const_i32(mmu_idx);

    gen_helper_probe_read_access(ret, cpu_env, addr, t0, t1);

    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
}

static void gen_probe_write_access(TCGv_i32 ret, TCGv addr, int size,
    int mmu_idx)
{
    TCGv_i32 t0 = tcg_const_i32(size);
    TCGv_i32 t1 = tcg_const_i32(mmu_idx);

    gen_helper_probe_write_access(ret, cpu_env, addr, t0, t1);

    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
}

static void gen_ld_raw_i64(Alop *alop, TCGv_i32 tag, TCGv addr,
    MemOp memop, bool skip, bool save)
{
    TCGLabel *l0 = gen_new_label();
    Tagged_i64 r = tagged_local_new_i64();

    if (alop->als.sm) {
        TCGLabel *l1 = gen_new_label();
        TCGv_i32 t0 = tcg_temp_new_i32();

        gen_probe_read_access(t0, addr, memop_size(memop), alop->ctx->mmuidx);
        tcg_gen_brcondi_i32(TCG_COND_NE, t0, 0, l1);

        /* address is not available */
        tcg_gen_movi_i32(r.tag, E2K_TAG_NON_NUMBER64);
        tcg_gen_movi_i64(r.val, E2K_LD_RESULT_INVALID);
        tcg_gen_br(l0);

        /* address is available */
        gen_set_label(l1);

        tcg_temp_free_i32(t0);
    }

    gen_tag1_i64(r.tag, tag);
    tcg_gen_qemu_ld_i64(r.val, addr, alop->ctx->mmuidx, memop);
    gen_set_label(l0);

    if (save) {
        /* save value for a further check with st+mod=2 */
        tcg_gen_mov_i64(cpu_last_val0, r.val);
    }

    gen_al_result_d(alop, r);

    tagged_free_i64(r);
}

static void gen_qemu_ld_i128(TCGv_i64 hi, TCGv_i64 lo, TCGv addr, TCGArg idx,
    MemOp memop)
{
    TCGv t0 = tcg_temp_new();

    tcg_gen_addi_tl(t0, addr, 8);

    if (memop & MO_BE) {
        tcg_gen_qemu_ld_i64(hi, addr, idx, memop);
        tcg_gen_qemu_ld_i64(lo, t0, idx, memop);
    } else {
        tcg_gen_qemu_ld_i64(lo, addr, idx, memop);
        tcg_gen_qemu_ld_i64(hi, t0, idx, memop);
    }

    tcg_temp_free(t0);
}

static void gen_ld_raw_i128(Alop *alop, TCGv_i32 tag, TCGv addr,
    MemOp memop, bool skip, bool save)
{
    TCGLabel *l0 = gen_new_label();
    Tagged_ptr r = tagged_local_new_ptr();
    TCGv_i64 t0 = tcg_temp_local_new_i64();
    TCGv_i64 t1 = tcg_temp_local_new_i64();

    gen_result_init_q(alop, r);

    if (alop->als.sm) {
        TCGLabel *l1 = gen_new_label();
        TCGv_i32 t3 = tcg_temp_new_i32();

        gen_probe_read_access(t3, addr, 16, alop->ctx->mmuidx);
        tcg_gen_brcondi_i32(TCG_COND_NE, t3, 0, l1);

        /* address is not available */
        tcg_gen_movi_i32(r.tag, E2K_TAG_NON_NUMBER128);
        tcg_gen_movi_i64(t0, E2K_LD_RESULT_INVALID);
        tcg_gen_movi_i64(t1, E2K_LD_RESULT_INVALID);
        tcg_gen_br(l0);

        /* address is available */
        gen_set_label(l1);

        tcg_temp_free_i32(t3);
    }

    gen_tag1_i128(r.tag, tag);
    gen_qemu_ld_i128(t1, t0, addr, alop->ctx->mmuidx, memop);

    gen_set_label(l0);

    if (save) {
        /* save value for a further check with st+mod=2 */
        tcg_gen_mov_i64(cpu_last_val0, t0);
        tcg_gen_mov_i64(cpu_last_val1, t1);
    }

    gen_qppackdl(r.val, t1, t0);
    gen_al_result_q(alop, r);

    tcg_temp_free_i64(t1);
    tcg_temp_free_i64(t0);
    tagged_free_ptr(r);
}

static void gen_atomic_cmpxchg_i64(Alop *alop, TCGv_i64 value, TCGv addr,
    MemOp memop)
{
    TCGv_i64 t0 = tcg_temp_new_i64();
    TCGv_i64 t1 = tcg_temp_new_i64();

    tcg_gen_atomic_cmpxchg_i64(t0, addr, cpu_last_val0, value,
        alop->ctx->mmuidx, memop);
    tcg_gen_setcond_i64(TCG_COND_NE, t1, t0, cpu_last_val0);
    tcg_gen_extrl_i64_i32(alop->ctx->mlock, t1);

    tcg_temp_free_i64(t1);
    tcg_temp_free_i64(t0);
}

static void gen_atomic_cmpxchg_i128(Alop *alop, TCGv_i64 hi, TCGv_i64 lo,
    TCGv addr, MemOp memop)
{
    TCGv_i64 t0 = tcg_temp_new_i64();
    TCGv_i64 t1 = tcg_temp_new_i64();
    TCGv t2 = tcg_temp_local_new();

    tcg_gen_addi_tl(t2, addr, 8);

    // FIXME: temp solution
    if (memop & MO_BE) {
        tcg_gen_atomic_cmpxchg_i64(t1, addr, cpu_last_val0, hi,
            alop->ctx->mmuidx, memop);
        tcg_gen_atomic_cmpxchg_i64(t0, t2, cpu_last_val1, lo,
            alop->ctx->mmuidx, memop);
    } else {
        tcg_gen_atomic_cmpxchg_i64(t0, addr, cpu_last_val0, lo,
            alop->ctx->mmuidx, memop);
        tcg_gen_atomic_cmpxchg_i64(t1, t2, cpu_last_val1, hi,
            alop->ctx->mmuidx, memop);
    }

    tcg_gen_setcond_i64(TCG_COND_NE, t0, t0, cpu_last_val0);
    tcg_gen_setcond_i64(TCG_COND_NE, t1, t1, cpu_last_val1);
    tcg_gen_or_i64(t0, t0, t1);
    tcg_gen_extrl_i64_i32(alop->ctx->mlock, t1);

    tcg_temp_free(t2);
    tcg_temp_free_i64(t1);
    tcg_temp_free_i64(t0);
}

static void gen_atomic_cmpxchg_i32(Alop *alop, TCGv_i32 value, TCGv addr,
    MemOp memop)
{
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i32 t1 = tcg_temp_new_i32();

    tcg_gen_extrl_i64_i32(t0, cpu_last_val0);
    tcg_gen_atomic_cmpxchg_i32(t1, addr, t0, value,
        alop->ctx->mmuidx, memop);
    tcg_gen_setcond_i32(TCG_COND_NE, alop->ctx->mlock, t1, t0);

    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
}

#define IMPL_GEN_ST(name, S, st1, st2) \
    static void name(Alop *alop, TCGv addr, \
        MemOp memop, bool skip, bool check) \
    { \
        TCGLabel *l0 = gen_new_label(); \
        tagged(S) s4 = tagged_local_new(S); \
        \
        gen_tagged_src(4, S, alop, s4); \
        \
        if (!skip) { \
            if (alop->als.sm) { \
                TCGv_i32 t0 = tcg_temp_new_i32(); \
                \
                gen_probe_write_access(t0, addr, memop_size(memop), \
                    alop->ctx->mmuidx); \
                tcg_gen_brcondi_i32(TCG_COND_EQ, t0, 0, l0); \
                \
                tcg_temp_free_i32(t0); \
            } \
            \
            if (check && alop->ctx->mlock) { \
                st1(alop, s4.val, addr, memop); \
            } else { \
                st2(s4.val, addr, alop->ctx->mmuidx, memop); \
            } \
        } \
        gen_set_label(l0); \
        \
        tagged_free(S, s4); \
    }

IMPL_GEN_ST(gen_st_raw_i32, s, gen_atomic_cmpxchg_i32, tcg_gen_qemu_st_i32)
IMPL_GEN_ST(gen_st_raw_i64, d, gen_atomic_cmpxchg_i64, tcg_gen_qemu_st_i64)

static void gen_qemu_st_i128(TCGv_i64 hi, TCGv_i64 lo, TCGv addr,
    TCGArg idx, MemOp memop)
{
    TCGv t0 = tcg_temp_new();

    tcg_gen_addi_tl(t0, addr, 8);

    if (memop & MO_BE) {
        tcg_gen_qemu_st_i64(hi, addr, idx, memop);
        tcg_gen_qemu_st_i64(lo, t0, idx, memop);
    } else {
        tcg_gen_qemu_st_i64(lo, addr, idx, memop);
        tcg_gen_qemu_st_i64(hi, t0, idx, memop);
    }

    tcg_temp_free(t0);
}

static void gen_st_raw_i128(Alop *alop, TCGv addr,
    MemOp memop, bool skip, bool check)
{
    TCGLabel *l0 = gen_new_label();
    Tagged_ptr s4 = tagged_local_new_ptr();

    gen_tagged_src4_q(alop, s4);

    if (!skip) {
        TCGv_i64 t0 = tcg_temp_new_i64();
        TCGv_i64 t1 = tcg_temp_new_i64();

        if (alop->als.sm) {
            TCGv_i32 t0 = tcg_temp_new_i32();

            gen_probe_write_access(t0, addr, 16, alop->ctx->mmuidx);
            tcg_gen_brcondi_i32(TCG_COND_EQ, t0, 0, l0);

            tcg_temp_free_i32(t0);
        }

        gen_qpunpackdl(t1, t0, s4.val);

        if (check && alop->ctx->mlock) {
            gen_atomic_cmpxchg_i128(alop, t1, t0, addr, memop);
        } else {
            gen_qemu_st_i128(t1, t0, addr, alop->ctx->mmuidx, memop);
        }

        tcg_temp_free_i64(t1);
        tcg_temp_free_i64(t0);
    }

    gen_set_label(l0);

    tagged_free_ptr(s4);
}

static void gen_mask8_i64(TCGv_i64 ret, TCGv_i64 arg)
{
    TCGv_i64 t0 = tcg_temp_new_i64();
    TCGv_i64 t1 = tcg_const_i64(0x0101010101010101);

    tcg_gen_andi_i64(t0, arg, 0xff);
    tcg_gen_mul_i64(t0, t0, t1);
    tcg_gen_andi_i64(t0, t0, 0x8040201008040201);
    tcg_gen_addi_i64(t0, t0, 0x00406070787c7e7f);
    tcg_gen_shri_i64(t0, t0, 7);
    tcg_gen_and_i64(t0, t0, t1);
    tcg_gen_muli_i64(ret, t0, 0xff);

    tcg_temp_free_i64(t1);
    tcg_temp_free_i64(t0);
}

static void gen_pmerge_i64(TCGv_i64 ret, TCGv_i64 a, TCGv_i64 b,
    TCGv_i64 bitmask)
{
    TCGv_i64 t0 = tcg_temp_new_i64();
    TCGv_i64 t1 = tcg_temp_new_i64();
    TCGv_i64 t2 = tcg_temp_new_i64();

    gen_mask8_i64(t0, bitmask);
    tcg_gen_and_i64(t1, a, t0);
    gen_andn_i64(t2, b, t0);
    tcg_gen_or_i64(ret, t1, t2);

    tcg_temp_free_i64(t2);
    tcg_temp_free_i64(t1);
    tcg_temp_free_i64(t0);
}

static void gen_stm_raw_i128(Alop *alop, TCGv addr,
    MemOp memop, bool skip, bool check)
{
    TCGLabel *l0 = gen_new_label();
    Tagged_i32 s2 = tagged_new_i32();
    Tagged_ptr s4 = tagged_local_new_ptr();
    TCGv_i64 mask = tcg_temp_local_new_i64();

    gen_tagged_src2_s(alop, s2);
    gen_tagged_src4_q(alop, s4);
    tcg_gen_extu_i32_i64(mask, s2.val);
    tcg_gen_andi_i64(mask, mask, 0xffff);
    tcg_gen_brcondi_i64(TCG_COND_EQ, mask, 0, l0);

    if (!skip) {
        TCGv_i64 t0 = tcg_temp_new_i64();
        TCGv_i64 t1 = tcg_temp_new_i64();
        TCGv_i64 t2 = tcg_temp_new_i64();
        TCGv_i64 t3 = tcg_temp_new_i64();
        TCGv_i64 t4 = tcg_temp_new_i64();

        if (alop->als.sm) {
            TCGv_i32 t0 = tcg_temp_new_i32();

            gen_probe_write_access(t0, addr, 16, alop->ctx->mmuidx);
            tcg_gen_brcondi_i32(TCG_COND_EQ, t0, 0, l0);

            tcg_temp_free_i32(t0);
        }

        gen_qpunpackdl(t1, t0, s4.val);
        gen_qemu_ld_i128(t3, t2, addr, alop->ctx->mmuidx, memop);
        tcg_gen_shri_i64(t4, mask, 8);
        gen_pmerge_i64(t0, t0, t2, mask);
        gen_pmerge_i64(t1, t1, t3, t4);

        if (check && alop->ctx->mlock) {
            gen_atomic_cmpxchg_i128(alop, t1, t0, addr, memop);
        } else {
            gen_qemu_st_i128(t1, t0, addr, alop->ctx->mmuidx, memop);
        }

        tcg_temp_free_i64(t4);
        tcg_temp_free_i64(t3);
        tcg_temp_free_i64(t2);
        tcg_temp_free_i64(t1);
        tcg_temp_free_i64(t0);
    }

    gen_set_label(l0);

    tcg_temp_free_i64(mask);
    tagged_free_ptr(s4);
    tagged_free_i32(s2);
}

typedef enum {
    ADDR_FLAT,
    ADDR_CS,
    ADDR_DS,
    ADDR_ES,
    ADDR_FS,
    ADDR_GD,
    ADDR_GS,
    ADDR_SS,
} AddrBase;

typedef void (*GenAddrFn)(Alop *alop, TCGv_i32 tag, TCGv addr, AddrBase base);
typedef void (*GenLoadFn)(Alop *alop, TCGv_i32 tag, TCGv addr,
    MemOp memop, bool skip, bool save);
typedef void (*GenStoreFn)(Alop *alop, TCGv addr,
    MemOp memop, bool skip, bool check);

static void gen_alopf1_mas(Alop *alop, GenAddrFn addr_fn,
    GenLoadFn ld_fn, MemOp memop, AddrBase base)
{
    bool skip = false, save = false;
    TCGv_i32 tag = tcg_temp_local_new_i32();
    TCGv addr = tcg_temp_local_new();

    memop |= scan_ld_mas(alop, &skip, &save);
    (*addr_fn)(alop, tag, addr, base);
    (*ld_fn)(alop, tag, addr, memop, skip, save);

    tcg_temp_free(addr);
    tcg_temp_free_i32(tag);
}

static void gen_alopf3_mas(Alop *alop, GenAddrFn addr_fn,
    GenStoreFn st_fn, MemOp memop, AddrBase base)
{
    bool skip = false, check = false;
    TCGv_i32 tag = tcg_temp_new_i32();
    TCGv addr = tcg_temp_local_new();

    memop |= scan_st_mas(alop, &skip, &check);
    (*addr_fn)(alop, tag, addr, base);
    (*st_fn)(alop, addr, memop, skip, check);

    tcg_temp_free(addr);
    tcg_temp_free_i32(tag);
}

#define IMPL_GEN_ADDR(name, S, cast) \
    static void name(Alop *alop, TCGv_i32 tag, TCGv addr, \
        AddrBase base) \
    { \
        tagged(S) s1 = tagged_new(S); \
        tagged(S) s2 = tagged_new(S); \
        temp(S) t0 = temp_new(S); \
        \
        /* TODO: addr base */  \
        gen_tagged_src(1, S, alop, s1); \
        gen_tagged_src(2, S, alop, s2); \
        tcg_gen_or_i32(tag, s1.tag, s2.tag); \
        call(S, tcg_gen_add, t0, s1.val, s2.val); \
        cast(addr, t0); \
        \
        temp_free(S, t0); \
        tagged_free(S, s2); \
        tagged_free(S, s1); \
    }

IMPL_GEN_ADDR(gen_addr_i64, d, tcg_gen_trunc_i64_tl)
IMPL_GEN_ADDR(gen_addr_i32, s, tcg_gen_ext_i32_tl)

#define IMPL_GEN_ADDR_SRC1(name, S, cast) \
    static void name(Alop *alop, TCGv_i32 tag, TCGv addr, \
        AddrBase base) \
    { \
        tagged(S) s1 = tagged_new(S); \
        \
        /* TODO: addr base */  \
        gen_tagged_src(1, S, alop, s1); \
        tcg_gen_mov_i32(tag, s1.tag); \
        cast(addr, s1.val); \
        \
        tagged_free(S, s1); \
    }

IMPL_GEN_ADDR_SRC1(gen_addr_src1_i64, d, tcg_gen_trunc_i64_tl)
IMPL_GEN_ADDR_SRC1(gen_addr_src1_i32, s, tcg_gen_ext_i32_tl)

#define gen_ldb(i, a, b)  gen_alopf1_mas(i, a, gen_ld_raw_i64,  MO_UB, b)
#define gen_ldh(i, a, b)  gen_alopf1_mas(i, a, gen_ld_raw_i64,  MO_UW, b)
#define gen_ldw(i, a, b)  gen_alopf1_mas(i, a, gen_ld_raw_i64,  MO_UL, b)
#define gen_ldd(i, a, b)  gen_alopf1_mas(i, a, gen_ld_raw_i64,  MO_Q,  b)
#define gen_ldqp(i, a, b) gen_alopf1_mas(i, a, gen_ld_raw_i128, MO_Q,  b)

#define gen_stb(i, a, b)   gen_alopf3_mas(i, a, gen_st_raw_i32,   MO_UB, b)
#define gen_sth(i, a, b)   gen_alopf3_mas(i, a, gen_st_raw_i32,   MO_UW, b)
#define gen_stw(i, a, b)   gen_alopf3_mas(i, a, gen_st_raw_i32,   MO_UL, b)
#define gen_std(i, a, b)   gen_alopf3_mas(i, a, gen_st_raw_i64,   MO_Q,  b)
#define gen_stqp(i, a, b)  gen_alopf3_mas(i, a, gen_st_raw_i128,  MO_Q,  b)
#define gen_stmqp(i, a, b) gen_alopf3_mas(i, a, gen_stm_raw_i128, MO_Q,  b)

static void gen_aad_tag(TCGv_i64 ret, TCGv_i32 tag)
{
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i64 t1 = tcg_temp_new_i64();

    tcg_gen_setcondi_i32(TCG_COND_NE, t0, tag, 0);
    tcg_gen_extu_i32_i64(t1, t0);
    tcg_gen_shli_i64(ret, t1, 54);

    tcg_temp_free_i64(t1);
    tcg_temp_free_i32(t0);
}

static void gen_aaurw_aad_lo_i64(Alop *alop, TCGv_i64 arg1, TCGv_i32 tag)
{
    TCGv_i64 lo = cpu_aad_lo[alop->als.aad];
    TCGv_i64 t0 = tcg_temp_new_i64();
    TCGv_i64 t1 = tcg_temp_new_i64();

    tcg_gen_andi_i64(t0, arg1, 3UL << 57);
    tcg_gen_andi_i64(lo, lo, ~(0x1fUL << 54));
    tcg_gen_or_i64(lo, lo, t0);
    tcg_gen_deposit_i64(lo, lo, arg1, 0, 48);
    tcg_gen_ori_i64(lo, lo, 3UL << 59);
    gen_aad_tag(t1, tag);
    tcg_gen_or_i64(lo, lo, t1);

    tcg_temp_free_i64(t1);
    tcg_temp_free_i64(t0);
}

static void gen_aaurw_aad_hi_i64(Alop *alop, TCGv_i64 arg1, TCGv_i32 tag)
{
    tcg_gen_andi_i64(cpu_aad_hi[alop->als.aad], arg1, 0xffffffff00000000);
}

static void gen_aaurw_aad_i32(Alop *alop, TCGv_i32 arg1, TCGv_i32 tag)
{
    TCGv_i64 lo = cpu_aad_lo[alop->als.aad];
    TCGv_i64 t0 = tcg_temp_new_i64();
    TCGv_i64 t1 = tcg_temp_new_i64();
    TCGv_i64 t2 = tcg_temp_new_i64();

    tcg_gen_extu_i32_i64(t0, arg1);
    tcg_gen_deposit_i64(lo, lo, t0, 0, 48);
    tcg_gen_ori_i64(lo, lo, 3UL << 59);
    tcg_gen_andi_i64(lo, lo, ~(0x7UL << 54));
    gen_aad_tag(t1, tag);
    tcg_gen_or_i64(lo, lo, t1);

    tcg_temp_free_i64(t2);
    tcg_temp_free_i64(t1);
    tcg_temp_free_i64(t0);
}

static void gen_aaurw_rest_i32(Alop *alop, TCGv_i32 arg1, TCGv_i32 tag)
{
    int idx = alop->als.aaind;
    TCGv_i32 t0 = tcg_temp_new_i32();
    tcg_gen_setcondi_i32(TCG_COND_NE, t0, tag, 0);
    switch(alop->als.aaopc) {
    case 1: /* aaurwd src4, aasti */
        tcg_gen_mov_i32(cpu_aasti[idx], arg1);
        tcg_gen_deposit_i32(cpu_aasti_tags, cpu_aasti_tags, t0, idx, 1);
        break;
    case 2: { /* aaurwd src4, aaind */
        if (idx == 0) {
            tcg_gen_movi_i32(cpu_aaind[idx], 0);
        } else {
            tcg_gen_mov_i32(cpu_aaind[idx], arg1);
            tcg_gen_deposit_i32(cpu_aaind_tags, cpu_aaind_tags, t0,
                idx, 1);
        }
        break;
    }
    case 3: /* aaurwd src4, aaincr */
        idx &= 7;
        if (idx > 0) {
            tcg_gen_mov_i32(cpu_aaincr[idx], arg1);
            tcg_gen_deposit_i32(cpu_aaincr_tags, cpu_aaincr_tags, t0,
                idx, 1);
        }
        break;
    default:
        g_assert_not_reached();
        break;
    }
    tcg_temp_free_i32(t0);
}

static void gen_st_lm_cond_i32(TCGv_i32 ret, TCGv_i32 loop_end)
{
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i32 t1 = tcg_temp_new_i32();
    TCGv_i32 t2 = tcg_temp_new_i32();

    tcg_gen_setcondi_i32(TCG_COND_EQ, t0, cpu_lsr_strmd, 0);
    tcg_gen_and_i32(t1, t0, loop_end);
    tcg_gen_setcondi_i32(TCG_COND_NE, t2, cpu_lsr_pcnt, 0);
    tcg_gen_or_i32(ret, t1, t2);

    tcg_temp_free_i32(t2);
    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
}

static void gen_dec_lsr_strmd(TCGv_i32 loop_end)
{
    TCGv_i32 t0 = tcg_const_i32(0);
    TCGv_i32 t1 = tcg_temp_new_i32();

    tcg_gen_subi_i32(t1, cpu_lsr_strmd, 1);
    tcg_gen_movcond_i32(TCG_COND_NE, cpu_lsr_strmd, loop_end, t0,
        t1, cpu_lsr_strmd);

    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
}

static void gen_aasti_incr(Alop *alop)
{
    DisasContext *ctx = alop->ctx;
    uint16_t rlp = find_am_cond(ctx, alop->chan);
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i32 t1 = tcg_temp_new_i32();
    int len;

    switch (alop->op) {
    case OP_STAAB:  len = 0; break;
    case OP_STAAH:  len = 1; break;
    case OP_STAAW:  len = 2; break;
    case OP_STAAD:  len = 3; break;
    case OP_STAAQ:  len = 3; break;
    case OP_STAAQP: len = 4; break;
    default:
        g_assert_not_reached();
        break;
    }

    tcg_gen_shli_i32(t0, cpu_aaincr[alop->als.aaincr], len);
    tcg_gen_add_i32(t1, cpu_aasti[alop->als.aaind], t0);

    if (rlp != 0) {
        // FIXME: need to test AM RLP
        TCGv_i32 t2 = tcg_temp_new_i32();
        TCGv_i32 t3 = tcg_const_i32(0);

        e2k_todo(ctx, "AM RLP found");
        gen_am_cond_i32(ctx, t2, alop->chan, rlp);
        tcg_gen_movcond_i32(TCG_COND_NE, cpu_aasti[alop->als.aaind], t2, t3,
            cpu_aasti[alop->als.aaind], t1);

        tcg_temp_free_i32(t3);
        tcg_temp_free_i32(t2);
    } else {
        tcg_gen_mov_i32(cpu_aasti[alop->als.aaind], t1);
    }

    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
}

static void gen_aad_ptr(DisasContext *ctx, TCGv ret, Alop *alop)
{
    uint32_t lit = 0;
    TCGv_i64 t0 = tcg_temp_new_i64();
    TCGv t1 = tcg_temp_new();
    TCGv t2 = tcg_temp_new();

    if (alop->als.aalit) {
        int lts = alop->als.aalit - 1;
        if (ctx->bundle.lts_present[lts]) {
            lit = ctx->bundle.lts[lts];
        } else {
            gen_tr_excp_illopn(ctx);
            return;
        }
    }

    tcg_gen_extract_i64(t0, cpu_aad_lo[alop->als.aad], 0, 48);
    tcg_gen_trunc_i64_tl(t1, t0);
    tcg_gen_extu_i32_tl(t2, cpu_aasti[alop->als.aaind]);

    if (lit != 0) {
        TCGv t3 = tcg_temp_new();
        tcg_gen_add_tl(t3, t1, t2);
        tcg_gen_addi_tl(ret, t3, lit);
        tcg_temp_free(t3);
    } else {
        tcg_gen_add_tl(ret, t1, t2);
    }

    tcg_temp_free(t2);
    tcg_temp_free(t1);
    tcg_temp_free_i64(t0);
}

static void gen_staaqp(Alop *alop)
{
    DisasContext *ctx = alop->ctx;
    uint8_t mas = alop->mas;
    Tagged_ptr s4 = tagged_local_new_ptr();

    gen_tagged_src4_q(alop, s4);

    if ((mas & 0x7) == 0x7) {
        int opc = mas >> 3;

        switch (opc) {
        case 7:
            /* should be aaurwqp but CPU do nothing */
            break;
        default:
            e2k_todo_illop(alop->ctx, "staaqp special opc=%#x", opc);
            break;
        }
    } else {
        /* staaqp */
        int mod = mas & 0x7;
        MemOp memop = memop_from_mas(mas) | MO_Q;
        TCGLabel *l0 = gen_new_label();
        TCGv t0 = tcg_temp_local_new();
        TCGv_i64 t1 = tcg_temp_new_i64();
        TCGv_i64 t2 = tcg_temp_new_i64();

        if (mod != 0) {
            e2k_todo(ctx, "staaqp mod=%#x is not implemented", mod);
        }

        gen_aad_ptr(ctx, t0, alop);

        if (alop->als.sm) {
            TCGv_i32 t1 = tcg_temp_new_i32();
            gen_probe_write_access(t1, t0, 16, alop->ctx->mmuidx);
            tcg_gen_brcondi_i32(TCG_COND_EQ, t1, 0, l0);
            tcg_temp_free_i32(t1);
        }

        gen_qpunpackdl(t2, t1, s4.val);
        gen_qemu_st_i128(t2, t1, t0, alop->ctx->mmuidx, memop);
        gen_set_label(l0);

        tcg_temp_free_i64(t2);
        tcg_temp_free_i64(t1);
        tcg_temp_free(t0);
    }

    tagged_free_ptr(s4);
}

static void gen_staa_i64(Alop *alop)
{
    DisasContext *ctx = alop->ctx;
    uint8_t mas = alop->mas;
    Tagged_i64 s4 = tagged_local_new_i64();

    gen_tagged_src4_d(alop, s4);

    if (mas == 0x3f) {
        /* aaurwd */
        if (alop->als.aaopc == 0) {
            if (alop->chan == 5 && alop->als.opc1 == 0x3f) {
                gen_aaurw_aad_hi_i64(alop, s4.val, s4.tag);
            } else {
                gen_aaurw_aad_lo_i64(alop, s4.val, s4.tag);
            }
        } else {
            TCGv_i32 t0 = tcg_temp_new_i32();
            tcg_gen_extrl_i64_i32(t0, s4.val);
            gen_aaurw_rest_i32(alop, t0, s4.tag);
            tcg_temp_free_i32(t0);
        }
    } else {
        /* staad */
        int mod = mas & 0x7;
        MemOp memop = memop_from_mas(mas) | MO_Q;
        TCGLabel *l0 = gen_new_label();
        TCGv t0 = tcg_temp_local_new();

        if (mod != 0) {
            e2k_todo(ctx, "staad mod=%#x is not implemented", mod);
        }

        gen_aad_ptr(ctx, t0, alop);

        if (alop->als.sm) {
            TCGv_i32 t1 = tcg_temp_new_i32();
            gen_probe_write_access(t1, t0, memop_size(memop), alop->ctx->mmuidx);
            tcg_gen_brcondi_i32(TCG_COND_EQ, t1, 0, l0);
            tcg_temp_free_i32(t1);
        }

        tcg_gen_qemu_st_i64(s4.val, t0, ctx->mmuidx, memop);
        gen_set_label(l0);
        tcg_temp_free(t0);
    }

    tagged_free_i64(s4);
}

static void gen_staa_i32(Alop *alop, MemOp memop)
{
    DisasContext *ctx = alop->ctx;
    uint8_t mas = alop->mas;
    Tagged_i32 s4 = tagged_local_new_i32();

    gen_tagged_src4_s(alop, s4);

    if (mas == 0x3f) {
        /* aaurw */
        /* CPU do nothing if size less than 32 bits */
        if ((memop & MO_SIZE) == MO_32) {
            if (alop->als.aaopc == 0) {
                gen_aaurw_aad_i32(alop, s4.val, s4.tag);
            } else {
                gen_aaurw_rest_i32(alop, s4.val, s4.tag);
            }
        }
    } else {
        /* staaw */
        int mod = mas & 0x7;
        TCGLabel *l0 = gen_new_label();
        TCGv t0 = tcg_temp_local_new();

        if (mod != 0) {
            char c;
            switch(memop & MO_SIZE) {
            case MO_8: c = 'b'; break;
            case MO_16: c = 'h'; break;
            case MO_32: c = 'w'; break;
            default:
                g_assert_not_reached();
                break;
            }
            e2k_todo(ctx, "staa%c mod=%#x is not implemented", c, mod);
        }

        gen_aad_ptr(ctx, t0, alop);

        if (alop->als.sm) {
            TCGv_i32 t1 = tcg_temp_new_i32();
            gen_probe_write_access(t1, t0, memop_size(memop), alop->ctx->mmuidx);
            tcg_gen_brcondi_i32(TCG_COND_EQ, t1, 0, l0);
            tcg_temp_free_i32(t1);
        }

        memop |= memop_from_mas(mas);
        tcg_gen_qemu_st_i32(s4.val, t0, ctx->mmuidx, memop);
        gen_set_label(l0);
        tcg_temp_free(t0);
    }

    tagged_free_i32(s4);
}

#define IMPL_ALOPF1_BASIC(name, S1, S2, R, T, code) \
    static void name(Alop *alop, T) \
    { \
        tagged(S1) s1 = tagged_local_new(S1); \
        tagged(S2) s2 = tagged_local_new(S2); \
        tagged(R) r = tagged_local_new(R); \
        \
        gen_tagged_src(1, S1, alop, s1); \
        gen_tagged_src(2, S2, alop, s2); \
        gen_result_init(R, alop, r); \
        gen_tag2(R, r, s1, s2); \
        { code; } \
        gen_al_result(R, alop, r); \
        \
        tagged_free(R, r); \
        tagged_free(S2, s2); \
        tagged_free(S1, s1); \
    }

#define IMPL_ALOPF2_BASIC(name, S2, R, T, code) \
    static void name(Alop *alop, T) \
    { \
        tagged(S2) s2 = tagged_local_new(S2); \
        tagged(R) r = tagged_local_new(R); \
        \
        gen_tagged_src(2, S2, alop, s2); \
        gen_result_init(R, alop, r); \
        gen_tag1(R, r, s2); \
        { code; } \
        gen_al_result(R, alop, r); \
        \
        tagged_free(R, r); \
        tagged_free(S2, s2); \
    }

#define IMPL_ALOPF7_BASIC(name, S1, S2, R, T, code) \
    static void name(Alop *alop, T) \
    { \
        tagged(S1) s1 = tagged_local_new(S1); \
        tagged(S2) s2 = tagged_local_new(S2); \
        tagged(R) r = tagged_local_new(R); \
        Tagged_i32 p = tagged_new_i32(); \
        temp(R) t0 = temp_new(R); \
        \
        gen_tagged_src(1, S1, alop, s1); \
        gen_tagged_src(2, S2, alop, s2); \
        gen_result_init(R, alop, r); \
        gen_tag2(R, r, s1, s2); \
        { code; } \
        tcg_gen_mov_i32(p.tag, r.tag); \
        call(R, tcg_gen_setcondi, TCG_COND_NE, t0, r.val, 0); \
        call(R, gen_extrl, p.val, t0); \
        gen_al_result_b(alop, p); \
        \
        temp_free(R, t0); \
        tagged_free_i32(p); \
        tagged_free(R, r); \
        tagged_free(S2, s2); \
        tagged_free(S1, s1); \
    }

#define IMPL_ALOPF21_BASIC(name, S1, S2, S3, R, T, code) \
    static void name(Alop *alop, T) \
    { \
        tagged(S1) s1 = tagged_local_new(S1); \
        tagged(S2) s2 = tagged_local_new(S2); \
        tagged(S3) s3 = tagged_local_new(S3); \
        tagged(R) r = tagged_local_new(R); \
        \
        gen_tagged_src(1, S1, alop, s1); \
        gen_tagged_src(2, S2, alop, s2); \
        gen_tagged_src(3, S3, alop, s3); \
        gen_result_init(R, alop, r); \
        gen_tag3(R, r, s1, s2, s3); \
        { code; } \
        gen_al_result(R, alop, r); \
        \
        tagged_free(R, r); \
        tagged_free(S3, s3); \
        tagged_free(S2, s2); \
        tagged_free(S1, s1); \
    }

#define IMPL_ALOPF1(name, S1, S2, R) \
    IMPL_ALOPF1_BASIC(name, S1, S2, R, \
        void (*op)(temp(R), temp(S1), temp(S2)), \
        { (*op)(r.val, s1.val, s2.val); })

#define IMPL_ALOPF1_TAG_SM(name, S1, S2, R) \
    IMPL_ALOPF1_BASIC(name, S1, S2, R, \
        void (*op)(temp(R), TCGv_i32, TCGv_i32, \
            temp(S1), temp(S2), bool sm), \
        { (*op)(r.val, r.tag, r.tag, s1.val, s2.val, alop->als.sm); })

#define IMPL_ALOPF1_ENV(name, S1, S2, R) \
    IMPL_ALOPF1_BASIC(name, S1, S2, R, \
        void (*op)(temp(R), TCGv_env, temp(S1), temp(S2)), \
        { (*op)(r.val, cpu_env, s1.val, s2.val); })

#define IMPL_ALOPF11_LIT8(name, S1, S2, R) \
    IMPL_ALOPF1_BASIC(name, S1, S2, R, \
        void (*op)(temp(R), temp(S1), temp(S2), int), \
        { (*op)(r.val, s1.val, s2.val, alop->ales.src3); })

#define IMPL_ALOPF2(name, S2, R) \
    IMPL_ALOPF2_BASIC(name, S2, R, \
        void (*op)(temp(R), temp(S2)), \
        { (*op)(r.val, s2.val); })

#define IMPL_ALOPF2_ENV(name, S2, R) \
    IMPL_ALOPF2_BASIC(name, S2, R, \
        void (*op)(temp(R), TCGv_env, temp(S2)), \
        { (*op)(r.val, cpu_env, s2.val); })

#define IMPL_ALOPF2_PSHUFH(name, S2, R) \
    IMPL_ALOPF2_BASIC(name, S2, R, \
        void (*op)(temp(R), temp(S2), TCGv_i32), \
        { \
            TCGv_i32 t0 = tcg_const_i32(alop->ales.src3); \
            (*op)(r.val, s2.val, t0); \
            tcg_temp_free_i32(t0); \
        })

#define IMPL_ALOPF7(name, S1, S2, R) \
    IMPL_ALOPF7_BASIC(name, S1, S2, R, \
        void (*op)(temp(R), temp(S1), temp(S2)), \
        { (*op)(r.val, s1.val, s2.val); })

#define IMPL_ALOPF7_ENV(name, S1, S2, R) \
    IMPL_ALOPF7_BASIC(name, S1, S2, R, \
        void (*op)(temp(R), TCGv_env, temp(S1), temp(S2)), \
        { (*op)(r.val, cpu_env, s1.val, s2.val); })

#define IMPL_ALOPF21(name, S1, S2, S3, R) \
    IMPL_ALOPF21_BASIC(name, S1, S2, S3, R, \
        void (*op)(temp(R), temp(S1), temp(S2), temp(S3)), \
        { (*op)(r.val, s1.val, s2.val, s3.val); })

#define IMPL_ALOPF21_ENV(name, S1, S2, S3, R) \
    IMPL_ALOPF21_BASIC(name, S1, S2, S3, R, \
        void (*op)(temp(R), TCGv_env, temp(S1), temp(S2), temp(S3)), \
        { (*op)(r.val, cpu_env, s1.val, s2.val, s3.val); })

#define IMPL_ALOPF21_LOG(name, S1, S2, S3, R) \
    IMPL_ALOPF21_BASIC(name, S1, S2, S3, R, \
        void (*op)(temp(R), uint32_t, temp(S1), temp(S2), temp(S3)), \
        { (*op)(r.val, alop->als.opc1, s1.val, s2.val, s3.val); })

IMPL_ALOPF1(gen_alopf1_sss, s, s, s)
IMPL_ALOPF1(gen_alopf1_ssd, s, s, d)
IMPL_ALOPF1(gen_alopf1_ddd, d, d, d)
IMPL_ALOPF1(gen_alopf1_dsx, d, s, x)
IMPL_ALOPF1(gen_alopf1_qsq, q, s, q)
IMPL_ALOPF1(gen_alopf1_ddq, d, d, q)
IMPL_ALOPF1(gen_alopf1_dqq, d, q, q)
IMPL_ALOPF1(gen_alopf1_qdq, q, d, q)
IMPL_ALOPF1(gen_alopf1_qqd, q, q, d)
IMPL_ALOPF1(gen_alopf1_qqq, q, q, q)

IMPL_ALOPF1_TAG_SM(gen_alopf1_tag_sss, s, s, s)
IMPL_ALOPF1_TAG_SM(gen_alopf1_tag_dss, d, s, s)
IMPL_ALOPF1_TAG_SM(gen_alopf1_tag_ddd, d, d, d)

IMPL_ALOPF1_ENV(gen_alopf1_env_sss, s, s, s)
IMPL_ALOPF1_ENV(gen_alopf1_env_dsd, d, s, d)
IMPL_ALOPF1_ENV(gen_alopf1_env_dds, d, d, s)
IMPL_ALOPF1_ENV(gen_alopf1_env_ddd, d, d, d)
IMPL_ALOPF1_ENV(gen_alopf1_env_xxs, x, x, s)
IMPL_ALOPF1_ENV(gen_alopf1_env_xxd, x, x, d)
IMPL_ALOPF1_ENV(gen_alopf1_env_xss, x, s, s)
IMPL_ALOPF1_ENV(gen_alopf1_env_xds, x, d, s)
IMPL_ALOPF1_ENV(gen_alopf1_env_xdd, x, d, d)
IMPL_ALOPF1_ENV(gen_alopf1_env_xsx, x, s, x)
IMPL_ALOPF1_ENV(gen_alopf1_env_xdx, x, d, x)
IMPL_ALOPF1_ENV(gen_alopf1_env_sxs, s, x, s)
IMPL_ALOPF1_ENV(gen_alopf1_env_dxd, d, x, d)
IMPL_ALOPF1_ENV(gen_alopf1_env_sxx, s, x, x)
IMPL_ALOPF1_ENV(gen_alopf1_env_dxx, d, x, x)
IMPL_ALOPF1_ENV(gen_alopf1_env_xxx, x, x, x)
IMPL_ALOPF1_ENV(gen_alopf1_env_dqq, d, q, q)
IMPL_ALOPF1_ENV(gen_alopf1_env_qqq, q, q, q)

IMPL_ALOPF11_LIT8(gen_alopf11_lit8_ddd, d, d, d)

IMPL_ALOPF2(gen_alopf2_ss, s, s)
IMPL_ALOPF2(gen_alopf2_dd, d, d)
IMPL_ALOPF2(gen_alopf2_xs, x, s)
IMPL_ALOPF2(gen_alopf2_xx, x, x)
IMPL_ALOPF2(gen_alopf2_qs, q, s)
IMPL_ALOPF2(gen_alopf2_dq, d, q)
IMPL_ALOPF2(gen_alopf2_qq, q, q)

IMPL_ALOPF2_ENV(gen_alopf2_env_ss, s, s)
IMPL_ALOPF2_ENV(gen_alopf2_env_ds, d, s)
IMPL_ALOPF2_ENV(gen_alopf2_env_sd, s, d)
IMPL_ALOPF2_ENV(gen_alopf2_env_dd, d, d)
IMPL_ALOPF2_ENV(gen_alopf2_env_dx, d, x)
IMPL_ALOPF2_ENV(gen_alopf2_env_xs, x, s)
IMPL_ALOPF2_ENV(gen_alopf2_env_xd, x, d)
IMPL_ALOPF2_ENV(gen_alopf2_env_sx, s, x)
IMPL_ALOPF2_ENV(gen_alopf2_env_dq, d, q)
IMPL_ALOPF2_ENV(gen_alopf2_env_qd, q, d)
IMPL_ALOPF2_ENV(gen_alopf2_env_qq, q, q)

IMPL_ALOPF2_PSHUFH(gen_alopf2_pshufh, d, d)

IMPL_ALOPF7(gen_alopf7_sss, s, s, s)
IMPL_ALOPF7(gen_alopf7_ddd, d, d, d)
IMPL_ALOPF7(gen_alopf7_qqd, q, q, d)

IMPL_ALOPF7_ENV(gen_alopf7_env_sss, s, s, s)
IMPL_ALOPF7_ENV(gen_alopf7_env_ddd, d, d, d)
IMPL_ALOPF7_ENV(gen_alopf7_env_xsd, x, s, d)
IMPL_ALOPF7_ENV(gen_alopf7_env_xdd, x, d, d)
IMPL_ALOPF7_ENV(gen_alopf7_env_xxd, x, x, d)

IMPL_ALOPF21(gen_alopf21_ssss, s, s, s, s)
IMPL_ALOPF21(gen_alopf21_ddsd, d, d, s, d)
IMPL_ALOPF21(gen_alopf21_dddd, d, d, d, d)
IMPL_ALOPF21(gen_alopf21_qqqq, q, q, q, q)

IMPL_ALOPF21_ENV(gen_alopf21_env_ssss, s, s, s, s)
IMPL_ALOPF21_ENV(gen_alopf21_env_dddd, d, d, d, d)
IMPL_ALOPF21_ENV(gen_alopf21_env_qqqq, q, q, q, q)

IMPL_ALOPF21_LOG(gen_alopf21_log_dddd, d, d, d, d)
IMPL_ALOPF21_LOG(gen_alopf21_log_qqqq, q, q, q, q)

static void alop_table_find(DisasContext *ctx, Alop *alop, AlesFlag ales_present)
{
    /* ALES2/5 may be allocated but must not be used */
    int opc2 = ales_present & ALES_PRESENT ? alop->ales.opc2 : 0;
    int16_t index = alops_map[opc2][alop->als.opc1][alop->chan];
    while (index != -1) {
        bool is_match = false;
        AlopDesc *desc = &alops[index];
        switch(desc->alopf) {
        case ALOPF1:
        case ALOPF1_MAS:
        case ALOPF1_MERGE:
        case ALOPF3:
        case ALOPF10:
        case ALOPF11_LIT8:
        case ALOPF12_PSHUFH:
        case ALOPF21:
            is_match = true;
            break;
        case ALOPF2:
        case ALOPF15:
            is_match = desc->extra1 == alop->als.opce1;
            break;
        case ALOPF7:
            is_match = desc->extra1 == alop->als.opc_cmp;
            break;
        case ALOPF8:
            is_match = desc->extra1 == alop->als.opc_cmp
                && alop->als.opce1 == 0xc0;
            break;
        case ALOPF11:
        case ALOPF11_MAS:
        case ALOPF11_MERGE:
        case ALOPF13:
        case ALOPF17:
            is_match = desc->extra1 == alop->ales.opce3;
            break;
        case ALOPF12:
        case ALOPF12_IBRANCHD:
        case ALOPF12_ICALLD:
        case ALOPF22:
            is_match = desc->extra1 == alop->als.opce1
                && desc->extra2 == alop->ales.opce3;
            break;
        case ALOPF16:
            is_match = desc->extra1 == alop->als.opce2;
            break;
        default:
            g_assert_not_reached();
            break;
        }

        if (is_match) {
            alop->format = desc->alopf;
            alop->op = desc->op;
            alop->args = desc->args;
            alop->name = desc->dsc;
            return;
        }

        index = desc->next[alop->chan];
    }

    gen_tr_excp_illopc(ctx);
}

static void gen_alop_simple(Alop *alop)
{
    DisasContext *ctx = alop->ctx;

    switch(alop->op) {
    case OP_ANDS: gen_alopf1_sss(alop, tcg_gen_and_i32); break;
    case OP_ANDD: gen_alopf1_ddd(alop, tcg_gen_and_i64); break;
    case OP_ANDNS: gen_alopf1_sss(alop, gen_andn_i32); break;
    case OP_ANDND: gen_alopf1_ddd(alop, gen_andn_i64); break;
    case OP_ORS: gen_alopf1_sss(alop, tcg_gen_or_i32); break;
    case OP_ORD: gen_alopf1_ddd(alop, tcg_gen_or_i64); break;
    case OP_ORNS: gen_alopf1_sss(alop, gen_orn_i32); break;
    case OP_ORND: gen_alopf1_ddd(alop, gen_orn_i64); break;
    case OP_XORS: gen_alopf1_sss(alop, tcg_gen_xor_i32); break;
    case OP_XORD: gen_alopf1_ddd(alop, tcg_gen_xor_i64); break;
    case OP_XORNS: gen_alopf1_sss(alop, gen_xorn_i32); break;
    case OP_XORND: gen_alopf1_ddd(alop, gen_xorn_i64); break;
    case OP_SXT: gen_alopf1_ssd(alop, gen_helper_sxt); break;
    case OP_ADDS: gen_alopf1_sss(alop, tcg_gen_add_i32); break;
    case OP_ADDD: gen_alopf1_ddd(alop, tcg_gen_add_i64); break;
    case OP_SUBS: gen_alopf1_sss(alop, tcg_gen_sub_i32); break;
    case OP_SUBD: gen_alopf1_ddd(alop, tcg_gen_sub_i64); break;
    case OP_SCLS: gen_alopf1_sss(alop, tcg_gen_rotl_i32); break;
    case OP_SCLD: gen_alopf1_ddd(alop, tcg_gen_rotl_i64); break;
    case OP_SCRS: gen_alopf1_sss(alop, tcg_gen_rotr_i32); break;
    case OP_SCRD: gen_alopf1_ddd(alop, tcg_gen_rotr_i64); break;
    case OP_SHLS: gen_alopf1_sss(alop, tcg_gen_shl_i32); break;
    case OP_SHLD: gen_alopf1_ddd(alop, tcg_gen_shl_i64); break;
    case OP_SHRS: gen_alopf1_sss(alop, tcg_gen_shr_i32); break;
    case OP_SHRD: gen_alopf1_ddd(alop, tcg_gen_shr_i64); break;
    case OP_SARS: gen_alopf1_sss(alop, tcg_gen_sar_i32); break;
    case OP_SARD: gen_alopf1_ddd(alop, tcg_gen_sar_i64); break;
    case OP_GETFS: gen_alopf1_sss(alop, gen_getfs); break;
    case OP_GETFD: gen_alopf1_ddd(alop, gen_getfd); break;
    case OP_MERGES: gen_merges(alop); break;
    case OP_MERGED: gen_merged(alop); break;
    case OP_CMPOSB: gen_alopf7_sss(alop, gen_cmposb); break;
    case OP_CMPBSB: gen_alopf7_sss(alop, gen_cmpbsb); break;
    case OP_CMPESB: gen_alopf7_sss(alop, gen_cmpesb); break;
    case OP_CMPBESB: gen_alopf7_sss(alop, gen_cmpbesb); break;
    case OP_CMPSSB: gen_alopf7_sss(alop, gen_cmpssb); break;
    case OP_CMPPSB: gen_alopf7_sss(alop, gen_cmppsb); break;
    case OP_CMPLSB: gen_alopf7_sss(alop, gen_cmplsb); break;
    case OP_CMPLESB: gen_alopf7_sss(alop, gen_cmplesb); break;
    case OP_CMPODB: gen_alopf7_ddd(alop, gen_cmpodb); break;
    case OP_CMPBDB: gen_alopf7_ddd(alop, gen_cmpbdb); break;
    case OP_CMPEDB: gen_alopf7_ddd(alop, gen_cmpedb); break;
    case OP_CMPBEDB: gen_alopf7_ddd(alop, gen_cmpbedb); break;
    case OP_CMPSDB: gen_alopf7_ddd(alop, gen_cmpsdb); break;
    case OP_CMPPDB: gen_alopf7_ddd(alop, gen_cmppdb); break;
    case OP_CMPLDB: gen_alopf7_ddd(alop, gen_cmpldb); break;
    case OP_CMPLEDB: gen_alopf7_ddd(alop, gen_cmpledb); break;
    case OP_CMPANDESB: gen_alopf7_sss(alop, gen_cmpandesb); break;
    case OP_CMPANDSSB: gen_alopf7_sss(alop, gen_cmpandssb); break;
    case OP_CMPANDPSB: gen_alopf7_sss(alop, gen_cmpandpsb); break;
    case OP_CMPANDLESB: gen_alopf7_sss(alop, gen_cmpandlesb); break;
    case OP_CMPANDEDB: gen_alopf7_ddd(alop, gen_cmpandedb); break;
    case OP_CMPANDSDB: gen_alopf7_ddd(alop, gen_cmpandsdb); break;
    case OP_CMPANDPDB: gen_alopf7_ddd(alop, gen_cmpandpdb); break;
    case OP_CMPANDLEDB: gen_alopf7_ddd(alop, gen_cmpandledb); break;
    case OP_FCMPEQSB:  gen_alopf7_env_sss(alop, gen_helper_fcmpeqs); break;
    case OP_FCMPLTSB:  gen_alopf7_env_sss(alop, gen_helper_fcmplts); break;
    case OP_FCMPLESB:  gen_alopf7_env_sss(alop, gen_helper_fcmples); break;
    case OP_FCMPUODSB: gen_alopf7_env_sss(alop, gen_helper_fcmpuods); break;
    case OP_FCMPNEQSB: gen_alopf7_env_sss(alop, gen_helper_fcmpneqs); break;
    case OP_FCMPNLTSB: gen_alopf7_env_sss(alop, gen_helper_fcmpnlts); break;
    case OP_FCMPNLESB: gen_alopf7_env_sss(alop, gen_helper_fcmpnles); break;
    case OP_FCMPODSB:  gen_alopf7_env_sss(alop, gen_helper_fcmpods); break;
    case OP_FCMPEQDB:  gen_alopf7_env_ddd(alop, gen_helper_fcmpeqd); break;
    case OP_FCMPLTDB:  gen_alopf7_env_ddd(alop, gen_helper_fcmpltd); break;
    case OP_FCMPLEDB:  gen_alopf7_env_ddd(alop, gen_helper_fcmpled); break;
    case OP_FCMPUODDB: gen_alopf7_env_ddd(alop, gen_helper_fcmpuodd); break;
    case OP_FCMPNEQDB: gen_alopf7_env_ddd(alop, gen_helper_fcmpneqd); break;
    case OP_FCMPNLTDB: gen_alopf7_env_ddd(alop, gen_helper_fcmpnltd); break;
    case OP_FCMPNLEDB: gen_alopf7_env_ddd(alop, gen_helper_fcmpnled); break;
    case OP_FCMPODDB:  gen_alopf7_env_ddd(alop, gen_helper_fcmpodd); break;
    case OP_FXCMPEQSB:  gen_alopf7_env_xsd(alop, gen_helper_fxcmpeqs); break;
    case OP_FXCMPLTSB:  gen_alopf7_env_xsd(alop, gen_helper_fxcmplts); break;
    case OP_FXCMPLESB:  gen_alopf7_env_xsd(alop, gen_helper_fxcmples); break;
    case OP_FXCMPUODSB: gen_alopf7_env_xsd(alop, gen_helper_fxcmpuods); break;
    case OP_FXCMPNEQSB: gen_alopf7_env_xsd(alop, gen_helper_fxcmpneqs); break;
    case OP_FXCMPNLTSB: gen_alopf7_env_xsd(alop, gen_helper_fxcmpnlts); break;
    case OP_FXCMPNLESB: gen_alopf7_env_xsd(alop, gen_helper_fxcmpnles); break;
    case OP_FXCMPODSB:  gen_alopf7_env_xsd(alop, gen_helper_fxcmpods); break;
    case OP_FXCMPEQDB:  gen_alopf7_env_xdd(alop, gen_helper_fxcmpeqd); break;
    case OP_FXCMPLTDB:  gen_alopf7_env_xdd(alop, gen_helper_fxcmpltd); break;
    case OP_FXCMPLEDB:  gen_alopf7_env_xdd(alop, gen_helper_fxcmpled); break;
    case OP_FXCMPUODDB: gen_alopf7_env_xdd(alop, gen_helper_fxcmpuodd); break;
    case OP_FXCMPNEQDB: gen_alopf7_env_xdd(alop, gen_helper_fxcmpneqd); break;
    case OP_FXCMPNLTDB: gen_alopf7_env_xdd(alop, gen_helper_fxcmpnltd); break;
    case OP_FXCMPNLEDB: gen_alopf7_env_xdd(alop, gen_helper_fxcmpnled); break;
    case OP_FXCMPODDB:  gen_alopf7_env_xdd(alop, gen_helper_fxcmpodd); break;
    case OP_FXCMPEQXB:  gen_alopf7_env_xxd(alop, gen_helper_fxcmpeqx); break;
    case OP_FXCMPLTXB:  gen_alopf7_env_xxd(alop, gen_helper_fxcmpltx); break;
    case OP_FXCMPLEXB:  gen_alopf7_env_xxd(alop, gen_helper_fxcmplex); break;
    case OP_FXCMPUODXB: gen_alopf7_env_xxd(alop, gen_helper_fxcmpuodx); break;
    case OP_FXCMPNEQXB: gen_alopf7_env_xxd(alop, gen_helper_fxcmpneqx); break;
    case OP_FXCMPNLTXB: gen_alopf7_env_xxd(alop, gen_helper_fxcmpnltx); break;
    case OP_FXCMPNLEXB: gen_alopf7_env_xxd(alop, gen_helper_fxcmpnlex); break;
    case OP_FXCMPODXB:  gen_alopf7_env_xxd(alop, gen_helper_fxcmpodx); break;
    case OP_STB: gen_stb(alop, gen_addr_i64, ADDR_FLAT); break;
    case OP_STH: gen_sth(alop, gen_addr_i64, ADDR_FLAT); break;
    case OP_STW: gen_stw(alop, gen_addr_i64, ADDR_FLAT); break;
    case OP_STD: gen_std(alop, gen_addr_i64, ADDR_FLAT); break;
    case OP_STQP: gen_stqp(alop, gen_addr_i64, ADDR_FLAT); break;
    case OP_STMQP: gen_stmqp(alop, gen_addr_src1_i64, ADDR_FLAT); break;
    case OP_STCSB: gen_stb(alop, gen_addr_i32, ADDR_CS); break;
    case OP_STDSB: gen_stb(alop, gen_addr_i32, ADDR_DS); break;
    case OP_STESB: gen_stb(alop, gen_addr_i32, ADDR_ES); break;
    case OP_STFSB: gen_stb(alop, gen_addr_i32, ADDR_FS); break;
    case OP_STGDB: gen_stb(alop, gen_addr_i32, ADDR_GD); break;
    case OP_STGSB: gen_stb(alop, gen_addr_i32, ADDR_GS); break;
    case OP_STSSB: gen_stb(alop, gen_addr_i32, ADDR_SS); break;
    case OP_STCSH: gen_sth(alop, gen_addr_i32, ADDR_CS); break;
    case OP_STDSH: gen_sth(alop, gen_addr_i32, ADDR_DS); break;
    case OP_STESH: gen_sth(alop, gen_addr_i32, ADDR_ES); break;
    case OP_STFSH: gen_sth(alop, gen_addr_i32, ADDR_FS); break;
    case OP_STGDH: gen_sth(alop, gen_addr_i32, ADDR_GD); break;
    case OP_STGSH: gen_sth(alop, gen_addr_i32, ADDR_GS); break;
    case OP_STSSH: gen_sth(alop, gen_addr_i32, ADDR_SS); break;
    case OP_STCSW: gen_stw(alop, gen_addr_i32, ADDR_CS); break;
    case OP_STDSW: gen_stw(alop, gen_addr_i32, ADDR_DS); break;
    case OP_STESW: gen_stw(alop, gen_addr_i32, ADDR_ES); break;
    case OP_STFSW: gen_stw(alop, gen_addr_i32, ADDR_FS); break;
    case OP_STGDW: gen_stw(alop, gen_addr_i32, ADDR_GD); break;
    case OP_STGSW: gen_stw(alop, gen_addr_i32, ADDR_GS); break;
    case OP_STSSW: gen_stw(alop, gen_addr_i32, ADDR_SS); break;
    case OP_STCSD: gen_std(alop, gen_addr_i32, ADDR_CS); break;
    case OP_STDSD: gen_std(alop, gen_addr_i32, ADDR_DS); break;
    case OP_STESD: gen_std(alop, gen_addr_i32, ADDR_ES); break;
    case OP_STFSD: gen_std(alop, gen_addr_i32, ADDR_FS); break;
    case OP_STGDD: gen_std(alop, gen_addr_i32, ADDR_GD); break;
    case OP_STGSD: gen_std(alop, gen_addr_i32, ADDR_GS); break;
    case OP_STSSD: gen_std(alop, gen_addr_i32, ADDR_SS); break;
    case OP_STCSQP: gen_stqp(alop, gen_addr_i32, ADDR_CS); break;
    case OP_STDSQP: gen_stqp(alop, gen_addr_i32, ADDR_DS); break;
    case OP_STESQP: gen_stqp(alop, gen_addr_i32, ADDR_ES); break;
    case OP_STFSQP: gen_stqp(alop, gen_addr_i32, ADDR_FS); break;
    case OP_STGDQP: gen_stqp(alop, gen_addr_i32, ADDR_GD); break;
    case OP_STGSQP: gen_stqp(alop, gen_addr_i32, ADDR_GS); break;
    case OP_STSSQP: gen_stqp(alop, gen_addr_i32, ADDR_SS); break;
    case OP_STCSMQP: gen_stmqp(alop, gen_addr_src1_i32, ADDR_CS); break;
    case OP_STDSMQP: gen_stmqp(alop, gen_addr_src1_i32, ADDR_DS); break;
    case OP_STESMQP: gen_stmqp(alop, gen_addr_src1_i32, ADDR_ES); break;
    case OP_STFSMQP: gen_stmqp(alop, gen_addr_src1_i32, ADDR_FS); break;
    case OP_STGDMQP: gen_stmqp(alop, gen_addr_src1_i32, ADDR_GD); break;
    case OP_STGSMQP: gen_stmqp(alop, gen_addr_src1_i32, ADDR_GS); break;
    case OP_STSSMQP: gen_stmqp(alop, gen_addr_src1_i32, ADDR_SS); break;
    case OP_LDB: gen_ldb(alop, gen_addr_i64, ADDR_FLAT); break;
    case OP_LDH: gen_ldh(alop, gen_addr_i64, ADDR_FLAT); break;
    case OP_LDW: gen_ldw(alop, gen_addr_i64, ADDR_FLAT); break;
    case OP_LDD: gen_ldd(alop, gen_addr_i64, ADDR_FLAT); break;
    case OP_LDQP: gen_ldqp(alop, gen_addr_i64, ADDR_FLAT); break;
#ifdef TARGET_E2K32
    case OP_LDCSB: gen_ldb(alop, gen_addr_i32, ADDR_CS); break;
    case OP_LDDSB: gen_ldb(alop, gen_addr_i32, ADDR_DS); break;
    case OP_LDESB: gen_ldb(alop, gen_addr_i32, ADDR_ES); break;
    case OP_LDFSB: gen_ldb(alop, gen_addr_i32, ADDR_FS); break;
    case OP_LDGDB: gen_ldb(alop, gen_addr_i32, ADDR_GD); break;
    case OP_LDGSB: gen_ldb(alop, gen_addr_i32, ADDR_GS); break;
    case OP_LDSSB: gen_ldb(alop, gen_addr_i32, ADDR_SS); break;
    case OP_LDCSH: gen_ldh(alop, gen_addr_i32, ADDR_CS); break;
    case OP_LDDSH: gen_ldh(alop, gen_addr_i32, ADDR_DS); break;
    case OP_LDESH: gen_ldh(alop, gen_addr_i32, ADDR_ES); break;
    case OP_LDFSH: gen_ldh(alop, gen_addr_i32, ADDR_FS); break;
    case OP_LDGDH: gen_ldh(alop, gen_addr_i32, ADDR_GD); break;
    case OP_LDGSH: gen_ldh(alop, gen_addr_i32, ADDR_GS); break;
    case OP_LDSSH: gen_ldh(alop, gen_addr_i32, ADDR_SS); break;
    case OP_LDCSW: gen_ldw(alop, gen_addr_i32, ADDR_CS); break;
    case OP_LDDSW: gen_ldw(alop, gen_addr_i32, ADDR_DS); break;
    case OP_LDESW: gen_ldw(alop, gen_addr_i32, ADDR_ES); break;
    case OP_LDFSW: gen_ldw(alop, gen_addr_i32, ADDR_FS); break;
    case OP_LDGDW: gen_ldw(alop, gen_addr_i32, ADDR_GD); break;
    case OP_LDGSW: gen_ldw(alop, gen_addr_i32, ADDR_GS); break;
    case OP_LDSSW: gen_ldw(alop, gen_addr_i32, ADDR_SS); break;
    case OP_LDCSD: gen_ldd(alop, gen_addr_i32, ADDR_CS); break;
    case OP_LDDSD: gen_ldd(alop, gen_addr_i32, ADDR_DS); break;
    case OP_LDESD: gen_ldd(alop, gen_addr_i32, ADDR_ES); break;
    case OP_LDFSD: gen_ldd(alop, gen_addr_i32, ADDR_FS); break;
    case OP_LDGDD: gen_ldd(alop, gen_addr_i32, ADDR_GD); break;
    case OP_LDGSD: gen_ldd(alop, gen_addr_i32, ADDR_GS); break;
    case OP_LDSSD: gen_ldd(alop, gen_addr_i32, ADDR_SS); break;
    case OP_LDCSQP: gen_ldqp(alop, gen_addr_i32, ADDR_CS); break;
    case OP_LDDSQP: gen_ldqp(alop, gen_addr_i32, ADDR_DS); break;
    case OP_LDESQP: gen_ldqp(alop, gen_addr_i32, ADDR_ES); break;
    case OP_LDFSQP: gen_ldqp(alop, gen_addr_i32, ADDR_FS); break;
    case OP_LDGDQP: gen_ldqp(alop, gen_addr_i32, ADDR_GD); break;
    case OP_LDGSQP: gen_ldqp(alop, gen_addr_i32, ADDR_GS); break;
    case OP_LDSSQP: gen_ldqp(alop, gen_addr_i32, ADDR_SS); break;
#else
    case OP_LDCSB:
    case OP_LDDSB:
    case OP_LDESB:
    case OP_LDFSB:
    case OP_LDGDB:
    case OP_LDGSB:
    case OP_LDSSB:
    case OP_LDCSH:
    case OP_LDDSH:
    case OP_LDESH:
    case OP_LDFSH:
    case OP_LDGDH:
    case OP_LDGSH:
    case OP_LDSSH:
    case OP_LDCSW:
    case OP_LDDSW:
    case OP_LDESW:
    case OP_LDFSW:
    case OP_LDGDW:
    case OP_LDGSW:
    case OP_LDSSW:
    case OP_LDCSD:
    case OP_LDDSD:
    case OP_LDESD:
    case OP_LDFSD:
    case OP_LDGDD:
    case OP_LDGSD:
    case OP_LDSSD:
    case OP_LDCSQP:
    case OP_LDDSQP:
    case OP_LDESQP:
    case OP_LDFSQP:
    case OP_LDGDQP:
    case OP_LDGSQP:
    case OP_LDSSQP:
        // FIXME: find out why the exception is generated
        gen_tr_excp_array_bounds(ctx);
        break;
#endif
    case OP_BITREVS: gen_alopf2_ss(alop, gen_bitrevs); break;
    case OP_BITREVD: gen_alopf2_dd(alop, gen_bitrevd); break;
    case OP_LZCNTS: gen_alopf2_ss(alop, gen_lzcnts); break;
    case OP_LZCNTD: gen_alopf2_dd(alop, gen_lzcntd); break;
    case OP_POPCNTS: gen_alopf2_ss(alop, tcg_gen_ctpop_i32); break;
    case OP_POPCNTD: gen_alopf2_dd(alop, tcg_gen_ctpop_i64); break;
    case OP_FADDS: gen_alopf1_env_sss(alop, gen_helper_fadds); break;
    case OP_FADDD: gen_alopf1_env_ddd(alop, gen_helper_faddd); break;
    case OP_FSUBS: gen_alopf1_env_sss(alop, gen_helper_fsubs); break;
    case OP_FSUBD: gen_alopf1_env_ddd(alop, gen_helper_fsubd); break;
    case OP_FMINS: gen_alopf1_env_sss(alop, gen_helper_fmins); break;
    case OP_FMIND: gen_alopf1_env_ddd(alop, gen_helper_fmind); break;
    case OP_FMAXS: gen_alopf1_env_sss(alop, gen_helper_fmaxs); break;
    case OP_FMAXD: gen_alopf1_env_ddd(alop, gen_helper_fmaxd); break;
    case OP_FMULS: gen_alopf1_env_sss(alop, gen_helper_fmuls); break;
    case OP_FMULD: gen_alopf1_env_ddd(alop, gen_helper_fmuld); break;
    case OP_FCMPEQS: gen_alopf1_env_sss(alop, gen_helper_fcmpeqs); break;
    case OP_FCMPLTS: gen_alopf1_env_sss(alop, gen_helper_fcmplts); break;
    case OP_FCMPLES: gen_alopf1_env_sss(alop, gen_helper_fcmples); break;
    case OP_FCMPUODS: gen_alopf1_env_sss(alop, gen_helper_fcmpuods); break;
    case OP_FCMPNEQS: gen_alopf1_env_sss(alop, gen_helper_fcmpneqs); break;
    case OP_FCMPNLTS: gen_alopf1_env_sss(alop, gen_helper_fcmpnlts); break;
    case OP_FCMPNLES: gen_alopf1_env_sss(alop, gen_helper_fcmpnles); break;
    case OP_FCMPODS: gen_alopf1_env_sss(alop, gen_helper_fcmpods); break;
    case OP_FCMPEQD: gen_alopf1_env_ddd(alop, gen_helper_fcmpeqd); break;
    case OP_FCMPLTD: gen_alopf1_env_ddd(alop, gen_helper_fcmpltd); break;
    case OP_FCMPLED: gen_alopf1_env_ddd(alop, gen_helper_fcmpled); break;
    case OP_FCMPUODD: gen_alopf1_env_ddd(alop, gen_helper_fcmpuodd); break;
    case OP_FCMPNEQD: gen_alopf1_env_ddd(alop, gen_helper_fcmpneqd); break;
    case OP_FCMPNLTD: gen_alopf1_env_ddd(alop, gen_helper_fcmpnltd); break;
    case OP_FCMPNLED: gen_alopf1_env_ddd(alop, gen_helper_fcmpnled); break;
    case OP_FCMPODD: gen_alopf1_env_ddd(alop, gen_helper_fcmpodd); break;
    case OP_FSTOIS: gen_alopf2_env_ss(alop, gen_helper_fstois); break;
    case OP_FSTOISTR: gen_alopf2_env_ss(alop, gen_helper_fstoistr); break;
    case OP_ISTOFS: gen_alopf2_env_ss(alop, gen_helper_istofs); break;
    case OP_FDTOID: gen_alopf2_env_dd(alop, gen_helper_fdtoid); break;
    case OP_IDTOFD: gen_alopf2_env_dd(alop, gen_helper_idtofd); break;
    case OP_FXTOFD: gen_alopf2_env_xd(alop, gen_helper_fxtofd); break;
    case OP_FDTOFX: gen_alopf2_env_dx(alop, gen_helper_fdtofx); break;
    case OP_FSTOID: gen_alopf2_env_sd(alop, gen_helper_fstoid); break;
    case OP_FSTOIDTR: gen_alopf2_env_sd(alop, gen_helper_fstoidtr); break;
    case OP_FDTOIDTR: gen_alopf2_env_dd(alop, gen_helper_fdtoidtr); break;
    case OP_ISTOFD: gen_alopf2_env_sd(alop, gen_helper_istofd); break;
    case OP_FSTOFD: gen_alopf2_env_sd(alop, gen_helper_fstofd); break;
    case OP_FSTOFX: gen_alopf2_env_sx(alop, gen_helper_fstofx); break;
    case OP_FDTOISTR: gen_alopf2_env_ds(alop, gen_helper_fdtoistr); break;
    case OP_FDTOIS: gen_alopf2_env_ds(alop, gen_helper_fdtois); break;
    case OP_IDTOFS: gen_alopf2_env_ds(alop, gen_helper_idtofs); break;
    case OP_FDTOFS: gen_alopf2_env_ds(alop, gen_helper_fdtofs); break;
    case OP_FXTOFS: gen_alopf2_env_xs(alop, gen_helper_fxtofs); break;
    case OP_FXTOIS: gen_alopf2_env_xs(alop, gen_helper_fxtois); break;
    case OP_FXTOISTR: gen_alopf2_env_xs(alop, gen_helper_fxtoistr); break;
    case OP_FXTOID: gen_alopf2_env_xd(alop, gen_helper_fxtoid); break;
    case OP_FXTOIDTR: gen_alopf2_env_xd(alop, gen_helper_fxtoidtr); break;
    case OP_ISTOFX: gen_alopf2_env_sx(alop, gen_helper_istofx); break;
    case OP_IDTOFX: gen_alopf2_env_dx(alop, gen_helper_idtofx); break;
    case OP_UDIVS: gen_alopf1_tag_sss(alop, gen_udivs); break;
    case OP_UDIVD: gen_alopf1_tag_ddd(alop, gen_udivd); break;
    case OP_SDIVS: gen_alopf1_tag_sss(alop, gen_sdivs); break;
    case OP_SDIVD: gen_alopf1_tag_ddd(alop, gen_sdivd); break;
    case OP_FXADDSS: gen_alopf1_env_xss(alop, gen_helper_fxaddss); break;
    case OP_FXADDDD: gen_alopf1_env_xdd(alop, gen_helper_fxadddd); break;
    case OP_FXADDSX: gen_alopf1_env_xsx(alop, gen_helper_fxaddsx); break;
    case OP_FXADDDX: gen_alopf1_env_xdx(alop, gen_helper_fxadddx); break;
    case OP_FXADDXX: gen_alopf1_env_xxx(alop, gen_helper_fxaddxx); break;
    case OP_FXADDXD: gen_alopf1_env_xxd(alop, gen_helper_fxaddxd); break;
    case OP_FXADDXS: gen_alopf1_env_xxs(alop, gen_helper_fxaddxs); break;
    case OP_FXSUBSS: gen_alopf1_env_xss(alop, gen_helper_fxsubss); break;
    case OP_FXSUBDD: gen_alopf1_env_xdd(alop, gen_helper_fxsubdd); break;
    case OP_FXSUBSX: gen_alopf1_env_xsx(alop, gen_helper_fxsubsx); break;
    case OP_FXSUBDX: gen_alopf1_env_xdx(alop, gen_helper_fxsubdx); break;
    case OP_FXSUBXX: gen_alopf1_env_xxx(alop, gen_helper_fxsubxx); break;
    case OP_FXSUBXD: gen_alopf1_env_xxd(alop, gen_helper_fxsubxd); break;
    case OP_FXSUBXS: gen_alopf1_env_xxs(alop, gen_helper_fxsubxs); break;
    case OP_FXRSUBSS: gen_alopf1_env_xss(alop, gen_helper_fxrsubss); break;
    case OP_FXRSUBDD: gen_alopf1_env_xdd(alop, gen_helper_fxrsubdd); break;
    case OP_FXRSUBSX: gen_alopf1_env_xsx(alop, gen_helper_fxrsubsx); break;
    case OP_FXRSUBDX: gen_alopf1_env_xdx(alop, gen_helper_fxrsubdx); break;
    case OP_FXMULSS: gen_alopf1_env_xss(alop, gen_helper_fxmulss); break;
    case OP_FXMULDD: gen_alopf1_env_xdd(alop, gen_helper_fxmuldd); break;
    case OP_FXMULSX: gen_alopf1_env_xsx(alop, gen_helper_fxmulsx); break;
    case OP_FXMULDX: gen_alopf1_env_xdx(alop, gen_helper_fxmuldx); break;
    case OP_FXMULXX: gen_alopf1_env_xxx(alop, gen_helper_fxmulxx); break;
    case OP_FXMULXD: gen_alopf1_env_xxd(alop, gen_helper_fxmulxd); break;
    case OP_FXMULXS: gen_alopf1_env_xxs(alop, gen_helper_fxmulxs); break;
    case OP_FXDIVSS: gen_alopf1_env_xss(alop, gen_helper_fxdivss); break;
    case OP_FXDIVDD: gen_alopf1_env_xdd(alop, gen_helper_fxdivdd); break;
    case OP_FXDIVSX: gen_alopf1_env_xsx(alop, gen_helper_fxdivsx); break;
    case OP_FXDIVDX: gen_alopf1_env_xdx(alop, gen_helper_fxdivdx); break;
    case OP_FXDIVXX: gen_alopf1_env_xxx(alop, gen_helper_fxdivxx); break;
    case OP_FXDIVXD: gen_alopf1_env_xxd(alop, gen_helper_fxdivxd); break;
    case OP_FXDIVXS: gen_alopf1_env_xxs(alop, gen_helper_fxdivxs); break;
    case OP_MOVFI: gen_alopf2_xs(alop, gen_movfi); break;
    case OP_MOVIF: gen_alopf1_dsx(alop, gen_movif); break;
    case OP_MOVX: gen_alopf2_xx(alop, gen_movx); break;
    case OP_MOVXA: gen_alopf2_xx(alop, gen_movxa); break;
    case OP_MOVXC: gen_alopf2_xx(alop, gen_movxc); break;
    case OP_MOVTS: gen_movts(alop); break;
    case OP_MOVTCS: gen_movtcs(alop); break;
    case OP_MOVTD: gen_movtd(alop); break;
    case OP_MOVTCD: gen_movtcd(alop); break;
    case OP_MOVTQ: gen_movtq(alop); break;
    case OP_MOVTCQ: gen_movtcq(alop); break;
    case OP_GETPL: gen_getpl(alop); break;
    case OP_PANDD: gen_alopf1_ddd(alop, tcg_gen_and_i64); break;
    case OP_PANDND: gen_alopf1_ddd(alop, gen_andn_i64); break;
    case OP_PORD: gen_alopf1_ddd(alop, tcg_gen_or_i64); break;
    case OP_PXORD: gen_alopf1_ddd(alop, tcg_gen_xor_i64); break;
    case OP_PMINUB: gen_alopf1_ddd(alop, gen_helper_pminub); break;
    case OP_PMINSB: gen_alopf1_ddd(alop, gen_helper_pminsb); break;
    case OP_PMINUH: gen_alopf1_ddd(alop, gen_helper_pminuh); break;
    case OP_PMINSH: gen_alopf1_ddd(alop, gen_helper_pminsh); break;
    case OP_PMINUW: gen_alopf1_ddd(alop, gen_helper_pminuw); break;
    case OP_PMINSW: gen_alopf1_ddd(alop, gen_helper_pminsw); break;
    case OP_PMAXUB: gen_alopf1_ddd(alop, gen_helper_pmaxub); break;
    case OP_PMAXSB: gen_alopf1_ddd(alop, gen_helper_pmaxsb); break;
    case OP_PMAXUH: gen_alopf1_ddd(alop, gen_helper_pmaxuh); break;
    case OP_PMAXSH: gen_alopf1_ddd(alop, gen_helper_pmaxsh); break;
    case OP_PMAXUW: gen_alopf1_ddd(alop, gen_helper_pmaxuw); break;
    case OP_PMAXSW: gen_alopf1_ddd(alop, gen_helper_pmaxsw); break;
    case OP_PCMPEQB: gen_alopf1_ddd(alop, gen_helper_pcmpeqb); break;
    case OP_PCMPEQH: gen_alopf1_ddd(alop, gen_helper_pcmpeqh); break;
    case OP_PCMPEQW: gen_alopf1_ddd(alop, gen_helper_pcmpeqw); break;
    case OP_PCMPEQD: gen_alopf1_ddd(alop, gen_helper_pcmpeqd); break;
    case OP_PCMPGTB: gen_alopf1_ddd(alop, gen_helper_pcmpgtb); break;
    case OP_PCMPGTH: gen_alopf1_ddd(alop, gen_helper_pcmpgth); break;
    case OP_PCMPGTW: gen_alopf1_ddd(alop, gen_helper_pcmpgtw); break;
    case OP_PCMPGTD: gen_alopf1_ddd(alop, gen_helper_pcmpgtd); break;
    case OP_PADDB: gen_alopf1_ddd(alop, tcg_gen_vec_add8_i64); break;
    case OP_PADDH: gen_alopf1_ddd(alop, tcg_gen_vec_add16_i64); break;
    case OP_PADDW: gen_alopf1_ddd(alop, tcg_gen_vec_add32_i64); break;
    case OP_PADDD: gen_alopf1_ddd(alop, tcg_gen_add_i64); break;
    case OP_PADDSB: gen_alopf1_ddd(alop, gen_helper_paddsb); break;
    case OP_PADDSH: gen_alopf1_ddd(alop, gen_helper_paddsh); break;
    case OP_PADDUSB: gen_alopf1_ddd(alop, gen_helper_paddusb); break;
    case OP_PADDUSH: gen_alopf1_ddd(alop, gen_helper_paddush); break;
    case OP_PHADDH: gen_alopf1_ddd(alop, gen_helper_phaddh); break;
    case OP_PHADDW: gen_alopf1_ddd(alop, gen_helper_phaddw); break;
    case OP_PHADDSH:  gen_alopf1_ddd(alop, gen_helper_phaddsh); break;
    case OP_PSUBB: gen_alopf1_ddd(alop, tcg_gen_vec_sub8_i64); break;
    case OP_PSUBH: gen_alopf1_ddd(alop, tcg_gen_vec_sub16_i64); break;
    case OP_PSUBW: gen_alopf1_ddd(alop, tcg_gen_vec_sub32_i64); break;
    case OP_PSUBD: gen_alopf1_ddd(alop, tcg_gen_sub_i64); break;
    case OP_PSUBSB: gen_alopf1_ddd(alop, gen_helper_psubsb); break;
    case OP_PSUBSH: gen_alopf1_ddd(alop, gen_helper_psubsh); break;
    case OP_PSUBUSB: gen_alopf1_ddd(alop, gen_helper_psubusb); break;
    case OP_PSUBUSH: gen_alopf1_ddd(alop, gen_helper_psubush); break;
    case OP_PHSUBH: gen_alopf1_ddd(alop, gen_helper_phsubh); break;
    case OP_PHSUBW: gen_alopf1_ddd(alop, gen_helper_phsubw); break;
    case OP_PHSUBSH: gen_alopf1_ddd(alop, gen_helper_phsubsh); break;
    case OP_PMULHH: gen_alopf1_ddd(alop, gen_helper_pmulhh); break;
    case OP_PMULLH: gen_alopf1_ddd(alop, gen_helper_pmullh); break;
    case OP_PMULHUH: gen_alopf1_ddd(alop, gen_helper_pmulhuh); break;
    case OP_PMULUBHH: gen_alopf1_ddd(alop, gen_helper_pmulubhh); break;
    case OP_PMULHRSH: gen_alopf1_ddd(alop, gen_helper_pmulhrsh); break;
    case OP_PMADDH: gen_alopf1_ddd(alop, gen_helper_pmaddh); break;
    case OP_PMADDUBSH: gen_alopf1_ddd(alop, gen_helper_pmaddubsh); break;
    case OP_MPSADBH: gen_alopf1_ddd(alop, gen_helper_mpsadbh); break;
    case OP_PSADBW: gen_alopf1_ddd(alop, gen_helper_psadbw); break;
    case OP_PSIGNB: gen_alopf1_ddd(alop, gen_helper_psignb); break;
    case OP_PSIGNH: gen_alopf1_ddd(alop, gen_helper_psignh); break;
    case OP_PSIGNW: gen_alopf1_ddd(alop, gen_helper_psignw); break;
    case OP_PSLLH: gen_alopf1_ddd(alop, gen_helper_psllh); break;
    case OP_PSLLW: gen_alopf1_ddd(alop, gen_helper_psllw); break;
    case OP_PSLLD: gen_alopf1_ddd(alop, gen_pslld); break;
    case OP_PSRLH: gen_alopf1_ddd(alop, gen_helper_psrlh); break;
    case OP_PSRLW: gen_alopf1_ddd(alop, gen_helper_psrlw); break;
    case OP_PSRLD: gen_alopf1_ddd(alop, gen_psrld); break;
    case OP_PSRAH: gen_alopf1_ddd(alop, gen_helper_psrah); break;
    case OP_PSRAW: gen_alopf1_ddd(alop, gen_helper_psraw); break;
    case OP_PAVGUSB: gen_alopf1_ddd(alop, gen_helper_pavgusb); break;
    case OP_PAVGUSH: gen_alopf1_ddd(alop, gen_helper_pavgush); break;
    case OP_PSLLQL: gen_alopf11_lit8_ddd(alop, gen_psllql); break;
    case OP_PSLLQH: gen_alopf11_lit8_ddd(alop, gen_psllqh); break;
    case OP_PSRLQL: gen_alopf11_lit8_ddd(alop, gen_psrlql); break;
    case OP_PSRLQH: gen_alopf11_lit8_ddd(alop, gen_psrlqh); break;
    case OP_PINSH: gen_alopf11_lit8_ddd(alop, gen_pinsh); break;
    case OP_PEXTRH: gen_alopf11_lit8_ddd(alop, gen_pextrh); break;
    case OP_PSHUFH: gen_alopf2_pshufh(alop, gen_helper_pshufh); break;
    case OP_PSHUFW: gen_alopf11_lit8_ddd(alop, gen_pshufw); break;
    case OP_PMOVMSKB: gen_alopf1_ddd(alop, gen_helper_pmovmskb); break;
    case OP_PMOVMSKPS: gen_alopf1_ddd(alop, gen_helper_pmovmskps); break;
    case OP_PMOVMSKPD: gen_alopf1_ddd(alop, gen_helper_pmovmskpd); break;
    case OP_PACKSSHB: gen_alopf1_ddd(alop, gen_helper_packsshb); break;
    case OP_PACKUSHB: gen_alopf1_ddd(alop, gen_helper_packushb); break;
    case OP_PACKSSWH: gen_alopf1_ddd(alop, gen_helper_packsswh); break;
    case OP_PACKUSWH: gen_alopf1_ddd(alop, gen_helper_packuswh); break;
    case OP_PUNPCKLBH: gen_alopf1_ddd(alop, gen_helper_punpcklbh); break;
    case OP_PUNPCKLHW: gen_alopf1_ddd(alop, gen_helper_punpcklhw); break;
    case OP_PUNPCKLWD: gen_alopf1_ddd(alop, gen_helper_punpcklwd); break;
    case OP_PUNPCKHBH: gen_alopf1_ddd(alop, gen_helper_punpckhbh); break;
    case OP_PUNPCKHHW: gen_alopf1_ddd(alop, gen_helper_punpckhhw); break;
    case OP_PUNPCKHWD: gen_alopf1_ddd(alop, gen_helper_punpckhwd); break;
    case OP_PHMINPOSUH: gen_alopf1_ddd(alop, gen_helper_phminposuh); break;
    case OP_GETTAGS: gen_gettags(alop); break;
    case OP_GETTAGD: gen_gettagd(alop); break;
    case OP_PUTTAGS: gen_puttags(alop); break;
    case OP_PUTTAGD: gen_puttagd(alop); break;
    case OP_PUTTAGQP: gen_puttagqp(alop); break;
    case OP_STAAB: gen_staa_i32(alop, MO_8); break;
    case OP_STAAH: gen_staa_i32(alop, MO_16); break;
    case OP_STAAW: gen_staa_i32(alop, MO_32); break;
    case OP_STAAD: gen_staa_i64(alop); break;
    case OP_STAAQ: {
        int pair_chan = alop->chan == 2 ? 5 : 2;
        if (!ctx->bundle.als_present[pair_chan] ||
            extract32(ctx->bundle.als[pair_chan], 24, 7) != 0x3f ||
            (alop->als.dst & 1) != (alop->chan == 2 ? 0 : 1))
        {
            gen_tr_excp_illopc(ctx);
            return;
        }
        gen_staa_i64(alop);
        break;
    }
    case OP_STAAQP: gen_staaqp(alop); break;
    case OP_MULS: gen_alopf1_sss(alop, tcg_gen_mul_i32); break;
    case OP_MULD: gen_alopf1_ddd(alop, tcg_gen_mul_i64); break;
    case OP_UMULX: gen_alopf1_ssd(alop, gen_umulx); break;
    case OP_SMULX: gen_alopf1_ssd(alop, gen_smulx); break;
    case OP_RWS: gen_rws(alop); break;
    case OP_RWD: gen_rwd(alop); break;
    case OP_RRS: gen_rrd(alop); break;
    case OP_RRD: gen_rrd(alop); break;
    case OP_FDIVS: gen_alopf1_env_sss(alop, gen_helper_fdivs); break;
    case OP_FDIVD: gen_alopf1_env_ddd(alop, gen_helper_fdivd); break;
    case OP_GETSP: gen_alopf2_env_sd(alop, gen_helper_getsp); break;
    case OP_UMULHD: gen_alopf1_ddd(alop, gen_umulhd); break;
    case OP_SMULHD: gen_alopf1_ddd(alop, gen_smulhd); break;
    case OP_FCMPODSF: gen_alopf1_env_sss(alop, gen_helper_fcmpodsf); break;
    case OP_FCMPUDSF: gen_alopf1_env_sss(alop, gen_helper_fcmpudsf); break;
    case OP_FCMPODDF: gen_alopf1_env_dds(alop, gen_helper_fcmpoddf); break;
    case OP_FCMPUDDF: gen_alopf1_env_dds(alop, gen_helper_fcmpoddf); break;
    case OP_FXCMPODSF: gen_alopf1_env_xss(alop, gen_helper_fxcmpodsf); break;
    case OP_FXCMPUDSF: gen_alopf1_env_xss(alop, gen_helper_fxcmpudsf); break;
    case OP_FXCMPODDF: gen_alopf1_env_xds(alop, gen_helper_fxcmpoddf); break;
    case OP_FXCMPUDDF: gen_alopf1_env_xds(alop, gen_helper_fxcmpuddf); break;
    case OP_FXCMPODXF: gen_alopf1_env_xxs(alop, gen_helper_fxcmpodxf); break;
    case OP_FXCMPUDXF: gen_alopf1_env_xxs(alop, gen_helper_fxcmpudxf); break;
    case OP_FSTOIFS: gen_alopf1_env_sss(alop, gen_helper_fstoifs); break;
    case OP_FDTOIFD: gen_alopf1_env_ddd(alop, gen_helper_fdtoifd); break;
    case OP_UDIVX: gen_alopf1_tag_dss(alop, gen_udivx); break;
    case OP_UMODX: gen_alopf1_tag_dss(alop, gen_umodx); break;
    case OP_SDIVX: gen_alopf1_tag_dss(alop, gen_sdivx); break;
    case OP_SMODX: gen_alopf1_tag_dss(alop, gen_smodx); break;
    case OP_PFMULD: gen_alopf1_env_ddd(alop, gen_helper_fmuld); break;
    case OP_PFADDD: gen_alopf1_env_ddd(alop, gen_helper_faddd); break;
    case OP_PFSUBD: gen_alopf1_env_ddd(alop, gen_helper_fsubd); break;
    case OP_PFDIVD: gen_alopf1_env_ddd(alop, gen_helper_fdivd); break;
    case OP_PFMIND: gen_alopf1_env_ddd(alop, gen_helper_fmind); break;
    case OP_PFMAXD: gen_alopf1_env_ddd(alop, gen_helper_fmaxd); break;
    case OP_PFADDS: gen_alopf1_env_ddd(alop, gen_helper_pfadds); break;
    case OP_PFSUBS: gen_alopf1_env_ddd(alop, gen_helper_pfsubs); break;
    case OP_PFMULS: gen_alopf1_env_ddd(alop, gen_helper_pfmuls); break;
    case OP_PFDIVS: gen_alopf1_env_sss(alop, gen_helper_fdivs); break;
    case OP_PFMAXS: gen_alopf1_env_ddd(alop, gen_helper_pfmaxs); break;
    case OP_PFMINS: gen_alopf1_env_ddd(alop, gen_helper_pfmins); break;
    case OP_PFHADDS: gen_alopf1_env_ddd(alop, gen_helper_pfhadds); break;
    case OP_PFHSUBS: gen_alopf1_env_ddd(alop, gen_helper_pfhsubs); break;
    case OP_PFADDSUBS: gen_alopf1_env_ddd(alop, gen_helper_pfaddsubs); break;
    case OP_PFSQRTS: gen_alopf2_env_ss(alop, gen_helper_fsqrts); break;
    case OP_PFSTOIFS: gen_alopf1_env_ddd(alop, gen_helper_pfstoifs); break;
    case OP_PISTOFS: gen_alopf2_env_dd(alop, gen_helper_pistofs); break;
    case OP_PFSTOIS: gen_alopf2_env_dd(alop, gen_helper_pfstois); break;
    case OP_PFSTOISTR: gen_alopf2_env_dd(alop, gen_helper_pfstoistr); break;
    case OP_PFSTOFD: gen_alopf2_env_sd(alop, gen_helper_fstofd); break;
    case OP_PFDTOFS: gen_alopf2_env_ds(alop, gen_helper_fdtofs); break;
    case OP_PFDTOIFD: gen_alopf1_env_ddd(alop, gen_helper_fdtoifd); break;
    case OP_PFDTOIS: gen_alopf2_env_ds(alop, gen_helper_fdtois); break;
    case OP_PFDTOISTR: gen_alopf2_env_ds(alop, gen_helper_fdtoistr); break;
    case OP_PFCMPEQS: gen_alopf1_env_ddd(alop, gen_helper_pfcmpeqs); break;
    case OP_PFCMPLTS: gen_alopf1_env_ddd(alop, gen_helper_pfcmplts); break;
    case OP_PFCMPLES: gen_alopf1_env_ddd(alop, gen_helper_pfcmples); break;
    case OP_PFCMPUODS: gen_alopf1_env_ddd(alop, gen_helper_pfcmpuods); break;
    case OP_PFCMPNEQS: gen_alopf1_env_ddd(alop, gen_helper_pfcmpneqs); break;
    case OP_PFCMPNLTS: gen_alopf1_env_ddd(alop, gen_helper_pfcmpnlts); break;
    case OP_PFCMPNLES: gen_alopf1_env_ddd(alop, gen_helper_pfcmpnles); break;
    case OP_PFCMPODS: gen_alopf1_env_ddd(alop, gen_helper_pfcmpods); break;
    case OP_PFCMPEQD: gen_alopf1_env_ddd(alop, gen_helper_fcmpeqd); break;
    case OP_PFCMPLTD: gen_alopf1_env_ddd(alop, gen_helper_fcmpltd); break;
    case OP_PFCMPLED: gen_alopf1_env_ddd(alop, gen_helper_fcmpled); break;
    case OP_PFCMPUODD: gen_alopf1_env_ddd(alop, gen_helper_fcmpuodd); break;
    case OP_PFCMPNEQD: gen_alopf1_env_ddd(alop, gen_helper_fcmpneqd); break;
    case OP_PFCMPNLTD: gen_alopf1_env_ddd(alop, gen_helper_fcmpnltd); break;
    case OP_PFCMPNLED: gen_alopf1_env_ddd(alop, gen_helper_fcmpnled); break;
    case OP_PFCMPODD: gen_alopf1_env_ddd(alop, gen_helper_fcmpodd); break;
    case OP_FSCALED: gen_alopf1_env_dsd(alop, gen_helper_fscaled); break;
    case OP_FSCALES: gen_alopf1_env_sss(alop, gen_helper_fscales); break;
    case OP_FXSCALESX: gen_alopf1_env_xsx(alop, gen_helper_fxscalesx); break;
    case OP_FRCPS: gen_alopf2_env_ss(alop, gen_helper_frcps); break;
    case OP_FSQRTS: gen_alopf2_env_ss(alop, gen_helper_fsqrts); break;
    case OP_FRSQRTS: gen_alopf2_env_ss(alop, gen_helper_frsqrts); break;
#ifndef TARGET_E2K_PRECISE_FSQRTID
    case OP_FSQRTID: gen_alopf2_dd(alop, tcg_gen_mov_i64); break;
    case OP_FXSQRTISX: gen_alopf2_env_sx(alop, gen_helper_fstofx); break;
    case OP_FXSQRTIDX: gen_alopf2_env_dx(alop, gen_helper_fdtofx); break;
    case OP_FXSQRTIXX: gen_alopf2_xx(alop, gen_movx); break;
    /* FIXME: these are not ALOPF2! */
    case OP_FXSQRTUSX: /* fallthrough */
    case OP_FXSQRTUDX: /* fallthrough */
    case OP_FXSQRTUXX: gen_alopf2_xx(alop, gen_movx); break;
#else
#error Not implemented
#endif
    case OP_FXSQRTTSX: gen_alopf1_env_sxx(alop, gen_helper_fxsqrttsx); break;
    case OP_FXSQRTTDX: gen_alopf1_env_dxx(alop, gen_helper_fxsqrttdx); break;
    case OP_FXSQRTTXX: gen_alopf1_env_xxx(alop, gen_helper_fxsqrttxx); break;
    case OP_PFSQRTTD: /* fallthrough */
    case OP_FSQRTTD: gen_alopf1_env_ddd(alop, gen_helper_fsqrttd); break;
    case OP_INSFS: gen_alopf21_ssss(alop, gen_insfs); break;
    case OP_INSFD: gen_insfd(alop); break;
    case OP_PSHUFB: gen_alopf21_dddd(alop, gen_helper_pshufb); break;
    case OP_PMERGE: gen_alopf21_dddd(alop, gen_helper_pmerge); break;
    case OP_FXDIVTSS: gen_alopf1_env_sxs(alop, gen_helper_fxdivtss); break;
    case OP_FXDIVTDD: gen_alopf1_env_dxd(alop, gen_helper_fxdivtdd); break;
    case OP_FXDIVTSX: gen_alopf1_env_sxx(alop, gen_helper_fxdivtsx); break;
    case OP_FXDIVTDX: gen_alopf1_env_dxx(alop, gen_helper_fxdivtdx); break;
    case OP_QPPACKDL: gen_alopf1_ddq(alop, gen_qppackdl); break;
    case OP_QPSWITCHW: gen_alopf2_qq(alop, gen_qpswitchw); break;
    case OP_QPSWITCHD: gen_alopf2_qq(alop, gen_qpswitchd); break;
    case OP_QPAND: gen_alopf1_qqq(alop, gen_qpand); break;
    case OP_QPANDN: gen_alopf1_qqq(alop, gen_qpandn); break;
    case OP_QPOR: gen_alopf1_qqq(alop, gen_qpor); break;
    case OP_QPXOR: gen_alopf1_qqq(alop, gen_qpxor); break;
    case OP_QPMAXSB: gen_alopf1_qqq(alop, gen_qpmaxsb); break;
    case OP_QPMAXSH: gen_alopf1_qqq(alop, gen_qpmaxsh); break;
    case OP_QPMAXSW: gen_alopf1_qqq(alop, gen_qpmaxsw); break;
    case OP_QPMAXUB: gen_alopf1_qqq(alop, gen_qpmaxub); break;
    case OP_QPMAXUH: gen_alopf1_qqq(alop, gen_qpmaxuh); break;
    case OP_QPMAXUW: gen_alopf1_qqq(alop, gen_qpmaxuw); break;
    case OP_QPMINSB: gen_alopf1_qqq(alop, gen_qpminsb); break;
    case OP_QPMINSH: gen_alopf1_qqq(alop, gen_qpminsh); break;
    case OP_QPMINSW: gen_alopf1_qqq(alop, gen_qpminsw); break;
    case OP_QPMINUB: gen_alopf1_qqq(alop, gen_qpminub); break;
    case OP_QPMINUH: gen_alopf1_qqq(alop, gen_qpminuh); break;
    case OP_QPMINUW: gen_alopf1_qqq(alop, gen_qpminuw); break;
    case OP_QPCMPEQB: gen_alopf1_qqq(alop, gen_qpcmpeqb); break;
    case OP_QPCMPEQD: gen_alopf1_qqq(alop, gen_qpcmpeqd); break;
    case OP_QPCMPEQH: gen_alopf1_qqq(alop, gen_qpcmpeqh); break;
    case OP_QPCMPEQW: gen_alopf1_qqq(alop, gen_qpcmpeqw); break;
    case OP_QPCMPGTB: gen_alopf1_qqq(alop, gen_qpcmpgtb); break;
    case OP_QPCMPGTD: gen_alopf1_qqq(alop, gen_qpcmpgtd); break;
    case OP_QPCMPGTH: gen_alopf1_qqq(alop, gen_qpcmpgth); break;
    case OP_QPCMPGTW: gen_alopf1_qqq(alop, gen_qpcmpgtw); break;
    case OP_QPADDSB: gen_alopf1_qqq(alop, gen_qpaddsb); break;
    case OP_QPADDSH: gen_alopf1_qqq(alop, gen_qpaddsh); break;
    case OP_QPADDUSB: gen_alopf1_qqq(alop, gen_qpaddusb); break;
    case OP_QPADDUSH: gen_alopf1_qqq(alop, gen_qpaddush); break;
    case OP_QPSUBSB: gen_alopf1_qqq(alop, gen_qpsubsb); break;
    case OP_QPSUBSH: gen_alopf1_qqq(alop, gen_qpsubsh); break;
    case OP_QPSUBUSB: gen_alopf1_qqq(alop, gen_qpsubusb); break;
    case OP_QPSUBUSH: gen_alopf1_qqq(alop, gen_qpsubush); break;
    case OP_QPADDB: gen_alopf1_qqq(alop, gen_qpaddb); break;
    case OP_QPADDH: gen_alopf1_qqq(alop, gen_qpaddh); break;
    case OP_QPADDW: gen_alopf1_qqq(alop, gen_qpaddw); break;
    case OP_QPADDD: gen_alopf1_qqq(alop, gen_qpaddd); break;
    case OP_QPSUBB: gen_alopf1_qqq(alop, gen_qpsubb); break;
    case OP_QPSUBH: gen_alopf1_qqq(alop, gen_qpsubh); break;
    case OP_QPSUBW: gen_alopf1_qqq(alop, gen_qpsubw); break;
    case OP_QPSUBD: gen_alopf1_qqq(alop, gen_qpsubd); break;
    case OP_QPSLLH: gen_alopf1_qdq(alop, gen_qpsllh); break;
    case OP_QPSLLW: gen_alopf1_qdq(alop, gen_qpsllw); break;
    case OP_QPSLLD: gen_alopf1_qdq(alop, gen_qpslld); break;
    case OP_QPSRLH: gen_alopf1_qdq(alop, gen_qpsrlh); break;
    case OP_QPSRLW: gen_alopf1_qdq(alop, gen_qpsrlw); break;
    case OP_QPSRLD: gen_alopf1_qdq(alop, gen_qpsrld); break;
    case OP_QPSRAH: gen_alopf1_qdq(alop, gen_qpsrah); break;
    case OP_QPSRAW: gen_alopf1_qdq(alop, gen_qpsraw); break;
    case OP_QPACKSSHB: gen_alopf1_qqq(alop, gen_qpacksshb); break;
    case OP_QPACKSSWH: gen_alopf1_qqq(alop, gen_qpacksswh); break;
    case OP_QPACKUSHB: gen_alopf1_qqq(alop, gen_qpackushb); break;
    case OP_QPACKUSWH: gen_alopf1_qqq(alop, gen_qpackuswh); break;
    case OP_QPAVGUSB: gen_alopf1_qqq(alop, gen_qpavgusb); break;
    case OP_QPAVGUSH: gen_alopf1_qqq(alop, gen_qpavgush); break;
    case OP_QPHADDH: gen_alopf1_qqq(alop, gen_helper_qphaddh); break;
    case OP_QPHADDSH: gen_alopf1_qqq(alop, gen_helper_qphaddsh); break;
    case OP_QPHADDW: gen_alopf1_qqq(alop, gen_helper_qphaddw); break;
    case OP_QPHSUBH: gen_alopf1_qqq(alop, gen_helper_qphsubh); break;
    case OP_QPHSUBSH: gen_alopf1_qqq(alop, gen_helper_qphsubsh); break;
    case OP_QPHSUBW: gen_alopf1_qqq(alop, gen_helper_qphsubw); break;
    case OP_QPMULHH: gen_alopf1_qqq(alop, gen_qpmulhh); break;
    case OP_QPMULHRSH: gen_alopf1_qqq(alop, gen_qpmulhrsh); break;
    case OP_QPMULHUH: gen_alopf1_qqq(alop, gen_qpmulhuh); break;
    case OP_QPMULLH: gen_alopf1_qqq(alop, gen_qpmullh); break;
    case OP_QPMULUBHH: gen_alopf1_dqq(alop, gen_helper_qpmulubhh); break;
    case OP_QPSIGNB: gen_alopf1_qqq(alop, gen_qpsignb); break;
    case OP_QPSIGNH: gen_alopf1_qqq(alop, gen_qpsignh); break;
    case OP_QPSIGNW: gen_alopf1_qqq(alop, gen_qpsignw); break;
    case OP_QPHMINPOSUH: gen_alopf1_qqd(alop, gen_helper_qphminposuh); break;
    case OP_QPMADDH: gen_alopf1_qqq(alop, gen_qpmaddh); break;
    case OP_QPMADDUBSH: gen_alopf1_qqq(alop, gen_qpmaddubsh); break;
    case OP_QPMPSADBH: gen_alopf1_qsq(alop, gen_helper_qpmpsadbh); break;
    case OP_QPSADBW: gen_alopf1_qqq(alop, gen_qpsadbw); break;
    case OP_QPFADDS: gen_alopf1_env_qqq(alop, gen_qpfadds); break;
    case OP_QPFADDD: gen_alopf1_env_qqq(alop, gen_qpfaddd); break;
    case OP_QPFSUBS: gen_alopf1_env_qqq(alop, gen_qpfsubs); break;
    case OP_QPFSUBD: gen_alopf1_env_qqq(alop, gen_qpfsubd); break;
    case OP_QPFMULS: gen_alopf1_env_qqq(alop, gen_qpfmuls); break;
    case OP_QPFMULD: gen_alopf1_env_qqq(alop, gen_qpfmuld); break;
    case OP_QPFHADDS: gen_alopf1_env_qqq(alop, gen_helper_qpfhadds); break;
    case OP_QPFHSUBS: gen_alopf1_env_qqq(alop, gen_helper_qpfhsubs); break;
    case OP_QPFADDSUBS: gen_alopf1_env_qqq(alop, gen_qpfaddsubs); break;
    case OP_QPFADDSUBD: gen_alopf1_env_qqq(alop, gen_qpfaddsubd); break;
    case OP_QPFMINS: gen_alopf1_env_qqq(alop, gen_qpfmins); break;
    case OP_QPFMIND: gen_alopf1_env_qqq(alop, gen_qpfmind); break;
    case OP_QPFMAXS: gen_alopf1_env_qqq(alop, gen_qpfmaxs); break;
    case OP_QPFMAXD: gen_alopf1_env_qqq(alop, gen_qpfmaxd); break;
    case OP_QPFCMPEQS: gen_alopf1_env_qqq(alop, gen_qpfcmpeqs); break;
    case OP_QPFCMPLTS: gen_alopf1_env_qqq(alop, gen_qpfcmplts); break;
    case OP_QPFCMPLES: gen_alopf1_env_qqq(alop, gen_qpfcmples); break;
    case OP_QPFCMPUODS: gen_alopf1_env_qqq(alop, gen_qpfcmpuods); break;
    case OP_QPFCMPNEQS: gen_alopf1_env_qqq(alop, gen_qpfcmpneqs); break;
    case OP_QPFCMPNLTS: gen_alopf1_env_qqq(alop, gen_qpfcmpnlts); break;
    case OP_QPFCMPNLES: gen_alopf1_env_qqq(alop, gen_qpfcmpnles); break;
    case OP_QPFCMPODS: gen_alopf1_env_qqq(alop, gen_qpfcmpods); break;
    case OP_QPFCMPEQD: gen_alopf1_env_qqq(alop, gen_qpfcmpeqd); break;
    case OP_QPFCMPLTD: gen_alopf1_env_qqq(alop, gen_qpfcmpltd); break;
    case OP_QPFCMPLED: gen_alopf1_env_qqq(alop, gen_qpfcmpled); break;
    case OP_QPFCMPUODD: gen_alopf1_env_qqq(alop, gen_qpfcmpuodd); break;
    case OP_QPFCMPNEQD: gen_alopf1_env_qqq(alop, gen_qpfcmpneqd); break;
    case OP_QPFCMPNLTD: gen_alopf1_env_qqq(alop, gen_qpfcmpnltd); break;
    case OP_QPFCMPNLED: gen_alopf1_env_qqq(alop, gen_qpfcmpnled); break;
    case OP_QPFCMPODD: gen_alopf1_env_qqq(alop, gen_qpfcmpodd); break;
    case OP_QPFSTOIFS: gen_alopf1_env_dqq(alop, gen_qpfstoifs); break;
    case OP_QPFDTOIFD: gen_alopf1_env_dqq(alop, gen_qpfdtoifd); break;
    case OP_QPSRCD: gen_alopf1_qdq(alop, gen_qpsrcd); break;
    case OP_QPSRCW: gen_alopf1_qdq(alop, gen_qpsrcw); break;
    case OP_PSRCD: gen_alopf1_ddd(alop, gen_psrcd); break;
    case OP_PSRCW: gen_alopf1_ddd(alop, gen_psrcw); break;
    case OP_PMULLW: gen_alopf1_ddd(alop, gen_pmullw); break;
    case OP_QPMULLW: gen_alopf1_qqq(alop, gen_qpmullw); break;
    case OP_QPFSTOIS: gen_alopf2_env_qq(alop, gen_qpfstois); break;
    case OP_QPFSTOISTR: gen_alopf2_env_qq(alop, gen_qpfstoistr); break;
    case OP_QPISTOFS: gen_alopf2_env_qq(alop, gen_qpistofs); break;
    case OP_QPFDTOID: gen_alopf2_env_qq(alop, gen_qpfdtoid); break;
    case OP_QPFDTOIDTR: gen_alopf2_env_qq(alop, gen_qpfdtoidtr); break;
    case OP_QPIDTOFD: gen_alopf2_env_qq(alop, gen_qpidtofd); break;
    case OP_QPFSTOID: gen_alopf2_env_dq(alop, gen_helper_qpfstoid); break;
    case OP_QPFSTOIDTR: gen_alopf2_env_dq(alop, gen_helper_qpfstoidtr); break;
    case OP_QPISTOFD: gen_alopf2_env_dq(alop, gen_helper_qpistofd); break;
    case OP_QPFSTOFD: gen_alopf2_env_dq(alop, gen_helper_qpfstofd); break;
    case OP_QPFDTOIS: gen_alopf2_env_qd(alop, gen_helper_qpfdtois); break;
    case OP_QPFDTOISTR: gen_alopf2_env_qd(alop, gen_helper_qpfdtoistr); break;
    case OP_QPIDTOFS: gen_alopf2_env_qd(alop, gen_helper_qpidtofs); break;
    case OP_QPFDTOFS: gen_alopf2_env_qd(alop, gen_helper_qpfdtofs); break;
    case OP_QPMSK2SGNB: gen_alopf1_qsq(alop, gen_helper_qpmsk2sgnb); break;
    case OP_QPSGN2MSKB: gen_alopf2_qs(alop, gen_helper_qpsgn2mskb); break;
    case OP_GETFZS: gen_alopf1_sss(alop, gen_getfzs); break;
    case OP_GETFZD: gen_alopf1_ddd(alop, gen_getfzd); break;
    case OP_ADDCD: gen_alopf21_ddsd(alop, gen_addcd); break;
    case OP_ADDCD_C: gen_alopf21_ddsd(alop, gen_addcd_c); break;
    case OP_SUBCD: gen_alopf21_ddsd(alop, gen_subcd); break;
    case OP_SUBCD_C: gen_alopf21_ddsd(alop, gen_subcd_c); break;
    case OP_QPMERGE: gen_alopf21_qqqq(alop, gen_qpmerge); break;
    case OP_QPSHUFB: gen_alopf21_qqqq(alop, gen_helper_qpshufb); break;
    case OP_QPPERMB: gen_alopf21_qqqq(alop, gen_helper_qppermb); break;
    case OP_PLOG_0x00: gen_alopf21_log_dddd(alop, gen_plog_0x00); break;
    case OP_PLOG_0x80: gen_alopf21_log_dddd(alop, gen_plog_0x80); break;
    case OP_QPLOG_0x00: gen_alopf21_log_qqqq(alop, gen_qplog_0x00); break;
    case OP_QPLOG_0x80: gen_alopf21_log_qqqq(alop, gen_qplog_0x80); break;
    case OP_FMAS: gen_alopf21_env_ssss(alop, gen_helper_fmas); break;
    case OP_FMSS: gen_alopf21_env_ssss(alop, gen_helper_fmss); break;
    case OP_FNMAS: gen_alopf21_env_ssss(alop, gen_helper_fnmas); break;
    case OP_FNMSS: gen_alopf21_env_ssss(alop, gen_helper_fnmss); break;
    case OP_FMAD: gen_alopf21_env_dddd(alop, gen_helper_fmad); break;
    case OP_FMSD: gen_alopf21_env_dddd(alop, gen_helper_fmsd); break;
    case OP_FNMAD: gen_alopf21_env_dddd(alop, gen_helper_fnmad); break;
    case OP_FNMSD: gen_alopf21_env_dddd(alop, gen_helper_fnmsd); break;
    case OP_QPFMAS: gen_alopf21_env_qqqq(alop, gen_helper_qpfmas); break;
    case OP_QPFMSS: gen_alopf21_env_qqqq(alop, gen_helper_qpfmss); break;
    case OP_QPFNMAS: gen_alopf21_env_qqqq(alop, gen_helper_qpfnmas); break;
    case OP_QPFNMSS: gen_alopf21_env_qqqq(alop, gen_helper_qpfnmss); break;
    case OP_QPFMASS: gen_alopf21_env_qqqq(alop, gen_helper_qpfmass); break;
    case OP_QPFMSAS: gen_alopf21_env_qqqq(alop, gen_helper_qpfmsas); break;
    case OP_QPFMAD: gen_alopf21_env_qqqq(alop, gen_helper_qpfmad); break;
    case OP_QPFMSD: gen_alopf21_env_qqqq(alop, gen_helper_qpfmsd); break;
    case OP_QPFNMAD: gen_alopf21_env_qqqq(alop, gen_helper_qpfnmad); break;
    case OP_QPFNMSD: gen_alopf21_env_qqqq(alop, gen_helper_qpfnmsd); break;
    case OP_QPFMASD: gen_alopf21_env_qqqq(alop, gen_helper_qpfmasd); break;
    case OP_QPFMSAD: gen_alopf21_env_qqqq(alop, gen_helper_qpfmsad); break;
    case OP_PCMPEQBOP: gen_alopf7_ddd(alop, gen_pcmpeqbop); break;
    case OP_PCMPEQHOP: gen_alopf7_ddd(alop, gen_pcmpeqhop); break;
    case OP_PCMPEQWOP: gen_alopf7_ddd(alop, gen_pcmpeqwop); break;
    case OP_PCMPEQDOP: gen_alopf7_ddd(alop, gen_pcmpeqdop); break;
    case OP_PCMPGTBOP: gen_alopf7_ddd(alop, gen_pcmpgtbop); break;
    case OP_PCMPGTHOP: gen_alopf7_ddd(alop, gen_pcmpgthop); break;
    case OP_PCMPGTWOP: gen_alopf7_ddd(alop, gen_pcmpgtwop); break;
    case OP_PCMPGTDOP: gen_alopf7_ddd(alop, gen_pcmpgtdop); break;
    case OP_PCMPEQBAP: gen_alopf7_ddd(alop, gen_pcmpeqbap); break;
    case OP_PCMPEQHAP: gen_alopf7_ddd(alop, gen_pcmpeqhap); break;
    case OP_PCMPEQWAP: gen_alopf7_ddd(alop, gen_pcmpeqwap); break;
    case OP_PCMPEQDAP: gen_alopf7_ddd(alop, gen_pcmpeqdap); break;
    case OP_PCMPGTBAP: gen_alopf7_ddd(alop, gen_pcmpgtbap); break;
    case OP_PCMPGTHAP: gen_alopf7_ddd(alop, gen_pcmpgthap); break;
    case OP_PCMPGTWAP: gen_alopf7_ddd(alop, gen_pcmpgtwap); break;
    case OP_PCMPGTDAP: gen_alopf7_ddd(alop, gen_pcmpgtdap); break;
    case OP_QPCMPEQBOP: gen_alopf7_qqd(alop, gen_qpcmpeqbop); break;
    case OP_QPCMPEQHOP: gen_alopf7_qqd(alop, gen_qpcmpeqhop); break;
    case OP_QPCMPEQWOP: gen_alopf7_qqd(alop, gen_qpcmpeqwop); break;
    case OP_QPCMPEQDOP: gen_alopf7_qqd(alop, gen_qpcmpeqdop); break;
    case OP_QPCMPGTBOP: gen_alopf7_qqd(alop, gen_qpcmpgtbop); break;
    case OP_QPCMPGTHOP: gen_alopf7_qqd(alop, gen_qpcmpgthop); break;
    case OP_QPCMPGTWOP: gen_alopf7_qqd(alop, gen_qpcmpgtwop); break;
    case OP_QPCMPGTDOP: gen_alopf7_qqd(alop, gen_qpcmpgtdop); break;
    case OP_QPCMPEQBAP: gen_alopf7_qqd(alop, gen_qpcmpeqbap); break;
    case OP_QPCMPEQHAP: gen_alopf7_qqd(alop, gen_qpcmpeqhap); break;
    case OP_QPCMPEQWAP: gen_alopf7_qqd(alop, gen_qpcmpeqwap); break;
    case OP_QPCMPEQDAP: gen_alopf7_qqd(alop, gen_qpcmpeqdap); break;
    case OP_QPCMPGTBAP: gen_alopf7_qqd(alop, gen_qpcmpgtbap); break;
    case OP_QPCMPGTHAP: gen_alopf7_qqd(alop, gen_qpcmpgthap); break;
    case OP_QPCMPGTWAP: gen_alopf7_qqd(alop, gen_qpcmpgtwap); break;
    case OP_QPCMPGTDAP: gen_alopf7_qqd(alop, gen_qpcmpgtdap); break;
    case OP_QPSRAD: gen_alopf1_qdq(alop, gen_qpsrad); break;
    case OP_PMRGP: gen_merged(alop); break;
    case OP_QPMRGP: gen_qpmrgp(alop); break;
    case OP_CLMULH: gen_alopf1_ddd(alop, gen_helper_clmulh); break;
    case OP_CLMULL: gen_alopf1_ddd(alop, gen_helper_clmull); break;
    case OP_QPCEXT_0X00: gen_alopf2_dq(alop, gen_qpcext_0x00); break;
    case OP_QPCEXT_0X7F: gen_alopf2_dq(alop, gen_qpcext_0x7f); break;
    case OP_QPCEXT_0X80: gen_alopf2_dq(alop, gen_qpcext_0x80); break;
    case OP_QPCEXT_0XFF: gen_alopf2_dq(alop, gen_qpcext_0xff); break;
    case OP_VFSI:
    case OP_MOVTRS:
    case OP_MOVTRCS:
    case OP_MOVTRD:
    case OP_MOVTRCD:
    case OP_GETSAP:
    case OP_CUDTOAP:
    case OP_GDTOAP:
    case OP_CCTOPO:
    case OP_CCTOPB:
    case OP_CCTOPE:
    case OP_CCTOPBE:
    case OP_CCTOPS:
    case OP_CCTOPP:
    case OP_CCTOPL:
    case OP_CCTOPLE:
    /*
    case OP_AAURW:
    case OP_AAURWS:
    case OP_AAURWD:
    case OP_AAURWQ:
    case OP_AAURR:
    case OP_AAURRD:
    case OP_AAURRQ:
    */
    case OP_APTOAP:
    case OP_APTOAPB:
    case OP_GETVA:
    case OP_LDRD:
    case OP_PUTTC:
    case OP_CAST:
    case OP_TDTOMP:
    case OP_ODTOAP:
    case OP_LDCUDB:
    case OP_LDCUDH:
    case OP_LDCUDW:
    case OP_LDCUDD:
    case OP_LDCUDQ:
    case OP_LDAPB:
    case OP_LDAPH:
    case OP_LDAPW:
    case OP_LDAPD:
    case OP_LDAPQ:
    case OP_LDODWB:
    case OP_LDODWD:
    case OP_LDODWH:
    case OP_LDODWQ:
    case OP_LDODWW:
    case OP_LDODPB:
    case OP_LDODPD:
    case OP_LDODPH:
    case OP_LDODPQ:
    case OP_LDODPW:
    case OP_LDODRB:
    case OP_LDODRD:
    case OP_LDODRH:
    case OP_LDODRQ:
    case OP_LDODRW:
    case OP_LDCSQ:
    case OP_LDDSQ:
    case OP_LDESQ:
    case OP_LDFSQ:
    case OP_LDGDQ:
    case OP_LDGSQ:
    case OP_LDSSQ:
    case OP_GETTD:
    case OP_GETTC:
    case OP_INVTC:
    case OP_GETSOD:
    case OP_STCSQ:
    case OP_STDSQ:
    case OP_STESQ:
    case OP_STFSQ:
    case OP_STGDQ:
    case OP_STGSQ:
    case OP_STSSQ:
    case OP_STRD:
    case OP_STAPB:
    case OP_STAPH:
    case OP_STAPW:
    case OP_STAPD:
    case OP_STAPQ:
    case OP_STODPB:
    case OP_STODPD:
    case OP_STODPH:
    case OP_STODPQ:
    case OP_STODPW:
    case OP_STODRB:
    case OP_STODRD:
    case OP_STODRH:
    case OP_STODRQ:
    case OP_STODRW:
    case OP_STODWB:
    case OP_STODWD:
    case OP_STODWH:
    case OP_STODWQ:
    case OP_STODWW:
    case OP_MOVTRQ:
    case OP_MOVTRCQ:
    case OP_PUTTST:
    case OP_LDQ:
    case OP_LDCUDQP:
    case OP_LDAPQP:
    case OP_LDRQP:
    case OP_STQ:
    case OP_STAPQP:
    case OP_STAPMQP:
    case OP_STRQP:
    case OP_VFBGV:
    case OP_MKFSW:
    case OP_MODBGV:
    case OP_IBRANCHD:
    case OP_ICALLD:
        e2k_todo_illop(ctx, "unimplemented %d (%s)", alop->op, alop->name); break;
    }
}

typedef enum {
    ICOMB_AND = 0,
    ICOMB_ANDN = 1,
    ICOMB_OR = 2,
    ICOMB_ORN = 3,
    ICOMB_XOR = 4,
    ICOMB_XORN = 5,
    ICOMB_RSUB = 6,
    ICOMB_MERGE = 7,
    ICOMB_ADD = 8,
    ICOMB_SUB = 9,
    ICOMB_SCL = 10,
    ICOMB_SCR = 11,
    ICOMB_SHL = 12,
    ICOMB_SHR = 13,
    ICOMB_SAR = 14,
    ICOMB_GETF = 15,
} IComb;

#define IMPL_GEN_ICOMB_OP(S) \
    static void glue(gen_icomb_op_, S)(Alop *alop, IComb opc, \
        glue(TCGv_, S) ret, glue(TCGv_, S) arg1, glue(TCGv_, S) arg2) \
    { \
        switch(opc) { \
        case ICOMB_AND: glue(tcg_gen_and_, S)(ret, arg1, arg2); break; \
        case ICOMB_ANDN: glue(gen_andn_, S)(ret, arg1, arg2); break; \
        case ICOMB_OR: glue(tcg_gen_or_, S)(ret, arg1, arg2); break; \
        case ICOMB_ORN: glue(gen_orn_, S)(ret, arg1, arg2); break; \
        case ICOMB_XOR: glue(tcg_gen_xor_, S)(ret, arg1, arg2); break; \
        case ICOMB_XORN: glue(gen_xorn_, S)(ret, arg1, arg2); break; \
        case ICOMB_RSUB: glue(tcg_gen_sub_, S)(ret, arg2, arg1); break; \
        case ICOMB_MERGE: { \
            TCGv_i32 t0 = tcg_temp_new_i32(); \
            gen_mrgc_i32(alop->ctx, alop->chan, t0); \
            glue(gen_merge_, S)(ret, arg1, arg2, t0); \
            tcg_temp_free_i32(t0); \
            break; \
        } \
        case ICOMB_ADD: glue(tcg_gen_add_, S)(ret, arg1, arg2); break; \
        case ICOMB_SUB: glue(tcg_gen_sub_, S)(ret, arg1, arg2); break; \
        case ICOMB_SCL: glue(tcg_gen_rotl_, S)(ret, arg1, arg2); break; \
        case ICOMB_SCR: glue(tcg_gen_rotr_, S)(ret, arg1, arg2); break; \
        case ICOMB_SHL: glue(tcg_gen_shl_, S)(ret, arg1, arg2); break; \
        case ICOMB_SHR: glue(tcg_gen_shr_, S)(ret, arg1, arg2); break; \
        case ICOMB_SAR: glue(tcg_gen_sar_, S)(ret, arg1, arg2); break; \
        case ICOMB_GETF: glue(gen_getf_, S)(ret, arg1, arg2); break; \
        default: g_assert_not_reached(); break; \
        } \
    }

IMPL_GEN_ICOMB_OP(i64)
IMPL_GEN_ICOMB_OP(i32)

static inline bool icomb_check(DisasContext *ctx, Alop *alop,
    IComb opc1, IComb opc2)
{
    if (!is_chan_14(alop->chan)) {
        return false;
    }

    if (ctx->version == 1) {
        return opc1 != ICOMB_RSUB;
    } else {
        return opc1 != ICOMB_RSUB
            && opc2 < ICOMB_SCL
            && opc2 != ICOMB_MERGE;
    }
}

typedef enum {
    FCOMB_ADD = 0,
    FCOMB_SUB = 1,
    FCOMB_HADD = 2,
    FCOMB_HSUB = 3,
    FCOMB_MUL = 4,
    FCOMB_RSUB = 5,
    FCOMB_ADDSUB = 7,
    FCOMB_COUNT = 8,
} FComb;

static inline bool fcomb_is_add_unit(FComb op)
{
    switch (op) {
    case FCOMB_ADD:
    case FCOMB_SUB:
    case FCOMB_RSUB:
        return true;
    default:
        return false;
    }
}

static inline bool fcomb_is_mul_unit(FComb op)
{
    return op == FCOMB_MUL;
}

static inline bool fcomb_check(DisasContext *ctx, Alop *alop,
    FComb opc1, FComb opc2)
{
    int ver = ctx->version;

    if (opc1 == FCOMB_RSUB || (ver < 4 && is_chan_25(alop->chan))) {
        return false;
    }

    if (ver >= 2) {
        return (fcomb_is_add_unit(opc1) || fcomb_is_mul_unit(opc1))
            && fcomb_is_add_unit(opc2);
    } else {
        return fcomb_is_add_unit(opc1) == fcomb_is_mul_unit(opc2);
    }
}

static bool pfcomb_map[FCOMB_COUNT][FCOMB_COUNT] = { false };

static void pfcomb_map_set(FComb op, FComb *list, int n)
{
    int i;

    for (i = 0; i < n; i++) {
        pfcomb_map[op][list[i]] = true;
    }
}

static void pfcomb_map_update(FComb *first, int nf, FComb *second, int ns)
{
    int i;

    for (i = 0; i < nf; i++) {
        pfcomb_map_set(first[i], second, ns);
    }
}

static void pfcomb_init(DisasContext *ctx)
{
    FComb l0[] = { FCOMB_ADD, FCOMB_SUB, FCOMB_RSUB };

    pfcomb_map_set(FCOMB_MUL, l0, ARRAY_SIZE(l0));

    if (ctx->version == 1) {
        pfcomb_map[FCOMB_ADD][FCOMB_MUL] = true;
        pfcomb_map[FCOMB_SUB][FCOMB_MUL] = true;
    }

    if (ctx->version >= 2) {
        FComb l1[] = { FCOMB_ADD, FCOMB_SUB };

        pfcomb_map_update(l1, ARRAY_SIZE(l1), l0, ARRAY_SIZE(l0));
    }

    if (ctx->version >= 3) {
        FComb l1[] = { FCOMB_ADD, FCOMB_SUB, FCOMB_MUL, };
        FComb l2[] = { FCOMB_HADD, FCOMB_HSUB, FCOMB_ADDSUB };

        pfcomb_map_update(l1, ARRAY_SIZE(l1), l2, ARRAY_SIZE(l2));
        pfcomb_map_update(l2, ARRAY_SIZE(l2), l2, ARRAY_SIZE(l2));
        pfcomb_map_update(l2, ARRAY_SIZE(l2), l0, ARRAY_SIZE(l0));
    }
}

static inline bool pfcomb_check(DisasContext *ctx, Alop *alop,
    FComb opc1, FComb opc2)
{
    if (ctx->version < 4 && is_chan_25(alop->chan)) {
        return false;
    }

    return pfcomb_map[opc1][opc2];
}

#define IMPL_GEN_FCOMB_OP(S, T) \
    static void glue(gen_fcomb_op_, S)(Alop *alop, FComb opc, \
        glue(TCGv_, S) ret, glue(TCGv_, S) arg1, glue(TCGv_, S) arg2) \
    { \
        switch(opc) { \
        case FCOMB_ADD: glue(gen_helper_fadd, T)(ret, cpu_env, arg1, arg2); break; \
        case FCOMB_SUB: glue(gen_helper_fsub, T)(ret, cpu_env, arg1, arg2); break; \
        case FCOMB_MUL: glue(gen_helper_fmul, T)(ret, cpu_env, arg1, arg2); break; \
        case FCOMB_RSUB: glue(gen_helper_fsub, T)(ret, cpu_env, arg2, arg1); break; \
        default: gen_tr_excp_illopc(alop->ctx); break; \
        } \
    }

IMPL_GEN_FCOMB_OP(i64, d)
IMPL_GEN_FCOMB_OP(i32, s)

static void gen_pfcomb_op_i32(Alop *alop, FComb opc,
    TCGv_i64 ret, TCGv_i64 arg1, TCGv_i64 arg2)
{
    switch (opc) {
    case FCOMB_ADD: gen_helper_pfadds(ret, cpu_env, arg1, arg2); break;
    case FCOMB_SUB: gen_helper_pfsubs(ret, cpu_env, arg1, arg2); break;
    case FCOMB_HADD: gen_helper_pfhadds(ret, cpu_env, arg1, arg2); break;
    case FCOMB_HSUB: gen_helper_pfhsubs(ret, cpu_env, arg1, arg2); break;
    case FCOMB_MUL: gen_helper_pfmuls(ret, cpu_env, arg1, arg2); break;
    case FCOMB_RSUB: gen_helper_pfsubs(ret, cpu_env, arg2, arg1); break;
    case FCOMB_ADDSUB: gen_helper_pfaddsubs(ret, cpu_env, arg1, arg2); break;
    default: gen_tr_excp_illopc(alop->ctx); break;
    }
}

static void gen_pfcomb_op_i64(Alop *alop, FComb opc,
    TCGv_i64 ret, TCGv_i64 arg1, TCGv_i64 arg2)
{
    switch (opc) {
    case FCOMB_ADD: gen_helper_faddd(ret, cpu_env, arg1, arg2); break;
    case FCOMB_SUB: gen_helper_fsubd(ret, cpu_env, arg1, arg2); break;
    case FCOMB_MUL: gen_helper_fmuld(ret, cpu_env, arg1, arg2); break;
    case FCOMB_RSUB: gen_helper_fsubd(ret, cpu_env, arg2, arg1); break;
    default: gen_tr_excp_illopc(alop->ctx); break;
    }
}

static void gen_qpfcomb_op_i32(Alop *alop, FComb opc,
    TCGv_ptr ret, TCGv_ptr arg1, TCGv_ptr arg2)
{
    switch (opc) {
    case FCOMB_ADD: gen_qpfadds(ret, cpu_env, arg1, arg2); break;
    case FCOMB_SUB: gen_qpfsubs(ret, cpu_env, arg1, arg2); break;
    case FCOMB_HADD: gen_helper_qpfhadds(ret, cpu_env, arg1, arg2); break;
    case FCOMB_HSUB: gen_helper_qpfhsubs(ret, cpu_env, arg1, arg2); break;
    case FCOMB_MUL: gen_qpfmuls(ret, cpu_env, arg1, arg2); break;
    case FCOMB_RSUB: gen_qpfsubs(ret, cpu_env, arg2, arg1); break;
    case FCOMB_ADDSUB: gen_qpfaddsubs(ret, cpu_env, arg1, arg2); break;
    default: gen_tr_excp_illopc(alop->ctx); break;
    }
}

static void gen_qpfcomb_op_i64(Alop *alop, FComb opc,
    TCGv_ptr ret, TCGv_ptr arg1, TCGv_ptr arg2)
{
    switch (opc) {
    case FCOMB_ADD: gen_qpfaddd(ret, cpu_env, arg1, arg2); break;
    case FCOMB_SUB: gen_qpfsubd(ret, cpu_env, arg1, arg2); break;
    case FCOMB_MUL: gen_qpfmuld(ret, cpu_env, arg1, arg2); break;
    case FCOMB_RSUB: gen_qpfsubd(ret, cpu_env, arg2, arg1); break;
    case FCOMB_ADDSUB: gen_qpfaddsubd(ret, cpu_env, arg1, arg2); break;
    default: gen_tr_excp_illopc(alop->ctx); break;
    }
}

static inline int comb_opc1(Alop *alop, int m1)
{
    return (alop->als.opc1 >> 1) & m1;
}

static inline int comb_opc2(Alop *alop, int m1, int m2)
{
    return ((alop->ales.opc2 & m2) << 2) | ((alop->als.opc1 >> 5) & m1);
}

#define icomb_opc1(instr) comb_opc1(instr, 0xf)
#define fcomb_opc1(instr) comb_opc1(instr, 0x7)

#define icomb_opc2(instr) comb_opc2(instr, 0x3, 0x3)
#define fcomb_opc2(instr) comb_opc2(instr, 0x3, 0x1)

#define IMPL_GEN_COMB(NAME, P, S, OP) \
    static void NAME(Alop *alop, int opc1, int opc2) \
    { \
        tagged(S) r0 = tagged_local_new(S); \
        tagged(S) r1 = tagged_local_new(S); \
        tagged(S) s1 = tagged_local_new(S); \
        tagged(S) s2 = tagged_local_new(S); \
        tagged(S) s3 = tagged_local_new(S); \
        \
        gen_tagged_src(1, S, alop, s1); \
        gen_tagged_src(2, S, alop, s2); \
        gen_tagged_src(3, S, alop, s3); \
        gen_tag2(S, r0, s1, s2); \
        gen_tag2(S, r1, s3, r0); \
        gen_result_init2(S, alop, r0); \
        gen_result_init(S, alop, r1); \
        OP(alop, opc1, r0.val, s1.val, s2.val); \
        OP(alop, opc2, r1.val, s3.val, r0.val); \
        gen_al_result(S, alop, r1); \
        \
        tagged_free(S, s3); \
        tagged_free(S, s2); \
        tagged_free(S, s1); \
        tagged_free(S, r1); \
        tagged_free(S, r0); \
    }

IMPL_GEN_COMB(gen_icomb_i64,  icomb,  d, gen_icomb_op_i64)
IMPL_GEN_COMB(gen_icomb_i32,  icomb,  s, gen_icomb_op_i32)
IMPL_GEN_COMB(gen_fcomb_i64,  fcomb,  d, gen_fcomb_op_i64)
IMPL_GEN_COMB(gen_fcomb_i32,  fcomb,  s, gen_fcomb_op_i32)
IMPL_GEN_COMB(gen_pfcomb_i64, pfcomb, d, gen_pfcomb_op_i64)
IMPL_GEN_COMB(gen_pfcomb_i32, pfcomb, d, gen_pfcomb_op_i32)
IMPL_GEN_COMB(gen_qpfcomb_i64, pfcomb, q, gen_qpfcomb_op_i64)
IMPL_GEN_COMB(gen_qpfcomb_i32, pfcomb, q, gen_qpfcomb_op_i32)

#define IMPL_GEN_COMB_SELECT(NAME) \
    static void glue(gen_, NAME)(Alop *alop) \
    { \
        int opc1 = alop->op & 0xffff; \
        int opc2 = alop->op >> 16; \
        \
        if (alop->als.opc1 & 1) { \
            glue3(gen_, NAME, _i64)(alop, opc1, opc2); \
        } else { \
            glue3(gen_, NAME, _i32)(alop, opc1, opc2); \
        } \
    }

IMPL_GEN_COMB_SELECT(icomb)
IMPL_GEN_COMB_SELECT(fcomb)
IMPL_GEN_COMB_SELECT(pfcomb)
IMPL_GEN_COMB_SELECT(qpfcomb)

static inline bool rlp_check_chan(uint16_t rlp, int chan)
{
    return extract16(rlp, 14, 1) == (chan > 2) &&
        extract16(rlp, 10 + chan % 3, 1);
}

static inline bool rlp_is_chan_pred(uint16_t rlp, int chan)
{
    return !extract16(rlp, 15, 1) && rlp_check_chan(rlp, chan);
}

static void gen_alop_pred(Alop *alop, TCGLabel *l)
{
    DisasContext *ctx = alop->ctx;
    bool has_pcnt = false;
    bool has_preg = false;
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i32 t1 = tcg_temp_new_i32();
    TCGv_i32 t2 = tcg_temp_new_i32();
    int i, j, chan = alop->chan;

    tcg_gen_movi_i32(t0, 0);
    tcg_gen_movi_i32(t1, 0);

    for (i = 0; i < 3; i++) {
        uint16_t *cds = (uint16_t *) &ctx->bundle.cds[i];

        if (!ctx->bundle.cds_present[i]) {
            continue;
        }

        for (j = 0; j < 2; j++) {
            uint16_t rlp = cds[j];
            uint8_t kind = extract8(rlp, 5, 2);
            uint8_t idx = extract8(rlp, 0, 5);
            bool invert = extract16(rlp, 7 + chan % 3, 1);

            if (!rlp_is_chan_pred(rlp, chan)) {
                continue;
            }

            switch(kind) {
            case 0x2: /* %pcntN */
                has_pcnt = true;
                tcg_gen_setcondi_i32(TCG_COND_LEU, t2, cpu_lsr_pcnt, idx);
                if (invert) {
                    tcg_gen_xori_i32(t2, t2, 1);
                }
                tcg_gen_or_i32(t0, t0, t2);
                break;
            case 0x3: /* %predN */
                has_preg = true;
                gen_preg_i32(ctx, t2, idx);
                if (invert) {
                    tcg_gen_xori_i32(t2, t2, 1);
                }
                tcg_gen_or_i32(t1, t1, t2);
                break;
            default:
                if (ctx->strict) {
                    gen_tr_excp_illopc(ctx);
                }
                break;
            }
        }
    }

    if (has_preg || has_pcnt) {
        TCGv_i32 cond = tcg_temp_new_i32();

        if (has_preg && has_pcnt) {
            tcg_gen_and_i32(cond, t0, t1);
        } else if (has_preg) {
            tcg_gen_mov_i32(cond, t1);
        } else {
            tcg_gen_mov_i32(cond, t0);
        }

        tcg_gen_brcondi_i32(TCG_COND_EQ, cond, 0, l);

        tcg_temp_free_i32(cond);
    }

    tcg_temp_free_i32(t2);
    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
}

static void decode_alop(Alop *alop, AlesFlag ales_present)
{
    DisasContext *ctx = alop->ctx;

    switch (alop->ales.opc2) {
    case ALOP_SHORT:
    case ALOP_EXT:
    case ALOP_EXT1:
    case ALOP_EXT2:
        alop_table_find(ctx, alop, ales_present);
        break;
    case ALOP_ICMB0:
    case ALOP_ICMB1:
    case ALOP_ICMB2:
    case ALOP_ICMB3:
        if (alop->ales.opc2 == ALOP_ICMB3
            && (alop->als.opc1 == 0x6c || alop->als.opc1 == 0x6d))
        {
            if (!is_chan_0134(alop->chan)) {
                gen_tr_excp_illopc(ctx);
                return;
            }
            alop->format = ALOPF21;
            alop->op = alop->als.opc1 & 1 ? OP_INSFD : OP_INSFS;
        } else {
            int opc1 = icomb_opc1(alop);
            int opc2 = icomb_opc2(alop);
            if (!icomb_check(ctx, alop, opc1, opc2)) {
                gen_tr_excp_illopc(ctx);
                return;
            }
            alop->format = ALOPF21_ICOMB;
            alop->op = (opc2 << 16) | opc1;
        }
        alop->args = alop->als.opc1 & 1 ? ARGS_DDDD : ARGS_SSSS;
        break;
    case ALOP_FLB:
    case ALOP_FLH:
    case ALOP_FLW:
    case ALOP_FLD:
        e2k_todo_illop(ctx, "flags ops");
        break;
    case ALOP_FCMB0:
    case ALOP_FCMB1: {
        int opc1 = fcomb_opc1(alop);
        int opc2 = fcomb_opc2(alop);
        if (!fcomb_check(ctx, alop, opc1, opc2)) {
            gen_tr_excp_illopc(ctx);
            return;
        }
        alop->format = ALOPF21_FCOMB;
        alop->op = (opc2 << 16) | opc1;
        alop->args = alop->als.opc1 & 1 ? ARGS_DDDD : ARGS_SSSS;
        break;
    }
    case ALOP_PFCMB0:
    case ALOP_PFCMB1:
        if (alop->ales.opc2 == ALOP_PFCMB1 && is_chan_0134(alop->chan)
            && ctx->version >= 2 && alop->als.opc1 == 0x4d)
        {
            alop->format = ALOPF21;
            alop->op = OP_PSHUFB;
            alop->args = ARGS_DDDD;
        } else if (alop->ales.opc2 == ALOP_PFCMB1 && is_chan_0134(alop->chan)
            && ctx->version >= 2 && alop->als.opc1 == 0x6d)
        {
            alop->format = ALOPF21;
            alop->op = OP_PMERGE;
            alop->args = ARGS_DDDD;
        } else {
            int opc1 = fcomb_opc1(alop);
            int opc2 = fcomb_opc2(alop);
            if (!pfcomb_check(ctx, alop, opc1, opc2)) {
                gen_tr_excp_illopc(ctx);
                return;
            }
            alop->format = ALOPF21_PFCOMB;
            alop->op = (opc2 << 16) | opc1;
            alop->args = ARGS_DDDD;
        }
        break;
    case ALOP_LCMBD0:
    case ALOP_LCMBD1:
        if (is_chan_0134(alop->chan) && ctx->version >= 5) {
            alop->format = ALOPF21;
            alop->op = alop->ales.opc2 == ALOP_LCMBD0 ? OP_PLOG_0x00 : OP_PLOG_0x80;
            alop->args = ARGS_DDDD;
        } else {
            gen_tr_excp_illopc(ctx);
        }
        break;
    case ALOP_LCMBQ0:
    case ALOP_LCMBQ1:
        if (is_chan_0134(alop->chan) && ctx->version >= 5) {
            alop->format = ALOPF21;
            alop->op = alop->ales.opc2 == ALOP_LCMBQ0 ? OP_QPLOG_0x00 : OP_QPLOG_0x80;
            alop->args = ARGS_PPPP;
        } else {
            gen_tr_excp_illopc(ctx);
        }
        break;
    case ALOP_QPFCMB0:
    case ALOP_QPFCMB1:
        if (alop->ales.opc2 == ALOP_QPFCMB1 && is_chan_0134(alop->chan)
            && ctx->version >= 5 && alop->als.opc1 == 0xd)
        {
            alop->format = ALOPF21;
            alop->op = OP_QPPERMB;
        } else if (alop->ales.opc2 == ALOP_QPFCMB1 && is_chan_0134(alop->chan)
            && ctx->version >= 5 && alop->als.opc1 == 0x4d)
        {
            alop->format = ALOPF21;
            alop->op = OP_QPSHUFB;
        } else if (alop->ales.opc2 == ALOP_QPFCMB1 && is_chan_0134(alop->chan)
            && ctx->version >= 5 && alop->als.opc1 == 0x6d)
        {
            alop->format = ALOPF21;
            alop->op = OP_QPMERGE;
        } else {
            int opc1 = fcomb_opc1(alop);
            int opc2 = fcomb_opc2(alop);
            // TODO: qpfcomb check ops
            if (ctx->version < 5) {
                gen_tr_excp_illopc(ctx);
                return;
            }
            alop->format = ALOPF21_QPFCOMB;
            alop->op = (opc2 << 16) | opc1;
        }
        alop->args = ARGS_PPPP;
        break;
    default:
        gen_tr_excp_illopc(ctx);
        break;
    }
}

static inline void check_reg_src(int *max_r, int *max_b, uint8_t src)
{
    if (IS_REGULAR(src)) {
        *max_r = MAX(*max_r, GET_REGULAR(src));
    } else if (IS_BASED(src)) {
        *max_b = MAX(*max_b, GET_BASED(src));
    }
}

static inline void check_reg_dst(int *max_r, int *max_b, uint8_t dst)
{
    if (IS_REGULAR(dst)) {
        *max_r = MAX(*max_r, GET_REGULAR(dst));
    } else if (IS_BASED(dst)) {
        *max_b = MAX(*max_b, GET_BASED(dst));
    }
}

static void alop_find_max_reg_indices(Alop *alop, int *max_r_src,
    int *max_r_dst, int *max_b)
{
    switch (alop->format) {
    case ALOPF_NONE:
        break;
    case ALOPF1:
    case ALOPF1_MAS:
    case ALOPF1_MERGE:
    case ALOPF11:
    case ALOPF11_MAS:
    case ALOPF11_MERGE:
    case ALOPF11_LIT8:
        check_reg_src(max_r_src, max_b, alop->als.src1);
        check_reg_src(max_r_src, max_b, alop->als.src2);
        check_reg_dst(max_r_dst, max_b, alop->als.dst);
        break;
    case ALOPF2:
    case ALOPF12:
    case ALOPF12_PSHUFH:
    case ALOPF22:
        check_reg_src(max_r_src, max_b, alop->als.src2);
        check_reg_dst(max_r_dst, max_b, alop->als.dst);
        break;
    case ALOPF3:
    case ALOPF13:
        check_reg_src(max_r_src, max_b, alop->als.src1);
        check_reg_src(max_r_src, max_b, alop->als.src2);
        check_reg_src(max_r_src, max_b, alop->als.src4);
        break;
    case ALOPF7:
    case ALOPF17:
        check_reg_src(max_r_src, max_b, alop->als.src1);
        check_reg_src(max_r_src, max_b, alop->als.src2);
        break;
    case ALOPF8:
    case ALOPF15:
        check_reg_src(max_r_src, max_b, alop->als.src2);
        break;
    case ALOPF10:
        check_reg_src(max_r_src, max_b, alop->als.src4);
        break;
    case ALOPF16:
        check_reg_dst(max_r_dst, max_b, alop->als.dst);
        break;
    case ALOPF21:
    case ALOPF21_ICOMB:
    case ALOPF21_FCOMB:
    case ALOPF21_PFCOMB:
    case ALOPF21_QPFCOMB:
        check_reg_src(max_r_src, max_b, alop->als.src1);
        check_reg_src(max_r_src, max_b, alop->als.src2);
        check_reg_src(max_r_src, max_b, alop->ales.src3);
        check_reg_dst(max_r_dst, max_b, alop->als.dst);
        break;
    default:
        g_assert_not_reached();
        break;
    }
}

static void decode_alops(DisasContext *ctx)
{
    int i;

    for (i = 0; i < 6; i++) {
        Alop *alop = &ctx->alops[i];

        alop->format = ALOPF_NONE;
        alop->ctx = ctx;
        alop->name = "none";
        alop->chan = i;

        if (ctx->bundle.als_present[i]) {
            alop->mas = ctx->cs1.type == CS1_MAS ? ctx->cs1.mas[i] : 0;
            alop->als.raw = ctx->bundle.als[i];
            alop->ales.raw = ctx->bundle.ales[i];

            decode_alop(alop, ctx->bundle.ales_present[i]);
        }
    }
}

static void gen_alop(Alop *alop)
{
    DisasContext *ctx = alop->ctx;
    TCGLabel *l0, *l1;

    if (alop->format == ALOPF_NONE) {
        return;
    }

    gen_alop_save_dst(alop);
    l0 = gen_new_label();
    l1 = gen_new_label();

    if (ctx->loop_mode && is_alop_store(alop)) {
        TCGv_i32 t0 = tcg_temp_new_i32();

        gen_st_lm_cond_i32(t0, ctx->loop_end);
        tcg_gen_brcondi_i32(TCG_COND_NE, t0, 0, l1);
        gen_dec_lsr_strmd(ctx->loop_end);

        tcg_temp_free_i32(t0);
    }

    gen_alop_pred(alop, l0);

    switch (alop->format) {
    case ALOPF21_ICOMB:
        gen_icomb(alop);
        break;
    case ALOPF21_FCOMB:
        gen_fcomb(alop);
        break;
    case ALOPF21_PFCOMB:
        gen_pfcomb(alop);
        break;
    case ALOPF21_QPFCOMB:
        gen_qpfcomb(alop);
        break;
    default:
        gen_alop_simple(alop);
        break;
    }

    gen_set_label(l0);

    if (alop->format == ALOPF10 && (alop->als.aaopc & 1)
        && (alop->mas & 0x7) != 0x7)
    {
        gen_aasti_incr(alop);
    }

    gen_set_label(l1);
}

static void gen_alc(DisasContext *ctx)
{
    int i;

    if (ctx->enable_tags) {
        for (i = 0; i < 6; i++) {
            Alop *alop = &ctx->alops[i];

            if (!alop->als.sm && is_alop_check_tag(alop)) {
                ctx->delayed_illop = e2k_get_temp_i32(ctx);
                tcg_gen_movi_i32(ctx->delayed_illop, 0);
                break;
            }
        }
    }

    for (i = 0; i < 6; i++) {
        gen_alop(&ctx->alops[i]);
    }
}

static void alc_init(DisasContext *ctx)
{
    int i, j;

    memset(alops_map, -1, sizeof(alops_map));
    memset(pfcomb_map, 0, sizeof(pfcomb_map));

    // TODO: symmetric alops table
    /* Most alops are symmetric and can be stored in a half table. */
    for (i = 0; i < ARRAY_SIZE(alops); i++) {
        AlopDesc *desc = &alops[i];
        if (desc->min_version <= ctx->version && ctx->version <= desc->max_version) {
            for (j = 0; j < 6; j++) {
                if (desc->channels & (1 << j)) {
                    int16_t *p = &alops_map[desc->opc2][desc->opc1][j];
                    desc->next[j] = *p;
                    *p = i;
                }
            }
        }
    }

    pfcomb_init(ctx);
}

static void gen_load_prefetch_program(DisasContext *ctx)
{
    gen_helper_aau_load_program(cpu_env);
}

static void gen_aau_result_d(DisasContext *ctx, Mova *instr,
    TCGv_i32 tag, TCGv_i64 val)
{
    uint8_t dst = instr->dst;

    if (dst == 0xdf) {
        /* %empty */
    } else if (IS_REG(dst)) {
        TCGv_i32 t0 = tcg_temp_local_new_i32();

        gen_reg_index(ctx, t0, dst);
        gen_reg_set_d(ctx, t0, tag, val);
        tcg_temp_free_i32(t0);
    } else {
        gen_tr_excp_illopc(ctx);
    }
}

static void gen_checked_ld(DisasContext *ctx, Mova *instr, TCGv ptr)
{
    TCGLabel *l0 = gen_new_label();
    TCGLabel *l1 = gen_new_label();
    TCGv_i32 tag = tcg_temp_local_new_i32();
    TCGv_i64 val = tcg_temp_local_new_i64();
    MemOp memop = instr->be ? MO_BE : MO_LE;

    switch(instr->opc) {
    case 1: memop |= MO_8; break; /* movab */
    case 2: memop |= MO_16; break; /* movah */
    case 3: memop |= MO_32; break; /* movaw */
    case 4: memop |= MO_64; break; /* movad */
    default:
        g_assert_not_reached();
        break;
    }

    tcg_gen_brcondi_tl(TCG_COND_NE, ptr, 0, l0);

    /* if address is invalid */
    tcg_gen_movi_i32(tag, E2K_TAG_NON_NUMBER64);
    tcg_gen_movi_i64(val, E2K_MOVA_RESULT_INVALID);
    tcg_gen_br(l1);

    /* if address is valid */
    gen_set_label(l0);
    tcg_gen_movi_i32(tag, E2K_TAG_NUMBER64);
    tcg_gen_qemu_ld_i64(val, ptr, 0, memop);

    gen_set_label(l1);
    gen_aau_result_d(ctx, instr, tag, val);

    tcg_temp_free_i64(val);
    tcg_temp_free_i32(tag);
}

static void gen_checked_ld_qp(DisasContext *ctx, Mova *instr, TCGv addr)
{
    MemOp memop = (instr->be ? MO_BE : MO_LE) | MO_64;
    TCGLabel *l0 = gen_new_label();
    TCGLabel *l1 = gen_new_label();
    TCGv_i32 tag = tcg_temp_local_new_i32();
    TCGv_i64 t0 = tcg_temp_local_new_i64();
    TCGv_i64 t1 = tcg_temp_local_new_i64();

    tcg_gen_brcondi_tl(TCG_COND_NE, addr, 0, l0);

    /* if address is invalid */
    tcg_gen_movi_i32(tag, E2K_TAG_NON_NUMBER128);
    tcg_gen_movi_i64(t0, E2K_MOVA_RESULT_INVALID);
    tcg_gen_movi_i64(t1, E2K_MOVA_RESULT_INVALID);
    tcg_gen_br(l1);

    /* if address is valid */
    gen_set_label(l0);
    tcg_gen_movi_i32(tag, E2K_TAG_NUMBER128);
    gen_qemu_ld_i128(t1, t0, addr, ctx->mmuidx, memop);

    gen_set_label(l1);

    if (instr->dst == 0xdf) {
        /* %empty */
    } else if (IS_REG(instr->dst)) {
        gen_reg_set_q(ctx, tag, t1, t0, instr->dst);
    } else {
        gen_tr_excp_illopc(ctx);
    }

    tcg_temp_free_i64(t1);
    tcg_temp_free_i64(t0);
    tcg_temp_free_i32(tag);
}

static void gen_mova_ptr(TCGv ret, Mova *instr, int size, int mmu_idx)
{
    TCGv_i32 t0 = tcg_const_i32(instr->chan);
    TCGv_i32 t1 = tcg_const_i32(instr->area);
    TCGv_i32 t2 = tcg_const_i32(instr->ind);
    TCGv_i32 t3 = tcg_const_i32(size);
    TCGv_i32 t4 = tcg_const_i32(mmu_idx);

    gen_helper_mova_ptr(ret, cpu_env, t0, t1, t2, t3, t4);
    tcg_temp_free_i32(t4);
    tcg_temp_free_i32(t3);
    tcg_temp_free_i32(t2);
    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
}

static void gen_mova(DisasContext *ctx, Mova *instr)
{
    /* branch in gen_checked_ld */
    TCGv t0 = tcg_temp_local_new();

    ctx->aau_am[instr->chan] = instr->am ? instr->area : -1;
    // TODO: check ind has proper alignment
    // TODO: check ind is less than mrng

    switch(instr->opc) {
    case 1: /* movab */
    case 2: /* movah */
    case 3: /* movaw */
    case 4: /* movad */
        gen_mova_ptr(t0, instr, 1 << (instr->opc - 1), ctx->mmuidx);
        gen_checked_ld(ctx, instr, t0);
        break;
    case 5: /* movaq */
        e2k_todo_illop(ctx, "movaq");
        break;
    case 7: /* movaqp */
        gen_mova_ptr(t0, instr, 16, ctx->mmuidx);
        gen_checked_ld_qp(ctx, instr, t0);
        break;
    default:
        gen_tr_excp_illopc(ctx);
        break;
    }

    tcg_temp_free(t0);
}

static inline void gen_aau_am(DisasContext *ctx, int chan, int area)
{
    TCGv_i32 t0 = tcg_const_i32(chan);
    TCGv_i32 t1 = tcg_const_i32(area);

    gen_helper_aau_am(cpu_env, t0, t1);
    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
}

static void gen_aau(DisasContext *ctx)
{
    const UnpackedBundle *bundle = &ctx->bundle;
    int i;

    for (i = 0; i < 4; i++) {
        Mova instr = { 0 };

        instr.chan = i;
        instr.aas = bundle->aas[i + 2];
        instr.dst = extract16(bundle->aas[i / 2], ((i & 1) ^ 1) * 8, 8);

        if (!bundle->aas_present[i + 2] || instr.opc == 0) {
            ctx->aau_am[i] = -1;
            continue;
        }

        gen_mova(ctx, &instr);
    }

    for (i = 0; i < 4; i++) {
        int area = ctx->aau_am[i];
        if (area == -1 || ((i == 1 || i == 3) && ctx->aau_am[i - 1] == area)) {
            continue;
        }
        gen_aau_am(ctx, i, area);
    }

    /* bap */
    if (ctx->bundle.ss & (1 << 28)) {
        gen_load_prefetch_program(ctx);
    }
}

static inline void gen_set_ctpr(int index, uint64_t ctpr)
{
    assert(0 < index && index < 4);
    tcg_gen_movi_i64(cpu_ctprs[index - 1], ctpr);
}

static inline void gen_ctpr_tag(TCGv_i64 ret, TCGv_i64 ctpr)
{
    tcg_gen_extract_i64(ret, ctpr, CTPR_TAG_OFF, CTPR_TAG_LEN);
}

static inline void gen_goto_ctpr_disp(TCGv_i64 ctpr)
{
    TCGv_i64 t0 = tcg_temp_new_i64();
    TCGv t1 = tcg_temp_new();

    tcg_gen_extract_i64(t0, ctpr, CTPR_BASE_OFF, CTPR_BASE_LEN);
    tcg_gen_trunc_i64_tl(t1, t0);
    tcg_gen_mov_tl(cpu_pc, t1);
    tcg_gen_lookup_and_goto_ptr();

    tcg_temp_free(t1);
    tcg_temp_free_i64(t0);
}

static inline void gen_vfrpsz(DisasContext *ctx)
{
    Cs1 *cs1 = &ctx->cs1;
    Cs1Setr *setr = &cs1->setr;

    if (cs1->type == CS1_SETR && (setr->type & SETR_VFRPSZ)) {
        e2k_todo_illop(ctx, "vfrpsz");
    }
}

static inline void gen_setwd(DisasContext *ctx)
{
    Cs1 *cs1 = &ctx->cs1;
    Cs1Setr *setr = &cs1->setr;

    if (cs1->type == CS1_SETR && (setr->type & SETR_WD)) {
        TCGv_i32 t0 = tcg_const_i32(setr->wsz);
        TCGv_i32 t1 = tcg_const_i32(setr->nfx);
        TCGv_i32 t2 = tcg_const_i32(setr->dbl);
        ctx->wd_size = setr->wsz * 2;
        gen_helper_setwd(cpu_env, t0, t1, t2);
        tcg_temp_free_i32(t2);
        tcg_temp_free_i32(t1);
        tcg_temp_free_i32(t0);
    }
}

static inline void gen_setbn(DisasContext *ctx)
{
    Cs1 *cs1 = &ctx->cs1;
    Cs1Setr *setr = &cs1->setr;

    if (cs1->type == CS1_SETR && (setr->type & SETR_BN)) {
        ctx->bsize = (setr->rsz + 1) * 2;
        tcg_gen_movi_i32(cpu_boff, setr->rbs * 2);
        tcg_gen_movi_i32(cpu_bsize, (setr->rsz + 1) * 2);
        tcg_gen_movi_i32(cpu_bcur, setr->rcur * 2);
    }
}

static inline void gen_setbp(DisasContext *ctx)
{
    Cs1 *cs1 = &ctx->cs1;
    Cs1Setr *setr = &cs1->setr;

    if (cs1->type == CS1_SETR && (setr->type & SETR_BP)) {
        tcg_gen_movi_i32(cpu_psize, setr->psz + 1);
        tcg_gen_movi_i32(cpu_pcur, 0);
    }
}

static inline void gen_cs1(DisasContext *ctx)
{
    Cs1 *cs1 = &ctx->cs1;

    switch(cs1->type) {
    case CS1_NONE:
    case CS1_MAS:
    case CS1_SETR:
        break;
    case CS1_CALL:
        ctx->ct.type = CT_CALL;
        ctx->ct.wbs = cs1->call_wbs;
        break;
    case CS1_WAIT:
        // TODO: wait
        break;
    case CS1_FLUSH:
        // TODO: flush
        break;
    default:
        e2k_todo_illop(ctx, "unimplemented %d", cs1->type);
        break;
    }
}

static inline void gen_cs0(DisasContext *ctx)
{
    Cs0 *cs0 = &ctx->cs0;

    switch(cs0->type) {
    case CS0_NONE:
        break;
    case CS0_IBRANCH:
        ctx->ct.type = CT_IBRANCH;
        ctx->ct.u.target = ctx->pc + cs0->ibranch.sdisp;
        break;
    case CS0_PREF:
        /* prefetch code */
        break;
    case CS0_DISP: {
        uint64_t ctpr = ctpr_new_disp(ctx, &cs0->disp);
        gen_set_ctpr(cs0->disp.ctpr, ctpr);
        break;
    }
    case CS0_SDISP: {
#ifdef CONFIG_USER_ONLY
        // TODO: real sdisp target address
        target_ulong target = E2K_FAKE_KERN_START;
        target = deposit64(target, 11, 17, cs0->sdisp.disp);
        uint64_t ctpr = ctpr_new(CTPR_TAG_SDISP, 0, cs0->sdisp.ipd, target);
        gen_set_ctpr(cs0->sdisp.ctpr, ctpr);
#else
        e2k_todo(ctx, "sdisp");
#endif
        break;
    }
    case CS0_RETURN: {
        TCGv_i32 t0 = tcg_const_i32(cs0->ret.ipd);
        gen_helper_prep_return(cpu_ctprs[2], cpu_env, t0);
        tcg_temp_free_i32(t0);
        break;
    }
    default:
        e2k_todo_illop(ctx, "unimplemented %d", cs0->type);
        break;
    }
}

static void gen_ct_cond(DisasContext *ctx)
{
    ControlTransfer *ct = &ctx->ct;
    TCGv_i32 pcond, lcond;

    if (ct->type == CT_NONE) {
        return;
    }

    if (ct->cond_type == 1) {
        tcg_gen_movi_i32(cpu_ct_cond, 1);
        return;
    }

    pcond = tcg_temp_new_i32();
    lcond = tcg_temp_new_i32();

    switch (ct->cond_type) {
    case 0x2:
    case 0x6:
    case 0xf:
        /* %predN */
        gen_preg_i32(ctx, pcond, ct->psrc);
        break;
    case 0x3:
    case 0x7:
    case 0xe: {
        TCGv_i32 t0 = tcg_temp_new_i32();
        gen_preg_i32(ctx, t0, ct->psrc);
        tcg_gen_setcondi_i32(TCG_COND_EQ, pcond, t0, 0);
        tcg_temp_free_i32(t0);
        break;
    }
    default:
        break;
    }

    switch (ct->cond_type) {
    case 0x4:
    case 0x6:
    case 0xe:
        /* #LOOP_END */
        tcg_gen_mov_i32(lcond, ctx->loop_end);
        break;
    case 0x5:
    case 0x7:
    case 0xf: /* #NOT_LOOP_END */
        tcg_gen_setcondi_i32(TCG_COND_EQ, lcond, ctx->loop_end, 0);
        break;
    default:
        break;
    }

    switch (ct->cond_type) {
    case 0x2:
    case 0x3:
        /* {,~}%predN */
        tcg_gen_mov_i32(cpu_ct_cond, pcond);
        break;
    case 0x4:
    case 0x5:
        /* #{,NOT_}LOOP_END */
        tcg_gen_mov_i32(cpu_ct_cond, lcond);
        break;
    case 0x6:
    case 0xe: {
        /* {,~}%predN || #LOOP_END */
        TCGv_i32 z = tcg_const_i32(0);
        TCGv_i32 t0 = tcg_temp_new_i32();

        tcg_gen_or_i32(t0, pcond, lcond);
        tcg_gen_movcond_i32(TCG_COND_EQ, cpu_ct_cond, cpu_lsr_pcnt, z, t0, lcond);
        tcg_temp_free_i32(t0);
        tcg_temp_free_i32(z);
        break;
    }
    case 0x7:
    case 0xf: {
        /* {,~}%predN && #NOT_LOOP_END */
        TCGv_i32 z = tcg_const_i32(0);
        TCGv_i32 t0 = tcg_temp_new_i32();

        tcg_gen_and_i32(t0, pcond, lcond);
        tcg_gen_movcond_i32(TCG_COND_EQ, cpu_ct_cond, cpu_lsr_pcnt, z, t0, lcond);
        tcg_temp_free_i32(t0);
        tcg_temp_free_i32(z);
        break;
    }
    case 0x8:
        /* %MLOCK || %dt_alM */
        if (ct->psrc & 0xf) {
//            static const int conv[] = {0, 1, 3, 4};
            int i;

            e2k_todo(ctx, "%%MLOCK || %%dt_alM");
            // %dt_al
            for (i = 0; i < 4; i++) {
                if (ct->psrc & (1 << i)) {
                    // i
                }
            }
        } else {
            /* %MLOCK */
            tcg_gen_mov_i32(cpu_ct_cond, ctx->mlock);
        }
        break;
    case 0x9: {
        /* `lock_cond || pl_cond' control transfer conditions.  */
        unsigned int type = (ct->psrc & 0x18) >> 3;
        if (type == 0) {
//            static const int cmp_num_to_alc[] = {0, 1, 3, 4};
//            unsigned int cmp_num = (psrc & 0x6) >> 1;
//            unsigned int neg = psrc & 0x1;
//
//            my_printf ("%%MLOCK || %s%%cmp%d", neg ? "~" : "",
//                cmp_num_to_alc[cmp_num]);
            e2k_todo(ctx, "%%MLOCK || %%cmpN");
        } else if (type == 1) {
//            unsigned int cmp_jk = (psrc & 0x4) >> 2;
//            unsigned int negj = (psrc & 0x2) >> 1;
//            unsigned int negk = psrc & 0x1;
//
//            my_printf ("%%MLOCK || %s%%cmp%d || %s%%cmp%d",
//                     negj ? "~" : "", cmp_jk == 0 ? 0 : 3,
//                     negk ? "~" : "", cmp_jk == 0 ? 1 : 4);
            e2k_todo(ctx, "%%MLOCK || %%cmpN || %%cmpM");
        } else if (type == 2) {
//                unsigned int clp_num = (psrc & 0x6) >> 1;
//                unsigned int neg = psrc & 0x1;

            // "%%MLOCK || %s%%clp%d", neg ? "~" : "", clp_num
            e2k_todo(ctx, "%%MLOCK || %%clpN");
        }
        break;
    }
    default:
        e2k_todo_illop(ctx, "undefined control transfer type %#x", ct->cond_type);
        break;
    }

    tcg_temp_free_i32(lcond);
    tcg_temp_free_i32(pcond);
}

static inline TCGCond cond_from_advance(int advance)
{
    switch (advance) {
    case 0x01: return TCG_COND_EQ;
    case 0x02: return TCG_COND_NE;
    case 0x03: return TCG_COND_ALWAYS;
    default: return TCG_COND_NEVER;
    }
}

static void gen_movcond_flag_i32(TCGv_i32 ret, int flag, TCGv_i32 cond,
    TCGv_i32 v1, TCGv_i32 v2)
{
    TCGv_i32 one = tcg_const_i32(1);
    TCGCond c = cond_from_advance(flag);

    tcg_gen_movcond_i32(c, ret, cond, one, v1, v2);
    tcg_temp_free_i32(one);
}

static void gen_dec_wrap(TCGv_i32 ret, TCGv_i32 cur, int n,
    TCGv_i32 size)
{
    TCGv_i32 t0 = tcg_temp_new_i32();

    tcg_gen_addi_i32(t0, size, n);
    tcg_gen_sub_i32(t0, t0, cur);
    tcg_gen_remu_i32(t0, t0, size);
    tcg_gen_sub_i32(ret, size, t0);

    tcg_temp_free_i32(t0);
}

static inline void gen_cur_dec(DisasContext *ctx, TCGv_i32 ret, int cond,
    TCGv_i32 cur, int n, TCGv_i32 size)
{
    TCGLabel *l0 = gen_new_label();
    TCGv_i32 t0 = tcg_temp_new_i32();

    tcg_gen_brcondi_i32(TCG_COND_EQ, size, 0, l0);
    gen_dec_wrap(t0, cur, n, size);
    gen_movcond_flag_i32(ret, cond, cpu_ct_cond, t0, cur);
    gen_set_label(l0);

    tcg_temp_free_i32(t0);
}

#define IMPL_GEN_DEC_SAT(name, S) \
    static void name(temp(S) ret, temp(S) arg0) \
    { \
        temp(S) t0 = call(S, tcg_const, 0); \
        temp(S) t1 = temp_new(S); \
        \
        call(S, tcg_gen_subi, t1, arg0, 1); \
        call(S, tcg_gen_movcond, TCG_COND_EQ, ret, arg0, t0, arg0, t1); \
        \
        temp_free(S, t1); \
        temp_free(S, t0); \
    }

IMPL_GEN_DEC_SAT(gen_dec_sat_i32, s)
IMPL_GEN_DEC_SAT(gen_dec_sat_i64, d)

static void gen_advance_loop_counters(void)
{
    TCGv_i32 z = tcg_const_i32(0);
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i64 t1 = tcg_temp_new_i64();
    TCGv_i32 t2 = tcg_temp_new_i32();
    TCGv_i32 t3 = tcg_temp_new_i32();
    TCGv_i32 t4 = tcg_temp_new_i32();

    gen_dec_sat_i32(cpu_lsr_pcnt, cpu_lsr_pcnt);
    gen_dec_sat_i64(cpu_lsr_lcnt, cpu_lsr_lcnt);
    tcg_gen_setcondi_i32(TCG_COND_EQ, t0, cpu_lsr_pcnt, 0);
    tcg_gen_setcondi_i64(TCG_COND_EQ, t1, cpu_lsr_lcnt, 0);
    tcg_gen_extrl_i64_i32(t2, t1);
    tcg_gen_mov_i32(cpu_lsr_over, t2);
    gen_dec_sat_i32(t3, cpu_lsr_ecnt);
    tcg_gen_and_i32(t4, t2, cpu_lsr_vlc);
    tcg_gen_movcond_i32(TCG_COND_NE, cpu_lsr_ecnt, t4, z, t3, cpu_lsr_ecnt);

    tcg_temp_free_i32(t4);
    tcg_temp_free_i32(t3);
    tcg_temp_free_i32(t2);
    tcg_temp_free_i64(t1);
    tcg_temp_free_i32(t0);
    tcg_temp_free_i32(z);
}

static void gen_stubs(DisasContext *ctx)
{
    uint32_t ss = ctx->bundle.ss;
    int alc = extract32(ss, 16, 2);
    int abp = extract32(ss, 18, 2);
    int abn = extract32(ss, 21, 2);
    int abg = extract32(ss, 23, 2);
    int vfdi = extract32(ss, 26, 1);

    if (alc) {
        TCGLabel *l0 = gen_new_label();
        TCGCond cond = cond_from_advance(alc);

        tcg_gen_brcondi_i32(tcg_invert_cond(cond), cpu_ct_cond, 1, l0);
        gen_advance_loop_counters();
        gen_set_label(l0);
    }

    if (abp) {
        gen_cur_dec(ctx, cpu_pcur, abp, cpu_pcur, 1, cpu_psize);
    }

    if (abn) {
        gen_cur_dec(ctx, cpu_bcur, abn, cpu_bcur, 2, cpu_bsize);
    }

    if (abg != 0) {
        // TODO: impl abg
        e2k_todo_illop(ctx, "abg");
    }

    if (vfdi != 0) {
        // TODO: impl vfdi
        e2k_todo_illop(ctx, "vfdi");
    }
}

static target_ulong do_decode(DisasContext *ctx, CPUState *cs)
{
    E2KCPU *cpu = E2K_CPU(cs);
    CPUE2KState *env = &cpu->env;
    unsigned int len;

    ctx->pc = ctx->base.pc_next;
    len = unpack_bundle(env, ctx->pc, &ctx->bundle);

    if (len == 0) {
        gen_tr_excp_illopc(ctx);
    }

    decode_cs1(ctx, &ctx->bundle);
    decode_cs0(ctx, &ctx->bundle);
    decode_ct_cond(ctx, &ctx->bundle);
    decode_alops(ctx);

    return ctx->pc + len;
}

static bool validate_state_reg(DisasContext *ctx, int index, bool write)
{
    switch (index) {
    case SR_PSR:
    case SR_WD:
    case SR_CWD:
    case SR_PSP_HI:
    case SR_PSP_LO:
    case SR_PSHTP:
    case SR_PCSP_HI:
    case SR_PCSP_LO:
    case SR_PCSHTP:
    case SR_CTPR1:
    case SR_CTPR2:
    case SR_CTPR3:
    case SR_SBR:
    case SR_CUTD:
    case SR_CUIR:
    case SR_OSCUD_HI:
    case SR_OSCUD_LO:
    case SR_OSGD_HI:
    case SR_OSGD_LO:
    case SR_OSEM:
    case SR_USD_HI:
    case SR_USD_LO:
    case SR_OSR0:
    case SR_CUD_HI:
    case SR_CUD_LO:
    case SR_GD_HI:
    case SR_GD_LO:
    case SR_CS_HI:
    case SR_CS_LO:
    case SR_DS_HI:
    case SR_DS_LO:
    case SR_ES_HI:
    case SR_ES_LO:
    case SR_FS_HI:
    case SR_FS_LO:
    case SR_GS_HI:
    case SR_GS_LO:
    case SR_SS_HI:
    case SR_SS_LO:
    case SR_DIBCR:
    case SR_DIMCR:
    case SR_DIBSR:
    case SR_DTCR:
    case SR_DIBAR0:
    case SR_DIBAR1:
    case SR_DIBAR2:
    case SR_DIBAR3:
    case SR_DIMAR0:
    case SR_DIMAR1:
    case SR_DTRAF:
    case SR_DTART:
    case SR_CR0_HI:
    case SR_CR0_LO:
    case SR_CR1_HI:
    case SR_CR1_LO:
    case SR_SCLKM1:
    case SR_SCLKM2:
    case SR_CU_HW0:
    case SR_UPSR:
    case SR_NIP:
    case SR_LSR:
    case SR_PFPFR:
    case SR_FPCR:
    case SR_FPSR:
    case SR_ILCR:
    case SR_BR:
    case SR_BGR:
    case SR_CLKR:
    case SR_RNDPR:
    case SR_SCLKR:
    case SR_TIR_HI:
    case SR_TIR_LO:
    case SR_RPR:
    case SR_SBBP:
    case SR_RPR_HI:
    case SR_UPSRM:
        return true;
    case SR_IDR:
    case SR_IP:
        return !write;
    case SR_EIR:
    case SR_TSD:
    case SR_TR:
        return false; /* FIXME */
    case SR_CORE_MODE:
        return ctx->version >= 3;
    case SR_LSR1:
    case SR_ILCR1:
        return ctx->version >= 5;
    default:
        return false;
    }
}

static void validate_bundle(DisasContext *ctx)
{
    int i;

    for (i = 0; i < 6; i++) {
        Alop *alop = &ctx->alops[i];

        switch (alop->format) {
        case ALOPF3:
        case ALOPF13:
            switch (ctx->alops[i == 2 ? 3 : 0].format) {
            case ALOPF1_MAS:
            case ALOPF11_MAS:
                /* Read and write operations cannot be executed simultaneously
                 * on the mirrored channels (2+3 or 0+5). */
                gen_tr_excp_illopc(alop->ctx);
                break;
            default:
                break;
            }
            break;
        case ALOPF15:
            if (!validate_state_reg(ctx, alop->als.dst, true)) {
                gen_tr_excp_illopc(alop->ctx);
            }
            break;
        case ALOPF16:
            if (!validate_state_reg(ctx, alop->als.src2, false)) {
                gen_tr_excp_illopc(alop->ctx);
            }
            break;
        default:
            break;
        }
    }
}

static inline void gen_window_bounds_checki_i32(TCGCond cond, TCGv_i32 arg1,
    int arg2)
{
    TCGLabel *l0 = gen_new_label();
    tcg_gen_brcondi_i32(tcg_invert_cond(cond), arg1, arg2, l0);
    gen_excp_window_bounds();
    gen_set_label(l0);
}

static inline void gen_alop_reg_indices_check(DisasContext *ctx)
{
    const Cs1 *cs1 = &ctx->cs1;
    const Cs1Setr *setr = &cs1->setr;
    int i, max_r_src, max_r_dst, max_b_cur;

    max_r_src = -1;
    max_r_dst = -1;
    max_b_cur = -1;
    for (i = 0; i < 6; i++) {
        alop_find_max_reg_indices(&ctx->alops[i], &max_r_src,
            &max_r_dst, &max_b_cur);
    }

    if (ctx->wd_size != DYNAMIC) {
        /* %rN src static check */
        if (ctx->wd_size <= max_r_src) {
            gen_tr_excp_window_bounds(ctx);
        }
        /* %rN dst static check */
        if (cs1->type == CS1_SETR && (setr->type & SETR_WD)) {
            if (setr->wsz * 2 <= max_r_dst) {
                gen_tr_excp_window_bounds(ctx);
            }
        } else if (ctx->wd_size <= max_r_dst) {
            gen_tr_excp_window_bounds(ctx);
        }
    } else if (cs1->type == CS1_SETR && (setr->type & SETR_WD)) {
        /* %rN src dynamic check */
        if (ctx->max_r < max_r_src) {
            ctx->max_r = max_r_src;
            gen_window_bounds_checki_i32(TCG_COND_LE, cpu_wd_size,
                max_r_src);
        }

        /* %rN dst static check */
        if (setr->wsz * 2 <= max_r_dst) {
            gen_tr_excp_window_bounds(ctx);
        }
    } else {
        /* %rN src/dst dynamic check */
        int max = MAX(max_r_src, max_r_dst);
        if (ctx->max_r < max) {
            ctx->max_r = max;
            gen_window_bounds_checki_i32(TCG_COND_LE, cpu_wd_size, max);
        }
    }

    if (ctx->bsize != DYNAMIC) {
        /* %b[N] src/dst static check */
        if (ctx->bsize <= max_b_cur) {
            gen_tr_excp_window_bounds(ctx);
        }
    } else if (ctx->max_b < max_b_cur) {
        /* %b[N] src/dst dynamic check */
        ctx->max_b = max_b_cur;
        gen_window_bounds_checki_i32(TCG_COND_LE, cpu_bsize, ctx->max_b);
    }
}

static void gen_mlock_init(DisasContext *ctx)
{
    ControlTransfer *ct = &ctx->ct;

    if (ct->type != CT_NONE && ct->cond_type == 0x8) {
        ctx->mlock = e2k_get_temp_i32(ctx);
        tcg_gen_movi_i32(ctx->mlock, 0);
    }
}

static void gen_loop_end_init(DisasContext *ctx)
{
    bool need = false;
    int i;

    switch (ctx->ct.cond_type) {
    case 0x4:
    case 0x5:
    case 0x6:
    case 0x7:
    case 0xe:
    case 0xf:
        need = true;
        break;
    default:
        break;
    }

    if (!need && ctx->loop_mode) {
        for (i = 0; i < 6; i++) {
            if (is_alop_store(&ctx->alops[i])) {
                need = true;
                break;
            }
        }
    }

    if (need) {
        ctx->loop_end = e2k_get_temp_i32(ctx);
        gen_is_loop_end_i32(ctx, ctx->loop_end);
    }
}

static void do_branch(DisasContext *ctx, target_ulong pc_next)
{
    /* FIXME: save PC only when necessary.  */
    gen_save_pc(ctx->base.pc_next);

    if (ctx->enable_tags && ctx->delayed_illop) {
        TCGLabel *l0 = gen_new_label();
        tcg_gen_brcondi_i32(TCG_COND_EQ, ctx->delayed_illop, 0, l0);
        gen_excp_illopc();
        gen_set_label(l0);
    }

    if (ctx->ct.type == CT_NONE) {
        return;
    }

    ctx->base.is_jmp = DISAS_NORETURN;

    if (ctx->ct.cond_type > 1) {
        TCGLabel *l0 = gen_new_label();
        tcg_gen_brcondi_i32(TCG_COND_NE, cpu_ct_cond, 0, l0);
        gen_goto_tb(ctx, TB_EXIT_IDX1, pc_next);
        gen_set_label(l0);
    }

    switch(ctx->ct.type) {
    case CT_NONE:
        break;
    case CT_IBRANCH:
        gen_goto_tb(ctx, TB_EXIT_IDX0, ctx->ct.u.target);
        break;
    case CT_JUMP: {
        TCGLabel *l0 = gen_new_label();
        TCGLabel *l1 = gen_new_label();
        TCGv_i64 t0 = tcg_temp_local_new_i64();

        gen_ctpr_tag(t0, ctx->ct.u.ctpr);
        tcg_gen_brcondi_i64(TCG_COND_EQ, t0, CTPR_TAG_DISP, l0);
        tcg_gen_brcondi_i64(TCG_COND_EQ, t0, CTPR_TAG_RETURN, l1);
        tcg_temp_free_i64(t0);

        // TODO: ldisp or sdisp
        gen_excp_illopc();

        gen_set_label(l1);
        gen_helper_return(cpu_env);

        gen_set_label(l0);
        gen_goto_ctpr_disp(ctx->ct.u.ctpr);
        break;
    }
    case CT_CALL: {
        TCGv_i32 wbs = tcg_const_i32(ctx->ct.wbs);
        TCGv npc = tcg_const_tl(pc_next);

        gen_helper_call(cpu_env, ctx->ct.u.ctpr, wbs, npc);
        tcg_gen_lookup_and_goto_ptr();

        tcg_temp_free(npc);
        tcg_temp_free_i32(wbs);
        break;
    }
    }
}

static void e2k_tr_init_disas_context(DisasContextBase *db, CPUState *cs)
{
    static int version = -1;
    DisasContext *ctx = container_of(db, DisasContext, base);
    E2KCPU *cpu = E2K_CPU(cs);
    CPUE2KState *env = &cpu->env;

    ctx->version = env->version;
    ctx->enable_tags = env->enable_tags;
    ctx->force_save_alc_dst = env->force_save_alc_dst;

    if (version != ctx->version) {
        if (version > 0) {
            // FIXME: can it happen?
            e2k_todo(ctx, "reinitialize alc map");
        }
        alc_init(ctx);
        version = ctx->version;
    }
}

static void e2k_tr_tb_start(DisasContextBase *db, CPUState *cs)
{
    DisasContext *ctx = container_of(db, DisasContext, base);
    E2KCPU *cpu = E2K_CPU(cs);
    CPUE2KState *env = &cpu->env;

    ctx->wd_size = DYNAMIC;
    ctx->max_r = -1;
    ctx->bsize = DYNAMIC;
    ctx->max_b = -1;

    tcg_gen_movi_i32(cpu_ct_cond, 0);

    if (env->is_bp) {
        TCGLabel *l0 = gen_new_label();
        tcg_gen_brcondi_i32(TCG_COND_EQ, cpu_is_bp, 0, l0);
        gen_helper_break_restore_state(cpu_env);
        gen_set_label(l0);
    }
}

static void e2k_tr_insn_start(DisasContextBase *db, CPUState *cs)
{
    DisasContext *ctx = container_of(db, DisasContext, base);

    tcg_gen_insn_start(ctx->base.pc_next);

    ctx->cs0.type = CS0_NONE;
    ctx->cs1.type = CS1_NONE;
    ctx->mlock = NULL;
    ctx->delayed_illop = NULL;
    ctx->saved_reg_len = 0;
    ctx->saved_preg_len = 0;
}

static void e2k_tr_translate_insn(DisasContextBase *db, CPUState *cs)
{
    DisasContext *ctx = container_of(db, DisasContext, base);
    target_ulong pc_next;

    switch (ctx->base.pc_next) {
#ifdef CONFIG_USER_ONLY
# ifdef TARGET_E2K32
    case E2K_SYSCALL_ADDR1:
    case E2K_SYSCALL_ADDR4:
# else /* !TARGET_E2K32 */
    case E2K_SYSCALL_ADDR3:
    case E2K_SYSCALL_ADDR6:
# endif
        /* fake enter into syscall handler */
        ctx->base.is_jmp = DISAS_NORETURN;
        /* force non-zero tb size */
        pc_next = ctx->base.pc_next + 8;
        gen_tr_exception(ctx, EXCP_SYSCALL);
        tcg_gen_exit_tb(NULL, TB_EXIT_IDX0);
        break;
    case E2K_SYSRET_BACKTRACE_ADDR:
    case E2K_SYSRET_ADDR:
    {
        /* fake return from syscall handler */
        TCGv_i32 t0 = tcg_const_i32(0);

        ctx->base.is_jmp = DISAS_NORETURN;
        /* force non-zero tb size */
        pc_next = ctx->base.pc_next + 8;
        gen_helper_prep_return(cpu_ctprs[2], cpu_env, t0);
        gen_helper_return(cpu_env);
        tcg_gen_exit_tb(NULL, TB_EXIT_IDX0);

        tcg_temp_free_i32(t0);
        break;
    }
#endif /* CONFIG_USER_ONLY */
    default:
    {
        pc_next = do_decode(ctx, cs);
#ifdef CONFIG_USER_ONLY
        if (ctx->cs1.type == CS1_CALL) {
            gen_save_cpu_state(ctx);
            gen_helper_expand_stacks(cpu_env);
        }
#endif /* CONFIG_USER_ONLY */

        ctx->loop_mode = (ctx->bundle.hs & (1 << 10)) != 0;
        validate_bundle(ctx);
        gen_alop_reg_indices_check(ctx);
        gen_setwd(ctx);
        gen_cs0(ctx);
        gen_cs1(ctx);
        gen_mlock_init(ctx);
        gen_loop_end_init(ctx);
        gen_alc(ctx);
        gen_aau(ctx);
        gen_plu(ctx);
        gen_ct_cond(ctx);
        gen_vfrpsz(ctx);
        gen_setbn(ctx);
        gen_setbp(ctx);
        gen_stubs(ctx);
        do_branch(ctx, pc_next);
        break;
    }
    }

    ctx->base.pc_next = pc_next;

    /* Free temporary values */
    while(ctx->t32_len) {
        tcg_temp_free_i32(ctx->t32[--ctx->t32_len]);
    }

    while(ctx->t64_len) {
        tcg_temp_free_i64(ctx->t64[--ctx->t64_len]);
    }

    while(ctx->tptr_len) {
        tcg_temp_free_ptr(ctx->tptr[--ctx->tptr_len]);
    }
}

static void e2k_tr_tb_stop(DisasContextBase *db, CPUState *cs)
{
    DisasContext *ctx = container_of(db, DisasContext, base);

    if (ctx->base.is_jmp == DISAS_TOO_MANY) {
        gen_goto_tb(ctx, TB_EXIT_IDX0, ctx->base.pc_next);
    }
}

static void e2k_tr_disas_log(const DisasContextBase *db, CPUState *cpu)
{
    DisasContext *dc = container_of(db, DisasContext, base);

    qemu_log("IN: %s\n", lookup_symbol(dc->base.pc_first));
    log_target_disas(cpu, dc->base.pc_first, dc->base.tb->size);
}

static const TranslatorOps e2k_tr_ops = {
    .init_disas_context = e2k_tr_init_disas_context,
    .tb_start           = e2k_tr_tb_start,
    .insn_start         = e2k_tr_insn_start,
    .translate_insn     = e2k_tr_translate_insn,
    .tb_stop            = e2k_tr_tb_stop,
    .disas_log          = e2k_tr_disas_log,
};

void gen_intermediate_code(CPUState *cs, TranslationBlock *tb, int max_insns)
{
    DisasContext dc = { 0 };

    translator_loop(&e2k_tr_ops, &dc.base, cs, tb, max_insns);
}

void restore_state_to_opc(CPUE2KState *env, TranslationBlock *tb,
                          target_ulong *data)
{
    env->ip = data[0];
}

void e2k_tcg_initialize(void) {
    char buf[16] = { 0 };

    static const struct { TCGv_i32 *ptr; int off; const char *name; } r32[] = {
        { &cpu_wd_base, offsetof(CPUE2KState, wd.base), "woff" },
        { &cpu_wd_size, offsetof(CPUE2KState, wd.size), "wsize" },
        { &cpu_boff, offsetof(CPUE2KState, bn.base), "boff" },
        { &cpu_bsize, offsetof(CPUE2KState, bn.size), "bsize" },
        { &cpu_bcur, offsetof(CPUE2KState, bn.cur), "bcur" },
        { &cpu_psize, offsetof(CPUE2KState, bp.size), "psize" },
        { &cpu_pcur, offsetof(CPUE2KState, bp.cur), "pcur" },
        { &cpu_is_bp, offsetof(CPUE2KState, is_bp), "is_bp" },
        { &cpu_wdbl, offsetof(CPUE2KState, wdbl), "wdbl" },
        { &cpu_lsr_ecnt, offsetof(CPUE2KState, lsr_ecnt), "lsr_ecnt" },
        { &cpu_lsr_vlc, offsetof(CPUE2KState, lsr_vlc), "lsr_vlc" },
        { &cpu_lsr_over, offsetof(CPUE2KState, lsr_over), "lsr_over" },
        { &cpu_lsr_pcnt, offsetof(CPUE2KState, lsr_pcnt), "lsr_pcnt" },
        { &cpu_lsr_strmd, offsetof(CPUE2KState, lsr_strmd), "lsr_strmd" },
        { &cpu_aasti_tags, offsetof(CPUE2KState, aau.sti_tags), "aasti_tags" },
        { &cpu_aaind_tags, offsetof(CPUE2KState, aau.ind_tags), "aaind_tags" },
        { &cpu_aaincr_tags, offsetof(CPUE2KState, aau.incr_tags), "aaincr_tags" },
        { &cpu_ct_cond, offsetof(CPUE2KState, ct_cond), "cond" },
    };

    static const struct { TCGv_i64 *ptr; int off; const char *name; } r64[] = {
        { &cpu_pregs, offsetof(CPUE2KState, pregs), "pregs" },
        { &cpu_last_val0, offsetof(CPUE2KState, last_val0), "last_val0" },
        { &cpu_last_val1, offsetof(CPUE2KState, last_val1), "last_val1" },
        { &cpu_lsr_lcnt, offsetof(CPUE2KState, lsr_lcnt), "lsr_lcnt" },
    };

    static const struct { TCGv *ptr; int off; const char *name; } rtl[] = {
        { &cpu_pc, offsetof(CPUE2KState, ip), "pc" },
        { &cpu_npc, offsetof(CPUE2KState, nip), "npc" },
    };

    unsigned int i;

    for (i = 0; i < ARRAY_SIZE(r32); i++) {
        *r32[i].ptr = tcg_global_mem_new_i32(cpu_env, r32[i].off, r32[i].name);
    }

    for (i = 0; i < ARRAY_SIZE(r64); i++) {
        *r64[i].ptr = tcg_global_mem_new_i64(cpu_env, r64[i].off, r64[i].name);
    }

    for (i = 0; i < ARRAY_SIZE(rtl); i++) {
        *rtl[i].ptr = tcg_global_mem_new(cpu_env, rtl[i].off, rtl[i].name);
    }

    for (i = 0; i < 3; i++) {
        snprintf(buf, ARRAY_SIZE(buf), "%%ctpr%d", i + 1);
        cpu_ctprs[i] = tcg_global_mem_new_i64(cpu_env,
            offsetof(CPUE2KState, ctprs[i].raw), buf);
    }

    for (i = 0; i < 16; i++) {
        snprintf(buf, ARRAY_SIZE(buf), "%%aasti%d", i);
        cpu_aasti[i] = tcg_global_mem_new_i32(cpu_env,
            offsetof(CPUE2KState, aau.stis[i]), buf);
    }

    for (i = 0; i < 16; i++) {
        snprintf(buf, ARRAY_SIZE(buf), "%%aaind%d", i);
        cpu_aaind[i] = tcg_global_mem_new_i32(cpu_env,
            offsetof(CPUE2KState, aau.inds[i]), buf);
    }

    for (i = 0; i < 7; i++) {
        snprintf(buf, ARRAY_SIZE(buf), "%%aaincr%d", i);
        cpu_aaincr[i] = tcg_global_mem_new_i32(cpu_env,
            offsetof(CPUE2KState, aau.incrs[i]), buf);
    }

    for (i = 0; i < 32; i++) {
        snprintf(buf, ARRAY_SIZE(buf), "%%aad%d_lo", i);
        cpu_aad_lo[i] = tcg_global_mem_new_i64(cpu_env,
            offsetof(CPUE2KState, aau.ds[i].lo), buf);
        snprintf(buf, ARRAY_SIZE(buf), "%%aad%d_hi", i);
        cpu_aad_hi[i] = tcg_global_mem_new_i64(cpu_env,
            offsetof(CPUE2KState, aau.ds[i].hi), buf);
    }
}
