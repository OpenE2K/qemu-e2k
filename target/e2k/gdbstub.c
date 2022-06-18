/*
 * Elbrus 2000 gdb server stub
 *
 * Copyright (c) 2003-2005 Fabrice Bellard
 * Copyright (c) 2013 SUSE LINUX Products GmbH
 * Copyright (c) 2020 Alibek Omarov
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
#include "qemu/timer.h"
#include "cpu.h"
#include "helper-tcg.h"
#include "exec/gdbstub.h"

/* TODO: reverse engineer e2k-linux-gdb register ids */

static uint64_t cr_read(CPUState *cs, CPUE2KState *env, size_t offset)
{
    target_ulong addr = env->pcsp.base + env->pcsp.index + offset;
    uint64_t r;
    cpu_memory_rw_debug(cs, addr, &r, sizeof(r), false);
    return r;
}

int e2k_cpu_gdb_read_register(CPUState *cs, GByteArray *mem_buf, int n)
{
    E2KCPU *cpu = E2K_CPU(cs);
//    CPUClass *cc = CPU_GET_CLASS(cs);
    CPUE2KState *env = &cpu->env;

    switch (n) {
    case 0: return gdb_get_reg64(mem_buf, 0); // unk
    case 1: return gdb_get_reg64(mem_buf, 0); // b0
    case 2: return gdb_get_reg64(mem_buf, 0); // b1
    default: break;
    }

    if (3 <= n && n < 35) {
        return gdb_get_reg64(mem_buf, env->regs[E2K_NR_COUNT + n - 3].lo);
    }
    
    switch (n) {
    case 35: return gdb_get_reg64(mem_buf, env->psr); // psr
    case 36: return gdb_get_reg64(mem_buf, env->upsr); // upsr
    case 37: return gdb_get_reg64(mem_buf, env->oscud.lo); // oscud_lo
    case 38: return gdb_get_reg64(mem_buf, env->oscud.hi); // oscud_hi
    case 39: return gdb_get_reg64(mem_buf, env->osgd.lo); // osgd_lo
    case 40: return gdb_get_reg64(mem_buf, env->osgd.hi); // osgd_hi
    case 41: return gdb_get_reg64(mem_buf, 0); // osem
    case 42: return gdb_get_reg64(mem_buf, env->osr0); // osr0
    case 43: return gdb_get_reg64(mem_buf, env->pfpfr.raw); // pfpfr
    case 44: return gdb_get_reg64(mem_buf, env->fpcr.raw); // fpcr
    case 45: return gdb_get_reg64(mem_buf, env->fpsr.raw); // fpsr
    case 46: return gdb_get_reg64(mem_buf, 0); // usbr
    case 47: return gdb_get_reg64(mem_buf, env->usd.lo); // usd_lo
    case 48: return gdb_get_reg64(mem_buf, env->usd.hi); // usd_hi
    case 49: return gdb_get_reg64(mem_buf, env->psp.lo); // psp_lo
    case 50: return gdb_get_reg64(mem_buf, env->psp.hi); // psp_hi
    case 51: return gdb_get_reg64(mem_buf, 0); // pshtp
    case 52: return gdb_get_reg64(mem_buf, env->pregs); // pregs
    case 53: return gdb_get_reg64(mem_buf, env->ip); // ip
    case 54: { // cr1_lo
        uint64_t cr1_lo = cr_read(cs, env, offsetof(E2KCrs, cr1.lo));
        return gdb_get_reg64(mem_buf, cr1_lo);
    }
    case 55: { // cr1_hi
        uint64_t cr1_hi = cr_read(cs, env, offsetof(E2KCrs, cr1.hi));
        return gdb_get_reg64(mem_buf, cr1_hi);
    }
    case 56: return gdb_get_reg64(mem_buf, 0); // cwd
    case 57: return gdb_get_reg64(mem_buf, env->pcsp.lo); // pcsp_lo
    case 58: return gdb_get_reg64(mem_buf, env->pcsp.hi); // pcsp_hi
    case 59: return gdb_get_reg64(mem_buf, 0); // pcshtp
    case 60: return gdb_get_reg64(mem_buf, env->cud.lo); // cud_lo
    case 61: return gdb_get_reg64(mem_buf, env->cud.hi); // cud_hi
    case 62: return gdb_get_reg64(mem_buf, env->gd.lo); // gd_lo
    case 63: return gdb_get_reg64(mem_buf, env->gd.hi); // gd_hi
    case 64: return gdb_get_reg64(mem_buf, env->cs.lo); // cs_lo
    case 65: return gdb_get_reg64(mem_buf, env->cs.hi); // cs_hi
    case 66: return gdb_get_reg64(mem_buf, env->ds.lo); // ds_lo
    case 67: return gdb_get_reg64(mem_buf, env->ds.hi); // ds_hi
    case 68: return gdb_get_reg64(mem_buf, env->es.lo); // es_lo
    case 69: return gdb_get_reg64(mem_buf, env->es.hi); // es_hi
    case 70: return gdb_get_reg64(mem_buf, env->fs.lo); // fs_lo
    case 71: return gdb_get_reg64(mem_buf, env->fs.hi); // fs_hi
    case 72: return gdb_get_reg64(mem_buf, env->gs.lo); // gs_lo
    case 73: return gdb_get_reg64(mem_buf, env->gs.hi); // gs_hi
    case 74: return gdb_get_reg64(mem_buf, env->ss.lo); // ss_lo
    case 75: return gdb_get_reg64(mem_buf, env->ss.hi); // ss_hi
    default:
        break;
    }

    if (76 <= n && n < 140) {
        int idx = (n - 76) >> 1;
        if (n & 1) {
            return gdb_get_reg64(mem_buf, env->aau.ds[idx].hi); // addN_hi
        } else {
            return gdb_get_reg64(mem_buf, env->aau.ds[idx].lo); // addN_lo
        }
    }

    if (140 <= n && n < 156) {
        return gdb_get_reg64(mem_buf, env->aau.inds[n - 140]); // aaindN
    }

    if (156 <= n && n < 164) {
        return gdb_get_reg64(mem_buf, env->aau.incrs[n - 156]); // aaincrN
    }

    if (164 <= n && n < 196) {
        return gdb_get_reg64(mem_buf, env->aau.pl.area[n - 164].ldi); // aaldiN
    }

    if (196 <= n && n < 228) {
        return gdb_get_reg64(mem_buf, env->aau.pr.area[n - 196].ldi); // aaldiN
    }

    if (n == 228) {
        return gdb_get_reg64(mem_buf, env->aau.ldv); // aaldv
    }

    if (229 <= n && n < 293) {
        return gdb_get_reg64(mem_buf, env->aau.lda[n - 229].raw); // aaldaN
    }

    switch (n) {
    case 293: return gdb_get_reg64(mem_buf, env->aau.ldm); // aaldm
    case 294: return gdb_get_reg64(mem_buf, env->aau.sr.raw); // aasr
    case 295: return gdb_get_reg64(mem_buf, env->aau.fstr); // aafstr
    default:
        break;
    }

    if (296 <= n && n < 312) {
        return gdb_get_reg64(mem_buf, env->aau.stis[n - 296]); // aastiN
    }

    switch (n) {
    case 312: return gdb_get_reg64(mem_buf, cpu_get_host_ticks()); // clkr
    case 313: return gdb_get_reg64(mem_buf, 0); // dibcr
    case 314: return gdb_get_reg64(mem_buf, 0); // ddbcr
    default:
        break;
    }

    if (315 <= n && n < 319) {
        return gdb_get_reg64(mem_buf, 0); // dibarN
    }

    if (319 <= n && n < 323) {
        return gdb_get_reg64(mem_buf, 0); // ddbarN
    }

    switch (n) {
    case 323: return gdb_get_reg64(mem_buf, 0); // dimcr
    case 324: return gdb_get_reg64(mem_buf, 0); // ddmcr
    case 325: return gdb_get_reg64(mem_buf, 0); // dimar0
    case 326: return gdb_get_reg64(mem_buf, 0); // dimar1
    case 327: return gdb_get_reg64(mem_buf, 0); // ddmar0
    case 328: return gdb_get_reg64(mem_buf, 0); // ddmar1
    case 329: return gdb_get_reg64(mem_buf, 0); // dibsr
    case 330: return gdb_get_reg64(mem_buf, 0); // ddbsr
    case 331: return gdb_get_reg64(mem_buf, 0); // dtcr
    case 332: return gdb_get_reg64(mem_buf, 0); // dtarf
    case 333: return gdb_get_reg64(mem_buf, 0); // dtart
    case 334: return gdb_get_reg64(mem_buf, env_wd_get(env)); // wd
    case 335: return gdb_get_reg64(mem_buf, 0); // unk
    case 336: return gdb_get_reg64(mem_buf, env->bgr); // bgr
    case 337: return gdb_get_reg64(mem_buf, 0); // unk
    case 338: return gdb_get_reg64(mem_buf, env->nip); // nip
    case 339: return gdb_get_reg64(mem_buf, env->ctprs[0].raw); // ctpr1
    case 340: return gdb_get_reg64(mem_buf, env->ctprs[1].raw); // ctpr2
    case 341: return gdb_get_reg64(mem_buf, env->ctprs[2].raw); // ctpr3
    case 342: return gdb_get_reg64(mem_buf, 0); // eir
    case 343: return gdb_get_reg64(mem_buf, 0); // tr
    case 344: return gdb_get_reg64(mem_buf, 0); // cutd
    case 345: return gdb_get_reg64(mem_buf, 0); // cuir
    case 346: return gdb_get_reg64(mem_buf, 0); // tsd
    case 347: return gdb_get_reg64(mem_buf, env_lsr_get(env)); /* lsr */
    case 348: return gdb_get_reg64(mem_buf, env_ilcr_get(env)); /* ilcr */
    default:
        break;
    }

    if (349 <= n && n < 356) {
        return gdb_get_reg64(mem_buf, 0); // unk
    }

    if (356 <= n && n < 360) {
        uint64_t tags = 0;
        int i, offset = E2K_NR_COUNT + (n - 356) * 8;

        for (i = 0; i < 8; i++) {
            tags |= (uint64_t) env->tags[offset + i] << (i * 8);
        }
        return gdb_get_reg64(mem_buf, tags); // gN tags (tag len is 1 byte)
    }

    if (360 <= n && n < 368) {
        uint64_t ext = 0;
        int i, offset = E2K_NR_COUNT + (n - 360) * 4;

        for (i = 0; i < 4; i++) {
            ext |= ((uint64_t) env->regs[offset + i].hi & 0xffff) << (i * 16);
        }
        return gdb_get_reg64(mem_buf, ext); // xgN
    }

    if (n == 368) {
        return gdb_get_reg64(mem_buf, 0); // rpr_hi
    }

    if (n == 369) {
        return gdb_get_reg64(mem_buf, 0); // rpr_lo
    }

    if (370 <= n && n < 389) {
        return gdb_get_reg64(mem_buf, 0); // tir_N_lo
    }

    if (389 <= n && n < 408) {
        return gdb_get_reg64(mem_buf, 0); // tir_N_hi
    }

    if (408 <= n && n < 418) {
        return gdb_get_reg64(mem_buf, 0); // trap_cell_addr_N
    }

    if (418 <= n && n < 428) {
        return gdb_get_reg64(mem_buf, 0); // trap_cell_val_N
    }

    if (n == 428) {
        return gdb_get_reg64(mem_buf, 0); // trap_cell_tag_N [0, 7)
    }

    if (n == 429) {
        return gdb_get_reg64(mem_buf, 0); // trap_cell_tag_N [8, 9)
    }

    if (430 <= n && n < 440) {
        return gdb_get_reg64(mem_buf, 0); // trap_cell_info_N
    }

    if (440 <= n && n < 472) {
        return gdb_get_reg64(mem_buf, env->dam[n - 440].raw); // dam_N
    }

    if (472 <= n && n < 504) {
        return gdb_get_reg64(mem_buf, 0); // sbbp_N
    }

    if (504 <= n && n < 552) {
        return gdb_get_reg64(mem_buf, 0); // mlt_N (3 regs)
    }

    if (n == 552) {
        return gdb_get_reg64(mem_buf, env->pcsp.base); // pcsp_base
    }

    if (n == 553) {
        return gdb_get_reg64(mem_buf, env->psp.base); // psp_base
    }

    if (554 <= n && n < 573) {
        return gdb_get_reg64(mem_buf, 0); // unk
    }

    if (n == 573) {
        return gdb_get_reg32(mem_buf, 0); // unk
    }

    fprintf(stderr, "%s: unknown register %d\n", __FUNCTION__, n);

    return 0;
}

int e2k_cpu_gdb_write_register(CPUState *cs, uint8_t *mem_buf, int n)
{
//    E2KCPU *cpu = E2K_CPU(cs);
//    CPUClass *cc = CPU_GET_CLASS(cs);
//    CPUE2KState *env = &cpu->env;

    fprintf(stderr, "%s: unknown register %d\n", __FUNCTION__, n);

    // TODO: e2k_cpu_gdb_write_register
    return 0;
}

static int gdb_get_v2(CPUE2KState *env, GByteArray *buf, int n)
{
    if (n == 0) {
        /* idr */
        return gdb_get_reg64(buf, env->idr);
    }

    return 0;
}

static int gdb_set_v2(CPUE2KState *env, uint8_t *mem_buf, int n)
{
    fprintf(stderr, "%s: unknown register %d\n", __FUNCTION__, n);
    return 0;
}

static int gdb_get_v3(CPUE2KState *env, GByteArray *buf, int n)
{
    if (n == 0) {
        /* core_mode */
        return gdb_get_reg64(buf, env->core_mode);
    }

    return 0;
}

static int gdb_set_v3(CPUE2KState *env, uint8_t *mem_buf, int n)
{
    fprintf(stderr, "%s: unknown register %d\n", __FUNCTION__, n);
    return 0;
}

static int gdb_get_v5(CPUE2KState *env, GByteArray *buf, int n)
{
    if (n == 0) {
        /* lsr1 */
        return gdb_get_reg64(buf, env->lsr_lcnt);
    } else if (n == 1) {
        /* ilcr1 */
        return gdb_get_reg64(buf, env->ilcr_lcnt);
    } else if (n >= 2 && n < 34) {
        /* xgN (upper 64-bit) */
        return gdb_get_reg64(buf, env->regs[E2K_NR_COUNT + n - 2].hi);
    } else if (n >= 34 && n < 66) {
        /* qpgN tags */
        return gdb_get_reg8(buf, env->tags[E2K_NR_COUNT + n - 34]);
    }

    return 0;
}

static int gdb_set_v5(CPUE2KState *env, uint8_t *mem_buf, int n)
{
    fprintf(stderr, "%s: unknown register %d\n", __FUNCTION__, n);
    return 0;
}

void e2k_cpu_register_gdb_regs_for_features(CPUState *cs)
{
    E2KCPU *cpu = E2K_CPU(cs);
    CPUE2KState *env = &cpu->env;

    if (env->version >= 2) {
        gdb_register_coprocessor(cs, gdb_get_v2, gdb_set_v2,
            1, "e2k-v2.xml", 574);
    }

    if (env->version >= 3) {
        gdb_register_coprocessor(cs, gdb_get_v3, gdb_set_v3,
            1, "e2k-v3.xml", 575);
    }

    if (env->version >= 5) {
        gdb_register_coprocessor(cs, gdb_get_v5, gdb_set_v5,
            66, "e2k-v5.xml", 576);
    }
}
