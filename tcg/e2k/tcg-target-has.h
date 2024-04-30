/*
 * SPDX-License-Identifier: LGPL-2.0-or-later
 * SPDX-FileCopyrightText: 2024 Denis Drakhnia <numas13@gmail.com>
 */

#ifndef TCG_TARGET_HAS_H
#define TCG_TARGET_HAS_H

/* optional instructions */
#define TCG_TARGET_HAS_negsetcond_i32   1
#define TCG_TARGET_HAS_movcond_i32      1
#define TCG_TARGET_HAS_rot_i32          1
#define TCG_TARGET_HAS_deposit_i32      1
#define TCG_TARGET_HAS_extract_i32      1
#define TCG_TARGET_HAS_sextract_i32     1
#define TCG_TARGET_HAS_extract2_i32     0
#define TCG_TARGET_HAS_ext8s_i32        1
#define TCG_TARGET_HAS_ext16s_i32       1
#define TCG_TARGET_HAS_ext8u_i32        1
#define TCG_TARGET_HAS_ext16u_i32       1
#define TCG_TARGET_HAS_bswap16_i32      0
#define TCG_TARGET_HAS_bswap32_i32      0
#define TCG_TARGET_HAS_not_i32          1
#define TCG_TARGET_HAS_neg_i32          1
#define TCG_TARGET_HAS_andc_i32         1
#define TCG_TARGET_HAS_orc_i32          1
#define TCG_TARGET_HAS_eqv_i32          1
#define TCG_TARGET_HAS_nand_i32         0
#define TCG_TARGET_HAS_nor_i32          0
#define TCG_TARGET_HAS_clz_i32          (__iset__ >= 2)
#define TCG_TARGET_HAS_ctz_i32          (__iset__ >= 2)
#define TCG_TARGET_HAS_ctpop_i32        (__iset__ >= 2)
#define TCG_TARGET_HAS_brcond2          0
#define TCG_TARGET_HAS_setcond2         0
#define TCG_TARGET_HAS_qemu_st8_i32     0
#define TCG_TARGET_HAS_add2_i32         0
#define TCG_TARGET_HAS_sub2_i32         0
#define TCG_TARGET_HAS_mulu2_i32        0
#define TCG_TARGET_HAS_muls2_i32        0
#define TCG_TARGET_HAS_muluh_i32        0
#define TCG_TARGET_HAS_mulsh_i32        0
#define TCG_TARGET_HAS_div_i32          1
#define TCG_TARGET_HAS_rem_i32          1
#define TCG_TARGET_HAS_div2_i32         0

#define TCG_TARGET_HAS_negsetcond_i64   1
#define TCG_TARGET_HAS_movcond_i64      1
#define TCG_TARGET_HAS_div_i64          1
#define TCG_TARGET_HAS_rem_i64          0
#define TCG_TARGET_HAS_div2_i64         0
#define TCG_TARGET_HAS_rot_i64          1
#define TCG_TARGET_HAS_deposit_i64      1
#define TCG_TARGET_HAS_extract_i64      1
#define TCG_TARGET_HAS_sextract_i64     1
#define TCG_TARGET_HAS_extract2_i64     0
#define TCG_TARGET_HAS_extr_i64_i32     0
#define TCG_TARGET_HAS_extrl_i64_i32    1
#define TCG_TARGET_HAS_extrh_i64_i32    1
#define TCG_TARGET_HAS_ext8s_i64        1
#define TCG_TARGET_HAS_ext16s_i64       1
#define TCG_TARGET_HAS_ext32s_i64       1
#define TCG_TARGET_HAS_ext8u_i64        1
#define TCG_TARGET_HAS_ext16u_i64       1
#define TCG_TARGET_HAS_ext32u_i64       1
#define TCG_TARGET_HAS_bswap16_i64      0
#define TCG_TARGET_HAS_bswap32_i64      0
#define TCG_TARGET_HAS_bswap64_i64      0
#define TCG_TARGET_HAS_not_i64          1
#define TCG_TARGET_HAS_neg_i64          1
#define TCG_TARGET_HAS_andc_i64         1
#define TCG_TARGET_HAS_orc_i64          1
#define TCG_TARGET_HAS_eqv_i64          1
#define TCG_TARGET_HAS_nand_i64         0
#define TCG_TARGET_HAS_nor_i64          0
#define TCG_TARGET_HAS_clz_i64          (__iset__ >= 2)
#define TCG_TARGET_HAS_ctz_i64          (__iset__ >= 2)
#define TCG_TARGET_HAS_ctpop_i64        (__iset__ >= 2)
#define TCG_TARGET_HAS_add2_i64         0
#define TCG_TARGET_HAS_sub2_i64         0
#define TCG_TARGET_HAS_mulu2_i64        0
#define TCG_TARGET_HAS_muls2_i64        0
#define TCG_TARGET_HAS_muluh_i64        (__iset__ >= 3)
#define TCG_TARGET_HAS_mulsh_i64        (__iset__ >= 3)

#define TCG_TARGET_HAS_qemu_ldst_i128   0 // TODO: stq (v1), ldq/ldqp/stqp (v5)

#define TCG_TARGET_HAS_tst              0

#define TCG_TARGET_HAS_v64              0 // TODO: v1
#define TCG_TARGET_HAS_v128             0 // TODO: v5
#define TCG_TARGET_HAS_v256             0

#define TCG_TARGET_HAS_andc_vec         0
#define TCG_TARGET_HAS_orc_vec          0
#define TCG_TARGET_HAS_nand_vec         0
#define TCG_TARGET_HAS_nor_vec          0
#define TCG_TARGET_HAS_eqv_vec          0
#define TCG_TARGET_HAS_not_vec          0
#define TCG_TARGET_HAS_neg_vec          0
#define TCG_TARGET_HAS_abs_vec          0
#define TCG_TARGET_HAS_roti_vec         0
#define TCG_TARGET_HAS_rots_vec         0
#define TCG_TARGET_HAS_rotv_vec         0
#define TCG_TARGET_HAS_shi_vec          0
#define TCG_TARGET_HAS_shs_vec          0
#define TCG_TARGET_HAS_shv_vec          0
#define TCG_TARGET_HAS_mul_vec          0
#define TCG_TARGET_HAS_sat_vec          0
#define TCG_TARGET_HAS_minmax_vec       0
#define TCG_TARGET_HAS_bitsel_vec       0
#define TCG_TARGET_HAS_cmpsel_vec       0
#define TCG_TARGET_HAS_tst_vec          0

#define TCG_TARGET_deposit_valid(type, ofs, len) \
    ((type) == TCG_TYPE_I32 || (type) == TCG_TYPE_I64)

#define TCG_TARGET_sextract_valid(type, ofs, len) \
    ((type) == TCG_TYPE_I32 || (type) == TCG_TYPE_I64)

#define TCG_TARGET_extract_valid(type, ofs, len) \
    ((type) == TCG_TYPE_I32 || (type) == TCG_TYPE_I64)

#endif
