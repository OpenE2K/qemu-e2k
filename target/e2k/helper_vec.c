#include "qemu/osdep.h"
#include "qemu/log.h"
#include "cpu.h"
#include "helper-tcg.h"
#include "exec/exec-all.h"
#include "qemu/host-utils.h"
#include "exec/helper-proto.h"

static uint8_t reverse_bits(uint8_t b)
{
    b = ((b & 0xf0) >> 4) | ((b & 0x0f) << 4);
    b = ((b & 0xcc) >> 2) | ((b & 0x33) << 2);
    b = ((b & 0xaa) >> 1) | ((b & 0x55) << 1);
    return b;
}

#define vec64_ub 8
#define vec64_uh 4
#define vec64_uw 2
#define vec64_ud 1

#define vec64_sb vec64_ub
#define vec64_sh vec64_uh
#define vec64_sw vec64_uw
#define vec64_sd vec64_ud

#define vec64_len(type) glue(vec64_, type)

#define vec128_ub 16
#define vec128_uh 8
#define vec128_uw 4
#define vec128_ud 2

#define vec128_sb vec128_ub
#define vec128_sh vec128_uh
#define vec128_sw vec128_uw
#define vec128_sd vec128_ud

#define vec128_len(type) glue(vec128_, type)

typedef union {
    uint8_t ub[vec64_ub];
    uint16_t uh[vec64_uh];
    uint32_t uw[vec64_uw];
    uint64_t ud[vec64_ud];
    int8_t sb[vec64_ub];
    int16_t sh[vec64_uh];
    int32_t sw[vec64_uw];
    int64_t sd[vec64_ud];
} vec64;

#define shr16(x) ((x) >> 16)
#define shr14_add1_shr1(x) ((((x) >> 14) + 1) >> 1)
#define and16(x) ((x) & 0xffff)
#define satsb(x) MIN(MAX(x,   -128),   127)
#define satsh(x) MIN(MAX(x, -32768), 32767)
#define satub(x) MIN(MAX(x,      0),   255)
#define satuh(x) MIN(MAX(x,      0), 65535)

#define add(a, b) ((a) + (b))
#define sub(a, b) ((a) - (b))
#define mul(a, b) ((a) * (b))
#define div(a, b) ((a) / (b))

#define GEN_HELPER_PACKED_N(name, n, code) \
    uint64_t HELPER(name)(uint64_t src1, uint64_t src2) \
    { \
        size_t i = 0; \
        vec64 s1 = { .ud[0] = src1 }, s2 = { .ud[0] = src2 }; \
        vec64 dst = { .ud[0] = 0 }; \
        for (; i < n; i++) { \
            code \
        } \
        return dst.ud[0]; \
    }
#define GEN_HELPER_PACKED(name, type, code) \
    GEN_HELPER_PACKED_N(name, vec64_len(type), code)

#define GEN_HELPER_PACKED_OP(name, type, op) \
    GEN_HELPER_PACKED_N(name, vec64_len(type), { \
        dst.type[i] = op(s1.type[i], s2.type[i]); \
    })

#define GEN_HELPER_PACKED_SCALAR(name, type, code) \
    uint64_t HELPER(name)(uint64_t src1, uint64_t s2) \
    { \
        size_t i = 0; \
        vec64 s1 = { .ud[0] = src1 }, dst = { .ud[0] = 0 }; \
        for (; i < glue(vec64_, type); i++) { \
            code \
        } \
        return dst.ud[0]; \
    }

#define IMPL_QPACKED_N(name, n, code) \
    Int128 HELPER(name)(Int128 s1_, Int128 s2_) \
    { \
        E2KReg dst, s1 = { .qp = s1_ }, s2 = { .qp = s2_ }; \
        int i; \
        \
        for (i = 0; i < n; i++) { \
            code \
        } \
        return dst.qp; \
    }

#define IMPL_QPACKED(name, type, code) \
    IMPL_QPACKED_N(name, vec64_len(type), code)

#define IMPL_QPACKED_OP(name, type, op) \
    IMPL_QPACKED_N(name, vec128_len(type), { \
        dst.type[i] = op(s1.type[i], s2.type[i]); \
    })

#define GEN_HELPER_PACKED_MINMAX(name, type, op) \
    GEN_HELPER_PACKED_OP(glue(name, type), type, op)

GEN_HELPER_PACKED_MINMAX(pmin, ub, MIN)
GEN_HELPER_PACKED_MINMAX(pmin, sb, MIN)
GEN_HELPER_PACKED_MINMAX(pmin, uh, MIN)
GEN_HELPER_PACKED_MINMAX(pmin, sh, MIN)
GEN_HELPER_PACKED_MINMAX(pmin, uw, MIN)
GEN_HELPER_PACKED_MINMAX(pmin, sw, MIN)

GEN_HELPER_PACKED_MINMAX(pmax, ub, MAX)
GEN_HELPER_PACKED_MINMAX(pmax, sb, MAX)
GEN_HELPER_PACKED_MINMAX(pmax, uh, MAX)
GEN_HELPER_PACKED_MINMAX(pmax, sh, MAX)
GEN_HELPER_PACKED_MINMAX(pmax, uw, MAX)
GEN_HELPER_PACKED_MINMAX(pmax, sw, MAX)

#define GEN_HELPER_PACKED_CMP(name, type, op) \
    GEN_HELPER_PACKED(name, type, { \
        dst.type[i] = s1.type[i] op s2.type[i] ? -1 : 0; \
    })

GEN_HELPER_PACKED_CMP(pcmpeqb, ub, ==)
GEN_HELPER_PACKED_CMP(pcmpeqh, uh, ==)
GEN_HELPER_PACKED_CMP(pcmpeqw, uw, ==)
GEN_HELPER_PACKED_CMP(pcmpeqd, ud, ==)

GEN_HELPER_PACKED_CMP(pcmpgtb, sb, >)
GEN_HELPER_PACKED_CMP(pcmpgth, sh, >)
GEN_HELPER_PACKED_CMP(pcmpgtw, sw, >)
GEN_HELPER_PACKED_CMP(pcmpgtd, sd, >)

#define GEN_HELPER_PACKED_BINOP_MAP(name, type, op, cast, map) \
    GEN_HELPER_PACKED(name, type, { \
        dst.type[i] = map((cast) s1.type[i] op s2.type[i]); \
    })

GEN_HELPER_PACKED_BINOP_MAP(paddsb,  sb, +, int16_t, satsb)
GEN_HELPER_PACKED_BINOP_MAP(paddsh,  sh, +, int32_t, satsh)
GEN_HELPER_PACKED_BINOP_MAP(paddusb, ub, +, int16_t, satub)
GEN_HELPER_PACKED_BINOP_MAP(paddush, uh, +, int32_t, satuh)

GEN_HELPER_PACKED_BINOP_MAP(psubsb,  sb, -, int16_t, satsb)
GEN_HELPER_PACKED_BINOP_MAP(psubsh,  sh, -, int32_t, satsh)
GEN_HELPER_PACKED_BINOP_MAP(psubusb, ub, -, int16_t, satub)
GEN_HELPER_PACKED_BINOP_MAP(psubush, uh, -, int32_t, satuh)

#define GEN_HELPER_PACKED_HORIZONTAL_OP(name, type, op, map) \
    GEN_HELPER_PACKED_N(name, vec64_len(type) / 2, { \
        int j = i * 2; \
        dst.type[i                      ] = map(op(s1.type[j], s1.type[j + 1])); \
        dst.type[i + vec64_len(type) / 2] = map(op(s2.type[j], s2.type[j + 1])); \
    }) \
    IMPL_QPACKED_N(glue(q, name), vec128_len(type) / 2, { \
        int j = i * 2; \
        dst.type[i                       ] = map(op(s1.type[j], s1.type[j + 1])); \
        dst.type[i + vec128_len(type) / 2] = map(op(s2.type[j], s2.type[j + 1])); \
    })

GEN_HELPER_PACKED_HORIZONTAL_OP(phaddh,  sh, add, ident)
GEN_HELPER_PACKED_HORIZONTAL_OP(phaddw,  sw, add, ident)
GEN_HELPER_PACKED_HORIZONTAL_OP(phaddsh, sh, add, satsh)
GEN_HELPER_PACKED_HORIZONTAL_OP(phsubh,  sh, sub, ident)
GEN_HELPER_PACKED_HORIZONTAL_OP(phsubw,  sw, sub, ident)
GEN_HELPER_PACKED_HORIZONTAL_OP(phsubsh, sh, sub, satsh)

#define GEN_HELPER_PACKED_SCALAR_BINOP(name, type, op) \
    GEN_HELPER_PACKED_SCALAR(name, type, { \
        dst.type[i] = s2 < sizeof(s1.type[0]) * 8 ? s1.type[i] op s2 : 0; \
    })

GEN_HELPER_PACKED_SCALAR_BINOP(psllh, uh, <<)
GEN_HELPER_PACKED_SCALAR_BINOP(psllw, uw, <<)
GEN_HELPER_PACKED_SCALAR_BINOP(pslld, ud, <<)
GEN_HELPER_PACKED_SCALAR_BINOP(psrlh, uh, >>)
GEN_HELPER_PACKED_SCALAR_BINOP(psrlw, uw, >>)
GEN_HELPER_PACKED_SCALAR_BINOP(psrld, ud, >>)

#define GEN_HELPER_PACKED_SRA(name, type, t) \
    GEN_HELPER_PACKED_SCALAR(name, type, { \
        int max = sizeof(s1.type[0]) * 8 - 1; \
        dst.type[i] = (t) s1.type[i] >> (s2 < max ? s2 : max); \
    })

GEN_HELPER_PACKED_SRA(psrah, sh, int32_t)
GEN_HELPER_PACKED_SRA(psraw, sw, int64_t)

#define GEN_HELPER_PACKED_MAD(name, dst_type, t0, t1, cast, op) \
    GEN_HELPER_PACKED(name, dst_type, { \
        int j = i * 2; \
        dst.dst_type[i] = op( \
            (cast) s1.t0[j + 1] * s2.t1[j + 1] + \
            (cast) s1.t0[j    ] * s2.t1[j    ] \
        ); \
    })

GEN_HELPER_PACKED_MAD(pmaddh, sw, sh, sh, int32_t, ident)
GEN_HELPER_PACKED_MAD(pmaddubsh, sh, sb, ub, int32_t, satsh)

GEN_HELPER_PACKED(psadbw, ub, { dst.uw[0] += abs(s1.ub[i] - s2.ub[i]); })

GEN_HELPER_PACKED(pavgusb, ub, { dst.ub[i] = (s1.ub[i] + s2.ub[i] + 1) >> 1; })
GEN_HELPER_PACKED(pavgush, uh, { dst.uh[i] = (s1.uh[i] + s2.uh[i] + 1) >> 1; })

#define GEN_HELPER_PACKED_MULH(name, type, cast, map) \
    GEN_HELPER_PACKED(name, type, { \
        dst.type[i] = map(((cast) s1.type[i]) * s2.type[i]); \
    })

GEN_HELPER_PACKED_MULH(pmulhh,   sh,  int32_t, shr16)
GEN_HELPER_PACKED_MULH(pmullh,   sh,  int32_t, and16)
GEN_HELPER_PACKED_MULH(pmulhuh,  uh, uint32_t, shr16)
GEN_HELPER_PACKED_MULH(pmulhrsh, sh,  int32_t, shr14_add1_shr1)

GEN_HELPER_PACKED(pmulubhh, uh, { \
    dst.uh[i] = ((int16_t) s1.ub[i] * s2.sh[i] + 0x80) >> 8; \
})

GEN_HELPER_PACKED(mpsadbh, uh, { \
    dst.uh[i] = abs((int16_t) s1.ub[i    ] - s2.ub[0]) \
              + abs((int16_t) s1.ub[i + 1] - s2.ub[1]) \
              + abs((int16_t) s1.ub[i + 2] - s2.ub[2]) \
              + abs((int16_t) s1.ub[i + 3] - s2.ub[3]); \
})

#define mul_sign(a, b) ((b) < 0 ? -(a) : ((b) > 0 ? (a) : 0))

GEN_HELPER_PACKED_OP(psignb, sb, mul_sign)
GEN_HELPER_PACKED_OP(psignh, sh, mul_sign)
GEN_HELPER_PACKED_OP(psignw, sw, mul_sign)

#define MOVMASK(mask_type, type) { \
    dst.mask_type[0] |= (s1.type[i] < 0) << (i + glue(vec64_, type)); \
    dst.mask_type[0] |= (s2.type[i] < 0) << (i                     ); \
}

GEN_HELPER_PACKED(pmovmskb,  sb, MOVMASK(uh, sb))
GEN_HELPER_PACKED(pmovmskps, sw, MOVMASK(ub, sw))
GEN_HELPER_PACKED(pmovmskpd, sd, MOVMASK(ub, sd))

#define PACK(dst_type, type, op) { \
    dst.dst_type[i + glue(vec64_, type)] = op(s1.type[i]); \
    dst.dst_type[i                     ] = op(s2.type[i]); \
}

GEN_HELPER_PACKED(packsshb, sh, PACK(sb, sh, satsb))
GEN_HELPER_PACKED(packushb, uh, PACK(ub, sh, satub))
GEN_HELPER_PACKED(packsswh, sw, PACK(sh, sw, satsh))
GEN_HELPER_PACKED(packuswh, sw, PACK(uh, sw, satuh))

#define GEN_HELPER_PACKED_UNPACK(name, type, offset) \
    GEN_HELPER_PACKED(name, type, { \
        int j = offset + i / 2; \
        dst.type[i] = i & 1 ? s1.type[j] : s2.type[j]; \
    })

GEN_HELPER_PACKED_UNPACK(punpcklbh, ub, 0)
GEN_HELPER_PACKED_UNPACK(punpcklhw, uh, 0)
GEN_HELPER_PACKED_UNPACK(punpcklwd, uw, 0)
GEN_HELPER_PACKED_UNPACK(punpckhbh, ub, 4)
GEN_HELPER_PACKED_UNPACK(punpckhhw, uh, 2)
GEN_HELPER_PACKED_UNPACK(punpckhwd, uw, 1)

uint64_t HELPER(pshufb)(uint64_t src1, uint64_t src2, uint64_t src3)
{
    vec64 ret, s1, s2, s3;
    int i;

    s1.ud[0] = src1;
    s2.ud[0] = src2;
    s3.ud[0] = src3;

    for (i = 0; i < 8; i++) {
        uint8_t desc = s3.ub[i];
        int index = desc & 7;
        uint8_t byte = desc & 8 ? s1.ub[index] : s2.ub[index];

        byte = desc & 0x10 ? ~byte : byte;
        byte = desc & 0x20 ? reverse_bits(byte) : byte;
        byte = desc & 0x40 ? (byte & 0x80 ? 0xff : 0) : byte;

        if (desc & 0x80) {
            switch ((desc & 0x70) >> 4) {
                case 2: byte = 0x7f; break;
                case 4: byte = 0x80; break;
                case 6: byte = 0xff; break;
                default: byte = 0; break;
            }
        }

        ret.ub[i] = byte;
    }

    return ret.ud[0];
}

uint64_t HELPER(pmerge)(uint64_t src1, uint64_t src2, uint64_t src3)
{
    vec64 r, s1, s2, s3;
    int i;

    s1.ud[0] = src1;
    s2.ud[0] = src2;
    s3.ud[0] = src3;

    for (i = 0; i < 8; i++) {
        r.ub[i] = s3.sb[i] < 0 ? s2.ub[i] : s1.ub[i];
    }

    return r.ud[0];
}

uint64_t HELPER(pshufh)(uint64_t src1, uint32_t imm8)
{
    int i;
    vec64 s1 = { .ud[0] = src1 }, dst;
    for (i = 0; i < vec64_uh; i++) {
        int j = (imm8 >> (i * 2)) & 0x3;
        dst.uh[i] = s1.uh[j];
    }
    return dst.ud[0];
}

uint64_t HELPER(pshufw)(uint64_t src1, uint64_t src2, uint32_t imm8)
{
    int i;
    vec64 s1 = { .ud[0] = src1 }, s2 = { .ud[0] = src2 }, dst;
    for (i = 0; i < vec64_uw; i++) {
        int sel = (imm8 >> (i * 2)) & 0x3;
        int j = sel & 1;
        dst.uw[i] = sel > 1 ? s1.uw[j] : s2.uw[j];
    }
    return dst.ud[0];
}

uint64_t HELPER(phminposuh)(uint64_t src1, uint64_t src2)
{
    int i;
    vec64 s1 = { .ud[0] = src1 }, s2 = { .ud[0] = src2 }, dst = { .ud[0] = 0 };
    dst.uh[0] = s1.uh[0];
    for (i = 0; i < 4; i++) {
        if (s1.uh[i] < dst.uh[0]) {
            dst.uh[0] = s1.uh[i];
            dst.uh[1] = i;
        }
    }
    for (i = 0; i < 4; i++) {
        if (s2.uh[i] < dst.uh[0]) {
            dst.uh[0] = s2.uh[i];
            dst.uh[1] = 4 + i;
        }
    }
    return dst.ud[0];
}

Int128 HELPER(qpmpsadbh)(Int128 s1_, uint32_t s2)
{
    E2KReg dst, s1 = { .qp = s1_ };
    dst.ud[0] = helper_mpsadbh(s1.ud[0], s2);
    dst.ud[1] = helper_mpsadbh(s1.ud[1], s2);
    return dst.qp;
}

Int128 HELPER(qpmulubhh)(uint64_t s1, Int128 s2_)
{
    E2KReg dst, s2 = { .qp = s2_ };
    dst.ud[0] = helper_pmulubhh(s1      , s2.ud[0]);
    dst.ud[1] = helper_pmulubhh(s1 >> 32, s2.ud[1]);
    return dst.qp;
}

uint64_t HELPER(qphminposuh)(Int128 s1_, Int128 s2_)
{
    E2KReg s1 = { .qp = s1_ }, s2 = { .qp = s2_ };
    int i;
    uint16_t v = s1.uh[0];
    uint16_t p = 0;

    for (i = 0; i < 8; i++) {
        if (s1.uh[i] < v) {
            v = s1.uh[i];
            p = i;
        }
    }
    for (i = 0; i < 8; i++) {
        if (s2.uh[i] < v) {
            v = s2.uh[i];
            p = 8 + i;
        }
    }
    return ((uint64_t) p << 16) | v;
}

uint32_t HELPER(qpsgn2mskb)(Int128 s2_)
{
    E2KReg s2 = { .qp = s2_ };
    uint32_t r = 0;
    int i;

    for (i = 0; i < 16; i++) {
        r |= s2.sb[i] < 0 ? 1 << i : 0;
    }

    return r;
}

Int128 HELPER(qpmsk2sgnb)(Int128 s1_, uint32_t s2)
{
    E2KReg dst, s1 = { .qp = s1_ };
    int i;

    for (i = 0; i < 16; i++) {
        dst.ub[i] = s2 & (1 << i) ? s1.ub[i] | 0x80 : s1.ub[i] & 0x7f;
    }

    return dst.qp;
}

Int128 HELPER(qppermb)(Int128 s1_, Int128 s2_, Int128 s3_)
{
    E2KReg dst, s1 = { .qp = s1_ }, s2 = { .qp = s2_ }, s3 = { .qp = s3_ };
    int i;

    for (i = 0; i < 16; i++) {
        uint8_t sel = s3.ub[i];
        int index = sel & 0x0f;
        uint8_t byte = sel & 0x10 ? s1.ub[index] : s2.ub[index];

        byte = sel & 0x20 ? ~byte : byte;
        byte = sel & 0x40 ? reverse_bits(byte) : byte;
        byte = sel & 0x80 ? (byte & 0x80 ? 0xff : 0) : byte;

        dst.ub[i] = byte;
    }

    return dst.qp;
}

Int128 HELPER(qpshufb)(Int128 s1_, Int128 s2_, Int128 s3_)
{
    E2KReg dst, s1 = { .qp = s1_ }, s2 = { .qp = s2_ }, s3 = { .qp = s3_ };
    dst.ud[0] = helper_pshufb(s2.ud[1], s2.ud[0], s3.ud[0]);
    dst.ud[1] = helper_pshufb(s1.ud[1], s1.ud[0], s3.ud[1]);
    return dst.qp;
}

static uint64_t get_value_from_truth_table(bool x, bool y, bool z, uint32_t truth_table)
{
    int pos = x << 2 | y << 1 | z;
    return (truth_table >> pos) & 1ULL;
}

uint64_t HELPER(plog)(uint32_t opc, uint64_t src1, uint64_t src2, uint64_t src3)
{
    int i;
    uint64_t ret = 0;

    for (i = 0; i < 64; i++) {
        uint32_t x = extract64(src1, i, 1);
        uint32_t y = extract64(src2, i, 1);
        uint32_t z = extract64(src3, i, 1);

        uint64_t bit = get_value_from_truth_table(x, y, z, opc);

        ret |= bit << i;
    }

    return ret;
}

#define GEN_ENV_HELPER_PACKED_N(name, n, code) \
    uint64_t HELPER(name)(CPUE2KState *env, uint64_t src1, uint64_t src2) \
    { \
        size_t i = 0; \
        vec64 s1 = { .ud[0] = src1 }, s2 = { .ud[0] = src2 }; \
        vec64 dst = { .ud[0] = 0 }; \
        for (; i < n; i++) { \
            code \
        } \
        return dst.ud[0]; \
    }

#define GEN_ENV_HELPER_PACKED(name, type, code) \
    GEN_ENV_HELPER_PACKED_N(name, vec64_len(type), code)

#define GEN_ENV_HELPER_PACKED_OP(name, type, op) \
    GEN_ENV_HELPER_PACKED_N(name, vec64_len(type), { \
        dst.type[i] = op(env, s1.type[i], s2.type[i]); \
    })

#define GEN_ENV_HELPER_PACKED_HORIZONTAL_OP(name, type, op, map) \
    GEN_ENV_HELPER_PACKED_N(name, vec64_len(type) / 2, { \
        int j = i * 2; \
        dst.type[i                      ] = map(op(env, s1.type[j], s1.type[j + 1])); \
        dst.type[i + vec64_len(type) / 2] = map(op(env, s2.type[j], s2.type[j + 1])); \
    })

#define IMPL_QPACKED_ENV_N(name, n, code) \
    Int128 HELPER(name)(CPUE2KState *env, Int128 s1_, Int128 s2_) \
    { \
        E2KReg dst, s1 = { .qp = s1_ }, s2 = { .qp = s2_ }; \
        int i; \
        \
        for (i = 0; i < n; i++) { \
            code \
        } \
        return dst.qp; \
    }

#define IMPL_QPACKED_ENV(name, type, code) \
    IMPL_QPACKED_ENV_N(name, vec128_len(type), code)

#define IMPL_QPACKED_ENV_OP(name, type, op) \
    IMPL_QPACKED_ENV_N(name, vec128_len(type), { \
        dst.type[i] = op(env, s1.type[i], s2.type[i]); \
    })

#define IMPL_QPACKED_ENV_HOP(name, type, op, map) \
    IMPL_QPACKED_ENV_N(name, vec128_len(type) / 2, { \
        int j = i * 2; \
        dst.type[i                       ] = map(op(env, s1.type[j], s1.type[j + 1])); \
        dst.type[i + vec128_len(type) / 2] = map(op(env, s2.type[j], s2.type[j + 1])); \
    })

GEN_ENV_HELPER_PACKED_OP(pfadds, uw, helper_fadds)
GEN_ENV_HELPER_PACKED_OP(pfsubs, uw, helper_fsubs)
GEN_ENV_HELPER_PACKED_OP(pfmuls, uw, helper_fmuls)
GEN_ENV_HELPER_PACKED_OP(pfmaxs, uw, helper_fmaxs)
GEN_ENV_HELPER_PACKED_OP(pfmins, uw, helper_fmins)
GEN_ENV_HELPER_PACKED_HORIZONTAL_OP(pfhadds, uw, helper_fadds, ident)
GEN_ENV_HELPER_PACKED_HORIZONTAL_OP(pfhsubs, uw, helper_fsubs, ident)
IMPL_QPACKED_ENV_HOP(qpfhadds, uw, helper_fadds, ident)
IMPL_QPACKED_ENV_HOP(qpfhsubs, uw, helper_fsubs, ident)

GEN_ENV_HELPER_PACKED_OP(pfcmpeqs,  uw, helper_fcmpeqs)
GEN_ENV_HELPER_PACKED_OP(pfcmplts,  uw, helper_fcmplts)
GEN_ENV_HELPER_PACKED_OP(pfcmples,  uw, helper_fcmples)
GEN_ENV_HELPER_PACKED_OP(pfcmpuods, uw, helper_fcmpuods)
GEN_ENV_HELPER_PACKED_OP(pfcmpneqs, uw, helper_fcmpneqs)
GEN_ENV_HELPER_PACKED_OP(pfcmpnlts, uw, helper_fcmpnlts)
GEN_ENV_HELPER_PACKED_OP(pfcmpnles, uw, helper_fcmpnles)
GEN_ENV_HELPER_PACKED_OP(pfcmpods,  uw, helper_fcmpods)

GEN_ENV_HELPER_PACKED(pfaddsubs, uw, { \
    if (i & 1) { \
        dst.uw[i] = helper_fadds(env, s1.uw[i], s2.uw[i]); \
    } else { \
        dst.uw[i] = helper_fsubs(env, s1.uw[i], s2.uw[i]); \
    } \
})

#define GEN_ENV_HELPER_PACKED_UNARY_N(name, n, code) \
    uint64_t HELPER(name)(CPUE2KState *env, uint64_t src2) \
    { \
        size_t i = 0; \
        vec64 s2 = { .ud[0] = src2 }; \
        vec64 dst = { .ud[0] = 0 }; \
        for (; i < n; i++) { \
            code \
        } \
        return dst.ud[0]; \
    }
#define GEN_ENV_HELPER_PACKED_UNARY(name, type, code) \
    GEN_ENV_HELPER_PACKED_UNARY_N(name, vec64_len(type), code)
#define GEN_ENV_HELPER_PACKED_UNARY_OP(name, type, op) \
    GEN_ENV_HELPER_PACKED_UNARY_N(name, vec64_len(type), { \
        dst.type[i] = op(env, s2.type[i]); \
    })

#define GEN_ENV_HELPER_PACKED_OP_CVT(name, type, op) \
    GEN_ENV_HELPER_PACKED_N(name, vec64_len(type), { \
        dst.type[i] = op(env, s1.uw[0], s2.type[i]); \
    })

GEN_ENV_HELPER_PACKED_OP_CVT(pfstoifs, uw, helper_fstoifs)
GEN_ENV_HELPER_PACKED_UNARY_OP(pistofs, uw, helper_istofs)
GEN_ENV_HELPER_PACKED_UNARY_OP(pfstois, uw, helper_fstois)
GEN_ENV_HELPER_PACKED_UNARY_OP(pfstoistr, uw, helper_fstoistr)

#define IMPL_QPACKED_ENV_CVT_EXT(name, op) \
    Int128 HELPER(name)(CPUE2KState *env, uint64_t src2) \
    { \
        E2KReg dst; \
        vec64 s2 = { .ud[0] = src2 }; \
        \
        dst.ud[0] = op(env, s2.uw[0]); \
        dst.ud[1] = op(env, s2.uw[1]); \
        \
        return dst.qp; \
    }

IMPL_QPACKED_ENV_CVT_EXT(qpfstoid,   helper_fstoid)
IMPL_QPACKED_ENV_CVT_EXT(qpfstoidtr, helper_fstoidtr)
IMPL_QPACKED_ENV_CVT_EXT(qpistofd,   helper_istofd)
IMPL_QPACKED_ENV_CVT_EXT(qpfstofd,   helper_fstofd)

#define IMPL_QPACKED_ENV_CVT_TRUNC(name, op) \
    uint64_t HELPER(name)(CPUE2KState *env, Int128 s2_) \
    { \
        E2KReg s2 = { .qp = s2_ }; \
        vec64 r; \
        \
        r.uw[0] = op(env, s2.ud[0]); \
        r.uw[1] = op(env, s2.ud[1]); \
        \
        return r.ud[0]; \
    }

IMPL_QPACKED_ENV_CVT_TRUNC(qpfdtois,   helper_fdtois)
IMPL_QPACKED_ENV_CVT_TRUNC(qpfdtoistr, helper_fdtoistr)
IMPL_QPACKED_ENV_CVT_TRUNC(qpidtofs,   helper_idtofs)
IMPL_QPACKED_ENV_CVT_TRUNC(qpfdtofs,   helper_fdtofs)

static uint32_t mask4(uint8_t bitmask)
{
    static uint32_t map[16] = {
        0x00000000,
        0x000000ff,
        0x0000ff00,
        0x0000ffff,
        0x00ff0000,
        0x00ff00ff,
        0x00ffff00,
        0x00ffffff,
        0xff000000,
        0xff0000ff,
        0xff00ff00,
        0xff00ffff,
        0xffff0000,
        0xffff00ff,
        0xffffff00,
        0xffffffff
    };
    return map[bitmask & 15];
}

static inline uint64_t mask8(uint8_t bitmask)
{
    return deposit64(mask4(bitmask), 32, 32, mask4(bitmask >> 4));
}

static inline Int128 mask16(uint16_t bitmask)
{
    return int128_make128(mask8(bitmask), mask8(bitmask >> 8));
}

static Int128 qmerge(Int128 a, Int128 b, uint32_t mask)
{
    Int128 m;

    m = mask16(mask);
    a = int128_and(a, m);
    b = int128_and(b, int128_not(m));
    return int128_or(a,b );
}

static void stmqp(CPUE2KState *env, target_ulong addr, Int128 value,
    uint32_t mask, MemOp memop, bool sm)
{
    Int128 t;
    MemOpIdx oi = make_memop_idx(memop | MO_LE | MO_UO, cpu_mmu_index(env, false));

    if (mask & 0xffff && (!sm || e2k_probe_rw_access(env, addr, 16))) {
        t = cpu_ld16_mmu(env, addr, oi, GETPC());
        t = qmerge(value, t, mask);
        cpu_st16_mmu(env, addr, t, oi, GETPC());
    }
}

void HELPER(stmqp5)(CPUE2KState *env, target_ulong addr, Int128 value,
    uint32_t mask, uint32_t sm)
{
    stmqp(env, addr, value, mask, MO_ALIGN, sm);
}

void HELPER(stmqp6)(CPUE2KState *env, target_ulong addr, Int128 value,
    uint32_t mask, uint32_t sm)
{
    target_ulong start, last;

    start = addr & TARGET_PAGE_MASK;
    last = (addr + 15) & TARGET_PAGE_MASK;

    if (start != last) {
        int align = addr & 15;

        stmqp(env, addr - align, int128_lshift(value, align * 8), mask << align, MO_UNALN, sm);

        align = 16 - align;
        value = int128_rshift(value, align * 8);
        addr += align;
        mask = (mask & 0xffff) >> align;
    }

    stmqp(env, addr, value, mask, MO_UNALN, sm);
}

uint32_t HELPER(stmqp_mlock)(CPUE2KState *env, target_ulong addr, Int128 value,
    uint32_t mask, uint32_t sm, Int128 last_val)
{
    Int128 t;
    MemOpIdx oi = make_memop_idx(MO_ALIGN | MO_LE | MO_UO, cpu_mmu_index(env, false));

    if (mask & 0xffff && (!sm || e2k_probe_rw_access(env, addr, 16))) {
        t = cpu_ld16_mmu(env, addr, oi, GETPC());
        t = qmerge(value, t, mask);
        t = cpu_atomic_cmpxchgo_le_mmu(env, addr, last_val, t, oi, GETPC());
        return int128_ne(t, last_val);
    } else {
        return 0;
    }
}
