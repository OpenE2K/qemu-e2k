#include "qemu/osdep.h"
#include "qemu.h"
#include "disas/disas.h"
#include "exec/translator.h"
#include "tcg/tcg-op.h"
#include "exec/log.h"
#include "translate.h"

struct CPUE2KStateTCG e2k_cpu;

#define GET_BIT(v, index) (((v) >> index) & 1)
#define GET_FIELD(v, start, end) \
    (((v) >> (start)) & ((1 << ((end) - (start) + 1)) - 1))

#define IS_BASED(i) (((i) & 0x80) == 0)
#define IS_REGULAR(i) (((i) & 0xc0) == 0x80)
#define IS_IMM5(i) (((i) & 0xe0) == 0xc0)
#define IS_IMM4(i) (((i) & 0xf0) == 0xc0)
#define IS_LIT(i) (((i) & 0xf0) == 0xd0)
#define IS_LIT16_LO(i) (((i) & 0x0e) == 0x00)
#define IS_LIT16_HI(i) (((i) & 0x0e) == 0x04)
#define IS_LIT32(i) (((i) & 0x0c) == 0x08)
#define IS_LIT64(i) (((i) & 0x0c) == 0x0c)
#define IS_GLOBAL(i) (((i) & 0xe0) == 0xe0)

#define GET_BASED(i) ((i) & 0x7f)
#define GET_REGULAR(i) ((i) & 0x3f)
#define GET_IMM5(i) ((i) & 0x1f)
#define GET_IMM4(i) ((i) & 0x0f)
#define GET_LIT(i) ((i) & 0x03)
#define GET_GLOBAL(i) ((i) & 0x1f)

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

static inline TCGv_i32 get_temp_i32(DisasContext *dc)
{
    assert(dc->t32_len < ARRAY_SIZE(dc->t32));
    return dc->t32[dc->t32_len++] = tcg_temp_new_i32();
}

static inline TCGv_i64 get_temp_i64(DisasContext *dc)
{
    assert(dc->t64_len < ARRAY_SIZE(dc->t64));
    return dc->t64[dc->t64_len++] = tcg_temp_new_i64();
}

static inline TCGv get_temp(DisasContext *dc)
{
    assert(dc->ttl_len < ARRAY_SIZE(dc->ttl));
    return dc->ttl[dc->ttl_len++] = tcg_temp_new();
}

static inline void save_state(DisasContext *dc)
{
    tcg_gen_movi_tl(e2k_cpu.pc, dc->pc);
}

static void gen_exception(DisasContext *dc, int which)
{
    TCGv_i32 t;

    save_state(dc);
    t = tcg_const_i32(which);
    gen_helper_raise_exception(cpu_env, t);
    tcg_temp_free_i32(t);
    dc->base.is_jmp = DISAS_NORETURN;
}

// FIXME: x must not be greater than y * 2
static inline void gen_wrap_i32(TCGv_i32 ret, TCGv_i32 x, TCGv_i32 y)
{
    TCGv_i32 t0 = tcg_temp_new_i32();

    tcg_gen_sub_i32(t0, x, y);
    tcg_gen_movcond_i32(TCG_COND_LTU, ret, x, y, x, t0);

    tcg_temp_free_i32(t0);
}

static inline void reset_is_jmp()
{
    tcg_gen_movi_i32(e2k_cpu.is_jmp, 0);
}

static inline void set_is_jmp()
{
    tcg_gen_movi_i32(e2k_cpu.is_jmp, 1);
}

static inline void gen_rcur_move()
{
    TCGv_i32 tmp = tcg_temp_new_i32();
    tcg_gen_addi_i32(tmp, e2k_cpu.rcur, 2);
    gen_wrap_i32(e2k_cpu.rcur, tmp, e2k_cpu.rsz);
    tcg_temp_free_i32(tmp);
}

static inline TCGv_i64 get_preg(DisasContext *dc, int reg)
{
    TCGv_i64 ret = get_temp(dc);
    TCGv_i64 tmp = tcg_temp_new_i64();
    assert(reg < 32);
    tcg_gen_shri_i64(tmp, e2k_cpu.pregs, reg * 2);
    // TODO: should return preg tag?
    tcg_gen_andi_i64(ret, tmp, 0x01);
    tcg_temp_free_i64(tmp);
    return ret;
}

static inline void gen_store_preg(int reg, TCGv_i64 val)
{
    TCGv_i64 t0 = tcg_temp_new_i64();
    TCGv_i64 t1 = tcg_temp_new_i64();
    TCGv_i64 t2 = tcg_temp_new_i64();
    tcg_gen_andi_i64(t0, e2k_cpu.pregs, ~(3 << (reg * 2)));
    tcg_gen_andi_i64(t1, val, 0x03);
    tcg_gen_shli_i64(t2, t1, reg * 2);
    tcg_gen_or_i64(e2k_cpu.pregs, t0, t2);
    tcg_temp_free_i64(t2);
    tcg_temp_free_i64(t1);
    tcg_temp_free_i64(t0);
}

static inline void gen_wreg_offset(TCGv_i32 ret, int reg)
{
    assert(reg < 64);
    // TODO: exception (wsz * 2) <= reg_index

    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i32 t1 = tcg_temp_new_i32();
    TCGv_i32 t2 = tcg_const_i32(WREGS_SIZE);

    tcg_gen_addi_i32(t0, e2k_cpu.wbs, reg);     // t = win_start + reg_index
    gen_wrap_i32(t1, t0, t2);           // t = t % WIN_REGS_COUNT
    tcg_gen_muli_i32(ret, t1, REG_SIZE);    // t = t * REG_SIZE_IN_BYTES

    tcg_temp_free_i32(t2);
    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
}

static inline void gen_wreg_ptr(TCGv_ptr ret, int reg)
{
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_ptr t1 = tcg_temp_new_ptr();
    gen_wreg_offset(t0, reg);
    tcg_gen_ext_i32_ptr(t1, t0);
    tcg_gen_add_ptr(ret, e2k_cpu.win_ptr, t1);
    tcg_temp_free_ptr(t1);
    tcg_temp_free_i32(t0);
}

static inline TCGv_i64 get_wreg(DisasContext *dc, int reg)
{
    TCGv_i64 ret = get_temp(dc);
    TCGv_ptr ptr = tcg_temp_new_ptr();
    gen_wreg_ptr(ptr, reg);
    tcg_gen_ld_i64(ret, ptr, 0);
    tcg_temp_free_ptr(ptr);
    return ret;
}

static inline void gen_store_wreg(int reg, TCGv_i64 val)
{
    TCGv_ptr ptr = tcg_temp_new_ptr();
    gen_wreg_ptr(ptr, reg);
    tcg_gen_st_i64(val, ptr, 0);
    tcg_temp_free_ptr(ptr);
}

static inline void gen_breg_offset(TCGv_i32 ret, int reg)
{
    assert(reg < 128);
    // TODO: exception cpu_wsz <= reg || cpu_rsz <= reg
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i32 t1 = tcg_temp_new_i32();
    TCGv_i32 t2 = tcg_temp_new_i32();
    TCGv_i32 t3 = tcg_temp_new_i32();
    TCGv_i32 t4 = tcg_temp_new_i32();
    TCGv_i32 t5 = tcg_const_i32(WREGS_SIZE);
    TCGv_i32 t6 = tcg_temp_new_i32();

    // t2 = (index + rsz + rcur) % rsz
    tcg_gen_sub_i32(t0, e2k_cpu.rsz, e2k_cpu.rcur);
    tcg_gen_addi_i32(t1, t0, reg);
    gen_wrap_i32(t2, t1, e2k_cpu.rsz);

    // (t2 + wbs + rbs) % WIN_REGS_COUNT
    tcg_gen_add_i32(t3, e2k_cpu.wbs, e2k_cpu.rbs);
    tcg_gen_add_i32(t4, t2, t3);
    gen_wrap_i32(t6, t4, t5);

    // (t6 * REG_SIZE_IN_BYTES
    tcg_gen_muli_i32(ret, t6, REG_SIZE);

    tcg_temp_free_i32(t6);
    tcg_temp_free_i32(t5);
    tcg_temp_free_i32(t4);
    tcg_temp_free_i32(t3);
    tcg_temp_free_i32(t2);
    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
}

static inline void gen_breg_ptr(TCGv_ptr ret, int reg)
{
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_ptr t1 = tcg_temp_new_ptr();
    gen_breg_offset(t0, reg);
    tcg_gen_ext_i32_ptr(t1, t0);
    tcg_gen_add_ptr(ret, e2k_cpu.win_ptr, t1);
    tcg_temp_free_ptr(t1);
    tcg_temp_free_i32(t0);
}

static inline TCGv_i64 get_breg(DisasContext *dc, int reg)
{
    TCGv_i64 ret = get_temp(dc);
    TCGv_ptr ptr = tcg_temp_new_ptr();
    gen_breg_ptr(ptr, reg);
    tcg_gen_ld_i64(ret, ptr, 0);
    tcg_temp_free_ptr(ptr);
    return ret;
}

static inline void gen_store_breg(int reg, TCGv_i64 val)
{
    TCGv_ptr ptr = tcg_temp_new_ptr();
    gen_breg_ptr(ptr, reg);
    tcg_gen_st_i64(val, ptr, 0);
    tcg_temp_free_ptr(ptr);
}

static inline TCGv_i64 gen_load_greg(DisasContext *dc, int reg)
{
    // TODO: rotated gregs
    assert(reg < 32);
    return e2k_cpu.gregs[reg];
}

static inline void gen_store_greg(DisasContext *dc, int reg, TCGv_i64 val)
{
    // TODO: rotated gregs
    tcg_gen_mov_i64(e2k_cpu.gregs[reg], val);
}

static TCGv_i64 get_src1(DisasContext *dc, unsigned int als)
{
    unsigned int src1 = GET_FIELD(als, 16, 27);
    if (IS_BASED(src1)) {
        unsigned int i = GET_BASED(src1);
        return get_breg(dc, i);
    } else if (IS_REGULAR(src1)) {
        unsigned int i = GET_REGULAR(src1);
        return get_wreg(dc, i);
    } else if (IS_IMM5(src1)) {
        unsigned int imm = GET_IMM5(src1);
        TCGv_i64 t = get_temp_i64(dc);
        tcg_gen_movi_i64(t, imm);
        return t;
    } else {
        unsigned int i = GET_GLOBAL(src1);
        return gen_load_greg(dc, i);
    }
}

static TCGv_i64 get_src2(DisasContext *dc, unsigned int als)
{
    unsigned int src2 = GET_FIELD(als, 8, 15);
    if (IS_BASED(src2)) {
        unsigned int i = GET_BASED(src2);
        return get_breg(dc, i);
    } else if (IS_REGULAR(src2)) {
        unsigned int i = GET_REGULAR(src2);
        return get_wreg(dc, i);
    } else if (IS_IMM4(src2)) {
        unsigned int imm = GET_IMM4(src2);
        TCGv t = get_temp_i64(dc);
        tcg_gen_movi_i64(t, imm);
        return t;
    } else if (IS_LIT(src2)) {
        TCGv t = get_temp_i64(dc);
        unsigned int i = GET_LIT(src2);
        uint64_t lit = dc->bundle.lts[i];
        // TODO: exception
        assert(dc->bundle.lts_present[i]);
        if (IS_LIT16_LO(src2) && i < 2) {
            lit &= 0xffff;
        } else if (IS_LIT16_HI(src2) && i < 2) {
            lit >>= 16;
        } else if (IS_LIT32(src2)) {
            // nop
        } else if (IS_LIT64(src2) && i < 3) {
            // TODO: exception
            assert(dc->bundle.lts_present[i + 1]);
            lit |= ((uint64_t) dc->bundle.lts[i + 1]) << 32;
        } else {
            // TODO: exception
            abort();
        }
        tcg_gen_movi_i64(t, lit);
        return t;
    } else {
        unsigned int i = GET_GLOBAL(src2);
        return gen_load_greg(dc, i);
    }
}

static TCGv_i64 get_dst(DisasContext *dc, unsigned int als)
{
    unsigned int dst = als & 0xff;
    if (IS_BASED(dst)) {
        unsigned int i = GET_BASED(dst);
        return get_breg(dc, i);
    } else if (IS_REGULAR(dst)) {
        unsigned int i = GET_REGULAR(dst);
        return get_wreg(dc, i);
    } else if (IS_GLOBAL(dst)) {
        unsigned int i = GET_GLOBAL(dst);
        return gen_load_greg(dc, i);
    } else {
        // TODO: %empty, %ctpr, etc
        abort();
    }
}

static void gen_cs0(DisasContext *dc, CPUE2KState *env)
{
    typedef enum {
        NOTHING,
        IBRANCH,
        PREF,
        PUTTSD,
        DONE,
        HRET,
        GLAUNCH,
        DISP,
        SDISP,
        GETTSD,
        LDISP,
        RETURN
    } cs0_type;

    static cs0_type cs0_ops[4][4] =  {
        {IBRANCH, PREF, PUTTSD, DONE},
        {DISP, NOTHING, SDISP, GETTSD},
        {DISP, LDISP, SDISP, GETTSD},
        {DISP, NOTHING, SDISP, RETURN}
    };
    const UnpackedBundle *bundle = &dc->bundle;
    uint32_t cs0 = bundle->cs0;

    unsigned int ctpr = (cs0 & 0xc0000000) >> 30;
    unsigned int ctp_opc = (cs0 & 0x30000000) >> 28;
    unsigned int param_type = (cs0 & 0x00000007);
    cs0_type type = cs0_ops[ctpr][ctp_opc];

    if (type == RETURN && param_type == 1) {
        type = GETTSD;
    } else if (type == DONE) {
        if (param_type == 3) {
            type = HRET;
        } else if (param_type == 4) {
            type = GLAUNCH;
        }
    }

    if (type == IBRANCH || type == DONE || type == HRET || type == GLAUNCH) {
        /* IBRANCH, DONE, HRET and GLAUNCH are special because they require SS
           to be properly encoded.  */
        if (! bundle->ss_present
            /* SS.ctop should be equal to zero for IBRANCH, DONE, HRET and
               GLAUNCH (see C.17.1.1, note that they don't mention the latter two
               instructions there which is probably an omission ).  */
                || (bundle->ss & 0x00000c00))
        {
            // TODO: invalid
            abort();
        }
        /* Don't output either of the aforementioned instructions under "never"
           condition. Don't disassemble CS0 being a part of HCALL. Unlike ldis
           HCALL is currently disassembled on behalf of CS1.  */
        else if ((bundle->ss & 0x1ff)
              && !(bundle->cs1_present
              /* CS1.opc == CALL */
              && (bundle->cs1 & 0xf0000000) >> 28 == 5
              /* CS1.param.ctopc == HCALL  */
              && (bundle->cs1 & 0x380) >> 7 == 2))
        {
            unsigned int cond = bundle->ss & 0x1ff;
            if (type == IBRANCH) {
                /* C0F2 has `disp' field. In `C0F1' it's called `param'. Is this
                   the only difference between these two formats?  Funnily enough,
                   DONE is also C0F2 and thus has `disp', though it obviously
                   makes no sense for it.  */
                unsigned int disp = (cs0 & 0x0fffffff);
                /// Calculate a signed displacement in bytes.
                int sdisp = ((int) (disp << 4)) >> 1;
                target_ulong tgt = dc->pc + sdisp;
                TCGv dest = tcg_const_i64(tgt);
                // TODO: temporary, need to move in ctcond evaluate
                set_is_jmp();
                dc->jmp.dest = dest;
                dc->jmp.cond = cond;
            }
        }
    } else {
        /* Note that according to Table B.4.1 it's possible to obtain
    `      gettsd %ctpr{1,2} with an invalid value for CS0.param.type.  */
        if (type == GETTSD && param_type != 1) {
            // invalid
            abort();
        }

        if (type == DISP
            || type == SDISP
            || type == LDISP
            /* Note that RETURN is said to be COPF1. I can't understand what its
              `CS0.param' is needed for: all of the bits except the three
               lowermost ones are undefined, while the latter also known as "type"
               field should be filled in with zeroes.  */
            || type == RETURN
            /* GETTSD has as meaningless `CS0.param' as RETURN. The only
               difference is that its `CS0.param.type' should be equal to `1'. I
               wonder if I should check for that and output something like
               "invalid gettsd" if this turns out not to be the case . . .  */
            || type == GETTSD)
        {
            // ctpr
        }

        if (type == SDISP) {
//            my_printf (", 0x%x", cs0 & 0x1f);
        } else if (type == DISP
                    || type == LDISP
                    || type == PUTTSD)
        {
            unsigned int disp = (cs0 & 0x0fffffff);
            int sgnd_disp = ((int) (disp << 4)) >> 1;
            /* PUTTSD obviously doesn't take %ctpr{j} parameter. TODO: beware of
               an optional predicate which may control its execution which is
               encoded via `SS.ctcond.psrc' and `SS.ts_opc == PUTTSDC{P,N}' in
               case of `SS.type == 1' (see C.21.4). I wonder if `ct %ctpr<j>'
               encoded in `SS.ctop' under the same `SS.ctcond' takes an effect in
               such a case.  */
//            my_printf ("%s0x%llx", type == PUTTSD ? "" : ", ",
            /* FIXME: this way I ensure that it'll work correctly
               both on 32 and 64-bit hosts.  */
//                (unsigned long long) (instr_addr + sgnd_disp));
        }

        if (type == PREF) {
            unsigned int pdisp = (bundle->cs0 & 0x0ffffff0) >> 4;
            unsigned int ipd = (bundle->cs0 & 0x00000008) >> 3;
            unsigned int prefr = bundle->cs0 & 0x00000007;
        }
    }
}


static void gen_cs1(DisasContext *dc, CPUE2KState *env)
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

    const UnpackedBundle *bundle = &dc->bundle;
    unsigned int cs1 = bundle->cs1;
    unsigned int opc = (cs1 & 0xf0000000) >> 28;

    if (opc == SETR0 || opc == SETR1 || opc == SETBR) {
        unsigned int setbp = (cs1 & 0x08000000) >> 27;
        unsigned int setbn = (cs1 & 0x04000000) >> 26;

        /* Try to follow the same order of these instructions as in LDIS.
        Presumably `vfrpsz' should come first, while `setbp' should be placed
        between `setwd' and `setbn', but this is to be verified. I don't have
        a binary with these commands by hand right now.  */

        if (opc == SETR1) {
            if (! bundle->lts_present[0]) {
//                my_printf ("<bogus vfrpsz>");
            } else {
                /* Find out if VFRPSZ is always encoded together with SETWD. This
                seems to be the case even if no SETWD has been explicitly
                specified.  */
                unsigned int rpsz = (bundle->lts[0] & 0x0001f000) >> 12;
//                my_printf ("vfrpsz rpsz = 0x%x", rpsz);
            }
        }

        // FIXME: Should windowing registers be precomputed or not?

        if (opc == SETR0 || opc == SETR1) {
            if (! bundle->lts_present[0]) {
                // TODO: <bogus setwd>
                abort();
            } else {
                unsigned int lts0 = bundle->lts[0];
                unsigned int wsz = (lts0 & 0x00000fe0) >> 5;
                unsigned int nfx = (lts0 & 0x00000010) >> 4;

                tcg_gen_movi_i32(e2k_cpu.wsz, wsz * 2);
                tcg_gen_movi_i32(e2k_cpu.nfx, nfx);

                if (env->version >= 3) {
                    // DBL parameter of SETWD was added only starting from
                    // elbrus-v3.
                    unsigned int dbl = (lts0 & 0x00000008) >> 3;
                    tcg_gen_movi_i32(e2k_cpu.dbl, dbl);
                }
            }
        }

        if (setbn) {
            unsigned int rcur = (cs1 & 0x0003f000) >> 12;
            unsigned int rsz = (cs1 & 0x00000fc0) >> 6;
            unsigned int rbs = cs1 & 0x0000003f;

            tcg_gen_movi_i32(e2k_cpu.rcur, rcur * 2);
            tcg_gen_movi_i32(e2k_cpu.rsz, rsz * 2 + 2);
            tcg_gen_movi_i32(e2k_cpu.rbs, rbs * 2);
        }

        if (setbp) {
            unsigned int psz = (cs1 & 0x007c0000) >> 18;

            tcg_gen_movi_i32(e2k_cpu.psz, psz);
        }
    } else if (opc == SETEI) {
        /* Verify that CS1.param.sft = CS1.param[27] is equal to zero as required
        in C.14.3.  */
        unsigned int sft = (cs1 & 0x08000000) >> 27;
        unsigned int eir = (cs1 & 0x000000ff);

        if (sft) {
//            my_printf ("%s", mcpu >= 2 ? "setsft" : "unimp");
        } else {
//            my_printf ("setei 0x%x", eir);
        }
    } else if (opc == WAIT) {
        unsigned int ma_c = (cs1 & 0x00000020) >> 5;
        unsigned int fl_c = (cs1 & 0x00000010) >> 4;
        unsigned int ld_c = (cs1 & 0x00000008) >> 3;
        unsigned int st_c = (cs1 & 0x00000004) >> 2;
        unsigned int all_e = (cs1 & 0x00000002) >> 1;
        unsigned int all_c = cs1 & 0x00000001;

        if (env->version >= 5) {
            /* `sa{l,s}' fields are `elbrus-v5'-specific. Each of them makes sense
            only in the presence of `{ld,st}_c == 1' respectively.  */
            if (ld_c) {
                unsigned int sal = (cs1 & 0x00000100) >> 8;
//                my_printf ("sal = %d, ", sal);
            }

            if (st_c) {
                unsigned int sas = (cs1 & 0x00000080) >> 7;
//                my_printf ("sas = %d, ", sas);
            }
        }

        if (env->version >= 2) {
          /* `trap' field was introduced starting from `elbrus-v2'.  */
            unsigned int trap = (cs1 & 0x00000040) >> 6;
//          my_printf ("trap = %d, ", trap);
        }

//      my_printf ("ma_c = %d, fl_c = %d, ld_c = %d, st_c = %d, all_e = %d, "
//                 "all_c = %d", ma_c, fl_c, ld_c, st_c, all_e, all_c);
    } else if (opc == CALL) {
        unsigned int ctop = (bundle->ss & 0x00000c00) >> 10;
        /* In C.17.4 it's said that other bits in CS1.param except for the
           seven lowermost ones are ignored.  */
        unsigned int wbs = cs1 & 0x7f;

        if (ctop) {
//            my_printf ("call %%ctpr%d, wbs = 0x%x", ctop, wbs);
//            print_ctcond (info, instr->ss & 0x1ff);
        } else {
            unsigned int cs1_ctopc = (cs1 & 0x380) >> 7;
            /* CS1.param.ctpopc == HCALL. CS0 is required to encode HCALL.  */
            if (cs1_ctopc == 2 && bundle->cs0_present) {
                unsigned int cs0 = bundle->cs0;
                unsigned int cs0_opc = (cs0 & 0xf0000000) >> 28;
                /* CS0.opc == HCALL, which means
                CS0.opc.ctpr == CS0.opc.ctp_opc == 0 */
                if (cs0_opc == 0) {
                    unsigned int hdisp = (cs0 & 0x1e) >> 1;
//                    my_printf ("hcall 0x%x, wbs = 0x%x", hdisp, wbs);
//                    print_ctcond (info, instr->ss & 0x1ff);
                }
            } else {
//                my_printf ("<bogus call>");
            }
        }
    } else if (opc == MAS_OPC) {
        /* Note that LDIS doesn't print it out as a standalone instruction.  */
        unsigned int mas = cs1 & 0x0fffffff;

//        my_printf ("mas 0x%x", mas);
    } else if (opc == FLUSHR) {
        /* . . . these stupid engineers off! FLUSHR seems to be responsible for
           encoding both `flushr' and `flushc'. Moreover, according to their
           logic it should be possible to encode them simultaneously.  */

        /* Check for `CS1.param.flr'.  */
        if (cs1 & 0x00000001) {
//            my_printf ("flushr");
        }

        /* Check for `CS1.param.flc'.  */
        if (cs1 & 0x00000002) {
//          my_printf ("flushc");
        }
    } else if (opc == BG) {
        /* Hopefully, `vfbg' is the only instruction encoded by BG. I'm currently
           unable to find other ones in `iset-v5.single' at least . . .  */
        unsigned int chkm4 = (cs1 & 0x00010000) >> 16;
        unsigned int dmask = (cs1 & 0x0000ff00) >> 8;
        unsigned int umsk = cs1 & 0x000000ff;

        /* Print its fields in the order proposed in C.14.10.  */
//        my_printf ("vfbg umask = 0x%x, dmask = 0x%x, chkm4 = 0x%x",
//                   umsk, dmask, chkm4);
    } else {
//        my_printf ("unimp");
        abort();
    }
}

static void gen_op2_i32(TCGv_i64 ret, TCGv_i64 s1, TCGv_i64 s2, TCGv_i64 dst,
    void (*op)(TCGv_i32, TCGv_i32, TCGv_i32))
{
    TCGv_i32 lo1 = tcg_temp_new_i32();
    TCGv_i32 lo2 = tcg_temp_new_i32();
    TCGv_i32 dst_hi = tcg_temp_new_i32();
    TCGv_i32 tmp = tcg_temp_new_i32();

    tcg_gen_extrl_i64_i32(lo1, s1);
    tcg_gen_extrl_i64_i32(lo2, s2);
    tcg_gen_extrh_i64_i32(dst_hi, dst);
    (*op)(tmp, lo1, lo2);
    tcg_gen_concat_i32_i64(ret, tmp, dst_hi);

    tcg_temp_free_i32(tmp);
    tcg_temp_free_i32(dst_hi);
    tcg_temp_free_i32(lo2);
    tcg_temp_free_i32(lo1);
}

static void gen_op21_i32(TCGv_i64 ret, TCGv_i64 s1, TCGv_i64 s2, TCGv_i64 dst,
    void (*op1)(TCGv_i32, TCGv_i32, TCGv_i32),
    void (*op2)(TCGv_i32, TCGv_i32))
{
    TCGv_i32 lo1 = tcg_temp_new_i32();
    TCGv_i32 lo2 = tcg_temp_new_i32();
    TCGv_i32 dst_hi = tcg_temp_new_i32();
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i32 t1 = tcg_temp_new_i32();

    tcg_gen_extrl_i64_i32(lo1, s1);
    tcg_gen_extrl_i64_i32(lo2, s2);
    tcg_gen_extrh_i64_i32(dst_hi, dst);
    (*op1)(t0, lo1, lo2);
    (*op2)(t1, t0);
    tcg_gen_concat_i32_i64(ret, t1, dst_hi);

    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
    tcg_temp_free_i32(dst_hi);
    tcg_temp_free_i32(lo2);
    tcg_temp_free_i32(lo1);
}

static Result gen_alc(DisasContext *dc, CPUE2KState *env, int chan)
{
    const UnpackedBundle *bundle = &dc->bundle;
    unsigned int als = bundle->als[chan];
    int opc = (als >> 24) & 0x7f;
    int sm  = als >> 31;
    unsigned int dst = als & 0xff;
    bool is_cmp = false;
    Result res = { 0 };

    TCGv_i64 cpu_src1 = get_src1(dc, als);
    TCGv_i64 cpu_src2 = get_src2(dc, als);
    TCGv_i64 tmp_dst = get_temp_i64(dc);
    TCGv_i64 t64 = get_temp_i64(dc);

    switch(opc) {
    case 0x00: // ands
        gen_op2_i32(tmp_dst, cpu_src1, cpu_src2, get_dst(dc, als), tcg_gen_and_i32);
        break;
    case 0x01: // andd
        tcg_gen_and_i64(tmp_dst, cpu_src1, cpu_src2);
        break;
    case 0x02: // andns
        gen_op21_i32(tmp_dst, cpu_src1, cpu_src2, get_dst(dc, als), tcg_gen_add_i32, tcg_gen_not_i32);
        break;
    case 0x03: // andnd
        tcg_gen_and_i64(t64, cpu_src1, cpu_src2);
        tcg_gen_not_i64(tmp_dst, t64);
        break;
    case 0x04: // ors
        gen_op2_i32(tmp_dst, cpu_src1, cpu_src2, get_dst(dc, als), tcg_gen_or_i32);
        break;
    case 0x05: // ord
        tcg_gen_or_i64(tmp_dst, cpu_src1, cpu_src2);
        break;
    case 0x06: // orns
        gen_op21_i32(tmp_dst, cpu_src1, cpu_src2, get_dst(dc, als), tcg_gen_or_i32, tcg_gen_not_i32);
        break;
    case 0x07: // ornd
        tcg_gen_or_i64(t64, cpu_src1, cpu_src2);
        tcg_gen_not_i64(tmp_dst, t64);
        break;
    case 0x08: // xors
        gen_op2_i32(tmp_dst, cpu_src1, cpu_src2, get_dst(dc, als), tcg_gen_xor_i32);
        break;
    case 0x09: // xord
        tcg_gen_xor_i64(tmp_dst, cpu_src1, cpu_src2);
        break;
    case 0x0a: // xorns
        gen_op21_i32(tmp_dst, cpu_src1, cpu_src2, get_dst(dc, als), tcg_gen_xor_i32, tcg_gen_not_i32);
        break;
    case 0x0b: // xornd
        tcg_gen_xor_i64(t64, cpu_src1, cpu_src2);
        tcg_gen_not_i64(tmp_dst, t64);
        break;
    case 0x0c:
        // TODO: sxt
        abort();
        break;
    case 0x0e:
        // TODO: merges
        abort();
        break;
    case 0x0f:
        // TODO: merged
        abort();
        break;
    case 0x10: // adds
        gen_op2_i32(tmp_dst, cpu_src1, cpu_src2, get_dst(dc, als), tcg_gen_add_i32);
        break;
    case 0x11: // addd
        tcg_gen_add_i64(tmp_dst, cpu_src1, cpu_src2);
        break;
    case 0x12: // subs
        gen_op2_i32(tmp_dst, cpu_src1, cpu_src2, get_dst(dc, als), tcg_gen_sub_i32);
        break;
    case 0x13: // subd
        tcg_gen_sub_i64(tmp_dst, cpu_src1, cpu_src2);
        break;
    case 0x14: // scls
        gen_op2_i32(tmp_dst, cpu_src1, cpu_src2, get_dst(dc, als), tcg_gen_rotl_i32);
        break;
    case 0x15: // scld
        tcg_gen_rotl_i64(tmp_dst, cpu_src1, cpu_src2);
        break;
    case 0x16: // scrs
        gen_op2_i32(tmp_dst, cpu_src1, cpu_src2, get_dst(dc, als), tcg_gen_rotr_i32);
        break;
    case 0x17: // scrd
        tcg_gen_rotr_i64(tmp_dst, cpu_src1, cpu_src2);
        break;
    case 0x18: // shls
        gen_op2_i32(tmp_dst, cpu_src1, cpu_src2, get_dst(dc, als), tcg_gen_shl_i32);
        break;
    case 0x19: // shld
        tcg_gen_shl_i64(tmp_dst, cpu_src1, cpu_src2);
        break;
    case 0x1a: // shrs
        gen_op2_i32(tmp_dst, cpu_src1, cpu_src2, get_dst(dc, als), tcg_gen_shr_i32);
        break;
    case 0x1b: // shrd
        tcg_gen_shr_i64(tmp_dst, cpu_src1, cpu_src2);
        break;
    case 0x1c:
        gen_op2_i32(tmp_dst, cpu_src1, cpu_src2, get_dst(dc, als), tcg_gen_sar_i32);
        break;
    case 0x1d: // sard
        tcg_gen_sar_i64(tmp_dst, cpu_src1, cpu_src2);
        break;
    case 0x1e:
        // TODO: getfs
        abort();
        break;
    case 0x1f:
        // TODO: getfd
        abort();
        break;
    case 0x21: // cmp{op}sd
        do {
            is_cmp = true;
            unsigned int cmp_op = (als & 0xe0) >> 5;
//            unsigned int index = als & 0x1f;
            TCGv_i64 reg = get_temp_i64(dc);
            // TODO: move to separate function
            switch(cmp_op) {
            case 1: // unsigned less
                tcg_gen_setcond_i64(TCG_COND_LEU, tmp_dst, cpu_src1, cpu_src2);
                break;
            case 2: // equal
                tcg_gen_setcond_i64(TCG_COND_EQ, tmp_dst, cpu_src1, cpu_src2);
                break;
            default:
                abort();
                break;
            }
        } while(0);
        break;
    case 0x40: // TODO: udivs used as temporary UD
        gen_exception(dc, 1);
        break;
    default:
        qemu_log_mask(LOG_UNIMP, "gen_alc: undefined instruction 0x%x %s\n", opc, sm ? "(speculative)" : "");
        break;
    }

    if (is_cmp) {
        res.tag = RESULT_PREG;
        res.u.reg.i = als & 0x1f;
        res.u.reg.v = tmp_dst;
    } else {
        if (IS_BASED(dst)) {
            unsigned int i = GET_BASED(dst);
            res.tag = RESULT_BASED_REG;
            res.u.reg.i = i;
            res.u.reg.v = tmp_dst;
        } else if (IS_REGULAR(dst)) {
            unsigned int i = GET_REGULAR(dst);
            res.tag = RESULT_REGULAR_REG;
            res.u.reg.i = i;
            res.u.reg.v = tmp_dst;
        } else if (IS_GLOBAL(dst)) {
            unsigned int i = GET_GLOBAL(dst);
            res.tag = RESULT_GLOBAL_REG;
            res.u.reg.i = i;
            res.u.reg.v = tmp_dst;
        } else {
            abort();
        }
    }

    return res;
}

static void gen_jmp(DisasContext *dc, target_ulong next_pc)
{
    unsigned int ctcond = dc->jmp.cond;
    unsigned int cond_type = (ctcond & 0x1e0) >> 5;
    unsigned int psrc = (ctcond & 0x01f);
    bool not_preg = cond_type == 3 || cond_type == 7 || cond_type == 0xe;

    if (cond_type == 1) {
        dc->base.is_jmp = DISAS_NORETURN;
        tcg_gen_mov_tl(e2k_cpu.pc, dc->jmp.dest);
        tcg_gen_exit_tb(NULL, 0);
        return;
    }

    // TODO: temporary only preg cond
    if (cond_type == 2) {
        TCGv_i64 preg = get_preg(dc, psrc);
        TCGv_i64 cond = tcg_const_i64(1);
        TCGv next = tcg_const_tl(next_pc);
        dc->base.is_jmp = DISAS_NORETURN;
        tcg_gen_movcond_i64(TCG_COND_EQ, e2k_cpu.pc,
            preg, cond,
            dc->jmp.dest, next
        );
        tcg_gen_exit_tb(NULL, 0);
        tcg_temp_free(cond);
    }

    /* These types of conditions involve a (probably negated) predicate
       register. */
    if (cond_type == 2
        || cond_type == 3
        || cond_type == 6
        || cond_type == 7
        || cond_type == 0xe
        || cond_type == 0xf)
    {
        // preg
    }

    if (cond_type == 4
        || cond_type == 6
        || cond_type == 0xe)
    {
        if (cond_type == 6 || cond_type == 0xe) {
            // or
        }

        // %LOOP_END
    }

    if (cond_type == 5
        || cond_type == 7
        || cond_type == 0xf)
    {
        if(cond_type == 7
            || cond_type == 0xf)
        {
            // AND
        }
        // %NOT_LOOP_END
    }

    if (cond_type == 8) {
        // %MLOCK
        /* It's not clearly said in C.17.1.2 of iset-vX.single if the uppermost
           fourth bit in `psrc' has any meaning at all.  */
        if (psrc & 0xf) {
            static const int conv[] = {0, 1, 3, 4};
            int i;

            // %dt_al
            for (i = 0; i < 4; i++) {
                if (psrc & (1 << i)) {
                    // i
                }
            }
        }
    }

    /* `lock_cond || pl_cond' control transfer conditions.  */
    if (cond_type == 9) {
        unsigned int type = (psrc & 0x18) >> 3;
        if (type == 0) {
            static const int cmp_num_to_alc[] = {0, 1, 3, 4};
            unsigned int cmp_num = (psrc & 0x6) >> 1;
            unsigned int neg = psrc & 0x1;

//            my_printf ("%%MLOCK || %s%%cmp%d", neg ? "~" : "",
//                cmp_num_to_alc[cmp_num]);
        } else if (type == 1) {
            unsigned int cmp_jk = (psrc & 0x4) >> 2;
            unsigned int negj = (psrc & 0x2) >> 1;
            unsigned int negk = psrc & 0x1;

//            my_printf ("%%MLOCK || %s%%cmp%d || %s%%cmp%d",
//                     negj ? "~" : "", cmp_jk == 0 ? 0 : 3,
//                     negk ? "~" : "", cmp_jk == 0 ? 1 : 4);
        } else if (type == 2) {
            unsigned int clp_num = (psrc & 0x6) >> 1;
            unsigned int neg = psrc & 0x1;

            // "%%MLOCK || %s%%clp%d", neg ? "~" : "", clp_num
        }
    }

    if (cond_type >= 0xa && cond_type <= 0xd) {
        // reserved condition type
        qemu_log_mask(LOG_UNIMP, "Undefined control transfer type %#x\n", cond_type);
        abort();
    }
}

static target_ulong disas_e2k_insn(DisasContext *dc, CPUState *cs)
{
    E2KCPU *cpu = E2K_CPU(cs);
    CPUE2KState *env = &cpu->env;
    UnpackedBundle *bundle = &dc->bundle;
    unsigned int i;
    unsigned int bundle_len = unpack_bundle(env, dc->pc, bundle);
    /* TODO: exception, check bundle_len */
    target_ulong pc_next = dc->pc + bundle_len;

    reset_is_jmp();

    if (bundle->cs0_present) {
        gen_cs0(dc, env);
    }

    if (bundle->cs1_present) {
        gen_cs1(dc, env);
    }

    for (i = 0; i < 6; i++) {
        if (!bundle->als_present[i]) {
            continue;
        }
        dc->alc[i] = gen_alc(dc, env, i);
    }

    // Commit results after all instructions in the bundle was executed
    for (i = 0; i < 6; i++) {
        Result *res = &dc->alc[i];
        if (!bundle->als_present[i]) {
            continue;
        }
        switch(res->tag) {
        case RESULT_BASED_REG:
            gen_store_breg(res->u.reg.i, res->u.reg.v);
            break;
        case RESULT_REGULAR_REG:
            gen_store_wreg(res->u.reg.i, res->u.reg.v);
            break;
        case RESULT_GLOBAL_REG:
            gen_store_greg(dc, res->u.reg.i, res->u.reg.v);
            break;
        case RESULT_PREG:
            gen_store_preg(res->u.reg.i, res->u.reg.v);
            break;
        default:
            break;
        }
    }

    // Change windowing registers after commit is done.
    unsigned int ss = dc->bundle.ss;
    unsigned int vfdi = (ss & 0x04000000) >> 26;
    unsigned int abg = (ss & 0x01800000) >> 23;
    unsigned int abn = (ss & 0x00600000) >> 21;
    unsigned int abp = (ss & 0x000c0000) >> 18;
    unsigned int alc = (ss & 0x00030000) >> 16;

    // FIXME: not working in cond branches
    // Change windowing registers
    if (dc->jmp.cond == COND_NEVER) {
        TCGv_i32 t0 = tcg_temp_new_i32();
        TCGv_i32 t1 = tcg_temp_new_i32();
        TCGv_i32 zero = tcg_const_i32(0);

        tcg_gen_addi_i32(t0, e2k_cpu.rcur, 2);
        gen_wrap_i32(t1, t0, e2k_cpu.rsz);
        tcg_gen_movcond_i32(TCG_COND_EQ, e2k_cpu.rcur,
            e2k_cpu.is_jmp, zero,
            t1, e2k_cpu.rcur);

        tcg_temp_free_i32(zero);
        tcg_temp_free_i32(t1);
        tcg_temp_free_i32(t0);
    }
    // TODO: move based if branch

    // Control transfer
    if (env->interrupt_index != 0) {
        tcg_gen_movi_tl(e2k_cpu.pc, dc->pc);
        tcg_gen_exit_tb(NULL, 0);
        dc->base.is_jmp = DISAS_NORETURN;
    } else if (dc->jmp.cond != 0) {
        // TODO: move condition compute before commit
        gen_jmp(dc, pc_next);
    }

    // Free temporary values.
    while(dc->t32_len) {
        tcg_temp_free_i32(dc->t32[--dc->t32_len]);
    }

    while(dc->t64_len) {
        tcg_temp_free_i64(dc->t64[--dc->t64_len]);
    }

    while(dc->ttl_len) {
        tcg_temp_free(dc->ttl[--dc->ttl_len]);
    }

    return pc_next;
}

static void e2k_tr_init_disas_context(DisasContextBase *db, CPUState *cs)
{
    DisasContext *dc = container_of(db, DisasContext, base);

    dc->pc = dc->base.pc_first;
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
    dc->pc = dc->base.pc_next;
    dc->base.pc_next = disas_e2k_insn(dc, cs);
}

static void e2k_tr_tb_start(DisasContextBase *db, CPUState *cs)
{
    DisasContext *dc = container_of(db, DisasContext, base);
    E2KCPU *cpu = E2K_CPU(cs);
    CPUE2KState *env = &cpu->env;

    dc->jmp.cond = COND_NEVER;
    dc->jmp.dest = NULL;
}

static void e2k_tr_tb_stop(DisasContextBase *db, CPUState *cs)
{
    DisasContext *dc = container_of(db, DisasContext, base);
    E2KCPU *cpu = E2K_CPU(cs);
    CPUE2KState *env = &cpu->env;
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
        { &e2k_cpu.wbs, offsetof(CPUE2KState, wbs), "wbs" },
        { &e2k_cpu.wsz, offsetof(CPUE2KState, wsz), "wsz" },
        { &e2k_cpu.nfx, offsetof(CPUE2KState, nfx), "nfx" },
        { &e2k_cpu.dbl, offsetof(CPUE2KState, dbl), "dbl" },
        { &e2k_cpu.rbs, offsetof(CPUE2KState, rbs), "rbs" },
        { &e2k_cpu.rsz, offsetof(CPUE2KState, rsz), "rsz" },
        { &e2k_cpu.rcur, offsetof(CPUE2KState, rcur), "rcur" },
        { &e2k_cpu.psz, offsetof(CPUE2KState, psz), "psz" },
    };

    static const struct { TCGv_i64 *ptr; int off; const char *name; } r64[] = {
        { &e2k_cpu.pregs, offsetof(CPUE2KState, pregs), "pregs" },
    };

    static const struct { TCGv *ptr; int off; const char *name; } rtl[] = {
        { &e2k_cpu.pc, offsetof(CPUE2KState, ip), "pc" },
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

    e2k_cpu.win_ptr = tcg_global_mem_new_ptr(cpu_env, offsetof(CPUE2KState, win_ptr), "win_ptr");
    e2k_cpu.is_jmp = tcg_global_mem_new_i32(cpu_env, offsetof(CPUE2KState, is_jmp), "is_jmp");

    for (i = 0; i < WREGS_SIZE; i++) {
        snprintf(buf, ARRAY_SIZE(buf), "%%r%d", i);
        e2k_cpu.wregs[i] = tcg_global_mem_new(e2k_cpu.win_ptr,
            i * REG_SIZE,
            buf);
    }

    for (i = 0; i < 32; i++) {
        snprintf(buf, ARRAY_SIZE(buf), "%%g%d", i);
        e2k_cpu.gregs[i] = tcg_global_mem_new(cpu_env,
            offsetof(CPUE2KState, gregs[i]),
            buf);
    }

    for (i = 0; i < 3; i++) {
        snprintf(buf, ARRAY_SIZE(buf), "%%ctpr%d", i + 1);
        e2k_cpu.ctprs[i] = tcg_global_mem_new(cpu_env,
            offsetof(CPUE2KState, ctprs[i]),
            buf
        );
    }
}
