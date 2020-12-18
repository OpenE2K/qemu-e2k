/*
 * QEMU TCG vCPU common functionality
 *
 * Functionality common to all TCG vcpu variants: mttcg, rr and icount.
 *
 * Copyright 2020 SUSE LLC
 *
 * This work is licensed under the terms of the GNU GPL, version 2 or later.
 * See the COPYING file in the top-level directory.
 */

#ifndef TCG_CPUS_H
#define TCG_CPUS_H

#include "sysemu/cpus.h"

extern const CpusAccel tcg_cpus_mttcg;
extern const CpusAccel tcg_cpus_icount;
extern const CpusAccel tcg_cpus_rr;

void tcg_cpus_destroy(CPUState *cpu);
int tcg_cpus_exec(CPUState *cpu);
void tcg_cpus_handle_interrupt(CPUState *cpu, int mask);

#endif /* TCG_CPUS_H */
