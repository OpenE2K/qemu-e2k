#include "qemu/osdep.h"
#include "qemu/log.h"
#include "cpu.h"
#include "exec/exec-all.h"
#include "qemu/host-utils.h"
#include "exec/helper-proto.h"

void HELPER(set_aad_i64)(CPUE2KState *env, uint64_t src4, int incr, int d)
{
    assert(d < 32);

    // FIXME: What register default values should be?
    env->aau.ds[d].lo = 0;
    env->aau.ds[d].hi = 0;

    env->aau.ds[d].base = extract64(src4, 0, 48);
    env->aau.ds[d].rw = 3;
    env->aau.incrs[incr] = 1;
}

void HELPER(set_aad_i32)(CPUE2KState *env, uint32_t src4, int incr, int d)
{
    assert(d < 32);

    // FIXME: What register default values should be?
    env->aau.ds[d].lo = 0;
    env->aau.ds[d].hi = 0;

    env->aau.ds[d].base = src4;
    env->aau.ds[d].rw = 3;
    env->aau.incrs[incr] = 1;
}

void HELPER(set_aasti_i64)(CPUE2KState *env, uint64_t src4, int ind)
{
    assert(ind < 16);
    env->aau.stis[ind] = src4;
}

void HELPER(set_aasti_i32)(CPUE2KState *env, uint32_t src4, int ind)
{
    assert(ind < 16);
    env->aau.stis[ind] = src4;
}

void HELPER(staa_i64)(CPUE2KState *env, uint64_t src4, uint32_t lit, int am,
    int incr, int ind, int d)
{
    abi_ulong ptr = env->aau.ds[d].base + env->aau.stis[ind] + lit;
    cpu_stq_le_data_ra(env, ptr, src4, GETPC());
    if (am) {
        env->aau.stis[ind] += env->aau.incrs[incr] * 8;
    }
}

void HELPER(staa_i32)(CPUE2KState *env, uint32_t src4, uint32_t lit, int am,
    int incr, int ind, int d)
{
    abi_ulong ptr = env->aau.ds[d].base + env->aau.stis[ind] + lit;
    cpu_stl_le_data_ra(env, ptr, src4, GETPC());
    if (am) {
        env->aau.stis[ind] += env->aau.incrs[incr] * 4;
    }
}
