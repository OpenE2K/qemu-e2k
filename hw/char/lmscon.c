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

#include "qemu/osdep.h"
#include "qemu/log.h"
#include "exec/memory.h"
#include "exec/address-spaces.h"
#include "hw/qdev-properties.h"
#include "hw/qdev-properties-system.h"
#include "hw/sysbus.h"
#include "chardev/char-fe.h"
#include "hw/char/lmscon.h"
#include "trace.h"

#define LMS_CONS_DATA_PORT      0x300UL /* On READ  - data from keyboard      */
                                        /* On WRITE - data to debug ouput     */
                                        /* port (console/journal)             */

#define LMS_CONS_STATUS_PORT    0x301UL /* On READ  - data available on 0x300 */
                                        /* On WRITE - shift count   for 0x304 */

#define LMS_NSOCK_BADDR_PORT    0x302UL /* On READ  - network socket base addr*/
                                        /* On WRITE - the same.               */

#define LMS_NSOCK_DATA_PORT     0x303UL /* On READ  - data from network socket*/
                                        /* On WRITE - data   to network socket*/

#define LMS_TRACE_CNTL_PORT     0x304UL /* On READ  - state of the instruction*/
                                        /* counter */
                                        /* On WRITE - LMS tracer control      */
                                        /* (1 - start, 0 - stop)              */

#define LMS_TRACE_CNTL_OFF      0
#define LMS_TRACE_CNTL_ON       1

static void lmscon_mem_write(void *opaque, hwaddr addr,
                           uint64_t val, unsigned size)
{
    LMSCONState *s = opaque;

    trace_lmscon_write(addr, val, size);

    if (!qemu_chr_fe_backend_connected(&s->chr))
        return;

    switch(LMS_CONS_DATA_PORT + addr)
    {
    case LMS_CONS_DATA_PORT:
        if (size == 1) {
            s->tx = val;
            qemu_chr_fe_write_all(&s->chr, (uint8_t*)&s->tx, size);
        } else {
            qemu_log_mask(LOG_UNIMP, "%s: LMS_CONS_DATA_PORT unknown size %d\n",
                          __func__, size);
        }
        break;
    default:
        qemu_log_mask(LOG_UNIMP, "%s: unimplemented lmscon packet %lx\n",
                      __func__, LMS_CONS_DATA_PORT + addr);
        break;
    }
}

static uint64_t lmscon_mem_read(void *opaque, hwaddr addr, unsigned size)
{
    LMSCONState *s = opaque;
    uint64_t r = 0;

    switch(LMS_CONS_DATA_PORT + addr)
    {
    case LMS_CONS_DATA_PORT:
        if (!qemu_chr_fe_backend_connected(&s->chr))
            r = 0xFFFFFFFF; /* port disabled */
        else qemu_chr_fe_read_all(&s->chr, (uint8_t*)&r, size);
        break;
    default:
        qemu_log_mask(LOG_UNIMP, "%s: unimplemented lmscon packet %lx\n",
                      __func__, LMS_CONS_DATA_PORT + addr);
        break;
    }

    trace_lmscon_read(addr, r, size);

    return r;
}

static const MemoryRegionOps lmscon_mem_ops = {
    .read = lmscon_mem_read,
    .write = lmscon_mem_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
    .valid = {
        .min_access_size = 1,
        .max_access_size = 4,
    },
};

static void lmscon_realize(DeviceState *dev, Error **errp)
{
    SysBusDevice *sb = SYS_BUS_DEVICE(dev);
    LMSCONState *s = LMSCON(dev);
    const uint64_t size = LMS_TRACE_CNTL_PORT - LMS_CONS_DATA_PORT + 1;

    memory_region_init_io(&s->io, OBJECT(dev), &lmscon_mem_ops, s, "lmscon", size);
    
    sysbus_add_io(sb, LMS_CONS_DATA_PORT, &s->io);
    sysbus_init_ioports(sb, LMS_CONS_DATA_PORT, size);
}

static Property lmscon_properties[] = {
    DEFINE_PROP_CHR("chr", LMSCONState, chr),
    DEFINE_PROP_END_OF_LIST(),
};

static void lmscon_class_init(ObjectClass *oc, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(oc);

    dc->realize = lmscon_realize;

    device_class_set_props(dc, lmscon_properties);
    set_bit(DEVICE_CATEGORY_INPUT, dc->categories);
}

static TypeInfo lmscon_info = {
    .name          = TYPE_LMSCON,
    .parent        = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(LMSCONState),
    .class_init    = lmscon_class_init
};

static void lmscon_register_types(void)
{
    type_register_static(&lmscon_info);
}

type_init(lmscon_register_types)
