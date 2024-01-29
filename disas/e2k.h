/* e2k.h.  E2K opcode list for GDB, the GNU debugger.
   Copyright (C) 2022 Free Software Foundation, Inc.

   This file is part of GDB, GAS, and the GNU binutils.

   GDB, GAS, and the GNU binutils are free software; you can redistribute
   them and/or modify them under the terms of the GNU General Public
   License as published by the Free Software Foundation; either version
   3, or (at your option) any later version.

   GDB, GAS, and the GNU binutils are distributed in the hope that they
   will be useful, but WITHOUT ANY WARRANTY; without even the implied
   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
   the GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING3. If not,
   see <http://www.gnu.org/licenses/>.  */

#ifndef _E2K_H_
#define _E2K_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define SIGN_EXTEND(x, n) \
 ((long long) (x) << (sizeof (long long) - (n)) >> (sizeof (long long) - (n)))
#define EXTRACT_BITS(x, s, n) (((x) >> (s)) & ((1 << (n)) - 1))
#define SEXTRACT_BITS(x, s, n) SIGN_EXTEND (EXTRACT_BITS (x, s, n), n)
#define CLEAR_BITS(x, s, n) ((x) & ~(((1 << (n)) - 1) << (s)))
#define ENCODE_BITS(x, s, n) (((x) & ((1 << (n)) - 1)) << (s))
#define DEPOSIT_BITS(x, s, n, v) (CLEAR_BITS (x, s, n) | ENCODE_BITS (v, s, n))
#define DEPOSIT_FIELD(x, s0, n, v, s1) \
 DEPOSIT_BITS (x, s0, n, EXTRACT_BITS (v, s1, n))

#define NGPR        256
#define NPREG       32
#define NLPRED      7
#define NCMP        4
#define NPCNT       32
#define NCTPR       4
#define NIPR        8
#define NAAD        32
#define NAAINCR     8
#define NAASTI      16
#define NAAIND      16
#define NRNDPRED    32
#define NSR         256

extern const char e2k_reg_names_gpr[NGPR][8];
extern const char e2k_reg_names_gpr_d[NGPR][8];
extern const char e2k_reg_names_gpr_x[NGPR][8];
extern const char e2k_reg_names_gpr_q[NGPR][8];
extern const char e2k_reg_names_gpr_qp[NGPR][8];
extern const char e2k_reg_names_preg[NPREG][4];
extern const char e2k_reg_names_lpred[NLPRED][4];
extern const char e2k_reg_names_cmp[NCMP][8];
extern const char e2k_reg_names_pcnt[NPCNT][8];
extern const char e2k_reg_names_ctpr[NCTPR][8];
extern const char e2k_reg_names_ipr[NIPR][8];
extern const char e2k_reg_names_aad[NAAD][8];
extern const char e2k_reg_names_aaincr[NAAINCR][8];
extern const char e2k_reg_names_aasti[NAASTI][8];
extern const char e2k_reg_names_aaind[NAAIND][8];
extern const char *e2k_reg_names_rndpred[NRNDPRED];
extern const char *e2k_reg_names_sr[NSR];
extern const uint8_t e2k_reg_version_sr[NSR];

#define GPR_NAME_SIZE sizeof(*e2k_reg_names_gpr)
#define GPR_D_NAME_SIZE sizeof(*e2k_reg_names_gpr_d)
#define GPR_X_NAME_SIZE sizeof(*e2k_reg_names_gpr_x)
#define GPR_Q_NAME_SIZE sizeof(*e2k_reg_names_gpr_q)
#define GPR_QP_NAME_SIZE sizeof(*e2k_reg_names_gpr_qp)
#define PREG_NAME_SIZE sizeof(*e2k_reg_names_preg)
#define LPRED_NAME_SIZE sizeof(*e2k_reg_names_lpred)
#define CMP_NAME_SIZE sizeof(*e2k_reg_names_cmp)
#define PCNT_NAME_SIZE sizeof(*e2k_reg_names_pcnt)
#define CTPR_NAME_SIZE sizeof(*e2k_reg_names_ctpr)
#define IPR_NAME_SIZE sizeof(*e2k_reg_names_ipr)
#define AAD_NAME_SIZE sizeof(*e2k_reg_names_aad)
#define AAINCR_NAME_SIZE sizeof(*e2k_reg_names_aaincr)
#define AASTI_NAME_SIZE sizeof(*e2k_reg_names_aasti)
#define AAIND_NAME_SIZE sizeof(*e2k_reg_names_aaind)

#define ALC_COUNT 6

enum ales_present
{
  ALES_NONE,
  ALES_ALLOCATED,
  ALES_PRESENT,
};

struct e2k_unpacked_bundle
{
  uint32_t hs;
  uint32_t ss;
  uint32_t als[ALC_COUNT];
  uint32_t cs[2];
  uint32_t lts[4];
  uint32_t pls[3];
  uint32_t cds[3];
  uint16_t ales[6];
  uint16_t aas[6];
  bool ss_present;
  bool als_present[ALC_COUNT];
  bool cs_present[2];
  enum ales_present ales_present[ALC_COUNT];
  bool aas_present[6];
  bool lts_present[4];
  bool pls_present[3];
  bool cds_present[3];
};

/* Keep in sync with al_format_info.  */
enum al_format
{
  ALF1,           /* dst, src1, src2 */
  ALF1_MERGE,     /* dst, src1, src2, mrgc */
  ALF1_MAS,       /* dst, src1, src2[, mas=u7] */
  ALF2,           /* dst, src2 */
  ALF2_MOVTD,     /* dst|ctpr_in_alc0, src2 */
  ALF3_MAS,       /* src4, src1, src2[, mas=u7] */
  ALF7,           /* dst_preg, src1, src2 */
  ALF8,           /* dst_preg, src2 */
  ALF10_MAS,      /* dst, aad, aasti[, lit32][, mas=u7] */
  ALF11,          /* dst, src1, src2 */
  ALF11_LIT8,     /* dst, src1, src2, u8 */
  ALF11_MERGE,    /* dst, src1, src2, mrgc */
  ALF11_MAS,      /* dst, src1, src2[, mas=u7] */
  ALF12,          /* dst, src2 */
  ALF12_IBRANCHD, /* dst, src2 */
  ALF12_ICALLD,   /* dst, wbs, src2 */
  ALF12_PSHUFH,   /* dst, src2, u8 */
  ALF13_MAS,      /* src4, src1, src2[, mas=u7] */
  ALF15,          /* dst, sr */
  ALF16,          /* dst_sr, src2 */
  ALF17,          /* dst_preg, src1, src2 */
  ALF21,          /* dst, src1, src2, src3 */
  ALF21_LT3,      /* dst, src1, src2, src3|lit32 */
  ALF21_MERGE,    /* dst, src1, src2, src3, mrgc */
  ALF21_LOG,      /* dst, imm8, src1, src2, src3 */
  ALF21_LOG_LT3,  /* dst, imm8, src1, src2, src3|lit32 */
  ALF22,          /* dst, src2 */
  AAURR,          /* dst, aau */
  AAURW,          /* dst_aau, src2 */
  ALF_MAX,
};

struct al_format_info
{
  uint32_t als_mask;
  uint16_t ales_mask;
  uint8_t pair[6];
  const char *operands;
};

enum al_flags
{
  AF_ALIAS              = 0x01,
  AF_PAIR               = 0x02,
  AF_ALT_ALES03         = 0x04, /* ales.opc = 0x02 */
  AF_EXPLICIT_ALES25    = 0x08, /* ales.opc = 0x02, force ales for >=v4 */
  AF_REMOVED_IN_V2      = 0x10,
  AF_REMOVED_IN_V3      = 0x20,
};

enum gpr_size
{
  GS_NONE,
  GS_S, /* 32-bit */
  GS_D, /* 64-bit */
  GS_X, /* 80-bit */
  GS_Q, /* 128-bit, even/odd pair of two 64-bit */
  GS_P, /* 128-bit */
};

struct al_opcode
{
  const char *name;
  uint32_t als;
  uint16_t ales;
  uint8_t version[6];
#ifdef __GNUC__
  enum gpr_size dst : 4;
  enum gpr_size src1 : 4;
  enum gpr_size src2 : 4;
  enum gpr_size src3 : 4;
  enum al_format format : 8;
  enum al_flags flags : 8;
#else
  uint8_t dst : 4;
  uint8_t src1 : 4;
  uint8_t src2 : 4;
  uint8_t src3 : 4;
  uint8_t format;
  uint8_t flags;
#endif
};

extern const struct al_format_info al_format_info[ALF_MAX];
extern const struct al_opcode al_opcodes[];

#define TEST_OPCODE(x, o, m) (((x) & (m)) == (o))

/* HS */

#define HS_ENCODE_HOFF(x)       ENCODE_BITS ((x) / 4, 0, 4)
#define HS_ENCODE_BSZ(x)        ENCODE_BITS ((x) / 8 - 1, 4, 3)
#define HS_ENCODE_NOP(x)        ENCODE_BITS ((x) - 1, 7, 3)
#define HS_ENCODE_LOOP(x)       ENCODE_BITS (x, 10, 1)
#define HS_ENCODE_SIM(x)        ENCODE_BITS (x, 11, 1)
#define HS_ENCODE_SS(x)         ENCODE_BITS (x, 12, 1)
#define HS_ENCODE_MDL(x)        ENCODE_BITS (x, 13, 1)
#define HS_ENCODE_CS_MASK(x)    ENCODE_BITS (x, 14, 2)
#define HS_ENCODE_CS(x, i)      ENCODE_BITS (x, 14 + (i), 2)
#define HS_ENCODE_CDS_COUNT(x)  ENCODE_BITS (x, 16, 2)
#define HS_ENCODE_PLS_COUNT(x)  ENCODE_BITS (x, 18, 2)
#define HS_ENCODE_ALES_MASK(x)  ENCODE_BITS (x, 20, 6)
#define HS_ENCODE_ALES(x, i)    ENCODE_BITS (x, 20 + (i), 1)
#define HS_ENCODE_ALS_MASK(x)   ENCODE_BITS (x, 26, 6)
#define HS_ENCODE_ALS(x, i)     ENCODE_BITS (x, 26 + (i), 1)
#define HS_DECODE_HOFF(x)       (EXTRACT_BITS (x, 0, 4) * 4)
#define HS_DECODE_BSZ(x)        (EXTRACT_BITS (x, 4, 3) * 8 + 8)
#define HS_DECODE_NOP(x)        (EXTRACT_BITS (x, 7, 3) + 1)
#define HS_DECODE_LOOP(x)       EXTRACT_BITS (x, 10, 1)
#define HS_DECODE_SIM(x)        EXTRACT_BITS (x, 11, 1)
#define HS_DECODE_SS(x)         EXTRACT_BITS (x, 12, 1)
#define HS_DECODE_MDL(x)        EXTRACT_BITS (x, 13, 1)
#define HS_DECODE_CS_MASK(x)    EXTRACT_BITS (x, 14, 2)
#define HS_DECODE_CS(x, i)      EXTRACT_BITS (x, 14 + (i), 1)
#define HS_DECODE_CDS_COUNT(x)  (int) EXTRACT_BITS (x, 16, 2)
#define HS_DECODE_PLS_COUNT(x)  (int) EXTRACT_BITS (x, 18, 2)
#define HS_DECODE_ALES_MASK(x)  EXTRACT_BITS (x, 20, 6)
#define HS_DECODE_ALES(x, i)    EXTRACT_BITS (x, 20 + (i), 1)
#define HS_DECODE_ALS_MASK(x)   EXTRACT_BITS (x, 26, 6)
#define HS_DECODE_ALS(x, i)     EXTRACT_BITS (x, 26 + (i), 1)

/* SS */

enum ct_cond
{
  CT_COND_NONE                       = 0,    /* none */
  CT_COND_ALWAYS                     = 1,    /* unconditional */
  CT_COND_PREG                       = 2,    /* pN */
  CT_COND_NOT_PREG                   = 3,    /* ~pN */
  CT_COND_LOOP_END                   = 4,    /* loop_end */
  CT_COND_NOT_LOOP_END               = 5,    /* ~loop_end */
  CT_COND_PREG_OR_LOOP_END           = 6,    /* pN || loop_end */
  CT_COND_NOT_PREG_AND_NOT_LOOP_END  = 7,    /* ~pN && ~loop_end */
  CT_COND_MLOCK_OR_DTAL              = 8,    /* mlock */
                                             /* mlock || dt_al0134 */
  CT_COND_MLOCK_OR_CMP               = 9,    /* mlock || [~]cmpN */
                                             /* mlock || [~]cmpN || [~]cmpN */
                                             /* mlock || [~]clpN */
  CT_COND_CMP_CLP                    = 11,   /* {~}{cmp,clp}N */
  CT_COND_NOT_PREG_OR_LOOP_END       = 14,   /* ~pN || loop_end */
  CT_COND_PREG_AND_NOT_LOOP_END      = 15    /* pN && ~loop_end */
};

#define DISP_MIN -1073741824
#define DISP_MAX  1073741823

#define SS_ENCODE_CT_PRED(x)	    ENCODE_BITS (x, 0, 5)
#define SS_ENCODE_CT_COND(x)        ENCODE_BITS (x, 5, 5)
#define SS_ENCODE_CTPR(x)           ENCODE_BITS (x, 10, 2)
#define SS_ENCODE_FORMAT(x)         ENCODE_BITS (x, 20, 1)
#define SS_ENCODE_IPD(x)            ENCODE_BITS (x, 30, 2)

#define SS_DECODE_CT_PRED(x)	    EXTRACT_BITS (x, 0, 5)
#define SS_DECODE_CT_COND(x)        EXTRACT_BITS (x, 5, 5)
#define SS_DECODE_CTPR(x)           EXTRACT_BITS (x, 10, 2)
#define SS_DECODE_FORMAT(x)         EXTRACT_BITS (x, 20, 1)
#define SS_DECODE_IPD(x)            EXTRACT_BITS (x, 30, 2)

/* SS type0 */
#define SS_ENCODE_AAS_MAS(x)        ENCODE_BITS (x, 12, 4)
#define SS_ENCODE_AAS(x, i)         ENCODE_BITS (x, 12 + (i), 1)
#define SS_ENCODE_ALC(x)            ENCODE_BITS (x, 16, 2)
#define SS_ENCODE_ALCT(x)           ENCODE_BITS (x, 16, 1)
#define SS_ENCODE_ALCF(x)           ENCODE_BITS (x, 17, 1)
#define SS_ENCODE_ABP(x)            ENCODE_BITS (x, 18, 2)
#define SS_ENCODE_ABPT(x)           ENCODE_BITS (x, 18, 1)
#define SS_ENCODE_ABPF(x)           ENCODE_BITS (x, 19, 1)
#define SS_ENCODE_ABN(x)            ENCODE_BITS (x, 21, 2)
#define SS_ENCODE_ABNT(x)           ENCODE_BITS (x, 21, 1)
#define SS_ENCODE_ABNF(x)           ENCODE_BITS (x, 22, 1)
#define SS_ENCODE_ABG(x)            ENCODE_BITS (x, 23, 2)
#define SS_ENCODE_ABGD(x)           ENCODE_BITS (x, 23, 1)
#define SS_ENCODE_ABGI(x)           ENCODE_BITS (x, 24, 1)
#define SS_ENCODE_RP_LO(x)          ENCODE_BITS (x, 25, 1)
#define SS_ENCODE_VFDI(x)           ENCODE_BITS (x, 26, 1)
#define SS_ENCODE_RP_HI(x)          ENCODE_BITS (x, 27, 1)
#define SS_ENCODE_BAP(x)            ENCODE_BITS (x, 28, 1)
#define SS_ENCODE_EAP(x)            ENCODE_BITS (x, 29, 1)
#define SS_DECODE_AAS_MAS(x)        EXTRACT_BITS (x, 12, 4)
#define SS_DECODE_AAS(x, i)         EXTRACT_BITS (x, 12 + (i), 1)
#define SS_DECODE_ALC(x)            EXTRACT_BITS (x, 16, 2)
#define SS_DECODE_ALCT(x)           EXTRACT_BITS (x, 16, 1)
#define SS_DECODE_ALCF(x)           EXTRACT_BITS (x, 17, 1)
#define SS_DECODE_ABP(x)            EXTRACT_BITS (x, 18, 2)
#define SS_DECODE_ABPT(x)           EXTRACT_BITS (x, 18, 1)
#define SS_DECODE_ABPF(x)           EXTRACT_BITS (x, 19, 1)
#define SS_DECODE_ABN(x)            EXTRACT_BITS (x, 21, 2)
#define SS_DECODE_ABNT(x)           EXTRACT_BITS (x, 21, 1)
#define SS_DECODE_ABNF(x)           EXTRACT_BITS (x, 22, 1)
#define SS_DECODE_ABG(x)            EXTRACT_BITS (x, 23, 2)
#define SS_DECODE_ABGD(x)           EXTRACT_BITS (x, 23, 1)
#define SS_DECODE_ABGI(x)           EXTRACT_BITS (x, 24, 1)
#define SS_DECODE_RP_LO(x)          EXTRACT_BITS (x, 25, 1)
#define SS_DECODE_VFDI(x)           EXTRACT_BITS (x, 26, 1)
#define SS_DECODE_RP_HI(x)          EXTRACT_BITS (x, 27, 1)
#define SS_DECODE_BAP(x)            EXTRACT_BITS (x, 28, 1)
#define SS_DECODE_EAP(x)            EXTRACT_BITS (x, 29, 1)

/* SS type1 */
#define SS_TYPE1_MASK		    0x3fefe000
#define SS_ENCODE_PREP_CALL_HINT(x) ENCODE_BITS (x, 12, 1)
#define SS_DECODE_PREP_CALL_HINT(x) EXTRACT_BITS (x, 12, 1)

/* TODO: SS flushts and invts formats.  */

/* ALS */

#define STAA_MODE_AAD           0
#define STAA_MODE_AASTI         1
#define STAA_MODE_AAIND         2
#define STAA_MODE_AAINCR        3

#define ALS_IS_DST_CTPR(dst) ((dst) >= 0xd1 && (dst) <= 0xd3)

#define ALS_ENCODE_DST(x)	    ENCODE_BITS (x, 0, 8)
#define ALS_ENCODE_SRC4             ALS_ENCODE_DST
#define ALS_ENCODE_SRC2(x)          ENCODE_BITS (x, 8, 8)
#define ALS_ENCODE_SRC1(x)          ENCODE_BITS (x, 16, 8)
#define ALS_ENCODE_IMM3(x)          ENCODE_BITS (x, 24, 3)
#define ALS_ENCODE_OPC(x)           ENCODE_BITS (x, 24, 7)
#define ALS_ENCODE_SM(x)            ENCODE_BITS (x, 31, 1)
#define ALS_ENCODE_CMP_DST(x)       ENCODE_BITS (x, 0, 5)
#define ALS_ENCODE_CMP_OPC(x)       ENCODE_BITS (x, 5, 3)
#define ALS_ENCODE_AA_LTS(x)        ENCODE_BITS (x, 8, 2)
#define ALS_ENCODE_AA_MODE(x)       ENCODE_BITS (x, 10, 2)
#define ALS_ENCODE_AA_INC(x)        ENCODE_BITS (x, 10, 1)
#define ALS_ENCODE_AA_S(x)          ENCODE_BITS (x, 11, 1)
#define ALS_ENCODE_AA_INCR(x)       ENCODE_BITS (x, 12, 3)
#define ALS_ENCODE_AA_INDEX(x)      ENCODE_BITS (x, 15, 4)
#define ALS_ENCODE_AA_AAD(x)        ENCODE_BITS (x, 19, 5)
#define ALS_DECODE_DST(x)	    EXTRACT_BITS (x, 0, 8)
#define ALS_DECODE_SRC4             ALS_DECODE_DST
#define ALS_DECODE_SRC2(x)          EXTRACT_BITS (x, 8, 8)
#define ALS_DECODE_SRC1(x)          EXTRACT_BITS (x, 16, 8)
#define ALS_DECODE_SRC1(x)          EXTRACT_BITS (x, 16, 8)
#define ALS_DECODE_IMM3(x)          EXTRACT_BITS (x, 24, 3)
#define ALS_DECODE_OPC(x)           EXTRACT_BITS (x, 24, 7)
#define ALS_DECODE_SM(x)            EXTRACT_BITS (x, 31, 1)
#define ALS_DECODE_CMP_DST(x)       EXTRACT_BITS (x, 0, 5)
#define ALS_DECODE_CMP_OPC(x)       EXTRACT_BITS (x, 5, 3)
#define ALS_DECODE_AA_LTS(x)        EXTRACT_BITS (x, 8, 2)
#define ALS_DECODE_AA_MODE(x)       EXTRACT_BITS (x, 10, 2)
#define ALS_DECODE_AA_S(x)          EXTRACT_BITS (x, 11, 1)
#define ALS_DECODE_AA_INC(x)        EXTRACT_BITS (x, 10, 1)
#define ALS_DECODE_AA_INCR(x)       EXTRACT_BITS (x, 12, 3)
#define ALS_DECODE_AA_INDEX(x)      EXTRACT_BITS (x, 15, 4)
#define ALS_DECODE_AA_AAD(x)        EXTRACT_BITS (x, 19, 5)

/* ALES */

#define ALES_ENCODE_SRC3(x)	    ENCODE_BITS (x, 0, 8)
#define ALES_ENCODE_OPC(x)          ENCODE_BITS (x, 8, 8)
#define ALES_DECODE_SRC3(x)         EXTRACT_BITS (x, 0, 8)
#define ALES_DECODE_OPC(x)          EXTRACT_BITS (x, 8, 8)

/* CS0 */

#define CS0_ENCODE_DISP_RAW(x)	    ENCODE_BITS ((x), 0, 28)
#define CS0_ENCODE_DISP(x)          ENCODE_BITS ((x) >> 3, 0, 28)
#define CS0_ENCODE_CTPR(x)          ENCODE_BITS (x, 30, 2)
#define CS0_DECODE_DISP_RAW(x)      EXTRACT_BITS (x, 0, 28)
#define CS0_DECODE_DISP(x)          (((int32_t) (EXTRACT_BITS (x, 0, 28) << 4)) >> 1)
#define CS0_DECODE_CTPR(x)          EXTRACT_BITS (x, 30, 2)
#define CS0_IS_DISP_VALID(x) \
 (CS0_DECODE_DISP (CS0_ENCODE_DISP (x)) == (int32_t) (x))

#define PREP_OPCODE_CS0             0x00000000
#define PREP_OPCODE_MASK_CS0        0x30000000
#define PREP_ENCODE_CTPR_CS0        CS0_ENCODE_CTPR
#define PREP_ENCODE_DISP_CS0        CS0_ENCODE_DISP
#define PREP_DECODE_CTPR_CS0        CS0_DECODE_CTPR
#define PREP_DECODE_DISP_CS0        CS0_DECODE_DISP
#define PREP_IS_DISP_VALID_CS0      CS0_IS_DISP_VALID

#define PREP_APB_OPCODE_CS0         0x90000000
#define PREP_APB_OPCODE_MASK_CS0    0xf0000000
#define PREP_APB_ENCODE_DISP_CS0    CS0_ENCODE_DISP
#define PREP_APB_DECODE_DISP_CS0    CS0_DECODE_DISP
#define PREP_APB_IS_DISP_VALID_CS0  CS0_IS_DISP_VALID

#define PREP_SYS_OPCODE_CS0         0x20000000
#define PREP_SYS_OPCODE_MASK_CS0    0x30000000
#define PREP_SYS_ENCODE_CTPR_CS0    CS0_ENCODE_CTPR
#define PREP_SYS_ENCODE_DISP_CS0    CS0_ENCODE_DISP_RAW
#define PREP_SYS_DECODE_CTPR_CS0    CS0_DECODE_CTPR
#define PREP_SYS_DECODE_DISP_CS0    CS0_DECODE_DISP_RAW

#define PREP_RET_OPCODE_CS0         0xf0000000
#define PREP_RET_OPCODE_MASK_CS0    0xffffffff

#define GETTSD_OPCODE_CS0           0x30000001
#define GETTSD_OPCODE_MASK_CS0      0x3fffffff
#define GETTSD_ENCODE_CTPR_CS0      CS0_ENCODE_CTPR
#define GETTSD_DECODE_CTPR_CS0      CS0_DECODE_CTPR

#define IBRANCH_OPCODE_CS0          0x00000000
#define IBRANCH_OPCODE_MASK_CS0     0xf0000000
#define IBRANCH_ENCODE_DISP_CS0     CS0_ENCODE_DISP
#define IBRANCH_DECODE_DISP_CS0     CS0_DECODE_DISP
#define IBRANCH_IS_DISP_VALID_CS0   CS0_IS_DISP_VALID

#define PREF_OPCODE_CS0             0x10000000
#define PREF_OPCODE_MASK_CS0        0xf0000000
#define PREF_IPD_CS0                (1u << 3)
#define PREF_ENCODE_IPR_CS0(x)      ENCODE_BITS (x, 0, 3)
#define PREF_ENCODE_DISP_CS0(x)     ENCODE_BITS (x, 4, 24)
#define PREF_DECODE_IPR_CS0(x)      EXTRACT_BITS (x, 0, 3)
#define PREF_DECODE_DISP_CS0(x)     EXTRACT_BITS (x, 4, 24)

#define PUTTSD_OPCODE_CS0           0x20000000
#define PUTTSD_OPCODE_MASK_CS0      0xf0000000
#define PUTTSD_ENCODE_DISP_CS0      CS0_ENCODE_DISP
#define PUTTSD_DECODE_DISP_CS0      CS0_DECODE_DISP
#define PUTTSD_IS_DISP_VALID_CS0    CS0_IS_DISP_VALID

#define DONE_OPCODE_CS0             0x30000000
#define DONE_OPCODE_MASK_CS0        0xf3fffff8
#define DONE_ENCODE_TYPE_CS0(x)     ENCODE_BITS (x, 0, 3)
#define DONE_DECODE_TYPE_CS0(x)     EXTRACT_BITS (x, 0, 3)
#define DONE_TYPE_DONE              0
#define DONE_TYPE_IRET              2
#define DONE_TYPE_HRET              3
#define DONE_TYPE_GLAUNCH           4
#define DONE_FDAM_CS0               (1u << 26)
#define DONE_TRAR_CS0               (1u << 27)

/* CS1 */

#define SETWD_OPCODE_CS1            0x00000000
#define SETWD_OPCODE_MASK_CS1       0xe1800000
#define SETWD_OPCODE_LTS0           0x00000000
#define SETWD_OPCODE_MASK_LTS0      0x00000003
#define SETWD_SETTR_CS1             0x02000000
#define SETWD_SETBN_CS1             0x04000000
#define SETWD_SETBP_CS1             0x08000000
#define SETWD_VFRPSZ_CS1            0x10000000

#define SETTR_ENCODE_TYPE_LTS0(x)   ENCODE_BITS (x, 17, 16)
#define SETTR_DECODE_TYPE_LTS0(x)   EXTRACT_BITS (x, 17, 16)

#define VFRPSZ_ENCODE_RPSZ_LTS0(x)  ENCODE_BITS (x, 12, 5)
#define VFRPSZ_DECODE_RPSZ_LTS0(x)  EXTRACT_BITS (x, 12, 5)

#define SETWD_MCN_LTS0              0x00000004
#define SETWD_DBL_LTS0              0x00000008
#define SETWD_NFX_LTS0              0x00000010
#define SETWD_ENCODE_WSZ_LTS0(x)    ENCODE_BITS ((x) / 2, 5, 7)
#define SETWD_DECODE_WSZ_LTS0(x)    (2 * EXTRACT_BITS (x, 5, 7))

#define SETBN_ENCODE_RBS_CS1(x)     ENCODE_BITS ((x) / 2, 0, 6)
#define SETBN_ENCODE_RSZ_CS1(x)     ENCODE_BITS ((x) / 2 - 1, 6, 6)
#define SETBN_ENCODE_RCUR_CS1(x)    ENCODE_BITS ((x) / 2, 12, 6)
#define SETBN_DECODE_RBS_CS1(x)     (2 * EXTRACT_BITS (x, 0, 6))
#define SETBN_DECODE_RSZ_CS1(x)     (2 + 2 * EXTRACT_BITS (x, 6, 6))
#define SETBN_DECODE_RCUR_CS1(x)    (2 * EXTRACT_BITS (x, 12, 6))

#define SETBP_ENCODE_PSZ_CS1(x)     ENCODE_BITS (x, 18, 5)
#define SETBP_DECODE_PSZ_CS1(x)     EXTRACT_BITS (x, 18, 5)

#define SETBN_SETBP_OPCODE_CS1      0x40000000
#define SETBN_SETBP_OPCODE_MASK_CS1 0xe3800000

#define SETEI_OPCODE_CS1            0x20000000
#define SETEI_OPCODE_MASK_CS1       0xf8000000
#define SETEI_ENCODE_VALUE_CS1(x)   ENCODE_BITS (x, 0, 8)
#define SETEI_DECODE_VALUE_CS1(x)   EXTRACT_BITS (x, 0, 8)

#define SETSFT_CS1_OPCODE           0x28000000
#define SETSFT_CS1_OPCODE_MASK      0xf8000000

#define WAIT_OPCODE_CS1             0x30000000
#define WAIT_OPCODE_MASK_CS1        0xfffffe00
#define WAIT_ALL_C_CS1              (1u << 0)
#define WAIT_ALL_E_CS1              (1u << 1)
#define WAIT_ST_C_CS1               (1u << 2)
#define WAIT_LD_C_CS1               (1u << 3)
#define WAIT_FL_C_CS1               (1u << 4)
#define WAIT_MA_C_CS1               (1u << 5)
#define WAIT_TRAP_CS1               (1u << 6)
#define WAIT_SAL_CS1                (1u << 7)
#define WAIT_SAS_CS1                (1u << 8)

#define CALL_OPCODE_CS1             0x50000000
#define CALL_OPCODE_MASK_CS1        0xffffff80
#define ICALL_OPCODE_CS1            0x50000080
#define ICALL_OPCODE_MASK_CS1       0xffffff80
#define CALL_ENCODE_WBS_CS1(x)      ENCODE_BITS ((x) / 2, 0, 7)
#define CALL_DECODE_WBS_CS1(x)      (EXTRACT_BITS (x, 0, 7) * 2)

#define SETMAS_OPCODE_CS1           0x60000000
#define SETMAS_OPCODE_MASK_CS1      0xf0000000
#define SETMAS_OFFSET_FROM_CHAN(i)  (7 * ((i) == 5 ? (i) - 2 : ((i) >= 2 ? (i) - 1 : (i))))
#define SETMAS_ENCODE_CS1(x, ch)    ENCODE_BITS (x, SETMAS_OFFSET_FROM_CHAN (5 - ch), 7)
#define SETMAS_DECODE_CS1(x, ch)    EXTRACT_BITS (x, SETMAS_OFFSET_FROM_CHAN (5 - ch), 7)

#define HAS_SETMAS(bundle) (bundle->cs_present[1] && \
  TEST_OPCODE (bundle->cs[1], SETMAS_OPCODE_CS1, SETMAS_OPCODE_MASK_CS1))

#define FLUSH_CS1_OPCODE            0x70000000
#define FLUSH_CS1_OPCODE_MASK       0xfffffffc
#define FLUSH_R_CS1                 0x00000001
#define FLUSH_C_CS1                 0x00000002

#define VFBG_OPCODE_CS1             0x80000000
#define VFBG_OPCODE_MASK_CS1        0xfffe0000
#define VFBG_CHKM4_CS1              0x00010000
#define VFBG_ENCODE_UMASK_CS1(x)    ENCODE_BITS (x, 0, 8)
#define VFBG_ENCODE_DMASK_CS1(x)    ENCODE_BITS (x, 8, 8)
#define VFBG_DECODE_UMASK_CS1(x)    EXTRACT_BITS (x, 0, 8)
#define VFBG_DECODE_DMASK_CS1(x)    EXTRACT_BITS (x, 8, 8)

/* CDS */

#define PSRC_PCNT                   0x40
#define PSRC_PREG                   0x60
#define RLP_ENCODE_PSRC(x)          ENCODE_BITS (x, 0, 7)
#define RLP_ENCODE_INV(x, i)        ENCODE_BITS (x, 7 + (i), 1)
#define RLP_ENCODE_ALC(x, i)        ENCODE_BITS (x, 10 + (i), 1)
#define RLP_DECODE_PSRC(x)          EXTRACT_BITS (x, 0, 7)
#define RLP_DECODE_INV(x, i)        EXTRACT_BITS (x, 7 + (i), 1)
#define RLP_DECODE_ALC(x, i)        EXTRACT_BITS (x, 10 + (i), 1)
#define RLP_AM                      0x2000
#define RLP_CLUSTER                 0x4000
#define RLP_MRGC                    0x8000

/* AAS 2..5 */

#define MOVA_ENCODE_IND(x)          ENCODE_BITS (x, 1, 5)
#define MOVA_ENCODE_AREA(x)         ENCODE_BITS (x, 6, 6)
#define MOVA_DECODE_IND(x)          EXTRACT_BITS (x, 1, 5)
#define MOVA_DECODE_AREA(x)         EXTRACT_BITS (x, 6, 6)

#define MOVA_AM                     0x0001
#define MOVA_OPC_MASK               0x7000
#define MOVA_NONE                   0x0000
#define MOVAB                       0x1000
#define MOVAH                       0x2000
#define MOVAW                       0x3000
#define MOVAD                       0x4000
#define MOVAQ                       0x5000
#define MOVA_UNDEFINED              0x6000
#define MOVAQP                      0x7000
#define MOVA_BE                     0x8000

/* PLS */

#define ELP_ENCODE(x, i)            ENCODE_BITS (x, (i) ? 16 : 24, 8)
#define ELP_DECODE(x, i)            EXTRACT_BITS (x, (i) ? 16 : 24, 8)

#define LP_ENCODE_PREG(x)           ENCODE_BITS (x, 0, 5)
#define LP_DECODE_PREG(x)           EXTRACT_BITS (x, 0, 5)
#define LP_WRITE_PREG               (1u << 5)
#define LP_ENCODE_LPSRC(x, i)       ENCODE_BITS (x, (i) ? 6 : 10, 3)
#define LP_DECODE_LPSRC(x, i)       EXTRACT_BITS (x, (i) ? 6 : 10, 3)
#define LP_ENCODE_LPSRC_INV(x, i)   ENCODE_BITS (x, (i) ? 9 : 13, 1)
#define LP_DECODE_LPSRC_INV(x, i)   EXTRACT_BITS (x, (i) ? 9 : 13, 1)
#define LP_ANDP                     (0u << 14)
#define LP_LANDP                    (1u << 14)
#define LP_MOVEP                    (3u << 14)
#define LP_OPC_MASK                 (3u << 14)

/* FAPB */

#define FAPB_ENCODE_ABS(x)          ENCODE_BITS (x, 0, 5)
#define FAPB_ENCODE_ASZ(x)          ENCODE_BITS (x, 5, 3)
#define FAPB_ENCODE_INDEX(x)        ENCODE_BITS (x, 8, 4)
#define FAPB_ENCODE_INCR(x)         ENCODE_BITS (x, 12, 3)
#define FAPB_ENCODE_AAD(x)          ENCODE_BITS (x, 15, 5)
#define FAPB_ENCODE_MRNG(x)         ENCODE_BITS (x, 20, 5)
#define FAPB_ENCODE_FMT(x)          ENCODE_BITS (x, 25, 3)
#define FAPB_ENCODE_DCD(x)          ENCODE_BITS (x, 28, 2)
#define FAPB_DECODE_ABS(x)          EXTRACT_BITS (x, 0, 5)
#define FAPB_DECODE_ASZ(x)          EXTRACT_BITS (x, 5, 3)
#define FAPB_DECODE_INDEX(x)        EXTRACT_BITS (x, 8, 4)
#define FAPB_DECODE_INCR(x)         EXTRACT_BITS (x, 12, 3)
#define FAPB_DECODE_AAD(x)          EXTRACT_BITS (x, 15, 5)
#define FAPB_DECODE_MRNG(x)         EXTRACT_BITS (x, 20, 5)
#define FAPB_DECODE_FMT(x)          EXTRACT_BITS (x, 25, 3)
#define FAPB_DECODE_DCD(x)          EXTRACT_BITS (x, 28, 2)

#define FAPB_SI                     (1u << 30)
#define FAPB_CT                     (1u << 31)
#define FAPB_DPL                    (1u << 31)

#endif /* _E2K_H_ */
