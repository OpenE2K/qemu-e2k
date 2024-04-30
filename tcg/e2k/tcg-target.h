/*
 * SPDX-License-Identifier: LGPL-2.0-or-later
 * SPDX-FileCopyrightText: 2024 Denis Drakhnia <numas13@gmail.com>
 */

#ifndef E2K_TCG_TARGET_H
#define E2K_TCG_TARGET_H

#define TCG_TARGET_INSN_UNIT_SIZE 4
#define TCG_TARGET_NB_REGS 32
#define MAX_CODE_GEN_BUFFER_SIZE (2 * GiB)

typedef enum {
    TCG_REG_B0,
    TCG_REG_B1,
    TCG_REG_B2,
    TCG_REG_B3,
    TCG_REG_B4,
    TCG_REG_B5,
    TCG_REG_B6,
    TCG_REG_B7,

    TCG_REG_R0,
    TCG_REG_R1,
    TCG_REG_R2,
    TCG_REG_R3,
    TCG_REG_R4,
    TCG_REG_R5,
    TCG_REG_R6,
    TCG_REG_R7,

    TCG_REG_G16,
    TCG_REG_G17,
    TCG_REG_G18,
    TCG_REG_G19,
    TCG_REG_G20,
    TCG_REG_G21,
    TCG_REG_G22,
    TCG_REG_G23,
    TCG_REG_G24,
    TCG_REG_G25,
    TCG_REG_G26,
    TCG_REG_G27,
    TCG_REG_G28,
    TCG_REG_G29,
    TCG_REG_G30,
    TCG_REG_G31,

    /* aliases */
    TCG_AREG0           = TCG_REG_R0,
    TCG_REG_SP          = TCG_REG_R6,
    TCG_GUEST_BASE_REG  = TCG_REG_R7,
    TCG_REG_TMP0        = TCG_REG_G31,
    TCG_REG_TMP1        = TCG_REG_G30,
    TCG_REG_TMP2        = TCG_REG_G29,
    TCG_REG_TMP3        = TCG_REG_G28,
} TCGReg;

/* used for function call generation */
#define TCG_REG_CALL_STACK              TCG_REG_SP
#define TCG_TARGET_STACK_ALIGN          16
#define TCG_TARGET_CALL_STACK_OFFSET    0
#define TCG_TARGET_CALL_ARG_I32         TCG_CALL_ARG_NORMAL
#define TCG_TARGET_CALL_ARG_I64         TCG_CALL_ARG_NORMAL
#define TCG_TARGET_CALL_ARG_I128        TCG_CALL_ARG_NORMAL
#define TCG_TARGET_CALL_RET_I128        TCG_CALL_RET_NORMAL

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
                                          //
#define TCG_TARGET_HAS_tst              0

#define TCG_TARGET_DEFAULT_MO 0

#define TCG_TARGET_NEED_LDST_LABELS

#endif /* E2K_TCG_TARGET_H */
