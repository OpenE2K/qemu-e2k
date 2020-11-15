#include "qemu/osdep.h"
#include "qemu.h"
#include "exec/log.h"
#include "translate.h"

static void gen_preg_offset(TCGv_i64 ret, int reg)
{
    assert(reg < 32);

    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i32 t1 = tcg_temp_new_i32();
    TCGv_i32 t2 = tcg_temp_new_i32();

    tcg_gen_addi_i32(t0, e2k_cs.pcur, reg);
    e2k_gen_wrap_i32(t1, t0, e2k_cs.psize);
    tcg_gen_muli_i32(t2, t1, 2);
    tcg_gen_extu_i32_i64(ret, t2);

    tcg_temp_free_i32(t2);
    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
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

static inline void gen_wreg_index(TCGv_i32 ret, int reg)
{
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i32 t1 = tcg_const_i32(WREGS_SIZE);

    /* TODO: exception if overflow window size */
    tcg_gen_addi_i32(t0, e2k_cs.woff, reg);
    tcg_gen_remu_i32(ret, t0, t1);

    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
}

static inline void gen_wreg_ptr(TCGv_ptr ret, int reg)
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

TCGv_i64 e2k_get_wreg(DisasContext *dc, int reg)
{
    TCGv_ptr t0 = tcg_temp_new_ptr();
    TCGv_i64 t1 = e2k_get_temp_i64(dc);

    gen_wreg_ptr(t0, reg);
    tcg_gen_ld_i64(t1, t0, 0);

    tcg_temp_free_ptr(t0);

    return t1;
}

void e2k_gen_store_wreg(int reg, TCGv_i64 val)
{
    TCGv_ptr t0 = tcg_temp_new_ptr();

    gen_wreg_ptr(t0, reg);
    tcg_gen_st_i64(val, t0, 0);

    tcg_temp_free_ptr(t0);
}

static inline void gen_breg_index(TCGv_i32 ret, int reg)
{
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i32 t1 = tcg_temp_new_i32();
    TCGv_i32 t2 = tcg_temp_new_i32();
    TCGv_i32 t3 = tcg_temp_new_i32();
    TCGv_i32 t4 = tcg_const_i32(WREGS_SIZE);

    tcg_gen_addi_i32(t0, e2k_cs.bcur, reg);
    tcg_gen_remu_i32(t1, t0, e2k_cs.bsize);
    tcg_gen_add_i32(t2, t1, e2k_cs.boff);
    tcg_gen_add_i32(t3, t2, e2k_cs.woff);
    tcg_gen_remu_i32(ret, t3, t4);

    tcg_temp_free_i32(t4);
    tcg_temp_free_i32(t3);
    tcg_temp_free_i32(t2);
    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
}

static inline void gen_breg_ptr(TCGv_ptr ret, int reg)
{
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i32 t1 = tcg_temp_new_i32();
    TCGv_ptr t2 = tcg_temp_new_ptr();

    gen_breg_index(t0, reg);
    tcg_gen_muli_i32(t1, t0, 8);
    tcg_gen_ext_i32_ptr(t2, t1);
    tcg_gen_add_ptr(ret, e2k_cs.wptr, t2);

    tcg_temp_free_ptr(t2);
    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
}

TCGv_i64 e2k_get_breg(DisasContext *dc, int reg)
{
    TCGv_i64 ret = e2k_get_temp(dc);
    TCGv_ptr ptr = tcg_temp_new_ptr();
    gen_breg_ptr(ptr, reg);
    tcg_gen_ld_i64(ret, ptr, 0);
    tcg_temp_free_ptr(ptr);
    return ret;
}

void e2k_gen_store_breg(int reg, TCGv_i64 val)
{
    TCGv_ptr ptr = tcg_temp_new_ptr();
    gen_breg_ptr(ptr, reg);
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
