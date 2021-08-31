/*
 *  E2K ELF definitions
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

#ifndef _TARGET_ARCH_ELF_H_
#define _TARGET_ARCH_ELF_H_

#define elf_check_arch(x) ((x) == EM_MCST_ELBRUS || (x) == EM_E2K_OLD)
#define ELF_START_MMAP 0x80000000
#define ELF_ET_DYN_LOAD_ADDR    0x100000
#define ELF_CLASS   ELFCLASS64

#define ELF_DATA    ELFDATA2LSB
#define ELF_ARCH    EM_MCST_ELBRUS

#define ELF_HWCAP   0

#define E2K_ELF_IPD_MASK  ((1U << 1)|(1U << 0))
#define E2K_ELF_X86APP    (1U << 2)
#define E2K_ELF_4MB_PAGES (1U << 3)
#define E2K_ELF_INCOMPAT  (1U << 4)
#define E2K_ELF_PM        (1U << 5)

#endif /* _TARGET_ARCH_ELF_H_ */
