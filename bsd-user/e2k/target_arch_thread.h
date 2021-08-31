/*
 *  E2K thread support
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

#ifndef _TARGET_ARCH_THREAD_H_
#define _TARGET_ARCH_THREAD_H_

static inline void target_thread_set_upcall(CPUE2KState *regs, abi_ulong entry,
    abi_ulong arg, abi_ulong stack_base, abi_ulong stack_size)
{
    assert(0 && "target_thread_set_upcall is not implemented yet");
}

static abi_ulong e2k_mmap(abi_ulong size)
{
    abi_ulong addr;
    abi_ulong guard = TARGET_PAGE_SIZE;

    if (size < TARGET_PAGE_SIZE) {
        size = TARGET_PAGE_SIZE;
    }
    if (guard < qemu_real_host_page_size) {
        guard = qemu_real_host_page_size;
    }

    addr = target_mmap(0, size + guard, PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (addr == -1) {
        perror("mmap e2k stack");
        exit(-1);
    }

    target_mprotect(addr + size, guard, PROT_NONE);
    return addr;
}

// compare to linux-user/elfload.c:init_thread for e2k
static inline void target_thread_init(struct target_pt_regs *regs,
    struct image_info *infop)
{
    abi_ulong start_stack = infop->start_stack & ~0xf;

    regs->ip = infop->entry;

    // FIXME: set real start stack address
    regs->sbr = infop->arg_strings & ~0xf;
    regs->usd_lo = (0x1800UL << 48) | start_stack;
    regs->usd_hi = (regs->sbr - start_stack) << 32;

    e2k_psp_new(&regs->pcsp, E2K_DEFAULT_PCS_SIZE, false);
    e2k_psp_new(&regs->psp, E2K_DEFAULT_PS_SIZE, true);
}

#endif /* !_TARGET_ARCH_THREAD_H_ */
