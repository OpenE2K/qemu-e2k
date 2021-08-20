/*
 * Sparc CPU init helpers
 *
 *  Copyright (c) 2003-2005 Fabrice Bellard
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#include "qemu/osdep.h"
#include "qemu/module.h"
#include "qemu/qemu-print.h"
#include "qapi/error.h"
#include "qapi/visitor.h"
#include "cpu.h"
#include "helper-tcg.h"
#include "exec/exec-all.h"
#include "hw/qdev-properties.h"
#include "hw/core/tcg-cpu-ops.h"

#include "sysemu/sysemu.h"
#include "sysemu/tcg.h"
#include "sysemu/reset.h"
#ifndef CONFIG_USER_ONLY
#include "exec/address-spaces.h"
#include "hw/e2k/apic_internal.h"
#include "hw/boards.h"
#endif

//#define DEBUG_FEATURES

static void e2k_cpu_reset(DeviceState *dev)
{
    CPUState *cs = CPU(dev);
    E2KCPU *cpu = E2K_CPU(cs);
    E2KCPUClass *ecc = E2K_CPU_GET_CLASS(cpu);
    CPUE2KState *env = &cpu->env;

    ecc->parent_reset(dev);

    memset(env, 0, offsetof(CPUE2KState, end_reset_fields));

    /* must be set by set_pc */
    /* env->ip = 0x10000; */
    env->psr = PSR_PM;
    env->upsr = UPSR_NMIE | UPSR_FE;
    env->wd.base = 0;
    env->wd.size = 16;
    env->wd.psize = 8;
    env->bn.base = 8;
    env->bn.size = 8;
    env->bn.cur = 0;
    env->aau.incrs[0] = 1; /* always one */
    env->fpcr._one = 1; 
    env->fpcr.pc = FPCR_PC_XP;
    env->fpcr.em = FP_EM;
    env->pfpfr.em = FP_EM;

    e2k_update_fp_status(env);
    e2k_update_fx_status(env);

    // FIXME: testing
    env->idr = 0x3a207; /* mimic 8c */

    // FIXME: correct values
    env->psp.base = 0x810000;
    env->psp.size = 0x100000;
    env->pcsp.base = 0x910000;
    env->pcsp.size = 0xa10000;

#if !defined(CONFIG_USER_ONLY)
    /* FIXME: default values calculated from E2K E1CP firmware */
    env->sbr = 0xb000;
    env->usd.hi = 0x400000000000;
    env->usd.lo = env->sbr + 0x1800000000000000;

    /* We hard-wire the BSP to the first CPU. */
    apic_designate_bsp(cpu->apic_state, cs->cpu_index == 0);

    cs->halted = !cpu_is_bsp(cpu);
#endif
}

static bool e2k_cpu_exec_interrupt(CPUState *cs, int interrupt_request)
{
    qemu_log_mask(LOG_UNIMP, "e2k_cpu_exec_interrupt: not implemented\n");
    if (interrupt_request & CPU_INTERRUPT_HARD) {
        e2k_cpu_do_interrupt(cs);
        return true;
    }
    return false;
}

void e2k_cpu_do_interrupt(CPUState *cs)
{
    E2KCPU *cpu = E2K_CPU(cs);
    CPUE2KState *env = &cpu->env;

    qemu_log_mask(LOG_UNIMP, "e2k_cpu_do_interrupt: not implemented %d at " TARGET_FMT_lx "\n",
        cs->exception_index, env->ip);
    exit(1);
}

static void cpu_e2k_disas_set_info(CPUState *cs, disassemble_info *info)
{
    E2KCPU *cpu = E2K_CPU(cs);
    CPUE2KState *env = &cpu->env;

    info->mach = env->version * 3;
    info->print_insn = print_insn_e2k;
}

/* https://www.altlinux.org/Модели_процессоров_Эльбрус */
#define DEFAULT_CPU_MODEL "e8c"
static const struct e2k_def_t e2k_defs[] = {
    {
        .name           = "e2c+", /* however it may work better */
        .canonical_name = "MCST Elbrus 2C+ (Monocube)",
        .gdb_arch       = "elbrus-v2",
        .isa_version    = 2,
    },
    {
        .name           = "e2s",
        .canonical_name = "MCST Elbrus 4C",
        .gdb_arch       = "elbrus-v3",
        .isa_version    = 3,
    },
    {
        .name           = "e8c", /* default choice for system */
        .canonical_name = "MCST Elbrus 8C",
        .gdb_arch       = "elbrus-8c",
        .isa_version    = 4,
    },
    {
        .name           = "e8c2",
        .canonical_name = "MCST Elbrus 8CB",
        .gdb_arch       = "elbrus-v5",
        .isa_version    = 5,
    },
    {
        .name           = "e16c",
        .canonical_name = "MCST Elbrus 16C",
        .gdb_arch       = "elbrus-v6",
        .isa_version    = 6,
    },
};

static bool e2k_get_paging_enabled(const CPUState *cs)
{
    E2KCPU *cpu = E2K_CPU(cs);
    CPUE2KState *env = &cpu->env;

    return env->mmu.cr & MMU_CR_TLB_EN;
}

static void e2k_cpu_set_pc(CPUState *cs, vaddr value)
{
    E2KCPU *cpu = E2K_CPU(cs);

    cpu->env.ip = value;
}

static void e2k_cpu_synchronize_from_tb(CPUState *cs, const TranslationBlock *tb)
{
    E2KCPU *cpu = E2K_CPU(cs);

    cpu->env.ip = tb->pc;
}

static bool e2k_cpu_has_work(CPUState *cs)
{
    // TODO: e2k_cpu_has_work
    qemu_log_mask(LOG_UNIMP, "e2k_cpu_has_work: not implemented\n");
    return true;
}

static char *e2k_cpu_type_name(const char *cpu_model)
{
    return g_strdup(cpu_model);
}

static ObjectClass *e2k_cpu_class_by_name(const char *cpu_model)
{
    ObjectClass *oc;
    char *typename;

#ifdef CONFIG_USER_ONLY
    if (!strcasecmp(cpu_model, "any")) {
        cpu_model = DEFAULT_CPU_MODEL;
    }
#endif
   
    typename = e2k_cpu_type_name(cpu_model);
    oc = object_class_by_name(typename);
    g_free(typename);
    return oc;
}

#ifndef CONFIG_USER_ONLY
bool cpu_is_bsp(E2KCPU *cpu)
{
    return cpu_get_apic_base(cpu->apic_state) & MSR_IA32_APICBASE_BSP;
}

/* TODO: remove me, when reset over QOM tree is implemented */
static void e2k_cpu_machine_reset_cb(void *opaque)
{
    E2KCPU *cpu = opaque;
    cpu_reset(CPU(cpu));
}

void cpu_clear_apic_feature(CPUE2KState *env)
{
}

APICCommonClass *apic_get_class(void)
{
    const char *apic_type = "apic";

    /* TODO: kvm */

    return APIC_COMMON_CLASS(object_class_by_name(apic_type));
}

static void e2k_cpu_apic_create(E2KCPU *cpu, Error **errp)
{
    APICCommonState *apic;
    ObjectClass *apic_class = OBJECT_CLASS(apic_get_class());

    cpu->apic_state = DEVICE(object_new_with_class(apic_class));

    object_property_add_child(OBJECT(cpu), "lapic",
                              OBJECT(cpu->apic_state));
    object_unref(OBJECT(cpu->apic_state));

    qdev_prop_set_uint32(cpu->apic_state, "id", cpu->apic_id);
    /* TODO: convert to link<> */
    apic = APIC_COMMON(cpu->apic_state);
    apic->cpu = cpu;
    apic->apicbase = APIC_DEFAULT_ADDRESS | MSR_IA32_APICBASE_ENABLE;
}

static void e2k_cpu_apic_realize(E2KCPU *cpu, Error **errp)
{
    APICCommonState *apic;
    static bool apic_mmio_map_once;

    if (cpu->apic_state == NULL) {
        return;
    }
    qdev_realize(DEVICE(cpu->apic_state), NULL, errp);

    /* Map APIC MMIO area */
    apic = APIC_COMMON(cpu->apic_state);
    if (!apic_mmio_map_once) {
        memory_region_add_subregion_overlap(get_system_memory(),
                                            apic->apicbase &
                                            MSR_IA32_APICBASE_BASE,
                                            &apic->io_memory,
                                            0x1000);
        apic_mmio_map_once = true;
     }
}
#endif /* !CONFIG_USER_ONLY */

static void e2k_cpu_realizefn(DeviceState *dev, Error **errp)
{
    CPUState *cs = CPU(dev);
    E2KCPUClass *ecc = E2K_CPU_GET_CLASS(dev);
    Error *local_err = NULL;
    E2KCPU *cpu = E2K_CPU(dev);
    CPUE2KState *env = &cpu->env;

    env->version = env->def.isa_version;

    cpu_exec_realizefn(cs, &local_err);
    if (local_err != NULL) {
        goto out;
    }

    e2k_cpu_register_gdb_regs_for_features(cs);

#ifndef CONFIG_USER_ONLY
    MachineState *ms = MACHINE(qdev_get_machine());
    qemu_register_reset(e2k_cpu_machine_reset_cb, cpu);

//    if (ms->smp.cpus > 1) {
        e2k_cpu_apic_create(cpu, &local_err);
        if (local_err != NULL) {
            goto out;
        }
//    }
#endif

    qemu_init_vcpu(cs);

    e2k_cpu_apic_realize(cpu, &local_err);
    if (local_err != NULL) {
        goto out;
    }
    cpu_reset(cs);

    ecc->parent_realize(dev, errp);

out:
    if (local_err != NULL) {
        error_propagate(errp, local_err);
        return;
    }
}

static void e2k_cpu_initfn(Object* obj)
{
    E2KCPU *cpu = E2K_CPU(obj);
    E2KCPUClass *ecc = E2K_CPU_GET_CLASS(obj);
    CPUE2KState *env = &cpu->env;

    cpu_set_cpustate_pointers(cpu);

    if (ecc->cpu_def) {
        env->def = *ecc->cpu_def;
    }
}

static gchar* e2k_cpu_gdb_arch_name(CPUState *cs)
{
    E2KCPU *cpu = E2K_CPU(cs);
    CPUE2KState *env = &cpu->env;

    return g_strdup_printf("%s:%d", env->def.gdb_arch, TARGET_LONG_BITS);
}

static struct TCGCPUOps e2k_tcg_ops = {
    .initialize = e2k_tcg_initialize,
    .synchronize_from_tb = e2k_cpu_synchronize_from_tb,
    .cpu_exec_interrupt = e2k_cpu_exec_interrupt,
    .do_interrupt = e2k_cpu_do_interrupt,
    .tlb_fill = e2k_cpu_tlb_fill,
};

static Property e2k_cpu_properties[] = {
    DEFINE_PROP_BOOL("force_save_alc_dst", E2KCPU, env.force_save_alc_dst, false),
    DEFINE_PROP_BOOL("tags", E2KCPU, env.enable_tags, false),
    DEFINE_PROP_END_OF_LIST()
};

#ifndef CONFIG_USER_ONLY
#include "hw/core/sysemu-cpu-ops.h"

static const struct SysemuCPUOps e2k_sysemu_ops = {
    .get_paging_enabled = e2k_get_paging_enabled,
    .get_phys_page_attrs_debug = e2k_get_phys_page_attrs_debug,
};
#endif

static void e2k_cpu_class_init(ObjectClass *oc, void *data)
{
    E2KCPUClass *ecc = E2K_CPU_CLASS(oc);
    CPUClass *cc = CPU_CLASS(oc);
    DeviceClass *dc = DEVICE_CLASS(oc);

    device_class_set_parent_realize(dc, e2k_cpu_realizefn,
                                    &ecc->parent_realize);
    device_class_set_props(dc, e2k_cpu_properties);
    device_class_set_parent_reset(dc, e2k_cpu_reset, &ecc->parent_reset);

    cc->has_work = e2k_cpu_has_work;
    cc->dump_state = e2k_cpu_dump_state;
    cc->set_pc = e2k_cpu_set_pc;
    cc->class_by_name = e2k_cpu_class_by_name;
    cc->disas_set_info = cpu_e2k_disas_set_info;

#ifndef CONFIG_USER_ONLY
    cc->sysemu_ops = &e2k_sysemu_ops;
#endif /* !CONFIG_USER_ONLY */

    cc->gdb_core_xml_file  = "e2k-v1.xml";
    cc->gdb_arch_name      = e2k_cpu_gdb_arch_name;
    cc->gdb_read_register  = e2k_cpu_gdb_read_register;
    cc->gdb_write_register = e2k_cpu_gdb_write_register;
    cc->gdb_num_core_regs  = 574;

    cc->tcg_ops = &e2k_tcg_ops;
}

static const TypeInfo e2k_cpu_type_info = {
    .name = TYPE_E2K_CPU,
    .parent = TYPE_CPU,
    .instance_size = sizeof(E2KCPU),
    .instance_init = e2k_cpu_initfn,
    .abstract = true,
    .class_size = sizeof(E2KCPUClass),
    .class_init = e2k_cpu_class_init,
};

static void e2k_cpu_cpudef_class_init(ObjectClass *oc, void *data)
{
    E2KCPUClass *ecc = E2K_CPU_CLASS(oc);
    ecc->cpu_def = data;
}

static void e2k_register_cpudef_type(const struct e2k_def_t *def)
{
    char *typename = e2k_cpu_type_name(def->name);
    TypeInfo ti = {
        .name = typename,
        .parent = TYPE_E2K_CPU,
        .class_init = e2k_cpu_cpudef_class_init,
        .class_data = (void *)def,
    };

    type_register(&ti);
    g_free(typename);
}

static void e2k_cpu_register_types(void)
{
    int i;

    type_register_static(&e2k_cpu_type_info);
    for (i = 0; i < ARRAY_SIZE(e2k_defs); i++) {
        e2k_register_cpudef_type(&e2k_defs[i]);
    }
}

type_init(e2k_cpu_register_types)

void e2k_cpu_list(void)
{
    unsigned int i;
    size_t len = 0;
    
    for (i = 0; i < ARRAY_SIZE(e2k_defs); i++) {
        qemu_printf("%6s (%-30s) ISA version: v%d\n",
            e2k_defs[i].name,
            e2k_defs[i].canonical_name,
            e2k_defs[i].isa_version
        );
    }

    qemu_printf("\nFeatures:\n");
    for (i = 0; i < ARRAY_SIZE(e2k_cpu_properties); i++) {
        const char *name = e2k_cpu_properties[i].name;
        size_t l = strlen(name) + 1;
        if (len + l >= 75) {
            qemu_printf("\n");
            len = 0;
        }
        qemu_printf("%s%s", len ? " " : "  ", name);
        len += l;
    }
    qemu_printf("\n");
}
