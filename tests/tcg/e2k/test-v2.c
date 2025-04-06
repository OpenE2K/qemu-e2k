#include "test.h"

static uint64_t int_src1[] = {
    0x0000000012345678, 0x0000000000012341, 0x0000000000012341, 0xffffffffffffffff,
    0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0x7fffffffffffffff,
    0x7fffffffffffffff, 0x7fffffffffffffff, 0x8000000000000000, 0x8000000000000000,
    0x8000000000000000, 0xaaaaaaaa12345678, 0xaaaaaaaa00012341, 0xaaaaaaaa00012341,
    0xaaaaaaaaffffffff, 0xaaaaaaaaffffffff, 0xaaaaaaaaffffffff, 0xaaaaaaaaffffffff,
    0xaaaaaaaa7fffffff, 0xaaaaaaaa7fffffff, 0xaaaaaaaa7fffffff, 0xaaaaaaaa80000000,
    0xaaaaaaaa80000000, 0xaaaaaaaa80000000, 0x123456789abcdef0, 0x123456789abcdef0,
};

static uint64_t int_src2[] = {
    0x000000000812fada, 0x0000000000012341, 0xfffffffffffedcbf, 0x0000000000000000,
    0xffffffffffffffff, 0x0000000000000001, 0x0000000000000002, 0x0000000000000000,
    0x0000000000000001, 0xffffffffffffffff, 0xffffffffffffffff, 0x0000000000000001,
    0xfffffffffffffffe, 0xeeeeeeee0812fada, 0xeeeeeeee00012341, 0xeeeeeeeefffedcbf,
    0xeeeeeeee00000000, 0xeeeeeeeeffffffff, 0xeeeeeeee00000001, 0xeeeeeeee00000002,
    0xeeeeeeee00000000, 0xeeeeeeee00000001, 0xeeeeeeeeffffffff, 0xeeeeeeeeffffffff,
    0xeeeeeeee00000001, 0xeeeeeeeefffffffe, 0x123456789abcdef0, 0xfedcba9876543210,
};

static uint64_t bit_src1[] = {
    0x0000000012345678, 0x0000000000012341, 0xffffffffffffffff, 0x7fffffffffffffff,
    0x8000000000000000, 0xaaaaaaaa12345678, 0xaaaaaaaa00012341, 0xaaaaaaaaffffffff,
    0xaaaaaaaa7fffffff, 0xaaaaaaaa80000000, 0x123456789abcdef0,
};

#include "test-v2-expect.inc"

int main(int argc, char *argv[]) {
    parse_args(argc, argv);

    CHECK1(EXEC_R, 0134, bitrevs, bit_src1);
    CHECK1(EXEC_R, 0134, bitrevd, bit_src1);

    CHECK1(EXEC_R, 14, lzcnts, bit_src1);
    CHECK1(EXEC_R, 14, lzcntd, bit_src1);
    CHECK1(EXEC_R, 14, popcnts, bit_src1);
    CHECK1(EXEC_R, 14, popcntd, bit_src1);

    CHECK2(EXEC_RR, 14, pmulubhh, int_src1, int_src2);

    // TODO: pshufb
    // TODO: pmerge

    // TODO: fdtoidtr
    // TODO: fxtoidtr
    // TODO: fstoidtr
    // TODO: fxtoistr

    // TODO: movx
    // TODO: movxa
    // TODO: movxc

    // new channels
    // TODO: movfi
    // TODO: movif

    // TODO: fadd_adds
    // TODO: fadd_addd
    // TODO: fsub_adds
    // TODO: fsub_addd
    // TODO: fadd_subs
    // TODO: fadd_subd
    // TODO: fsub_subs
    // TODO: fsub_subd
    // TODO: fadd_rsubs
    // TODO: fadd_rsubd
    // TODO: fsub_rsubs
    // TODO: fsub_rsubd
    // TODO: pfadd_adds
    // TODO: pfadd_addd
    // TODO: pfsub_adds
    // TODO: pfsub_addd
    // TODO: pfadd_subs
    // TODO: pfadd_subd
    // TODO: pfsub_subs
    // TODO: pfsub_subd
    // TODO: pfadd_rsubs
    // TODO: pfadd_rsubd
    // TODO: pfsub_rsubs
    // TODO: pfsub_rsubd

    return is_failed();
}
