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
#include "qemu/osdep.h"
#include "qemu/datadir.h"
#include "qemu/log.h"
#include "qapi/error.h"
#include "trace.h"

#include "sysemu/cpus.h"
#include "sysemu/reset.h"
#include "hw/loader.h"
#include "hw/e2k/e2k.h"
#include "hw/e2k/iohub.h"
#include "target/e2k/cpu.h"
#include "elf.h"

#define ES2_NSR_AREA_PHYS_BASE      0x0000000110000000UL    /* node 0 */
#define ES2_NBSR_AREA_SIZE          0x0000000000100000UL

enum SICRegsAddrs
{
    SIC_node_offset = 0x4,

    SIC_st_p        = 0x0,

    SIC_rt_lcfg0    = 0x10, /* node configuration, max nodes 4 */

    SIC_rt_mhi0     = 0x20,

    SIC_rt_mlo0     = 0x30,

    SIC_rt_pcim0    = 0x40,

    SIC_rt_pciio0   = 0x50,

    SIC_rt_ioapic0  = 0x60,

    SIC_rt_pcicfgb  = 0x90,

    SIC_st_core     = 0x100, /* core configuration, max cores 16 */

    /* IO link & RDMA */
    SIC_iol_csr     = 0x900,

    SIC_unknown_0x1040 = 0x1040,

    SIC_unknown_0x1050 = 0x1050,

    SIC_rt_ioapic10 = 0x1060,

    /* Second IO link */
    SIC_iol_csr1    = 0x1900,
};

static uint64_t sic_mem_read(void *opaque, hwaddr addr, unsigned size)
{
//    E2KMachineState *ms = opaque;
    uint64_t val;
    int index;

    if (size < 4) {
        return 0;
    }

    index = addr & (ES2_NBSR_AREA_SIZE - 1);
    switch (index) {
    case SIC_rt_pcim0:
        val = 0;
        break;
    case SIC_rt_pciio0:
        val = 0;
        break;
    case SIC_rt_pcicfgb:
        // TODO: io page address [48:12]
        // TODO: page + 0x8044 = 16-bit reg
        val = 0xdeadbeef000 >> 12;
        break;
    default:
        val = 0;
        break;
    }

    trace_sic_mem_readl(addr, val);
    return val;
}

static void sic_mem_write(void *opaque, hwaddr addr, uint64_t val,
    unsigned size)
{
//    E2KMachineState *ms = opaque;

     trace_sic_mem_writel(addr, val);
}

static const MemoryRegionOps sic_io_ops = {
    .read = sic_mem_read,
    .write = sic_mem_write,
    .impl.min_access_size = 1,
    .impl.max_access_size = 4,
    .valid.min_access_size = 1,
    .valid.max_access_size = 4,
    .endianness = DEVICE_NATIVE_ENDIAN,
};

void sic_init(E2KMachineState *ms)
{
    MemoryRegion *io_memory;
    const hwaddr base = ES2_NSR_AREA_PHYS_BASE;
    const uint64_t size = ES2_NBSR_AREA_SIZE;

    io_memory = g_malloc(sizeof(*io_memory));
    memory_region_init_io(io_memory, OBJECT(ms), &sic_io_ops, ms, "sic-msi",
        size);
    memory_region_add_subregion(get_system_memory(), base, io_memory);
}
