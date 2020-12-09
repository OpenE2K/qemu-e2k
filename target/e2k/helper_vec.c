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

typedef union {
    uint8_t ub[8];
    uint16_t uh[4];
    uint32_t uw[2];
    uint64_t uq;
    int8_t sb[8];
    int16_t sh[4];
    int32_t sw[2];
    int64_t sq;
} vec64;

uint64_t HELPER(packed_shuffle_i64)(uint64_t src1, uint64_t src2, uint64_t src3)
{
    vec64 ret, s1, s2, s3;
    unsigned int i;

    s1.uq = src1;
    s2.uq = src2;
    s3.uq = src3;

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

    return ret.uq;
}

// FIXME: not tested
uint64_t HELPER(pcmpeqb)(uint64_t src1, uint64_t src2)
{
    unsigned int i;
    vec64 s1, s2, ret;

    s1.uq = src1;
    s2.uq = src2;

    for (i = 0; i < 8; i++) {
        if (s1.ub[i] == s2.ub[i]) {
            ret.ub[i] = 0xff;
        } else {
            ret.ub[i] = 0;
        }
    }

    return ret.uq;
}

uint64_t HELPER(pminub)(uint64_t src1, uint64_t src2)
{
    unsigned int i;
    vec64 s1, s2, ret;

    s1.uq = src1;
    s2.uq = src2;

    for (i = 0; i < 8; i++) {
        ret.ub[i] = MIN(s1.ub[i], s2.ub[i]);
    }

    return ret.uq;
}

uint64_t HELPER(pminsh)(uint64_t src1, uint64_t src2)
{
    unsigned int i;
    vec64 s1, s2, ret;

    s1.uq = src1;
    s2.uq = src2;

    for (i = 0; i < 4; i++) {
        ret.sh[i] = MIN(s1.sh[i], s2.sh[i]);
    }

    return ret.uq;
}

uint64_t HELPER(pmaxub)(uint64_t src1, uint64_t src2)
{
    unsigned int i;
    vec64 s1, s2, ret;

    s1.uq = src1;
    s2.uq = src2;

    for (i = 0; i < 8; i++) {
        ret.ub[i] = MAX(s1.ub[i], s2.ub[i]);
    }

    return ret.uq;
}

uint64_t HELPER(pmaxsh)(uint64_t src1, uint64_t src2)
{
    unsigned int i;
    vec64 s1, s2, ret;

    s1.uq = src1;
    s2.uq = src2;

    for (i = 0; i < 4; i++) {
        ret.sh[i] = MAX(s1.sh[i], s2.sh[i]);
    }

    return ret.uq;
}
