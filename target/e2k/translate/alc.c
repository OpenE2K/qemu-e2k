#include "qemu/osdep.h"
#include "qemu.h"
#include "exec/log.h"
#include "translate.h"

static TCGv_i64 get_src1(DisasContext *dc, uint32_t als)
{
    unsigned int src1 = GET_FIELD(als, 16, 8);
    if (IS_BASED(src1)) {
        unsigned int i = GET_BASED(src1);
        return e2k_get_breg(dc, i);
    } else if (IS_REGULAR(src1)) {
        unsigned int i = GET_REGULAR(src1);
        return e2k_get_wreg(dc, i);
    } else if (IS_IMM5(src1)) {
        unsigned int imm = GET_IMM5(src1);
        TCGv_i64 t = e2k_get_temp_i64(dc);
        tcg_gen_movi_i64(t, imm);
        return t;
    } else {
        unsigned int i = GET_GLOBAL(src1);
        return e2k_get_greg(dc, i);
    }
}

static TCGv_i64 get_src2(DisasContext *dc, uint32_t als)
{
    unsigned int src2 = GET_FIELD(als, 8, 8);
    if (IS_BASED(src2)) {
        unsigned int i = GET_BASED(src2);
        return e2k_get_breg(dc, i);
    } else if (IS_REGULAR(src2)) {
        unsigned int i = GET_REGULAR(src2);
        return e2k_get_wreg(dc, i);
    } else if (IS_IMM4(src2)) {
        unsigned int imm = GET_IMM4(src2);
        TCGv t = e2k_get_temp_i64(dc);
        tcg_gen_movi_i64(t, imm);
        return t;
    } else if (IS_LIT(src2)) {
        TCGv t = e2k_get_temp_i64(dc);
        unsigned int i = GET_LIT(src2);
        uint64_t lit = dc->bundle.lts[i];
        // TODO: exception
        assert(dc->bundle.lts_present[i]);
        if (IS_LIT16_LO(src2) && i < 2) {
            lit = ((int64_t) lit << 48) >> 48;
        } else if (IS_LIT16_HI(src2) && i < 2) {
            lit = ((int64_t) lit << 32) >> 48;
        } else if (IS_LIT32(src2)) {
            lit = ((int64_t) lit << 32) >> 32;
        } else if (IS_LIT64(src2) && i < 3) {
            if (!dc->bundle.lts_present[i + 1]) {
                // TODO: check what exception must be raised
                e2k_tr_gen_exception(dc, E2K_EXCP_MAPERR);
            }
            lit |= (uint64_t) dc->bundle.lts[i + 1] << 32;
        } else {
            // TODO: check what exception must be raised
            e2k_tr_gen_exception(dc, E2K_EXCP_MAPERR);
        }
        tcg_gen_movi_i64(t, lit);
        return t;
    } else {
        unsigned int i = GET_GLOBAL(src2);
        return e2k_get_greg(dc, i);
    }
}

static TCGv_i64 get_dst(DisasContext *dc, unsigned int als)
{
    unsigned int dst = als & 0xff;
    if (IS_BASED(dst)) {
        unsigned int i = GET_BASED(dst);
        return e2k_get_breg(dc, i);
    } else if (IS_REGULAR(dst)) {
        unsigned int i = GET_REGULAR(dst);
        return e2k_get_wreg(dc, i);
    } else if (IS_GLOBAL(dst)) {
        unsigned int i = GET_GLOBAL(dst);
        return e2k_get_greg(dc, i);
    } else {
        // TODO: %empty, %ctpr, etc
        abort();
    }
    // TODO: remove
    return e2k_get_temp_i64(dc);
}

static void store_reg_alc_result(DisasContext *dc, int chan, TCGv_i64 val)
{
    uint8_t dst = dc->bundle.als[chan];
    Result *res = &dc->alc[chan];
    res->u.reg.v = val;

    if (IS_BASED(dst)) {
        res->tag = RESULT_BASED_REG;
        res->u.reg.i = GET_BASED(dst);
    } else if (IS_REGULAR(dst)) {
        res->tag = RESULT_REGULAR_REG;
        res->u.reg.i = GET_REGULAR(dst);
    } else if (IS_GLOBAL(dst)) {
        res->tag = RESULT_GLOBAL_REG;
        res->u.reg.i = GET_GLOBAL(dst);
    } else if (dst == 0xdf) { /* %empty */
        res->tag = RESULT_NONE;
        res->u.reg.i = -1;
    } else {
        // TODO: %ctpr, etc
        abort();
    }
}

static inline void gen_andn_i32(TCGv_i32 ret, TCGv_i32 x, TCGv_i32 y)
{
    TCGv_i32 t0 = tcg_temp_new_i32();
    tcg_gen_and_i32(t0, x, y);
    tcg_gen_not_i32(ret, t0);
    tcg_temp_free_i32(t0);
}

static inline void gen_andn_i64(TCGv_i64 ret, TCGv_i64 x, TCGv_i64 y)
{
    TCGv_i64 t0 = tcg_temp_new_i64();
    tcg_gen_and_i64(t0, x, y);
    tcg_gen_not_i64(ret, t0);
    tcg_temp_free_i64(t0);
}

static inline void gen_orn_i32(TCGv_i32 ret, TCGv_i32 x, TCGv_i32 y)
{
    TCGv_i32 t0 = tcg_temp_new_i32();
    tcg_gen_or_i32(t0, x, y);
    tcg_gen_not_i32(ret, t0);
    tcg_temp_free_i32(t0);
}

static inline void gen_orn_i64(TCGv_i64 ret, TCGv_i64 x, TCGv_i64 y)
{
    TCGv_i64 t0 = tcg_temp_new_i64();
    tcg_gen_or_i64(t0, x, y);
    tcg_gen_not_i64(ret, t0);
    tcg_temp_free_i64(t0);
}

static inline void gen_xorn_i32(TCGv_i32 ret, TCGv_i32 x, TCGv_i32 y)
{
    TCGv_i32 t0 = tcg_temp_new_i32();
    tcg_gen_xor_i32(t0, x, y);
    tcg_gen_not_i32(ret, t0);
    tcg_temp_free_i32(t0);
}

static inline void gen_xorn_i64(TCGv_i64 ret, TCGv_i64 x, TCGv_i64 y)
{
    TCGv_i64 t0 = tcg_temp_new_i64();
    tcg_gen_xor_i64(t0, x, y);
    tcg_gen_not_i64(ret, t0);
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

static inline void gen_cmpsb(DisasContext *dc, int chan)
{
    uint32_t als = dc->bundle.als[chan];
    TCGv_i64 s1 = get_src1(dc, als);
    TCGv_i64 s2 = get_src2(dc, als);
    TCGv_i64 dst64 = e2k_get_temp_i64(dc);
    TCGv_i32 dst32 = tcg_temp_new_i32();
    TCGv_i32 lo1 = tcg_temp_new_i32();
    TCGv_i32 lo2 = tcg_temp_new_i32();
    TCGCond cond = e2k_gen_cmp_op(GET_FIELD(als, 5, 3));
    Result res = { 0 };

    tcg_gen_extrl_i64_i32(lo1, s1);
    tcg_gen_extrl_i64_i32(lo2, s2);
    tcg_gen_setcond_i32(cond, dst32, lo1, lo2);
    tcg_gen_extu_i32_i64(dst64, dst32);

    res.tag = RESULT_PREG;
    res.u.reg.i = als & 0x1f;
    res.u.reg.v = dst64;

    dc->alc[chan] = res;

    tcg_temp_free_i32(lo2);
    tcg_temp_free_i32(lo1);
    tcg_temp_free_i32(dst32);
}

static inline void gen_cmpdb(DisasContext* dc, int chan)
{
    uint32_t als = dc->bundle.als[chan];
    TCGv_i64 src1 = get_src1(dc, als);
    TCGv_i64 src2 = get_src2(dc, als);
    TCGv_i64 dst = e2k_get_temp_i64(dc);
    TCGCond cond = e2k_gen_cmp_op(GET_FIELD(als, 5, 3));
    Result res = { 0 };

    tcg_gen_setcond_i64(cond, dst, src1, src2);

    res.tag = RESULT_PREG;
    res.u.reg.i = als & 0x1f;
    res.u.reg.v = dst;

    dc->alc[chan] = res;
}

static inline void gen_cmpand_i64(DisasContext *ctx, TCGv_i64 ret, int opc,
    TCGv_i64 src1, TCGv_i64 src2)
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
        e2k_tr_gen_exception(ctx, E2K_EXCP_ILLOPC);
        break;
    }

    tcg_temp_free_i64(t0);
}

static inline void gen_cmpandsb(DisasContext *ctx, int chan)
{
    uint32_t als = ctx->bundle.als[chan];
    int opc = GET_FIELD(als, 5, 3);
    TCGv_i64 src1 = get_src1(ctx, als);
    TCGv_i64 src2 = get_src2(ctx, als);
    TCGv_i64 dst = e2k_get_temp_i64(ctx);
    Result res = { 0 };
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i32 t1 = tcg_temp_new_i32();

    tcg_gen_extrl_i64_i32(t0, src1);
    tcg_gen_extrl_i64_i32(t1, src2);
    tcg_gen_ext_i32_i64(src1, t0);
    tcg_gen_ext_i32_i64(src2, t1);
    gen_cmpand_i64(ctx, dst, opc, src1, src2);

    res.tag = RESULT_PREG;
    res.u.reg.i = als & 0x1f;
    res.u.reg.v = dst;

    ctx->alc[chan] = res;
}

static inline void gen_cmpanddb(DisasContext *ctx, int chan)
{
    uint32_t als = ctx->bundle.als[chan];
    int opc = GET_FIELD(als, 5, 3);
    TCGv_i64 src1 = get_src1(ctx, als);
    TCGv_i64 src2 = get_src2(ctx, als);
    TCGv_i64 dst = e2k_get_temp_i64(ctx);
    Result res = { 0 };

    gen_cmpand_i64(ctx, dst, opc, src1, src2);

    res.tag = RESULT_PREG;
    res.u.reg.i = als & 0x1f;
    res.u.reg.v = dst;

    ctx->alc[chan] = res;
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
    TCGv_i32 one = tcg_const_i32(1);
    tcg_gen_movcond_i32(TCG_COND_EQ, ret, cond, one, x, y);
    tcg_temp_free_i32(one);
}

static inline void gen_merge_i64(TCGv_i64 ret, TCGv_i64 x, TCGv_i64 y, TCGv_i64 cond)
{
    TCGv_i64 one = tcg_const_i64(1);
    tcg_gen_movcond_i64(TCG_COND_EQ, ret, cond, one, x, y);
    tcg_temp_free_i64(one);
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

static inline void gen_mrgc_i64(DisasContext *dc, int chan, TCGv_i64 ret)
{
    uint16_t rlp = find_mrgc(dc, chan);
    TCGv_i64 t0;
    TCGv_i64 t1;
    TCGv_i64 inv;

    if(rlp == 0) {
        /* TODO: exception */
        abort();
    }

    t0 = tcg_temp_new_i64();
    t1 = tcg_temp_new_i64();
    inv = tcg_const_i64(GET_BIT(rlp, 7 + chan % 3));

    e2k_gen_cond_i64(dc, t0, rlp & 0x7f);
    tcg_gen_not_i64(t1, t0);
    tcg_gen_movcond_i64(TCG_COND_EQ, ret, t0, inv, t0, t1);

    tcg_temp_free_i64(inv);
    tcg_temp_free_i64(t1);
    tcg_temp_free_i64(t0);
}

static inline void gen_mrgc_i32(DisasContext *dc, int chan, TCGv_i32 ret)
{
    TCGv_i64 t0 = tcg_temp_new_i64();
    gen_mrgc_i64(dc, chan, t0);
    tcg_gen_extrl_i64_i32(ret, t0);
    tcg_temp_free(t0);
}

static inline void gen_rr_i64(TCGv_i64 ret, uint8_t state_reg)
{
    TCGv_i32 t0 = tcg_const_i32(state_reg);

    gen_helper_state_reg_get(ret, cpu_env, t0);

    tcg_temp_free_i32(t0);
}

static inline void gen_rrd(DisasContext *dc, int chan)
{
    uint32_t als = dc->bundle.als[chan];
    uint8_t state_reg = GET_FIELD(als, 16, 8);
    TCGv_i64 ret = e2k_get_temp_i64(dc);

    gen_rr_i64(ret, state_reg);
    store_reg_alc_result(dc, chan, ret);
}

static inline void gen_rrs(DisasContext *dc, int chan)
{
    uint32_t als = dc->bundle.als[chan];
    uint8_t state_reg = GET_FIELD(als, 16, 8);
    TCGv_i64 t0 = tcg_temp_new_i64();
    TCGv_i64 t1 = e2k_get_temp_i64(dc);

    gen_rr_i64(t0, state_reg);
    gen_movehl_i64(t1, t0, get_dst(dc, als));
    store_reg_alc_result(dc, chan, t1);

    tcg_temp_free_i64(t0);
}

static inline void gen_rw_i64(uint8_t state_reg, TCGv_i64 val)
{
    TCGv_i32 t0 = tcg_const_i32(state_reg);

    gen_helper_state_reg_set(cpu_env, t0, val);

    tcg_temp_free_i32(t0);
}

static inline void gen_rwd(DisasContext *dc, int chan)
{
    uint32_t als = dc->bundle.als[chan];
    TCGv_i64 src2 = get_src2(dc, als);
    gen_rw_i64(als & 0xff, src2);
}

static inline void gen_rws(DisasContext *dc, int chan)
{
    uint32_t als = dc->bundle.als[chan];
    uint8_t state_reg = als & 0xff;
    TCGv_i64 t0 = tcg_temp_new_i64();
    TCGv_i64 t1 = tcg_temp_new_i64();

    gen_rr_i64(t0, state_reg);
    gen_movehl_i64(t1, get_src2(dc, als), t0);
    gen_rw_i64(state_reg, t1);

    tcg_temp_free_i64(t1);
    tcg_temp_free_i64(t0);
}

static void gen_getsp(DisasContext *dc, int chan)
{
    uint32_t als = dc->bundle.als[chan];
    TCGv_i64 src2 = get_src2(dc, als);
    TCGv_i64 t0 = e2k_get_temp_i64(dc);

    gen_helper_getsp(t0, cpu_env, src2);

    store_reg_alc_result(dc, chan, t0);
}

/* FIXME: movtd: don't know what it does */
static void gen_movtd(DisasContext *dc, int chan)
{
    uint32_t als = dc->bundle.als[chan];
    TCGv_i64 src2 = get_src2(dc, als);
    TCGv_i64 t0 = e2k_get_temp_i64(dc);

    tcg_gen_mov_i64(t0, src2);
    store_reg_alc_result(dc, chan, t0);
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

static void gen_ld(DisasContext *dc, int chan, MemOp memop)
{
    uint32_t als = dc->bundle.als[chan];
    TCGv_i64 src1 = get_src1(dc, als);
    TCGv_i64 src2 = get_src2(dc, als);
    TCGv_i64 t0 = tcg_temp_new_i64();
    TCGv_i64 t1 = e2k_get_temp_i64(dc);

    tcg_gen_add_i64(t0, src1, src2);
    memop = gen_mas(dc, chan, memop, t0);
    tcg_gen_qemu_ld_i64(t1, t0, dc->mmuidx, memop);
    store_reg_alc_result(dc, chan, t1);

    tcg_temp_free_i64(t0);
}

static void gen_st(DisasContext *dc, int chan, MemOp memop)
{
    uint32_t als = dc->bundle.als[chan];
    TCGv_i64 src1 = get_src1(dc, als);
    TCGv_i64 src2 = get_src2(dc, als);
    TCGv_i64 src4 = get_dst(dc, als);
    TCGv_i64 t0 = tcg_temp_new_i64();

    tcg_gen_add_i64(t0, src1, src2);
    memop = gen_mas(dc, chan, memop, t0);
    tcg_gen_qemu_st_i64(src4, t0, dc->mmuidx, memop);

    tcg_temp_free_i64(t0);
}

static void gen_op2_i32(TCGv_i64 ret, TCGv_i64 s1, TCGv_i64 s2, TCGv_i64 dst,
    void (*op)(TCGv_i32, TCGv_i32, TCGv_i32))
{
    TCGv_i32 lo1 = tcg_temp_new_i32();
    TCGv_i32 lo2 = tcg_temp_new_i32();
    TCGv_i32 dst_hi = tcg_temp_new_i32();
    TCGv_i32 tmp = tcg_temp_new_i32();

    tcg_gen_extrl_i64_i32(lo1, s1);
    tcg_gen_extrl_i64_i32(lo2, s2);
    tcg_gen_extrh_i64_i32(dst_hi, dst);
    (*op)(tmp, lo1, lo2);
    tcg_gen_concat_i32_i64(ret, tmp, dst_hi);

    tcg_temp_free_i32(tmp);
    tcg_temp_free_i32(dst_hi);
    tcg_temp_free_i32(lo2);
    tcg_temp_free_i32(lo1);
}

static void gen_alopf1_i32(DisasContext *dc, int chan,
    void (*op)(TCGv_i32, TCGv_i32, TCGv_i32))
{
    uint32_t als = dc->bundle.als[chan];
    TCGv_i64 t0 = e2k_get_temp_i64(dc);

    gen_op2_i32(t0, get_src1(dc, als), get_src2(dc, als), get_dst(dc, als), op);
    store_reg_alc_result(dc, chan, t0);
}

static void gen_alopf1_i64(DisasContext *dc, int chan,
    void (*op)(TCGv_i64, TCGv_i64, TCGv_i64))
{
    uint32_t als = dc->bundle.als[chan];
    TCGv_i64 t0 = e2k_get_temp_i64(dc);

    (*op)(t0, get_src1(dc, als), get_src2(dc, als));
    store_reg_alc_result(dc, chan, t0);
}

static void gen_alopf1_mrgc_i32(DisasContext *dc, int chan)
{
    uint32_t als = dc->bundle.als[chan];
    TCGv_i64 src1 = get_src1(dc, als);
    TCGv_i64 src2 = get_src2(dc, als);
    TCGv_i64 dst = get_dst(dc, als);
    TCGv_i64 t0 = tcg_temp_new_i64();
    TCGv_i64 t1 = e2k_get_temp_i64(dc);
    TCGv_i64 cond = tcg_temp_new_i64();

    gen_mrgc_i64(dc, chan, cond);
    gen_merge_i64(t0, src1, src2, cond);
    gen_movehl_i64(t1, t0, dst);
    store_reg_alc_result(dc, chan, t1);

    tcg_temp_free_i64(cond);
    tcg_temp_free_i64(t0);
}

static void gen_alopf1_mrgc_i64(DisasContext *dc, int chan)
{
    uint32_t als = dc->bundle.als[chan];
    TCGv_i64 src1 = get_src1(dc, als);
    TCGv_i64 src2 = get_src2(dc, als);
    TCGv_i64 t0 = e2k_get_temp_i64(dc);
    TCGv_i64 cond = tcg_temp_new_i64();

    gen_mrgc_i64(dc, chan, cond);
    gen_merge_i64(t0, src1, src2, cond);
    store_reg_alc_result(dc, chan, t0);

    tcg_temp_free_i64(cond);
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
        if (chan == 2 || chan == 5) {
            abort();
        } else {
            gen_cmpsb(dc, chan); /* cmp{op}sb */
        }
        break;
    case 0x21:
        if (chan == 2 || chan == 5) {
            abort();
        } else {
            gen_cmpdb(dc, chan); /* cmp{op}db */
        }
        break;
    case 0x22:
        if (chan == 2 || chan == 5) {
            abort();
        } else {
            gen_cmpandsb(dc, chan); /* cmpand{op}sb */
        }
        break;
    case 0x23:
        if (chan == 2 || chan == 5) {
            abort();
        } else {
            gen_cmpanddb(dc, chan); /* cmpand{op}db */
        }
        break;
    case 0x24: { /* stb */
        if (chan == 2 || chan == 5) {
            gen_st(dc, chan, MO_UB);
        } else {
            abort();
        }
        break;
    }
    case 0x25: { /* sth */
        if (chan == 2 || chan == 5) {
            gen_st(dc, chan, MO_UW);
        } else {
            abort();
        }
        break;
    }
    case 0x26: { /* stw */
        if (chan == 2 || chan == 5) {
            gen_st(dc, chan, MO_UL);
        } else {
            abort();
        }
        break;
    }
    case 0x27: { /* std */
        if (chan == 2 || chan == 5) {
            gen_st(dc, chan, MO_Q);
        } else {
            abort();
        }
        break;
    }
    case 0x40:
        if (chan == 5) {
            // FIXME: temp hack
            TCGLabel *l0 = gen_new_label();
            TCGv_i64 src2 = get_src2(dc, als);
            tcg_gen_brcondi_i64(TCG_COND_NE, src2, 0, l0);
            e2k_tr_gen_exception(dc, 0);
            gen_set_label(l0);

            gen_alopf1_i32(dc, chan, tcg_gen_divu_i32);
        } else {
            abort();
        }
        break;
    case 0x41:
        if (chan == 5) {
            gen_alopf1_i64(dc, chan, tcg_gen_divu_i64);
        } else {
            abort();
        }
        break;
    case 0x42:
        if (chan == 5) {
            gen_alopf1_i32(dc, chan, tcg_gen_div_i32);
        } else {
            abort();
        }
        break;
    case 0x43:
        if (chan == 5) {
            gen_alopf1_i64(dc, chan, tcg_gen_div_i64);
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
        if (chan == 1 || chan == 4) {
            abort();
        } else {
            gen_ld(dc, chan, MO_UB);
        }
        break;
    }
    case 0x65: { /* ldh */
        if (chan == 1 || chan == 4) {
            abort();
        } else {
            gen_ld(dc, chan, MO_UW);
        }
        break;
    }
    case 0x66: { /* ldw */
        if (chan == 1 || chan == 4) {
            abort();
        } else {
            gen_ld(dc, chan, MO_UL);
        }
        break;
    }
    case 0x67: { /* ldd */
        if (chan == 1 || chan == 4) {
            abort();
        } else {
            gen_ld(dc, chan, MO_Q);
        }
        break;
    }
    default:
        e2k_tr_gen_exception(dc, E2K_EXCP_ILLOPC);
        break;
    }
}

static void execute_ext1(DisasContext *dc, int chan)
{
    uint8_t opc = GET_FIELD(dc->bundle.als[chan], 24, 7);

    switch (opc) {
    case 0x58: {
        if (chan == 0 || chan == 3) {
            gen_getsp(dc, chan);
        }
        /* TODO: exception */
        break;
    }
    case 0x3c: /* rws */ gen_rws(dc, chan); break; /* TODO: only channel 1 */
    case 0x3d: /* rwd */ gen_rwd(dc, chan); break; /* TODO: only channel 1 */
    case 0x3e: /* rrs */ gen_rrs(dc, chan); break; /* TODO: only channel 1 */
    case 0x3f: /* rrd */ gen_rrd(dc, chan); break; /* TODO: only channel 1 */
    default:
        /* TODO: exception */
        break;
    }
}

void e2k_execute_alc(DisasContext *ctx, int chan)
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
        e2k_tr_gen_exception(ctx, E2K_EXCP_ILLOPC);
        break;
    case ALES_PRESENT: {
        uint8_t opc = GET_FIELD(bundle->ales[chan], 8, 8);
        switch (opc) {
        case 0x01:
            execute_ext1(ctx, chan);
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

    ctx->alc[chan].has_cond = rlp != 0;
    ctx->alc[chan].cond = cond;
}
