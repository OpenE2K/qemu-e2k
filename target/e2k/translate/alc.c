#include "qemu/osdep.h"
#include "qemu.h"
#include "tcg/tcg-op.h"
#include "exec/log.h"
#include "exec/translator.h"
#include "translate.h"

static TCGv_i64 get_src1(DisasContext *dc, unsigned int als)
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

static TCGv_i64 get_src2(DisasContext *dc, unsigned int als)
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
            lit &= 0xffff;
        } else if (IS_LIT16_HI(src2) && i < 2) {
            lit >>= 16;
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

static void gen_op21_i32(TCGv_i64 ret, TCGv_i64 s1, TCGv_i64 s2, TCGv_i64 dst,
    void (*op1)(TCGv_i32, TCGv_i32, TCGv_i32),
    void (*op2)(TCGv_i32, TCGv_i32))
{
    TCGv_i32 lo1 = tcg_temp_new_i32();
    TCGv_i32 lo2 = tcg_temp_new_i32();
    TCGv_i32 dst_hi = tcg_temp_new_i32();
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i32 t1 = tcg_temp_new_i32();

    tcg_gen_extrl_i64_i32(lo1, s1);
    tcg_gen_extrl_i64_i32(lo2, s2);
    tcg_gen_extrh_i64_i32(dst_hi, dst);
    (*op1)(t0, lo1, lo2);
    (*op2)(t1, t0);
    tcg_gen_concat_i32_i64(ret, t1, dst_hi);

    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
    tcg_temp_free_i32(dst_hi);
    tcg_temp_free_i32(lo2);
    tcg_temp_free_i32(lo1);
}

static void gen_channel(DisasContext *dc, int chan)
{
    const UnpackedBundle *bundle = &dc->bundle;
    unsigned int als = bundle->als[chan];
    int opc = GET_FIELD(als, 24, 30);
    int sm  = GET_BIT(als, 31);
    unsigned int dst = als & 0xff;
    bool is_cmp = false;
    Result res = { 0 };

    TCGv_i64 cpu_src1 = get_src1(dc, als);
    TCGv_i64 cpu_src2 = get_src2(dc, als);
    TCGv_i64 tmp_dst = e2k_get_temp_i64(dc);
    TCGv_i64 t64 = e2k_get_temp_i64(dc);

    switch(opc) {
    case 0x00: // ands
        gen_op2_i32(tmp_dst, cpu_src1, cpu_src2, get_dst(dc, als), tcg_gen_and_i32);
        break;
    case 0x01: // andd
        tcg_gen_and_i64(tmp_dst, cpu_src1, cpu_src2);
        break;
    case 0x02: // andns
        gen_op21_i32(tmp_dst, cpu_src1, cpu_src2, get_dst(dc, als), tcg_gen_add_i32, tcg_gen_not_i32);
        break;
    case 0x03: // andnd
        tcg_gen_and_i64(t64, cpu_src1, cpu_src2);
        tcg_gen_not_i64(tmp_dst, t64);
        break;
    case 0x04: // ors
        gen_op2_i32(tmp_dst, cpu_src1, cpu_src2, get_dst(dc, als), tcg_gen_or_i32);
        break;
    case 0x05: // ord
        tcg_gen_or_i64(tmp_dst, cpu_src1, cpu_src2);
        break;
    case 0x06: // orns
        gen_op21_i32(tmp_dst, cpu_src1, cpu_src2, get_dst(dc, als), tcg_gen_or_i32, tcg_gen_not_i32);
        break;
    case 0x07: // ornd
        tcg_gen_or_i64(t64, cpu_src1, cpu_src2);
        tcg_gen_not_i64(tmp_dst, t64);
        break;
    case 0x08: // xors
        gen_op2_i32(tmp_dst, cpu_src1, cpu_src2, get_dst(dc, als), tcg_gen_xor_i32);
        break;
    case 0x09: // xord
        tcg_gen_xor_i64(tmp_dst, cpu_src1, cpu_src2);
        break;
    case 0x0a: // xorns
        gen_op21_i32(tmp_dst, cpu_src1, cpu_src2, get_dst(dc, als), tcg_gen_xor_i32, tcg_gen_not_i32);
        break;
    case 0x0b: // xornd
        tcg_gen_xor_i64(t64, cpu_src1, cpu_src2);
        tcg_gen_not_i64(tmp_dst, t64);
        break;
    case 0x0c:
        // TODO: sxt
        abort();
        break;
    case 0x0e:
        // TODO: merges
        abort();
        break;
    case 0x0f:
        // TODO: merged
        abort();
        break;
    case 0x10: // adds
        gen_op2_i32(tmp_dst, cpu_src1, cpu_src2, get_dst(dc, als), tcg_gen_add_i32);
        break;
    case 0x11: // addd
        tcg_gen_add_i64(tmp_dst, cpu_src1, cpu_src2);
        break;
    case 0x12: // subs
        gen_op2_i32(tmp_dst, cpu_src1, cpu_src2, get_dst(dc, als), tcg_gen_sub_i32);
        break;
    case 0x13: // subd
        tcg_gen_sub_i64(tmp_dst, cpu_src1, cpu_src2);
        break;
    case 0x14: // scls
        gen_op2_i32(tmp_dst, cpu_src1, cpu_src2, get_dst(dc, als), tcg_gen_rotl_i32);
        break;
    case 0x15: // scld
        tcg_gen_rotl_i64(tmp_dst, cpu_src1, cpu_src2);
        break;
    case 0x16: // scrs
        gen_op2_i32(tmp_dst, cpu_src1, cpu_src2, get_dst(dc, als), tcg_gen_rotr_i32);
        break;
    case 0x17: // scrd
        tcg_gen_rotr_i64(tmp_dst, cpu_src1, cpu_src2);
        break;
    case 0x18: // shls
        gen_op2_i32(tmp_dst, cpu_src1, cpu_src2, get_dst(dc, als), tcg_gen_shl_i32);
        break;
    case 0x19: // shld
        tcg_gen_shl_i64(tmp_dst, cpu_src1, cpu_src2);
        break;
    case 0x1a: // shrs
        gen_op2_i32(tmp_dst, cpu_src1, cpu_src2, get_dst(dc, als), tcg_gen_shr_i32);
        break;
    case 0x1b: // shrd
        tcg_gen_shr_i64(tmp_dst, cpu_src1, cpu_src2);
        break;
    case 0x1c:
        gen_op2_i32(tmp_dst, cpu_src1, cpu_src2, get_dst(dc, als), tcg_gen_sar_i32);
        break;
    case 0x1d: // sard
        tcg_gen_sar_i64(tmp_dst, cpu_src1, cpu_src2);
        break;
    case 0x1e:
        // TODO: getfs
        abort();
        break;
    case 0x1f:
        // TODO: getfd
        abort();
        break;
    case 0x21: { // cmp{op}sd
        is_cmp = true;
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
        break;
    }
    case 0x40: // TODO: udivs used as temporary UD
        e2k_gen_exception(dc, 1);
        break;
    default:
        qemu_log_mask(LOG_UNIMP, "gen_alc: undefined instruction 0x%x %s\n", opc, sm ? "(speculative)" : "");
        break;
    }

    if (is_cmp) {
        res.tag = RESULT_PREG;
        res.u.reg.i = als & 0x1f;
        res.u.reg.v = tmp_dst;
    } else {
        if (IS_BASED(dst)) {
            unsigned int i = GET_BASED(dst);
            res.tag = RESULT_BASED_REG;
            res.u.reg.i = i;
            res.u.reg.v = tmp_dst;
        } else if (IS_REGULAR(dst)) {
            unsigned int i = GET_REGULAR(dst);
            res.tag = RESULT_REGULAR_REG;
            res.u.reg.i = i;
            res.u.reg.v = tmp_dst;
        } else if (IS_GLOBAL(dst)) {
            unsigned int i = GET_GLOBAL(dst);
            res.tag = RESULT_GLOBAL_REG;
            res.u.reg.i = i;
            res.u.reg.v = tmp_dst;
        } else {
            abort();
        }
    }

    dc->alc[chan] = res;
}

void e2k_alc_gen(DisasContext *dc)
{
    unsigned int i;

    for (i = 0; i < 6; i++) {
        if (!dc->bundle.als_present[i]) {
            continue;
        }
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
