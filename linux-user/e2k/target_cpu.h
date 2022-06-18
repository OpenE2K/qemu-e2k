#ifndef E2K_TARGET_CPU_H
#define E2K_TARGET_CPU_H

#include "qemu/log.h"

abi_long e2k_copy_from_user_crs(E2KCrs *crs, abi_ulong target_crs_addr);
abi_long e2k_copy_to_user_crs(abi_ulong target_crs_addr, E2KCrs *crs);

static inline void cpu_clone_regs_child(CPUE2KState *env, target_ulong newsp,
                                        unsigned flags)
{
    if (newsp) {
        // FIXME: what size must be?
        env->usd.size = 0x20000;
        env->usd.base = env->sbr = newsp & ~0xf;
        env->usd.read = 1;
        env->usd.write = 1;
    }

    if (flags & CLONE_VM) {
        E2KPsp pcs = { 0 };
        E2KPsp ps = { 0 };
        E2KCrs crs = { 0 };
        uint64_t *ps_old, *ps_new;
        size_t frame_size;
        target_ulong pcsp = env->pcsp.base + env->pcsp.index;
        target_ulong ps_base = env->psp.base + env->psp.index;
        int i;

        e2k_psp_new(&pcs, E2K_DEFAULT_PCS_SIZE, false);
        e2k_psp_new(&ps, E2K_DEFAULT_PS_SIZE, true);

        // TODO: size checks and a way to report errors
        // TODO: set a chain info to return to kernel

        pcs.index += sizeof(E2KCrs);
        if (e2k_copy_from_user_crs(&crs, pcsp)
            || e2k_copy_to_user_crs(pcs.base + pcs.index, &crs))
        {
            qemu_log("qemu-e2k internal error: failed to copy parent frame\n");
            env->ip = 0;
            return;
        }

        frame_size = crs.cr1.wbs * (crs.cr1.wfx || E2K_FORCE_FX ? 32 : 16);
        ps_base -= frame_size;
        ps.index += frame_size;
        ps_old = lock_user(VERIFY_READ, ps_base, frame_size, 1);
        ps_new = lock_user(VERIFY_WRITE, ps.base, frame_size, 0);

        for (i = 0; i < frame_size / 8; i++) {
            uint64_t tmp;
            __get_user(tmp, &ps_old[i]);
            __put_user(tmp, &ps_new[i]);
        }

        unlock_user_struct(ps_new, ps.base, 1);
        unlock_user_struct(ps_old, ps_base, 0);

        env->ip = E2K_SYSRET_ADDR;
        env->pcsp = pcs;
        env->psp = ps;
        env->regs[0].lo = 0;
        env->tags[0] = 0;
    }
}

static inline void cpu_clone_regs_parent(CPUE2KState *env, unsigned flags)
{
}

static inline void cpu_set_tls(CPUE2KState *env, target_ulong newtls)
{
    env->regs[E2K_TLS_REG].lo = newtls;
}

static inline target_ulong cpu_get_tls(CPUE2KState *env)
{
    return env->regs[E2K_TLS_REG].lo;
}

static inline abi_ulong get_sp_from_cpustate(CPUE2KState *env)
{
    return env->usd.base;
}

#endif /* E2K_TARGET_CPU_H */
