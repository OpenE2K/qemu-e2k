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
#include "qemu/help-texts.h"
#include "qemu.h"
#include "user-internals.h"
#include "signal-common.h"
#include "user-mmap.h"
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

static void e2k_clear_probe_page_cache(uint32_t syscall)
{
    CPUState *other_cpu;

	switch (syscall) {
	case TARGET_NR_execve:
	case TARGET_NR_brk:
	case TARGET_NR_mmap:
	case TARGET_NR_munmap:
	case TARGET_NR_clone:
	case TARGET_NR_mmap2:
	case TARGET_NR_mremap:
	case TARGET_NR_remap_file_pages:
	case TARGET_NR_move_pages:
	case TARGET_NR_migrate_pages:
		start_exclusive();
		CPU_FOREACH(other_cpu) {
			E2KCPU *cpu = E2K_CPU(other_cpu);
			CPUE2KState *env = &cpu->env;

			memset(env->probe_cache_page, 0, sizeof(env->probe_cache_page));
			memset(env->probe_cache_flags, 0, sizeof(env->probe_cache_flags));
		}
		end_exclusive();
		break;
	}
}

void cpu_loop(CPUE2KState *env)
{
    CPUState *cs = env_cpu(env);
    int trapnr;

    while (1) {
        if (env->is_bp) {
            e2k_proc_return(env, true);
            env->is_bp = false;
        }

        cpu_exec_start(cs);
        trapnr = cpu_exec(cs);
        cpu_exec_end(cs);
        process_queued_cpu_work(cs);

        switch (trapnr) {
        case E2K_EXCP_SYSCALL: {
            int psize = MIN(E2K_SYSCALL_MAX_ARGS, env->wd.size);

            if (psize) {
                abi_ullong ret, args[E2K_SYSCALL_MAX_ARGS] = { 0 };
                int i;

                for (i = 0; i < psize; i++) {
                    args[i] = env->wreg[i].lo;
                }

                if (!env->enable_tags || (env->wtag[0] & E2K_TAG_MASK_32) == E2K_TAG_NUMBER32) {
                    args[0] = (uint32_t) args[0];
					e2k_clear_probe_page_cache(args[0]);
                    ret = do_syscall(env, args[0], args[1], args[2], args[3],
                                     args[4], args[5], args[6], args[7], args[8]);
                } else {
                    ret = TARGET_ENOSYS;
                }

                if (ret == -QEMU_ERESTARTSYS) {
                    /* do not set sysret address and syscall will be restarted */
                } else if (ret != -QEMU_ESIGRETURN && env->wd.psize > 0) {
                    env->ip = E2K_SYSRET_ADDR;
                    env->wreg[0].lo = ret;

                    if (env->enable_tags) {
						env->wtag[0] = E2K_TAG_NUMBER64;

                        for (i = 1; i < E2K_SYSCALL_MAX_ARGS; i++) {
                            env->wtag[i] = E2K_TAG_NON_NUMBER64;
                        }
                    }
                }
            } else {
                env->ip = E2K_SYSRET_ADDR;
            }
            break;
        }
        case E2K_EXCP_ILLEGAL_OPCODE:
        case E2K_EXCP_PRIV_ACTION:
            gen_signal(env, TARGET_SIGILL, TARGET_ILL_ILLOPC, env->ip);
            break;
        case E2K_EXCP_ILLEGAL_OPERAND:
            gen_signal(env, TARGET_SIGILL, TARGET_ILL_ILLOPN, env->ip);
            break;
        case E2K_EXCP_CHAIN_STACK_BOUNDS:
            stack_expand(env, &env->pcsp);
            break;
        case E2K_EXCP_PROC_STACK_BOUNDS:
            stack_expand(env, &env->psp);
            break;
        case E2K_EXCP_WINDOW_BOUNDS:
            gen_signal(env, TARGET_SIGSEGV, TARGET_SEGV_BNDERR, env->ip);
            break;
        case E2K_EXCP_ARRAY_BOUNDS:
        case E2K_EXCP_DATA_PAGE:
            gen_signal(env, TARGET_SIGSEGV, TARGET_SEGV_MAPERR, env->ip);
            break;
        case E2K_EXCP_DIV:
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

    env->wreg = env->regs;
    env->wtag = env->tags;
    env->wd.base = 0;
    env->wd.size = 16;
    env->wd.psize = 8;
    e2k_set_rbs(env, 8);
    env->bn.size = 8;
    e2k_set_rcur(env, 0);

    // Save initial frame for gdb.
    env->is_bp = true;
    e2k_proc_call(env, env->wd.size, env->ip, true);

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
    case E2K_MACH_EV1: name = "elbrus-v1"; break;
    case E2K_MACH_EV2: name = "elbrus-v2"; break;
    case E2K_MACH_EV3: name = "elbrus-v3"; break;
    case E2K_MACH_1CPLUS:
    case E2K_MACH_8C:
    case E2K_MACH_EV4: name = "elbrus-v4"; break;
    case E2K_MACH_EV5: name = "elbrus-v5"; break;
    case E2K_MACH_12C:
    case E2K_MACH_16C:
    case E2K_MACH_2C3:
    case E2K_MACH_EV6: name = "elbrus-v6"; break;
    case E2K_MACH_48C:
    case E2K_MACH_8V7:
    case E2K_MACH_EV7: name = "elbrus-v7"; break;
    }

    return name;
}
