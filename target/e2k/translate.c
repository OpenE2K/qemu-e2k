#include "qemu/osdep.h"
#include "qemu.h"
#include "disas/disas.h"
#include "exec/translator.h"
#include "tcg/tcg-op.h"

typedef struct {
    DisasContextBase base;
    target_ulong pc;
    target_ulong npc;
} DisasContext;

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

// TODO: return error on invalid instruction
static target_ulong unpack_instr(CPUE2KState *env, target_ulong pos,
                                 struct unpacked_instr *instr)
{
  unsigned int i;
  target_ulong gap, next_pc = pos + 8;
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
  mdl = ((hs & 0xf) + 1) * 4;

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
  pos = ((((hs & 0x70) >> 4) + 1) << 3) - 4;

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

static target_ulong disas_e2k_insn(DisasContext *dc, CPUState *cs)
{
    E2KCPU *cpu = E2K_CPU(cs);
    CPUE2KState *env = &cpu->env;
    struct unpacked_instr *instr = &unpacked_instr;
    target_ulong next_pc = unpack_instr(env, dc->base.pc_next, instr);

    // TODO: gen ops
    qemu_log_mask(LOG_UNIMP, "disas_e2k_insn: not implemented, ip 0x%lx, HS 0x%x\n",
        dc->base.pc_next, instr->hs);

    return next_pc;
}

static void e2k_tr_init_disas_context(DisasContextBase *db, CPUState *cs)
{
    DisasContext *dc = container_of(db, DisasContext, base);

    dc->pc = dc->base.pc_first;
    dc->npc = dc->base.pc_next;
}

static void e2k_tr_tb_start(DisasContextBase *db, CPUState *cs)
{
    // TODO
    qemu_log_mask(LOG_UNIMP, "e2k_tr_tb_start: not implemented\n");
}

static void e2k_tr_insn_start(DisasContextBase *db, CPUState *cs)
{
    DisasContext *dc = container_of(db, DisasContext, base);
    tcg_gen_insn_start(dc->pc);
    // FIXME: stop decoding on page boundary?
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
    dc->base.pc_next = disas_e2k_insn(dc, cs);
}

static void e2k_tr_tb_stop(DisasContextBase *db, CPUState *cs)
{
    // TODO
    qemu_log_mask(LOG_UNIMP, "e2k_tr_tb_stop: not implemented\n");
}

static void e2k_tr_disas_log(const DisasContextBase *db, CPUState *cpu)
{
    // TODO
    qemu_log_mask(LOG_UNIMP, "e2k_tr_disas_log: not implemented\n");
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
    DisasContext dc = {};

    translator_loop(&e2k_tr_ops, &dc.base, cs, tb, max_insns);
}

void restore_state_to_opc(CPUE2KState *env, TranslationBlock *tb,
                          target_ulong *data)
{
    // TODO
    qemu_log_mask(LOG_UNIMP, "restore_state_to_opc: not implemented\n");
}

void e2k_tcg_initialize(void) {
    // TODO
    qemu_log_mask(LOG_UNIMP, "e2k_tcg_initialize: not implemented\n");
}
