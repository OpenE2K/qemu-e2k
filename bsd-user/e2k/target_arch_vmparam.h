/*
 *  E2K VM parameters definitions
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

#ifndef _TARGET_ARCH_VMPARAM_H_
#define _TARGET_ARCH_VMPARAM_H_

#include "cpu.h"

// TODO: find out proper values for these constants
#define TARGET_MAXTSIZ  (128UL*1024*1024)   /* max text size */
#define TARGET_DFLDSIZ  (32768UL*1024*1024) /* initial data size limit */
#define TARGET_MAXDSIZ  (32768UL*1024*1024) /* max data size */
#define TARGET_DFLSSIZ  (8UL*1024*1024)     /* initial stack size limit */
#define TARGET_MAXSSIZ  (512UL*1024*1024)   /* max stack size */
#define TARGET_SGROWSIZ (128UL*1024)        /* amount to grow stack */

#define TARGET_VM_MAXUSER_ADDRESS   (0x00007fffff000000UL)

#define TARGET_USRSTACK (TARGET_VM_MAXUSER_ADDRESS - TARGET_PAGE_SIZE)

// see linux-user/e2k/target_cpu.h:get_sp_from_cpustate
static inline abi_ulong get_sp_from_cpustate(CPUE2KState *state)
{
    return state->usd.base;
}

static inline void set_second_rval(CPUE2KState *state, abi_ulong retval2)
{
    assert(0 && "set_second_rval not implemented yet");
}

#endif /* !_TARGET_ARCH_VMPARAM_H_ */
