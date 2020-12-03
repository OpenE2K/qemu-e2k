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

static inline void gen_reg_index(TCGv_i32 ret, uint8_t arg)
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

static inline void gen_reg_i64(DisasContext *ctx, Src64 *ret, uint8_t arg)
{
    TCGv_i32 t0 = tcg_temp_new_i32();

    gen_reg_index(t0, arg);
    ret->tag = e2k_get_temp_i32(ctx);
    ret->value = e2k_get_temp_i64(ctx);
    e2k_gen_reg_tag_read_i64(ret->tag, t0);
    e2k_gen_reg_read_i64(ret->value, t0);

    tcg_temp_free_i32(t0);
}

static inline void gen_reg_i32(DisasContext *ctx, Src32 *ret, uint8_t arg)
{
    TCGv_i32 t0 = tcg_temp_new_i32();

    gen_reg_index(t0, arg);
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
        res->type = AL_RESULT_CTPR64;
        res->ctpr64.index = (arg & 3) - 1;
        res->ctpr64.value = value;
    } else {
        res->type = AL_RESULT_REG64;
        res->reg.v64 = value;
        res->reg.tag = tag;
        res->reg.index = e2k_get_temp_i32(ctx);
        gen_reg_index(res->reg.index, arg);
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
        gen_reg_index(res->reg.index, arg);
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

static void gen_tag_check_i64(bool sm, TCGv_i64 dst, TCGv_i32 dst_tag,
    TCGv_i32 src_tag, TCGLabel *l)
{
    if (src_tag != NULL) {
        TCGLabel *l0 = gen_new_label();
        TCGv_i32 t0 = tcg_temp_new_i32();

        e2k_gen_reg_tag_check_i64(t0, src_tag);
        tcg_gen_brcondi_i32(TCG_COND_EQ, t0, 0, l0);
        if (sm) {
            if (dst_tag != NULL) {
                tcg_gen_movi_i32(dst_tag, 5);
            }
            if (dst != NULL) {
                tcg_gen_ori_i64(dst, dst, (1UL << 62) | (1UL << 30));
            }
        } else {
            e2k_gen_exception(E2K_EXCP_ILLOPN);
        }
        if (l != NULL) {
            tcg_gen_br(l);
        }
        gen_set_label(l0);

        tcg_temp_free_i32(t0);
    }
}

static void gen_tag_check_i32(bool sm, TCGv_i32 dst, TCGv_i32 dst_tag,
    TCGv_i32 tag, TCGLabel *l)
{
    if (tag != NULL) {
        TCGLabel *l0 = gen_new_label();
        TCGv_i32 t0 = tcg_temp_new_i32();

        e2k_gen_reg_tag_check_i32(t0, tag);
        tcg_gen_brcondi_i32(TCG_COND_EQ, t0, 0, l0);
        if (sm) {
            if (dst_tag != NULL) {
                tcg_gen_movi_i32(dst_tag, 1);
            }
            if (dst != NULL) {
                tcg_gen_ori_i32(dst, dst, 1 << 30);
            }
        } else {
            e2k_gen_exception(E2K_EXCP_ILLOPN);
        }
        if (l != NULL) {
            tcg_gen_br(l);
        }
        gen_set_label(l0);

        tcg_temp_free_i32(t0);
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

static inline void gen_merge_i32(TCGv_i32 ret, TCGv_i32 x, TCGv_i32 y, TCGv_i32 cond)
{
    TCGv_i32 zero = tcg_const_i32(0);
    tcg_gen_movcond_i32(TCG_COND_EQ, ret, cond, zero, x, y);
    tcg_temp_free_i32(zero);
}

static inline void gen_merge_i64(TCGv_i64 ret, TCGv_i64 x, TCGv_i64 y, TCGv_i64 cond)
{
    TCGv_i64 zero = tcg_const_i64(0);
    tcg_gen_movcond_i64(TCG_COND_EQ, ret, cond, zero, x, y);
    tcg_temp_free_i64(zero);
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

static inline void gen_mrgc_i64(DisasContext *ctx, int chan, TCGv_i64 ret)
{
    uint16_t rlp = find_mrgc(ctx, chan);
    TCGv_i64 t0 = tcg_temp_new_i64();

    e2k_gen_cond_i64(ctx, t0, rlp & 0x7f);
    tcg_gen_xori_i64(ret, t0, GET_BIT(rlp, 7 + chan % 3));

    tcg_temp_free_i64(t0);
}

static inline void gen_mrgc_i32(DisasContext *dc, int chan, TCGv_i32 ret)
{
    TCGv_i64 t0 = tcg_temp_new_i64();
    gen_mrgc_i64(dc, chan, t0);
    tcg_gen_extrl_i64_i32(ret, t0);
    tcg_temp_free(t0);
}

static inline void gen_udivd(TCGv_i64 ret, TCGv_i32 ret_tag, bool sm,
    TCGv_i64 src1, TCGv_i64 src2)
{
    TCGLabel *l0 = gen_new_label();

    if (sm) {
        TCGLabel *l1 = gen_new_label();
        tcg_gen_brcondi_i64(TCG_COND_NE, src2, 0, l1);
        tcg_gen_movi_i32(ret_tag, 5);
        tcg_gen_movi_i64(ret, 0);
        tcg_gen_br(l0);
        gen_set_label(l1);
    }
    tcg_gen_movi_i32(ret_tag, 0);
    tcg_gen_divu_i64(ret, src1, src2);
    gen_set_label(l0);
}

static inline void gen_udivs(TCGv_i32 ret, TCGv_i32 ret_tag, bool sm,
    TCGv_i32 src1, TCGv_i32 src2)
{
    TCGLabel *l0 = gen_new_label();

    if (sm) {
        TCGLabel *l1 = gen_new_label();
        tcg_gen_brcondi_i32(TCG_COND_NE, src2, 0, l1);
        tcg_gen_movi_i32(ret_tag, 5);
        tcg_gen_movi_i32(ret, 0);
        tcg_gen_br(l0);
        gen_set_label(l1);
    }
    tcg_gen_movi_i32(ret_tag, 0);
    tcg_gen_divu_i32(ret, src1, src2);
    gen_set_label(l0);
}

static inline void gen_sdivd(TCGv_i64 ret, TCGv_i32 ret_tag, bool sm,
    TCGv_i64 src1, TCGv_i64 src2)
{
    TCGLabel *l0 = gen_new_label();

    if (sm) {
        TCGLabel *l1 = gen_new_label();
        tcg_gen_brcondi_i64(TCG_COND_NE, src2, 0, l1);
        tcg_gen_movi_i32(ret_tag, 5);
        tcg_gen_movi_i64(ret, 0);
        tcg_gen_br(l0);
        gen_set_label(l1);
    }
    tcg_gen_movi_i32(ret_tag, 0);
    tcg_gen_div_i64(ret, src1, src2);
    gen_set_label(l0);
}

static inline void gen_sdivs(TCGv_i32 ret, TCGv_i32 ret_tag, bool sm,
    TCGv_i32 src1, TCGv_i32 src2)
{
    TCGLabel *l0 = gen_new_label();

    if (sm) {
        TCGLabel *l1 = gen_new_label();
        tcg_gen_brcondi_i32(TCG_COND_NE, src2, 0, l1);
        tcg_gen_movi_i32(ret_tag, 5);
        tcg_gen_movi_i32(ret, 0);
        tcg_gen_br(l0);
        gen_set_label(l1);
    }
    tcg_gen_movi_i32(ret_tag, 0);
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

    tcg_gen_movi_i32(tag, 0);
    tcg_gen_mov_i64(dst, s1.value);
    tcg_gen_brcondi_i32(TCG_COND_EQ, s2.value, 0, l0);
    gen_tag_check_i64(sm, dst, tag, s1.tag, l1);
    gen_tag_check_i64(sm, dst, tag, s2.tag, l1);
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

    tcg_gen_movi_i32(tag, 0);
    tcg_gen_mov_i32(dst, s1.value);
    tcg_gen_brcondi_i32(TCG_COND_EQ, s2.value, 0, l0);
    gen_tag_check_i32(sm, dst, tag, s1.tag, l1);
    gen_tag_check_i32(sm, dst, tag, s2.tag, l1);
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
    TCGv_i32 t0;

    gen_tag_check_i64(false, NULL, NULL, s2.tag, NULL);
    t0 = tcg_const_i32(als & 0xff);
    gen_helper_state_reg_write_i64(cpu_env, t0, s2.value);
    tcg_temp_free_i32(t0);
}

static inline void gen_rw_i32(DisasContext *ctx, int chan)
{
    uint32_t als = ctx->bundle.als[chan];
    Src32 s2 = get_src2_i32(ctx, chan);
    TCGv_i32 t0;

    gen_tag_check_i32(false, NULL, NULL, s2.tag, NULL);
    t0 = tcg_const_i32(als & 0xff);
    gen_helper_state_reg_write_i32(cpu_env, t0, s2.value);
    tcg_temp_free_i32(t0);
}

static void gen_getsp(DisasContext *ctx, int chan)
{
    Src64 s2 = get_src2_i64(ctx, chan);
    TCGv_i64 dst = e2k_get_temp_i64(ctx);

    // TODO: check tags
    gen_helper_getsp(dst, cpu_env, s2.value);
    set_al_result_reg64(ctx, chan, dst);
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
        // TODO: special mas
        e2k_tr_gen_exception(ctx, E2K_EXCP_ILLOPC);
    } else if (mas) {
        int mod = extract8(mas, 0, 3);
//        int dc = extract8(mas, 5, 2);

        if (mod != 0) {
            // TODO: mas modes
            e2k_tr_gen_exception(ctx, E2K_EXCP_ILLOPC);
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
    TCGv_i64 t0 = tcg_temp_local_new_i64();
    TCGv_i32 tag = e2k_get_temp_i32(ctx);
    TCGv_i64 dst = e2k_get_temp_i64(ctx);

    tcg_gen_movi_i64(dst, 0);
    gen_tag_check_i64(sm, dst, tag, s1.tag, l1);
    gen_tag_check_i64(sm, dst, tag, s2.tag, l1);

    tcg_gen_add_i64(t0, s1.value, s2.value);
    memop = gen_mas(ctx, chan, memop, t0);

    if (sm) {
        TCGv_i32 t1 = tcg_temp_new_i32();

        gen_helper_probe_read_access(t1, cpu_env, t0);
        tcg_gen_brcondi_i32(TCG_COND_EQ, t1, 1, l0);
        tcg_temp_free_i32(t1);
        tcg_gen_movi_i32(tag, 5);
        tcg_gen_movi_i64(dst, 0x4afafafa4afafafa);
        tcg_gen_br(l1);
    }

    gen_set_label(l0);
    tcg_gen_movi_i32(tag, 0);
    tcg_gen_qemu_ld_i64(dst, t0, ctx->mmuidx, memop);

    gen_set_label(l1);
    set_al_result_reg64_tag(ctx, chan, dst, tag);

    tcg_temp_free_i64(t0);
}

static void gen_staa_i64(DisasContext *ctx, int chan)
{
    uint32_t als = ctx->bundle.als[chan];
    uint8_t mas = ctx->mas[chan];
    bool sm = extract32(als, 31, 1);
    int lit = extract32(als, 8, 2);
    int am = extract32(als, 10, 1);
    int incr = extract32(als, 12, 3);
    int ind = extract32(als, 15, 4);
    int d = extract32(als, 19, 5);
    Src64 s4 = get_src4_i64(ctx, chan);
    TCGv_i32 t0 = tcg_const_i32(am);
    TCGv_i32 t1 = tcg_const_i32(incr);
    TCGv_i32 t2 = tcg_const_i32(ind);
    TCGv_i32 t3 = tcg_const_i32(d);
    TCGv_i32 t4;

    if (lit) {
        if (!ctx->bundle.lts_present[lit - 1]) {
            e2k_tr_gen_exception(ctx, E2K_EXCP_ILLOPN);
            return;
        }
        t4 = tcg_const_i32(ctx->bundle.lts[lit - 1]);
    } else {
        t4 = tcg_const_i32(0);
    }

    if (sm) {
        qemu_log_mask(LOG_UNIMP, "staad: sm is not implemented\n");
        abort();
    }

    if (mas == 0) {
        gen_helper_staa_i64(cpu_env, s4.value, t4, t0, t1, t2, t3);
    } else if (mas == 0x3f) {
        if (!am) {
            gen_helper_set_aad_i64(cpu_env, s4.value, t1, t3);
        } else {
            gen_helper_set_aasti_i64(cpu_env, s4.value, t2);
        }
    } else {
        qemu_log_mask(LOG_UNIMP, "staad: not implemented mas\n");
        abort();
    }

    tcg_temp_free_i32(t4);
    tcg_temp_free_i32(t3);
    tcg_temp_free_i32(t2);
    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
}
static void gen_staa_i32(DisasContext *ctx, int chan)
{
    uint32_t als = ctx->bundle.als[chan];
    uint8_t mas = ctx->mas[chan];
    bool sm = extract32(als, 31, 1);
    int lit = extract32(als, 8, 2);
    int am = extract32(als, 10, 1);
    int incr = extract32(als, 12, 3);
    int ind = extract32(als, 15, 4);
    int d = extract32(als, 19, 5);
    Src32 s4 = get_src4_i32(ctx, chan);
    TCGv_i32 t0 = tcg_const_i32(am);
    TCGv_i32 t1 = tcg_const_i32(incr);
    TCGv_i32 t2 = tcg_const_i32(ind);
    TCGv_i32 t3 = tcg_const_i32(d);
    TCGv_i32 t4;

    if (lit) {
        if (!ctx->bundle.lts_present[lit - 1]) {
            e2k_tr_gen_exception(ctx, E2K_EXCP_ILLOPN);
            return;
        }
        t4 = tcg_const_i32(ctx->bundle.lts[lit - 1]);
    } else {
        t4 = tcg_const_i32(0);
    }

    if (sm) {
        qemu_log_mask(LOG_UNIMP, "staaw: sm is not implemented\n");
        abort();
    }

    if (mas == 0) {
        gen_helper_staa_i32(cpu_env, s4.value, t4, t0, t1, t2, t3);
    } else if (mas == 0x3f) {
        if (!am) {
            gen_helper_set_aad_i32(cpu_env, s4.value, t1, t3);
        } else {
            gen_helper_set_aasti_i32(cpu_env, s4.value, t2);
        }
    } else {
        qemu_log_mask(LOG_UNIMP, "staaw: not implemented mas\n");
        abort();
    }

    tcg_temp_free_i32(t4);
    tcg_temp_free_i32(t3);
    tcg_temp_free_i32(t2);
    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
}

static void gen_st(DisasContext *ctx, int chan, MemOp memop)
{
    bool sm = GET_BIT(ctx->bundle.als[chan], 31);
    TCGLabel *l0 = gen_new_label();
    TCGLabel *l1 = gen_new_label();
    Src64 s1 = get_src1_i64(ctx, chan);
    Src64 s2 = get_src2_i64(ctx, chan);
    Src64 s4 = get_src4_i64(ctx, chan);
    TCGv_i64 t0 = tcg_temp_local_new_i64();

    gen_tag_check_i64(sm, NULL, NULL, s1.tag, l1);
    gen_tag_check_i64(sm, NULL, NULL, s2.tag, l1);
    gen_tag_check_i64(sm, NULL, NULL, s4.tag, l1);

    tcg_gen_add_i64(t0, s1.value, s2.value);
    memop = gen_mas(ctx, chan, memop, t0);

    if (sm) {
        TCGv_i32 t1 = tcg_temp_new_i32();

        gen_helper_probe_write_access(t1, cpu_env, t0);
        tcg_gen_brcondi_i32(TCG_COND_EQ, t1, 0, l1);
        tcg_temp_free_i32(t1);
    }

    gen_set_label(l0);
    tcg_gen_qemu_st_i64(s4.value, t0, ctx->mmuidx, memop);
    gen_set_label(l1);

    tcg_temp_free_i64(t0);
}

static void gen_alopf1_i32(DisasContext *ctx, int chan,
    void (*op)(TCGv_i32, TCGv_i32, TCGv_i32))
{
    bool sm = GET_BIT(ctx->bundle.als[chan], 31);
    Src32 s1 = get_src1_i32(ctx, chan);
    Src32 s2 = get_src2_i32(ctx, chan);
    TCGv_i32 tag = e2k_get_temp_i32(ctx);
    TCGv_i32 dst = e2k_get_temp_i32(ctx);

    tcg_gen_movi_i32(tag, 0);
    (*op)(dst, s1.value, s2.value);

    gen_tag_check_i32(sm, dst, tag, s1.tag, NULL);
    gen_tag_check_i32(sm, dst, tag, s2.tag, NULL);

    set_al_result_reg32_tag(ctx, chan, dst, tag);
}

static void gen_alopf1_i64(DisasContext *ctx, int chan,
    void (*op)(TCGv_i64, TCGv_i64, TCGv_i64))
{
    bool sm = GET_BIT(ctx->bundle.als[chan], 31);
    Src64 s1 = get_src1_i64(ctx, chan);
    Src64 s2 = get_src2_i64(ctx, chan);
    TCGv_i32 tag = e2k_get_temp_i32(ctx);
    TCGv_i64 dst = e2k_get_temp_i64(ctx);

    tcg_gen_movi_i32(tag, 0);
    (*op)(dst, s1.value, s2.value);

    gen_tag_check_i64(sm, dst, tag, s1.tag, NULL);
    gen_tag_check_i64(sm, dst, tag, s2.tag, NULL);

    set_al_result_reg64_tag(ctx, chan, dst, tag);
}

static void gen_alopf1_tag_i64(DisasContext *ctx, int chan,
    void (*op)(TCGv_i64, TCGv_i32, bool, TCGv_i64, TCGv_i64))
{
    bool sm = GET_BIT(ctx->bundle.als[chan], 31);
    TCGLabel *l0 = gen_new_label();
    TCGLabel *l1 = gen_new_label();
    Src64 s1 = get_src1_i64(ctx, chan);
    Src64 s2 = get_src2_i64(ctx, chan);
    TCGv_i32 tag = e2k_get_temp_i32(ctx);
    TCGv_i64 dst = e2k_get_temp_i64(ctx);

    tcg_gen_movi_i32(tag, 0);
    (*op)(dst, tag, sm, s1.value, s2.value);
    tcg_gen_brcondi_i32(TCG_COND_EQ, tag, 0, l0);
    tcg_gen_ori_i64(dst, dst, (1UL << 62) | (1UL << 30));
    tcg_gen_br(l1);

    gen_set_label(l0);
    gen_tag_check_i64(sm, dst, tag, s1.tag, NULL);
    gen_tag_check_i64(sm, dst, tag, s2.tag, NULL);

    gen_set_label(l1);
    set_al_result_reg64_tag(ctx, chan, dst, tag);
}

static void gen_alopf1_tag_i32(DisasContext *ctx, int chan,
    void (*op)(TCGv_i32, TCGv_i32, bool, TCGv_i32, TCGv_i32))
{
    bool sm = GET_BIT(ctx->bundle.als[chan], 31);
    TCGLabel *l0 = gen_new_label();
    TCGLabel *l1 = gen_new_label();
    Src32 s1 = get_src1_i32(ctx, chan);
    Src32 s2 = get_src2_i32(ctx, chan);
    TCGv_i32 tag = e2k_get_temp_i32(ctx);
    TCGv_i32 dst = e2k_get_temp_i32(ctx);

    tcg_gen_movi_i32(tag, 0);
    (*op)(dst, tag, sm, s1.value, s2.value);
    tcg_gen_brcondi_i32(TCG_COND_EQ, tag, 0, l0);
    tcg_gen_ori_i32(dst, dst, 1UL << 30);
    tcg_gen_br(l1);

    gen_set_label(l0);
    gen_tag_check_i32(sm, dst, tag, s1.tag, NULL);
    gen_tag_check_i32(sm, dst, tag, s2.tag, NULL);

    gen_set_label(l1);
    set_al_result_reg32_tag(ctx, chan, dst, tag);
}

static void gen_alopf1_cmp_i64(DisasContext *ctx, int chan,
    void (*op)(TCGv_i64, int, TCGv_i64, TCGv_i64))
{
    uint32_t als = ctx->bundle.als[chan];
    bool sm = extract32(als, 31, 1);
    int opc = extract32(als, 5, 3);
    int idx = extract32(als, 0, 5);
    TCGLabel *l0 = gen_new_label();
    TCGLabel *l1 = gen_new_label();
    Src64 s1 = get_src1_i64(ctx, chan);
    Src64 s2 = get_src2_i64(ctx, chan);
    TCGv_i64 dst = e2k_get_temp_i64(ctx);

    gen_tag_check_i32(sm, NULL, NULL, s1.tag, l0);
    gen_tag_check_i32(sm, NULL, NULL, s2.tag, l0);

    (*op)(dst, opc, s1.value, s2.value);
    tcg_gen_br(l1);

    gen_set_label(l0);
    tcg_gen_movi_i64(dst, 2);

    gen_set_label(l1);
    set_al_result_preg(ctx, chan, idx, dst);
}

static void gen_alopf1_cmp_i32(DisasContext *ctx, int chan,
    void (*op)(TCGv_i32, int, TCGv_i32, TCGv_i32))
{
    uint32_t als = ctx->bundle.als[chan];
    bool sm = extract32(als, 31, 1);
    int opc = extract32(als, 5, 3);
    int idx = extract32(als, 0, 5);
    TCGLabel *l0 = gen_new_label();
    TCGLabel *l1 = gen_new_label();
    Src32 s1 = get_src1_i32(ctx, chan);
    Src32 s2 = get_src2_i32(ctx, chan);
    TCGv_i64 dst = e2k_get_temp_i64(ctx);
    TCGv_i32 t0 = tcg_temp_new_i32();

    gen_tag_check_i32(sm, NULL, NULL, s1.tag, l0);
    gen_tag_check_i32(sm, NULL, NULL, s2.tag, l0);

    (*op)(t0, opc, s1.value, s2.value);
    tcg_gen_extu_i32_i64(dst, t0);
    tcg_gen_br(l1);
    tcg_temp_free_i32(t0);

    gen_set_label(l0);
    tcg_gen_movi_i64(dst, 2);

    gen_set_label(l1);
    set_al_result_preg(ctx, chan, idx, dst);
}

static void gen_alopf1_mrgc_i32(DisasContext *ctx, int chan)
{
    bool sm = extract32(ctx->bundle.als[chan], 31, 1);
    Src32 s1 = get_src1_i32(ctx, chan);
    Src32 s2 = get_src2_i32(ctx, chan);
    TCGv_i32 tag = e2k_get_temp_i32(ctx);
    TCGv_i32 dst = e2k_get_temp_i32(ctx);
    TCGv_i32 cond = tcg_temp_new_i32();

    gen_mrgc_i32(ctx, chan, cond);
    gen_merge_i32(dst, s1.value, s2.value, cond);
    tcg_gen_movi_i32(tag, 0);
    gen_tag_check_i32(sm, dst, tag, s1.tag, NULL);
    gen_tag_check_i32(sm, dst, tag, s2.tag, NULL);
    set_al_result_reg32_tag(ctx, chan, dst, tag);

    tcg_temp_free_i32(cond);
}

static void gen_alopf1_mrgc_i64(DisasContext *ctx, int chan)
{
    bool sm = extract32(ctx->bundle.als[chan], 31, 1);
    Src64 s1 = get_src1_i64(ctx, chan);
    Src64 s2 = get_src2_i64(ctx, chan);
    TCGv_i64 cond = tcg_temp_new_i64();
    TCGv_i32 tag = e2k_get_temp_i32(ctx);
    TCGv_i64 dst = e2k_get_temp_i64(ctx);

    gen_mrgc_i64(ctx, chan, cond);
    gen_merge_i64(dst, s1.value, s2.value, cond);
    tcg_gen_movi_i32(tag, 0);
    gen_tag_check_i64(sm, dst, tag, s1.tag, NULL);
    gen_tag_check_i64(sm, dst, tag, s2.tag, NULL);
    set_al_result_reg64_tag(ctx, chan, dst, tag);

    tcg_temp_free_i64(cond);
}

static void gen_alopf21_i64(DisasContext *ctx, int chan,
    void (*op)(TCGv_i64, TCGv_i64, TCGv_i64, TCGv_i64))
{
    bool sm = GET_BIT(ctx->bundle.als[chan], 31);
    Src64 s1 = get_src1_i64(ctx, chan);
    Src64 s2 = get_src2_i64(ctx, chan);
    Src64 s3 = get_src3_i64(ctx, chan);
    TCGv_i32 tag = e2k_get_temp_i32(ctx);
    TCGv_i64 dst = e2k_get_temp_i64(ctx);

    tcg_gen_movi_i32(tag, 0);
    (*op)(dst, s1.value, s2.value, s3.value);

    gen_tag_check_i64(sm, dst, tag, s1.tag, NULL);
    gen_tag_check_i64(sm, dst, tag, s2.tag, NULL);
    gen_tag_check_i64(sm, dst, tag, s3.tag, NULL);

    set_al_result_reg64_tag(ctx, chan, dst, tag);
}

static void gen_alopf21_i32(DisasContext *ctx, int chan,
    void (*op)(TCGv_i32, TCGv_i32, TCGv_i32, TCGv_i32))
{
    bool sm = GET_BIT(ctx->bundle.als[chan], 31);
    Src32 s1 = get_src1_i32(ctx, chan);
    Src32 s2 = get_src2_i32(ctx, chan);
    Src32 s3 = get_src3_i32(ctx, chan);
    TCGv_i32 tag = e2k_get_temp_i32(ctx);
    TCGv_i32 dst = e2k_get_temp_i32(ctx);

    tcg_gen_movi_i32(tag, 0);
    (*op)(dst, s1.value, s2.value, s3.value);

    gen_tag_check_i32(sm, dst, tag, s1.tag, NULL);
    gen_tag_check_i32(sm, dst, tag, s2.tag, NULL);
    gen_tag_check_i32(sm, dst, tag, s3.tag, NULL);

    set_al_result_reg32_tag(ctx, chan, dst, tag);
}

static void execute_alopf_simple(DisasContext *dc, int chan)
{
    uint32_t als = dc->bundle.als[chan];
    int opc = GET_FIELD(als, 24, 7);

    switch(opc) {
    case 0x00: /* ands */ gen_alopf1_i32(dc, chan, tcg_gen_and_i32); break;
    case 0x01: /* andd */ gen_alopf1_i64(dc, chan, tcg_gen_and_i64); break;
    case 0x02: /* andns */ gen_alopf1_i32(dc, chan, gen_andn_i32); break;
    case 0x03: /* andnd */ gen_alopf1_i64(dc, chan, gen_andn_i64); break;
    case 0x04: /* ors */ gen_alopf1_i32(dc, chan, tcg_gen_or_i32); break;
    case 0x05: /* ord */ gen_alopf1_i64(dc, chan, tcg_gen_or_i64); break;
    case 0x06: /* orns */ gen_alopf1_i32(dc, chan, gen_orn_i32); break;
    case 0x07: /* ornd */ gen_alopf1_i64(dc, chan, gen_orn_i64); break;
    case 0x08: /* xors */ gen_alopf1_i32(dc, chan, tcg_gen_xor_i32); break;
    case 0x09: /* xord */ gen_alopf1_i64(dc, chan, tcg_gen_xor_i64); break;
    case 0x0a: /* xorns */ gen_alopf1_i32(dc, chan, gen_xorn_i32); break;
    case 0x0b: /* xornd */ gen_alopf1_i64(dc, chan, gen_xorn_i64); break;
    case 0x0c: /* sxt */ gen_alopf1_i64(dc, chan, gen_helper_sxt); break;
    case 0x0e: /* merges */ gen_alopf1_mrgc_i32(dc, chan); break;
    case 0x0f: /* merged */ gen_alopf1_mrgc_i64(dc, chan); break;
    case 0x10: /* adds */ gen_alopf1_i32(dc, chan, tcg_gen_add_i32); break;
    case 0x11: /* addd */ gen_alopf1_i64(dc, chan, tcg_gen_add_i64); break;
    case 0x12: /* subs */ gen_alopf1_i32(dc, chan, tcg_gen_sub_i32); break;
    case 0x13: /* subd */ gen_alopf1_i64(dc, chan, tcg_gen_sub_i64); break;
    case 0x14: /* scls */ gen_alopf1_i32(dc, chan, tcg_gen_rotl_i32); break;
    case 0x15: /* scld */ gen_alopf1_i64(dc, chan, tcg_gen_rotl_i64); break;
    case 0x16: /* scrs */ gen_alopf1_i32(dc, chan, tcg_gen_rotr_i32); break;
    case 0x17: /* scrd */ gen_alopf1_i64(dc, chan, tcg_gen_rotr_i64); break;
    case 0x18: /* shls */ gen_alopf1_i32(dc, chan, tcg_gen_shl_i32); break;
    case 0x19: /* shld */ gen_alopf1_i64(dc, chan, tcg_gen_shl_i64); break;
    case 0x1a: /* shrs */ gen_alopf1_i32(dc, chan, tcg_gen_shr_i32); break;
    case 0x1b: /* shrd */ gen_alopf1_i64(dc, chan, tcg_gen_shr_i64); break;
    case 0x1c: /* sars */ gen_alopf1_i32(dc, chan, tcg_gen_sar_i32); break;
    case 0x1d: /* sard */ gen_alopf1_i64(dc, chan, tcg_gen_sar_i64); break;
    case 0x1e: /* getfs */ gen_alopf1_i32(dc, chan, gen_getfs); break;
    case 0x1f: /* getfd */ gen_alopf1_i64(dc, chan, gen_getfd); break;
    case 0x20:
        if (is_cmp_chan(chan)) {
            gen_alopf1_cmp_i32(dc, chan, gen_cmp_i32); /* cmp{op}sb */
        } else {
            abort();
        }
        break;
    case 0x21:
        if (is_cmp_chan(chan)) {
            gen_alopf1_cmp_i64(dc, chan, gen_cmp_i64); /* cmp{op}db */
        } else {
            abort();
        }
        break;
    case 0x22:
        if (is_cmp_chan(chan)) {
            gen_alopf1_cmp_i32(dc, chan, gen_cmpand_i32); /* cmpand{op}sb */
        } else {
            abort();
        }
        break;
    case 0x23:
        if (is_cmp_chan(chan)) {
            gen_alopf1_cmp_i64(dc, chan, gen_cmpand_i64); /* cmpand{op}db */
        } else {
            abort();
        }
        break;
    case 0x24: { /* stb */
        if (is_store_chan(chan)) {
            gen_st(dc, chan, MO_UB);
        } else {
            abort();
        }
        break;
    }
    case 0x25: { /* sth */
        if (is_store_chan(chan)) {
            gen_st(dc, chan, MO_UW);
        } else {
            abort();
        }
        break;
    }
    case 0x26: { /* stw */
        if (is_store_chan(chan)) {
            gen_st(dc, chan, MO_UL);
        } else {
            abort();
        }
        break;
    }
    case 0x27: { /* std */
        if (is_store_chan(chan)) {
            gen_st(dc, chan, MO_Q);
        } else {
            abort();
        }
        break;
    }
    case 0x40:
        if (is_div_chan(chan)) {
            // FIXME: temp hack
            TCGLabel *l0 = gen_new_label();
            Src64 s2 = get_src2_i64(dc, chan);
            tcg_gen_brcondi_i64(TCG_COND_NE, s2.value, 0, l0);
            e2k_gen_exception(0);
            gen_set_label(l0);

            gen_alopf1_tag_i32(dc, chan, gen_udivs); /* udivs */
        } else {
            abort();
        }
        break;
    case 0x41:
        if (is_div_chan(chan)) {
            gen_alopf1_tag_i64(dc, chan, gen_udivd); /* udivd */
        } else {
            abort();
        }
        break;
    case 0x42:
        if (is_div_chan(chan)) {
            gen_alopf1_tag_i32(dc, chan, gen_sdivs); /* sdivs */
        } else {
            abort();
        }
        break;
    case 0x43:
        if (is_div_chan(chan)) {
            gen_alopf1_tag_i64(dc, chan, gen_sdivd); /* sdivd */
        } else {
            abort();
        }
        break;
    case 0x61:
        if (chan == 2 || chan == 5) {
            abort();
        } else {
            gen_movtd(dc, chan);
        }
        break;
    case 0x64: { /* ldb */
        if (is_load_chan(chan)) {
            gen_ld(dc, chan, MO_UB);
        } else {
            abort();
        }
        break;
    }
    case 0x65: { /* ldh */
        if (is_load_chan(chan)) {
            gen_ld(dc, chan, MO_UW);
        } else {
            abort();
        }
        break;
    }
    case 0x66: { /* ldw */
        if (is_load_chan(chan)) {
            gen_ld(dc, chan, MO_UL);
        } else {
            abort();
        }
        break;
    }
    case 0x67: { /* ldd */
        if (is_load_chan(chan)) {
            gen_ld(dc, chan, MO_Q);
        } else {
            abort();
        }
        break;
    }
    default:
        e2k_tr_gen_exception(dc, E2K_EXCP_ILLOPC);
        break;
    }
}

static void execute_ext_01_25(DisasContext *ctx, int chan)
{
    uint8_t opc = GET_FIELD(ctx->bundle.als[chan], 24, 7);

    if (chan != 2 && chan != 5) {
        e2k_tr_gen_exception(ctx, E2K_EXCP_ILLOPC);
        return;
    }

    switch(opc) {
    case 0x08: gen_gettag_i32(ctx, chan); break; /* gettags */
    case 0x09: gen_gettag_i64(ctx, chan); break; /* gettagd */
    case 0x0a: gen_puttag_i32(ctx, chan); break; /* puttags */
    case 0x0b: gen_puttag_i64(ctx, chan); break; /* puttagd */
    case 0x1e: gen_staa_i32(ctx, chan); break; /* staaw */
    case 0x1f: gen_staa_i64(ctx, chan); break; /* staad */
    default:
        e2k_tr_gen_exception(ctx, E2K_EXCP_ILLOPC);
        break;
    }
}

static void execute_ext_01(DisasContext *dc, int chan)
{
    uint8_t opc = GET_FIELD(dc->bundle.als[chan], 24, 7);

    switch (opc) {
    case 0x0f:
        if (chan == 0 || chan == 3) {
            // FIXME: what is the point in packed add on vector of 64-bit elements if reg size is 64 bits?
            gen_alopf1_i64(dc, chan, tcg_gen_add_i64); /* paddd */
        } else {
            e2k_tr_gen_exception(dc, E2K_EXCP_ILLOPC);
        }
        break;
    case 0x20:
        if (is_cmp_chan(chan)) {
            gen_alopf1_i32(dc, chan, tcg_gen_mul_i32); /* muls */
        } else {
            e2k_tr_gen_exception(dc, E2K_EXCP_ILLOPC);
        }
        break;
    case 0x21:
        if (is_cmp_chan(chan)) {
            gen_alopf1_i64(dc, chan, tcg_gen_mul_i64); /* muld */
        } else {
            e2k_tr_gen_exception(dc, E2K_EXCP_ILLOPC);
        }
        break;
    case 0x3c: /* rws */ gen_rw_i32(dc, chan); break; /* TODO: only channel 1 */
    case 0x3d: /* rwd */ gen_rw_i64(dc, chan); break; /* TODO: only channel 1 */
    case 0x3e: /* rrs */ gen_rr_i32(dc, chan); break; /* TODO: only channel 1 */
    case 0x3f: /* rrd */ gen_rr_i64(dc, chan); break; /* TODO: only channel 1 */
    case 0x58: {
        if (chan == 0 || chan == 3) {
            gen_getsp(dc, chan);
        }
        /* TODO: exception */
        break;
    }
    default:
        e2k_tr_gen_exception(dc, E2K_EXCP_ILLOPC);
        break;
    }
}

static void execute_ext_0b(DisasContext *ctx, int chan)
{
    uint8_t opc = GET_FIELD(ctx->bundle.als[chan], 24, 7);

    switch(opc) {
    case 0x6c:
        if (is_cmp_chan(chan) && ctx->version >= 4) {
            gen_alopf21_i32(ctx, chan, gen_insert_field_i32); /* insfs */
        } else {
            e2k_tr_gen_exception(ctx, E2K_EXCP_ILLOPC);
        }
        break;
    case 0x6d:
        if (is_cmp_chan(chan) && ctx->version>= 4) {
            gen_alopf21_i64(ctx, chan, gen_insert_field_i64); /* insfd */
        } else {
            e2k_tr_gen_exception(ctx, E2K_EXCP_ILLOPC);
        }
        break;
    default:
        e2k_tr_gen_exception(ctx, E2K_EXCP_ILLOPC);
        break;
    }
}

static void execute_ext_0f(DisasContext *ctx, int chan)
{
    uint8_t opc = GET_FIELD(ctx->bundle.als[chan], 24, 7);

    switch(opc) {
    case 0x4d:
        if (is_cmp_chan(chan) && ctx->version >= 2) {
            gen_alopf21_i64(ctx, chan, gen_helper_packed_shuffle_i64); /* pshufb */
        } else {
            e2k_tr_gen_exception(ctx, E2K_EXCP_ILLOPC);
        }
        break;
    default:
        e2k_tr_gen_exception(ctx, E2K_EXCP_ILLOPC);
        break;
    }
}

void e2k_alc_execute(DisasContext *ctx, int chan)
{
    const UnpackedBundle *bundle = &ctx->bundle;
    uint16_t rlp = find_cond(ctx, chan);
    TCGLabel *l0 = gen_new_label();
    TCGv_i64 cond = NULL;

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

    switch(bundle->ales_present[chan]) {
    case ALES_NONE:
        execute_alopf_simple(ctx, chan);
        break;
    case ALES_ALLOCATED: // 2 or 5 channel
        execute_ext_01_25(ctx, chan);
        break;
    case ALES_PRESENT: {
        uint8_t opc = GET_FIELD(bundle->ales[chan], 8, 8);
        switch (opc) {
        case 0x01:
            execute_ext_01(ctx, chan);
            break;
        case 0x0b:
            execute_ext_0b(ctx, chan);
            break;
        case 0x0f:
            execute_ext_0f(ctx, chan);
            break;
        default:
            e2k_tr_gen_exception(ctx, E2K_EXCP_ILLOPC);
            break;
        }
        break;
    }
    default:
        g_assert_not_reached();
        break;
    }

    gen_set_label(l0);

    ctx->al_cond[chan].is_set = rlp != 0;
    ctx->al_cond[chan].value = cond;
}

void e2k_alc_commit(DisasContext *ctx)
{
    unsigned int i;

    for (i = 0; i < 6; i++) {
        TCGLabel *l0;
        AlResult *res = &ctx->al_results[i];
        AlCond *cond = &ctx->al_cond[i];

        if (!ctx->bundle.als_present[i]) {
            continue;
        }

        if (res->type != AL_RESULT_NONE && cond->is_set) {
            l0 = gen_new_label();
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

        if (res->type != AL_RESULT_NONE && cond->is_set) {
            gen_set_label(l0);
        }
    }
}
