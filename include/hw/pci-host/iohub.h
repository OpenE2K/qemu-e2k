/*
 * Copyright (c) 2021 Alibek Omarov
 *
 * This work is licensed under the terms of the GNU GPL, version 2 or later.
 * See the COPYING file in the top-level directory.
 *
 */

#ifndef HW_PCI_IOHUB_H
#define HW_PCI_IOHUB_H

#include "hw/pci/pci_bus.h"
#include "qom/object.h"

#define TYPE_IOHUB_PCI_HOST_BRIDGE "iohub-pcihost"
#define TYPE_IOHUB_PCI_DEVICE "iohub"

OBJECT_DECLARE_SIMPLE_TYPE(PCIIOHUBState, IOHUB_PCI_DEVICE)

struct PCIIOHUBState {
    /*< private >*/
    PCIDevice parent_obj;
    /*< public >*/

    MemoryRegion *system_memory;
    MemoryRegion *pci_address_space;
};

PCIBus *iohub_init(const char *host_type, const char *pci_type,
                   PCIIOHUBState **piohub_state,
                    MemoryRegion *address_space_mem,
                    MemoryRegion *address_space_io);

PCIBus *find_iohub(void);


#endif
