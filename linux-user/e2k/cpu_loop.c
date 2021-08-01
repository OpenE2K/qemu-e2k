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

static void gen_signal(CPUE2KState *env, int signo, int code, abi_ulong addr)
{
    target_siginfo_t info = {
        .si_signo = signo,
        .si_code = code,
        ._sifields._sigfault._addr = addr,
        // TODO: ._sifields._sigfault._trapno = trapnr
    };

    queue_signal(env, signo, QEMU_SI_FAULT, &info);
}

static void stack_expand(CPUE2KState *env, E2KPsp *s)
{
    abi_ulong new_size, new_size_tag;
    abi_long new_base, new_base_tag = 0;

    new_size = s->size * 2;
    new_size_tag = new_size / 8;
    new_base = target_mremap(s->base, s->size, new_size, MREMAP_MAYMOVE, 0);
    if (s->base_tag) {
        new_base_tag = target_mremap(s->base_tag, s->size / 8, new_size_tag,
            MREMAP_MAYMOVE, 0);
    }
    if (new_base == -1 || new_base_tag == -1) {
        gen_signal(env, TARGET_SIGSEGV, TARGET_SEGV_MAPERR, env->ip);
        return;
    }

    s->base = new_base;
    s->base_tag = new_base_tag;
    s->size = new_size;
}

void cpu_loop(CPUE2KState *env)
{
    CPUState *cs = env_cpu(env);
    int trapnr;

    while (1) {
        cpu_exec_start(cs);
        trapnr = cpu_exec(cs);
        cpu_exec_end(cs);
        process_queued_cpu_work(cs);

        switch (trapnr) {
        case EXCP_SYSCALL: {
            abi_ullong args[E2K_SYSCALL_MAX_ARGS] = { 0 };
            int i, psize = MIN(E2K_SYSCALL_MAX_ARGS, env->wd.size);
            abi_ulong ret;

            // TODO: check what happens if env->wd.size is zero
            for (i = 0; i < psize; i++) {
                args[i] = env->regs[i].lo;
            }

            ret = do_syscall(env, args[0], args[1], args[2], args[3],
                args[4], args[5], args[6], args[7], args[8]);

            if (ret == -TARGET_ERESTARTSYS) {
                /* do not set sysret address and syscall will be restarted */
            } else if (ret != -TARGET_QEMU_ESIGRETURN && env->wd.psize > 0) {
                memset(env->tags, E2K_TAG_NON_NUMBER64,
                    psize * sizeof(env->tags[0]));

                env->regs[0].lo = ret;
                env->tags[0] = E2K_TAG_NUMBER64;
                env->ip = E2K_SYSRET_ADDR;
            }
            break;
        }
        case EXCP_ILLEGAL_OPCODE:
        case EXCP_PRIV_ACTION:
            gen_signal(env, TARGET_SIGILL, TARGET_ILL_ILLOPC, env->ip);
            break;
        case EXCP_ILLEGAL_OPERAND:
            gen_signal(env, TARGET_SIGILL, TARGET_ILL_ILLOPN, env->ip);
            break;
        case EXCP_CHAIN_STACK_BOUNDS:
            stack_expand(env, &env->pcsp);
            break;
        case EXCP_PROC_STACK_BOUNDS:
            stack_expand(env, &env->psp);
            break;
        case EXCP_WINDOW_BOUNDS:
        case EXCP_ARRAY_BOUNDS:
        case EXCP_DATA_PAGE:
            gen_signal(env, TARGET_SIGSEGV, TARGET_SEGV_MAPERR, env->ip);
            break;
        case EXCP_DIV:
            gen_signal(env, TARGET_SIGFPE, 0, env->ip);
            break;
        /* QEMU common interrupts */
        case EXCP_INTERRUPT:
            /* just indicate that signals should be handled asap */
            break;
        case EXCP_DEBUG:
            env->is_bp = true;
            e2k_proc_call(env, env->wd.size, env->ip, true);
            gen_signal(env, TARGET_SIGTRAP, TARGET_TRAP_BRKPT, 0);
            break;
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
