#include "qemu/osdep.h"
#include "qemu.h"
#include "exec/log.h"
#include "translate.h"

static inline void gen_ptr_from_index(TCGv_ptr ret, TCGv_ptr ptr, TCGv_i32 idx,
    int size)
{
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_ptr t1 = tcg_temp_new_ptr();

    tcg_gen_muli_i32(t0, idx, size);
    tcg_gen_ext_i32_ptr(t1, t0);
    tcg_gen_add_ptr(ret, ptr, t1);

    tcg_temp_free_ptr(t1);
    tcg_temp_free_i32(t0);
}

static inline void gen_preg_index(TCGv_i32 ret, int idx)
{
    TCGv_i32 i = tcg_const_i32(idx);
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i32 t1 = tcg_temp_new_i32();
    TCGv_i32 t2 = tcg_temp_new_i32();
    TCGv_i32 t3 = tcg_temp_new_i32();

    assert(idx < 32);

    tcg_gen_addi_i32(t0, e2k_cs.psize, 1);
    tcg_gen_addi_i32(t1, e2k_cs.pcur, idx);
    tcg_gen_remu_i32(t2, t1, t0);
    tcg_gen_movi_i32(t3, idx);
    tcg_gen_movcond_i32(TCG_COND_LEU, ret, i, e2k_cs.psize, t2, t3);

    tcg_temp_free_i32(t3);
    tcg_temp_free_i32(t2);
    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
    tcg_temp_free_i32(i);
}

static void gen_preg_offset(TCGv_i64 ret, int idx)
{
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i32 t1 = tcg_temp_new_i32();

    gen_preg_index(t0, idx);
    tcg_gen_muli_i32(t1, t0, 2);
    tcg_gen_extu_i32_i64(ret, t1);

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

void e2k_gen_preg_i64(TCGv_i64 ret, int reg)
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

void e2k_gen_preg_i32(TCGv_i32 ret, int reg)
{
    TCGv_i64 t0 = tcg_temp_new_i64();

    e2k_gen_preg_i64(t0, reg);
    tcg_gen_extrl_i64_i32(ret, t0);
    tcg_temp_free_i64(t0);
}

TCGv_i64 e2k_get_preg(DisasContext *dc, int reg)
{
    TCGv_i64 ret = e2k_get_temp_i64(dc);
    e2k_gen_preg_i64(ret, reg);
    return ret;
}

void e2k_gen_store_preg(int idx, TCGv_i32 val)
{
    TCGv_i64 t0 = tcg_temp_new_i64();
    TCGv_i64 t1 = tcg_temp_new_i64();
    TCGv_i64 t2 = tcg_temp_new_i64();
    TCGv_i64 t3 = tcg_temp_new_i64();
    TCGv_i64 t4 = tcg_temp_new_i64();

    gen_preg_offset(t0, idx);
    gen_preg_clear(t1, t0);
    tcg_gen_extu_i32_i64(t2, val);
    tcg_gen_andi_i64(t3, t2, 3);
    tcg_gen_shl_i64(t4, t2, t0);
    tcg_gen_or_i64(e2k_cs.pregs, t1, t4);

    tcg_temp_free_i64(t4);
    tcg_temp_free_i64(t3);
    tcg_temp_free_i64(t2);
    tcg_temp_free_i64(t1);
    tcg_temp_free_i64(t0);
}

static inline void gen_reg_tag_ptr(TCGv_ptr ret, TCGv_i32 idx)
{
    TCGv_ptr t0 = tcg_temp_new_ptr();
    TCGv_ptr t1 = tcg_temp_new_ptr();

    tcg_gen_ext_i32_ptr(t0, idx);
    tcg_gen_addi_ptr(t1, cpu_env, offsetof(CPUE2KState, tags));
    tcg_gen_add_ptr(ret, t1, t0);

    tcg_temp_free_ptr(t1);
    tcg_temp_free_ptr(t0);
}

void e2k_gen_reg_tag_read_i64(TCGv_i32 ret, TCGv_i32 idx)
{
    TCGv_ptr t0 = tcg_temp_new_ptr();

    gen_reg_tag_ptr(t0, idx);
    tcg_gen_ld8u_i32(ret, t0, 0);
    tcg_temp_free_ptr(t0);
}

void e2k_gen_reg_tag_read_i32(TCGv_i32 ret, TCGv_i32 idx)
{
    TCGv_ptr t0 = tcg_temp_new_ptr();
    TCGv_i32 t1 = tcg_temp_new_i32();

    gen_reg_tag_ptr(t0, idx);
    tcg_gen_ld8u_i32(t1, t0, 0);
    tcg_gen_andi_i32(ret, t1, GEN_MASK(0, E2K_TAG_SIZE));
    tcg_temp_free_ptr(t0);
}

static inline void gen_tag_check(TCGv_i32 ret, TCGv_i32 tag)
{
    // FIXME: what CPU does if tag is greater than 1?
    tcg_gen_setcondi_i32(TCG_COND_NE, ret, tag, 0);
}

void e2k_gen_reg_tag_check_i64(TCGv_i32 ret, TCGv_i32 tag)
{
    gen_tag_check(ret, tag);
}

void e2k_gen_reg_tag_check_i32(TCGv_i32 ret, TCGv_i32 tag)
{
    TCGv_i32 t0 = tcg_temp_new_i32();

    e2k_gen_reg_tag_extract_lo(t0, tag);
    gen_tag_check(ret, t0);
    tcg_temp_free_i32(t0);
}

void e2k_gen_reg_tag_write_i64(TCGv_i32 value, TCGv_i32 idx)
{
    TCGv_ptr t0 = tcg_temp_new_ptr();

    gen_reg_tag_ptr(t0, idx);
    tcg_gen_st8_i32(value, t0, 0);
    tcg_temp_free_ptr(t0);
}

void e2k_gen_reg_tag_write_i32(TCGv_i32 value, TCGv_i32 idx)
{
    TCGv_ptr t0 = tcg_temp_new_ptr();
    TCGv_i32 t1 = tcg_temp_new_i32();
    TCGv_i32 t2 = tcg_temp_new_i32();

    gen_reg_tag_ptr(t0, idx);
    tcg_gen_ld8u_i32(t1, t0, 0);
    tcg_gen_deposit_i32(t2, t1, value, 0, E2K_TAG_SIZE);
    tcg_gen_st8_i32(t2, t0, 0);

    tcg_temp_free_i32(t2);
    tcg_temp_free_i32(t1);
    tcg_temp_free_ptr(t0);
}

static inline void gen_reg_index_from_wreg(TCGv_i32 ret, TCGv_i32 idx)
{
    TCGv_i32 t0 = tcg_temp_local_new_i32();
    TCGv_i32 t1 = tcg_temp_new_i32();
    TCGv_i32 t2;

    tcg_gen_mov_i32(t0, idx);

    tcg_gen_add_i32(t1, e2k_cs.wd_base, t0);
    tcg_temp_free_i32(t0);
    t2 = tcg_const_i32(E2K_NR_COUNT);
    tcg_gen_remu_i32(ret, t1, t2);
    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t2);
}

void e2k_gen_reg_index_from_wregi(TCGv_i32 ret, int idx)
{
    TCGv_i32 t0 = tcg_const_i32(idx);
    gen_reg_index_from_wreg(ret, t0);
    tcg_temp_free_i32(t0);
}

void e2k_gen_reg_index_from_bregi(TCGv_i32 ret, int idx)
{
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i32 t1 = tcg_temp_new_i32();
    TCGv_i32 t2 = tcg_temp_new_i32();

    tcg_gen_addi_i32(t0, e2k_cs.bcur, idx);
    tcg_gen_remu_i32(t1, t0, e2k_cs.bsize);
    tcg_temp_free_i32(t0);
    tcg_gen_add_i32(t2, e2k_cs.boff, t1);
    tcg_temp_free_i32(t1);
    gen_reg_index_from_wreg(ret, t2);
    tcg_temp_free_i32(t2);
}

void e2k_gen_reg_index_from_gregi(TCGv_i32 ret, int idx)
{
    // TODO: based global registers index
    tcg_gen_movi_i32(ret, E2K_NR_COUNT + idx);
}

static inline void gen_reg_ptr(TCGv_ptr ret, TCGv_i32 idx)
{
    TCGv_ptr t0 = tcg_temp_new_ptr();

    tcg_gen_addi_ptr(t0, cpu_env, offsetof(CPUE2KState, regs));
    gen_ptr_from_index(ret, t0, idx, 8);
    tcg_temp_free_ptr(t0);
}

static inline void gen_xreg_ptr(TCGv_ptr ret, TCGv_i32 idx)
{
    TCGv_ptr t0 = tcg_temp_new_ptr();

    tcg_gen_addi_ptr(t0, cpu_env, offsetof(CPUE2KState, xregs));
    gen_ptr_from_index(ret, t0, idx, 8);
    tcg_temp_free_ptr(t0);
}

#define GEN_REG_READ(name, ty, ptr_func, ld_func) \
    void name(ty ret, TCGv_i32 idx) \
    { \
        TCGv_ptr t0 = tcg_temp_new_ptr(); \
        ptr_func(t0, idx); \
        ld_func(ret, t0, 0); \
        tcg_temp_free_ptr(t0); \
    }

#define GEN_REG_WRITE(name, ty, ptr_func, st_func) \
    void name(ty value, TCGv_i32 idx) \
    { \
        TCGv_ptr t0 = tcg_temp_new_ptr(); \
        ptr_func(t0, idx); \
        st_func(value, t0, 0); \
        tcg_temp_free_ptr(t0); \
    }

GEN_REG_READ(e2k_gen_reg_read_i64, TCGv_i64, gen_reg_ptr, tcg_gen_ld_i64)
GEN_REG_READ(e2k_gen_reg_read_i32, TCGv_i32, gen_reg_ptr, tcg_gen_ld_i32)
GEN_REG_WRITE(e2k_gen_reg_write_i64, TCGv_i64, gen_reg_ptr, tcg_gen_st_i64)
GEN_REG_WRITE(e2k_gen_reg_write_i32, TCGv_i32, gen_reg_ptr, tcg_gen_st_i32)

GEN_REG_READ(e2k_gen_xreg_read_i64, TCGv_i64, gen_xreg_ptr, tcg_gen_ld_i64)
GEN_REG_READ(e2k_gen_xreg_read_i32, TCGv_i32, gen_xreg_ptr, tcg_gen_ld_i32)
GEN_REG_READ(e2k_gen_xreg_read16u_i32, TCGv_i32, gen_xreg_ptr, tcg_gen_ld16u_i32)
GEN_REG_WRITE(e2k_gen_xreg_write_i64, TCGv_i64, gen_xreg_ptr, tcg_gen_st_i64)
GEN_REG_WRITE(e2k_gen_xreg_write_i32, TCGv_i32, gen_xreg_ptr, tcg_gen_st_i32)
GEN_REG_WRITE(e2k_gen_xreg_write16u_i32, TCGv_i32, gen_xreg_ptr, tcg_gen_st16_i32)
