#ifndef E2K_TRANSLATE_H
#define E2K_TRANSLATE_H

#include "tcg/tcg-op.h"
#include "exec/translator.h"

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

typedef enum {
    ALES_NONE = 0x00,
    ALES_PRESENT = 0x01,
    ALES_ALLOCATED = 0x02,
} AlesFlag;

typedef struct {
    TCGv_i32 cdi[32];
    TCGv_i64 pib[32];
} CPUE2KAauPrefStateTCG;

typedef struct CPUE2KStateTCG {
    TCGv pc;
    TCGv npc;
    TCGv_i64 ctprs[3];
    TCGv_i32 ct_cond;
    TCGv_i32 is_bp; /* breakpoint flag */
    TCGv_i32 wdbl;
    TCGv_i32 wd_base; /* holds wbs * 2 */
    TCGv_i32 wd_size; /* holds wsz * 2 */
    TCGv_i32 boff; /* holds rbs * 2 */
    TCGv_i32 bsize; /* holds rsz * 2 + 2 */
    TCGv_i32 bcur; /* holds rcur * 2 */
    TCGv_i64 pregs;
    TCGv_i32 psize; /* holds psz */
    TCGv_i32 pcur; /* holds pcur */
    /* lsr */
    TCGv_i32 lsr_lcnt;
    TCGv_i32 lsr_ecnt;
    TCGv_i32 lsr_vlc;
    TCGv_i32 lsr_over;
    TCGv_i32 lsr_pcnt;
    TCGv_i32 lsr_strmd;
    /* AAU */
    TCGv_i32 aasti[16];
    TCGv_i32 aasti_tags;
    TCGv_i32 aaind[16];
    TCGv_i32 aaind_tags;
    TCGv_i32 aaincr[8];
    TCGv_i32 aaincr_tags;
    TCGv_i64 aad_lo[32];
    TCGv_i64 aad_hi[32];
    CPUE2KAauPrefStateTCG aapl, aapr;
} CPUE2KStateTCG;

extern struct CPUE2KStateTCG e2k_cs;

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

typedef struct {
    Cs0 cs0;
    Cs1 cs1;
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
    /* check tag for 32-bit ops if wdbl is set */
    bool check_tag;
    /* poison result if tag is not zero */
    bool poison;
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
    uint8_t dst;
    TCGv_i32 index;
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
    TCGv ttl[8];
    // Allocated temporary values count.
    int t32_len;
    int t64_len;
    int ttl_len;

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

/* exception generated in translation time */
void e2k_tr_gen_exception(DisasContext *dc, int which);
void e2k_tr_gen_exception_no_spill(DisasContext *ctx, int excp);

/* exception generated in runtime */
static inline void e2k_gen_exception(int excp)
{
    TCGv_i32 t0 = tcg_const_i32(excp);

    // TODO: check if need to save state
    gen_helper_raise_exception(cpu_env, t0);

    tcg_temp_free_i32(t0);
}

#define e2k_todo(ctx, fmt, ...) \
    qemu_log("%#lx: " fmt " (%s:%d)\n", ctx->pc, \
        ## __VA_ARGS__, __FILE__, __LINE__)

#define e2k_todo_illop(ctx, fmt, ...) \
    e2k_todo(ctx, fmt, ## __VA_ARGS__); \
    e2k_tr_gen_exception(ctx, E2K_EXCP_ILLOPC)

static inline void e2k_gen_mask_i64(TCGv_i64 ret, TCGv_i64 len)
{
    TCGv_i64 one = tcg_const_i64(1);
    TCGv_i64 t0 = tcg_temp_new_i64();

    tcg_gen_shl_i64(t0, one, len);
    tcg_gen_subi_i64(ret, t0, 1);

    tcg_temp_free_i64(t0);
    tcg_temp_free_i64(one);
}

static inline void e2k_gen_maski_i64(TCGv_i64 ret, int len)
{
    TCGv_i64 t0 = tcg_const_i64(len);
    e2k_gen_mask_i64(ret, t0);
    tcg_temp_free_i64(t0);
}

static inline void e2k_gen_extract_i64(TCGv_i64 ret, TCGv_i64 arg1,
    TCGv_i64 offset, TCGv_i64 len)
{
    TCGv_i64 t0 = tcg_temp_new_i64();
    TCGv_i64 t1 = tcg_temp_new_i64();

    tcg_gen_shr_i64(t0, arg1, offset);
    e2k_gen_mask_i64(t1, len);
    tcg_gen_and_i64(ret, t0, t1);

    tcg_temp_free_i64(t1);
    tcg_temp_free_i64(t0);
}

static inline void e2k_gen_deposit_i64(TCGv_i64 ret, TCGv_i64 arg1,
    TCGv_i64 arg2, TCGv_i64 offset, TCGv_i64 len)
{
    TCGv_i64 t0 = tcg_temp_new_i64();
    TCGv_i64 t1 = tcg_temp_new_i64();
    TCGv_i64 t2 = tcg_temp_new_i64();
    TCGv_i64 t3 = tcg_temp_new_i64();
    TCGv_i64 t4 = tcg_temp_new_i64();
    TCGv_i64 t5 = tcg_temp_new_i64();

    e2k_gen_mask_i64(t0, len);
    tcg_gen_shl_i64(t1, t0, offset);
    tcg_gen_not_i64(t2, t1);
    tcg_gen_and_i64(t3, arg1, t2);
    tcg_gen_and_i64(t4, arg2, t0);
    tcg_gen_shl_i64(t5, t4, offset);
    tcg_gen_or_i64(ret, t3, t5);

    tcg_temp_free_i64(t5);
    tcg_temp_free_i64(t4);
    tcg_temp_free_i64(t3);
    tcg_temp_free_i64(t2);
    tcg_temp_free_i64(t1);
    tcg_temp_free_i64(t0);
}

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

static inline TCGv_i64 e2k_get_const_i64(DisasContext *dc, uint64_t value)
{
    assert(dc->t64_len < ARRAY_SIZE(dc->t64));
    return dc->t64[dc->t64_len++] = tcg_const_local_i64(value);
}

static inline TCGv e2k_get_temp(DisasContext *dc)
{
    assert(dc->ttl_len < ARRAY_SIZE(dc->ttl));
    return dc->ttl[dc->ttl_len++] = tcg_temp_local_new();
}

static inline void e2k_gen_save_pc(target_ulong pc)
{
    tcg_gen_movi_tl(e2k_cs.pc, pc);
}

static inline void e2k_gen_save_cpu_state(DisasContext *ctx)
{
    e2k_gen_save_pc(ctx->pc);
}

static inline void e2k_gen_lcntex(TCGv_i32 ret)
{
    tcg_gen_setcondi_i32(TCG_COND_EQ, ret, e2k_cs.lsr_lcnt, 0);
}

void e2k_gen_store_preg(int idx, TCGv_i32 val);

void e2k_gen_reg_tag_read_i64(TCGv_i32 ret, TCGv_i32 idx);
void e2k_gen_reg_tag_read_i32(TCGv_i32 ret, TCGv_i32 idx);
void e2k_gen_reg_tag_write_i64(TCGv_i32 value, TCGv_i32 idx);
void e2k_gen_reg_tag_write_i32(TCGv_i32 value, TCGv_i32 idx);

static inline void e2k_gen_reg_tag_writei_i64(int value, TCGv_i32 idx)
{
    TCGv_i32 t0 = tcg_const_i32(value);
    e2k_gen_reg_tag_write_i64(t0, idx);
    tcg_temp_free_i32(t0);
}

static inline void e2k_gen_reg_tag_writei_i32(int value, TCGv_i32 idx)
{
    TCGv_i32 t0 = tcg_const_i32(value);
    e2k_gen_reg_tag_write_i32(t0, idx);
    tcg_temp_free_i32(t0);
}

static inline void e2k_gen_reg_tag_extract_lo(TCGv_i32 ret, TCGv_i32 tags)
{
    tcg_gen_andi_i32(ret, tags, GEN_MASK(0, E2K_TAG_SIZE));
}

static inline void e2k_gen_reg_tag_extract_hi(TCGv_i32 ret, TCGv_i32 tags)
{
    tcg_gen_shri_i32(ret, tags, E2K_TAG_SIZE);
}

void e2k_gen_reg_tag_check_i64(TCGv_i32 ret, TCGv_i32 tag);
void e2k_gen_reg_tag_check_i32(TCGv_i32 ret, TCGv_i32 tag);

void e2k_gen_reg_index_from_wregi(TCGv_i32 ret, int idx);
void e2k_gen_reg_index_from_bregi(TCGv_i32 ret, int idx);
void e2k_gen_reg_index_from_gregi(TCGv_i32 ret, int idx);
static inline void e2k_gen_reg_index(DisasContext *ctx, TCGv_i32 ret, uint8_t arg)
{
    if (IS_BASED(arg)) {
        e2k_gen_reg_index_from_bregi(ret, GET_BASED(arg));
    } else if (IS_REGULAR(arg)) {
        e2k_gen_reg_index_from_wregi(ret, GET_REGULAR(arg));
    } else if (IS_GLOBAL(arg)) {
        e2k_gen_reg_index_from_gregi(ret, GET_GLOBAL(arg));
    } else {
        e2k_tr_gen_exception(ctx, E2K_EXCP_ILLOPN);
    }
}

void e2k_gen_reg_read_i64(TCGv_i64 ret, TCGv_i32 idx);
void e2k_gen_reg_read_i32(TCGv_i32 ret, TCGv_i32 idx);
void e2k_gen_reg_write_i64(TCGv_i64 value, TCGv_i32 idx);
void e2k_gen_reg_write_i32(TCGv_i32 value, TCGv_i32 idx);

void e2k_gen_xreg_read_i64(TCGv_i64 ret, TCGv_i32 idx);
void e2k_gen_xreg_read_i32(TCGv_i32 ret, TCGv_i32 idx);
void e2k_gen_xreg_read16u_i32(TCGv_i32 ret, TCGv_i32 idx);
void e2k_gen_xreg_write_i64(TCGv_i64 value, TCGv_i32 idx);
void e2k_gen_xreg_write_i32(TCGv_i32 value, TCGv_i32 idx);
void e2k_gen_xreg_write16u_i32(TCGv_i32 value, TCGv_i32 idx);

void e2k_gen_preg_i32(TCGv_i32 ret, int reg);
void e2k_gen_cond_i32(DisasContext *ctx, TCGv_i32 ret, uint8_t psrc);

static inline void e2k_gen_is_loop_end_i32(TCGv_i32 ret)
{
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i32 t1 = tcg_temp_new_i32();
    TCGv_i32 t2 = tcg_temp_new_i32();

    tcg_gen_setcondi_i32(TCG_COND_EQ, t0, e2k_cs.lsr_ecnt, 0);
    tcg_gen_setcondi_i32(TCG_COND_LTU, t1, e2k_cs.lsr_lcnt, 2);
    tcg_gen_and_i32(t2, t0, t1);
    tcg_gen_and_i32(ret, t2, e2k_cs.lsr_vlc);

    tcg_temp_free_i32(t2);
    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
}

void e2k_decode_jmp(DisasContext *ctx);
void e2k_stubs_commit(DisasContext *ctx);

void alc_init(DisasContext *ctx);
void e2k_alc_execute(DisasContext *ctx);
void e2k_alc_commit(DisasContext *ctx);

void e2k_aau_execute(DisasContext *ctx);
void e2k_aau_commit(DisasContext *ctx);
void e2k_plu_execute(DisasContext *ctx);
void e2k_plu_commit(DisasContext *ctx);

#endif
