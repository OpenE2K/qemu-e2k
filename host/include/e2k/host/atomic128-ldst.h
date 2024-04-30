/*
 * SPDX-License-Identifier: GPL-2.0-or-later
 * Load/store for 128-bit atomic operations, e2k version.
 *
 * Copyright (C) 2024 Denis Drakhnia <numas13@gmail.com>
 *
 * See docs/devel/atomics.rst for discussion about the guarantees each
 * atomic primitive is meant to provide.
 */

#ifndef E2K_ATOMIC128_LDST_H
#define E2K_ATOMIC128_LDST_H

#if defined(CONFIG_INT128_TYPE) && __iset__ >= 5
#include <e2kintrin.h>

#if __LCC__ >= 128
# define E2K_ASM_QR "x"
#else
# define E2K_ASM_QR "r"
#endif

typedef union {
    __v2di v;
    Int128 s;
} E2KInt128Union;

#define HAVE_ATOMIC128_RO  1
#define HAVE_ATOMIC128_RW  1

/* NOTE: DO NOT USE pragma asm_inline because LCC can convert
 * ldqp|stqp into a two separate ldd|std. */

static inline Int128 atomic16_read_ro(const Int128 *ptr)
{
    E2KInt128Union r;

    asm("ldqp [ %1 ], %0" : "=" E2K_ASM_QR (r.v) : "m" (*ptr));

    return r.s;
}

static inline Int128 atomic16_read_rw(Int128 *ptr)
{
    __v2di *ptr_align = (__v2di *) __builtin_assume_aligned(ptr, 16);
    E2KInt128Union r;

    asm("ldqp [ %1 ], %0" : "=" E2K_ASM_QR (r.v) : "m" (*ptr_align));

    return r.s;
}

static inline void atomic16_set(Int128 *ptr, Int128 val)
{
    __v2di *ptr_align = (__v2di *) __builtin_assume_aligned(ptr, 16);
    E2KInt128Union r = { .s = val };

    asm("stqp [ %0 ], %1" : "=m" (*ptr_align) : E2K_ASM_QR (r.v));
}
#else
/* Provide QEMU_ERROR stubs. */
#include "host/include/generic/host/atomic128-ldst.h"
#endif

#endif /* E2K_ATOMIC128_LDST_H */
