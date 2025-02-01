/*
 * QEMU E2K CPU
 *
 * SPDX-License-Identifier: GPL-2.0-only
 */

#ifndef TARGET_E2K_CPU_QOM_H
#define TARGET_E2K_CPU_QOM_H

#include "hw/core/cpu.h"
#include "qom/object.h"

#define TYPE_E2K_CPU "e2k-cpu"

OBJECT_DECLARE_CPU_TYPE(E2KCPU, E2KCPUClass, E2K_CPU)

typedef struct e2k_def_t e2k_def_t;
/**
 * E2KCPUClass:
 * @parent_realize: The parent class' realize handler.
 * @parent_phases: The parent class' reset phase handlers.
 *
 * A E2K CPU model.
 */
struct E2KCPUClass {
    /*< private >*/
    CPUClass parent_class;
    /*< public >*/

    DeviceRealize parent_realize;
    ResettablePhases parent_phasses;
    e2k_def_t *cpu_def;
};

#endif /* TARGET_E2K_CPU_QOM_H */
