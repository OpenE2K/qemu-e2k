/*
 * Copyright (c) 2021 Alibek Omarov
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
#ifndef HW_LMSCON_H
#define HW_LMSCON_H

#define TYPE_LMSCON "lmscon"

OBJECT_DECLARE_SIMPLE_TYPE(LMSCONState, LMSCON)

struct LMSCONState {
    DeviceState parent;

    uint64_t baseaddr;
    CharBackend chr;
    uint8_t tx;
};


#endif /* HW_LMSCON_H */
