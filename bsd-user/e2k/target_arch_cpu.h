/*
 *  E2K cpu init and loop
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

#ifndef _TARGET_ARCH_CPU_H_
#define _TARGET_ARCH_CPU_H_

#include "target_arch.h"
#include "target_arch_elf.h"

#define TARGET_DEFAULT_CPU_MODEL "e8c"

static inline void target_cpu_init(CPUE2KState *env,
        struct target_pt_regs *regs)
{
    CPUState *cpu = env_cpu(env);
    TaskState *ts = cpu->opaque;
    struct image_info *info = ts->info;
    uint32_t eflags = info->elf_flags;

    env->psr = PSR_NMIE | PSR_SGE | PSR_IE;
    env->upsr = UPSR_NMIE | UPSR_IE | UPSR_FE;
    env->ip = regs->ip;
    env->pcsp = regs->pcsp;
    env->psp = regs->psp;
    env->usd.lo = regs->usd_lo;
    env->usd.hi = regs->usd_hi;
    env->sbr = regs->sbr;
    env->elf_flags = info->elf_flags;

    // TODO: set a chain info to return to kernel

    if (eflags & E2K_ELF_PM) {
        fprintf(stderr, "Protected mode is unsupported\n");
        exit(EXIT_FAILURE);
    }

    if (eflags & E2K_ELF_X86APP) {
        fprintf(stderr, "x86 recompiler is unsupported\n");
        exit(EXIT_FAILURE);
    }
}

static inline void target_cpu_loop(CPUE2KState *env)
{
    CPUState *cs = env_cpu(env);
    int trapnr;

    for (;;) {
        cpu_exec_start(cs);
        trapnr = cpu_exec(cs);
        cpu_exec_end(cs);
        process_queued_cpu_work(cs);

        switch (trapnr) {

        default:
            fprintf(stderr, "Unhandled trap: 0x%x\n", trapnr);
            cpu_dump_state(cs, stderr, 0);
            abort();
        }
        process_pending_signals(env);
    }
}

static inline void target_cpu_clone_regs(CPUE2KState *env, target_ulong newsp)
{
    if (newsp)
        env->usd.curptr = newsp;
}

static inline void target_cpu_set_tls(CPUE2KState *env, target_ulong newtls)
{
    assert(0 && "target_cpu_set_tls not implemented yet");
}


#endif /* !_TARGET_ARCH_CPU_H */
