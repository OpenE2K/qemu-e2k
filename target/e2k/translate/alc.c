#include "qemu/osdep.h"
#include "qemu.h"
#include "exec/log.h"
#include "translate.h"

static TCGv_i64 get_src1(DisasContext *dc, uint32_t als)
{
    unsigned int src1 = GET_FIELD(als, 16, 27);
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
    unsigned int src2 = GET_FIELD(als, 8, 15);
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
            lit = ((int64_t) lit & 0xffff) << 48 >> 48;
        } else if (IS_LIT16_HI(src2) && i < 2) {
            lit = ((int64_t) lit >> 16) << 48 >> 48;
        } else if (IS_LIT32(src2)) {
            // nop
        } else if (IS_LIT64(src2) && i < 3) {
            // TODO: exception
            assert(dc->bundle.lts_present[i + 1]);
            lit |= ((uint64_t) dc->bundle.lts[i + 1]) << 32;
        } else {
            // TODO: exception
            abort();
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
}

/* FIXME: now only %r, %b, %g */
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
    } else {
        /* TODO: exception */
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
    int alc_mask = GET_FIELD(rlp, 10, 12);
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

static inline void gen_cond_psrc(TCGv_i64 ret, uint8_t psrc)
{
    if ((psrc & 0xe0) == 0x60) {
        e2k_gen_preg(ret, psrc & 0x1f);
    } else {
        /* TODO: lcntex, spred, pcnt */
        abort();
    }
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

    gen_cond_psrc(t0, rlp & 0x7f);
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
    switch (state_reg) {
    case 0x2c: /* %usd.hi */
        tcg_gen_mov_i64(ret, e2k_cs.usd_hi);
        break;
    case 0x2d: /* %usd.lo */
        tcg_gen_mov_i64(ret, e2k_cs.usd_lo);
        break;
    case 0x83: /* %lsr */
        tcg_gen_mov_i64(ret, e2k_cs.lsr);
        break;
    default:
        /* TODO: exception */
        abort();
        break;
    }
}

static inline void gen_rrd(DisasContext *dc, int chan)
{
    uint32_t als = dc->bundle.als[chan];
    uint8_t state_reg = GET_FIELD(als, 16, 23);
    TCGv_i64 ret = e2k_get_temp_i64(dc);

    gen_rr_i64(ret, state_reg);
    store_reg_alc_result(dc, chan, ret);
}

static inline void gen_rrs(DisasContext *dc, int chan)
{
    uint32_t als = dc->bundle.als[chan];
    uint8_t state_reg = GET_FIELD(als, 16, 23);
    TCGv_i64 t0 = tcg_temp_new_i64();
    TCGv_i64 t1 = e2k_get_temp_i64(dc);

    gen_rr_i64(t0, state_reg);
    gen_movehl_i64(t1, t0, get_dst(dc, als));
    store_reg_alc_result(dc, chan, t1);

    tcg_temp_free_i64(t0);
}

static inline void gen_rw_i64(uint8_t state_reg, TCGv_i64 val)
{
    switch (state_reg) {
    case 0x2c: /* %usd.hi */
        /* FIXME: user cannot write */
        tcg_gen_mov_i64(e2k_cs.usd_hi, val);
        break;
    case 0x2d: /* %usd.lo */
        /* FIXME: user cannot write */
        tcg_gen_mov_i64(e2k_cs.usd_lo, val);
        break;
    case 0x83: /* %lsr */
        tcg_gen_mov_i64(e2k_cs.lsr, val);
        break;
    default:
        /* TODO: exception */
        abort();
        break;
    }
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
    TCGv_i64 t0 = tcg_temp_new_i64();
    TCGv_i64 t1 = tcg_temp_new_i64();

    /* TODO: exception stack overflow */
    tcg_gen_extract_i64(t0, e2k_cs.usd_lo, 0, 48);
    tcg_gen_add_i64(t1, t0, src2);
    tcg_gen_deposit_i64(e2k_cs.usd_lo, e2k_cs.usd_lo, t1, 0, 48);

    tcg_temp_free_i64(t1);
    tcg_temp_free_i64(t0);
}

static void gen_ld(DisasContext *dc, int chan, MemOp memop)
{
    uint32_t als = dc->bundle.als[chan];
    TCGv_i64 src1 = get_src1(dc, als);
    TCGv_i64 src2 = get_src2(dc, als);
    TCGv_i64 t0 = tcg_temp_new_i64();
    TCGv_i64 t1 = e2k_get_temp_i64(dc);

    tcg_gen_add_i64(t0, src1, src2);
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
    TCGv_i64 t0 = tcg_temp_new_i64();
    TCGv_i64 t1 = e2k_get_temp_i64(dc);
    TCGv_i64 cond = tcg_temp_new_i64();

    gen_mrgc_i64(dc, chan, cond);
    gen_merge_i64(t0, get_src1(dc, als), get_src2(dc, als), cond);
    gen_movehl_i64(t1, t0, get_dst(dc, als));
    store_reg_alc_result(dc, chan, t1);

    tcg_temp_free_i64(cond);
    tcg_temp_free_i64(t0);
}

static void gen_alopf1_mrgc_i64(DisasContext *dc, int chan)
{
    uint32_t als = dc->bundle.als[chan];
    TCGv_i64 t0 = e2k_get_temp_i64(dc);
    TCGv_i64 cond = tcg_temp_new_i64();

    gen_mrgc_i64(dc, chan, cond);
    gen_merge_i64(t0, get_src1(dc, als), get_src2(dc, als), cond);
    store_reg_alc_result(dc, chan, t0);

    tcg_temp_free_i64(cond);
}

static void gen_alopf_simple(DisasContext *dc, int chan)
{
    uint32_t als = dc->bundle.als[chan];
    int opc = GET_FIELD(als, 24, 30);
    int sm  = GET_BIT(als, 31);
    Result res = { 0 };

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
    case 0x1e: /* TODO: getfs */ abort(); break;
    case 0x1f: /* TODO: getfd */ abort(); break;
    case 0x21: { // cmp{op}sd
        TCGv_i64 cpu_src1 = get_src1(dc, als);
        TCGv_i64 cpu_src2 = get_src2(dc, als);
        TCGv_i64 tmp_dst = e2k_get_temp_i64(dc);

        unsigned int cmp_op = GET_FIELD(als, 5, 7);
        // TODO: move to separate function
        switch(cmp_op) {
        case 1: // unsigned less
            tcg_gen_setcond_i64(TCG_COND_LEU, tmp_dst, cpu_src1, cpu_src2);
            break;
        case 2: // equal
            tcg_gen_setcond_i64(TCG_COND_EQ, tmp_dst, cpu_src1, cpu_src2);
            break;
        default:
            abort();
            break;
        }

        res.tag = RESULT_PREG;
        res.u.reg.i = als & 0x1f;
        res.u.reg.v = tmp_dst;
        dc->alc[chan] = res;

        break;
    }
    case 0x24: { /* stb */
        if (chan == 2 || chan == 5) {
            gen_st(dc, chan, MO_UB);
        } else {
            /* TODO: exception */
            abort();
        }
        break;
    }
    case 0x25: { /* sth */
        if (chan == 2 || chan == 5) {
            gen_st(dc, chan, MO_TEUW);
        } else {
            /* TODO: exception */
            abort();
        }
        break;
    }
    case 0x26: { /* stw */
        if (chan == 2 || chan == 5) {
            gen_st(dc, chan, MO_TEUL);
        } else {
            /* TODO: exception */
            abort();
        }
        break;
    }
    case 0x27: { /* std */
        if (chan == 2 || chan == 5) {
            gen_st(dc, chan, MO_TEQ);
        } else {
            /* TODO: exception */
            abort();
        }
        break;
    }
    case 0x40: // TODO: udivs used as temporary UD
        e2k_gen_exception(dc, E2K_EXCP_UNIMPL);
        break;
    case 0x64: { /* ldb */
        if (chan == 2 || chan == 5) {
            /* TODO: exception */
            abort();
        } else {
            gen_ld(dc, chan, MO_UB);
        }
        break;
    }
    case 0x65: { /* ldh */
        if (chan == 2 || chan == 5) {
            /* TODO: exception */
            abort();
        } else {
            gen_ld(dc, chan, MO_TEUW);
        }
        break;
    }
    case 0x66: { /* ldw */
        if (chan == 2 || chan == 5) {
            /* TODO: exception */
            abort();
        } else {
            gen_ld(dc, chan, MO_TEUL);
        }
        break;
    }
    case 0x67: { /* ldd */
        if (chan == 2 || chan == 5) {
            /* TODO: exception */
            abort();
        } else {
            gen_ld(dc, chan, MO_TEQ);
        }
        break;
    }
    default:
        qemu_log_mask(LOG_UNIMP, "gen_alc: undefined instruction 0x%x %s\n", opc, sm ? "(speculative)" : "");
        break;
    }
}

static void gen_ext1(DisasContext *dc, int chan)
{
    uint8_t opc = GET_FIELD(dc->bundle.als[chan], 24, 30);

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

static void gen_channel(DisasContext *dc, int chan)
{
    const UnpackedBundle *bundle = &dc->bundle;

    switch(bundle->ales_present[chan]) {
    case ALES_NONE:
        gen_alopf_simple(dc, chan);
        break;
    case ALES_ALLOCATED: // 2 or 5 channel
        abort();
        break;
    case ALES_PRESENT: {
        uint8_t opc = GET_FIELD(bundle->ales[chan], 8, 15);
        switch (opc) {
        case 0x01:
            gen_ext1(dc, chan);
            break;
        default:
            /* TODO */
            abort();
            break;
        }
        break;
    }
    default:
        g_assert_not_reached();
        break;
    }
}

void e2k_alc_gen(DisasContext *dc)
{
    unsigned int i;

    for (i = 0; i < 6; i++) {
        if (!dc->bundle.als_present[i]) {
            continue;
        }
        dc->alc[i].tag = RESULT_NONE;
        gen_channel(dc, i);
    }
}

void e2k_alc_commit(DisasContext *dc)
{
    unsigned int i;

    for (i = 0; i < 6; i++) {
        Result *res = &dc->alc[i];
        if (!dc->bundle.als_present[i]) {
            continue;
        }
        switch(res->tag) {
        case RESULT_BASED_REG:
            e2k_gen_store_breg(res->u.reg.i, res->u.reg.v);
            break;
        case RESULT_REGULAR_REG:
            e2k_gen_store_wreg(res->u.reg.i, res->u.reg.v);
            break;
        case RESULT_GLOBAL_REG:
            e2k_gen_store_greg(res->u.reg.i, res->u.reg.v);
            break;
        case RESULT_PREG:
            e2k_gen_store_preg(res->u.reg.i, res->u.reg.v);
            break;
        default:
            break;
        }
    }
}
