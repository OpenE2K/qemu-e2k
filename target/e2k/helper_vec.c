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
    uint64_t n;
    uint8_t s[8];
} u8x8;

uint64_t HELPER(packed_shuffle_i64)(uint64_t src1, uint64_t src2, uint64_t src3)
{
    u8x8 ret, s1, s2, s3;
    unsigned int i;

    s1.n = src1;
    s2.n = src2;
    s3.n = src3;

    for (i = 0; i < 8; i++) {
        uint8_t desc = s3.s[i];
        int index = extract8(desc, 0, 3);
        uint8_t byte;

        if (desc < 0x80) {
            if (desc & 0x08) {
                byte = s1.s[index];
            } else {
                byte = s2.s[index];
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

        ret.s[i] = byte;
    }

    return ret.n;
}
