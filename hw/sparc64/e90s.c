#include "qemu/osdep.h"
#include "qemu/datadir.h"
#include "qemu/error-report.h"
#include "qemu/typedefs.h"
#include "qemu/units.h"
#include "qom/object.h"
#include "qapi/error.h"
#include "elf.h"
#include "exec/address-spaces.h"
#include "hw/boards.h"
#include "hw/loader.h"
#include "hw/sysbus.h"
#include "hw/qdev-properties.h"
#include "hw/char/escc.h"
#include "hw/pci/pci.h"
#include "hw/pci/pci_bridge.h"
#include "hw/pci/pci_bus.h"
#include "hw/pci/pci_host.h"
#include "hw/sparc/sparc64.h"
#include "hw/sparc/sun4u_iommu.h"
#include "migration/vmstate.h"
#include "sysemu/sysemu.h"
#include "cpu.h"
#include "trace.h"

// memory map 
#define PROM_BASE   0xFFFF000000UL // bootloader 
#define PCIIO_BASE  0xFF20000000UL 
#define PEXCFG_BASE 0xFF10000000UL // pci express configuration registers
#define NBSR_BASE   0xFE00000000UL // north bridge system registers (?)
#define NBSR_SIZE   (1 * MiB)

#define PROM_SIZE_MAX (16 * MiB)
#define PROM_FILENAME "_4R.bin"

#define ESCC_CLOCK 4915200 // TODO: why?

#define TYPE_E90S       MACHINE_TYPE_NAME("e90s")
#define TYPE_E90S_NBSR  "e90s-nbsr"
#define TYPE_E90S_IOHUB "e90s-iohub"

typedef struct E90SState {
    MemoryRegion prom;
    MemoryRegion nbsr;
} E90SState;
DECLARE_INSTANCE_CHECKER(E90SState, E90S, TYPE_E90S)

typedef struct NBSRState {
	SysBusDevice parent_obj;
	MemoryRegion region;
} NBSRState;
DECLARE_INSTANCE_CHECKER(NBSRState, NBSR, TYPE_E90S_NBSR)

typedef struct IOHubState {
	PCIHostState parent_obj;

	MemoryRegion pci_cfg;
	MemoryRegion pci_io;
	MemoryRegion pci_mmio;
} IOHubState;
DECLARE_INSTANCE_CHECKER(IOHubState, IOHUB, TYPE_E90S_IOHUB)

/* North Bridge System Registers */
static uint64_t nbsr_read(void *opaque, hwaddr addr, unsigned size)
{
	trace_nbsr_read(addr, 0, size);

	return 0;
}

static void nbsr_write(void *opaque, hwaddr addr, uint64_t val, unsigned size)
{
	trace_nbsr_write(addr, val, size);
}

static const MemoryRegionOps nbsr_ops = {
	.read = nbsr_read,
	.write = nbsr_write,
	.endianness = DEVICE_NATIVE_ENDIAN,
};

static void nbsr_realize(DeviceState *d, Error **errp)
{
	NBSRState *nbsr = NBSR(d);

	memory_region_init_io(&nbsr->region, OBJECT(d),
		&nbsr_ops, OBJECT(d), "e90s.nbsr", NBSR_SIZE);
	memory_region_add_subregion(get_system_memory(), NBSR_BASE,
		&nbsr->region);
}

static void nbsr_class_init(ObjectClass *oc, void *data)
{
	DeviceClass *dc = DEVICE_CLASS(oc);
	dc->realize = nbsr_realize;
}

static const TypeInfo nbsr_info = {
	.name = TYPE_E90S_NBSR,
	.parent = TYPE_SYS_BUS_DEVICE,
	.instance_size = sizeof(NBSRState),
	.class_init = nbsr_class_init,
};

/* IOHub */
/* TODO: move to hw/pci/ */
static void pci_iohub_set_irq(void *opaque, int irq_num, int level)
{
	// TODO:
}

static int pci_iohub_map_irq(PCIDevice *pci_dev, int irq_num)
{
	return irq_num;
}

static void iohub_realize(DeviceState *dev, Error **errp)
{
	IOHubState *s = IOHUB(dev);
	PCIHostState *phb = PCI_HOST_BRIDGE(dev);

	phb->bus = pci_register_root_bus(dev, "pci", pci_iohub_set_irq, pci_iohub_map_irq,
		s, &s->pci_mmio, &s->pci_io, 0, 0x40, TYPE_PCI_BUS);

	// pci_create_simple(phb->bus, 0, TYPE_IOHUB_PCI_DEVICE);
}

static void iohub_class_init(ObjectClass *oc, void *data)
{
	DeviceClass *dc = DEVICE_CLASS(oc);

	dc->realize = iohub_realize;
	set_bit(DEVICE_CATEGORY_BRIDGE, dc->categories);
	dc->fw_name = "pci";
}

const TypeInfo iohub_info = {
	.name = TYPE_E90S_IOHUB,
	.parent = TYPE_PCI_HOST_BRIDGE,
	.instance_size = sizeof(IOHubState),
	.class_init = iohub_class_init,
};

static void pci_init(E90SState *e90s)
{
#if 0
	dev = qdev_new(TYPE_ESCC);
	qdev_prop_set_uint32(dev, "disabled", 0);
	qdev_prop_set_uint32(dev, "frequency", ESCC_CLOCK);
	qdev_prop_set_uint32(dev, "it_shift", 0);
	qdev_prop_set_chr(dev, "chrB", serial_hd(1));
	qdev_prop_set_chr(dev, "chrA", serial_hd(0));
	qdev_prop_set_uint32(dev, "chnBtype", escc_serial);
	qdev_prop_set_uint32(dev, "chnAtype", escc_serial);

	sb = SYS_BUS_DEVICE(dev);
	sysbus_realize_and_unref(sb, &error_fatal);
	sysbus_mmio_map(sb, 0, ELBRUS_SCC_START);
#endif
}

/* Boot PROM */
static void prom_init(hwaddr addr, const char *bios_name)
{
    char *filename;
    int ret;
    
    /* load boot prom */
    if (bios_name == NULL) {
        bios_name = PROM_FILENAME;
    }
    filename = qemu_find_file(QEMU_FILE_TYPE_BIOS, bios_name);
    if (filename) {
        // as we're working with raw images, no elf parsing needed
        ret = load_image_targphys(filename, addr, PROM_SIZE_MAX);
        g_free(filename);
    } else {
        ret = -1;
    }
    if (ret < 0 || ret > PROM_SIZE_MAX) {
        error_report("could not load prom '%s'", bios_name);
        exit(1);
    }
}

static void e90s_init(MachineState *machine)
{
	E90SState *s = E90S(machine);
	DeviceState *dev;
	SysBusDevice *sb;
    // SPARCCPU *cpu;
    
    /* init CPUs */
    sparc64_cpu_devinit(machine->cpu_type, PROM_BASE);
    
	memory_region_add_subregion(get_system_memory(), 0, machine->ram);
    
	memory_region_init_ram(&s->prom, OBJECT(machine),
		"e90s.prom", PROM_SIZE_MAX, &error_fatal);
	memory_region_add_subregion(get_system_memory(), PROM_BASE, &s->prom);
    prom_init(PROM_BASE, machine->firmware);

	dev = qdev_new(TYPE_E90S_NBSR);
	sb = SYS_BUS_DEVICE(dev);
	sysbus_realize_and_unref(sb, &error_fatal);

	pci_init(s);
}

static void e90s_class_init(ObjectClass *oc, void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    
    mc->desc = "E90S platform";
    mc->init = e90s_init;
    mc->max_cpus = 1; /* XXX for now */
    mc->default_boot_order = "c";
    mc->default_cpu_type = SPARC_CPU_TYPE_NAME("MCST-Elbrus-R1000");
    mc->default_ram_id = "e90s.ram";
}

static const TypeInfo e90s_info = {
    .name = MACHINE_TYPE_NAME("e90s"),
    .parent = TYPE_MACHINE,
    .class_init = e90s_class_init,
};

static void e90s_register_types(void)
{
	type_register_static(&nbsr_info);
	// type_register_static(&iohub_info);

	type_register_static(&e90s_info);
}

type_init(e90s_register_types)
