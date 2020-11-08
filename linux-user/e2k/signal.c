/*
 *  Emulation of Linux signals
 *
 *  Copyright (c) 2003 Fabrice Bellard
 *  Copyright (c) 2020 Alibek Omarov
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, see <http://www.gnu.org/licenses/>.
 */
#include "qemu/osdep.h"
#include "qemu.h"
#include "signal-common.h"
#include "linux-user/trace.h"

#define MAX_TC_SIZE	10

#define TIR_NUM 19
#define DAM_ENTRIES_NUM 32
#define SBBP_ENTRIES_NUM 32

/* from user.h !!! */
#define MLT_NUM (16 * 3)		/* common for E3M and E3S */

struct target_sigcontext {
	abi_ulong	cr0_lo;
	abi_ulong	cr0_hi;
	abi_ulong	cr1_lo;
	abi_ulong	cr1_hi;
	abi_ulong	sbr;	 /* 21 Stack base register: top of */
					 /*    local data (user) stack */
	abi_ulong	usd_lo;	 /* 22 Local data (user) stack */
	abi_ulong	usd_hi;	 /* 23 descriptor: base & size */
	abi_ulong	psp_lo;	 /* 24 Procedure stack pointer: */
	abi_ulong	psp_hi;	 /* 25 base & index & size */
	abi_ulong	pcsp_lo; /* 26 Procedure chain stack */
	abi_ulong	pcsp_hi; /* 27 pointer: base & index & size */
/*
 *  additional part (for binary compiler)
 */
    uint8_t   bincomp_padding[sizeof(abi_ulong)*184 + 10];

#if 0
	abi_ulong rpr_hi;
	abi_ulong rpr_lo;

	abi_ulong nr_TIRs;
	abi_ulong tir_lo[TIR_NUM];
	abi_ulong tir_hi[TIR_NUM];
	abi_ulong trap_cell_addr[MAX_TC_SIZE];
	abi_ulong trap_cell_val[MAX_TC_SIZE];
	uint8_t      trap_cell_tag[MAX_TC_SIZE];
	abi_ulong trap_cell_info[MAX_TC_SIZE];

	abi_ulong dam[DAM_ENTRIES_NUM];

	abi_ulong sbbp[SBBP_ENTRIES_NUM];

	abi_ulong mlt[MLT_NUM];

	abi_ulong upsr;
#endif
};

#define NF_ALIGNEDSZ  (((sizeof(struct target_signal_frame) + 7) & (~7)))

void setup_frame(int sig, struct target_sigaction *ka,
                 target_sigset_t *set, CPUE2KState *env)
{
    qemu_log_mask(LOG_UNIMP, "setup_frame: not implemented\n");
}

void setup_rt_frame(int sig, struct target_sigaction *ka,
                    target_siginfo_t *info,
                    target_sigset_t *set, CPUE2KState *env)
{
    qemu_log_mask(LOG_UNIMP, "setup_rt_frame: not implemented\n");
}

long do_sigreturn(CPUE2KState *env)
{
    // TODO
    return 0;
}

long do_rt_sigreturn(CPUE2KState *env)
{
    trace_user_do_rt_sigreturn(env, 0);
    qemu_log_mask(LOG_UNIMP, "do_rt_sigreturn: not implemented\n");
    return -TARGET_ENOSYS;
}

abi_long do_swapcontext(CPUArchState *env, abi_ulong uold_ctx,
                        abi_ulong unew_ctx, abi_long ctx_size)
{
    // TODO
    qemu_log_mask(LOG_UNIMP, "do_swapcontext: not implemented\n");
    return 0;
}
