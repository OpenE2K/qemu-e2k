#include "qemu/osdep.h"
#include "qemu.h"
#include "exec/log.h"
#include "disas/disas.h"
#include "translate.h"

struct CPUE2KStateTCG e2k_cs;

/* returns zero if bundle is invalid */
static target_ulong unpack_bundle(CPUE2KState *env,
    target_ulong pc, UnpackedBundle *bundle)
{
    unsigned int gap;
    unsigned int pos = 0;
    unsigned int mdl;
    unsigned int hsyll_cntr = 0;
    unsigned int i;
    uint32_t hs;

    memset(bundle, 0, sizeof(UnpackedBundle));

    bundle->hs = hs = translator_ldl(env, pc + pos);
    pos += 4;

    /* Check for SS.  */
    if (GET_BIT(hs, 12)) {
        bundle->ss_present = true;
        bundle->ss = translator_ldl(env, pc + pos);
        pos += 4;
    }

    /* Check for available ALS syllables.  */
    for (i = 0; i < 6; i++) {
        if (GET_BIT(hs, 26 + i)) {
            bundle->als_present[i] = true;
            bundle->als[i] = translator_ldl(env, pc + pos);
            pos += 4;
        }
    }

    /* Check for CS0.  */
    if (GET_BIT(hs, 14)) {
        bundle->cs0_present = true;
        bundle->cs0 = translator_ldl(env, pc + pos);
        pos += 4;
    }

    if (GET_BIT(hs, 25)) {
        bundle->ales_present[5] = ALES_ALLOCATED;
        bundle->ales[5] = 0x01c0;
    }

    if (GET_BIT(hs, 22)) {
        bundle->ales_present[2] = ALES_ALLOCATED;
        bundle->ales[2] = 0x01c0;
    }

    /* Calculate the size of f1 fragment in bytes. For a valid bundle it
       should be equal to either of `pos', `pos + 4' or `pos + 8'. What should I
       do if it's not?  */
    /* TODO: exception */
    mdl = ((hs & 0xf) + 1) * 4;

    /* The following condition means that ALES{2,5} are physically present within
       the wide instruction. However, they should be probably taken into account
       only if HS.ale{2,5} are set. Should I disassemble them if these bits are
       not set but the syllables physically exist?  */
    if ((GET_BIT(hs, 15) && mdl == pos + 8) ||
        (!GET_BIT(hs, 15) && mdl == pos + 4)) {
        /* Fill in ALES5 and ALES2 syllables even if none of them is specified in
           HS as present. This will let me output this syllable into disassembly
           whichever case takes place. */
        bundle->ales[5] = translator_lduw(env, pc + pos);
        bundle->ales[2] = translator_lduw(env, pc + pos + 2);

        /* Adjust `ALES_PRESENT[{5,2}]' as proposed above now that we know that
           they are allocated.  */
        bundle->ales_present[5] |= ALES_PRESENT;
        bundle->ales_present[2] |= ALES_PRESENT;

        pos += 4;
    }

    /* Check for CS1.  */
    if (GET_BIT(hs, 15)) {
        bundle->cs1_present = 1;
        bundle->cs1 = translator_ldl(env, pc + pos);
        pos += 4;
    }

    /* A primitive control just for a moment.  */
    if (mdl != pos) {
        /* This is either an APB instruction or an invalid one. Let's stupidly
           believe that the former takes place and signalize our caller about
           that by returning 0.  */
        return 0;
    }

    /* Check for ALES{0,1,3,4}.  */
    for (i = 0; i < 5; i++) {
        if (i == 2)
            continue;

        if (GET_BIT(hs, 20 + i)) {
            unsigned int offset = 2 * ((hsyll_cntr & ~0x1) + 1 - (hsyll_cntr & 0x1));
            bundle->ales_present[i] = ALES_PRESENT;

            /* Recall the idiotic order of half-syllables in the packed wide
               instruction.  */
            bundle->ales[i] = translator_lduw(env, pc + pos + offset);
            hsyll_cntr++;
        }
    }

    /* Check for AASj half-syllables. To encode them SS syllable of SF1 type
       should be present.  */
    if (bundle->ss_present && !GET_BIT(bundle->ss, 20)) {
        for (i = 0; i < 4; i++) {
            if (GET_BIT(bundle->ss, 12 + i)) {
                bundle->aas_present[i >> 1] = true;
                bundle->aas_present[i + 2] = true;
            }
        }

        for (i = 0; i < 6; i++) {
            if (bundle->aas_present[i]) {
                unsigned int offset = 2 * ((hsyll_cntr & ~0x1) + 1 - (hsyll_cntr & 0x1));
                /* Recall the idiotic order of half-syllables in the packed wide
                   instruction. Note that the first AAS half-syllable may share a
                   syllable with the last ALES.  */
                bundle->aas[i] = translator_lduw(env, pc + pos + offset);
                hsyll_cntr++;
            }
        }
    }

    /* align half-syllables */
    hsyll_cntr += hsyll_cntr & 1;

    /* Calculate the next 32-bit syllable's position. It may be the uppermost LTS
       syllable. Note that I don't consider the case when LTS syllables reuse the
       values encoded in the preceding ones, though according to `iset-v5.single'
       this is quite legal. GAS doesn't produce such a code. Hopefully neither LAS
       has ever done that . . .  */
    gap = pos + 2 * hsyll_cntr;

    /* Set POS to point to the last syllable in the current wide instruction and
       extract CDSj and PLSj syllables if any.  */
    pos = ((GET_FIELD(hs, 4, 6) + 1) << 3) - 4;

    /* Check for CDSj syllables.  */
    for (i = 0; i < GET_FIELD(hs, 16, 17); i++) {
        bundle->cds_present[i] = true;
        bundle->cds[i] = translator_ldl(env, pc + pos);
        pos -= 4;
    }

    /* Check for PLSj syllables.  */
    for (i = 0; i < GET_FIELD(hs, 18, 19); i++) {
        bundle->pls_present[i] = true;
        bundle->pls[i] = translator_ldl(env, pc + pos);
        pos -= 4;
    }

    /* Now POS should point to the lowermost LTS0 syllable if any. If there are
       no LTSj syllables in this instruction, POS should point to the last
       syllable consisting of half-syllables.

       If neither of these conditions holds true, believe that it's not a valid
       synchronous instruction by analogy with the middle point test above.
       Engineers are said to customize instructions with references to missing
       literal syllables occasionally, but the lack of space for more substantial
       syllables should not be allowed for.  */
    if (pos < gap && pos != gap - 4) {
        return 0;
    }

    /* Extract available LTSj syllables.  */
    for (i = 0; i < 4 && pos >= gap; i++) {
        bundle->lts_present[i] = true;
        bundle->lts[i] = translator_ldl(env, pc + pos);
        pos -= 4;
    }

    return 8 + GET_FIELD(hs, 4, 6) * 8;
}

static inline void save_state(DisasContext *dc)
{
    tcg_gen_movi_tl(e2k_cs.pc, dc->pc);
//    tcg_gen_movi_tl(e2k_cs.pc, dc->npc);
}

void e2k_gen_exception(DisasContext *dc, int which)
{
    TCGv_i32 t = tcg_const_i32(which);

    save_state(dc);
    gen_helper_raise_exception(cpu_env, t);
    tcg_temp_free_i32(t);
    dc->base.is_jmp = DISAS_NORETURN;
}

static void e2k_tr_init_disas_context(DisasContextBase *db, CPUState *cs)
{
    DisasContext *dc = container_of(db, DisasContext, base);
    E2KCPU *cpu = E2K_CPU(cs);
    CPUE2KState *env = &cpu->env;

    dc->pc = dc->base.pc_first;
    dc->version = env->version;
}

static void e2k_tr_insn_start(DisasContextBase *db, CPUState *cs)
{
    DisasContext *dc = container_of(db, DisasContext, base);
    tcg_gen_insn_start(dc->pc);
}

static bool e2k_tr_breakpoint_check(DisasContextBase *db, CPUState *cs,
                                    const CPUBreakpoint *bp)
{
    // TODO
    qemu_log_mask(LOG_UNIMP, "e2k_tr_breakpoint_check: not implemented\n");
    return false;
}

static void e2k_tr_translate_insn(DisasContextBase *db, CPUState *cs)
{
    DisasContext *dc = container_of(db, DisasContext, base);
    E2KCPU *cpu = E2K_CPU(cs);
    CPUE2KState *env = &cpu->env;
    UnpackedBundle *bundle = &dc->bundle;
    dc->pc = dc->base.pc_next;
    unsigned int bundle_len = unpack_bundle(env, dc->pc, bundle);
    /* TODO: exception, check bundle_len */
    dc->base.pc_next = dc->npc = dc->pc + bundle_len;

    e2k_alc_gen(dc);
    e2k_control_gen(dc);

    e2k_alc_commit(dc);
    e2k_win_commit(dc);

    /* Free temporary values */
    while(dc->t32_len) {
        tcg_temp_free_i32(dc->t32[--dc->t32_len]);
    }

    while(dc->t64_len) {
        tcg_temp_free_i64(dc->t64[--dc->t64_len]);
    }

    while(dc->ttl_len) {
        tcg_temp_free(dc->ttl[--dc->ttl_len]);
    }
}

static void e2k_tr_tb_start(DisasContextBase *db, CPUState *cs)
{
    DisasContext *dc = container_of(db, DisasContext, base);

    dc->is_call = false;
    dc->jmp.dest = tcg_const_i64(0);
    dc->jmp.cond = tcg_const_i64(0);
}

static void e2k_tr_tb_stop(DisasContextBase *db, CPUState *cs)
{
    DisasContext *dc = container_of(db, DisasContext, base);

    /* Control transfer */
    switch(dc->base.is_jmp) {
    case DISAS_NEXT:
    case DISAS_TOO_MANY:
        break;
    case DISAS_CALL: {
        tcg_gen_movi_tl(e2k_cs.pc, dc->pc);
        tcg_gen_movi_tl(e2k_cs.pc, dc->npc);
        gen_helper_call(cpu_env, e2k_cs.ctprs[dc->call_ctpr], dc->jmp.cond);
        tcg_gen_exit_tb(NULL, 0);
        break;
    }
    case DISAS_NORETURN: {
        /* exception */
        tcg_gen_exit_tb(NULL, 0);
        break;
    }
    case STATIC_JUMP:
        tcg_gen_mov_tl(e2k_cs.pc, dc->jmp.dest);
        tcg_gen_exit_tb(NULL, 0);
        break;
    case DYNAMIC_JUMP: {
        TCGv_i64 one = tcg_const_tl(1);
        TCGv_i64 npc = tcg_const_tl(dc->npc);
        tcg_gen_movcond_tl(TCG_COND_EQ, e2k_cs.pc,
            dc->jmp.cond, one,
            dc->jmp.dest, npc
        );
        tcg_temp_free(npc);
        tcg_temp_free(one);
        tcg_gen_exit_tb(NULL, 0);
        break;
    }
    default:
        g_assert_not_reached();
        break;
    }

    tcg_temp_free(dc->jmp.dest);
    tcg_temp_free(dc->jmp.cond);
}

static void e2k_tr_disas_log(const DisasContextBase *db, CPUState *cpu)
{
    DisasContext *dc = container_of(db, DisasContext, base);

    qemu_log("IN: %s\n", lookup_symbol(dc->base.pc_first));
    log_target_disas(cpu, dc->base.pc_first, dc->base.tb->size);
}

static const TranslatorOps e2k_tr_ops = {
    .init_disas_context = e2k_tr_init_disas_context,
    .tb_start           = e2k_tr_tb_start,
    .insn_start         = e2k_tr_insn_start,
    .breakpoint_check   = e2k_tr_breakpoint_check,
    .translate_insn     = e2k_tr_translate_insn,
    .tb_stop            = e2k_tr_tb_stop,
    .disas_log          = e2k_tr_disas_log,
};

void gen_intermediate_code(CPUState *cs, TranslationBlock *tb, int max_insns)
{
    DisasContext dc = { 0 };

    translator_loop(&e2k_tr_ops, &dc.base, cs, tb, max_insns);
}

void restore_state_to_opc(CPUE2KState *env, TranslationBlock *tb,
                          target_ulong *data)
{
    // TODO
    qemu_log_mask(LOG_UNIMP, "restore_state_to_opc: not implemented\n");
}

void e2k_tcg_initialize(void) {
    char buf[8] = { 0 };

    static const struct { TCGv_i32 *ptr; int off; const char *name; } r32[] = {
        { &e2k_cs.wbs, offsetof(CPUE2KState, wbs), "wbs" },
        { &e2k_cs.wsz, offsetof(CPUE2KState, wsz), "wsz" },
        { &e2k_cs.nfx, offsetof(CPUE2KState, nfx), "nfx" },
        { &e2k_cs.dbl, offsetof(CPUE2KState, dbl), "dbl" },
        { &e2k_cs.br, offsetof(CPUE2KState, br), "br" },
        { &e2k_cs.syscall_wbs, offsetof(CPUE2KState, syscall_wbs), "syscall_wbs" },
    };

    static const struct { TCGv_i64 *ptr; int off; const char *name; } r64[] = {
        { &e2k_cs.pregs, offsetof(CPUE2KState, pregs), "pregs" },
        { &e2k_cs.usd_lo, offsetof(CPUE2KState, usd_lo), "usd.lo" },
        { &e2k_cs.usd_hi, offsetof(CPUE2KState, usd_hi), "usd.hi" },
        { &e2k_cs.lsr, offsetof(CPUE2KState, lsr), "lsr" },
    };

    static const struct { TCGv *ptr; int off; const char *name; } rtl[] = {
        { &e2k_cs.pc, offsetof(CPUE2KState, ip), "pc" },
    };

    unsigned int i;

    for (i = 0; i < ARRAY_SIZE(r32); i++) {
        *r32[i].ptr = tcg_global_mem_new_i32(cpu_env, r32[i].off, r32[i].name);
    }

    for (i = 0; i < ARRAY_SIZE(r64); i++) {
        *r64[i].ptr = tcg_global_mem_new_i64(cpu_env, r64[i].off, r64[i].name);
    }

    for (i = 0; i < ARRAY_SIZE(rtl); i++) {
        *rtl[i].ptr = tcg_global_mem_new(cpu_env, rtl[i].off, rtl[i].name);
    }

    e2k_cs.win_ptr = tcg_global_mem_new_ptr(cpu_env, offsetof(CPUE2KState, win_ptr), "win_ptr");

    for (i = 0; i < WREGS_SIZE; i++) {
        snprintf(buf, ARRAY_SIZE(buf), "%%r%d", i);
        e2k_cs.wregs[i] = tcg_global_mem_new(e2k_cs.win_ptr,
            i * REG_SIZE,
            buf);
    }

    for (i = 0; i < 32; i++) {
        snprintf(buf, ARRAY_SIZE(buf), "%%g%d", i);
        e2k_cs.gregs[i] = tcg_global_mem_new(cpu_env,
            offsetof(CPUE2KState, gregs[i]),
            buf);
    }

    for (i = 0; i < 3; i++) {
        snprintf(buf, ARRAY_SIZE(buf), "%%ctpr%d", i + 1);
        e2k_cs.ctprs[i] = tcg_global_mem_new(cpu_env,
            offsetof(CPUE2KState, ctprs[i]),
            buf
        );
    }
}