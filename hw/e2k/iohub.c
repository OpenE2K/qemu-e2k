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
#include "hw/pci/pci.h"
#include "hw/pci/pci_bridge.h"
#include "hw/sysbus.h"
#include "hw/irq.h"
#include "hw/e2k/e2k.h"
#include "hw/e2k/iohub.h"

#define TYPE_PCI_BRIDGE_IOHUB "iohub-pci"

#define PCI_VENDOR_ID_MCST             0x1fff
#define PCI_DEVICE_ID_MCST_HOST_BRIDGE 0x8017

#define GSI_NUM_PINS IOAPIC_NUM_PINS
static void gsi_handler(void *opaque, int n, int level)
{
    GSIState *s = opaque;
    
    trace_e2k_gsi_interrupt(n, level);
    
    qemu_set_irq(s->ioapic_irq[n], level);
}

static void ioapic_init_gsi(GSIState *s, const char *parent_name)
{
    DeviceState *dev;
    SysBusDevice *d;
    int i;
    
    assert(parent_name);
    dev = qdev_new(TYPE_IOAPIC);
#if 0
    object_property_add_child(object_resolve_path(parent_name, NULL),
                              TYPE_IOAPIC, OBJECT(dev));
#endif
    d = SYS_BUS_DEVICE(dev);
    sysbus_realize_and_unref(d, &error_fatal);
    sysbus_mmio_map(d, 0, IO_APIC_DEFAULT_ADDRESS);
    
    for (i = 0; i < IOAPIC_NUM_PINS; i++) {
        s->ioapic_irq[i] = qdev_get_gpio_in(dev, i);
    }
}

void iohub_init_pci(qemu_irq *pic, MemoryRegion *address_space_mem,
                    MemoryRegion *system_io)
{
    
}

void iohub_init(E2KMachineState *ms)
{
    GSIState *s;
    
    s = g_new0(GSIState, 1);
    
    ms->pic = qemu_allocate_irqs(gsi_handler, s, GSI_NUM_PINS);
    
    iohub_init_pci(ms->pic, get_system_memory(), get_system_io());
    
    ioapic_init_gsi(s, TYPE_IOHUB_PCI_HOST_BRIDGE);
}

static void iohub_pci_realize(DeviceState *dev, Error **errp)
{
#if 0
    PCIHostStateIOHub *s = IOHUB_PCI_HOST_BRIDGE(dev);
    
    qemu_log("%s: created IOHub host bridge %s at %p\n",
             __FUNCTION__, object_get_typename(OBJECT(s)), s);
    pci_root_bus_new_inplace(&s->bus, sizeof(s->bus), dev,
                             TYPE_IOHUB_PCI_ROOT_BUS,
                             &s->addr_mem, &s->addr_io,
                             
#endif                        
}

static void iohub_pci_class_init(ObjectClass *oc, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(oc);
    
    set_bit(DEVICE_CATEGORY_BRIDGE, dc->categories);
    dc->realize = iohub_pci_realize;
    dc->fw_name = "pci";
}

static TypeInfo iohub_pci_info = {
    .name = TYPE_IOHUB_PCI_HOST_BRIDGE,
    .parent = TYPE_PCI_HOST_BRIDGE,
    .instance_size = sizeof(PCIHostStateIOHub),
    .class_init = iohub_pci_class_init,
};

static void iohub_pci_bridge_realize(PCIDevice *dev, Error **errp)
{
    pci_set_word(dev->config + PCI_COMMAND, PCI_COMMAND_MEMORY);
    pci_bridge_initfn(dev, TYPE_PCI_BUS);
    pci_set_word(dev->config + PCI_STATUS,
            PCI_STATUS_FAST_BACK | PCI_STATUS_66MHZ |
            PCI_STATUS_DEVSEL_MEDIUM);
}

static void iohub_pci_bridge_class_init(ObjectClass *oc, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(oc);
    PCIDeviceClass *pci = PCI_DEVICE_CLASS(oc);
    
    pci->realize = iohub_pci_bridge_realize;
    pci->exit = pci_bridge_exitfn;
    pci->vendor_id = PCI_VENDOR_ID_MCST;
    pci->device_id = PCI_DEVICE_ID_MCST_HOST_BRIDGE;
    pci->revision = 0x10;
    pci->is_bridge = 1;
    dc->reset = pci_bridge_reset;
}

static TypeInfo iohub_pci_bridge_info = {
    .name = TYPE_IOHUB_PCI_BRIDGE,
    .parent = TYPE_PCI_BRIDGE,
    .instance_size = sizeof(PCIBridge),
    .class_init = iohub_pci_bridge_class_init,
    .interfaces = (InterfaceInfo[]) {
        { INTERFACE_CONVENTIONAL_PCI_DEVICE },
        { },
    },
};

static void iohub_pci_bridge_register_types(void)
{
    type_register_static(&iohub_pci_info);
    type_register_static(&iohub_pci_bridge_info);
}

type_init(iohub_pci_bridge_register_types)
