#ifndef E2K_TRANSLATE_H
#define E2K_TRANSLATE_H

#include "tcg/tcg-op.h"
#include "exec/translator.h"

#define STATIC_JUMP DISAS_TARGET_0
#define DYNAMIC_JUMP DISAS_TARGET_1
#define DISAS_CALL DISAS_TARGET_2

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
    CTPR_TAG_RETURN = 0x2,
    CTPR_TAG_DISP = 0x3,
    CTPR_TAG_LDISP = 0x3,
    CTPR_TAG_SDISP = 0x5,
} CtprTag;

typedef enum {
    CTPR_OPC_LDISP = 0x1,
} CtprOpc;

typedef enum {
    ALES_NONE = 0x00,
    ALES_PRESENT = 0x01,
    ALES_ALLOCATED = 0x02,
} AlesFlag;

typedef struct CPUE2KStateTCG {
    TCGv pc;
    TCGv ctprs[3];
    TCGv_i32 wbs;
    TCGv_i32 wsz;
    TCGv_i32 nfx;
    TCGv_i32 dbl;
    TCGv_i64 pcsp_hi;
    TCGv_i64 pcsp_lo;
    TCGv_i64 cr1_hi;
    TCGv_i64 cr1_lo;
    TCGv_i64 lsr;
    TCGv_i32 syscall_wbs;
    TCGv_ptr win_ptr;
    TCGv_i64 wregs[WREGS_SIZE];
    TCGv_i64 gregs[32];
    TCGv_i64 pregs;
    TCGv_i64 usd_lo;
    TCGv_i64 usd_hi;
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

enum ResultType {
    RESULT_NONE,
    RESULT_BASED_REG,
    RESULT_REGULAR_REG,
    RESULT_GLOBAL_REG,
    RESULT_PREG,
};

typedef struct {
    enum ResultType tag;
    union {
        struct {
            unsigned int i;
            TCGv_i64 v;
        } reg;
    } u;
} Result;

typedef struct DisasContext {
    DisasContextBase base;
    UnpackedBundle bundle;
    target_ulong pc;
    target_ulong npc;
    bool is_call;
    int call_ctpr;
    int mmuidx;

    int version;

    // Temporary values.
    TCGv_i32 t32[16];
    TCGv_i64 t64[32];
    TCGv ttl[8];
    // Allocated temporary values count.
    int t32_len;
    int t64_len;
    int ttl_len;

    /* TODO: move to CPUE2KState */
    Result alc[6];
    /* TODO: move to CPUE2KState */
    struct {
        TCGv dest;
        TCGv_i64 cond;
    } jmp;
} DisasContext;

static inline TCGv_i32 e2k_get_temp_i32(DisasContext *dc)
{
    assert(dc->t32_len < ARRAY_SIZE(dc->t32));
    return dc->t32[dc->t32_len++] = tcg_temp_new_i32();
}

static inline TCGv_i64 e2k_get_temp_i64(DisasContext *dc)
{
    assert(dc->t64_len < ARRAY_SIZE(dc->t64));
    return dc->t64[dc->t64_len++] = tcg_temp_new_i64();
}

static inline TCGv e2k_get_temp(DisasContext *dc)
{
    assert(dc->ttl_len < ARRAY_SIZE(dc->ttl));
    return dc->ttl[dc->ttl_len++] = tcg_temp_new();
}

// FIXME: x must not be greater than y * 2
static inline void e2k_gen_wrap_i32(TCGv_i32 ret, TCGv_i32 x, TCGv_i32 y)
{
    TCGv_i32 t0 = tcg_temp_new_i32();

    tcg_gen_sub_i32(t0, x, y);
    tcg_gen_movcond_i32(TCG_COND_LTU, ret, x, y, x, t0);

    tcg_temp_free_i32(t0);
}

// FIXME: x must not be greater than y * 2
static inline void e2k_gen_wrapi_i32(TCGv_i32 ret, TCGv_i32 x, uint32_t y)
{
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i32 t1 = tcg_const_i32(y);

    tcg_gen_sub_i32(t0, x, t1);
    tcg_gen_movcond_i32(TCG_COND_LTU, ret, x, t1, x, t0);

    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
}

// FIXME: x must not be greater than y * 2
static inline void e2k_gen_wrap_i64(TCGv_i64 ret, TCGv_i64 x, TCGv_i64 y)
{
    TCGv_i64 t0 = tcg_temp_new_i64();

    tcg_gen_sub_i64(t0, x, y);
    tcg_gen_movcond_i64(TCG_COND_LTU, ret, x, y, x, t0);

    tcg_temp_free_i64(t0);
}

static inline void e2k_gen_get_field_i64(TCGv_i64 ret, TCGv_i64 val,
    unsigned int start, unsigned int end)
{
    TCGv_i64 t0 = tcg_temp_new_i64();

    tcg_gen_andi_i64(t0, val, GEN_MASK(start, end));
    tcg_gen_shli_i64(ret, t0, start);

    tcg_temp_free_i64(t0);
}

static inline void e2k_gen_set_field_i64(TCGv_i64 ret, TCGv_i64 val,
    uint64_t field, unsigned int start, unsigned int end)
{
    uint64_t mask = GEN_MASK(start, end);
    TCGv_i64 t0 = tcg_const_i64(~mask);
    TCGv_i64 t1 = tcg_const_i64((field << start) & mask);
    TCGv_i64 t2 = tcg_temp_new_i64();

    tcg_gen_and_i64(t2, val, t0);
    tcg_gen_or_i64(ret, t2, t1);

    tcg_temp_free_i64(t2);
    tcg_temp_free_i64(t1);
    tcg_temp_free_i64(t0);
}

static inline void e2k_gen_get_br(TCGv_i32 ret)
{
    TCGv_i64 t0 = tcg_temp_new_i64();

    tcg_gen_extract_i64(t0, e2k_cs.cr1_hi, CR1_HI_BR_OFF, CR1_HI_BR_LEN);
    tcg_gen_extrl_i64_i32(ret, t0);

    tcg_temp_free_i64(t0);
}

static inline void e2k_gen_set_br(TCGv_i32 val)
{
    TCGv_i64 t0 = tcg_temp_new_i64();
    TCGv_i64 t1 = tcg_temp_new_i64();

    tcg_gen_extu_i32_i64(t0, val);
    tcg_gen_deposit_i64(t1, e2k_cs.cr1_hi, t0, CR1_HI_BR_OFF, CR1_HI_BR_LEN);
    tcg_gen_mov_i64(e2k_cs.cr1_hi, t1);

    tcg_temp_free_i64(t1);
    tcg_temp_free_i64(t0);
}

static inline void e2k_gen_lcnt(TCGv_i64 ret)
{
    tcg_gen_andi_i64(ret, e2k_cs.lsr, (1UL << 32) - 1);
}

static inline void e2k_gen_ecnt(TCGv_i64 ret)
{
    tcg_gen_extract_i64(ret, e2k_cs.lsr, LSR_ECNT_OFF, LSR_ECNT_LEN);
}

static inline void e2k_gen_pcnt(TCGv_i32 ret)
{
    TCGv_i64 t0 = tcg_temp_new_i64();

    tcg_gen_extract_i64(t0, e2k_cs.lsr, LSR_PCNT_OFF, LSR_PCNT_LEN);
    tcg_gen_extrl_i64_i32(ret, t0);

    tcg_temp_free_i64(t0);
}

void e2k_gen_preg(TCGv_i64 ret, int reg);
TCGv_i64 e2k_get_preg(DisasContext *dc, int reg);
void e2k_gen_store_preg(int reg, TCGv_i64 val);
TCGv_i64 e2k_get_wreg(DisasContext *dc, int reg);
void e2k_gen_store_wreg(int reg, TCGv_i64 val);
TCGv_i64 e2k_get_breg(DisasContext *dc, int reg);
void e2k_gen_store_breg(int reg, TCGv_i64 val);
TCGv_i64 e2k_get_greg(DisasContext *dc, int reg);
void e2k_gen_store_greg(int reg, TCGv_i64 val);

void e2k_gen_exception(DisasContext *dc, int which);

void e2k_control_gen(DisasContext *dc);

void e2k_alc_gen(DisasContext *dc);
void e2k_alc_commit(DisasContext *dc);

void e2k_win_commit(DisasContext *dc);

#endif
