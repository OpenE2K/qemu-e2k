#ifndef E2K_TARGET_CPU_H
#define E2K_TARGET_CPU_H

static inline void cpu_clone_regs_child(CPUE2KState *env, target_ulong newsp,
                                        unsigned flags)
{
    // TODO
}

static inline void cpu_clone_regs_parent(CPUE2KState *env, unsigned flags)
{
    // TODO
}

static inline void cpu_set_tls(CPUE2KState *env, target_ulong newtls)
{
    // TODO
}

static inline target_ulong cpu_get_tls(CPUE2KState *env)
{
    // TODO
    return 0;
}

static inline abi_ulong get_sp_from_cpustate(CPUE2KState *state)
{
    // TODO
    return 0;
}

#endif /* E2K_TARGET_CPU_H */
