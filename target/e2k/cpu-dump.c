/*
 * e2k CPU dump to file
 *
 * Copyright (c) 2021 Denis Drakhnya
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#include "qemu/osdep.h"
#include "cpu.h"
#include "helper-tcg.h"
#include "qemu/qemu-print.h"

static inline const char *indent(int width, int max_width)
{
    static const char s[] = "               ";

    assert(width < sizeof(s) && max_width < sizeof(s));
    return s + sizeof(s) - 1 - max_width + width;
}

static void dump_psp(FILE *f, int flags, const char *name, E2KPsp *psp,
    bool newline)
{
    qemu_fprintf(f, "%s %016" PRIx64 "  base %016" PRIx64 "  index %08" PRIx32
        " size %08" PRIx32 "  %c%c itag=%x%s",
        name,
        (uint64_t) psp->base + psp->index,
        (uint64_t) psp->base,
        (uint32_t) psp->index,
        (uint32_t) psp->size,
        psp->is_readable ? 'R' : ' ',
        psp->is_writable ? 'W' : ' ',
        (int) psp->itag,
        newline ? "\n" : "");
}

static void dump_rwap(FILE *f, int flags, const char *name, E2KRwap *rwap,
    bool newline)
{
    qemu_fprintf(f, "%s %016" PRIx64 "  base %016" PRIx64 " curptr %08" PRIx32
        " size %08" PRIx32 " %c%c%c%s",
        name,
        (uint64_t) rwap->base + rwap->curptr,
        (uint64_t) rwap->base,
        (uint32_t) rwap->curptr,
        (uint32_t) rwap->size,
        rwap->protected ? 'P' : ' ',
        rwap->read ? 'R' : ' ',
        rwap->write ? 'W' : ' ',
        newline ? "\n" : "");
}

static void dump_lsr(CPUE2KState *env, FILE *f, int flags)
{
    qemu_fprintf(f,
        " lcnt %016" PRIx64 "  pcnt=%02x  ecnt=%02x  vlc=%d ldmc=%d ldovl=%02x\n"
        "ilcnt %016" PRIx64 " ipcnt=%02x iecnt=%02x over=%d semc=%d strmd=%02x\n",
        env->lsr_lcnt,
        (int) env->lsr_pcnt,
        (int) env->lsr_ecnt,
        (int) env->lsr_vlc,
        (int) extract32(env->lsr, LSR_LDMC_OFF - 32, 1),
        (int) extract32(env->lsr, LSR_LDOVL_OFF - 32, LSR_LDOVL_LEN),
        env->ilcr_lcnt,
        (int) extract32(env->ilcr, LSR_PCNT_OFF - 32, LSR_PCNT_LEN),
        (int) extract32(env->ilcr, LSR_ECNT_OFF - 32, LSR_ECNT_LEN),
        (int) env->lsr_over,
        (int) extract32(env->lsr, LSR_SEMC_OFF - 32, 1),
        (int) env->lsr_strmd
    );
}

static void dump_predicate_regs(CPUE2KState *env, FILE *f, int flags)
{
    int i;

    qemu_fprintf(f, "pred");
    for (i = 0; i < 32; i++) {
        qemu_fprintf(f, "%s%d", indent(i > 9, 2), i);
    }
    qemu_fprintf(f, "\n    ");
    for (i = 0; i < 32; i++) {
        int index = i < env->bp.size ? (i + env->bp.cur) % env->bp.size : i;
        int preg = (env->pregs >> (index * 2)) & 3;
        qemu_fprintf(f, " %c%d", preg >> 1 ? '*' : ' ', preg & 1);
    }
    qemu_fprintf(f, "\n");
}

static inline void dump_reg(FILE *f, char mnemonic, int index,
    uint8_t tag, E2KReg reg)
{
    int width = (index > 9) + (index > 99);
    const char *prefix = indent(width, 3 - ((index & 3) == 0));
    uint8_t tag_lo = tag & 3;
    uint8_t tag_hi = (tag >> 2) & 3;

    qemu_fprintf(f, "%s%c%d <%d%d>%016" PRIx64, prefix, mnemonic, index,
        tag_hi, tag_lo, reg.lo);
    if ((index & 3) == 3) {
        qemu_fprintf(f, "\n");
    }
}

static void dump_regs(CPUE2KState *env, FILE *f, int flags)
{
    int i;

    for (i = 0; i < env->wd.size; i++) {
        dump_reg(f, 'r', i, env->tags[i], env->regs[i]);
    }
    if (env->wd.size & 3) {
        qemu_fprintf(f, "\n");
    }

    if ((env->wd.size - env->bn.base) >= env->bn.size) {
        for (i = 0; i < env->bn.size; i++) {
            int index = env->bn.base + (i + env->bn.cur) % env->bn.size;
            dump_reg(f, 'b', i, env->tags[index], env->regs[index]);
        }
        if (env->bn.size & 3) {
            qemu_fprintf(f, "\n");
        }
    }

    for (i = 0; i < 32; i++) {
        int index = E2K_NR_COUNT + i;
        dump_reg(f, 'g', i, env->tags[index], env->regs[index]);
    }
}

void e2k_cpu_dump_state(CPUState *cs, FILE *f, int flags)
{
    E2KCPU *cpu = E2K_CPU(cs);
    CPUE2KState *env = &cpu->env;


    qemu_fprintf(f,
        "   ip %016" PRIx64 " ctpr1 %016" PRIx64 "  ctpr2 %016" PRIx64 "  ctpr3 %016" PRIx64 "\n"
        "  sbr %016" PRIx64 "  osr0 %016" PRIx64 " psr=%02x upsr=%04x\n",
        (uint64_t) env->ip,
        env->ctprs[0].raw,
        env->ctprs[1].raw,
        env->ctprs[2].raw,
        env->sbr,
        env->osr0,
        (int) env->psr,
        (int) env->upsr
    );
    dump_rwap(f, flags, "  usd", &env->usd, true);
    dump_psp(f,  flags, " pcsp", &env->pcsp, true);
    dump_psp(f,  flags, "  psp", &env->psp, true);
    dump_rwap(f, flags, "oscud", &env->oscud, true);
    dump_rwap(f, flags, " osgd", &env->osgd, true);
    dump_rwap(f, flags, "  cud", &env->cud, true);
    dump_rwap(f, flags, "   gd", &env->gd, true);
    dump_lsr(env, f, flags);
    qemu_fprintf(f, " wsz=%d wbdl=%d rbs=%d rsz=%d rcur=%d psz=%d pcur=%d\n",
        (int) env->wd.size,
        (int) env->wdbl,
        (int) env->bn.base,
        (int) env->bn.size,
        (int) env->bn.cur,
        (int) env->bp.size,
        (int) env->bp.cur
    );
    dump_predicate_regs(env, f, flags);
    dump_regs(env, f, flags);
}

