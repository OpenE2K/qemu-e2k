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

#ifndef HW_E2K_H
#define HW_E2K_H

#include "qemu-common.h"

#include "hw/boards.h"
#include "qom/object.h"

struct E2KMachineClass {
    /*< private >*/
    MachineClass parent;

    /*< public >*/
};

struct E2KMachineState {
    /*< private >*/
    MachineClass parent;

    /*< public >*/
};

#define TYPE_E2K_MACHINE    MACHINE_TYPE_NAME("e2k")
OBJECT_DECLARE_TYPE(E2KMachineState, E2KMachineClass, E2K_MACHINE)

#endif /* HW_E2K_H */
