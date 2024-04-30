/*
 * safe-syscall.inc.S : host-specific assembly fragment
 * to handle signals occurring at the same time as system calls.
 * This is intended to be included by common-user/safe-syscall.S
 *
 * Copyright (C) 2024 Denis Drakhnia <numas13@gmail.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 2 or later.
 * See the COPYING file in the top-level directory.
 */

    .global safe_syscall_base
    .global safe_syscall_start
    .global safe_syscall_end
    .type   safe_syscall_base, @function
    .type   safe_syscall_start, @function
    .type   safe_syscall_end, @function

    /*
     * This is the entry point for making a system call. The calling
     * convention here is that of a C varargs function with the
     * first argument an 'int *' to the signal_pending flag, the
     * second one the system call number (as a 'long'), and all further
     * arguments being syscall arguments (also 'long').
     */
safe_syscall_base:
    .cfi_startproc
    /*
     * The syscall calling convention is the same as C:
     * we enter with r0 == &signal_pending
     *               r1 == syscall number
     *               r2 ... r8 == syscall arguments
     *               and return the result in b0
     * and the syscall instruction needs
     *               b0 == syscall number
     *               b1 ... b7 == syscall arguments
     *               and returns the result in b0
     * Shuffle everything around appropriately.
     */
    {
        setwd wsz=8, nfx=1
        setbn rsz=3, rbs=4, rcur=0
        sdisp %ctpr1, 0x3
    }
    {
        disp %ctpr2, safe_syscall_set_errno_tail

        addd,0,sm %r1, 0, %b[0]     /* syscall number */
        addd,1,sm %r2, 0, %b[1]     /* syscall arguments */
        addd,2,sm %r3, 0, %b[2]
        addd,3,sm %r4, 0, %b[3]
        addd,4,sm %r5, 0, %b[4]
        addd,5,sm %r6, 0, %b[5]
    }

    /*
     * This next sequence of code works in conjunction with the
     * rewind_if_safe_syscall_function(). If a signal is taken
     * and the interrupted PC is anywhere between 'safe_syscall_start'
     * and 'safe_syscall_end' then we rewind it to 'safe_syscall_start'.
     * The code sequence must therefore be able to cope with this, and
     * the syscall instruction must be the final one in the sequence.
     */
safe_syscall_start:
    {
        ldw,0 %r0, 0, %g16
        addd,1,sm %r7, 0, %b[6]
        addd,2,sm %r8, 0, %b[7]
#if __iset__ == 6
        nop 4
#else
        nop 2
#endif
    }
    {
        /* If signal_pending is non-zero, don't do the call */
        cmpedb,0 %g16, 0, %pred0
#if __iset__ < 6
        nop 1
#endif
    }
    {
        call %ctpr1, wbs=4 ? %pred0
    }
safe_syscall_end:
    {
        return %ctpr3
        addd,0 %b[0], 0, %r0 ? %pred0
        cmpbedb,1 %b[0], -4096, %pred1

        /* code path when we didn't execute the syscall */
        addd,2 0, QEMU_ERESTARTSYS, %r0 ? ~%pred0
        ct %ctpr2 ? ~%pred0
    }
    {
        disp %ctpr1, safe_syscall_set_errno_tail
        nop 4
    }
    {
        subd,0 0, %r0, %r0 ? ~%pred1

        /* code path for having successfully executed the syscall */
        ct %ctpr3 ? %pred1
    }
    {
        /* code path setting errno */
        ct %ctpr1
    }

    .cfi_endproc
    .size   safe_syscall_base, .-safe_syscall_base
