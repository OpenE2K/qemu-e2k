#include "qemu/osdep.h"
#include "qemu.h"
#include "exec/log.h"
#include "translate.h"

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

void e2k_gen_store_preg(int idx, TCGv_i64 val)
{
    TCGv_i64 t0 = tcg_temp_new_i64();
    TCGv_i64 t1 = tcg_temp_new_i64();
    TCGv_i64 t2 = tcg_temp_new_i64();
    TCGv_i64 t3 = tcg_temp_new_i64();

    gen_preg_offset(t0, idx);
    gen_preg_clear(t1, t0);
    tcg_gen_andi_i64(t2, val, 3);
    tcg_gen_shl_i64(t3, val, t0);
    tcg_gen_or_i64(e2k_cs.pregs, t1, t3);

    tcg_temp_free_i64(t3);
    tcg_temp_free_i64(t2);
    tcg_temp_free_i64(t1);
    tcg_temp_free_i64(t0);
}

static inline void gen_reg_tags_group_ptr(TCGv_ptr ret, TCGv_i32 idx)
{
    TCGv_i32 t0 = tcg_const_i32(E2K_TAGS_PER_REG);
    TCGv_i32 t1 = tcg_temp_new_i32();
    TCGv_i32 t2 = tcg_temp_new_i32();
    TCGv_ptr t3 = tcg_temp_new_ptr();

    tcg_gen_divu_i32(t1, idx, t0);
    tcg_temp_free_i32(t0);
    tcg_gen_muli_i32(t2, t1, E2K_REG_LEN);
    tcg_temp_free_i32(t1);
    tcg_gen_ext_i32_ptr(t3, t2);
    tcg_temp_free_i32(t2);
    tcg_gen_add_ptr(ret, e2k_cs.tptr, t3);
    tcg_temp_free_ptr(t3);
}

static inline void gen_reg_tags_group_offset(TCGv_i64 ret, TCGv_i32 idx)
{
    TCGv_i64 t0 = tcg_temp_new_i64();
    TCGv_i64 t1 = tcg_temp_new_i64();

    tcg_gen_extu_i32_i64(t0, idx);
    tcg_gen_andi_i64(t1, t0, E2K_TAGS_PER_REG - 1);
    tcg_temp_free_i64(t0);
    tcg_gen_muli_i64(ret, t1, E2K_REG_TAGS_SIZE);
    tcg_temp_free_i64(t1);
}

static inline void gen_reg_tags_read(TCGv_i32 ret, TCGv_i32 idx, int len)
{
    TCGv_ptr t0 = tcg_temp_new_ptr();
    TCGv_i64 t1 = tcg_temp_new_i64();
    TCGv_i64 t2 = tcg_temp_new_i64();
    TCGv_i64 t3 = tcg_const_i64(len);
    TCGv_i64 t4 = tcg_temp_new_i64();

    gen_reg_tags_group_ptr(t0, idx);
    tcg_gen_ld_i64(t1, t0, 0);
    tcg_temp_free_ptr(t0);
    gen_reg_tags_group_offset(t2, idx);
    e2k_gen_extract_i64(t4, t1, t2, t3);
    tcg_temp_free_i64(t1);
    tcg_temp_free_i64(t2);
    tcg_temp_free_i64(t3);
    tcg_gen_extrl_i64_i32(ret, t4);
    tcg_temp_free_i64(t4);
}

void e2k_gen_reg_tag_read_i64(TCGv_i32 ret, TCGv_i32 idx)
{
    gen_reg_tags_read(ret, idx, E2K_REG_TAGS_SIZE);
}

void e2k_gen_reg_tag_read_i32(TCGv_i32 ret, TCGv_i32 idx)
{
    gen_reg_tags_read(ret, idx, E2K_TAG_SIZE);
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

static inline void gen_reg_tags_write(TCGv_i32 value, TCGv_i32 idx, int len)
{
    TCGv_ptr t0 = tcg_temp_new_ptr();
    TCGv_i64 t1 = tcg_temp_new_i64();
    TCGv_i64 t2 = tcg_temp_new_i64();
    TCGv_i64 t3 = tcg_temp_new_i64();
    TCGv_i64 t4 = tcg_const_i64(len);
    TCGv_i64 t5 = tcg_temp_new_i64();

    gen_reg_tags_group_ptr(t0, idx);
    tcg_gen_ld_i64(t1, t0, 0);
    tcg_gen_extu_i32_i64(t2, value);
    gen_reg_tags_group_offset(t3, idx);
    e2k_gen_deposit_i64(t5, t1, t2, t3, t4);
    tcg_temp_free_i64(t1);
    tcg_temp_free_i64(t2);
    tcg_temp_free_i64(t3);
    tcg_temp_free_i64(t4);
    tcg_gen_st_i64(t5, t0, 0);
    tcg_temp_free_ptr(t0);
    tcg_temp_free_i64(t5);
}

void e2k_gen_reg_tag_write_i64(TCGv_i32 value, TCGv_i32 idx)
{
    gen_reg_tags_write(value, idx, E2K_REG_TAGS_SIZE);
}

void e2k_gen_reg_tag_write_i32(TCGv_i32 value, TCGv_i32 idx)
{
    gen_reg_tags_write(value, idx, E2K_TAG_SIZE);
}

static inline void gen_reg_index_from_wreg(TCGv_i32 ret, TCGv_i32 idx)
{
    TCGLabel *l0 = gen_new_label();
    TCGv_i32 t0 = tcg_temp_local_new_i32();
    TCGv_i32 t1 = tcg_temp_new_i32();
    TCGv_i32 t2;

    tcg_gen_mov_i32(t0, idx);

    tcg_gen_brcond_i32(TCG_COND_LTU, t0, e2k_cs.wd_size, l0);
    e2k_gen_exception(E2K_EXCP_MAPERR);
    gen_set_label(l0);

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
    TCGLabel *l0 = gen_new_label();
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i32 t1 = tcg_temp_new_i32();
    TCGv_i32 t2 = tcg_temp_new_i32();

    tcg_gen_brcondi_i32(tcg_invert_cond(TCG_COND_LTU), e2k_cs.bsize, idx, l0);
    e2k_gen_exception(E2K_EXCP_MAPERR);
    gen_set_label(l0);

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

static inline void gen_reg_ptr_from_index(TCGv_ptr ret, TCGv_i32 idx)
{
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_ptr t1 = tcg_temp_new_ptr();

    tcg_gen_muli_i32(t0, idx, 8);
    tcg_gen_ext_i32_ptr(t1, t0);
    tcg_gen_add_ptr(ret, e2k_cs.rptr, t1);

    tcg_temp_free_ptr(t1);
    tcg_temp_free_i32(t0);
}

void e2k_gen_reg_read_i64(TCGv_i64 ret, TCGv_i32 idx)
{
    TCGv_ptr t0 = tcg_temp_new_ptr();

    gen_reg_ptr_from_index(t0, idx);
    tcg_gen_ld_i64(ret, t0, 0);
    tcg_temp_free_ptr(t0);
}

void e2k_gen_reg_read_i32(TCGv_i32 ret, TCGv_i32 idx)
{
    TCGv_ptr t0 = tcg_temp_new_ptr();

    gen_reg_ptr_from_index(t0, idx);
    tcg_gen_ld_i32(ret, t0, 0);
    tcg_temp_free_ptr(t0);
}

void e2k_gen_reg_write_i64(TCGv_i64 value, TCGv_i32 idx)
{
    TCGv_i64 t0 = tcg_temp_new_i64();
    TCGv_ptr t1 = tcg_temp_new_ptr();

    tcg_gen_mov_i64(t0, value);
    gen_reg_ptr_from_index(t1, idx);
    tcg_gen_st_i64(t0, t1, 0);

    tcg_temp_free_ptr(t1);
    tcg_temp_free_i64(t0);
}

void e2k_gen_reg_write_i32(TCGv_i32 value, TCGv_i32 idx)
{
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_ptr t1 = tcg_temp_new_ptr();

    tcg_gen_mov_i32(t0, value);
    gen_reg_ptr_from_index(t1, idx);
    tcg_gen_st_i32(t0, t1, 0);

    tcg_temp_free_ptr(t1);
    tcg_temp_free_i32(t0);
}
