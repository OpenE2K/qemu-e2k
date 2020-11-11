#include "qemu/osdep.h"
#include "qemu.h"
#include "disas/disas.h"
#include "exec/translator.h"
#include "tcg/tcg-op.h"
#include "exec/log.h"

#define TEMP_COUNT_32 16
#define TEMP_COUNT_64 16
#define TEMP_COUNT_TL 8
#define COND_NEVER 0
#define COND_ALWAYS 1

static TCGv_ptr cpu_win_ptr;
static TCGv_i64 cpu_wregs[WREGS_SIZE];
static TCGv_i64 cpu_gregs[32];
static TCGv cpu_pc;
static TCGv_i64 cpu_pregs;
static TCGv cpu_ctprs[3];
static TCGv_i32 cpu_wbs;
static TCGv_i32 cpu_wsz;
static TCGv_i32 cpu_nfx;
static TCGv_i32 cpu_dbl;
static TCGv_i32 cpu_rbs;
static TCGv_i32 cpu_rsz;
static TCGv_i32 cpu_rcur;
static TCGv_i32 cpu_psz;

enum ResultType {
    RESULT_NONE,
    RESULT_BASED_REG,
    RESULT_REGULAR_REG,
    RESULT_GLOBAL_REG,
    RESULT_PREG,
};

typedef struct {
    enum ResultType tag;
    union {
        struct {
            unsigned int i;
            TCGv_i64 v;
        } reg;
    } u;
} Result;

static struct unpacked_instr {
  unsigned int hs;
  unsigned int ss;
  unsigned int als[6];
  unsigned int cs0;
  unsigned short ales[6];
  unsigned int cs1;
  unsigned short aas[6];
  unsigned short half_gap;

  /* It should be impossible to have more than 16 words of GAP
     in principle.  */
  unsigned int gap[16];

  unsigned int lts[4];
  unsigned int pls[3];
  unsigned int cds[3];

  unsigned char ss_present;
  unsigned char als_present[6];
  unsigned char cs0_present;
  unsigned char ales_present[6];
  unsigned char cs1_present;
  unsigned char aas_present[6];
  unsigned char half_gap_present;
  unsigned char gap_present[16];
  unsigned char lts_present[4];
  unsigned char pls_present[3];
  unsigned char cds_present[3];

  unsigned int api_l[2];
  unsigned int api_r[2];
} unpacked_instr;

typedef struct {
    DisasContextBase base;
    struct unpacked_instr instr;
    target_ulong pc;

    // FIXME: I do not know if it is right place to store these values.
    unsigned int wbs;
    unsigned int wsz;
    unsigned int nfx;
    unsigned int dbl;
    unsigned int rbs;
    unsigned int rsz;
    unsigned int rcur;
    unsigned int psz;

    // Temporary values.
    TCGv_i32 t32[TEMP_COUNT_32];
    TCGv_i64 t64[TEMP_COUNT_64];
    TCGv ttl[TEMP_COUNT_TL];
    // Allocated temporary values count.
    int t32_len;
    int t64_len;
    int ttl_len;

    struct {
        // raw condition code from SS[8:0]
        unsigned int cond;
        TCGv dest;
    } jmp;
    Result alc[6];
} DisasContext;

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

// TODO: return error on invalid instruction
static target_ulong unpack_instr(CPUE2KState *env, target_ulong pc,
                                 struct unpacked_instr *instr)
{
  unsigned int i;
  target_ulong gap, pos = pc, next_pc = pos + 8;
  int hsyll_cntr = 0;
  unsigned int hs;
  unsigned int mdl;
  memset (instr, 0, sizeof (unpacked_instr));

  hs = translator_ldl(env, pos);
  pos += 4;
  instr->hs = hs;
  next_pc += ((hs & 0x70) >> 4) * 8;

  /* Check for SS.  */
  if (hs & (0x1 << 12))
    {
      instr->ss_present = 1;
      instr->ss = translator_ldl(env, pos);
      pos += 4;
    }

  /* Check for available ALS syllables.  */
  for (i = 0; i < 6; i++)
    {
      if (hs & (1 << (26 + i)))
        {
          instr->als_present[i] = 1;
          instr->als[i] = translator_ldl(env, pos);
          pos += 4;
        }
    }

  /* Check for CS0.  */
  if (hs & (0x1 << 14))
  {
    instr->cs0_present = 1;
    instr->cs0 = translator_ldl(env, pos);
    pos += 4;
  }

  /* If either `ALES5' or `ALES2' has been marked as present in HS, set its
     value to default to properly account for the case when it's not allocated.
     `ALES_PRESENT[{2,5}]' are treated this way in the code below: 0 means that
     the syllable has been neither marked in HS, nor allocated; 1 - marked in
     HS, but not allocated; 2 - not marked in HS, but allocated; 3 - both marked
     in HS and allocated.  */
  if (hs & (0x1 << 25))
    {
      instr->ales_present[5] = 1;
      instr->ales[5] = 0x01c0;
    }

  if (hs & (0x1 << 22))
    {
      instr->ales_present[2] = 1;
      instr->ales[2] = 0x01c0;
    }


  /* Calculate the size of f1 fragment in bytes. For a valid instruction it
     should be equal to either of `pos', `pos + 4' or `pos + 8'. What should I
     do if it's not?  */
  mdl = pc + ((hs & 0xf) + 1) * 4;

  /* The following condition means that ALES{2,5} are physically present within
     the wide instruction. However, they should be probably taken into account
     only if HS.ale{2,5} are set. Should I disassemble them if these bits are
     not set but the syllables physically exist?  */
  if (((hs & (0x1 << 15)) && mdl == pos + 8)
      || (!(hs & (0x1 << 15)) && mdl == pos + 4))
    {
      /* Fill in ALES5 and ALES2 syllables even if none of them is specified in
         HS as present. This will let me output this syllable into disassembly
         whichever case takes place. */
      instr->ales[5] = translator_lduw(env, pos);
      instr->ales[2] = translator_lduw(env, pos + 2);

      /* Adjust `ALES_PRESENT[{5,2}]' as proposed above now that we know that
         they are allocated.  */
      instr->ales_present[5] |= 0x2;
      instr->ales_present[2] |= 0x2;

      pos += 4;
    }

  /* Check for CS1.  */
  if (hs & (0x1 << 15))
    {
      instr->cs1_present = 1;
      instr->cs1 = translator_ldl(env, pos);
      pos += 4;
    }

  /* A primitive control just for a moment.  */
  if (mdl != pos)
    {
    /* This is either an APB instruction or an invalid one. Let's stupidly
       believe that the former takes place and signalize our caller about
       that by returning 0.  */

      return next_pc;
    }


  /* Check for ALES{0,1,3,4}.  */
  for (i = 0; i < 5; i++)
    {
      if (i == 2)
        continue;

      if (hs & (0x1 << (20 + i)))
        {
          instr->ales_present[i] = 1;

          /* Recall the idiotic order of half-syllables in the packed wide
             instruction.  */
          instr->ales[i] = translator_lduw(env,
              pos + 2 * ((hsyll_cntr & ~0x1) + 1 - (hsyll_cntr & 0x1)));
          hsyll_cntr++;
        }
    }

  /* Check for AASj half-syllables. To encode them SS syllable of SF1 type
     should be present.  */
  if (instr->ss_present && (instr->ss & (0x1 << 20)) == 0)
    {
      for (i = 0; i < 4; i++)
        {
          if (instr->ss & (0x1 << (12 + i)))
            {
              instr->aas_present[i >> 1] = 1;
              instr->aas_present[2 + i] = 1;
            }
        }

      for (i = 0; i < 6; i++)
        {
          if (instr->aas_present[i])
            {
              /* Recall the idiotic order of half-syllables in the packed wide
                 instruction. Note that the first AAS half-syllable may share a
                 syllable with the last ALES.  */
              instr->aas[i] = translator_lduw(env,
                  pos + 2 * ((hsyll_cntr & ~0x1) + 1 - (hsyll_cntr & 0x1)));
              hsyll_cntr++;
            }
        }
    }

  if (hsyll_cntr & 0x1)
    {
      /* Simplify the calculation of offset in BUF[] a bit by taking the above
         condition into account.  */
      instr->half_gap = translator_lduw(env, pos + 2 * (hsyll_cntr & ~0x1));
      instr->half_gap_present = 1;

      /* Ensure that hsyll_cntr is even. This is implied when calculating GAP
         below.  */
      hsyll_cntr++;
    }

  /* Calculate the next 32-bit syllable's position. It may be the uppermost LTS
     syllable. Note that I don't consider the case when LTS syllables reuse the
     values encoded in the preceding ones, though according to `iset-v5.single'
     this is quite legal. GAS doesn't produce such a code. Hopefully neither LAS
     has ever done that . . .  */
  gap = pos + 2 * hsyll_cntr;

  /* Set POS to point to the last syllable in the current wide instruction and
     extract CDSj and PLSj syllables if any.  */
  pos = pc + ((((hs & 0x70) >> 4) + 1) << 3) - 4;

  /* Check for CDSj syllables.  */
  for (i = 0; i < ((hs & 0x30000) >> 16); i++)
    {
      instr->cds_present[i] = 1;
      instr->cds[i] = translator_ldl(env, pos);
      pos -= 4;
    }


  /* Check for PLSj syllables.  */
  for (i = 0; i < ((hs & 0xc0000) >> 18); i++)
    {
      instr->pls_present[i] = 1;
      instr->pls[i] = translator_ldl(env, pos);
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
  if (pos < gap && pos != gap - 4)
    return next_pc;

  /* Extract available LTSj syllables.  */
  for (i = 0; i < 4 && pos >= gap; i++)
    {
      instr->lts_present[i] = 1;
      instr->lts[i] = translator_ldl(env, pos);
      pos -= 4;
    }

  /* It makes sense to enumerate GAP syllables in a normal order unlike LTS
     ones.  */
  for (i = 0; i < 16 && gap <= pos; i++)
    {
      instr->gap_present[i] = 1;
      instr->gap[i] = translator_ldl(env, gap);
      gap += 4;
    }


  return next_pc;
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
    tcg_gen_movi_tl(cpu_pc, dc->pc);
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

static inline TCGv_i64 get_preg(DisasContext *dc, int reg)
{
    TCGv_i64 ret = get_temp(dc);
    TCGv_i64 tmp = tcg_temp_new_i64();
    assert(reg < 32);
    tcg_gen_shri_i64(tmp, cpu_pregs, reg * 2);
    // TODO: should return preg tag?
    tcg_gen_andi_i64(ret, tmp, 0x01);
    tcg_temp_free_i64(tmp);
    return ret;
}

static inline void set_preg(DisasContext *dc, int reg, TCGv_i64 val)
{
    TCGv_i64 t0 = tcg_temp_new_i64();
    TCGv_i64 t1 = tcg_temp_new_i64();
    TCGv_i64 t2 = tcg_temp_new_i64();
    tcg_gen_andi_i64(t0, cpu_pregs, ~(3 << (reg * 2)));
    tcg_gen_andi_i64(t1, val, 0x03);
    tcg_gen_shli_i64(t2, t1, reg * 2);
    tcg_gen_or_i64(cpu_pregs, t0, t2);
    tcg_temp_free_i64(t2);
    tcg_temp_free_i64(t1);
    tcg_temp_free_i64(t0);
}

static inline unsigned int wreg_index(DisasContext *dc, int reg)
{
    assert(reg < 64);
    // TODO: exception
    assert(reg < (dc->wsz * 2));
    return (reg + dc->wbs * 2) % WREGS_SIZE;
}

static inline TCGv_i64 gen_load_wreg(DisasContext *dc, int reg)
{
    TCGv_i64 tmp = get_temp(dc);
    tcg_gen_ld_i64(tmp, cpu_win_ptr, reg * sizeof(target_ulong));
    return tmp;
}

static inline void gen_store_wreg(DisasContext *dc, int reg, TCGv_i64 val)
{
    unsigned int i = wreg_index(dc, reg);
    tcg_gen_st_i64(val, cpu_win_ptr, i * sizeof(target_ulong));
}

static inline unsigned int breg_index(DisasContext *dc, int reg)
{
    assert(reg < 128);
    int rsz = dc->rsz * 2 + 2;
    // TODO: exception
    assert(reg < (dc->wsz * 2) && reg < rsz);
    int i = (reg + (dc->rsz + 1 - dc->rcur) * 2) % rsz;
    return (i + dc->wbs * 2 + dc->rbs * 2) % WREGS_SIZE;
}

static inline TCGv_i64 gen_load_breg(DisasContext *dc, int reg)
{
    TCGv_i64 tmp = get_temp(dc);
    unsigned int i = breg_index(dc, reg);
    tcg_gen_ld_i64(tmp, cpu_win_ptr, i * sizeof(target_ulong));
    return tmp;
}

static inline void gen_store_breg(DisasContext *dc, int reg, TCGv_i64 val)
{
    unsigned int i = breg_index(dc, reg);
    tcg_gen_st_i64(val, cpu_win_ptr, i * sizeof(target_ulong));
}

static inline TCGv_i64 gen_load_greg(DisasContext *dc, int reg)
{
    // TODO: rotated gregs
    assert(reg < 32);
    return cpu_gregs[reg];
}

static inline void gen_store_greg(DisasContext *dc, int reg, TCGv_i64 val)
{
    // TODO: rotated gregs
    tcg_gen_mov_i64(cpu_gregs[reg], val);
}

static TCGv_i64 get_src1(DisasContext *dc, unsigned int als)
{
    unsigned int src1 = GET_FIELD(als, 16, 27);
    if (IS_BASED(src1)) {
        unsigned int i = GET_BASED(src1);
        return gen_load_breg(dc, i);
    } else if (IS_REGULAR(src1)) {
        unsigned int i = GET_REGULAR(src1);
        return gen_load_wreg(dc, i);
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
        return gen_load_breg(dc, i);
    } else if (IS_REGULAR(src2)) {
        unsigned int i = GET_REGULAR(src2);
        return gen_load_wreg(dc, i);
    } else if (IS_IMM4(src2)) {
        unsigned int imm = GET_IMM4(src2);
        TCGv t = get_temp_i64(dc);
        tcg_gen_movi_i64(t, imm);
        return t;
    } else if (IS_LIT(src2)) {
        TCGv t = get_temp_i64(dc);
        unsigned int i = GET_LIT(src2);
        uint64_t lit = dc->instr.lts[i];
        // TODO: exception
        assert(dc->instr.lts_present[i]);
        if (IS_LIT16_LO(src2) && i < 2) {
            lit &= 0xffff;
        } else if (IS_LIT16_HI(src2) && i < 2) {
            lit >>= 16;
        } else if (IS_LIT32(src2)) {
            // nop
        } else if (IS_LIT64(src2) && i < 3) {
            // TODO: exception
            assert(dc->instr.lts_present[i + 1]);
            lit |= ((uint64_t) dc->instr.lts[i + 1]) << 32;
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
        return gen_load_breg(dc, i);
    } else if (IS_REGULAR(dst)) {
        unsigned int i = GET_REGULAR(dst);
        return gen_load_wreg(dc, i);
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
    const struct unpacked_instr *instr = &dc->instr;
    unsigned int cs0 = instr->cs0;

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
        if (! instr->ss_present
            /* SS.ctop should be equal to zero for IBRANCH, DONE, HRET and
               GLAUNCH (see C.17.1.1, note that they don't mention the latter two
               instructions there which is probably an omission ).  */
                || (instr->ss & 0x00000c00))
        {
            // TODO: invalid
            abort();
        }
        /* Don't output either of the aforementioned instructions under "never"
           condition. Don't disassemble CS0 being a part of HCALL. Unlike ldis
           HCALL is currently disassembled on behalf of CS1.  */
        else if ((instr->ss & 0x1ff)
              && !(instr->cs1_present
              /* CS1.opc == CALL */
              && (instr->cs1 & 0xf0000000) >> 28 == 5
              /* CS1.param.ctopc == HCALL  */
              && (instr->cs1 & 0x380) >> 7 == 2))
        {
            unsigned int cond = instr->ss & 0x1ff;
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
            unsigned int pdisp = (instr->cs0 & 0x0ffffff0) >> 4;
            unsigned int ipd = (instr->cs0 & 0x00000008) >> 3;
            unsigned int prefr = instr->cs0 & 0x00000007;
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

    const struct unpacked_instr *instr = &dc->instr;
    unsigned int cs1 = instr->cs1;
    unsigned int opc = (cs1 & 0xf0000000) >> 28;

    if (opc == SETR0 || opc == SETR1 || opc == SETBR) {
        unsigned int setbp = (cs1 & 0x08000000) >> 27;
        unsigned int setbn = (cs1 & 0x04000000) >> 26;

        /* Try to follow the same order of these instructions as in LDIS.
        Presumably `vfrpsz' should come first, while `setbp' should be placed
        between `setwd' and `setbn', but this is to be verified. I don't have
        a binary with these commands by hand right now.  */

        if (opc == SETR1) {
            if (! instr->lts_present[0]) {
//                my_printf ("<bogus vfrpsz>");
            } else {
                /* Find out if VFRPSZ is always encoded together with SETWD. This
                seems to be the case even if no SETWD has been explicitly
                specified.  */
                unsigned int rpsz = (instr->lts[0] & 0x0001f000) >> 12;
//                my_printf ("vfrpsz rpsz = 0x%x", rpsz);
            }
        }

        if (opc == SETR0 || opc == SETR1) {
            if (! instr->lts_present[0]) {
                // TODO: <bogus setwd>
                abort();
            } else {
                unsigned int lts0 = instr->lts[0];
                dc->wsz = (lts0 & 0x00000fe0) >> 5;
                dc->nfx = (lts0 & 0x00000010) >> 4;

                if (env->version >= 3) {
                    // DBL parameter of SETWD was added only starting from
                    // elbrus-v3.
                    dc->dbl = (lts0 & 0x00000008) >> 3;
                }
            }
        }

        if (setbn) {
            unsigned int rcur = (cs1 & 0x0003f000) >> 12;
            unsigned int rsz = (cs1 & 0x00000fc0) >> 6;
            unsigned int rbs = cs1 & 0x0000003f;
            dc->rcur = rcur;
            dc->rsz = rsz;
            dc->rbs = rbs;
        }

        if (setbp) {
            dc->psz = (cs1 & 0x007c0000) >> 18;
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
        unsigned int ctop = (instr->ss & 0x00000c00) >> 10;
        /* In C.17.4 it's said that other bits in CS1.param except for the
           seven lowermost ones are ignored.  */
        unsigned int wbs = cs1 & 0x7f;

        if (ctop) {
//            my_printf ("call %%ctpr%d, wbs = 0x%x", ctop, wbs);
//            print_ctcond (info, instr->ss & 0x1ff);
        } else {
            unsigned int cs1_ctopc = (cs1 & 0x380) >> 7;
            /* CS1.param.ctpopc == HCALL. CS0 is required to encode HCALL.  */
            if (cs1_ctopc == 2 && instr->cs0_present) {
                unsigned int cs0 = instr->cs0;
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
    const struct unpacked_instr *instr = &dc->instr;
    unsigned int als = instr->als[chan];
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
        tcg_gen_mov_tl(cpu_pc, dc->jmp.dest);
        tcg_gen_exit_tb(NULL, 0);
        return;
    }

    // TODO: temporary only preg cond
    if (cond_type == 2) {
        TCGv_i64 preg = get_preg(dc, psrc);
        TCGv next = tcg_const_tl(next_pc);
        dc->base.is_jmp = DISAS_NORETURN;
        tcg_gen_movcond_i64(TCG_COND_EQ, cpu_pc,
            preg, tcg_const_i64(1),
            dc->jmp.dest, next
        );
        tcg_gen_exit_tb(NULL, 0);
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
    struct unpacked_instr *instr = &dc->instr;
    unsigned int i;
    target_ulong pc_next = unpack_instr(env, dc->pc, instr);

    if (dc->instr.cs0_present) {
        gen_cs0(dc, env);
    }

    if (dc->instr.cs1_present) {
        gen_cs1(dc, env);
    }

    for (i = 0; i < 6; i++) {
        if (!instr->als_present[i]) {
            continue;
        }
        dc->alc[i] = gen_alc(dc, env, i);
    }

    // Control transfer
    if (env->interrupt_index != 0) {
        tcg_gen_movi_tl(cpu_pc, dc->pc);
        tcg_gen_exit_tb(NULL, 0);
        dc->base.is_jmp = DISAS_NORETURN;
    } else if (dc->jmp.cond != 0) {
        gen_jmp(dc, pc_next);
    }

    // Commit results after all instructions in the bundle was executed
    for (i = 0; i < 6; i++) {
        Result *res = &dc->alc[i];
        if (!instr->als_present[i]) {
            continue;
        }
        switch(res->tag) {
        case RESULT_BASED_REG:
            gen_store_breg(dc, res->u.reg.i, res->u.reg.v);
            break;
        case RESULT_REGULAR_REG:
            gen_store_wreg(dc, res->u.reg.i, res->u.reg.v);
            break;
        case RESULT_GLOBAL_REG:
            gen_store_greg(dc, res->u.reg.i, res->u.reg.v);
            break;
        case RESULT_PREG:
            set_preg(dc, res->u.reg.i, res->u.reg.v);
            break;
        default:
            break;
        }
    }

    // Change windowing registers after commit is done.
    unsigned int ss = dc->instr.ss;
    unsigned int vfdi = (ss & 0x04000000) >> 26;
    unsigned int abg = (ss & 0x01800000) >> 23;
    unsigned int abn = (ss & 0x00600000) >> 21;
    unsigned int abp = (ss & 0x000c0000) >> 18;
    unsigned int alc = (ss & 0x00030000) >> 16;

    // Change windowing registers
    if (dc->jmp.cond == COND_NEVER) {
        // move based if not branch
        if (abn >> 1 != 0) {
            dc->rcur = (dc->rcur + 1) % (dc->rsz + 1);
        }
    // TODO: handle any branch
    } else if (dc->jmp.cond != COND_NEVER) {
        // move based if branch
        if (abn & 0x1 != 0) {
            dc->rcur = (dc->rcur + 1) % (dc->rsz + 1);
        }
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
    target_ulong pc_next = disas_e2k_insn(dc, cs);
    dc->base.pc_next = pc_next;
}

static void e2k_tr_tb_start(DisasContextBase *db, CPUState *cs)
{
    DisasContext *dc = container_of(db, DisasContext, base);
    E2KCPU *cpu = E2K_CPU(cs);
    CPUE2KState *env = &cpu->env;

    dc->jmp.cond = TCG_COND_NEVER;
    dc->jmp.dest = NULL;

    // restore window state
    dc->wbs = env->wbs;
    dc->wsz = env->wsz;
    dc->nfx = env->nfx;
    dc->dbl = env->dbl;
    dc->rbs = env->rbs;
    dc->rsz = env->rsz;
    dc->rcur = env->rcur;
    dc->psz = env->psz;
}

static void e2k_tr_tb_stop(DisasContextBase *db, CPUState *cs)
{
    DisasContext *dc = container_of(db, DisasContext, base);
    E2KCPU *cpu = E2K_CPU(cs);
    CPUE2KState *env = &cpu->env;

    // save window state
    env->wbs = dc->wbs;
    env->wsz = dc->wsz;
    env->nfx = dc->nfx;
    env->dbl = dc->dbl;
    env->rbs = dc->rbs;
    env->rsz = dc->rsz;
    env->rcur = dc->rcur;
    env->psz = dc->psz;
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
        { &cpu_wbs, offsetof(CPUE2KState, wbs), "wbs" },
        { &cpu_wsz, offsetof(CPUE2KState, wsz), "wsz" },
        { &cpu_nfx, offsetof(CPUE2KState, nfx), "nfx" },
        { &cpu_dbl, offsetof(CPUE2KState, dbl), "dbl" },
        { &cpu_rbs, offsetof(CPUE2KState, rbs), "rbs" },
        { &cpu_rsz, offsetof(CPUE2KState, rsz), "rsz" },
        { &cpu_rcur, offsetof(CPUE2KState, rcur), "rcur" },
        { &cpu_psz, offsetof(CPUE2KState, psz), "psz" },
    };

    static const struct { TCGv_i64 *ptr; int off; const char *name; } r64[] = {
        { &cpu_pregs, offsetof(CPUE2KState, pregs), "pregs" },
    };

    static const struct { TCGv *ptr; int off; const char *name; } rtl[] = {
        { &cpu_pc, offsetof(CPUE2KState, ip), "pc" },
    };

    unsigned int i;

    for (i = 0; i < ARRAY_SIZE(r32); i++) {
        *r32[i].ptr = tcg_global_mem_new(cpu_env, r32[i].off, r32[i].name);
    }

    for (i = 0; i < ARRAY_SIZE(r64); i++) {
        *r64[i].ptr = tcg_global_mem_new(cpu_env, r64[i].off, r64[i].name);
    }

    for (i = 0; i < ARRAY_SIZE(rtl); i++) {
        *rtl[i].ptr = tcg_global_mem_new(cpu_env, rtl[i].off, rtl[i].name);
    }

    cpu_win_ptr = tcg_global_mem_new_ptr(cpu_env, offsetof(CPUE2KState, win_ptr), "win_ptr");

    for (i = 0; i < WREGS_SIZE; i++) {
        snprintf(buf, ARRAY_SIZE(buf), "%%r%d", i);
        cpu_wregs[i] = tcg_global_mem_new(cpu_win_ptr,
            i * sizeof(target_ulong),
            buf);
    }

    for (i = 0; i < 32; i++) {
        snprintf(buf, ARRAY_SIZE(buf), "%%g%d", i);
        cpu_gregs[i] = tcg_global_mem_new(cpu_env,
            offsetof(CPUE2KState, gregs[i]),
            buf);
    }

    for (i = 0; i < 3; i++) {
        snprintf(buf, ARRAY_SIZE(buf), "%%ctpr%d", i + 1);
        cpu_ctprs[i] = tcg_global_mem_new(cpu_env,
            offsetof(CPUE2KState, ctprs[i]),
            buf
        );
    }
}
