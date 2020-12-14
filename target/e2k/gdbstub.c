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
#include "exec/gdbstub.h"

/* TODO: reverse engineer e2k-linux-gdb register ids */

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
        return gdb_get_reg64(mem_buf, env->regs[E2K_NR_COUNT + n - 3]);
    }
    
    switch (n) {
    case 35: return gdb_get_reg64(mem_buf, 0); // psr
    case 36: return gdb_get_reg64(mem_buf, env->upsr); // upsr
    case 37: return gdb_get_reg64(mem_buf, 0); // oscud_lo
    case 38: return gdb_get_reg64(mem_buf, 0); // oscud_hi
    case 39: return gdb_get_reg64(mem_buf, 0); // osgd_lo
    case 40: return gdb_get_reg64(mem_buf, 0); // osgd_hi
    case 41: return gdb_get_reg64(mem_buf, 0); // osem
    case 42: return gdb_get_reg64(mem_buf, 0); // osr0
    case 43: return gdb_get_reg64(mem_buf, 0); // pfpfr
    case 44: return gdb_get_reg64(mem_buf, 0); // fpcr
    case 45: return gdb_get_reg64(mem_buf, 0); // fpsr
    case 46: return gdb_get_reg64(mem_buf, 0); // usbr
    case 47: return gdb_get_reg64(mem_buf, env->usd.lo); // usd_lo
    case 48: return gdb_get_reg64(mem_buf, env->usd.hi); // usd_hi
    case 49: return gdb_get_reg64(mem_buf, e2k_state_psp_lo(env)); // psp_lo
    case 50: return gdb_get_reg64(mem_buf, e2k_state_psp_hi(env)); // psp_hi
    case 51: return gdb_get_reg64(mem_buf, e2k_state_pshtp(env)); // pshtp
    case 52: return gdb_get_reg64(mem_buf, env->pregs); // pregs
    case 53: return gdb_get_reg64(mem_buf, env->ip); // ip
    case 54: return gdb_get_reg64(mem_buf, env->cr1.lo); // cr1_lo
    case 55: return gdb_get_reg64(mem_buf, env->cr1.hi); // cr1_hi
    case 56: return gdb_get_reg64(mem_buf, 0); // cwd
    case 57: return gdb_get_reg64(mem_buf, e2k_state_pcsp_lo(env)); // pcsp_lo
    case 58: return gdb_get_reg64(mem_buf, e2k_state_pcsp_hi(env)); // pcsp_hi
    case 59: return gdb_get_reg64(mem_buf, env->pcshtp); // pcshtp
    case 60: return gdb_get_reg64(mem_buf, 0); // cud_lo
    case 61: return gdb_get_reg64(mem_buf, 0); // cud_hi
    case 62: return gdb_get_reg64(mem_buf, 0); // gd_lo
    case 63: return gdb_get_reg64(mem_buf, 0); // gd_hi
    case 64: return gdb_get_reg64(mem_buf, 0); // cs_lo
    case 65: return gdb_get_reg64(mem_buf, 0); // cs_hi
    case 66: return gdb_get_reg64(mem_buf, 0); // ds_lo
    case 67: return gdb_get_reg64(mem_buf, 0); // ds_hi
    case 68: return gdb_get_reg64(mem_buf, 0); // es_lo
    case 69: return gdb_get_reg64(mem_buf, 0); // es_hi
    case 70: return gdb_get_reg64(mem_buf, 0); // fs_lo
    case 71: return gdb_get_reg64(mem_buf, 0); // fs_hi
    case 72: return gdb_get_reg64(mem_buf, 0); // gs_lo
    case 73: return gdb_get_reg64(mem_buf, 0); // gs_hi
    case 74: return gdb_get_reg64(mem_buf, 0); // ss_lo
    case 75: return gdb_get_reg64(mem_buf, 0); // ss_hi
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
    case 334: return gdb_get_reg64(mem_buf, e2k_state_wd(env)); // wd
    case 335: return gdb_get_reg64(mem_buf, 0); // unk
    case 336: return gdb_get_reg64(mem_buf, 0); // bgr
    case 337: return gdb_get_reg64(mem_buf, 0); // unk
    case 338: return gdb_get_reg64(mem_buf, 0); // nip
    case 339: return gdb_get_reg64(mem_buf, env->ctprs[0].raw); // ctpr1
    case 340: return gdb_get_reg64(mem_buf, env->ctprs[1].raw); // ctpr2
    case 341: return gdb_get_reg64(mem_buf, env->ctprs[2].raw); // ctpr3
    case 342: return gdb_get_reg64(mem_buf, 0); // eir
    case 343: return gdb_get_reg64(mem_buf, 0); // tr
    case 344: return gdb_get_reg64(mem_buf, 0); // cutd
    case 345: return gdb_get_reg64(mem_buf, 0); // cuir
    case 346: return gdb_get_reg64(mem_buf, 0); // tsd
    case 347: return gdb_get_reg64(mem_buf, env->lsr); // lsr
    case 348: return gdb_get_reg64(mem_buf, env->lsr & LSR_ILCR_MASK); // ilcr
    default:
        break;
    }

    if (349 <= n && n < 356) {
        return gdb_get_reg64(mem_buf, 0); // unk
    }

    if (356 <= n && n < 360) {
        return gdb_get_reg64(mem_buf, 0); // gN tags (tag len is 1 byte)
    }

    if (360 <= n && n < 368) {
        return gdb_get_reg64(mem_buf, 0); // xgN
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
        return gdb_get_reg64(mem_buf, 0); // dam_N
    }

    if (472 <= n && n < 504) {
        return gdb_get_reg64(mem_buf, 0); // sbbp_N
    }

    if (504 <= n && n < 552) {
        return gdb_get_reg64(mem_buf, 0); // mlt_N (3 regs)
    }

    if (n == 552) {
        return gdb_get_reg64(mem_buf, 0); // pcsp_base
    }

    if (n == 553) {
        return gdb_get_reg64(mem_buf, 0); // psp_base
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
