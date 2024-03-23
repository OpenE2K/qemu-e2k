/* E2K disassembler
   Copyright (C) 2022 Free Software Foundation, Inc.

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
   along with this program; see the file COPYING3. If not,
   see <http://www.gnu.org/licenses/>.  */

#include "qemu/osdep.h"
#include "qemu/bitops.h"
#include "libiberty.h"
#include "disas/dis-asm.h"
#include "disas/e2k.h"

#include <stdint.h>

enum ctpr_type
{
  CTPR_NONE,
  CTPR_DISP,
  CTPR_APB,
  CTPR_SYS,
  CTPR_RET,
  CTPR_MOVTD,
};

struct ctpr_info
{
  bfd_vma target;
  enum ctpr_type type;
};

struct syllable
{
  const char *name;
  short index;
  bool is_half;
};

struct e2k_private_data
{
  struct ctpr_info ctpr[4];

  struct syllable syll[32];
  short syll_cur;
  short syll_len;

  bool debug;
  bool aliases;
  bool show_syllables;
  bool show_literal_loc;
  bool show_gpr_size;
  bool show_pair;

  /* Set if controll transfer was decoded.  */
  bool ct_decoded;

  unsigned short version;
  int lines;

  int align;
};

#define implement_me(c) assert((c) && "implement me");

#define print_styled(info, style, ...) \
 (*info->fprintf_func) (info->stream, __VA_ARGS__)

#define print_text(info, ...) \
 print_styled (info, dis_style_text, __VA_ARGS__)

#define print_mnemonic(info, ...) \
 print_styled (info, dis_style_mnemonic, __VA_ARGS__)

#define print_align(info) \
 print_text (info, "        ")

#define print_aligned_text(info, ...) \
 print_align (info); \
 print_styled (info, dis_style_text, __VA_ARGS__)

#define print_aligned_mnemonic(info, ...) \
 print_align (info); \
 print_styled (info, dis_style_mnemonic, __VA_ARGS__)

#define print_operand_alignment(info, width) \
 print_text (info, "%*c", width < 8 ? 8 - width % 8 : 4 - width % 4, ' ')

#define print_sub_mnemonic(info, ...) \
 print_styled (info, dis_style_sub_mnemonic, __VA_ARGS__)

#define print_asm_directive(info, ...) \
 print_styled (info, dis_style_assembler_directive, __VA_ARGS__)

#define print_register(info, ...) \
 print_styled (info, dis_style_register, __VA_ARGS__)

#define print_immediate(info, ...) \
 print_styled (info, dis_style_immediate, __VA_ARGS__)

#define print_address(info, ...) \
 print_styled (info, dis_style_address, __VA_ARGS__)

#define print_address_offset(info, ...) \
 print_styled (info, dis_style_address_offset, __VA_ARGS__)

#define print_symbol(info, ...) \
 print_styled (info, dis_style_symbol, __VA_ARGS__)

#define print_comment(info, ...) \
 print_styled (info, dis_style_comment_start, __VA_ARGS__)

#define print_uimm(info, value) \
 print_immediate (info, "%u", (unsigned int) value)

#define print_hex(info, value) \
 print_immediate (info, "%#x", (unsigned int) (value))

#define print_hex64(info, value) \
 print_immediate (info, "%#lx", (uint64_t) (value))

#define print_named_assign(info, name) \
 print_register (info, "%s", name); \
 print_text (info, "=");

#define print_named_uimm(info, name, value) \
 print_named_assign (info, name); \
 print_uimm (info, value)

#define print_named_hex(info, name, value) \
 print_named_assign (info, name); \
 print_hex (info, value)

#define print_aad(info, index) \
 print_register (info, "%s", e2k_reg_names_aad[index])

#define print_aasti(info, index) \
 print_register (info, "%s", e2k_reg_names_aasti[index])

#define print_aaind(info, index) \
 print_register (info, "%s", e2k_reg_names_aaind[index])

#define print_aaincr(info, index) \
 print_register (info, "%s", e2k_reg_names_aaincr[index]);

#define print_ctpr(info, index) \
 print_register (info, "%s", e2k_reg_names_ctpr[index])

#define print_cmp(info, index) \
 print_register (info, "%s", e2k_reg_names_cmp[index])

#define print_ipr(info, index) \
 print_register (info, "%s", e2k_reg_names_ipr[index])

#define print_not(info) \
 print_text (info, "~")

#define print_preg(info, index) \
 print_register (info, "%s", e2k_reg_names_preg[index])

#define print_pcnt(info, index) \
 print_register (info, "%s", e2k_reg_names_pcnt[index])

#define print_lpred(info, index) \
 print_register (info, "%s", e2k_reg_names_lpred[index])

#define print_rndpred(info, index) \
 print_register (info, "%s", e2k_reg_names_rndpred[index])

#define print_not_preg(info, index) \
 print_not (info); \
 print_preg (info, index)

#define print_loop_end(info) \
 print_register (info, "loop_end")

#define print_not_loop_end(info) \
 print_not (info); \
 print_loop_end (info)

#define print_or(info) \
 print_text (info, " || ")

#define print_and(info) \
 print_text (info, " && ")

static void
print_state_register (struct disassemble_info *info, uint8_t value)
{
  const char *name = e2k_reg_names_sr[value];

  if (name)
    print_register (info, "%s", name);
  else
    {
      print_text (info, "<invalid sr:%02x>", value);
      implement_me(0);
    }
}

static bool
print_gpr (struct disassemble_info *info, uint8_t value, enum gpr_size size)
{
  struct e2k_private_data *pd = info->private_data;

  if (value >= 0xc0 && value < 0xe0)
    return false;

  if (pd->show_gpr_size)
    switch (size)
      {
      case GS_NONE:
      case GS_S:
        print_register (info, "%s", e2k_reg_names_gpr[value]);
        break;
      case GS_D:
        print_register (info, "%s", e2k_reg_names_gpr_d[value]);
        break;
      case GS_X:
        print_register (info, "%s", e2k_reg_names_gpr_x[value]);
        break;
      case GS_Q:
        print_register (info, "%s", e2k_reg_names_gpr_q[value]);
        break;
      case GS_P:
        print_register (info, "%s", e2k_reg_names_gpr_qp[value]);
        break;
      }
  else
    print_register (info, "%s", e2k_reg_names_gpr[value]);

  return true;
}

static void
print_dst (struct disassemble_info *info, uint8_t value, enum gpr_size size)
{
  if (print_gpr (info, value, size))
    return;
  else if (value == 0xdf || value == 0xde)
    print_text (info, "_");
  else
    {
      print_text (info, "<invalid dst:%02x>", value);
      implement_me(0);
    }
}

static bool
print_literal (struct disassemble_info *info, uint8_t value,
               struct e2k_unpacked_bundle *bundle, bool is_label)
{
  struct e2k_private_data *pd = info->private_data;
  unsigned int loc = value & 3;

  if (value >= 0xdc && value <= 0xde)
    {
      uint64_t lit;

      if (!bundle->lts_present[loc] ||
          !bundle->lts_present[loc + 1])
        goto err;

      lit = ((uint64_t) bundle->lts[loc + 1] << 32) | bundle->lts[loc];

      if (is_label)
        (*info->print_address_func)(lit, info);
      else
        print_hex64 (info, lit);

      if (pd->show_literal_loc)
        {
          print_mnemonic (info, " as i64");
          print_text (info, "@%u", loc);
        }
    }
  else if (value >= 0xd8 && value <= 0xdb)
    {
      if (!bundle->lts_present[loc])
        goto err;

      if (is_label)
        (*info->print_address_func)(bundle->lts[loc], info);
      else
        print_hex (info, bundle->lts[loc]);

      if (pd->show_literal_loc)
        {
          print_mnemonic (info, " as i32");
          print_text (info, "@%u", loc);
        }
    }
  else if (value >= 0xd0 && value <= 0xd5)
    {
      uint16_t lit;

      if (!bundle->lts_present[loc])
        goto err;

      lit = bundle->lts[loc] >> (value & 4 ? 16 : 0);

      if (is_label)
        (*info->print_address_func)(lit, info);
      else
        print_hex (info, lit);

      if (pd->show_literal_loc)
        {
          print_mnemonic (info, " as i16");
          print_text (info, "@%u%c", loc, value & 4 ? 'h' : 'l');
        }
    }
  else
    return false;
  return true;

err:
  print_text (info, "<nonexistent literal>");
  return true;
}

static void
print_psrc (struct disassemble_info *info, uint8_t psrc, bool rndpred)
{
  if (psrc == 0)
    print_register (info, "lcntex");
  else if ((psrc & 0xe0) == 0)
    {
      unsigned int i;

      print_register (info, "spred");
      for (i = 0; i < 6; ++i)
        if (psrc & (1 << i))
          print_register (info, "%d", i);
    }
  else if ((psrc & 0xe0) == 0x40)
    {
      if (rndpred)
        print_rndpred (info, psrc & 0x1f);
      else
        print_pcnt (info, psrc & 0x1f);
    }
  else if ((psrc & 0xe0) == 0x60)
    print_preg (info, psrc & 0x1f);
  else
    {
      print_text (info, "<invalid predicate:%02x>", psrc);
      implement_me(0);
    }
}

static void
print_rlp (struct disassemble_info *info,
           struct e2k_unpacked_bundle *bundle, unsigned int ch)
{
  uint32_t cds = 0;
  uint16_t rlp;
  unsigned int count, index, i;
  bool first = true;

  count = HS_DECODE_CDS_COUNT (bundle->hs) * 2;
  index = ch < 3 ? ch : ch - 3;
  for (i = 0; i < count; ++i)
    {
      if (i & 1)
        rlp = cds >> 16;
      else
        rlp = cds = bundle->cds[i / 2];

      if ((rlp & RLP_MRGC) ||
          ((ch < 3) == ((rlp & RLP_CLUSTER) != 0)) ||
          (!RLP_DECODE_ALC (rlp, index)))
        continue;

      print_text (info, " %s ", first ? "?" : "&&");
      if (RLP_DECODE_INV (rlp, index))
        print_not (info);
      print_psrc (info, RLP_DECODE_PSRC (rlp), false);
      first = false;
    }
}

static bool
print_merge_condition (struct disassemble_info *info,
                       struct e2k_unpacked_bundle *bundle, unsigned int ch)
{
  uint32_t cds = 0;
  uint16_t rlp;
  unsigned int count, index, i;

  count = HS_DECODE_CDS_COUNT (bundle->hs) * 2;
  index = ch < 3 ? ch : ch - 3;
  for (i = 0; i < count; ++i)
    {
      if (i & 1)
        rlp = cds >> 16;
      else
        rlp = cds = bundle->cds[i / 2];

      if (((rlp & (RLP_MRGC | RLP_AM)) != RLP_MRGC) ||
          ((ch < 3) == ((rlp & RLP_CLUSTER) != 0)) ||
          (!RLP_DECODE_ALC (rlp, index)))
        continue;

      if (RLP_DECODE_INV (rlp, index))
        print_not (info);
      print_psrc (info, RLP_DECODE_PSRC (rlp), false);
      return true;
    }

  return false;
}

static void
print_am (struct disassemble_info *info,
          struct e2k_unpacked_bundle *bundle, unsigned int ch)
{
  uint32_t cds = 0;
  uint16_t rlp;
  unsigned int count, index, i;

  count = HS_DECODE_CDS_COUNT (bundle->hs) * 2;
  index = ch < 3 ? ch : ch - 3;
  for (i = 0; i < count; ++i)
    {
      if (i & 1)
        rlp = cds >> 16;
      else
        rlp = cds = bundle->cds[i / 2];

      if (((rlp & (RLP_MRGC | RLP_AM)) != RLP_AM) ||
          ((ch < 3) == ((rlp & RLP_CLUSTER) != 0)))
        continue;

      print_text (info, " ? ");
      if (RLP_DECODE_INV (rlp, index))
        print_not (info);
      print_psrc (info, RLP_DECODE_PSRC (rlp), false);
      return;
    }
}

static void
set_default_e2k_dis_options (struct e2k_private_data *pd)
{
  pd->debug = false;
  pd->aliases = true;
  pd->show_syllables = false; /* TODO: depends on objdump.c:show_raw_insn  */
  pd->show_literal_loc = false;
  pd->show_gpr_size = true;
  pd->show_pair = false;
}

static void
set_isa_version (struct e2k_private_data *pd, unsigned long mach)
{
  pd->version = mach ? mach : bfd_mach_e2k_v7;
}

static void push_syllable(struct e2k_private_data *pd, const char *name,
                          unsigned short index, bool is_half)
{
  if (pd->show_syllables)
    {
      pd->syll[pd->syll_len].name = name;
      pd->syll[pd->syll_len].index = index;
      pd->syll[pd->syll_len].is_half = is_half;
      pd->syll_len += 1;
    }
}

static void
print_syllable_impl (struct disassemble_info *info)
{
  struct e2k_private_data *pd = info->private_data;
  const struct syllable *syll;
  char buf[64] = { 0 }, *p;
  unsigned int i;

  if (pd->syll_cur >= pd->syll_len)
    {
      print_text (info, "%16c", ' ');
      return;
    }

  syll = pd->syll + pd->syll_cur++;
  p = buf;
  p += sprintf (p, "%s", syll->name);
  if (syll->index != -1)
    p += sprintf (p, "%d", syll->index);

  if (syll->is_half)
    {
      syll = pd->syll + pd->syll_cur++;

      p += sprintf (p, " %s", syll->name);
      if (syll->index != -1)
        p += sprintf (p, "%d", syll->index);
    }

  for (i = p - buf; i < 16; ++i)
    *p++ = ' ';
  *p = '\0';

  print_text (info, "%s", buf);
}

#define print_syllable(pd, info) \
  if (pd->show_syllables) \
    print_syllable_impl (info)

static bool
bundle_unpack (struct e2k_private_data *pd, bfd_byte *packet,
               unsigned int len, struct e2k_unpacked_bundle *bundle)
{
  bfd_byte *p, *m, *e;
  unsigned int i, j, h, cds_count, pls_count, lts_count;

  p = packet;
  bundle->hs = bfd_getl32 (p);
  p += 4;
  m = p + HS_DECODE_HOFF (bundle->hs);
  e = packet + len;

  if (m > e)
    return false;

  push_syllable (pd, "HS", -1, false);

  if (HS_DECODE_SS (bundle->hs))
    {
      if ((p + 4) > e)
        return false;

      push_syllable (pd, "SS", -1, false);
      bundle->ss_present = true;
      bundle->ss = bfd_getl32 (p);
      p += 4;
    }

  for (i = 0; i < ALC_COUNT; ++i)
    {
      if (!HS_DECODE_ALS (bundle->hs, i))
        continue;

      if ((p + 4) > e)
        return false;

      push_syllable (pd, "ALS", i, false);
      bundle->als_present[i] = true;
      bundle->als[i] = bfd_getl32 (p);
      p += 4;
    }

  if (HS_DECODE_CS (bundle->hs, 0))
    {
      if ((p + 4) > e)
        return false;

      push_syllable (pd, "CS", 0, false);
      bundle->cs_present[0] = true;
      bundle->cs[0] = bfd_getl32 (p);
      p += 4;
    }

  if ((p + (HS_DECODE_CS (bundle->hs, 1) ? 8 : 4)) == m)
    {
      if ((p + 4) > e)
        return false;

      push_syllable (pd, "ALES", 2, true);
      push_syllable (pd, "ALES", 5, true);
      for (j = 0; j < 2; ++j)
        {
          i = j == 0 ? 5 : 2;
          bundle->ales_present[i] = ALES_ALLOCATED;
          bundle->ales[i] = bfd_getl16 (p);
          p += 2;
        }
    }

  if (HS_DECODE_CS (bundle->hs, 1))
    {
      if ((p + 4) > e)
        return false;

      push_syllable (pd, "CS", 1, false);
      bundle->cs_present[1] = true;
      bundle->cs[1] = bfd_getl32 (p);
      p += 4;
    }

  if (p != m)
    return false;

  /* XXX: stupid order of half-syllables.  */
  h = 0;
  for (i = 0; i < ALC_COUNT; ++i)
    {
      if (!HS_DECODE_ALES (bundle->hs, i))
        continue;

      bundle->ales_present[i] |= ALES_PRESENT;
      if (i != 2 && i != 5)
        {
          push_syllable (pd, "ALES", i, true);
          bundle->ales[i] = bfd_getl16 (h & 1 ? p - 2 : p + 2);
          p += 2;
          ++h;
        }
      else if ((bundle->ales_present[i] & ALES_ALLOCATED) == 0)
        bundle->ales[i] = 0x01c0;
    }

  if (bundle->ss_present && SS_DECODE_FORMAT (bundle->ss) == 0)
    {
      for (i = 0; i < 4; ++i)
        if (SS_DECODE_AAS (bundle->ss, i))
          {
            bundle->aas_present[i / 2] = true;
            bundle->aas_present[i + 2] = true;
          }

      for (i = 0; i < 6; ++i)
        if (bundle->aas_present[i])
          {
            push_syllable (pd, "AAS", i, true);
            bundle->aas[i] = bfd_getl16 (h & 1 ? p - 2 : p + 2);
            p += 2;
            ++h;
          }
    }

  /* Align to next syllable.  */
  if (h & 1)
    {
      push_syllable (pd, "    ", -1, true);
      p += 2;
      ++h;
    }

  cds_count = HS_DECODE_CDS_COUNT (bundle->hs);
  pls_count = HS_DECODE_PLS_COUNT (bundle->hs);
  lts_count = (e - p) / 4;

  e -= (cds_count + pls_count) * 4;
  if (e < p)
    return false;

  lts_count = (e - p) / 4;
  if (lts_count > 4)
    lts_count = 4;
  e -= lts_count * 4;

  while (p < e)
    {
      push_syllable (pd, "gap", -1, false);
      p += 4;
    }

  for (i = lts_count; i--;)
    {
      push_syllable (pd, "LTS", i, false);
      bundle->lts_present[i] = true;
      bundle->lts[i] = bfd_getl32 (e);
      e += 4;
    }

  for (i = pls_count; i--;)
    {
      push_syllable (pd, "PLS", i, false);
      bundle->pls_present[i] = true;
      bundle->pls[i] = bfd_getl32 (e);
      e += 4;
    }

  for (i = cds_count; i--;)
    {
      push_syllable (pd, "CDS", i, false);
      bundle->cds_present[i] = true;
      bundle->cds[i] = bfd_getl32 (e);
      e += 4;
    }

  return true;
}

static void
print_nl (struct disassemble_info *info)
{
  struct e2k_private_data *pd = info->private_data;
  print_styled (info, dis_style_comment_stop, "\n");
  print_text (info, "%*c", pd->align, ' ');
  print_syllable (pd, info);
  ++pd->lines;
}

static void
print_ss_advance (struct disassemble_info *info, const char *name,
                  unsigned int x, const char* l, const char* h)
{
  if (!x)
    return;
  print_aligned_mnemonic (info, "%s.", name);
  if (x & 1)
    print_sub_mnemonic (info, "%s", l);
  if (x & 2)
    print_sub_mnemonic (info, "%s", h);
  print_nl (info);
}

static void
print_sf1 (struct disassemble_info *info, uint32_t ss)
{
  print_ss_advance (info, "alc", SS_DECODE_ALC (ss), "t", "f");
  print_ss_advance (info, "abp", SS_DECODE_ABP (ss), "t", "f");
  print_ss_advance (info, "abn", SS_DECODE_ABN (ss), "t", "f");
  print_ss_advance (info, "abg", SS_DECODE_ABG (ss), "d", "i");

  if (SS_DECODE_BAP (ss))
    {
      print_aligned_mnemonic (info, "bap");
      print_nl (info);
    }

  if (SS_DECODE_EAP (ss))
    {
      print_aligned_mnemonic (info, "eap");
      print_nl (info);
    }
}

static void
decode_ss (struct disassemble_info *info, uint32_t ss)
{
  if (SS_DECODE_FORMAT (ss) == 0)
    print_sf1 (info, ss);
  else
    {
      if (ss & SS_TYPE1_MASK)
        {
          print_aligned_mnemonic (info, "SS");
          print_text (info, "(%08x)", ss);
          print_nl (info);
          implement_me(0);
        }
    }
}

static void
decode_hs (struct disassemble_info *info, uint32_t hs)
{
  if (HS_DECODE_LOOP (hs))
    {
      print_aligned_mnemonic (info, "loop_mode");
      print_nl (info);
    }

  /* XXX: unknown bit  */
  if (HS_DECODE_SIM (hs))
    {
      print_aligned_text (info, "<invalid HS:SIM>");
      print_nl (info);
      implement_me(0);
    }

  /* XXX: unknown bit  */
  if (HS_DECODE_MDL (hs))
    {
      print_aligned_text (info, "<invalid HS:MDL>");
      print_nl (info);
      implement_me(0);
    }
}

static void
decode_nop (struct disassemble_info *info, uint32_t hs)
{
  struct e2k_private_data *pd = info->private_data;
  unsigned int nop = HS_DECODE_NOP (hs);

  if (nop > 1 || pd->lines == 0)
    {
      print_aligned_mnemonic (info, "nop ");
      print_uimm (info, nop);
      print_nl (info);
    }
}

static void
decode_aau (struct disassemble_info *info,
            struct e2k_unpacked_bundle *bundle)
{
  unsigned int i;

  for (i = 0; i < 4; ++i)
    {
      const char *name = NULL;
      uint16_t aas, aas_dst;
      enum gpr_size size = GS_D;
      int width;

      if (!bundle->aas_present[i + 2])
        continue;

      aas = bundle->aas[i + 2];
      aas_dst = bundle->aas[i / 2];

      switch (aas & MOVA_OPC_MASK)
        {
        case MOVA_NONE:
          continue;
        case MOVAB:
          name = "movab";
          break;
        case MOVAH:
          name = "movah";
          break;
        case MOVAW:
          name = "movaw";
          break;
        case MOVAD:
          name = "movad";
          break;
        case MOVAQ:
          name = "movaq";
          size = GS_Q;
          break;
        case MOVAQP:
          name = "movaqp";
          size = GS_P;
          break;
        default:
          print_text (info, "AAS@%d(%04x) AAS@%d(%04x)", i + 2, aas, i / 2, aas_dst);
          print_nl (info);
          continue;
        }
      width = strlen (name);

      print_text (info, "apb%d ", i);
      print_mnemonic (info, "%s", name);
      if (aas & MOVA_BE)
        {
          print_sub_mnemonic (info, ".be");
          width += 3;
        }
      if (aas & MOVA_AM)
        {
          print_sub_mnemonic (info, ".am");
          width += 3;
        }
      print_operand_alignment (info, width);
      print_dst (info, (aas_dst >> (i & 1 ? 0 : 8)) & 0xff, size);
      print_text (info, ", ");
      print_named_uimm (info, "area", MOVA_DECODE_AREA (aas));
      print_text (info, ", ");
      print_named_uimm (info, "index", MOVA_DECODE_IND (aas));
      print_nl (info);
    }
}

static void
decode_pls (struct disassemble_info *info,
            struct e2k_unpacked_bundle *bundle)
{
  unsigned int i, count, used[8] = { 0 };

  count = HS_DECODE_PLS_COUNT (bundle->hs);

  for (i = count; i--;)
    if ((bundle->pls[i] & LP_WRITE_PREG) || used[i + 4])
      {
        used[i + 4] += 1;
        used[LP_DECODE_LPSRC (bundle->pls[i], 0)] += 1;
        used[LP_DECODE_LPSRC (bundle->pls[i], 1)] += 1;
      }

  for (i = 0; i < count; ++i)
    {
      uint32_t pls = bundle->pls[i];
      unsigned int j;

      if (!used[i + 4])
        continue;

      print_text (info, "plu%d    ", i);
      switch (pls & LP_OPC_MASK)
        {
        case LP_ANDP:
          print_mnemonic (info, "andp");
          print_operand_alignment (info, 4);
          break;
        case LP_LANDP:
          print_mnemonic (info, "landp");
          print_operand_alignment (info, 5);
          break;
        case LP_MOVEP:
          print_mnemonic (info, "movep");
          print_operand_alignment (info, 5);
          break;
        default:
          print_mnemonic (info, "invalid_clp");
          print_operand_alignment (info, 11);
          implement_me(0);
          break;
        }

      if (pls & LP_WRITE_PREG)
        print_preg (info, LP_DECODE_PREG (pls));
      else
        print_text (info, "_");

      for (j = 0; j < 2; ++j)
        {
          unsigned int lp;

          print_text (info, ", ");
          if (LP_DECODE_LPSRC_INV (pls, j))
            print_not (info);
          lp = LP_DECODE_LPSRC (pls, j);
          if (lp < 4)
            print_psrc (info, ELP_DECODE (bundle->pls[lp / 2], lp & 1), true);
          else
            print_register (info, "plu%u", lp - 4);
        }
      print_nl (info);
    }
}

static void
print_ct_cond (struct disassemble_info *info, unsigned int cond, unsigned int pred)
{
  if (cond != CT_COND_ALWAYS)
    print_text (info, " ? ");

  switch (cond)
    {
    case CT_COND_NONE:
      abort (); /* unreachable */
    case CT_COND_ALWAYS:
      break;
    case CT_COND_PREG:
      print_preg (info, pred);
      break;
    case CT_COND_NOT_PREG:
      print_not_preg (info, pred);
      break;
    case CT_COND_LOOP_END:
      print_loop_end (info);
      break;
    case CT_COND_NOT_LOOP_END:
      print_not_loop_end (info);
      break;
    case CT_COND_PREG_OR_LOOP_END:
      print_preg (info, pred);
      print_or (info);
      print_loop_end (info);
      break;
    case CT_COND_NOT_PREG_AND_NOT_LOOP_END:
      print_not_preg (info, pred);
      print_and (info);
      print_not_loop_end (info);
      break;
    case CT_COND_MLOCK_OR_DTAL:
      print_register (info, "mlock");
      if (pred)
        {
          char buf[8] = { 0 };
          char *p = buf;
          unsigned int i;

          for (i = 0; i < 4; ++i)
            if (pred & (1 << i))
              *p++ = '0' + (i > 1 ? i + 1 : i);

          print_text (info, " || ");
          print_register (info, "dt_al%s", buf);
        }
      break;
    case CT_COND_MLOCK_OR_CMP:
      print_register (info, "mlock");
      print_or (info);

      if ((pred & 0x18) == 0x00)
        {
          if (pred & 1)
            print_not (info);
          print_cmp (info, EXTRACT_BITS (pred, 1, 2));
        }
      else if ((pred & 0x18) == 0x08)
        {
          unsigned int index = pred & 4 ? 3 : 0;

          if (pred & 2)
            print_not (info);
          print_register (info, "alc%u", index);
          print_or (info);
          if (pred & 1)
            print_not (info);
          print_register (info, "alc%u", index + 1);
        }
      else if ((pred & 0x18) == 0x10)
        {
          if (pred & 1)
            print_not (info);

          if ((pred & 0x6) == 0)
            print_register (info, "plu0");
          else if ((pred & 0x6) == 2)
            print_register (info, "plu1");
          else if ((pred & 0x6) == 4)
            print_register (info, "plu2");
          else
            goto cmp_pred_err;
        }
      else
cmp_pred_err:
        print_text (info, "<unknown predicate %#x>", pred);
      break;
    case CT_COND_CMP_CLP:
      if (pred & 1)
        print_not (info);

      if (pred & 0x10)
        {
          int plu = EXTRACT_BITS(pred, 1, 3);

          print_register (info, "plu%d", plu);
          implement_me(plu <= 3);
        }
      else
        {
          int alc;

          switch (EXTRACT_BITS(pred, 1, 3))
            {
              case 0: alc = 0; break;
              case 1: alc = 1; break;
              case 2: alc = 3; break;
              case 3: alc = 4; break;
              default: alc = -1; break;
            }

          print_register (info, "alc%d", alc);
          implement_me(alc != -1);
        }
      break;
    case CT_COND_NOT_PREG_OR_LOOP_END:
      print_not_preg (info, pred);
      print_or (info);
      print_loop_end (info);
      break;
    case CT_COND_PREG_AND_NOT_LOOP_END:
      print_not_preg (info, pred);
      print_and (info);
      print_loop_end (info);
      break;
    default:
      print_text (info, "<unknown ct cond %d>", cond);
      implement_me(0);
      break;
    }
}

/* XXX: when it should be printed?  */
static void
print_ipd (struct disassemble_info *info,
           struct e2k_unpacked_bundle *bundle)
{
  unsigned int ipd;

  if (bundle->ss_present && (ipd = SS_DECODE_IPD (bundle->ss)))
    {
      print_aligned_mnemonic (info, "ipd ");
      print_uimm (info, ipd);
      print_nl (info);
    }
}

static void
decode_ct (bfd_vma memaddr, struct disassemble_info *info,
           struct e2k_unpacked_bundle *bundle)
{
  struct e2k_private_data *pd = info->private_data;
  enum ct_cond cond;
  unsigned int ctpr;
  bool hint_target = false;

  if (!bundle->ss_present || SS_DECODE_FORMAT (bundle->ss) != 0 ||
      pd->ct_decoded)
    return;

  cond = SS_DECODE_CT_COND (bundle->ss);
  if (cond == CT_COND_NONE)
    return;

  pd->ct_decoded = true;
  info->insn_info_valid = 1;
  info->target = 0;
  info->insn_type = cond == CT_COND_ALWAYS ? dis_branch : dis_condbranch;

  ctpr = SS_DECODE_CTPR (bundle->ss);
  if (ctpr > 0)
    {
      if (bundle->cs_present[1] &&
          TEST_OPCODE (bundle->cs[1], CALL_OPCODE_CS1, CALL_OPCODE_MASK_CS1))
        {
          hint_target = true;
          info->insn_type = cond == CT_COND_ALWAYS ? dis_jsr : dis_condjsr;
          print_aligned_mnemonic (info, "call");
          print_operand_alignment (info, 4);
          print_uimm (info, CALL_DECODE_WBS_CS1 (bundle->cs[1]));
          print_text (info, ", ");
          print_ctpr (info, ctpr);
        }
      else
        {
          hint_target = true;
          print_aligned_mnemonic (info, "ct");
          print_operand_alignment (info, 2);
          print_ctpr (info, ctpr);
          if (pd->ctpr[ctpr].type == CTPR_DISP)
            info->target = pd->ctpr[ctpr].target;
        }
    }
  else if (bundle->cs_present[0] &&
           TEST_OPCODE (bundle->cs[0], IBRANCH_OPCODE_CS0, IBRANCH_OPCODE_MASK_CS0))
    {
      int32_t disp = IBRANCH_DECODE_DISP_CS0 (bundle->cs[0]);

      info->insn_type = cond == CT_COND_ALWAYS ? dis_jsr : dis_condjsr;
      info->target = memaddr + disp;
      print_ipd (info, bundle);
      if (bundle->cs_present[1] &&
          TEST_OPCODE (bundle->cs[1], ICALL_OPCODE_CS1, ICALL_OPCODE_MASK_CS1))
        {
          print_aligned_mnemonic (info, "icall");
          print_operand_alignment (info, 5);
          print_uimm (info, CALL_DECODE_WBS_CS1 (bundle->cs[1]));
          print_text (info, ", ");
        }
      else
        {
          print_aligned_mnemonic (info, "ibranch");
          print_operand_alignment (info, 7);
        }
        (*info->print_address_func)(info->target, info);
    }
  else if (bundle->cs_present[0] &&
           TEST_OPCODE (bundle->cs[0], DONE_OPCODE_CS0, DONE_OPCODE_MASK_CS0))
    {
      unsigned int type = DONE_DECODE_TYPE_CS0 (bundle->cs[0]);

      switch (type)
        {
        case DONE_TYPE_DONE:
          print_aligned_mnemonic (info, "done");
          print_operand_alignment (info, 4);

          /* XXX: deprecated?  */
          if (bundle->cs[0] & DONE_FDAM_CS0)
            print_mnemonic (info, "fdam");

          /* XXX: deprecated?  */
          if (bundle->cs[0] & DONE_TRAR_CS0)
            {
              if (bundle->cs[0] & DONE_FDAM_CS0)
                print_text (info, ",");
              print_mnemonic (info, " trar");
            }
          break;
        case DONE_TYPE_IRET:
          print_aligned_mnemonic (info, "iret");
          break;
        case DONE_TYPE_HRET:
          print_aligned_mnemonic (info, "hret");
          break;
        case DONE_TYPE_GLAUNCH:
          print_aligned_mnemonic (info, "glaunch");
          break;
        default:
          print_text (info, "CS0(%08x)", bundle->cs[0]);
          implement_me(0);
          break;
        }
    }
  else
    {
      print_text (info, "invalid ct: SS(%08x)", bundle->ss);
      implement_me(0);
      print_nl (info);
      return;
    }

  print_ct_cond (info, cond, SS_DECODE_CT_PRED (bundle->ss));

  if (hint_target)
    switch (pd->ctpr[ctpr].type)
      {
      case CTPR_DISP:
        if (pd->ctpr[ctpr].target)
         {
           print_comment (info, " # ");
           (*info->print_address_func)(pd->ctpr[ctpr].target, info);
         }
        break;
      case CTPR_SYS:
        print_comment (info, " # syscall %#lx", pd->ctpr[ctpr].target);
        break;
      case CTPR_RET:
        print_comment (info, " # return");
        break;
      case CTPR_MOVTD:
        print_comment (info, " # movtd at %#lx", pd->ctpr[ctpr].target);
        break;
      default:
        break;
      }

  print_nl (info);
}

static int
print_hint_call (struct disassemble_info *info,
                 struct e2k_unpacked_bundle *bundle)
{
  if (bundle->ss_present &&
      SS_DECODE_FORMAT(bundle->ss) &&
      SS_DECODE_PREP_CALL_HINT (bundle->ss))
    {
      print_sub_mnemonic (info, ".call");
      return 5;
    }
  return 0;
}

static void
print_prep (struct disassemble_info *info, struct e2k_unpacked_bundle *bundle,
            const char *sub, unsigned int ctpr, int32_t disp ATTRIBUTE_UNUSED,
            bfd_vma target)
{
  int width = 4;
  print_aligned_mnemonic (info, "prep");

  if (sub)
    {
      print_sub_mnemonic (info, "%s", sub);
      width += strlen (sub);
    }
  else
    width += print_hint_call (info, bundle);

  print_operand_alignment (info, width);
  print_ctpr (info, ctpr);
  print_text (info, ", ");
  (*info->print_address_func)(target, info);

  print_nl (info);
}

static void
decode_cs0 (bfd_vma memaddr, struct disassemble_info *info,
            struct e2k_unpacked_bundle *bundle)
{
  struct e2k_private_data *pd = info->private_data;
  uint32_t cs0;
  unsigned int ctpr;

  if (!bundle->cs_present[0])
    return;

  cs0 = bundle->cs[0];
  ctpr = CS0_DECODE_CTPR (cs0);

  if (ctpr != 0)
    {
      if (TEST_OPCODE (cs0, PREP_OPCODE_CS0, PREP_OPCODE_MASK_CS0))
        {
          int32_t disp = PREP_DECODE_DISP_CS0 (cs0);
          bfd_vma target = memaddr + disp;

          pd->ctpr[ctpr].target = target;
          pd->ctpr[ctpr].type = CTPR_DISP;

          print_ipd (info, bundle);
          print_prep (info, bundle, NULL, ctpr, disp, target);
        }
      else if (TEST_OPCODE (cs0, PREP_APB_OPCODE_CS0, PREP_APB_OPCODE_MASK_CS0))
        {
          int32_t disp = PREP_APB_DECODE_DISP_CS0 (cs0);
          bfd_vma target = memaddr + disp;

          pd->ctpr[ctpr].target = target;
          pd->ctpr[ctpr].type = CTPR_APB;

          print_ipd (info, bundle);
          print_prep (info, bundle, ".apb", ctpr, disp, target);
        }
      else if (TEST_OPCODE (cs0, PREP_SYS_OPCODE_CS0, PREP_SYS_OPCODE_MASK_CS0))
        {
          uint32_t target = PREP_SYS_DECODE_DISP_CS0 (cs0);

          pd->ctpr[ctpr].type = CTPR_SYS;
          pd->ctpr[ctpr].target = target;

          print_ipd (info, bundle);
          print_aligned_mnemonic (info, "prep");
          print_sub_mnemonic (info, ".sys");
          print_operand_alignment (info, 8);
          print_ctpr (info, ctpr);
          print_text (info, ", ");
          print_hex (info, target);
          print_nl (info);
        }
      else if (TEST_OPCODE (cs0, PREP_RET_OPCODE_CS0, PREP_RET_OPCODE_MASK_CS0))
        {
          pd->ctpr[ctpr].type = CTPR_RET;
          pd->ctpr[ctpr].target = 0;

          print_ipd (info, bundle);
          print_aligned_mnemonic (info, "prep");
          print_sub_mnemonic (info, ".ret");
          print_operand_alignment (info, 8);
          print_ctpr (info, ctpr);
          print_nl (info);
        }
      else if (TEST_OPCODE (cs0, GETTSD_OPCODE_CS0, GETTSD_OPCODE_MASK_CS0))
        {
          print_aligned_mnemonic (info, "gettsd");
          print_operand_alignment (info, 6);
          print_ctpr (info, ctpr);
          print_nl (info);
        }
      else
        goto err;
    }
  else if (TEST_OPCODE (cs0, IBRANCH_OPCODE_CS0, IBRANCH_OPCODE_MASK_CS0) &&
           pd->ct_decoded)
    ; /* ibranch decoded in decode_ct.  */
  else if (TEST_OPCODE (cs0, DONE_OPCODE_CS0, DONE_OPCODE_MASK_CS0))
    ; /* done decoded in decode_ct.  */
  else if (TEST_OPCODE (cs0, PREF_OPCODE_CS0, PREF_OPCODE_MASK_CS0))
    {
      unsigned int ipr = PREF_DECODE_IPR_CS0 (cs0);
      unsigned int disp = PREF_DECODE_DISP_CS0 (cs0);

      print_aligned_mnemonic (info, "pref");
      print_operand_alignment (info, 4);
      print_ipr (info, ipr);
      print_text (info, ", ");
      print_hex (info, disp);
      if (cs0 & PREF_IPD_CS0)
        {
          print_text (info, ", ");
          print_aligned_mnemonic (info, "ipd");
        }
      print_nl (info);
    }
  else if (TEST_OPCODE (cs0, PUTTSD_OPCODE_CS0, PUTTSD_OPCODE_MASK_CS0))
    {
      int32_t disp = PUTTSD_DECODE_DISP_CS0 (cs0);

      print_aligned_mnemonic (info, "puttsd");
      print_operand_alignment (info, 6);
      (*info->print_address_func)(memaddr + disp, info);
      print_nl (info);
    }
  else
    {
err:
      print_aligned_mnemonic (info, "CS0");
      print_text (info, "(%08x)", cs0);
      print_nl (info);
    }
}

static void
print_setwd (struct disassemble_info *info, uint32_t lts0)
{
  int width = 5;

  print_aligned_mnemonic (info, "setwd");
  if (!(lts0 & SETWD_NFX_LTS0))
    {
      print_sub_mnemonic (info, ".x");
      width += 2;
    }
  if (lts0 & SETWD_DBL_LTS0)
    {
      print_sub_mnemonic (info, ".z");
      width += 2;
    }
  if (lts0 & SETWD_MCN_LTS0)
    {
      /* TODO: mode check numeric value (protected mode) */
      print_sub_mnemonic (info, ".mcn");
      width += 4;
    }
  print_operand_alignment (info, width);
  print_uimm (info, SETWD_DECODE_WSZ_LTS0 (lts0));
  print_nl (info);
}

static void
print_setbn (struct disassemble_info *info, uint32_t cs1)
{
  int rsz, rbs, rcur;

  rsz = SETBN_DECODE_RSZ_CS1 (cs1);
  rbs = SETBN_DECODE_RBS_CS1 (cs1);
  rcur = SETBN_DECODE_RCUR_CS1 (cs1);

  print_aligned_mnemonic (info, "setbn");
  print_operand_alignment (info, 5);
  print_uimm (info, rsz);
  print_text (info, ", ");
  print_uimm (info, rbs);
  if (rcur)
    {
      print_text (info, ", ");
      print_uimm (info, rcur);
    }
  print_nl (info);
}

static void
print_setbp (struct disassemble_info *info, uint32_t cs1)
{
  print_aligned_mnemonic (info, "setbp");
  print_operand_alignment (info, 5);
  print_uimm (info, SETBP_DECODE_PSZ_CS1 (cs1));
  print_nl (info);
}

static void
decode_cs1 (struct disassemble_info *info,
            struct e2k_unpacked_bundle *bundle)
{
  struct e2k_private_data *pd = info->private_data;
  uint32_t cs1;

  if (!bundle->cs_present[1])
    return;

  cs1 = bundle->cs[1];

  if (TEST_OPCODE (cs1, CALL_OPCODE_CS1, CALL_OPCODE_MASK_CS1) &&
      pd->ct_decoded)
    ; /* call decoded in decode_ct.  */
  else if (TEST_OPCODE (cs1, SETWD_OPCODE_CS1, SETWD_OPCODE_MASK_CS1) &&
           bundle->lts_present[0] &&
           TEST_OPCODE (bundle->lts[0], SETWD_OPCODE_LTS0, SETWD_OPCODE_MASK_LTS0))
    {
      if (cs1 & SETWD_VFRPSZ_CS1)
        {
          print_aligned_mnemonic (info, "vfrpsz");
          print_operand_alignment (info, 6);
          print_named_hex (info, "rpsz", VFRPSZ_DECODE_RPSZ_LTS0 (bundle->lts[0]));
          print_nl (info);
        }

      print_setwd (info, bundle->lts[0]);

      if (cs1 & SETWD_SETBN_CS1)
        print_setbn (info, cs1);

      if (cs1 & SETWD_SETBP_CS1)
        print_setbp (info, cs1);

      if (cs1 & SETWD_SETTR_CS1)
        {
          print_aligned_mnemonic (info, "settr");
          print_operand_alignment (info, 5);
          print_named_hex (info, "type", VFRPSZ_DECODE_RPSZ_LTS0 (bundle->lts[0]));
          print_nl (info);
        }
    }
  else if (TEST_OPCODE (cs1, SETBN_SETBP_OPCODE_CS1, SETBN_SETBP_OPCODE_MASK_CS1))
    {
      if (cs1 & SETWD_SETBN_CS1)
        print_setbn (info, cs1);

      if (cs1 & SETWD_SETBP_CS1)
        print_setbp (info, cs1);
    }
  else if (TEST_OPCODE (cs1, SETEI_OPCODE_CS1, SETEI_OPCODE_MASK_CS1))
    {
      print_aligned_mnemonic (info, "setei");
      print_operand_alignment (info, 5);
      print_hex (info, SETEI_DECODE_VALUE_CS1 (cs1));
      print_nl (info);
    }
  else if (TEST_OPCODE (cs1, SETSFT_CS1_OPCODE, SETSFT_CS1_OPCODE_MASK))
    {
      print_aligned_mnemonic (info, "setsft");
      print_operand_alignment (info, 6);
      print_nl (info);
    }
  else if (TEST_OPCODE (cs1, WAIT_OPCODE_CS1, WAIT_OPCODE_MASK_CS1))
    {
      static const struct {
        const char *name;
        unsigned int bit;
      } fields[] =
        {
            {"sas",     WAIT_SAS_CS1},
            {"sal",     WAIT_SAL_CS1},
            {"trap",    WAIT_TRAP_CS1},
            {"ma_c",    WAIT_MA_C_CS1},
            {"fl_c",    WAIT_FL_C_CS1},
            {"ld_c",    WAIT_LD_C_CS1},
            {"st_c",    WAIT_ST_C_CS1},
            {"all_e",   WAIT_ALL_E_CS1},
            {"all_c",   WAIT_ALL_C_CS1}
        };
      unsigned int i;
      bool first = true;

      print_aligned_mnemonic (info, "wait");
      print_operand_alignment (info, 4);
      for (i = 0; i < ARRAY_SIZE (fields); ++i)
        {
          if ((cs1 & fields[i].bit) == 0)
            continue;

          if (first)
            {
              print_text (info, " %s", fields[i].name);
              first = false;
            }
          else
            print_text (info, ", %s", fields[i].name);
        }
      print_nl (info);
    }
  else if (TEST_OPCODE (cs1, SETMAS_OPCODE_CS1, SETMAS_OPCODE_MASK_CS1))
    {
      if (pd->debug)
        {
          print_aligned_mnemonic (info, "setmas");
          print_operand_alignment (info, 6);
          print_hex (info, SETMAS_DECODE_CS1 (cs1, 0));
          print_text (info, ", ");
          print_hex (info, SETMAS_DECODE_CS1 (cs1, 2));
          print_text (info, ", ");
          print_hex (info, SETMAS_DECODE_CS1 (cs1, 3));
          print_text (info, ", ");
          print_hex (info, SETMAS_DECODE_CS1 (cs1, 5));
          print_nl (info);
        }
    }
  else if (TEST_OPCODE (cs1, FLUSH_CS1_OPCODE, FLUSH_CS1_OPCODE_MASK))
    {
      if (cs1 & FLUSH_R_CS1)
        {
          print_aligned_mnemonic (info, "flushr");
          print_operand_alignment (info, 6);
          print_nl (info);
        }
      if (cs1 & FLUSH_C_CS1)
        {
          print_aligned_mnemonic (info, "flushc");
          print_operand_alignment (info, 6);
          print_nl (info);
        }
    }
  else if (TEST_OPCODE (cs1, VFBG_OPCODE_CS1, VFBG_OPCODE_MASK_CS1))
    {
      print_aligned_mnemonic (info, "vfbg");
      print_operand_alignment (info, 4);
      print_named_hex (info, "umask", VFBG_DECODE_UMASK_CS1 (cs1));
      print_text (info, ", ");
      print_named_hex (info, "dmask", VFBG_DECODE_DMASK_CS1 (cs1));
      if (cs1 & VFBG_CHKM4_CS1)
        {
          print_text (info, ", chkm4");
        }
      print_nl (info);
    }
  else
    {
      print_aligned_mnemonic (info, "CS1");
      print_text (info, "(%08x)", cs1);
      print_nl (info);
    }
}

#define AL_HASH(als, ales) ((((ales) >> 1) & 0xf80) | (((als) >> 24) & 0x7f))

static const struct al_opcode *
al_lookup (struct e2k_private_data *pd, struct e2k_unpacked_bundle *bundle,
           unsigned int ch, uint32_t als, uint16_t ales, enum al_flags flags)
{
  static uint16_t al_hash[1 << (5 + 7)];
  static bool initialized = false;
  uint8_t opc1 = (als >> 24) & 0x7f;
  uint8_t opc2 = ales >> 8;
  uint16_t hash;
  unsigned int i;

  if (!initialized)
    {
      const struct al_opcode *p = al_opcodes;

      memset (al_hash, 0xff, sizeof(al_hash));
      for (i = 0; p->name; ++i, ++p)
        {
          if (((p->flags & AF_REMOVED_IN_V2) && pd->version >= 2) ||
              ((p->flags & AF_REMOVED_IN_V3) && pd->version >= 3))
            continue;

          hash = AL_HASH (p->als, p->ales);
          if (al_hash[hash] == 0xffff)
            al_hash[hash] = i;
        }
      initialized = true;
    }

  /* Hack for plog and qplog instructions.  */
  if (opc2 >= 0x10 && opc2 <= 0x13)
    hash = AL_HASH (0, ales | 0x0100);
  else
    hash = AL_HASH (als, ales);

  i = al_hash[hash];
  if (i == 0xffff)
    return NULL;

  for (; al_opcodes[i].name; ++i)
    {
      const struct al_opcode *p = &al_opcodes[i];
      const struct al_format_info *format_info = &al_format_info[p->format];

      if (flags && (p->flags & flags) == 0)
        continue;

      /* Skip deleted instructions.  */
      if (((p->flags & AF_REMOVED_IN_V2) && pd->version >= 2) ||
          ((p->flags & AF_REMOVED_IN_V3) && pd->version >= 3))
        continue;

      /* Hack for plog and qplog instructions.  */
      if (!(opc2 >= 0x10 && opc2 <= 0x13) && opc1 != (p->als >> 24))
        break;

      /* Additional matching for specific formats.  */
      switch (p->format)
        {
        case AAURR:
        case AAURW:
          if (!HAS_SETMAS (bundle) || SETMAS_DECODE_CS1 (bundle->cs[1], ch) != 0x3f)
            continue;
          break;
        default:
          break;
        }

      if ((als & format_info->als_mask) == p->als &&
          (ales & format_info->ales_mask) == p->ales &&
          (p->version[ch] && p->version[ch] <= pd->version))
        return p;
    }

  return NULL;
}

static const struct al_opcode *
al_find (struct e2k_private_data *pd, struct e2k_unpacked_bundle *bundle,
         unsigned int ch, uint32_t als, uint16_t ales)
{
  const struct al_opcode *opcode;

  opcode = al_lookup (pd, bundle, ch, als, ales, 0);
  /* Hack for stupid encoding.  */
  if (!opcode && ALES_DECODE_OPC (ales) == 0x02)
    {
      if (ch == 2 || ch == 5)
        opcode = al_lookup (pd, bundle, ch, als, 0, AF_EXPLICIT_ALES25);
      else if (ch == 0 || ch == 3)
        {
          ales = 0x0100 | ALES_DECODE_SRC3 (ales);
          opcode = al_lookup (pd, bundle, ch, als, ales, AF_ALT_ALES03);
        }
    }
  return opcode;
}

static void
decode_al (bfd_vma memaddr, struct disassemble_info *info,
           struct e2k_unpacked_bundle *bundle, unsigned int ch)
{
  struct e2k_private_data *pd = info->private_data;
  const struct al_opcode *opcode;
  const struct al_format_info *format_info;
  uint32_t als;
  uint16_t ales;
  const char *operand;
  unsigned int width;
  bool is_ctpr_dst;

  als = bundle->als[ch];
  ales = (bundle->ales_present[ch] & ALES_PRESENT) ? bundle->ales[ch] : 0;
  opcode = al_find (pd, bundle, ch, als, ales);

  if (!opcode)
    {
      print_aligned_mnemonic (info, "als%d", ch);
      print_text (info, "(%08x)", als);
      if (bundle->ales_present[ch])
        {
          print_mnemonic (info, " ales%d", ch);
          print_text (info, "(%04x)", ales);
        }
      print_nl (info);
      return;
    }

  format_info = &al_format_info[opcode->format];

  if (opcode->flags & AF_PAIR)
    {
      unsigned int pair = format_info->pair[ch];

      /* TODO: check pair instruction  */

      if (!pd->show_pair && pair < ch)
        return;
    }

  if (ALS_DECODE_SM (als))
    {
      print_text (info, "alc%d", ch);
      print_sub_mnemonic (info, ".sm ");
    }
  else
    print_text (info, "alc%d    ", ch);
  print_mnemonic (info, "%s", opcode->name);
  width = strlen (opcode->name);

  is_ctpr_dst = ch == 0 && ALS_IS_DST_CTPR (ALS_DECODE_DST (als)) &&
    (opcode->format == ALF2_MOVTD ||
    (opcode->format == ALF2 && strcmp (opcode->name, "getpl") == 0));

  if (is_ctpr_dst)
      width += print_hint_call (info, bundle);

  operand = al_format_info[opcode->format].operands;
  if (*operand != '\0')
    print_operand_alignment (info, width);

  for (; *operand; ++operand)
    {
      unsigned int value;

      switch (*operand)
        {
        case ',':
          print_text (info, ", ");
          break;
        case 'D':
          value = ALS_DECODE_DST (als);
          if (is_ctpr_dst)
            {
              unsigned int ctpr = value & 3;

              pd->ctpr[ctpr].type = CTPR_MOVTD;
              pd->ctpr[ctpr].target = memaddr;
              print_ctpr (info, ctpr);
            }
          else
            print_dst (info, value, opcode->dst);
          break;
        case '4':
          value = ALS_DECODE_SRC4 (als);
          if (!print_gpr (info, value, opcode->dst))
            {
              print_text (info, "<invalid src4:%02x>", value);
              implement_me(0);
            }
          break;
        case 'P':
          value = ALS_DECODE_CMP_DST (als);
          print_preg (info, value);
          break;
        case 'S':
          value = ALS_DECODE_DST (als);
          print_state_register (info, value);
          break;
        case '1':
          value = ALS_DECODE_SRC1 (als);
          if (!print_gpr (info, value, opcode->src1))
            print_uimm (info, value - 0xc0);
          break;
        case '2':
          value = ALS_DECODE_SRC2 (als);
          if (print_gpr (info, value, opcode->src2))
            ;
          else if (value >= 0xc0 && value < 0xd0)
            print_uimm (info, value - 0xc0);
          else if (!print_literal (info, value, bundle, is_ctpr_dst))
            {
              print_text (info, "<invalid src2:%02x>", value);
              implement_me(0);
            }
          break;
        case '3':
          value = ALES_DECODE_SRC3 (ales);
          if (!print_gpr (info, value, opcode->src3))
            {
              print_text (info, "<invalid src3:%02x>", value);
              implement_me(0);
            }
          break;
        case 'L':
          value = ALES_DECODE_SRC3 (ales);
          if (print_gpr (info, value, opcode->src2))
            ;
          else if (pd->version < bfd_mach_e2k_v7 || !print_literal (info, value, bundle, is_ctpr_dst))
            {
              print_text (info, "<invalid src3:%02x>", value);
              implement_me(0);
            }
          break;
        case 'p':
          if (!print_merge_condition (info, bundle, ch))
            {
              print_text (info, "<nonexistent mrgc>");
              implement_me(0);
            }
          break;
        case 's':
          value = ALS_DECODE_SRC1 (als);
          print_state_register (info, value);
          break;
        case 'i':
          value = ALES_DECODE_SRC3 (ales);
          print_hex (info, value);
          break;
        case 'a':
          value = ALS_DECODE_AA_AAD (als);
          print_aad (info, value);
          break;
        case 'A':
          value = ALS_DECODE_AA_INDEX (als);
          print_aasti (info, value);
          break;
        case 'l':
          value = ALS_DECODE_AA_LTS (als);
          if (value == 0)
            break;

          print_text (info, ", ");
          --value;
          if (bundle->lts_present[value])
            print_hex (info, bundle->lts[value]);
          else
            print_text (info, "<nonexistent literal>");
          break;
        case 'U':
        case 'u':
          switch (ALS_DECODE_AA_MODE (als))
            {
            case STAA_MODE_AAD:
              print_aad (info, ALS_DECODE_AA_AAD (als));
              break;
            case STAA_MODE_AASTI:
              print_aasti (info, ALS_DECODE_AA_INDEX (als));
              break;
            case STAA_MODE_AAIND:
              print_aaind (info, ALS_DECODE_AA_INDEX (als));
              break;
            case STAA_MODE_AAINCR:
              print_aaincr (info, ALS_DECODE_AA_INCR (als));
              break;
            default:
              print_text (info, "<invalid staa_mode>");
              implement_me(0);
              break;
            }
          break;
        case 't':
          print_hex (info, ((ales >> 1) & 0x80) | ALS_DECODE_OPC (als));
          break;
        case 'm':
          if (bundle->cs_present[1] &&
              TEST_OPCODE (bundle->cs[1], SETMAS_OPCODE_CS1, SETMAS_OPCODE_MASK_CS1))
            {
              unsigned int mas = SETMAS_DECODE_CS1 (bundle->cs[1], ch);

              if (mas)
                {
                  print_text (info, ", mas=");
                  print_hex (info, mas);
                }
            }
          break;
        case 'w':
          print_uimm (info, ALS_DECODE_SRC1 (als) * 2);
          break;
        case '?':
          print_rlp (info, bundle, ch);
          break;
        case 'c':
            {
              unsigned int cond, ctpr;

              /* Prevent decoding in decode_ct.  */
              pd->ct_decoded = true;

              if (!bundle->ss_present || SS_DECODE_FORMAT (bundle->ss) != 0)
                {
                  print_text (info, " <SS missing>");
                  implement_me(0);
                  return;
                }

              cond = SS_DECODE_CT_COND (bundle->ss);
              if (cond == CT_COND_NONE)
                {
                  print_text (info, " <invalid SS:ct_condition>");
                  implement_me(0);
                  return;
                }
              ctpr = SS_DECODE_CTPR (bundle->ss);
              if (cond == CT_COND_NONE || ctpr != 0)
                {
                  print_text (info, " <invalid SS:ct_ctpr>");
                  implement_me(0);
                  return;
                }

              print_ct_cond (info, cond, SS_DECODE_CT_PRED (bundle->ss));
              break;
            }
        default:
          implement_me(0);
        }
    }

  print_nl (info);

  /* Special case for staa instructions.  */
  if (opcode->format == ALF10_MAS && ALS_DECODE_AA_INC (als))
    {
      print_text (info, "alc%d    ", ch);
      print_mnemonic (info, "incr");
      print_operand_alignment (info, 4);
      print_aaincr (info, ALS_DECODE_AA_INCR (als));
      print_am (info, bundle, ch);
      print_nl (info);
    }
}

static void
print_apb (struct disassemble_info *info, unsigned int ch, uint32_t apb,
           uint32_t disp)
{
  print_mnemonic (info, "apb");
  print_text (info, "@%d ", ch);
  print_named_uimm (info, ch == 0 ? "ct" : "dpl", (apb & FAPB_CT) != 0);
  print_text (info, ", ");
  print_named_uimm (info, "dcd", FAPB_DECODE_DCD (apb));
  print_text (info, ", ");
  print_named_uimm (info, "fmt", FAPB_DECODE_FMT (apb));
  print_text (info, ", ");
  print_named_uimm (info, "mrng", FAPB_DECODE_MRNG (apb));
  print_text (info, ", ");
  print_aad (info, FAPB_DECODE_AAD (apb));
  print_text (info, ", ");
  print_aaincr (info, FAPB_DECODE_INCR (apb));
  print_text (info, ", ");
  print_aaind (info, FAPB_DECODE_INDEX (apb));
  print_text (info, ", ");
  print_named_uimm (info, "asz", FAPB_DECODE_ASZ (apb));
  print_text (info, ", ");
  print_named_uimm (info, "abs", FAPB_DECODE_ABS (apb));
  print_text (info, ", ");
  print_named_hex (info, "disp", disp);
  print_nl (info);
}

static int
print_bundle (bfd_vma memaddr, struct disassemble_info *info)
{
  struct e2k_private_data *pd = info->private_data;
  struct e2k_unpacked_bundle bundle = { 0 };
  bfd_byte packet[64];
  int status;
  unsigned int len, i;

  info->bytes_per_line = pd->show_syllables ? 4 : 8;
  info->bytes_per_chunk = 4;
  info->display_endian = BFD_ENDIAN_LITTLE;
  //info->flags |= MULTILINE_OUTPUT;
  info->insn_type = dis_nonbranch;

  /* Read the first byte to check the length of bundle.  */
  status = (*info->read_memory_func) (memaddr, packet, 1, info);
  if (status != 0)
    {
      (*info->memory_error_func) (status, memaddr, info);
      return status;
    }

  len = HS_DECODE_BSZ (packet[0]);
  status = (*info->read_memory_func) (memaddr, packet, len, info);
  if (status != 0 || !bundle_unpack (pd, packet, len, &bundle))
    {
      unsigned int j;

      len = 16;
      status = (*info->read_memory_func) (memaddr, packet, len, info);
      if (status != 0)
        {
          (*info->memory_error_func) (status, memaddr, info);
          return status;
        }

      pd->lines = 0;
      pd->syll_cur = 0;
      pd->syll_len = 0;

      for (j = 0; j < 2; ++j)
        {
          push_syllable (pd, "FAPB", j, false);
          push_syllable (pd, "DISP", j, false);
        }
      print_syllable (pd, info);

      for (j = 0; j < 2; ++j)
        {
          uint32_t apb, disp;

          apb = bfd_getl32 (packet + j * 8);
          disp = bfd_getl32 (packet + j * 8 + 4);
          print_apb (info, j, apb, disp);
        }
    }
  else
    {
      print_syllable (pd, info);
      decode_hs (info, bundle.hs);

      if (bundle.ss_present)
        decode_ss (info, bundle.ss);

      decode_cs1 (info, &bundle);

      for (i = 0; i < ALC_COUNT; ++i)
        if (bundle.als_present[i])
          decode_al (memaddr, info, &bundle, i);

      decode_aau (info, &bundle);
      decode_pls (info, &bundle);
      decode_ct (memaddr, info, &bundle);
      decode_cs0 (memaddr, info, &bundle);

      if (info->insn_type == dis_branch ||
          info->insn_type == dis_jsr ||
          info->insn_type == dis_condjsr)
        memset (pd->ctpr, 0, sizeof (pd->ctpr));

      decode_nop (info, bundle.hs);
    }

  print_aligned_text (info, "--");

  /* Force print tail syllables.  */
  if (pd->show_syllables)
    {
      int syll_count = (int) len / info->bytes_per_line - 1;

      while (pd->lines < syll_count)
        print_nl (info);
    }

  return len;
}

int
print_insn_e2k (bfd_vma memaddr, struct disassemble_info *info)
{
  struct e2k_private_data *pd;

  if (info->private_data == NULL)
    info->private_data = calloc (1, sizeof (struct e2k_private_data));

  pd = info->private_data;

  pd->align = MAX((64 - clz64(memaddr) + 3) / 4, 8) + 5;

  if (info->disassembler_options != NULL)
    {
      set_isa_version (pd, info->mach);
      info->disassembler_options = NULL;
    }
  else if (pd->version == 0)
    {
      set_default_e2k_dis_options (pd);
      set_isa_version (pd, info->mach);
    }

  pd->ct_decoded = false;
  pd->lines = 0;
  pd->syll_cur = 0;
  pd->syll_len = 0;

  return print_bundle (memaddr, info);
}
