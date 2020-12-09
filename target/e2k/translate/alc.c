#include "qemu/osdep.h"
#include "qemu.h"
#include "exec/log.h"
#include "translate.h"

typedef struct {
    TCGv_i32 tag;
    TCGv_i64 value;
} Src64;

typedef struct {
    TCGv_i32 tag;
    TCGv_i32 value;
} Src32;

typedef struct {
    int chan;
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

static inline bool is_chan_0235(int c)
{
    return is_chan_03(c) || is_chan_25(c);
}

static inline void gen_reg_i64(DisasContext *ctx, Src64 *ret, uint8_t arg)
{
    TCGv_i32 t0 = tcg_temp_new_i32();

    e2k_gen_reg_index(t0, arg);
    ret->tag = e2k_get_temp_i32(ctx);
    ret->value = e2k_get_temp_i64(ctx);
    e2k_gen_reg_tag_read_i64(ret->tag, t0);
    e2k_gen_reg_read_i64(ret->value, t0);

    tcg_temp_free_i32(t0);
}

static inline void gen_reg_i32(DisasContext *ctx, Src32 *ret, uint8_t arg)
{
    TCGv_i32 t0 = tcg_temp_new_i32();

    e2k_gen_reg_index(t0, arg);
    ret->tag = e2k_get_temp_i32(ctx);
    ret->value = e2k_get_temp_i32(ctx);
    e2k_gen_reg_tag_read_i32(ret->tag, t0);
    e2k_gen_reg_read_i32(ret->value, t0);

    tcg_temp_free_i32(t0);
}

static inline void gen_literal_i64(DisasContext *ctx, Src64 *ret, uint8_t arg)
{
    int i = GET_LIT(arg);
    uint64_t lit = ctx->bundle.lts[i];

    if (!ctx->bundle.lts_present[i]) {
        e2k_gen_exception(E2K_EXCP_ILLOPN);
    } else if (IS_LIT16_LO(arg) && i < 2) {
        lit = ((int64_t) lit << 48) >> 48;
    } else if (IS_LIT16_HI(arg) && i < 2) {
        lit = ((int64_t) lit << 32) >> 48;
    } else if (IS_LIT32(arg)) {
        lit = ((int64_t) lit << 32) >> 32;
    } else if (IS_LIT64(arg) && i < 3) {
        if (!ctx->bundle.lts_present[i + 1]) {
            e2k_tr_gen_exception(ctx, E2K_EXCP_ILLOPN);
        }
        lit |= (uint64_t) ctx->bundle.lts[i + 1] << 32;
    } else {
        e2k_tr_gen_exception(ctx, E2K_EXCP_ILLOPN);
    }

    ret->value = e2k_get_const_i64(ctx, lit);
}

static inline void gen_literal_i32(DisasContext *ctx, Src32 *ret, uint8_t arg)
{
    int i = GET_LIT(arg);
    int32_t lit = ctx->bundle.lts[i];

    if (!ctx->bundle.lts_present[i]) {
        e2k_gen_exception(E2K_EXCP_ILLOPN);
    } else if (IS_LIT16_LO(arg) && i < 2) {
        lit = (lit << 16) >> 16;
    } else if (IS_LIT16_HI(arg) && i < 2) {
        lit = lit >> 16;
    } else if (IS_LIT32(arg)) {
        // nop
    } else if (IS_LIT64(arg) && i < 3) {
        // FIXME: check CPU behavior in such situation
        if (!ctx->bundle.lts_present[i + 1]) {
            e2k_tr_gen_exception(ctx, E2K_EXCP_ILLOPN);
        }
    } else {
        e2k_tr_gen_exception(ctx, E2K_EXCP_ILLOPN);
    }

    ret->value = e2k_get_const_i32(ctx, lit);
}

static inline Src64 get_src1_i64(DisasContext *ctx, int chan)
{
    uint8_t arg = extract32(ctx->bundle.als[chan], 16, 8);
    Src64 ret = { 0 };

    if (IS_IMM5(arg)) {
        ret.value = e2k_get_const_i64(ctx, GET_IMM5(arg));
    } else {
        gen_reg_i64(ctx, &ret, arg);
    }

    return ret;
}

static inline Src32 get_src1_i32(DisasContext *ctx, int chan)
{
    uint8_t arg = extract32(ctx->bundle.als[chan], 16, 8);
    Src32 ret = { 0 };

    if (IS_IMM5(arg)) {
        ret.value = e2k_get_const_i32(ctx, GET_IMM5(arg));
    } else {
        gen_reg_i32(ctx, &ret, arg);
    }

    return ret;
}

static inline Src64 get_src2_i64(DisasContext *ctx, int chan)
{
    uint8_t arg = extract32(ctx->bundle.als[chan], 8, 8);
    Src64 ret = { 0 };

    if (IS_IMM4(arg)) {
        ret.value = e2k_get_const_i64(ctx, GET_IMM4(arg));
    } else if (IS_LIT(arg)) {
        gen_literal_i64(ctx, &ret, arg);
    } else {
        gen_reg_i64(ctx, &ret, arg);
    }

    return ret;
}

static inline Src32 get_src2_i32(DisasContext *ctx, int chan)
{
    uint8_t arg = extract32(ctx->bundle.als[chan], 8, 8);
    Src32 ret = { 0 };

    if (IS_IMM4(arg)) {
        ret.tag = NULL;
        ret.value = e2k_get_const_i32(ctx, GET_IMM4(arg));
    } else if (IS_LIT(arg)) {
        gen_literal_i32(ctx, &ret, arg);
    } else {
        gen_reg_i32(ctx, &ret, arg);
    }

    return ret;
}

static inline Src64 get_src3_i64(DisasContext *ctx, int chan)
{
    uint8_t arg = extract32(ctx->bundle.ales[chan], 0, 8);
    Src64 ret = { 0 };

    gen_reg_i64(ctx, &ret, arg);

    return ret;
}

static inline Src32 get_src3_i32(DisasContext *ctx, int chan)
{
    uint8_t arg = extract32(ctx->bundle.ales[chan], 0, 8);
    Src32 ret = { 0 };

    gen_reg_i32(ctx, &ret, arg);

    return ret;
}

static inline Src64 get_src4_i64(DisasContext *ctx, int chan)
{
    uint8_t arg = extract32(ctx->bundle.als[chan], 0, 8);
    Src64 ret = { 0 };

    gen_reg_i64(ctx, &ret, arg);

    return ret;
}

static inline Src32 get_src4_i32(DisasContext *ctx, int chan)
{
    uint8_t arg = extract32(ctx->bundle.als[chan], 0, 8);
    Src32 ret = { 0 };

    gen_reg_i32(ctx, &ret, arg);

    return ret;
}

/*
 * Returns zero if all @args is zero otherwise returns @tag.
 */
static inline void gen_tag3(TCGv_i32 ret, int tag, TCGv_i32 arg1, TCGv_i32 arg2,
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

static inline void gen_tag2(TCGv_i32 ret, int tag, TCGv_i32 arg1, TCGv_i32 arg2)
{
    gen_tag3(ret, tag, arg1, arg2, NULL);
}

static inline void gen_tag1(TCGv_i32 ret, int tag, TCGv_i32 arg1)
{
    gen_tag2(ret, tag, arg1, NULL);
}

/*
 * Returns zero if all @args is zero otherwise returns tag of non 64-bit number.
 */
#define gen_tag3_i64(ret, a1, a2, a3) \
    gen_tag3((ret), E2K_TAG_NON_NUMBER64, (a1), (a2), (a3))
#define gen_tag2_i64(ret, a1, a2) gen_tag3_i64((ret), (a1), (a2), NULL)
#define gen_tag1_i64(ret, a1) gen_tag2_i64((ret), (a1), NULL)

/*
 * Returns zero if all @args is zero otherwise returns tag of non 32-bit number.
 */
#define gen_tag3_i32(ret, a1, a2, a3) \
    gen_tag3((ret), E2K_TAG_NON_NUMBER32, (a1), (a2), (a3))
#define gen_tag2_i32(ret, a1, a2) gen_tag3_i32((ret), (a1), (a2), NULL)
#define gen_tag1_i32(ret, a1) gen_tag2_i32((ret), (a1), NULL)

static inline void gen_tag_check(DisasContext *ctx, bool sm, TCGv_i32 tag)
{
    if (!sm && tag != NULL) {
        tcg_gen_or_i32(ctx->illtag, ctx->illtag, tag);
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

static inline void set_al_result_reg64_tag(DisasContext *ctx, int chan,
    TCGv_i64 value, TCGv_i32 tag)
{
    uint8_t arg = extract32(ctx->bundle.als[chan], 0, 8);
    AlResult *res = &ctx->al_results[chan];

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
        res->ctpr64.index = (arg & 3) - 1;
        res->ctpr64.value = value;
    } else {
        res->type = AL_RESULT_REG64;
        res->reg.v64 = value;
        res->reg.tag = tag;
        res->reg.index = e2k_get_temp_i32(ctx);
        if (IS_REGULAR(arg)) {
            res->reg.dst = arg;
        } else {
            res->reg.dst = 0;
            e2k_gen_reg_index(res->reg.index, arg);
        }
    }
}

static inline void set_al_result_reg64(DisasContext *ctx, int chan,
    TCGv_i64 value)
{
    set_al_result_reg64_tag(ctx, chan, value, e2k_get_const_i32(ctx, 0));
}

static inline void set_al_result_reg32_tag(DisasContext *ctx, int chan,
    TCGv_i32 value, TCGv_i32 tag)
{
    uint8_t arg = extract32(ctx->bundle.als[chan], 0, 8);
    AlResult *res = &ctx->al_results[chan];

    // TODO: %tst, %tc, %tcd
    if (arg == 0xdf) { /* %empty */
        res->type = AL_RESULT_NONE;
        res->reg.index = NULL;
        res->reg.v32 = NULL;
        res->reg.tag = NULL;
    } else if ((arg & 0xfc) == 0xd0 && (arg & 3) != 0) {
        // TODO: check if instruction can write to ctpr
        res->type = AL_RESULT_CTPR32;
        res->ctpr32.index = (arg & 3) - 1;
        res->ctpr32.value = value;
    } else {
        res->type = AL_RESULT_REG32;
        res->reg.v32 = value;
        res->reg.tag = tag;
        res->reg.index = e2k_get_temp_i32(ctx);
        if (IS_REGULAR(arg)) {
            res->reg.dst = arg;
        } else {
            res->reg.dst = 0;
            e2k_gen_reg_index(res->reg.index, arg);
        }
    }
}

static inline void set_al_result_reg32(DisasContext *ctx, int chan,
    TCGv_i32 value)
{
    set_al_result_reg32_tag(ctx, chan, value, e2k_get_const_i32(ctx, 0));
}

static inline void set_al_result_preg(DisasContext *ctx, int chan, int index,
    TCGv_i64 value)
{
    AlResult *res = &ctx->al_results[chan];

    res->type = AL_RESULT_PREG;
    res->preg.index = index;
    res->preg.value = value;
}

static inline void gen_al_result_i64(DisasContext *ctx, int chan, TCGv_i64 dst,
    TCGv_i32 tag)
{
    bool sm = extract32(ctx->bundle.als[chan], 31, 1);
    gen_tag_check(ctx, sm, tag);
    gen_dst_poison_i64(dst, dst, tag);
    set_al_result_reg64_tag(ctx, chan, dst, tag);
}

static inline void gen_al_result_i32(DisasContext *ctx, int chan, TCGv_i32 dst,
    TCGv_i32 tag)
{
    bool sm = extract32(ctx->bundle.als[chan], 31, 1);
    gen_tag_check(ctx, sm, tag);
    gen_dst_poison_i32(dst, dst, tag);
    set_al_result_reg32_tag(ctx, chan, dst, tag);
}

static inline bool is_mrgc(uint16_t rlp, int chan)
{
    int is_mrgc = GET_BIT(rlp, 15);
    int cluster = GET_BIT(rlp, 14);
    int alc_mask = GET_FIELD(rlp, 10, 3);
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

static inline bool is_cond(uint16_t rlp, int chan)
{
    int is_mrgc = GET_BIT(rlp, 15);
    int cluster = GET_BIT(rlp, 14);
    int alc_mask = GET_FIELD(rlp, 10, 3);
    int alc = GET_BIT(alc_mask, chan % 3);

    return !is_mrgc && (cluster == (chan > 2)) && (alc != 0);
}

static uint16_t find_cond(DisasContext *ctx, int chan)
{
    unsigned int i;

    for (i = 0; ctx->bundle.cds_present[i]; i++) {
        uint32_t cds = ctx->bundle.cds[i];
        uint16_t rlp0 = cds >> 16;
        uint16_t rlp1 = cds & 0xffff;

        if (is_cond(rlp0, chan)) {
            return rlp0;
        }
        if (is_cond(rlp1, chan)) {
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

    e2k_gen_cond_i32(ctx, t0, extract16(rlp, 0, 7));
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

            e2k_gen_cond_i32(ctx, t0, psrc);
            tcg_gen_setcondi_i32(TCG_COND_EQ, ret, t0, 0);

            tcg_temp_free_i32(t0);
        } else {
            e2k_gen_cond_i32(ctx, ret, psrc);
        }
    } else {
        /* Undefined behavior if MRGC is not provided but CPU returns src2. */
        tcg_gen_movi_i32(ret, 0);
    }
}

static void gen_loop_mode_st(DisasContext *ctx, TCGLabel *l)
{
    if (ctx->loop_mode) {
        TCGLabel *l0 = gen_new_label();
        TCGv_i32 t0 = tcg_temp_local_new_i32();

        tcg_gen_brcondi_i32(TCG_COND_EQ, ctx->is_prologue, 1, l);
        e2k_gen_is_loop_end_i32(t0);
        tcg_gen_brcondi_i32(TCG_COND_EQ, t0, 0, l0);
        e2k_gen_lsr_strem_i32(t0);
        tcg_gen_brcondi_i32(TCG_COND_EQ, t0, 0, l);
        tcg_gen_subi_i32(t0, t0, 1);
        e2k_gen_lsr_strem_set_i32(t0);
        gen_set_label(l0);
    }
}

static inline void gen_andn_i32(TCGv_i32 ret, TCGv_i32 src1, TCGv_i32 src2)
{
    TCGv_i32 t0 = tcg_temp_new_i32();
    tcg_gen_not_i32(t0, src2);
    tcg_gen_and_i32(ret, src1, t0);
    tcg_temp_free_i32(t0);
}

static inline void gen_andn_i64(TCGv_i64 ret, TCGv_i64 src1, TCGv_i64 src2)
{
    TCGv_i64 t0 = tcg_temp_new_i64();
    tcg_gen_not_i64(t0, src2);
    tcg_gen_and_i64(ret, src1, t0);
    tcg_temp_free_i64(t0);
}

static inline void gen_orn_i32(TCGv_i32 ret, TCGv_i32 src1, TCGv_i32 src2)
{
    TCGv_i32 t0 = tcg_temp_new_i32();
    tcg_gen_not_i32(t0, src2);
    tcg_gen_or_i32(ret, src1, t0);
    tcg_temp_free_i32(t0);
}

static inline void gen_orn_i64(TCGv_i64 ret, TCGv_i64 src1, TCGv_i64 src2)
{
    TCGv_i64 t0 = tcg_temp_new_i64();
    tcg_gen_not_i64(t0, src2);
    tcg_gen_or_i64(ret, src1, t0);
    tcg_temp_free_i64(t0);
}

static inline void gen_xorn_i32(TCGv_i32 ret, TCGv_i32 src1, TCGv_i32 src2)
{
    TCGv_i32 t0 = tcg_temp_new_i32();
    tcg_gen_not_i32(t0, src2);
    tcg_gen_xor_i32(ret, src1, t0);
    tcg_temp_free_i32(t0);
}

static inline void gen_xorn_i64(TCGv_i64 ret, TCGv_i64 src1, TCGv_i64 src2)
{
    TCGv_i64 t0 = tcg_temp_new_i64();
    tcg_gen_not_i64(t0, src2);
    tcg_gen_xor_i64(ret, src1, t0);
    tcg_temp_free_i64(t0);
}

static inline void gen_getfield_i32(TCGv_i32 ret, TCGv_i32 src1,
    TCGv_i32 shift, TCGv_i32 size, TCGv_i32 sign)
{
    TCGv_i32 n32 = tcg_const_i32(32);
    TCGv_i32 n1 = tcg_const_i32(1);
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i32 t1 = tcg_temp_new_i32();
    TCGv_i32 t2 = tcg_temp_new_i32();
    TCGv_i32 t3 = tcg_temp_new_i32();
    TCGv_i32 t4 = tcg_temp_new_i32();
    TCGv_i32 t5 = tcg_temp_new_i32();

    tcg_gen_add_i32(t0, shift, size);
    tcg_gen_sub_i32(t1, n32, t0);
    tcg_gen_add_i32(t2, shift, t1);
    tcg_gen_shl_i32(t3, src1, t1);
    tcg_gen_shr_i32(t4, t3, t2);
    tcg_gen_sar_i32(t5, t3, t2);
    tcg_gen_movcond_i32(TCG_COND_NE, ret, sign, n1, t4, t5);

    tcg_temp_free_i32(t5);
    tcg_temp_free_i32(t4);
    tcg_temp_free_i32(t3);
    tcg_temp_free_i32(t2);
    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
    tcg_temp_free_i32(n1);
    tcg_temp_free_i32(n32);
}

static inline void gen_getfield_i64(TCGv_i64 ret, TCGv_i64 src1,
    TCGv_i64 shift, TCGv_i64 size, TCGv_i64 sign)
{
    TCGv_i64 n64 = tcg_const_i64(64);
    TCGv_i64 n1 = tcg_const_i64(1);
    TCGv_i64 t0 = tcg_temp_new_i64();
    TCGv_i64 t1 = tcg_temp_new_i64();
    TCGv_i64 t2 = tcg_temp_new_i64();
    TCGv_i64 t3 = tcg_temp_new_i64();
    TCGv_i64 t4 = tcg_temp_new_i64();
    TCGv_i64 t5 = tcg_temp_new_i64();

    tcg_gen_add_i64(t0, shift, size);
    tcg_gen_sub_i64(t1, n64, t0);
    tcg_gen_add_i64(t2, shift, t1);
    tcg_gen_shl_i64(t3, src1, t1);
    tcg_gen_shr_i64(t4, t3, t2);
    tcg_gen_sar_i64(t5, t3, t2);
    tcg_gen_movcond_i64(TCG_COND_NE, ret, sign, n1, t4, t5);

    tcg_temp_free_i64(t5);
    tcg_temp_free_i64(t4);
    tcg_temp_free_i64(t3);
    tcg_temp_free_i64(t2);
    tcg_temp_free_i64(t1);
    tcg_temp_free_i64(t0);
    tcg_temp_free_i64(n1);
    tcg_temp_free_i64(n64);
}

static inline void gen_getfs(TCGv_i32 ret, TCGv_i32 src1, TCGv_i32 src2)
{
    TCGv_i32 shift = tcg_temp_new_i32();
    TCGv_i32 size = tcg_temp_new_i32();
    TCGv_i32 sign = tcg_temp_new_i32();

    tcg_gen_extract_i32(shift, src2, 0, 5);
    tcg_gen_extract_i32(size, src2, 6, 5);
    tcg_gen_extract_i32(sign, src2, 12, 1);
    gen_getfield_i32(ret, src1, shift, size, sign);

    tcg_temp_free_i32(sign);
    tcg_temp_free_i32(size);
    tcg_temp_free_i32(shift);
}

static inline void gen_getfd(TCGv_i64 ret, TCGv_i64 src1, TCGv_i64 src2)
{
    TCGv_i64 shift = tcg_temp_new_i64();
    TCGv_i64 size = tcg_temp_new_i64();
    TCGv_i64 sign = tcg_temp_new_i64();

    tcg_gen_extract_i64(shift, src2, 0, 6);
    tcg_gen_extract_i64(size, src2, 6, 6);
    tcg_gen_extract_i64(sign, src2, 12, 1);
    gen_getfield_i64(ret, src1, shift, size, sign);

    tcg_temp_free_i64(sign);
    tcg_temp_free_i64(size);
    tcg_temp_free_i64(shift);
}

static void gen_bitrevs(TCGv_i32 ret, TCGv_i32 src1) {
    TCGv_i32 ltemp0 = tcg_temp_new_i32();
    TCGv_i32 rtemp0 = tcg_temp_new_i32();
    TCGv_i32 ltemp1 = tcg_temp_new_i32();
    TCGv_i32 rtemp1 = tcg_temp_new_i32();
    
    tcg_gen_bswap32_i32(ret, src1);
    
    tcg_gen_andi_i32(ltemp0, ret, 0xf0f0f0f0);
    tcg_gen_andi_i32(rtemp0, ret, 0x0f0f0f0f);
    tcg_gen_shri_i32(ltemp1, ltemp0, 4);
    tcg_gen_shli_i32(rtemp1, rtemp0, 4);
    tcg_gen_or_i32(ret, ltemp1, rtemp1);
    
    tcg_gen_andi_i32(ltemp0, ret, 0xcccccccc);
    tcg_gen_andi_i32(rtemp0, ret, 0x33333333);
    tcg_gen_shri_i32(ltemp1, ltemp0, 2);
    tcg_gen_shli_i32(rtemp1, rtemp0, 2);
    tcg_gen_or_i32(ret, ltemp1, rtemp1);
    
    tcg_gen_andi_i32(ltemp0, ret, 0xaaaaaaaa);
    tcg_gen_andi_i32(rtemp0, ret, 0x55555555);
    tcg_gen_shri_i32(ltemp1, ltemp0, 1);
    tcg_gen_shli_i32(rtemp1, rtemp0, 1);
    tcg_gen_or_i32(ret, ltemp1, rtemp1);
    
    tcg_temp_free_i32(rtemp1);
    tcg_temp_free_i32(ltemp1);
    tcg_temp_free_i32(rtemp0);
    tcg_temp_free_i32(ltemp0);
}

static void gen_bitrevd(TCGv_i64 ret, TCGv_i64 src1) {
    TCGv_i64 ltemp0 = tcg_temp_new_i64();
    TCGv_i64 rtemp0 = tcg_temp_new_i64();
    TCGv_i64 ltemp1 = tcg_temp_new_i64();
    TCGv_i64 rtemp1 = tcg_temp_new_i64();
    
    tcg_gen_bswap64_i64(ret, src1);
    
    tcg_gen_andi_i64(ltemp0, ret, 0xf0f0f0f0f0f0f0f0);
    tcg_gen_andi_i64(rtemp0, ret, 0x0f0f0f0f0f0f0f0f);
    tcg_gen_shri_i64(ltemp1, ltemp0, 4);
    tcg_gen_shli_i64(rtemp1, rtemp0, 4);
    tcg_gen_or_i64(ret, ltemp1, rtemp1);
    
    tcg_gen_andi_i64(ltemp0, ret, 0xcccccccccccccccc);
    tcg_gen_andi_i64(rtemp0, ret, 0x3333333333333333);
    tcg_gen_shri_i64(ltemp1, ltemp0, 2);
    tcg_gen_shli_i64(rtemp1, rtemp0, 2);
    tcg_gen_or_i64(ret, ltemp1, rtemp1);
    
    tcg_gen_andi_i64(ltemp0, ret, 0xaaaaaaaaaaaaaaaa);
    tcg_gen_andi_i64(rtemp0, ret, 0x5555555555555555);
    tcg_gen_shri_i64(ltemp1, ltemp0, 1);
    tcg_gen_shli_i64(rtemp1, rtemp0, 1);
    tcg_gen_or_i64(ret, ltemp1, rtemp1);
    
    tcg_temp_free_i64(rtemp1);
    tcg_temp_free_i64(ltemp1);
    tcg_temp_free_i64(rtemp0);
    tcg_temp_free_i64(ltemp0);
}

static void gen_lzcnts(TCGv_i32 ret, TCGv_i32 src1) {
    tcg_gen_clzi_i32(ret, src1, 32);
}

static void gen_lzcntd(TCGv_i64 ret, TCGv_i64 src1) {
    tcg_gen_clzi_i64(ret, src1, 64);
}

static TCGCond e2k_gen_cmp_op(unsigned int cmp_op)
{
    switch(cmp_op) {
    case 0: abort(); break;
    case 1: return TCG_COND_LTU; break;
    case 2: return TCG_COND_EQ; break;
    case 3: return TCG_COND_LEU; break;
    case 4: abort(); break;
    case 5: abort(); break;
    case 6: return TCG_COND_LT; break;
    case 7: return TCG_COND_LE; break;
    default: g_assert_not_reached(); break;
    }

    return TCG_COND_NEVER; /* unreachable */
}

static inline void gen_cmp_i32(TCGv_i32 ret, int opc,
    TCGv_i32 src1, TCGv_i32 src2)
{
    TCGCond cond = e2k_gen_cmp_op(opc);
    tcg_gen_setcond_i32(cond, ret, src1, src2);
}

static inline void gen_cmp_i64(TCGv_i64 ret, int opc, TCGv_i64 src1,
    TCGv_i64 src2)
{
    TCGCond cond = e2k_gen_cmp_op(opc);
    tcg_gen_setcond_i64(cond, ret, src1, src2);
}

static inline void gen_cmpand_i64(TCGv_i64 ret, int opc, TCGv_i64 src1,
    TCGv_i64 src2)
{
    TCGv_i64 t0 = tcg_temp_new_i64();
    tcg_gen_and_i64(t0, src1, src2);

    switch (opc) {
    case 2: /* is zero */
        tcg_gen_setcondi_i64(TCG_COND_EQ, ret, t0, 0);
        break;
    case 4: /* is negative */
        tcg_gen_setcondi_i64(TCG_COND_LT, ret, t0, 0);
        break;
    case 5: { /* is odd and greater than 3 */
        TCGv_i64 t1 = tcg_temp_new_i64();
        TCGv_i64 t2 = tcg_temp_new_i64();
        TCGv_i64 t3 = tcg_temp_new_i64();

        tcg_gen_andi_i64(t1, t0, 1);
        tcg_gen_setcondi_i64(TCG_COND_NE, t2, t1, 0);
        tcg_gen_setcondi_i64(TCG_COND_GTU, t3, t0, 2);
        tcg_gen_setcond_i64(TCG_COND_EQ, ret, t2, t3);

        tcg_temp_free_i64(t3);
        tcg_temp_free_i64(t2);
        tcg_temp_free_i64(t1);
        break;
    }
    case 7: /* signed less or equal 0 */
        tcg_gen_setcondi_i64(TCG_COND_LE, ret, t0, 0);
        break;
    default:
        e2k_gen_exception(E2K_EXCP_ILLOPC);
        break;
    }

    tcg_temp_free_i64(t0);
}

static inline void gen_cmpand_i32(TCGv_i32 ret, int opc, TCGv_i32 src1,
    TCGv_i32 src2)
{
    TCGv_i32 t0 = tcg_temp_new_i32();
    tcg_gen_and_i32(t0, src1, src2);

    switch (opc) {
    case 2: /* is zero */
        tcg_gen_setcondi_i32(TCG_COND_EQ, ret, t0, 0);
        break;
    case 4: /* is negative */
        tcg_gen_setcondi_i32(TCG_COND_LT, ret, t0, 0);
        break;
    case 5: { /* is odd and greater than 3 */
        TCGv_i32 t1 = tcg_temp_new_i32();
        TCGv_i32 t2 = tcg_temp_new_i32();
        TCGv_i32 t3 = tcg_temp_new_i32();

        tcg_gen_andi_i32(t1, t0, 1);
        tcg_gen_setcondi_i32(TCG_COND_NE, t2, t1, 0);
        tcg_gen_setcondi_i32(TCG_COND_GTU, t3, t0, 2);
        tcg_gen_setcond_i32(TCG_COND_EQ, ret, t2, t3);

        tcg_temp_free_i32(t3);
        tcg_temp_free_i32(t2);
        tcg_temp_free_i32(t1);
        break;
    }
    case 7: /* signed less or equal 0 */
        tcg_gen_setcondi_i32(TCG_COND_LE, ret, t0, 0);
        break;
    default:
        e2k_gen_exception(E2K_EXCP_ILLOPC);
        break;
    }

    tcg_temp_free_i32(t0);
}

/*
 * ret[31:0] = x[31:0]
 * ret[63:32] = y[63:32]
 */
static inline void gen_movehl_i64(TCGv_i64 ret, TCGv_i64 x, TCGv_i64 y)
{
    TCGv_i32 lo = tcg_temp_new_i32();
    TCGv_i32 hi = tcg_temp_new_i32();

    tcg_gen_extrl_i64_i32(lo, x);
    tcg_gen_extrh_i64_i32(hi, y);
    tcg_gen_concat_i32_i64(ret, lo, hi);

    tcg_temp_free_i32(hi);
    tcg_temp_free_i32(lo);
}

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

static inline void gen_udivd(TCGv_i64 ret, TCGv_i32 ret_tag, TCGv_i32 tag,
    TCGv_i64 src1, TCGv_i64 src2)
{
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
}

static inline void gen_udivs(TCGv_i32 ret, TCGv_i32 ret_tag, TCGv_i32 tag,
    TCGv_i32 src1, TCGv_i32 src2)
{
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
}

static inline void gen_sdivd(TCGv_i64 ret, TCGv_i32 ret_tag, TCGv_i32 tag,
    TCGv_i64 src1, TCGv_i64 src2)
{
    TCGLabel *l0 = gen_new_label();
    TCGLabel *l1 = gen_new_label();
    tcg_gen_brcondi_i64(TCG_COND_NE, src2, 0, l1);
    tcg_gen_movi_i32(ret_tag, 5);
    tcg_gen_movi_i64(ret, 0);
    tcg_gen_br(l0);
    gen_set_label(l1);
    tcg_gen_mov_i32(ret_tag, tag);
    tcg_gen_div_i64(ret, src1, src2);
    gen_set_label(l0);
}

static inline void gen_sdivs(TCGv_i32 ret, TCGv_i32 ret_tag, TCGv_i32 tag,
    TCGv_i32 src1, TCGv_i32 src2)
{
    TCGLabel *l0 = gen_new_label();
    TCGLabel *l1 = gen_new_label();
    tcg_gen_brcondi_i32(TCG_COND_NE, src2, 0, l1);
    tcg_gen_movi_i32(ret_tag, 5);
    tcg_gen_movi_i32(ret, 0);
    tcg_gen_br(l0);
    gen_set_label(l1);
    tcg_gen_mov_i32(ret_tag, tag);
    tcg_gen_div_i32(ret, src1, src2);
    gen_set_label(l0);
}

static inline void gen_gettag_i64(DisasContext *ctx, int chan)
{
    Src64 s2 = get_src2_i64(ctx, chan);
    TCGv_i64 dst = e2k_get_temp_i64(ctx);

    if (s2.tag != NULL) {
        tcg_gen_extu_i32_i64(dst, s2.tag);
    } else {
        tcg_gen_movi_i64(dst, 0);
    }

    set_al_result_reg64(ctx, chan, dst);
}

static inline void gen_gettag_i32(DisasContext *ctx, int chan)
{
    Src32 s2 = get_src2_i32(ctx, chan);
    TCGv_i32 dst = e2k_get_temp_i32(ctx);

    if (s2.tag != NULL) {
        tcg_gen_mov_i32(dst, s2.tag);
    } else {
        tcg_gen_movi_i32(dst, 0);
    }

    set_al_result_reg32(ctx, chan, dst);
}

static inline void gen_puttag_i64(DisasContext *ctx, int chan)
{
    bool sm = extract32(ctx->bundle.als[chan], 31, 1);
    TCGLabel *l0 = gen_new_label();
    TCGLabel *l1 = gen_new_label();
    Src64 s1 = get_src1_i64(ctx, chan);
    Src32 s2 = get_src2_i32(ctx, chan);
    TCGv_i32 tag = e2k_get_temp_i32(ctx);
    TCGv_i64 dst = e2k_get_temp_i64(ctx);

    tcg_gen_mov_i64(dst, s1.value);
    tcg_gen_brcondi_i32(TCG_COND_EQ, s2.value, 0, l0);
    gen_tag2_i64(tag, s1.tag, s2.tag);
    tcg_gen_brcondi_i32(TCG_COND_EQ, tag, 0, l0);
    gen_tag_check(ctx, sm, tag);
    gen_dst_poison_i64(dst, dst, tag);
    tcg_gen_br(l1);
    gen_set_label(l0);
    tcg_gen_mov_i32(tag, s2.value);
    gen_set_label(l1);
    set_al_result_reg64_tag(ctx, chan, dst, tag);
}

static inline void gen_puttag_i32(DisasContext *ctx, int chan)
{
    bool sm = extract32(ctx->bundle.als[chan], 31, 1);
    TCGLabel *l0 = gen_new_label();
    TCGLabel *l1 = gen_new_label();
    Src32 s1 = get_src1_i32(ctx, chan);
    Src32 s2 = get_src2_i32(ctx, chan);
    TCGv_i32 tag = e2k_get_temp_i32(ctx);
    TCGv_i32 dst = e2k_get_temp_i32(ctx);

    tcg_gen_mov_i32(dst, s1.value);
    tcg_gen_brcondi_i32(TCG_COND_EQ, s2.value, 0, l0);
    gen_tag2_i32(tag, s1.tag, s2.tag);
    tcg_gen_brcondi_i32(TCG_COND_EQ, tag, 0, l0);
    gen_tag_check(ctx, sm, tag);
    gen_dst_poison_i32(dst, dst, tag);
    tcg_gen_br(l1);
    gen_set_label(l0);
    tcg_gen_mov_i32(tag, s2.value);
    gen_set_label(l1);
    set_al_result_reg32_tag(ctx, chan, dst, tag);
}

static inline void gen_insert_field_i64(TCGv_i64 ret, TCGv_i64 src1,
    TCGv_i64 src2, TCGv_i64 src3)
{
    TCGv_i64 one = tcg_const_i64(1);
    TCGv_i64 offset = tcg_temp_new_i64();
    TCGv_i64 len = tcg_temp_new_i64();
    TCGv_i64 t0 = tcg_temp_new_i64();
    TCGv_i64 t1 = tcg_temp_new_i64();
    TCGv_i64 t2 = tcg_temp_new_i64();
    TCGv_i64 t3 = tcg_temp_new_i64();
    TCGv_i64 t4 = tcg_temp_new_i64();
    TCGv_i64 t5 = tcg_temp_new_i64();

    tcg_gen_extract_i64(offset, src2, 0, 6);
    tcg_gen_extract_i64(len, src2, 6, 6);
    tcg_gen_shl_i64(t0, one, len);
    tcg_gen_subi_i64(t1, t0, 1);
    tcg_gen_rotr_i64(t2, src1, offset);
    tcg_gen_not_i64(t3, t1);
    tcg_gen_and_i64(t4, t2, t3);
    tcg_gen_and_i64(t5, src3, t1);
    tcg_gen_or_i64(ret, t4, t5);

    tcg_temp_free_i64(t5);
    tcg_temp_free_i64(t4);
    tcg_temp_free_i64(t3);
    tcg_temp_free_i64(t2);
    tcg_temp_free_i64(t1);
    tcg_temp_free_i64(t0);
    tcg_temp_free_i64(len);
    tcg_temp_free_i64(offset);
    tcg_temp_free_i64(one);
}

static inline void gen_insert_field_i32(TCGv_i32 ret, TCGv_i32 src1,
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

static inline void gen_rr_i64(DisasContext *ctx, int chan)
{
    uint32_t als = ctx->bundle.als[chan];
    uint8_t state_reg = extract32(als, 16, 8);
    TCGv_i64 dst = e2k_get_temp_i64(ctx);
    TCGv_i32 t0 = tcg_const_i32(state_reg);

    gen_helper_state_reg_read_i64(dst, cpu_env, t0);
    set_al_result_reg64(ctx, chan, dst);
    tcg_temp_free_i32(t0);
}

static inline void gen_rr_i32(DisasContext *ctx, int chan)
{
    uint32_t als = ctx->bundle.als[chan];
    uint8_t state_reg = extract32(als, 16, 8);
    TCGv_i32 dst = e2k_get_temp_i32(ctx);
    TCGv_i32 t0 = tcg_const_i32(state_reg);

    gen_helper_state_reg_read_i32(dst, cpu_env, t0);
    set_al_result_reg32(ctx, chan, dst);
    tcg_temp_free_i32(t0);
}

static inline void gen_rw_i64(DisasContext *ctx, int chan)
{
    uint32_t als = ctx->bundle.als[chan];
    Src64 s2 = get_src2_i64(ctx, chan);
    TCGv_i32 t0 = tcg_const_i32(als & 0xff);

    gen_tag_check(ctx, als >> 31, s2.tag);
    gen_helper_state_reg_write_i64(cpu_env, t0, s2.value);
    tcg_temp_free_i32(t0);
}

static inline void gen_rw_i32(DisasContext *ctx, int chan)
{
    uint32_t als = ctx->bundle.als[chan];
    Src32 s2 = get_src2_i32(ctx, chan);
    TCGv_i32 t0 = tcg_const_i32(als & 0xff);

    gen_tag_check(ctx, als >> 31, s2.tag);
    gen_helper_state_reg_write_i32(cpu_env, t0, s2.value);
    tcg_temp_free_i32(t0);
}

static void gen_sxt(DisasContext *ctx, int chan)
{
    Src64 s1 = get_src1_i64(ctx, chan);
    Src32 s2 = get_src2_i32(ctx, chan);
    TCGv_i32 tag = e2k_get_temp_i32(ctx);
    TCGv_i64 dst = e2k_get_temp_i64(ctx);

    gen_tag2_i64(tag, s1.tag, s2.tag);
    gen_helper_sxt(dst, s1.value, s2.value);
    gen_al_result_i64(ctx, chan, dst, tag);
}

static void gen_getsp(DisasContext *ctx, int chan)
{
    Src32 s2 = get_src2_i32(ctx, chan);
    TCGv_i32 tag = e2k_get_temp_i32(ctx);
    TCGv_i64 dst = e2k_get_temp_i64(ctx);

    gen_tag1_i64(tag, s2.tag);
    gen_helper_getsp(dst, cpu_env, s2.value);
    gen_al_result_i64(ctx, chan, dst, tag);
}

static void gen_movtd(DisasContext *ctx, int chan)
{
    Src64 s2 = get_src2_i64(ctx, chan);
    set_al_result_reg64_tag(ctx, chan, s2.value, s2.tag);
}

static MemOp gen_mas(DisasContext *ctx, int chan, MemOp memop, TCGv_i64 addr)
{
    uint8_t mas = ctx->mas[chan];

    if ((mas & 0x7) == 7) {
        int opc = mas >> 3;
        // TODO: special mas
//        e2k_tr_gen_exception(ctx, E2K_EXCP_ILLOPC);
        qemu_log_mask(LOG_UNIMP, "0x%lx: mas=%#x, opc=%#x is not implemented!\n",
            ctx->pc, mas, opc);
        return 0;
    } else if (mas) {
        int mod = extract8(mas, 0, 3);
//        int dc = extract8(mas, 5, 2);

        if (mod != 0) {
            // TODO: mas modes
//            e2k_tr_gen_exception(ctx, E2K_EXCP_ILLOPC);
            qemu_log_mask(LOG_UNIMP, "0x%lx: mas=%#x, mod=%#x is not implemented!\n",
                ctx->pc, mas, mod);
        }

        memop |= GET_BIT(mas, 3) ? MO_BE : MO_LE;
        memop |= GET_BIT(mas, 4) ? MO_UNALN : MO_ALIGN;
    } else {
        memop |= MO_LE | MO_ALIGN;
    }

    return memop;
}

static void gen_ld(DisasContext *ctx, int chan, MemOp memop)
{
    bool sm = GET_BIT(ctx->bundle.als[chan], 31);
    TCGLabel *l0 = gen_new_label();
    TCGLabel *l1 = gen_new_label();
    Src64 s1 = get_src1_i64(ctx, chan);
    Src64 s2 = get_src2_i64(ctx, chan);
    TCGv_i32 tag = e2k_get_temp_i32(ctx);
    TCGv_i64 dst = e2k_get_temp_i64(ctx);
    TCGv_i64 t0 = tcg_temp_local_new_i64();
    TCGv_i32 t1 = tcg_temp_new_i32();

    gen_tag2_i64(tag, s1.tag, s2.tag);
    tcg_gen_add_i64(t0, s1.value, s2.value);
    memop = gen_mas(ctx, chan, memop, t0);

    if (sm) {
        gen_helper_probe_read_access(t1, cpu_env, t0);
        tcg_gen_brcondi_i32(TCG_COND_EQ, t1, 1, l0);
        tcg_gen_movi_i32(tag, 5);
        tcg_gen_movi_i64(dst, 0x0afafafa0afafafa);
        tcg_gen_br(l1);
    }

    gen_set_label(l0);
    tcg_gen_qemu_ld_i64(dst, t0, ctx->mmuidx, memop);

    gen_set_label(l1);
    gen_al_result_i64(ctx, chan, dst, tag);

    tcg_temp_free_i32(t1);
    tcg_temp_free_i64(t0);
}

static void gen_st_i64(DisasContext *ctx, int chan, MemOp memop)
{
    bool sm = GET_BIT(ctx->bundle.als[chan], 31);
    TCGLabel *l0 = gen_new_label();
    Src64 s1 = get_src1_i64(ctx, chan);
    Src64 s2 = get_src2_i64(ctx, chan);
    Src64 s4 = get_src4_i64(ctx, chan);
    TCGv_i64 t0 = tcg_temp_local_new_i64();

    gen_loop_mode_st(ctx, l0);
    gen_tag_check(ctx, sm, s1.tag);
    gen_tag_check(ctx, sm, s2.tag);
    gen_tag_check(ctx, sm, s4.tag);
    tcg_gen_add_i64(t0, s1.value, s2.value);
    memop = gen_mas(ctx, chan, memop, t0);

    if (memop == 0) {
        // FIXME: hack
        return;
    }

    if (sm) {
        TCGv_i32 t1 = tcg_temp_new_i32();
        gen_helper_probe_write_access(t1, cpu_env, t0);
        tcg_gen_brcondi_i32(TCG_COND_EQ, t1, 0, l0);
        tcg_temp_free_i32(t1);
    }

    tcg_gen_qemu_st_i64(s4.value, t0, ctx->mmuidx, memop);
    gen_set_label(l0);

    tcg_temp_free_i64(t0);
}

static void gen_st_i32(DisasContext *ctx, int chan, MemOp memop)
{
    bool sm = GET_BIT(ctx->bundle.als[chan], 31);
    TCGLabel *l0 = gen_new_label();
    Src64 s1 = get_src1_i64(ctx, chan);
    Src64 s2 = get_src2_i64(ctx, chan);
    Src32 s4 = get_src4_i32(ctx, chan);
    TCGv_i64 t0 = tcg_temp_local_new_i64();

    gen_loop_mode_st(ctx, l0);
    gen_tag_check(ctx, sm, s1.tag);
    gen_tag_check(ctx, sm, s2.tag);
    gen_tag_check(ctx, sm, s4.tag);
    tcg_gen_add_i64(t0, s1.value, s2.value);
    memop = gen_mas(ctx, chan, memop, t0);

    if (memop == 0) {
        // FIXME: hack
        return;
    }

    if (sm) {
        TCGv_i32 t1 = tcg_temp_new_i32();
        gen_helper_probe_write_access(t1, cpu_env, t0);
        tcg_gen_brcondi_i32(TCG_COND_EQ, t1, 0, l0);
        tcg_temp_free_i32(t1);
    }

    tcg_gen_qemu_st_i32(s4.value, t0, ctx->mmuidx, memop);
    gen_set_label(l0);

    tcg_temp_free_i64(t0);
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
    TCGv_i64 lo = e2k_cs.aad_lo[instr->aad];
    TCGv_i64 t0 = tcg_temp_new_i64();
    TCGv_i64 t1 = tcg_temp_new_i64();

    tcg_gen_andi_i64(t0, arg1, 3UL << 57);
    tcg_gen_andi_i64(lo, lo, !(0x1fUL << 54));
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
    tcg_gen_andi_i64(e2k_cs.aad_hi[instr->aad], arg1, 0xffffffff00000000);
}

static void gen_aaurw_aad_i32(Instr *instr, TCGv_i32 arg1, TCGv_i32 tag)
{
    TCGv_i64 lo = e2k_cs.aad_lo[instr->aad];
    TCGv_i64 t0 = tcg_temp_new_i64();
    TCGv_i64 t1 = tcg_temp_new_i64();
    TCGv_i64 t2 = tcg_temp_new_i64();

    tcg_gen_extu_i32_i64(t0, arg1);
    tcg_gen_deposit_i64(lo, lo, t0, 0, 48);
    tcg_gen_ori_i64(lo, lo, 3UL << 59);
    tcg_gen_andi_i64(lo, lo, !(0x7UL << 54));
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
        tcg_gen_mov_i32(e2k_cs.aasti[idx], arg1);
        tcg_gen_deposit_i32(e2k_cs.aasti_tags, e2k_cs.aasti_tags, t0, idx, 1);
        break;
    case 2: { /* aaurwd src4, aaind */
        if (idx == 0) {
            tcg_gen_movi_i32(e2k_cs.aaind[idx], 0);
        } else {
            tcg_gen_mov_i32(e2k_cs.aaind[idx], arg1);
            tcg_gen_deposit_i32(e2k_cs.aaind_tags, e2k_cs.aaind_tags, t0,
                idx, 1);
        }
        break;
    }
    case 3: /* aaurwd src4, aaincr */
        idx &= 7;
        if (idx > 0) {
            tcg_gen_mov_i32(e2k_cs.aaincr[idx], arg1);
            tcg_gen_deposit_i32(e2k_cs.aaincr_tags, e2k_cs.aaincr_tags, t0,
                idx, 1);
        }
        break;
    default:
        g_assert_not_reached();
        break;
    }
    tcg_temp_free_i32(t0);
}

static void gen_aasti_incr(DisasContext *ctx, Instr *instr, int size)
{
    uint16_t rlp = find_am_cond(ctx, instr->chan);
    TCGLabel *l0 = gen_new_label();
    TCGv_i32 t0 = tcg_temp_new_i32();

    if (rlp != 0) {
        TCGv_i32 t1 = tcg_temp_new_i32();

        gen_am_cond_i32(ctx, t1, instr->chan, rlp);
        tcg_gen_brcondi_i32(TCG_COND_EQ, t1, 0, l0);
        tcg_temp_free_i32(t1);
    }

    tcg_gen_muli_i32(t0, e2k_cs.aaincr[instr->aaincr], size);
    tcg_gen_add_i32(e2k_cs.aasti[instr->aaind], e2k_cs.aasti[instr->aaind], t0);
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
            e2k_tr_gen_exception(ctx, E2K_EXCP_ILLOPN);
            return;
        }
    }

    tcg_gen_extract_i64(t0, e2k_cs.aad_lo[instr->aad], 0, 48);
    tcg_gen_trunc_i64_tl(t1, t0);
    tcg_gen_ext_i32_tl(t2, e2k_cs.aasti[instr->aaind]);
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

static void gen_staa_i64(DisasContext *ctx, Instr *instr)
{
    uint8_t mas = ctx->mas[instr->chan];
    TCGLabel *l0 = gen_new_label();
    Src64 s4 = get_src4_i64(ctx, instr->chan);

    gen_loop_mode_st(ctx, l0);
    gen_tag_check(ctx, instr->sm, s4.tag);
    if (mas == 0x3f) {
        /* aaurwd */
        if (instr->aaopc == 0) {
            if (instr->chan == 5 && instr->opc1 == 0x3f) {
                gen_aaurw_aad_hi_i64(instr, s4.value, s4.tag);
            } else {
                gen_aaurw_aad_lo_i64(instr, s4.value, s4.tag);
            }
        } else {
            TCGv_i32 t0 = tcg_temp_new_i32();
            tcg_gen_extrl_i64_i32(t0, s4.value);
            gen_aaurw_rest_i32(instr, t0, s4.tag);
            tcg_temp_free_i32(t0);
        }
    } else {
        /* staad */
        TCGLabel *l0 = gen_new_label();
        TCGv t0 = tcg_temp_local_new();

        if (mas != 0) {
            qemu_log_mask(LOG_UNIMP,
                "0x%lx: staad mas=%#x is not implemented\n", ctx->pc, mas);
        }

        gen_aad_ptr(ctx, t0, instr);

        if (instr->sm) {
            TCGv_i32 t1 = tcg_temp_new_i32();
            gen_helper_probe_write_access(t1, cpu_env, t0);
            tcg_gen_brcondi_i32(TCG_COND_EQ, t1, 0, l0);
        }

        tcg_gen_qemu_st_i64(s4.value, t0, 0, MO_Q);
        gen_set_label(l0);
        tcg_temp_free(t0);

        if (instr->aaopc & 1) {
            gen_aasti_incr(ctx, instr, 8);
        }
    }

    gen_set_label(l0);
}

static void gen_staa_i32(DisasContext *ctx, Instr *instr, MemOp memop)
{
    uint8_t mas = ctx->mas[instr->chan];
    TCGLabel *l0 = gen_new_label();
    Src32 s4 = get_src4_i32(ctx, instr->chan);

    gen_loop_mode_st(ctx, l0);
    gen_tag_check(ctx, instr->sm, s4.tag);
    if (mas == 0x3f) {
        /* aaurw */
        /* CPU do nothing if size less than 32 bits */
        if ((memop & MO_SIZE) == MO_32) {
            if (instr->aaopc == 0) {
                gen_aaurw_aad_i32(instr, s4.value, s4.tag);
            } else {
                gen_aaurw_rest_i32(instr, s4.value, s4.tag);
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
            qemu_log_mask(LOG_UNIMP,
                "0x%lx: staa%c mas=%#x is not implemented\n", ctx->pc, c, mas);
        }

        gen_aad_ptr(ctx, t0, instr);

        if (instr->sm) {
            TCGv_i32 t1 = tcg_temp_new_i32();
            gen_helper_probe_write_access(t1, cpu_env, t0);
            tcg_gen_brcondi_i32(TCG_COND_EQ, t1, 0, l0);
        }

        tcg_gen_qemu_st_i32(s4.value, t0, 0, memop);
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
            gen_aasti_incr(ctx, instr, len);
        }
    }

    gen_set_label(l0);
}

static void gen_alopf1_i64(DisasContext *ctx, int chan,
    void (*op)(TCGv_i64, TCGv_i64, TCGv_i64))
{
    Src64 s1 = get_src1_i64(ctx, chan);
    Src64 s2 = get_src2_i64(ctx, chan);
    TCGv_i32 tag = e2k_get_temp_i32(ctx);
    TCGv_i64 dst = e2k_get_temp_i64(ctx);

    gen_tag2_i64(tag, s1.tag, s2.tag);
    (*op)(dst, s1.value, s2.value);
    gen_al_result_i64(ctx, chan, dst, tag);
}

static void gen_alopf1_i32(DisasContext *ctx, int chan,
    void (*op)(TCGv_i32, TCGv_i32, TCGv_i32))
{
    Src32 s1 = get_src1_i32(ctx, chan);
    Src32 s2 = get_src2_i32(ctx, chan);
    TCGv_i32 tag = e2k_get_temp_i32(ctx);
    TCGv_i32 dst = e2k_get_temp_i32(ctx);

    gen_tag2_i32(tag, s1.tag, s2.tag);
    (*op)(dst, s1.value, s2.value);
    gen_al_result_i32(ctx, chan, dst, tag);
}

static void gen_alopf1_i32_i64(DisasContext *ctx, int chan,
    void (*op)(TCGv_i64, TCGv_i32, TCGv_i32))
{
    Src32 s1 = get_src1_i32(ctx, chan);
    Src32 s2 = get_src2_i32(ctx, chan);
    TCGv_i32 tag = e2k_get_temp_i32(ctx);
    TCGv_i64 dst = e2k_get_temp_i64(ctx);

    gen_tag2_i64(tag, s1.tag, s2.tag);
    (*op)(dst, s1.value, s2.value);
    gen_al_result_i64(ctx, chan, dst, tag);
}

static void gen_alopf1_tag_i64(DisasContext *ctx, int chan,
    void (*op)(TCGv_i64, TCGv_i32, TCGv_i32, TCGv_i64, TCGv_i64))
{
    Src64 s1 = get_src1_i64(ctx, chan);
    Src64 s2 = get_src2_i64(ctx, chan);
    TCGv_i32 tag = e2k_get_temp_i32(ctx);
    TCGv_i64 dst = e2k_get_temp_i64(ctx);

    gen_tag2_i64(tag, s1.tag, s2.tag);
    (*op)(dst, tag, tag, s1.value, s2.value);
    gen_al_result_i64(ctx, chan, dst, tag);
}

static void gen_alopf1_tag_i32(DisasContext *ctx, int chan,
    void (*op)(TCGv_i32, TCGv_i32, TCGv_i32, TCGv_i32, TCGv_i32))
{
    Src32 s1 = get_src1_i32(ctx, chan);
    Src32 s2 = get_src2_i32(ctx, chan);
    TCGv_i32 tag = e2k_get_temp_i32(ctx);
    TCGv_i32 dst = e2k_get_temp_i32(ctx);

    gen_tag2_i32(tag, s1.tag, s2.tag);
    (*op)(dst, tag, tag, s1.value, s2.value);
    gen_al_result_i32(ctx, chan, dst, tag);
}

static void gen_alopf1_cmp_i64(DisasContext *ctx, Instr *instr,
    void (*op)(TCGv_i64, int, TCGv_i64, TCGv_i64))
{
    int chan = instr->chan;
    TCGLabel *l0 = gen_new_label();
    TCGLabel *l1 = gen_new_label();
    Src64 s1 = get_src1_i64(ctx, chan);
    Src64 s2 = get_src2_i64(ctx, chan);
    TCGv_i64 dst = e2k_get_temp_i64(ctx);
    TCGv_i32 t0 = tcg_temp_new_i32();

    gen_tag2_i64(t0, s1.tag, s2.tag);
    gen_tag_check(ctx, instr->sm, t0);
    tcg_gen_brcondi_i32(TCG_COND_NE, t0, 0, l0);
    (*op)(dst, instr->opc_cmp, s1.value, s2.value);
    tcg_gen_br(l1);
    gen_set_label(l0);
    tcg_gen_movi_i64(dst, 2);
    gen_set_label(l1);
    set_al_result_preg(ctx, chan, instr->dst_preg, dst);
    tcg_temp_free_i32(t0);
}

static void gen_alopf1_cmp_i32(DisasContext *ctx, Instr *instr,
    void (*op)(TCGv_i32, int, TCGv_i32, TCGv_i32))
{
    int chan = instr->chan;
    TCGLabel *l0 = gen_new_label();
    TCGLabel *l1 = gen_new_label();
    Src32 s1 = get_src1_i32(ctx, chan);
    Src32 s2 = get_src2_i32(ctx, chan);
    TCGv_i64 dst = e2k_get_temp_i64(ctx);
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i32 t1 = tcg_temp_new_i32();

    gen_tag2_i32(t0, s1.tag, s2.tag);
    gen_tag_check(ctx, instr->sm, t0);
    tcg_gen_brcondi_i32(TCG_COND_NE, t0, 0, l0);
    (*op)(t1, instr->opc_cmp, s1.value, s2.value);
    tcg_gen_extu_i32_i64(dst, t1);
    tcg_gen_br(l1);
    gen_set_label(l0);
    tcg_gen_movi_i64(dst, 2);
    gen_set_label(l1);
    set_al_result_preg(ctx, chan, instr->dst_preg, dst);
    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
}

static void gen_alopf1_mrgc_i32(DisasContext *ctx, Instr *instr)
{
    int chan = instr->chan;
    Src32 s1 = get_src1_i32(ctx, chan);
    Src32 s2 = get_src2_i32(ctx, chan);
    TCGv_i32 tag = e2k_get_temp_i32(ctx);
    TCGv_i32 dst = e2k_get_temp_i32(ctx);
    TCGv_i32 t0 = tcg_temp_new_i32();

    gen_tag2_i32(tag, s1.tag, s2.tag);
    gen_mrgc_i32(ctx, instr->chan, t0);
    gen_merge_i32(dst, s1.value, s2.value, t0);
    tcg_temp_free_i32(t0);
    gen_al_result_i32(ctx, chan, dst, tag);
}

static void gen_alopf1_mrgc_i64(DisasContext *ctx, Instr *instr)
{
    int chan = instr->chan;
    Src64 s1 = get_src1_i64(ctx, chan);
    Src64 s2 = get_src2_i64(ctx, chan);
    TCGv_i32 tag = e2k_get_temp_i32(ctx);
    TCGv_i64 dst = e2k_get_temp_i64(ctx);
    TCGv_i32 t0 = tcg_temp_new_i32();

    gen_tag2_i64(tag, s1.tag, s2.tag);
    gen_mrgc_i32(ctx, instr->chan, t0);
    gen_merge_i64(dst, s1.value, s2.value, t0);
    tcg_temp_free_i32(t0);
    gen_al_result_i64(ctx, chan, dst, tag);
}

static void gen_alopf21_i64(DisasContext *ctx, Instr *instr,
    void (*op)(TCGv_i64, TCGv_i64, TCGv_i64, TCGv_i64))
{
    Src64 s1 = get_src1_i64(ctx, instr->chan);
    Src64 s2 = get_src2_i64(ctx, instr->chan);
    Src64 s3 = get_src3_i64(ctx, instr->chan);
    TCGv_i32 tag = e2k_get_temp_i32(ctx);
    TCGv_i64 dst = e2k_get_temp_i64(ctx);

    gen_tag3_i64(tag, s1.tag, s2.tag, s3.tag);
    (*op)(dst, s1.value, s2.value, s3.value);
    gen_al_result_i64(ctx, instr->chan, dst, tag);
}

static void gen_alopf21_i32(DisasContext *ctx, Instr *instr,
    void (*op)(TCGv_i32, TCGv_i32, TCGv_i32, TCGv_i32))
{
    Src32 s1 = get_src1_i32(ctx, instr->chan);
    Src32 s2 = get_src2_i32(ctx, instr->chan);
    Src32 s3 = get_src3_i32(ctx, instr->chan);
    TCGv_i32 tag = e2k_get_temp_i32(ctx);
    TCGv_i32 dst = e2k_get_temp_i32(ctx);

    gen_tag3_i32(tag, s1.tag, s2.tag, s3.tag);
    (*op)(dst, s1.value, s2.value, s3.value);
    gen_al_result_i32(ctx, instr->chan, dst, tag);
}

static void gen_alopf2_i32(DisasContext *ctx, int chan, void (*op)(TCGv_i32, TCGv_i32))
{
    Src32 s2 = get_src2_i32(ctx, chan);
    TCGv_i32 dst = e2k_get_temp_i32(ctx);
    TCGv_i32 tag = e2k_get_temp_i32(ctx);
    
    gen_tag1_i32(tag, s2.tag);
    (*op)(dst, s2.value);
    gen_al_result_i32(ctx, chan, dst, tag);
}

static void gen_alopf2_i64(DisasContext *ctx, int chan, void (*op)(TCGv_i64, TCGv_i64))
{
    Src64 s2 = get_src2_i64(ctx, chan);
    TCGv_i64 dst = e2k_get_temp_i64(ctx);
    TCGv_i32 tag = e2k_get_temp_i32(ctx);

    gen_tag1_i64(tag, s2.tag);
    (*op)(dst, s2.value);
    gen_al_result_i64(ctx, chan, dst, tag);
}

static void execute_ext_00(DisasContext *ctx, Instr *instr)
{
    int chan = instr->chan;

    switch(instr->opc1) {
    case 0x00: /* ands */ gen_alopf1_i32(ctx, chan, tcg_gen_and_i32); return;
    case 0x01: /* andd */ gen_alopf1_i64(ctx, chan, tcg_gen_and_i64); return;
    case 0x02: /* andns */ gen_alopf1_i32(ctx, chan, gen_andn_i32); return;
    case 0x03: /* andnd */ gen_alopf1_i64(ctx, chan, gen_andn_i64); return;
    case 0x04: /* ors */ gen_alopf1_i32(ctx, chan, tcg_gen_or_i32); return;
    case 0x05: /* ord */ gen_alopf1_i64(ctx, chan, tcg_gen_or_i64); return;
    case 0x06: /* orns */ gen_alopf1_i32(ctx, chan, gen_orn_i32); return;
    case 0x07: /* ornd */ gen_alopf1_i64(ctx, chan, gen_orn_i64); return;
    case 0x08: /* xors */ gen_alopf1_i32(ctx, chan, tcg_gen_xor_i32); return;
    case 0x09: /* xord */ gen_alopf1_i64(ctx, chan, tcg_gen_xor_i64); return;
    case 0x0a: /* xorns */ gen_alopf1_i32(ctx, chan, gen_xorn_i32); return;
    case 0x0b: /* xornd */ gen_alopf1_i64(ctx, chan, gen_xorn_i64); return;
    case 0x0c: /* sxt */ gen_sxt(ctx, chan); return;
    case 0x0e: /* merges */ gen_alopf1_mrgc_i32(ctx, instr); return;
    case 0x0f: /* merged */ gen_alopf1_mrgc_i64(ctx, instr); return;
    case 0x10: /* adds */ gen_alopf1_i32(ctx, chan, tcg_gen_add_i32); return;
    case 0x11: /* addd */ gen_alopf1_i64(ctx, chan, tcg_gen_add_i64); return;
    case 0x12: /* subs */ gen_alopf1_i32(ctx, chan, tcg_gen_sub_i32); return;
    case 0x13: /* subd */ gen_alopf1_i64(ctx, chan, tcg_gen_sub_i64); return;
    case 0x14: /* scls */ gen_alopf1_i32(ctx, chan, tcg_gen_rotl_i32); return;
    case 0x15: /* scld */ gen_alopf1_i64(ctx, chan, tcg_gen_rotl_i64); return;
    case 0x16: /* scrs */ gen_alopf1_i32(ctx, chan, tcg_gen_rotr_i32); return;
    case 0x17: /* scrd */ gen_alopf1_i64(ctx, chan, tcg_gen_rotr_i64); return;
    case 0x18: /* shls */ gen_alopf1_i32(ctx, chan, tcg_gen_shl_i32); return;
    case 0x19: /* shld */ gen_alopf1_i64(ctx, chan, tcg_gen_shl_i64); return;
    case 0x1a: /* shrs */ gen_alopf1_i32(ctx, chan, tcg_gen_shr_i32); return;
    case 0x1b: /* shrd */ gen_alopf1_i64(ctx, chan, tcg_gen_shr_i64); return;
    case 0x1c: /* sars */ gen_alopf1_i32(ctx, chan, tcg_gen_sar_i32); return;
    case 0x1d: /* sard */ gen_alopf1_i64(ctx, chan, tcg_gen_sar_i64); return;
    case 0x1e: /* getfs */ gen_alopf1_i32(ctx, chan, gen_getfs); return;
    case 0x1f: /* getfd */ gen_alopf1_i64(ctx, chan, gen_getfd); return;
    case 0x20:
        if (is_chan_0134(chan)) {
            /* cmp{op}sb */
            gen_alopf1_cmp_i32(ctx, instr, gen_cmp_i32);
            return;
        }
        break;
    case 0x21:
        if (is_chan_0134(chan)) {
            /* cmp{op}db */
            gen_alopf1_cmp_i64(ctx, instr, gen_cmp_i64);
            return;
        }
        break;
    case 0x22:
        if (is_chan_0134(chan)) {
            /* cmpand{op}sb */
            gen_alopf1_cmp_i32(ctx, instr, gen_cmpand_i32);
            return;
        }
        break;
    case 0x23:
        if (is_chan_0134(chan)) {
            /* cmpand{op}db */
            gen_alopf1_cmp_i64(ctx, instr, gen_cmpand_i64);
            return;
        }
        break;
    case 0x24: {
        if (is_chan_25(chan)) {
            /* stb */
            gen_st_i32(ctx, chan, MO_UB);
            return;
        }
        break;
    }
    case 0x25: {
        if (is_chan_25(chan)) {
            /* sth */
            gen_st_i32(ctx, chan, MO_UW);
            return;
        }
        break;
    }
    case 0x26: {
        if (is_chan_25(chan)) {
            /* stw */
            gen_st_i32(ctx, chan, MO_UL);
            return;
        } else if(instr->opce1 == 0xc0 && ctx->version >= 2) {
            /* bitrevs */
            gen_alopf2_i32(ctx, chan, gen_bitrevs);
            return;
        }
        break;
    }
    case 0x27: {
        if (is_chan_25(chan)) {
            /* std */
            gen_st_i64(ctx, chan, MO_Q);
            return;
        } else if(instr->opce1 == 0xc0 && ctx->version >= 2) {
            /* bitrevd */
            gen_alopf2_i64(ctx, chan, gen_bitrevd);
            return;
        }
        break;
    }
    case 0x40:
        if (chan == 5) {
            // FIXME: temp hack
            TCGLabel *l0 = gen_new_label();
            Src64 s2 = get_src2_i64(ctx, chan);
            tcg_gen_brcondi_i64(TCG_COND_NE, s2.value, 0, l0);
            e2k_gen_exception(0);
            gen_set_label(l0);

            /* udivs */
            gen_alopf1_tag_i32(ctx, chan, gen_udivs);
            return;
        }
        break;
    case 0x41:
        if (chan == 5) {
            /* udivd */
            gen_alopf1_tag_i64(ctx, chan, gen_udivd);
            return;
        }
        break;
    case 0x42:
        if (chan == 5) {
            /* sdivs */
            gen_alopf1_tag_i32(ctx, chan, gen_sdivs);
            return;
        }
        break;
    case 0x43:
        if (chan == 5) {
            /* sdivd */
            gen_alopf1_tag_i64(ctx, chan, gen_sdivd);
            return;
        }
        break;
    case 0x61:
        if (is_chan_0134(chan)) {
            gen_movtd(ctx, chan);
            return;
        }
        break;
    case 0x64: {
        if (is_chan_0235(chan)) {
            /* ldb */
            gen_ld(ctx, chan, MO_UB);
            return;
        } else if (is_chan_14(chan) && instr->opce1 == 0xc0 &&
            ctx->version >= 2)
        {
            /* lzcnts */
            gen_alopf2_i32(ctx, chan, gen_lzcnts);
            return;
        }
        break;
    }
    case 0x65: { 
        if (is_chan_0235(chan)) {
            /* ldh */
            gen_ld(ctx, chan, MO_UW);
            return;
        } else if (is_chan_14(chan) && instr->opce1 == 0xc0 &&
            ctx->version >= 2)
        {
            /* lzcntd */
            gen_alopf2_i64(ctx, chan, gen_lzcntd);
            return;
        }
        break;
    }
    case 0x66: { 
        if (is_chan_0235(chan)) {
            /* ldw */
            gen_ld(ctx, chan, MO_UL);
            return;
        } else if (is_chan_14(chan) && instr->opce1 == 0xc0 &&
            ctx->version >= 2)
        {
            /* popcnts */
            gen_alopf2_i32(ctx, chan, tcg_gen_ctpop_i32);
            return;
        }
        break;
    }
    case 0x67: { 
        if (is_chan_0235(chan)) {
            /* ldd */
            gen_ld(ctx, chan, MO_Q);
            return;
        } else if (is_chan_14(chan) && instr->opce1 == 0xc0 &&
            ctx->version >= 2)
        {
            /* popcntd */
            gen_alopf2_i64(ctx, chan, tcg_gen_ctpop_i64);
            return;
        }
        break;
    }
    default:
        break;
    }

    e2k_tr_gen_exception(ctx, E2K_EXCP_ILLOPC);
}

static void execute_ext_01(DisasContext *ctx, Instr *instr)
{
    int chan = instr->chan;
    switch (instr->opc1) {
    case 0x00:
        if (is_chan_03(chan)) {
            /* pminub */
            gen_alopf1_i64(ctx, chan, gen_helper_pminub);
            return;
        }
        break;
    case 0x01:
        if (is_chan_03(chan)) {
            /* pminsh */
            gen_alopf1_i64(ctx, chan, gen_helper_pminsh);
            return;
        }
        break;
    case 0x02:
        if (is_chan_03(chan)) {
            /* pmaxub */
            gen_alopf1_i64(ctx, chan, gen_helper_pmaxub);
            return;
        }
        break;
    case 0x03:
        if (is_chan_03(chan)) {
            /* pmaxsh */
            gen_alopf1_i64(ctx, chan, gen_helper_pmaxsh);
            return;
        }
        break;
    case 0x08:
        if (is_chan_25(chan)) {
            /* gettags */
            gen_gettag_i32(ctx, chan);
            return;
        }
        break;
    case 0x09:
        if (is_chan_25(chan)) {
            /* gettagd */
            gen_gettag_i64(ctx, chan);
            return;
        }
        break;
    case 0x0a:
        if (is_chan_25(chan)) {
            /* puttags */
            gen_puttag_i32(ctx, chan);
            return;
        }
        break;
    case 0x0b:
        if (is_chan_14(chan)) {
            /* pmovmskb */
            gen_alopf1_i64(ctx, chan, gen_helper_pmovmskb);
            return;
        } else if (is_chan_25(chan)) {
            /* puttagd */
            gen_puttag_i64(ctx, chan);
            return;
        }
        break;
    case 0x0f:
        if (is_chan_03(chan)) {
            /* paddd */
            gen_alopf1_i64(ctx, chan, tcg_gen_add_i64);
            return;
        }
        break;
    case 0x18:
        if (is_chan_03(chan)) {
            /* pcmpeqb */
            gen_alopf1_i64(ctx, chan, gen_helper_pcmpeqb);
            return;
        }
        break;
    case 0x1c:
        if (is_chan_25(chan)) {
            /* staab */
            gen_staa_i32(ctx, instr, MO_8);
            return;
        }
        break;
    case 0x1d:
        if (is_chan_25(chan)) {
            /* staah */
            gen_staa_i32(ctx, instr, MO_16);
            return;
        }
        break;
    case 0x1e:
        if (is_chan_25(chan)) {
            /* staaw */
            gen_staa_i32(ctx, instr, MO_32);
            return;
        }
        break;
    case 0x1f:
        if (is_chan_25(chan)) {
            /* staad */
            gen_staa_i64(ctx, instr);
            return;
        }
        break;
    case 0x20:
        if (is_chan_0134(chan)) {
            /* muls */
            gen_alopf1_i32(ctx, chan, tcg_gen_mul_i32);
            return;
        }
        break;
    case 0x21:
        if (is_chan_0134(chan)) {
            /* muld */
            gen_alopf1_i64(ctx, chan, tcg_gen_mul_i64);
            return;
        }
        break;
    case 0x22:
        if (is_chan_0134(chan)) {
            /* umulx */
            gen_alopf1_i32_i64(ctx, chan, gen_umulx);
            return;
        }
        break;
    case 0x23:
        if (is_chan_0134(chan)) {
            /* smulx */
            gen_alopf1_i32_i64(ctx, chan, gen_smulx);
            return;
        }
        break;
    case 0x3c:
        if (chan == 0) {
            /* rws */
            gen_rw_i32(ctx, chan);
            return;
        }
        break;
    case 0x3d:
        if (chan == 0) {
            /* rwd */
            gen_rw_i64(ctx, chan);
            return;
        }
        break;
    case 0x3e:
        if (chan == 0) {
            /* rrs */
            gen_rr_i32(ctx, chan);
            return;
        }
        break;
    case 0x3f:
        if (chan == 0) {
            /* rrd */
            gen_rr_i64(ctx, chan);
            return;
        } else if (is_chan_25(chan)) {
            /* staaq */
            int pair_chan = chan == 2 ? 5 : 2;
            if (!ctx->bundle.als_present[pair_chan] ||
                extract32(ctx->bundle.als[pair_chan], 24, 7) != 0x3f ||
                (instr->dst & 1) != (chan == 2 ? 0 : 1))
            {
                e2k_tr_gen_exception(ctx, E2K_EXCP_ILLOPC);
                return;
            }
            gen_staa_i64(ctx, instr);
            return;
        }
        break;
    case 0x58:
        if (is_chan_03(chan)) {
            /* getsp */
            gen_getsp(ctx, chan);
            return;
        }
        break;
    case 0x70:
        if (is_chan_0134(chan)) {
            /* umulhd */
            gen_alopf1_i64(ctx, chan, gen_umulhd);
            return;
        }
        break;
    case 0x71:
        if (is_chan_0134(chan)) {
            /* smulhd */
            gen_alopf1_i64(ctx, chan, gen_smulhd);
            return;
        }
        break;
    default:
        break;
    }

    e2k_tr_gen_exception(ctx, E2K_EXCP_ILLOPC);
}

static void execute_icomb_i64(DisasContext *ctx, Instr *instr)
{
    int opc1 = instr->opc1 & 0x1f;
    int opc2 = instr->opc1 & 0x60;
    Src64 s1 = get_src1_i64(ctx, instr->chan);
    Src64 s2 = get_src2_i64(ctx, instr->chan);
    Src64 s3 = get_src3_i64(ctx, instr->chan);
    TCGv_i32 tag = e2k_get_temp_i32(ctx);
    TCGv_i64 dst = e2k_get_temp_i64(ctx);
    TCGv_i32 mrgc = NULL;

    gen_tag3_i64(tag, s1.tag, s2.tag, s3.tag);
    switch(opc1) {
    case 0x01:
        /* and_{op}d */
        tcg_gen_and_i64(dst, s1.value, s2.value);
        break;
    case 0x03:
        /* andn_{op}d */
        gen_andn_i64(dst, s1.value, s2.value);
        break;
    case 0x05:
        /* or_{op}d */
        tcg_gen_or_i64(dst, s1.value, s2.value);
        break;
    case 0x07:
        /* orn_{op}d */
        gen_orn_i64(dst, s1.value, s2.value);
        break;
    case 0x09:
        /* xor_{op}d */
        tcg_gen_xor_i64(dst, s1.value, s2.value);
        break;
    case 0x0b:
        /* xorn_{op}d */
        gen_xorn_i64(dst, s1.value, s2.value);
        break;
    case 0x0f:
        /* merge_{op}d */
        mrgc = tcg_temp_new_i32();
        gen_mrgc_i32(ctx, instr->chan, mrgc);
        gen_merge_i64(dst, s1.value, s2.value, mrgc);
        break;
    case 0x11:
        /* add_{op}d */
        tcg_gen_add_i64(dst, s1.value, s2.value);
        break;
    case 0x13:
        /* sub_{op}d */
        tcg_gen_sub_i64(dst, s1.value, s2.value);
        break;
    case 0x15:
        /* scl_{op}d */
        tcg_gen_rotl_i64(dst, s1.value, s2.value);
        break;
    case 0x17:
        /* scr_{op}d */
        tcg_gen_rotr_i64(dst, s1.value, s2.value);
        break;
    case 0x19:
        /* shl_{op}d */
        tcg_gen_shl_i64(dst, s1.value, s2.value);
        break;
    case 0x1b:
        /* shr_{op}d */
        tcg_gen_shr_i64(dst, s1.value, s2.value);
        break;
    case 0x1d:
        /* sar_{op}d */
        tcg_gen_sar_i64(dst, s1.value, s2.value);
        break;
    case 0x1f:
        /* gef_{op}d */
        gen_getfd(dst, s1.value, s2.value);
        break;
    default:
        e2k_tr_gen_exception(ctx, E2K_EXCP_ILLOPC);
        return;
    }

    if (instr->opc2 == 0x08) {
        switch (opc2) {
        case 0x00:
            /* {op}_andd */
            tcg_gen_and_i64(dst, dst, s3.value);
            break;
        case 0x20:
            /* {op}_andnd */
            gen_andn_i64(dst, dst, s3.value);
            break;
        case 0x40:
            /* {op}_ord */
            tcg_gen_or_i64(dst, dst, s3.value);
            break;
        case 0x60:
            /* {op}_ornd */
            gen_orn_i64(dst, dst, s3.value);
            break;
        default:
            g_assert_not_reached();
            break;
        }
    } else if (instr->opc2 == 0x09) {
        switch(opc2) {
        case 0x00:
            /* {op}_xord */
            tcg_gen_xor_i64(dst, dst, s3.value);
            break;
        case 0x20:
            /* {op}_xornd */
            gen_xorn_i64(dst, dst, s3.value);
            break;
        case 0x40:
            /* {op}_rsubd */
            tcg_gen_sub_i64(dst, s3.value, dst);
            break;
        case 0x60:
            /* {op}_merged */
            if (ctx->version == 1) {
                if (mrgc == NULL) {
                    mrgc = tcg_temp_new_i32();
                    gen_mrgc_i32(ctx, instr->chan, mrgc);
                }
                gen_merge_i64(dst, dst, s3.value, mrgc);
            } else {
                e2k_tr_gen_exception(ctx, E2K_EXCP_ILLOPC);
                return;
            }
            break;
        default:
            g_assert_not_reached();
            break;
        }
    } else if (instr->opc2 == 0x0a) {
        switch (opc2) {
        case 0x00:
            /* {op}_addd */
            tcg_gen_add_i64(dst, dst, s3.value);
            break;
        case 0x20:
            /* {op}_subd */
            tcg_gen_sub_i64(dst, dst, s3.value);
            break;
        case 0x40:
            /* {op}_scld */
            if (ctx->version == 1) {
                tcg_gen_rotl_i64(dst, dst, s3.value);
            } else {
                e2k_tr_gen_exception(ctx, E2K_EXCP_ILLOPC);
                return;
            }
            break;
        case 0x60:
            /* {op}_scrd */
            if (ctx->version == 1) {
                tcg_gen_rotr_i64(dst, dst, s3.value);
            } else {
                e2k_tr_gen_exception(ctx, E2K_EXCP_ILLOPC);
                return;
            }
            break;
        default:
            g_assert_not_reached();
            break;
        }
    } else if (instr->opc2 == 0x0b) {
        if (ctx->version != 1) {
            e2k_tr_gen_exception(ctx, E2K_EXCP_ILLOPC);
            return;
        }

        switch (opc2) {
        case 0x00:
            /* {op}_shld */
            tcg_gen_shl_i64(dst, dst, s3.value);
            break;
        case 0x20:
            /* {op}_shrd */
            tcg_gen_shr_i64(dst, dst, s3.value);
            break;
        case 0x40:
            /* {op}_sard */
            tcg_gen_sar_i64(dst, dst, s3.value);
            break;
        case 0x60:
            /* {op}_getfd */
            gen_getfd(dst, dst, s3.value);
            break;
        default:
            g_assert_not_reached();
            break;
        }
    }

    if (mrgc != NULL) {
        tcg_temp_free_i32(mrgc);
    }

    gen_al_result_i64(ctx, instr->chan, dst, tag);
}

static void execute_icomb_i32(DisasContext *ctx, Instr *instr)
{
    int opc1 = instr->opc1 & 0x1f;
    int opc2 = instr->opc1 & 0x60;
    Src32 s1 = get_src1_i32(ctx, instr->chan);
    Src32 s2 = get_src2_i32(ctx, instr->chan);
    Src32 s3 = get_src3_i32(ctx, instr->chan);
    TCGv_i32 tag = e2k_get_temp_i32(ctx);
    TCGv_i32 dst = e2k_get_temp_i32(ctx);
    TCGv_i32 mrgc = NULL;

    gen_tag3_i32(tag, s1.tag, s2.tag, s3.tag);
    switch(opc1) {
    case 0x00:
        /* and_{op}s */
        tcg_gen_and_i32(dst, s1.value, s2.value);
        break;
    case 0x02:
        /* andn_{op}s */
        gen_andn_i32(dst, s1.value, s2.value);
        break;
    case 0x04:
        /* or_{op}s */
        tcg_gen_or_i32(dst, s1.value, s2.value);
        break;
    case 0x06:
        /* orn_{op}s */
        gen_orn_i32(dst, s1.value, s2.value);
        break;
    case 0x08:
        /* xor_{op}s */
        tcg_gen_xor_i32(dst, s1.value, s2.value);
        break;
    case 0x0a:
        /* xorn_{op}s */
        gen_xorn_i32(dst, s1.value, s2.value);
        break;
    case 0x0e:
        /* merge_{op}s */
        mrgc = tcg_temp_new_i32();
        gen_mrgc_i32(ctx, instr->chan, mrgc);
        gen_merge_i32(dst, s1.value, s2.value, mrgc);
        break;
    case 0x10:
        /* add_{op}s */
        tcg_gen_add_i32(dst, s1.value, s2.value);
        break;
    case 0x12:
        /* sub_{op}s */
        tcg_gen_sub_i32(dst, s1.value, s2.value);
        break;
    case 0x14:
        /* scl_{op}s */
        tcg_gen_rotl_i32(dst, s1.value, s2.value);
        break;
    case 0x16:
        /* scr_{op}s */
        tcg_gen_rotr_i32(dst, s1.value, s2.value);
        break;
    case 0x18:
        /* shl_{op}s */
        tcg_gen_shl_i32(dst, s1.value, s2.value);
        break;
    case 0x1a:
        /* shr_{op}s */
        tcg_gen_shr_i32(dst, s1.value, s2.value);
        break;
    case 0x1c:
        /* sar_{op}s */
        tcg_gen_sar_i32(dst, s1.value, s2.value);
        break;
    case 0x1e:
        /* gef_{op}s */
        gen_getfs(dst, s1.value, s2.value);
        break;
    default:
        e2k_tr_gen_exception(ctx, E2K_EXCP_ILLOPC);
        return;
    }

    if (instr->opc2 == 0x08) {
        switch (opc2) {
        case 0x00:
            /* {op}_ands */
            tcg_gen_and_i32(dst, dst, s3.value);
            break;
        case 0x20:
            /* {op}_andns */
            gen_andn_i32(dst, dst, s3.value);
            break;
        case 0x40:
            /* {op}_ors */
            tcg_gen_or_i32(dst, dst, s3.value);
            break;
        case 0x60:
            /* {op}_orns */
            gen_orn_i32(dst, dst, s3.value);
            break;
        default:
            g_assert_not_reached();
            break;
        }
    } else if (instr->opc2 == 0x09) {
        switch(opc2) {
        case 0x00:
            /* {op}_xors */
            tcg_gen_xor_i32(dst, dst, s3.value);
            break;
        case 0x20:
            /* {op}_xorns */
            gen_xorn_i32(dst, dst, s3.value);
            break;
        case 0x40:
            /* {op}_rsubs */
            tcg_gen_sub_i32(dst, s3.value, dst);
            break;
        case 0x60:
            /* {op}_merges */
            if (ctx->version == 1) {
                if (mrgc == NULL) {
                    mrgc = tcg_temp_new_i32();
                    gen_mrgc_i32(ctx, instr->chan, mrgc);
                }
                gen_merge_i32(dst, dst, s3.value, mrgc);
            } else {
                e2k_tr_gen_exception(ctx, E2K_EXCP_ILLOPC);
                return;
            }
            break;
        default:
            g_assert_not_reached();
            break;
        }
    } else if (instr->opc2 == 0x0a) {
        switch (opc2) {
        case 0x00:
            /* {op}_adds */
            tcg_gen_add_i32(dst, dst, s3.value);
            break;
        case 0x20:
            /* {op}_subs */
            tcg_gen_sub_i32(dst, dst, s3.value);
            break;
        case 0x40:
            /* {op}_scls */
            if (ctx->version == 1) {
                tcg_gen_rotl_i32(dst, dst, s3.value);
            } else {
                e2k_tr_gen_exception(ctx, E2K_EXCP_ILLOPC);
                return;
            }
            break;
        case 0x60:
            /* {op}_scrs */
            if (ctx->version == 1) {
                tcg_gen_rotr_i32(dst, dst, s3.value);
            } else {
                e2k_tr_gen_exception(ctx, E2K_EXCP_ILLOPC);
                return;
            }
            break;
        default:
            g_assert_not_reached();
            break;
        }
    } else if (instr->opc2 == 0x0b) {
        if (ctx->version != 1) {
            e2k_tr_gen_exception(ctx, E2K_EXCP_ILLOPC);
            return;
        }

        switch (opc2) {
        case 0x00:
            /* {op}_shls */
            tcg_gen_shl_i32(dst, dst, s3.value);
            break;
        case 0x20:
            /* {op}_shrs */
            tcg_gen_shr_i32(dst, dst, s3.value);
            break;
        case 0x40:
            /* {op}_sars */
            tcg_gen_sar_i32(dst, dst, s3.value);
            break;
        case 0x60:
            /* {op}_getfs */
            gen_getfs(dst, dst, s3.value);
            break;
        default:
            g_assert_not_reached();
            break;
        }
    }

    if (mrgc != NULL) {
        tcg_temp_free_i32(mrgc);
    }

    gen_al_result_i32(ctx, instr->chan, dst, tag);
}

static void execute_icomb(DisasContext *ctx, Instr *instr)
{
    if (!is_chan_14(instr->chan)) {
        e2k_tr_gen_exception(ctx, E2K_EXCP_ILLOPC);
        return;
    }

    assert(0x08 <= instr->opc2 && instr->opc2 <= 0x0b);

    if (instr->opc1 & 1) {
        execute_icomb_i64(ctx, instr);
    } else {
        execute_icomb_i32(ctx, instr);
    }
}

static void execute_ext_0b(DisasContext *ctx, Instr *instr)
{
    switch(instr->opc1) {
    case 0x6c:
        if (is_chan_0134(instr->chan) && ctx->version >= 4) {
            /* insfs */
            gen_alopf21_i32(ctx, instr, gen_insert_field_i32);
            return;
        }
        break;
    case 0x6d:
        if (is_chan_0134(instr->chan) && ctx->version>= 4) {
            /* insfd */
            gen_alopf21_i64(ctx, instr, gen_insert_field_i64);
            return;
        }
        break;
    default:
        execute_icomb(ctx, instr);
        break;
    }
}

static void execute_ext_0f(DisasContext *ctx, Instr *instr)
{
    switch(instr->opc1) {
    case 0x4d:
        if (is_chan_0134(instr->chan) && ctx->version >= 2) {
            /* pshufb */
            gen_alopf21_i64(ctx, instr, gen_helper_packed_shuffle_i64);
            return;
        }
        break;
    default:
        break;
    }

    e2k_tr_gen_exception(ctx, E2K_EXCP_ILLOPC);
}

static void chan_execute(DisasContext *ctx, int chan)
{
    uint16_t rlp = find_cond(ctx, chan);
    TCGLabel *l0 = gen_new_label();
    Instr instr = { 0 };
    TCGv_i64 cond = NULL;

    instr.chan = chan;
    instr.als = ctx->bundle.als[chan];
    instr.ales = ctx->bundle.ales[chan];

    if (rlp != 0) {
        TCGv_i64 t0 = tcg_temp_new_i64();
        uint8_t psrc = GET_FIELD(rlp, 0, 7);
        bool neg = GET_BIT(rlp, 7 + chan % 3);

        e2k_gen_cond_i64(ctx, t0, psrc);
        cond = e2k_get_temp_i64(ctx);
        tcg_gen_xori_i64(cond, t0, neg);
        tcg_gen_brcondi_i64(TCG_COND_EQ, cond, 0, l0);

        tcg_temp_free_i64(t0);
    }

    switch (instr.opc2) {
    case 0x00: execute_ext_00(ctx, &instr); break; /* no ales */
    case 0x01: execute_ext_01(ctx, &instr); break;
    case 0x08:
    case 0x09:
    case 0x0a: execute_icomb(ctx, &instr); break;
    case 0x0b: execute_ext_0b(ctx, &instr); break;
    case 0x0f: execute_ext_0f(ctx, &instr); break;
    default:
        e2k_tr_gen_exception(ctx, E2K_EXCP_ILLOPC);
        break;
    }

    gen_set_label(l0);

    ctx->al_cond[chan].is_set = rlp != 0;
    ctx->al_cond[chan].value = cond;
}

void e2k_alc_execute(DisasContext *ctx)
{
    unsigned int i;

    for (i = 0; i < 6; i++) {
        ctx->al_results[i].type = AL_RESULT_NONE;

        if (ctx->bundle.als_present[i]) {
            chan_execute(ctx, i);
        }
    }
}

void e2k_alc_commit(DisasContext *ctx)
{
    unsigned int i;

    for (i = 0; i < 6; i++) {
        AlResult *res = &ctx->al_results[i];

        if (res->type == AL_RESULT_REG32 || res->type == AL_RESULT_REG64) {
            uint8_t dst = res->reg.dst;
            if (IS_REGULAR(dst)) {
                e2k_gen_reg_index_from_wregi(res->reg.index, GET_REGULAR(dst));
            }
        }
    }

    for (i = 0; i < 6; i++) {
        TCGLabel *l0 = gen_new_label();
        AlResult *res = &ctx->al_results[i];
        AlCond *cond = &ctx->al_cond[i];

        if (!ctx->bundle.als_present[i]) {
            continue;
        }

        if (res->type != AL_RESULT_NONE && cond->is_set) {
            tcg_gen_brcondi_i64(TCG_COND_EQ, cond->value, 0, l0);
        }

        switch(res->type) {
        case AL_RESULT_REG32:
            e2k_gen_reg_tag_write_i32(res->reg.tag, res->reg.index);
            e2k_gen_reg_write_i32(res->reg.v32, res->reg.index);
            break;
        case AL_RESULT_REG64:
            e2k_gen_reg_tag_write_i64(res->reg.tag, res->reg.index);
            e2k_gen_reg_write_i64(res->reg.v64, res->reg.index);
            break;
        case AL_RESULT_PREG:
            e2k_gen_store_preg(res->preg.index, res->preg.value);
            break;
        case AL_RESULT_CTPR32: {
            TCGv_i64 ctpr = e2k_cs.ctprs[res->ctpr32.index];
            TCGv_i64 t0 = tcg_temp_new_i64();
            TCGv_i64 t1 = tcg_const_i64(CTPR_TAG_DISP);

            assert(res->ctpr32.index < 3);
            tcg_gen_extu_i32_i64(t0, res->ctpr32.value);
            tcg_gen_deposit_i64(ctpr, ctpr, t0, 0, 48);
            tcg_gen_deposit_i64(ctpr, ctpr, t1, CTPR_TAG_OFF,
                CTPR_TAG_LEN);
            tcg_temp_free_i64(t1);
            tcg_temp_free_i64(t0);
            break;
        }
        case AL_RESULT_CTPR64: {
            TCGv_i64 ctpr = e2k_cs.ctprs[res->ctpr64.index];
            TCGv_i64 t0 = tcg_const_i64(CTPR_TAG_DISP);

            assert(res->ctpr64.index < 3);
            tcg_gen_deposit_i64(ctpr, ctpr, res->ctpr64.value, 0, 48);
            tcg_gen_deposit_i64(ctpr, ctpr, t0, CTPR_TAG_OFF,
                CTPR_TAG_LEN);
            tcg_temp_free_i64(t0);
            break;
        }
        default:
            break;
        }

        gen_set_label(l0);
    }
}
