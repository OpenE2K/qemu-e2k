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
#include "target_elf.h"

void helper_return(CPUE2KState *env);

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
            abi_long args[10] = { 0 };
            int psize = env->wd.psize <= 10 ? env->wd.psize : 10;
            int i;

            for (i = 0; i < psize; i++) {
                args[i] = env->regs[i];
            }

            abi_ulong ret = do_syscall(env, args[0], args[1], args[2], args[3],
                args[4], args[5], args[6], args[7], args[8]);

            if (ret == -TARGET_ERESTARTSYS) {
                /* TODO: restart syscall */
                abort();
            } else if (env->wd.psize > 0 && ret != -TARGET_QEMU_ESIGRETURN) {
                env->regs[0] = ret;
                env->tags[0] = E2K_TAG_NUMBER64;

                for (i = 1; i < env->wd.psize; i++) {
                    env->tags[i] = E2K_TAG_NON_NUMBER64;
                }
            }
            // FIXME: Can call helpers from here?
            helper_return(env);
            break;
        }
        case E2K_EXCP_ILLOPC:
        case E2K_EXCP_ILLOPN:
            info.si_signo = TARGET_SIGILL;
            info.si_errno = 0;
            info.si_code = trapnr == E2K_EXCP_ILLOPC ?
                TARGET_ILL_ILLOPC : TARGET_ILL_ILLOPN;
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
        case E2K_EXCP_DIV:
            info.si_signo = TARGET_SIGFPE;
            info.si_errno = 0;
            info.si_code = 0;
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
    CPUState *cpu = env_cpu(env);
    TaskState *ts = cpu->opaque;
    struct image_info *info = ts->info;
    uint32_t eflags = info->elf_flags;

    env->ip = regs->ip;
    env->pcsp = regs->pcsp;
    env->psp = regs->psp;
    env->usd.lo = regs->usd_lo;
    env->usd.hi = regs->usd_hi;
    env->sbr = regs->sbr;
    env->elf_flags = info->elf_flags;

    if (eflags & E2K_ELF_PM) {
        fprintf(stderr, "Protected mode is unsupported\n");
        exit(EXIT_FAILURE);
    }

    if (eflags & E2K_ELF_X86APP) {
        fprintf(stderr, "x86 recompiler is unsupported\n");
        exit(EXIT_FAILURE);
    }

    e2k_break_save_state(env);

    env->pcs_base = env->pcsp.base;
    env->ps_base = env->psp.base;
}

const char *cpu_get_model(uint32_t eflags)
{
    const char *name = "any";
    uint32_t machine = E2K_ELF_MACH(eflags);

    /* TODO: can't check for EM_E2K_OLD flags because e_machine isn't saved anywhere... */
    switch(machine) {
    case E2K_MACH_EV2: name = "e2c+"; break;
    case E2K_MACH_EV3: name = "e2s"; break;
    case E2K_MACH_1CPLUS:
    case E2K_MACH_8C:
    case E2K_MACH_EV4: name = "e8c"; break;
    case E2K_MACH_EV5: name = "e8c2"; break;
    case E2K_MACH_EV6: name = "e16c"; break;
    }

    return name;
}
