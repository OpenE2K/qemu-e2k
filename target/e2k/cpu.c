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
#include "qemu/module.h"
#include "qemu/qemu-print.h"
#include "exec/exec-all.h"
#include "hw/qdev-properties.h"
#include "qapi/visitor.h"

//#define DEBUG_FEATURES

static void e2k_cpu_reset(DeviceState *dev)
{
}

static bool e2k_cpu_exec_interrupt(CPUState *cs, int interrupt_request)
{
    
}

static void cpu_e2k_disas_set_info(CPUState *cpu, disassemble_info *info)
{
    info->print_insn = print_insn_e2k;
}


void cpu_e2k_set_id(CPUSPARCState *env, unsigned int cpu)
{
}

void e2k_cpu_dump_state(CPUState *cs, FILE *f, int flags)
{
}

static void e2k_cpu_set_pc(CPUState *cs, vaddr value)
{
}

static void e2k_cpu_synchronize_from_tb(CPUState *cs, TranslationBlock *tb)
{

}

static bool e2k_cpu_has_work(CPUState *cs)
{
    return true;
}

static char *e2k_cpu_type_name(const char *cpu_model)
{
    char *name = g_strdup_printf("%s", cpu_model);
    return name;
}

static ObjectClass *e2k_cpu_class_by_name(const char *cpu_model)
{
    ObjectClass *oc;
    char *typename;

    typename = e2k_cpu_type_name(cpu_model);
    oc = object_class_by_name(typename);
    g_free(typename);
    return oc;
}

static void e2k_cpu_realizefn(DeviceState *dev, Error **errp)
{
}

static void e2k_cpu_class_init(ObjectClass *oc, void *data)
{
    E2KCPUClass *scc = E2K_CPU_CLASS(oc);
    CPUClass *cc = CPU_CLASS(oc);
    DeviceClass *dc = DEVICE_CLASS(oc);

    device_class_set_parent_realize(dc, e2k_cpu_realizefn,
                                    &scc->parent_realize);
    
    device_class_set_parent_reset(dc, e2k_cpu_reset, &scc->parent_reset);

    cc->disas_set_info = cpu_e2k_disas_set_info;
}

static const TypeInfo sparc_cpu_type_info = {
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
    E2KCCPUClass *scc = E2K_CPU_CLASS(oc);
    scc->cpu_def = data;
}

/*static void e2k_register_cpudef_type(const struct e2k_def_t *def)
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

static void sparc_cpu_register_types(void)
{
    int i;

    type_register_static(&sparc_cpu_type_info);
}

type_init(sparc_cpu_register_types)
