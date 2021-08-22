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

#include "chardev/char-fe.h"
#include "sysemu/sysemu.h"
#include "sysemu/cpus.h"
#include "sysemu/reset.h"
#include "hw/irq.h"
#include "hw/loader.h"
#include "hw/qdev-properties.h"
#include "hw/sysbus.h"
#include "hw/pci/pci_bridge.h"
#include "hw/e2k/e2k.h"
#include "hw/e2k/bootinfo.h"
#include "target/e2k/cpu.h"
#include "elf.h"

#include "hw/char/escc.h"
#include "hw/char/lmscon.h"

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

/*
static void lmscon_init(E2KMachineState *e2kms)
{
    DeviceState *dev;

    dev = qdev_new(TYPE_LMSCON);

    qdev_prop_set_chr(dev, "chr", serial_hd(0));

    if (!sysbus_realize_and_unref(SYS_BUS_DEVICE(dev), &error_fatal)) {
        error_report("failed to create lmscon");
        exit(1);
    }
}
*/

static bootblock_struct_t *generate_bootblock(E2KMachineState *e2kms,
    ram_addr_t kernel_base, long kernel_size)
{
    static bootblock_struct_t bootblock;
    boot_info_t *const bootinfo = &bootblock.info;
    MachineState *ms = MACHINE(e2kms);
    
    memset(&bootblock, 0, sizeof(bootblock));
    
    /* Set signature */
    bootinfo->signature = bootblock.x86_marker = X86BOOT_SIGNATURE;
    bootblock.bootblock_ver = BOOTBLOCK_VER;
    
    /* Init CPU info */
    bootinfo->num_of_nodes = 1; /* TODO: numa */
    bootinfo->nodes_map = 0x1;
    bootinfo->num_of_cpus = ms->smp.cpus;
    
    /* Init RAM banks */
    if(ms->ram_size > E2K_MLO_SIZE) {
        /* high mem */
        bootinfo->num_of_banks = 2;
        bootinfo->nodes_mem[0].banks[0].address = 0;
        bootinfo->nodes_mem[0].banks[0].size = E2K_MLO_SIZE;
        
        bootinfo->nodes_mem[0].banks[1].address = E2K_HIMEM_BASE;
        bootinfo->nodes_mem[0].banks[1].size = ms->ram_size - E2K_MLO_SIZE;
    } else {
        /* low mem only */
        bootinfo->num_of_banks = 1;
        bootinfo->nodes_mem[0].banks[0].address = 0;
        bootinfo->nodes_mem[0].banks[0].size = ms->ram_size;
    }
    
    bootinfo->kernel_base = kernel_base;
    bootinfo->kernel_size = kernel_size;
    
    strncpy(bootinfo->kernel_args_string, ms->kernel_cmdline, sizeof(*bootinfo->kernel_args_string));
    bootinfo->kernel_args_string[sizeof(*bootinfo->kernel_args_string)] = 0;
    
    return &bootblock;
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
    
    e2kms->bootblock = generate_bootblock(e2kms, entry, size);
    
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
                           E2K_BIOS_AREA_BASE, -1) != 0) {
        error_report("could not load firmware '%s'", firmware_name);
        exit(1);
    }
    g_free(filename);
    firmware = g_malloc(sizeof(*firmware));
    memory_region_init_ram(firmware, NULL, "e2k.firmware",
                           E2K_BIOS_AREA_SIZE, &error_fatal);
    memory_region_add_subregion(rom_memory,
                                E2K_BIOS_AREA_BASE, firmware);

    reset_params.loadaddr = E2K_BIOS_AREA_BASE;
}

/*static void e2k_gsi_handler(void *opaque, int n, int level)
{
    GSIState *s = opaque;
    qemu_set_irq(s->ioapic_irq[n], level);
}*/


#define ESCC_CLOCK 2457600

static PCIDevice *pci_init(E2KMachineState *e2kms)
{
    PCIDevice *br, *d;
    
    e2kms->bus = iohub_init(TYPE_IOHUB_PCI_HOST_BRIDGE, TYPE_IOHUB_PCI_DEVICE,
                            &e2kms->iohub,
                            get_system_memory(), get_system_io());
    
    br = d = pci_new_multifunction(PCI_DEVFN(0, 0), false, "pci-bridge");
    qdev_prop_set_uint8(DEVICE(d), PCI_BRIDGE_DEV_PROP_CHASSIS_NR, 1);
    
    pci_realize_and_unref(d, e2kms->bus, &error_fatal);
    
    d = pci_new_multifunction(PCI_DEVFN(2, 2), false, "escc-pci");
    qdev_prop_set_uint32(DEVICE(d), "disabled", 0);
    qdev_prop_set_uint32(DEVICE(d), "frequency", ESCC_CLOCK);
    qdev_prop_set_uint32(DEVICE(d), "it_shift", 1);
    qdev_prop_set_chr(DEVICE(d), "chrA", serial_hd(0));
    qdev_prop_set_chr(DEVICE(d), "chrB", serial_hd(1));
    qdev_prop_set_uint32(DEVICE(d), "chnBtype", escc_serial);
    qdev_prop_set_uint32(DEVICE(d), "chnAtype", escc_serial);
    
    pci_realize_and_unref(d, pci_bridge_get_sec_bus(PCI_BRIDGE(br)), &error_fatal);
    
    /* pci_vga_init(); */
    
    return d;
}

static void e2k_machine_init(MachineState *ms)
{
    E2KMachineState *e2kms = E2K_MACHINE(ms);
    MemoryRegion *rom_memory;
    MemoryRegion *ram_below_4g, *ram_above_4g;

    rom_memory = get_system_memory();

    /* Init RAM banks */
    if(ms->ram_size > E2K_MLO_SIZE) {
        e2kms->above_4g_mem_size = ms->ram_size - E2K_MLO_SIZE;
        e2kms->below_4g_mem_size = E2K_MLO_SIZE;
    } else {
        e2kms->above_4g_mem_size = E2K_MLO_BASE;
        e2kms->below_4g_mem_size = ms->ram_size;
    }
    
    ram_below_4g = g_malloc(sizeof(*ram_below_4g));
    memory_region_init_alias(ram_below_4g, NULL, "ram-below-4g", ms->ram,
                             0, e2kms->below_4g_mem_size);
    memory_region_add_subregion(rom_memory, 0, ram_below_4g);
    if(e2kms->above_4g_mem_size > 0) {
        ram_above_4g = g_malloc(sizeof(*ram_above_4g));
        memory_region_init_alias(ram_above_4g, NULL, "ram-above-4g",
                                 ms->ram,
                                 e2kms->below_4g_mem_size, 
                                 e2kms->above_4g_mem_size);
        memory_region_add_subregion(rom_memory, E2K_HIMEM_BASE, ram_above_4g);
    }
    
    e2kms->ioapic_as = &address_space_memory;

    if (!e2k_kernel_init(e2kms, rom_memory))
        firmware_init(e2kms, "e2k.bin", rom_memory);
    
    e2kms->gsi_state = g_new0(GSIState, 1);
    // pic = qemu_allocate_irqs(e2k_gsi_handler, e2kms->gsi_state, IOAPIC_NUM_PINS);

    cpus_init(e2kms);
    /* lmscon_init(e2kms); */
    sic_init(e2kms);
    pci_init(e2kms);
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
    .instance_size = sizeof(E2KMachineState),
    .class_init = e2k_machine_class_init,
};

static void e2k_machine_register_types(void)
{
    type_register_static(&e2k_machine_info);
}

type_init(e2k_machine_register_types)
