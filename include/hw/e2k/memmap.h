/*
 * Copyright (c) 2021 Alibek Omarov
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2 or later, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#ifndef HW_E2K_MEMMAP_H
#define HW_E2K_MEMMAP_H

/* Memory map */

/* may be overriden by VGAMEM, these bytes will be lost */
#define E2K_MLO_BASE       0x0
#define E2K_MLO_SIZE       0x080000000 /* 2 GiB */

#define E2K_PCIMEM_BASE    0x080000000 
#define E2K_PCIMEM_SIZE    0x07ec00000 /* 2028 MiB */

/* there goes APIC/EPIC, compatible with x86 */

#define E2K_BIOS_AREA_BASE 0x100000000
#define E2K_BIOS_AREA_SIZE 0x001000000 /* 16 MiB */

#define E2K_IO_AREA_BASE   0x101000000
#define E2K_IO_AREA_SIZE   0x000010000 /* 64 KiB */

#define E2K_SICREGS_BASE   0x110000000
#define E2K_SICREGS_SIZE   0x000100000 /* 1 MiB */

#define E2K_PCICFG_BASE    0x200000000 /* set by SIC_rt_pcicfgb but we hardcode it */
#define E2K_PCICFG_SIZE    0x010000000 /* 256 MiB */

#define E2K_PCIMEMP_BASE   0x210000000 /* Prefetchable MEM */
#define E2K_PCIMEMP_SIZE   0x1f0000000 /* 7936 MiB */

#define E2K_HIMEM_BASE     0x400000000 /* and to the end of address space... */

#endif /* HW_E2K_MEMMAP_H */
