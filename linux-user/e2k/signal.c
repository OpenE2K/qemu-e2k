/*
 *  Emulation of Linux signals
 *
 *  Copyright (c) 2003 Fabrice Bellard
 *  Copyright (c) 2020 Alibek Omarov
 *  Copyright (c) 2021 Denis Drakhnya
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
#include "target/e2k/helper-tcg.h"

#define MAX_TC_SIZE	10

#define TIR_NUM 19
#define DAM_ENTRIES_NUM 32
#define SBBP_ENTRIES_NUM 32

/* from user.h !!! */
#define MLT_NUM (16 * 3)		/* common for E3M and E3S */

struct target_sigcontext {
	abi_ullong	cr0_lo;
	abi_ullong	cr0_hi;
	abi_ullong	cr1_lo;
	abi_ullong	cr1_hi;
	abi_ullong	sbr;	 /* 21 Stack base register: top of */
					 /*    local data (user) stack */
	abi_ullong	usd_lo;	 /* 22 Local data (user) stack */
	abi_ullong	usd_hi;	 /* 23 descriptor: base & size */
	abi_ullong	psp_lo;	 /* 24 Procedure stack pointer: */
	abi_ullong	psp_hi;	 /* 25 base & index & size */
	abi_ullong	pcsp_lo; /* 26 Procedure chain stack */
	abi_ullong	pcsp_hi; /* 27 pointer: base & index & size */

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
};

/*
 * This structure is used for compatibility
 * All new fields must be added in this structure
 */
struct target_extra_ucontext {
    abi_int sizeof_extra_uc;    /* size of used fields(in bytes) */
    abi_int curr_cnt;           /* current index into trap_celler */
    abi_int tc_count;           /* trap_celler records count */

    /*
     * For getcontext()
     */
    abi_int fpcr;
    abi_int fpsr;
    abi_int pfpfr;

    abi_ullong ctpr1;
    abi_ullong ctpr2;
    abi_ullong ctpr3;

    abi_int sc_need_rstrt;
};

struct target_ucontext {
    abi_ulong uc_flags;
    struct target_ucontext *uc_link;
    target_stack_t uc_stack;
    struct target_sigcontext uc_mcontext;
    union {
        target_sigset_t uc_sigmask; /* mask last for extensibility */
        abi_ullong pad[16];
    };
    struct target_extra_ucontext uc_extra; /* for compatibility */
};

struct target_sigframe {
    target_siginfo_t info;
    union {
        struct target_ucontext uc;
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

#define NF_ALIGNEDSZ  (((sizeof(struct target_signal_frame) + 7) & (~7)))

static abi_long setup_sigcontext(CPUE2KState *env,
    struct target_sigcontext *sc, struct target_extra_ucontext *extra)
{
    E2KCrs crs;
    int i, ret;

    // TODO: save binary compiler state (uspr, rpr, MLT)
    __put_user(env->upsr, &sc->upsr);

    ret = e2k_copy_from_user_crs(&crs, env->pcsp.base + env->pcsp.index);
    if (ret) {
        return ret;
    }

    __put_user(crs.cr0_lo, &sc->cr0_lo);
    __put_user(crs.cr0_hi, &sc->cr0_hi);
    __put_user(crs.cr1.lo, &sc->cr1_lo);
    __put_user(crs.cr1.hi, &sc->cr1_hi);

    __put_user(env->sbr, &sc->sbr);
    __put_user(env->usd.lo, &sc->usd_lo);
    __put_user(env->usd.hi, &sc->usd_hi);
    __put_user(env->psp.lo, &sc->psp_lo);
    __put_user(env->psp.hi, &sc->psp_hi);
    __put_user(env->pcsp.lo, &sc->pcsp_lo);
    __put_user(env->pcsp.hi, &sc->pcsp_hi);

    // TODO: save trap state
    __put_user(0, &sc->nr_TIRs);
    __put_user(0, &sc->tir_lo[0]);
    __put_user(0, &sc->tir_hi[0]);
    __put_user(-1, &extra->curr_cnt);
    __put_user(0, &extra->tc_count);

    __put_user(sizeof(struct target_extra_ucontext) - sizeof(abi_int),
        &extra->sizeof_extra_uc);

    __put_user(env->fpcr.raw, &extra->fpcr);
    __put_user(env->fpsr.raw, &extra->fpsr);
    __put_user(env->pfpfr.raw, &extra->pfpfr);

    __put_user(env->ctprs[0].raw, &extra->ctpr1);
    __put_user(env->ctprs[1].raw, &extra->ctpr2);
    __put_user(env->ctprs[2].raw, &extra->ctpr3);

    for (i = 0; i < DAM_ENTRIES_NUM; i++) {
        __put_user(env->dam[i].raw, &sc->dam[i]);
    }

    return 0;
}

static abi_long setup_ucontext(struct target_ucontext *uc, CPUE2KState *env)
{
    __put_user(0, &uc->uc_flags);
    __put_user(0, &uc->uc_link);

    target_save_altstack(&uc->uc_stack, env);
    return setup_sigcontext(env, &uc->uc_mcontext, &uc->uc_extra);
}

static abi_ulong get_sigframe(struct target_sigaction *ka, CPUE2KState *env,
    size_t frame_size)
{
    abi_ulong sp;

    sp = target_sigsp(get_sp_from_cpustate(env), ka);
    sp = (sp - frame_size) & ~15;

    return sp;
}

static void target_setup_frame(int sig, struct target_sigaction *ka,
    target_siginfo_t *info, target_sigset_t *set, CPUE2KState *env)
{
    abi_ulong frame_addr;
    struct target_sigframe *frame;

    if (env->is_bp) {
        /* numas13 FIXME: I am not sure that it is a good solution but
         * the way we handle breakpoints requires these steps.
         * Maybe we need to create more fake kernel frames for breakpoints? */
        e2k_proc_return(env, true);
    }

    /* save current frame */
    e2k_proc_call(env, env->wd.size, env->ip, false);

    frame_addr = get_sigframe(ka, env, sizeof(*frame));
    trace_user_setup_rt_frame(env, frame_addr);
    if (!lock_user_struct(VERIFY_WRITE, frame, frame_addr, 0)) {
        force_sigsegv(sig);
    }
    if (setup_ucontext(&frame->uc, env)) {
        goto fail;
    }
    copy_to_user(frame_addr + offsetof(struct target_sigframe, uc.uc_sigmask),
        set, sizeof(*set));
    copy_to_user(frame_addr + offsetof(struct target_sigframe, aau),
        &env->aau, sizeof(env->aau));
    __put_user(env_lsr_get(env), &frame->lsr);
    __put_user(env->lsr_lcnt, &frame->lsr_lcnt);
    __put_user(env->ilcr, &frame->ilcr);
    __put_user(env->ilcr_lcnt, &frame->ilcr_lcnt);
    copy_to_user(frame_addr + offsetof(struct target_sigframe, gregs),
        &env->regs[E2K_NR_COUNT + 16], 16 * sizeof(E2KReg));
    copy_to_user(frame_addr + offsetof(struct target_sigframe, gtags),
        &env->tags[E2K_NR_COUNT + 16], 16);

    if (ka->sa_flags & TARGET_SA_RESTORER) {
        // TODO: sa_restorer?
        qemu_log_mask(LOG_UNIMP, "target_setup_frame sa_restorer +\n");
    } else {
        // TODO: ignore?
    }

    /* fake kernel frame */
    env->wd.size = 0;
    env->wd.psize = 0;
    env->usd.size = env->sbr - frame_addr;
    env->usd.base = frame_addr;
    e2k_proc_call(env, 0, E2K_SIGRET_ADDR, false);

    env->ip = ka->_sa_handler;
    env->regs[0].lo = sig;
    env->tags[0] = E2K_TAG_NUMBER64;
    env->wd.size = 8;

    if (info && (ka->sa_flags & TARGET_SA_SIGINFO)) {
        tswap_siginfo(&frame->info, info);
        env->regs[1].lo = frame_addr + offsetof(struct target_sigframe, info);
        env->tags[1] = E2K_TAG_NUMBER64;
        env->regs[2].lo = frame_addr + offsetof(struct target_sigframe, uc);
        env->tags[2] = E2K_TAG_NUMBER64;
    }

    unlock_user_struct(frame, frame_addr, 1);
    return;

fail:
    unlock_user_struct(frame, frame_addr, 1);
    force_sigsegv(sig);
}

static abi_long target_restore_sigframe(CPUE2KState *env,
    struct target_sigframe *frame)
{
    target_ulong crs_addr = env->pcsp.base + env->pcsp.index;
    E2KCrs crs, *p;

    if (!lock_user_struct(VERIFY_WRITE, p, crs_addr, 0)) {
        return -TARGET_EFAULT;
    }
    __get_user(crs.cr0_hi, &frame->uc.uc_mcontext.cr0_hi);
    __put_user(crs.cr0_hi, &p->cr0_hi);
    unlock_user_struct(p, crs_addr, 1);

    __get_user(env->ctprs[0].raw, &frame->uc.uc_extra.ctpr1);
    __get_user(env->ctprs[1].raw, &frame->uc.uc_extra.ctpr2);
    __get_user(env->ctprs[2].raw, &frame->uc.uc_extra.ctpr3);

    return 0;
}

void setup_frame(int sig, struct target_sigaction *ka,
    target_sigset_t *set, CPUE2KState *env)
{
    target_setup_frame(sig, ka, 0, set, env);
}

void setup_rt_frame(int sig, struct target_sigaction *ka,
    target_siginfo_t *info, target_sigset_t *set, CPUE2KState *env)
{
    target_setup_frame(sig, ka, info, set, env);
}

long do_sigreturn(CPUE2KState *env)
{
    return do_rt_sigreturn(env);
}

long do_rt_sigreturn(CPUE2KState *env)
{
    abi_ulong frame_addr;
    struct target_sigframe *frame;
    sigset_t set;

    /* restore fake kernel frame */
    e2k_proc_return(env, false);
    frame_addr = env->usd.base;

    trace_user_do_rt_sigreturn(env, frame_addr);
    if (!lock_user_struct(VERIFY_READ, frame, frame_addr, 1)) {
        goto badframe;
    }

    target_to_host_sigset(&set, &frame->uc.uc_sigmask);
    set_sigmask(&set);

    if (target_restore_sigframe(env, frame)) {
        goto badframe;
    }
    copy_from_user(&env->aau, frame_addr
        + offsetof(struct target_sigframe, aau), sizeof(env->aau));
    __get_user(env->lsr, &frame->lsr);
    __get_user(env->lsr_lcnt, &frame->lsr_lcnt);
    __get_user(env->ilcr, &frame->ilcr);
    __get_user(env->ilcr_lcnt, &frame->ilcr_lcnt);
    copy_from_user(&env->regs[E2K_NR_COUNT + 16], frame_addr
        + offsetof(struct target_sigframe, gregs), 16 * sizeof(E2KReg));
    copy_from_user(&env->tags[E2K_NR_COUNT + 16], frame_addr
        + offsetof(struct target_sigframe, gtags), 16);

    if (do_sigaltstack(frame_addr +
            offsetof(struct target_sigframe, uc.uc_stack),
            0, env) == -EFAULT)
    {
        goto badframe;
    }

    /* restore user */
    e2k_proc_return(env, false);

    unlock_user_struct(frame, frame_addr, 0);
    return -TARGET_QEMU_ESIGRETURN;

badframe:
    unlock_user_struct(frame, frame_addr, 0);
    force_sig(TARGET_SIGSEGV);
    return -TARGET_QEMU_ESIGRETURN;
}

abi_long do_swapcontext(CPUArchState *env, abi_ulong uold_ctx,
                        abi_ulong unew_ctx, abi_long ctx_size)
{
    // TODO: do_swapcontext
    qemu_log_mask(LOG_UNIMP, "do_swapcontext: not implemented\n");
    return 0;
}
