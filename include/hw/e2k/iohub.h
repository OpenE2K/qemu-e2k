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
#ifndef HW_E2K_IOHUB_H
#define HW_E2K_IOHUB_H

#include "hw/pci/pci.h"
#include "hw/pci/pci_host.h"
#include "hw/pci/pci_bus.h"

/* Chipset PCI */
#define TYPE_IOHUB_PCI_HOST_BRIDGE "iohub-pci-host-bridge"
OBJECT_DECLARE_TYPE(PCIHostStateIOHub, PCIHostBridgeClass, IOHUB_PCI_HOST_BRIDGE)
struct PCIHostStateIOHub
{
    PCIHostState parent;
    PCIBus bus;
    qemu_irq *pic;
    
    PCIBridge *root_bridge;
};

/* PCI devices/buses */

#define PCI_VENDOR_ID_MCST 0x1fff

/* MCST Virtual Bridge 00:00.0 */
#define PCI_DEVICE_ID_MCST_ROOT_BUS 0x8017
#define IOHUB_PCI_ROOT_BUS 0
#define IOHUB_PCI_ROOT_BUS_SLOT 0
#define IOHUB_PCI_ROOT_BUS_FUNC 0
#define IOHUB_PCI_ROOT_BUS_DEVFN \
    PCI_DEVFN(IOHUB_PCI_ROOT_BUS_SLOT, IOHUB_PCI_ROOT_BUS_FUNC)
#define TYPE_IOHUB_PCI_ROOT_BUS "iohub-pci-root-bus"

/* MCST PCI Bridge 01:00.0 */
#define PCI_DEVICE_ID_MCST_BRIDGE 0x8000
#define IOHUB_PCI_BRIDGE 1
#define IOHUB_PCI_BRIDGE_SLOT 0
#define IOHUB_PCI_BRIDGE_FUNC 0
#define IOHUB_PCI_BRIDGE_DEVFN \
    PCI_DEVFN(IOHUB_PCI_BRIDGE_SLOT, IOHUB_PCI_BRIDGE_FUNC)
#define TYPE_IOHUB_PCI_BRIDGE "iohub-pci-bridge"

void iohub_init(E2KMachineState *ms);

#endif /* HW_E2K_IOHUB_H */
