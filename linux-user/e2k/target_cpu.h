#ifndef E2K_TARGET_CPU_H
#define E2K_TARGET_CPU_H

#include "qemu/log.h"

static inline void cpu_clone_regs_child(CPUE2KState *env, target_ulong newsp,
                                        unsigned flags)
{
    if (newsp) {
        uint64_t size = 0x20000UL;
        env->usd_hi = size << 32;
        env->usd_lo = (0x1800UL << 48) | newsp;
    }
}

static inline void cpu_clone_regs_parent(CPUE2KState *env, unsigned flags)
{
    qemu_log_mask(LOG_UNIMP, "cpu_clone_regs_parent: unimplemented\n");
    // TODO: cpu_clone_regs_parent
}

static inline void cpu_set_tls(CPUE2KState *env, target_ulong newtls)
{
    qemu_log_mask(LOG_UNIMP, "cpu_set_tls: unimplemented\n");
    // TODO: cpu_set_tls
}

static inline target_ulong cpu_get_tls(CPUE2KState *env)
{
    // TODO: cpu_get_tls
    qemu_log_mask(LOG_UNIMP, "cpu_get_tls: unimplemented\n");
    return 0;
}

static inline abi_ulong get_sp_from_cpustate(CPUE2KState *env)
{
    return extract64(env->usd_lo, 0, 48);
}

#endif /* E2K_TARGET_CPU_H */
