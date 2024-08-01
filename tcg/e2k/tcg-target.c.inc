/*
 * SPDX-License-Identifier: LGPL-2.0-or-later
 * SPDX-FileCopyrightText: 2024 Denis Drakhnia <numas13@gmail.com>
 *
 * Based on RISC-V and i386 backends.
 */

#include "../tcg-ldst.c.inc"

#define E2K_LATENCY_PREP 5
#define E2K_LATENCY_PREP_RETURN 6

#ifdef CONFIG_DEBUG_TCG
static const char * const tcg_target_reg_names[TCG_TARGET_NB_REGS] = {
    "b0",
    "b1",
    "b2",
    "b3",
    "b4",
    "b5",
    "b6",
    "b7",

    "r0",
    "r1",
    "r2",
    "r3",
    "r4",
    "r5",
    "r6",
    "r7",

    "g16",
    "g17",
    "g18",
    "g19",
    "g20",
    "g21",
    "g22",
    "g23",
    "g24",
    "g25",
    "g26",
    "g27",
    "g28",
    "g29",
    "g30",
    "g31",
};
#endif /* CONFIG_DEBUG_TCG */

static const int tcg_target_reg_alloc_order[] = {
    /* Call saved registers */
    /* TCG_REG_R0 reservered for TCG_AREG0 */
    TCG_REG_R1,
    TCG_REG_R2,
    TCG_REG_R3,
    TCG_REG_R4,
    TCG_REG_R5,
    /* TCG_REG_R6 reservered for TCG_REG_SP */
    TCG_REG_R7,

    /* Call clobbered registers */
    TCG_REG_G16,
    TCG_REG_G17,
    TCG_REG_G18,
    TCG_REG_G19,
    TCG_REG_G20,
    TCG_REG_G21,
    TCG_REG_G22,
    TCG_REG_G23,
    TCG_REG_G24,
    TCG_REG_G25,
    TCG_REG_G26,
    TCG_REG_G27,
    TCG_REG_G28,
    TCG_REG_G29,
    TCG_REG_G30,
    TCG_REG_G31,

    /* Argument registers */
    TCG_REG_B0,
    TCG_REG_B1,
    TCG_REG_B2,
    TCG_REG_B3,
    TCG_REG_B4,
    TCG_REG_B5,
    TCG_REG_B6,
    TCG_REG_B7,
};

static const int tcg_target_call_iarg_regs[] = {
    TCG_REG_B0,
    TCG_REG_B1,
    TCG_REG_B2,
    TCG_REG_B3,
    TCG_REG_B4,
    TCG_REG_B5,
    TCG_REG_B6,
    TCG_REG_B7,
};

static TCGReg tcg_target_call_oarg_reg(TCGCallReturnKind kind, int slot)
{
    tcg_debug_assert(kind == TCG_CALL_RET_NORMAL);
    tcg_debug_assert(slot >= 0 && slot <= 1);
    return TCG_REG_B0 + slot;
}

#define TCG_CT_CONST_U      0x100
#define TCG_CT_CONST_I      0x200

#define ALL_GENERAL_REGS    MAKE_64BIT_MASK(0, 32)

/* test if a constant matches the constraint */
static bool tcg_target_const_match(int64_t val, int ct,
                                   TCGType type, TCGCond cond, int vece)
{
    if (ct & TCG_CT_CONST) {
        return 1;
    }
    /*
     * Zero extended from 5 bits: [0, 0x1f]
     * Used for src1 operand.
     */
    if ((ct & TCG_CT_CONST_U) && val >= 0 && val <= 0x1f) {
        return 1;
    }
    /*
     * Zero extended from 64 bits: [0, 0xffffffffffffffff]
     * Used for src2 operand.
     */
    if (ct & TCG_CT_CONST_I) {
        return 1;
    }

    return 0;
}

/*
 * E2K Operands
 */

#define WBS 4

typedef enum {
    CTPR_NONE = 0,
    CTPR1 = 1,
    CTPR2 = 2,
    CTPR3 = 3,
} CtprReg;

typedef enum {
    PREG0,
    PREG1,
    PREG2,
    PREG3,
    PREG4,
    PREG5,
    PREG6,
    PREG7,
    PREG8,
    PREG9,
    PREG10,
    PREG11,
    PREG12,
    PREG13,
    PREG14,
    PREG15,
    PREG16,
    PREG17,
    PREG18,
    PREG19,
    PREG20,
    PREG21,
    PREG22,
    PREG23,
    PREG24,
    PREG25,
    PREG26,
    PREG27,
    PREG28,
    PREG29,
    PREG30,
    PREG31,

    PREG_TMP0 = PREG31,
    PREG_TMP1 = PREG30,
    PREG_TMP2 = PREG29,
} PredReg;

#define CTPR(reg) encode_ctpr(reg)
static inline uint8_t encode_ctpr(CtprReg ctpr)
{
    return 0xd0 | ctpr;
}

#define GPR(reg) encode_gpr(reg)
static uint8_t encode_gpr(TCGReg reg)
{
    switch (reg) {
    case TCG_REG_B0:
    case TCG_REG_B1:
    case TCG_REG_B2:
    case TCG_REG_B3:
    case TCG_REG_B4:
    case TCG_REG_B5:
    case TCG_REG_B6:
    case TCG_REG_B7:
        return reg - TCG_REG_B0;

    case TCG_REG_R0:
    case TCG_REG_R1:
    case TCG_REG_R2:
    case TCG_REG_R3:
    case TCG_REG_R4:
    case TCG_REG_R5:
    case TCG_REG_R6:
    case TCG_REG_R7:
        return 128 + reg - TCG_REG_R0;

    case TCG_REG_G16:
    case TCG_REG_G17:
    case TCG_REG_G18:
    case TCG_REG_G19:
    case TCG_REG_G20:
    case TCG_REG_G21:
    case TCG_REG_G22:
    case TCG_REG_G23:
    case TCG_REG_G24:
    case TCG_REG_G25:
    case TCG_REG_G26:
    case TCG_REG_G27:
    case TCG_REG_G28:
    case TCG_REG_G29:
    case TCG_REG_G30:
    case TCG_REG_G31:
        return 240 + reg - TCG_REG_G16;

    default:
        g_assert_not_reached();
    }
}

#define UIMM5(val) encode_src1_uimm5(val)
static uint8_t encode_src1_uimm5(uint8_t val)
{
    tcg_debug_assert(val < 32);
    return 0xc0 | (val & 31);
}

#define UIMM4(val) encode_src2_uimm4(val)
static uint8_t encode_src2_uimm4(uint8_t val)
{
    tcg_debug_assert(val < 16);
    return 0xc0 | (val & 15);
}

#define UIMM0 UIMM4(0)

#if 0
static uint8_t encode_src2_lit16(int pos)
{
    g_assert(pos < 4);
    switch (pos) {
    case 0: return 0xd0;
    case 1: return 0xd4;
    case 2: return 0xd1;
    case 3: return 0xd5;
    default: g_assert_not_reached();
    }
}
#endif

static uint8_t encode_src2_lit32(int pos)
{
    g_assert(pos < 4);
    return 0xd8 | pos;
}

static uint8_t encode_src2_lit64(int pos)
{
    g_assert(pos < 3);
    return 0xdc | pos;
}

/*
 * E2K Opcodes
 */

typedef struct {
  uint32_t ss;
  uint32_t als[6];
  uint32_t cs[2];
  uint16_t ales[6];
  uint16_t aas_dst[2];
  uint16_t aas[4];
  uint32_t lts[4];
  uint32_t pls[3];
  uint32_t cds[3];

  int64_t lit[6];

  uint8_t latency;
  bool loop_mode;
  bool ss_present;
  bool als_present[6];
  bool cs_present[2];
  bool ales_present[6];
  bool aas_present[4];
  bool lts_present[4];
  bool pls_present[3];
  bool cds_present[3];
  bool lit_present[6];

  uint8_t len;
  uint8_t cs_offset[2];
} Bundle;

static void e2k_set_latency(Bundle *bundle, int latency)
{
    if (bundle->latency < latency) {
        bundle->latency = latency;
    }
}

static void e2k_encode_literals(Bundle *bundle)
{
    uint8_t src2;

    for (int i = 0; i < 6; ++i) {
        int64_t val = bundle->lit[i];

        if (!bundle->lit_present[i]) {
            continue;
        }

        tcg_debug_assert(extract32(bundle->als[i], 8, 8) == 0);

        if (val >= 0 && val <= 15) {
            src2 = UIMM4(val);
            goto done;
        }

        if (val == (int16_t) val) {
            // TODO: lit16
        }

        if (val == (int32_t) val) {
            for (int j = 0; j < 4; ++j) {
                if (bundle->lts_present[j]) {
                    continue;
                }
                bundle->lts_present[j] = true;
                bundle->lts[j] = val;
                src2 = encode_src2_lit32(j);
                goto done;
            }
            goto err;
        }

        for (int j = 0; j < 3; ++j) {
            if (bundle->lts_present[j] || bundle->lts_present[j + 1]) {
                continue;
            }
            bundle->lts_present[j    ] = true;
            bundle->lts_present[j + 1] = true;
            bundle->lts[j    ] = val;
            bundle->lts[j + 1] = val >> 32;
            src2 = encode_src2_lit64(j);
            goto done;
        }

err:
        tcg_debug_assert(0 && "failed to allocate LTS");

done:
        bundle->als[i] |= (uint32_t) src2 << 8;
    }
}

static void e2k_out_start(Bundle *bundle)
{
    memset(bundle, 0, sizeof(*bundle));
}

static void e2k_out_end(TCGContext *s, Bundle *bundle)
{
    uint32_t buffer[32], syll = 0;
    int cur, count, lts, pls, cds;

    cur = count = 0;

    /* propagate literals to LTS */
    e2k_encode_literals(bundle);

    /* SS must be present if any AAS is present */
    if (!bundle->ss_present) {
        for (int i = 0; i < 4; ++i) {
            if (bundle->aas_present[i]) {
                bundle->ss_present = true;
                bundle->ss = 0;
                break;
            }
        }
    }

    /* HS */
    buffer[cur++] = 0;

    if (bundle->latency) {
        buffer[0] |= ((bundle->latency > 8 ? 8 : bundle->latency) - 1) << 7;
    }

    if (bundle->ss_present) {
        buffer[0] |= 1 << 12;
        buffer[cur++] = bundle->ss;
    }

    for (int i = 0; i < 6; ++i) {
        if (bundle->als_present[i]) {
            buffer[0] |= 1 << (26 + i);
            buffer[cur++] = bundle->als[i];
        }
    }

    if (bundle->cs_present[0]) {
        bundle->cs_offset[0] = cur;
        buffer[0] |= 1 << 14;
        buffer[cur++] = bundle->cs[0];
    }

    /* ALES2/ALES5 is not like any other half-syllables */
    if (bundle->ales_present[2] || bundle->ales_present[5]) {
        syll = 0;
        for (int j = 0; j < 2; ++j) {
            int i = j == 0 ? 2 : 5;
            syll <<= 16;
            if (bundle->ales_present[i]) {
                syll |= bundle->ales[i];
            }
        }
        buffer[cur++] = syll;
    }

    if (bundle->cs_present[1]) {
        bundle->cs_offset[1] = cur;
        buffer[0] |= 1 << 15;
        buffer[cur++] = bundle->cs[1];
    }

    /* set offset to half-syllables */
    buffer[0] |= cur - 1;

    for (int i = 0; i < 6; ++i) {
        if (bundle->ales_present[i]) {
            buffer[0] |= 1 << (20 + i);

            /* ALES2/ALES5 handled above */
            if (i == 2 || i == 5) {
                continue;
            }

            syll <<= 16;
            syll |= bundle->ales[i];
            if (count & 1) {
                buffer[cur++] = syll;
            }
            count += 1;
        }
    }

    for (int i = 0; i < 4; i += 2) {
        if (bundle->aas_present[i] || bundle->aas_present[i + 1]) {
            syll <<= 16;
            syll |= bundle->aas_dst[i / 2];
            if (count & 1) {
                buffer[cur++] = syll;
            }
            count += 1;
        }
    }

    for (int i = 0; i < 4; ++i) {
        if (bundle->aas_present[i]) {
            buffer[1] |= 1 << (12 + i);
            syll <<= 16;
            syll |= bundle->aas[i];
            if (count & 1) {
                buffer[cur++] = syll;
            }
            count += 1;
        }
    }

    if (count & 1) {
        syll <<= 16;
        buffer[cur++] = syll;
    }

    lts = 4, pls = 3, cds = 3;

    for (; lts; --lts) {
        if (bundle->lts_present[lts - 1]) {
            break;
        }
    }
    for (; pls; --pls) {
        if (bundle->pls_present[pls - 1]) {
            break;
        }
    }
    for (; cds; --cds) {
        if (bundle->cds_present[cds - 1]) {
            break;
        }
    }

    bundle->len = cur + lts + pls + cds;

    /* check for bundle overflow */
    tcg_debug_assert(bundle->len <= 16);

    /* the bundle size must be a multiple of 8 bytes */
    if (bundle->len & 1) {
        /* insert padding */
        buffer[cur++] = 0;
        bundle->len += 1;
    }

    buffer[0] |= (bundle->len / 2 - 1) << 4;
    buffer[0] |= (uint32_t) cds << 16;
    buffer[0] |= (uint32_t) pls << 18;

    for (int i = lts; i; --i) {
        buffer[cur++] = bundle->lts[i - 1];
    }

    for (int i = pls; i; --i) {
        buffer[cur++] = bundle->pls[i - 1];
    }

    for (int i = cds; i; --i) {
        buffer[cur++] = bundle->cds[i - 1];
    }

    for (int i = 0; i < bundle->len; ++i) {
        tcg_out32(s, buffer[i]);
    }

    /* output additional nops to fit latency requirements */
    for (int i = bundle->latency - 8; i > 0; i -= 8) {
        tcg_out32(s, ((i > 8 ? 8 : i) - 1) << 7);
        tcg_out32(s, 0);
    }
}

typedef enum {
#define DEF(NAME, LAT, ALES, ALS, ALC0, ALC1, ALC2, ALC3, ALC4, ALC5) \
    ALC_##NAME,
#include "tcg-target-alc-opcodes.c.inc"
#undef DEF
} AlcOpc;

typedef struct AlcOpcode {
    uint64_t opc;
    uint8_t latency;
    uint8_t ver[6];
} AlcOpcode;

static const struct AlcOpcode alc_opcodes[] = {
#define DEF(NAME, LAT, ALES, ALS, ALC0, ALC1, ALC2, ALC3, ALC4, ALC5) \
    [ALC_##NAME] = { \
        ((uint64_t) (ALES) << 32) | ALS, \
        LAT, \
        { ALC0, ALC1, ALC2, ALC3, ALC4, ALC5 }, \
    },
#include "tcg-target-alc-opcodes.c.inc"
#undef DEF
};

static void e2k_out_rlp_(Bundle *bundle, int alc, bool mrgc, PredReg preg, bool inv)
{
    bool cluster = alc > 2;

    if (cluster) {
        alc -= 3;
    }

    for (int i = 0; i < 3; ++i) {
        if (bundle->cds_present[i]) {
            // TODO: e2k reuse cds/rlp
        } else {
            uint16_t rlp = 0x60 | preg;

            rlp |= (uint16_t) inv << (7 + alc);
            rlp |= 1 << (10 + alc);
            rlp |= (uint16_t) cluster << 14;
            rlp |= (uint16_t) mrgc << 15; 

            bundle->cds_present[i] = true;
            bundle->cds[i] = rlp;
            return;
        }
    }

    tcg_debug_assert(0 && "failed to insert RLP/MRGC");
}

static void e2k_out_rlp(Bundle *bundle, int alc, PredReg preg, bool inv)
{
    e2k_out_rlp_(bundle, alc, false, preg, inv);
}

static void e2k_out_mrgc(Bundle *bundle, int alc, PredReg preg, bool inv)
{
    e2k_out_rlp_(bundle, alc, true, preg, inv);
}

static void e2k_out_lit(Bundle *bundle, int alc, int64_t val)
{
    tcg_debug_assert(!bundle->lit_present[alc]);
    bundle->lit_present[alc] = true;
    bundle->lit[alc] = val;
}

static void e2k_out_alf(Bundle *bundle, int alc, AlcOpc opc, uint8_t dst,
                        uint8_t src1, uint8_t src2, uint8_t src3)
{
    const AlcOpcode *info = &alc_opcodes[opc];

    tcg_debug_assert(info->ver[alc] != 0 && __iset__ >= info->ver[alc]);
    tcg_debug_assert(!bundle->als_present[alc]);

    bundle->als_present[alc] = true;
    bundle->als[alc] = info->opc | ((uint32_t) src1 << 16) | ((uint32_t) src2 << 8) | dst;

    if (info->opc >> 32) {
        tcg_debug_assert(!bundle->ales_present[alc]);
        bundle->ales_present[alc] = true;
        bundle->ales[alc] = (info->opc >> 32) | src3;
    }

    if (bundle->latency < info->latency) {
        bundle->latency = info->latency;
    }
}

static void e2k_out_alf_lit(Bundle *bundle, int alc, AlcOpc opc, uint8_t dst,
                            uint8_t src1, int64_t src2, bool c_src2, uint8_t src3)
{
    if (c_src2) {
        e2k_out_lit(bundle, alc, src2);
    }
    e2k_out_alf(bundle, alc, opc, dst, src1, c_src2 ? 0 : GPR(src2), src3);
}

/* ALF1 dst:reg, src1:reg:uimm5, src2:reg:lit */
static void e2k_out_alf1_rrr(Bundle *bundle, int alc, AlcOpc opc, TCGReg dst,
                             TCGReg src1, TCGReg src2)
{
    e2k_out_alf(bundle, alc, opc, GPR(dst), GPR(src1), GPR(src2), 0);
}

static void e2k_out_alf1_rrc(Bundle *bundle, int alc, AlcOpc opc, TCGReg dst,
                             TCGReg src1, TCGArg src2, bool c_src2)
{
    e2k_out_alf_lit(bundle, alc, opc, GPR(dst), GPR(src1), src2, c_src2, 0);
}

static void e2k_out_alf1_rri(Bundle *bundle, int alc, AlcOpc opc, TCGReg dst,
                             TCGReg src1, int64_t src2)
{
    e2k_out_alf_lit(bundle, alc, opc, GPR(dst), GPR(src1), src2, true, 0);
}

/* ALF2 dst:reg, src2:reg:lit */
static void e2k_out_alf2_rr(Bundle *bundle, int alc, AlcOpc opc, TCGReg dst,
                            TCGReg src2)
{
    e2k_out_alf(bundle, alc, opc, GPR(dst), 0, GPR(src2), 0);
}

static void e2k_out_alf2_rc(Bundle *bundle, int alc, AlcOpc opc, TCGReg dst,
                            TCGArg src2, bool c_src2)
{
    e2k_out_alf_lit(bundle, alc, opc, GPR(dst), 0, src2, c_src2, 0);
}

static void e2k_out_alf2_ri(Bundle *bundle, int alc, AlcOpc opc, TCGReg dst,
                            int64_t src2)
{
    e2k_out_alf_lit(bundle, alc, opc, GPR(dst), 0, src2, true, 0);
}

/* ALF7 dst:pred_reg, src1:reg:uimm5, src2:reg:lit */
static void e2k_out_alf7_prr(Bundle *bundle, int alc, AlcOpc opc, PredReg dst,
                             TCGReg src1, TCGReg src2)
{
    e2k_out_alf(bundle, alc, opc, dst, GPR(src1), GPR(src2), 0);
}

static void e2k_out_alf7_prc(Bundle *bundle, int alc, AlcOpc opc, PredReg dst,
                             TCGReg src1, TCGArg src2, bool c_src2)
{
    e2k_out_alf_lit(bundle, alc, opc, dst, GPR(src1), src2, c_src2, 0);
}

static void e2k_out_alf7_pri(Bundle *bundle, int alc, AlcOpc opc, PredReg dst,
                             TCGReg src1, int64_t src2)
{
    e2k_out_alf_lit(bundle, alc, opc, dst, GPR(src1), src2, true, 0);
}

static uint32_t encode_cs0_imm28(int32_t offset)
{
    return (offset >> 3) & 0xfffffff;
}

static uint32_t encode_cs0_prep(CtprReg ctpr, int32_t offset)
{
    return ((uint32_t) ctpr << 30) | encode_cs0_imm28(offset);
}

static void e2k_out_cs(Bundle *bundle, int n, uint32_t cs)
{
    tcg_debug_assert(!bundle->cs_present[n]);

    bundle->cs_present[n] = true;
    bundle->cs[n] = cs;
}

static void e2k_out_prep_r(Bundle *bundle, int alc, CtprReg dst, TCGReg src2)
{
    e2k_out_alf(bundle, alc, ALC_MOVTD_CTPR, CTPR(dst), 0, GPR(src2), 0);
}

static void e2k_out_prep(Bundle *bundle, CtprReg dst, int32_t offset)
{
    e2k_set_latency(bundle, E2K_LATENCY_PREP);
    e2k_out_cs(bundle, 0, (uint32_t) dst << 30 | encode_cs0_imm28(offset));
}

static void e2k_out_prep_return(Bundle *bundle)
{
    e2k_set_latency(bundle, E2K_LATENCY_PREP_RETURN);
    e2k_out_cs(bundle, 0, 0xf0000000);
}

typedef enum {
    CT_NONE = 0,
    CT_ALWAYS = 1,
    CT_PREG = 2,                        /* pN */
    CT_NOT_PREG = 3,                    /* ~pN */
    CT_LOOP_END = 4,                    /* loop_end */
    CT_NOT_LOOP_END = 5,                /* ~loop_end */
    CT_PREG_OR_LOOP_END = 6,            /* pN || loop_end */
    CT_NOT_PREG_AND_NOT_LOOP_END = 7,   /* ~pN && ~loop_end */
    CT_MLOCK_OR_ALC = 8,                /* mlock || %dt_al0134  (SS[4:0] dt_al bitmask) */
    CT_MLOCK_OR_CMP = 9,                /* mlock || {~}alcN || {~}%alcN (SS[4:0] cmpN in ALC or clpN) */
    CT_CMP_CLP = 11,                    /* {~}{alp,plu}N v7+ */
    CT_NOT_PREG_OR_LOOP_END = 14,       /* ~pN || loop_end */
    CT_PREG_AND_NOT_LOOP_END = 15,      /* pN && ~loop_end */
} CtCond;

static void e2k_out_ct(Bundle *bundle, CtprReg ctpr, CtCond cond, uint8_t pred)
{
    bundle->ss_present = true;
    bundle->ss = deposit32(bundle->ss,  0, 5, pred);
    bundle->ss = deposit32(bundle->ss,  5, 3, cond);
    bundle->ss = deposit32(bundle->ss, 10, 2, ctpr);
}

static void e2k_out_ibranch(Bundle *bundle, int32_t offset, CtCond cond, uint8_t pred)
{
    e2k_out_ct(bundle, CTPR_NONE, cond, pred);
    e2k_out_cs(bundle, 0, encode_cs0_imm28(offset));
}

static void e2k_out_call(Bundle *bundle, CtprReg ctpr, CtCond cond, uint8_t pred)
{
    e2k_out_ct(bundle, ctpr, cond, pred);
    e2k_out_cs(bundle, 1, 0x50000000 | WBS);
}

static void e2k_out_mov(Bundle *bundle, int alc, TCGType type, TCGReg ret, TCGReg arg)
{
    if (ret == arg) {
        return;
    }

    switch (type) {
    case TCG_TYPE_I32:
        e2k_out_alf2_rr(bundle, alc, ALC_MOVW, ret, arg);
        break;
    case TCG_TYPE_I64:
        e2k_out_alf2_rr(bundle, alc, ALC_MOVD, ret, arg);
        break;
    default:
        g_assert_not_reached();
    }
}

static void e2k_out_movi(Bundle *bundle, int alc, TCGReg ret, int64_t val)
{
    e2k_out_alf2_ri(bundle, alc, ALC_MOVD, ret, val);
}

/*
 * E2K single instruction emitters
 */

static void tcg_out_nop(TCGContext *s)
{
    tcg_out32(s, 0);
    tcg_out32(s, 0);
}

#define E2K_CS0_WAIT 0x30000000

#define E2K_WAIT_mem_mod        0x2000  /* watch for modification */
#define E2K_WAIT_int            0x1000  /* stop the conveyor untill interrupt */
#define E2K_WAIT_mt             0x800
#define E2K_WAIT_lal            0x400   /* load-after-load modifier for _ld_c */
#define E2K_WAIT_las            0x200   /* load-after-store modifier for _st_c */
#define E2K_WAIT_sal            0x100   /* store-after-load modifier for _ld_c */
#define E2K_WAIT_sas            0x80    /* store-after-store modifier for _st_c */
#define E2K_WAIT_ma_c           0x20    /* stop until all memory operations complete */
#define E2K_WAIT_fl_c           0x10    /* stop until TLB/cache flush operations complete */
#define E2K_WAIT_ld_c           0x8     /* stop until all load operations complete */
#define E2K_WAIT_st_c           0x4     /* stop until all store operations complete */
#define E2K_WAIT_all_e          0x2     /* stop until prev. operations issue all exceptions */
#define E2K_WAIT_all_c          0x1     /* stop until prev. operations complete */

static void tcg_out_wait(TCGContext *s, uint32_t flags)
{
    Bundle bundle;
    e2k_out_start(&bundle);
    e2k_out_cs(&bundle, 1, E2K_CS0_WAIT | flags);
    e2k_out_end(s, &bundle);
}

static void tcg_out_alf(TCGContext *s, int alc, AlcOpc opc, uint8_t dst,
                        uint8_t src1, uint8_t src2, uint8_t src3)
{
    Bundle bundle;
    e2k_out_start(&bundle);
    e2k_out_alf(&bundle, alc, opc, dst, src1, src2, src3);
    e2k_out_end(s, &bundle);
}

static void tcg_out_alf_lit(TCGContext *s, int alc, AlcOpc opc, uint8_t dst,
                            uint8_t src1, int64_t src2, bool c_src2, uint8_t src3)
{
    Bundle bundle;
    e2k_out_start(&bundle);
    e2k_out_alf_lit(&bundle, alc, opc, dst, src1, src2, c_src2, src3);
    e2k_out_end(s, &bundle);
}

/* ALF1 dst:reg, src1:reg:uimm5, src2:reg:lit */
static void tcg_out_alf1_rrr(TCGContext *s, int alc, AlcOpc opc,
                             TCGReg dst, TCGReg src1, TCGReg src2)
{
    tcg_out_alf(s, alc, opc, GPR(dst), GPR(src1), GPR(src2), 0);
}

static void tcg_out_alf1_rrc(TCGContext *s, int alc, AlcOpc opc,
                             TCGReg dst, TCGReg src1, TCGArg src2, bool c_src2)
{
    tcg_out_alf_lit(s, alc, opc, GPR(dst), GPR(src1), src2, c_src2, 0);
}

static void tcg_out_alf1_rir(TCGContext *s, int alc, AlcOpc opc,
                             TCGReg dst, uint8_t src1, TCGReg src2)
{
    tcg_out_alf(s, alc, opc, GPR(dst), UIMM5(src1), GPR(src2), 0);
}

static void tcg_out_alf1_rii(TCGContext *s, int alc, AlcOpc opc,
                             TCGReg dst, uint8_t src1, int64_t src2)
{
    tcg_out_alf_lit(s, alc, opc, GPR(dst), UIMM5(src1), src2, true, 0);
}

static void tcg_out_alf1_rri(TCGContext *s, int alc, AlcOpc opc,
                             TCGReg dst, TCGReg src1, TCGArg src2)
{
    tcg_out_alf_lit(s, alc, opc, GPR(dst), GPR(src1), src2, true, 0);
}

/* ALF2 dst:reg, src2:reg:lit */
static void tcg_out_alf2_rr(TCGContext *s, int alc, AlcOpc opc,
                            TCGReg dst, TCGReg src2)
{
    tcg_out_alf(s, alc, opc, GPR(dst), 0, GPR(src2), 0);
}

static void tcg_out_alf2_ri(TCGContext *s, int alc, AlcOpc opc,
                            TCGReg dst, int64_t src2)
{
    tcg_out_alf_lit(s, alc, opc, GPR(dst), 0, src2, true, 0);
}

static void tcg_out_alf2_rc(TCGContext *s, int alc, AlcOpc opc,
                            TCGReg dst, int64_t src2, bool c_src2)
{
    tcg_out_alf_lit(s, alc, opc, GPR(dst), 0, src2, c_src2, 0);
}

/* ALF21 dst:reg, src1:reg:uimm5, src2:reg:lit, src2:reg */
static void tcg_out_alf21_rrrr(TCGContext *s, int alc, AlcOpc opc, TCGReg dst,
                               TCGReg src1, TCGArg src2, TCGReg src3)
{
    tcg_out_alf(s, alc, opc, GPR(dst), GPR(src1), GPR(src2), GPR(src3));
}

static void tcg_out_alf21_rrir(TCGContext *s, int alc, AlcOpc opc, TCGReg dst,
                               TCGReg src1, TCGArg src2, TCGReg src3)
{
    tcg_out_alf_lit(s, alc, opc, GPR(dst), GPR(src1), src2, true, GPR(src3));
}

static void tcg_out_prep_i(TCGContext *s, CtprReg dst, int64_t src2)
{
    tcg_out_alf_lit(s, 0, ALC_MOVTD_CTPR, CTPR(dst), 0, src2, true, 0);
}

static void tcg_out_prep_r(TCGContext *s, CtprReg dst, TCGReg src2)
{
    tcg_out_alf(s, 0, ALC_MOVTD_CTPR, CTPR(dst), 0, GPR(src2), 0);
}

static void tcg_out_prep(TCGContext *s, CtprReg dst, int32_t offset)
{
    Bundle bundle;
    e2k_out_start(&bundle);
    e2k_out_prep(&bundle, dst, offset);
    e2k_out_end(s, &bundle);
}

static void tcg_out_ct(TCGContext *s, CtprReg ctpr, CtCond cond, PredReg preg)
{
    Bundle bundle;
    e2k_out_start(&bundle);
    e2k_out_ct(&bundle, ctpr, cond, preg);
    e2k_out_end(s, &bundle);
}

/*
 * Relocations
 */

static bool reloc_prep_ctpr(tcg_insn_unit *src_rw, const tcg_insn_unit *target, intptr_t addend)
{
    const tcg_insn_unit *src_rx = tcg_splitwx_to_rx(src_rw);
    intptr_t offset = (intptr_t)target - (intptr_t)src_rx + addend * 4;

    tcg_debug_assert((offset & 7) == 0);
    if (offset == sextract32(offset, 0, 31)) {
        *src_rw |= encode_cs0_imm28(offset);
        return true;
    }

    return false;
}


static bool patch_reloc(tcg_insn_unit *code_ptr, int type,
                        intptr_t value, intptr_t addend)
{
    switch (type) {
    case R_E2K_DISP:
        return reloc_prep_ctpr(code_ptr, (tcg_insn_unit *) value, addend);
    default:
        g_assert_not_reached();
    }
}

static void tcg_out_reloc_prep(TCGContext *s, const Bundle *bundle,
                               TCGLabel *l)
{
    tcg_debug_assert(bundle->cs_present[0]);
    tcg_out_reloc(s, s->code_ptr - bundle->len + bundle->cs_offset[0],
                  R_E2K_DISP, l, bundle->cs_offset[0]);
}

/*
 * TCG intrinsics
 */

static bool tcg_out_mov(TCGContext *s, TCGType type, TCGReg ret, TCGReg arg)
{
    Bundle bundle;

    if (ret == arg) {
        return true;
    }

    e2k_out_start(&bundle);
    e2k_out_mov(&bundle, 0, type, ret, arg);
    e2k_out_end(s, &bundle);

    return true;
}

static void tcg_out_movi(TCGContext *s, TCGType type, TCGReg rd,
                         tcg_target_long val)
{
    Bundle bundle;
    e2k_out_start(&bundle);
    e2k_out_movi(&bundle, 0, rd, val);
    e2k_out_end(s, &bundle);
}

static bool tcg_out_xchg(TCGContext *s, TCGType type, TCGReg r1, TCGReg r2)
{
    return false;
}

static void tcg_out_addi_ptr(TCGContext *s, TCGReg rd, TCGReg rs,
                             tcg_target_long imm)
{
    /* This function is only used for passing structs by reference. */
    g_assert_not_reached();
}

static void tcg_out_ext8s(TCGContext *s, TCGType type, TCGReg ret, TCGReg arg)
{
    tcg_out_alf2_rr(s, 0, ALC_SXTB, ret, arg);
}

static void tcg_out_ext16s(TCGContext *s, TCGType type, TCGReg ret, TCGReg arg)
{
    tcg_out_alf2_rr(s, 0, ALC_SXTH, ret, arg);
}

static void tcg_out_ext32s(TCGContext *s, TCGReg ret, TCGReg arg)
{
    tcg_out_alf2_rr(s, 0, ALC_SXTW, ret, arg);
}

static void tcg_out_ext8u(TCGContext *s, TCGReg ret, TCGReg arg)
{
    tcg_out_alf2_rr(s, 0, ALC_ZXTB, ret, arg);
}

static void tcg_out_ext16u(TCGContext *s, TCGReg ret, TCGReg arg)
{
    tcg_out_alf2_rr(s, 0, ALC_ZXTH, ret, arg);
}

static void tcg_out_ext32u(TCGContext *s, TCGReg ret, TCGReg arg)
{
    tcg_out_alf2_rr(s, 0, ALC_ZXTW, ret, arg);
}

static void tcg_out_exts_i32_i64(TCGContext *s, TCGReg ret, TCGReg arg)
{
    tcg_out_ext32s(s, ret, arg);
}

static void tcg_out_extu_i32_i64(TCGContext *s, TCGReg ret, TCGReg arg)
{
    tcg_out_ext32u(s, ret, arg);
}

static void tcg_out_extrl_i64_i32(TCGContext *s, TCGReg ret, TCGReg arg)
{
    tcg_out_ext32s(s, ret, arg);
}

static const struct {
    AlcOpc opc;
    bool invert;
} tcg_cond_to_e2k_i32[] = {
    [TCG_COND_EQ]  = { ALC_CMPEWB,  false },
    [TCG_COND_NE]  = { ALC_CMPEWB,  true  },
    [TCG_COND_LT]  = { ALC_CMPLWB,  false },
    [TCG_COND_GE]  = { ALC_CMPLWB,  true  },
    [TCG_COND_LE]  = { ALC_CMPLEWB, false },
    [TCG_COND_GT]  = { ALC_CMPLEWB, true  },
    [TCG_COND_LTU] = { ALC_CMPBWB,  false },
    [TCG_COND_GEU] = { ALC_CMPBWB,  true  },
    [TCG_COND_LEU] = { ALC_CMPBEWB, false },
    [TCG_COND_GTU] = { ALC_CMPBEWB, true  },
};

static const struct {
    AlcOpc opc;
    bool invert;
} tcg_cond_to_e2k_i64[] = {
    [TCG_COND_EQ]  = { ALC_CMPEDB,  false },
    [TCG_COND_NE]  = { ALC_CMPEDB,  true  },
    [TCG_COND_LT]  = { ALC_CMPLDB,  false },
    [TCG_COND_GE]  = { ALC_CMPLDB,  true  },
    [TCG_COND_LE]  = { ALC_CMPLEDB, false },
    [TCG_COND_GT]  = { ALC_CMPLEDB, true  },
    [TCG_COND_LTU] = { ALC_CMPBDB,  false },
    [TCG_COND_GEU] = { ALC_CMPBDB,  true  },
    [TCG_COND_LEU] = { ALC_CMPBEDB, false },
    [TCG_COND_GTU] = { ALC_CMPBEDB, true  },
};

static bool e2k_out_setcond(Bundle *bundle, int alc, TCGType type, TCGCond cond,
                            PredReg ret, TCGReg src1, int64_t src2, bool c_src2)
{
    AlcOpc cmp;
    bool invert;

    if (type == TCG_TYPE_I64) {
        cmp = tcg_cond_to_e2k_i64[cond].opc;
        invert = tcg_cond_to_e2k_i64[cond].invert;
    } else {
        cmp = tcg_cond_to_e2k_i32[cond].opc;
        invert = tcg_cond_to_e2k_i32[cond].invert;
    }

    tcg_debug_assert(cmp);

    e2k_out_alf7_prc(bundle, alc, cmp, ret, src1, src2, c_src2);

    return invert;
}

static void tcg_out_brcond(TCGContext *s, TCGType type, TCGCond cond, TCGReg src1,
                           TCGArg src2, bool c_src2, TCGLabel *l)
{
    Bundle bundle;
    bool invert;

    e2k_out_start(&bundle);
    invert = e2k_out_setcond(&bundle, 0, type, cond, PREG_TMP0, src1, src2, c_src2);
    e2k_out_prep(&bundle, CTPR1, 0);
    e2k_out_end(s, &bundle);
    tcg_out_reloc_prep(s, &bundle, l);

    tcg_out_ct(s, CTPR1, invert ? CT_NOT_PREG : CT_PREG, PREG_TMP0);
}

static void tcg_out_setcond_int(TCGContext *s, TCGType type, TCGCond cond, TCGReg ret,
                                TCGReg src1, TCGArg src2, bool c_src2, bool neg)
{
    Bundle bundle;
    bool invert;
    uint8_t result = UIMM4(1);

    e2k_out_start(&bundle);
    invert = e2k_out_setcond(&bundle, 0, type, cond, PREG_TMP0, src1, src2, c_src2);
    e2k_out_end(s, &bundle);

    e2k_out_start(&bundle);
    if (neg) {
        e2k_out_lit(&bundle, 0, -1);
        result = 0;
    }
    e2k_out_alf(&bundle, 0, ALC_MERGED, GPR(ret), UIMM5(0), result, 0);
    e2k_out_mrgc(&bundle, 0, PREG_TMP0, invert);
    e2k_out_end(s, &bundle);
}

static void tcg_out_setcond(TCGContext *s, TCGType type, TCGCond cond, TCGReg ret,
                            TCGReg src1, TCGArg src2, bool c_src2)
{
    tcg_out_setcond_int(s, type, cond, ret, src1, src2, c_src2, false);
}

static void tcg_out_negsetcond(TCGContext *s, TCGType type, TCGCond cond, TCGReg ret,
                            TCGReg src1, TCGArg src2, bool c_src2)
{
    tcg_out_setcond_int(s, type, cond, ret, src1, src2, c_src2, true);
}

static void tcg_out_movcond(TCGContext *s, TCGType type, TCGCond cond, TCGReg ret,
                            TCGReg cmp1, TCGArg cmp2, bool c_cmp2,
                            TCGArg val1, bool c_val1,
                            TCGArg val2, bool c_val2)
{
    Bundle bundle;
    bool inv;
    AlcOpc mov = type == TCG_TYPE_I32 ? ALC_MOVW : ALC_MOVD;

    e2k_out_start(&bundle);
    inv = e2k_out_setcond(&bundle, 0, type, cond, PREG_TMP0, cmp1, cmp2, c_cmp2);
    e2k_out_end(s, &bundle);

    e2k_out_start(&bundle);
    e2k_out_alf2_rc(&bundle, 0, mov, ret, val1, c_val1);
    e2k_out_alf2_rc(&bundle, 1, mov, ret, val2, c_val2);
    e2k_out_rlp(&bundle, 0, PREG_TMP0,  inv);
    e2k_out_rlp(&bundle, 1, PREG_TMP0, !inv);
    e2k_out_end(s, &bundle);
}

static void tcg_out_clz_int(TCGContext *s, TCGType type, TCGReg ret,
                            TCGReg a0, TCGArg a1, bool c1, bool do_bitrev)
{
    Bundle bundle;
    AlcOpc cmp, mov, clz, bitrev;

    if (type == TCG_TYPE_I64) {
        cmp = ALC_CMPEDB;
        mov = ALC_MOVD;
        clz = ALC_LZCNTD;
        bitrev = ALC_BITREVD;
    } else {
        cmp = ALC_CMPEWB;
        mov = ALC_MOVW;
        clz = ALC_LZCNTW;
        bitrev = ALC_BITREVW;
    }

    e2k_out_start(&bundle);
    e2k_out_alf7_pri(&bundle, 0, cmp, PREG_TMP0, a0, 0);
    if (do_bitrev) {
        e2k_out_alf2_rr(&bundle, 1, bitrev, TCG_REG_TMP0, a0);
        a0 = TCG_REG_TMP0;
    }
    e2k_out_end(s, &bundle);

    e2k_out_start(&bundle);
    e2k_out_alf2_rc(&bundle, 0, mov, ret, a1, c1);
    e2k_out_alf2_rr(&bundle, 1, clz, ret, a0);
    e2k_out_rlp(&bundle, 0, PREG_TMP0, false);
    e2k_out_rlp(&bundle, 1, PREG_TMP0, true);
    e2k_out_end(s, &bundle);
}

static uint16_t e2k_field_sel(int ofs, int len, bool sign)
{
    int ret = (len << 6) | ofs;

    tcg_debug_assert(len < 64);
    tcg_debug_assert(ofs < 64);

    if (len != 0 && sign) {
        ret |= ((ofs + len - 1) / 8) << 13 | (1u << 12);
    }

    return ret;
}

static void tcg_out_extract_int(TCGContext *s, TCGType type, TCGReg ret,
                                TCGReg arg, int ofs, int len, bool sign)
{
    AlcOpc opc = type == TCG_TYPE_I64 ? ALC_GETFD : ALC_GETFW;
    tcg_out_alf1_rri(s, 0, opc, ret, arg, e2k_field_sel(ofs, len, sign));
}

static void tcg_out_deposit_int(TCGContext *s, TCGType type, TCGReg ret,
                                TCGReg a1, TCGReg a2, int ofs, int len)
{
    AlcOpc opc1 = type == TCG_TYPE_I64 ? ALC_INSFD : ALC_INSFW;
    AlcOpc opc2 = type == TCG_TYPE_I64 ? ALC_SCLD : ALC_SCLW;

    tcg_out_alf21_rrir(s, 0, opc1, ret, a1, e2k_field_sel(ofs, len, false), a2);
    tcg_out_alf1_rri(s, 0, opc2, ret, ret, ofs);
}

static void e2k_out_ld(Bundle *bundle, int alc, TCGType type, TCGReg ret,
                       TCGReg addr, intptr_t offset)
{
    AlcOpc opc = type == TCG_TYPE_I32 ? ALC_LWU : ALC_LDU;
    e2k_out_alf1_rri(bundle, alc, opc, ret, addr, offset);
}

static void e2k_out_st(Bundle *bundle, int alc, TCGType type, TCGReg data,
                       TCGReg addr, intptr_t offset)
{
    AlcOpc opc = type == TCG_TYPE_I32 ? ALC_SW : ALC_SD;
    e2k_out_alf1_rri(bundle, alc, opc, data, addr, offset);
}

static void tcg_out_ld(TCGContext *s, TCGType type, TCGReg ret,
                       TCGReg addr, intptr_t offset)
{
    Bundle bundle;

    e2k_out_start(&bundle);
    e2k_out_ld(&bundle, 0, type, ret, addr, offset);
    e2k_out_end(s, &bundle);

    if (type == TCG_TYPE_I32) {
        tcg_out_ext32s(s, ret, ret);
    }
}

static void tcg_out_st(TCGContext *s, TCGType type, TCGReg data,
                       TCGReg addr, intptr_t offset)
{
    Bundle bundle;
    e2k_out_start(&bundle);
    e2k_out_st(&bundle, 2, type, data, addr, offset);
    e2k_out_end(s, &bundle);
}

static bool tcg_out_sti(TCGContext *s, TCGType type, TCGArg val,
                        TCGReg base, intptr_t ofs)
{
    return false;
}

static void tcg_out_call_int(TCGContext *s, const tcg_insn_unit *arg, bool tail)
{
    Bundle bundle;
    ptrdiff_t offset = tcg_pcrel_diff(s, arg);

    tcg_debug_assert((offset & 7) == 0);

    if (offset == (int32_t) offset) {
        /* short jump +/-1GiB */
        tcg_out_prep(s, CTPR1, offset);
    } else {
        /* far jump: 64-bit */
        tcg_out_prep_i(s, CTPR1, (int64_t) arg);
    }

    e2k_out_start(&bundle);
    if (tail) {
        e2k_out_ct(&bundle, CTPR1, CT_ALWAYS, 0);
    } else {
        e2k_out_call(&bundle, CTPR1, CT_ALWAYS, 0);
    }
    e2k_out_end(s, &bundle);

    /* Always keep ctpr3 with a return address */
    if (!tail) {
        e2k_out_start(&bundle);
        e2k_out_prep_return(&bundle);
        bundle.latency = 1;
        e2k_out_end(s, &bundle);
    }
}

static void tcg_out_call(TCGContext *s, const tcg_insn_unit *arg,
                         const TCGHelperInfo *info)
{
    tcg_out_call_int(s, arg, false);
}

static void tcg_out_mb(TCGContext *s, TCGArg a0)
{
    uint32_t wait = 0, soft = 0;

    // TODO: CPU_HWBUG_SOFT_WAIT_E8C2

    if (a0 & TCG_MO_LD_LD) {
        wait |= E2K_WAIT_ld_c;
        soft |= E2K_WAIT_lal;
    }
    if (a0 & TCG_MO_ST_LD) {
        wait |= E2K_WAIT_ld_c;
        soft |= E2K_WAIT_sal;
    }
    if (a0 & TCG_MO_LD_ST) {
        wait |= E2K_WAIT_st_c;
        soft |= E2K_WAIT_las;
    }
    if (a0 & TCG_MO_ST_ST) {
        wait |= E2K_WAIT_st_c;
        soft |= E2K_WAIT_sas;
    }

    tcg_out_nop(s);
    tcg_out_wait(s, wait);
    tcg_out_wait(s, wait | soft);
    tcg_out_nop(s);
}

/*
 * Load/store and TLB
 */

static void tcg_out_goto(TCGContext *s, const tcg_insn_unit *target)
{
    Bundle bundle;

    e2k_out_start(&bundle);
    e2k_out_prep(&bundle, CTPR1, 0);
    e2k_out_end(s, &bundle);
    bool ok = reloc_prep_ctpr(s->code_ptr - bundle.len + bundle.cs_offset[0],
                              target, bundle.cs_offset[0]);
    tcg_debug_assert(ok);

    tcg_out_ct(s, CTPR1, CT_ALWAYS, 0);
}

bool tcg_target_has_memory_bswap(MemOp memop)
{
    return false;
}

/* We have three temps, we might as well expose them. */
static const TCGLdstHelperParam ldst_helper_param = {
    .ntmp = 4, .tmp = { TCG_REG_TMP0, TCG_REG_TMP1, TCG_REG_TMP2, TCG_REG_TMP3 }
};

static bool tcg_out_qemu_ld_slow_path(TCGContext *s, TCGLabelQemuLdst *l)
{
    MemOp opc = get_memop(l->oi);

    /* resolve label address */
    if (!reloc_prep_ctpr(l->label_ptr[0], tcg_splitwx_to_rx(s->code_ptr),
                         l->label_addend[0])) {
        return false;
    }

    /* call load helper */
    tcg_out_ld_helper_args(s, l, &ldst_helper_param);
    tcg_out_call_int(s, qemu_ld_helpers[opc & MO_SSIZE], false);
    tcg_out_ld_helper_ret(s, l, true, &ldst_helper_param);

    tcg_out_goto(s, l->raddr);
    return true;
}

static bool tcg_out_qemu_st_slow_path(TCGContext *s, TCGLabelQemuLdst *l)
{
    MemOp opc = get_memop(l->oi);

    /* resolve label address */
    if (!reloc_prep_ctpr(l->label_ptr[0], tcg_splitwx_to_rx(s->code_ptr),
                         l->label_addend[0])) {
        return false;
    }

    /* call store helper */
    tcg_out_st_helper_args(s, l, &ldst_helper_param);
    tcg_out_call_int(s, qemu_st_helpers[opc & MO_SIZE], false);

    tcg_out_goto(s, l->raddr);
    return true;
}

#define MIN_TLB_MASK_TABLE_OFS  INT_MIN

/*
 * For softmmu, perform the TLB load and compare.
 * For useronly, perform any required alignment tests.
 * In both cases, return a TCGLabelQemuLdst structure if the slow path
 * is required and fill in @h with the host address for the fast path.
 */
static TCGLabelQemuLdst *prepare_host_addr(TCGContext *s, TCGReg *pbase,
                                           TCGReg addr_reg, MemOpIdx oi,
                                           bool is_ld)
{
    Bundle bundle;
    TCGType addr_type = s->addr_type;
    TCGLabelQemuLdst *ldst = NULL;
    MemOp opc = get_memop(oi);
    TCGAtomAlign aa;
    unsigned a_mask;

    aa = atom_and_align_for_opc(s, opc, MO_ATOM_IFALIGN, false);
    a_mask = (1u << aa.align) - 1;

#ifdef CONFIG_SOFTMMU
    unsigned s_bits = opc & MO_SIZE;
    unsigned s_mask = (1u << s_bits) - 1;
    int mem_index = get_mmuidx(oi);
    int fast_ofs = tlb_mask_table_ofs(s, mem_index);
    int mask_ofs = fast_ofs + offsetof(CPUTLBDescFast, mask);
    int table_ofs = fast_ofs + offsetof(CPUTLBDescFast, table);
    int compare_mask;
    TCGReg addr_adj;

    ldst = new_ldst_label(s);
    ldst->is_ld = is_ld;
    ldst->oi = oi;
    ldst->addrlo_reg = addr_reg;

    e2k_out_start(&bundle);
    e2k_out_ld(&bundle, 0, TCG_TYPE_PTR, TCG_REG_TMP0, TCG_AREG0, mask_ofs);
    e2k_out_ld(&bundle, 2, TCG_TYPE_PTR, TCG_REG_TMP1, TCG_AREG0, table_ofs);
    if (addr_type == TCG_TYPE_I32) {
        e2k_out_alf2_rr(&bundle, 1, ALC_ZXTW, TCG_REG_TMP3, addr_reg);
        addr_reg = TCG_REG_TMP3;
    }
    e2k_out_end(s, &bundle);

    tcg_out_alf1_rri(s, 0, ALC_SHRD, TCG_REG_TMP2, addr_reg,
                     s->page_bits - CPU_TLB_ENTRY_BITS);
    tcg_out_alf1_rrr(s, 0, ALC_ANDD, TCG_REG_TMP2, TCG_REG_TMP2, TCG_REG_TMP0);

    e2k_out_start(&bundle);
    e2k_out_alf1_rrr(&bundle, 0, ALC_ADDD, TCG_REG_TMP2, TCG_REG_TMP2, TCG_REG_TMP1);
    /*
     * For aligned accesses, we check the first byte and include the alignment
     * bits within the address.  For unaligned access, we check that we don't
     * cross pages using the address of the last byte of the access.
     */
    addr_adj = addr_reg;
    if (a_mask < s_mask) {
        addr_adj = TCG_REG_TMP0;
        e2k_out_alf1_rri(&bundle, 1, ALC_ADDD, addr_adj, addr_reg, s_mask - a_mask);
    }
    e2k_out_end(s, &bundle);

    e2k_out_start(&bundle);
    compare_mask = s->page_mask | a_mask;
    e2k_out_alf1_rri(&bundle, 1, ALC_ANDD, TCG_REG_TMP1, addr_adj, compare_mask);

    /* Load the tlb comparator and the addend.  */
    e2k_out_ld(&bundle, 0, addr_type, TCG_REG_TMP0, TCG_REG_TMP2,
               is_ld ? offsetof(CPUTLBEntry, addr_read)
                     : offsetof(CPUTLBEntry, addr_write));
    e2k_out_ld(&bundle, 2, TCG_TYPE_PTR, TCG_REG_TMP2, TCG_REG_TMP2,
               offsetof(CPUTLBEntry, addend));
    e2k_out_end(s, &bundle);

    /* Compare masked address with the TLB entry. */
    e2k_out_start(&bundle);
    e2k_out_prep(&bundle, CTPR1, 0);
    e2k_out_alf7_prr(&bundle, 0, ALC_CMPEDB, PREG_TMP0, TCG_REG_TMP0, TCG_REG_TMP1);
    e2k_out_end(s, &bundle);
    ldst->label_ptr[0] = s->code_ptr - bundle.len + bundle.cs_offset[0];
    ldst->label_addend[0] = bundle.cs_offset[0];

    /* TLB Hit - translate address using addend.  */
    if (addr_type != TCG_TYPE_I32) {
        e2k_out_start(&bundle);
        e2k_out_alf1_rrr(&bundle, 0, ALC_ADDD, TCG_REG_TMP0, addr_reg, TCG_REG_TMP2);
        e2k_out_ct(&bundle, CTPR1, CT_NOT_PREG, PREG_TMP0);
        e2k_out_end(s, &bundle);
    } else {
        e2k_out_start(&bundle);
        e2k_out_ct(&bundle, CTPR1, CT_NOT_PREG, PREG_TMP0);
        e2k_out_alf2_rr(&bundle, 0, ALC_ZXTW, TCG_REG_TMP0, addr_reg);
        e2k_out_end(s, &bundle);

        tcg_out_alf1_rrr(s, 0, ALC_ADDD, TCG_REG_TMP0, TCG_REG_TMP0, TCG_REG_TMP2);
    }
    *pbase = TCG_REG_TMP0;
#else
    TCGReg base;

    if (a_mask) {
        ldst = new_ldst_label(s);
        ldst->is_ld = is_ld;
        ldst->oi = oi;
        ldst->addrlo_reg = addr_reg;

        e2k_out_start(&bundle);
        e2k_out_prep(&bundle, CTPR1, 0);
        e2k_out_alf7_pri(&bundle, 0, ALC_CMPANDEDB, PREG_TMP0, addr_reg, a_mask);
        e2k_out_end(s, &bundle);
        ldst->label_ptr[0] = s->code_ptr - bundle.len + bundle.cs_offset[0];
        ldst->label_addend[0] = bundle.cs_offset[0];

        tcg_out_ct(s, CTPR1, CT_NOT_PREG, PREG_TMP0);
    }

    if (guest_base != 0) {
        base = TCG_REG_TMP0;
        if (addr_type != TCG_TYPE_I32) {
            tcg_out_alf1_rrr(s, 0, ALC_ADDD, base, addr_reg, TCG_GUEST_BASE_REG);
        } else {
            tcg_out_ext32u(s, base, addr_reg);
            tcg_out_alf1_rrr(s, 0, ALC_ADDD, base, base, TCG_GUEST_BASE_REG);
        }
    } else if (addr_type != TCG_TYPE_I32) {
        base = addr_reg;
    } else {
        base = TCG_REG_TMP0;
        tcg_out_ext32u(s, base, addr_reg);
    }
    *pbase = base;
#endif

    return ldst;
}

static void tcg_out_qemu_ld_direct(TCGContext *s, TCGReg ret,
                                   TCGReg base, MemOp opc, TCGType type)
{
    AlcOpc ldopc;

    /* Byte swapping is left to middle-end expansion. */
    tcg_debug_assert((opc & MO_BSWAP) == 0);

    switch (opc & MO_SIZE) {
    case MO_8:  ldopc = ALC_LBU; break;
    case MO_16: ldopc = ALC_LHU; break;
    case MO_32: ldopc = ALC_LWU; break;
    case MO_64: ldopc = ALC_LDU; break;
    default:
        g_assert_not_reached();
    }

    tcg_out_alf(s, 0, ldopc, GPR(ret), GPR(base), UIMM4(0), 0);

    if (opc & MO_SIGN) {
        switch (opc & MO_SIZE) {
        case MO_8:
            tcg_out_ext8s(s, type, ret, ret);
            break;
        case MO_16:
            tcg_out_ext16s(s, type, ret, ret);
            break;
        case MO_32:
            tcg_out_ext32s(s, ret, ret);
            break;
        case MO_64:
            break;
        default:
            g_assert_not_reached();
        }
    }
}

static void tcg_out_qemu_ld(TCGContext *s, TCGReg data_reg, TCGReg addr_reg,
                            MemOpIdx oi, TCGType data_type)
{
    TCGLabelQemuLdst *ldst;
    TCGReg base;

    ldst = prepare_host_addr(s, &base, addr_reg, oi, true);
    tcg_out_qemu_ld_direct(s, data_reg, base, get_memop(oi), data_type);

    if (ldst) {
        ldst->type = data_type;
        ldst->datalo_reg = data_reg;
        ldst->raddr = tcg_splitwx_to_rx(s->code_ptr);
    }
}

static void tcg_out_qemu_st_direct(TCGContext *s, TCGReg val,
                                   TCGReg base, MemOp opc)
{
    AlcOpc stopc;

    /* Byte swapping is left to middle-end expansion. */
    tcg_debug_assert((opc & MO_BSWAP) == 0);

    switch (opc & MO_SIZE) {
    case MO_8:  stopc = ALC_SB; break;
    case MO_16: stopc = ALC_SH; break;
    case MO_32: stopc = ALC_SW; break;
    case MO_64: stopc = ALC_SD; break;
    default:
        g_assert_not_reached();
    }

    tcg_out_alf(s, 2, stopc, GPR(val), GPR(base), UIMM4(0), 0);
}

static void tcg_out_qemu_st(TCGContext *s, TCGReg data_reg, TCGReg addr_reg,
                            MemOpIdx oi, TCGType data_type)
{
    TCGLabelQemuLdst *ldst;
    TCGReg base;

    ldst = prepare_host_addr(s, &base, addr_reg, oi, false);
    tcg_out_qemu_st_direct(s, data_reg, base, get_memop(oi));

    if (ldst) {
        ldst->type = data_type;
        ldst->datalo_reg = data_reg;
        ldst->raddr = tcg_splitwx_to_rx(s->code_ptr);
    }
}

static void tcg_out_exit_tb(TCGContext *s, uintptr_t a0)
{
    Bundle bundle;
    e2k_out_start(&bundle);
    e2k_out_movi(&bundle, TCG_TYPE_REG, TCG_REG_R0, a0);
    e2k_out_ct(&bundle, CTPR3, CT_ALWAYS, 0);
    e2k_out_end(s, &bundle);
}

static void tcg_out_goto_tb(TCGContext *s, int which)
{
    /* Direct branch will be patched by tb_target_set_jmp_target. */
    set_jmp_insn_offset(s, which);
    tcg_out_prep(s, CTPR1, 0);
    tcg_out_ct(s, CTPR1, CT_ALWAYS, 0);

    /* When branch is out of range, fall through to indirect. */
    AlcOpc opc = TCG_TYPE_PTR == TCG_TYPE_I32 ? ALC_LWU : ALC_LDU;
    tcg_out_alf1_rii(s, 0, opc, TCG_REG_TMP0, 0, get_jmp_target_addr(s, which));
    tcg_out_prep_r(s, CTPR1, TCG_REG_TMP0);
    tcg_out_ct(s, CTPR1, CT_ALWAYS, 0);
    set_jmp_reset_offset(s, which);
}

void tb_target_set_jmp_target(const TranslationBlock *tb, int n,
                              uintptr_t jmp_rx, uintptr_t jmp_rw)
{
    uintptr_t addr = tb->jmp_target_addr[n];
    ptrdiff_t offset = addr - jmp_rx;

    /* Either directly branch, or fall through to indirect branch. */
    if (offset == sextract32(offset, 0, 31)) {
        uint32_t cs0 = encode_cs0_prep(CTPR1, offset);
        qatomic_set((uint32_t *)jmp_rw + 1, cs0);   /* prep ctpr1 */
    } else {
        qatomic_set((uint32_t *)jmp_rw + 2, 0);     /* ct ctpr1 */
        qatomic_set((uint32_t *)jmp_rw    , 0);     /* prep ctpr1 */
    }
    flush_idcache_range(jmp_rx, jmp_rw, 16);
}

static void tcg_out_op(TCGContext *s, TCGOpcode opc,
                       const TCGArg args[TCG_MAX_OP_ARGS],
                       const int const_args[TCG_MAX_OP_ARGS])
{
    Bundle bundle;
    TCGArg a0 = args[0];
    TCGArg a1 = args[1];
    TCGArg a2 = args[2];
    TCGArg a3 = args[3];
    int c1 = const_args[1];
    int c2 = const_args[2];

    switch (opc) {
    case INDEX_op_goto_ptr:
        tcg_out_prep_r(s, CTPR1, a0);
        tcg_out_ct(s, CTPR1, CT_ALWAYS, 0);
        break;

    case INDEX_op_br:
        e2k_out_start(&bundle);
        e2k_out_prep(&bundle, CTPR1, 0);
        e2k_out_end(s, &bundle);
        tcg_out_reloc_prep(s, &bundle, arg_label(a0));

        tcg_out_ct(s, CTPR1, CT_ALWAYS, 0);
        break;

    case INDEX_op_ld8u_i32:
    case INDEX_op_ld8u_i64:
        tcg_out_alf1_rri(s, 0, ALC_LBU, a0, a1, a2);
        break;
    case INDEX_op_ld16u_i32:
    case INDEX_op_ld16u_i64:
        tcg_out_alf1_rri(s, 0, ALC_LHU, a0, a1, a2);
        break;
    case INDEX_op_ld32u_i64:
        tcg_out_alf1_rri(s, 0, ALC_LWU, a0, a1, a2);
        break;
    case INDEX_op_ld_i64:
        tcg_out_alf1_rri(s, 0, ALC_LDU, a0, a1, a2);
        break;
    case INDEX_op_ld8s_i32:
    case INDEX_op_ld8s_i64:
        tcg_out_alf1_rri(s, 0, ALC_LBU, a0, a1, a2);
        tcg_out_alf2_rr(s, 0, ALC_SXTB, a0, a0);
        break;
    case INDEX_op_ld16s_i32:
    case INDEX_op_ld16s_i64:
        tcg_out_alf1_rri(s, 0, ALC_LHU, a0, a1, a2);
        tcg_out_alf2_rr(s, 0, ALC_SXTH, a0, a0);
        break;
    case INDEX_op_ld_i32:
        tcg_out_alf1_rri(s, 0, ALC_LWU, a0, a1, a2);
        break;
    case INDEX_op_ld32s_i64:
        tcg_out_alf1_rri(s, 0, ALC_LWU, a0, a1, a2);
        tcg_out_alf2_rr(s, 0, ALC_SXTW, a0, a0);
        break;

    case INDEX_op_st8_i32:
    case INDEX_op_st8_i64:
        tcg_out_alf1_rri(s, 2, ALC_SB, a0, a1, a2);
        break;
    case INDEX_op_st16_i32:
    case INDEX_op_st16_i64:
        tcg_out_alf1_rri(s, 2, ALC_SH, a0, a1, a2);
        break;
    case INDEX_op_st_i32:
    case INDEX_op_st32_i64:
        tcg_out_alf1_rri(s, 2, ALC_SW, a0, a1, a2);
        break;
    case INDEX_op_st_i64:
        tcg_out_alf1_rri(s, 2, ALC_SD, a0, a1, a2);
        break;

    case INDEX_op_and_i32:
        tcg_out_alf1_rrc(s, 0, ALC_ANDW, a0, a1, a2, c2);
        break;
    case INDEX_op_and_i64:
        tcg_out_alf1_rrc(s, 0, ALC_ANDD, a0, a1, a2, c2);
        break;

    case INDEX_op_or_i32:
        tcg_out_alf1_rrc(s, 0, ALC_ORW, a0, a1, a2, c2);
        break;
    case INDEX_op_or_i64:
        tcg_out_alf1_rrc(s, 0, ALC_ORD, a0, a1, a2, c2);
        break;

    case INDEX_op_xor_i32:
        tcg_out_alf1_rrc(s, 0, ALC_XORW, a0, a1, a2, c2);
        break;
    case INDEX_op_xor_i64:
        tcg_out_alf1_rrc(s, 0, ALC_XORD, a0, a1, a2, c2);
        break;

    case INDEX_op_shl_i32:
        tcg_out_alf1_rrc(s, 0, ALC_SHLW, a0, a1, a2, c2);
        break;
    case INDEX_op_shl_i64:
        tcg_out_alf1_rrc(s, 0, ALC_SHLD, a0, a1, a2, c2);
        break;

    case INDEX_op_shr_i32:
        tcg_out_alf1_rrc(s, 0, ALC_SHRW, a0, a1, a2, c2);
        break;
    case INDEX_op_shr_i64:
        tcg_out_alf1_rrc(s, 0, ALC_SHRD, a0, a1, a2, c2);
        break;

    case INDEX_op_sar_i32:
        tcg_out_alf1_rrc(s, 0, ALC_SARW, a0, a1, a2, c2);
        break;
    case INDEX_op_sar_i64:
        tcg_out_alf1_rrc(s, 0, ALC_SARD, a0, a1, a2, c2);
        break;

    case INDEX_op_rotl_i32:
        tcg_out_alf1_rrc(s, 0, ALC_SCLW, a0, a1, a2, c2);
        break;
    case INDEX_op_rotl_i64:
        tcg_out_alf1_rrc(s, 0, ALC_SCLD, a0, a1, a2, c2);
        break;

    case INDEX_op_rotr_i32:
        tcg_out_alf1_rrc(s, 0, ALC_SCRW, a0, a1, a2, c2);
        break;
    case INDEX_op_rotr_i64:
        tcg_out_alf1_rrc(s, 0, ALC_SCRD, a0, a1, a2, c2);
        break;

    case INDEX_op_add_i32:
        tcg_out_alf1_rrc(s, 0, ALC_ADDW, a0, a1, a2, c2);
        break;
    case INDEX_op_add_i64:
        tcg_out_alf1_rrc(s, 0, ALC_ADDD, a0, a1, a2, c2);
        break;

    case INDEX_op_sub_i32:
        tcg_out_alf1_rrc(s, 0, ALC_SUBW, a0, a1, a2, c2);
        break;
    case INDEX_op_sub_i64:
        tcg_out_alf1_rrc(s, 0, ALC_SUBD, a0, a1, a2, c2);
        break;

    case INDEX_op_mul_i32:
        tcg_out_alf1_rrc(s, 0, ALC_MULW, a0, a1, a2, c2);
        break;
    case INDEX_op_mul_i64:
        tcg_out_alf1_rrc(s, 0, ALC_MULD, a0, a1, a2, c2);
        break;

    case INDEX_op_div_i32:
        tcg_out_alf1_rrc(s, 5, ALC_SDIVW, a0, a1, a2, c2);
        break;
    case INDEX_op_div_i64:
        tcg_out_alf1_rrc(s, 5, ALC_SDIVD, a0, a1, a2, c2);
        break;

    case INDEX_op_divu_i32:
        tcg_out_alf1_rrc(s, 5, ALC_UDIVW, a0, a1, a2, c2);
        break;
    case INDEX_op_divu_i64:
        tcg_out_alf1_rrc(s, 5, ALC_UDIVD, a0, a1, a2, c2);
        break;

    case INDEX_op_rem_i32:
        tcg_out_alf2_rr(s, 3, ALC_SXTW, TCG_REG_TMP0, a1);
        tcg_out_alf1_rrc(s, 5, ALC_SMODX, a0, TCG_REG_TMP0, a2, c2);
        break;
    case INDEX_op_remu_i32:
        tcg_out_alf2_rr(s, 3, ALC_ZXTW, TCG_REG_TMP0, a1);
        tcg_out_alf1_rrc(s, 5, ALC_UMODX, a0, TCG_REG_TMP0, a2, c2);
        break;

    case INDEX_op_rem_i64:
    case INDEX_op_remu_i64:
        g_assert_not_reached();

    case INDEX_op_andc_i32:
        tcg_out_alf1_rrc(s, 0, ALC_ANDNW, a0, a1, a2, c2);
        break;
    case INDEX_op_andc_i64:
        tcg_out_alf1_rrc(s, 0, ALC_ANDND, a0, a1, a2, c2);
        break;
    case INDEX_op_orc_i32:
        tcg_out_alf1_rrc(s, 0, ALC_ORNW, a0, a1, a2, c2);
        break;
    case INDEX_op_orc_i64:
        tcg_out_alf1_rrc(s, 0, ALC_ORND, a0, a1, a2, c2);
        break;
    case INDEX_op_eqv_i32:
        tcg_out_alf1_rrc(s, 0, ALC_XORNW, a0, a1, a2, c2);
        break;
    case INDEX_op_eqv_i64:
        tcg_out_alf1_rrc(s, 0, ALC_XORND, a0, a1, a2, c2);
        break;

    case INDEX_op_not_i32:
        tcg_out_alf1_rir(s, 0, ALC_ORNW, a0, 0, a1);
        break;
    case INDEX_op_not_i64:
        tcg_out_alf1_rir(s, 0, ALC_ORND, a0, 0, a1);
        break;

    case INDEX_op_neg_i32:
        tcg_out_alf1_rir(s, 0, ALC_SUBW, a0, 0, a1);
        break;
    case INDEX_op_neg_i64:
        tcg_out_alf1_rir(s, 0, ALC_SUBD, a0, 0, a1);
        break;

    case INDEX_op_bswap64_i64:
    case INDEX_op_bswap32_i32:
    case INDEX_op_bswap32_i64:
    case INDEX_op_bswap16_i64:
    case INDEX_op_bswap16_i32:
        g_assert_not_reached();

    case INDEX_op_ctz_i32:
        tcg_out_clz_int(s, TCG_TYPE_I32, a0, a1, a2, c2, true);
        break;
    case INDEX_op_clz_i32:
        tcg_out_clz_int(s, TCG_TYPE_I32, a0, a1, a2, c2, false);
        break;
    case INDEX_op_ctz_i64:
        tcg_out_clz_int(s, TCG_TYPE_I64, a0, a1, a2, c2, true);
        break;
    case INDEX_op_clz_i64:
        tcg_out_clz_int(s, TCG_TYPE_I64, a0, a1, a2, c2, false);
        break;

    case INDEX_op_ctpop_i32:
        tcg_out_alf2_rr(s, 1, ALC_POPCNTW, a0, a1);
        break;
    case INDEX_op_ctpop_i64:
        tcg_out_alf2_rr(s, 1, ALC_POPCNTD, a0, a1);
        break;

    case INDEX_op_add2_i32:
    case INDEX_op_add2_i64:
    case INDEX_op_sub2_i32:
    case INDEX_op_sub2_i64:
        g_assert_not_reached();

    case INDEX_op_brcond_i32:
        tcg_out_brcond(s, TCG_TYPE_I32, a2, a0, a1, c1, arg_label(a3));
        break;
    case INDEX_op_brcond_i64:
        tcg_out_brcond(s, TCG_TYPE_I64, a2, a0, a1, c1, arg_label(a3));
        break;

    case INDEX_op_setcond_i32:
        tcg_out_setcond(s, TCG_TYPE_I32, a3, a0, a1, a2, c2);
        break;
    case INDEX_op_setcond_i64:
        tcg_out_setcond(s, TCG_TYPE_I64, a3, a0, a1, a2, c2);
        break;

    case INDEX_op_negsetcond_i32:
        tcg_out_negsetcond(s, TCG_TYPE_I32, a3, a0, a1, a2, c2);
        break;
    case INDEX_op_negsetcond_i64:
        tcg_out_negsetcond(s, TCG_TYPE_I64, a3, a0, a1, a2, c2);
        break;

    case INDEX_op_movcond_i32:
        tcg_out_movcond(s, TCG_TYPE_I32, args[5], a0, a1, a2, c2,
                        args[3], const_args[3], args[4], const_args[4]);
        break;
    case INDEX_op_movcond_i64:
        tcg_out_movcond(s, TCG_TYPE_I64, args[5], a0, a1, a2, c2,
                        args[3], const_args[3], args[4], const_args[4]);
        break;

    case INDEX_op_qemu_ld_a32_i32:
    case INDEX_op_qemu_ld_a64_i32:
        tcg_out_qemu_ld(s, a0, a1, a2, TCG_TYPE_I32);
        break;
    case INDEX_op_qemu_ld_a32_i64:
    case INDEX_op_qemu_ld_a64_i64:
        tcg_out_qemu_ld(s, a0, a1, a2, TCG_TYPE_I64);
        break;
    case INDEX_op_qemu_st_a32_i32:
    case INDEX_op_qemu_st_a64_i32:
        tcg_out_qemu_st(s, a0, a1, a2, TCG_TYPE_I32);
        break;
    case INDEX_op_qemu_st_a32_i64:
    case INDEX_op_qemu_st_a64_i64:
        tcg_out_qemu_st(s, a0, a1, a2, TCG_TYPE_I64);
        break;

    case INDEX_op_extract_i32:
        tcg_out_extract_int(s, TCG_TYPE_I32, a0, a1, a2, a3, false);
        break;
    case INDEX_op_extract_i64:
        tcg_out_extract_int(s, TCG_TYPE_I64, a0, a1, a2, a3, false);
        break;
    case INDEX_op_sextract_i32:
        tcg_out_extract_int(s, TCG_TYPE_I32, a0, a1, a2, a3, true);
        break;
    case INDEX_op_sextract_i64:
        tcg_out_extract_int(s, TCG_TYPE_I64, a0, a1, a2, a3, true);
        break;

    case INDEX_op_deposit_i32:
        tcg_out_deposit_int(s, TCG_TYPE_I32, a0, a1, a2, args[3], args[4]);
        break;
    case INDEX_op_deposit_i64:
        tcg_out_deposit_int(s, TCG_TYPE_I64, a0, a1, a2, args[3], args[4]);
        break;

    case INDEX_op_extrh_i64_i32:
        tcg_out_alf1_rri(s, 0, ALC_SARD, a0, a1, 32);
        break;

    case INDEX_op_mulsh_i32:
    case INDEX_op_muluh_i32:
        g_assert_not_reached();

    case INDEX_op_muluh_i64:
        tcg_out_alf1_rrc(s, 0, ALC_UMULHD, a0, a1, a2, c2);
        break;
    case INDEX_op_mulsh_i64:
        tcg_out_alf1_rrc(s, 0, ALC_SMULHD, a0, a1, a2, c2);
        break;

    case INDEX_op_mb:
        tcg_out_mb(s, a0);
        break;

    case INDEX_op_mov_i32:  /* Always emitted via tcg_out_mov.  */
    case INDEX_op_mov_i64:
    case INDEX_op_call:     /* Always emitted via tcg_out_call.  */
    case INDEX_op_exit_tb:  /* Always emitted via tcg_out_exit_tb.  */
    case INDEX_op_goto_tb:  /* Always emitted via tcg_out_goto_tb.  */
    case INDEX_op_ext8s_i32:  /* Always emitted via tcg_reg_alloc_op.  */
    case INDEX_op_ext8s_i64:
    case INDEX_op_ext8u_i32:
    case INDEX_op_ext8u_i64:
    case INDEX_op_ext16s_i32:
    case INDEX_op_ext16s_i64:
    case INDEX_op_ext16u_i32:
    case INDEX_op_ext16u_i64:
    case INDEX_op_ext32s_i64:
    case INDEX_op_ext32u_i64:
    case INDEX_op_ext_i32_i64:
    case INDEX_op_extu_i32_i64:
    case INDEX_op_extrl_i64_i32:
    default:
        g_assert_not_reached();
    }
}

static TCGConstraintSetIndex tcg_target_op_def(TCGOpcode op)
{
    switch (op) {
    case INDEX_op_goto_ptr:
        return C_O0_I1(r);

    case INDEX_op_ld8u_i32:
    case INDEX_op_ld8u_i64:
    case INDEX_op_ld8s_i32:
    case INDEX_op_ld8s_i64:
    case INDEX_op_ld16u_i32:
    case INDEX_op_ld16u_i64:
    case INDEX_op_ld16s_i32:
    case INDEX_op_ld16s_i64:
    case INDEX_op_ld32u_i64:
    case INDEX_op_ld32s_i64:
    case INDEX_op_ld_i32:
    case INDEX_op_ld_i64:
    case INDEX_op_ext8u_i32:
    case INDEX_op_ext8s_i32:
    case INDEX_op_ext8u_i64:
    case INDEX_op_ext8s_i64:
    case INDEX_op_ext16u_i32:
    case INDEX_op_ext16u_i64:
    case INDEX_op_ext16s_i32:
    case INDEX_op_ext16s_i64:
    case INDEX_op_ext32u_i64:
    case INDEX_op_ext32s_i64:
    case INDEX_op_extu_i32_i64:
    case INDEX_op_ext_i32_i64:
    case INDEX_op_extrl_i64_i32:
    case INDEX_op_extrh_i64_i32:
    case INDEX_op_not_i32:
    case INDEX_op_not_i64:
    case INDEX_op_neg_i32:
    case INDEX_op_neg_i64:
    case INDEX_op_bswap16_i32:
    case INDEX_op_bswap16_i64:
    case INDEX_op_bswap32_i32:
    case INDEX_op_bswap32_i64:
    case INDEX_op_bswap64_i64:
    case INDEX_op_ctpop_i32:
    case INDEX_op_ctpop_i64:
        return C_O1_I1(r, r);

    case INDEX_op_clz_i32:
    case INDEX_op_clz_i64:
    case INDEX_op_ctz_i32:
    case INDEX_op_ctz_i64:
        return C_O1_I2(r, r, rI);

    case INDEX_op_st8_i32:
    case INDEX_op_st8_i64:
    case INDEX_op_st16_i32:
    case INDEX_op_st16_i64:
    case INDEX_op_st32_i64:
    case INDEX_op_st_i32:
    case INDEX_op_st_i64:
        return C_O0_I2(r, r);

    case INDEX_op_and_i32:
    case INDEX_op_and_i64:
    case INDEX_op_or_i32:
    case INDEX_op_or_i64:
    case INDEX_op_xor_i32:
    case INDEX_op_xor_i64:
    case INDEX_op_shl_i32:
    case INDEX_op_shl_i64:
    case INDEX_op_shr_i32:
    case INDEX_op_shr_i64:
    case INDEX_op_sar_i32:
    case INDEX_op_sar_i64:
    case INDEX_op_rotl_i32:
    case INDEX_op_rotl_i64:
    case INDEX_op_rotr_i32:
    case INDEX_op_rotr_i64:
    case INDEX_op_add_i32:
    case INDEX_op_add_i64:
    case INDEX_op_sub_i32:
    case INDEX_op_sub_i64:
    case INDEX_op_mul_i32:
    case INDEX_op_mul_i64:
    case INDEX_op_mulsh_i32:
    case INDEX_op_mulsh_i64:
    case INDEX_op_muluh_i32:
    case INDEX_op_muluh_i64:
    case INDEX_op_div_i32:
    case INDEX_op_div_i64:
    case INDEX_op_divu_i32:
    case INDEX_op_divu_i64:
    case INDEX_op_rem_i32:
    case INDEX_op_rem_i64:
    case INDEX_op_remu_i32:
    case INDEX_op_remu_i64:
    case INDEX_op_setcond_i32:
    case INDEX_op_setcond_i64:
    case INDEX_op_negsetcond_i32:
    case INDEX_op_negsetcond_i64:
    case INDEX_op_andc_i32:
    case INDEX_op_andc_i64:
    case INDEX_op_orc_i32:
    case INDEX_op_orc_i64:
    case INDEX_op_eqv_i32:
    case INDEX_op_eqv_i64:
        return C_O1_I2(r, r, rI);

    case INDEX_op_brcond_i32:
    case INDEX_op_brcond_i64:
        return C_O0_I2(r, rI);

    case INDEX_op_movcond_i32:
    case INDEX_op_movcond_i64:
        return C_O1_I4(r, r, rI, rI, rI);

    case INDEX_op_extract_i32:
    case INDEX_op_extract_i64:
    case INDEX_op_sextract_i32:
    case INDEX_op_sextract_i64:
        return C_O1_I1(r, r);

    case INDEX_op_extract2_i32:
    case INDEX_op_deposit_i32:
    case INDEX_op_deposit_i64:
        return C_O1_I2(r, r, r);

    case INDEX_op_add2_i32:
    case INDEX_op_add2_i64:
    case INDEX_op_sub2_i32:
    case INDEX_op_sub2_i64:
        g_assert_not_reached();

    case INDEX_op_qemu_ld_a32_i32:
    case INDEX_op_qemu_ld_a64_i32:
    case INDEX_op_qemu_ld_a32_i64:
    case INDEX_op_qemu_ld_a64_i64:
        return C_O1_I1(r, r);
    case INDEX_op_qemu_st_a32_i32:
    case INDEX_op_qemu_st_a64_i32:
    case INDEX_op_qemu_st_a32_i64:
    case INDEX_op_qemu_st_a64_i64:
        return C_O0_I2(r, r);

    default:
        g_assert_not_reached();
    }
}

#define TEMP_SIZE   (CPU_TEMP_BUF_NLONGS * (int)sizeof(long))
#define FRAME_SIZE  ((TCG_STATIC_CALL_ARGS_SIZE + TEMP_SIZE \
                      + TCG_TARGET_STACK_ALIGN - 1) \
                     & -TCG_TARGET_STACK_ALIGN)

/* Generate global QEMU prologue and epilogue code */
static void tcg_target_qemu_prologue(TCGContext *s)
{
    Bundle bundle; 

    tcg_set_frame(s, TCG_REG_SP, TCG_STATIC_CALL_ARGS_SIZE, TEMP_SIZE);

    /* TB prologue */
    e2k_out_start(&bundle);

    /*
     * setwd    16
     * setbn    8, 8
     */
    bundle.cs_present[1] = true;
    bundle.cs[1] = 0x040000c4;
    bundle.lts_present[0] = true;
    bundle.lts[0] = 0x00000110;

    e2k_out_prep_r(&bundle, 0, CTPR1, TCG_REG_R1);
    e2k_out_mov(&bundle, 1, TCG_TYPE_PTR, TCG_AREG0, TCG_REG_R0);

#if !defined(CONFIG_SOFTMMU)
    e2k_out_movi(&bundle, 2, TCG_GUEST_BASE_REG, guest_base);
    tcg_regset_set_reg(s->reserved_regs, TCG_GUEST_BASE_REG);
#endif
    int latency = bundle.latency - 1;
    bundle.latency = 1;
    e2k_out_end(s, &bundle);

    /* Always keep ctpr3 with a return address */
    e2k_out_start(&bundle);
    e2k_out_prep_return(&bundle);
    bundle.latency = latency;
    e2k_out_end(s, &bundle);

    e2k_out_start(&bundle);
        /* user stack is freed automatically */
        e2k_out_alf2_ri(&bundle, 0, ALC_GETSP, TCG_REG_SP, -FRAME_SIZE);
        /* Call generated code */
        e2k_out_ct(&bundle, CTPR1, CT_ALWAYS, 0);
    e2k_out_end(s, &bundle);

    /* Return path for goto_ptr. Set return value to 0 */
    tcg_code_gen_epilogue = tcg_splitwx_to_rx(s->code_ptr);

    /* TB epilogue */
    e2k_out_start(&bundle);
    e2k_out_movi(&bundle, TCG_TYPE_REG, TCG_REG_R0, 0);
    e2k_out_ct(&bundle, CTPR3, CT_ALWAYS, 0);
    e2k_out_end(s, &bundle);
}

static void tcg_out_tb_start(TCGContext *s)
{
    /* nothing to do */
}

static void tcg_target_init(TCGContext *s)
{
    tcg_target_available_regs[TCG_TYPE_I32] = 0xffffffff;
    tcg_target_available_regs[TCG_TYPE_I64] = 0xffffffff;

    tcg_target_call_clobber_regs = -1u;
    tcg_regset_reset_reg(tcg_target_call_clobber_regs, TCG_REG_R0);
    tcg_regset_reset_reg(tcg_target_call_clobber_regs, TCG_REG_R1);
    tcg_regset_reset_reg(tcg_target_call_clobber_regs, TCG_REG_R2);
    tcg_regset_reset_reg(tcg_target_call_clobber_regs, TCG_REG_R3);
    tcg_regset_reset_reg(tcg_target_call_clobber_regs, TCG_REG_R4);
    tcg_regset_reset_reg(tcg_target_call_clobber_regs, TCG_REG_R5);
    tcg_regset_reset_reg(tcg_target_call_clobber_regs, TCG_REG_R6);
    tcg_regset_reset_reg(tcg_target_call_clobber_regs, TCG_REG_R7);

    s->reserved_regs = 0;
    tcg_regset_set_reg(s->reserved_regs, TCG_REG_TMP0);
    tcg_regset_set_reg(s->reserved_regs, TCG_REG_TMP1);
    tcg_regset_set_reg(s->reserved_regs, TCG_REG_TMP2);
    tcg_regset_set_reg(s->reserved_regs, TCG_REG_TMP3);
    tcg_regset_set_reg(s->reserved_regs, TCG_REG_SP);
}

typedef struct {
    DebugFrameHeader h;
} DebugFrame;

#define ELF_HOST_MACHINE EM_MCST_ELBRUS

// TODO: e2k debug frame
static const DebugFrame debug_frame = {
    .h.cie.len = sizeof(DebugFrameCIE) - 4, /* length after .len member */
    .h.cie.id = -1,
    .h.cie.version = 1,
    .h.cie.code_align = 1,
    .h.cie.data_align = -(TCG_TARGET_REG_BITS / 8) & 0x7f, /* sleb128 */
    .h.cie.return_column = 0, // TODO:

    /* Total FDE size does not include the "len" member.  */
    .h.fde.len = sizeof(DebugFrame) - offsetof(DebugFrame, h.fde.cie_offset),
};

void tcg_register_jit(const void *buf, size_t buf_size)
{
    tcg_register_jit_int(buf, buf_size, &debug_frame, sizeof(debug_frame));
}

#undef UIMM5
#undef UIMM4
#undef GPR
#undef CTPR
