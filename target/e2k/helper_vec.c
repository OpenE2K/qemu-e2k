#include "qemu/osdep.h"
#include "qemu/log.h"
#include "cpu.h"
#include "exec/exec-all.h"
#include "qemu/host-utils.h"
#include "exec/helper-proto.h"
#include "translate.h"

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

uint64_t HELPER(packed_shuffle_i64)(uint64_t src1, uint64_t src2, uint64_t src3)
{
    vec64 ret, s1, s2, s3;
    unsigned int i;

    s1.ud[0] = src1;
    s2.ud[0] = src2;
    s3.ud[0] = src3;

    for (i = 0; i < 8; i++) {
        uint8_t desc = s3.ub[i];
        int index = extract8(desc, 0, 3);
        uint8_t byte;

        if (desc < 0x80) {
            if (desc & 0x08) {
                byte = s1.ub[index];
            } else {
                byte = s2.ub[index];
            }

            switch(desc >> 5) {
            case 0x1:
                byte = reverse_bits(byte);
                break;
            case 0x2:
                if ((byte & 0x80) != 0) {
                    byte = 0xff;
                } else {
                    byte = 0;
                }
                break;
            case 0x3:
                if ((byte & 1) != 0) {
                    byte = 0xff;
                } else {
                    byte = 0;
                }
                break;
            default:
                break;
            }

            if (desc & 0x10) {
                byte = ~byte;
            }
        } else {
            switch(desc >> 6) {
            case 0xa:
                byte = 0x7f;
                break;
            case 0xc:
                byte = 0x80;
                break;
            case 0xe:
                byte = 0xff;
                break;
            default:
                byte = 0;
                break;
            }
        }

        ret.ub[i] = byte;
    }

    return ret.ud[0];
}

#define GEN_HELPER_PACKED(name, type, code) \
    uint64_t HELPER(name)(uint64_t src1, uint64_t src2) \
    { \
        size_t i = 0; \
        vec64 s1 = { .ud[0] = src1 }, s2 = { .ud[0] = src2 }, dst; \
        for (; i < glue(vec64_, type); i++) { \
            code \
        } \
        return dst.ud[0]; \
    }
#define GEN_HELPER_PACKED_SCALAR(name, type, code) \
    uint64_t HELPER(name)(uint64_t src1, uint64_t s2) \
    { \
        size_t i = 0; \
        vec64 s1 = { .ud[0] = src1 }, dst; \
        for (; i < glue(vec64_, type); i++) { \
            code \
        } \
        return dst.ud[0]; \
    }
#define GEN_HELPER_PACKED_MINMAX(name, type, op) \
    GEN_HELPER_PACKED(glue(name, type), type, { \
        dst.type[i] = op(s1.type[i], s2.type[i]); \
    })

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
        dst.type[i] = s1.type[i] op s2.type[i] ? -1UL : 0; \
    })

GEN_HELPER_PACKED_CMP(pcmpeqb, ub, ==)
GEN_HELPER_PACKED_CMP(pcmpeqh, uh, ==)
GEN_HELPER_PACKED_CMP(pcmpeqw, uw, ==)
GEN_HELPER_PACKED_CMP(pcmpeqd, ud, ==)

GEN_HELPER_PACKED_CMP(pcmpgtb, sb, >)
GEN_HELPER_PACKED_CMP(pcmpgth, sh, >)
GEN_HELPER_PACKED_CMP(pcmpgtw, sw, >)
GEN_HELPER_PACKED_CMP(pcmpgtd, sd, >)

#define GEN_HELPER_PACKED_BINOP_SATURATE(name, type, op, t, min, max) \
    GEN_HELPER_PACKED(name, type, { \
        dst.type[i] = MIN(MAX(s1.type[i] op s2.type[i], min), max); \
    })

GEN_HELPER_PACKED_BINOP_SATURATE(paddsb, sb, +, int, -128, 127)
GEN_HELPER_PACKED_BINOP_SATURATE(paddsh, sh, +, int, -32768, 32767)
GEN_HELPER_PACKED_BINOP_SATURATE(paddusb, ub, +, int, 0, 255)
GEN_HELPER_PACKED_BINOP_SATURATE(paddush, uh, +, int, 0, 65535)

GEN_HELPER_PACKED_BINOP_SATURATE(psubsb, sb, -, int, -128, 127)
GEN_HELPER_PACKED_BINOP_SATURATE(psubsh, sh, -, int, -32768, 32767)
GEN_HELPER_PACKED_BINOP_SATURATE(psubusb, ub, -, int, 0, 255)
GEN_HELPER_PACKED_BINOP_SATURATE(psubush, uh, -, int, 0, 65535)

#define GEN_HELPER_PACKED_SCALAR_BINOP(name, type, op) \
    GEN_HELPER_PACKED_SCALAR(name, type, { \
        dst.type[i] = s1.type[i] op s2; \
    })

GEN_HELPER_PACKED_SCALAR_BINOP(psllh, uh, <<)
GEN_HELPER_PACKED_SCALAR_BINOP(psllw, uw, <<)
GEN_HELPER_PACKED_SCALAR_BINOP(psrlh, uh, >>)
GEN_HELPER_PACKED_SCALAR_BINOP(psrlw, uw, >>)

#define GEN_HELPER_PACKED_SRA(name, type, t) \
    GEN_HELPER_PACKED_SCALAR(name, type, { \
        dst.type[i] = (t) s1.type[i] >> s2; \
    })

GEN_HELPER_PACKED_SRA(psrah, sh, int32_t)
GEN_HELPER_PACKED_SRA(psraw, sw, int64_t)

uint64_t HELPER(pmovmskb)(uint64_t src1, uint64_t src2)
{
    unsigned int i;
    vec64 s1, s2;
    uint16_t ret = 0;

    s1.ud[0] = src1;
    s2.ud[0] = src2;

    for (i = 0; i < 8; i++) {
        if (s1.sb[i] < 0) {
            ret |= 1 << (i + 8);
        }
        if (s2.sb[i] < 0) {
            ret |= 1 << i;
        }
    }

    return ret;
}
