#include "qemu/osdep.h"
#include "qemu.h"
#include "exec/log.h"
#include "translate.h"

static void gen_preg_offset(TCGv_i64 ret, int reg)
{
    assert(reg < 32);

    TCGv_i32 zero = tcg_const_i32(0);
    TCGv_i32 pf_size = tcg_const_i32(PF_SIZE);
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i32 t1 = tcg_temp_new_i32();
    TCGv_i32 t2 = tcg_temp_new_i32();
    TCGv_i32 t3 = tcg_temp_new_i32();

    tcg_gen_addi_i32(t0, e2k_cs.pcur, reg);
    tcg_gen_movcond_i32(TCG_COND_NE, t1, e2k_cs.psize, zero,
        e2k_cs.psize, pf_size);
    tcg_gen_remu_i32(t2, t0, t1);
    tcg_gen_muli_i32(t3, t2, 2);
    tcg_gen_extu_i32_i64(ret, t3);

    tcg_temp_free_i32(t3);
    tcg_temp_free_i32(t2);
    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
    tcg_temp_free_i32(pf_size);
    tcg_temp_free_i32(zero);
}

static void gen_preg_clear(TCGv_i64 ret, TCGv_i64 offset)
{
    TCGv_i64 t0 = tcg_const_i64(3);
    TCGv_i64 t1 = tcg_temp_new_i64();
    TCGv_i64 t2 = tcg_temp_new_i64();

    tcg_gen_shl_i64(t1, t0, offset);
    tcg_gen_not_i64(t2, t1);
    tcg_gen_and_i64(ret, t2, e2k_cs.pregs);

    tcg_temp_free_i64(t2);
    tcg_temp_free_i64(t1);
    tcg_temp_free_i64(t0);
}

void e2k_gen_preg(TCGv_i64 ret, int reg)
{
    TCGv_i64 one = tcg_const_i64(1);
    TCGv_i64 t0 = tcg_temp_new_i64();
    TCGv_i64 t1 = tcg_temp_new_i64();
    TCGv_i64 t2 = tcg_temp_new_i64();

    gen_preg_offset(t0, reg);
    tcg_gen_shl_i64(t1, one, t0);
    tcg_gen_and_i64(t2, e2k_cs.pregs, t1);
    tcg_gen_setcondi_i64(TCG_COND_NE, ret, t2, 0);

    tcg_temp_free_i64(t2);
    tcg_temp_free_i64(t1);
    tcg_temp_free_i64(t0);
    tcg_temp_free_i64(one);
}

TCGv_i64 e2k_get_preg(DisasContext *dc, int reg)
{
    TCGv_i64 ret = e2k_get_temp_i64(dc);
    e2k_gen_preg(ret, reg);
    return ret;
}

void e2k_gen_store_preg(int reg, TCGv_i64 val)
{
    TCGv_i64 t0 = tcg_temp_new_i64();
    TCGv_i64 t1 = tcg_temp_new_i64();
    TCGv_i64 t2 = tcg_temp_new_i64();
    TCGv_i64 t3 = tcg_temp_new_i64();

    gen_preg_offset(t0, reg);
    gen_preg_clear(t1, t0);
    tcg_gen_setcondi_i64(TCG_COND_NE, t2, val, 0);
    tcg_gen_shl_i64(t3, t2, t0);
    tcg_gen_or_i64(e2k_cs.pregs, t1, t3);

    tcg_temp_free_i64(t3);
    tcg_temp_free_i64(t2);
    tcg_temp_free_i64(t1);
    tcg_temp_free_i64(t0);
}

static inline void gen_wreg_index(TCGv_i32 ret, TCGv_i32 reg)
{
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i32 t1 = tcg_const_i32(WREGS_SIZE);

    tcg_gen_add_i32(t0, e2k_cs.wd_base, reg);
    tcg_gen_remu_i32(ret, t0, t1);

    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
}

static inline void gen_wreg_ptr(TCGv_ptr ret, TCGv_i32 reg)
{
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i32 t1 = tcg_temp_new_i32();
    TCGv_ptr t2 = tcg_temp_new_ptr();

    gen_wreg_index(t0, reg);
    tcg_gen_muli_i32(t1, t0, 8);
    tcg_gen_ext_i32_ptr(t2, t1);
    tcg_gen_add_ptr(ret, e2k_cs.wptr, t2);

    tcg_temp_free_ptr(t2);
    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
}

static inline void gen_wregi_ptr(TCGv_ptr ret, int reg)
{
    TCGv_i32 t0 = tcg_const_i32(reg);

    gen_wreg_ptr(ret, t0);

    tcg_temp_free_i32(t0);
}

static inline void gen_wreg_check(DisasContext *ctx, int reg)
{
    TCGLabel *l0 = gen_new_label();

    assert(reg < 64);

    tcg_gen_brcondi_i32(TCG_COND_GTU, e2k_cs.wd_size, reg, l0);
    e2k_gen_exception(ctx, E2K_EXCP_MAPERR);
    gen_set_label(l0);
}

TCGv_i64 e2k_get_wreg(DisasContext *ctx, int reg)
{
    TCGv_ptr t0 = tcg_temp_new_ptr();
    TCGv_i64 t1 = e2k_get_temp_i64(ctx);

    gen_wreg_check(ctx, reg);
    gen_wregi_ptr(t0, reg);
    tcg_gen_ld_i64(t1, t0, 0);

    tcg_temp_free_ptr(t0);

    return t1;
}

void e2k_gen_store_wreg(DisasContext *ctx, int reg, TCGv_i64 val)
{
    TCGv_ptr t0 = tcg_temp_new_ptr();

    gen_wreg_check(ctx, reg);
    gen_wregi_ptr(t0, reg);
    tcg_gen_st_i64(val, t0, 0);

    tcg_temp_free_ptr(t0);
}

static inline void gen_breg_index(TCGv_i32 ret, int idx)
{
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i32 t1 = tcg_temp_new_i32();

    tcg_gen_addi_i32(t0, e2k_cs.bcur, idx);
    tcg_gen_remu_i32(t1, t0, e2k_cs.bsize);
    tcg_gen_add_i32(ret, t1, e2k_cs.boff);

    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
}

static inline void gen_bregi_ptr(DisasContext *ctx, TCGv_ptr ret, int idx)
{
    TCGLabel *l0 = gen_new_label();
    TCGLabel *l1 = gen_new_label();
    TCGv_i32 t0 = tcg_temp_new_i32();

    assert(idx < 128);

    tcg_gen_brcondi_i32(TCG_COND_LEU, e2k_cs.bsize, idx, l0);
    tcg_gen_add_i32(t0, e2k_cs.boff, e2k_cs.bsize);
    tcg_gen_brcond_i32(TCG_COND_LTU, e2k_cs.wd_size, t0, l0);
    tcg_gen_br(l1);
    gen_set_label(l0);
    e2k_gen_exception(ctx, E2K_EXCP_MAPERR);
    gen_set_label(l1);

    gen_breg_index(t0, idx);
    gen_wreg_ptr(ret, t0);

    tcg_temp_free_i32(t0);
}

TCGv_i64 e2k_get_breg(DisasContext *ctx, int reg)
{
    TCGv_i64 ret = e2k_get_temp(ctx);
    TCGv_ptr ptr = tcg_temp_new_ptr();
    gen_bregi_ptr(ctx, ptr, reg);
    tcg_gen_ld_i64(ret, ptr, 0);
    tcg_temp_free_ptr(ptr);
    return ret;
}

void e2k_gen_store_breg(DisasContext *ctx, int reg, TCGv_i64 val)
{
    TCGv_ptr ptr = tcg_temp_new_ptr();
    gen_bregi_ptr(ctx, ptr, reg);
    tcg_gen_st_i64(val, ptr, 0);
    tcg_temp_free_ptr(ptr);
}

TCGv_i64 e2k_get_greg(DisasContext *dc, int reg)
{
    assert(reg < GREGS_MAX + BGREGS_MAX);
    return e2k_cs.gregs[reg];
}

void e2k_gen_store_greg(int reg, TCGv_i64 val)
{
    assert(reg < GREGS_MAX + BGREGS_MAX);
    tcg_gen_mov_i64(e2k_cs.gregs[reg], val);
}
