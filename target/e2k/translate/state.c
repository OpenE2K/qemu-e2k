#include "qemu/osdep.h"
#include "qemu.h"
#include "exec/log.h"
#include "translate.h"

static void gen_preg_offset(TCGv_i64 ret, int reg)
{
    assert(reg < 32);

    TCGv_i64 t0 = tcg_temp_new_i64();
    TCGv_i64 t1 = tcg_temp_new_i64();

    tcg_gen_addi_i64(t0, e2k_cs.pcur, reg);
    e2k_gen_wrap_i64(t1, t0, e2k_cs.psz);
    tcg_gen_shli_i64(ret, t1, 1);

    tcg_temp_free_i64(t1);
    tcg_temp_free_i64(t0);
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
    TCGv_i64 t0 = tcg_temp_new_i64();
    TCGv_i64 t1 = tcg_temp_new_i64();

    gen_preg_offset(t0, reg);
    tcg_gen_shri_i64(t1, e2k_cs.pregs, reg * 2);
    tcg_gen_andi_i64(ret, t1, 0x01);

    tcg_temp_free_i64(t1);
    tcg_temp_free_i64(t0);
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
    TCGv_i64 t2 = tcg_const_i64(1);
    TCGv_i64 t3 = tcg_temp_new_i64();

    gen_preg_offset(t0, reg);
    gen_preg_clear(t1, t0);
    tcg_gen_shl_i64(t3, t2, t0);
    tcg_gen_or_i64(e2k_cs.pregs, t1, t3);

    tcg_temp_free_i64(t3);
    tcg_temp_free_i64(t2);
    tcg_temp_free_i64(t1);
    tcg_temp_free_i64(t0);
}

static inline void gen_wreg_offset(TCGv_i32 ret, int reg)
{
    assert(reg < 64);
    // TODO: exception (wsz * 2) <= reg_index

    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i32 t1 = tcg_temp_new_i32();
    TCGv_i32 t2 = tcg_temp_new_i32();
    TCGv_i32 t3 = tcg_const_i32(WREGS_SIZE);

    tcg_gen_shli_i32(t0, e2k_cs.wbs, 1);    // t = wbs * 2
    tcg_gen_addi_i32(t1, t0, reg);          // t = t + reg_index
    e2k_gen_wrap_i32(t2, t1, t3);           // t = t % WIN_REGS_COUNT
    tcg_gen_muli_i32(ret, t2, REG_SIZE);    // t = t * REG_SIZE_IN_BYTES

    tcg_temp_free_i32(t3);
    tcg_temp_free_i32(t2);
    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
}

static inline void gen_wreg_ptr(TCGv_ptr ret, int reg)
{
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_ptr t1 = tcg_temp_new_ptr();
    gen_wreg_offset(t0, reg);
    tcg_gen_ext_i32_ptr(t1, t0);
    tcg_gen_add_ptr(ret, e2k_cs.win_ptr, t1);
    tcg_temp_free_ptr(t1);
    tcg_temp_free_i32(t0);
}

TCGv_i64 e2k_get_wreg(DisasContext *dc, int reg)
{
    TCGv_i64 ret = e2k_get_temp_i64(dc);
    TCGv_ptr ptr = tcg_temp_new_ptr();
    gen_wreg_ptr(ptr, reg);
    tcg_gen_ld_i64(ret, ptr, 0);
    tcg_temp_free_ptr(ptr);
    return ret;
}

void e2k_gen_store_wreg(int reg, TCGv_i64 val)
{
    TCGv_ptr ptr = tcg_temp_new_ptr();
    gen_wreg_ptr(ptr, reg);
    tcg_gen_st_i64(val, ptr, 0);
    tcg_temp_free_ptr(ptr);
}

static inline void gen_breg_start(TCGv_i32 ret)
{
    TCGv_i32 t0 = tcg_temp_new_i32();

    tcg_gen_add_i32(t0, e2k_cs.wbs, e2k_cs.rbs);
    tcg_gen_shli_i32(ret, t0, 1);

    tcg_temp_free_i32(t0);
}

static inline void gen_breg_offset(TCGv_i32 ret, int reg)
{
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i32 t1 = tcg_temp_new_i32();
    TCGv_i32 t2 = tcg_temp_new_i32();
    TCGv_i32 t3 = tcg_temp_new_i32();
    TCGv_i32 t4 = tcg_temp_new_i32();
    TCGv_i32 t5 = tcg_temp_new_i32();
    TCGv_i32 t6 = tcg_temp_new_i32();
    TCGv_i32 t7 = tcg_temp_new_i32();

    /* TODO: exception: reg > (rsz * 2 + 2) */
    /* t = (reg + rcur * 2) % (rsz * 2 + 2) */
    tcg_gen_shli_i32(t0, e2k_cs.rcur, 1);
    tcg_gen_addi_i32(t1, t0, reg);
    tcg_gen_addi_i32(t2, e2k_cs.rsz, 1);
    tcg_gen_shli_i32(t3, t2, 1);
    e2k_gen_wrap_i32(t4, t1, t3);

    /* TODO: exceptioon: (reg + rbs * 2) > (wsz * 2) */
    /* t = (t + wbs * 2 + rbs * 2) % 192 */
    gen_breg_start(t5);
    tcg_gen_add_i32(t6, t4, t5);
    e2k_gen_wrapi_i32(t7, t6, WREGS_SIZE);

    /* ret = t * 8 */
    tcg_gen_muli_i32(ret, t7, REG_SIZE);

    tcg_temp_free_i32(t7);
    tcg_temp_free_i32(t6);
    tcg_temp_free_i32(t5);
    tcg_temp_free_i32(t4);
    tcg_temp_free_i32(t3);
    tcg_temp_free_i32(t2);
    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
}

static inline void gen_breg_ptr(TCGv_ptr ret, int reg)
{
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_ptr t1 = tcg_temp_new_ptr();
    gen_breg_offset(t0, reg);
    tcg_gen_ext_i32_ptr(t1, t0);
    tcg_gen_add_ptr(ret, e2k_cs.win_ptr, t1);
    tcg_temp_free_ptr(t1);
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
    // TODO: rotated gregs
    assert(reg < 32);
    return e2k_cs.gregs[reg];
}

void e2k_gen_store_greg(int reg, TCGv_i64 val)
{
    // TODO: rotated gregs
    tcg_gen_mov_i64(e2k_cs.gregs[reg], val);
}
