/*
 *  qemu user cpu loop
 *
 *  Copyright (c) 2003-2008 Fabrice Bellard
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
#include "qemu-common.h"
#include "qemu.h"
#include "cpu_loop-common.h"

void cpu_loop(CPUE2KState *env)
{
    CPUState *cs = env_cpu(env);
    int trapnr;
    target_siginfo_t info;

    while (1) {
        cpu_exec_start(cs);
        trapnr = cpu_exec(cs);
        cpu_exec_end(cs);
        process_queued_cpu_work(cs);

        switch (trapnr) {
        case E2K_EXCP_SYSCALL: {
            int offset = env->wd.base + env->syscall_wbs * 2;
            uint64_t *regs = env->regs;
            abi_ulong ret = do_syscall(env,
                regs[(0 + offset) % E2K_NR_COUNT],
                regs[(1 + offset) % E2K_NR_COUNT],
                regs[(2 + offset) % E2K_NR_COUNT],
                regs[(3 + offset) % E2K_NR_COUNT],
                regs[(4 + offset) % E2K_NR_COUNT],
                regs[(5 + offset) % E2K_NR_COUNT],
                regs[(6 + offset) % E2K_NR_COUNT],
                regs[(7 + offset) % E2K_NR_COUNT],
                regs[(8 + offset) % E2K_NR_COUNT]
            );
            if (ret == -TARGET_ERESTARTSYS) {
                /* TODO: restart syscall */
                abort();
            } else if (ret != -TARGET_QEMU_ESIGRETURN) {
                unsigned int i;

                regs[offset % E2K_NR_COUNT] = ret;
                for (i = 1; i < 8; i++) {
                    regs[(i + offset) % E2K_NR_COUNT] = 0;
                }
            }
            break;
        }
        case E2K_EXCP_ILLOPC:
            info.si_signo = TARGET_SIGILL;
            info.si_errno = 0;
            info.si_code = TARGET_ILL_ILLOPC;
            info._sifields._sigfault._addr = env->ip;
            queue_signal(env, info.si_signo, QEMU_SI_FAULT, &info);
            break;
        case E2K_EXCP_ILLOPN:
            info.si_signo = TARGET_SIGILL;
            info.si_errno = 0;
            info.si_code = TARGET_ILL_ILLOPN;
            info._sifields._sigfault._addr = env->ip;
            queue_signal(env, info.si_signo, QEMU_SI_FAULT, &info);
            break;
        case E2K_EXCP_MAPERR:
            info.si_signo = TARGET_SIGSEGV;
            info.si_errno = 0;
            info.si_code = TARGET_SEGV_MAPERR;
            info._sifields._sigfault._addr = env->ip;
            queue_signal(env, info.si_signo, QEMU_SI_FAULT, &info);
            break;
        /* QEMU common interrupts */
        case EXCP_INTERRUPT:
            /* just indicate that signals should be handled asap */
            break;
        case EXCP_DEBUG:
        {
            info.si_signo = TARGET_SIGTRAP;
            info.si_errno = 0;
            info.si_code = TARGET_TRAP_BRKPT;
            queue_signal(env, info.si_signo, QEMU_SI_FAULT, &info);
            break;
        }
        case EXCP_ATOMIC:
            cpu_exec_step_atomic(cs);
            break;
        case EXCP_HLT:
        case EXCP_HALTED:
        case EXCP_YIELD:
            fprintf(stderr, "Unhandled QEMU trap: 0x%x\n", trapnr);
            break;
        default:
            fprintf(stderr, "Unhandled trap: 0x%x\n", trapnr);
            cpu_dump_state(cs, stderr, 0);
            exit(EXIT_FAILURE);
        }
        process_pending_signals (env);
    }
}

void target_cpu_copy_regs(CPUE2KState *env, struct target_pt_regs *regs)
{
    env->ip = regs->ip;
    env->pcsp = regs->pcsp;
    env->psp = regs->psp;
    env->usd_lo = regs->usd_lo;
    env->usd_hi = regs->usd_hi;

    e2k_break_save_state(env);
}
