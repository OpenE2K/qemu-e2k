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
            int wbs = e2k_state_wbs_get(env);
            int offset = (wbs + env->call_wbs) * 2;
            uint64_t *regs = env->wregs;
            abi_ulong ret = do_syscall(env,
                regs[(0 + offset) % WREGS_SIZE],
                regs[(1 + offset) % WREGS_SIZE],
                regs[(2 + offset) % WREGS_SIZE],
                regs[(3 + offset) % WREGS_SIZE],
                regs[(4 + offset) % WREGS_SIZE],
                regs[(5 + offset) % WREGS_SIZE],
                regs[(6 + offset) % WREGS_SIZE],
                regs[(7 + offset) % WREGS_SIZE],
                regs[(8 + offset) % WREGS_SIZE]
            );
            if (ret == -TARGET_ERESTARTSYS) {
                /* TODO: restart syscall */
            } else if (ret != -TARGET_QEMU_ESIGRETURN) {
                unsigned int i;

                regs[0] = ret;
                for (i = 1; i < 8; i++) {
                    regs[(i + offset) % WREGS_SIZE] = 0;
                }
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
    env->pcsp_lo = regs->pcsp_lo;
    env->pcsp_hi = regs->pcsp_hi;
    env->psp_lo = regs->psp_lo;
    env->psp_hi = regs->psp_hi;
    env->usd_lo = regs->usd_lo;
    env->usd_hi = regs->usd_hi;
}
