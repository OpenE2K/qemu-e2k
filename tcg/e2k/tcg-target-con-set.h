/*
 * SPDX-License-Identifier: LGPL-2.0-or-later
 * SPDX-FileCopyrightText: 2024 Denis Drakhnia <numas13@gmail.com>
 */

/*
 * C_On_Im(...) defines a constraint set with <n> outputs and <m> inputs.
 * Each operand should be a sequence of constraint letters as defined by
 * tcg-target-con-str.h; the constraint combination is inclusive or.
 */

C_O0_I1(r)
C_O0_I2(r, r)
C_O0_I2(r, rI)

C_O1_I1(r, r)
C_O1_I1(r, rI)

C_O1_I2(r, r, r)
C_O1_I2(r, r, ri)
C_O1_I2(r, r, rI)

C_O1_I4(r, r, rI, rI, rI)
