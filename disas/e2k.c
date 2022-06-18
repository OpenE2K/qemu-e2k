/* Print E2K instructions.
   Copyright (C) 1989-2016 Free Software Foundation, Inc.

   This file is part of the GNU opcodes library.

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   It is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston,
   MA 02110-1301, USA.  */

/* This is a concatenation of e2k-opc.c and e2k-dis.c */

#include "qemu/osdep.h"
#include "disas/dis-asm.h"
#include "e2k.h"

/* I need MCPU referred to in "e2k-opc.h" here to prevent "e2k-opc.c" from
   being linked into OBJDUMP. At the same time I want to prevent "e2k-dis.c"
   from being mistakenly linked into GAS because of this variable, which is why
   it's made static and renamed here.  */
#define mcpu e2k_dis_mcpu
static unsigned long mcpu;

typedef struct e2k_opcode_hash
{
  struct e2k_opcode_hash *next;
  const e2k_alf_opcode_templ *templ;
} e2k_opcode_hash;

/* Use opcode as a hash key for now.  */
static e2k_opcode_hash *opcode_hash_table[128];

/* At STAGE = 0 the hash table's buffer of an appropriate size is allocated,
   at STAGE = 1 it is filled in.  */
static int build_hash_table_stage = 0;
static int num_opcodes;
static e2k_opcode_hash *hash_buf = NULL;

static void
add_to_insn_table (e2k_opcode_templ *t)
{
  const e2k_alf_opcode_templ *alf;
  e2k_opcode_hash *h;

  if (build_hash_table_stage == 0)
    {
      num_opcodes++;
      return;
    }

  h = &hash_buf[num_opcodes++];
  alf = (const e2k_alf_opcode_templ *) t;

  assert (alf->opc < 128);

  h->next = opcode_hash_table[alf->opc];
  h->templ = alf;
  opcode_hash_table[alf->opc] = h;
}

static const char *
merge_alopf1_with_alopf11 (e2k_alf1_opcode_templ *l,
                           e2k_alopf11_opcode_templ *r)
{
  int i;
  for (i = 0; i < 6; i++)
    {
      if (l->allowed_channels[i] != 0)
        {
          if (r->allowed_channels[i] != 0)
            abort ();

          r->allowed_channels[i] = l->allowed_channels[i];
          /* This value seems to be not currently reserved in iset.  Use it to
             specify that an instruction in the related ALC should be actually
             encoded as ALOPF1, not ALOPF11.  */
          r->ales_opc2[i] = 0xff;
        }
    }

  /* Signalize to the caller that the hash entry should be replaced with more
     "universal" merged ALOPF11.  */
  return "jam";
}

static const char *
merge_alopf11 (struct e2k_opcode_templ *lhs,
               const struct e2k_opcode_templ *rhs)
{
  int i;
  e2k_alopf11_opcode_templ *l = (e2k_alopf11_opcode_templ *) lhs;
  const e2k_alopf11_opcode_templ *r = (const e2k_alopf11_opcode_templ *) rhs;

  /* FIXME: accessing `alopf' field in LHS which may very well turn out to be
     ALOPF1 via `e2k_alopf11_opcode_templ *' is controversial. It'd be be better
     to use `e2k_alf_opcode_templ *'.  */
  if (l->alopf == ALOPF1 && r->alopf == ALOPF11)
    {
      e2k_alf1_opcode_templ *alopf1 = (e2k_alf1_opcode_templ *) lhs;
      e2k_alopf11_opcode_templ *alopf11 = (e2k_alopf11_opcode_templ *) rhs;
      return merge_alopf1_with_alopf11 (alopf1, alopf11);
    }

  if (l->alopf != r->alopf
      || (l->alopf != ALOPF11 && l->alopf != ALOPF11_LIT8))
    abort ();

  if (l->opc != r->opc)
    abort ();

  for (i = 0; i < 6; i++)
    {
      if (r->allowed_channels[i] != 0)
        {
          if (l->allowed_channels[i] != 0)
            {
              if (l->ales_opc2[i] != r->ales_opc2[i])
                abort ();
            }
          else
            {
              l->allowed_channels[i] = r->allowed_channels[i];
              l->ales_opc2[i] = r->ales_opc2[i];
            }
        }
    }

  /* Return NULL by analogy with succesfull hash_jam to make it clear to the
     caller that there's no point in replacing the hash entry.  */
  return NULL;
}

static const char *
merge_alopf_simple (struct e2k_opcode_templ *lhs,
                    const struct e2k_opcode_templ *rhs)
{
  int i;
  e2k_alf_opcode_templ *l = (e2k_alf_opcode_templ *) lhs;
  const e2k_alf_opcode_templ *r = (const e2k_alf_opcode_templ *) rhs;

  if ((l->alopf != ALOPF1 && l->alopf != ALOPF2 && l->alopf != ALOPF21)
      || l->alopf != r->alopf)
    abort ();

  for (i = 0; i < 6; i++)
    {
      if (r->allowed_channels[i] != 0)
        {
          /* There should be no duplication of ALC'es now. I can probably rely
             on this above in `merge_alopf11 ()' as well . . .  */
          if (l->allowed_channels[i] != 0)
            abort ();
          else
            l->allowed_channels[i] = r->allowed_channels[i];
        }
    }

  return NULL;
}

void
init_opcode_templs (void)
{
#if 0
  init_opcode_templs_from_table (0, e2k_short_opers, sizeof (e2k_short_opers) / sizeof (e2k_short_opers[0]));
  init_opcode_templs_from_table (1, e2k_long_opers, sizeof (e2k_long_opers) / sizeof (e2k_long_opers[0]));
#endif /* 0  */

  /* Opcodes are mostly added from an automatically generated e2k-opc.h  */
#include "e2k-opc.h"

  /* Add some opcodes manually. */

#define E2K_OPCODE_VEC_ENTRY(name, parse_fn)                            \
  {                                                                     \
    static const e2k_opcode_templ dummy = {#name, parse_fn, NULL};      \
    add_to_insn_table ((e2k_opcode_templ *) &dummy);                    \
  }

#define COPF2_VEC_ENTRY(name, ctp_opc, allowed_ctprs, label_expected)   \
  {                                                                     \
    static const e2k_copf2_opcode_templ dummy =                         \
      {#name, parse_copf2_args, NULL, ctp_opc, allowed_ctprs,           \
       label_expected};                                                 \
                                                                        \
    add_to_insn_table ((e2k_opcode_templ *) &dummy);                    \
  }

#define COPF4_VEC_ENTRY(name)                                           \
  {                                                                     \
    static const e2k_opcode_templ dummy =                               \
      {#name, parse_copf4_args, NULL};                                  \
                                                                        \
    add_to_insn_table ((e2k_opcode_templ *) &dummy);                    \
  }


#define AAOPF1_VEC_ENTRY(name, opc, format)             \
  {                                                     \
    static e2k_mova_opcode_templ dummy =                \
      {#name, parse_mova_args, NULL, opc, format};      \
                                                        \
    add_to_insn_table ((e2k_opcode_templ *) &dummy);    \
  }

/* FIXME: stupidly adding an extra cast to `(void *)' to avoid strict
   aliasing warnings when compiling with `-O2'.  */
#define SETCMD_VEC_ENTRY(name, id)                                      \
  {                                                                     \
    static const e2k_setcmd_opcode_templ dummy =                        \
      {#name,parse_setcmd_args, NULL, id};                              \
                                                                        \
    add_to_insn_table ((e2k_opcode_templ *) (void *) &dummy);           \
  }

  COPF2_VEC_ENTRY (disp, 0x0, ALL_CTPRS, EXPECT_LABEL);
  COPF2_VEC_ENTRY (ldisp, 0x1, CTPR2, EXPECT_LABEL);
  /* FIXME: unlike DISP only a numeric value should be
     allowed for SDISP, a symbol makes no sense. */
  COPF2_VEC_ENTRY (sdisp, 0x2, ALL_CTPRS, EXPECT_LABEL);


  /* FIXME: the underlying two entries are COPF1 in fact.  */
  COPF2_VEC_ENTRY (return, 0x3, CTPR3, NO_LABEL);
  if (mcpu >= 3)
    COPF2_VEC_ENTRY (gettsd, 0x3, ALL_CTPRS, NO_LABEL);

  E2K_OPCODE_VEC_ENTRY (pref, parse_pref_args);

  COPF4_VEC_ENTRY (flushr);
  COPF4_VEC_ENTRY (flushc);

  SETCMD_VEC_ENTRY (setbn, 0);
  SETCMD_VEC_ENTRY (setbp, 1);
  SETCMD_VEC_ENTRY (settr, 2);
  SETCMD_VEC_ENTRY (setwd, 3);
  SETCMD_VEC_ENTRY (vfrpsz, 4);
  SETCMD_VEC_ENTRY (setei, 5);

  if (mcpu >= 2)
    E2K_OPCODE_VEC_ENTRY (setsft, parse_setsft_args);

  E2K_OPCODE_VEC_ENTRY (nop, parse_nop_args);
  E2K_OPCODE_VEC_ENTRY (ct, parse_ct_args);
  E2K_OPCODE_VEC_ENTRY (call, parse_ct_args);

  if (mcpu >= 6)
    E2K_OPCODE_VEC_ENTRY (hcall, parse_hcall_args);

  E2K_OPCODE_VEC_ENTRY (ipd, parse_ipd_args);

  E2K_OPCODE_VEC_ENTRY (loop_mode, parse_loop_mode_args);
  E2K_OPCODE_VEC_ENTRY (alc, parse_alc_args);
  E2K_OPCODE_VEC_ENTRY (abn, parse_abn_args);
  E2K_OPCODE_VEC_ENTRY (abp, parse_abp_args);
  E2K_OPCODE_VEC_ENTRY (abg, parse_abg_args);
  E2K_OPCODE_VEC_ENTRY (bap, parse_bap_args);
  E2K_OPCODE_VEC_ENTRY (eap, parse_eap_args);

  E2K_OPCODE_VEC_ENTRY (pass, parse_pass_args);
  E2K_OPCODE_VEC_ENTRY (andp, parse_andp_args);
  E2K_OPCODE_VEC_ENTRY (landp, parse_landp_args);

  E2K_OPCODE_VEC_ENTRY (ibranch, parse_ibranch_args);
  E2K_OPCODE_VEC_ENTRY (rbranch, parse_ibranch_args);
  E2K_OPCODE_VEC_ENTRY (done, parse_done_hret_glaunch_args);

  if (mcpu >= 6)
    {
      E2K_OPCODE_VEC_ENTRY (hret, parse_done_hret_glaunch_args);
      E2K_OPCODE_VEC_ENTRY (glaunch, parse_done_hret_glaunch_args);
    }

  E2K_OPCODE_VEC_ENTRY (wait, parse_wait_args);

  {
    static const e2k_alf3_opcode_templ mmurr =
      {"mmurr", parse_alf_args, NULL, MMURR, NO_MAS, 0x67, {0, 0, 1, 0, 0, 1},
       ARGS_DDD};

    /* FIXME: I've intentionally specified NO_MAS here so that `parse_alf_args
       ()' doesn't attempt to parse it. I set it manually in parse_mmurw_args
       instead.  */
    static const e2k_alf3_opcode_templ mmurw =
      {"mmurw", parse_alf_args, NULL, MMURW, NO_MAS, 0x27, {0, 0, 1, 0, 0, 0},
       ARGS_DDD};

    add_to_insn_table ((e2k_opcode_templ *) &mmurr);
    add_to_insn_table ((e2k_opcode_templ *) &mmurw);
  }

  E2K_OPCODE_VEC_ENTRY (incr, parse_incr_args);

  AAOPF1_VEC_ENTRY (movab, 0x1, ARGS_S);
  AAOPF1_VEC_ENTRY (movah, 0x2, ARGS_S);
  AAOPF1_VEC_ENTRY (movaw, 0x3, ARGS_S);
  AAOPF1_VEC_ENTRY (movad, 0x4, ARGS_D);
  AAOPF1_VEC_ENTRY (movaq, 0x5, ARGS_Q);
  AAOPF1_VEC_ENTRY (movaqp, 0x7, ARGS_D);

  E2K_OPCODE_VEC_ENTRY (fapb, parse_fapb_args);
  E2K_OPCODE_VEC_ENTRY (movep, parse_movep_args);

  if (mcpu >= 3)
    E2K_OPCODE_VEC_ENTRY (flushts, parse_flushts_args);

  E2K_OPCODE_VEC_ENTRY (clpandp, parse_cpl_args);
  E2K_OPCODE_VEC_ENTRY (clplandp, parse_cpl_args);

  E2K_OPCODE_VEC_ENTRY (set_mark, parse_set_mark_args);
  E2K_OPCODE_VEC_ENTRY (vfdi, parse_vfdi_args);
}

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


static int
unpack_instr (bfd_byte *buf)
{
  unsigned int i;
  size_t pos = 0;
  size_t gap;
  int hsyll_cntr = 0;
  unsigned int hs;
  unsigned int mdl;
  struct unpacked_instr *instr = &unpacked_instr;
  memset (instr, 0, sizeof (unpacked_instr));

  memcpy (&hs, &buf[pos], 4);
  pos += 4;
  instr->hs = hs;

  /* Check for SS.  */
  if (hs & (0x1 << 12))
    {
      instr->ss_present = 1;
      memcpy (&instr->ss, &buf[pos], 4);
      pos += 4;
    }

  /* Check for available ALS syllables.  */
  for (i = 0; i < 6; i++)
    {
      if (hs & (1 << (26 + i)))
        {
          instr->als_present[i] = 1;
          memcpy (&instr->als[i], &buf[pos], 4);
          pos += 4;
        }
    }

  /* Check for CS0.  */
  if (hs & (0x1 << 14))
  {
    instr->cs0_present = 1;
    memcpy (&instr->cs0, &buf[pos], 4);
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
      memcpy (&instr->ales[5], &buf[pos], 2);
      memcpy (&instr->ales[2], &buf[pos + 2], 2);

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
      memcpy (&instr->cs1, &buf[pos], 4);
      pos += 4;
    }

  /* A primitive control just for a moment.  */
  if (mdl != pos)
    {
    /* This is either an APB instruction or an invalid one. Let's stupidly
       believe that the former takes place and signalize our caller about
       that by returning 0.  */

      return 0;
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
          memcpy (&instr->ales[i],
                  &buf[pos + 2 * ((hsyll_cntr & ~0x1) + 1
                                  - (hsyll_cntr & 0x1))], 2);
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
              memcpy (&instr->aas[i],
                      &buf[pos + 2 * ((hsyll_cntr & ~0x1) + 1
                                      - (hsyll_cntr & 0x1))], 2);
              hsyll_cntr++;
            }
        }
    }

  if (hsyll_cntr & 0x1)
    {
      /* Simplify the calculation of offset in BUF[] a bit by taking the above
         condition into account.  */
      memcpy (&instr->half_gap, &buf[pos + 2 * (hsyll_cntr & ~0x1)], 2);
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
      memcpy (&instr->cds[i], &buf[pos], 4);
      pos -= 4;
    }


  /* Check for PLSj syllables.  */
  for (i = 0; i < ((hs & 0xc0000) >> 18); i++)
    {
      instr->pls_present[i] = 1;
      memcpy (&instr->pls[i], &buf[pos], 4);
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
    return 0;

  /* Extract available LTSj syllables.  */
  for (i = 0; i < 4 && pos >= gap; i++)
    {
      instr->lts_present[i] = 1;
      memcpy (&instr->lts[i], &buf[pos], 4);
      pos -= 4;
    }

  /* It makes sense to enumerate GAP syllables in a normal order unlike LTS
     ones.  */
  for (i = 0; i < 16 && gap <= pos; i++)
    {
      instr->gap_present[i] = 1;
      memcpy (&instr->gap[i], &buf[gap], 4);
      gap += 4;
    }

  return 1;
}

/* A convenience macro used in the functions below.  */
#define my_printf(...) (*info->fprintf_func) (info->stream, __VA_ARGS__)


static int indentation;

static void
print_syllable (disassemble_info *info __attribute__ ((unused)),
                const char *name __attribute__ ((unused)),
                int half,
                unsigned int val __attribute__ ((unused)))
{
  /* Specifies that we are going to print an uppermost half-syllable.  */
  static int uppermost = 1;

#if defined ENABLE_E2K_ENCODINGS
  int i;
  char tpl[] = "      ";
#endif /* ENABLE_E2K_ENCODINGS  */

  /* If a lowermost 16-bit half-syllable hasn't been printed out and we are
     said to print a 32-bit syllable, this is a logical error.  */
  assert (uppermost == 1 || half == 1);

#if defined ENABLE_E2K_ENCODINGS

  for (i = 0; i < 6 && name[i] != '\0'; i++)
    tpl[i] = name[i];

  if (half == 0)
    my_printf ("%s%08x", tpl, val);
  else
    my_printf ("%s%s%04x", tpl, uppermost ? "" : "    ", val);

#endif /* defined ENABLE_E2K_ENCODINGS  */

#if defined ENABLE_E2K_ENCODINGS
  if (half == 0 || uppermost == 0)
    /* Two spaces should be appended after other syllables or lowermost
       half-syllables.  */
    indentation = 2;
  else
    /* Six spaces should be appended after uppermost half-syllables.  */
    indentation = 1;
#else /* ! defined ENABLE_E2K_ENCODINGS  */
  /* Six spaces make no sense if encodings are disabled.  */
  indentation = 2;
#endif /* ! defined ENABLE_E2K_ENCODINGS  */

  /* Toggle UPPERMOST mark if a half syllable is printed out.  */
  if (half)
    uppermost = 1 - uppermost;
}

#define print_syllable(...) print_syllable (info, __VA_ARGS__)


static void
end_syllable (disassemble_info *info)
{
#if ! defined ENABLE_E2K_ENCODINGS
  /* In "release" mode `indentation == 2' means that nothing related to the
     curent syllable has been printed out in fact. This check saves me the
     trouble of putting all invocations of `print_syllable ()' under `defined
     ENABLE_E2K_ENCODINGS' condition.  */
  if (indentation != 2)
#endif
    my_printf ("\n");
}

#define end_syllable() end_syllable (info)

static void
indentate (disassemble_info *info)
{
  /* Indentation prepended to all lines except the first one in multiline
     instructions.  */
  const char *newline_indentation =
#if defined ENABLE_E2K_ENCODINGS
    "                "
#else /* ! defined ENABLE_E2K_ENCODINGS  */
    "  "
#endif /* ! defined ENABLE_E2K_ENCODINGS  */
    ;

  if (indentation)
    {
      if (indentation == 1)
        my_printf ("      ");
      else
        my_printf ("  ");

      indentation = 0;
    }
  else
    my_printf ("\n%s", newline_indentation);
}

#define indentate() indentate (info);


static const char *reg_prefix[] = {"", "d", "q", "qp"};

static void
print_src1 (disassemble_info *info, int chn, e2k_register_format fmt)
{
  unsigned int src1 = (unpacked_instr.als[chn] & 0x00ff0000) >> 16;

  assert (fmt == SINGLE || fmt == DOUBLE || fmt == QUAD || fmt == QPACKED);

  if ((src1 & 0x80) == 0)
    my_printf ("%%%sb[%d]", reg_prefix[fmt], src1 & 0x7f);
  else if ((src1 & 0xc0) == 0x80)
    my_printf ("%%%sr%d", reg_prefix[fmt], src1 & 0x3f);
  else if ((src1 & 0xe0) == 0xc0)
    my_printf ("0x%x", src1 & 0x1f);
  else
    my_printf ("%%%sg%d", reg_prefix[fmt], src1 & 0x1f);
}

static void
print_src2 (disassemble_info *info, int chn, e2k_register_format fmt)
{
  const struct unpacked_instr *instr = &unpacked_instr;
  unsigned int src2 = (instr->als[chn] & 0x0000ff00) >> 8;

  assert (fmt == SINGLE || fmt == DOUBLE || fmt == QUAD || fmt == QPACKED);

  if ((src2 & 0x80) == 0)
    my_printf ("%%%sb[%d]", reg_prefix[fmt], src2 & 0x7f);
  else if ((src2 & 0xc0) == 0x80)
    my_printf ("%%%sr%d", reg_prefix[fmt], src2 & 0x3f);
  else if ((src2 & 0xf0) == 0xc0)
    my_printf ("0x%x", src2 & 0xf);
  else if ((src2 & 0xf8) == 0xd0)
    {
      /* Print a 16-bit literal.  */
      unsigned int lit_num = src2 & 0x3;
      int shift = (src2 & 0x4) ? 16 : 0;

      if (lit_num > 1)
        my_printf ("<illegal _f16s,_lts%d%s>", lit_num, shift ? "hi" : "lo");
      else if (! instr->lts_present[lit_num])
        my_printf ("<nonexistent _f16s,_lts%d%s>", lit_num,
                   shift ? "hi" : "lo");
      else
        {
#if defined ENABLE_E2K_ENCODINGS
          my_printf ("_f16s,_lts%d%s ", lit_num, shift ? "hi" : "lo");
#else
          my_printf ("_f16s ");
#endif /* ENABLE_E2K_ENCODINGS  */
          my_printf ("0x%x", ((instr->lts[lit_num] >> shift) & 0xffff));
        }
    }
  else if ((src2 & 0xfc) == 0xd8)
    {
      /* Print a 32-bit literal.  */
      unsigned int lit_num = src2 & 0x3;

      if (! instr->lts_present[lit_num])
        my_printf ("<nonexistent _f32s,_lts%d>", lit_num);
      else
        {
#if defined ENABLE_E2K_ENCODINGS
          my_printf ("_f32s,_lts%d ", lit_num);
#else
          my_printf ("_f32s ");
#endif /* ENABLE_E2K_ENCODINGS  */
          my_printf ("0x%x", instr->lts[lit_num]);
        }
    }
  else if ((src2 & 0xfc) == 0xdc)
    {
      /* Print a 64-bit literal.  */
      unsigned int lit_num = src2 & 0x3;

      if (lit_num == 3)
        my_printf ("<illegal _f64,_lts%d>", lit_num);
      else if (! instr->lts_present[lit_num]
               || ! instr->lts_present[lit_num + 1])
        my_printf ("<nonexistent _f64,_lts%d>", lit_num);
      else
        {
#if defined ENABLE_E2K_ENCODINGS
          my_printf ("_f64,_lts%d ", lit_num);
#else
          my_printf ("_f64 ");
#endif /* ENABLE_E2K_ENCODINGS  */

          if (instr->lts[lit_num + 1] == 0)
            my_printf ("0x%x", instr->lts[lit_num]);
          else
            my_printf ("0x%x%08x", instr->lts[lit_num + 1], instr->lts[lit_num]);
        }
    }
  else
    my_printf ("%%%sg%d", reg_prefix[fmt], src2 & 0x1f);
}

static void
print_src3 (disassemble_info *info, int chn, int in_ales,
            e2k_register_format fmt)
{
  unsigned int src3 = (in_ales
                       ? unpacked_instr.ales[chn] & 0xff
                       : unpacked_instr.als[chn] & 0x000000ff);

  assert (fmt == SINGLE || fmt == DOUBLE || fmt == QUAD || fmt == QPACKED);

  if ((src3 & 0x80) == 0)
    my_printf ("%%%sb[%d]", reg_prefix[fmt], src3 & 0x7f);
  else if ((src3 & 0xc0) == 0x80)
    my_printf ("%%%sr%d", reg_prefix[fmt], src3 & 0x3f);
  else if ((src3 & 0xe0) == 0xc0)
    my_printf ("<illegal operand>");
  else
    my_printf ("%%%sg%d", reg_prefix[fmt], src3 & 0x1f);
}

static void
print_dst (disassemble_info *info, unsigned int dst, e2k_register_format fmt)
{
  assert (fmt == SINGLE || fmt == DOUBLE || fmt == QUAD || fmt == QPACKED);

  if ((dst & 0x80) == 0)
    my_printf ("%%%sb[%d]", reg_prefix[fmt], dst & 0x7f);
  else if ((dst & 0xc0) == 0x80)
    my_printf ("%%%sr%d", reg_prefix[fmt], dst & 0x3f);
  else if ((dst & 0xe0) == 0xc0)
    {
      /* FIXME: these names are to be shared with GAS somehow.  */
      static const char *special[] = {
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        "%tst",
        "%tc",
        "%tcd",
        NULL,
        "%ctpr1",
        "%ctpr2",
        "%ctpr3",
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        "%empty.lo",
        "%empty.hi"
      };

      unsigned int idx = dst & 0x1f;
      my_printf ("%s", (special[idx]
                        ? special[idx]
                        : "<reserved special register>"));
    }
  else
    my_printf ("%%%sg%d", reg_prefix[fmt], dst & 0x1f);
}

static void
print_dst_in_als (disassemble_info *info, int chn, e2k_register_format fmt)
{
  print_dst (info, unpacked_instr.als[chn] & 0x000000ff, fmt);
}

static void
print_dst2 (disassemble_info *info, int chn)
{
  unsigned int pred_num = unpacked_instr.als[chn] & 0x1f;
  my_printf ("%%pred%d", pred_num);
}

static void
print_state_reg (disassemble_info *info, int code)
{
  static const char *names[] = {
    /* ALT_ENTRies are ignored for the sake of unambiguous disassembly.  */
#define ALT_ENTRY(a,b,c,d)
#define ENTRY(a,b,c,d) [b] = a,
#define __e3s(arg) arg
#define __e2s(arg) arg
#define __v5(arg) arg

/* ALT_ENTRies below make sense for assembler only to allow for different
   variants of state register names. They are ignored in disassembler.  */

    ENTRY("%psr", 0x00, 1, 0)
    ENTRY("%wd", 0x01, 0, 0)
    ENTRY("%core_mode", 0x04, 0, 0)
    ENTRY("%cwd", 0x06, 1, 0)
    ENTRY("%psp.hi", 0x07, 1, 0)
    ENTRY("%psp.lo", 0x09, 1, 0)
    ENTRY("%pshtp", 0x0b, 1, 0)
    ENTRY("%pcsp.hi", 0x0d, 1, 0)
    ENTRY("%pcsp.lo", 0x0f, 1, 0)
    ENTRY("%pcshtp", 0x13, 0, 0)
    ENTRY("%ctpr1", 0x15, 0, 0)
    ENTRY("%ctpr2", 0x16, 0, 0)
    ENTRY("%ctpr3", 0x17, 0, 0)
    ENTRY("%sbr", 0x1e, 0, 0)
    /* This flavour of %sbr register name won't be used in disassembler.  */
    ALT_ENTRY("%usbr", 0x1e, 0, 0)
    ENTRY("%cutd", 0x21, 1, 0)
    ENTRY("%eir", 0x23, 0, 0)

    /* Note that `%tsd' register corresponding to 0x24 has been "officially"
       eliminated from all versions of Elbrus instruction set (see Bug #74420
       and Bug #53587). In fact it's not supported starting from elbrus-v3
       only.  */

    ENTRY("%cuir", 0x25, 0, 0)

    ENTRY("%oscud.hi", 0x26, 1, 0)
    ENTRY("%oscud.lo", 0x27, 1, 0)
    ENTRY("%osgd.hi", 0x28, 1, 0)
    ENTRY("%osgd.lo", 0x29, 1, 0)
    ENTRY("%osem", 0x2a, 1, 0)
    ENTRY("%usd.hi", 0x2c, 1, 0)
    ENTRY("%usd.lo", 0x2d, 1, 0)

    /* `%tr' register which used to be encoded with 0x2e has been eliminated
       together with `%tsd' (see above).  */

    ENTRY("%osr0", 0x2f, 0, 0)
    ENTRY("%cud.hi", 0x30, 0, 1)
    ENTRY("%cud.lo", 0x31, 0, 1)
    ENTRY("%gd.hi", 0x32, 0, 1)
    ENTRY("%gd.lo", 0x33, 0, 1)
    ENTRY("%cs.hi", 0x34, 1, 0)
    ENTRY("%cs.lo", 0x35, 1, 0)
    ENTRY("%ds.hi", 0x36, 1, 0)
    ENTRY("%ds.lo", 0x37, 1, 0)
    ENTRY("%es.hi", 0x38, 1, 0)
    ENTRY("%es.lo", 0x39, 1, 0)
    ENTRY("%fs.hi", 0x3a, 1, 0)
    ENTRY("%fs.lo", 0x3b, 1, 0)
    ENTRY("%gs.hi", 0x3c, 1, 0)
    ENTRY("%gs.lo", 0x3d, 1, 0)
    ENTRY("%ss.hi", 0x3e, 1, 0)
    ENTRY("%ss.lo", 0x3f, 1, 0)
    ENTRY("%dibcr", 0x40, 0, 0)
    ENTRY("%dimcr", 0x41, 0, 0)
    ENTRY("%dibsr", 0x42, 0, 0)
    ENTRY("%dtcr", 0x43, 0, 0)
    ENTRY("%dibar0", 0x48, 0, 0)
    ENTRY("%dibar1", 0x49, 0, 0)
    ENTRY("%dibar2", 0x4a, 0, 0)
    ENTRY("%dibar3", 0x4b, 0, 0)
    ENTRY("%dimar0", 0x4c, 0, 0)
    ENTRY("%dimar1", 0x4d, 0, 0)
    ENTRY("%dtarf", 0x4e, 0, 0)
    ENTRY("%dtart", 0x4f, 0, 0)
    ENTRY("%cr0.hi", 0x51, 0, 0)
    ENTRY("%cr0.lo", 0x53, 0, 0)
    ENTRY("%cr1.hi", 0x55, 0, 0)
    ENTRY("%cr1.lo", 0x57, 0, 0)
    ENTRY(__e2s("%sclkm1"), 0x70, 0, 0)
    ENTRY(__e2s("%sclkm2"), 0x71, 0, 0)
    ENTRY(__e3s("%cu_hw0"), 0x78, 0, 0)
    ENTRY(__v5("%cu_hw1"), 0x79, 0, 0)
    ENTRY("%upsr", 0x80, 0, 0)
    ENTRY("%ip", 0x81, 0, 0)
    ENTRY("%nip", 0x82, 0, 0)
    ENTRY("%lsr1", 0xc3, 0, 0)
    ENTRY("%lsr", 0x83, 0, 0)
    ENTRY("%pfpfr", 0x84, 0, 0)
    ENTRY("%fpcr", 0x85, 0, 0)
    ENTRY("%fpsr", 0x86, 0, 0)
    ENTRY("%ilcr1", 0xc7, 0, 0)
    ENTRY("%ilcr", 0x87, 0, 0)
    ENTRY("%br", 0x88, 0, 0)
    ENTRY("%bgr", 0x89, 0, 0)
    ENTRY(__e3s("%idr"), 0x8a, 0, 0)
    ENTRY("%clkr", 0x90, 0, 0)
    ENTRY("%rndpr", 0x91, 0, 0)
    ENTRY(__e2s("%sclkr"), 0x92, 0, 0)
    ENTRY("%tir.hi", 0x9c, 0, 0)
    ENTRY("%tir.lo", 0x9d, 0, 0)
    ENTRY("%rpr.lo", 0xa0, 0, 0)
    ENTRY("%sbbp", 0xa1, 0, 0)
    ENTRY("%rpr.hi", 0xa2, 0, 0)
    /* This flavour of %rpr.hi register name won't be used in disassembler.  */
    ALT_ENTRY("%rpr", 0xa0, 0, 0)
    ENTRY("%upsrm", 0xc0, 0, 0)

#undef __v5
#undef __e2s
#undef __e3s
#undef ENTRY
#undef ALT_ENTRY
  };

  my_printf ("%s", names[code]);
}


static void
print_predicates (disassemble_info *info,
                  int chn,
                  int mrgc)
{
  int i;
  /* Has the first separator been printed out? For RLP predicates this is a
     question sign, for MRGC - a comma.  */
  int first_separator = 0;
  const struct unpacked_instr *instr = &unpacked_instr;
  /* Required RLP for this ALC.  */
  unsigned short req_rlp = chn <= 2 ? 0 : 1;
  /* Required MRGC for this ALC in case ALF proves to be `merge{s,d}'.  */
  unsigned short req_mrgc = chn <= 2 ? 2 : 3;

  for (i = 0; i < 3; i++)
    {
      int j;
      unsigned short *cds;

      if (! instr->cds_present[i])
        continue;

      cds = (unsigned short *) &instr->cds[i];
      for (j = 0; j < 2; j++)
        {
          unsigned short opc = (cds[j] & 0xc000) >> 14;
          unsigned short mask = (cds[j] & 0x3c00) >> 10;
          /* Is this RLP suitable for CHN and is CHN present in its mask?  */
          if (((! mrgc && opc == req_rlp)
               || (mrgc && opc == req_mrgc))
              && (mask & (1 << (chn - 3 * req_rlp))))
            {
              unsigned short neg = (cds[j] & 0x0380) >> 7;
              unsigned short pred = cds[j] & 0x7f;
              unsigned short num = pred & 0x1f;

              if (! first_separator)
                {
                  my_printf (mrgc ? ", " : " ? ");
                  first_separator = 1;
                }
              else
                /* LDIS separates several RLP predicates acting on the same ALC
                   with a comma. Note that several MRGC predicates related to
                   the same ALC are inappropriate in fact . . .  */
                my_printf (mrgc ? " " : ", ");

              if (neg & (1 << (chn - 3 * req_rlp)))
                my_printf ("~");

              if ((pred & 0x60) == 0x40)
                my_printf ("%%pcnt%hd", num);
              else if ((pred & 0x60) == 0x60)
                my_printf ("%%pred%hd", num);
              else
                my_printf ("<invalid predicate>");
            }
        }
    }
}

static void
print_alf (disassemble_info *info, int chn)
{
  e2k_opcode_hash *cur;
  const e2k_alf_opcode_templ *match = NULL;
  const char *name;
  char syll_name[5];
  const struct unpacked_instr *instr = &unpacked_instr;
  unsigned int opc = (instr->als[chn] & 0x7f000000) >> 24;
  unsigned int sm = (instr->als[chn] & 0x80000000) >> 31;

  snprintf (syll_name, 5, "ALS%d", chn);
  print_syllable (syll_name, 0, instr->als[chn]);

  for (cur = opcode_hash_table[opc]; cur != NULL; cur = cur->next)
    {
      const e2k_alf_opcode_templ *templ = cur->templ;

      if (! templ->allowed_channels[chn])
        continue;

      if (templ->alopf == ALOPF1
          /* `merge{s,d} are considered to be ALOPF1 according to
             iset-vX.single and thus have a short encoding.  */
          || templ->alopf == MERGE
          || templ->alopf == ALOPF2
          || templ->alopf == ALOPF3
          || templ->alopf == ALOPF7
          || templ->alopf == ALOPF8)
        {
          /* If the current instruction has one of the short encodings the
             related ALES should not be present. (By "present" I mean that it
             has been marked in HS, not allocated in a wide instruction. This
             makes difference for `ALES{5,2}'.)  */
          if (instr->ales_present[chn] & 0x1)
            continue;

          if (templ->alopf == ALOPF2)
            {
              const e2k_alf2_opcode_templ *alf2;
              alf2 = (const e2k_alf2_opcode_templ *) templ;

              if (((instr->als[chn] & 0x00ff0000) >> 16) != alf2->opce)
                continue;
            }
          else if (templ->alopf == ALOPF7)
            {
              const e2k_alf7_opcode_templ *alf7;
              alf7 = (const e2k_alf7_opcode_templ *) templ;

              if (((instr->als[chn] & 0x000000e0) >> 5) != alf7->cmpopce)
                continue;
            }
          else if (templ->alopf == ALOPF8)
            {
              const e2k_alf8_opcode_templ *alf8;
              alf8 = (const e2k_alf8_opcode_templ *) templ;

              if (((instr->als[chn] & 0x000000e0) >> 5) != alf8->cmpopce)
                continue;
            }
        }
      else if (! (instr->ales_present[chn] & 0x1))
        /* ALES should be present for instructions having a long encoding. */
        continue;
      else
        {
          if (templ->alopf == ALOPF10
              /* According to `iset-vX.single' these ones are ALOPF10.  */
              /* || templ->alopf == AAURW  */)
            {
              if (strcmp (templ->name, "staaqp") == 0)
                {
                  if (instr->ales[chn] != 0x02c0)
                    continue;
                }
              else if (instr->ales[chn] != 0x01c0)
                continue;
            }
          else if (templ->alopf == ALOPF11
		   || templ->alopf == ALOPF11_MERGE)
            {
              const e2k_alopf11_opcode_templ *alopf11;

              alopf11 = (const e2k_alopf11_opcode_templ *) templ;
              if (instr->ales[chn] != ((alopf11->ales_opc2[chn] << 8)
                                       | alopf11->ales_opce))
                continue;
            }
          else if (templ->alopf == ALOPF11_LIT8)
            {
              const e2k_alopf11_lit8_opcode_templ *alopf11_lit8;

              alopf11_lit8 = (const e2k_alopf11_lit8_opcode_templ *) templ;
              if ((instr->ales[chn] & 0xff00)
                  != ((alopf11_lit8->ales_opc2[chn] << 8)))
                continue;
            }

          else if (templ->alopf == ALOPF12)
            {
              const e2k_alopf12_opcode_templ *alopf12;

              alopf12 = (const e2k_alopf12_opcode_templ *) templ;
              if ((instr->als[chn] & 0x00ff0000) != (unsigned) (alopf12->opce << 16)
                  || instr->ales[chn] != ((alopf12->ales_opc2 << 8)
                                          | alopf12->ales_opce))
                continue;
            }
            else if (templ->alopf == ALOPF12_PSHUFH)
              {
                /* Note that for PSHUFH ALES.opce holds the COUNT parameter,
                   therefore don't check for it.  */
                const e2k_alopf12_opcode_templ *alopf12;
                alopf12 = (const e2k_alopf12_opcode_templ *) templ;
                if ((instr->als[chn] & 0x00ff0000) != (unsigned) (alopf12->opce << 16)
                    || (instr->ales[chn] & 0xff00) != (alopf12->ales_opc2 << 8))
                continue;
              }
          else if (templ->alopf == ALOPF13)
            {
              const e2k_alopf13_opcode_templ *alopf13;

              alopf13 = (const e2k_alopf13_opcode_templ *) templ;
              if (instr->ales[chn] != ((alopf13->ales_opc2 << 8)
                                       | alopf13->ales_opce))
                continue;
            }
          else if (templ->alopf == ALOPF15
                   || templ->alopf == ALOPF16)
            {
              if (instr->ales[chn] != 0x01c0)
                continue;
            }
          /* Note that according to `iset-vX.single' these ones are ALOPF19.  */
          else if (templ->alopf == AAURR)
            {
              if (instr->ales[chn] != 0x01c0)
                continue;
            }
          else if (templ->alopf == ALOPF21
                   || templ->alopf == ALOPF21_MERGE)
            {
              const e2k_alopf21_opcode_templ *alopf21;

              alopf21 = (const e2k_alopf21_opcode_templ *) templ;
              if ((instr->ales[chn] & 0xff00) != (alopf21->ales_opc2 << 8))
                continue;
            }
            else if (templ->alopf == ALOPF22)
            {
              /* FIXME: according to the up-to-date version of iset-v5.single
                 there are no instructions with ALOPF22 encoding at all! Where
                 have I taken them from then? Moreover, according to the
                 underlying test, their ALES seems to be of ALEF2 format, not
                 ALEF1. What's the difference between them and ALOPF12
                 instructions then?  */
              const e2k_alf2_opcode_templ *alf2;
              alf2 = (const e2k_alf2_opcode_templ *) templ;

              if (instr->ales[chn] != 0x01c0
                  || ((instr->als[chn] & 0x00ff0000) >> 16) != alf2->opce)
                continue;
            }
	    else if (templ->alopf == ALOPF17)
	      {
		/* FIXME: eventually migrate to `e2k_alopf17_opcode_templ' here
		   simultaneously with other components.  */
		const e2k_alf7_opcode_templ *alf7;
		alf7 = (const e2k_alf7_opcode_templ *) templ;

		if (instr->ales[chn] != (0x0200 | alf7->cmpopce))
		  continue;
	      }
          /* Ignore them for now since they are a special case of `STAA*'
             instructions which are disassembled as ALOPF10 above.  */
            else if (templ->alopf == AAURW)
              continue;
          /* It's a 100% internal error if we turned out to be incapable of
             recognizing our own instruction template. It would be easier to
             skip it silently, of course, and not to disappoint the user, who
             would obtain an unremarkable "unrecognized" instruction in the
             worst case. But . . .  */
            else
              assert (0);
        }

      if (match == NULL)
        /* This is the first matching ALF template.  */
        match = templ;
      else
        /* More than one matching ALF templates found.  */
        break;
    }

  if (match == NULL)
    name = "unrecognized";
  else if (cur == NULL)
    /* Exactly one matching template found.  */
    name = match->name;
  else
    {
      /* More than one matching templates found.  */
      name = "ambiguous";
      match = NULL;
    }

  /* Stupidly prevent instructions requiring a pair of ALCes from being
     disassembled twice.
     FIXME: get rid of this code along with the corresponding one in GAS as
     soon as you add info on "real_alses[]" to `opcodes/e2k-opc.h'.  */
  if (((chn == 1 || chn == 4)
       && (strcmp (name, "movtq") == 0
           || strcmp (name, "gdtoap") == 0
           || strcmp (name, "cast") == 0
           || strcmp (name, "odtoap") == 0))
      || ((chn == 2 || chn == 5)
          && (strcmp (name, "ldgdq") == 0
              || strcmp (name, "ldcudq") == 0
              || strcmp (name, "ldapq") == 0
              || strcmp (name, "ldq") == 0

              || strcmp (name, "ldcsq") == 0
              || strcmp (name, "lddsq") == 0
              || strcmp (name, "ldesq") == 0
              || strcmp (name, "ldfsq") == 0
              || strcmp (name, "ldgsq") == 0
              || strcmp (name, "ldssq") == 0))
      || (chn == 5
          && (strcmp (name, "staaq") == 0
              || strcmp (name, "stgdq") == 0
              || strcmp (name, "stapq") == 0
              || strcmp (name, "aaurwq") == 0
              || strcmp (name, "aaurrq") == 0

              || strcmp (name, "stcsq") == 0
              || strcmp (name, "stdsq") == 0
              || strcmp (name, "stesq") == 0
              || strcmp (name, "stfsq") == 0
              || strcmp (name, "stgsq") == 0
              || strcmp (name, "stssq") == 0))
      || (chn == 1
          && strcmp (name, "getsap") == 0)
      || ((chn == 1 || chn == 4)
          && (strcmp (name, "aptoap") == 0
              || strcmp (name, "aptoapb") == 0)))
    {
      end_syllable ();
      return;
    }

  indentate ();
  my_printf ("%s,%d%s ", name, chn, sm ? ",sm" : "");

  if (match == NULL)
    {
      end_syllable ();
      return;
    }

  /* Now print the arguments.  */
  if (match->alopf == ALOPF1
      /* In iset-vX.single `merge{s,d}' are said to be ALOPF1.  */
      || match->alopf == MERGE
      || match->alopf == ALOPF11
      || match->alopf == ALOPF11_LIT8
      || match->alopf == ALOPF11_MERGE
      || match->alopf == ALOPF21
      || match->alopf == ALOPF21_MERGE)
    {
      /* FIXME: here I make use of the fact that in `e2k_alopf{1,2}1_opcode_
         templ' `arg_fmt[]' is located at the same offset as in
         `e2k_alf1_opcode_templ'.  */
      const e2k_alf1_opcode_templ *alf1 = (const e2k_alf1_opcode_templ *) match;

      print_src1 (info, chn, alf1->arg_fmt[0]);
      my_printf (", ");
      print_src2 (info, chn, alf1->arg_fmt[1]);
      my_printf (", ");

      if (match->alopf == ALOPF11_LIT8)
	/* Output an additional lit8 argument encoded in `ALES.opce'. */
	my_printf ("0x%x, ", (unsigned) (instr->ales[chn] & 0xff));
      else if (match->alopf == ALOPF21 || match->alopf == ALOPF21_MERGE)
        {
          print_src3 (info, chn, 1, alf1->arg_fmt[2]);
          my_printf (", ");
        }

      print_dst_in_als (info, chn,
                        alf1->arg_fmt[match->alopf == ALOPF21 ? 3 : 2]);
    }
  else if (match->alopf == ALOPF2
           || match->alopf == ALOPF12
	   || match->alopf == ALOPF12_PSHUFH
           || match->alopf == ALOPF22)
    {
      const e2k_alf2_opcode_templ *alf2 = (const e2k_alf2_opcode_templ *) match;

      print_src2 (info, chn, alf2->arg_fmt[0]);
      my_printf (", ");

      /* pshufh accepts an additional lit8 argument encoded in `ALES.opce'.  */
      if (match->alopf == ALOPF12_PSHUFH)
	my_printf ("0x%x, ", (unsigned) (instr->ales[chn] & 0xff));

      print_dst_in_als (info, chn, alf2->arg_fmt[1]);
    }
  else if (match->alopf == ALOPF3
           || match->alopf == ALOPF13)
    {
      const e2k_alf3_opcode_templ *alf3 = (const e2k_alf3_opcode_templ *) match;

      print_src1 (info, chn, alf3->arg_fmt[0]);
      my_printf (", ");
      print_src2 (info, chn, alf3->arg_fmt[1]);
      my_printf (", ");
      print_src3 (info, chn, 0, alf3->arg_fmt[2]);
    }
  else if (match->alopf == ALOPF15)
    {
      const e2k_alopf15_opcode_templ *alopf15
        = (const e2k_alopf15_opcode_templ *) match;

      print_src2 (info, chn, alopf15->arg_fmt);
      my_printf (", ");
      print_state_reg (info, instr->als[chn] & 0xff);
    }
  else if (match->alopf == ALOPF16)
    {
      const e2k_alopf16_opcode_templ *alopf16
        = (const e2k_alopf16_opcode_templ *) match;

      print_state_reg (info, (instr->als[chn] & 0x00ff0000) >> 16);
      my_printf (", ");
      print_dst_in_als (info, chn, alopf16->arg_fmt);
    }
  else if (match->alopf == ALOPF7 || match->alopf == ALOPF17)
    {
      const e2k_alf7_opcode_templ *alf7
        = (const e2k_alf7_opcode_templ *) match;

      print_src1 (info, chn, alf7->arg_fmt[0]);
      my_printf (", ");
      print_src2 (info, chn, alf7->arg_fmt[1]);
      my_printf (", ");
      print_dst2 (info, chn);
    }
  else if (match->alopf == ALOPF8)
    {
      const e2k_alf8_opcode_templ *alf8
        = (const e2k_alf8_opcode_templ *) match;

      print_src2 (info, chn, alf8->arg_fmt);
      my_printf (", ");
      print_dst2 (info, chn);
    }
#if 0
  else if (match->alopf == ALOPF19)
    {
    }
#endif /* 0  */
  else if (match->alopf == ALOPF10)
    {
      unsigned int opce1 = (instr->als[chn] & 0x00ffff00) >> 8;
      unsigned int lt = opce1 & 0x3;
      unsigned int am = (opce1 & 0x4) >> 2;
      unsigned int incr = (opce1 & 0x70) >> 4;
      unsigned int ind = (opce1 & 0x780) >> 7;
      unsigned int d = (opce1 & 0xf800) >> 11;

      const e2k_alf10_opcode_templ *alf10
        = (const e2k_alf10_opcode_templ *) match;

  
      print_src3 (info, chn, 0, alf10->arg_fmt);
      my_printf (", %%aad%d [ %%aasti%d", d, ind);

      if (lt)
        {
          if ( instr->lts_present[lt - 1])
            my_printf (" + _f32s,_lts%d 0x%x", lt - 1, instr->lts[lt - 1]);
          else
            my_printf (" + <non-existent literal>");
        }

      my_printf (" ]");

      if (am)
        {
          indentate ();
          my_printf ("incr,%d %%aaincr%d", chn, incr);
        }
      
    }

  if (match->need_mas
      /* I wonder if instructions requiring MAS may be placed into MASless ALC
         in principle . . .  */
      && (chn == 0 || chn == 2 || chn == 3 || chn == 5)
      && instr->cs1_present
      && ((instr->cs1 & 0xf0000000) >> 28) == 6)
    {
      static int shift[] = {21, -1, 14, 7, -1, 0};
      unsigned int mas = (instr->cs1 & (0x7f << shift[chn])) >> shift[chn];

      /* There's no point in printing zero MAS.  */
      if (mas)
        my_printf (", mas = 0x%x", mas);
    }

  /* Print MRGC predicates if they make sense.  */
  if (match->alopf == MERGE
      || match->alopf == ALOPF11_MERGE
      || match->alopf == ALOPF21_MERGE)
    print_predicates (info, chn, 1);

  /* Print RLP predicates.  */
  print_predicates (info, chn, 0);
  end_syllable ();
}

static void
print_elp (disassemble_info *info, int idx, unsigned int elp,
	   int explicitly_needed)
{
  /* One may wish to print ELP out either if it takes part in the formation of
     an output %predX (explicitly needed) or if has some valuable side effect.
     The latter is likely to take place in case of `pass %rndpredY, @pZ' only
     (see Bug #101471).  */
  if (! explicitly_needed
      && ! ((elp & 0x60) == 0x40
	    && ((elp & 0x1f) >= 1 && (elp & 0x1f) <= 15)))
    return;

  /* FIXME: `indentate ()' can't be called from the outside of this function
     at present since it's not evident if the latter will output anything at
     all and thus indentation will be needed.  */
  indentate ();

  my_printf ("pass ");

  if ((elp & 0x40) == 0)
    {
      if ((elp & 0x3f) == 0)
        my_printf ("%%lcntex");
      else
        {
          int i;
          my_printf ("%%spred");
          for (i = 0; i < 5; i++)
            {
              if (elp & (1 << i))
                my_printf ("%d", i);
            }
        }
    }
  else if ((elp & 0x60) == 0x40)
    {
      if ((elp & 0x1f) == 0)
        my_printf ("%%bgrpred");
      else if ((elp & 0x1f) <= 15)
        my_printf  ("%%rndpred%d", elp & 0x1f);
      else
        my_printf ("<invalid pred>");
    }
  else
    my_printf ("%%pred%d", elp & 0x1f);

  my_printf (", @p%d", idx);
}

static void
print_clp (disassemble_info *info, int idx, unsigned int clp)
{
  static const char *names[4] = {"andp", "landp", "reserved", "movep"};
  unsigned int opc = (clp & 0xc000) >> 14;
  unsigned int neg0 = (clp & 0x2000) >> 13;
  unsigned int p0 = (clp & 0x1c00) >> 10;
  unsigned int neg1 = (clp & 0x0200) >> 9;
  unsigned int p1 = (clp & 0x01c0) >> 6;
  unsigned int vdst = (clp & 0x0020) >> 5;
  unsigned int pdst = (clp &0x001f);

  my_printf ("%s %s@p%d, %s@p%d, @p%d", names[opc], neg0 ? "~" : "", p0,
             neg1 ? "~" : "", p1, idx);

  if (vdst)
    {
      indentate ();
      my_printf ("pass @p%d, %%pred%d", idx, pdst);
    }
}


static void
print_pls_syllables (disassemble_info *info)
{
  int i;
  /* The first pass is always needed.  */
  int once_more = 1;
  const struct unpacked_instr *instr = &unpacked_instr;
  int need[7] = {0, 0, 0, 0, 0, 0, 0};

  for (i = 0; i < 3; i++)
    {
      /* Check if PLS{i}.{C,M}LP.vdst is set to 1 and mark the corresponding
         pdst local predicates as needed. Note that for correctly encoded MLP
         operations it should be set to one unconditionally. Should I allow for
         incorrectly encoded MLPs?  */
      if (instr->pls_present[i] && (instr->pls[i] & 0x20))
        need[4 + i] = 1;
    }

  while (once_more)
    {
      /* It will be set to 1 if some of `@p{i >= 4}' dependencies are
         found.  */
      once_more = 0;

      for (i = 0; i < 3; i++)
        {
          if (need[4 + i] == 1)
            {
              int p0;
              int p1;

              p0 = (instr->pls[i] & 0x1c00) >> 10;
              p1 = (instr->pls[i] & 0x1c0) >> 6;

              /* Note that only `@p0, . . ., @p6' are allowed, while `@p7' can
                 be formally encoded as well.  */
              if (p0 < 7 && need[p0] == 0)
                {
                  need[p0] = 1;

                  /* One more pass will be needed to analyze dependencies of
                     this predicate.  */
                  if (p0 >= 4)
                    once_more = 1;
                }

              if (p1 < 7 && need[p1] == 0)
                {
                  need[p1] = 1;

                  if (p1 >= 4)
                    once_more = 1;
                }

              /* Make it clear that the dependencies of `p@{4 + i}' have been
                 analyzed.  */
              need[4 + i] = 2;
            }
        }
    }

  for (i = 2; i >= 0; i--)
    {
      char syll_name[32];
      if (! instr->pls_present[i])
        continue;

      snprintf (syll_name, sizeof(syll_name), "PLS%d", i);
      print_syllable (syll_name, 0, instr->pls[i]);

      if (i < 2)
        {
	  print_elp (info, 2 * i, (instr->pls[i] & 0x7f000000) >> 24,
		     need[2 * i]);
	  print_elp (info, 2 * i + 1, (instr->pls[i] & 0x007f0000) >> 16,
		     need[2 * i + 1]);
        }

      if (need[4 + i])
        {
          indentate ();
          print_clp (info, 4 + i, instr->pls[i] & 0x0000ffff);
        }

      end_syllable ();
    }
}

static void
print_aas(disassemble_info *info, int idx)
{
  static const char *names[] = {"reserved", "movab", "movah", "movaw", "movad",
                                "movaq", "reserved", "movaqp"};

  const struct unpacked_instr *instr = &unpacked_instr;
  unsigned short aa0f1 = instr->aas[idx / 2];
  /* Note the idiotic order of DST0 and DST1 in AA0F1.  */
  unsigned short dst = (aa0f1 >> ((1 - idx % 2) * 8)) & 0xff;
  unsigned short aa2f1 = instr->aas[idx + 2];
  unsigned short be = (aa2f1 & 0x8000) >> 15;
  unsigned short opc = (aa2f1 & 0x7000) >> 12;
  unsigned short area = (aa2f1 & 0x07c0) >> 6;
  unsigned short ind = (aa2f1 & 0x003e) >> 1;
  unsigned short am = aa2f1 & 0x0001;

  /* Follow the same strange order of fields as in LDIS.  */
  my_printf ("%s,%d area = %d, ind = %d, am = %d, be = %d, ",
             names[opc], idx, area, ind, am, be);

  print_dst (info, dst, opc < 5 ? DOUBLE : QUAD);
}

static void
print_hs (disassemble_info *info)
{
  const struct unpacked_instr *instr = &unpacked_instr;
  unsigned int hs = instr->hs;
  /* Mimic the behaviour of LDIS. I wonder what this colon is needed for. */
  print_syllable ("HS", 0, hs);

  if (hs & 0x00000400)
    {
      indentate ();
      my_printf ("loop_mode");
    }

  if (hs & 0x00000380)
    {
      unsigned int nop = (hs & 0x00000380) >> 7;

      indentate ();
      my_printf ("nop %d", nop);
    }

  end_syllable ();
}


static void
print_ctcond (disassemble_info *info, unsigned int ctcond)
{
  /* I don't want to call it `ct' as it's done in C.17.1.2 of iset-vX.single
     because it looks rather confusing.  */
  unsigned int cond_type = (ctcond & 0x1e0) >> 5;
  unsigned int psrc = (ctcond & 0x01f);

  /* There's no point in printing `? always' condition.  */
  if (cond_type == 1)
    return;

  my_printf (" ? ");

  /* These types of conditions imply the negation of the consequent predicate
     register.  */
  if (cond_type == 3
      || cond_type == 7
      || cond_type == 0xe)
    my_printf ("~");

  /* These types of conditions involve a (probably negated) predicate
     register. */
  if (cond_type == 2
      || cond_type == 3
      || cond_type == 6
      || cond_type == 7
      || cond_type == 0xe
      || cond_type == 0xf)
    my_printf ("%%pred%d", psrc);

  if (cond_type == 4
      || cond_type == 6
      || cond_type == 0xe)
    {
      if (cond_type == 6
          || cond_type == 0xe)
        my_printf (" || ");

      my_printf ("%%LOOP_END");
    }

  if (cond_type == 5
      || cond_type == 7
      || cond_type == 0xf)
    {
      if (cond_type == 7
          || cond_type == 0xf)
        my_printf (" && ");
      my_printf ("%%NOT_LOOP_END");
    }

  if (cond_type == 8)
    {
      my_printf ("%%MLOCK");
      /* It's not clearly said in C.17.1.2 of iset-vX.single if the uppermost
         fourth bit in `psrc' has any meaning at all.  */
      if (psrc & 0xf)
        {
          static const int conv[] = {0, 1, 3, 4};
          int i;

          my_printf (" || %%dt_al");
          for (i = 0; i < 4; i++)
            {
              if (psrc & (1 << i))
                my_printf ("%d", conv[i]);
            }
        }
    }

  /* `lock_cond || pl_cond' control transfer conditions.  */
  if (cond_type == 9)
    {
      unsigned int type = (psrc & 0x18) >> 3;
      if (type == 0)
        {
          static const int cmp_num_to_alc[] = {0, 1, 3, 4};
          unsigned int cmp_num = (psrc & 0x6) >> 1;
          unsigned int neg = psrc & 0x1;

          my_printf ("%%MLOCK || %s%%cmp%d", neg ? "~" : "",
                     cmp_num_to_alc[cmp_num]);
        }
      else if (type == 1)
        {
          unsigned int cmp_jk = (psrc & 0x4) >> 2;
          unsigned int negj = (psrc & 0x2) >> 1;
          unsigned int negk = psrc & 0x1;

          my_printf ("%%MLOCK || %s%%cmp%d || %s%%cmp%d",
                     negj ? "~" : "", cmp_jk == 0 ? 0 : 3,
                     negk ? "~" : "", cmp_jk == 0 ? 1 : 4);
        }
      else if (type == 2)
        {
          unsigned int clp_num = (psrc & 0x6) >> 1;
          unsigned int neg = psrc & 0x1;

          my_printf ("%%MLOCK || %s%%clp%d", neg ? "~" : "", clp_num);
        }
    }

  if (cond_type >= 0xa && cond_type <= 0xd)
    my_printf ("<reserved condition type>");

}

static void
print_ct (disassemble_info *info, unsigned int ctop, unsigned int ctcond)
{
  const struct unpacked_instr *instr = &unpacked_instr;

  /* I don't want to call it `ct' as it's done in C.17.1.2 of iset-vX.single
     because it looks rather confusing.  */
  unsigned int cond_type = (ctcond & 0x1e0) >> 5;

  /* There's no point in printing any CT instruction if it's blocked by
     `? never' condition. Note that there's always some sort of CT in any SS. */
  if (cond_type == 0)
    return;

  /* This is a CALL in fact. There's no point in printing a duplicating CT.  */
  if (instr->cs1_present
      && ((instr->cs1 & 0xf0000000) >> 28) == 5)
    return;

  /* According to C.17.1.1 in iset-vX.single `SS.ctop == 0' encodes an immediate
     branch (i.e. IBRANCH). Provided that this instruction requires also CS0
     with IBRANCH encoded within it, it makes little sense to print it here.
     Moreover, if CS0 is missing or `CS0.opc != IBRANCH' this operation should
     take no effect.  */
  if (ctop == 0)
    return;

  /* According to C.17.1.1 we are left with a "prepared" `ct %ctpr<ctop={1,2,
     3}>' under some condition different from "never".  */
  indentate ();
  my_printf ("ct %%ctpr%d", ctop);

  print_ctcond (info, ctcond);
}

static void
print_ss (disassemble_info *info)
{
  const struct unpacked_instr *instr = &unpacked_instr;
  unsigned int ss = instr->ss;

  /* These fields are present both for `type == {0, 1}'.  */
  unsigned int ipd = (ss & 0xc0000000) >> 30;
  unsigned int rp_opc_hi = (ss & 0x08000000) >> 27;
  unsigned int rp_opc_lo = (ss & 0x02000000) >> 25;
  unsigned int rp_opc = (rp_opc_hi << 1) | rp_opc_lo;
  unsigned int type = (ss & 0x00100000) >> 20;
  unsigned int ctop = (ss & 0x00000c00) >> 10;
  unsigned int ctcond = (ss & 0x000001ff);

  print_syllable ("SS", 0, ss);

  /* When printing out SS instructions try to follow the same order as LDIS at
     least for `SS.type == 0'. To find out this order I customized
     `SS == 0xfffeffff' to get all possible fields in `SS.type == 0' case and
     disassembled it with LDIS.  */

  /* CT deserves its own output function due to the complexity of CTCOND.  */
  print_ct (info, ctop, ctcond);

  if (ipd)
    {
      indentate ();
      my_printf ("ipd %d", ipd);
    }

  if (type == 0)
    {
      unsigned int eap = (ss & 0x20000000) >> 29;
      unsigned int bap = (ss & 0x10000000) >> 28;

      if (eap)
        {
          indentate ();
          my_printf ("eap");
        }

      if (bap)
        {
          indentate ();
          my_printf ("bap");
        }
    }

  /* According to iset-v5.single this should be output both for `type = {0,1}',
     shouldn't it? Beware of some changes between different versions of
     instruction set related to these instructions! There used to be no CRP
     field on elbrus-v1, while on elbrus-v2 SRP and CRP used to be treated as
     two independent instructions.  */
  if (rp_opc)
    {
      static const char *rp_names[] = {"", "crp", "srp", "slrp"};

      indentate ();
      my_printf ("%s", rp_names[rp_opc]);

    }

  if (type == 0)
    {
      unsigned int vfdi = (ss & 0x04000000) >> 26;
      unsigned int abg = (ss & 0x01800000) >> 23;
      unsigned int abn = (ss & 0x00600000) >> 21;
      unsigned int abp = (ss & 0x000c0000) >> 18;
      unsigned int alc = (ss & 0x00030000) >> 16;

      if (vfdi)
        {
          indentate ();
          my_printf ("vfdi");
        }

      if (abg)
        {
          indentate ();
          my_printf ("abg abgi=%d, abgd=%d", abg >> 1, abg & 0x1);
        }

      if (abn)
        {
          indentate ();
          my_printf ("abn abnf=%d, abnt=%d", abn >> 1, abn & 0x1);
        }

      if (abp)
        {
          indentate ();
          my_printf ("abp abpf=%d, abpt=%d", abp >> 1, abp & 0x1);
        }

      if (alc)
        {
          indentate ();
          my_printf ("alc alcf=%d, alct=%d", alc >> 1, alc & 0x1);
        }
    }
  else
    {
    }

  end_syllable ();
}

static void
print_cs0 (disassemble_info *info, bfd_vma instr_addr)
{
  typedef enum
  {
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

  static const char *cs0_names[] = {"", "ibranch", "pref", "puttsd",
                                    "done", "hret", "glaunch", "disp", "sdisp",
				    "gettsd", "ldisp", "return"};
  /* This is a copy of Table B.4.1 in `iset-v6.single'.  */
  static cs0_type cs0_ops[4][4] =  {
    {IBRANCH, PREF, PUTTSD, DONE},
    {DISP, NOTHING, SDISP, GETTSD},
    {DISP, LDISP, SDISP, GETTSD},
    {DISP, NOTHING, SDISP, RETURN}
  };
  const struct unpacked_instr *instr = &unpacked_instr;
  unsigned int cs0 = instr->cs0;

  /* Note that `ctpr' and `ctp_opc' fields are present both in C0F1 and
     C0F2.  */
  unsigned int ctpr = (cs0 & 0xc0000000) >> 30;
  unsigned int ctp_opc = (cs0 & 0x30000000) >> 28;
  unsigned int param_type = (cs0 & 0x00000007);
  cs0_type type = cs0_ops[ctpr][ctp_opc];

  if (type == RETURN && param_type == 1)
    type = GETTSD;
  else if (type == DONE)
    {
      if (param_type == 3)
	type = HRET;
      else if (param_type == 4)
	type = GLAUNCH;
    }


  print_syllable ("CS0", 0, instr->cs0);

  if (type == IBRANCH || type == DONE || type == HRET || type == GLAUNCH)
    {
      /* IBRANCH, DONE, HRET and GLAUNCH are special because they require SS
	 to be properly encoded.  */
      if (! instr->ss_present
          /* SS.ctop should be equal to zero for IBRANCH, DONE, HRET and
	     GLAUNCH (see C.17.1.1, note that they don't mention the latter two
	     instructions there which is probably an omission ).  */
          || (instr->ss & 0x00000c00))
        {
          indentate ();
          my_printf ("invalid %s", cs0_names[type]);
        }
      /* Don't output either of the aforementioned instructions under "never"
	 condition. Don't disassemble CS0 being a part of HCALL. Unlike ldis
	 HCALL is currently disassembled on behalf of CS1.  */
      else if (instr->ss & 0x1ff
	       && !(instr->cs1_present
		    /* CS1.opc == CALL */
		    && (instr->cs1 & 0xf0000000) >> 28 == 5
		    /* CS1.param.ctopc == HCALL  */
		    && (instr->cs1 & 0x380) >> 7 == 2))
        {
          indentate ();
          my_printf ("%s", cs0_names[type]);

          if (type == IBRANCH)
            {
              /* C0F2 has `disp' field. In `C0F1' it's called `param'. Is this
                 the only difference between these two formats?  Funnily enough,
                 DONE is also C0F2 and thus has `disp', though it obviously
                 makes no sense for it.  */
              unsigned int disp = (cs0 & 0x0fffffff);
              /* Calculate a signed displacement in bytes. I wonder if it
                 should be actually printed out.  */
              int sdisp = ((int) (disp << 4)) >> 1;
              /* FIXME: this way I ensure that it'll work correctly
                 both on 32 and 64-bit hosts.  */
              my_printf (" 0x%llx", (unsigned long long) (instr_addr + sdisp));
            }

          print_ctcond (info, instr->ss & 0x1ff);
        }
    }
  else
    {
      indentate ();
      /* Note that according to Table B.4.1 it's possible to obtain
	 `gettsd %ctpr{1,2} with an invalid value for CS0.param.type.  */
      if (type == GETTSD && param_type != 1)
	my_printf ("invalid ");

      my_printf ("%s ", cs0_names[type]);

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
        my_printf ("%%ctpr%d", ctpr);

      if (type == SDISP)
        my_printf (", 0x%x", cs0 & 0x1f);
      else if (type == DISP
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
          my_printf ("%s0x%llx", type == PUTTSD ? "" : ", ",
                     /* FIXME: this way I ensure that it'll work correctly
                        both on 32 and 64-bit hosts.  */
                     (unsigned long long) (instr_addr + sgnd_disp));
        }

      if (type == PREF)
        {
          /* LDIS doesn't currently bother about producing a target label for
             PREF, it just stupidly dumps its parameters in their immediate
             form. Do the same for now.  */

          unsigned int pdisp = (instr->cs0 & 0x0ffffff0) >> 4;
          unsigned int ipd = (instr->cs0 & 0x00000008) >> 3;
          unsigned int prefr = instr->cs0 & 0x00000007;

          my_printf ("%%ipr%d, disp = 0x%x, ipd = %d", prefr, pdisp, ipd);
        }
    }

  end_syllable ();
}

static void
print_cs1 (disassemble_info *info)
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

  const struct unpacked_instr *instr = &unpacked_instr;
  unsigned int cs1 = instr->cs1;
  unsigned int opc = (cs1 & 0xf0000000) >> 28;

  print_syllable ("CS1", 0, cs1);

  if (opc == SETR0
      || opc == SETR1
      || opc == SETBR)
    {
      unsigned int setbp = (cs1 & 0x08000000) >> 27;
      unsigned int setbn = (cs1 & 0x04000000) >> 26;

      /* Try to follow the same order of these instructions as in LDIS.
         Presumably `vfrpsz' should come first, while `setbp' should be placed
         between `setwd' and `setbn', but this is to be verified. I don't have
         a binary with these commands by hand right now.  */

      if (opc == SETR1)
        {
          if (! instr->lts_present[0])
            {
              indentate ();
              my_printf ("<bogus vfrpsz>");
            }
          else
            {
              /* Find out if VFRPSZ is always encoded together with SETWD. This
                 seems to be the case even if no SETWD has been explicitly
                 specified.  */
              unsigned int rpsz = (instr->lts[0] & 0x0001f000) >> 12;
              indentate ();
              my_printf ("vfrpsz rpsz = 0x%x", rpsz);
            }
        }

      if (opc == SETR0
          || opc == SETR1)
        {
          if (! instr->lts_present[0])
            {
              indentate ();
              my_printf ("<bogus setwd>");
            }
          else
            {
              unsigned int lts0 = instr->lts[0];
              unsigned int wsz = (lts0 & 0x00000fe0) >> 5;
              unsigned int nfx = (lts0 & 0x00000010) >> 4;

              indentate ();
              my_printf ("setwd wsz = 0x%x, nfx = 0x%x", wsz, nfx);

              if (mcpu >= 3)
                {
                  /* DBL parameter of SETWD was added only starting from
                     elbrus-v3.  */
                  unsigned int dbl = (lts0 & 0x00000008) >> 3;
                  my_printf (", dbl = 0x%x", dbl);
                }
            }
        }

      if (setbn)
        {
          unsigned int rcur = (cs1 & 0x0003f000) >> 12;
          unsigned int rsz = (cs1 & 0x00000fc0) >> 6;
          unsigned int rbs = cs1 & 0x0000003f;

          indentate ();
          my_printf ("setbn rbs = 0x%x, rsz = 0x%x, rcur = 0x%x",
                     rbs, rsz, rcur);
        }

      if (setbp)
        {
          unsigned int psz = (cs1 & 0x007c0000) >> 18;

          indentate ();
          my_printf ("setbp psz = 0x%x", psz);
        }
    }
  else if (opc == SETEI)
    {
      /* Verify that CS1.param.sft = CS1.param[27] is equal to zero as required
         in C.14.3.  */
      unsigned int sft = (cs1 & 0x08000000) >> 27;
      unsigned int eir = (cs1 & 0x000000ff);

      indentate ();
      if (sft)
	my_printf ("%s", mcpu >= 2 ? "setsft" : "unimp");
      else
	my_printf ("setei 0x%x", eir);
    }
  else if (opc == WAIT)
    {
      unsigned int ma_c = (cs1 & 0x00000020) >> 5;
      unsigned int fl_c = (cs1 & 0x00000010) >> 4;
      unsigned int ld_c = (cs1 & 0x00000008) >> 3;
      unsigned int st_c = (cs1 & 0x00000004) >> 2;
      unsigned int all_e = (cs1 & 0x00000002) >> 1;
      unsigned int all_c = cs1 & 0x00000001;

      indentate ();
      my_printf ("wait ");

      if (mcpu >= 5)
        {
          /* `sa{l,s}' fields are `elbrus-v5'-specific. Each of them makes sense
             only in the presence of `{ld,st}_c == 1' respectively.  */
          if (ld_c)
            {
              unsigned int sal = (cs1 & 0x00000100) >> 8;
              my_printf ("sal = %d, ", sal);
            }

          if (st_c)
            {
              unsigned int sas = (cs1 & 0x00000080) >> 7;
              my_printf ("sas = %d, ", sas);
            }
        }

      if (mcpu >= 2)
        {
          /* `trap' field was introduced starting from `elbrus-v2'.  */
          unsigned int trap = (cs1 & 0x00000040) >> 6;
          my_printf ("trap = %d, ", trap);
        }

      my_printf ("ma_c = %d, fl_c = %d, ld_c = %d, st_c = %d, all_e = %d, "
                 "all_c = %d", ma_c, fl_c, ld_c, st_c, all_e, all_c);
    }
  else if (opc == CALL)
    {
      unsigned int ctop = (instr->ss & 0x00000c00) >> 10;
      /* In C.17.4 it's said that other bits in CS1.param except for the
	 seven lowermost ones are ignored.  */
      unsigned int wbs = cs1 & 0x7f;

      indentate ();
      if (ctop)
        {
          my_printf ("call %%ctpr%d, wbs = 0x%x", ctop, wbs);
          print_ctcond (info, instr->ss & 0x1ff);
        }
      else
	{
	  unsigned int cs1_ctopc = (cs1 & 0x380) >> 7;
	  /* CS1.param.ctpopc == HCALL. CS0 is required to encode HCALL.  */
	  if (cs1_ctopc == 2 && instr->cs0_present)
	    {
	      unsigned int cs0 = instr->cs0;
	      unsigned int cs0_opc = (cs0 & 0xf0000000) >> 28;
	      /* CS0.opc == HCALL, which means
		 CS0.opc.ctpr == CS0.opc.ctp_opc == 0 */
	      if (cs0_opc == 0)
		{
		  unsigned int hdisp = (cs0 & 0x1e) >> 1;
		  my_printf ("hcall 0x%x, wbs = 0x%x", hdisp, wbs);
		  print_ctcond (info, instr->ss & 0x1ff);
		}
	    }
	  else
	    my_printf ("<bogus call>");
	}
    }
  else if (opc == MAS_OPC)
    {
#if defined ENABLE_E2K_ENCODINGS
      /* Note that LDIS doesn't print it out as a standalone instruction.  */
      unsigned int mas = cs1 & 0x0fffffff;

      indentate ();
      my_printf ("mas 0x%x", mas);
#endif /* ENABLE_E2K_ENCODINGS  */
    }
  else if (opc == FLUSHR)
    {
      /* . . . these stupid engineers off! FLUSHR seems to be responsible for
         encoding both `flushr' and `flushc'. Moreover, according to their
         logic it should be possible to encode them simultaneously.  */

      /* Check for `CS1.param.flr'.  */
      if (cs1 & 0x00000001)
        {
          indentate ();
          my_printf ("flushr");
        }

      /* Check for `CS1.param.flc'.  */
      if (cs1 & 0x00000002)
        {
          indentate ();
          my_printf ("flushc");
        }
    }
  else if (opc == BG)
    {
      /* Hopefully, `vfbg' is the only instruction encoded by BG. I'm currently 
         unable to find other ones in `iset-v5.single' at least . . .  */
      unsigned int chkm4 = (cs1 & 0x00010000) >> 16;
      unsigned int dmask = (cs1 & 0x0000ff00) >> 8;
      unsigned int umsk = cs1 & 0x000000ff;

      indentate ();
      /* Print its fields in the order proposed in C.14.10.  */
      my_printf ("vfbg umask = 0x%x, dmask = 0x%x, chkm4 = 0x%x",
                 umsk, dmask, chkm4);
    }
  else
    {
      indentate ();
      my_printf ("unimp");
    }

  end_syllable ();
}


#if defined ENABLE_E2K_ENCODINGS
static void
print_cds (disassemble_info *info, int idx)
{
  int i;
  const struct unpacked_instr *instr = &unpacked_instr;
  /* FIXME: this approach is not safe from the point of view of endianness.  */
  unsigned short *cds = (unsigned short *) &instr->cds[idx];
  char syll_name[5];

  snprintf (syll_name, 5, "CDS%d", idx);
  print_syllable (syll_name, 0, instr->cds[idx]);


  for (i = 1; i >= 0; i--)
    {
      unsigned short mask = (cds[i] & 0x3c00) >> 10;

      if (mask == 0)
        continue;

      unsigned short opc = (cds[i] & 0xc000) >> 14;
      unsigned short neg = (cds[i] & 0x0380) >> 7;
      unsigned short pred = cds[i] & 0x7f;
      unsigned short num = pred & 0x1f;

      indentate ();
      my_printf ("%s,cd%d%d ", opc < 2 ? "rlp" : "mrgc", idx, 1 - i);


      if ((pred & 0x60) == 0x40)
        my_printf ("%%pcnt%hd", num);
      else if ((pred & 0x60) == 0x60)
        my_printf ("%%pred%hd", num);
      else
        my_printf ("<invalid predicate>");

      int j;
      for (j = 0; j < 3; j++)
        {
          if (mask & (1 << j))
            {
              int alc = ((opc & 0x1) ? 3 : 0) + j;
              my_printf (", %s>alc%d", (neg & (1 << j)) ? "~" : "", alc);
            }
        }
    }

  end_syllable ();
}

#endif /* ENABLE_E2K_ENCODINGS  */

static void
print_instr_e2k (bfd_vma instr_addr, disassemble_info *info)
{
  int i;
  const struct unpacked_instr *instr = &unpacked_instr;

  my_printf ("\n");

  print_hs (info);

  if (instr->ss_present)
    print_ss (info);

  for (i = 0; i < 6; i++)
    {
      if (instr->als_present[i])
        print_alf (info, i);
    }

  if (instr->cs0_present)
    print_cs0 (info, instr_addr);

  /* Note that `ALES{5,2}' are always marked as allocated together, so it's
     enough to check for only one of them.  */
  if ((instr->ales_present[5] & 0x2) == 0x2)
    {
      /* Don't print the name of either of `ALES{5,2}' unless it has been marked
         as present in HS. However, always print the contents of these syllables
         if they are allocated.  */
      print_syllable ((instr->ales_present[2] & 0x1) ? "ALES2" : "", 1,
                      instr->ales[2]);
      end_syllable ();

      print_syllable ((instr->ales_present[5] & 0x1) ? "ALES5" : "", 1,
                      instr->ales[5]);
      end_syllable ();
    }

  if (instr->cs1_present)
    print_cs1 (info);

  for (i = 0; i < 5; i++)
    {
      /* We've already taken care of ALES2 above.  */
      if (i == 2)
        continue;

      if (instr->ales_present[i])
        {
          char syll_name[6];
          snprintf (syll_name, 6, "ALES%d", i);
          print_syllable (syll_name, 1, instr->ales[i]);
          end_syllable ();
        }
    }

    for (i = 0; i < 6; i++)
    {
      if (instr->aas_present[i])
        {
          char syll_name[5];
          snprintf (syll_name, 5, "AAS%d", i);
          print_syllable (syll_name, 1, instr->aas[i]);

          if (i >= 2)
            {
              indentate ();
              print_aas (info, i - 2);
            }

          end_syllable ();
        }
    }

    if (instr->half_gap_present)
      {
        print_syllable ("GAP", 1, instr->half_gap);
        end_syllable ();
      }

    for (i = 0; i < 16 && instr->gap_present[i] != 0; i++)
      {
        if (instr->gap_present[i])
          {
            print_syllable ("GAP", 0, instr->gap[i]);
            end_syllable ();
          }
      }

    for (i = 3; i >= 0; i--)
      {
        if (instr->lts_present[i])
          {
            char syll_name[32];
            snprintf (syll_name, sizeof(syll_name), "LTS%d", i);
            print_syllable (syll_name, 0, instr->lts[i]);
            end_syllable ();
          }
      }

    print_pls_syllables (info);

#if defined ENABLE_E2K_ENCODINGS
    /* Some people believe that CDSes are redundant in "release" mode (see
       Bug #88977).  */
    for (i = 2; i >= 0; i--)
      {
        if (instr->cds_present[i])
          print_cds (info, i);
      }
#endif /* !defined ENABLE_E2K_ENCODINGS  */
}

static void
print_apb_instr (disassemble_info *info)
{
  int i;
  const struct unpacked_instr *instr = &unpacked_instr;

  my_printf ("\n");

  /* It seems that API_L should be always followed by API_R according to 6.8 in
     `iset-vX.single'. However, because it's not stated there for certain, one
     may still doubt . . . If that wasn't the case, however, how would
     disassembler distinguish them?  */

  for (i = 0; i < 2; i++)
    {
      const unsigned int *api = i == 0 ? instr->api_l : instr->api_r;
      unsigned int ct_or_dpl = (api[0] & 0x80000000) >> 31;
      unsigned int si = (api[0] & 0x40000000) >> 30;
      unsigned int fmt = (api[0] & 0x0e000000) >> 25;

      print_syllable (i == 0 ? "API_L" : "API_R", 0, api[0]);

      indentate ();
      my_printf ("apb %s=%d", i == 0 ? "ct" : "dpl", ct_or_dpl);

      /* FIXME: in iset-vX.single they refer to `fmt == "none"'. However,
         "none" which is typically believed to be 0xc0 across this document
         can't be obviously encoded in 3 bytes. For now stupidly believe that
         it means zero in context of APB.  */
      if (mcpu <= 1 && fmt != 0)
        /* "Secondary index access" was removed starting from elbrus-v2.  */
        my_printf (", si=%d", si);

      if (fmt != 0)
        {
          unsigned int dcd = (api[0] & 0x30000000) >> 28;
          my_printf (", dcd=%d", dcd);
        }

      my_printf (", fmt=%d", fmt);

      if (fmt != 0)
        {
          unsigned int mrng = (api[0] & 0x01f00000) >> 20;
          unsigned int d = (api[0] & 0x000f8000) >> 15;
          unsigned int asz = (api[0] & 0x000000e0) >> 5;
          /* I don't call it `abs' as it's called in `iset-vX.single' since
             gcc-4.6.3 and more recent ones complain that it shadows the global
             `abs ()' function provided that `-Wshadow' is supplied.  */
          unsigned int area_base = (api[0] & 0x0000001f);

          my_printf (", mrng=%d", mrng);
          my_printf (", d=%d", d);

          /* APS.si makes no sense for `elbrus-v{X>=2}'.  */
          if (mcpu >= 2 || si == 0)
            {
              unsigned int incr = (api[0] & 0x00007000) >> 12;
              unsigned int ind = (api[0] & 0x00000f00) >> 8;

              my_printf (", incr=%d", incr);
              my_printf (", ind=%d", ind);

            }
          else
            {
              unsigned int be = (api[0] & 0x00004000) >> 14;
              unsigned int am = (api[0] & 0x00002000) >> 13;
              unsigned int area = (api[0] & 0x00001f00) >> 8;

              my_printf (", be=%d", be);
              my_printf (", am=%d", am);
              my_printf (", area=%d", area);
            }

          my_printf (", asz=%d", asz);
          my_printf (", abs=%d", area_base);

          /* FIXME: should DISP be printed for `fmt="none"'?  */
          my_printf (", disp=0x%x", api[1]);
        }

      end_syllable ();

      /* Print the second syllable in a pair.  */
      print_syllable ("", 0, api[1]);
      end_syllable ();
    }
}

static void
build_hash_table (void)
{
  /* FIXME: this way I temporarely get rid of the fields which are inappropriate
     to disassembler.  */
#define parse_alf_args     NULL
#define merge_alopf_simple NULL
#define merge_alopf11      NULL
#include "e2k-opc.h"

  if (build_hash_table_stage == 0)
    {
      if (hash_buf != NULL)
        free (hash_buf);

      memset (opcode_hash_table, 0, sizeof (opcode_hash_table));
      hash_buf = malloc (sizeof (* hash_buf) * num_opcodes);
      return;
    }
}


/* Print one instruction from MEMADDR on INFO->STREAM.  */
int
print_insn_e2k (bfd_vma memaddr, disassemble_info *info)
{
  bfd_byte buffer[256];
  int status;
  size_t command_length;
  static int initialized;

  /* This lets me stupidly shut Bug #88528 up for now . . .  */
  if (! initialized
      /* Recall how `bfd_arch_info_type.mach' is set for E2K.  */
      || info->mach / 3 != mcpu)
    {
      mcpu = info->mach / 3;

      build_hash_table_stage = 0;
      num_opcodes = 0;
      build_hash_table ();

      build_hash_table_stage = 1;
      num_opcodes = 0;
      build_hash_table ();

      initialized = 1;
    }

  /* Don't even try to disassemble if the available address range doesn't fit
     the most short possible instruction or is not properly aligned.  */
  if (info->stop_vma - memaddr < 8
      || (memaddr & 0x7) != 0)
    {
      /* Skip remaining bytes. Hopefully, we'll resume disassembly starting
         from a more meaningful symbol.  */
      command_length = info->stop_vma - memaddr;
      info->bytes_per_line = command_length;
      return command_length;
    }

  /* I can't read instruction bytes with a reserve here since this may cause
     a memory access error. Therefore, read just one byte . . .  */
  status = (*info->read_memory_func) (memaddr, buffer, 1, info);
  if (status != 0)
    {
      (*info->memory_error_func) (status, memaddr, info);
      return -1;
    }

  /* . . . and use it to determine the current instruction's length as if it
     were synchronous. FIXME: what's going to happen if it's not? Previously
     when reading bytes with a surplus I used `UnpackInstruction' to determine
     COMMAND_LENGTH. I wonder if this function is capable of distinguishing
     between synchronous and asynchronous instructions . . .  */
  command_length = (((buffer[0] >> 4) & 0x7) + 1) * 2;
  command_length *= 4;

  /* FIXME: the underlying condition makes no sense provided that
     COMMAND_LENGTH is bounded with 64 bytes.  */
  if (command_length > sizeof (buffer))
    return -1;

  if (/* Is there enough space in the available address range to hold a
         presumable synchronous instruction?  */
      info->stop_vma - memaddr >= command_length
      /* If so, read its remaining bytes.  */
      && (*info->read_memory_func) (memaddr + 1, &buffer[1],
                                    command_length - 1, info) == 0
      /* If that succeeded, try to unpack it. In case of success, it's a valid
         synchronous instruction.  */
      && unpack_instr (buffer))
    print_instr_e2k (memaddr, info);
  else if (info->stop_vma - memaddr < 16)
    {
      /* There's no point in trying to interpret this bogus sequence of bytes
         as an APB instruction if it's not sufficiently long.  */
      command_length = info->stop_vma - memaddr;
      info->bytes_per_line = command_length;
      return command_length;
    }
  else
    {
      struct unpacked_instr *instr = &unpacked_instr;

      /* Note that the read above may have failed if we mistakenly tried to
         load a too lengthy invalid synchronous instruction. Therefore, re-read
         everything from the very beginning.  */

      command_length = 16;
      status = (*info->read_memory_func) (memaddr, buffer, 16, info);

      if (status != 0)
        {
          (*info->memory_error_func) (status, memaddr, info);
          return -1;
        }

      memcpy (instr->api_l, &buffer[0], 8);
      memcpy (instr->api_r, &buffer[8], 8);

      print_apb_instr (info);
    }


  /* When disassembling via OBJDUMP the latter prints out instruction bytes
     "by default" unlike GDB. This setting makes it print all of them in a
     single line unless `--insn-width=X' option has been explicitly specified.
     Without that `X = 4' bytes will be printed per line by default and each
     quad will be prefixed by its `ADDRESS:' which will make OBJDUMP output
     unrecognizable by `linux-mcst-3.14/scripts/recordmcount.pl' (that was one
     of the reasons for Bug #85734).  */
  info->bytes_per_line = command_length;
  return command_length;
}

