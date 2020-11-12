#include "qemu/osdep.h"
#include "qemu.h"
#include "tcg/tcg-op.h"
#include "exec/log.h"
#include "exec/translator.h"
#include "translate.h"

static inline void gen_abn_inc(DisasContext *dc, TCGCond cond)
{
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i32 t1 = tcg_temp_new_i32();
    TCGv_i32 one = tcg_const_i32(1);
    TCGv_i32 jmp_cond = tcg_temp_new_i32();

    tcg_gen_addi_i32(t0, e2k_cs.rcur, 2);
    e2k_gen_wrap_i32(t1, t0, e2k_cs.rsz);
    tcg_gen_extrl_i64_i32(jmp_cond, dc->jmp.cond);
    tcg_gen_movcond_i32(cond, e2k_cs.rcur,
        jmp_cond, one,
        t1, e2k_cs.rcur);

    tcg_temp_free_i32(jmp_cond);
    tcg_temp_free_i32(one);
    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
}

void e2k_win_commit(DisasContext *dc)
{
    // Change windowing registers after commit is done.
    uint32_t ss = dc->bundle.ss;
//    unsigned int vfdi = (ss & 0x04000000) >> 26;
//    unsigned int abg = (ss & 0x01800000) >> 23;
//    unsigned int abp = (ss & 0x000c0000) >> 18;
//    unsigned int alc = (ss & 0x00030000) >> 16;

    if (GET_BIT(ss, 21)) {
        gen_abn_inc(dc, TCG_COND_EQ);
    }
    if (GET_BIT(ss, 22)) {
        gen_abn_inc(dc, TCG_COND_NE);
    }
}
