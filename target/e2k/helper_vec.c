/*
 * SIMD E2K helpers
 *
 * SPDX-License-Identifier: GPL-2.0-only
 */

#include "qemu/osdep.h"
#include "cpu.h"
#include "helper-tcg.h"
#include "exec/exec-all.h"
#include "qemu/host-utils.h"
#include "exec/helper-proto.h"

#define glue3(x, y, z) glue(glue(x, y), z)
#define glue4(x, y, z, w) glue(glue3(x, y, z), w)

#define vec64_ub 8
#define vec64_uh 4
#define vec64_uw 2
#define vec64_ud 1
#define vec64_sb vec64_ub
#define vec64_sh vec64_uh
#define vec64_sw vec64_uw
#define vec64_sd vec64_ud

#define vec128_ub 16
#define vec128_uh 8
#define vec128_uw 4
#define vec128_ud 2
#define vec128_sb vec128_ub
#define vec128_sh vec128_uh
#define vec128_sw vec128_uw
#define vec128_sd vec128_ud

#define vec_count(len, type) glue4(vec, len, _, type)

typedef union {
    uint8_t     ub[vec_count(64, ub)];
    uint16_t    uh[vec_count(64, uh)];
    uint32_t    uw[vec_count(64, uw)];
    uint64_t    ud[vec_count(64, ud)];
    int8_t      sb[vec_count(64, sb)];
    int16_t     sh[vec_count(64, sh)];
    int32_t     sw[vec_count(64, sw)];
    int64_t     sd[vec_count(64, sd)];
} vec64;

typedef union {
    uint8_t     ub[vec_count(128, ub)];
    uint16_t    uh[vec_count(128, uh)];
    uint32_t    uw[vec_count(128, uw)];
    uint64_t    ud[vec_count(128, ud)];
    int8_t      sb[vec_count(128, sb)];
    int16_t     sh[vec_count(128, sh)];
    int32_t     sw[vec_count(128, sw)];
    int64_t     sd[vec_count(128, sd)];
} vec128;

static inline vec64 vec64_from_raw(uint64_t raw)
{
    vec64 ret;
    ret.ud[0] = raw;
    return ret;
}

static inline uint64_t vec64_into_raw(vec64 vec)
{
    return vec.ud[0];
}

static inline vec128 vec128_from_raw(Int128 raw)
{
    vec128 ret;
    ret.ud[0] = int128_getlo(raw);
    ret.ud[1] = int128_gethi(raw);
    return ret;
}

static inline Int128 vec128_into_raw(vec128 vec)
{
    return int128_make128(vec.ud[0], vec.ud[1]);
}

#define vec_from_raw(len, x) glue3(vec, len, _from_raw)(x)
#define vec_into_raw(len, x) glue3(vec, len, _into_raw)(x)

#define vec_zero64()  vec64_from_raw(0)
#define vec_zero128() vec128_from_raw(int128_zero())
#define vec_zero(len) glue(vec_zero, len)()

#define vec_raw_64  uint64_t
#define vec_raw_128 Int128
#define vec_raw(len) glue(vec_raw_, len)

#define vec(len) glue(vec, len)

#define shr16(x) ((x) >> 16)
#define shr14_add1_shr1(x) ((((x) >> 14) + 1) >> 1)
#define and16(x) ((x) & 0xffff)
#define satsb(x) MIN(MAX(x,   -128),   127)
#define satsh(x) MIN(MAX(x, -32768), 32767)
#define satub(x) MIN(MAX(x,      0),   255)
#define satuh(x) MIN(MAX(x,      0), 65535)

#define and(a, b)   ((a) & (b))
#define andn(a, b)  ((a) & !(b))
#define or(a, b)    ((a) | (b))
#define xor(a, b)   ((a) ^ (b))
#define shl(a, b)   ((b) < sizeof(a) * 8 ? (a) << (b) : 0)
#define shr(a, b)   ((b) < sizeof(a) * 8 ? (a) >> (b) : 0)
#define sar(a, b)   ((a) >> MIN(b, sizeof(a) * 8 - 1))
#define add(a, b)   ((a) + (b))
#define sub(a, b)   ((a) - (b))
#define mul(a, b)   ((a) * (b))
#define div(a, b)   ((a) / (b))

#define GEN_HELPER_PACKED_N(name, len, n, code) \
    vec_raw(len) HELPER(name)(vec_raw(len) src1, vec_raw(len) src2) \
    { \
        vec(len) dst, s1, s2; \
        dst = vec_zero(len); \
        s1 = vec_from_raw(len, src1); \
        s2 = vec_from_raw(len, src2); \
        for (int i = 0; i < n; i++) { \
            code \
        } \
        return vec_into_raw(len, dst); \
    }

#define GEN_HELPER_PACKED(name, len, type, code) \
    GEN_HELPER_PACKED_N(name, len, vec_count(len, type), code)

#define GEN_HELPER_PACKED_OP(name, len, type, op) \
    GEN_HELPER_PACKED_N(name, len, vec_count(len, type), { \
        dst.type[i] = op(s1.type[i], s2.type[i]); \
    })

#define GEN_HELPER_PACKED_QP(name, op) \
    vec_raw(128) HELPER(name)(vec_raw(128) src1, vec_raw(128) src2) \
    { \
        vec(128) dst, s1, s2; \
        \
        s1 = vec_from_raw(128, src1); \
        s2 = vec_from_raw(128, src2); \
        \
        for (int i = 0; i < vec_count(128, ud); ++i) \
            dst.ud[i] = op(s1.ud[i], s2.ud[i]); \
        \
        return vec_into_raw(128, dst); \
    }

GEN_HELPER_PACKED_OP(paddb,    64, ub, add)
GEN_HELPER_PACKED_OP(paddh,    64, uh, add)
GEN_HELPER_PACKED_OP(paddw,    64, uw, add)

GEN_HELPER_PACKED_OP(psubb,    64, ub, sub)
GEN_HELPER_PACKED_OP(psubh,    64, uh, sub)
GEN_HELPER_PACKED_OP(psubw,    64, uw, sub)

GEN_HELPER_PACKED_OP(qpaddb,  128, ub, add)
GEN_HELPER_PACKED_OP(qpaddh,  128, uh, add)
GEN_HELPER_PACKED_OP(qpaddw,  128, uw, add)

GEN_HELPER_PACKED_OP(qpsubb,  128, ub, sub)
GEN_HELPER_PACKED_OP(qpsubh,  128, uh, sub)
GEN_HELPER_PACKED_OP(qpsubw,  128, uw, sub)

GEN_HELPER_PACKED_OP(pminub,   64, ub, MIN)
GEN_HELPER_PACKED_OP(pminsb,   64, sb, MIN)
GEN_HELPER_PACKED_OP(pminuh,   64, uh, MIN)
GEN_HELPER_PACKED_OP(pminsh,   64, sh, MIN)
GEN_HELPER_PACKED_OP(pminuw,   64, uw, MIN)
GEN_HELPER_PACKED_OP(pminsw,   64, sw, MIN)

GEN_HELPER_PACKED_OP(pmaxub,   64, ub, MAX)
GEN_HELPER_PACKED_OP(pmaxsb,   64, sb, MAX)
GEN_HELPER_PACKED_OP(pmaxuh,   64, uh, MAX)
GEN_HELPER_PACKED_OP(pmaxsh,   64, sh, MAX)
GEN_HELPER_PACKED_OP(pmaxuw,   64, uw, MAX)
GEN_HELPER_PACKED_OP(pmaxsw,   64, sw, MAX)

GEN_HELPER_PACKED_OP(qpminub, 128, ub, MIN)
GEN_HELPER_PACKED_OP(qpminsb, 128, sb, MIN)
GEN_HELPER_PACKED_OP(qpminuh, 128, uh, MIN)
GEN_HELPER_PACKED_OP(qpminsh, 128, sh, MIN)
GEN_HELPER_PACKED_OP(qpminuw, 128, uw, MIN)
GEN_HELPER_PACKED_OP(qpminsw, 128, sw, MIN)

GEN_HELPER_PACKED_OP(qpmaxub, 128, ub, MAX)
GEN_HELPER_PACKED_OP(qpmaxsb, 128, sb, MAX)
GEN_HELPER_PACKED_OP(qpmaxuh, 128, uh, MAX)
GEN_HELPER_PACKED_OP(qpmaxsh, 128, sh, MAX)
GEN_HELPER_PACKED_OP(qpmaxuw, 128, uw, MAX)
GEN_HELPER_PACKED_OP(qpmaxsw, 128, sw, MAX)

#define GEN_HELPER_PACKED_CMP(name, len, type, op) \
    GEN_HELPER_PACKED(name, len, type, { \
        dst.type[i] = s1.type[i] op s2.type[i] ? -1 : 0; \
    })

GEN_HELPER_PACKED_CMP(pcmpeqb, 64, ub, ==)
GEN_HELPER_PACKED_CMP(pcmpeqh, 64, uh, ==)
GEN_HELPER_PACKED_CMP(pcmpeqw, 64, uw, ==)
GEN_HELPER_PACKED_CMP(pcmpeqd, 64, ud, ==)

GEN_HELPER_PACKED_CMP(pcmpgtb, 64, sb, >)
GEN_HELPER_PACKED_CMP(pcmpgth, 64, sh, >)
GEN_HELPER_PACKED_CMP(pcmpgtw, 64, sw, >)
GEN_HELPER_PACKED_CMP(pcmpgtd, 64, sd, >)

GEN_HELPER_PACKED_CMP(qpcmpeqb, 128, ub, ==)
GEN_HELPER_PACKED_CMP(qpcmpeqh, 128, uh, ==)
GEN_HELPER_PACKED_CMP(qpcmpeqw, 128, uw, ==)
GEN_HELPER_PACKED_CMP(qpcmpeqd, 128, ud, ==)

GEN_HELPER_PACKED_CMP(qpcmpgtb, 128, sb, >)
GEN_HELPER_PACKED_CMP(qpcmpgth, 128, sh, >)
GEN_HELPER_PACKED_CMP(qpcmpgtw, 128, sw, >)
GEN_HELPER_PACKED_CMP(qpcmpgtd, 128, sd, >)

#define GEN_HELPER_PACKED_OP_MAP(name, len, type, op, cast, map) \
    GEN_HELPER_PACKED(name, len, type, { \
        dst.type[i] = map(op((cast) s1.type[i], s2.type[i])); \
    })

GEN_HELPER_PACKED_OP_MAP(paddsb,  64, sb, add, int16_t, satsb)
GEN_HELPER_PACKED_OP_MAP(paddsh,  64, sh, add, int32_t, satsh)
GEN_HELPER_PACKED_OP_MAP(paddusb, 64, ub, add, int16_t, satub)
GEN_HELPER_PACKED_OP_MAP(paddush, 64, uh, add, int32_t, satuh)

GEN_HELPER_PACKED_OP_MAP(psubsb,  64, sb, sub, int16_t, satsb)
GEN_HELPER_PACKED_OP_MAP(psubsh,  64, sh, sub, int32_t, satsh)
GEN_HELPER_PACKED_OP_MAP(psubusb, 64, ub, sub, int16_t, satub)
GEN_HELPER_PACKED_OP_MAP(psubush, 64, uh, sub, int32_t, satuh)

GEN_HELPER_PACKED_OP_MAP(qpaddsb,  128, sb, add, int16_t, satsb)
GEN_HELPER_PACKED_OP_MAP(qpaddsh,  128, sh, add, int32_t, satsh)
GEN_HELPER_PACKED_OP_MAP(qpaddusb, 128, ub, add, int16_t, satub)
GEN_HELPER_PACKED_OP_MAP(qpaddush, 128, uh, add, int32_t, satuh)

GEN_HELPER_PACKED_OP_MAP(qpsubsb,  128, sb, sub, int16_t, satsb)
GEN_HELPER_PACKED_OP_MAP(qpsubsh,  128, sh, sub, int32_t, satsh)
GEN_HELPER_PACKED_OP_MAP(qpsubusb, 128, ub, sub, int16_t, satub)
GEN_HELPER_PACKED_OP_MAP(qpsubush, 128, uh, sub, int32_t, satuh)

#define GEN_HELPER_PACKED_OP_HORIZONTAL(name, len, type, op, map) \
    GEN_HELPER_PACKED_N(name, len, vec_count(len, type) / 2, { \
        int j = i * 2; \
        dst.type[i                           ] = map(op(s1.type[j], s1.type[j + 1])); \
        dst.type[i + vec_count(len, type) / 2] = map(op(s2.type[j], s2.type[j + 1])); \
    })

GEN_HELPER_PACKED_OP_HORIZONTAL(phaddh,    64, sh, add, ident)
GEN_HELPER_PACKED_OP_HORIZONTAL(phaddw,    64, sw, add, ident)
GEN_HELPER_PACKED_OP_HORIZONTAL(phaddsh,   64, sh, add, satsh)
GEN_HELPER_PACKED_OP_HORIZONTAL(phsubh,    64, sh, sub, ident)
GEN_HELPER_PACKED_OP_HORIZONTAL(phsubw,    64, sw, sub, ident)
GEN_HELPER_PACKED_OP_HORIZONTAL(phsubsh,   64, sh, sub, satsh)

GEN_HELPER_PACKED_OP_HORIZONTAL(qphaddh,  128, sh, add, ident)
GEN_HELPER_PACKED_OP_HORIZONTAL(qphaddw,  128, sw, add, ident)
GEN_HELPER_PACKED_OP_HORIZONTAL(qphaddsh, 128, sh, add, satsh)
GEN_HELPER_PACKED_OP_HORIZONTAL(qphsubh,  128, sh, sub, ident)
GEN_HELPER_PACKED_OP_HORIZONTAL(qphsubw,  128, sw, sub, ident)
GEN_HELPER_PACKED_OP_HORIZONTAL(qphsubsh, 128, sh, sub, satsh)

#define GEN_HELPER_PACKED_SHIFT(name, len, type, op) \
    vec_raw(len) HELPER(name)(vec_raw(len) src1, uint64_t s2) \
    { \
        vec(len) dst, s1; \
        dst = vec_zero(len); \
        s1 = vec_from_raw(len, src1); \
        for (int i = 0; i < vec_count(len, type); i++) { \
            dst.type[i] = op(s1.type[i], s2); \
        } \
        return vec_into_raw(len, dst); \
    }

GEN_HELPER_PACKED_SHIFT(psllh,   64, uh, shl)
GEN_HELPER_PACKED_SHIFT(psllw,   64, uw, shl)
GEN_HELPER_PACKED_SHIFT(pslld,   64, ud, shl)
GEN_HELPER_PACKED_SHIFT(psrlh,   64, uh, shr)
GEN_HELPER_PACKED_SHIFT(psrlw,   64, uw, shr)
GEN_HELPER_PACKED_SHIFT(psrld,   64, ud, shr)
GEN_HELPER_PACKED_SHIFT(psrah,   64, sh, sar)
GEN_HELPER_PACKED_SHIFT(psraw,   64, sw, sar)
GEN_HELPER_PACKED_SHIFT(psrcw,   64, uw, ror32)

GEN_HELPER_PACKED_SHIFT(qpsllh, 128, uh, shl)
GEN_HELPER_PACKED_SHIFT(qpsllw, 128, uw, shl)
GEN_HELPER_PACKED_SHIFT(qpslld, 128, ud, shl)
GEN_HELPER_PACKED_SHIFT(qpsrlh, 128, uh, shr)
GEN_HELPER_PACKED_SHIFT(qpsrlw, 128, uw, shr)
GEN_HELPER_PACKED_SHIFT(qpsrld, 128, ud, shr)
GEN_HELPER_PACKED_SHIFT(qpsrah, 128, sh, sar)
GEN_HELPER_PACKED_SHIFT(qpsraw, 128, sw, sar)
GEN_HELPER_PACKED_SHIFT(qpsrad, 128, sd, sar)
GEN_HELPER_PACKED_SHIFT(qpsrcw, 128, uw, ror32)
GEN_HELPER_PACKED_SHIFT(qpsrcd, 128, ud, ror64)

#define GEN_HELPER_PACKED_MAD(name, len, dst_type, t0, t1, cast, op) \
    GEN_HELPER_PACKED(name, len, dst_type, { \
        int j = i * 2; \
        dst.dst_type[i] = op( \
            (cast) s1.t0[j + 1] * s2.t1[j + 1] + \
            (cast) s1.t0[j    ] * s2.t1[j    ] \
        ); \
    })

GEN_HELPER_PACKED_MAD(pmaddh,      64, sw, sh, sh, int32_t, ident)
GEN_HELPER_PACKED_MAD(pmaddubsh,   64, sh, sb, ub, int32_t, satsh)

GEN_HELPER_PACKED_MAD(qpmaddh,    128, sw, sh, sh, int32_t, ident)
GEN_HELPER_PACKED_MAD(qpmaddubsh, 128, sh, sb, ub, int32_t, satsh)

GEN_HELPER_PACKED(psadbw, 64, ub, { dst.uw[0] += abs(s1.ub[i] - s2.ub[i]); })
GEN_HELPER_PACKED_QP(qpsadbw, helper_psadbw)

#define avgus(a, b) (((a) + (b) + 1) >> 1)

GEN_HELPER_PACKED_OP(pavgusb,   64, ub, avgus)
GEN_HELPER_PACKED_OP(pavgush,   64, uh, avgus)

GEN_HELPER_PACKED_OP(qpavgusb, 128, ub, avgus)
GEN_HELPER_PACKED_OP(qpavgush, 128, uh, avgus)

GEN_HELPER_PACKED_OP(pmullw,   64, uw, mul)
GEN_HELPER_PACKED_OP(qpmullw, 128, uw, mul)

#define GEN_HELPER_PACKED_MULH(name, len, type, cast, map) \
    GEN_HELPER_PACKED(name, len, type, { \
        dst.type[i] = map(((cast) s1.type[i]) * s2.type[i]); \
    })

GEN_HELPER_PACKED_MULH(pmulhh,     64, sh,  int32_t, shr16)
GEN_HELPER_PACKED_MULH(pmullh,     64, sh,  int32_t, and16)
GEN_HELPER_PACKED_MULH(pmulhuh,    64, uh, uint32_t, shr16)
GEN_HELPER_PACKED_MULH(pmulhrsh,   64, sh,  int32_t, shr14_add1_shr1)

GEN_HELPER_PACKED_MULH(qpmulhh,   128, sh,  int32_t, shr16)
GEN_HELPER_PACKED_MULH(qpmullh,   128, sh,  int32_t, and16)
GEN_HELPER_PACKED_MULH(qpmulhuh,  128, uh, uint32_t, shr16)
GEN_HELPER_PACKED_MULH(qpmulhrsh, 128, sh,  int32_t, shr14_add1_shr1)

GEN_HELPER_PACKED(pmulubhh, 64, uh, { \
    dst.uh[i] = ((int16_t) s1.ub[i] * s2.sh[i] + 0x80) >> 8; \
})

GEN_HELPER_PACKED(mpsadbh, 64, uh, { \
    dst.uh[i] = abs((int16_t) s1.ub[i    ] - s2.ub[0]) \
              + abs((int16_t) s1.ub[i + 1] - s2.ub[1]) \
              + abs((int16_t) s1.ub[i + 2] - s2.ub[2]) \
              + abs((int16_t) s1.ub[i + 3] - s2.ub[3]); \
})

#define mul_sign(a, b) ((b) < 0 ? -(a) : ((b) > 0 ? (a) : 0))

GEN_HELPER_PACKED_OP(psignb,   64, sb, mul_sign)
GEN_HELPER_PACKED_OP(psignh,   64, sh, mul_sign)
GEN_HELPER_PACKED_OP(psignw,   64, sw, mul_sign)

GEN_HELPER_PACKED_OP(qpsignb, 128, sb, mul_sign)
GEN_HELPER_PACKED_OP(qpsignh, 128, sh, mul_sign)
GEN_HELPER_PACKED_OP(qpsignw, 128, sw, mul_sign)

#define MOVMASK(len, mask_type, type) { \
    dst.mask_type[0] |= (s1.type[i] < 0) << (i + vec_count(len, type)); \
    dst.mask_type[0] |= (s2.type[i] < 0) << (i                       ); \
}

GEN_HELPER_PACKED(pmovmskb,  64, sb, MOVMASK(64, uh, sb))
GEN_HELPER_PACKED(pmovmskps, 64, sw, MOVMASK(64, ub, sw))
GEN_HELPER_PACKED(pmovmskpd, 64, sd, MOVMASK(64, ub, sd))

#define PACK(len, dst_type, type, op) { \
    dst.dst_type[i + vec_count(64, type)] = op(s1.type[i]); \
    dst.dst_type[i                      ] = op(s2.type[i]); \
}

GEN_HELPER_PACKED(packsshb, 64, sh, PACK(64, sb, sh, satsb))
GEN_HELPER_PACKED(packushb, 64, uh, PACK(64, ub, sh, satub))
GEN_HELPER_PACKED(packsswh, 64, sw, PACK(64, sh, sw, satsh))
GEN_HELPER_PACKED(packuswh, 64, sw, PACK(64, uh, sw, satuh))

#define GEN_HELPER_PACKED_QPACK(name, op) \
    vec_raw(128) HELPER(name)(vec_raw(128) src1, vec_raw(128) src2) \
    { \
        vec(128) dst, s1, s2; \
        \
        s1 = vec_from_raw(128, src1); \
        s2 = vec_from_raw(128, src2); \
        \
        dst.ud[1] = op(s1.ud[1], s1.ud[0]); \
        dst.ud[0] = op(s2.ud[1], s2.ud[0]); \
        \
        return vec_into_raw(128, dst); \
    }

GEN_HELPER_PACKED_QPACK(qpacksshb, helper_packsshb)
GEN_HELPER_PACKED_QPACK(qpacksswh, helper_packsswh)
GEN_HELPER_PACKED_QPACK(qpackushb, helper_packushb)
GEN_HELPER_PACKED_QPACK(qpackuswh, helper_packuswh)

#define GEN_HELPER_PACKED_UNPACK(name, len, type, offset) \
    GEN_HELPER_PACKED(name, len, type, { \
        int j = offset + i / 2; \
        dst.type[i] = i & 1 ? s1.type[j] : s2.type[j]; \
    })

GEN_HELPER_PACKED_UNPACK(punpcklbh, 64, ub, 0)
GEN_HELPER_PACKED_UNPACK(punpcklhw, 64, uh, 0)
GEN_HELPER_PACKED_UNPACK(punpcklwd, 64, uw, 0)
GEN_HELPER_PACKED_UNPACK(punpckhbh, 64, ub, 4)
GEN_HELPER_PACKED_UNPACK(punpckhhw, 64, uh, 2)
GEN_HELPER_PACKED_UNPACK(punpckhwd, 64, uw, 1)

vec_raw(64) HELPER(pshufb)(vec_raw(64) src1, vec_raw(64) src2, vec_raw(64) src3)
{
    vec(64) dst, s1, s2, s3;

    s1 = vec_from_raw(64, src1);
    s2 = vec_from_raw(64, src2);
    s3 = vec_from_raw(64, src3);

    for (int i = 0; i < vec_count(64, ub); i++) {
        uint8_t byte, desc = s3.ub[i];
        if (desc & 0x80) {
            switch (desc & 0x70) {
                case 0x20: byte = 0x7f; break;
                case 0x40: byte = 0x80; break;
                case 0x60: byte = 0xff; break;
                default: byte = 0; break;
            }
        } else {
            int index = desc & 7;
            byte = desc & 8 ? s1.ub[index] : s2.ub[index];
            byte = desc & 0x10 ? ~byte : byte;
            byte = desc & 0x20 ? revbit8(byte) : byte;
            byte = desc & 0x40 ? (byte & 0x80 ? 0xff : 0) : byte;
        }
        dst.ub[i] = byte;
    }

    return vec_into_raw(64, dst);
}

#define GEN_HELPER_PACKED_PMERGE(name, len) \
    vec_raw(len) HELPER(name)(vec_raw(len) src1, vec_raw(len) src2, vec_raw(len) src3) \
    { \
        vec(len) dst, s1, s2, s3; \
        \
        s1 = vec_from_raw(len, src1); \
        s2 = vec_from_raw(len, src2); \
        s3 = vec_from_raw(len, src3); \
        \
        for (int i = 0; i < vec_count(len, ub); i++) { \
            dst.ub[i] = s3.sb[i] < 0 ? s2.ub[i] : s1.ub[i]; \
        } \
        \
        return vec_into_raw(len, dst); \
    }

GEN_HELPER_PACKED_PMERGE(pmerge,   64)
GEN_HELPER_PACKED_PMERGE(qpmerge, 128)

vec_raw(64) HELPER(pshufh)(vec_raw(64) src1, uint32_t imm8)
{
    vec(64) dst, s1 = vec_from_raw(64, src1);
    for (int i = 0; i < vec_count(64, uh); i++) {
        int j = (imm8 >> (i * 2)) & 0x3;
        dst.uh[i] = s1.uh[j];
    }
    return vec_into_raw(64, dst);
}

vec_raw(64) HELPER(pshufw)(vec_raw(64) src1, vec_raw(64) src2, uint32_t imm8)
{
    vec(64) dst, s1, s2;

    s1 = vec_from_raw(64, src1);
    s2 = vec_from_raw(64, src2);

    for (int i = 0; i < vec_count(64, uw); i++) {
        int sel = (imm8 >> (i * 2)) & 0x3;
        int j = sel & 1;
        dst.uw[i] = sel > 1 ? s1.uw[j] : s2.uw[j];
    }

    return vec_into_raw(64, dst);
}

#define GEN_HELPER_PHMINPOS(name, len, type) \
    uint64_t HELPER(name)(vec_raw(len) src1, vec_raw(len) src2) \
    { \
        vec(len) s1, s2; \
        uint64_t v, p; \
        s1 = vec_from_raw(len, src1); \
        s2 = vec_from_raw(len, src2); \
        v = s1.type[0]; \
        p = 0; \
        for (int i = 1; i < vec_count(len, type); i++) { \
            if (s1.type[i] < v) { \
                v = s1.type[i]; \
                p = i; \
            } \
        } \
        for (int i = 0; i < vec_count(len, type); i++) { \
            if (s2.type[i] < v) { \
                v = s2.type[i]; \
                p = i + vec_count(len, type); \
            } \
        } \
        return (p << (sizeof(s1.type[0]) * 8)) | v; \
    }

GEN_HELPER_PHMINPOS(phminposuh,   64, uh)
GEN_HELPER_PHMINPOS(qphminposuh, 128, uh)

vec_raw(128) HELPER(qpmpsadbh)(vec_raw(128) src1, uint32_t s2)
{
    vec(128) dst, s1 = vec_from_raw(128, src1);
    dst.ud[0] = helper_mpsadbh(s1.ud[0], s2);
    dst.ud[1] = helper_mpsadbh(s1.ud[1], s2);
    return vec_into_raw(128, dst);
}

vec_raw(128) HELPER(qpmulubhh)(uint64_t s1, vec_raw(128) src2)
{
    vec(128) dst, s2 = vec_from_raw(128, src2);
    dst.ud[0] = helper_pmulubhh(s1      , s2.ud[0]);
    dst.ud[1] = helper_pmulubhh(s1 >> 32, s2.ud[1]);
    return vec_into_raw(128, dst);
}

uint32_t HELPER(qpsgn2mskb)(vec_raw(128) src2)
{
    vec(128) s2 = vec_from_raw(128, src2);
    uint32_t ret = 0;

    for (int i = 0; i < vec_count(128, sb); i++) {
        ret |= s2.sb[i] < 0 ? 1 << i : 0;
    }

    return ret;
}

vec_raw(128) HELPER(qpmsk2sgnb)(vec_raw(128) src1, uint32_t s2)
{
    vec(128) dst, s1 = vec_from_raw(128, src1);

    for (int i = 0; i < vec_count(128, ub); i++) {
        dst.ub[i] = s2 & (1 << i) ? s1.ub[i] | 0x80 : s1.ub[i] & 0x7f;
    }

    return vec_into_raw(128, dst);
}

vec_raw(128) HELPER(qppermb)(vec_raw(128) src1, vec_raw(128) src2, vec_raw(128) src3)
{
    vec(128) dst, s1, s2, s3;

    s1 = vec_from_raw(128, src1);
    s2 = vec_from_raw(128, src2);
    s3 = vec_from_raw(128, src3);

    for (int i = 0; i < vec_count(128, ub); i++) {
        uint8_t sel = s3.ub[i];
        int index = sel & 0x0f;
        uint8_t byte = sel & 0x10 ? s1.ub[index] : s2.ub[index];

        byte = sel & 0x20 ? ~byte : byte;
        byte = sel & 0x40 ? revbit8(byte) : byte;
        byte = sel & 0x80 ? (byte & 0x80 ? 0xff : 0) : byte;

        dst.ub[i] = byte;
    }

    return vec_into_raw(128, dst);
}

vec_raw(128) HELPER(qpshufb)(vec_raw(128) src1, vec_raw(128) src2, vec_raw(128) src3)
{
    vec(128) ret, s1, s2, s3;

    s1 = vec_from_raw(128, src1);
    s2 = vec_from_raw(128, src2);
    s3 = vec_from_raw(128, src3);

    ret.ud[0] = helper_pshufb(s2.ud[1], s2.ud[0], s3.ud[0]);
    ret.ud[1] = helper_pshufb(s1.ud[1], s1.ud[0], s3.ud[1]);

    return vec_into_raw(128, ret);
}

static uint64_t get_value_from_truth_table(bool x, bool y, bool z, uint32_t truth_table)
{
    int pos = x << 2 | y << 1 | z;
    return (truth_table >> pos) & 1ULL;
}

static uint64_t plog64(uint64_t src1, uint64_t src2, uint64_t src3, uint32_t table)
{
    uint64_t ret = 0;

    for (int i = 0; i < 64; i++) {
        bool x = extract64(src1, i, 1);
        bool y = extract64(src2, i, 1);
        bool z = extract64(src3, i, 1);
        ret |= get_value_from_truth_table(x, y, z, table) << i;
    }

    return ret;
}

#define GEN_HELPER_PACKED_PLOG(name, len) \
    vec_raw(len) HELPER(name)(vec_raw(len) src1, vec_raw(len) src2, vec_raw(len) src3, uint32_t table) \
    { \
        vec(len) dst, s1, s2, s3; \
        \
        s1 = vec_from_raw(len, src1); \
        s2 = vec_from_raw(len, src2); \
        s3 = vec_from_raw(len, src3); \
        \
        for (int i = 0; i < vec_count(len, ud); ++i) { \
            dst.ud[i] = plog64(s1.ud[i], s2.ud[i], s3.ud[i], table); \
        } \
        \
        return vec_into_raw(len, dst); \
    }

GEN_HELPER_PACKED_PLOG(plog,   64)
GEN_HELPER_PACKED_PLOG(qplog, 128)

#define GEN_ENV_HELPER_PACKED_N(name, len, n, code) \
    vec_raw(len) HELPER(name)(CPUE2KState *env, vec_raw(len) src1, vec_raw(len) src2) \
    { \
        vec(len) dst, s1, s2; \
        s1 = vec_from_raw(len, src1); \
        s2 = vec_from_raw(len, src2); \
        dst = vec_zero(len); \
        for (int i = 0; i < n; i++) { \
            code \
        } \
        return vec_into_raw(len, dst); \
    }

#define GEN_ENV_HELPER_PACKED(name, len, type, code) \
    GEN_ENV_HELPER_PACKED_N(name, len, vec_count(len, type), code)

#define GEN_ENV_HELPER_PACKED_OP(name, len, type, op) \
    GEN_ENV_HELPER_PACKED_N(name, len, vec_count(len, type), { \
        dst.type[i] = op(env, s1.type[i], s2.type[i]); \
    })

GEN_ENV_HELPER_PACKED_OP(pfadds,      64, uw, helper_fadds)
GEN_ENV_HELPER_PACKED_OP(pfsubs,      64, uw, helper_fsubs)
GEN_ENV_HELPER_PACKED_OP(pfmuls,      64, uw, helper_fmuls)
GEN_ENV_HELPER_PACKED_OP(pfmaxs,      64, uw, helper_fmaxs)
GEN_ENV_HELPER_PACKED_OP(pfmins,      64, uw, helper_fmins)

GEN_ENV_HELPER_PACKED_OP(qpfadds,    128, uw, helper_fadds)
GEN_ENV_HELPER_PACKED_OP(qpfsubs,    128, uw, helper_fsubs)
GEN_ENV_HELPER_PACKED_OP(qpfmuls,    128, uw, helper_fmuls)
GEN_ENV_HELPER_PACKED_OP(qpfmaxs,    128, uw, helper_fmaxs)
GEN_ENV_HELPER_PACKED_OP(qpfmins,    128, uw, helper_fmins)

GEN_ENV_HELPER_PACKED_OP(qpfaddd,    128, ud, helper_faddd)
GEN_ENV_HELPER_PACKED_OP(qpfsubd,    128, ud, helper_fsubd)
GEN_ENV_HELPER_PACKED_OP(qpfmuld,    128, ud, helper_fmuld)
GEN_ENV_HELPER_PACKED_OP(qpfmaxd,    128, ud, helper_fmaxd)
GEN_ENV_HELPER_PACKED_OP(qpfmind,    128, ud, helper_fmind)

GEN_ENV_HELPER_PACKED_OP(pfcmpeqs,    64, uw, helper_fcmpeqs)
GEN_ENV_HELPER_PACKED_OP(pfcmplts,    64, uw, helper_fcmplts)
GEN_ENV_HELPER_PACKED_OP(pfcmples,    64, uw, helper_fcmples)
GEN_ENV_HELPER_PACKED_OP(pfcmpuods,   64, uw, helper_fcmpuods)
GEN_ENV_HELPER_PACKED_OP(pfcmpneqs,   64, uw, helper_fcmpneqs)
GEN_ENV_HELPER_PACKED_OP(pfcmpnlts,   64, uw, helper_fcmpnlts)
GEN_ENV_HELPER_PACKED_OP(pfcmpnles,   64, uw, helper_fcmpnles)
GEN_ENV_HELPER_PACKED_OP(pfcmpods,    64, uw, helper_fcmpods)

GEN_ENV_HELPER_PACKED_OP(qpfcmpeqs,  128, uw, helper_fcmpeqs)
GEN_ENV_HELPER_PACKED_OP(qpfcmplts,  128, uw, helper_fcmplts)
GEN_ENV_HELPER_PACKED_OP(qpfcmples,  128, uw, helper_fcmples)
GEN_ENV_HELPER_PACKED_OP(qpfcmpuods, 128, uw, helper_fcmpuods)
GEN_ENV_HELPER_PACKED_OP(qpfcmpneqs, 128, uw, helper_fcmpneqs)
GEN_ENV_HELPER_PACKED_OP(qpfcmpnlts, 128, uw, helper_fcmpnlts)
GEN_ENV_HELPER_PACKED_OP(qpfcmpnles, 128, uw, helper_fcmpnles)
GEN_ENV_HELPER_PACKED_OP(qpfcmpods,  128, uw, helper_fcmpods)

GEN_ENV_HELPER_PACKED_OP(qpfcmpeqd,  128, ud, helper_fcmpeqd)
GEN_ENV_HELPER_PACKED_OP(qpfcmpltd,  128, ud, helper_fcmpltd)
GEN_ENV_HELPER_PACKED_OP(qpfcmpled,  128, ud, helper_fcmpled)
GEN_ENV_HELPER_PACKED_OP(qpfcmpuodd, 128, ud, helper_fcmpuodd)
GEN_ENV_HELPER_PACKED_OP(qpfcmpneqd, 128, ud, helper_fcmpneqd)
GEN_ENV_HELPER_PACKED_OP(qpfcmpnltd, 128, ud, helper_fcmpnltd)
GEN_ENV_HELPER_PACKED_OP(qpfcmpnled, 128, ud, helper_fcmpnled)
GEN_ENV_HELPER_PACKED_OP(qpfcmpodd,  128, ud, helper_fcmpodd)

#define GEN_ENV_HELPER_PACKED_OP_HORIZONTAL(name, len, type, op, map) \
    GEN_ENV_HELPER_PACKED_N(name, len, vec_count(len, type) / 2, { \
        int j = i * 2; \
        dst.type[i                                ] = map(op(env, s1.type[j], s1.type[j + 1])); \
        dst.type[i + vec_count(len, type) / 2] = map(op(env, s2.type[j], s2.type[j + 1])); \
    })

GEN_ENV_HELPER_PACKED_OP_HORIZONTAL(pfhadds,    64, uw, helper_fadds, ident)
GEN_ENV_HELPER_PACKED_OP_HORIZONTAL(pfhsubs,    64, uw, helper_fsubs, ident)
GEN_ENV_HELPER_PACKED_OP_HORIZONTAL(qpfhadds,  128, uw, helper_fadds, ident)
GEN_ENV_HELPER_PACKED_OP_HORIZONTAL(qpfhsubs,  128, uw, helper_fsubs, ident)

#define GEN_ENV_HELPER_PACKED_OP_ALT(name, len, type, op1, op2) \
    GEN_ENV_HELPER_PACKED(name, len, type, { \
        if (i & 1) { \
            dst.type[i] = op2(env, s1.type[i], s2.type[i]); \
        } else { \
            dst.type[i] = op1(env, s1.type[i], s2.type[i]); \
        } \
    })

GEN_ENV_HELPER_PACKED_OP_ALT(pfaddsubs,   64, uw, helper_fsubs, helper_fadds)
GEN_ENV_HELPER_PACKED_OP_ALT(qpfaddsubs, 128, uw, helper_fsubs, helper_fadds)
GEN_ENV_HELPER_PACKED_OP_ALT(qpfaddsubd, 128, ud, helper_fsubd, helper_faddd)

#define GEN_ENV_HELPER_PACKED_CONVERT(name, len_from, len_to, from, to, op) \
    vec_raw(len_to) HELPER(name)(CPUE2KState *env, vec_raw(len_from) src2) \
    { \
        vec(len_to) dst = vec_zero(len_to); \
        vec(len_from) s2 = vec_from_raw(len_from, src2); \
        \
        for (int i = 0; i < vec_count(len_from, from); ++i) { \
            dst.to[i] = op(env, s2.from[i]); \
        } \
        return vec_into_raw(len_to, dst); \
    }

GEN_ENV_HELPER_PACKED_CONVERT(pistofs,     64,  64, uw, uw, helper_istofs)
GEN_ENV_HELPER_PACKED_CONVERT(pfstois,     64,  64, uw, uw, helper_fstois)
GEN_ENV_HELPER_PACKED_CONVERT(pfstoistr,   64,  64, uw, uw, helper_fstoistr)

GEN_ENV_HELPER_PACKED_CONVERT(qpistofs,   128, 128, uw, uw, helper_istofs)
GEN_ENV_HELPER_PACKED_CONVERT(qpfstois,   128, 128, uw, uw, helper_fstois)
GEN_ENV_HELPER_PACKED_CONVERT(qpfstoistr, 128, 128, uw, uw, helper_fstoistr)

GEN_ENV_HELPER_PACKED_CONVERT(qpidtofd,   128, 128, ud, ud, helper_idtofd)
GEN_ENV_HELPER_PACKED_CONVERT(qpfdtoid,   128, 128, ud, ud, helper_fdtoid)
GEN_ENV_HELPER_PACKED_CONVERT(qpfdtoidtr, 128, 128, ud, ud, helper_fdtoidtr)

GEN_ENV_HELPER_PACKED_CONVERT(qpfstoid,    64, 128, uw, ud, helper_fstoid)
GEN_ENV_HELPER_PACKED_CONVERT(qpfstoidtr,  64, 128, uw, ud, helper_fstoidtr)
GEN_ENV_HELPER_PACKED_CONVERT(qpistofd,    64, 128, uw, ud, helper_istofd)
GEN_ENV_HELPER_PACKED_CONVERT(qpfstofd,    64, 128, uw, ud, helper_fstofd)

GEN_ENV_HELPER_PACKED_CONVERT(qpfdtois,   128,  64, ud, uw, helper_fdtois)
GEN_ENV_HELPER_PACKED_CONVERT(qpfdtoistr, 128,  64, ud, uw, helper_fdtoistr)
GEN_ENV_HELPER_PACKED_CONVERT(qpidtofs,   128,  64, ud, uw, helper_idtofs)
GEN_ENV_HELPER_PACKED_CONVERT(qpfdtofs,   128,  64, ud, uw, helper_fdtofs)

#define GEN_ENV_HELPER_PACKED_CONVERT2(name, len_from, len_to, from, to, op) \
    vec_raw(len_to) HELPER(name)(CPUE2KState *env, uint64_t src1, vec_raw(len_from) src2) \
    { \
        vec(len_to) dst = vec_zero(len_to); \
        vec(len_from) s2 = vec_from_raw(len_from, src2); \
        \
        for (int i = 0; i < vec_count(len_from, from); ++i) { \
            dst.to[i] = op(env, src1, s2.from[i]); \
        } \
        return vec_into_raw(len_to, dst); \
    }

GEN_ENV_HELPER_PACKED_CONVERT2(pfstoifs,    64,  64, uw, uw, helper_fstoifs)

GEN_ENV_HELPER_PACKED_CONVERT2(qpfstoifs,  128, 128, uw, uw, helper_fstoifs)
GEN_ENV_HELPER_PACKED_CONVERT2(qpfdtoifd,  128, 128, ud, ud, helper_fdtoifd)

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
    MemOpIdx oi = make_memop_idx(memop | MO_LE | MO_UO, e2k_env_mmu_index(env, false));

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
#if defined(CONFIG_ATOMIC128) || HAVE_CMPXCHG128
    Int128 t;
    MemOpIdx oi = make_memop_idx(MO_ALIGN | MO_LE | MO_UO, e2k_env_mmu_index(env, false));

    if (mask & 0xffff && (!sm || e2k_probe_rw_access(env, addr, 16))) {
        t = cpu_ld16_mmu(env, addr, oi, GETPC());
        t = qmerge(value, t, mask);
        t = cpu_atomic_cmpxchgo_le_mmu(env, addr, last_val, t, oi, GETPC());
        return int128_ne(t, last_val);
    } else {
        return 0;
    }
#else
    // TODO: e2k mlock without cmpxchg128
    g_assert(0 && "implement stmqp mlock");
    return 0;
#endif
}
