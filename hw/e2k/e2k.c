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

#include "qemu/osdep.h"
#include "qemu/datadir.h"
#include "qemu/log.h"
#include "qapi/error.h"
#include "trace.h"

#include "sysemu/cpus.h"
#include "sysemu/reset.h"
#include "hw/loader.h"
#include "hw/e2k/e2k.h"
#include "hw/e2k/iohub.h"
#include "target/e2k/cpu.h"
#include "elf.h"

#include "hw/char/escc.h"

#define ES2_NSR_AREA_PHYS_BASE      0x0000000110000000UL    /* node 0 */
#define ES2_NSR_AREA_MAX_SIZE       0x0000000010000000UL    /* max NSRs */
                                /* area: */
                                /* limited */
                                /* by follow */
                                /* LAPICINT */

DeviceState *cpu_get_current_apic(void)
{
    if (current_cpu) {
        E2KCPU *cpu = E2K_CPU(current_cpu);
        return cpu->apic_state;
    } else {
        return NULL;
    }
}

static void cpus_init(E2KMachineState *e2kms)
{
    MachineState *ms = MACHINE(e2kms);
    int i;

    for (i = 0; i < ms->smp.cpus; i++) {
        Object *cpu = object_new(ms->cpu_type);

        if (!qdev_realize(DEVICE(cpu), NULL, &error_fatal)) {
            error_report("failed to create cpu");
            exit(1);
        }
    }
}

static bool e2k_kernel_init(E2KMachineState *e2kms, MemoryRegion *rom_memory)
{
    MachineState *ms = MACHINE(e2kms);
    uint64_t entry, kernel_high;
    long size;
    MemoryRegion *kernel;

    if (!ms->kernel_filename)
        return false;

    /* FIXME: set ip to entry, for now it just equals to FIRMWARE_LOAD_ADDR */
    size = load_elf(ms->kernel_filename, NULL, NULL, NULL,
                    &entry, NULL, &kernel_high, NULL, 0, EM_MCST_ELBRUS,
                    0, 0);

    if (size <= 0) {
        error_report("could not load kernel '%s': %s",
                     ms->kernel_filename, load_elf_strerror(size));
        return false;
    }

    /* TODO: load initrd */

    kernel = g_malloc(sizeof(*kernel));
    memory_region_init_ram(kernel, NULL, "e2k.kernel", size, &error_fatal);
    memory_region_add_subregion(rom_memory, entry, kernel);

    return true;
}

static void firmware_init(E2KMachineState *e2kms, const char *default_filename,
    MemoryRegion *rom_memory)
{
    MachineState *ms = MACHINE(e2kms);
    const char *firmware_name;
    char *filename;
    int size;
    MemoryRegion *firmware;

    firmware_name = ms->firmware ?: default_filename;
    filename = qemu_find_file(QEMU_FILE_TYPE_BIOS, firmware_name);
    if (filename == NULL) {
        error_report("no firmware provided");
        exit(1);
    }
    size = get_image_size(filename);
    if (size < 0) {
        error_report("failed to get firmware size '%s'", filename);
        exit(1);
    }
    if (rom_add_file_fixed(firmware_name, FIRMWARE_LOAD_ADDR, -1) != 0) {
        error_report("could not load firmware '%s'", firmware_name);
        exit(1);
    }
    g_free(filename);
    firmware = g_malloc(sizeof(*firmware));
    memory_region_init_ram(firmware, NULL, "e2k.firmware", size, &error_fatal);
    memory_region_add_subregion(rom_memory, FIRMWARE_LOAD_ADDR, firmware);
}

static uint64_t sic_mem_read(void *opaque, hwaddr addr, unsigned size)
{
//    E2KMachineState *ms = opaque;
    uint64_t val;
    int index;

    if (size < 4) {
        return 0;
    }

    index = (addr >> 4) & 0xfff;
    switch (index) {
    case 0x09:
        // TODO: io page address [48:12]
        // TODO: page + 0x8044 = 16-bit reg
        val = 0xdeadbeef000 >> 12;
        break;
    default:
        val = 0;
        break;
    }

    trace_sic_mem_readl(addr, val);
    return val;
}

static void sic_mem_write(void *opaque, hwaddr addr, uint64_t val,
    unsigned size)
{
//    E2KMachineState *ms = opaque;

     trace_sic_mem_writel(addr, val);
}

static const MemoryRegionOps sic_io_ops = {
    .read = sic_mem_read,
    .write = sic_mem_write,
    .impl.min_access_size = 1,
    .impl.max_access_size = 4,
    .valid.min_access_size = 1,
    .valid.max_access_size = 4,
    .endianness = DEVICE_NATIVE_ENDIAN,
};

static void sic_init(E2KMachineState *ms)
{
    MemoryRegion *io_memory;

    io_memory = g_malloc(sizeof(*io_memory));
    memory_region_init_io(io_memory, OBJECT(ms), &sic_io_ops, ms, "sic-msi",
        ES2_NSR_AREA_MAX_SIZE);
    memory_region_add_subregion(get_system_memory(), ES2_NSR_AREA_PHYS_BASE,
        io_memory);
}

static void e2k_machine_init(MachineState *ms)
{
    E2KMachineState *e2kms = E2K_MACHINE(ms);
    MemoryRegion *rom_memory;

    rom_memory = get_system_memory();
    cpus_init(e2kms);
    iohub_init(e2kms);
    memory_region_add_subregion(get_system_memory(), 0, ms->ram);
    sic_init(e2kms);
    
    if (!e2k_kernel_init(e2kms, rom_memory))
        firmware_init(e2kms, "e2k.bin", rom_memory);
}

static void e2k_machine_class_init(ObjectClass *oc, void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);

    mc->desc = "Generic E2K Machine";
    mc->init = e2k_machine_init;
    mc->default_machine_opts = "firmware=e2k.bin";
    mc->is_default = true;
    mc->max_cpus = 8;
    mc->default_cpu_type = "e8c"; // TODO
    mc->default_ram_id = "e2k.ram";
}

static const TypeInfo e2k_machine_info = {
    .name = TYPE_E2K_MACHINE,
    .parent = TYPE_MACHINE,
    .class_size = sizeof(E2KMachineClass),
    .class_init = e2k_machine_class_init,
};

static void e2k_machine_register_types(void)
{
    type_register_static(&e2k_machine_info);
}

type_init(e2k_machine_register_types)
