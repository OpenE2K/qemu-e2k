/*
 * Copyright (c) 2021 Alibek Omarov
 *
 * System Interchange Controller emulation
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

#define SIC_rt_msi      0xb0
#define SIC_rt_msi_h    0xb4
#define SIC_rt_pcim0    0x40
#define SIC_rt_pcim1    0x44
#define SIC_rt_pcim2    0x48
#define SIC_rt_pcim3    0x4c
#define SIC_rt_pciio0   0x50
#define SIC_rt_pciio1   0x54
#define SIC_rt_pciio2   0x58
#define SIC_rt_pciio3   0x5c
#define SIC_rt_pcimp_b0 0x70
#define SIC_rt_pcimp_b1 0x74
#define SIC_rt_pcimp_b2 0x78
#define SIC_rt_pcimp_b3 0x7c
#define SIC_rt_pcimp_e0 0x80
#define SIC_rt_pcimp_e1 0x84
#define SIC_rt_pcimp_e2 0x88
#define SIC_rt_pcimp_e3 0x8c
#define SIC_rt_pcicfgb  0x90
#define SIC_prepic_ctrl2    0x8030 
#define SIC_prepic_err_stat 0x8040 
#define SIC_prepic_err_int  0x8060 
#define SIC_prepic_linp0    0x8c00
#define SIC_prepic_linp1    0x8c04
#define SIC_prepic_linp2    0x8c08
#define SIC_prepic_linp3    0x8c0c
#define SIC_prepic_linp4    0x8c10
#define SIC_prepic_linp5    0x8c14 
#define SIC_iommu_ctrl      0x0380 
#define SIC_iommu_ba_lo     0x0390 
#define SIC_iommu_dtba_lo   0x0398 
#define SIC_iommu_err       0x03b0 
#define SIC_iommu_err_info_lo 0x03b8 

static uint64_t sic_mem_read(void *opaque, hwaddr addr, unsigned size)
{
    E2KMachineState *ms = opaque;
    SICState *regs = &ms->sicregs;
    uint64_t ret;
    int index;

    index = addr & (ES2_NBSR_AREA_SIZE - 1);
    switch (index) {
    case SIC_rt_msi:
        ret = regs->rt_msi & 0xffffffff;
        break;
    case SIC_rt_msi_h:
        ret = regs->rt_msi >> 32;
        break;
    case SIC_rt_pcim0:
        ret = regs->rt_pcim0;
        break;
    case SIC_rt_pcim1:
        ret = regs->rt_pcim1;
        break;
    case SIC_rt_pcim2:
        ret = regs->rt_pcim2;
        break;
    case SIC_rt_pcim3:
        ret = regs->rt_pcim3;
        break;
    case SIC_rt_pciio0:
        ret = regs->rt_pciio0;
        break;
    case SIC_rt_pciio1:
        ret = regs->rt_pciio1;
        break;
    case SIC_rt_pciio2:
        ret = regs->rt_pciio2;
        break;
    case SIC_rt_pciio3:
        ret = regs->rt_pciio3;
        break;
    case SIC_rt_pcimp_b0:
        ret = regs->rt_pcimp_b0;
        break;
    case SIC_rt_pcimp_b1:
        ret = regs->rt_pcimp_b1;
        break;
    case SIC_rt_pcimp_b2:
        ret = regs->rt_pcimp_b2;
        break;
    case SIC_rt_pcimp_b3:
        ret = regs->rt_pcimp_b3;
        break;
    case SIC_rt_pcimp_e0:
        ret = regs->rt_pcimp_e0;
        break;
    case SIC_rt_pcimp_e1:
        ret = regs->rt_pcimp_e1;
        break;
    case SIC_rt_pcimp_e2:
        ret = regs->rt_pcimp_e2;
        break;
    case SIC_rt_pcimp_e3:
        ret = regs->rt_pcimp_e3;
        break;
    case SIC_rt_pcicfgb:
        ret = regs->rt_pcicfgb;
        break;
    case SIC_prepic_ctrl2:
        ret = regs->prepic_ctrl2;
        break;
    case SIC_prepic_err_stat:
        ret = regs->prepic_err_stat;
        break;
    case SIC_prepic_err_int:
        ret = regs->prepic_err_int;
        break;
    case SIC_prepic_linp0:
        ret = regs->prepic_linp0;
        break;
    case SIC_prepic_linp1:
        ret = regs->prepic_linp1;
        break;
    case SIC_prepic_linp2:
        ret = regs->prepic_linp2;
        break;
    case SIC_prepic_linp3:
        ret = regs->prepic_linp3;
        break;
    case SIC_prepic_linp4:
        ret = regs->prepic_linp4;
        break;
    case SIC_prepic_linp5:
        ret = regs->prepic_linp5;
        break;
    case SIC_iommu_err:
        ret = regs->iommu_err;
        break;
    case SIC_iommu_err_info_lo:
        ret = regs->iommu_err_info;
        break;
    default:
        qemu_log_mask(LOG_UNIMP, "%s: unknown SIC register 0x%x\n", __FUNCTION__, index);
        ret = 0;
        break;
    }

    trace_sic_mem_readl(addr, ret);
    return ret;
}

static void sic_mem_write(void *opaque, hwaddr addr, uint64_t val,
    unsigned size)
{
    E2KMachineState *ms = opaque;
    SICState *regs = &ms->sicregs;
    int index;

    index = addr & (ES2_NBSR_AREA_SIZE - 1);

    switch (index) {
    case SIC_rt_msi:
        regs->rt_msi = (regs->rt_msi & 0xffffffff00000000) | val;
        break;
    case SIC_rt_msi_h:
        regs->rt_msi = (regs->rt_msi & 0x00000000ffffffff) | (val << 32);
        break;
    case SIC_rt_pcim0:
        regs->rt_pcim0 = val;
        break;
    case SIC_rt_pcim1:
        regs->rt_pcim1 = val;
        break;
    case SIC_rt_pcim2:
        regs->rt_pcim2 = val;
        break;
    case SIC_rt_pcim3:
        regs->rt_pcim3 = val;
        break;
    case SIC_rt_pciio0:
        regs->rt_pciio0 = val;
        break;
    case SIC_rt_pciio1:
        regs->rt_pciio1 = val;
        break;
    case SIC_rt_pciio2:
        regs->rt_pciio2 = val;
        break;
    case SIC_rt_pciio3:
        regs->rt_pciio3 = val;
        break;
    case SIC_rt_pcimp_b0:
        regs->rt_pcimp_b0 = val;
        break;
    case SIC_rt_pcimp_b1:
        regs->rt_pcimp_b1 = val;
        break;
    case SIC_rt_pcimp_b2:
        regs->rt_pcimp_b2 = val;
        break;
    case SIC_rt_pcimp_b3:
        regs->rt_pcimp_b3 = val;
        break;
    case SIC_rt_pcimp_e0:
        regs->rt_pcimp_e0 = val;
        break;
    case SIC_rt_pcimp_e1:
        regs->rt_pcimp_e1 = val;
        break;
    case SIC_rt_pcimp_e2:
        regs->rt_pcimp_e2 = val;
        break;
    case SIC_rt_pcimp_e3:
        regs->rt_pcimp_e3 = val;
        break;
    case SIC_rt_pcicfgb:
        regs->rt_pcicfgb = val;
        break;
    case SIC_prepic_ctrl2:
        regs->prepic_ctrl2 = val;
        break;
    case SIC_prepic_err_stat:
        regs->prepic_err_stat = val;
        break;
    case SIC_prepic_err_int:
        regs->prepic_err_int = val;
        break;
    case SIC_prepic_linp0:
        regs->prepic_linp0 = val;
        break;
    case SIC_prepic_linp1:
        regs->prepic_linp1 = val;
        break;
    case SIC_prepic_linp2:
        regs->prepic_linp2 = val;
        break;
    case SIC_prepic_linp3:
        regs->prepic_linp3 = val;
        break;
    case SIC_prepic_linp4:
        regs->prepic_linp4 = val;
        break;
    case SIC_prepic_linp5:
        regs->prepic_linp5 = val;
        break;
    case SIC_iommu_ctrl:
        regs->iommu_ctrl = val;
        break;
    case SIC_iommu_ba_lo:
        regs->iommu_ptbar = val;
        break;
    case SIC_iommu_dtba_lo:
        regs->iommu_dtbar = val;
        break;
    default:
        qemu_log_mask(LOG_UNIMP, "%s: unknown SIC register 0x%x\n", __FUNCTION__, index);
        break;
    }
    
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

    memset(&ms->sicregs, 0, sizeof(ms->sicregs));
    
    io_memory = g_malloc(sizeof(*io_memory));
    memory_region_init_io(io_memory, OBJECT(ms), &sic_io_ops, ms, "sic-msi",
        size);
    memory_region_add_subregion(get_system_memory(), base, io_memory);
}
