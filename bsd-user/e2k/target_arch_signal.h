/*
 *  E2K signal definitions
 *
 *  Copyright (c) 2021 Gleb Popov <arrowd@freebsd.org>
 *
 *  Redistribution and use in source and binary forms, with or without modification,
 *  are permitted provided that the following conditions are met:
 *  1. Redistributions of source code must retain the above copyright notice,
 *  this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *  this list of conditions and the following disclaimer in the documentation
 *  and/or other materials provided with the distribution.
 *  3. Neither the name of the copyright holder nor the names of its contributors may
 *  be used to endorse or promote products derived from this software without specific
 *  prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 *  IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 *  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 *  OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 *  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _TARGET_ARCH_SIGNAL_H_
#define _TARGET_ARCH_SIGNAL_H_

#include "cpu.h"

#define MAX_TC_SIZE     10

#define TIR_NUM         19
#define DAM_ENTRIES_NUM 32
#define SBBP_ENTRIES_NUM    32

/* from user.h !!! */
#define MLT_NUM (16 * 3)    /* common for E3M and E3S */

/* TODO: Size of the signal trampolin code placed on the stack. */
#define TARGET_SZSIGCODE    0

#define TARGET_MINSIGSTKSZ      (512 * 4)   /* min sig stack size */

#define TARGET_MC_GET_CLEAR_RET 0x0001

typedef struct target_mcontext {
    abi_ullong cr0_lo;
    abi_ullong cr0_hi;
    abi_ullong cr1_lo;
    abi_ullong cr1_hi;
    abi_ullong sbr;	 /* 21 Stack base register: top of */
                        /*    local data (user) stack */
    abi_ullong usd_lo;	 /* 22 Local data (user) stack */
    abi_ullong usd_hi;	 /* 23 descriptor: base & size */
    abi_ullong psp_lo;	 /* 24 Procedure stack pointer: */
    abi_ullong psp_hi;	 /* 25 base & index & size */
    abi_ullong pcsp_lo; /* 26 Procedure chain stack */
    abi_ullong pcsp_hi; /* 27 pointer: base & index & size */

    /* additional part (for binary compiler) */
    abi_ullong rpr_hi;
    abi_ullong rpr_lo;

    abi_ullong nr_TIRs;
    abi_ullong tir_lo[TIR_NUM];
    abi_ullong tir_hi[TIR_NUM];
    abi_ullong trap_cell_addr[MAX_TC_SIZE];
    abi_ullong trap_cell_val[MAX_TC_SIZE];
    uint8_t    trap_cell_tag[MAX_TC_SIZE];
    abi_ullong trap_cell_info[MAX_TC_SIZE];

    abi_ullong dam[DAM_ENTRIES_NUM];
    abi_ullong sbbp[SBBP_ENTRIES_NUM];
    abi_ullong mlt[MLT_NUM];
    abi_ullong upsr;
} target_mcontext_t;

typedef struct target_ucontext {
    target_sigset_t     uc_sigmask;
    target_mcontext_t   uc_mcontext;
    abi_ulong           uc_link;
    target_stack_t      uc_stack;
    int32_t             uc_flags;
    int32_t             __spare__[4];
} target_ucontext_t;

struct target_sigframe {
    target_siginfo_t sf_si;
    union {
        target_ucontext_t sf_uc;
        // TODO: ucontext_prot
    };

    /* FIXME: move this data to TaskState? */
    E2KAauState aau;
    uint64_t lsr;
    uint64_t lsr_lcnt;
    uint32_t ilcr;
    uint64_t ilcr_lcnt;
    // FIXME: according to ABI only 16-31 must be saved
    E2KReg gregs[16];
    uint8_t gtags[16];
};

static inline abi_long set_sigtramp_args(CPUE2KState *regs,
        int sig, struct target_sigframe *frame, abi_ulong frame_addr,
        struct target_sigaction *ka)
{
    assert(0 && "set_sigtramp_args not implemented yet");
    /* XXX */
    return -TARGET_EOPNOTSUPP;
}

static inline abi_long get_mcontext(CPUE2KState *regs,
                target_mcontext_t *mcp, int flags)
{
    assert(0 && "get_mcontext not implemented yet");
    /* XXX */
    return -TARGET_EOPNOTSUPP;
}

static inline abi_long set_mcontext(CPUE2KState *regs,
        target_mcontext_t *mcp, int srflag)
{
    assert(0 && "set_mcontext not implemented yet");
    /* XXX */
    return -TARGET_EOPNOTSUPP;
}

static inline abi_long get_ucontext_sigreturn(CPUE2KState *regs,
        abi_ulong target_sf, abi_ulong *target_uc)
{
    /* XXX */
    *target_uc = 0;
    assert(0 && "get_ucontext_sigreturn not implemented yet");
    return -TARGET_EOPNOTSUPP;
}

#endif /* !TARGET_ARCH_SIGNAL_H_ */
