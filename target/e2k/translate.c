#include "qemu/osdep.h"
#include "qemu.h"
#include "exec/log.h"
#include "disas/disas.h"
#include "translate.h"

struct CPUE2KStateTCG e2k_cs;

/* returns zero if bundle is invalid */
static size_t unpack_bundle(CPUE2KState *env,
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

static inline void gen_save_pc(target_ulong pc)
{
    tcg_gen_movi_tl(e2k_cs.pc, pc);
}

static inline void gen_save_npc(target_ulong npc)
{
    tcg_gen_movi_tl(e2k_cs.npc, npc);
}

static inline void gen_save_cpu_state(DisasContext *ctx)
{
    gen_save_pc(ctx->pc);
    gen_save_npc(ctx->npc);
    gen_helper_save_cpu_state(cpu_env);
}

static inline bool use_goto_tb(DisasContext *s, target_ulong pc,
                               target_ulong npc)
{
    if (unlikely(s->base.singlestep_enabled || singlestep)) {
        return false;
    }

#ifndef CONFIG_USER_ONLY
    return (pc & TARGET_PAGE_MASK) == (s->base.tb->pc & TARGET_PAGE_MASK) &&
           (npc & TARGET_PAGE_MASK) == (s->base.tb->pc & TARGET_PAGE_MASK);
#else
    return true;
#endif
}

static inline void gen_goto_tb(DisasContext *ctx, int tb_num,
    target_ulong pc, target_ulong npc)
{
    if (use_goto_tb(ctx, pc, npc))  {
        /* jump to same page: we can use a direct jump */
        tcg_gen_goto_tb(tb_num);
        tcg_gen_movi_tl(e2k_cs.pc, pc);
        tcg_gen_movi_tl(e2k_cs.npc, npc);
        tcg_gen_exit_tb(ctx->base.tb, tb_num);
    } else {
        /* jump to another page: currently not optimized */
        tcg_gen_movi_tl(e2k_cs.pc, pc);
        tcg_gen_movi_tl(e2k_cs.npc, npc);
        tcg_gen_exit_tb(NULL, 0);
    }
}

void e2k_gen_exception(DisasContext *ctx, int which)
{
    TCGv_i32 t = tcg_const_i32(which);

    gen_save_cpu_state(ctx);
    gen_helper_raise_exception(cpu_env, t);
    tcg_gen_exit_tb(NULL, 0);
    ctx->base.is_jmp = DISAS_NORETURN;

    tcg_temp_free_i32(t);
}

static inline void gen_ctpr_tag(TCGv_i64 ret, TCGv_i64 ctpr)
{
    tcg_gen_extract_i64(ret, ctpr, CTPR_TAG_OFF, CTPR_TAG_LEN);
}

static inline void gen_goto_ctpr_disp(TCGv_i64 ctpr)
{
    TCGv_i64 t0 = tcg_temp_new_i64();
    TCGv t1 = tcg_temp_new();

    tcg_gen_extract_i64(t0, ctpr, CTPR_BASE_OFF, CTPR_BASE_LEN);
    tcg_gen_trunc_i64_tl(t1, t0);
    // FIXME: save state here?
    tcg_gen_mov_tl(e2k_cs.pc, t1);
    tcg_gen_lookup_and_goto_ptr();

    tcg_temp_free(t1);
    tcg_temp_free_i64(t0);
}

static inline void do_decode(DisasContext *ctx, CPUState *cs)
{
    E2KCPU *cpu = E2K_CPU(cs);
    CPUE2KState *env = &cpu->env;
    UnpackedBundle *bundle = &ctx->bundle;
    unsigned int bundle_len;

    ctx->pc = ctx->base.pc_next;
    bundle_len = unpack_bundle(env, ctx->pc, bundle);
    /* TODO: exception, check bundle_len */
    ctx->npc = ctx->base.pc_next = ctx->pc + bundle_len;
}

/*
 * Executes instructions from a bundle and store the results to
 * temporary buffer.
 */
static inline void do_execute(DisasContext *ctx)
{
    unsigned int i;

    for (i = 0; i < 6; i++) {
        if (ctx->bundle.als_present[i]) {
            ctx->alc[i].tag = RESULT_NONE;
            e2k_execute_alc(ctx, i);
        }
    }

    e2k_control_gen(ctx);
}

/*
 * Writes results of instructions from a bundle to the state
 *
 * Note
 * ====
 *
 * Must not generate any exception.
 * */
static inline void do_commit(DisasContext *ctx)
{
    unsigned int i;

    for (i = 0; i < 6; i++) {
        Result *res = &ctx->alc[i];
        if (!ctx->bundle.als_present[i]) {
            continue;
        }
        switch(res->tag) {
        case RESULT_BASED_REG:
            e2k_gen_store_breg(res->u.reg.i, res->u.reg.v);
            break;
        case RESULT_REGULAR_REG:
            e2k_gen_store_wreg(res->u.reg.i, res->u.reg.v);
            break;
        case RESULT_GLOBAL_REG:
            e2k_gen_store_greg(res->u.reg.i, res->u.reg.v);
            break;
        case RESULT_PREG:
            e2k_gen_store_preg(res->u.reg.i, res->u.reg.v);
            break;
        default:
            break;
        }
    }

    e2k_commit_stubs(ctx);
}

static inline void do_branch(DisasContext *ctx)
{
    TCGLabel *l0;

    if (ctx->ct.type == CT_NONE) {
        return;
    }

    ctx->base.is_jmp = DISAS_NORETURN;

    l0 = gen_new_label();
    tcg_gen_brcondi_tl(TCG_COND_NE, e2k_cs.ct_cond, 0, l0);
    tcg_gen_movi_tl(e2k_cs.pc, ctx->npc);
    tcg_gen_exit_tb(NULL, 0);
    gen_set_label(l0);

    switch(ctx->ct.type) {
    case CT_IBRANCH:
        // TODO: goto_tb
        gen_goto_tb(ctx, 0, ctx->pc, ctx->ct.u.target);
        break;
    case CT_JUMP: {
        TCGLabel *l0 = gen_new_label();
        TCGLabel *l1 = gen_new_label();
        TCGv_i64 t0 = tcg_temp_new_i64();

        gen_ctpr_tag(t0, ctx->ct.u.ctpr);
        tcg_gen_brcondi_i64(TCG_COND_EQ, t0, CTPR_TAG_DISP, l0);
        gen_ctpr_tag(t0, ctx->ct.u.ctpr);
        tcg_gen_brcondi_i64(TCG_COND_EQ, t0, CTPR_TAG_RETURN, l1);
        // TODO: ldisp, sdisp
        e2k_gen_exception(ctx, E2K_EXCP_UNIMPL);

        gen_set_label(l0);
        gen_goto_ctpr_disp(ctx->ct.u.ctpr);

        gen_set_label(l1);
        gen_save_cpu_state(ctx);
        gen_helper_return(e2k_cs.pc, cpu_env);
        tcg_gen_lookup_and_goto_ptr();

        tcg_temp_free_i64(t0);
        break;
    }
    case CT_CALL: {
        TCGv_i32 wbs = tcg_const_i32(ctx->ct.wbs);
        gen_save_cpu_state(ctx);
        gen_helper_call(e2k_cs.pc, cpu_env, ctx->ct.u.ctpr, wbs);
        tcg_temp_free_i32(wbs);
        tcg_gen_lookup_and_goto_ptr();
        break;
    }
    default:
        break;
    }
}

static void e2k_tr_init_disas_context(DisasContextBase *db, CPUState *cs)
{
    DisasContext *dc = container_of(db, DisasContext, base);
    E2KCPU *cpu = E2K_CPU(cs);
    CPUE2KState *env = &cpu->env;

    dc->version = env->version;
}

static bool e2k_tr_breakpoint_check(DisasContextBase *db, CPUState *cs,
                                    const CPUBreakpoint *bp)
{
    DisasContext *dc = container_of(db, DisasContext, base);

    tcg_gen_movi_tl(e2k_cs.pc, dc->base.pc_next);
    tcg_gen_movi_tl(e2k_cs.npc, dc->npc);
    gen_helper_debug(cpu_env);
    tcg_gen_exit_tb(NULL, 0);
    dc->base.is_jmp = DISAS_NORETURN;
    /*
     * The address covered by the breakpoint must be included in
     * [tb->pc, tb->pc + tb->size) in order to for it to be
     * properly cleared -- thus we increment the PC here so that
     * the logic setting tb->size below does the right thing.
     */
    dc->base.pc_next += 1;
    return true;
}

static void e2k_tr_tb_start(DisasContextBase *db, CPUState *cs)
{
    DisasContext *ctx = container_of(db, DisasContext, base);

    tcg_gen_movi_tl(e2k_cs.ct_cond, 1);
}

static void e2k_tr_insn_start(DisasContextBase *db, CPUState *cs)
{
    DisasContext *dc = container_of(db, DisasContext, base);
    tcg_gen_insn_start(dc->pc);
}

static void e2k_tr_translate_insn(DisasContextBase *db, CPUState *cs)
{
    DisasContext *ctx = container_of(db, DisasContext, base);

    do_decode(ctx, cs);
    do_execute(ctx);
    do_commit(ctx);
    do_branch(ctx);

    /* Free temporary values */
    while(ctx->t32_len) {
        tcg_temp_free_i32(ctx->t32[--ctx->t32_len]);
    }

    while(ctx->t64_len) {
        tcg_temp_free_i64(ctx->t64[--ctx->t64_len]);
    }

    while(ctx->ttl_len) {
        tcg_temp_free(ctx->ttl[--ctx->ttl_len]);
    }
}

static void e2k_tr_tb_stop(DisasContextBase *db, CPUState *cs)
{
    DisasContext *ctx = container_of(db, DisasContext, base);

    switch(ctx->base.is_jmp) {
    case DISAS_NEXT:
    case DISAS_TOO_MANY:
        gen_helper_save_cpu_state(cpu_env);
        gen_goto_tb(ctx, 0, ctx->pc, ctx->npc);
        break;
    case DISAS_NORETURN:
        break;
    default:
        g_assert_not_reached();
        break;
    }
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
    .breakpoint_check   = e2k_tr_breakpoint_check,
    .insn_start         = e2k_tr_insn_start,
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
//    target_ulong pc = data[0];
//    target_ulong npc = data[0];
    // TODO
    qemu_log_mask(LOG_UNIMP, "restore_state_to_opc: not implemented\n");
}

void e2k_tcg_initialize(void) {
    char buf[16] = { 0 };

    static const struct { TCGv_i32 *ptr; int off; const char *name; } r32[] = {
        { &e2k_cs.woff, offsetof(CPUE2KState, wd_base), "woff" },
        { &e2k_cs.wsize, offsetof(CPUE2KState, wd_size), "wsize" },
        { &e2k_cs.boff, offsetof(CPUE2KState, boff), "boff" },
        { &e2k_cs.bsize, offsetof(CPUE2KState, bsize), "bsize" },
        { &e2k_cs.bcur, offsetof(CPUE2KState, bcur), "bcur" },
        { &e2k_cs.psize, offsetof(CPUE2KState, psize), "psize" },
        { &e2k_cs.pcur, offsetof(CPUE2KState, pcur), "pcur" },
    };

    static const struct { TCGv_i64 *ptr; int off; const char *name; } r64[] = {
        { &e2k_cs.pregs, offsetof(CPUE2KState, pf), "pregs" },
        { &e2k_cs.lsr, offsetof(CPUE2KState, lsr), "lsr" },
    };

    static const struct { TCGv *ptr; int off; const char *name; } rtl[] = {
        { &e2k_cs.pc, offsetof(CPUE2KState, ip), "pc" },
        { &e2k_cs.npc, offsetof(CPUE2KState, nip), "npc" },
        { &e2k_cs.ct_cond, offsetof(CPUE2KState, ct_cond), "cond" },
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

    e2k_cs.wptr = tcg_global_mem_new_ptr(cpu_env,
        offsetof(CPUE2KState, wptr), "wptr");

    for (i = 0; i < WREGS_SIZE; i++) {
        snprintf(buf, ARRAY_SIZE(buf), "%%r%d", i);
        e2k_cs.wregs[i] = tcg_global_mem_new_i64(e2k_cs.wptr,
            i * REG_SIZE, buf);
    }

    for (i = 0; i < GREGS_SIZE; i++) {
        snprintf(buf, ARRAY_SIZE(buf), "%%g%d", i);
        e2k_cs.gregs[i] = tcg_global_mem_new_i64(cpu_env,
            offsetof(CPUE2KState, gregs[i]), buf);
    }

    for (i = 0; i < 3; i++) {
        snprintf(buf, ARRAY_SIZE(buf), "%%ctpr%d", i + 1);
        e2k_cs.ctprs[i] = tcg_global_mem_new(cpu_env,
            offsetof(CPUE2KState, ctprs[i]), buf);
    }
}
