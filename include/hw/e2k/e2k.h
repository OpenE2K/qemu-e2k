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

typedef struct SICState {
    uint32_t rt_pcim0;
    uint32_t rt_pcim1;
    uint32_t rt_pcim2;
    uint32_t rt_pcim3;
    uint32_t rt_pciio0;
    uint32_t rt_pciio1;
    uint32_t rt_pciio2;
    uint32_t rt_pciio3;
    uint32_t rt_pcimp_b0;
    uint32_t rt_pcimp_b1;
    uint32_t rt_pcimp_b2;
    uint32_t rt_pcimp_b3;
    uint32_t rt_pcimp_e0;
    uint32_t rt_pcimp_e1;
    uint32_t rt_pcimp_e2;
    uint32_t rt_pcimp_e3;
    uint32_t rt_pcicfgb;
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
    
    struct SICState sicregs;
    
    AddressSpace *ioapic_as;
};

#define TYPE_E2K_MACHINE    MACHINE_TYPE_NAME("e2k")
OBJECT_DECLARE_TYPE(E2KMachineState, E2KMachineClass, E2K_MACHINE)

typedef struct GSIState {
    qemu_irq ioapic_irq[IOAPIC_NUM_PINS];
} GSIState;

void sic_init(E2KMachineState *ms);

#endif /* HW_E2K_H */
