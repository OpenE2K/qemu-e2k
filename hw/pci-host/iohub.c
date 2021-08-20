/*
 * QEMU IOHUB PCI Bridge Emulation
 *
 * Copyright (c) 2021 Alibek Omarov
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "qemu/osdep.h"
#include "hw/e2k/e2k.h"
#include "hw/pci/pci.h"
#include "hw/pci/pci_host.h"
#include "hw/pci-host/iohub.h"
#include "hw/qdev-properties.h"
#include "hw/sysbus.h"
#include "qapi/error.h"
#include "qapi/visitor.h"
#include "qemu/error-report.h"
#include "qom/object.h"

OBJECT_DECLARE_SIMPLE_TYPE(IOHUBState, IOHUB_PCI_HOST_BRIDGE)

struct IOHUBState {
    PCIHostState parent_obj;
};

PCIBus *iohub_init(const char *host_type, const char *pci_type,
                   PCIIOHUBState **piohub_state,
                    MemoryRegion *address_space_mem,
                    MemoryRegion *address_space_io)
{
    DeviceState *dev;
    PCIDevice *d;
    PCIHostState *s;
    PCIBus *b;
    PCIIOHUBState *f;
    MemoryRegion *pci_address_space;
    
    dev = qdev_new(host_type);
    s = PCI_HOST_BRIDGE(dev);
    
    pci_address_space = g_malloc(sizeof(*pci_address_space));
    memory_region_init_alias(pci_address_space, OBJECT(dev),
                             "pcimem", address_space_mem, 0, E2K_PCIMEM_SIZE);
    memory_region_add_subregion(address_space_mem, E2K_PCIMEM_BASE, pci_address_space);
    
    s->bus = b = pci_root_bus_new(dev, NULL, 
                                  pci_address_space, 
                                  address_space_io, 
                                  0, TYPE_PCI_BUS);

    object_property_add_child(qdev_get_machine(), "iohub", OBJECT(dev));
    sysbus_realize_and_unref(SYS_BUS_DEVICE(dev), &error_fatal);
    
    d = pci_create_simple(b, 0, pci_type);
    f = *piohub_state = IOHUB_PCI_DEVICE(d);
    f->system_memory = address_space_mem;
    f->pci_address_space = pci_address_space;
}

static void iohub_class_init(ObjectClass *oc, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(oc);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(oc);
    
    k->vendor_id = 0x1fff;
    k->device_id = 0x8017;
    k->revision  = 0x02;
    k->class_id  = PCI_CLASS_BRIDGE_HOST;
    dc->desc = "IOHUB Host Bridge";
    dc->user_creatable = false;
    dc->hotpluggable = false;
}

static const TypeInfo iohub_info = {
    .name = TYPE_IOHUB_PCI_DEVICE,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(PCIIOHUBState),
    .class_init = iohub_class_init,
    .interfaces = (InterfaceInfo[]) {
        { INTERFACE_CONVENTIONAL_PCI_DEVICE },
        { },
    },
};

static void iohub_pcihost_initfn(Object *obj)
{
    PCIHostState *s = PCI_HOST_BRIDGE(obj);

    memory_region_init_io(&s->conf_mem, obj, &pci_host_conf_le_ops, s,
                          "pci-conf-idx", 4);
    memory_region_init_io(&s->data_mem, obj, &pci_host_data_le_ops, s,
                          "pci-conf-data", 4);
}

static void iohub_pcihost_realize(DeviceState *dev, Error **errp)
{
    PCIHostState *s = PCI_HOST_BRIDGE(dev);
    SysBusDevice *sbd = SYS_BUS_DEVICE(dev);

    sysbus_add_io(sbd, 0xcf8, &s->conf_mem);
    sysbus_init_ioports(sbd, 0xcf8, 4);

    sysbus_add_io(sbd, 0xcfc, &s->data_mem);
    sysbus_init_ioports(sbd, 0xcfc, 4);

    /* register iohub 0xcf8 port as coalesced pio */
    memory_region_set_flush_coalesced(&s->data_mem);
    memory_region_add_coalescing(&s->conf_mem, 0, 4);
}

static void iohub_pcihost_class_init(ObjectClass *oc, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(oc);
    PCIHostBridgeClass *hc = PCI_HOST_BRIDGE_CLASS(oc);
    
    dc->realize = iohub_pcihost_realize;
    dc->fw_name = "pci";
    dc->user_creatable = false;
}

static const TypeInfo iohub_pcihost_info = {
    .name = TYPE_IOHUB_PCI_HOST_BRIDGE,
    .parent = TYPE_PCI_HOST_BRIDGE,
    .instance_size = sizeof(IOHUBState),
    .instance_init = iohub_pcihost_initfn,
    .class_init = iohub_pcihost_class_init,
};

static void iohub_register_types(void)
{
    type_register_static(&iohub_info);
    type_register_static(&iohub_pcihost_info);
}

type_init(iohub_register_types);
