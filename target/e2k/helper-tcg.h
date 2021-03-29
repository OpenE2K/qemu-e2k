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
void QEMU_NORETURN raise_exception(CPUE2KState *desc, int exception_index);
void QEMU_NORETURN raise_exception_ra(CPUE2KState *desc, int exception_index,
    uintptr_t retaddr);

static inline uint64_t env_wd_get(CPUE2KState *env)
{
    E2KWdState *wd = &env->wd;
    uint64_t ret = 0;

    ret = deposit64(ret, WD_SIZE_OFF, WD_SIZE_LEN, wd->size * 8);
    ret = deposit64(ret, WD_PSIZE_OFF, WD_PSIZE_LEN, wd->psize * 8);
    ret = deposit64(ret, WD_FX_OFF, 1, wd->fx);

    return ret;
}

static inline void env_wd_set(CPUE2KState *env, uint64_t raw)
{
    env->wd.size = extract64(raw, WD_SIZE_OFF, WD_SIZE_LEN) / 8;
    env->wd.psize = extract64(raw, WD_PSIZE_OFF, WD_PSIZE_LEN) / 8;
    env->wd.fx = extract64(raw, WD_FX_OFF, 1);
}

static inline uint32_t env_br_get(CPUE2KState *env)
{
    E2KBnState *bn = &env->bn;
    E2KBpState *bp = &env->bp;
    uint32_t ret = 0;

    ret = deposit32(ret, BR_RBS_OFF, BR_RBS_LEN, bn->base / 2);
    ret = deposit32(ret, BR_RSZ_OFF, BR_RSZ_LEN, bn->size / 2 - 1);
    ret = deposit32(ret, BR_RCUR_OFF, BR_RCUR_LEN, bn->cur / 2);

    ret = deposit32(ret, BR_PSZ_OFF, BR_PSZ_LEN, bp->size - 1);
    ret = deposit32(ret, BR_PCUR_OFF, BR_PCUR_LEN, bp->cur);

    return ret;
}

static inline void env_br_set(CPUE2KState *env, uint32_t br)
{
    E2KBnState *bn = &env->bn;
    E2KBpState *bp = &env->bp;

    bn->base = extract32(br, BR_RBS_OFF, BR_RBS_LEN) * 2;
    bn->size = extract32(br, BR_RSZ_OFF, BR_RSZ_LEN) * 2 + 2;
    bn->cur = extract32(br, BR_RCUR_OFF, BR_RCUR_LEN) * 2;

    bp->size = extract32(br, BR_PSZ_OFF, BR_PSZ_LEN) + 1;
    bp->cur = extract32(br, BR_PCUR_OFF, BR_PCUR_LEN);
}

static inline uint64_t env_lsr_get(CPUE2KState *env)
{
    uint64_t lsr = env->lsr;
    lsr = deposit64(lsr, LSR_LCNT_OFF, LSR_LCNT_LEN, env->lsr_lcnt);
    lsr = deposit64(lsr, LSR_ECNT_OFF, LSR_ECNT_LEN, env->lsr_ecnt);
    lsr = deposit64(lsr, LSR_VLC_OFF, 1, env->lsr_vlc);
    lsr = deposit64(lsr, LSR_OVER_OFF, 1, env->lsr_over);
    lsr = deposit64(lsr, LSR_PCNT_OFF, LSR_PCNT_LEN, env->lsr_pcnt);
    lsr = deposit64(lsr, LSR_STRMD_OFF, LSR_STRMD_LEN, env->lsr_strmd);
    return lsr;
}

static inline void env_lsr_set(CPUE2KState *env, uint64_t val)
{
    env->lsr = val;
    env->lsr_lcnt = extract64(val, LSR_LCNT_OFF, LSR_LCNT_LEN);
    env->lsr_ecnt = extract64(val, LSR_ECNT_OFF, LSR_ECNT_LEN);
    env->lsr_vlc = extract64(val, LSR_VLC_OFF, 1);
    env->lsr_over = extract64(val, LSR_OVER_OFF, 1);
    env->lsr_pcnt = extract64(val, LSR_PCNT_OFF, LSR_PCNT_LEN);
    env->lsr_strmd = extract64(val, LSR_STRMD_OFF, LSR_STRMD_LEN);
}

static inline uint64_t env_ilcr_get(CPUE2KState *env)
{
    return ((uint64_t) env->ilcr << LSR_ECNT_OFF)
        | (env->ilcr_lcnt & GEN_MASK(0, LSR_LCNT_LEN));
}

static inline void env_ilcr_set(CPUE2KState *env, uint64_t value)
{
    env->ilcr = (value >> LSR_ECNT_OFF) & ILCR_MASK;
    env->ilcr_lcnt = value & GEN_MASK(0, LSR_LCNT_LEN);
}

static inline uint64_t env_usd_lo_get(CPUE2KState *env)
{
    uint64_t r = 0;

    r = deposit64(r, USD_LO_BASE_OFF, USD_LO_BASE_LEN, env->usd.base);
    r |= env->usd.protected ? USD_LO_PROTECTED_BIT : 0;
    r |= env->usd.read ? USD_LO_READ_BIT : 0;
    r |= env->usd.write ? USD_LO_WRITE_BIT : 0;
    return r;
}

static inline void env_usd_lo_set(CPUE2KState *env, uint64_t value)
{
    env->usd.base = extract64(value, USD_LO_BASE_OFF, USD_LO_BASE_LEN);
    env->usd.protected = (value & USD_LO_PROTECTED_BIT) != 0;
    env->usd.read = (value & USD_LO_READ_BIT) != 0;
    env->usd.write = (value & USD_LO_WRITE_BIT) != 0;
}

#endif /* E2K_HELPER_TCG_H */
