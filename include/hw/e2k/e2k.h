/*
 * Copyright (c) 2021 Denis Drakhnya
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

#ifndef HW_E2K_H
#define HW_E2K_H

#include "qemu-common.h"

#include "hw/boards.h"
#include "qom/object.h"
#include "hw/e2k/ioapic.h"
#include "hw/e2k/iohub.h"
#include "hw/e2k/bootinfo.h"

#define E2K_LOWMEM 0x80000000
#define E2K_HIMEM  0x400000000ULL

/*
 *   Read/Write RT_PCIIOj Regs
 */
typedef unsigned int    e2k_rt_pciio_t; /* Read/write pointer (32 bits) */
typedef struct e2k_rt_pciio_fields {
    e2k_rt_pciio_t   unused1 :  12;                 /* [11:0] */
    e2k_rt_pciio_t   bgn     :  4;                  /* [15:12] */
    e2k_rt_pciio_t   unused2 :  12;                 /* [27:16] */
    e2k_rt_pciio_t   end     :  4;                  /* [31:28] */
} e2k_rt_pciio_fields_t;
typedef union e2k_rt_pciio_struct {             /* Structure of lower word */
    e2k_rt_pciio_fields_t   fields;         /* as fields */
    e2k_rt_pciio_t          word;           /* as entire register */
} e2k_rt_pciio_struct_t;

#define E2K_SIC_ALIGN_RT_PCIIO  12                      /* 4 Kb */
#define E2K_SIC_SIZE_RT_PCIIO   (1 << E2K_SIC_ALIGN_RT_PCIIO)
#define E2K_RT_PCIIO_bgn        fields.bgn
#define E2K_RT_PCIIO_end        fields.end
#define E2K_RT_PCIIO_reg        word

/*
 *   Read/Write RT_PCIMj Regs
 */
typedef unsigned int    e2k_rt_pcim_t;  /* Read/write pointer (32 bits) */
typedef struct e2k_rt_pcim_fields {
    e2k_rt_pcim_t   unused1 :  11;                  /* [10:0] */
    e2k_rt_pcim_t   bgn     :  5;                   /* [15:11] */
    e2k_rt_pcim_t   unused2 :  11;                  /* [26:16] */
    e2k_rt_pcim_t   end     :  5;                   /* [31:27] */
} e2k_rt_pcim_fields_t;
typedef union e2k_rt_pcim_struct {              /* Structure of lower word */
    e2k_rt_pcim_fields_t    fields;         /* as fields */
    e2k_rt_pcim_t           word;           /* as entire register */
} e2k_rt_pcim_struct_t;

#define E2K_SIC_ALIGN_RT_PCIM   27                      /* 128 Mb */
#define E2K_SIC_SIZE_RT_PCIM    (1 << E2K_SIC_ALIGN_RT_PCIM)
#define E2K_RT_PCIM_bgn         fields.bgn
#define E2K_RT_PCIM_end         fields.end
#define E2K_RT_PCIM_reg         word

/*
 *   Read/Write RT_PCIMPj Regs
 */
typedef unsigned int    e2k_rt_pcimp_t; /* Read/write pointer (32 bits) */
typedef struct e2k_rt_pcimp_struct {
    e2k_rt_pcimp_t  addr;           /* [PA_MSB: 0] */
} e2k_rt_pcimp_struct_t;

#define E2K_SIC_ALIGN_RT_PCIMP  27                      /* 128 Mb */
#define E2K_SIC_SIZE_RT_PCIMP   (1 << E2K_SIC_ALIGN_RT_PCIMP)
#define E2K_RT_PCIMP_bgn        addr
#define E2K_RT_PCIMP_end        addr
#define E2K_RT_PCIMP_reg        addr

/*
 *   Read/Write RT_PCICFGB Reg
 */
typedef unsigned int    e2k_rt_pcicfgb_t;       /* Read/write pointer (32 bits) */
typedef struct e2k_rt_pcicfgb_fields {
    e2k_rt_pcicfgb_t        unused1 :  3;                   /* [2:0] */
    e2k_rt_pcicfgb_t        bgn     :  18;                  /* [20:3] */
    e2k_rt_pcicfgb_t        unused2 :  11;                  /* [31:21] */
} e2k_rt_pcicfgb_fields_t;
typedef union e2k_rt_pcicfgb_struct {           /* Structure of lower word */
    e2k_rt_pcicfgb_fields_t fields;         /* as fields */
    e2k_rt_pcicfgb_t        word;           /* as entire register */
} e2k_rt_pcicfgb_struct_t;

#define E2K_SIC_ALIGN_RT_PCICFGB        28              /* 256 Mb */
#define E2K_SIC_SIZE_RT_PCICFGB         (1 << E2K_SIC_ALIGN_RT_PCICFGB)
#define E2K_RT_PCICFGB_bgn              fields.bgn
#define E2K_RT_PCICFGB_reg              word
#define RT_PCICFGB_NODE_BITS    2       /* 4 nodes */
#define RT_PCICFGB_IOLINK_BITS  0       /* 0 IO links per node */

/*
 *   Read/Write RT_MLOj Regs
 */
typedef unsigned int    e2k_rt_mlo_t;   /* Read/write pointer (32 bits) */
typedef struct e2k_rt_mlo_fields {
    e2k_rt_mlo_t   unused1  :  11;                  /* [10:0] */
    e2k_rt_mlo_t   bgn      :  5;                   /* [15:11] */
    e2k_rt_mlo_t   unused2  :  11;                  /* [26:16] */
    e2k_rt_mlo_t   end      :  5;                   /* [31:27] */
} e2k_rt_mlo_fields_t;
typedef union e2k_rt_mlo_struct {               /* Structure of lower word */
    e2k_rt_mlo_fields_t     fields;         /* as fields */
    e2k_rt_mlo_t            word;           /* as entire register */
} e2k_rt_mlo_struct_t;

#define E2K_SIC_ALIGN_RT_MLO    27              /* 128 Mb */
#define E2K_SIC_SIZE_RT_MLO     (1 << E2K_SIC_ALIGN_RT_MLO)
#define E2K_RT_MLO_bgn          fields.bgn
#define E2K_RT_MLO_end          fields.end
#define E2K_RT_MLO_reg          word

/*
 *   Read/Write RT_MHIj Regs
 */
typedef unsigned int    e2k_rt_mhi_t;   /* Read/write pointer (32 bits) */
typedef struct e2k_rt_mhi_fields {
    e2k_rt_mhi_t    bgn     : 16;                   /* [15: 0] */
    e2k_rt_mhi_t    end     : 16;                   /* [31:16] */
} e2k_rt_mhi_fields_t;
typedef union e2k_rt_mhi_struct {               /* Structure of lower word */
    e2k_rt_mhi_fields_t     fields;         /* as fields */
    e2k_rt_mhi_t            word;           /* as entire register */
} e2k_rt_mhi_struct_t;

#define E2K_SIC_ALIGN_RT_MHI    32              /* 4 Gb */
#define E2K_SIC_SIZE_RT_MHI     (1UL << E2K_SIC_ALIGN_RT_MHI)
#define E2K_RT_MHI_bgn          fields.bgn
#define E2K_RT_MHI_end          fields.end
#define E2K_RT_MHI_reg          word


typedef struct SICState {
    uint32_t rt_lcfg0;
    uint32_t rt_lcfg1;
    uint32_t rt_lcfg2;
    uint32_t rt_lcfg3;
    
    e2k_rt_mhi_struct_t rt_mhi0;
    e2k_rt_mhi_struct_t rt_mhi1;
    e2k_rt_mhi_struct_t rt_mhi2;
    e2k_rt_mhi_struct_t rt_mhi3;
    
    e2k_rt_mlo_struct_t rt_mlo0;
    e2k_rt_mlo_struct_t rt_mlo1;
    e2k_rt_mlo_struct_t rt_mlo2;
    e2k_rt_mlo_struct_t rt_mlo3;
    
    e2k_rt_pcim_struct_t rt_pcim0;
    e2k_rt_pcim_struct_t rt_pcim1;
    e2k_rt_pcim_struct_t rt_pcim2;
    e2k_rt_pcim_struct_t rt_pcim3;
    e2k_rt_pciio_struct_t rt_pciio0;
    e2k_rt_pciio_struct_t rt_pciio1;
    e2k_rt_pciio_struct_t rt_pciio2;
    e2k_rt_pciio_struct_t rt_pciio3;
    e2k_rt_pcimp_struct_t rt_pcimp_b0;
    e2k_rt_pcimp_struct_t rt_pcimp_b1;
    e2k_rt_pcimp_struct_t rt_pcimp_b2;
    e2k_rt_pcimp_struct_t rt_pcimp_b3;
    e2k_rt_pcimp_struct_t rt_pcimp_e0;
    e2k_rt_pcimp_struct_t rt_pcimp_e1;
    e2k_rt_pcimp_struct_t rt_pcimp_e2;
    e2k_rt_pcimp_struct_t rt_pcimp_e3;
    e2k_rt_pcicfgb_struct_t rt_pcicfgb;
    uint64_t rt_msi;
    uint32_t iommu_ctrl;
    uint64_t iommu_ptbar;
    uint64_t iommu_dtbar;
    uint64_t iommu_err;
    uint64_t iommu_err_info;
    uint32_t prepic_ctrl2;
    uint32_t prepic_err_stat;
    uint32_t prepic_err_int;
    uint32_t prepic_linp0;
    uint32_t prepic_linp1;
    uint32_t prepic_linp2;
    uint32_t prepic_linp3;
    uint32_t prepic_linp4;
    uint32_t prepic_linp5;
} SICState;

typedef struct GSIState {
    qemu_irq ioapic_irq[IOAPIC_NUM_PINS];
} GSIState;

struct E2KMachineClass {
    /*< private >*/
    MachineClass parent;

    /*< public >*/
};

struct E2KMachineState {
    /*< private >*/
    MachineClass parent;

    /*< public >*/
    qemu_irq *pic;
    GSIState *gsi_state;
    AddressSpace *ioapic_as;
    
    MemoryRegion sicregion;
    struct SICState sicregs;
    
    IOHubPCIState *iohub_sys_bus;
    
    ram_addr_t above_4g_mem_size;
    ram_addr_t below_4g_mem_size;
    
    bootblock_struct_t *bootblock;
};

extern struct SICState *sicregs; /* TODO: remove this!!!!!!!! */
extern IOHubPCIState *iohub_sys_bus; /* AND THIS!!!! */

#define TYPE_E2K_MACHINE    MACHINE_TYPE_NAME("e2k")
OBJECT_DECLARE_TYPE(E2KMachineState, E2KMachineClass, E2K_MACHINE)

void sic_init(E2KMachineState *ms);
void iohub_pci_create(E2KMachineState *e2kms, qemu_irq *pic,
    MemoryRegion *address_space_mem, MemoryRegion *system_io);
void setup_iohub_devices(E2KMachineState *ms);

static inline uint64_t nbsr_get_pcicfg_base(void)
{
    return ((uint64_t)sicregs->rt_pcicfgb.E2K_RT_PCICFGB_bgn) << (E2K_SIC_ALIGN_RT_PCICFGB + RT_PCICFGB_NODE_BITS + RT_PCICFGB_IOLINK_BITS);
}

#endif /* HW_E2K_H */
