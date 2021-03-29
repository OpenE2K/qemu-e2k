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
#include "qapi/error.h"
#include "cpu.h"
#include "helper-tcg.h"
#include "qemu/module.h"
#include "qemu/qemu-print.h"
#include "exec/exec-all.h"
#include "hw/qdev-properties.h"
#include "qapi/visitor.h"
#include "hw/core/tcg-cpu-ops.h"

//#define DEBUG_FEATURES

static void e2k_cpu_reset(DeviceState *dev)
{
    CPUState *cs = CPU(dev);
    E2KCPU *cpu = E2K_CPU(cs);
    E2KCPUClass *ecc = E2K_CPU_GET_CLASS(cpu);
    CPUE2KState *env = &cpu->env;

    ecc->parent_reset(dev);

    memset(env, 0, offsetof(CPUE2KState, end_reset_fields));

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
    qemu_log_mask(LOG_UNIMP, "e2k_cpu_do_interrupt: not implemented\n");
    cs->exception_index = -1;
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
        error_propagate(errp, local_err);
        return;
    }

    e2k_cpu_register_gdb_regs_for_features(cs);

    qemu_init_vcpu(cs);

    ecc->parent_realize(dev, errp);
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
