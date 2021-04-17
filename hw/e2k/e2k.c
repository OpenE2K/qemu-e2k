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

#define E2K_FULL_SIC_BIOS_AREA_PHYS_BASE        0x0000000100000000UL
#define E2K_FULL_SIC_BIOS_AREA_SIZE             0x0000000001000000UL

#define ES2_NSR_AREA_PHYS_BASE      0x0000000110000000UL    /* node 0 */
#define ES2_NSR_AREA_MAX_SIZE       0x0000000010000000UL    /* max NSRs */
                                /* area: */
                                /* limited */
                                /* by follow */
                                /* LAPICINT */

struct
{
    target_ulong loadaddr;
} reset_params;

DeviceState *cpu_get_current_apic(void)
{
    if (current_cpu) {
        E2KCPU *cpu = E2K_CPU(current_cpu);
        return cpu->apic_state;
    } else {
        return NULL;
    }
}

static void e2k_cpu_reset(void *opaque)
{
    E2KCPU *cpu = opaque;
    CPUState *cs = CPU(cpu);

    cpu_reset(cs);

    cpu_set_pc(cs, reset_params.loadaddr);
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

        qemu_register_reset(e2k_cpu_reset, cpu);
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

    reset_params.loadaddr = entry;

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
    if (rom_add_file_fixed(firmware_name,
                           E2K_FULL_SIC_BIOS_AREA_PHYS_BASE, -1) != 0) {
        error_report("could not load firmware '%s'", firmware_name);
        exit(1);
    }
    g_free(filename);
    firmware = g_malloc(sizeof(*firmware));
    memory_region_init_ram(firmware, NULL, "e2k.firmware",
                           E2K_FULL_SIC_BIOS_AREA_SIZE, &error_fatal);
    memory_region_add_subregion(rom_memory,
                                E2K_FULL_SIC_BIOS_AREA_PHYS_BASE, firmware);

    reset_params.loadaddr = E2K_FULL_SIC_BIOS_AREA_PHYS_BASE;
}

static void e2k_machine_init(MachineState *ms)
{
    E2KMachineState *e2kms = E2K_MACHINE(ms);
    MemoryRegion *rom_memory;

    rom_memory = get_system_memory();
    memory_region_add_subregion(get_system_memory(), 0, ms->ram);
    
    if (!e2k_kernel_init(e2kms, rom_memory))
        firmware_init(e2kms, "e2k.bin", rom_memory);

    cpus_init(e2kms);
    iohub_init(e2kms);
    sic_init(e2kms);
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
