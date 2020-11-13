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
    int trapnr; //TODO , n, sig;
    //target_siginfo_t info;
    //target_ulong addr;
    //abi_long ret;

    while (1) {
        cpu_exec_start(cs);
        trapnr = cpu_exec(cs);
        cpu_exec_end(cs);
        process_queued_cpu_work(cs);

        switch (trapnr) {
        case E2K_EXCP_SYSCALL: {
            /* TODO: wrap register indices */
            uint64_t *regs = &env->wregs[env->wbs + env->syscall_wbs];
            abi_ulong ret = do_syscall(env, regs[0],
                regs[1], regs[2], regs[3], regs[4],
                regs[5], regs[6], regs[7], regs[8]);
            if (ret == -TARGET_ERESTARTSYS) {
                /* TODO: restart syscall */
            } else if (ret != -TARGET_QEMU_ESIGRETURN) {
                regs[0] = ret;
            }
            break;
        }
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
    env->usd_hi = regs->usd_hi;
    env->usd_lo = regs->usd_lo;
}
