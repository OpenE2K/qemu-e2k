#include "qemu/osdep.h"
#include "qemu/log.h"
#include "cpu.h"
#include "exec/exec-all.h"
#include "qemu/host-utils.h"
#include "exec/helper-proto.h"

static inline void init_prefetch_area(E2KAauAreaState *s, E2KAauPrefInstr pi,
    uint32_t *inds)
{
    s->last_page = 0;
    s->last_page_valid = false;
    if (pi.fmt != 0) {
        s->pi = pi;
        s->ldi = 0;
        s->cdi = inds[pi.ind];
    } else {
        s->pi.raw = 0;
        s->ldi = 0;
        s->cdi = 0;
    }
}

void HELPER(aau_load_program)(CPUE2KState *env, uint64_t ctpr2)
{
    unsigned int i;
    E2KCtpr ctpr = { .raw = ctpr2 };

    if (ctpr.tag != CTPR_TAG_DISP || ctpr.opc != CTPR_OPC_LDISP) {
        helper_raise_exception(env, E2K_EXCP_ILLEGAL_OPCODE);
    }

    for (i = 0; i < 32; i++) {
        E2KAauPrefInstr l, r;
        size_t offset = i * 16;

        l.raw = cpu_ldq_le_data(env, ctpr.base + offset);
        r.raw = cpu_ldq_le_data(env, ctpr.base + offset + 8);

        init_prefetch_area(&env->aau.pl.area[i], l, env->aau.inds);
        init_prefetch_area(&env->aau.pr.area[i], r, env->aau.inds);

        if (l.ct || r.ct) {
            break;
        }
    }
}

target_ulong HELPER(mova_ptr)(CPUE2KState *env, int chan, int area, int ind,
    int size, int mmu_idx)
{
    E2KAauPrefState *ps = chan < 2 ? &env->aau.pl : &env->aau.pr;
    E2KAauAreaState *as = &ps->area[area];
    E2KAauPrefInstr instr = as->pi;
    E2KAad aad = env->aau.ds[instr.aad];
    target_ulong addr = aad.base + as->cdi + instr.disp + ind;
    target_ulong page = addr & TARGET_PAGE_MASK;

    if ((1 << instr.fmt) <= size) {
        helper_raise_exception(env, E2K_EXCP_ILLEGAL_OPCODE);
    }

    if ((env->version <= 4 || (env->version == 5 && size == 16)) && addr & (size - 1)) {
        return 0;
    } else if (page != as->last_page) {
        void *ignore;
        int flags;

        flags = probe_access_flags(env, page, 0, MMU_DATA_LOAD, mmu_idx,
            true, &ignore, 0);
        as->last_page = page;
        as->last_page_valid = !(flags & TLB_INVALID_MASK);
    }

    return as->last_page_valid ? addr : 0;
}

void HELPER(aau_am)(CPUE2KState *env, int chan, int area)
{
    E2KAauPrefState *ps = chan < 2 ? &env->aau.pl : &env->aau.pr;
    E2KAauAreaState *as = &ps->area[area];
    E2KAauPrefInstr instr = as->pi;
    uint32_t incr = env->aau.incrs[instr.incr];
    int size;

    if (instr.fmt == 0) {
        return;
    }

    size = 1 << (instr.fmt - 1);
    as->cdi += size * incr;
}

void HELPER(aaurwd_aad)(CPUE2KState *env, uint32_t aad, uint64_t val, uint32_t tag)
{
    env->aau.ds[aad].base = val;
    if (env->enable_tags) {
        // TODO: aad.tag
        env->aau.ds[aad].tag = 0;
    }
    env->aau.ds[aad].mb = 1;
    env->aau.ds[aad].ed = 1;
    env->aau.ds[aad].rw = 3;
}

void HELPER(aaurwq_aad)(CPUE2KState *env, uint32_t aad, Int128 val, uint32_t tag)
{
    helper_aaurwd_aad(env, aad, int128_getlo(val), tag);
    env->aau.ds[aad].hi = int128_gethi(val) & 0xffffffff00000000;
}

void HELPER(aaurwd_aaind)(CPUE2KState *env, uint32_t index, uint32_t val, uint32_t tag)
{
    env->aau.inds[index] = val;

    if (env->enable_tags) {
        env->aau.ind_tags = deposit64(env->aau.ind_tags, index, 1, tag != 0);
    }
}

void HELPER(aaurwd_aasti)(CPUE2KState *env, uint32_t index, uint32_t val, uint32_t tag)
{
    env->aau.stis[index] = val;

    if (env->enable_tags) {
        env->aau.sti_tags = deposit64(env->aau.sti_tags, index, 1, tag != 0);
    }
}

void HELPER(aaurwd_aaincr)(CPUE2KState *env, uint32_t index, uint32_t val, uint32_t tag)
{
    env->aau.incrs[index] = val;

    if (env->enable_tags) {
        env->aau.incr_tags = deposit64(env->aau.incr_tags, index, 1, tag != 0);
    }
}

uint64_t HELPER(aad_ptr)(CPUE2KState *env, uint32_t aad, uint32_t aasti)
{
    return extract64(env->aau.ds[aad].lo, 0, 48) + env->aau.stis[aasti];
}

void HELPER(aasti_incr)(CPUE2KState *env, uint32_t aasti, uint32_t aaincr, uint32_t len)
{
    env->aau.stis[aasti] += env->aau.incrs[aaincr] << len;
}
