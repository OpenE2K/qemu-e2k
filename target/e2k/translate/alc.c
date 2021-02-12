#include "qemu/osdep.h"
#include "qemu.h"
#include "exec/log.h"
#include "translate.h"
#include "tcg/tcg-op-gvec.h"

#include "alops.inc"

#define glue3(a, b, c) glue(glue(a, b), c)
#define glue4(a, b, c, d) glue(glue(a, b), glue(c, d))

static int16_t alops_map[4][128][6];

typedef struct {
    TCGv_i32 tag;
    TCGv_i64 lo;
    TCGv_i32 hi;
} Src80;

typedef struct {
    TCGv_i32 tag;
    TCGv_i64 value;
} Src64;

typedef struct {
    TCGv_i32 tag;
    TCGv_i32 value;
} Src32;

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

static inline TCGv_i64 get_temp_i64(Instr *instr)
{
    return e2k_get_temp_i64(instr->ctx);
}

static inline TCGv_i32 get_temp_i32(Instr *instr)
{
    return e2k_get_temp_i32(instr->ctx);
}

static inline Src80 temp_new_src80(void)
{
    Src80 t = { 0 };

    t.tag = tcg_temp_new_i32();
    t.lo = tcg_temp_new_i64();
    t.hi = tcg_temp_new_i32();

    return t;
}

static inline void temp_free_src80(Src80 *t)
{
    tcg_temp_free_i32(t->tag);
    tcg_temp_free_i64(t->lo);
    tcg_temp_free_i32(t->hi);
}

static inline Src80 get_temp_src80(Instr *instr)
{
    Src80 t = { 0 };

    t.tag = get_temp_i32(instr);
    t.lo = get_temp_i64(instr);
    t.hi = get_temp_i32(instr);

    return t;
}

static inline void gen_reg_i80(DisasContext *ctx, Src80 *ret, uint8_t arg)
{
    TCGv_i32 t0 = tcg_temp_new_i32();

    e2k_gen_reg_index(ctx, t0, arg);
    ret->tag = e2k_get_temp_i32(ctx);
    ret->lo = e2k_get_temp_i64(ctx);
    ret->hi = e2k_get_temp_i32(ctx);
    e2k_gen_reg_tag_read_i64(ret->tag, t0);
    e2k_gen_reg_read_i64(ret->lo, t0);
    e2k_gen_xreg_read16u_i32(ret->hi, t0);
    tcg_temp_free_i32(t0);
}

static inline void gen_reg_i64(DisasContext *ctx, Src64 *ret, uint8_t arg)
{
    TCGv_i32 t0 = tcg_temp_new_i32();

    e2k_gen_reg_index(ctx, t0, arg);
    ret->tag = e2k_get_temp_i32(ctx);
    ret->value = e2k_get_temp_i64(ctx);
    e2k_gen_reg_tag_read_i64(ret->tag, t0);
    e2k_gen_reg_read_i64(ret->value, t0);

    tcg_temp_free_i32(t0);
}

static inline void gen_reg_i32(DisasContext *ctx, Src32 *ret, uint8_t arg)
{
    TCGv_i32 t0 = tcg_temp_new_i32();

    e2k_gen_reg_index(ctx, t0, arg);
    ret->tag = e2k_get_temp_i32(ctx);
    ret->value = e2k_get_temp_i32(ctx);
    e2k_gen_reg_tag_read_i32(ret->tag, t0);
    e2k_gen_reg_read_i32(ret->value, t0);

    tcg_temp_free_i32(t0);
}

static inline void gen_temp_reg_write_i64_i32(TCGv_i64 lo, TCGv_i32 hi,
    TCGv_ptr ptr)
{
    tcg_gen_st_i64(lo, ptr, offsetof(E2KReg, f80.low));
    tcg_gen_st16_i32(hi, ptr, offsetof(E2KReg, f80.high));
}

static inline void gen_temp_reg_read_i64_i32(TCGv_ptr ptr, TCGv_i64 ret_lo,
    TCGv_i32 ret_hi)
{
    tcg_gen_ld_i64(ret_lo, ptr, offsetof(E2KReg, f80.low));
    tcg_gen_ld16u_i32(ret_hi, ptr, offsetof(E2KReg, f80.high));
}

static inline void gen_literal_i64(DisasContext *ctx, Src64 *ret, uint8_t arg)
{
    int i = GET_LIT(arg);
    uint64_t lit = ctx->bundle.lts[i];

    if (!ctx->bundle.lts_present[i]) {
        e2k_tr_gen_exception(ctx, E2K_EXCP_ILLOPN);
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

    ret->tag = e2k_get_const_i32(ctx, 0);
    ret->value = e2k_get_const_i64(ctx, lit);
}

static inline void gen_literal_i32(DisasContext *ctx, Src32 *ret, uint8_t arg)
{
    int i = GET_LIT(arg);
    int32_t lit = ctx->bundle.lts[i];

    if (!ctx->bundle.lts_present[i]) {
        e2k_tr_gen_exception(ctx, E2K_EXCP_ILLOPN);
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

    ret->tag = e2k_get_const_i32(ctx, 0);
    ret->value = e2k_get_const_i32(ctx, lit);
}

static inline Src80 get_src1_i80(Instr *instr)
{
    DisasContext *ctx = instr->ctx;
    uint8_t src1 = instr->src1;
    Src80 ret = { 0 };

    if (IS_IMM5(src1)) {
        ret.tag = e2k_get_const_i32(ctx, 0);
        ret.lo = e2k_get_const_i64(ctx, GET_IMM5(src1));
        ret.hi = e2k_get_const_i32(ctx, 0);
    } else {
        gen_reg_i80(ctx, &ret, src1);
    }

    return ret;
}

static inline Src64 get_src1_i64(Instr *instr)
{
    DisasContext *ctx = instr->ctx;
    uint8_t src1 = instr->src1;
    Src64 ret = { 0 };

    if (IS_IMM5(src1)) {
        ret.tag = e2k_get_const_i32(ctx, 0);
        ret.value = e2k_get_const_i64(ctx, GET_IMM5(src1));
    } else {
        gen_reg_i64(ctx, &ret, src1);
    }

    return ret;
}

static inline Src32 get_src1_i32(Instr *instr)
{
    DisasContext *ctx = instr->ctx;
    uint8_t src1 = instr->src1;
    Src32 ret = { 0 };

    if (IS_IMM5(src1)) {
        ret.tag = e2k_get_const_i32(ctx, 0);
        ret.value = e2k_get_const_i32(ctx, GET_IMM5(src1));
    } else {
        gen_reg_i32(ctx, &ret, src1);
    }

    return ret;
}

static inline Src80 get_src2_i80(Instr *instr)
{
    DisasContext *ctx = instr->ctx;
    uint8_t src2 = instr->src2;
    Src80 ret = { 0 };

    if (IS_IMM4(src2)) {
        ret.tag = e2k_get_const_i32(ctx, 0);
        ret.lo = e2k_get_const_i64(ctx, GET_IMM4(src2));
        ret.hi = e2k_get_const_i32(ctx, 0);
    } else if (IS_LIT(src2)) {
        Src64 t = { 0 };
        gen_literal_i64(ctx, &t, src2);
        ret.tag = t.tag;
        ret.lo = t.value;
        ret.hi = e2k_get_const_i32(ctx, 0);
    } else {
        gen_reg_i80(ctx, &ret, src2);
    }

    return ret;
}

static inline Src64 get_src2_i64(Instr *instr)
{
    DisasContext *ctx = instr->ctx;
    uint8_t src2 = instr->src2;
    Src64 ret = { 0 };

    if (IS_IMM4(src2)) {
        ret.tag = e2k_get_const_i32(ctx, 0);
        ret.value = e2k_get_const_i64(ctx, GET_IMM4(src2));
    } else if (IS_LIT(src2)) {
        gen_literal_i64(ctx, &ret, src2);
    } else {
        gen_reg_i64(ctx, &ret, src2);
    }

    return ret;
}

static inline Src32 get_src2_i32(Instr *instr)
{
    DisasContext *ctx = instr->ctx;
    uint8_t src2 = instr->src2;
    Src32 ret = { 0 };

    if (IS_IMM4(src2)) {
        ret.tag = e2k_get_const_i32(ctx, 0);
        ret.value = e2k_get_const_i32(ctx, GET_IMM4(src2));
    } else if (IS_LIT(src2)) {
        gen_literal_i32(ctx, &ret, src2);
    } else {
        gen_reg_i32(ctx, &ret, src2);
    }

    return ret;
}

static inline Src64 get_src3_i64(Instr *instr)
{
    Src64 ret = { 0 };
    gen_reg_i64(instr->ctx, &ret, instr->src3);
    return ret;
}

static inline Src32 get_src3_i32(Instr *instr)
{
    Src32 ret = { 0 };
    gen_reg_i32(instr->ctx, &ret, instr->src3);
    return ret;
}

static inline Src64 get_src4_i64(Instr *instr)
{
    Src64 ret = { 0 };
    gen_reg_i64(instr->ctx, &ret, instr->src4);
    return ret;
}

static inline Src32 get_src4_i32(Instr *instr)
{
    Src32 ret = { 0 };
    gen_reg_i32(instr->ctx, &ret, instr->src4);
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

static inline void set_al_result_reg80_tag(Instr *instr, TCGv_i64 lo,
    TCGv_i32 hi, TCGv_i32 tag, bool poison)
{
    uint8_t dst = instr->dst;
    AlResult *res = &instr->ctx->al_results[instr->chan];

    res->poison = poison;
    if (dst == 0xdf) {
        res->type = AL_RESULT_NONE;
    } else {
        res->type = AL_RESULT_REG80;
        res->reg.tag = tag;
        res->reg.v64 = lo;
        res->reg.x32 = hi;
        res->reg.index = get_temp_i32(instr);
        e2k_gen_reg_index(instr->ctx, res->reg.index, dst);
    }
}

static inline void set_al_result_reg64_tag(Instr *instr,
    TCGv_i64 value, TCGv_i32 tag, bool poison)
{
    uint8_t arg = instr->dst;
    AlResult *res = &instr->ctx->al_results[instr->chan];

    res->poison = poison;
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
        e2k_gen_reg_index(instr->ctx, res->reg.index, arg);
    }
}

static inline void set_al_result_reg64(Instr *instr, TCGv_i64 value)
{
    TCGv_i32 tag = e2k_get_const_i32(instr->ctx, 0);
    set_al_result_reg64_tag(instr, value, tag, true);
}

static inline void set_al_result_reg32_tag(Instr *instr,
    TCGv_i32 value, TCGv_i32 tag, bool poison, bool check_tag, bool dbl)
{
    uint8_t arg = instr->dst;
    AlResult *res = &instr->ctx->al_results[instr->chan];

    res->check_tag = check_tag;
    res->dbl = dbl;
    res->poison = poison;
    // TODO: %tst, %tc, %tcd
    if (arg == 0xdf) { /* %empty */
        res->type = AL_RESULT_NONE;
        res->reg.index = NULL;
        res->reg.v32 = NULL;
        res->reg.tag = NULL;
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
        e2k_gen_reg_index(instr->ctx, res->reg.index, arg);
    }
}

static inline void set_al_result_reg32(Instr *instr, TCGv_i32 value)
{
    TCGv_i32 tag = e2k_get_const_i32(instr->ctx, 0);
    set_al_result_reg32_tag(instr, value, tag, true, true, true);
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
    gen_tag_check(instr, tag);
    set_al_result_reg80_tag(instr, lo, hi, tag, true);
}

static inline void gen_al_result_i64(Instr *instr, TCGv_i64 dst, TCGv_i32 tag)
{
    gen_tag_check(instr, tag);
    set_al_result_reg64_tag(instr, dst, tag, true);
}

static inline void gen_al_result_i32(Instr *instr, TCGv_i32 dst, TCGv_i32 tag)
{
    gen_tag_check(instr, tag);
    set_al_result_reg32_tag(instr, dst, tag, true, true, true);
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
        TCGv_i32 t0 = tcg_temp_new_i32();
        tcg_gen_brcondi_i32(TCG_COND_NE, e2k_cs.lsr_pcnt, 0, l);
        e2k_gen_is_loop_end_i32(t0);
        tcg_gen_brcondi_i32(TCG_COND_EQ, t0, 0, l0);
        tcg_gen_brcondi_i32(TCG_COND_EQ, e2k_cs.lsr_strmd, 0, l);
        tcg_gen_subi_i32(e2k_cs.lsr_strmd, e2k_cs.lsr_strmd, 1);
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
    static inline void glue(gen_maski_, S)(T ret, int size) \
    { \
        T t0 = glue(tcg_const_, S)(size); \
        glue(gen_mask_, S)(ret, t0); \
        glue(tcg_temp_free_, S)(t0); \
    }

IMPL_GEN_MASK(i64, TCGv_i64, 64)
IMPL_GEN_MASK(i32, TCGv_i32, 32)

#define IMPL_GEN_GETF_SIGN(S, T) \
    static inline void glue(gen_getf_sign_, S)(T ret, T val, T size, \
        T off, T sign_off) \
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
       T t8 = glue(tcg_temp_new_, S)(); \
       /* sign = (src1 >> ((sign_offset << 3) + ((size - 1) & 7) - (offset & 7))) & 1 */ \
       glue(tcg_gen_subi_, S)(t0, size, 1); \
       glue(tcg_gen_andi_, S)(t1, t0, 7); \
       glue(tcg_gen_andi_, S)(t2, off, 7); \
       glue(tcg_gen_shli_, S)(t3, sign_off, 3); \
       glue(tcg_gen_add_, S)(t4, t3, t1); \
       glue(tcg_gen_sub_, S)(t5, t4, t2); \
       glue(tcg_gen_shr_, S)(t6, val, t5); \
       glue(tcg_gen_andi_, S)(t7, t6, 1); \
       glue(tcg_gen_shl_, S)(t8, ones, size); \
       glue(tcg_gen_movcond_, S)(TCG_COND_NE, ret, t7, z, t8, z); \
       glue(tcg_temp_free_, S)(t8); \
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

#define IMPL_GEN_GETF(NAME, S, T, OFFSET, SIZE, SIGN_OFF, N) \
    static inline void NAME(T ret, T src1, T src2) \
    { \
        T z = glue(tcg_const_, S)(0); \
        T offset = glue(tcg_temp_new_, S)(); \
        T size = glue(tcg_temp_new_, S)(); \
        T sign = glue(tcg_temp_new_, S)(); \
        T sign_off = glue(tcg_temp_new_, S)(); \
        T t0 = glue(tcg_temp_new_, S)(); \
        T t1 = glue(tcg_temp_new_, S)(); \
        T t2 = glue(tcg_temp_new_, S)(); \
        T t3 = glue(tcg_temp_new_, S)(); \
        T t4 = glue(tcg_temp_new_, S)(); \
        glue(tcg_gen_extract_, S)(offset, src2, 0, OFFSET); \
        glue(tcg_gen_extract_, S)(size, src2, 6, SIZE); \
        glue(tcg_gen_extract_, S)(sign, src2, 12, 1); \
        glue(tcg_gen_extract_, S)(sign_off, src2, 13, SIGN_OFF); \
        glue(tcg_gen_rotr_, S)(t0, src1, offset); \
        glue(gen_mask_, S)(t1, size); \
        glue(tcg_gen_and_, S)(t2, t0, t1); \
        glue(gen_getf_sign_, S)(t3, src1, size, offset, sign_off); \
        glue(tcg_gen_or_, S)(t4, t3, t2); \
        glue(tcg_gen_movcond_, S)(TCG_COND_NE, ret, sign, z, t4, t2); \
        glue(tcg_temp_free_, S)(t4); \
        glue(tcg_temp_free_, S)(t3); \
        glue(tcg_temp_free_, S)(t2); \
        glue(tcg_temp_free_, S)(t1); \
        glue(tcg_temp_free_, S)(t0); \
        glue(tcg_temp_free_, S)(sign_off); \
        glue(tcg_temp_free_, S)(sign); \
        glue(tcg_temp_free_, S)(size); \
        glue(tcg_temp_free_, S)(offset); \
        glue(tcg_temp_free_, S)(z); \
    }

IMPL_GEN_GETF(gen_getf_i64, i64, TCGv_i64, 6, 6, 3, 64)
IMPL_GEN_GETF(gen_getf_i32, i32, TCGv_i32, 5, 5, 2, 32)

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

static void gen_sm_i32(bool sm, TCGv_i32 ret, TCGv_i32 ret_tag, Exception excp)
{
    if (sm) {
        tcg_gen_movi_i32(ret_tag, E2K_TAG_NON_NUMBER32);
        tcg_gen_movi_i32(ret, 0);
    } else {
        e2k_gen_exception(excp);
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
            TCG_COND_NE, src2, 0, l0, E2K_EXCP_DIV); \
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
        TCG_COND_LEU, t1, UINT32_MAX, l0, E2K_EXCP_DIV); \
})
GEN_OP_DIVX(sdivx, tcg_gen_div_i64, { \
    gen_brchecki_i32_i64(sm, ret, ret_tag, \
        TCG_COND_LE, t1, INT32_MAX, l0, E2K_EXCP_DIV); \
    gen_brchecki_i32_i64(sm, ret, ret_tag, \
        TCG_COND_GE, t1, INT32_MIN, l0, E2K_EXCP_DIV); \
})

#define GEN_OP_MODX(name, op) \
    static void glue(gen_, name)(TCGv_i32 ret, TCGv_i32 tag, TCGv_i32 ret_tag, \
        TCGv_i64 src1, TCGv_i32 src2, bool sm) { \
        TCGLabel *l0 = gen_new_label(); \
        TCGv_i64 t0 = tcg_temp_new_i64(); \
        TCGv_i64 t1 = tcg_temp_new_i64(); \
        gen_brchecki_i32(sm, ret, ret_tag, \
            TCG_COND_NE, src2, 0, l0, E2K_EXCP_DIV); \
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

static inline void gen_cmp_i64(TCGv_i64 ret, int opc, TCGv_i64 src1,
    TCGv_i64 src2)
{
    switch(opc) {
    case 0: { /* cmpodb */
        TCGv_i64 s2 = tcg_temp_new_i64();
        TCGv_i64 t0 = tcg_temp_new_i64();
        TCGv_i64 t1 = tcg_temp_new_i64();
        TCGv_i64 t2 = tcg_temp_new_i64();
        TCGv_i64 t3 = tcg_temp_new_i64();

        tcg_gen_not_i64(s2, src2);
        tcg_gen_sub_i64(t0, src1, src2);
        tcg_gen_xor_i64(t1, src1, t0);
        tcg_gen_xor_i64(t2, s2, t0);
        tcg_gen_and_i64(t3, t1, t2);
        tcg_gen_setcondi_i64(TCG_COND_LT, ret, t3, 0);

        tcg_temp_free_i64(t3);
        tcg_temp_free_i64(t2);
        tcg_temp_free_i64(t1);
        tcg_temp_free_i64(t0);
        tcg_temp_free_i64(s2);
        break;
    }
    case 1: /* cmpbdb */
        tcg_gen_setcond_i64(TCG_COND_LTU, ret, src1, src2);
        break;
    case 2: /* cmpedb */
        tcg_gen_setcond_i64(TCG_COND_EQ, ret, src1, src2);
        break;
    case 3: /* cmpbedb */
        tcg_gen_setcond_i64(TCG_COND_LEU, ret, src1, src2);
        break;
    case 4: { /* cmpsdb */
        TCGv_i64 t0 = tcg_temp_new_i64();

        tcg_gen_sub_i64(t0, src1, src2);
        tcg_gen_setcondi_i64(TCG_COND_LT, ret, t0, 0);
        tcg_temp_free_i64(t0);
        break;
    }
    case 5: { /* cmppdb */
        TCGv_i64 t0 = tcg_temp_new_i64();
        TCGv_i64 t1 = tcg_temp_new_i64();
        TCGv_i64 t2 = tcg_temp_new_i64();

        tcg_gen_sub_i64(t0, src1, src2);
        tcg_gen_ctpop_i64(t1, t0);
        tcg_gen_andi_i64(t2, t1, 1);
        tcg_gen_setcondi_i64(TCG_COND_EQ, ret, t2, 0);

        tcg_temp_free_i64(t2);
        tcg_temp_free_i64(t1);
        tcg_temp_free_i64(t0);
        break;
    }
    case 6: /* cmpldb */
        tcg_gen_setcond_i64(TCG_COND_LT, ret, src1, src2);
        break;
    case 7: /* cmpledb */
        tcg_gen_setcond_i64(TCG_COND_LE, ret, src1, src2);
        break;
    default:
        g_assert_not_reached();
        break;
    }
}

static inline void gen_cmp_i32(TCGv_i32 ret, int opc, TCGv_i32 src1,
    TCGv_i32 src2)
{
    switch(opc) {
    case 0: { /* cmposb */
        TCGv_i32 s2 = tcg_temp_new_i32();
        TCGv_i32 t0 = tcg_temp_new_i32();
        TCGv_i32 t1 = tcg_temp_new_i32();
        TCGv_i32 t2 = tcg_temp_new_i32();
        TCGv_i32 t3 = tcg_temp_new_i32();

        tcg_gen_not_i32(s2, src2);
        tcg_gen_sub_i32(t0, src1, src2);
        tcg_gen_xor_i32(t1, src1, t0);
        tcg_gen_xor_i32(t2, s2, t0);
        tcg_gen_and_i32(t3, t1, t2);
        tcg_gen_setcondi_i32(TCG_COND_LT, ret, t3, 0);

        tcg_temp_free_i32(t3);
        tcg_temp_free_i32(t2);
        tcg_temp_free_i32(t1);
        tcg_temp_free_i32(t0);
        tcg_temp_free_i32(s2);
        break;
    }
    case 1: /* cmpbsb */
        tcg_gen_setcond_i32(TCG_COND_LTU, ret, src1, src2);
        break;
    case 2: /* cmpesb */
        tcg_gen_setcond_i32(TCG_COND_EQ, ret, src1, src2);
        break;
    case 3: /* cmpbesb */
        tcg_gen_setcond_i32(TCG_COND_LEU, ret, src1, src2);
        break;
    case 4: { /* cmpssb */
        TCGv_i32 t0 = tcg_temp_new_i32();

        tcg_gen_sub_i32(t0, src1, src2);
        tcg_gen_setcondi_i32(TCG_COND_LT, ret, t0, 0);
        tcg_temp_free_i32(t0);
        break;
    }
    case 5: { /* cmppsb */
        TCGv_i32 t0 = tcg_temp_new_i32();
        TCGv_i32 t1 = tcg_temp_new_i32();
        TCGv_i32 t2 = tcg_temp_new_i32();

        tcg_gen_sub_i32(t0, src1, src2);
        tcg_gen_ctpop_i32(t1, t0);
        tcg_gen_andi_i32(t2, t1, 1);
        tcg_gen_setcondi_i32(TCG_COND_EQ, ret, t2, 0);

        tcg_temp_free_i32(t2);
        tcg_temp_free_i32(t1);
        tcg_temp_free_i32(t0);
        break;
    }
    case 6: /* cmplsb */
        tcg_gen_setcond_i32(TCG_COND_LT, ret, src1, src2);
        break;
    case 7: /* cmplesb */
        tcg_gen_setcond_i32(TCG_COND_LE, ret, src1, src2);
        break;
    default:
        g_assert_not_reached();
        break;
    }
}

static inline void gen_cmpand_i64(TCGv_i64 ret, int opc, TCGv_i64 src1,
    TCGv_i64 src2)
{
    TCGv_i64 t0 = tcg_temp_new_i64();
    tcg_gen_and_i64(t0, src1, src2);

    switch (opc) {
    case 2: /* cmpandedb */
        tcg_gen_setcondi_i64(TCG_COND_EQ, ret, t0, 0);
        break;
    case 4: /* cmpandsdb */
        tcg_gen_setcondi_i64(TCG_COND_LT, ret, t0, 0);
        break;
    case 5: { /* cmpandpdb */
        TCGv_i64 t1 = tcg_temp_new_i64();
        TCGv_i64 t2 = tcg_temp_new_i64();

        tcg_gen_ctpop_i64(t1, t0);
        tcg_gen_andi_i64(t2, t1, 1);
        tcg_gen_setcondi_i64(TCG_COND_EQ, ret, t2, 0);

        tcg_temp_free_i64(t2);
        tcg_temp_free_i64(t1);
        break;
    }
    case 7: /* cmpandledb */
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
    case 2: /* cmpandesb */
        tcg_gen_setcondi_i32(TCG_COND_EQ, ret, t0, 0);
        break;
    case 4: /* cmpandssb */
        tcg_gen_setcondi_i32(TCG_COND_LT, ret, t0, 0);
        break;
    case 5: { /* cmpandpsb */
        TCGv_i32 t1 = tcg_temp_new_i32();
        TCGv_i32 t2 = tcg_temp_new_i32();

        tcg_gen_ctpop_i32(t1, t0);
        tcg_gen_andi_i32(t2, t1, 1);
        tcg_gen_setcondi_i32(TCG_COND_EQ, ret, t2, 0);

        tcg_temp_free_i32(t2);
        tcg_temp_free_i32(t1);
        break;
    }
    case 7: /* cmpandlesb */
        tcg_gen_setcondi_i32(TCG_COND_LE, ret, t0, 0);
        break;
    default:
        e2k_gen_exception(E2K_EXCP_ILLOPC);
        break;
    }

    tcg_temp_free_i32(t0);
}

#define WRAP_FCMP_HELPER_(helper, type) \
    static void glue(gen_, helper)(type ret, type src1, type src2) \
    { \
        glue(gen_helper_, helper)(ret, cpu_env, src1, src2); \
    }
#define WRAP_FCMP_HELPER(op) \
    WRAP_FCMP_HELPER_(glue(op, s), TCGv_i32) \
    WRAP_FCMP_HELPER_(glue(op, d), TCGv_i64)
/* FIXME: currently unused but will be
WRAP_FCMP_HELPER(fcmpeq)
WRAP_FCMP_HELPER(fcmplt)
WRAP_FCMP_HELPER(fcmple)
WRAP_FCMP_HELPER(fcmpuod)
WRAP_FCMP_HELPER(fcmpneq)
WRAP_FCMP_HELPER(fcmpnlt)
WRAP_FCMP_HELPER(fcmpnle)
WRAP_FCMP_HELPER(fcmpod)
*/
#undef WRAP_FCMP_HELPER
#undef WRAP_FCMP_HELPER_

#define glue4(a, b, c, d) glue(glue(a, b), glue(c, d))

#define GENERATE_FCMP_SWITCH_TABLE(f, opc, b, pre, suf, post) \
    switch((opc)) { \
    case b + 0: /* eq */ \
        f = glue4(pre, suf, cmpeq, post); \
        break; \
    case b + 1: /* lt */ \
        f = glue4(pre, suf, cmplt, post); \
        break; \
    case b + 2: /* le */ \
        f = glue4(pre, suf, cmple, post); \
        break; \
    case b + 3: /* uod */ \
        f = glue4(pre, suf, cmpuod, post); \
        break; \
    case b + 4: /* neq */ \
        f = glue4(pre, suf, cmpneq, post); \
        break; \
    case b + 5: /* nlt */ \
        f = glue4(pre, suf, cmpnlt, post); \
        break; \
    case b + 6: /* nle */ \
        f = glue4(pre, suf, cmpnle, post); \
        break; \
    case b + 7: /* od */ \
        f = glue4(pre, suf, cmpod, post); \
        break; \
    default: \
        e2k_gen_exception(E2K_EXCP_ILLOPC); \
        break; \
    }

static void gen_fcmp_i32(TCGv_i32 ret, int opc, TCGv_i32 src1, TCGv_i32 src2)
{
    void (*f)(TCGv_i32, TCGv_env, TCGv_i32, TCGv_i32) = 0;
    TCGv_i32 dst = tcg_temp_new_i32();

    GENERATE_FCMP_SWITCH_TABLE(f, opc, 0, gen_helper_, f, s)

    (*f)(dst, cpu_env, src1, src2);
    tcg_gen_setcondi_i32(TCG_COND_NE, ret, dst, 0);

    tcg_temp_free_i32(dst);
}

static void gen_fcmp_i64(TCGv_i64 ret, int opc, TCGv_i64 src1, TCGv_i64 src2)
{
    void (*f)(TCGv_i64, TCGv_env, TCGv_i64, TCGv_i64) = 0;
    TCGv_i64 dst = tcg_temp_new_i64();

    GENERATE_FCMP_SWITCH_TABLE(f, opc, 0, gen_helper_, f, d)

    (*f)(dst, cpu_env, src1, src2);
    tcg_gen_setcondi_i64(TCG_COND_NE, ret, dst, 0);

    tcg_temp_free_i64(dst);
}

static void gen_fcmp_f80(TCGv_i64 ret, int opc, Src80 src1, Src80 src2)
{
    void (*f)(TCGv_i64, TCGv_env, TCGv_ptr, TCGv_ptr) = 0;
    TCGv_i64 dst = tcg_temp_new_i64();
    TCGv_ptr t0 = tcg_temp_new_ptr();
    TCGv_ptr t1 = tcg_temp_new_ptr();

    tcg_gen_addi_ptr(t0, cpu_env, offsetof(CPUE2KState, t0.f80));
    tcg_gen_addi_ptr(t1, cpu_env, offsetof(CPUE2KState, t1.f80));

    gen_temp_reg_write_i64_i32(src1.lo, src1.hi, t0);
    gen_temp_reg_write_i64_i32(src2.lo, src2.hi, t1);

    GENERATE_FCMP_SWITCH_TABLE(f, opc, 0, gen_helper_, fx, x)

    (*f)(dst, cpu_env, t0, t1);

    tcg_gen_setcondi_i64(TCG_COND_NE, ret, dst, 0);

    tcg_temp_free_ptr(t1);
    tcg_temp_free_ptr(t0);
    tcg_temp_free_i64(dst);
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

static inline void gen_gettag_i64(Instr *instr)
{
    Src64 s2 = get_src2_i64(instr);
    TCGv_i64 dst = get_temp_i64(instr);

    if (s2.tag != NULL) {
        tcg_gen_extu_i32_i64(dst, s2.tag);
    } else {
        tcg_gen_movi_i64(dst, 0);
    }

    set_al_result_reg64(instr, dst);
}

static inline void gen_gettag_i32(Instr *instr)
{
    Src32 s2 = get_src2_i32(instr);
    TCGv_i32 dst = get_temp_i32(instr);

    if (s2.tag != NULL) {
        tcg_gen_mov_i32(dst, s2.tag);
    } else {
        tcg_gen_movi_i32(dst, 0);
    }

    set_al_result_reg32(instr, dst);
}

static inline void gen_puttag_i64(Instr *instr)
{
    TCGLabel *l0 = gen_new_label();
    TCGLabel *l1 = gen_new_label();
    Src64 s1 = get_src1_i64(instr);
    Src32 s2 = get_src2_i32(instr);
    TCGv_i32 tag = get_temp_i32(instr);
    TCGv_i64 dst = get_temp_i64(instr);

    tcg_gen_mov_i64(dst, s1.value);
    tcg_gen_brcondi_i32(TCG_COND_EQ, s2.value, 0, l0);
    gen_tag2_i64(tag, s1.tag, s2.tag);
    tcg_gen_brcondi_i32(TCG_COND_EQ, tag, 0, l0);
    gen_tag_check(instr, tag);
    gen_dst_poison_i64(dst, dst, tag);
    tcg_gen_br(l1);
    gen_set_label(l0);
    tcg_gen_mov_i32(tag, s2.value);
    gen_set_label(l1);
    set_al_result_reg64_tag(instr, dst, tag, false);
}

static inline void gen_puttag_i32(Instr *instr)
{
    TCGLabel *l0 = gen_new_label();
    TCGLabel *l1 = gen_new_label();
    Src32 s1 = get_src1_i32(instr);
    Src32 s2 = get_src2_i32(instr);
    TCGv_i32 tag = get_temp_i32(instr);
    TCGv_i32 dst = get_temp_i32(instr);

    tcg_gen_mov_i32(dst, s1.value);
    tcg_gen_brcondi_i32(TCG_COND_EQ, s2.value, 0, l0);
    gen_tag2_i32(tag, s1.tag, s2.tag);
    tcg_gen_brcondi_i32(TCG_COND_EQ, tag, 0, l0);
    gen_tag_check(instr, tag);
    gen_dst_poison_i32(dst, dst, tag);
    tcg_gen_br(l1);
    gen_set_label(l0);
    tcg_gen_mov_i32(tag, s2.value);
    gen_set_label(l1);
    set_al_result_reg32_tag(instr, dst, tag, false, false, true);
}

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
    Src64 s1 = get_src1_i64(instr);
    Src64 s2 = get_src2_i64(instr);
    Src64 s3 = get_src3_i64(instr);
    TCGv_i32 tag = get_temp_i32(instr);
    TCGv_i64 dst = get_temp_i64(instr);
    TCGv_i64 one = tcg_const_i64(1);
    TCGv_i64 offset = tcg_temp_new_i64();
    TCGv_i64 len = tcg_temp_new_i64();
    TCGv_i64 t0 = tcg_temp_new_i64();
    TCGv_i64 t1 = tcg_temp_new_i64();
    TCGv_i64 t2 = tcg_temp_new_i64();
    TCGv_i64 t3 = tcg_temp_new_i64();
    TCGv_i64 t4 = tcg_temp_new_i64();
    TCGv_i64 t5 = tcg_temp_new_i64();
    TCGv_i32 t6 = tcg_temp_new_i32();

    tcg_gen_extract_i64(offset, s2.value, 0, 6);
    tcg_gen_extract_i64(len, s2.value, 6, 6);
    tcg_gen_shl_i64(t0, one, len);
    tcg_gen_subi_i64(t1, t0, 1);
    tcg_gen_rotr_i64(t2, s1.value, offset);
    tcg_gen_not_i64(t3, t1);
    tcg_gen_and_i64(t4, t2, t3);
    tcg_gen_and_i64(t5, s3.value, t1);
    tcg_gen_or_i64(dst, t4, t5);

    gen_insfd_tag(t6, s2.value, s1.tag, s3.tag);
    gen_tag2_i64(tag, s2.tag, t6);
    gen_al_result_i64(instr, dst, tag);

    tcg_temp_free_i32(t6);
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

static inline void gen_rr_i64(Instr *instr)
{
    TCGv_i64 dst = get_temp_i64(instr);
    TCGv_i32 t0 = tcg_const_i32(instr->src1);

    e2k_gen_save_cpu_state(instr->ctx);
    gen_helper_state_reg_read_i64(dst, cpu_env, t0);
    set_al_result_reg64(instr, dst);
    tcg_temp_free_i32(t0);
}

static inline void gen_rr_i32(Instr *instr)
{
    TCGv_i32 dst = get_temp_i32(instr);
    TCGv_i32 t0 = tcg_const_i32(instr->src1);

    e2k_gen_save_cpu_state(instr->ctx);
    gen_helper_state_reg_read_i32(dst, cpu_env, t0);
    set_al_result_reg32(instr, dst);
    tcg_temp_free_i32(t0);
}

static inline void gen_rw_i64(Instr *instr)
{
    Src64 s2 = get_src2_i64(instr);
    TCGv_i32 t0 = tcg_const_i32(instr->dst);

    gen_tag_check(instr, s2.tag);
    gen_helper_state_reg_write_i64(cpu_env, t0, s2.value);
    tcg_temp_free_i32(t0);
}

static inline void gen_rw_i32(Instr *instr)
{
    Src32 s2 = get_src2_i32(instr);
    TCGv_i32 t0 = tcg_const_i32(instr->dst);

    gen_tag_check(instr, s2.tag);
    gen_helper_state_reg_write_i32(cpu_env, t0, s2.value);
    tcg_temp_free_i32(t0);
}

static void gen_sxt(DisasContext *ctx, Instr *instr)
{
    Src64 s1 = get_src1_i64(instr);
    Src32 s2 = get_src2_i32(instr);
    TCGv_i32 tag = e2k_get_temp_i32(ctx);
    TCGv_i64 dst = e2k_get_temp_i64(ctx);

    gen_tag2_i64(tag, s1.tag, s2.tag);
    gen_helper_sxt(dst, s1.value, s2.value);
    gen_al_result_i64(instr, dst, tag);
}

static void gen_getsp(DisasContext *ctx, Instr *instr)
{
    Src32 s2 = get_src2_i32(instr);
    TCGv_i32 tag = e2k_get_temp_i32(ctx);
    TCGv_i64 dst = e2k_get_temp_i64(ctx);

    gen_tag1_i64(tag, s2.tag);
    gen_helper_getsp(dst, cpu_env, s2.value);
    gen_al_result_i64(instr, dst, tag);
}

static void gen_movts(Instr *instr)
{
    Src32 s2 = get_src2_i32(instr);
    set_al_result_reg32_tag(instr, s2.value, s2.tag, false, false, false);
}

static void gen_movtcs(Instr *instr)
{
    Src32 s2 = get_src2_i32(instr);
    gen_tag_check(instr, s2.tag);
    set_al_result_reg32_tag(instr, s2.value, s2.tag, false, false, false);
}

static void gen_movtd(Instr *instr)
{
    Src64 s2 = get_src2_i64(instr);
    set_al_result_reg64_tag(instr, s2.value, s2.tag, false);
}

static void gen_movtcd(Instr *instr)
{
    Src64 s2 = get_src2_i64(instr);
    gen_tag_check(instr, s2.tag);
    set_al_result_reg64_tag(instr, s2.value, s2.tag, false);
}

static void gen_movtq_inner(Instr *instr, Src64 src)
{
    if (check_qr(instr->src2, instr->chan) && check_qr(instr->dst, instr->chan)) {
        set_al_result_reg64_tag(instr, src.value, src.tag, false);
    } else {
        e2k_tr_gen_exception(instr->ctx, E2K_EXCP_ILLOPN);
    }
}

static void gen_movtq(Instr *instr)
{
    gen_movtq_inner(instr, get_src2_i64(instr));
}

static void gen_movtcq(Instr *instr)
{
    Src64 s2 = get_src2_i64(instr);
    gen_tag_check(instr, s2.tag);
    gen_movtq_inner(instr, s2);
}

static inline bool gen_ld_mas_mod(DisasContext *ctx, Instr *instr, uint8_t mod)
{
    TCGv_i32 size;
    TCGv_i32 reg = tcg_temp_new_i32();
    bool ret = true;

    // FIXME: %empty
    e2k_gen_reg_index(instr->ctx, reg, instr->dst);
    size = tcg_const_i32(1 << (instr->opc1 - 0x64));

    switch (mod) {
    case 3:
        if (is_chan_25(instr->chan)) {
            /* ld,{2,5} [ addr ], dst, mas=X (mod 3) */
            if (ctx->mlock == NULL) {
                ctx->mlock = e2k_get_temp_i32(ctx);
                /* always go to fixing code */
                tcg_gen_movi_i32(ctx->mlock, 1);
            }
            goto ok_exit;
        }
        break;
    case 4:
        if (instr->sm && is_chan_03(instr->chan)) {
            /* ld,{0,3},sm [ addr ], dst, mas=X (mod 4) */
            /* always ignore lock load */
            ret = false;
            goto ok_exit;
        }
        break;
    }

    e2k_todo(ctx, "opc %#x, chan %d, mod=%#x", instr->opc1, instr->chan, mod);

ok_exit:
    tcg_temp_free_i32(size);
    tcg_temp_free_i32(reg);

    return ret;
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
            /* flush cache */
            memop |= MO_LE;
            e2k_todo(ctx, "opc %#x, chan %d, flush cache", instr->opc1,
                instr->chan);
            break;
        default:
            e2k_todo(ctx, "opc %#x, chan %d, mas=%#x (opc %#x)", instr->opc1,
                instr->chan, mas, opc);
            return 0;
        }
    } else if (mas) {
        int mod = extract8(mas, 0, 3);
//        int dc = extract8(mas, 5, 2);

        if (mod != 0) {
            if (0x64 <= instr->opc1 && instr->opc1 < 0x68) {
                if (!gen_ld_mas_mod(ctx, instr, mod)) {
                    return 0;
                }
            } else {
                // TODO: mas modes
                e2k_todo(ctx, "opc %#x, chan %d, mas=%#x, mod=%#x", instr->opc1,
                    instr->chan, mas, mod);
            }
        }

        memop |= GET_BIT(mas, 3) ? MO_BE : MO_LE;
        memop |= GET_BIT(mas, 4) ? MO_UNALN : MO_ALIGN;
    } else {
        memop |= MO_LE | MO_ALIGN;
    }

    return memop;
}

static void gen_ld(Instr *instr, MemOp memop)
{
    TCGv_i32 tag = get_temp_i32(instr);
    TCGv_i64 dst = get_temp_i64(instr);

    memop = gen_mas(instr, memop);

    if (memop == 0) {
        /* ignore load */
        tcg_gen_movi_i32(tag, E2K_TAG_NON_NUMBER64);
        tcg_gen_movi_i64(dst, E2K_LD_RESULT_INVALID);
    } else {
        TCGLabel *l0 = gen_new_label();
        TCGLabel *l1 = gen_new_label();
        Src64 s1 = get_src1_i64(instr);
        Src64 s2 = get_src2_i64(instr);
        TCGv_i64 t0 = tcg_temp_local_new_i64();

        gen_tag2_i64(tag, s1.tag, s2.tag);
        tcg_gen_add_i64(t0, s1.value, s2.value);

        if (instr->sm) {
            TCGv_i32 t1 = tcg_temp_new_i32();

            gen_helper_probe_read_access(t1, cpu_env, t0);
            tcg_gen_brcondi_i32(TCG_COND_EQ, t1, 1, l0);
            tcg_gen_movi_i32(tag, E2K_TAG_NON_NUMBER64);
            tcg_gen_movi_i64(dst, E2K_LD_RESULT_INVALID);
            tcg_gen_br(l1);

            tcg_temp_free_i32(t1);
        }

        gen_set_label(l0);
        tcg_gen_qemu_ld_i64(dst, t0, instr->ctx->mmuidx, memop);

        gen_set_label(l1);

        tcg_temp_free_i64(t0);
    }

    gen_al_result_i64(instr, dst, tag);
}

#define IMPL_ST(NAME, S) \
    static void NAME(Instr *instr, MemOp memop) \
    { \
        memop = gen_mas(instr, memop); \
        \
        if (memop != 0) { \
            TCGLabel *l0 = gen_new_label(); \
            Src64 s1 = get_src1_i64(instr); \
            Src64 s2 = get_src2_i64(instr); \
            glue(Src, S) s4 = glue(get_src4_i, S)(instr); \
            TCGv_i64 t0 = tcg_temp_local_new_i64(); \
            \
            gen_loop_mode_st(instr->ctx, l0); \
            gen_tag_check(instr, s1.tag); \
            gen_tag_check(instr, s2.tag); \
            gen_tag_check(instr, s4.tag); \
            tcg_gen_add_i64(t0, s1.value, s2.value); \
            \
            if (instr->sm) { \
                TCGv_i32 t1 = tcg_temp_new_i32(); \
                gen_helper_probe_write_access(t1, cpu_env, t0); \
                tcg_gen_brcondi_i32(TCG_COND_EQ, t1, 0, l0); \
                tcg_temp_free_i32(t1); \
            } \
            \
            glue(tcg_gen_qemu_st_i, S)(s4.value, t0, instr->ctx->mmuidx, memop); \
            gen_set_label(l0); \
            \
            tcg_temp_free_i64(t0); \
        } \
    }

IMPL_ST(gen_st_ddd, 64)
IMPL_ST(gen_st_dds, 32)

static inline void gen_movfi(Instr *instr)
{
    Src80 src2 = get_src2_i80(instr);
    TCGv_i32 tag = get_temp_i32(instr);
    gen_tag1_i32(tag, src2.tag);
    gen_al_result_i32(instr, src2.hi, tag);
}

static inline void gen_movif(Instr *instr)
{
    Src64 src1 = get_src1_i64(instr);
    Src32 src2 = get_src2_i32(instr);
    TCGv_i32 tag = get_temp_i32(instr);
    gen_tag2_i64(tag, src1.tag, src2.tag);
    gen_al_result_i80(instr, src1.value, src2.value, tag);
}

static inline void gen_movx(Instr *instr)
{
    Src80 src2 = get_src2_i80(instr);
    TCGv_i32 tag = get_temp_i32(instr);
    gen_tag1_i64(tag, src2.tag);
    gen_al_result_i80(instr, src2.lo, src2.hi, tag);
}

static inline void gen_movxa(Instr *instr)
{
    Src80 src2 = get_src2_i80(instr);
    TCGv_i32 tag = get_temp_i32(instr);
    TCGv_i32 dst_hi = get_temp_i32(instr);
    gen_tag1_i64(tag, src2.tag);
    tcg_gen_andi_i32(dst_hi, src2.hi, 0x7fff);
    gen_al_result_i80(instr, src2.lo, dst_hi, tag);
}

static inline void gen_movxc(Instr *instr)
{
    Src80 src2 = get_src2_i80(instr);
    TCGv_i32 tag = get_temp_i32(instr);
    TCGv_i32 dst_hi = get_temp_i32(instr);
    gen_tag1_i64(tag, src2.tag);
    tcg_gen_xori_i32(dst_hi, src2.hi, 0x8000);
    gen_al_result_i80(instr, src2.lo, dst_hi, tag);
}

static inline void gen_fstofx(Src80 *ret, TCGv_i32 src2)
{
    TCGv_ptr t0 = tcg_temp_new_ptr();

    tcg_gen_addi_ptr(t0, cpu_env, offsetof(CPUE2KState, t0.f80));
    gen_helper_fstofx(t0, cpu_env, src2);
    gen_temp_reg_read_i64_i32(t0, ret->lo, ret->hi);
    tcg_temp_free_ptr(t0);
}

static inline void gen_istofx(Src80 *ret, TCGv_i32 src2)
{
    TCGv_ptr t0 = tcg_temp_new_ptr();

    tcg_gen_addi_ptr(t0, cpu_env, offsetof(CPUE2KState, t0.f80));
    gen_helper_istofx(t0, cpu_env, src2);
    gen_temp_reg_read_i64_i32(t0, ret->lo, ret->hi);
    tcg_temp_free_ptr(t0);
}

static inline void gen_fdtofx(Src80 *ret, TCGv_i64 src2)
{
    TCGv_ptr t0 = tcg_temp_new_ptr();

    tcg_gen_addi_ptr(t0, cpu_env, offsetof(CPUE2KState, t0));
    gen_helper_fdtofx(t0, cpu_env, src2);
    gen_temp_reg_read_i64_i32(t0, ret->lo, ret->hi);
    tcg_temp_free_ptr(t0);
}

static inline void gen_idtofx(Src80 *ret, TCGv_i64 src2)
{
    TCGv_ptr t0 = tcg_temp_new_ptr();

    tcg_gen_addi_ptr(t0, cpu_env, offsetof(CPUE2KState, t0));
    gen_helper_idtofx(t0, cpu_env, src2);
    gen_temp_reg_read_i64_i32(t0, ret->lo, ret->hi);
    tcg_temp_free_ptr(t0);
}

static inline void gen_fxtofs(TCGv_i32 ret, Src80 src2)
{
    TCGv_ptr t0 = tcg_temp_new_ptr();

    tcg_gen_addi_ptr(t0, cpu_env, offsetof(CPUE2KState, t0));
    gen_temp_reg_write_i64_i32(src2.lo, src2.hi, t0);
    gen_helper_fxtofs(ret, cpu_env, t0);
    tcg_temp_free_ptr(t0);
}

static inline void gen_fxtois(TCGv_i32 ret, Src80 src2)
{
    TCGv_ptr t0 = tcg_temp_new_ptr();

    tcg_gen_addi_ptr(t0, cpu_env, offsetof(CPUE2KState, t0));
    gen_temp_reg_write_i64_i32(src2.lo, src2.hi, t0);
    gen_helper_fxtois(ret, cpu_env, t0);
    tcg_temp_free_ptr(t0);
}

static inline void gen_fxtoistr(TCGv_i32 ret, Src80 src2)
{
    TCGv_ptr t0 = tcg_temp_new_ptr();

    tcg_gen_addi_ptr(t0, cpu_env, offsetof(CPUE2KState, t0));
    gen_temp_reg_write_i64_i32(src2.lo, src2.hi, t0);
    gen_helper_fxtoistr(ret, cpu_env, t0);
    tcg_temp_free_ptr(t0);
}

static inline void gen_fxtofd(TCGv_i64 ret, Src80 src2)
{
    TCGv_ptr t0 = tcg_temp_new_ptr();

    tcg_gen_addi_ptr(t0, cpu_env, offsetof(CPUE2KState, t0));
    gen_temp_reg_write_i64_i32(src2.lo, src2.hi, t0);
    gen_helper_fxtofd(ret, cpu_env, t0);
    tcg_temp_free_ptr(t0);
}

static inline void gen_fxtoid(TCGv_i64 ret, Src80 src2)
{
    TCGv_ptr t0 = tcg_temp_new_ptr();

    tcg_gen_addi_ptr(t0, cpu_env, offsetof(CPUE2KState, t0));
    gen_temp_reg_write_i64_i32(src2.lo, src2.hi, t0);
    gen_helper_fxtoid(ret, cpu_env, t0);
    tcg_temp_free_ptr(t0);
}

static inline void gen_fxtoidtr(TCGv_i64 ret, Src80 src2)
{
    TCGv_ptr t0 = tcg_temp_new_ptr();

    tcg_gen_addi_ptr(t0, cpu_env, offsetof(CPUE2KState, t0));
    gen_temp_reg_write_i64_i32(src2.lo, src2.hi, t0);
    gen_helper_fxtoidtr(ret, cpu_env, t0);
    tcg_temp_free_ptr(t0);
}

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

static inline void gen_pshufh(Instr *instr)
{
    Src64 s2 = get_src2_i64(instr);
    TCGv_i32 tag = get_temp_i32(instr);
    TCGv_i64 dst = get_temp_i64(instr);
    TCGv_i32 t0 = tcg_const_i32(instr->src3);

    gen_tag1_i64(tag, s2.tag);
    gen_helper_pshufh(dst, s2.value, t0);
    gen_al_result_i64(instr, dst, tag);
    tcg_temp_free_i32(t0);
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

static void gen_aasti_incr(DisasContext *ctx, Instr *instr)
{
    uint16_t rlp = find_am_cond(ctx, instr->chan);
    TCGLabel *l0 = gen_new_label();
    TCGv_i32 t0 = tcg_temp_new_i32();

    if (ctx->loop_mode) {
        TCGLabel *l1 = gen_new_label();
        TCGv_i32 t0 = tcg_temp_local_new_i32();

        tcg_gen_brcondi_i32(TCG_COND_NE, e2k_cs.lsr_pcnt, 0, l0);
        e2k_gen_is_loop_end_i32(t0);
        tcg_gen_brcondi_i32(TCG_COND_EQ, t0, 0, l1);
        tcg_gen_brcondi_i32(TCG_COND_EQ, e2k_cs.lsr_strmd, 0, l0);
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

    tcg_gen_muli_i32(t0, e2k_cs.aaincr[instr->aaincr], instr->aaincr_len);
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
    tcg_gen_extu_i32_tl(t2, e2k_cs.aasti[instr->aaind]);
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
    Src64 s4 = get_src4_i64(instr);

    gen_loop_mode_st(ctx, l0);
    gen_tag_check(instr, s4.tag);
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
            tcg_temp_free_i32(t1);
        }

        tcg_gen_qemu_st_i64(s4.value, t0, ctx->mmuidx, MO_Q);
        gen_set_label(l0);
        tcg_temp_free(t0);

        if (instr->aaopc & 1) {
            /* incr must be executed outside of the staa predicate condition */
            instr->aaincr_len = 8;
        }
    }

    gen_set_label(l0);
}

static void gen_staa_i32(Instr *instr, MemOp memop)
{
    DisasContext *ctx = instr->ctx;
    uint8_t mas = instr->mas;
    TCGLabel *l0 = gen_new_label();
    Src32 s4 = get_src4_i32(instr);

    gen_loop_mode_st(ctx, l0);
    gen_tag_check(instr, s4.tag);
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
            e2k_todo(ctx, "staa%c mas=%#x", c, mas);
        }

        gen_aad_ptr(ctx, t0, instr);

        if (instr->sm) {
            TCGv_i32 t1 = tcg_temp_new_i32();
            gen_helper_probe_write_access(t1, cpu_env, t0);
            tcg_gen_brcondi_i32(TCG_COND_EQ, t1, 0, l0);
            tcg_temp_free_i32(t1);
        }

        tcg_gen_qemu_st_i32(s4.value, t0, ctx->mmuidx, memop);
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
}

static void gen_alopf1_ddd(Instr *instr,
    void (*op)(TCGv_i64, TCGv_i64, TCGv_i64))
{
    Src64 s1 = get_src1_i64(instr);
    Src64 s2 = get_src2_i64(instr);
    TCGv_i32 tag = get_temp_i32(instr);
    TCGv_i64 dst = get_temp_i64(instr);

    gen_tag2_i64(tag, s1.tag, s2.tag);
    (*op)(dst, s1.value, s2.value);
    gen_al_result_i64(instr, dst, tag);
}

static void gen_alopf11_dddi(Instr *instr,
    void (*op)(TCGv_i64, TCGv_i64, TCGv_i64, int))
{
    Src64 s1 = get_src1_i64(instr);
    Src64 s2 = get_src2_i64(instr);
    TCGv_i32 tag = get_temp_i32(instr);
    TCGv_i64 dst = get_temp_i64(instr);

    gen_tag2_i64(tag, s1.tag, s2.tag);
    (*op)(dst, s1.value, s2.value, instr->src3);
    gen_al_result_i64(instr, dst, tag);
}

static void gen_alopf1_dedd(Instr *instr,
    void (*op)(TCGv_i64, TCGv_env, TCGv_i64, TCGv_i64))
{
    Src64 s1 = get_src1_i64(instr);
    Src64 s2 = get_src2_i64(instr);
    TCGv_i32 tag = get_temp_i32(instr);
    TCGv_i64 dst = get_temp_i64(instr);
    
    gen_tag2_i64(tag, s1.tag, s2.tag);
    (*op)(dst, cpu_env, s1.value, s2.value);
    gen_al_result_i64(instr, dst, tag);
}

static void gen_alopf1_deds(Instr *instr,
    void (*op)(TCGv_i64, TCGv_env, TCGv_i64, TCGv_i32))
{
    Src64 s1 = get_src1_i64(instr);
    Src32 s2 = get_src2_i32(instr);
    TCGv_i32 tag = get_temp_i32(instr);
    TCGv_i64 dst = get_temp_i64(instr);

    gen_tag2_i64(tag, s1.tag, s2.tag);
    (*op)(dst, cpu_env, s1.value, s2.value);
    gen_al_result_i64(instr, dst, tag);
}

static void gen_alopf1_sss(Instr *instr,
    void (*op)(TCGv_i32, TCGv_i32, TCGv_i32))
{
    Src32 s1 = get_src1_i32(instr);
    Src32 s2 = get_src2_i32(instr);
    TCGv_i32 tag = get_temp_i32(instr);
    TCGv_i32 dst = get_temp_i32(instr);

    gen_tag2_i32(tag, s1.tag, s2.tag);
    (*op)(dst, s1.value, s2.value);
    gen_al_result_i32(instr, dst, tag);
}

static void gen_alopf1_sess(Instr *instr,
    void (*op)(TCGv_i32, TCGv_env, TCGv_i32, TCGv_i32))
{
    Src32 s1 = get_src1_i32(instr);
    Src32 s2 = get_src2_i32(instr);
    TCGv_i32 tag = get_temp_i32(instr);
    TCGv_i32 dst = get_temp_i32(instr);
    
    gen_tag2_i32(tag, s1.tag, s2.tag);
    (*op)(dst, cpu_env, s1.value, s2.value);
    gen_al_result_i32(instr, dst, tag);
}

static void gen_alopf1_sedd(Instr *instr,
    void (*op)(TCGv_i32, TCGv_env, TCGv_i64, TCGv_i64))
{
    Src64 s1 = get_src1_i64(instr);
    Src64 s2 = get_src2_i64(instr);
    TCGv_i32 tag = get_temp_i32(instr);
    TCGv_i32 dst = get_temp_i32(instr);

    gen_tag2_i64(tag, s1.tag, s2.tag);
    (*op)(dst, cpu_env, s1.value, s2.value);
    gen_al_result_i32(instr, dst, tag);
}

static inline void gen_alopf1_sf80(TCGv_i32 dst, Src80 src1, Src80 src2,
   void (*op)(TCGv_i32, TCGv_env, TCGv_ptr, TCGv_ptr))
{
    TCGv_ptr t0 = tcg_temp_new_ptr();
    TCGv_ptr t1 = tcg_temp_new_ptr();

    tcg_gen_addi_ptr(t0, cpu_env, offsetof(CPUE2KState, t0.f80));
    tcg_gen_addi_ptr(t1, cpu_env, offsetof(CPUE2KState, t1.f80));

    gen_temp_reg_write_i64_i32(src1.lo, src1.hi, t0);
    gen_temp_reg_write_i64_i32(src2.lo, src2.hi, t1);

    (*op)(dst, cpu_env, t0, t1);

    tcg_temp_free_ptr(t1);
    tcg_temp_free_ptr(t0);
}

static void gen_alopf1_sexs(Instr *instr,
    void (*op)(TCGv_i32, TCGv_env, TCGv_ptr, TCGv_ptr))
{
    Src80 s1 = get_src1_i80(instr);
    Src32 s2 = get_src2_i32(instr);
    TCGv_i32 tag = get_temp_i32(instr);
    TCGv_i32 dst = get_temp_i32(instr);
    Src80 t0 = temp_new_src80();

    gen_tag2_i32(tag, s1.tag, s2.tag);
    gen_fstofx(&t0, s2.value);
    gen_alopf1_sf80(dst, s1, t0, op);
    gen_al_result_i32(instr, dst, tag);

    temp_free_src80(&t0);
}

static void gen_alopf1_sexd(Instr *instr,
    void (*op)(TCGv_i32, TCGv_env, TCGv_ptr, TCGv_ptr))
{
    Src80 s1 = get_src1_i80(instr);
    Src64 s2 = get_src2_i64(instr);
    TCGv_i32 tag = get_temp_i32(instr);
    TCGv_i32 dst = get_temp_i32(instr);
    Src80 t0 = temp_new_src80();

    gen_tag2_i64(tag, s1.tag, s2.tag);
    gen_fdtofx(&t0, s2.value);
    gen_alopf1_sf80(dst, s1, t0, op);
    gen_al_result_i32(instr, dst, tag);

    temp_free_src80(&t0);
}

static void gen_alopf1_sexx(Instr *instr,
    void (*op)(TCGv_i32, TCGv_env, TCGv_ptr, TCGv_ptr))
{
    Src80 s1 = get_src1_i80(instr);
    Src80 s2 = get_src2_i80(instr);
    TCGv_i32 tag = get_temp_i32(instr);
    TCGv_i32 dst = get_temp_i32(instr);

    gen_tag2_i32(tag, s1.tag, s2.tag);
    gen_alopf1_sf80(dst, s1, s2, op);
    gen_al_result_i32(instr, dst, tag);
}

static void gen_alopf1_dss(Instr *instr,
    void (*op)(TCGv_i64, TCGv_i32, TCGv_i32))
{
    Src32 s1 = get_src1_i32(instr);
    Src32 s2 = get_src2_i32(instr);
    TCGv_i32 tag = get_temp_i32(instr);
    TCGv_i64 dst = get_temp_i64(instr);

    gen_tag2_i64(tag, s1.tag, s2.tag);
    (*op)(dst, s1.value, s2.value);
    gen_al_result_i64(instr, dst, tag);
}

static void gen_alopf1_dttdd(Instr *instr,
    void (*op)(TCGv_i64, TCGv_i32, TCGv_i32, TCGv_i64, TCGv_i64))
{
    Src64 s1 = get_src1_i64(instr);
    Src64 s2 = get_src2_i64(instr);
    TCGv_i32 tag = get_temp_i32(instr);
    TCGv_i64 dst = get_temp_i64(instr);

    gen_tag2_i64(tag, s1.tag, s2.tag);
    (*op)(dst, tag, tag, s1.value, s2.value);
    gen_al_result_i64(instr, dst, tag);
}

static void gen_alopf1_sttss(Instr *instr,
    void (*op)(TCGv_i32, TCGv_i32, TCGv_i32, TCGv_i32, TCGv_i32))
{
    Src32 s1 = get_src1_i32(instr);
    Src32 s2 = get_src2_i32(instr);
    TCGv_i32 tag = get_temp_i32(instr);
    TCGv_i32 dst = get_temp_i32(instr);

    gen_tag2_i32(tag, s1.tag, s2.tag);
    (*op)(dst, tag, tag, s1.value, s2.value);
    gen_al_result_i32(instr, dst, tag);
}

static void gen_alopf1_sttds(Instr *instr,
    void (*op)(TCGv_i32, TCGv_i32, TCGv_i32, TCGv_i64, TCGv_i32, bool))
{
    Src64 s1 = get_src1_i64(instr);
    Src32 s2 = get_src2_i32(instr);
    TCGv_i32 tag = get_temp_i32(instr);
    TCGv_i32 dst = get_temp_i32(instr);

    gen_tag2_i32(tag, s1.tag, s2.tag);
    (*op)(dst, tag, tag, s1.value, s2.value, instr->sm);
    gen_al_result_i32(instr, dst, tag);
}

static void gen_alopf1_cmp_ddb(Instr *instr,
    void (*op)(TCGv_i64, int, TCGv_i64, TCGv_i64))
{
    TCGLabel *l0 = gen_new_label();
    TCGLabel *l1 = gen_new_label();
    Src64 s1 = get_src1_i64(instr);
    Src64 s2 = get_src2_i64(instr);
    TCGv_i32 dst = get_temp_i32(instr);
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i64 t1 = tcg_temp_new_i64();

    gen_tag2_i64(t0, s1.tag, s2.tag);
    gen_tag_check(instr, t0);
    tcg_gen_brcondi_i32(TCG_COND_NE, t0, 0, l0);
    (*op)(t1, instr->opc_cmp, s1.value, s2.value);
    tcg_gen_extrl_i64_i32(dst, t1);
    tcg_gen_br(l1);
    gen_set_label(l0);
    tcg_gen_movi_i32(dst, 2);
    gen_set_label(l1);
    set_al_result_preg(instr, instr->dst_preg, dst);
    tcg_temp_free_i64(t1);
    tcg_temp_free_i32(t0);
}

static void gen_alopf1_cmp_ssb(Instr *instr,
    void (*op)(TCGv_i32, int, TCGv_i32, TCGv_i32))
{
    TCGLabel *l0 = gen_new_label();
    TCGLabel *l1 = gen_new_label();
    Src32 s1 = get_src1_i32(instr);
    Src32 s2 = get_src2_i32(instr);
    TCGv_i32 dst = get_temp_i32(instr);
    TCGv_i32 t0 = tcg_temp_new_i32();

    gen_tag2_i32(t0, s1.tag, s2.tag);
    gen_tag_check(instr, t0);
    tcg_gen_brcondi_i32(TCG_COND_NE, t0, 0, l0);
    (*op)(dst, instr->opc_cmp, s1.value, s2.value);
    tcg_gen_br(l1);
    gen_set_label(l0);
    tcg_gen_movi_i32(dst, 2);
    gen_set_label(l1);
    set_al_result_preg(instr, instr->dst_preg, dst);
    tcg_temp_free_i32(t0);
}

static void gen_alopf1_cmp_f80(Instr *instr, Src80 s2, TCGv_i32 s2tag)
{
    TCGLabel *l0 = gen_new_label();
    TCGLabel *l1 = gen_new_label();
    Src80 s1 = get_src1_i80(instr);
    TCGv_i32 dst = get_temp_i32(instr);
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i64 t1 = tcg_temp_new_i64();

    gen_tag2_i64(t0, s1.tag, s2tag);
    gen_tag_check(instr, t0);
    tcg_gen_brcondi_i32(TCG_COND_NE, t0, 0, l0);
    gen_fcmp_f80(t1, instr->opc_cmp, s1, s2);
    tcg_gen_extrl_i64_i32(dst, t1);
    tcg_gen_br(l1);
    gen_set_label(l0);
    tcg_gen_movi_i32(dst, 2);
    gen_set_label(l1);
    set_al_result_preg(instr, instr->dst_preg, dst);
    tcg_temp_free_i64(t1);
    tcg_temp_free_i32(t0);
}

static void gen_alopf1_cmp_xx(Instr *instr)
{
    Src80 s2 = get_src2_i80(instr);
    gen_alopf1_cmp_f80(instr, s2, s2.tag);
}

static void gen_alopf1_cmp_xd(Instr *instr)
{
    Src64 s2 = get_src2_i64(instr);
    Src80 t2 = temp_new_src80();

    gen_fdtofx(&t2, s2.value);
    gen_alopf1_cmp_f80(instr, t2, s2.tag);

    temp_free_src80(&t2);
}

static void gen_alopf1_cmp_xs(Instr *instr)
{
    Src32 s2 = get_src2_i32(instr);
    Src80 t2 = temp_new_src80();

    gen_fstofx(&t2, s2.value);
    gen_alopf1_cmp_f80(instr, t2, s2.tag);

    temp_free_src80(&t2);
}

static void gen_alopf1_mrgc_sss(Instr *instr)
{
    Src32 s1 = get_src1_i32(instr);
    Src32 s2 = get_src2_i32(instr);
    TCGv_i32 tag = get_temp_i32(instr);
    TCGv_i32 dst = get_temp_i32(instr);
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i32 t1 = tcg_temp_new_i32();

    gen_mrgc_i32(instr->ctx, instr->chan, t0);
    gen_merge_i32(t1, s1.tag, s2.tag, t0);
    gen_merge_i32(dst, s1.value, s2.value, t0);
    gen_tag1_i32(tag, t1);
    gen_al_result_i32(instr, dst, tag);

    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
}

static void gen_alopf1_mrgc_ddd(Instr *instr)
{
    Src64 s1 = get_src1_i64(instr);
    Src64 s2 = get_src2_i64(instr);
    TCGv_i32 tag = get_temp_i32(instr);
    TCGv_i64 dst = get_temp_i64(instr);
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i32 t1 = tcg_temp_new_i32();

    gen_mrgc_i32(instr->ctx, instr->chan, t0);
    gen_merge_i32(t1, s1.tag, s2.tag, t0);
    gen_merge_i64(dst, s1.value, s2.value, t0);
    gen_tag1_i64(tag, t1);
    gen_al_result_i64(instr, dst, tag);

    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
}

static void gen_alopf21_i64(Instr *instr,
    void (*op)(TCGv_i64, TCGv_i64, TCGv_i64, TCGv_i64))
{
    Src64 s1 = get_src1_i64(instr);
    Src64 s2 = get_src2_i64(instr);
    Src64 s3 = get_src3_i64(instr);
    TCGv_i32 tag = get_temp_i32(instr);
    TCGv_i64 dst = get_temp_i64(instr);

    gen_tag3_i64(tag, s1.tag, s2.tag, s3.tag);
    (*op)(dst, s1.value, s2.value, s3.value);
    gen_al_result_i64(instr, dst, tag);
}

static void gen_alopf21_i32(Instr *instr,
    void (*op)(TCGv_i32, TCGv_i32, TCGv_i32, TCGv_i32))
{
    Src32 s1 = get_src1_i32(instr);
    Src32 s2 = get_src2_i32(instr);
    Src32 s3 = get_src3_i32(instr);
    TCGv_i32 tag = get_temp_i32(instr);
    TCGv_i32 dst = get_temp_i32(instr);

    gen_tag3_i32(tag, s1.tag, s2.tag, s3.tag);
    (*op)(dst, s1.value, s2.value, s3.value);
    gen_al_result_i32(instr, dst, tag);
}

static void gen_alopf2_ss(Instr *instr, void (*op)(TCGv_i32, TCGv_i32))
{
    Src32 s2 = get_src2_i32(instr);
    TCGv_i32 dst = get_temp_i32(instr);
    TCGv_i32 tag = get_temp_i32(instr);
    
    gen_tag1_i32(tag, s2.tag);
    (*op)(dst, s2.value);
    gen_al_result_i32(instr, dst, tag);
}

static void gen_alopf2_dd(Instr *instr, void (*op)(TCGv_i64, TCGv_i64))
{
    Src64 s2 = get_src2_i64(instr);
    TCGv_i64 dst = get_temp_i64(instr);
    TCGv_i32 tag = get_temp_i32(instr);

    gen_tag1_i64(tag, s2.tag);
    (*op)(dst, s2.value);
    gen_al_result_i64(instr, dst, tag);
}

static void gen_alopf2_ses(Instr *instr,
    void (*op)(TCGv_i32, TCGv_env, TCGv_i32))
{
    Src32 s2 = get_src2_i32(instr);
    TCGv_i32 dst = get_temp_i32(instr);
    TCGv_i32 tag = get_temp_i32(instr);

    gen_tag1_i32(tag, s2.tag);
    (*op)(dst, cpu_env, s2.value);
    gen_al_result_i32(instr, dst, tag);
}

static void gen_alopf2_ded(Instr *instr,
    void (*op)(TCGv_i64, TCGv_env, TCGv_i64))
{
    Src64 s2 = get_src2_i64(instr);
    TCGv_i64 dst = get_temp_i64(instr);
    TCGv_i32 tag = get_temp_i32(instr);

    gen_tag1_i64(tag, s2.tag);
    (*op)(dst, cpu_env, s2.value);
    gen_al_result_i64(instr, dst, tag);
}

static void gen_alopf2_sed(Instr *instr,
    void (*op)(TCGv_i32, TCGv_env, TCGv_i64))
{
    Src64 s2 = get_src2_i64(instr);
    TCGv_i32 dst = get_temp_i32(instr);
    TCGv_i32 tag = get_temp_i32(instr);

    gen_tag1_i32(tag, s2.tag);
    (*op)(dst, cpu_env, s2.value);
    gen_al_result_i32(instr, dst, tag);
}

static void gen_alopf2_des(Instr *instr,
    void (*op)(TCGv_i64, TCGv_env, TCGv_i32))
{
    Src32 s2 = get_src2_i32(instr);
    TCGv_i64 dst = get_temp_i64(instr);
    TCGv_i32 tag = get_temp_i32(instr);

    gen_tag1_i64(tag, s2.tag);
    (*op)(dst, cpu_env, s2.value);
    gen_al_result_i64(instr, dst, tag);
}

static inline void gen_alopf1_f80(Src80 *ret, Src80 src1, Src80 src2,
    void (*op)(TCGv_env, TCGv_ptr, TCGv_ptr))
{
    TCGv_ptr t0 = tcg_temp_new_ptr();
    TCGv_ptr t1 = tcg_temp_new_ptr();

    gen_tag2_i64(ret->tag, src1.tag, src2.tag);

    tcg_gen_addi_ptr(t0, cpu_env, offsetof(CPUE2KState, t0.f80));
    tcg_gen_addi_ptr(t1, cpu_env, offsetof(CPUE2KState, t1.f80));

    gen_temp_reg_write_i64_i32(src1.lo, src1.hi, t0);
    gen_temp_reg_write_i64_i32(src2.lo, src2.hi, t1);

    (*op)(cpu_env, t0, t1);

    gen_temp_reg_read_i64_i32(t0, ret->lo, ret->hi);

    tcg_temp_free_ptr(t1);
    tcg_temp_free_ptr(t0);
}

static inline void gen_alopf1_xexs_raw(Src80 *ret, Src80 src1, Src32 src2,
    void (*op)(TCGv_env, TCGv_ptr, TCGv_i32))
{
    TCGv_ptr t0 = tcg_temp_new_ptr();

    gen_tag2_i64(ret->tag, src1.tag, src2.tag);
    tcg_gen_addi_ptr(t0, cpu_env, offsetof(CPUE2KState, t0.f80));
    gen_temp_reg_write_i64_i32(src1.lo, src1.hi, t0);
    (*op)(cpu_env, t0, src2.value);
    gen_temp_reg_read_i64_i32(t0, ret->lo, ret->hi);
    tcg_temp_free_ptr(t0);
}

static inline void gen_alopf1_xxx(Instr *instr,
    void (*op)(TCGv_env, TCGv_ptr, TCGv_ptr))
{
    Src80 src1 = get_src1_i80(instr);
    Src80 src2 = get_src2_i80(instr);
    Src80 res = get_temp_src80(instr);
    gen_alopf1_f80(&res, src1, src2, op);
    gen_al_result_i80(instr, res.lo, res.hi, res.tag);
}

static inline void gen_alopf1_xxs(Instr *instr,
    void (*op)(TCGv_env, TCGv_ptr, TCGv_ptr))
{
    Src80 src1 = get_src1_i80(instr);
    Src80 src2 = get_src2_i80(instr);
    TCGv_i32 tag = get_temp_i32(instr);
    TCGv_i32 dst = get_temp_i32(instr);
    Src80 t0 = temp_new_src80();

    gen_tag2_i32(tag, src1.tag, src2.tag);
    gen_alopf1_f80(&t0, src1, src2, op);
    gen_fxtofs(dst, t0);
    gen_al_result_i32(instr, dst, tag);

    temp_free_src80(&t0);
}

static inline void gen_alopf1_xexi(Instr *instr,
    void (*op)(TCGv_env, TCGv_ptr, TCGv_i32))
{
    Src80 src1 = get_src1_i80(instr);
    Src32 src2 = get_src2_i32(instr);
    Src80 res = get_temp_src80(instr);

    gen_alopf1_xexs_raw(&res, src1, src2, op);
    gen_al_result_i80(instr, res.lo, res.hi, res.tag);
}

static inline void gen_alopf1_xxd(Instr *instr,
    void (*op)(TCGv_env, TCGv_ptr, TCGv_ptr))
{
    Src80 src1 = get_src1_i80(instr);
    Src80 src2 = get_src2_i80(instr);
    TCGv_i32 tag = get_temp_i32(instr);
    TCGv_i64 dst = get_temp_i64(instr);
    Src80 t0 = temp_new_src80();

    gen_tag2_i64(tag, src1.tag, src2.tag);
    gen_alopf1_f80(&t0, src1, src2, op);
    gen_fxtofd(dst, t0);
    gen_al_result_i64(instr, dst, tag);

    temp_free_src80(&t0);
}

static inline void gen_alopf1_xss(Instr *instr,
    void (*op)(TCGv_env, TCGv_ptr, TCGv_ptr))
{
    Src80 src1 = get_src1_i80(instr);
    Src32 src2 = get_src2_i32(instr);
    TCGv_i32 tag = get_temp_i32(instr);
    TCGv_i32 dst = get_temp_i32(instr);
    Src80 t0 = temp_new_src80();
    Src80 t1 = temp_new_src80();

    gen_tag2_i32(tag, src1.tag, src2.tag);
    gen_fstofx(&t0, src2.value);
    gen_alopf1_f80(&t1, src1, t0, op);
    gen_fxtofs(dst, t1);
    gen_al_result_i32(instr, dst, tag);

    temp_free_src80(&t1);
    temp_free_src80(&t0);
}

static inline void gen_alopf1_xdd(Instr *instr,
    void (*op)(TCGv_env, TCGv_ptr, TCGv_ptr))
{
    Src80 src1 = get_src1_i80(instr);
    Src64 src2 = get_src2_i64(instr);
    TCGv_i32 tag = get_temp_i32(instr);
    TCGv_i64 dst = get_temp_i64(instr);
    Src80 t0 = temp_new_src80();
    Src80 t1 = temp_new_src80();

    gen_tag2_i64(tag, src1.tag, src2.tag);
    gen_fdtofx(&t0, src2.value);
    gen_alopf1_f80(&t1, src1, t0, op);
    gen_fxtofd(dst, t1);
    gen_al_result_i64(instr, dst, tag);

    temp_free_src80(&t1);
    temp_free_src80(&t0);
}

static inline void gen_alopf1_xsx(Instr *instr,
    void (*op)(TCGv_env, TCGv_ptr, TCGv_ptr))
{
    Src80 src1 = get_src1_i80(instr);
    Src32 src2 = get_src2_i32(instr);
    TCGv_i32 tag = get_temp_i32(instr);
    Src80 t0 = temp_new_src80();
    Src80 t1 = get_temp_src80(instr);

    gen_tag2_i64(tag, src1.tag, src2.tag);
    gen_fstofx(&t0, src2.value);
    gen_alopf1_f80(&t1, src1, t0, op);
    gen_al_result_i80(instr, t1.lo, t1.hi, tag);

    temp_free_src80(&t0);
}

static inline void gen_alopf1_xdx(Instr *instr,
    void (*op)(TCGv_env, TCGv_ptr, TCGv_ptr))
{
    Src80 src1 = get_src1_i80(instr);
    Src64 src2 = get_src2_i64(instr);
    TCGv_i32 tag = get_temp_i32(instr);
    Src80 t0 = temp_new_src80();
    Src80 t1 = get_temp_src80(instr);

    gen_tag2_i64(tag, src1.tag, src2.tag);
    gen_fdtofx(&t0, src2.value);
    gen_alopf1_f80(&t1, src1, t0, op);
    gen_al_result_i80(instr, t1.lo, t1.hi, tag);

    temp_free_src80(&t0);
}

static inline void gen_alopf2_xs(Instr *instr,
    void (*op)(TCGv_i32 ret, Src80 src2))
{
    Src80 src2 = get_src2_i80(instr);
    TCGv_i32 tag = get_temp_i32(instr);
    TCGv_i32 dst = get_temp_i32(instr);

    gen_tag1_i32(tag, src2.tag);
    (*op)(dst, src2);
    gen_al_result_i32(instr, dst, tag);
}

static inline void gen_alopf2_xd(Instr *instr,
    void (*op)(TCGv_i64 ret, Src80 src2))
{
    Src80 src2 = get_src2_i80(instr);
    TCGv_i32 tag = get_temp_i32(instr);
    TCGv_i64 dst = get_temp_i64(instr);

    gen_tag1_i32(tag, src2.tag);
    (*op)(dst, src2);
    gen_al_result_i64(instr, dst, tag);
}

static inline void gen_alopf2_sx(Instr *instr,
    void (*op)(Src80 *ret, TCGv_i32 src2))
{
    Src32 src2 = get_src2_i32(instr);
    Src80 res = get_temp_src80(instr);

    gen_tag1_i64(res.tag, src2.tag);
    (*op)(&res, src2.value);
    gen_al_result_i80(instr, res.lo, res.hi, res.tag);
}

static inline void gen_alopf2_dx(Instr *instr,
    void (*op)(Src80 *ret, TCGv_i64 src2))
{
    Src64 src2 = get_src2_i64(instr);
    Src80 res = get_temp_src80(instr);

    gen_tag1_i64(res.tag, src2.tag);
    (*op)(&res, src2.value);
    gen_al_result_i80(instr, res.lo, res.hi, res.tag);
}

static inline void gen_mov_f80(Src80 *ret, Src80 src2)
{
    tcg_gen_mov_i32(ret->hi, src2.hi);
    tcg_gen_mov_i64(ret->lo, src2.lo);
}

static inline void gen_alopf2_xx(Instr *instr,
    void (*op)(Src80 *ret, Src80 src2))
{
    Src80 src2 = get_src2_i80(instr);
    Src80 res = get_temp_src80(instr);

    gen_tag1_i64(res.tag, src2.tag);
    (*op)(&res, src2);
    gen_al_result_i80(instr, res.lo, res.hi, res.tag);
}

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
        // FIXME: not tested
        e2k_todo(ctx, "check_args ALOPF13");
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

static void gen_alop_simple(Instr *instr, uint32_t op)
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
    case OP_SXT: gen_sxt(ctx, instr); break;
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
    case OP_GETFS: gen_alopf1_sss(instr, gen_getf_i32); break;
    case OP_GETFD: gen_alopf1_ddd(instr, gen_getf_i64); break;
    case OP_MERGES: gen_alopf1_mrgc_sss(instr); break;
    case OP_MERGED: gen_alopf1_mrgc_ddd(instr); break;
    case OP_CMPOSB:
    case OP_CMPBSB:
    case OP_CMPESB:
    case OP_CMPBESB:
    case OP_CMPSSB:
    case OP_CMPPSB:
    case OP_CMPLSB:
    case OP_CMPLESB:
        gen_alopf1_cmp_ssb(instr, gen_cmp_i32);
        break;
    case OP_CMPODB:
    case OP_CMPBDB:
    case OP_CMPEDB:
    case OP_CMPBEDB:
    case OP_CMPSDB:
    case OP_CMPPDB:
    case OP_CMPLDB:
    case OP_CMPLEDB:
        gen_alopf1_cmp_ddb(instr, gen_cmp_i64);
        break;
    case OP_CMPANDESB:
    case OP_CMPANDSSB:
    case OP_CMPANDPSB:
    case OP_CMPANDLESB:
        gen_alopf1_cmp_ssb(instr, gen_cmpand_i32);
        break;
    case OP_CMPANDEDB:
    case OP_CMPANDSDB:
    case OP_CMPANDPDB:
    case OP_CMPANDLEDB:
        gen_alopf1_cmp_ddb(instr, gen_cmpand_i64);
        break;
    case OP_FCMPEQSB:
    case OP_FCMPLTSB:
    case OP_FCMPLESB:
    case OP_FCMPUODSB:
    case OP_FCMPNEQSB:
    case OP_FCMPNLTSB:
    case OP_FCMPNLESB:
    case OP_FCMPODSB:
        gen_alopf1_cmp_ssb(instr, gen_fcmp_i32);
        break;
    case OP_FCMPEQDB:
    case OP_FCMPLTDB:
    case OP_FCMPLEDB:
    case OP_FCMPUODDB:
    case OP_FCMPNEQDB:
    case OP_FCMPNLTDB:
    case OP_FCMPNLEDB:
    case OP_FCMPODDB:
        gen_alopf1_cmp_ddb(instr, gen_fcmp_i64);
        break;
    case OP_FXCMPEQSB:
    case OP_FXCMPLTSB:
    case OP_FXCMPLESB:
    case OP_FXCMPUODSB:
    case OP_FXCMPNEQSB:
    case OP_FXCMPNLTSB:
    case OP_FXCMPNLESB:
    case OP_FXCMPODSB:
        gen_alopf1_cmp_xs(instr);
        break;
    case OP_FXCMPEQDB:
    case OP_FXCMPLTDB:
    case OP_FXCMPLEDB:
    case OP_FXCMPUODDB:
    case OP_FXCMPNEQDB:
    case OP_FXCMPNLTDB:
    case OP_FXCMPNLEDB:
    case OP_FXCMPODDB:
        gen_alopf1_cmp_xd(instr);
        break;
    case OP_FXCMPEQXB:
    case OP_FXCMPLTXB:
    case OP_FXCMPLEXB:
    case OP_FXCMPUODXB:
    case OP_FXCMPNEQXB:
    case OP_FXCMPNLTXB:
    case OP_FXCMPNLEXB:
    case OP_FXCMPODXB:
        gen_alopf1_cmp_xx(instr);
        break;
    case OP_STB: gen_st_dds(instr, MO_UB); break;
    case OP_STH: gen_st_dds(instr, MO_UW); break;
    case OP_STW: gen_st_dds(instr, MO_UL); break;
    case OP_STD: gen_st_ddd(instr, MO_Q); break;
    case OP_LDB: gen_ld(instr, MO_UB); break;
    case OP_LDH: gen_ld(instr, MO_UW); break;
    case OP_LDW: gen_ld(instr, MO_UL); break;
    case OP_LDD: gen_ld(instr, MO_Q); break;
    case OP_BITREVS: gen_alopf2_ss(instr, gen_bitrevs); break;
    case OP_BITREVD: gen_alopf2_dd(instr, gen_bitrevd); break;
    case OP_LZCNTS: gen_alopf2_ss(instr, gen_lzcnts); break;
    case OP_LZCNTD: gen_alopf2_dd(instr, gen_lzcntd); break;
    case OP_POPCNTS: gen_alopf2_ss(instr, tcg_gen_ctpop_i32); break;
    case OP_POPCNTD: gen_alopf2_dd(instr, tcg_gen_ctpop_i64); break;
    case OP_FADDS: gen_alopf1_sess(instr, gen_helper_fadds); break;
    case OP_FADDD: gen_alopf1_dedd(instr, gen_helper_faddd); break;
    case OP_FSUBS: gen_alopf1_sess(instr, gen_helper_fsubs); break;
    case OP_FSUBD: gen_alopf1_dedd(instr, gen_helper_fsubd); break;
    case OP_FMINS: gen_alopf1_sess(instr, gen_helper_fmins); break;
    case OP_FMIND: gen_alopf1_dedd(instr, gen_helper_fmind); break;
    case OP_FMAXS: gen_alopf1_sess(instr, gen_helper_fmaxs); break;
    case OP_FMAXD: gen_alopf1_dedd(instr, gen_helper_fmaxd); break;
    case OP_FMULS: gen_alopf1_sess(instr, gen_helper_fmuls); break;
    case OP_FMULD: gen_alopf1_dedd(instr, gen_helper_fmuld); break;
    case OP_FCMPEQS: gen_alopf1_sess(instr, gen_helper_fcmpeqs); break;
    case OP_FCMPLTS: gen_alopf1_sess(instr, gen_helper_fcmplts); break;
    case OP_FCMPLES: gen_alopf1_sess(instr, gen_helper_fcmples); break;
    case OP_FCMPUODS: gen_alopf1_sess(instr, gen_helper_fcmpuods); break;
    case OP_FCMPNEQS: gen_alopf1_sess(instr, gen_helper_fcmpneqs); break;
    case OP_FCMPNLTS: gen_alopf1_sess(instr, gen_helper_fcmpnlts); break;
    case OP_FCMPNLES: gen_alopf1_sess(instr, gen_helper_fcmpnles); break;
    case OP_FCMPODS: gen_alopf1_sess(instr, gen_helper_fcmpods); break;
    case OP_FCMPEQD: gen_alopf1_dedd(instr, gen_helper_fcmpeqd); break;
    case OP_FCMPLTD: gen_alopf1_dedd(instr, gen_helper_fcmpltd); break;
    case OP_FCMPLED: gen_alopf1_dedd(instr, gen_helper_fcmpled); break;
    case OP_FCMPUODD: gen_alopf1_dedd(instr, gen_helper_fcmpuodd); break;
    case OP_FCMPNEQD: gen_alopf1_dedd(instr, gen_helper_fcmpneqd); break;
    case OP_FCMPNLTD: gen_alopf1_dedd(instr, gen_helper_fcmpnltd); break;
    case OP_FCMPNLED: gen_alopf1_dedd(instr, gen_helper_fcmpnled); break;
    case OP_FCMPODD: gen_alopf1_dedd(instr, gen_helper_fcmpodd); break;
    case OP_FSTOIS: gen_alopf2_ses(instr, gen_helper_fstois); break;
    case OP_FSTOISTR: gen_alopf2_ses(instr, gen_helper_fstoistr); break;
    case OP_ISTOFS: gen_alopf2_ses(instr, gen_helper_istofs); break;
    case OP_FDTOID: gen_alopf2_ded(instr, gen_helper_fdtoid); break;
    case OP_IDTOFD: gen_alopf2_ded(instr, gen_helper_idtofd); break;
    case OP_FXTOFD: gen_alopf2_xd(instr, gen_fxtofd); break;
    case OP_FDTOFX: gen_alopf2_dx(instr, gen_fdtofx); break;
    case OP_FSTOID: gen_alopf2_des(instr, gen_helper_fstoid); break;
    case OP_FSTOIDTR: gen_alopf2_des(instr, gen_helper_fstoidtr); break;
    case OP_FDTOIDTR: gen_alopf2_ded(instr, gen_helper_fdtoidtr); break;
    case OP_ISTOFD: gen_alopf2_des(instr, gen_helper_istofd); break;
    case OP_FSTOFD: gen_alopf2_des(instr, gen_helper_fstofd); break;
    case OP_FSTOFX: gen_alopf2_sx(instr, gen_fstofx); break;
    case OP_FDTOISTR: gen_alopf2_sed(instr, gen_helper_fdtoistr); break;
    case OP_FDTOIS: gen_alopf2_sed(instr, gen_helper_fdtois); break;
    case OP_IDTOFS: gen_alopf2_sed(instr, gen_helper_idtofs); break;
    case OP_FDTOFS: gen_alopf2_sed(instr, gen_helper_fdtofs); break;
    case OP_FXTOFS: gen_alopf2_xs(instr, gen_fxtofs); break;
    case OP_FXTOIS: gen_alopf2_xs(instr, gen_fxtois); break;
    case OP_FXTOISTR: gen_alopf2_xs(instr, gen_fxtoistr); break;
    case OP_FXTOID: gen_alopf2_xd(instr, gen_fxtoid); break;
    case OP_FXTOIDTR: gen_alopf2_xd(instr, gen_fxtoidtr); break;
    case OP_ISTOFX: gen_alopf2_sx(instr, gen_istofx); break;
    case OP_IDTOFX: gen_alopf2_dx(instr, gen_idtofx); break;
    case OP_UDIVS:
        if (instr->src2 == 0xc0) {
            // FIXME: temp hack
            e2k_tr_gen_exception_no_spill(ctx, 0);
            return;
        }
        gen_alopf1_sttss(instr, gen_udivs);
        break;
    case OP_UDIVD: gen_alopf1_dttdd(instr, gen_udivd); break;
    case OP_SDIVS: gen_alopf1_sttss(instr, gen_sdivs); break;
    case OP_SDIVD: gen_alopf1_dttdd(instr, gen_sdivd); break;
    case OP_FXADDSS: gen_alopf1_xss(instr, gen_helper_fxaddxx); break;
    case OP_FXADDDD: gen_alopf1_xdd(instr, gen_helper_fxaddxx); break;
    case OP_FXADDSX: gen_alopf1_xsx(instr, gen_helper_fxaddxx); break;
    case OP_FXADDDX: gen_alopf1_xdx(instr, gen_helper_fxaddxx); break;
    case OP_FXADDXX: gen_alopf1_xxx(instr, gen_helper_fxaddxx); break;
    case OP_FXADDXD: gen_alopf1_xxd(instr, gen_helper_fxaddxx); break;
    case OP_FXADDXS: gen_alopf1_xxs(instr, gen_helper_fxaddxx); break;
    case OP_FXSUBSS: gen_alopf1_xss(instr, gen_helper_fxsubxx); break;
    case OP_FXSUBDD: gen_alopf1_xdd(instr, gen_helper_fxsubxx); break;
    case OP_FXSUBSX: gen_alopf1_xsx(instr, gen_helper_fxsubxx); break;
    case OP_FXSUBDX: gen_alopf1_xdx(instr, gen_helper_fxsubxx); break;
    case OP_FXSUBXX: gen_alopf1_xxx(instr, gen_helper_fxsubxx); break;
    case OP_FXSUBXD: gen_alopf1_xxd(instr, gen_helper_fxsubxx); break;
    case OP_FXSUBXS: gen_alopf1_xxs(instr, gen_helper_fxsubxx); break;
    case OP_FXRSUBSS: gen_alopf1_xss(instr, gen_helper_fxrsubxx); break;
    case OP_FXRSUBDD: gen_alopf1_xdd(instr, gen_helper_fxrsubxx); break;
    case OP_FXRSUBSX: gen_alopf1_xsx(instr, gen_helper_fxrsubxx); break;
    case OP_FXRSUBDX: gen_alopf1_xdx(instr, gen_helper_fxrsubxx); break;
    case OP_FXMULSS: gen_alopf1_xss(instr, gen_helper_fxmulxx); break;
    case OP_FXMULDD: gen_alopf1_xdd(instr, gen_helper_fxmulxx); break;
    case OP_FXMULSX: gen_alopf1_xsx(instr, gen_helper_fxmulxx); break;
    case OP_FXMULDX: gen_alopf1_xdx(instr, gen_helper_fxmulxx); break;
    case OP_FXMULXX: gen_alopf1_xxx(instr, gen_helper_fxmulxx); break;
    case OP_FXMULXD: gen_alopf1_xxd(instr, gen_helper_fxmulxx); break;
    case OP_FXMULXS: gen_alopf1_xxs(instr, gen_helper_fxmulxx); break;
    case OP_FXDIVSS: gen_alopf1_xss(instr, gen_helper_fxdivxx); break;
    case OP_FXDIVDD: gen_alopf1_xdd(instr, gen_helper_fxdivxx); break;
    case OP_FXDIVSX: gen_alopf1_xsx(instr, gen_helper_fxdivxx); break;
    case OP_FXDIVDX: gen_alopf1_xdx(instr, gen_helper_fxdivxx); break;
    case OP_FXDIVXX: gen_alopf1_xxx(instr, gen_helper_fxdivxx); break;
    case OP_FXDIVXD: gen_alopf1_xxd(instr, gen_helper_fxdivxx); break;
    case OP_FXDIVXS: gen_alopf1_xxs(instr, gen_helper_fxdivxx); break;
    case OP_MOVFI: gen_movfi(instr); break;
    case OP_MOVIF: gen_movif(instr); break;
    case OP_MOVX: gen_movx(instr); break;
    case OP_MOVXA: gen_movxa(instr); break;
    case OP_MOVXC: gen_movxc(instr); break;
    case OP_MOVTS: gen_movts(instr); break;
    case OP_MOVTCS: gen_movtcs(instr); break;
    case OP_MOVTD: gen_movtd(instr); break;
    case OP_MOVTCD: gen_movtcd(instr); break;
    case OP_MOVTQ: gen_movtq(instr); break;
    case OP_MOVTCQ: gen_movtcq(instr); break;
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
    case OP_PSLLD: gen_alopf1_ddd(instr, tcg_gen_shl_i64); break;
    case OP_PSRLH: gen_alopf1_ddd(instr, gen_helper_psrlh); break;
    case OP_PSRLW: gen_alopf1_ddd(instr, gen_helper_psrlw); break;
    case OP_PSRLD: gen_alopf1_ddd(instr, tcg_gen_shr_i64); break;
    case OP_PSRAH: gen_alopf1_ddd(instr, gen_helper_psrah); break;
    case OP_PSRAW: gen_alopf1_ddd(instr, gen_helper_psraw); break;
    case OP_PAVGUSB: gen_alopf1_ddd(instr, gen_helper_pavgusb); break;
    case OP_PAVGUSH: gen_alopf1_ddd(instr, gen_helper_pavgush); break;
    case OP_PSLLQL: gen_alopf11_dddi(instr, gen_psllql); break;
    case OP_PSLLQH: gen_alopf11_dddi(instr, gen_psllqh); break;
    case OP_PSRLQL: gen_alopf11_dddi(instr, gen_psrlql); break;
    case OP_PSRLQH: gen_alopf11_dddi(instr, gen_psrlqh); break;
    case OP_PINSH: gen_alopf11_dddi(instr, gen_pinsh); break;
    case OP_PEXTRH: gen_alopf11_dddi(instr, gen_pextrh); break;
    case OP_PSHUFH: gen_pshufh(instr); break;
    case OP_PSHUFW: gen_alopf11_dddi(instr, gen_pshufw); break;
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
    case OP_GETTAGS: gen_gettag_i32(instr); break;
    case OP_GETTAGD: gen_gettag_i64(instr); break;
    case OP_PUTTAGS: gen_puttag_i32(instr); break;
    case OP_PUTTAGD: gen_puttag_i64(instr); break;
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
            e2k_tr_gen_exception(ctx, E2K_EXCP_ILLOPC);
            return;
        }
        gen_staa_i64(instr);
        break;
    }
    case OP_MULS: gen_alopf1_sss(instr, tcg_gen_mul_i32); break;
    case OP_MULD: gen_alopf1_ddd(instr, tcg_gen_mul_i64); break;
    case OP_UMULX: gen_alopf1_dss(instr, gen_umulx); break;
    case OP_SMULX: gen_alopf1_dss(instr, gen_smulx); break;
    case OP_RWS: gen_rw_i32(instr); break;
    case OP_RWD: gen_rw_i64(instr); break;
    case OP_RRS: gen_rr_i32(instr); break;
    case OP_RRD: gen_rr_i64(instr); break;
    case OP_FDIVS: gen_alopf1_sess(instr, gen_helper_fdivs); break;
    case OP_FDIVD: gen_alopf1_dedd(instr, gen_helper_fdivd); break;
    case OP_GETSP: gen_getsp(ctx, instr); break;
    case OP_UMULHD: gen_alopf1_ddd(instr, gen_umulhd); break;
    case OP_SMULHD: gen_alopf1_ddd(instr, gen_smulhd); break;
    case OP_FCMPODSF: gen_alopf1_sess(instr, gen_helper_fcmpodsf); break;
    case OP_FCMPUDSF: gen_alopf1_sess(instr, gen_helper_fcmpudsf); break;
    case OP_FCMPODDF: gen_alopf1_sedd(instr, gen_helper_fcmpoddf); break;
    case OP_FCMPUDDF: gen_alopf1_sedd(instr, gen_helper_fcmpoddf); break;
    case OP_FXCMPODSF: gen_alopf1_sexs(instr, gen_helper_fxcmpodxf); break;
    case OP_FXCMPUDSF: gen_alopf1_sexs(instr, gen_helper_fxcmpudxf); break;
    case OP_FXCMPODDF: gen_alopf1_sexd(instr, gen_helper_fxcmpodxf); break;
    case OP_FXCMPUDDF: gen_alopf1_sexd(instr, gen_helper_fxcmpudxf); break;
    case OP_FXCMPODXF: gen_alopf1_sexx(instr, gen_helper_fxcmpodxf); break;
    case OP_FXCMPUDXF: gen_alopf1_sexx(instr, gen_helper_fxcmpudxf); break;
    case OP_FSTOIFS: gen_alopf1_sess(instr, gen_helper_fstoifs); break;
    case OP_FDTOIFD: gen_alopf1_dedd(instr, gen_helper_fdtoifd); break;
    case OP_UDIVX: gen_alopf1_sttds(instr, gen_udivx); break;
    case OP_UMODX: gen_alopf1_sttds(instr, gen_umodx); break;
    case OP_SDIVX: gen_alopf1_sttds(instr, gen_sdivx); break;
    case OP_SMODX: gen_alopf1_sttds(instr, gen_smodx); break;
    case OP_PFMULD: gen_alopf1_dedd(instr, gen_helper_fmuld); break;
    case OP_PFADDD: gen_alopf1_dedd(instr, gen_helper_faddd); break;
    case OP_PFSUBD: gen_alopf1_dedd(instr, gen_helper_fsubd); break;
    case OP_PFDIVD: gen_alopf1_dedd(instr, gen_helper_fdivd); break;
    case OP_PFMIND: gen_alopf1_dedd(instr, gen_helper_fmind); break;
    case OP_PFMAXD: gen_alopf1_dedd(instr, gen_helper_fmaxd); break;
    case OP_PFADDS: gen_alopf1_dedd(instr, gen_helper_pfadds); break;
    case OP_PFSUBS: gen_alopf1_dedd(instr, gen_helper_pfsubs); break;
    case OP_PFMULS: gen_alopf1_dedd(instr, gen_helper_pfmuls); break;
    case OP_PFDIVS: gen_alopf1_sess(instr, gen_helper_fdivs); break;
    case OP_PFMAXS: gen_alopf1_dedd(instr, gen_helper_pfmaxs); break;
    case OP_PFMINS: gen_alopf1_dedd(instr, gen_helper_pfmins); break;
    case OP_PFHADDS: gen_alopf1_dedd(instr, gen_helper_pfhadds); break;
    case OP_PFHSUBS: gen_alopf1_dedd(instr, gen_helper_pfhsubs); break;
    case OP_PFADDSUBS: gen_alopf1_dedd(instr, gen_helper_pfaddsubs); break;
    case OP_PFSQRTS: gen_alopf2_ses(instr, gen_helper_fsqrts); break;
    case OP_PFSTOIFS: gen_alopf1_dedd(instr, gen_helper_pfstoifs); break;
    case OP_PISTOFS: gen_alopf2_ded(instr, gen_helper_pistofs); break;
    case OP_PFSTOIS: gen_alopf2_ded(instr, gen_helper_pfstois); break;
    case OP_PFSTOISTR: gen_alopf2_ded(instr, gen_helper_pfstoistr); break;
    case OP_PFSTOFD: gen_alopf2_des(instr, gen_helper_fstofd); break;
    case OP_PFDTOFS: gen_alopf2_sed(instr, gen_helper_fdtofs); break;
    case OP_PFDTOIFD: gen_alopf1_dedd(instr, gen_helper_fdtoifd); break;
    case OP_PFDTOIS: gen_alopf2_sed(instr, gen_helper_fdtois); break;
    case OP_PFDTOISTR: gen_alopf2_sed(instr, gen_helper_fdtoistr); break;
    case OP_PFCMPEQS: gen_alopf1_dedd(instr, gen_helper_pfcmpeqs); break;
    case OP_PFCMPLTS: gen_alopf1_dedd(instr, gen_helper_pfcmplts); break;
    case OP_PFCMPLES: gen_alopf1_dedd(instr, gen_helper_pfcmples); break;
    case OP_PFCMPUODS: gen_alopf1_dedd(instr, gen_helper_pfcmpuods); break;
    case OP_PFCMPNEQS: gen_alopf1_dedd(instr, gen_helper_pfcmpneqs); break;
    case OP_PFCMPNLTS: gen_alopf1_dedd(instr, gen_helper_pfcmpnlts); break;
    case OP_PFCMPNLES: gen_alopf1_dedd(instr, gen_helper_pfcmpnles); break;
    case OP_PFCMPODS: gen_alopf1_dedd(instr, gen_helper_pfcmpods); break;
    case OP_PFCMPEQD: gen_alopf1_dedd(instr, gen_helper_fcmpeqd); break;
    case OP_PFCMPLTD: gen_alopf1_dedd(instr, gen_helper_fcmpltd); break;
    case OP_PFCMPLED: gen_alopf1_dedd(instr, gen_helper_fcmpled); break;
    case OP_PFCMPUODD: gen_alopf1_dedd(instr, gen_helper_fcmpuodd); break;
    case OP_PFCMPNEQD: gen_alopf1_dedd(instr, gen_helper_fcmpneqd); break;
    case OP_PFCMPNLTD: gen_alopf1_dedd(instr, gen_helper_fcmpnltd); break;
    case OP_PFCMPNLED: gen_alopf1_dedd(instr, gen_helper_fcmpnled); break;
    case OP_PFCMPODD: gen_alopf1_dedd(instr, gen_helper_fcmpodd); break;
    case OP_FSCALED: gen_alopf1_deds(instr, gen_helper_fscaled); break;
    case OP_FSCALES: gen_alopf1_sess(instr, gen_helper_fscales); break;
    case OP_FXSCALESX: gen_alopf1_xexi(instr, gen_helper_fxscalesx); break;
    case OP_FRCPS: gen_alopf2_ses(instr, gen_helper_frcps); break;
    case OP_FSQRTS: gen_alopf2_ses(instr, gen_helper_fsqrts); break;
    case OP_FRSQRTS: gen_alopf2_ses(instr, gen_helper_frsqrts); break;
#ifndef TARGET_E2K_PRECISE_FSQRTID
    case OP_FSQRTID: gen_alopf2_dd(instr, tcg_gen_mov_i64); break;
    case OP_FXSQRTISX: gen_alopf2_sx(instr, gen_fstofx); break;
    case OP_FXSQRTIDX: gen_alopf2_dx(instr, gen_fdtofx); break;
    case OP_FXSQRTIXX: gen_alopf2_xx(instr, gen_mov_f80); break;
    /* FIXME: these are not ALOPF2! */
    case OP_FXSQRTUSX: /* fallthrough */
    case OP_FXSQRTUDX: /* fallthrough */
    case OP_FXSQRTUXX: gen_alopf2_xx(instr, gen_mov_f80); break;
#else
#error Not implemented
#endif
    case OP_PFSQRTTD: /* fallthrough */
    case OP_FSQRTTD: gen_alopf1_dedd(instr, gen_helper_fsqrttd); break;
    case OP_FXSQRTTSX: gen_alopf1_xsx(instr, gen_helper_fxsqrttxx); break;
    case OP_FXSQRTTDX: gen_alopf1_xdx(instr, gen_helper_fxsqrttxx); break;
    case OP_FXSQRTTXX: gen_alopf1_xxx(instr, gen_helper_fxsqrttxx); break;
    case OP_INSFS: gen_alopf21_i32(instr, gen_insfs); break;
    case OP_INSFD: gen_insfd(instr); break;
    case OP_PSHUFB: gen_alopf21_i64(instr, gen_helper_pshufb); break;
    case OP_PMERGE: gen_alopf21_i64(instr, gen_helper_pmerge); break;
    case OP_FXDIVTSS:
    case OP_FXDIVTDD:
    case OP_FXDIVTSX:
    case OP_FXDIVTDX:
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
    case OP_GETPL:
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
    case OP_LDGDB:
    case OP_LDGDH:
    case OP_LDGDW:
    case OP_LDGDD:
    case OP_LDGDQ:
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
    case OP_STGDB:
    case OP_STGDH:
    case OP_STGDW:
    case OP_STGDD:
    case OP_STGDQ:
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
        e2k_todo_illop(ctx, "unimplemented %d", op); break;
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

static void pfcomb_init(DisasContext *ctx)
{
    pfcomb_map[FCOMB_MUL][FCOMB_ADD] = true;
    pfcomb_map[FCOMB_MUL][FCOMB_SUB] = true;
    pfcomb_map[FCOMB_MUL][FCOMB_RSUB] = true;

    if (ctx->version == 1) {
        pfcomb_map[FCOMB_ADD][FCOMB_MUL] = true;
        pfcomb_map[FCOMB_SUB][FCOMB_MUL] = true;
    }

    if (ctx->version >= 2) {
        pfcomb_map[FCOMB_ADD][FCOMB_ADD] = true;
        pfcomb_map[FCOMB_ADD][FCOMB_SUB] = true;
        pfcomb_map[FCOMB_ADD][FCOMB_RSUB] = true;

        pfcomb_map[FCOMB_SUB][FCOMB_ADD] = true;
        pfcomb_map[FCOMB_SUB][FCOMB_SUB] = true;
        pfcomb_map[FCOMB_SUB][FCOMB_RSUB] = true;
    }

    if (ctx->version >= 3) {
        pfcomb_map[FCOMB_HADD][FCOMB_ADD] = true;
        pfcomb_map[FCOMB_HADD][FCOMB_SUB] = true;
        pfcomb_map[FCOMB_HADD][FCOMB_RSUB] = true;
        pfcomb_map[FCOMB_HADD][FCOMB_HADD] = true;
        pfcomb_map[FCOMB_HADD][FCOMB_HSUB] = true;
        pfcomb_map[FCOMB_HADD][FCOMB_ADDSUB] = true;

        pfcomb_map[FCOMB_HSUB][FCOMB_ADD] = true;
        pfcomb_map[FCOMB_HSUB][FCOMB_SUB] = true;
        pfcomb_map[FCOMB_HSUB][FCOMB_RSUB] = true;
        pfcomb_map[FCOMB_HSUB][FCOMB_HADD] = true;
        pfcomb_map[FCOMB_HSUB][FCOMB_HSUB] = true;
        pfcomb_map[FCOMB_HSUB][FCOMB_ADDSUB] = true;

        pfcomb_map[FCOMB_ADDSUB][FCOMB_ADD] = true;
        pfcomb_map[FCOMB_ADDSUB][FCOMB_SUB] = true;
        pfcomb_map[FCOMB_ADDSUB][FCOMB_RSUB] = true;
        pfcomb_map[FCOMB_ADDSUB][FCOMB_HADD] = true;
        pfcomb_map[FCOMB_ADDSUB][FCOMB_HSUB] = true;
        pfcomb_map[FCOMB_ADDSUB][FCOMB_ADDSUB] = true;

        pfcomb_map[FCOMB_ADD][FCOMB_HADD] = true;
        pfcomb_map[FCOMB_ADD][FCOMB_HSUB] = true;
        pfcomb_map[FCOMB_ADD][FCOMB_ADDSUB] = true;

        pfcomb_map[FCOMB_SUB][FCOMB_HADD] = true;
        pfcomb_map[FCOMB_SUB][FCOMB_HSUB] = true;
        pfcomb_map[FCOMB_SUB][FCOMB_ADDSUB] = true;

        pfcomb_map[FCOMB_MUL][FCOMB_HADD] = true;
        pfcomb_map[FCOMB_MUL][FCOMB_HSUB] = true;
        pfcomb_map[FCOMB_MUL][FCOMB_ADDSUB] = true;
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
        default: e2k_tr_gen_exception(instr->ctx, E2K_EXCP_ILLOPC); break; \
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
    default: e2k_tr_gen_exception(instr->ctx, E2K_EXCP_ILLOPC); break;
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
    default: e2k_tr_gen_exception(instr->ctx, E2K_EXCP_ILLOPC); break;
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

#define IMPL_GEN_COMB(NAME, P, S, T, OP) \
    static void NAME(Instr *instr, int opc1, int opc2) \
    { \
        glue(Src, T) s1 = glue(get_src1_, S)(instr); \
        glue(Src, T) s2 = glue(get_src2_, S)(instr); \
        glue(Src, T) s3 = glue(get_src3_, S)(instr); \
        TCGv_i32 tag = get_temp_i32(instr); \
        glue(TCGv_, S) dst = glue(get_temp_, S)(instr); \
        glue(gen_tag3_, S)(tag, s1.tag, s2.tag, s3.tag); \
        OP(instr, opc1, dst, s1.value, s2.value); \
        OP(instr, opc2, dst, s3.value, dst); \
        glue(gen_al_result_, S)(instr, dst, tag); \
    }

IMPL_GEN_COMB(gen_icomb_i64, icomb, i64, 64, gen_icomb_op_i64)
IMPL_GEN_COMB(gen_icomb_i32, icomb, i32, 32, gen_icomb_op_i32)
IMPL_GEN_COMB(gen_fcomb_i64, fcomb, i64, 64, gen_fcomb_op_i64)
IMPL_GEN_COMB(gen_fcomb_i32, fcomb, i32, 32, gen_fcomb_op_i32)
IMPL_GEN_COMB(gen_pfcomb_i64, pfcomb, i64, 64, gen_pfcomb_op_i64)
IMPL_GEN_COMB(gen_pfcomb_i32, pfcomb, i64, 64, gen_pfcomb_op_i32)

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
    Src64 s1 = get_src1_i64(instr);
    Src64 s2 = get_src2_i64(instr);
    Src64 s3 = get_src3_i64(instr);
    TCGv_i32 tag = get_temp_i32(instr);
    TCGv_i64 dst = get_temp_i64(instr);
    TCGv_i32 opc = tcg_const_i32(base + instr->opc1);

    check_args(ALOPF21, instr);
    gen_tag3_i64(tag, s1.tag, s2.tag, s3.tag);
    gen_helper_plog(dst, opc, s1.value, s2.value, s3.value);
    gen_al_result_i64(instr, dst, tag);
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
    unsigned int i;
    bool has_pcnt = false;
    bool has_preg = false;
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i32 t1 = tcg_temp_new_i32();
    TCGv_i32 t2 = tcg_temp_new_i32();

    tcg_gen_movi_i32(t0, 0);
    tcg_gen_movi_i32(t1, 0);

    for (i = 0; i < 3; i++) {
        unsigned int j;
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
                tcg_gen_setcondi_i32(TCG_COND_LEU, t2, e2k_cs.lsr_pcnt, idx);
                if (invert) {
                    tcg_gen_xori_i32(t2, t2, 1);
                }
                tcg_gen_or_i32(t0, t0, t2);
                break;
            }
            case 0x3: /* %predN */
                has_preg = true;
                e2k_gen_preg_i32(t2, idx);
                if (invert) {
                    tcg_gen_xori_i32(t2, t2, 1);
                }
                tcg_gen_or_i32(t1, t1, t2);
                break;
            default:
                if (ctx->strict) {
                    e2k_tr_gen_exception(ctx, E2K_EXCP_ILLOPN);
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

static inline void alop_instr_init(Instr *instr, DisasContext *ctx, int chan)
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
            e2k_tr_gen_exception(instr->ctx, E2K_EXCP_ILLOPC);
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
                e2k_tr_gen_exception(instr->ctx, E2K_EXCP_ILLOPC);
                return;
            }
            alop->format = ALOPF21;
            alop->op = instr->opc1 & 1 ? OP_INSFD : OP_INSFS;
        } else {
            int opc1 = icomb_opc1(instr);
            int opc2 = icomb_opc2(instr);
            if (!icomb_check(instr, opc1, opc2)) {
                e2k_tr_gen_exception(instr->ctx, E2K_EXCP_ILLOPC);
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
            e2k_tr_gen_exception(instr->ctx, E2K_EXCP_ILLOPC);
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
                e2k_tr_gen_exception(instr->ctx, E2K_EXCP_ILLOPC);
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
            e2k_tr_gen_exception(instr->ctx, E2K_EXCP_ILLOPC);
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
        e2k_tr_gen_exception(instr->ctx, E2K_EXCP_ILLOPC);
        break;
    }
}

void e2k_alc_decode(DisasContext *ctx)
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
        gen_alop_simple(instr, alop->op);
        break;
    }

    gen_set_label(l0);

    if (instr->aaincr_len != 0) {
        gen_aasti_incr(instr->ctx, instr);
    }
}

static void gen_alops(DisasContext *ctx)
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

void e2k_alc_execute(DisasContext *ctx)
{
    gen_alops(ctx);
}

static inline void gen_al_result_commit_reg32(bool poison, TCGv_i32 index,
    TCGv_i32 tag, TCGv_i32 value)
{
    TCGv_i32 t0 = tcg_temp_new_i32();

    e2k_gen_reg_tag_write_i32(tag, index);
    if (poison) {
        gen_dst_poison_i32(t0, value, tag);
    } else {
        tcg_gen_mov_i32(t0, value);
    }
    e2k_gen_reg_write_i32(t0, index);

    tcg_temp_free_i32(t0);
}

static inline void gen_al_result_commit_reg64(bool poison, TCGv_i32 index,
    TCGv_i32 tag, TCGv_i64 value)
{
    TCGv_i64 t0 = tcg_temp_new_i64();

    e2k_gen_reg_tag_write_i64(tag, index);
    if (poison) {
        gen_dst_poison_i64(t0, value, tag);
    } else {
        tcg_gen_mov_i64(t0, value);
    }
    e2k_gen_reg_write_i64(t0, index);

    tcg_temp_free_i64(t0);
}

static inline void gen_al_result_commit_reg(AlResult *res)
{
    AlResultType size = e2k_al_result_size(res->type);

    switch (size) {
    case AL_RESULT_32: {
        TCGLabel *l0 = gen_new_label();

        if (res->dbl) {
            tcg_gen_brcondi_i32(TCG_COND_NE, e2k_cs.wdbl, 0, l0);
            /* wdbl is not set */
        }

        gen_al_result_commit_reg32(res->poison, res->reg.index, res->reg.tag,
            res->reg.v32);

        if (res->dbl) {
            TCGv_i32 t0 = tcg_temp_new_i32();
            TCGv_i64 t1 = tcg_temp_new_i64();
            TCGLabel *l1 = gen_new_label();
            tcg_gen_br(l1);

            /* wdbl is set */
            gen_set_label(l0);
            if (res->check_tag) {
                gen_tag1_i64(t0, res->reg.tag);
            } else {
                tcg_gen_mov_i32(t0, res->reg.tag);
            }
            tcg_gen_extu_i32_i64(t1, res->reg.v32);
            gen_al_result_commit_reg64(res->poison, res->reg.index, t0, t1);

            /* exit */
            gen_set_label(l1);
            tcg_temp_free_i64(t1);
            tcg_temp_free_i32(t0);
        }

        break;
    }
    case AL_RESULT_64:
        gen_al_result_commit_reg64(res->poison, res->reg.index, res->reg.tag,
            res->reg.v64);
        break;
    case AL_RESULT_80:
        gen_al_result_commit_reg64(res->poison, res->reg.index, res->reg.tag,
            res->reg.v64);
        e2k_gen_xreg_write16u_i32(res->reg.x32, res->reg.index);
        break;
    case AL_RESULT_128:
        gen_al_result_commit_reg64(res->poison, res->reg.index, res->reg.tag,
            res->reg.v64);
        e2k_gen_xreg_write_i64(res->reg.x64, res->reg.index);
        break;
    default:
        g_assert_not_reached();
        break;
    }
}

static inline void gen_al_result_commit_ctpr(AlResult *res)
{
    AlResultType size = e2k_al_result_size(res->type);
    TCGv_i64 ctpr = e2k_cs.ctprs[res->ctpr.index];
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

void e2k_alc_commit(DisasContext *ctx)
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
            gen_al_result_commit_reg(res);
            break;
        case AL_RESULT_PREG:
            /* %predN */
            e2k_gen_store_preg(res->preg.index, res->preg.val);
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


void alc_init(DisasContext *ctx)
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
