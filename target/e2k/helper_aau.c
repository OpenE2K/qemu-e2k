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

void HELPER(aau_load_program)(CPUE2KState *env)
{
    unsigned int i;
    E2KCtpr ctpr = env->ctprs[1];

    if (ctpr.tag != CTPR_TAG_DISP || ctpr.opc != CTPR_OPC_LDISP) {
        helper_raise_exception(env, EXCP_ILLEGAL_OPCODE);
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

target_ulong HELPER(mova_ptr)(CPUE2KState *env, int chan, int area, int ind)
{
    E2KAauPrefState *ps = chan < 2 ? &env->aau.pl : &env->aau.pr;
    E2KAauAreaState *as = &ps->area[area];
    E2KAauPrefInstr instr = as->pi;
    E2KAad aad = env->aau.ds[instr.aad];
    target_ulong ptr = aad.base + as->cdi + instr.disp + ind;
    target_ulong page = ptr & ~(TARGET_PAGE_SIZE - 1);

    if (as->last_page == 0 || page != as->last_page) {
        if (!helper_probe_read_access(env, page)) {
            return 0;
        }
        as->last_page = page;
    }

    return ptr;
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
