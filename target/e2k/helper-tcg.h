/*
 * e2k TCG specific prototypes for helpers
 *
 * Copyright (c) 2020-2021 Denis Drakhnya, Alibek Omarov
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef E2K_HELPER_TCG_H
#define E2K_HELPER_TCG_H

#include "qemu/osdep.h"
#include "cpu.h"
#include "exec/exec-all.h"

#define glue3(x, y, z) glue(glue(x, y), z)
#define glue4(x, y, z, w) glue(glue3(x, y, z), w)
#define deref(x) *(x)
#define ident(x) (x)

/* helper.c */
void QEMU_NORETURN raise_exception(CPUE2KState *env, int exception_index);
void QEMU_NORETURN raise_exception_ra(CPUE2KState *env, int exception_index,
    uintptr_t retaddr);

#endif /* E2K_HELPER_TCG_H */
