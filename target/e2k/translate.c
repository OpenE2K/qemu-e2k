#include "qemu/osdep.h"
#include "qemu.h"
#include "exec/log.h"
#include "exec/translator.h"
#include "tcg/tcg-op.h"
#include "tcg/tcg-op-gvec.h"

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

#define GET_BASED(i) ((i) & 0x7f)
#define GET_REGULAR(i) ((i) & 0x3f)
#define GET_IMM5(i) ((i) & 0x1f)
#define GET_IMM4(i) ((i) & 0x0f)
#define GET_LIT(i) ((i) & 0x03)
#define GET_GLOBAL(i) ((i) & 0x1f)

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

#define gen_tag3x(r, a, b, c) gen_tag3_i64(r.tag, a.tag, b.tag, c.tag)
#define gen_tag3d(r, a, b, c) gen_tag3_i64(r.tag, a.tag, b.tag, c.tag)
#define gen_tag3s(r, a, b, c) gen_tag3_i32(r.tag, a.tag, b.tag, c.tag)
#define gen_tag3(s, r, a, b, c) glue(gen_tag3, s)(r, a, b, c)

#define gen_tag2x(r, a, b) gen_tag2_i64(r.tag, a.tag, b.tag)
#define gen_tag2d(r, a, b) gen_tag2_i64(r.tag, a.tag, b.tag)
#define gen_tag2s(r, a, b) gen_tag2_i32(r.tag, a.tag, b.tag)
#define gen_tag2(s, r, a, b) glue(gen_tag2, s)(r, a, b)

#define gen_tag1x(r, a) gen_tag1_i64(r.tag, a.tag)
#define gen_tag1d(r, a) gen_tag1_i64(r.tag, a.tag)
#define gen_tag1s(r, a) gen_tag1_i32(r.tag, a.tag)
#define gen_tag1(s, r, a) glue(gen_tag1, s)(r, a)

#define gen_result_init_s(i, r)
#define gen_result_init_d(i, r)
#define gen_result_init_x(i, r) gen_temp_result_ptr(r.val, instr->chan)
#define gen_result_init(R, instr, r) glue(gen_result_init_, R)(instr, r)

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
static TCGv_i64 cpu_last_value;
/* lsr */
static TCGv_i32 cpu_lsr_lcnt;
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
    ALOPF1_MERGE,
    ALOPF2,
    ALOPF3,
    ALOPF7,
    ALOPF8,
    ALOPF10,
    ALOPF11,
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
    ALOPF21_LCOMB,
    ALOPF22,
} Alopf;

typedef struct {
    Alopf format;
    uint32_t op;
    const char *name;
} Alop;

typedef struct {
    Cs0 cs0;
    Cs1 cs1;
    Alop alops[6];
} Bundle;

typedef enum {
    AL_RESULT_NONE = 0,

    AL_RESULT_SIZE_MASK = 0x3,
    AL_RESULT_32 = 0x00,
    AL_RESULT_64 = 0x01,
    AL_RESULT_80 = 0x02,
    AL_RESULT_128 = 0x03,

    AL_RESULT_TYPE_MASK = 0xc,
    AL_RESULT_REG = 0x04,
    AL_RESULT_PREG = 0x08,
    AL_RESULT_CTPR = 0x0c,

    AL_RESULT_REG32 = AL_RESULT_REG | AL_RESULT_32,
    AL_RESULT_REG64 = AL_RESULT_REG | AL_RESULT_64,
    AL_RESULT_REG80 = AL_RESULT_REG | AL_RESULT_80,
    AL_RESULT_REG128 = AL_RESULT_REG | AL_RESULT_128,
    AL_RESULT_CTPR32 = AL_RESULT_CTPR | AL_RESULT_32,
    AL_RESULT_CTPR64 = AL_RESULT_CTPR | AL_RESULT_64,
} AlResultType;

#define e2k_al_result_size(x) ((x) & AL_RESULT_SIZE_MASK)
#define e2k_al_result_type(x) ((x) & AL_RESULT_TYPE_MASK)

typedef struct {
    AlResultType type;
    union {
        struct {
            TCGv_i32 index;
            TCGv_i32 tag;
            union {
                TCGv_i32 v32;
                TCGv_i64 v64;
            };
            union {
                TCGv_i32 x32; /* FX ops */
                TCGv_i64 x64; /* SIMD ops v5+ */
            };
        } reg;
        struct {
            int index;
            union {
                TCGv_i32 v32;
                TCGv_i64 v64;
            };
        } ctpr;
        struct {
            int index;
            TCGv_i32 val;
        } preg;
    };
} AlResult;

typedef struct {
    bool is_set;
    TCGv_i32 index;
    TCGv_i32 tag;
    TCGv_i64 value;
} AauResult;

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

typedef struct DisasContext {
    DisasContextBase base;
    UnpackedBundle bundle;
    Bundle bundle2;
    target_ulong pc;
    int jump_ctpr;
    int mmuidx;
    uint8_t mas[6];
    bool loop_mode;
    TCGv_i32 is_epilogue;
    /* optional, can be NULL */
    TCGv_i32 mlock;

    int version;
    /* Force ILLOP for bad instruction format for cases where real CPU
       do not generate it. */
    bool strict;

    // Temporary values.
    TCGv_i32 t32[64];
    TCGv_i64 t64[32];
    // Allocated temporary values count.
    int t32_len;
    int t64_len;

    /* Delayed illegal tag check */
    TCGv_i32 illtag;
    bool do_check_illtag;

    /* Delayed window bounds check */
    int wd_size;
    int max_r;
    int max_r_src;
    int max_r_dst;
    int bsize;
    int max_b;
    int max_b_cur;

    TCGv_i64 cond[6];
    AlResult al_results[6];
    TCGv_i32 al_cond[6];
    AauResult aau_results[4];
    int aau_am[4];
    PlResult pl_results[3];
    ControlTransfer ct;
} DisasContext;

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

typedef struct {
    DisasContext *ctx;
    int chan;
    AlesFlag ales_present;
    int aaincr_len;
    uint8_t mas;
    union {
        uint32_t als;
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
    };
    union {
        uint16_t ales;
        struct {
            uint16_t src3: 8;
            uint16_t opc2: 8;
        };
        struct {
            uint16_t opce3: 8;
            uint16_t unused5: 8;
        };
    };
} Instr;

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

#include "alops.inc"

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

static inline TCGv_i32 e2k_get_temp_i32(DisasContext *dc)
{
    assert(dc->t32_len < ARRAY_SIZE(dc->t32));
    return dc->t32[dc->t32_len++] = tcg_temp_local_new_i32();
}

static inline TCGv_i32 e2k_get_const_i32(DisasContext *dc, uint32_t value)
{
    assert(dc->t32_len < ARRAY_SIZE(dc->t32));
    return dc->t32[dc->t32_len++] = tcg_const_local_i32(value);
}

static inline TCGv_i64 e2k_get_temp_i64(DisasContext *dc)
{
    assert(dc->t64_len < ARRAY_SIZE(dc->t64));
    return dc->t64[dc->t64_len++] = tcg_temp_local_new_i64();
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

static inline bool use_goto_tb(DisasContext *s, target_ulong pc,
                               target_ulong npc)
{
    if (unlikely(s->base.singlestep_enabled || singlestep)) {
        return false;
    }

#ifndef CONFIG_USER_ONLY
    return (pc & TARGET_PAGE_MASK) == (s->base.tb->pc & TARGET_PAGE_MASK) &&
           (npc & TARGET_PAGE_MASK) == (s->base.tb->pc & TARGET_PAGE_MASK);
#else
    return true;
#endif
}

static inline void gen_goto_tb(DisasContext *ctx, int tb_num,
    target_ulong pc, target_ulong npc)
{
    if (use_goto_tb(ctx, pc, npc))  {
        /* jump to same page: we can use a direct jump */
        tcg_gen_goto_tb(tb_num);
        tcg_gen_movi_tl(cpu_pc, npc);
        tcg_gen_exit_tb(ctx->base.tb, tb_num);
    } else {
        /* jump to another page: currently not optimized */
        tcg_gen_movi_tl(cpu_pc, npc);
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

    Bundle *bundle = &ctx->bundle2;
    Cs1 *ret = &bundle->cs1;
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
    Bundle *bundle = &ctx->bundle2;
    Cs0 *ret = &bundle->cs0;
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
            || bundle->cs1.type == CS1_CALL
            || bundle->cs1.type == CS1_HCALL)
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
    tcg_gen_setcondi_i32(TCG_COND_EQ, ret, cpu_lsr_lcnt, 0);
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

static inline TCGv_i64 get_temp_i64(Instr *instr)
{
    return e2k_get_temp_i64(instr->ctx);
}

static inline TCGv_i32 get_temp_i32(Instr *instr)
{
    return e2k_get_temp_i32(instr->ctx);
}

static inline void gen_ptr_from_index(TCGv_ptr ret, TCGv_ptr ptr, TCGv_i32 idx,
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

static inline void gen_preg_index(TCGv_i32 ret, int idx)
{
    TCGv_i32 i = tcg_const_i32(idx);
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i32 t1 = tcg_temp_new_i32();
    TCGv_i32 t2 = tcg_temp_new_i32();
    TCGv_i32 t3 = tcg_temp_new_i32();

    assert(idx < 32);

    tcg_gen_addi_i32(t0, cpu_psize, 1);
    tcg_gen_addi_i32(t1, cpu_pcur, idx);
    tcg_gen_remu_i32(t2, t1, t0);
    tcg_gen_movi_i32(t3, idx);
    tcg_gen_movcond_i32(TCG_COND_LEU, ret, i, cpu_psize, t2, t3);

    tcg_temp_free_i32(t3);
    tcg_temp_free_i32(t2);
    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
    tcg_temp_free_i32(i);
}

static void gen_preg_offset(TCGv_i64 ret, int idx)
{
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i32 t1 = tcg_temp_new_i32();

    gen_preg_index(t0, idx);
    tcg_gen_muli_i32(t1, t0, 2);
    tcg_gen_extu_i32_i64(ret, t1);

    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
}

static void gen_preg_clear(TCGv_i64 ret, TCGv_i64 offset)
{
    TCGv_i64 t0 = tcg_const_i64(3);
    TCGv_i64 t1 = tcg_temp_new_i64();
    TCGv_i64 t2 = tcg_temp_new_i64();

    tcg_gen_shl_i64(t1, t0, offset);
    tcg_gen_not_i64(t2, t1);
    tcg_gen_and_i64(ret, t2, cpu_pregs);

    tcg_temp_free_i64(t2);
    tcg_temp_free_i64(t1);
    tcg_temp_free_i64(t0);
}

static inline void gen_preg_i64(TCGv_i64 ret, int reg)
{
    TCGv_i64 one = tcg_const_i64(1);
    TCGv_i64 t0 = tcg_temp_new_i64();
    TCGv_i64 t1 = tcg_temp_new_i64();
    TCGv_i64 t2 = tcg_temp_new_i64();

    gen_preg_offset(t0, reg);
    tcg_gen_shl_i64(t1, one, t0);
    tcg_gen_and_i64(t2, cpu_pregs, t1);
    tcg_gen_setcondi_i64(TCG_COND_NE, ret, t2, 0);

    tcg_temp_free_i64(t2);
    tcg_temp_free_i64(t1);
    tcg_temp_free_i64(t0);
    tcg_temp_free_i64(one);
}

static void gen_preg_i32(TCGv_i32 ret, int reg)
{
    TCGv_i64 t0 = tcg_temp_new_i64();

    gen_preg_i64(t0, reg);
    tcg_gen_extrl_i64_i32(ret, t0);
    tcg_temp_free_i64(t0);
}

static void gen_preg_set_i32(int idx, TCGv_i32 val)
{
    TCGv_i64 t0 = tcg_temp_new_i64();
    TCGv_i64 t1 = tcg_temp_new_i64();
    TCGv_i64 t2 = tcg_temp_new_i64();
    TCGv_i64 t3 = tcg_temp_new_i64();
    TCGv_i64 t4 = tcg_temp_new_i64();

    gen_preg_offset(t0, idx);
    gen_preg_clear(t1, t0);
    tcg_gen_extu_i32_i64(t2, val);
    tcg_gen_andi_i64(t3, t2, 3);
    tcg_gen_shl_i64(t4, t2, t0);
    tcg_gen_or_i64(cpu_pregs, t1, t4);

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
    TCGv_ptr t0 = tcg_temp_new_ptr();

    gen_reg_tag_ptr(t0, index);

    if (is_tag_mask_all(ctx, mask)) {
        tcg_gen_st8_i32(tag, t0, 0);
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

static void gen_reg_index_i32(DisasContext *ctx, TCGv_i32 ret, uint8_t arg)
{
    if (IS_BASED(arg)) {
        TCGv_i32 t0 = tcg_temp_new_i32();
        TCGv_i32 t1 = tcg_temp_new_i32();

        tcg_gen_addi_i32(t0, cpu_bcur, GET_BASED(arg));
        tcg_gen_remu_i32(t1, t0, cpu_bsize);
        tcg_gen_add_i32(ret, cpu_boff, t1);

        tcg_temp_free_i32(t1);
        tcg_temp_free_i32(t0);
    } else if (IS_REGULAR(arg)) {
        tcg_gen_movi_i32(ret, GET_REGULAR(arg));
    } else if (IS_GLOBAL(arg)) {
        tcg_gen_movi_i32(ret, E2K_NR_COUNT + GET_GLOBAL(arg));
    } else {
        gen_tr_exception(ctx, EXCP_ILLEGAL_OPERAND);
    }
}

static void gen_reg_lo_ptr(TCGv_ptr ret, TCGv_i32 idx)
{
    TCGv_ptr t0 = tcg_temp_new_ptr();

    tcg_gen_addi_ptr(t0, cpu_env, offsetof(CPUE2KState, regs));
    gen_ptr_from_index(ret, t0, idx, sizeof(E2KReg));
    tcg_temp_free_ptr(t0);
}

static void gen_reg_hi_ptr(TCGv_ptr ret, TCGv_i32 idx)
{
    TCGv_ptr t0 = tcg_temp_new_ptr();

    gen_reg_lo_ptr(t0, idx);
    tcg_gen_addi_ptr(ret, t0, offsetof(E2KReg, hi));
    tcg_temp_free_ptr(t0);
}

#define GEN_REG_WRITE(name, ty, ptr_func, st_func) \
    static void name(ty value, TCGv_i32 idx) \
    { \
        TCGv_ptr t0 = tcg_temp_new_ptr(); \
        ptr_func(t0, idx); \
        st_func(value, t0, 0); \
        tcg_temp_free_ptr(t0); \
    }

GEN_REG_WRITE(gen_reg_lo_write_i64, TCGv_i64, gen_reg_lo_ptr, tcg_gen_st_i64)
GEN_REG_WRITE(gen_reg_lo_write_i32, TCGv_i32, gen_reg_lo_ptr, tcg_gen_st_i32)
GEN_REG_WRITE(gen_reg_hi_write_i64, TCGv_i64, gen_reg_hi_ptr, tcg_gen_st_i64)
GEN_REG_WRITE(gen_reg_hi_write16u_i32, TCGv_i32, gen_reg_hi_ptr, tcg_gen_st16_i32)

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
        lit = *(uint64_t *) &ctx->bundle.lts[i];
    } else {
        gen_tr_excp_illopc(ctx);
    }

    return lit;
}

static void gen_tagged_temp_ptr(TCGv_ptr ret, int i)
{
    tcg_gen_addi_ptr(ret, cpu_env, offsetof(CPUE2KState, tmp[i]));
}

static void gen_temp_result_ptr(TCGv_ptr ret, int i)
{
    tcg_gen_addi_ptr(ret, cpu_env, offsetof(CPUE2KState, al_result[i]));
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

static void gen_tagged_const_x(Tagged_ptr ret, uint8_t tag, int64_t val, int i)
{
    gen_tagged_temp_ptr(ret.val, i);
    gen_tagged_const_ptr(ret, tag, val);
}

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

static void gen_tagged_reg_x(DisasContext *ctx, Tagged_ptr ret, uint8_t arg)
{
    gen_tagged_reg_ptr(ctx, ret, arg, E2K_TAG_MASK_80);
}

static void gen_tagged_reg_d(DisasContext *ctx, Tagged_i64 ret, uint8_t arg)
{
    Tagged_ptr t0 = tagged_new_ptr();

    gen_tagged_reg_ptr(ctx, t0, arg, E2K_TAG_MASK_64);
    tcg_gen_mov_i32(ret.tag, t0.tag);
    tcg_gen_ld_i64(ret.val, t0.val, offsetof(E2KReg, lo));

    tagged_free_ptr(t0);
}

static void gen_tagged_reg_s(DisasContext *ctx, Tagged_i32 ret, uint8_t arg)
{
    Tagged_ptr t0 = tagged_new_ptr();

    gen_tagged_reg_ptr(ctx, t0, arg, E2K_TAG_MASK_32);
    tcg_gen_mov_i32(ret.tag, t0.tag);
    tcg_gen_ld_i32(ret.val, t0.val, offsetof(E2KReg, lo));

    tagged_free_ptr(t0);
}

#define IMPL_GEN_TAGGED_SRC1(S, T) \
    static void glue(gen_tagged_src1_, S)(Instr *instr, T ret) \
    { \
        if (IS_IMM5(instr->src1)) { \
            glue(gen_tagged_const_, S)(ret, 0, GET_IMM5(instr->src1), 1); \
        } else { \
            glue(gen_tagged_reg_, S)(instr->ctx, ret, instr->src1); \
        } \
    }

IMPL_GEN_TAGGED_SRC1(x, Tagged_ptr)
IMPL_GEN_TAGGED_SRC1(d, Tagged_i64)
IMPL_GEN_TAGGED_SRC1(s, Tagged_i32)

#define IMPL_GEN_TAGGED_SRC2(S, T) \
    static void glue(gen_tagged_src2_, S)(Instr *instr, T ret) \
    { \
        if (IS_IMM4(instr->src2)) { \
            glue(gen_tagged_const_, S)(ret, 0, GET_IMM4(instr->src2), 2); \
        } else if (IS_LIT(instr->src2)) { \
            int64_t lit = get_literal(instr->ctx, instr->src2); \
            \
            glue(gen_tagged_const_, S)(ret, 0, lit, 2); \
        } else { \
            glue(gen_tagged_reg_, S)(instr->ctx, ret, instr->src2); \
        } \
    }

IMPL_GEN_TAGGED_SRC2(x, Tagged_ptr)
IMPL_GEN_TAGGED_SRC2(d, Tagged_i64)
IMPL_GEN_TAGGED_SRC2(s, Tagged_i32)

#define IMPL_GEN_TAGGED_SRC_REG(N, S, T) \
    static void glue4(gen_tagged_src, N, _, S)(Instr *instr, T ret) \
    { \
        glue(gen_tagged_reg_, S)(instr->ctx, ret, glue(instr->src, N)); \
    }

IMPL_GEN_TAGGED_SRC_REG(3, d, Tagged_i64)
IMPL_GEN_TAGGED_SRC_REG(3, s, Tagged_i32)

IMPL_GEN_TAGGED_SRC_REG(4, d, Tagged_i64)
IMPL_GEN_TAGGED_SRC_REG(4, s, Tagged_i32)

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

/*
 * Returns zero if all @args is zero otherwise returns tag of non 64-bit number.
 */
#define gen_tag3_i64(ret, a1, a2, a3) \
    gen_tag3_raw((ret), E2K_TAG_NON_NUMBER64, (a1), (a2), (a3))
#define gen_tag2_i64(ret, a1, a2) gen_tag3_i64((ret), (a1), (a2), NULL)
#define gen_tag1_i64(ret, a1) gen_tag2_i64((ret), (a1), NULL)

/*
 * Returns zero if all @args is zero otherwise returns tag of non 32-bit number.
 */
#define gen_tag3_i32(ret, a1, a2, a3) \
    gen_tag3_raw((ret), E2K_TAG_NON_NUMBER32, (a1), (a2), (a3))
#define gen_tag2_i32(ret, a1, a2) gen_tag3_i32((ret), (a1), (a2), NULL)
#define gen_tag1_i32(ret, a1) gen_tag2_i32((ret), (a1), NULL)

static inline void gen_tag_check(Instr *instr, TCGv_i32 tag)
{
    if (!instr->sm && tag != NULL) {
        instr->ctx->do_check_illtag = true;
        TCGv_i32 illtag = instr->ctx->illtag;
        tcg_gen_or_i32(illtag, illtag, tag);
    }
}

static inline void gen_dst_poison_i64(TCGv_i64 ret, TCGv_i64 value,
    TCGv_i32 tag)
{
    TCGv_i64 zero = tcg_const_i64(0);
    TCGv_i64 t0 = tcg_temp_new_i64();
    TCGv_i64 t1 = tcg_temp_new_i64();

    tcg_gen_ori_i64(t0, value, (1UL << 62) | (1 << 30));
    tcg_gen_extu_i32_i64(t1, tag);
    tcg_gen_movcond_i64(TCG_COND_NE, ret, t1, zero, t0, value);

    tcg_temp_free_i64(t1);
    tcg_temp_free_i64(t0);
    tcg_temp_free_i64(zero);
}

static inline void gen_dst_poison_i32(TCGv_i32 ret, TCGv_i32 value,
    TCGv_i32 tag)
{
    TCGv_i32 zero = tcg_const_i32(0);
    TCGv_i32 t0 = tcg_temp_new_i32();

    tcg_gen_ori_i32(t0, value, 1 << 30);
    tcg_gen_movcond_i32(TCG_COND_NE, ret, tag, zero, t0, value);

    tcg_temp_free_i32(t0);
    tcg_temp_free_i32(zero);
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
            gen_preg_i32(ret, idx);
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
    int need[7] = { 0 };
    unsigned int i;
    TCGv_i32 lp[7];

    scan_needed(bundle, need);

    for (i = 0; i < 7; i++) {
        if (need[i]) {
            lp[i] = tcg_temp_new_i32();
        }
    }

    for (i = 0; i < 3; i++) {
        ctx->pl_results[i].reg = -1;

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
            int vdst = GET_BIT(clp, 5);
            int pdst = extract32(clp, 0, 5);

            // TODO: check clp arg
            // {C/M}LP0 0, 1             => 4
            // {C/M}LP1 0, 1, 2, 3, 4    => 5
            // {C/M}LP2 0, 1, 2, 3, 4, 5 => 6
            // maximal cascading is 2

            gen_get_lp(p0, clp, 10, lp);
            gen_get_lp(p1, clp, 6, lp);

            if (vdst) {
                ctx->pl_results[i].reg = pdst;
                ctx->pl_results[i].value = e2k_get_temp_i32(ctx);
            }

            switch (opc) {
            case 0: /* andp */
                // FIXME: what is the difference between `andp` and `landp`?
            case 1: /* landp */
                tcg_gen_and_i32(lp[4 + i], p0, p1);
                if (vdst) {
                    tcg_gen_mov_i32(ctx->pl_results[i].value, lp[4 + i]);
                }
                break;
            case 3: { /* movep */
                // FIXME: clp cannot read result of movep???
                tcg_gen_and_i32(lp[4 + i], p0, p1);

                if (vdst) {
                    TCGv_i32 z = tcg_const_i32(0);
                    TCGv_i32 t0 = tcg_temp_new_i32();

                    gen_preg_i32(t0, pdst);
                    tcg_gen_movcond_i32(TCG_COND_NE, ctx->pl_results[i].value,
                        p0, z, p1, t0);

                    tcg_temp_free_i32(t0);
                    tcg_temp_free_i32(z);
                }
                break;
            }
            default:
                abort();
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

static void gen_plu_commit(DisasContext *ctx)
{
    unsigned int i;

    for (i = 0; i < 3; i++) {
        if (ctx->pl_results[i].reg < 0) {
            continue;
        }
        gen_preg_set_i32(ctx->pl_results[i].reg, ctx->pl_results[i].value);
    }
}

static inline void set_al_result_reg80_tag(Instr *instr, TCGv_i64 lo,
    TCGv_i32 hi, TCGv_i32 tag)
{
    uint8_t dst = instr->dst;
    AlResult *res = &instr->ctx->al_results[instr->chan];

    if (dst == 0xdf) {
        res->type = AL_RESULT_NONE;
    } else {
        res->type = AL_RESULT_REG80;
        res->reg.tag = tag;
        res->reg.v64 = lo;
        res->reg.x32 = hi;
        res->reg.index = get_temp_i32(instr);
        gen_reg_index_i32(instr->ctx, res->reg.index, dst);
    }
}

static inline void set_al_result_reg64_tag(Instr *instr,
    TCGv_i64 value, TCGv_i32 tag)
{
    uint8_t arg = instr->dst;
    AlResult *res = &instr->ctx->al_results[instr->chan];

    // TODO: %tst, %tc, %tcd
    if (arg == 0xdf) { /* %empty */
        res->type = AL_RESULT_NONE;
        res->reg.index = NULL;
        res->reg.v64 = NULL;
        res->reg.tag = NULL;
    } else if ((arg & 0xfc) == 0xd0 && (arg & 3) != 0) {
        // TODO: check if instruction can write to ctpr
        // TODO: check tag
        res->type = AL_RESULT_CTPR64;
        res->ctpr.index = (arg & 3) - 1;
        res->ctpr.v64 = value;
    } else {
        res->type = AL_RESULT_REG64;
        res->reg.v64 = value;
        res->reg.tag = tag;
        res->reg.index = e2k_get_temp_i32(instr->ctx);
        gen_reg_index_i32(instr->ctx, res->reg.index, arg);
    }
}

static inline void set_al_result_reg64(Instr *instr, TCGv_i64 value)
{
    TCGv_i32 tag = e2k_get_const_i32(instr->ctx, 0);
    set_al_result_reg64_tag(instr, value, tag);
}

static inline void set_al_result_reg32_tag(Instr *instr,
    TCGv_i32 value, TCGv_i32 tag)
{
    uint8_t arg = instr->dst;
    AlResult *res = &instr->ctx->al_results[instr->chan];

    // TODO: %tst, %tc, %tcd
    if (arg == 0xdf) { /* %empty */
        res->type = AL_RESULT_NONE;
    } else if ((arg & 0xfc) == 0xd0 && (arg & 3) != 0) {
        // TODO: check if instruction can write to ctpr
        res->type = AL_RESULT_CTPR32;
        res->ctpr.index = (arg & 3) - 1;
        res->ctpr.v32 = value;
    } else {
        res->type = AL_RESULT_REG32;
        res->reg.v32 = value;
        res->reg.tag = tag;
        res->reg.index = e2k_get_temp_i32(instr->ctx);
        gen_reg_index_i32(instr->ctx, res->reg.index, arg);
    }
}

static inline void set_al_result_preg(Instr *instr, int index, TCGv_i32 value)
{
    AlResult *res = &instr->ctx->al_results[instr->chan];

    res->type = AL_RESULT_PREG;
    res->preg.index = index;
    res->preg.val = value;
}

static inline void gen_al_result_i80(Instr *instr, TCGv_i64 lo, TCGv_i32 hi,
    TCGv_i32 tag)
{
    set_al_result_reg80_tag(instr, lo, hi, tag);
}

static inline void gen_al_result_i64(Instr *instr, TCGv_i64 dst, TCGv_i32 tag)
{
    set_al_result_reg64_tag(instr, dst, tag);
}

static inline void gen_al_result_i32(Instr *instr, TCGv_i32 dst, TCGv_i32 tag)
{
    set_al_result_reg32_tag(instr, dst, tag);
}

static void gen_al_result_x(Instr *instr, Tagged_ptr result)
{
    TCGv_i32 tag = get_temp_i32(instr);
    TCGv_i64 lo = get_temp_i64(instr);
    TCGv_i32 hi = get_temp_i32(instr);

    tcg_gen_mov_i32(tag, result.tag);
    tcg_gen_ld_i64(lo, result.val, offsetof(E2KReg, lo));
    tcg_gen_ld16u_i32(hi, result.val, offsetof(E2KReg, hi));
    gen_al_result_i80(instr, lo, hi, tag);
}

static void gen_al_result_d(Instr *instr, Tagged_i64 result)
{
    TCGv_i32 tag = get_temp_i32(instr);
    TCGv_i64 val = get_temp_i64(instr);

    tcg_gen_mov_i32(tag, result.tag);
    tcg_gen_mov_i64(val, result.val);
    gen_al_result_i64(instr, val, tag);
}

static void gen_al_result_s(Instr *instr, Tagged_i32 result)
{
    TCGv_i32 tag = get_temp_i32(instr);
    TCGv_i32 val = get_temp_i32(instr);

    tcg_gen_mov_i32(tag, result.tag);
    tcg_gen_mov_i32(val, result.val);
    gen_al_result_i32(instr, val, tag);
}

static void gen_al_result_b(Instr *instr, Tagged_i32 result)
{
    TCGv_i32 res = get_temp_i32(instr);
    TCGv_i32 t0 = tcg_const_i32(0);
    TCGv_i32 t1 = tcg_const_i32(2);

    tcg_gen_movcond_i32(TCG_COND_NE, res, result.tag, t0, t1, result.val);
    set_al_result_preg(instr, instr->dst_preg, res);

    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
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

static void gen_is_loop_end_i32(TCGv_i32 ret)
{
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i32 t1 = tcg_temp_new_i32();
    TCGv_i32 t2 = tcg_temp_new_i32();

    tcg_gen_setcondi_i32(TCG_COND_EQ, t0, cpu_lsr_ecnt, 0);
    tcg_gen_setcondi_i32(TCG_COND_LTU, t1, cpu_lsr_lcnt, 2);
    tcg_gen_and_i32(t2, t0, t1);
    tcg_gen_and_i32(ret, t2, cpu_lsr_vlc);

    tcg_temp_free_i32(t2);
    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
}

static void gen_loop_mode_st(DisasContext *ctx, TCGLabel *l)
{
    if (ctx->loop_mode) {
        TCGLabel *l0 = gen_new_label();
        TCGv_i32 t0 = tcg_temp_new_i32();
        tcg_gen_brcondi_i32(TCG_COND_NE, cpu_lsr_pcnt, 0, l);
        gen_is_loop_end_i32(t0);
        tcg_gen_brcondi_i32(TCG_COND_EQ, t0, 0, l0);
        tcg_gen_brcondi_i32(TCG_COND_EQ, cpu_lsr_strmd, 0, l);
        tcg_gen_subi_i32(cpu_lsr_strmd, cpu_lsr_strmd, 1);
        gen_set_label(l0);
        tcg_temp_free_i32(t0);
    }
}

#define IMPL_GEN_OPN(name, S, op) \
    static inline void name(glue(TCGv_i, S) ret, glue(TCGv_i, S) src1, \
        glue(TCGv_i, S) src2) \
    { \
        glue(tcg_gen_not_i, S)(ret, src2); \
        op(ret, src1, ret); \
    }

IMPL_GEN_OPN(gen_andn_i32, 32, tcg_gen_and_i32)
IMPL_GEN_OPN(gen_andn_i64, 64, tcg_gen_and_i64)
IMPL_GEN_OPN(gen_orn_i32, 32, tcg_gen_or_i32)
IMPL_GEN_OPN(gen_orn_i64, 64, tcg_gen_or_i64)
IMPL_GEN_OPN(gen_xorn_i32, 32, tcg_gen_xor_i32)
IMPL_GEN_OPN(gen_xorn_i64, 64, tcg_gen_xor_i64)

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

IMPL_GEN_GETF(gen_getf_i64, i64, TCGv_i64, 6, 6, 3, 64)
IMPL_GEN_GETF(gen_getf_i32, i32, TCGv_i32, 5, 5, 2, 32)

#define gen_getfd gen_getf_i64
#define gen_getfs gen_getf_i32

#define IMPL_GEN_BSWAP_MASK(name, S, M) \
    static void name(temp(S) ret, temp(S) arg, int shift, M mask) \
    { \
        temp(S) t0 = temp_new(S); \
        temp(S) t1 = temp_new(S); \
        \
        call(S, tcg_gen_andi, t0, arg, mask); \
        call(S, tcg_gen_andi, t1, arg, mask >> shift); \
        call(S, tcg_gen_shri, t0, t0, shift); \
        call(S, tcg_gen_shli, t1, t1, shift); \
        call(S, tcg_gen_or, ret, t0, t1); \
        \
        temp_free(S, t1); \
        temp_free(S, t0); \
    }

IMPL_GEN_BSWAP_MASK(gen_bswap_mask_i64, d, uint64_t)
IMPL_GEN_BSWAP_MASK(gen_bswap_mask_i32, s, uint32_t)

static void gen_bitrevs(TCGv_i32 ret, TCGv_i32 src1) {
    tcg_gen_bswap32_i32(ret, src1);
    gen_bswap_mask_i32(ret, ret, 4, 0xf0f0f0f0);
    gen_bswap_mask_i32(ret, ret, 2, 0xcccccccc);
    gen_bswap_mask_i32(ret, ret, 1, 0xaaaaaaaa);
}

static void gen_bitrevd(TCGv_i64 ret, TCGv_i64 src1) {
    tcg_gen_bswap64_i64(ret, src1);
    gen_bswap_mask_i64(ret, ret, 4, 0xf0f0f0f0f0f0f0f0);
    gen_bswap_mask_i64(ret, ret, 2, 0xcccccccccccccccc);
    gen_bswap_mask_i64(ret, ret, 1, 0xaaaaaaaaaaaaaaaa);
}

static void gen_lzcnts(TCGv_i32 ret, TCGv_i32 src1) {
    tcg_gen_clzi_i32(ret, src1, 32);
}

static void gen_lzcntd(TCGv_i64 ret, TCGv_i64 src1) {
    tcg_gen_clzi_i64(ret, src1, 64);
}

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
    static void name(Instr *instr) \
    { \
        tagged(S) r = tagged_new(S); \
        tagged(S) a = tagged_new(S); \
        tagged(S) b = tagged_new(S); \
        TCGv_i32 t0 = tcg_temp_new_i32(); \
        \
        gen_tagged_src(1, S, instr, a); \
        gen_tagged_src(2, S, instr, b); \
        \
        gen_mrgc_i32(instr->ctx, instr->chan, t0); \
        gen_merge_i32(r.tag, a.tag, b.tag, t0); \
        call(S, gen_tag1, r.tag, r.tag); \
        call(S, gen_merge, r.val, a.val, b.val, t0); \
        gen_al_result(S, instr, r); \
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
    static void name(Instr *instr) \
    { \
        tagged(S) r = tagged_new(S); \
        tagged(S) b = tagged_new(S); \
        \
        gen_tagged_src(2, S, instr, b); \
        tcg_gen_movi_i32(r.tag, 0); \
        ext(r.val, b.tag); \
        gen_al_result(S, instr, r); \
        \
        tagged_free(S, b); \
        tagged_free(S, r); \
    }

IMPL_GEN_GETTAG(gen_gettagd, d, tcg_gen_extu_i32_i64)
IMPL_GEN_GETTAG(gen_gettags, s, tcg_gen_mov_i32)

#define IMPL_GEN_PUTTAG(name, R, A, B) \
    static void name(Instr *instr) \
    { \
        TCGLabel *l0 = gen_new_label(); \
        TCGLabel *l1 = gen_new_label(); \
        tagged(R) r = tagged_local_new(R); \
        tagged(A) a = tagged_local_new(A); \
        tagged(B) b = tagged_local_new(B); \
        \
        gen_tagged_src(1, A, instr, a); \
        gen_tagged_src(2, B, instr, b); \
        call(R, tcg_gen_mov, r.val, a.val); \
        tcg_gen_brcondi_i32(TCG_COND_EQ, b.val, 0, l0); \
        gen_tag2(R, r, a, b); \
        tcg_gen_brcondi_i32(TCG_COND_EQ, r.tag, 0, l0); \
        gen_tag_check(instr, r.tag); \
        call(R, gen_dst_poison, r.val, r.val, r.tag); \
        tcg_gen_br(l1); \
        gen_set_label(l0); \
        tcg_gen_mov_i32(r.tag, b.val); \
        gen_set_label(l1); \
        gen_al_result(R, instr, r); \
        \
        tagged_free(B, b); \
        tagged_free(A, a); \
        tagged_free(R, r); \
    }

IMPL_GEN_PUTTAG(gen_puttagd, d, d, s)
IMPL_GEN_PUTTAG(gen_puttags, s, s, s)

static inline void gen_insfd_tag_mask(TCGv_i32 ret, TCGv_i32 flags,
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

static inline void gen_insfd_tag(TCGv_i32 ret, TCGv_i64 value,
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

static inline void gen_insfd(Instr *instr)
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

    gen_tagged_src1_d(instr, a);
    gen_tagged_src2_d(instr, b);
    gen_tagged_src3_d(instr, c);
    tcg_gen_extract_i64(offset, b.val, 0, 6);
    tcg_gen_extract_i64(len, b.val, 6, 6);
    gen_mask_i64(t0, len);
    tcg_gen_rotr_i64(t1, a.val, offset);
    gen_andn_i64(t2, t1, t0);
    tcg_gen_and_i64(t3, c.val, t0);
    tcg_gen_or_i64(r.val, t2, t3);

    gen_insfd_tag(r.tag, b.val, a.tag, c.tag);
    gen_tag2_i64(r.tag, b.tag, r.tag);
    gen_al_result_d(instr, r);

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

static inline void gen_insfs(TCGv_i32 ret, TCGv_i32 src1,
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

static inline void gen_rrd(Instr *instr)
{
    TCGv_i64 dst = get_temp_i64(instr);
    TCGv_i32 t0 = tcg_const_i32(instr->src1);

    gen_save_cpu_state(instr->ctx);
    gen_helper_rrd(dst, cpu_env, t0);
    set_al_result_reg64(instr, dst);
    tcg_temp_free_i32(t0);
}

#define IMPL_GEN_RW(name, S, helper) \
    static inline void name(Instr *instr) \
    { \
        tagged(S) b = tagged_new(S); \
        TCGv_i32 t0 = tcg_const_i32(instr->dst); \
        \
        gen_tagged_src(2, S, instr, b); \
        gen_tag_check(instr, b.tag); \
        helper(cpu_env, t0, b.val); \
        \
        tcg_temp_free_i32(t0); \
        tagged_free(S, b); \
    }

IMPL_GEN_RW(gen_rwd, d, gen_helper_rwd)
IMPL_GEN_RW(gen_rws, s, gen_helper_rws)

#define IMPL_GEN_MOV(name, S, code) \
    static void name(Instr *instr) \
    { \
        tagged(S) b = tagged_new(S); \
        \
        gen_tagged_src(2, S, instr, b); \
        { code; } \
        gen_al_result(S, instr, b); \
        \
        tagged_free(S, b); \
    }

static void gen_qpair_check(Instr *instr)
{
    if (!check_qr(instr->src2, instr->chan)
        || !check_qr(instr->dst, instr->chan))
    {
        gen_tr_excp_illopc(instr->ctx);
    }
}

IMPL_GEN_MOV(gen_movts,  s, { })
IMPL_GEN_MOV(gen_movtd,  d, { })
IMPL_GEN_MOV(gen_movtq,  d, { gen_qpair_check(instr); })
IMPL_GEN_MOV(gen_movtcs, s, { })
IMPL_GEN_MOV(gen_movtcd, d, { })
IMPL_GEN_MOV(gen_movtcq, d, { gen_qpair_check(instr); })

static void gen_getpl(Instr *instr)
{
#ifdef TARGET_E2K32
    Tagged_i64 r = tagged_new_i64();
    Tagged_i32 b = tagged_new_i32();

    // TODO: CUD
    gen_tagged_src2_s(instr, b);
    gen_tag1_i64(r.tag, b.tag);
    tcg_gen_extu_i32_i64(r.val, b.val);
    gen_al_result_d(instr, r);

    tagged_free_i32(b);
    tagged_free_i64(r);
#else /* !TARGET_E2K32 */
    // TODO: getpl 64-bit
    e2k_todo_illop(instr->ctx, "getpl");
#endif
}

static inline bool gen_ld_mas_mod(DisasContext *ctx, Instr *instr, uint8_t mod)
{
    switch (mod) {
    case 3:
        if (is_chan_25(instr->chan)) {
            // TODO: DAM
            /* always go to fixing code */
            if (ctx->mlock) {
                tcg_gen_movi_i32(ctx->mlock, 1);
            }
            return true;
        }
        break;
    case 4:
        if (instr->sm && is_chan_03(instr->chan)) {
            // TODO: DAM
            /* always ignore lock load */
            return false;
        } else if (!instr->sm && (is_chan_25(instr->chan) || is_chan_03(instr->chan))) {
            // TODO
            return true;
        }
        break;
    }

    e2k_todo(ctx, "opc %#x, chan %d, mod=%#x", instr->opc1, instr->chan, mod);
    return true;
}

static MemOp gen_mas(Instr *instr, MemOp memop)
{
    DisasContext *ctx = instr->ctx;
    uint8_t mas = instr->mas;

    if ((mas & 0x7) == 7) {
        int opc = mas >> 3;
        // TODO: special mas
        switch (opc) {
        case 0:
            /* handled in a gen_ld_mas */
            // TODO: only chan 0?
            return memop | MO_LE;
        case 3:
            if (!instr->sm && is_chan_25(instr->chan)) {
                // TODO: unknown store MAS OPC 0x3
                return 0;
            }
            break;
        }
        e2k_todo(ctx, "opc %#x, chan %d, mas=%#x (opc %#x)", instr->opc1,
            instr->chan, mas, opc);
        return 0;
    } else if (mas) {
        int mod = extract8(mas, 0, 3);
//        int dc = extract8(mas, 5, 2);

        if (mod != 0) {
            if (0x64 <= instr->opc1 && instr->opc1 < 0x68) {
                if (!gen_ld_mas_mod(ctx, instr, mod)) {
                    return 0;
                }
            } else {
                switch (mod) {
                case 2:
                    /* handled in a gen_st */
                    // TODO: only chan 2?
                    break;
                default:
                    e2k_todo(ctx, "opc %#x, chan %d, mas=%#x, mod=%#x", instr->opc1,
                        instr->chan, mas, mod);
                    break;
                }
            }
        }

        memop |= GET_BIT(mas, 3) ? MO_BE : MO_LE;
        memop |= GET_BIT(mas, 4) ? MO_UNALN : MO_ALIGN;
    } else {
        memop |= MO_LE | MO_ALIGN;
    }

    return memop;
}

static void gen_ld_tl(Instr *instr, TCGv_i64 ret, TCGv_i32 ret_tag, TCGv addr,
    MemOp memop)
{
    if (instr->sm) {
        TCGLabel *l0 = gen_new_label();
        TCGLabel *l1 = gen_new_label();
        TCGv t0 = tcg_temp_local_new();
        TCGv_i32 t1 = tcg_temp_new_i32();

        tcg_gen_mov_tl(t0, addr);
        gen_helper_probe_read_access(t1, cpu_env, t0);
        tcg_gen_brcondi_i32(TCG_COND_EQ, t1, 1, l0);

        /* address is not available */
        tcg_gen_movi_i32(ret_tag, E2K_TAG_NON_NUMBER64);
        tcg_gen_movi_i64(ret, E2K_LD_RESULT_INVALID);
        tcg_gen_br(l1);

        /* address is available */
        gen_set_label(l0);
        tcg_gen_qemu_ld_i64(ret, t0, instr->ctx->mmuidx, memop);

        gen_set_label(l1);

        tcg_temp_free_i32(t1);
        tcg_temp_free(t0);
    } else {
        tcg_gen_qemu_ld_i64(ret, addr, instr->ctx->mmuidx, memop);
    }
}

#define IMPL_GEN_LD(name, S, cast) \
    static void name(Instr *instr, Tagged_i64 r, \
        MemOp memop) \
    { \
        tagged(S) a = tagged_new(S); \
        tagged(S) b = tagged_new(S); \
        temp(S) t0 = temp_new(S); \
        TCGv t1 = tcg_temp_local_new(); \
        \
        gen_tagged_src(1, S, instr, a); \
        gen_tagged_src(2, S, instr, b); \
        gen_tag2d(r, a, b); \
        call(S, tcg_gen_add, t0, a.val, b.val); \
        cast(t1, t0); \
        gen_ld_tl(instr, r.val, r.tag, t1, memop); \
        \
        tcg_temp_free(t1); \
        temp_free(S, t0); \
        tagged_free(S, b); \
        tagged_free(S, a); \
    }

IMPL_GEN_LD(gen_ld_i64, d, tcg_gen_trunc_i64_tl)

#define IMPL_GEN_LD_MAS(name, load) \
    static void name(Instr *instr, MemOp memop) \
    { \
        Tagged_i64 r = tagged_local_new_i64(); \
        \
        memop = gen_mas(instr, memop); \
        \
        if (memop == 0) { \
            /* ignore load */ \
            tcg_gen_movi_i32(r.tag, E2K_TAG_NON_NUMBER64); \
            tcg_gen_movi_i64(r.val, E2K_LD_RESULT_INVALID); \
        } else { \
            load(instr, r, memop); \
            if (instr->chan == 0 && instr->mas == 7) { \
                /* TODO: gen illop if called twice before st/mod=2 */ \
                tcg_gen_mov_i64(cpu_last_value, r.val); \
            } \
        } \
        \
        gen_al_result_d(instr, r); \
        \
        tagged_free_i64(r); \
    }

IMPL_GEN_LD_MAS(gen_ld_mas_i64, gen_ld_i64)

#ifdef TARGET_E2K32
// TODO: ldgd ops must use GD.base
IMPL_GEN_LD(gen_ld_i32, s, tcg_gen_extu_i32_tl)
IMPL_GEN_LD_MAS(gen_ld_mas_i32, gen_ld_i32)
#endif

static void gen_atomic_cmpxchg_i64(Instr *instr, TCGv_i64 value, TCGv addr,
    MemOp memop)
{
    TCGv_i64 t0 = tcg_temp_new_i64();
    TCGv_i64 t1 = tcg_temp_new_i64();

    tcg_gen_atomic_cmpxchg_i64(t0, addr, cpu_last_value, value,
        instr->ctx->mmuidx, memop);
    tcg_gen_setcond_i64(TCG_COND_NE, t1, t0, cpu_last_value);
    tcg_gen_extrl_i64_i32(instr->ctx->mlock, t1);

    tcg_temp_free_i64(t1);
    tcg_temp_free_i64(t0);
}

static void gen_atomic_cmpxchg_i32(Instr *instr, TCGv_i32 value, TCGv addr,
    MemOp memop)
{
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i32 t1 = tcg_temp_new_i32();

    tcg_gen_extrl_i64_i32(t0, cpu_last_value);
    tcg_gen_atomic_cmpxchg_i32(t1, addr, t0, value,
        instr->ctx->mmuidx, memop);
    tcg_gen_setcond_i32(TCG_COND_NE, instr->ctx->mlock, t1, t0);

    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
}

#define IMPL_ST(NAME, S, N, cast) \
    static void NAME(Instr *instr, MemOp memop) \
    { \
        memop = gen_mas(instr, memop); \
        \
        if (memop != 0) { \
            TCGLabel *l0 = gen_new_label(); \
            tagged(N) a = tagged_local_new(N); \
            tagged(N) b = tagged_local_new(N); \
            tagged(S) d = tagged_local_new(S); \
            temp(N) t0 = temp_new(N); \
            TCGv t1 = tcg_temp_local_new(); \
            \
            gen_tagged_src(1, N, instr, a); \
            gen_tagged_src(2, N, instr, b); \
            gen_tagged_src(4, S, instr, d); \
            gen_loop_mode_st(instr->ctx, l0); \
            gen_tag_check(instr, a.tag); \
            gen_tag_check(instr, b.tag); \
            gen_tag_check(instr, d.tag); \
            call(N, tcg_gen_add, t0, a.val, b.val); \
            cast(t1, t0); \
            \
            if (instr->sm) { \
                TCGv_i32 t2 = tcg_temp_new_i32(); \
                gen_helper_probe_write_access(t2, cpu_env, t1); \
                tcg_gen_brcondi_i32(TCG_COND_EQ, t2, 0, l0); \
                tcg_temp_free_i32(t2); \
            } \
            \
            if (instr->ctx->mlock && instr->chan == 2 \
                && (instr->mas & 7) == 2) \
            { \
                /* FIXME: Does an any write before the st/mod=2 leads to a branch? */ \
                call(S, gen_atomic_cmpxchg, instr, d.val, t1, memop); \
            } else { \
                call(S, tcg_gen_qemu_st, d.val, t1, \
                    instr->ctx->mmuidx, memop); \
            } \
            gen_set_label(l0); \
            \
            tcg_temp_free(t1); \
            temp_free(N, t0); \
            tagged_free(S, d); \
            tagged_free(N, b); \
            tagged_free(N, a); \
        } \
    }

IMPL_ST(gen_st_ddd, d, d, tcg_gen_trunc_i64_tl)
IMPL_ST(gen_st_sdd, s, d, tcg_gen_trunc_i64_tl)

// TODO: stgd ops must use GD.base
IMPL_ST(gen_st_dss, d, s, tcg_gen_extu_i32_tl)
IMPL_ST(gen_st_sss, s, s, tcg_gen_extu_i32_tl)

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

static void gen_aaurw_aad_lo_i64(Instr *instr, TCGv_i64 arg1, TCGv_i32 tag)
{
    TCGv_i64 lo = cpu_aad_lo[instr->aad];
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

static void gen_aaurw_aad_hi_i64(Instr *instr, TCGv_i64 arg1, TCGv_i32 tag)
{
    tcg_gen_andi_i64(cpu_aad_hi[instr->aad], arg1, 0xffffffff00000000);
}

static void gen_aaurw_aad_i32(Instr *instr, TCGv_i32 arg1, TCGv_i32 tag)
{
    TCGv_i64 lo = cpu_aad_lo[instr->aad];
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

static void gen_aaurw_rest_i32(Instr* instr, TCGv_i32 arg1, TCGv_i32 tag)
{
    int idx = instr->aaind;
    TCGv_i32 t0 = tcg_temp_new_i32();
    tcg_gen_setcondi_i32(TCG_COND_NE, t0, tag, 0);
    switch(instr->aaopc) {
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

static void gen_aasti_incr(DisasContext *ctx, Instr *instr)
{
    uint16_t rlp = find_am_cond(ctx, instr->chan);
    TCGLabel *l0 = gen_new_label();
    TCGv_i32 t0 = tcg_temp_new_i32();

    if (ctx->loop_mode) {
        TCGLabel *l1 = gen_new_label();
        TCGv_i32 t0 = tcg_temp_local_new_i32();

        tcg_gen_brcondi_i32(TCG_COND_NE, cpu_lsr_pcnt, 0, l0);
        gen_is_loop_end_i32(t0);
        tcg_gen_brcondi_i32(TCG_COND_EQ, t0, 0, l1);
        tcg_gen_brcondi_i32(TCG_COND_EQ, cpu_lsr_strmd, 0, l0);
        gen_set_label(l1);
    }

    if (rlp != 0) {
        // FIXME: need to test AM RLP
        TCGv_i32 t1 = tcg_temp_new_i32();

        e2k_todo(ctx, "AM RLP found");
        gen_am_cond_i32(ctx, t1, instr->chan, rlp);
        tcg_gen_brcondi_i32(TCG_COND_EQ, t1, 0, l0);
        tcg_temp_free_i32(t1);
    }

    tcg_gen_muli_i32(t0, cpu_aaincr[instr->aaincr], instr->aaincr_len);
    tcg_gen_add_i32(cpu_aasti[instr->aaind], cpu_aasti[instr->aaind], t0);
    gen_set_label(l0);

    tcg_temp_free_i32(t0);
}

static void gen_aad_ptr(DisasContext *ctx, TCGv ret, Instr *instr)
{
    uint32_t lit = 0;
    TCGv_i64 t0 = tcg_temp_new_i64();
    TCGv t1 = tcg_temp_new();
    TCGv t2 = tcg_temp_new();

    if (instr->aalit) {
        int lts = instr->aalit - 1;
        if (ctx->bundle.lts_present[lts]) {
            lit = ctx->bundle.lts[lts];
        } else {
            gen_tr_excp_illopn(ctx);
            return;
        }
    }

    tcg_gen_extract_i64(t0, cpu_aad_lo[instr->aad], 0, 48);
    tcg_gen_trunc_i64_tl(t1, t0);
    tcg_gen_extu_i32_tl(t2, cpu_aasti[instr->aaind]);
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

static void gen_staa_i64(Instr *instr)
{
    DisasContext *ctx = instr->ctx;
    uint8_t mas = instr->mas;
    TCGLabel *l0 = gen_new_label();
    Tagged_i64 s4 = tagged_local_new_i64();

    gen_loop_mode_st(ctx, l0);
    gen_tagged_src4_d(instr, s4);
    gen_tag_check(instr, s4.tag);
    if (mas == 0x3f) {
        /* aaurwd */
        if (instr->aaopc == 0) {
            if (instr->chan == 5 && instr->opc1 == 0x3f) {
                gen_aaurw_aad_hi_i64(instr, s4.val, s4.tag);
            } else {
                gen_aaurw_aad_lo_i64(instr, s4.val, s4.tag);
            }
        } else {
            TCGv_i32 t0 = tcg_temp_new_i32();
            tcg_gen_extrl_i64_i32(t0, s4.val);
            gen_aaurw_rest_i32(instr, t0, s4.tag);
            tcg_temp_free_i32(t0);
        }
    } else {
        /* staad */
        TCGLabel *l0 = gen_new_label();
        TCGv t0 = tcg_temp_local_new();

        if (mas != 0) {
            e2k_todo(ctx, "staad mas=%#x is not implemented", mas);
        }

        gen_aad_ptr(ctx, t0, instr);

        if (instr->sm) {
            TCGv_i32 t1 = tcg_temp_new_i32();
            gen_helper_probe_write_access(t1, cpu_env, t0);
            tcg_gen_brcondi_i32(TCG_COND_EQ, t1, 0, l0);
            tcg_temp_free_i32(t1);
        }

        tcg_gen_qemu_st_i64(s4.val, t0, ctx->mmuidx, MO_Q);
        gen_set_label(l0);
        tcg_temp_free(t0);

        if (instr->aaopc & 1) {
            /* incr must be executed outside of the staa predicate condition */
            instr->aaincr_len = 8;
        }
    }

    gen_set_label(l0);

    tagged_free_i64(s4);
}

static void gen_staa_i32(Instr *instr, MemOp memop)
{
    DisasContext *ctx = instr->ctx;
    uint8_t mas = instr->mas;
    TCGLabel *l0 = gen_new_label();
    Tagged_i32 s4 = tagged_local_new_i32();

    gen_loop_mode_st(ctx, l0);
    gen_tagged_src4_s(instr, s4);
    gen_tag_check(instr, s4.tag);
    if (mas == 0x3f) {
        /* aaurw */
        /* CPU do nothing if size less than 32 bits */
        if ((memop & MO_SIZE) == MO_32) {
            if (instr->aaopc == 0) {
                gen_aaurw_aad_i32(instr, s4.val, s4.tag);
            } else {
                gen_aaurw_rest_i32(instr, s4.val, s4.tag);
            }
        }
    } else {
        /* staaw */
        int len;
        TCGLabel *l0 = gen_new_label();
        TCGv t0 = tcg_temp_local_new();

        if (mas != 0) {
            char c;
            switch(memop & MO_SIZE) {
            case MO_8: c = 'b'; break;
            case MO_16: c = 'h'; break;
            case MO_32: c = 'w'; break;
            default:
                g_assert_not_reached();
                break;
            }
            e2k_todo(ctx, "staa%c mas=%#x", c, mas);
        }

        gen_aad_ptr(ctx, t0, instr);

        if (instr->sm) {
            TCGv_i32 t1 = tcg_temp_new_i32();
            gen_helper_probe_write_access(t1, cpu_env, t0);
            tcg_gen_brcondi_i32(TCG_COND_EQ, t1, 0, l0);
            tcg_temp_free_i32(t1);
        }

        tcg_gen_qemu_st_i32(s4.val, t0, ctx->mmuidx, memop);
        gen_set_label(l0);
        tcg_temp_free(t0);

        switch(memop & MO_SIZE) {
        case MO_8: len = 1; break;
        case MO_16: len = 2; break;
        case MO_32: len = 4; break;
        default:
            g_assert_not_reached();
            break;
        }

        if (instr->aaopc & 1) {
            /* incr must be executed outside of the staa predicate condition */
            instr->aaincr_len = len;
        }
    }

    gen_set_label(l0);

    tagged_free_i32(s4);
}

#define IMPL_ALOPF1_BASIC(name, S1, S2, R, T, code) \
    static void name(Instr *instr, T) \
    { \
        tagged(S1) s1 = tagged_local_new(S1); \
        tagged(S2) s2 = tagged_local_new(S2); \
        tagged(R) r = tagged_local_new(R); \
        \
        gen_tagged_src(1, S1, instr, s1); \
        gen_tagged_src(2, S2, instr, s2); \
        gen_result_init(R, instr, r); \
        gen_tag2(R, r, s1, s2); \
        { code; } \
        gen_al_result(R, instr, r); \
        \
        tagged_free(R, r); \
        tagged_free(S2, s2); \
        tagged_free(S1, s1); \
    }

#define IMPL_ALOPF1(name, S1, S2, R) \
    IMPL_ALOPF1_BASIC(name, S1, S2, R, \
        void (*op)(temp(R), temp(S1), temp(S2)), \
        { (*op)(r.val, s1.val, s2.val); })

IMPL_ALOPF1(gen_alopf1_sss, s, s, s)
IMPL_ALOPF1(gen_alopf1_ssd, s, s, d)
IMPL_ALOPF1(gen_alopf1_sdd, d, s, d)
IMPL_ALOPF1(gen_alopf1_ddd, d, d, d)
IMPL_ALOPF1(gen_alopf1_dsx, d, s, x)

#define IMPL_ALOPF1_TAG_SM(name, S1, S2, R) \
    IMPL_ALOPF1_BASIC(name, S1, S2, R, \
        void (*op)(temp(R), TCGv_i32, TCGv_i32, \
            temp(S1), temp(S2), bool sm), \
        { (*op)(r.val, r.tag, r.tag, s1.val, s2.val, instr->sm); })

IMPL_ALOPF1_TAG_SM(gen_alopf1_ttsss, s, s, s)
IMPL_ALOPF1_TAG_SM(gen_alopf1_ttdss, d, s, s)
IMPL_ALOPF1_TAG_SM(gen_alopf1_ttddd, d, d, d)

#define IMPL_ALOPF1_ENV(name, S1, S2, R) \
    IMPL_ALOPF1_BASIC(name, S1, S2, R, \
        void (*op)(temp(R), TCGv_env, temp(S1), temp(S2)), \
        { (*op)(r.val, cpu_env, s1.val, s2.val); })

IMPL_ALOPF1_ENV(gen_alopf1_esss, s, s, s)
IMPL_ALOPF1_ENV(gen_alopf1_edsd, d, s, d)
IMPL_ALOPF1_ENV(gen_alopf1_eddd, d, d, d)
IMPL_ALOPF1_ENV(gen_alopf1_exxs, x, x, s)
IMPL_ALOPF1_ENV(gen_alopf1_exxd, x, x, d)
IMPL_ALOPF1_ENV(gen_alopf1_exss, x, s, s)
IMPL_ALOPF1_ENV(gen_alopf1_exds, x, d, s)
IMPL_ALOPF1_ENV(gen_alopf1_exdd, x, d, d)
IMPL_ALOPF1_ENV(gen_alopf1_exsx, x, s, x)
IMPL_ALOPF1_ENV(gen_alopf1_exdx, x, d, x)
IMPL_ALOPF1_ENV(gen_alopf1_esxs, s, x, s)
IMPL_ALOPF1_ENV(gen_alopf1_edxd, d, x, d)
IMPL_ALOPF1_ENV(gen_alopf1_esxx, s, x, x)
IMPL_ALOPF1_ENV(gen_alopf1_edxx, d, x, x)
IMPL_ALOPF1_ENV(gen_alopf1_exxx, x, x, x)

#define IMPL_ALOPF2_BASIC(name, S2, R, T, code) \
    static void name(Instr *instr, T) \
    { \
        tagged(S2) s2 = tagged_local_new(S2); \
        tagged(R) r = tagged_local_new(R); \
        \
        gen_tagged_src(2, S2, instr, s2); \
        gen_result_init(R, instr, r); \
        gen_tag1(R, r, s2); \
        { code; } \
        gen_al_result(R, instr, r); \
        \
        tagged_free(R, r); \
        tagged_free(S2, s2); \
    }

#define IMPL_ALOPF2(name, S2, R) \
    IMPL_ALOPF2_BASIC(name, S2, R, \
        void (*op)(temp(R), temp(S2)), \
        { (*op)(r.val, s2.val); })

IMPL_ALOPF2(gen_alopf2_ss, s, s)
IMPL_ALOPF2(gen_alopf2_dd, d, d)
IMPL_ALOPF2(gen_alopf2_xs, x, s)
IMPL_ALOPF2(gen_alopf2_xx, x, x)

#define IMPL_ALOPF2_ENV(name, S2, R) \
    IMPL_ALOPF2_BASIC(name, S2, R, \
        void (*op)(temp(R), TCGv_env, temp(S2)), \
        { (*op)(r.val, cpu_env, s2.val); })

IMPL_ALOPF2_ENV(gen_alopf2_ess, s, s)
IMPL_ALOPF2_ENV(gen_alopf2_eds, d, s)
IMPL_ALOPF2_ENV(gen_alopf2_esd, s, d)
IMPL_ALOPF2_ENV(gen_alopf2_edd, d, d)
IMPL_ALOPF2_ENV(gen_alopf2_edx, d, x)
IMPL_ALOPF2_ENV(gen_alopf2_exs, x, s)
IMPL_ALOPF2_ENV(gen_alopf2_exd, x, d)
IMPL_ALOPF2_ENV(gen_alopf2_esx, s, x)

#define IMPL_ALOPF2_PSHUFH(name, S2, R) \
    IMPL_ALOPF2_BASIC(name, S2, R, \
        void (*op)(temp(R), temp(S2), TCGv_i32), \
        { \
            TCGv_i32 t0 = tcg_const_i32(instr->src3); \
            (*op)(r.val, s2.val, t0); \
            tcg_temp_free_i32(t0); \
        })

IMPL_ALOPF2_PSHUFH(gen_alopf2_pshufh, d, d)

#define IMPL_ALOPF7_BASIC(name, S1, S2, R, T, code) \
    static void name(Instr *instr, T) \
    { \
        tagged(S1) s1 = tagged_local_new(S1); \
        tagged(S2) s2 = tagged_local_new(S2); \
        tagged(R) r = tagged_local_new(R); \
        Tagged_i32 p = tagged_new_i32(); \
        temp(R) t0 = temp_new(R); \
        \
        gen_tagged_src(1, S1, instr, s1); \
        gen_tagged_src(2, S2, instr, s2); \
        gen_result_init(R, instr, r); \
        gen_tag2(R, r, s1, s2); \
        { code; } \
        tcg_gen_mov_i32(p.tag, r.tag); \
        call(R, tcg_gen_setcondi, TCG_COND_NE, t0, r.val, 0); \
        call(R, gen_extrl, p.val, t0); \
        gen_al_result_b(instr, p); \
        \
        temp_free(R, t0); \
        tagged_free_i32(p); \
        tagged_free(R, r); \
        tagged_free(S2, s2); \
        tagged_free(S1, s1); \
    }

#define IMPL_ALOPF7(name, S1, S2, R) \
    IMPL_ALOPF7_BASIC(name, S1, S2, R, \
        void (*op)(temp(R), temp(S1), temp(S2)), \
        { (*op)(r.val, s1.val, s2.val); })

IMPL_ALOPF7(gen_alopf7_sss, s, s, s)
IMPL_ALOPF7(gen_alopf7_ddd, d, d, d)

#define IMPL_ALOPF7_ENV(name, S1, S2, R) \
    IMPL_ALOPF7_BASIC(name, S1, S2, R, \
        void (*op)(temp(R), TCGv_env, temp(S1), temp(S2)), \
        { (*op)(r.val, cpu_env, s1.val, s2.val); })

IMPL_ALOPF7_ENV(gen_alopf7_esss, s, s, s)
IMPL_ALOPF7_ENV(gen_alopf7_eddd, d, d, d)
IMPL_ALOPF7_ENV(gen_alopf7_exsd, x, s, d)
IMPL_ALOPF7_ENV(gen_alopf7_exdd, x, d, d)
IMPL_ALOPF7_ENV(gen_alopf7_exxd, x, x, d)

#define IMPL_ALOPF11_LIT8(name, S1, S2, R) \
    IMPL_ALOPF1_BASIC(name, S1, S2, R, \
        void (*op)(temp(R), temp(S1), temp(S2), int), \
        { (*op)(r.val, s1.val, s2.val, instr->src3); })

IMPL_ALOPF11_LIT8(gen_alopf11_ddid, d, d, d)

#define IMPL_ALOPF21_BASIC(name, S1, S2, S3, R, T, code) \
    static void name(Instr *instr, T) \
    { \
        tagged(S1) s1 = tagged_local_new(S1); \
        tagged(S2) s2 = tagged_local_new(S2); \
        tagged(S3) s3 = tagged_local_new(S3); \
        tagged(R) r = tagged_local_new(R); \
        \
        gen_tagged_src(1, S1, instr, s1); \
        gen_tagged_src(2, S2, instr, s2); \
        gen_tagged_src(3, S3, instr, s3); \
        gen_result_init(R, instr, r); \
        gen_tag3(R, r, s1, s2, s3); \
        { code; } \
        gen_al_result(R, instr, r); \
        \
        tagged_free(R, r); \
        tagged_free(S3, s3); \
        tagged_free(S2, s2); \
        tagged_free(S1, s1); \
    }

#define IMPL_ALOPF21(name, S1, S2, S3, R) \
    IMPL_ALOPF21_BASIC(name, S1, S2, S3, R, \
        void (*op)(temp(R), temp(S1), temp(S2), temp(S3)), \
        { (*op)(r.val, s1.val, s2.val, s3.val); })

IMPL_ALOPF21(gen_alopf21_ssss, s, s, s, s)
IMPL_ALOPF21(gen_alopf21_dddd, d, d, d, d)

static AlopDesc *find_op(Instr *instr)
{
    /* ALES2/5 may be allocated but must not be used */
    int opc2 = instr->ales_present & ALES_PRESENT ? instr->opc2 : 0;
    int16_t index = alops_map[opc2][instr->opc1][instr->chan];
    while (index != -1) {
        bool is_match = false;
        AlopDesc *desc = &alops[index];
        switch(desc->alopf) {
        case ALOPF1:
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
            is_match = desc->extra1 == instr->opce1;
            break;
        case ALOPF7:
            is_match = desc->extra1 == instr->opc_cmp;
            break;
        case ALOPF8:
            is_match = desc->extra1 == instr->opc_cmp && instr->opce1 == 0xc0;
            break;
        case ALOPF11:
        case ALOPF11_MERGE:
        case ALOPF13:
        case ALOPF17:
            is_match = desc->extra1 == instr->opce3;
            break;
        case ALOPF12:
        case ALOPF12_IBRANCHD:
        case ALOPF12_ICALLD:
        case ALOPF22:
            is_match = desc->extra1 == instr->opce1 && desc->extra2 == instr->opce3;
            break;
        case ALOPF16:
            is_match = desc->extra1 == instr->opce2;
            break;
        default:
            g_assert_not_reached();
            break;
        }

        if (is_match) {
            return desc;
        }

        index = desc->next[instr->chan];
    }

    return NULL;
}

static inline void check_reg_src(DisasContext *ctx, uint8_t src)
{
    if (IS_REGULAR(src)) {
        ctx->max_r_src = MAX(ctx->max_r_src, GET_REGULAR(src));
    } else if (IS_BASED(src)) {
        ctx->max_b_cur = MAX(ctx->max_b_cur, GET_BASED(src));
    }
}

static inline void check_reg_dst(DisasContext *ctx, uint8_t dst)
{
    if (IS_REGULAR(dst)) {
        ctx->max_r_dst = MAX(ctx->max_r_dst, GET_REGULAR(dst));
    } else if (IS_BASED(dst)) {
        ctx->max_b_cur = MAX(ctx->max_b_cur, GET_BASED(dst));
    }
}

static void check_args(Alopf alopf, Instr *instr)
{
    DisasContext *ctx = instr->ctx;

    switch(alopf) {
    case ALOPF1:
    case ALOPF1_MERGE:
    case ALOPF11:
    case ALOPF11_MERGE:
    case ALOPF11_LIT8:
        check_reg_src(ctx, instr->src1);
        check_reg_src(ctx, instr->src2);
        check_reg_dst(ctx, instr->dst);
        break;
    case ALOPF2:
    case ALOPF12:
    case ALOPF12_PSHUFH:
    case ALOPF15:
    case ALOPF22:
        check_reg_src(ctx, instr->src2);
        check_reg_dst(ctx, instr->dst);
        break;
    case ALOPF3:
        check_reg_src(ctx, instr->src1);
        check_reg_src(ctx, instr->src2);
        check_reg_src(ctx, instr->src4);
        break;
    case ALOPF7:
    case ALOPF17:
        check_reg_src(ctx, instr->src1);
        check_reg_src(ctx, instr->src2);
        break;
    case ALOPF8:
        check_reg_src(ctx, instr->src2);
        break;
    case ALOPF10:
        check_reg_src(ctx, instr->src4);
        break;
    case ALOPF13:
        check_reg_src(ctx, instr->src1);
        check_reg_src(ctx, instr->src2);
        check_reg_src(ctx, instr->src4);
        break;
    case ALOPF16:
        check_reg_dst(ctx, instr->dst);
        break;
    case ALOPF21:
    case ALOPF21_ICOMB:
    case ALOPF21_FCOMB:
    case ALOPF21_PFCOMB:
    case ALOPF21_LCOMB:
        check_reg_src(ctx, instr->src1);
        check_reg_src(ctx, instr->src2);
        check_reg_src(ctx, instr->src3);
        check_reg_dst(ctx, instr->dst);
        break;
    default:
        e2k_todo(ctx, "check_args %d", alopf);
        break;
    }
}

static void gen_alop_simple(Instr *instr, uint32_t op, const char *name)
{
    DisasContext *ctx = instr->ctx;
    int chan = instr->chan;
    switch(op) {
    case OP_ANDS: gen_alopf1_sss(instr, tcg_gen_and_i32); break;
    case OP_ANDD: gen_alopf1_ddd(instr, tcg_gen_and_i64); break;
    case OP_ANDNS: gen_alopf1_sss(instr, gen_andn_i32); break;
    case OP_ANDND: gen_alopf1_ddd(instr, gen_andn_i64); break;
    case OP_ORS: gen_alopf1_sss(instr, tcg_gen_or_i32); break;
    case OP_ORD: gen_alopf1_ddd(instr, tcg_gen_or_i64); break;
    case OP_ORNS: gen_alopf1_sss(instr, gen_orn_i32); break;
    case OP_ORND: gen_alopf1_ddd(instr, gen_orn_i64); break;
    case OP_XORS: gen_alopf1_sss(instr, tcg_gen_xor_i32); break;
    case OP_XORD: gen_alopf1_ddd(instr, tcg_gen_xor_i64); break;
    case OP_XORNS: gen_alopf1_sss(instr, gen_xorn_i32); break;
    case OP_XORND: gen_alopf1_ddd(instr, gen_xorn_i64); break;
    case OP_SXT: gen_alopf1_sdd(instr, gen_helper_sxt); break;
    case OP_ADDS: gen_alopf1_sss(instr, tcg_gen_add_i32); break;
    case OP_ADDD: gen_alopf1_ddd(instr, tcg_gen_add_i64); break;
    case OP_SUBS: gen_alopf1_sss(instr, tcg_gen_sub_i32); break;
    case OP_SUBD: gen_alopf1_ddd(instr, tcg_gen_sub_i64); break;
    case OP_SCLS: gen_alopf1_sss(instr, tcg_gen_rotl_i32); break;
    case OP_SCLD: gen_alopf1_ddd(instr, tcg_gen_rotl_i64); break;
    case OP_SCRS: gen_alopf1_sss(instr, tcg_gen_rotr_i32); break;
    case OP_SCRD: gen_alopf1_ddd(instr, tcg_gen_rotr_i64); break;
    case OP_SHLS: gen_alopf1_sss(instr, tcg_gen_shl_i32); break;
    case OP_SHLD: gen_alopf1_ddd(instr, tcg_gen_shl_i64); break;
    case OP_SHRS: gen_alopf1_sss(instr, tcg_gen_shr_i32); break;
    case OP_SHRD: gen_alopf1_ddd(instr, tcg_gen_shr_i64); break;
    case OP_SARS: gen_alopf1_sss(instr, tcg_gen_sar_i32); break;
    case OP_SARD: gen_alopf1_ddd(instr, tcg_gen_sar_i64); break;
    case OP_GETFS: gen_alopf1_sss(instr, gen_getfs); break;
    case OP_GETFD: gen_alopf1_ddd(instr, gen_getfd); break;
    case OP_MERGES: gen_merges(instr); break;
    case OP_MERGED: gen_merged(instr); break;
    case OP_CMPOSB: gen_alopf7_sss(instr, gen_cmposb); break;
    case OP_CMPBSB: gen_alopf7_sss(instr, gen_cmpbsb); break;
    case OP_CMPESB: gen_alopf7_sss(instr, gen_cmpesb); break;
    case OP_CMPBESB: gen_alopf7_sss(instr, gen_cmpbesb); break;
    case OP_CMPSSB: gen_alopf7_sss(instr, gen_cmpssb); break;
    case OP_CMPPSB: gen_alopf7_sss(instr, gen_cmppsb); break;
    case OP_CMPLSB: gen_alopf7_sss(instr, gen_cmplsb); break;
    case OP_CMPLESB: gen_alopf7_sss(instr, gen_cmplesb); break;
    case OP_CMPODB: gen_alopf7_ddd(instr, gen_cmpodb); break;
    case OP_CMPBDB: gen_alopf7_ddd(instr, gen_cmpbdb); break;
    case OP_CMPEDB: gen_alopf7_ddd(instr, gen_cmpedb); break;
    case OP_CMPBEDB: gen_alopf7_ddd(instr, gen_cmpbedb); break;
    case OP_CMPSDB: gen_alopf7_ddd(instr, gen_cmpsdb); break;
    case OP_CMPPDB: gen_alopf7_ddd(instr, gen_cmppdb); break;
    case OP_CMPLDB: gen_alopf7_ddd(instr, gen_cmpldb); break;
    case OP_CMPLEDB: gen_alopf7_ddd(instr, gen_cmpledb); break;
    case OP_CMPANDESB: gen_alopf7_sss(instr, gen_cmpandesb); break;
    case OP_CMPANDSSB: gen_alopf7_sss(instr, gen_cmpandssb); break;
    case OP_CMPANDPSB: gen_alopf7_sss(instr, gen_cmpandpsb); break;
    case OP_CMPANDLESB: gen_alopf7_sss(instr, gen_cmpandlesb); break;
    case OP_CMPANDEDB: gen_alopf7_ddd(instr, gen_cmpandedb); break;
    case OP_CMPANDSDB: gen_alopf7_ddd(instr, gen_cmpandsdb); break;
    case OP_CMPANDPDB: gen_alopf7_ddd(instr, gen_cmpandpdb); break;
    case OP_CMPANDLEDB: gen_alopf7_ddd(instr, gen_cmpandledb); break;
    case OP_FCMPEQSB:  gen_alopf7_esss(instr, gen_helper_fcmpeqs); break;
    case OP_FCMPLTSB:  gen_alopf7_esss(instr, gen_helper_fcmplts); break;
    case OP_FCMPLESB:  gen_alopf7_esss(instr, gen_helper_fcmples); break;
    case OP_FCMPUODSB: gen_alopf7_esss(instr, gen_helper_fcmpuods); break;
    case OP_FCMPNEQSB: gen_alopf7_esss(instr, gen_helper_fcmpneqs); break;
    case OP_FCMPNLTSB: gen_alopf7_esss(instr, gen_helper_fcmpnlts); break;
    case OP_FCMPNLESB: gen_alopf7_esss(instr, gen_helper_fcmpnles); break;
    case OP_FCMPODSB:  gen_alopf7_esss(instr, gen_helper_fcmpods); break;
    case OP_FCMPEQDB:  gen_alopf7_eddd(instr, gen_helper_fcmpeqd); break;
    case OP_FCMPLTDB:  gen_alopf7_eddd(instr, gen_helper_fcmpltd); break;
    case OP_FCMPLEDB:  gen_alopf7_eddd(instr, gen_helper_fcmpled); break;
    case OP_FCMPUODDB: gen_alopf7_eddd(instr, gen_helper_fcmpuodd); break;
    case OP_FCMPNEQDB: gen_alopf7_eddd(instr, gen_helper_fcmpneqd); break;
    case OP_FCMPNLTDB: gen_alopf7_eddd(instr, gen_helper_fcmpnltd); break;
    case OP_FCMPNLEDB: gen_alopf7_eddd(instr, gen_helper_fcmpnled); break;
    case OP_FCMPODDB:  gen_alopf7_eddd(instr, gen_helper_fcmpodd); break;
    case OP_FXCMPEQSB:  gen_alopf7_exsd(instr, gen_helper_fxcmpeqs); break;
    case OP_FXCMPLTSB:  gen_alopf7_exsd(instr, gen_helper_fxcmplts); break;
    case OP_FXCMPLESB:  gen_alopf7_exsd(instr, gen_helper_fxcmples); break;
    case OP_FXCMPUODSB: gen_alopf7_exsd(instr, gen_helper_fxcmpuods); break;
    case OP_FXCMPNEQSB: gen_alopf7_exsd(instr, gen_helper_fxcmpneqs); break;
    case OP_FXCMPNLTSB: gen_alopf7_exsd(instr, gen_helper_fxcmpnlts); break;
    case OP_FXCMPNLESB: gen_alopf7_exsd(instr, gen_helper_fxcmpnles); break;
    case OP_FXCMPODSB:  gen_alopf7_exsd(instr, gen_helper_fxcmpods); break;
    case OP_FXCMPEQDB:  gen_alopf7_exdd(instr, gen_helper_fxcmpeqd); break;
    case OP_FXCMPLTDB:  gen_alopf7_exdd(instr, gen_helper_fxcmpltd); break;
    case OP_FXCMPLEDB:  gen_alopf7_exdd(instr, gen_helper_fxcmpled); break;
    case OP_FXCMPUODDB: gen_alopf7_exdd(instr, gen_helper_fxcmpuodd); break;
    case OP_FXCMPNEQDB: gen_alopf7_exdd(instr, gen_helper_fxcmpneqd); break;
    case OP_FXCMPNLTDB: gen_alopf7_exdd(instr, gen_helper_fxcmpnltd); break;
    case OP_FXCMPNLEDB: gen_alopf7_exdd(instr, gen_helper_fxcmpnled); break;
    case OP_FXCMPODDB:  gen_alopf7_exdd(instr, gen_helper_fxcmpodd); break;
    case OP_FXCMPEQXB:  gen_alopf7_exxd(instr, gen_helper_fxcmpeqx); break;
    case OP_FXCMPLTXB:  gen_alopf7_exxd(instr, gen_helper_fxcmpltx); break;
    case OP_FXCMPLEXB:  gen_alopf7_exxd(instr, gen_helper_fxcmplex); break;
    case OP_FXCMPUODXB: gen_alopf7_exxd(instr, gen_helper_fxcmpuodx); break;
    case OP_FXCMPNEQXB: gen_alopf7_exxd(instr, gen_helper_fxcmpneqx); break;
    case OP_FXCMPNLTXB: gen_alopf7_exxd(instr, gen_helper_fxcmpnltx); break;
    case OP_FXCMPNLEXB: gen_alopf7_exxd(instr, gen_helper_fxcmpnlex); break;
    case OP_FXCMPODXB:  gen_alopf7_exxd(instr, gen_helper_fxcmpodx); break;
    case OP_STB: gen_st_sdd(instr, MO_UB); break;
    case OP_STH: gen_st_sdd(instr, MO_UW); break;
    case OP_STW: gen_st_sdd(instr, MO_UL); break;
    case OP_STD: gen_st_ddd(instr, MO_Q); break;
    case OP_STGDB: gen_st_sss(instr, MO_UB); break;
    case OP_STGDH: gen_st_sss(instr, MO_UW); break;
    case OP_STGDW: gen_st_sss(instr, MO_UL); break;
    case OP_STGDD: gen_st_dss(instr, MO_Q); break;
    case OP_STGDQ: e2k_todo_illop(instr->ctx, "stgdq"); break;
    case OP_LDB: gen_ld_mas_i64(instr, MO_UB); break;
    case OP_LDH: gen_ld_mas_i64(instr, MO_UW); break;
    case OP_LDW: gen_ld_mas_i64(instr, MO_UL); break;
    case OP_LDD: gen_ld_mas_i64(instr, MO_Q); break;
#ifdef TARGET_E2K32
    case OP_LDGDB: gen_ld_mas_i32(instr, MO_UB); break;
    case OP_LDGDH: gen_ld_mas_i32(instr, MO_UW); break;
    case OP_LDGDW: gen_ld_mas_i32(instr, MO_UL); break;
    case OP_LDGDD: gen_ld_mas_i32(instr, MO_Q); break;
    case OP_LDGDQ: e2k_todo_illop(instr->ctx, "ldgdq"); break;
#else /* !TARGET_E2K32 */
    case OP_LDGDB: /* fallthrough */
    case OP_LDGDH: /* fallthrough */
    case OP_LDGDW: /* fallthrough */
    case OP_LDGDD: /* fallthrough */
    case OP_LDGDQ: gen_tr_excp_array_bounds(instr->ctx); break;
#endif
    case OP_BITREVS: gen_alopf2_ss(instr, gen_bitrevs); break;
    case OP_BITREVD: gen_alopf2_dd(instr, gen_bitrevd); break;
    case OP_LZCNTS: gen_alopf2_ss(instr, gen_lzcnts); break;
    case OP_LZCNTD: gen_alopf2_dd(instr, gen_lzcntd); break;
    case OP_POPCNTS: gen_alopf2_ss(instr, tcg_gen_ctpop_i32); break;
    case OP_POPCNTD: gen_alopf2_dd(instr, tcg_gen_ctpop_i64); break;
    case OP_FADDS: gen_alopf1_esss(instr, gen_helper_fadds); break;
    case OP_FADDD: gen_alopf1_eddd(instr, gen_helper_faddd); break;
    case OP_FSUBS: gen_alopf1_esss(instr, gen_helper_fsubs); break;
    case OP_FSUBD: gen_alopf1_eddd(instr, gen_helper_fsubd); break;
    case OP_FMINS: gen_alopf1_esss(instr, gen_helper_fmins); break;
    case OP_FMIND: gen_alopf1_eddd(instr, gen_helper_fmind); break;
    case OP_FMAXS: gen_alopf1_esss(instr, gen_helper_fmaxs); break;
    case OP_FMAXD: gen_alopf1_eddd(instr, gen_helper_fmaxd); break;
    case OP_FMULS: gen_alopf1_esss(instr, gen_helper_fmuls); break;
    case OP_FMULD: gen_alopf1_eddd(instr, gen_helper_fmuld); break;
    case OP_FCMPEQS: gen_alopf1_esss(instr, gen_helper_fcmpeqs); break;
    case OP_FCMPLTS: gen_alopf1_esss(instr, gen_helper_fcmplts); break;
    case OP_FCMPLES: gen_alopf1_esss(instr, gen_helper_fcmples); break;
    case OP_FCMPUODS: gen_alopf1_esss(instr, gen_helper_fcmpuods); break;
    case OP_FCMPNEQS: gen_alopf1_esss(instr, gen_helper_fcmpneqs); break;
    case OP_FCMPNLTS: gen_alopf1_esss(instr, gen_helper_fcmpnlts); break;
    case OP_FCMPNLES: gen_alopf1_esss(instr, gen_helper_fcmpnles); break;
    case OP_FCMPODS: gen_alopf1_esss(instr, gen_helper_fcmpods); break;
    case OP_FCMPEQD: gen_alopf1_eddd(instr, gen_helper_fcmpeqd); break;
    case OP_FCMPLTD: gen_alopf1_eddd(instr, gen_helper_fcmpltd); break;
    case OP_FCMPLED: gen_alopf1_eddd(instr, gen_helper_fcmpled); break;
    case OP_FCMPUODD: gen_alopf1_eddd(instr, gen_helper_fcmpuodd); break;
    case OP_FCMPNEQD: gen_alopf1_eddd(instr, gen_helper_fcmpneqd); break;
    case OP_FCMPNLTD: gen_alopf1_eddd(instr, gen_helper_fcmpnltd); break;
    case OP_FCMPNLED: gen_alopf1_eddd(instr, gen_helper_fcmpnled); break;
    case OP_FCMPODD: gen_alopf1_eddd(instr, gen_helper_fcmpodd); break;
    case OP_FSTOIS: gen_alopf2_ess(instr, gen_helper_fstois); break;
    case OP_FSTOISTR: gen_alopf2_ess(instr, gen_helper_fstoistr); break;
    case OP_ISTOFS: gen_alopf2_ess(instr, gen_helper_istofs); break;
    case OP_FDTOID: gen_alopf2_edd(instr, gen_helper_fdtoid); break;
    case OP_IDTOFD: gen_alopf2_edd(instr, gen_helper_idtofd); break;
    case OP_FXTOFD: gen_alopf2_exd(instr, gen_helper_fxtofd); break;
    case OP_FDTOFX: gen_alopf2_edx(instr, gen_helper_fdtofx); break;
    case OP_FSTOID: gen_alopf2_esd(instr, gen_helper_fstoid); break;
    case OP_FSTOIDTR: gen_alopf2_esd(instr, gen_helper_fstoidtr); break;
    case OP_FDTOIDTR: gen_alopf2_edd(instr, gen_helper_fdtoidtr); break;
    case OP_ISTOFD: gen_alopf2_esd(instr, gen_helper_istofd); break;
    case OP_FSTOFD: gen_alopf2_esd(instr, gen_helper_fstofd); break;
    case OP_FSTOFX: gen_alopf2_esx(instr, gen_helper_fstofx); break;
    case OP_FDTOISTR: gen_alopf2_eds(instr, gen_helper_fdtoistr); break;
    case OP_FDTOIS: gen_alopf2_eds(instr, gen_helper_fdtois); break;
    case OP_IDTOFS: gen_alopf2_eds(instr, gen_helper_idtofs); break;
    case OP_FDTOFS: gen_alopf2_eds(instr, gen_helper_fdtofs); break;
    case OP_FXTOFS: gen_alopf2_exs(instr, gen_helper_fxtofs); break;
    case OP_FXTOIS: gen_alopf2_exs(instr, gen_helper_fxtois); break;
    case OP_FXTOISTR: gen_alopf2_exs(instr, gen_helper_fxtoistr); break;
    case OP_FXTOID: gen_alopf2_exd(instr, gen_helper_fxtoid); break;
    case OP_FXTOIDTR: gen_alopf2_exd(instr, gen_helper_fxtoidtr); break;
    case OP_ISTOFX: gen_alopf2_esx(instr, gen_helper_istofx); break;
    case OP_IDTOFX: gen_alopf2_edx(instr, gen_helper_idtofx); break;
    case OP_UDIVS: gen_alopf1_ttsss(instr, gen_udivs); break;
    case OP_UDIVD: gen_alopf1_ttddd(instr, gen_udivd); break;
    case OP_SDIVS: gen_alopf1_ttsss(instr, gen_sdivs); break;
    case OP_SDIVD: gen_alopf1_ttddd(instr, gen_sdivd); break;
    case OP_FXADDSS: gen_alopf1_exss(instr, gen_helper_fxaddss); break;
    case OP_FXADDDD: gen_alopf1_exdd(instr, gen_helper_fxadddd); break;
    case OP_FXADDSX: gen_alopf1_exsx(instr, gen_helper_fxaddsx); break;
    case OP_FXADDDX: gen_alopf1_exdx(instr, gen_helper_fxadddx); break;
    case OP_FXADDXX: gen_alopf1_exxx(instr, gen_helper_fxaddxx); break;
    case OP_FXADDXD: gen_alopf1_exxd(instr, gen_helper_fxaddxd); break;
    case OP_FXADDXS: gen_alopf1_exxs(instr, gen_helper_fxaddxs); break;
    case OP_FXSUBSS: gen_alopf1_exss(instr, gen_helper_fxsubss); break;
    case OP_FXSUBDD: gen_alopf1_exdd(instr, gen_helper_fxsubdd); break;
    case OP_FXSUBSX: gen_alopf1_exsx(instr, gen_helper_fxsubsx); break;
    case OP_FXSUBDX: gen_alopf1_exdx(instr, gen_helper_fxsubdx); break;
    case OP_FXSUBXX: gen_alopf1_exxx(instr, gen_helper_fxsubxx); break;
    case OP_FXSUBXD: gen_alopf1_exxd(instr, gen_helper_fxsubxd); break;
    case OP_FXSUBXS: gen_alopf1_exxs(instr, gen_helper_fxsubxs); break;
    case OP_FXRSUBSS: gen_alopf1_exss(instr, gen_helper_fxrsubss); break;
    case OP_FXRSUBDD: gen_alopf1_exdd(instr, gen_helper_fxrsubdd); break;
    case OP_FXRSUBSX: gen_alopf1_exsx(instr, gen_helper_fxrsubsx); break;
    case OP_FXRSUBDX: gen_alopf1_exdx(instr, gen_helper_fxrsubdx); break;
    case OP_FXMULSS: gen_alopf1_exss(instr, gen_helper_fxmulss); break;
    case OP_FXMULDD: gen_alopf1_exdd(instr, gen_helper_fxmuldd); break;
    case OP_FXMULSX: gen_alopf1_exsx(instr, gen_helper_fxmulsx); break;
    case OP_FXMULDX: gen_alopf1_exdx(instr, gen_helper_fxmuldx); break;
    case OP_FXMULXX: gen_alopf1_exxx(instr, gen_helper_fxmulxx); break;
    case OP_FXMULXD: gen_alopf1_exxd(instr, gen_helper_fxmulxd); break;
    case OP_FXMULXS: gen_alopf1_exxs(instr, gen_helper_fxmulxs); break;
    case OP_FXDIVSS: gen_alopf1_exss(instr, gen_helper_fxdivss); break;
    case OP_FXDIVDD: gen_alopf1_exdd(instr, gen_helper_fxdivdd); break;
    case OP_FXDIVSX: gen_alopf1_exsx(instr, gen_helper_fxdivsx); break;
    case OP_FXDIVDX: gen_alopf1_exdx(instr, gen_helper_fxdivdx); break;
    case OP_FXDIVXX: gen_alopf1_exxx(instr, gen_helper_fxdivxx); break;
    case OP_FXDIVXD: gen_alopf1_exxd(instr, gen_helper_fxdivxd); break;
    case OP_FXDIVXS: gen_alopf1_exxs(instr, gen_helper_fxdivxs); break;
    case OP_MOVFI: gen_alopf2_xs(instr, gen_movfi); break;
    case OP_MOVIF: gen_alopf1_dsx(instr, gen_movif); break;
    case OP_MOVX: gen_alopf2_xx(instr, gen_movx); break;
    case OP_MOVXA: gen_alopf2_xx(instr, gen_movxa); break;
    case OP_MOVXC: gen_alopf2_xx(instr, gen_movxc); break;
    case OP_MOVTS: gen_movts(instr); break;
    case OP_MOVTCS: gen_movtcs(instr); break;
    case OP_MOVTD: gen_movtd(instr); break;
    case OP_MOVTCD: gen_movtcd(instr); break;
    case OP_MOVTQ: gen_movtq(instr); break;
    case OP_MOVTCQ: gen_movtcq(instr); break;
    case OP_GETPL: gen_getpl(instr); break;
    case OP_PANDD: gen_alopf1_ddd(instr, tcg_gen_and_i64); break;
    case OP_PANDND: gen_alopf1_ddd(instr, gen_andn_i64); break;
    case OP_PORD: gen_alopf1_ddd(instr, tcg_gen_or_i64); break;
    case OP_PXORD: gen_alopf1_ddd(instr, tcg_gen_xor_i64); break;
    case OP_PMINUB: gen_alopf1_ddd(instr, gen_helper_pminub); break;
    case OP_PMINSB: gen_alopf1_ddd(instr, gen_helper_pminsb); break;
    case OP_PMINUH: gen_alopf1_ddd(instr, gen_helper_pminuh); break;
    case OP_PMINSH: gen_alopf1_ddd(instr, gen_helper_pminsh); break;
    case OP_PMINUW: gen_alopf1_ddd(instr, gen_helper_pminuw); break;
    case OP_PMINSW: gen_alopf1_ddd(instr, gen_helper_pminsw); break;
    case OP_PMAXUB: gen_alopf1_ddd(instr, gen_helper_pmaxub); break;
    case OP_PMAXSB: gen_alopf1_ddd(instr, gen_helper_pmaxsb); break;
    case OP_PMAXUH: gen_alopf1_ddd(instr, gen_helper_pmaxuh); break;
    case OP_PMAXSH: gen_alopf1_ddd(instr, gen_helper_pmaxsh); break;
    case OP_PMAXUW: gen_alopf1_ddd(instr, gen_helper_pmaxuw); break;
    case OP_PMAXSW: gen_alopf1_ddd(instr, gen_helper_pmaxsw); break;
    case OP_PCMPEQB: gen_alopf1_ddd(instr, gen_helper_pcmpeqb); break;
    case OP_PCMPEQH: gen_alopf1_ddd(instr, gen_helper_pcmpeqh); break;
    case OP_PCMPEQW: gen_alopf1_ddd(instr, gen_helper_pcmpeqw); break;
    case OP_PCMPEQD: gen_alopf1_ddd(instr, gen_helper_pcmpeqd); break;
    case OP_PCMPGTB: gen_alopf1_ddd(instr, gen_helper_pcmpgtb); break;
    case OP_PCMPGTH: gen_alopf1_ddd(instr, gen_helper_pcmpgth); break;
    case OP_PCMPGTW: gen_alopf1_ddd(instr, gen_helper_pcmpgtw); break;
    case OP_PCMPGTD: gen_alopf1_ddd(instr, gen_helper_pcmpgtd); break;
    case OP_PADDB: gen_alopf1_ddd(instr, tcg_gen_vec_add8_i64); break;
    case OP_PADDH: gen_alopf1_ddd(instr, tcg_gen_vec_add16_i64); break;
    case OP_PADDW: gen_alopf1_ddd(instr, tcg_gen_vec_add32_i64); break;
    case OP_PADDD: gen_alopf1_ddd(instr, tcg_gen_add_i64); break;
    case OP_PADDSB: gen_alopf1_ddd(instr, gen_helper_paddsb); break;
    case OP_PADDSH: gen_alopf1_ddd(instr, gen_helper_paddsh); break;
    case OP_PADDUSB: gen_alopf1_ddd(instr, gen_helper_paddusb); break;
    case OP_PADDUSH: gen_alopf1_ddd(instr, gen_helper_paddush); break;
    case OP_PHADDH: gen_alopf1_ddd(instr, gen_helper_phaddh); break;
    case OP_PHADDW: gen_alopf1_ddd(instr, gen_helper_phaddw); break;
    case OP_PHADDSH:  gen_alopf1_ddd(instr, gen_helper_phaddsh); break;
    case OP_PSUBB: gen_alopf1_ddd(instr, tcg_gen_vec_sub8_i64); break;
    case OP_PSUBH: gen_alopf1_ddd(instr, tcg_gen_vec_sub16_i64); break;
    case OP_PSUBW: gen_alopf1_ddd(instr, tcg_gen_vec_sub32_i64); break;
    case OP_PSUBD: gen_alopf1_ddd(instr, tcg_gen_sub_i64); break;
    case OP_PSUBSB: gen_alopf1_ddd(instr, gen_helper_psubsb); break;
    case OP_PSUBSH: gen_alopf1_ddd(instr, gen_helper_psubsh); break;
    case OP_PSUBUSB: gen_alopf1_ddd(instr, gen_helper_psubusb); break;
    case OP_PSUBUSH: gen_alopf1_ddd(instr, gen_helper_psubush); break;
    case OP_PHSUBH: gen_alopf1_ddd(instr, gen_helper_phsubh); break;
    case OP_PHSUBW: gen_alopf1_ddd(instr, gen_helper_phsubw); break;
    case OP_PHSUBSH: gen_alopf1_ddd(instr, gen_helper_phsubsh); break;
    case OP_PMULHH: gen_alopf1_ddd(instr, gen_helper_pmulhh); break;
    case OP_PMULLH: gen_alopf1_ddd(instr, gen_helper_pmullh); break;
    case OP_PMULHUH: gen_alopf1_ddd(instr, gen_helper_pmulhuh); break;
    case OP_PMULUBHH: gen_alopf1_ddd(instr, gen_helper_pmulubhh); break;
    case OP_PMULHRSH: gen_alopf1_ddd(instr, gen_helper_pmulhrsh); break;
    case OP_PMADDH: gen_alopf1_ddd(instr, gen_helper_pmaddh); break;
    case OP_PMADDUBSH: gen_alopf1_ddd(instr, gen_helper_pmaddubsh); break;
    case OP_MPSADBH: gen_alopf1_ddd(instr, gen_helper_mpsadbh); break;
    case OP_PSADBW: gen_alopf1_ddd(instr, gen_helper_psadbw); break;
    case OP_PSIGNB: gen_alopf1_ddd(instr, gen_helper_psignb); break;
    case OP_PSIGNH: gen_alopf1_ddd(instr, gen_helper_psignh); break;
    case OP_PSIGNW: gen_alopf1_ddd(instr, gen_helper_psignw); break;
    case OP_PSLLH: gen_alopf1_ddd(instr, gen_helper_psllh); break;
    case OP_PSLLW: gen_alopf1_ddd(instr, gen_helper_psllw); break;
    case OP_PSLLD: gen_alopf1_ddd(instr, gen_pslld); break;
    case OP_PSRLH: gen_alopf1_ddd(instr, gen_helper_psrlh); break;
    case OP_PSRLW: gen_alopf1_ddd(instr, gen_helper_psrlw); break;
    case OP_PSRLD: gen_alopf1_ddd(instr, gen_psrld); break;
    case OP_PSRAH: gen_alopf1_ddd(instr, gen_helper_psrah); break;
    case OP_PSRAW: gen_alopf1_ddd(instr, gen_helper_psraw); break;
    case OP_PAVGUSB: gen_alopf1_ddd(instr, gen_helper_pavgusb); break;
    case OP_PAVGUSH: gen_alopf1_ddd(instr, gen_helper_pavgush); break;
    case OP_PSLLQL: gen_alopf11_ddid(instr, gen_psllql); break;
    case OP_PSLLQH: gen_alopf11_ddid(instr, gen_psllqh); break;
    case OP_PSRLQL: gen_alopf11_ddid(instr, gen_psrlql); break;
    case OP_PSRLQH: gen_alopf11_ddid(instr, gen_psrlqh); break;
    case OP_PINSH: gen_alopf11_ddid(instr, gen_pinsh); break;
    case OP_PEXTRH: gen_alopf11_ddid(instr, gen_pextrh); break;
    case OP_PSHUFH: gen_alopf2_pshufh(instr, gen_helper_pshufh); break;
    case OP_PSHUFW: gen_alopf11_ddid(instr, gen_pshufw); break;
    case OP_PMOVMSKB: gen_alopf1_ddd(instr, gen_helper_pmovmskb); break;
    case OP_PMOVMSKPS: gen_alopf1_ddd(instr, gen_helper_pmovmskps); break;
    case OP_PMOVMSKPD: gen_alopf1_ddd(instr, gen_helper_pmovmskpd); break;
    case OP_PACKSSHB: gen_alopf1_ddd(instr, gen_helper_packsshb); break;
    case OP_PACKUSHB: gen_alopf1_ddd(instr, gen_helper_packushb); break;
    case OP_PACKSSWH: gen_alopf1_ddd(instr, gen_helper_packsswh); break;
    case OP_PACKUSWH: gen_alopf1_ddd(instr, gen_helper_packuswh); break;
    case OP_PUNPCKLBH: gen_alopf1_ddd(instr, gen_helper_punpcklbh); break;
    case OP_PUNPCKLHW: gen_alopf1_ddd(instr, gen_helper_punpcklhw); break;
    case OP_PUNPCKLWD: gen_alopf1_ddd(instr, gen_helper_punpcklwd); break;
    case OP_PUNPCKHBH: gen_alopf1_ddd(instr, gen_helper_punpckhbh); break;
    case OP_PUNPCKHHW: gen_alopf1_ddd(instr, gen_helper_punpckhhw); break;
    case OP_PUNPCKHWD: gen_alopf1_ddd(instr, gen_helper_punpckhwd); break;
    case OP_PHMINPOSUH: gen_alopf1_ddd(instr, gen_helper_phminposuh); break;
    case OP_GETTAGS: gen_gettags(instr); break;
    case OP_GETTAGD: gen_gettagd(instr); break;
    case OP_PUTTAGS: gen_puttags(instr); break;
    case OP_PUTTAGD: gen_puttagd(instr); break;
    case OP_STAAB: gen_staa_i32(instr, MO_8); break;
    case OP_STAAH: gen_staa_i32(instr, MO_16); break;
    case OP_STAAW: gen_staa_i32(instr, MO_32); break;
    case OP_STAAD: gen_staa_i64(instr); break;
    case OP_STAAQ: {
        int pair_chan = chan == 2 ? 5 : 2;
        if (!ctx->bundle.als_present[pair_chan] ||
            extract32(ctx->bundle.als[pair_chan], 24, 7) != 0x3f ||
            (instr->dst & 1) != (chan == 2 ? 0 : 1))
        {
            gen_tr_excp_illopc(ctx);
            return;
        }
        gen_staa_i64(instr);
        break;
    }
    case OP_MULS: gen_alopf1_sss(instr, tcg_gen_mul_i32); break;
    case OP_MULD: gen_alopf1_ddd(instr, tcg_gen_mul_i64); break;
    case OP_UMULX: gen_alopf1_ssd(instr, gen_umulx); break;
    case OP_SMULX: gen_alopf1_ssd(instr, gen_smulx); break;
    case OP_RWS: gen_rws(instr); break;
    case OP_RWD: gen_rwd(instr); break;
    case OP_RRS: gen_rrd(instr); break;
    case OP_RRD: gen_rrd(instr); break;
    case OP_FDIVS: gen_alopf1_esss(instr, gen_helper_fdivs); break;
    case OP_FDIVD: gen_alopf1_eddd(instr, gen_helper_fdivd); break;
    case OP_GETSP: gen_alopf2_esd(instr, gen_helper_getsp); break;
    case OP_UMULHD: gen_alopf1_ddd(instr, gen_umulhd); break;
    case OP_SMULHD: gen_alopf1_ddd(instr, gen_smulhd); break;
    case OP_FCMPODSF: gen_alopf1_esss(instr, gen_helper_fcmpodsf); break;
    case OP_FCMPUDSF: gen_alopf1_esss(instr, gen_helper_fcmpudsf); break;
    case OP_FCMPODDF: gen_alopf1_eddd(instr, gen_helper_fcmpoddf); break;
    case OP_FCMPUDDF: gen_alopf1_eddd(instr, gen_helper_fcmpoddf); break;
    case OP_FXCMPODSF: gen_alopf1_exss(instr, gen_helper_fxcmpodsf); break;
    case OP_FXCMPUDSF: gen_alopf1_exss(instr, gen_helper_fxcmpudsf); break;
    case OP_FXCMPODDF: gen_alopf1_exds(instr, gen_helper_fxcmpoddf); break;
    case OP_FXCMPUDDF: gen_alopf1_exds(instr, gen_helper_fxcmpuddf); break;
    case OP_FXCMPODXF: gen_alopf1_exxs(instr, gen_helper_fxcmpodxf); break;
    case OP_FXCMPUDXF: gen_alopf1_exxs(instr, gen_helper_fxcmpudxf); break;
    case OP_FSTOIFS: gen_alopf1_esss(instr, gen_helper_fstoifs); break;
    case OP_FDTOIFD: gen_alopf1_eddd(instr, gen_helper_fdtoifd); break;
    case OP_UDIVX: gen_alopf1_ttdss(instr, gen_udivx); break;
    case OP_UMODX: gen_alopf1_ttdss(instr, gen_umodx); break;
    case OP_SDIVX: gen_alopf1_ttdss(instr, gen_sdivx); break;
    case OP_SMODX: gen_alopf1_ttdss(instr, gen_smodx); break;
    case OP_PFMULD: gen_alopf1_eddd(instr, gen_helper_fmuld); break;
    case OP_PFADDD: gen_alopf1_eddd(instr, gen_helper_faddd); break;
    case OP_PFSUBD: gen_alopf1_eddd(instr, gen_helper_fsubd); break;
    case OP_PFDIVD: gen_alopf1_eddd(instr, gen_helper_fdivd); break;
    case OP_PFMIND: gen_alopf1_eddd(instr, gen_helper_fmind); break;
    case OP_PFMAXD: gen_alopf1_eddd(instr, gen_helper_fmaxd); break;
    case OP_PFADDS: gen_alopf1_eddd(instr, gen_helper_pfadds); break;
    case OP_PFSUBS: gen_alopf1_eddd(instr, gen_helper_pfsubs); break;
    case OP_PFMULS: gen_alopf1_eddd(instr, gen_helper_pfmuls); break;
    case OP_PFDIVS: gen_alopf1_esss(instr, gen_helper_fdivs); break;
    case OP_PFMAXS: gen_alopf1_eddd(instr, gen_helper_pfmaxs); break;
    case OP_PFMINS: gen_alopf1_eddd(instr, gen_helper_pfmins); break;
    case OP_PFHADDS: gen_alopf1_eddd(instr, gen_helper_pfhadds); break;
    case OP_PFHSUBS: gen_alopf1_eddd(instr, gen_helper_pfhsubs); break;
    case OP_PFADDSUBS: gen_alopf1_eddd(instr, gen_helper_pfaddsubs); break;
    case OP_PFSQRTS: gen_alopf2_ess(instr, gen_helper_fsqrts); break;
    case OP_PFSTOIFS: gen_alopf1_eddd(instr, gen_helper_pfstoifs); break;
    case OP_PISTOFS: gen_alopf2_edd(instr, gen_helper_pistofs); break;
    case OP_PFSTOIS: gen_alopf2_edd(instr, gen_helper_pfstois); break;
    case OP_PFSTOISTR: gen_alopf2_edd(instr, gen_helper_pfstoistr); break;
    case OP_PFSTOFD: gen_alopf2_esd(instr, gen_helper_fstofd); break;
    case OP_PFDTOFS: gen_alopf2_eds(instr, gen_helper_fdtofs); break;
    case OP_PFDTOIFD: gen_alopf1_eddd(instr, gen_helper_fdtoifd); break;
    case OP_PFDTOIS: gen_alopf2_eds(instr, gen_helper_fdtois); break;
    case OP_PFDTOISTR: gen_alopf2_eds(instr, gen_helper_fdtoistr); break;
    case OP_PFCMPEQS: gen_alopf1_eddd(instr, gen_helper_pfcmpeqs); break;
    case OP_PFCMPLTS: gen_alopf1_eddd(instr, gen_helper_pfcmplts); break;
    case OP_PFCMPLES: gen_alopf1_eddd(instr, gen_helper_pfcmples); break;
    case OP_PFCMPUODS: gen_alopf1_eddd(instr, gen_helper_pfcmpuods); break;
    case OP_PFCMPNEQS: gen_alopf1_eddd(instr, gen_helper_pfcmpneqs); break;
    case OP_PFCMPNLTS: gen_alopf1_eddd(instr, gen_helper_pfcmpnlts); break;
    case OP_PFCMPNLES: gen_alopf1_eddd(instr, gen_helper_pfcmpnles); break;
    case OP_PFCMPODS: gen_alopf1_eddd(instr, gen_helper_pfcmpods); break;
    case OP_PFCMPEQD: gen_alopf1_eddd(instr, gen_helper_fcmpeqd); break;
    case OP_PFCMPLTD: gen_alopf1_eddd(instr, gen_helper_fcmpltd); break;
    case OP_PFCMPLED: gen_alopf1_eddd(instr, gen_helper_fcmpled); break;
    case OP_PFCMPUODD: gen_alopf1_eddd(instr, gen_helper_fcmpuodd); break;
    case OP_PFCMPNEQD: gen_alopf1_eddd(instr, gen_helper_fcmpneqd); break;
    case OP_PFCMPNLTD: gen_alopf1_eddd(instr, gen_helper_fcmpnltd); break;
    case OP_PFCMPNLED: gen_alopf1_eddd(instr, gen_helper_fcmpnled); break;
    case OP_PFCMPODD: gen_alopf1_eddd(instr, gen_helper_fcmpodd); break;
    case OP_FSCALED: gen_alopf1_edsd(instr, gen_helper_fscaled); break;
    case OP_FSCALES: gen_alopf1_esss(instr, gen_helper_fscales); break;
    case OP_FXSCALESX: gen_alopf1_exsx(instr, gen_helper_fxscalesx); break;
    case OP_FRCPS: gen_alopf2_ess(instr, gen_helper_frcps); break;
    case OP_FSQRTS: gen_alopf2_ess(instr, gen_helper_fsqrts); break;
    case OP_FRSQRTS: gen_alopf2_ess(instr, gen_helper_frsqrts); break;
#ifndef TARGET_E2K_PRECISE_FSQRTID
    case OP_FSQRTID: gen_alopf2_dd(instr, tcg_gen_mov_i64); break;
    case OP_FXSQRTISX: gen_alopf2_esx(instr, gen_helper_fstofx); break;
    case OP_FXSQRTIDX: gen_alopf2_edx(instr, gen_helper_fdtofx); break;
    case OP_FXSQRTIXX: gen_alopf2_xx(instr, gen_movx); break;
    /* FIXME: these are not ALOPF2! */
    case OP_FXSQRTUSX: /* fallthrough */
    case OP_FXSQRTUDX: /* fallthrough */
    case OP_FXSQRTUXX: gen_alopf2_xx(instr, gen_movx); break;
#else
#error Not implemented
#endif
    case OP_FXSQRTTSX: gen_alopf1_esxx(instr, gen_helper_fxsqrttsx); break;
    case OP_FXSQRTTDX: gen_alopf1_edxx(instr, gen_helper_fxsqrttdx); break;
    case OP_FXSQRTTXX: gen_alopf1_exxx(instr, gen_helper_fxsqrttxx); break;
    case OP_PFSQRTTD: /* fallthrough */
    case OP_FSQRTTD: gen_alopf1_eddd(instr, gen_helper_fsqrttd); break;
    case OP_INSFS: gen_alopf21_ssss(instr, gen_insfs); break;
    case OP_INSFD: gen_insfd(instr); break;
    case OP_PSHUFB: gen_alopf21_dddd(instr, gen_helper_pshufb); break;
    case OP_PMERGE: gen_alopf21_dddd(instr, gen_helper_pmerge); break;
    case OP_FXDIVTSS: gen_alopf1_esxs(instr, gen_helper_fxdivtss); break;
    case OP_FXDIVTDD: gen_alopf1_edxd(instr, gen_helper_fxdivtdd); break;
    case OP_FXDIVTSX: gen_alopf1_esxx(instr, gen_helper_fxdivtsx); break;
    case OP_FXDIVTDX: gen_alopf1_edxx(instr, gen_helper_fxdivtdx); break;
    case OP_VFSI:
    case OP_LDCSB:
    case OP_LDDSB:
    case OP_LDESB:
    case OP_LDFSB:
    case OP_LDGSB:
    case OP_LDSSB:
    case OP_LDCSH:
    case OP_LDDSH:
    case OP_LDESH:
    case OP_LDFSH:
    case OP_LDGSH:
    case OP_LDSSH:
    case OP_LDCSW:
    case OP_LDDSW:
    case OP_LDESW:
    case OP_LDFSW:
    case OP_LDGSW:
    case OP_LDSSW:
    case OP_LDCSD:
    case OP_LDDSD:
    case OP_LDESD:
    case OP_LDFSD:
    case OP_LDGSD:
    case OP_LDSSD:
    case OP_MOVTRS:
    case OP_MOVTRCS:
    case OP_MOVTRD:
    case OP_MOVTRCD:
    case OP_GETSAP:
    case OP_CUDTOAP:
    case OP_GDTOAP:
    case OP_STCSB:
    case OP_STDSB:
    case OP_STESB:
    case OP_STFSB:
    case OP_STGSB:
    case OP_STSSB:
    case OP_STCSH:
    case OP_STDSH:
    case OP_STESH:
    case OP_STFSH:
    case OP_STGSH:
    case OP_STSSH:
    case OP_STCSW:
    case OP_STDSW:
    case OP_STESW:
    case OP_STFSW:
    case OP_STGSW:
    case OP_STSSW:
    case OP_STCSD:
    case OP_STDSD:
    case OP_STESD:
    case OP_STFSD:
    case OP_STGSD:
    case OP_STSSD:
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
    case OP_STAAQP:
    case OP_QPAND:
    case OP_QPANDN:
    case OP_QPOR:
    case OP_QPXOR:
    case OP_QPADDB:
    case OP_QPADDH:
    case OP_QPADDSB:
    case OP_QPADDSH:
    case OP_QPADDUSB:
    case OP_QPADDUSH:
    case OP_QPADDW:
    case OP_QPADDD:
    case OP_QPSUBB:
    case OP_QPSUBH:
    case OP_QPSUBSB:
    case OP_QPSUBSH:
    case OP_QPSUBUSB:
    case OP_QPSUBUSH:
    case OP_QPSUBW:
    case OP_QPSUBD:
    case OP_QPFADDS:
    case OP_QPFADDD:
    case OP_QPFHADDS:
    case OP_QPFHSUBS:
    case OP_QPFADDSUBS:
    case OP_QPFADDSUBD:
    case OP_QPFSTOIFS:
    case OP_QPFDTOIFD:
    case OP_QPFMINS:
    case OP_QPFMIND:
    case OP_QPFMAXS:
    case OP_QPFMAXD:
    case OP_QPFMULS:
    case OP_QPFMULD:
    case OP_QPFSUBS:
    case OP_QPFSUBD:
    case OP_QPMSK2SGNB:
    case OP_QPPACKDL:
    case OP_QPSLLH:
    case OP_QPSLLW:
    case OP_QPSLLD:
    case OP_QPSRLH:
    case OP_QPSRLW:
    case OP_QPSRLD:
    case OP_QPSRAH:
    case OP_QPSRAW:
    case OP_QPACKSSHB:
    case OP_QPACKSSWH:
    case OP_QPACKUSHB:
    case OP_QPACKUSWH:
    case OP_QPAVGUSB:
    case OP_QPAVGUSH:
    case OP_QPCMPEQB:
    case OP_QPCMPEQD:
    case OP_QPCMPEQH:
    case OP_QPCMPEQW:
    case OP_QPCMPGTB:
    case OP_QPCMPGTD:
    case OP_QPCMPGTH:
    case OP_QPCMPGTW:
    case OP_QPHADDH:
    case OP_QPHADDSH:
    case OP_QPHADDW:
    case OP_QPHSUBH:
    case OP_QPHSUBSH:
    case OP_QPHSUBW:
    case OP_QPMAXSB:
    case OP_QPMAXSH:
    case OP_QPMAXSW:
    case OP_QPMAXUB:
    case OP_QPMAXUH:
    case OP_QPMAXUW:
    case OP_QPMINSB:
    case OP_QPMINSH:
    case OP_QPMINSW:
    case OP_QPMINUB:
    case OP_QPMINUH:
    case OP_QPMINUW:
    case OP_QPMULHH:
    case OP_QPMULHRSH:
    case OP_QPMULHUH:
    case OP_QPMULLH:
    case OP_QPMULUBHH:
    case OP_QPSIGNB:
    case OP_QPSIGNH:
    case OP_QPSIGNW:
    case OP_QPHMINPOSUH:
    case OP_QPMADDH:
    case OP_QPMADDUBSH:
    case OP_QPMPSADBH:
    case OP_QPSADBW:
    case OP_QPSRCD:
    case OP_QPSRCW:
    case OP_PSRCD:
    case OP_PSRCW:
    case OP_GETFZS:
    case OP_GETFZD:
    case OP_PUTTAGQP:
    case OP_PMULLW:
    case OP_QPMULLW:
    case OP_QPFCMPEQS:
    case OP_QPFCMPLTS:
    case OP_QPFCMPLES:
    case OP_QPFCMPUODS:
    case OP_QPFCMPNEQS:
    case OP_QPFCMPNLTS:
    case OP_QPFCMPNLES:
    case OP_QPFCMPODS:
    case OP_QPFCMPEQD:
    case OP_QPFCMPLTD:
    case OP_QPFCMPLED:
    case OP_QPFCMPUODD:
    case OP_QPFCMPNEQD:
    case OP_QPFCMPNLTD:
    case OP_QPFCMPNLED:
    case OP_QPFCMPODD:
    case OP_LDQ:
    case OP_LDQP:
    case OP_LDGDQP:
    case OP_LDCUDQP:
    case OP_LDCSQP:
    case OP_LDDSQP:
    case OP_LDESQP:
    case OP_LDFSQP:
    case OP_LDGSQP:
    case OP_LDSSQP:
    case OP_LDAPQP:
    case OP_LDRQP:
    case OP_QPSGN2MSKB:
    case OP_QPSWITCHW:
    case OP_QPSWITCHD:
    case OP_QPFSTOIS:
    case OP_QPFSTOISTR:
    case OP_QPISTOFS:
    case OP_QPFSTOID:
    case OP_QPFSTOIDTR:
    case OP_QPISTOFD:
    case OP_QPFSTOFD:
    case OP_QPFDTOIS:
    case OP_QPFDTOISTR:
    case OP_QPIDTOFS:
    case OP_QPFDTOFS:
    case OP_QPFDTOID:
    case OP_QPFDTOIDTR:
    case OP_QPIDTOFD:
    case OP_STQ:
    case OP_STGDMQP:
    case OP_STGDQP:
    case OP_STAPQP:
    case OP_STAPMQP:
    case OP_STMQP:
    case OP_STQP:
    case OP_STCSMQP:
    case OP_STCSQP:
    case OP_STDSMQP:
    case OP_STDSQP:
    case OP_STESMQP:
    case OP_STESQP:
    case OP_STFSMQP:
    case OP_STFSQP:
    case OP_STGSMQP:
    case OP_STGSQP:
    case OP_STSSMQP:
    case OP_STSSQP:
    case OP_STRQP:
    case OP_ADDCD:
    case OP_ADDCD_C:
    case OP_SUBCD:
    case OP_SUBCD_C:
    case OP_VFBGV:
    case OP_MKFSW:
    case OP_MODBGV:
    case OP_PCMPEQBOP:
    case OP_PCMPEQHOP:
    case OP_PCMPEQWOP:
    case OP_PCMPEQDOP:
    case OP_PCMPGTBOP:
    case OP_PCMPGTHOP:
    case OP_PCMPGTWOP:
    case OP_PCMPGTDOP:
    case OP_PCMPEQBAP:
    case OP_PCMPEQHAP:
    case OP_PCMPEQWAP:
    case OP_PCMPEQDAP:
    case OP_PCMPGTBAP:
    case OP_PCMPGTHAP:
    case OP_PCMPGTWAP:
    case OP_PCMPGTDAP:
    case OP_QPCMPEQBOP:
    case OP_QPCMPEQHOP:
    case OP_QPCMPEQWOP:
    case OP_QPCMPEQDOP:
    case OP_QPCMPGTBOP:
    case OP_QPCMPGTHOP:
    case OP_QPCMPGTWOP:
    case OP_QPCMPGTDOP:
    case OP_QPCMPEQBAP:
    case OP_QPCMPEQHAP:
    case OP_QPCMPEQWAP:
    case OP_QPCMPEQDAP:
    case OP_QPCMPGTBAP:
    case OP_QPCMPGTHAP:
    case OP_QPCMPGTWAP:
    case OP_QPCMPGTDAP:
    case OP_PMRGP:
    case OP_QPMRGP:
    case OP_CLMULH:
    case OP_CLMULL:
    case OP_IBRANCHD:
    case OP_ICALLD:
    case OP_QPCEXT_0X00:
    case OP_QPCEXT_0X7F:
    case OP_QPCEXT_0X80:
    case OP_QPCEXT_0XFF:
    case OP_FMAS:
    case OP_FMSS:
    case OP_FNMAS:
    case OP_FNMSS:
    case OP_FMAD:
    case OP_FMSD:
    case OP_FNMAD:
    case OP_FNMSD:
    case OP_QPFMAS:
    case OP_QPFMSS:
    case OP_QPFNMAS:
    case OP_QPFNMSS:
    case OP_QPFMAD:
    case OP_QPFMSD:
    case OP_QPFNMAD:
    case OP_QPFNMSD:
    case OP_QPFMASS:
    case OP_QPFMSAS:
    case OP_QPFMASD:
    case OP_QPFMSAD:
        e2k_todo_illop(ctx, "unimplemented %d (%s)", op, name); break;
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
    static void glue(gen_icomb_op_, S)(Instr *instr, IComb opc, \
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
            gen_mrgc_i32(instr->ctx, instr->chan, t0); \
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

static inline bool icomb_check(Instr *instr, IComb opc1, IComb opc2)
{
    if (!is_chan_14(instr->chan)) {
        return false;
    }

    if (instr->ctx->version == 1) {
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

static inline bool fcomb_check(Instr *instr, FComb opc1, FComb opc2)
{
    int ver = instr->ctx->version;

    if (opc1 == FCOMB_RSUB || (ver < 4 && is_chan_25(instr->chan))) {
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

static inline bool pfcomb_check(Instr *instr, FComb opc1, FComb opc2)
{
    int ver = instr->ctx->version;

    if (ver < 4 && is_chan_25(instr->chan)) {
        return false;
    }

    return pfcomb_map[opc1][opc2];
}

#define IMPL_GEN_FCOMB_OP(S, T) \
    static void glue(gen_fcomb_op_, S)(Instr *instr, FComb opc, \
        glue(TCGv_, S) ret, glue(TCGv_, S) arg1, glue(TCGv_, S) arg2) \
    { \
        switch(opc) { \
        case FCOMB_ADD: glue(gen_helper_fadd, T)(ret, cpu_env, arg1, arg2); break; \
        case FCOMB_SUB: glue(gen_helper_fsub, T)(ret, cpu_env, arg1, arg2); break; \
        case FCOMB_MUL: glue(gen_helper_fmul, T)(ret, cpu_env, arg1, arg2); break; \
        case FCOMB_RSUB: glue(gen_helper_fsub, T)(ret, cpu_env, arg2, arg1); break; \
        default: gen_tr_excp_illopc(instr->ctx); break; \
        } \
    }

IMPL_GEN_FCOMB_OP(i64, d)
IMPL_GEN_FCOMB_OP(i32, s)

static void gen_pfcomb_op_i32(Instr *instr, FComb opc,
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
    default: gen_tr_excp_illopc(instr->ctx); break;
    }
}

static void gen_pfcomb_op_i64(Instr *instr, FComb opc,
    TCGv_i64 ret, TCGv_i64 arg1, TCGv_i64 arg2)
{
    switch (opc) {
    case FCOMB_ADD: gen_helper_faddd(ret, cpu_env, arg1, arg2); break;
    case FCOMB_SUB: gen_helper_fsubd(ret, cpu_env, arg1, arg2); break;
    case FCOMB_MUL: gen_helper_fmuld(ret, cpu_env, arg1, arg2); break;
    case FCOMB_RSUB: gen_helper_fsubd(ret, cpu_env, arg2, arg1); break;
    default: gen_tr_excp_illopc(instr->ctx); break;
    }
}

static inline int comb_opc1(Instr *instr, int m1)
{
    return (instr->opc1 >> 1) & m1;
}

static inline int comb_opc2(Instr *instr, int m1, int m2)
{
    return ((instr->opc2 & m2) << 2) | ((instr->opc1 >> 5) & m1);
}

#define icomb_opc1(instr) comb_opc1(instr, 0xf)
#define fcomb_opc1(instr) comb_opc1(instr, 0x7)

#define icomb_opc2(instr) comb_opc2(instr, 0x3, 0x3)
#define fcomb_opc2(instr) comb_opc2(instr, 0x3, 0x1)

#define IMPL_GEN_COMB(NAME, P, S, OP) \
    static void NAME(Instr *instr, int opc1, int opc2) \
    { \
        tagged(S) r = tagged_local_new(S); \
        tagged(S) s1 = tagged_local_new(S); \
        tagged(S) s2 = tagged_local_new(S); \
        tagged(S) s3 = tagged_local_new(S); \
        \
        gen_tagged_src(1, S, instr, s1); \
        gen_tagged_src(2, S, instr, s2); \
        gen_tagged_src(3, S, instr, s3); \
        gen_tag3(S, r, s1, s2, s3); \
        OP(instr, opc1, r.val, s1.val, s2.val); \
        OP(instr, opc2, r.val, s3.val, r.val); \
        gen_al_result(S, instr, r); \
        \
        tagged_free(S, s3); \
        tagged_free(S, s2); \
        tagged_free(S, s1); \
        tagged_free(S, r); \
    }

IMPL_GEN_COMB(gen_icomb_i64,  icomb,  d, gen_icomb_op_i64)
IMPL_GEN_COMB(gen_icomb_i32,  icomb,  s, gen_icomb_op_i32)
IMPL_GEN_COMB(gen_fcomb_i64,  fcomb,  d, gen_fcomb_op_i64)
IMPL_GEN_COMB(gen_fcomb_i32,  fcomb,  s, gen_fcomb_op_i32)
IMPL_GEN_COMB(gen_pfcomb_i64, pfcomb, d, gen_pfcomb_op_i64)
IMPL_GEN_COMB(gen_pfcomb_i32, pfcomb, d, gen_pfcomb_op_i32)

#define IMPL_GEN_COMB_SELECT(NAME) \
    static void glue(gen_, NAME)(Instr *instr, uint32_t op) \
    { \
        int opc1 = op & 0xffff; \
        int opc2 = op >> 16; \
        \
        if (instr->opc1 & 1) { \
            glue3(gen_, NAME, _i64)(instr, opc1, opc2); \
        } else { \
            glue3(gen_, NAME, _i32)(instr, opc1, opc2); \
        } \
    }

IMPL_GEN_COMB_SELECT(icomb)
IMPL_GEN_COMB_SELECT(fcomb)
IMPL_GEN_COMB_SELECT(pfcomb)

static void gen_lcomb_i64(Instr *instr, uint32_t base)
{
    /* see gen_alopf21_i64 */
    Tagged_i64 r = tagged_new_i64();
    Tagged_i64 s1 = tagged_new_i64();
    Tagged_i64 s2 = tagged_new_i64();
    Tagged_i64 s3 = tagged_new_i64();
    TCGv_i32 opc = tcg_const_i32(base + instr->opc1);

    gen_tagged_src1_d(instr, s1);
    gen_tagged_src2_d(instr, s2);
    gen_tagged_src3_d(instr, s3);
    check_args(ALOPF21, instr);
    gen_tag3d(r, s1, s2, s3);
    gen_helper_plog(r.val, opc, s1.val, s2.val, s3.val);
    gen_al_result_d(instr, r);

    tagged_free_i64(s3);
    tagged_free_i64(s2);
    tagged_free_i64(s2);
    tagged_free_i64(r);
}

static inline bool rlp_check_chan(uint16_t rlp, int chan)
{
    return extract16(rlp, 14, 1) == (chan > 2) &&
        extract16(rlp, 10 + chan % 3, 1);
}

static inline bool rlp_is_chan_pred(uint16_t rlp, int chan)
{
    return !extract16(rlp, 15, 1) && rlp_check_chan(rlp, chan);
}

static void chan_check_preds(DisasContext *ctx, int chan, TCGLabel *l)
{
    bool has_pcnt = false;
    bool has_preg = false;
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i32 t1 = tcg_temp_new_i32();
    TCGv_i32 t2 = tcg_temp_new_i32();
    int i, j;

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
            case 0x2: { /* %pcntN */
                has_pcnt = true;
                tcg_gen_setcondi_i32(TCG_COND_LEU, t2, cpu_lsr_pcnt, idx);
                if (invert) {
                    tcg_gen_xori_i32(t2, t2, 1);
                }
                tcg_gen_or_i32(t0, t0, t2);
                break;
            }
            case 0x3: /* %predN */
                has_preg = true;
                gen_preg_i32(t2, idx);
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
        TCGv_i32 cond = e2k_get_temp_i32(ctx);

        if (has_preg && has_pcnt) {
            tcg_gen_and_i32(cond, t0, t1);
        } else if (has_preg) {
            tcg_gen_mov_i32(cond, t1);
        } else {
            tcg_gen_mov_i32(cond, t0);
        }

        ctx->al_cond[chan] = cond;
        tcg_gen_brcondi_i32(TCG_COND_EQ, cond, 0, l);
    }

    tcg_temp_free_i32(t2);
    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
}

static void alop_instr_init(Instr *instr, DisasContext *ctx, int chan)
{
    memset(instr, 0, sizeof(Instr));

    instr->ctx = ctx;
    instr->chan = chan;
    instr->mas = ctx->bundle2.cs1.type == CS1_MAS ? ctx->bundle2.cs1.mas[chan] : 0;
    instr->als = ctx->bundle.als[chan];
    instr->ales = ctx->bundle.ales[chan];
    instr->ales_present = ctx->bundle.ales_present[chan];
}

static void alop_decode(Instr *instr)
{
    Alop *alop = &instr->ctx->bundle2.alops[instr->chan];

    alop->format = ALOPF_NONE;
    alop->op = 0;
    alop->name = "none";

    switch (instr->opc2) {
    case SHORT:
    case EXT:
    case EXT1:
    case EXT2: {
        AlopDesc *desc = find_op(instr);
        if (!desc) {
            gen_tr_excp_illopc(instr->ctx);
            return;
        }
        alop->format = desc->alopf;
        alop->op = desc->op;
        alop->name = desc->dsc;
        break;
    }
    case ICMB0:
    case ICMB1:
    case ICMB2:
    case ICMB3:
        if (instr->opc2 == ICMB3
            && (instr->opc1 == 0x6c || instr->opc1 == 0x6d))
        {
            if (!is_chan_0134(instr->chan)) {
                gen_tr_excp_illopc(instr->ctx);
                return;
            }
            alop->format = ALOPF21;
            alop->op = instr->opc1 & 1 ? OP_INSFD : OP_INSFS;
        } else {
            int opc1 = icomb_opc1(instr);
            int opc2 = icomb_opc2(instr);
            if (!icomb_check(instr, opc1, opc2)) {
                gen_tr_excp_illopc(instr->ctx);
                return;
            }
            alop->format = ALOPF21_ICOMB;
            alop->op = (opc2 << 16) | opc1;
        }
        break;
    case FLB:
    case FLH:
    case FLW:
    case FLD:
        e2k_todo_illop(instr->ctx, "flags ops");
        break;
    case FCMB0:
    case FCMB1: {
        int opc1 = fcomb_opc1(instr);
        int opc2 = fcomb_opc2(instr);
        if (!fcomb_check(instr, opc1, opc2)) {
            gen_tr_excp_illopc(instr->ctx);
            return;
        }
        alop->format = ALOPF21_FCOMB;
        alop->op = (opc2 << 16) | opc1;
        break;
    }
    case PFCMB0:
    case PFCMB1:
        if (instr->opc2 == PFCMB1 && is_chan_0134(instr->chan)
            && instr->ctx->version >= 2 && instr->opc1 == 0x4d)
        {
            alop->format = ALOPF12_PSHUFH;
            alop->op = OP_PSHUFB;
        } else if (instr->opc2 == PFCMB1 && is_chan_0134(instr->chan)
            && instr->ctx->version >= 2 && instr->opc1 == 0x6d)
        {
            alop->format = ALOPF21;
            alop->op = OP_PMERGE;
        } else {
            int opc1 = fcomb_opc1(instr);
            int opc2 = fcomb_opc2(instr);
            if (!pfcomb_check(instr, opc1, opc2)) {
                gen_tr_excp_illopc(instr->ctx);
                return;
            }
            alop->format = ALOPF21_PFCOMB;
            alop->op = (opc2 << 16) | opc1;
        }
        break;
    case LCMBD0:
    case LCMBD1:
        if (is_chan_0134(instr->chan) && instr->ctx->version >= 5) {
            alop->format = ALOPF21_LCOMB;
            alop->op = instr->opc2 == LCMBD0 ? 0 : 0x80;
        } else {
            gen_tr_excp_illopc(instr->ctx);
        }
        break;
    case LCMBQ0:
    case LCMBQ1:
        e2k_todo_illop(instr->ctx, "logical combined ops");
        break;
    case QPFCMB0:
    case QPFCMB1:
        e2k_todo_illop(instr->ctx, "packed128 float combined ops");
        break;
    default:
        gen_tr_excp_illopc(instr->ctx);
        break;
    }
}

static void alc_decode(DisasContext *ctx)
{
    int i;

    for (i = 0; i < 6; i++) {
        if (ctx->bundle.als_present[i]) {
            Instr instr;
            alop_instr_init(&instr, ctx, i);
            alop_decode(&instr);
        }
    }
}

static void gen_alop(Instr *instr, Alop *alop)
{
    TCGLabel *l0 = gen_new_label();

    if (alop->format == ALOPF_NONE) {
        return;
    }

    chan_check_preds(instr->ctx, instr->chan, l0);
    check_args(alop->format, instr);

    switch (alop->format) {
    case ALOPF21_ICOMB:
        gen_icomb(instr, alop->op);
        break;
    case ALOPF21_FCOMB:
        gen_fcomb(instr, alop->op);
        break;
    case ALOPF21_PFCOMB:
        gen_pfcomb(instr, alop->op);
        break;
    case ALOPF21_LCOMB:
        gen_lcomb_i64(instr, alop->op);
        break;
    default:
        gen_alop_simple(instr, alop->op, alop->name);
        break;
    }

    gen_set_label(l0);

    if (instr->aaincr_len != 0) {
        gen_aasti_incr(instr->ctx, instr);
    }
}

static void gen_alc(DisasContext *ctx)
{
    int i;

    for (i = 0; i < 6; i++) {
        Instr instr;
        Alop *alop = &ctx->bundle2.alops[i];
        ctx->al_results[i].type = AL_RESULT_NONE;
        ctx->al_cond[i] = NULL;
        alop_instr_init(&instr, ctx, i);
        gen_alop(&instr, alop);
    }
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

static void gen_al_result_commit_reg32(DisasContext *ctx, Alop *alop,
    TCGv_i32 index, TCGv_i32 tag, TCGv_i32 value)
{
    TCGv_i32 t0 = tcg_temp_new_i32();

    gen_reg_tag_set_i32(ctx, tag, index);
    if (is_alop_poison_result(alop)) {
        gen_dst_poison_i32(t0, value, tag);
    } else {
        tcg_gen_mov_i32(t0, value);
    }
    gen_reg_lo_write_i32(t0, index);

    tcg_temp_free_i32(t0);
}

static void gen_al_result_commit_reg64(DisasContext *ctx, Alop *alop,
    TCGv_i32 index, TCGv_i32 tag, TCGv_i64 value)
{
    TCGv_i64 t0 = tcg_temp_new_i64();

    gen_reg_tag_set_i64(ctx, tag, index);
    if (is_alop_poison_result(alop)) {
        gen_dst_poison_i64(t0, value, tag);
    } else {
        tcg_gen_mov_i64(t0, value);
    }
    gen_reg_lo_write_i64(t0, index);

    tcg_temp_free_i64(t0);
}

static inline void gen_al_result_commit_reg(DisasContext *ctx, Alop *alop,
    AlResult *res)
{
    AlResultType size = e2k_al_result_size(res->type);

    if (is_alop_check_tag(alop)) {
        // TODO: tag check before alops execution
    }

    switch (size) {
    case AL_RESULT_32: {
        if (is_alop_affected_by_dbl(alop)) {
            TCGLabel *l0 = gen_new_label();
            TCGLabel *l1 = gen_new_label();
            TCGv_i32 t0 = tcg_temp_new_i32();
            TCGv_i64 t1 = tcg_temp_new_i64();

            tcg_gen_brcondi_i32(TCG_COND_NE, cpu_wdbl, 0, l0);

            /* wdbl is not set */
            gen_al_result_commit_reg32(ctx, alop, res->reg.index,
                res->reg.tag, res->reg.v32);
            tcg_gen_br(l1);

            /* wdbl is set */
            gen_set_label(l0);
            gen_tag1_i64(t0, res->reg.tag);
            tcg_gen_extu_i32_i64(t1, res->reg.v32);
            gen_al_result_commit_reg64(ctx, alop, res->reg.index, t0, t1);

            /* exit */
            gen_set_label(l1);

            tcg_temp_free_i64(t1);
            tcg_temp_free_i32(t0);
        } else {
            gen_al_result_commit_reg32(ctx, alop, res->reg.index,
                res->reg.tag, res->reg.v32);
        }

        break;
    }
    case AL_RESULT_64:
        if (alop->format == ALOPF16 && alop->op == OP_RRS) {
            TCGLabel *l0 = gen_new_label();
            TCGLabel *l1 = gen_new_label();

            /* rrs is affected by wdbl but unlike other 32-bit ops
             * the upper half must be an actual value */
            tcg_gen_brcondi_i32(TCG_COND_NE, cpu_wdbl, 0, l1);
            gen_al_result_commit_reg32(ctx, alop, res->reg.index,
                res->reg.tag, res->reg.v32);
            tcg_gen_br(l0);
            gen_set_label(l1);
            gen_al_result_commit_reg64(ctx, alop, res->reg.index,
                res->reg.tag, res->reg.v64);
            gen_set_label(l0);
        } else {
            gen_al_result_commit_reg64(ctx, alop, res->reg.index,
                res->reg.tag, res->reg.v64);
        }
        break;
    case AL_RESULT_80:
        gen_al_result_commit_reg64(ctx, alop, res->reg.index, res->reg.tag,
            res->reg.v64);
        gen_reg_hi_write16u_i32(res->reg.x32, res->reg.index);
        break;
    case AL_RESULT_128:
        gen_al_result_commit_reg64(ctx, alop, res->reg.index, res->reg.tag,
            res->reg.v64);
        gen_reg_hi_write_i64(res->reg.x64, res->reg.index);
        break;
    default:
        g_assert_not_reached();
        break;
    }
}

static inline void gen_al_result_commit_ctpr(AlResult *res)
{
    AlResultType size = e2k_al_result_size(res->type);
    TCGv_i64 ctpr = cpu_ctprs[res->ctpr.index];
    TCGv_i64 t0 = tcg_temp_new_i64();
    TCGv_i64 t1 = tcg_const_i64(CTPR_TAG_DISP);

    assert(res->ctpr.index < 3);

    switch (size) {
    case AL_RESULT_32:
        tcg_gen_extu_i32_i64(t0, res->ctpr.v32);
        break;
    case AL_RESULT_64:
        tcg_gen_mov_i64(t0, res->ctpr.v64);
        break;
    default:
        g_assert_not_reached();
        break;
    }

    tcg_gen_deposit_i64(ctpr, ctpr, t0, CTPR_BASE_OFF, CTPR_BASE_LEN);
    tcg_gen_deposit_i64(ctpr, ctpr, t1, CTPR_TAG_OFF, CTPR_TAG_LEN);

    tcg_temp_free_i64(t1);
    tcg_temp_free_i64(t0);
}

static void gen_al_commit(DisasContext *ctx)
{
    int i;

    for (i = 0; i < 6; i++) {
        TCGLabel *l0 = gen_new_label();
        AlResult *res = &ctx->al_results[i];

        if (!ctx->bundle.als_present[i]) {
            continue;
        }

        if (res->type != AL_RESULT_NONE && ctx->al_cond[i] != NULL) {
            tcg_gen_brcondi_i32(TCG_COND_EQ, ctx->al_cond[i], 0, l0);
        }

        switch (e2k_al_result_type(res->type)) {
        case AL_RESULT_NONE:
            /* %empty */
            break;
        case AL_RESULT_REG:
            /* %rN, %b[N], %gN */
            gen_al_result_commit_reg(ctx, &ctx->bundle2.alops[i], res);
            break;
        case AL_RESULT_PREG:
            /* %predN */
            gen_preg_set_i32(res->preg.index, res->preg.val);
            break;
        case AL_RESULT_CTPR:
            /* %ctprN */
            gen_al_result_commit_ctpr(res);
            break;
        default:
            g_assert_not_reached();
            break;
        }

        gen_set_label(l0);
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

static void gen_aau_result(DisasContext *ctx, Mova *instr, TCGv_i64 dst,
    TCGv_i32 tag)
{
    AauResult *res = &ctx->aau_results[instr->chan];
    res->is_set = true;
    res->index = e2k_get_temp_i32(ctx);
    res->value = dst;
    res->tag = tag;
    gen_reg_index_i32(ctx, res->index, instr->dst);
}

static void gen_checked_ld(DisasContext *ctx, Mova *instr, TCGv ptr)
{
    TCGLabel *l0 = gen_new_label();
    TCGLabel *l1 = gen_new_label();
    TCGv_i32 tag = e2k_get_temp_i32(ctx);
    TCGv_i64 dst = e2k_get_temp_i64(ctx);
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
    tcg_gen_movi_i64(dst, E2K_MOVA_RESULT_INVALID);
    tcg_gen_br(l1);

    /* if address is valid */
    gen_set_label(l0);
    tcg_gen_movi_i32(tag, E2K_TAG_NUMBER64);
    tcg_gen_qemu_ld_i64(dst, ptr, 0, memop);

    gen_set_label(l1);
    gen_aau_result(ctx, instr, dst, tag);
}

static inline void gen_mova_ptr(TCGv ret, Mova *instr)
{
    TCGv_i32 t0 = tcg_const_i32(instr->chan);
    TCGv_i32 t1 = tcg_const_i32(instr->area);
    TCGv_i32 t2 = tcg_const_i32(instr->ind);
    gen_helper_mova_ptr(ret, cpu_env, t0, t1, t2);
    tcg_temp_free_i32(t2);
    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
}

static void gen_mova(DisasContext *ctx, Mova *instr)
{
    /* branch in gen_checked_ld */
    TCGv t5 = tcg_temp_local_new();

    ctx->aau_am[instr->chan] = instr->am ? instr->area : -1;
    // TODO: check ind has proper alignment
    // TODO: check ind is less than mrng
    gen_mova_ptr(t5, instr);

    switch(instr->opc) {
    case 1: /* movab */
    case 2: /* movah */
    case 3: /* movaw */
    case 4: /* movad */
        gen_checked_ld(ctx, instr, t5);
        break;
    case 5: /* movaq */
        e2k_todo_illop(ctx, "movaq");
        break;
    case 7: /* movaqp */
        e2k_todo_illop(ctx, "movaqp");
        break;
    default:
        gen_tr_excp_illopc(ctx);
        break;
    }

    tcg_temp_free(t5);
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
    unsigned int i;

    for (i = 0; i < 4; i++) {
        Mova instr = { 0 };
        AauResult *res = &ctx->aau_results[i];

        instr.chan = i;
        instr.aas = bundle->aas[i + 2];
        instr.dst = extract16(bundle->aas[i / 2], ((i & 1) ^ 1) * 8, 8);

        if (!bundle->aas_present[i + 2] || instr.opc == 0) {
            ctx->aau_am[i] = -1;
            res->is_set = false;
            continue;
        }

        // TODO: invalid value if addr is unaligned
        gen_mova(ctx, &instr);
    }

    /* bap */
    if (ctx->bundle.ss & (1 << 28)) {
        gen_load_prefetch_program(ctx);
    }
}

static void gen_aau_commit(DisasContext *ctx)
{
    unsigned int i;

    for (i = 0; i < 4; i++) {
        AauResult *res = &ctx->aau_results[i];

        // TODO: aau.tags
        if (res->is_set) {
            gen_reg_tag_set_i64(ctx, res->tag, res->index);
            gen_reg_lo_write_i64(res->value, res->index);
        }
    }

    for (i = 0; i < 4; i++) {
        int area = ctx->aau_am[i];
        if (area == -1 || ((i == 1 || i == 3) && ctx->aau_am[i - 1] == area)) {
            continue;
        }
        gen_aau_am(ctx, i, area);
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
    Cs1 *cs1 = &ctx->bundle2.cs1;
    Cs1Setr *setr = &cs1->setr;

    if (cs1->type == CS1_SETR && (setr->type & SETR_VFRPSZ)) {
        e2k_todo_illop(ctx, "vfrpsz");
    }
}

static inline void gen_setwd(DisasContext *ctx)
{
    Cs1 *cs1 = &ctx->bundle2.cs1;
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
    Cs1 *cs1 = &ctx->bundle2.cs1;
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
    Cs1 *cs1 = &ctx->bundle2.cs1;
    Cs1Setr *setr = &cs1->setr;

    if (cs1->type == CS1_SETR && (setr->type & SETR_BP)) {
        tcg_gen_movi_i32(cpu_psize, setr->psz);
        tcg_gen_movi_i32(cpu_pcur, 0);
    }
}

static inline void gen_setr(DisasContext *ctx)
{
    gen_vfrpsz(ctx);
    gen_setwd(ctx);
    gen_setbn(ctx);
    gen_setbp(ctx);
}

static inline void gen_cs1(DisasContext *ctx)
{
    Cs1 *cs1 = &ctx->bundle2.cs1;

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
    Cs0 *cs0 = &ctx->bundle2.cs0;

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
        // TODO: real sdisp target address
        target_ulong target = E2K_FAKE_KERN_START;
        target = deposit64(target, 11, 17, cs0->sdisp.disp);
        uint64_t ctpr = ctpr_new(CTPR_TAG_SDISP, 0, cs0->sdisp.ipd, target);
        gen_set_ctpr(cs0->sdisp.ctpr, ctpr);
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

static inline void gen_ct_cond(DisasContext *ctx)
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
        gen_preg_i32(pcond, ct->psrc);
        break;
    case 0x3:
    case 0x7:
    case 0xe: {
        TCGv_i32 t0 = tcg_temp_new_i32();
        gen_preg_i32(t0, ct->psrc);
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
        gen_is_loop_end_i32(lcond);
        break;
    case 0x5:
    case 0x7:
    case 0xf: { /* #NOT_LOOP_END */
        TCGv_i32 t0 = tcg_temp_new_i32();
        gen_is_loop_end_i32(t0);
        tcg_gen_setcondi_i32(TCG_COND_EQ, lcond, t0, 0);
        tcg_temp_free_i32(t0);
        break;
    }
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

static inline void gen_dec_wrap(TCGv_i32 ret, TCGv_i32 cur, int n,
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

static void gen_dec_sat_i32(TCGv_i32 ret, TCGv_i32 arg0)
{
    TCGv_i32 t0 = tcg_const_i32(0);
    TCGv_i32 t1 = tcg_temp_new_i32();
    tcg_gen_subi_i32(t1, arg0, 1);
    tcg_gen_movcond_i32(TCG_COND_EQ, ret, arg0, t0, arg0, t1);
    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
}

static void gen_advance_loop_counters(void)
{
    TCGv_i32 z = tcg_const_i32(0);
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i32 t1 = tcg_temp_new_i32();
    TCGv_i32 t2 = tcg_temp_new_i32();
    TCGv_i32 t3 = tcg_temp_new_i32();

    gen_dec_sat_i32(cpu_lsr_pcnt, cpu_lsr_pcnt);
    gen_dec_sat_i32(cpu_lsr_lcnt, cpu_lsr_lcnt);
    tcg_gen_setcondi_i32(TCG_COND_EQ, t0, cpu_lsr_pcnt, 0);
    tcg_gen_setcondi_i32(TCG_COND_EQ, t1, cpu_lsr_lcnt, 0);
    tcg_gen_mov_i32(cpu_lsr_over, t1);
    gen_dec_sat_i32(t2, cpu_lsr_ecnt);
    tcg_gen_and_i32(t3, t1, cpu_lsr_vlc);
    tcg_gen_movcond_i32(TCG_COND_NE, cpu_lsr_ecnt, t3, z, t2, cpu_lsr_ecnt);

    tcg_temp_free_i32(t3);
    tcg_temp_free_i32(t2);
    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
    tcg_temp_free_i32(z);
}

static inline void gen_stubs(DisasContext *ctx)
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
        TCGv_i32 t0 = tcg_temp_local_new_i32();
        tcg_gen_addi_i32(t0, cpu_psize, 1);
        gen_cur_dec(ctx, cpu_pcur, abp, cpu_pcur, 1, t0);
        tcg_temp_free_i32(t0);
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

static inline target_ulong do_decode(DisasContext *ctx, CPUState *cs)
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
    alc_decode(ctx);

    return ctx->pc + len;
}

static inline void gen_window_bounds_checki_i32(TCGCond cond, TCGv_i32 arg1,
    int arg2)
{
    TCGLabel *l0 = gen_new_label();
    tcg_gen_brcondi_i32(tcg_invert_cond(cond), arg1, arg2, l0);
    gen_excp_window_bounds();
    gen_set_label(l0);
}

static inline void do_checks(DisasContext *ctx)
{
    const Bundle *b = &ctx->bundle2;
    const Cs1Setr *setr = &b->cs1.setr;

    if (ctx->wd_size != DYNAMIC) {
        /* %rN src static check */
        if (ctx->wd_size <= ctx->max_r_src) {
            gen_tr_excp_window_bounds(ctx);
        }
        /* %rN dst static check */
        if (b->cs1.type == CS1_SETR && (setr->type & SETR_WD)) {
            if (setr->wsz * 2 <= ctx->max_r_dst) {
                gen_tr_excp_window_bounds(ctx);
            }
        } else if (ctx->wd_size <= ctx->max_r_dst) {
            gen_tr_excp_window_bounds(ctx);
        }
    } else if (b->cs1.type == CS1_SETR && (setr->type & SETR_WD)) {
        /* %rN src dynamic check */
        if (ctx->max_r < ctx->max_r_src) {
            ctx->max_r = ctx->max_r_src;
            gen_window_bounds_checki_i32(TCG_COND_LE, cpu_wd_size,
                ctx->max_r_src);
        }

        /* %rN dst static check */
        if (setr->wsz * 2 <= ctx->max_r_dst) {
            gen_tr_excp_window_bounds(ctx);
        }
    } else {
        /* %rN src/dst dynamic check */
        int max = MAX(ctx->max_r_src, ctx->max_r_dst);
        if (ctx->max_r < max) {
            ctx->max_r = max;
            gen_window_bounds_checki_i32(TCG_COND_LE, cpu_wd_size, max);
        }
    }

    if (ctx->bsize != DYNAMIC) {
        /* %b[N] src/dst static check */
        if (ctx->bsize <= ctx->max_b_cur) {
            gen_tr_excp_window_bounds(ctx);
        }
    } else if (ctx->max_b < ctx->max_b_cur) {
        /* %b[N] src/dst dynamic check */
        ctx->max_b = ctx->max_b_cur;
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

/*
 * Executes instructions from a bundle and store the results to
 * temporary buffer.
 */
static inline void gen_ops(DisasContext *ctx)
{
    ctx->loop_mode = (ctx->bundle.hs & (1 << 10)) != 0;

    gen_cs0(ctx);
    gen_cs1(ctx);
    gen_mlock_init(ctx);
    gen_alc(ctx);
    gen_ct_cond(ctx);
    gen_aau(ctx);
    gen_plu(ctx);
}

/*
 * Writes results of instructions from a bundle to the state
 *
 * Note
 * ====
 *
 * Must not generate any exception.
 * */
static inline void do_commit(DisasContext *ctx)
{
    gen_setr(ctx);
    gen_al_commit(ctx);
    gen_aau_commit(ctx);
    gen_plu_commit(ctx);
    gen_stubs(ctx);
}

static inline void do_branch(DisasContext *ctx, target_ulong pc_next)
{
    TCGLabel *l0 = gen_new_label();

    if (ctx->do_check_illtag) {
        tcg_gen_brcondi_i32(TCG_COND_EQ, ctx->illtag, 0, l0);
        gen_excp_illopc();
        gen_set_label(l0);
    }

    if (ctx->ct.type == CT_NONE) {
        gen_save_pc(ctx->base.pc_next);
        return;
    }

    ctx->base.is_jmp = DISAS_NORETURN;

    if (ctx->ct.cond_type > 1) {
        TCGLabel *l0 = gen_new_label();
        tcg_gen_brcondi_i32(TCG_COND_NE, cpu_ct_cond, 0, l0);
        gen_goto_tb(ctx, TB_EXIT_IDX0, ctx->pc, pc_next);
        gen_set_label(l0);
    }

    switch(ctx->ct.type) {
    case CT_IBRANCH:
        gen_goto_tb(ctx, TB_EXIT_IDX1, ctx->pc, ctx->ct.u.target);
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
    default:
        break;
    }
}

static void e2k_tr_init_disas_context(DisasContextBase *db, CPUState *cs)
{
    static int version = -1;
    DisasContext *ctx = container_of(db, DisasContext, base);
    E2KCPU *cpu = E2K_CPU(cs);
    CPUE2KState *env = &cpu->env;

    ctx->version = env->version;

    if (version != ctx->version) {
        if (version > 0) {
            // FIXME: can it happen?
            e2k_todo(ctx, "reinitialize alc map");
        }
        alc_init(ctx);
        version = ctx->version;
    }
}

static bool e2k_tr_breakpoint_check(DisasContextBase *db, CPUState *cs,
                                    const CPUBreakpoint *bp)
{
    DisasContext *ctx = container_of(db, DisasContext, base);

#ifdef CONFIG_USER_ONLY
    if (E2K_FAKE_KERN_START <= bp->pc && bp->pc <= E2K_FAKE_KERN_END) {
        ctx->base.is_jmp = DISAS_TOO_MANY;
        return false;
    } else
#endif
    {
        gen_save_pc(ctx->base.pc_next);
        gen_helper_debug(cpu_env);
        ctx->base.is_jmp = DISAS_NORETURN;
        /*
         * The address covered by the breakpoint must be included in
         * [tb->pc, tb->pc + tb->size) in order to for it to be
         * properly cleared -- thus we increment the PC here so that
         * the logic setting tb->size below does the right thing.
         */
        ctx->base.pc_next += 1;
        return true;
    }
}

static void e2k_tr_tb_start(DisasContextBase *db, CPUState *cs)
{
    DisasContext *ctx = container_of(db, DisasContext, base);
    E2KCPU *cpu = E2K_CPU(cs);
    CPUE2KState *env = &cpu->env;

    ctx->wd_size = DYNAMIC;
    ctx->max_r = -1;
    ctx->max_r_src = -1;
    ctx->max_r_dst = -1;
    ctx->bsize = DYNAMIC;
    ctx->max_b = -1;
    ctx->max_b_cur = -1;

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

    memset(ctx->mas, 0, sizeof(ctx->mas));
    memset(&ctx->bundle2, 0, sizeof(ctx->bundle2));

    ctx->max_r_src = -1;
    ctx->max_r_dst = -1;
    ctx->max_b_cur = -1;

    ctx->mlock = NULL;
    ctx->do_check_illtag = false;
    ctx->illtag = e2k_get_temp_i32(ctx);
    tcg_gen_movi_i32(ctx->illtag, 0);
}

static void e2k_tr_translate_insn(DisasContextBase *db, CPUState *cs)
{
    DisasContext *ctx = container_of(db, DisasContext, base);

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
        gen_helper_syscall(cpu_env);
        tcg_gen_exit_tb(NULL, TB_EXIT_IDX0);
        break;
    case E2K_SYSRET_BACKTRACE_ADDR:
    case E2K_SYSRET_ADDR:
    {
        /* fake return from syscall handler */
        TCGv_i32 t0 = tcg_const_i32(0);

        ctx->base.is_jmp = DISAS_NORETURN;
        gen_helper_prep_return(cpu_ctprs[2], cpu_env, t0);
        gen_helper_return(cpu_env);
        tcg_gen_exit_tb(NULL, TB_EXIT_IDX0);

        tcg_temp_free_i32(t0);
        break;
    }
    case E2K_SIGRET_ADDR:
        /* fake return from signal handler */
        gen_helper_signal_return(cpu_env);
        tcg_gen_exit_tb(NULL, TB_EXIT_IDX0);
        break;
#endif /* CONFIG_USER_ONLY */
    default:
    {
        target_ulong pc_next;

        pc_next = do_decode(ctx, cs);
#ifdef CONFIG_USER_ONLY
        if (ctx->bundle2.cs1.type == CS1_CALL) {
            gen_save_cpu_state(ctx);
            gen_helper_expand_stacks(cpu_env);
        }
#endif /* CONFIG_USER_ONLY */
        gen_ops(ctx);
        do_checks(ctx);
        do_commit(ctx);
        do_branch(ctx, pc_next);
        ctx->base.pc_next = pc_next;
        break;
    }
    }

    /* Free temporary values */
    while(ctx->t32_len) {
        tcg_temp_free_i32(ctx->t32[--ctx->t32_len]);
    }

    while(ctx->t64_len) {
        tcg_temp_free_i64(ctx->t64[--ctx->t64_len]);
    }
}

static void e2k_tr_tb_stop(DisasContextBase *db, CPUState *cs)
{
    DisasContext *ctx = container_of(db, DisasContext, base);

    if (ctx->base.is_jmp == DISAS_TOO_MANY) {
        gen_goto_tb(ctx, TB_EXIT_IDX0, ctx->pc, ctx->base.pc_next);
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
    .breakpoint_check   = e2k_tr_breakpoint_check,
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
        { &cpu_lsr_lcnt, offsetof(CPUE2KState, lsr_lcnt), "lsr_lcnt" },
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
        { &cpu_last_value, offsetof(CPUE2KState, last_value), "last_value" },
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
