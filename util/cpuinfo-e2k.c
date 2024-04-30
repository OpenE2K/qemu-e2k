/*
 * SPDX-License-Identifier: GPL-2.0-or-later
 * Host specific cpu indentification for E2K.
 */

#include "qemu/osdep.h"
#include "host/cpuinfo.h"

unsigned cpuinfo;

/* Called both as constructor and (possibly) via other constructors. */
unsigned __attribute__((constructor)) cpuinfo_init(void)
{
    unsigned info = cpuinfo;

    if (info) {
        return info;
    }

    info |= CPUINFO_ALWAYS;
    cpuinfo = info;
    return info;
}
