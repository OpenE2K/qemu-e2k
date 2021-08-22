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
#include "trace.h"

OBJECT_DECLARE_SIMPLE_TYPE(IOHUBState, IOHUB_PCI_HOST_BRIDGE)

struct IOHUBState {
    PCIHostState parent_obj;
};

/* PCI config address are 27-bit in length.
 * bit 20-27: bus number
 * bit 12-19: devfn 
 * bit 0-11:  device register address
 */

#define IOHUB_CONFIG_ADDR_BUS(addr)     (((addr) >> 20) & 0xff)
#define IOHUB_CONFIG_ADDR_DEVFN(addr)   (((addr) >> 12) & 0xff)
#define IOHUB_CONFIG_ADDR_REG(addr)     (((addr) >> 0) & 0xfff)
#define TO_IOHUB_CONFIG_ADDR(bus, devfn, where) ((((bus) & 0xff) << 20) | (((devfn) & 0xff) << 12) | ((where) & 0xfff))
#define TO_PCI_CONFIG_ADDR(bus, devfn, reg) ((((bus) & 0xff) << 16) | (((devfn) & 0xff) << 8) | (((reg) & 0xff) << 0))

static inline uint32_t
iohub_pci_config_addr(hwaddr addr, uint8_t *bus, uint8_t *devfn, uint16_t *reg)
{
    *bus   = IOHUB_CONFIG_ADDR_BUS(addr);
    *devfn = IOHUB_CONFIG_ADDR_DEVFN(addr);
    *reg   = IOHUB_CONFIG_ADDR_REG(addr);
    
    return TO_PCI_CONFIG_ADDR(*bus, *devfn, *reg);
}

static uint64_t iohub_pci_config_read(void *opaque, hwaddr addr, unsigned size)
{
    uint8_t bus, devfn;
    uint16_t reg;
    uint64_t val;
    hwaddr conf_addr = iohub_pci_config_addr(addr, &bus, &devfn, &reg);
    
    val = pci_data_read(opaque, conf_addr, size);
    
    trace_iohub_pci_config_read(addr, conf_addr, bus, PCI_SLOT(devfn), PCI_FUNC(devfn), reg, val);
    
    return val;
}

static void iohub_pci_config_write(void *opaque, hwaddr addr, uint64_t val, unsigned size)
{
    uint8_t bus, devfn;
    uint16_t reg;
    hwaddr conf_addr = iohub_pci_config_addr(addr, &bus, &devfn, &reg);
    
    trace_iohub_pci_config_write(addr, conf_addr, bus, PCI_SLOT(devfn), PCI_FUNC(devfn), reg, val);
    
    pci_data_write(opaque, conf_addr, val, size);
}

static const MemoryRegionOps iohub_pci_config_ops = {
    .read = iohub_pci_config_read,
    .write = iohub_pci_config_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
};

PCIBus *iohub_init(const char *host_type, const char *pci_type,
                   PCIIOHUBState **piohub_state,
                   MemoryRegion *address_space_mem,
                   MemoryRegion *address_space_io)
{
    DeviceState *dev;
    SysBusDevice *sb;
    PCIDevice *d;
    PCIHostState *s;
    PCIBus *b;
    PCIIOHUBState *f;
    MemoryRegion *pci_address_space, *pci_config_as;
    
    dev = qdev_new(host_type);
    s = PCI_HOST_BRIDGE(dev);
    sb = SYS_BUS_DEVICE(dev);
    
    pci_address_space = g_malloc(sizeof(*pci_address_space));
    memory_region_init_io(pci_address_space, OBJECT(dev), NULL, NULL, "pcimem", E2K_PCIMEM_SIZE);
    memory_region_add_subregion(address_space_mem, E2K_PCIMEM_BASE, pci_address_space);
        
    s->bus = b = pci_root_bus_new(dev, NULL, 
                                  pci_address_space, 
                                  address_space_io,
                                  0, TYPE_PCI_BUS);
    
    object_property_add_child(qdev_get_machine(), "iohub", OBJECT(dev));
    sysbus_realize_and_unref(sb, &error_fatal);
    
    pci_config_as = g_malloc(sizeof(*pci_config_as));
    memory_region_init_io(pci_config_as, OBJECT(dev), &iohub_pci_config_ops, s->bus, "pcicfg", E2K_PCICFG_SIZE);
    sysbus_init_mmio(sb, pci_config_as);
    sysbus_mmio_map(sb, 0, E2K_PCICFG_BASE);
    
    d = pci_create_simple(b, PCI_DEVFN(1, 0), pci_type);
    f = *piohub_state = IOHUB_PCI_DEVICE(d);
    f->system_memory = address_space_mem;
    f->pci_address_space = pci_address_space;
    
    return b;
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
