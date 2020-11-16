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

void cpu_e2k_set_id(CPUE2KState *env, unsigned int cpu);
void e2k_cpu_dump_state(CPUState *cs, FILE *f, int flags);

static void e2k_cpu_reset(DeviceState *dev)
{
    CPUState *cs = CPU(dev);
    E2KCPU *cpu = E2K_CPU(cs);
    E2KCPUClass *ecc = E2K_CPU_GET_CLASS(cpu);
    CPUE2KState *env = &cpu->env;

    ecc->parent_reset(dev);

    memset(env, 0, offsetof(CPUE2KState, end_reset_fields));

    env->wptr = &env->wregs[0];
}

static bool e2k_cpu_exec_interrupt(CPUState *cs, int interrupt_request)
{
    return false;
}

static void cpu_e2k_disas_set_info(CPUState *cpu, disassemble_info *info)
{
    info->print_insn = print_insn_e2k;
}


void cpu_e2k_set_id(CPUE2KState *env, unsigned int cpu)
{
    // TODO
    qemu_log_mask(LOG_UNIMP, "cpu_e2k_set_id: not implemented\n");
}

static const struct e2k_def_t e2k_defs[] = {
    {
        .name = "any",
        .isa_version = 0xffffffff,
    }
};

static inline void cpu_dump_state_wd(CPUE2KState *env, FILE *f, int flags)
{
    int wbs = GET_FIELD(env->cr1_lo, CR1_LO_WBS_OFF, CR1_LO_WBS_END);
    int wsz = GET_FIELD(env->cr1_lo, CR1_LO_WPSZ_OFF, CR1_LO_WPSZ_END);

    qemu_fprintf(f, "wbs = %#x, wsz = %#x\n", wbs, wsz);
}

static inline void cpu_dump_state_br(CPUE2KState *env, FILE *f, int flags)
{
    uint32_t br = GET_FIELD(env->cr1_hi, CR1_HI_BR_OFF, CR1_HI_BR_END);
    int rbs = GET_FIELD(br, BR_RBS_OFF, BR_RBS_END);
    int rsz = GET_FIELD(br, BR_RSZ_OFF, BR_RSZ_END);
    int rcur = GET_FIELD(br, BR_RCUR_OFF, BR_RCUR_END);
    int psz = GET_FIELD(br, BR_PSZ_OFF, BR_PSZ_END);
    int pcur = GET_FIELD(br, BR_PCUR_OFF, BR_PCUR_END);

    qemu_fprintf(f, "br         %#x\n", br);
    qemu_fprintf(f, "    rbs    %#x\n", rbs);
    qemu_fprintf(f, "    rsz    %#x\n", rsz);
    qemu_fprintf(f, "    rcur   %#x\n", rcur);
    qemu_fprintf(f, "    psz    %#x\n", psz);
    qemu_fprintf(f, "    pcur   %#x\n", pcur);
}

void e2k_cpu_dump_state(CPUState *cs, FILE *f, int flags)
{
    E2KCPU *cpu = E2K_CPU(cs);
    CPUE2KState *env = &cpu->env;
    unsigned int i;

    qemu_fprintf(f, "ip: " TARGET_FMT_lx "\n", env->ip);
    qemu_fprintf(f, "pcsp_hi: %016lx, pcsp_lo: %016lx\n",
        env->pcsp_hi, env->pcsp_lo);
    qemu_fprintf(f, "psp_hi: %016lx, psp_lo: %016lx\n",
        env->psp_hi, env->psp_lo);
    qemu_fprintf(f, "PF: %016lx\n", env->pf);
    qemu_fprintf(f, "cr1_hi: %016lx, cr1_lo: %016lx\n",
        env->cr1_hi, env->cr1_lo);
    qemu_fprintf(f, "usd_hi: %016lx, usd_lo: %016lx\n",
        env->usd_hi, env->usd_lo);
    cpu_dump_state_wd(env, f, flags);
    cpu_dump_state_br(env, f, flags);
    qemu_fprintf(f, "lsr: %016lx\n", env->lsr);

    for (i = 0; i < 192; i += 4) {
        const char *s1 = i < 10 ? "  " : (i < 100 ? " " : "");
        const char *s2 = i + 2 < 10 ? "  " : (i + 2 < 100 ? " " : "");
        qemu_fprintf(f, "%s%%r%d = %16lx  ", s1, i    , env->wregs[i]);
        qemu_fprintf(f, "%s%%r%d = %16lx  ", s1, i + 1, env->wregs[i + 1]);
        qemu_fprintf(f, "%s%%r%d = %16lx  ", s2, i + 2, env->wregs[i + 2]);
        qemu_fprintf(f, "%s%%r%d = %16lx\n", s2, i + 3, env->wregs[i + 3]);
    }

    for (i = 0; i < 32; i += 4) {
        const char *s1 = i < 10 ? "  " : (i < 100 ? " " : "");
        const char *s2 = i + 2 < 10 ? "  " : (i + 2 < 100 ? " " : "");
        qemu_fprintf(f, "%s%%g%d = %16lx  ", s1, i    , env->gregs[i]);
        qemu_fprintf(f, "%s%%g%d = %16lx  ", s1, i + 1, env->gregs[i + 1]);
        qemu_fprintf(f, "%s%%g%d = %16lx  ", s2, i + 2, env->gregs[i + 2]);
        qemu_fprintf(f, "%s%%g%d = %16lx\n", s2, i + 3, env->gregs[i + 3]);
    }
}

static void e2k_cpu_set_pc(CPUState *cs, vaddr value)
{
    E2KCPU *cpu = E2K_CPU(cs);

    cpu->env.ip = value;
}

static void e2k_cpu_synchronize_from_tb(CPUState *cs, TranslationBlock *tb)
{
    E2KCPU *cpu = E2K_CPU(cs);

    cpu->env.ip = tb->pc;
}

static bool e2k_cpu_has_work(CPUState *cs)
{
    // TODO
    return true;
}

static char *e2k_cpu_type_name(const char *cpu_model)
{
    // TODO
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

static void e2k_cpu_class_init(ObjectClass *oc, void *data)
{
    E2KCPUClass *ecc = E2K_CPU_CLASS(oc);
    CPUClass *cc = CPU_CLASS(oc);
    DeviceClass *dc = DEVICE_CLASS(oc);

    device_class_set_parent_realize(dc, e2k_cpu_realizefn,
                                    &ecc->parent_realize);

    device_class_set_parent_reset(dc, e2k_cpu_reset, &ecc->parent_reset);

    cc->has_work = e2k_cpu_has_work;
    cc->dump_state = e2k_cpu_dump_state;
    cc->set_pc = e2k_cpu_set_pc;
    cc->cpu_exec_interrupt = e2k_cpu_exec_interrupt;
    cc->synchronize_from_tb = e2k_cpu_synchronize_from_tb;
    cc->class_by_name = e2k_cpu_class_by_name;
    cc->disas_set_info = cpu_e2k_disas_set_info;
    cc->tcg_initialize = e2k_tcg_initialize;

    cc->gdb_read_register  = e2k_cpu_gdb_read_register;
    cc->gdb_write_register = e2k_cpu_gdb_write_register;
    cc->gdb_num_core_regs  = 300; /* TODO: bogus value, depends on e2k-linux-gdb */
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
