#include "qemu/osdep.h"
#include "qemu.h"
#include "exec/log.h"
#include "disas/disas.h"
#include "translate.h"

struct CPUE2KStateTCG e2k_cs;

static inline uint64_t ctpr_new(uint8_t tag, uint8_t opc, uint8_t ipd,
    target_ulong base)
{
    uint64_t ctpr = 0;
    ctpr = deposit64(ctpr, CTPR_BASE_OFF, CTPR_BASE_LEN, base);
    ctpr = deposit64(ctpr, CTPR_TAG_OFF, CTPR_TAG_LEN, tag);
    ctpr = deposit64(ctpr, CTPR_IPD_OFF, CTPR_IPD_LEN, ipd);
    ctpr = deposit64(ctpr, CTPR_OPC_OFF, CTPR_OPC_LEN, opc);
    return ctpr;
}

static inline uint64_t ctpr_new_disp(DisasContext *ctx, Cs0Disp *disp)
{
    target_ulong base = ctx->pc + disp->sdisp;
    return ctpr_new(CTPR_TAG_DISP, disp->opc, disp->ipd, base);
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
        tcg_gen_movi_tl(e2k_cs.pc, npc);
        tcg_gen_exit_tb(ctx->base.tb, tb_num);
    } else {
        /* jump to another page: currently not optimized */
        tcg_gen_movi_tl(e2k_cs.pc, npc);
        tcg_gen_exit_tb(NULL, 0);
    }
}

void e2k_tr_gen_exception(DisasContext *ctx, int which)
{
    TCGv_i32 t = tcg_const_i32(which);

    e2k_gen_save_cpu_state(ctx);
    gen_helper_raise_exception(cpu_env, t);
    ctx->base.is_jmp = DISAS_NORETURN;

    tcg_temp_free_i32(t);
}

void e2k_tr_gen_exception_no_spill(DisasContext *ctx, int excp)
{
    TCGv_i32 t0 = tcg_const_i32(excp);

    e2k_gen_save_cpu_state(ctx);
    gen_helper_raise_exception_no_spill(cpu_env, t0);
    ctx->base.is_jmp = DISAS_NORETURN;

    tcg_temp_free_i32(t0);
}

static inline void illop(DisasContext *ctx)
{
    e2k_tr_gen_exception(ctx, E2K_EXCP_ILLOPC);
}

////////////////////////////////////////////////////////////////////
//// Decode
////////////////////////////////////////////////////////////////////

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
        bundle->ales_present[5] = ALES_PRESENT;
        bundle->ales[5] = 0x01c0;
    }

    if (GET_BIT(hs, 22)) {
        bundle->ales_present[2] = ALES_PRESENT;
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
        bundle->ales_present[5] |= ALES_ALLOCATED;
        bundle->ales_present[2] |= ALES_ALLOCATED;

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
    pos = ((extract32(hs, 4, 3) + 1) << 3) - 4;

    /* Check for CDSj syllables.  */
    for (i = 0; i < extract32(hs, 16, 2); i++) {
        bundle->cds_present[i] = true;
        bundle->cds[i] = translator_ldl(env, pc + pos);
        pos -= 4;
    }

    /* Check for PLSj syllables.  */
    for (i = 0; i < extract32(hs, 18, 2); i++) {
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

    return 8 + extract32(hs, 4, 3) * 8;
}

static inline uint8_t ss_extract_ipd(const UnpackedBundle *raw)
{
    return raw->ss_present ? extract32(raw->ss, 30, 2) : 3;
}

static inline int32_t cs0_extract_sdisp(const UnpackedBundle *raw)
{
    return sextract32(raw->cs0, 0, 28) << 3;
}

static inline void decode_cs1(DisasContext *ctx, const UnpackedBundle *raw)
{
    enum {
        SETR0,
        SETR1,
        SETEI,
        WAIT,
        SETBR,
        CALL,
        MAS_OPC,
        FLUSHR,
        BG
    };

    Bundle *bundle = &ctx->bundle2;
    Cs1 *ret = &bundle->cs1;
    uint32_t cs1 = raw->cs1;
    int opc = extract32(cs1, 28, 4);

    ret->type = CS1_NONE;
    if (!raw->cs1_present) {
        return;
    }

    if (opc == SETR0 || opc == SETR1 || opc == SETBR) {
        Cs1Setr *setr = &ret->setr;
        setr->type = 0;
        if (opc == SETR0 || opc == SETR1) {
            uint32_t lts0 = raw->lts[0];
            if (!raw->lts_present[0]) {
                illop(ctx);
                return;
            }
            setr->type |= opc == SETR1 ? SETR_VFRPSZ : 0;
            setr->type |= SETR_WD;
            setr->wsz = extract32(lts0, 5, 7);
            setr->nfx = extract32(lts0, 4, 1);
            if (ctx->version >= 3) {
                setr->dbl = extract32(lts0, 3, 1);
            }
        }
        if (extract32(cs1, 26, 1)) {
            setr->type |= SETR_BN;
            setr->rbs = extract32(cs1, BR_RBS_OFF, BR_RBS_LEN);
            setr->rsz = extract32(cs1, BR_RSZ_OFF, BR_RSZ_LEN);
            setr->rcur = extract32(cs1, BR_RCUR_OFF, BR_RCUR_LEN);
        }
        if (extract32(cs1, 27, 1)) {
            setr->type |= SETR_BP;
            setr->psz = extract32(cs1, BR_PSZ_OFF, BR_PSZ_LEN);
        }
        ret->type = CS1_SETR;
    } else if (opc == SETEI) {
        if (extract32(cs1, 27, 1)) {
            if (ctx->version < 2) {
                illop(ctx);
                return;
            }
            ret->type = CS1_SETSFT;
        } else {
            ret->type = CS1_SETEI;
            ret->ei = extract32(cs1, 0, 8);
        }
    } else if (opc == WAIT) {
        ret->type = CS1_WAIT;
        ret->wait.all_c = extract32(cs1, 0, 1);
        ret->wait.all_e = extract32(cs1, 1, 1);
        ret->wait.st_c = extract32(cs1, 2, 1);
        ret->wait.ld_c = extract32(cs1, 3, 1);
        ret->wait.fl_c = extract32(cs1, 4, 1);
        ret->wait.ma_c = extract32(cs1, 5, 1);

        if (ctx->version >= 2) {
            ret->wait.trap = extract32(cs1, 6, 1);
        }

        if (ctx->version >= 5) {
            if (ret->wait.st_c) {
                ret->wait.sas = extract32(cs1, 7, 1);
            }
            if (ret->wait.ld_c) {
                ret->wait.sal = extract32(cs1, 8, 1);
            }
        }
    } else if (opc == CALL) {
        int ctop = extract32(raw->ss, 10, 2);
        int wbs = extract32(cs1, 0, 7);
        if (ctop) {
            ret->type = CS1_CALL;
            ret->call_wbs = wbs;
        } else {
            int cs1_ctopc = extract32(cs1, 7, 3);
            int cs0_opc = extract32(raw->cs0, 28, 4);
            int disp = extract32(raw->cs0, 1, 5);
            if (cs1_ctopc != 2 || cs0_opc != 0 || !raw->cs0_present) {
                illop(ctx);
                return;
            }
            ret->type = CS1_HCALL;
            ret->hcall.disp = disp;
            ret->hcall.wbs = wbs;
        }
    } else if (opc == MAS_OPC) {
        ret->type = CS1_MAS;
        ret->mas[0] = extract32(cs1, 21, 7);
        ret->mas[2] = extract32(cs1, 14, 7);
        ret->mas[3] = extract32(cs1, 7, 7);
        ret->mas[5] = extract32(cs1, 0, 7);
    } else if (opc == FLUSHR) {
        ret->type = CS1_FLUSH;
        ret->flush.flushr = (cs1 & 1) != 0;
        ret->flush.flushc = (cs1 & 2) != 0;
    } else if (opc == BG) {
        ret->type = CS1_VFBG;
        ret->vfbg.umask = extract32(cs1, 0, 8);
        ret->vfbg.dmask = extract32(cs1, 8, 8);
        ret->vfbg.chkm4 = extract32(cs1, 16, 1);
    } else {
        illop(ctx);
    }
}

static inline void decode_cs0(DisasContext *ctx, const UnpackedBundle *raw)
{
    Bundle *bundle = &ctx->bundle2;
    Cs0 *ret = &bundle->cs0;
    uint32_t cs0 = raw->cs0;
    int ctpr = extract32(cs0, 30, 2);
    int ctp_opc = extract32(cs0, 28, 2);
    int param_type = extract32(cs0, 0, 3);

    if (!raw->cs0_present) {
        ret->type = CS0_NONE;
        return;
    } else if (ctpr > 0) {
        switch(ctp_opc) {
        case 0: // disp
        case 1: // ldisp
            if (ctp_opc == 1 && ctpr != 2) {
                illop(ctx);
                return;
            }
            ret->type = CS0_DISP;
            ret->disp.opc = ctp_opc;
            ret->disp.ctpr = ctpr;
            ret->disp.sdisp = cs0_extract_sdisp(raw);
            ret->disp.ipd = ss_extract_ipd(raw);
            break;
        case 2:
            ret->type = CS0_SDISP;
            ret->sdisp.ctpr = ctpr;
            ret->sdisp.disp = extract32(cs0, 0, 28);
            ret->sdisp.ipd = ss_extract_ipd(raw);
            break;
        case 3:
            if (param_type == 0 && ctpr == 3) {
                ret->type = CS0_RETURN;
            } else if (param_type == 1) {
                ret->type = CS0_GETTSD;
            }
            break;
        }
    } else {
        switch(ctp_opc) {
        case 0:
            ret->type = CS0_IBRANCH;
            ret->ibranch.sdisp = cs0_extract_sdisp(raw);
            break;
        case 1:
            ret->type = CS0_PREF;
            ret->pref.prefr = extract32(cs0, 0, 2);
            ret->pref.ipd = extract32(cs0, 3, 1);
            ret->pref.disp = extract32(cs0, 4, 24);
            break;
        case 2:
            ret->type = CS0_PUTTSD;
            break;
        case 3:
            if (param_type == 0) {
                ret->type = CS0_DONE;
            } else if (param_type == 3) {
                ret->type = CS0_HRET;
            } else if (param_type == 4) {
                ret->type = CS0_GLAUNCH;
            }
            break;
        }
    }

    switch(ret->type) {
    case CS0_NONE:
        illop(ctx);
        break;
    case CS0_IBRANCH:
    case CS0_DONE:
    case CS0_HRET:
    case CS0_GLAUNCH:
        if (!raw->ss_present
            || extract32(raw->ss, 10, 2) != 0
            || extract32(raw->ss, 0, 9) == 0
            || bundle->cs1.type == CS1_CALL
            || bundle->cs1.type == CS1_HCALL)
        {
            ret->type = CS0_NONE;
            illop(ctx);
        }
        break;
    default:
        break;
    }
}

static inline void decode_ct_cond(DisasContext *ctx, const UnpackedBundle *raw)
{
    ctx->ct.type = CT_NONE;
    ctx->ct.cond_type = 0;
    int ctpr = extract32(raw->ss, 10, 2);
    if (ctpr != 0) {
        if (ctx->ct.type == CT_NONE) {
            ctx->ct.type = CT_JUMP;
        }
        ctx->ct.u.ctpr = e2k_cs.ctprs[ctpr - 1];
    }
    ctx->ct.psrc = extract32(raw->ss, 0, 5);
    ctx->ct.cond_type = extract32(raw->ss, 5, 4);
}

////////////////////////////////////////////////////////////////////
//// Generate
////////////////////////////////////////////////////////////////////

static inline void gen_set_ctpr(int index, uint64_t ctpr)
{
    assert(0 < index && index < 4);
    tcg_gen_movi_i64(e2k_cs.ctprs[index - 1], ctpr);
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
    tcg_gen_mov_tl(e2k_cs.pc, t1);
    tcg_gen_lookup_and_goto_ptr();

    tcg_temp_free(t1);
    tcg_temp_free_i64(t0);
}

static inline void gen_vfrpsz(DisasContext *ctx)
{
    Cs1 *cs1 = &ctx->bundle2.cs1;
    Cs1Setr *setr = &cs1->setr;

    if (cs1->type == CS1_SETR && (setr->type & SETR_VFRPSZ)) {
        e2k_todo_illop(ctx, "vfrpsz");
    }
}

static inline void gen_setwd(DisasContext *ctx)
{
    Cs1 *cs1 = &ctx->bundle2.cs1;
    Cs1Setr *setr = &cs1->setr;

    if (cs1->type == CS1_SETR && (setr->type & SETR_WD)) {
        TCGv_i32 t0 = tcg_const_i32(setr->wsz);
        TCGv_i32 t1 = tcg_const_i32(setr->nfx);
        TCGv_i32 t2 = tcg_const_i32(setr->dbl);
        ctx->wd_size = setr->wsz * 2;
        gen_helper_setwd(cpu_env, t0, t1, t2);
        tcg_temp_free_i32(t2);
        tcg_temp_free_i32(t1);
        tcg_temp_free_i32(t0);
    }
}

static inline void gen_setbn(DisasContext *ctx)
{
    Cs1 *cs1 = &ctx->bundle2.cs1;
    Cs1Setr *setr = &cs1->setr;

    if (cs1->type == CS1_SETR && (setr->type & SETR_BN)) {
        ctx->bsize = (setr->rsz + 1) * 2;
        tcg_gen_movi_i32(e2k_cs.boff, setr->rbs * 2);
        tcg_gen_movi_i32(e2k_cs.bsize, (setr->rsz + 1) * 2);
        tcg_gen_movi_i32(e2k_cs.bcur, setr->rcur * 2);
    }
}

static inline void gen_setbp(DisasContext *ctx)
{
    Cs1 *cs1 = &ctx->bundle2.cs1;
    Cs1Setr *setr = &cs1->setr;

    if (cs1->type == CS1_SETR && (setr->type & SETR_BP)) {
        tcg_gen_movi_i32(e2k_cs.psize, setr->psz);
        tcg_gen_movi_i32(e2k_cs.pcur, 0);
    }
}

static inline void gen_setr(DisasContext *ctx)
{
    gen_vfrpsz(ctx);
    gen_setwd(ctx);
    gen_setbn(ctx);
    gen_setbp(ctx);
}

static inline void gen_cs1(DisasContext *ctx)
{
    Cs1 *cs1 = &ctx->bundle2.cs1;

    switch(cs1->type) {
    case CS1_NONE:
    case CS1_MAS:
    case CS1_SETR:
        break;
    case CS1_CALL:
        ctx->ct.type = CT_CALL;
        ctx->ct.wbs = cs1->call_wbs;
        break;
    case CS1_WAIT:
        e2k_todo(ctx, "wait");
        break;
    case CS1_FLUSH:
        e2k_todo(ctx, "flush");
        break;
    default:
        e2k_todo_illop(ctx, "unimplemented %d", cs1->type);
        break;
    }
}

static inline void gen_cs0(DisasContext *ctx)
{
    Cs0 *cs0 = &ctx->bundle2.cs0;

    switch(cs0->type) {
    case CS0_NONE:
        break;
    case CS0_IBRANCH:
        ctx->ct.type = CT_IBRANCH;
        ctx->ct.u.target = ctx->pc + cs0->ibranch.sdisp;
        break;
    case CS0_DISP: {
        uint64_t ctpr = ctpr_new_disp(ctx, &cs0->disp);
        gen_set_ctpr(cs0->disp.ctpr, ctpr);
        break;
    }
    case CS0_SDISP: {
        // TODO: real sdisp target address
        target_ulong target = (0xe2UL << 40) | cs0->sdisp.disp;
        uint64_t ctpr = ctpr_new(CTPR_TAG_SDISP, 0, cs0->sdisp.ipd, target);
        gen_set_ctpr(cs0->sdisp.ctpr, ctpr);
        break;
    }
    case CS0_RETURN: {
        TCGv_i32 t0 = tcg_const_i32(cs0->ret.ipd);
        gen_helper_prep_return(e2k_cs.ctprs[2], cpu_env, t0);
        tcg_temp_free_i32(t0);
        break;
    }
    default:
        e2k_todo_illop(ctx, "unimplemented %d", cs0->type);
        break;
    }
}

static inline void gen_ct_cond(DisasContext *ctx)
{
    ControlTransfer *ct = &ctx->ct;
    TCGv_i32 pcond, lcond;

    if (ct->type == CT_NONE) {
        return;
    }

    if (ct->cond_type == 1) {
        tcg_gen_movi_i32(e2k_cs.ct_cond, 1);
        return;
    }

    pcond = tcg_temp_new_i32();
    lcond = tcg_temp_new_i32();

    switch (ct->cond_type) {
    case 0x2:
    case 0x6:
    case 0xf:
        /* %predN */
        e2k_gen_preg_i32(pcond, ct->psrc);
        break;
    case 0x3:
    case 0x7:
    case 0xe: {
        TCGv_i32 t0 = tcg_temp_new_i32();
        e2k_gen_preg_i32(t0, ct->psrc);
        tcg_gen_setcondi_i32(TCG_COND_EQ, pcond, t0, 0);
        tcg_temp_free_i32(t0);
        break;
    }
    default:
        break;
    }

    switch (ct->cond_type) {
    case 0x4:
    case 0x6:
    case 0xe:
        /* #LOOP_END */
        e2k_gen_is_loop_end_i32(lcond);
        break;
    case 0x5:
    case 0x7:
    case 0xf: { /* #NOT_LOOP_END */
        TCGv_i32 t0 = tcg_temp_new_i32();
        e2k_gen_is_loop_end_i32(t0);
        tcg_gen_setcondi_i32(TCG_COND_EQ, lcond, t0, 0);
        tcg_temp_free_i32(t0);
        break;
    }
    }

    switch (ct->cond_type) {
    case 0x2:
    case 0x3:
        /* {,~}%predN */
        tcg_gen_mov_i32(e2k_cs.ct_cond, pcond);
        break;
    case 0x4:
    case 0x5:
        /* #{,NOT_}LOOP_END */
        tcg_gen_mov_i32(e2k_cs.ct_cond, lcond);
        break;
    case 0x6:
    case 0xe: {
        /* {,~}%predN || #LOOP_END */
        TCGv_i32 z = tcg_const_i32(0);
        TCGv_i32 t0 = tcg_temp_new_i32();

        tcg_gen_or_i32(t0, pcond, lcond);
        tcg_gen_movcond_i32(TCG_COND_EQ, e2k_cs.ct_cond, e2k_cs.lsr_pcnt, z, t0, lcond);
        tcg_temp_free_i32(t0);
        tcg_temp_free_i32(z);
        break;
    }
    case 0x7:
    case 0xf: {
        /* {,~}%predN && #NOT_LOOP_END */
        TCGv_i32 z = tcg_const_i32(0);
        TCGv_i32 t0 = tcg_temp_new_i32();

        tcg_gen_and_i32(t0, pcond, lcond);
        tcg_gen_movcond_i32(TCG_COND_EQ, e2k_cs.ct_cond, e2k_cs.lsr_pcnt, z, t0, lcond);
        tcg_temp_free_i32(t0);
        tcg_temp_free_i32(z);
        break;
    }
    case 0x8:
        /* %MLOCK || %dt_alM */
        if (ct->psrc & 0xf) {
//            static const int conv[] = {0, 1, 3, 4};
            int i;

            e2k_todo(ctx, "%%MLOCK || %%dt_alM");
            // %dt_al
            for (i = 0; i < 4; i++) {
                if (ct->psrc & (1 << i)) {
                    // i
                }
            }
        } else {
            /* %MLOCK */
            if (ctx->mlock) {
                tcg_gen_mov_i32(e2k_cs.ct_cond, ctx->mlock);
            } else {
                tcg_gen_movi_i32(e2k_cs.ct_cond, 0);
            }
        }
        break;
    case 0x9: {
        /* `lock_cond || pl_cond' control transfer conditions.  */
        unsigned int type = (ct->psrc & 0x18) >> 3;
        if (type == 0) {
//            static const int cmp_num_to_alc[] = {0, 1, 3, 4};
//            unsigned int cmp_num = (psrc & 0x6) >> 1;
//            unsigned int neg = psrc & 0x1;
//
//            my_printf ("%%MLOCK || %s%%cmp%d", neg ? "~" : "",
//                cmp_num_to_alc[cmp_num]);
            e2k_todo(ctx, "%%MLOCK || %%cmpN");
        } else if (type == 1) {
//            unsigned int cmp_jk = (psrc & 0x4) >> 2;
//            unsigned int negj = (psrc & 0x2) >> 1;
//            unsigned int negk = psrc & 0x1;
//
//            my_printf ("%%MLOCK || %s%%cmp%d || %s%%cmp%d",
//                     negj ? "~" : "", cmp_jk == 0 ? 0 : 3,
//                     negk ? "~" : "", cmp_jk == 0 ? 1 : 4);
            e2k_todo(ctx, "%%MLOCK || %%cmpN || %%cmpM");
        } else if (type == 2) {
//                unsigned int clp_num = (psrc & 0x6) >> 1;
//                unsigned int neg = psrc & 0x1;

            // "%%MLOCK || %s%%clp%d", neg ? "~" : "", clp_num
            e2k_todo(ctx, "%%MLOCK || %%clpN");
        }
        break;
    }
    default:
        e2k_todo_illop(ctx, "undefined control transfer type %#x", ct->cond_type);
        break;
    }

    tcg_temp_free_i32(lcond);
    tcg_temp_free_i32(pcond);
}

static inline TCGCond cond_from_advance(int advance)
{
    switch (advance) {
    case 0x01: return TCG_COND_EQ;
    case 0x02: return TCG_COND_NE;
    case 0x03: return TCG_COND_ALWAYS;
    default: return TCG_COND_NEVER;
    }
}

static void gen_movcond_flag_i32(TCGv_i32 ret, int flag, TCGv_i32 cond,
    TCGv_i32 v1, TCGv_i32 v2)
{
    TCGv_i32 one = tcg_const_i32(1);
    TCGCond c = cond_from_advance(flag);

    tcg_gen_movcond_i32(c, ret, cond, one, v1, v2);
    tcg_temp_free_i32(one);
}

static inline void gen_dec_wrap(TCGv_i32 ret, TCGv_i32 cur, int n,
    TCGv_i32 size)
{
    TCGv_i32 t0 = tcg_temp_new_i32();

    tcg_gen_addi_i32(t0, size, n);
    tcg_gen_sub_i32(t0, t0, cur);
    tcg_gen_remu_i32(t0, t0, size);
    tcg_gen_sub_i32(ret, size, t0);

    tcg_temp_free_i32(t0);
}

static inline void gen_cur_dec(DisasContext *ctx, TCGv_i32 ret, int cond,
    TCGv_i32 cur, int n, TCGv_i32 size)
{
    TCGLabel *l0 = gen_new_label();
    TCGv_i32 t0 = tcg_temp_new_i32();

    tcg_gen_brcondi_i32(TCG_COND_EQ, size, 0, l0);
    gen_dec_wrap(t0, cur, n, size);
    gen_movcond_flag_i32(ret, cond, e2k_cs.ct_cond, t0, cur);
    gen_set_label(l0);

    tcg_temp_free_i32(t0);
}

static void gen_dec_sat_i32(TCGv_i32 ret, TCGv_i32 arg0)
{
    TCGv_i32 t0 = tcg_const_i32(0);
    TCGv_i32 t1 = tcg_temp_new_i32();
    tcg_gen_subi_i32(t1, arg0, 1);
    tcg_gen_movcond_i32(TCG_COND_EQ, ret, arg0, t0, arg0, t1);
    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
}

static void gen_advance_loop_counters(void)
{
    TCGv_i32 z = tcg_const_i32(0);
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i32 t1 = tcg_temp_new_i32();
    TCGv_i32 t2 = tcg_temp_new_i32();
    TCGv_i32 t3 = tcg_temp_new_i32();

    gen_dec_sat_i32(e2k_cs.lsr_pcnt, e2k_cs.lsr_pcnt);
    gen_dec_sat_i32(e2k_cs.lsr_lcnt, e2k_cs.lsr_lcnt);
    tcg_gen_setcondi_i32(TCG_COND_EQ, t0, e2k_cs.lsr_pcnt, 0);
    tcg_gen_setcondi_i32(TCG_COND_EQ, t1, e2k_cs.lsr_lcnt, 0);
    tcg_gen_mov_i32(e2k_cs.lsr_over, t1);
    gen_dec_sat_i32(t2, e2k_cs.lsr_ecnt);
    tcg_gen_and_i32(t3, t1, e2k_cs.lsr_vlc);
    tcg_gen_movcond_i32(TCG_COND_NE, e2k_cs.lsr_ecnt, t3, z, t2, e2k_cs.lsr_ecnt);

    tcg_temp_free_i32(t3);
    tcg_temp_free_i32(t2);
    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
    tcg_temp_free_i32(z);
}

static inline void gen_stubs(DisasContext *ctx)
{
    uint32_t ss = ctx->bundle.ss;
    int alc = extract32(ss, 16, 2);
    int abp = extract32(ss, 18, 2);
    int abn = extract32(ss, 21, 2);
    int abg = extract32(ss, 23, 2);
    int vfdi = extract32(ss, 26, 1);

    if (alc) {
        TCGLabel *l0 = gen_new_label();
        TCGCond cond = cond_from_advance(alc);

        tcg_gen_brcondi_i32(tcg_invert_cond(cond), e2k_cs.ct_cond, 1, l0);
        gen_advance_loop_counters();
        gen_set_label(l0);
    }

    if (abp) {
        TCGv_i32 t0 = tcg_temp_local_new_i32();
        tcg_gen_addi_i32(t0, e2k_cs.psize, 1);
        gen_cur_dec(ctx, e2k_cs.pcur, abp, e2k_cs.pcur, 1, t0);
        tcg_temp_free_i32(t0);
    }

    if (abn) {
        gen_cur_dec(ctx, e2k_cs.bcur, abn, e2k_cs.bcur, 2, e2k_cs.bsize);
    }

    if (abg != 0) {
        // TODO: impl abg
        e2k_todo_illop(ctx, "abg");
    }

    if (vfdi != 0) {
        // TODO: impl vfdi
        e2k_todo_illop(ctx, "vfdi");
    }
}

static inline target_ulong do_decode(DisasContext *ctx, CPUState *cs)
{
    E2KCPU *cpu = E2K_CPU(cs);
    CPUE2KState *env = &cpu->env;
    unsigned int len;

    ctx->pc = ctx->base.pc_next;
    len = unpack_bundle(env, ctx->pc, &ctx->bundle);

    if (len == 0) {
        e2k_tr_gen_exception(ctx, E2K_EXCP_ILLOPC);
    }

    decode_cs1(ctx, &ctx->bundle);
    decode_cs0(ctx, &ctx->bundle);
    decode_ct_cond(ctx, &ctx->bundle);

    return ctx->pc + len;
}

static inline void gen_maperr_condi_i32(TCGCond cond, TCGv_i32 arg1, int arg2)
{
    TCGLabel *l0 = gen_new_label();
    tcg_gen_brcondi_i32(tcg_invert_cond(cond), arg1, arg2, l0);
    e2k_gen_exception(E2K_EXCP_MAPERR);
    gen_set_label(l0);
}

static inline void do_checks(DisasContext *ctx)
{
    const Bundle *b = &ctx->bundle2;
    const Cs1Setr *setr = &b->cs1.setr;

    if (ctx->wd_size != DYNAMIC) {
        /* %rN src static check */
        if (ctx->wd_size <= ctx->max_r_src) {
            e2k_tr_gen_exception(ctx, E2K_EXCP_MAPERR);
        }
        /* %rN dst static check */
        if (b->cs1.type == CS1_SETR && (setr->type & SETR_WD)) {
            if (setr->wsz * 2 <= ctx->max_r_dst) {
                e2k_tr_gen_exception(ctx, E2K_EXCP_MAPERR);
            }
        } else if (ctx->wd_size <= ctx->max_r_dst) {
            e2k_tr_gen_exception(ctx, E2K_EXCP_MAPERR);
        }
    } else if (b->cs1.type == CS1_SETR && (setr->type & SETR_WD)) {
        /* %rN src dynamic check */
        if (ctx->max_r < ctx->max_r_src) {
            ctx->max_r = ctx->max_r_src;
            gen_maperr_condi_i32(TCG_COND_LE, e2k_cs.wd_size, ctx->max_r_src);
        }

        /* %rN dst static check */
        if (setr->wsz * 2 <= ctx->max_r_dst) {
            e2k_tr_gen_exception(ctx, E2K_EXCP_MAPERR);
        }
    } else {
        /* %rN src/dst dynamic check */
        int max = MAX(ctx->max_r_src, ctx->max_r_dst);
        if (ctx->max_r < max) {
            ctx->max_r = max;
            gen_maperr_condi_i32(TCG_COND_LE, e2k_cs.wd_size, max);
        }
    }

    if (ctx->bsize != DYNAMIC) {
        /* %b[N] src/dst static check */
        if (ctx->bsize <= ctx->max_b_cur) {
            e2k_tr_gen_exception(ctx, E2K_EXCP_MAPERR);
        }
    } else if (ctx->max_b < ctx->max_b_cur) {
        /* %b[N] src/dst dynamic check */
        ctx->max_b = ctx->max_b_cur;
        gen_maperr_condi_i32(TCG_COND_LE, e2k_cs.bsize, ctx->max_b);
    }
}

/*
 * Executes instructions from a bundle and store the results to
 * temporary buffer.
 */
static inline void do_execute(DisasContext *ctx)
{
    ctx->loop_mode = (ctx->bundle.hs & (1 << 10)) != 0;
    gen_cs0(ctx);
    gen_cs1(ctx);
    e2k_alc_execute(ctx);
    gen_ct_cond(ctx);
    e2k_aau_execute(ctx);
    e2k_plu_execute(ctx);
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
    gen_setr(ctx);
    e2k_alc_commit(ctx);
    e2k_aau_commit(ctx);
    e2k_plu_commit(ctx);
    gen_stubs(ctx);
}

static inline void do_branch(DisasContext *ctx, target_ulong pc_next)
{
    TCGLabel *l0 = gen_new_label();

    if (ctx->do_check_illtag) {
        tcg_gen_brcondi_i32(TCG_COND_EQ, ctx->illtag, 0, l0);
        e2k_gen_exception(E2K_EXCP_ILLOPC);
        gen_set_label(l0);
    }

    if (ctx->ct.type == CT_NONE) {
        e2k_gen_save_pc(ctx->base.pc_next);
        return;
    }

    ctx->base.is_jmp = DISAS_NORETURN;

    if (ctx->ct.cond_type > 1) {
        TCGLabel *l0 = gen_new_label();
        tcg_gen_brcondi_i32(TCG_COND_NE, e2k_cs.ct_cond, 0, l0);
        gen_goto_tb(ctx, TB_EXIT_IDX0, ctx->pc, pc_next);
        gen_set_label(l0);
    }

    switch(ctx->ct.type) {
    case CT_IBRANCH:
        gen_goto_tb(ctx, TB_EXIT_IDX1, ctx->pc, ctx->ct.u.target);
        break;
    case CT_JUMP: {
        TCGLabel *l0 = gen_new_label();
        TCGLabel *l1 = gen_new_label();
        TCGv_i64 t0 = tcg_temp_local_new_i64();

        gen_ctpr_tag(t0, ctx->ct.u.ctpr);
        tcg_gen_brcondi_i64(TCG_COND_EQ, t0, CTPR_TAG_DISP, l0);
        tcg_gen_brcondi_i64(TCG_COND_EQ, t0, CTPR_TAG_RETURN, l1);
        tcg_temp_free_i64(t0);

        // TODO: ldisp, sdisp
        e2k_gen_exception(0);

        gen_set_label(l1);
        gen_helper_return(cpu_env);

        gen_set_label(l0);
        gen_goto_ctpr_disp(ctx->ct.u.ctpr);
        break;
    }
    case CT_CALL: {
        TCGv_i32 wbs = tcg_const_i32(ctx->ct.wbs);
        TCGv npc = tcg_const_tl(pc_next);

        gen_helper_call(cpu_env, ctx->ct.u.ctpr, wbs, npc);
        tcg_gen_lookup_and_goto_ptr();

        tcg_temp_free(npc);
        tcg_temp_free_i32(wbs);
        break;
    }
    default:
        break;
    }
}

static void e2k_tr_init_disas_context(DisasContextBase *db, CPUState *cs)
{
    static int version = -1;
    DisasContext *ctx = container_of(db, DisasContext, base);
    E2KCPU *cpu = E2K_CPU(cs);
    CPUE2KState *env = &cpu->env;

    ctx->version = env->version;

    if (version != ctx->version) {
        if (version > 0) {
            // FIXME: can it happen?
            e2k_todo(ctx, "reinitialize alc map");
        }
        alc_init(ctx);
        version = ctx->version;
    }
}

static bool e2k_tr_breakpoint_check(DisasContextBase *db, CPUState *cs,
                                    const CPUBreakpoint *bp)
{
    DisasContext *ctx = container_of(db, DisasContext, base);

    e2k_gen_save_pc(ctx->base.pc_next);
    gen_helper_debug(cpu_env);
    tcg_gen_exit_tb(NULL, TB_EXIT_IDX0);
    ctx->base.is_jmp = DISAS_NORETURN;
    /*
     * The address covered by the breakpoint must be included in
     * [tb->pc, tb->pc + tb->size) in order to for it to be
     * properly cleared -- thus we increment the PC here so that
     * the logic setting tb->size below does the right thing.
     */
    ctx->base.pc_next += 1;
    return true;
}

static void e2k_tr_tb_start(DisasContextBase *db, CPUState *cs)
{
    DisasContext *ctx = container_of(db, DisasContext, base);
    E2KCPU *cpu = E2K_CPU(cs);
    CPUE2KState *env = &cpu->env;

    ctx->wd_size = DYNAMIC;
    ctx->max_r = -1;
    ctx->max_r_src = -1;
    ctx->max_r_dst = -1;
    ctx->bsize = DYNAMIC;
    ctx->max_b = -1;
    ctx->max_b_cur = -1;

    tcg_gen_movi_i32(e2k_cs.ct_cond, 0);

    if (env->is_bp) {
        TCGLabel *l0 = gen_new_label();
        tcg_gen_brcondi_i32(TCG_COND_EQ, e2k_cs.is_bp, 0, l0);
        gen_helper_break_restore_state(cpu_env);
        gen_set_label(l0);
    }
}

static void e2k_tr_insn_start(DisasContextBase *db, CPUState *cs)
{
    DisasContext *ctx = container_of(db, DisasContext, base);

    tcg_gen_insn_start(ctx->base.pc_next);

    memset(ctx->mas, 0, sizeof(ctx->mas));
    memset(&ctx->bundle2, 0, sizeof(ctx->bundle2));

    ctx->max_r_src = -1;
    ctx->max_r_dst = -1;
    ctx->max_b_cur = -1;

    ctx->do_check_illtag = false;
    ctx->illtag = e2k_get_temp_i32(ctx);
    tcg_gen_movi_i32(ctx->illtag, 0);
}

static void e2k_tr_translate_insn(DisasContextBase *db, CPUState *cs)
{
    DisasContext *ctx = container_of(db, DisasContext, base);
    target_ulong pc_next;

    pc_next = do_decode(ctx, cs);
    do_execute(ctx);
    do_checks(ctx);
    do_commit(ctx);
    do_branch(ctx, pc_next);

    ctx->mlock = NULL;
    ctx->base.pc_next = pc_next;

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

    if (ctx->base.is_jmp == DISAS_TOO_MANY) {
        gen_goto_tb(ctx, TB_EXIT_IDX0, ctx->pc, ctx->base.pc_next);
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
    env->ip = data[0];
}

void e2k_tcg_initialize(void) {
    char buf[16] = { 0 };

    static const struct { TCGv_i32 *ptr; int off; const char *name; } r32[] = {
        { &e2k_cs.wd_base, offsetof(CPUE2KState, wd.base), "woff" },
        { &e2k_cs.wd_size, offsetof(CPUE2KState, wd.size), "wsize" },
        { &e2k_cs.boff, offsetof(CPUE2KState, bn.base), "boff" },
        { &e2k_cs.bsize, offsetof(CPUE2KState, bn.size), "bsize" },
        { &e2k_cs.bcur, offsetof(CPUE2KState, bn.cur), "bcur" },
        { &e2k_cs.psize, offsetof(CPUE2KState, bp.size), "psize" },
        { &e2k_cs.pcur, offsetof(CPUE2KState, bp.cur), "pcur" },
        { &e2k_cs.is_bp, offsetof(CPUE2KState, is_bp), "is_bp" },
        { &e2k_cs.wdbl, offsetof(CPUE2KState, wdbl), "wdbl" },
        { &e2k_cs.lsr_lcnt, offsetof(CPUE2KState, lsr_lcnt), "lsr_lcnt" },
        { &e2k_cs.lsr_ecnt, offsetof(CPUE2KState, lsr_ecnt), "lsr_ecnt" },
        { &e2k_cs.lsr_vlc, offsetof(CPUE2KState, lsr_vlc), "lsr_vlc" },
        { &e2k_cs.lsr_over, offsetof(CPUE2KState, lsr_over), "lsr_over" },
        { &e2k_cs.lsr_pcnt, offsetof(CPUE2KState, lsr_pcnt), "lsr_pcnt" },
        { &e2k_cs.lsr_strmd, offsetof(CPUE2KState, lsr_strmd), "lsr_strmd" },
        { &e2k_cs.aasti_tags, offsetof(CPUE2KState, aau.sti_tags), "aasti_tags" },
        { &e2k_cs.aaind_tags, offsetof(CPUE2KState, aau.ind_tags), "aaind_tags" },
        { &e2k_cs.aaincr_tags, offsetof(CPUE2KState, aau.incr_tags), "aaincr_tags" },
        { &e2k_cs.ct_cond, offsetof(CPUE2KState, ct_cond), "cond" },
    };

    static const struct { TCGv_i64 *ptr; int off; const char *name; } r64[] = {
        { &e2k_cs.pregs, offsetof(CPUE2KState, pregs), "pregs" },
    };

    static const struct { TCGv *ptr; int off; const char *name; } rtl[] = {
        { &e2k_cs.pc, offsetof(CPUE2KState, ip), "pc" },
        { &e2k_cs.npc, offsetof(CPUE2KState, nip), "npc" },
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

    for (i = 0; i < 3; i++) {
        snprintf(buf, ARRAY_SIZE(buf), "%%ctpr%d", i + 1);
        e2k_cs.ctprs[i] = tcg_global_mem_new_i64(cpu_env,
            offsetof(CPUE2KState, ctprs[i].raw), buf);
    }

    for (i = 0; i < 16; i++) {
        snprintf(buf, ARRAY_SIZE(buf), "%%aasti%d", i);
        e2k_cs.aasti[i] = tcg_global_mem_new_i32(cpu_env,
            offsetof(CPUE2KState, aau.stis[i]), buf);
    }

    for (i = 0; i < 16; i++) {
        snprintf(buf, ARRAY_SIZE(buf), "%%aaind%d", i);
        e2k_cs.aaind[i] = tcg_global_mem_new_i32(cpu_env,
            offsetof(CPUE2KState, aau.inds[i]), buf);
    }

    for (i = 0; i < 7; i++) {
        snprintf(buf, ARRAY_SIZE(buf), "%%aaincr%d", i);
        e2k_cs.aaincr[i] = tcg_global_mem_new_i32(cpu_env,
            offsetof(CPUE2KState, aau.incrs[i]), buf);
    }

    for (i = 0; i < 32; i++) {
        snprintf(buf, ARRAY_SIZE(buf), "%%aad%d_lo", i);
        e2k_cs.aad_lo[i] = tcg_global_mem_new_i64(cpu_env,
            offsetof(CPUE2KState, aau.ds[i].lo), buf);
        snprintf(buf, ARRAY_SIZE(buf), "%%aad%d_hi", i);
        e2k_cs.aad_hi[i] = tcg_global_mem_new_i64(cpu_env,
            offsetof(CPUE2KState, aau.ds[i].hi), buf);
    }
}
