#ifndef E2K_TRANSLATE_H
#define E2K_TRANSLATE_H

#include "tcg/tcg-op.h"
#include "exec/translator.h"

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
    TCGv_i64 lsr;
    TCGv_i64 regs[E2K_REG_COUNT];
    TCGv_i64 tags[E2K_TAGS_REG_COUNT];
    TCGv_ptr rptr; /* pointer to wregs */
    TCGv_ptr tptr; /* pointer to wtags */
    TCGv_i32 wd_base; /* holds wbs * 2 */
    TCGv_i32 wd_size; /* holds wsz * 2 */
    TCGv_i32 boff; /* holds rbs * 2 */
    TCGv_i32 bsize; /* holds rsz * 2 + 2 */
    TCGv_i32 bcur; /* holds rcur * 2 */
    TCGv_i64 pregs;
    TCGv_i32 psize; /* holds psz */
    TCGv_i32 pcur; /* holds pcur */
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

typedef struct UnpackedBundle {
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

typedef enum {
    AL_RESULT_NONE,
    AL_RESULT_REG32,
    AL_RESULT_REG64,
    AL_RESULT_PREG,
    AL_RESULT_CTPR32,
    AL_RESULT_CTPR64,
} AlResultType;

typedef struct {
    AlResultType type;
    union {
        struct {
            uint8_t dst;    /* %rN, 1st phase */
            TCGv_i32 index;
            union {
                TCGv_i32 v32;
                TCGv_i64 v64;
            };
            TCGv_i32 tag;
        } reg;
        struct {
            int index;
            TCGv_i64 value;
        } preg, ctpr64;
        struct {
            int index;
            TCGv_i32 value;
        } ctpr32;
    };
} AlResult;

typedef enum {
    AAU_RESULT_NONE,
    AAU_RESULT_REG32,
    AAU_RESULT_REG64,
} AauResultType;

typedef struct {
    AauResultType type;
    uint8_t dst;
    TCGv_i32 index;
    union {
        TCGv_i32 v32;
        TCGv_i64 v64;
    };
} AauResult;

typedef struct {
    bool is_set;
    TCGv_i64 value;
} AlCond;

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
    bool is_branch;
} ControlTransfer;

typedef struct DisasContext {
    DisasContextBase base;
    UnpackedBundle bundle;
    target_ulong pc;
    int jump_ctpr;
    int mmuidx;
    uint8_t mas[6];
    bool loop_mode;
    TCGv_i32 is_prologue;
    TCGv_i32 is_epilogue;

    int version;

    // Temporary values.
    TCGv_i32 t32[64];
    TCGv_i64 t64[32];
    TCGv ttl[8];
    // Allocated temporary values count.
    int t32_len;
    int t64_len;
    int ttl_len;

    /* illegal tag for delayed exception */
    TCGv_i32 illtag;
    TCGv_i64 cond[6];
    AlResult al_results[6];
    AlCond al_cond[6];
    AauResult aau_results[4];
    int aau_am[4];
    PlResult pl_results[3];
    ControlTransfer ct;
} DisasContext;

/* exception generated in translation time */
void e2k_tr_gen_exception(DisasContext *dc, int which);

/* exception generated in runtime */
static inline void e2k_gen_exception(int excp)
{
    TCGv_i32 t0 = tcg_const_i32(excp);

    // TODO: check if need to save state
    gen_helper_raise_exception(cpu_env, t0);

    tcg_temp_free_i32(t0);
}

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

static inline void e2k_gen_lcnt_i64(TCGv_i64 ret)
{
    tcg_gen_andi_i64(ret, e2k_cs.lsr, (1UL << 32) - 1);
}

static inline void e2k_gen_lcnt_i32(TCGv_i32 ret)
{
    TCGv_i64 t0 = tcg_temp_new_i64();

    e2k_gen_lcnt_i64(t0);
    tcg_gen_extrl_i64_i32(ret, t0);
    tcg_temp_free_i64(t0);
}

static inline void e2k_gen_lcnt_set_i32(TCGv_i32 value)
{
    TCGv_i64 t0 = tcg_temp_new_i64();

    tcg_gen_extu_i32_i64(t0, value);
    tcg_gen_deposit_i64(e2k_cs.lsr, e2k_cs.lsr, t0, LSR_LCNT_OFF, LSR_LCNT_LEN);
    tcg_temp_free_i64(t0);
}

static inline void e2k_gen_ecnt_i64(TCGv_i64 ret)
{
    tcg_gen_extract_i64(ret, e2k_cs.lsr, LSR_ECNT_OFF, LSR_ECNT_LEN);
}

static inline void e2k_gen_ecnt_i32(TCGv_i32 ret)
{
    TCGv_i64 t0 = tcg_temp_new_i64();

    e2k_gen_ecnt_i64(t0);
    tcg_gen_extrl_i64_i32(ret, t0);
    tcg_temp_free_i64(t0);
}

static inline void e2k_gen_ecnt_set_i32(TCGv_i32 value)
{
    TCGv_i64 t0 = tcg_temp_new_i64();

    tcg_gen_extu_i32_i64(t0, value);
    tcg_gen_deposit_i64(e2k_cs.lsr, e2k_cs.lsr, t0, LSR_ECNT_OFF, LSR_ECNT_LEN);
    tcg_temp_free_i64(t0);
}

static inline void e2k_gen_pcnt_i32(TCGv_i32 ret)
{
    TCGv_i64 t0 = tcg_temp_new_i64();

    tcg_gen_extract_i64(t0, e2k_cs.lsr, LSR_PCNT_OFF, LSR_PCNT_LEN);
    tcg_gen_extrl_i64_i32(ret, t0);
    tcg_temp_free_i64(t0);
}

static inline void e2k_gen_pcnt_set_i32(TCGv_i32 value)
{
    TCGv_i64 t0 = tcg_temp_new_i64();

    tcg_gen_extu_i32_i64(t0, value);
    tcg_gen_deposit_i64(e2k_cs.lsr, e2k_cs.lsr, t0, LSR_PCNT_OFF, LSR_PCNT_LEN);
    tcg_temp_free_i64(t0);
}

static inline void e2k_gen_lsr_strem_i32(TCGv_i32 ret)
{
    TCGv_i64 t0 = tcg_temp_new_i64();

    tcg_gen_extract_i64(t0, e2k_cs.lsr, LSR_STRMD_OFF, LSR_STRMD_LEN);
    tcg_gen_extrl_i64_i32(ret, t0);
    tcg_temp_free_i64(t0);
}

static inline void e2k_gen_lsr_strem_set_i32(TCGv_i32 value)
{
    TCGv_i64 t0 = tcg_temp_new_i64();

    tcg_gen_extu_i32_i64(t0, value);
    tcg_gen_deposit_i64(e2k_cs.lsr, e2k_cs.lsr, t0, LSR_STRMD_OFF,
        LSR_STRMD_LEN);
    tcg_temp_free_i64(t0);
}

static inline void e2k_gen_lcntex(TCGv_i32 ret)
{
    TCGv_i32 t0 = tcg_temp_new_i32();

    tcg_gen_extrl_i64_i32(t0, e2k_cs.lsr);
    tcg_gen_setcondi_i32(TCG_COND_EQ, ret, t0, 0);

    tcg_temp_free_i32(t0);
}

void e2k_gen_store_preg(int idx, TCGv_i64 val);

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
static inline void e2k_gen_reg_index(TCGv_i32 ret, uint8_t arg)
{
    if (IS_BASED(arg)) {
        e2k_gen_reg_index_from_bregi(ret, GET_BASED(arg));
    } else if (IS_REGULAR(arg)) {
        e2k_gen_reg_index_from_wregi(ret, GET_REGULAR(arg));
    } else if (IS_GLOBAL(arg)) {
        e2k_gen_reg_index_from_gregi(ret, GET_GLOBAL(arg));
    } else {
        e2k_gen_exception(E2K_EXCP_ILLOPN);
    }
}

void e2k_gen_reg_read_i64(TCGv_i64 ret, TCGv_i32 idx);
void e2k_gen_reg_read_i32(TCGv_i32 ret, TCGv_i32 idx);
void e2k_gen_reg_write_i64(TCGv_i64 value, TCGv_i32 idx);
void e2k_gen_reg_write_i32(TCGv_i32 value, TCGv_i32 idx);

void e2k_gen_preg_i64(TCGv_i64 ret, int reg);
void e2k_gen_preg_i32(TCGv_i32 ret, int reg);
TCGv_i64 e2k_get_preg(DisasContext *dc, int reg);
void e2k_gen_cond_i32(DisasContext *ctx, TCGv_i32 ret, uint8_t psrc);

static inline void e2k_gen_cond_i64(DisasContext *ctx, TCGv_i64 ret,
    uint8_t psrc)
{
    TCGv_i32 t0 = tcg_temp_new_i32();

    e2k_gen_cond_i32(ctx, t0, psrc);
    tcg_gen_extu_i32_i64(ret, t0);

    tcg_temp_free_i32(t0);
}

static inline void e2k_gen_is_last_iter(TCGv_i32 ret)
{
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i32 t1 = tcg_temp_new_i32();
    TCGv_i64 t2 = tcg_temp_new_i64();
    TCGv_i32 t3 = tcg_temp_new_i32();

    e2k_gen_lcnt_i32(t0);
    tcg_gen_setcondi_i32(TCG_COND_LTU, t1, t0, 2);
    tcg_gen_andi_i64(t2, e2k_cs.lsr, LSR_VLC_BIT);
    tcg_gen_setcondi_i64(TCG_COND_NE, t2, t2, 0);
    tcg_gen_extrl_i64_i32(t3, t2);
    tcg_gen_and_i32(ret, t1, t3);

    tcg_temp_free_i32(t3);
    tcg_temp_free_i64(t2);
    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
}

static inline void e2k_gen_is_loop_end_i32(TCGv_i32 ret)
{
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i32 t1 = tcg_temp_new_i32();
    TCGv_i32 t2 = tcg_temp_new_i32();

    e2k_gen_ecnt_i32(t0);
    tcg_gen_setcondi_i32(TCG_COND_EQ, t1, t0, 0);
    e2k_gen_is_last_iter(t2);
    tcg_gen_and_i32(ret, t1, t2);

    tcg_temp_free_i32(t2);
    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
}

void e2k_control_execute(DisasContext *ctx);
void e2k_control_window_change(DisasContext *ctx);
void e2k_stubs_commit(DisasContext *ctx);

void e2k_alc_execute(DisasContext *ctx);
void e2k_alc_commit(DisasContext *ctx);

void e2k_aau_execute(DisasContext *ctx);
void e2k_aau_commit(DisasContext *ctx);
void e2k_plu_execute(DisasContext *ctx);
void e2k_plu_commit(DisasContext *ctx);

#endif
