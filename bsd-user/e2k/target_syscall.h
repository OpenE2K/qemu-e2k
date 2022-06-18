/*
 *  E2K system call definitions
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

#ifndef __ARCH_SYSCALL_H_
#define __ARCH_SYSCALL_H_

#define E2K_DEFAULT_PCS_SIZE (TARGET_PAGE_SIZE)
#define E2K_DEFAULT_PS_SIZE (TARGET_PAGE_SIZE * 4)

struct target_pt_regs {
    /* special registers */
    uint64_t wd; // Current window descriptor (WD)

    uint64_t sbr; // User Stack Base Register (USBR/SBR)
                  // SBR - contains the base (top) virtual address of the current User Stack area.

    // uint64_t tr; // current type register

    E2KPsp pcsp;
    E2KPsp psp;

    uint32_t psr; // Processor State Register (PSR)
    uint32_t upsr; // User processor status register (UPSR)
    uint64_t ip; // instruction pointer
    uint64_t nip; // next instruction pointer

    uint64_t ctpr1; // Control Transfer Preparation Register (CTPR)
    uint64_t ctpr2;
    uint64_t ctpr3;

    uint32_t pfpfr; // Packed Floating Point Flag Register (PFPFR)
    uint32_t fpcr; // Floating point control register (FPCR)
    uint32_t fpsr; // Floating point state register (FPSR)

    // borrowed from Embox OS
    uint64_t lsr; // Loop status register (LSR)
    uint64_t ilcr; // Loop counter register (ILCR)

    uint64_t dr0;

    uint64_t cr0_hi;
    uint64_t cr1_lo;
    uint64_t cr1_hi;
    uint64_t pcsp_hi;
    uint64_t pcsp_lo;
    uint64_t usd_lo; // User data
    uint64_t usd_hi;

    uint64_t gbase[32];
    uint16_t gext[32];
};

#define UNAME_MACHINE "e2k"

#define TARGET_HW_MACHINE       "e2k"
#define TARGET_HW_MACHINE_ARCH  UNAME_MACHINE

#endif /* !__ARCH_SYSCALL_H_ */
