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
#include "hw/sparc/sparc64.h"
#include "hw/sparc/sun4u_iommu.h"
#include "migration/vmstate.h"
#include "sysemu/sysemu.h"
#include "cpu.h"

#define ELBRUS_GPIO_START 0xf1200000ULL // TODO: verify this address
#define ELBRUS_SCC_START  0xf1100000ULL // TODO: verify this address
#define ELBRUS_PROM_START 0xf0000000ULL // prom seems to be fine to be loaded from this address
#define PROM_SIZE_MAX (8 * MiB)
#define PROM_FILENAME "_4R.bin"

#define ESCC_CLOCK 4915200 // TODO: why?

typedef struct ElbrusBoardState {
    MemoryRegion prom;
} ElbrusBoardState;

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
    ElbrusBoardState *s = g_new(ElbrusBoardState, 1);
    MemoryRegion *sysmem = get_system_memory();
    DeviceState *dev;
    SysBusDevice *sb;
    // SPARCCPU *cpu;
    
    /* init CPUs */
    sparc64_cpu_devinit(machine->cpu_type, ELBRUS_PROM_START);
    
    memory_region_add_subregion(sysmem, 0, machine->ram);
    
    memory_region_init_ram(&s->prom, NULL, "e90s.prom", PROM_SIZE_MAX, &error_fatal);
    memory_region_add_subregion(sysmem, ELBRUS_PROM_START, &s->prom);
    prom_init(ELBRUS_PROM_START, machine->firmware);
    
    dev = qdev_new(TYPE_ESCC);
    qdev_prop_set_uint32(dev, "disabled", 0);
    qdev_prop_set_uint32(dev, "frequency", ESCC_CLOCK);
    qdev_prop_set_uint32(dev, "it_shift", 1);
    qdev_prop_set_chr(dev, "chrB", serial_hd(1));
    qdev_prop_set_chr(dev, "chrA", serial_hd(0));
    qdev_prop_set_uint32(dev, "chnBtype", escc_serial);
    qdev_prop_set_uint32(dev, "chnAtype", escc_serial);

    sb = SYS_BUS_DEVICE(dev);
    sysbus_realize_and_unref(sb, &error_fatal);
    sysbus_mmio_map(sb, 0, ELBRUS_SCC_START);
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

static const TypeInfo e90s_type = {
    .name = MACHINE_TYPE_NAME("e90s"),
    .parent = TYPE_MACHINE,
    .class_init = e90s_class_init,
};

static void e90s_register_types(void)
{
    type_register_static(&e90s_type);
}

type_init(e90s_register_types)
