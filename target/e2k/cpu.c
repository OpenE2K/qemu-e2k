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

    env->rptr = &env->regs[0];
    env->tptr = &env->tags[0];
    env->cr1.wpsz = 4;
    env->cr1.wbs = 4;
    env->wd.base = 0;
    env->wd.size = 16;
    env->wd.psize = 8;
    env->bn.base = 8;
    env->bn.size = 8;
    env->bn.cur = 0;
    env->aau.incrs[0] = 1; /* always one */

    // FIXME: testing
    env->idr = 0x3a207; // mimic 8c
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

static void cpu_e2k_disas_set_info(CPUState *cpu, disassemble_info *info)
{
    info->print_insn = print_insn_e2k;
}


void cpu_e2k_set_id(CPUE2KState *env, unsigned int cpu)
{
    // TODO: cpu_e2k_set_id
    qemu_log_mask(LOG_UNIMP, "cpu_e2k_set_id: not implemented\n");
}

/* https://www.altlinux.org/Модели_процессоров_Эльбрус */
#define DEFAULT_CPU_MODEL "e8c"
static const struct e2k_def_t e2k_defs[] = {
    {
        .name           = "e8c", // default choice
        .canonical_name = "MCST Elbrus 8C",
        .gdb_arch       = "elbrus-8c",
        .isa_version    = 4,
    },
    {
        .name           = "e2c+", // however it works better
        .canonical_name = "MCST Elbrus 2C+ (Monocube)",
        .gdb_arch       = "elbrus-v2",
        .isa_version    = 2,
    },
#if 0 /* for reference, never tested */
    {
        .name           = "e2s",
        .canonical_name = "MCST Elbrus 4C",
        .gdb_arch       = "elbrus-v3",
        .isa_version    = 3,
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
#endif
};

static inline void cpu_dump_state_br(CPUE2KState *env, FILE *f, int flags)
{
    uint32_t br = e2k_state_br(env);
    E2KBnState *bn = &env->bn;
    E2KBpState *bp = &env->bp;

    qemu_fprintf(f, "br         0x%x\n", br);
    qemu_fprintf(f, "    rbs    %d\n", bn->base / 2);
    qemu_fprintf(f, "    rsz    %d\n", bn->size / 2 - 1);
    qemu_fprintf(f, "    rcur   %d\n", bn->cur / 2);
    qemu_fprintf(f, "    psz    %d\n", bp->size);
    qemu_fprintf(f, "    pcur   %d\n", bp->cur);
}

static inline int extract_tag(uint64_t *tags, int idx)
{
    uint64_t tmp = tags[idx / E2K_TAGS_PER_REG];
    int offset = (idx & GEN_MASK(0, E2K_REG_TAGS_SIZE)) * E2K_REG_TAGS_SIZE;
    return extract64(tmp, offset, E2K_REG_TAGS_SIZE);
}

void e2k_cpu_dump_state(CPUState *cs, FILE *f, int flags)
{
    E2KCPU *cpu = E2K_CPU(cs);
    CPUE2KState *env = &cpu->env;
    unsigned int i;

    qemu_fprintf(f, "cr0_lo/pf = 0x%016lx\n", env->cr0_lo);
    qemu_fprintf(f, "cr0_hi/ip = 0x%016lx\n", env->cr0_hi);
    qemu_fprintf(f, "   cr1_lo = 0x%016lx\n", e2k_state_cr1_lo(env));
    qemu_fprintf(f, "   cr1_hi = 0x%016lx\n", e2k_state_cr1_hi(env));
    qemu_fprintf(f, "  pcsp_lo = 0x%016lx\n", e2k_state_pcsp_lo(env));
    qemu_fprintf(f, "  pcsp_hi = 0x%016lx\n", e2k_state_pcsp_hi(env));
    qemu_fprintf(f, "   psp_lo = 0x%016lx\n", e2k_state_psp_lo(env));
    qemu_fprintf(f, "   psp_hi = 0x%016lx\n", e2k_state_psp_hi(env));
    qemu_fprintf(f, "   usd_lo = 0x%016lx\n", env->usd.lo);
    qemu_fprintf(f, "   usd_hi = 0x%016lx\n", env->usd.hi);
    qemu_fprintf(f, "      lsr = 0x%016lx\n", env->lsr);
    cpu_dump_state_br(env, f, flags);

    for (i = 0; i < E2K_REG_COUNT; i++) {
        char name = i < E2K_NR_COUNT ? 'r' : 'g';
        int tag = extract_tag(env->tags, i);
        qemu_fprintf(f, "%%%c%d\t<%d%d> 0x%lx\n", name, i, tag >> 2, tag & 3,
            env->regs[i]);
    }

    for (i = 0; i < 32; i++) {
        int preg = (env->pregs >> (i * 2)) & 3;
        qemu_fprintf(f, "pred%d\t<%d> %s\n", i, preg >> 1,
            preg & 1 ? "true" : "false");
    }
}

static void e2k_cpu_set_pc(CPUState *cs, vaddr value)
{
    E2KCPU *cpu = E2K_CPU(cs);

    qemu_log_mask(LOG_UNIMP, "e2k_cpu_synchronize_from_tb: not implemented\n");

    cpu->env.ip = value;
}

static void e2k_cpu_synchronize_from_tb(CPUState *cs, TranslationBlock *tb)
{
    E2KCPU *cpu = E2K_CPU(cs);

    qemu_log_mask(LOG_UNIMP, "e2k_cpu_synchronize_from_tb: not implemented\n");

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
#endif // CONFIG_USER_ONLY
   
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

static gchar* e2k_cpu_gdb_arch_name(CPUState *cs)
{
    E2KCPU *cpu = E2K_CPU(cs);
    CPUE2KState *env = &cpu->env;

    return g_strdup_printf("%s:%d", env->def.gdb_arch, TARGET_LONG_BITS);
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
    cc->do_interrupt = e2k_cpu_do_interrupt;
    cc->cpu_exec_interrupt = e2k_cpu_exec_interrupt;
    cc->synchronize_from_tb = e2k_cpu_synchronize_from_tb;
    cc->class_by_name = e2k_cpu_class_by_name;
    cc->disas_set_info = cpu_e2k_disas_set_info;
    cc->tcg_initialize = e2k_tcg_initialize;

    cc->gdb_arch_name      = e2k_cpu_gdb_arch_name;
    cc->gdb_read_register  = e2k_cpu_gdb_read_register;
    cc->gdb_write_register = e2k_cpu_gdb_write_register;
    cc->gdb_num_core_regs  = 574;
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
    
    for (i = 0; i < ARRAY_SIZE(e2k_defs); i++) {
        qemu_printf("%6s (%-30s) ISA version: v%d\n",
            e2k_defs[i].name,
            e2k_defs[i].canonical_name,
            e2k_defs[i].isa_version
        );
    }
}
