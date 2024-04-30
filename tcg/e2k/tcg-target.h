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

#endif /* E2K_TCG_TARGET_H */
