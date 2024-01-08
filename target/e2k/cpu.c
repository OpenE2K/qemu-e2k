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
#include "qemu/log.h"
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
#if !defined(CONFIG_USER_ONLY)
    env->wreg = env->regs;
    env->wtag = env->tags;
    env->wd.base = 0;
    env->wd.size = 16;
    env->wd.psize = 8;
    e2k_set_rbs(env, 8);
    env->bn.size = 8;
    e2k_set_rcur(env, 0);
#endif
    /* Based predicate window must not be zero. */
    env->bp.size = 1;
    env->aau.incrs[0] = 1; /* always one */
    env->fpcr._one = 1; 
    env->fpcr.pc = FPCR_PC_XP;
    env->fpcr.em = FP_EM;
    env->pfpfr.em = FP_EM;

    e2k_update_fp_status(env);
    e2k_update_fx_status(env);

    // FIXME: correct values
    env->psp.base = 0x810000;
    env->psp.size = 0x100000;
    env->pcsp.base = 0x910000;
    env->pcsp.size = 0xa10000;
}

#ifdef CONFIG_SOFTMMU
static bool e2k_cpu_exec_interrupt(CPUState *cs, int interrupt_request)
{
    qemu_log_mask(LOG_UNIMP, "e2k_cpu_exec_interrupt: not implemented\n");
    if (interrupt_request & CPU_INTERRUPT_HARD) {
        e2k_cpu_do_interrupt(cs);
        return true;
    }
    return false;
}
#endif

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

#define MDL_E3M              0    /* Elbrus */
#define MDL_ES               1    /* Elbrus-S */
#define MDL_E3S              2    /* Elbrus-3S */
#define MDL_E2S              3    /* Elbrus-4C */
#define MDL_ES2              4    /* Elbrus-2C+ */
#define MDL_RESERVED         5    /* unknown "reserved" processor */
#define MDL_ES2_NO_DSP       6    /* Elbrus-2CM */
#define MDL_E8C              7    /* Elbrus-8C  */
#define MDL_E1CP             8    /* Elbrus-1C+ */
#define MDL_E8C2             9    /* Elbrus-8C2 */
#define MDL_E12C            10    /* Elbrus-12C */
#define MDL_E16C            11    /* Elbrus-16C */
#define MDL_E2C3            12    /* Elbrus-2C3 */
#define MDL_E48C            13    /* Elbrus-48C FIXME: assumption */
#define MDL_E8V7            14    /* Elbrus-8v7 FIXME: assumption */

#define CPU_MODEL(ISET, NAME, GDB_ARCH, IDR, CANON) \
    { \
        .name           = NAME,     \
        .canonical_name = CANON,    \
        .gdb_arch       = GDB_ARCH, \
        .isa_version    = ISET,     \
        .idr            = IDR,      \
    }

static const struct e2k_def_t e2k_defs[] = {
    CPU_MODEL(1, "elbrus-v1",   "elbrus-v1",    MDL_E3M,        "MCST Elbrus"),
    CPU_MODEL(2, "elbrus-v2",   "elbrus-v2",    MDL_ES,         "MCST Elbrus-S"),
    CPU_MODEL(3, "elbrus-v3",   "elbrus-v3",    MDL_E2S,        "MCST Elbrus-4C"),
    CPU_MODEL(4, "elbrus-v4",   "elbrus-v4",    MDL_E8C,        "MCST Elbrus-8C"),
    CPU_MODEL(5, "elbrus-v5",   "elbrus-v5",    MDL_E8C2,       "MCST Elbrus-8C2"),
    CPU_MODEL(6, "elbrus-v6",   "elbrus-v6",    MDL_E16C,       "MCST Elbrus-16C"),
    CPU_MODEL(7, "elbrus-v7",   "elbrus-v7",    MDL_E48C,       "MCST Elbrus-48C"),
    CPU_MODEL(1, "generic",     "elbrus-v1",    MDL_E3M,        "MCST Elbrus"),
    CPU_MODEL(1, "elbrus",      "elbrus-v1",    MDL_E3M,        "MCST Elbrus"),
    CPU_MODEL(1, "e3m",         "elbrus-v1",    MDL_E3M,        "MCST Elbrus-3M1"),
    CPU_MODEL(2, "elbrus-s",    "elbrus-v2",    MDL_ES,         "MCST Elbrus-S"),
    CPU_MODEL(2, "e3s",         "elbrus-v2",    MDL_E3S,        "MCST Elbrus-3S"),
    CPU_MODEL(2, "es2",         "elbrus-v2",    MDL_ES2,        "MCST Elbrus-2C+ (Monocube)"),
    CPU_MODEL(2, "e2c+",        "elbrus-v2",    MDL_ES2,        "MCST Elbrus-2C+ (Monocube)"),
    CPU_MODEL(2, "e2cplus",     "elbrus-v2",    MDL_ES2,        "MCST Elbrus-2C+ (Monocube)"),
    CPU_MODEL(2, "e2cm",        "elbrus-v2",    MDL_ES2_NO_DSP, "MCST Elbrus-2CM"),
    CPU_MODEL(3, "e2s",         "elbrus-v3",    MDL_E2S,        "MCST Elbrus-4C"),
    CPU_MODEL(3, "e4c",         "elbrus-v3",    MDL_E2S,        "MCST Elbrus-4C"),
    CPU_MODEL(4, "e8c",         "elbrus-8c",    MDL_E8C,        "MCST Elbrus-8C"),
    CPU_MODEL(4, "e1c+",        "elbrus-1c+",   MDL_E1CP,       "MCST Elbrus-1C+"),
    CPU_MODEL(4, "e1cplus",     "elbrus-1c+",   MDL_E1CP,       "MCST Elbrus-1C+"),
    CPU_MODEL(5, "e8c2",        "elbrus-v5",    MDL_E8C2,       "MCST Elbrus-8C2"),
    CPU_MODEL(6, "e16c",        "elbrus-16c",   MDL_E16C,       "MCST Elbrus-16C"),
    CPU_MODEL(6, "e12c",        "elbrus-12c",   MDL_E12C,       "MCST Elbrus-12C"),
    CPU_MODEL(6, "e2c3",        "elbrus-2c3",   MDL_E2C3,       "MCST Elbrus-2C3"),
    CPU_MODEL(7, "e48c",        "elbrus-v7",    MDL_E48C,       "MCST Elbrus-48C"),
    CPU_MODEL(7, "e8v7",        "elbrus-v7",    MDL_E8V7,       "MCST Elbrus-8v7"),
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

static void e2k_restore_state_to_opc(CPUState *cs, const TranslationBlock *tb,
                                     const uint64_t *data)
{
    E2KCPU *cpu = E2K_CPU(cs);
    CPUE2KState *env = &cpu->env;

    env->ip = data[0];
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

    env->idr = env->def.idr;
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
    .restore_state_to_opc = e2k_restore_state_to_opc,

    .do_interrupt = e2k_cpu_do_interrupt,
#ifdef CONFIG_SOFTMMU
    .cpu_exec_interrupt = e2k_cpu_exec_interrupt,
    .tlb_fill = e2k_cpu_tlb_fill,
#endif
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
    cc->gdb_rw_tags        = e2k_cpu_gdb_rw_tags;
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
    Property* prop;
    size_t len = 0;
    
    for (i = 0; i < ARRAY_SIZE(e2k_defs); i++) {
        qemu_printf("%12s (%-30s) ISA version: v%d\n",
            e2k_defs[i].name,
            e2k_defs[i].canonical_name,
            e2k_defs[i].isa_version
        );
    }

    qemu_printf("\nFeatures:\n");
    for (prop = e2k_cpu_properties; prop->name != 0; prop++) {
        size_t l = strlen(prop->name) + 1;
        if (len + l >= 75) {
            qemu_printf("\n");
            len = 0;
        }
        qemu_printf("%s%s", len ? " " : "  ", prop->name);
        len += l;
    }
    qemu_printf("\n");
}

void e2k_psp_new(E2KPsp *psp, uint32_t size, uint64_t base, uint64_t base_tags)
{
    psp->is_readable = true;
    psp->is_writable = true;
    psp->index = 0;
    psp->size = size;
    psp->base = base;
    psp->base_tag = base_tags;
}
