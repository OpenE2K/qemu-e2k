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
#include "cpu.h"
#include "exec/gdbstub.h"

/* TODO: reverse engineer e2k-linux-gdb register ids */

int e2k_cpu_gdb_read_register(CPUState *cs, GByteArray *mem_buf, int n)
{
    E2KCPU *cpu = E2K_CPU(cs);
    CPUClass *cc = CPU_GET_CLASS(cs);
    CPUE2KState *env = &cpu->env;

    fprintf(stderr, "%s: unknown register %d\n", __FUNCTION__, n);
    return 0;
}

int e2k_cpu_gdb_write_register(CPUState *cs, uint8_t *mem_buf, int n)
{
    E2KCPU *cpu = E2K_CPU(cs);
    CPUClass *cc = CPU_GET_CLASS(cs);
    CPUE2KState *env = &cpu->env;

    fprintf(stderr, "%s: unknown register %d\n", __FUNCTION__, n);

    /* TODO */
    return 0;
}
