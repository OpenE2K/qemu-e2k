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
#include "target/e2k/cpu.h"
#include "elf.h"

struct SICState *sicregs;

#define SIC_rt_lcfg0    0x10
#define SIC_rt_lcfg1    0x14
#define SIC_rt_lcfg2    0x18
#define SIC_rt_lcfg3    0x1c

#define SIC_rt_mhi0     0x20
#define SIC_rt_mhi1     0x24
#define SIC_rt_mhi2     0x28
#define SIC_rt_mhi3     0x2c

#define SIC_rt_mlo0     0x30
#define SIC_rt_mlo1     0x34
#define SIC_rt_mlo2     0x38
#define SIC_rt_mlo3     0x3c

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

    index = addr & (E2K_SICREGS_SIZE - 1);
    switch (index) {
    case SIC_rt_lcfg0:
        ret = regs->rt_lcfg0;
        break;
    case SIC_rt_lcfg1:
        ret = regs->rt_lcfg1;
        break;
    case SIC_rt_lcfg2:
        ret = regs->rt_lcfg2;
        break;
    case SIC_rt_lcfg3:
        ret = regs->rt_lcfg3;
        break;
    case SIC_rt_mhi0:
        ret = regs->rt_mhi0.word;
        break;
    case SIC_rt_mhi1:
        ret = regs->rt_mhi1.word;
        break;
    case SIC_rt_mhi2:
        ret = regs->rt_mhi2.word;
        break;
    case SIC_rt_mhi3:
        ret = regs->rt_mhi3.word;
        break;
    case SIC_rt_mlo0:
        ret = regs->rt_mlo0.word;
        break;
    case SIC_rt_mlo1:
        ret = regs->rt_mlo1.word;
        break;
    case SIC_rt_mlo2:
        ret = regs->rt_mlo2.word;
        break;
    case SIC_rt_mlo3:
        ret = regs->rt_mlo3.word;
        break;
    case SIC_rt_msi:
        ret = regs->rt_msi & 0xffffffff;
        break;
    case SIC_rt_msi_h:
        ret = regs->rt_msi >> 32;
        break;
    case SIC_rt_pcim0:
        ret = regs->rt_pcim0.word;
        break;
    case SIC_rt_pcim1:
        ret = regs->rt_pcim1.word;
        break;
    case SIC_rt_pcim2:
        ret = regs->rt_pcim2.word;
        break;
    case SIC_rt_pcim3:
        ret = regs->rt_pcim3.word;
        break;
    case SIC_rt_pciio0:
        ret = regs->rt_pciio0.word;
        break;
    case SIC_rt_pciio1:
        ret = regs->rt_pciio1.word;
        break;
    case SIC_rt_pciio2:
        ret = regs->rt_pciio2.word;
        break;
    case SIC_rt_pciio3:
        ret = regs->rt_pciio3.word;
        break;
    case SIC_rt_pcimp_b0:
        ret = regs->rt_pcimp_b0.addr;
        break;
    case SIC_rt_pcimp_b1:
        ret = regs->rt_pcimp_b1.addr;
        break;
    case SIC_rt_pcimp_b2:
        ret = regs->rt_pcimp_b2.addr;
        break;
    case SIC_rt_pcimp_b3:
        ret = regs->rt_pcimp_b3.addr;
        break;
    case SIC_rt_pcimp_e0:
        ret = regs->rt_pcimp_e0.addr;
        break;
    case SIC_rt_pcimp_e1:
        ret = regs->rt_pcimp_e1.addr;
        break;
    case SIC_rt_pcimp_e2:
        ret = regs->rt_pcimp_e2.addr;
        break;
    case SIC_rt_pcimp_e3:
        ret = regs->rt_pcimp_e3.addr;
        break;
    case SIC_rt_pcicfgb:
        ret = regs->rt_pcicfgb.word;
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
        ;
        break;
    default:
        qemu_log_mask(LOG_UNIMP, "%s: unknown SIC register 0x%x\n", __func__, index);
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

    index = addr & (E2K_SICREGS_SIZE - 1);

    switch (index) {
    case SIC_rt_msi:
        regs->rt_msi = (regs->rt_msi & 0xffffffff00000000) | val;
        break;
    case SIC_rt_msi_h:
        regs->rt_msi = (regs->rt_msi & 0x00000000ffffffff) | (val << 32);
        break;
    case SIC_rt_mhi0:
        regs->rt_mhi0.word = val;
        break;
    case SIC_rt_mhi1:
        regs->rt_mhi1.word = val;
        break;
    case SIC_rt_mhi2:
        regs->rt_mhi2.word = val;
        break;
    case SIC_rt_mhi3:
        regs->rt_mhi3.word = val;
        break;
    case SIC_rt_mlo0:
        regs->rt_mlo0.word = val;
        break;
    case SIC_rt_mlo1:
        regs->rt_mlo1.word = val;
        break;
    case SIC_rt_mlo2:
        regs->rt_mlo2.word = val;
        break;
    case SIC_rt_mlo3:
        regs->rt_mlo3.word = val;
        break;
    case SIC_rt_pcim0:
        regs->rt_pcim0.word = val;
        break;
    case SIC_rt_pcim1:
        regs->rt_pcim1.word = val;
        break;
    case SIC_rt_pcim2:
        regs->rt_pcim2.word = val;
        break;
    case SIC_rt_pcim3:
        regs->rt_pcim3.word = val;
        break;
    case SIC_rt_pciio0:
        regs->rt_pciio0.word = val;
        break;
    case SIC_rt_pciio1:
        regs->rt_pciio1.word = val;
        break;
    case SIC_rt_pciio2:
        regs->rt_pciio2.word = val;
        break;
    case SIC_rt_pciio3:
        regs->rt_pciio3.word = val;
        break;
    case SIC_rt_pcimp_b0:
        regs->rt_pcimp_b0.addr = val;
        break;
    case SIC_rt_pcimp_b1:
        regs->rt_pcimp_b1.addr = val;
        break;
    case SIC_rt_pcimp_b2:
        regs->rt_pcimp_b2.addr = val;
        break;
    case SIC_rt_pcimp_b3:
        regs->rt_pcimp_b3.addr = val;
        break;
    case SIC_rt_pcimp_e0:
        regs->rt_pcimp_e0.addr = val;
        break;
    case SIC_rt_pcimp_e1:
        regs->rt_pcimp_e1.addr = val;
        break;
    case SIC_rt_pcimp_e2:
        regs->rt_pcimp_e2.addr = val;
        break;
    case SIC_rt_pcimp_e3:
        regs->rt_pcimp_e3.addr = val;
        break;
    case SIC_rt_pcicfgb:
        regs->rt_pcicfgb.word = val;
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
        qemu_log_mask(LOG_UNIMP, "%s: unknown SIC register 0x%x\n", __func__, index);
        break;
    }

    trace_sic_mem_writel(addr, val);
}

static const MemoryRegionOps sic_io_ops = {
    .read = sic_mem_read,
    .write = sic_mem_write,
    .impl = {
        .min_access_size = 1,
        .max_access_size = 4,
    },
    .valid = {
        .min_access_size = 1,
        .max_access_size = 4,
    },
    .endianness = DEVICE_NATIVE_ENDIAN,
};

static void sic_reset(SICState *sic)
{
    memset(sic, 0, sizeof(*sic));

    sic->rt_lcfg0 = 0xb; /* BSP node */
    sic->rt_lcfg1 = 0x30;
    sic->rt_lcfg2 = 0x30;
    sic->rt_lcfg3 = 0x30;

    sic->rt_mhi0.E2K_RT_MHI_reg =
        sic->rt_mhi1.E2K_RT_MHI_reg =
        sic->rt_mhi2.E2K_RT_MHI_reg =
        sic->rt_mhi3.E2K_RT_MHI_reg = 0;
    sic->rt_mhi0.E2K_RT_MHI_bgn =
        sic->rt_mhi1.E2K_RT_MHI_bgn =
        sic->rt_mhi2.E2K_RT_MHI_bgn =
        sic->rt_mhi3.E2K_RT_MHI_bgn = ~0;
    sic->rt_mhi0.E2K_RT_MHI_end =
        sic->rt_mhi1.E2K_RT_MHI_end =
        sic->rt_mhi2.E2K_RT_MHI_end =
        sic->rt_mhi3.E2K_RT_MHI_end = 0;

    sic->rt_mlo0.E2K_RT_MLO_reg =
        sic->rt_mlo1.E2K_RT_MLO_reg =
        sic->rt_mlo2.E2K_RT_MLO_reg =
        sic->rt_mlo3.E2K_RT_MLO_reg = 0;
    sic->rt_mlo0.E2K_RT_MLO_bgn =
        sic->rt_mlo1.E2K_RT_MLO_bgn =
        sic->rt_mlo2.E2K_RT_MLO_bgn =
        sic->rt_mlo3.E2K_RT_MLO_bgn = ~0;
    sic->rt_mlo0.E2K_RT_MLO_end =
        sic->rt_mlo1.E2K_RT_MLO_end =
        sic->rt_mlo2.E2K_RT_MLO_end =
        sic->rt_mlo3.E2K_RT_MLO_end = 0;

    sic->rt_pcim0.E2K_RT_PCIM_reg =
        sic->rt_pcim1.E2K_RT_PCIM_reg =
        sic->rt_pcim2.E2K_RT_PCIM_reg =
        sic->rt_pcim3.E2K_RT_PCIM_reg = 0;
    sic->rt_pcim0.E2K_RT_PCIM_bgn =
        sic->rt_pcim1.E2K_RT_PCIM_bgn =
        sic->rt_pcim2.E2K_RT_PCIM_bgn =
        sic->rt_pcim3.E2K_RT_PCIM_bgn = ~0;
    sic->rt_pcim0.E2K_RT_PCIM_end =
        sic->rt_pcim1.E2K_RT_PCIM_end =
        sic->rt_pcim2.E2K_RT_PCIM_end =
        sic->rt_pcim3.E2K_RT_PCIM_end = 0;

    sic->rt_pciio0.E2K_RT_PCIIO_reg =
        sic->rt_pciio1.E2K_RT_PCIIO_reg =
        sic->rt_pciio2.E2K_RT_PCIIO_reg =
        sic->rt_pciio3.E2K_RT_PCIIO_reg = 0;
    sic->rt_pciio0.E2K_RT_PCIIO_bgn =
        sic->rt_pciio1.E2K_RT_PCIIO_bgn =
        sic->rt_pciio2.E2K_RT_PCIIO_bgn =
        sic->rt_pciio3.E2K_RT_PCIIO_bgn = ~0;
    sic->rt_pciio0.E2K_RT_PCIIO_end =
        sic->rt_pciio1.E2K_RT_PCIIO_end =
        sic->rt_pciio2.E2K_RT_PCIIO_end =
        sic->rt_pciio3.E2K_RT_PCIIO_end = 0;

    sic->rt_pcimp_b0.E2K_RT_PCIMP_reg =
        sic->rt_pcimp_b1.E2K_RT_PCIMP_reg =
        sic->rt_pcimp_b2.E2K_RT_PCIMP_reg =
        sic->rt_pcimp_b3.E2K_RT_PCIMP_reg = 0;
    sic->rt_pcimp_b0.E2K_RT_PCIMP_bgn =
        sic->rt_pcimp_b1.E2K_RT_PCIMP_bgn =
        sic->rt_pcimp_b2.E2K_RT_PCIMP_bgn =
        sic->rt_pcimp_b3.E2K_RT_PCIMP_bgn = ~0;

    sic->rt_pcimp_e0.E2K_RT_PCIMP_reg =
        sic->rt_pcimp_e1.E2K_RT_PCIMP_reg =
        sic->rt_pcimp_e2.E2K_RT_PCIMP_reg =
        sic->rt_pcimp_e3.E2K_RT_PCIMP_reg = 0;

    sic->rt_pcicfgb.E2K_RT_PCICFGB_bgn = 0x8; /* see E2K_PCICFG_BASE */
}

void sic_init(E2KMachineState *ms)
{
    sic_reset(&ms->sicregs);
    memory_region_init_io(&ms->sicregion, OBJECT(ms), &sic_io_ops, ms, "sic-nbsr",
        E2K_SICREGS_SIZE);
    memory_region_add_subregion(get_system_memory(), E2K_SICREGS_BASE, &ms->sicregion);
}
