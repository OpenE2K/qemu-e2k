/*
 * QEMU E2K CPU
 *
 * Copyright (c) 2012 SUSE LINUX Products GmbH
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see
 * <http://www.gnu.org/licenses/lgpl-2.1.html>
 */
#ifndef QEMU_E2K_CPU_QOM_H
#define QEMU_E2K_CPU_QOM_H

#include "hw/core/cpu.h"
#include "qom/object.h"

#define TYPE_E2K_CPU "e2k-cpu"

OBJECT_DECLARE_TYPE(E2KCPU, E2KCPUClass,
                    E2K_CPU)

typedef struct e2k_def_t e2k_def_t;
/**
 * E2KCPUClass:
 * @parent_realize: The parent class' realize handler.
 * @parent_reset: The parent class' reset handler.
 *
 * A E2K CPU model.
 */
struct E2KCPUClass {
    /*< private >*/
    CPUClass parent_class;
    /*< public >*/

    DeviceRealize parent_realize;
    DeviceReset parent_reset;
    e2k_def_t *cpu_def;
};


#endif
