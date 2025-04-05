#include "test-e2k.h"

static uint64_t test_data_bits[] = {
    0x0000000012345678, 0x0000000000012341, 0xffffffffffffffff, 0x7fffffffffffffff,
    0x8000000000000000, 0xaaaaaaaa12345678, 0xaaaaaaaa00012341, 0xaaaaaaaaffffffff,
    0xaaaaaaaa7fffffff, 0xaaaaaaaa80000000, 0x123456789abcdef0,
};

#define TEST1_BIT(CHECK, EXEC, INSN) \
    TEST1_DATA(CHECK, EXEC, INSN, test_data_bits, glue(INSN, _expect))

static uint64_t test_data_bin_src1[] = {
    0x0000000012345678, 0x0000000000012341, 0x0000000000012341, 0xffffffffffffffff,
    0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0x7fffffffffffffff,
    0x7fffffffffffffff, 0x7fffffffffffffff, 0x8000000000000000, 0x8000000000000000,
    0x8000000000000000, 0xaaaaaaaa12345678, 0xaaaaaaaa00012341, 0xaaaaaaaa00012341,
    0xaaaaaaaaffffffff, 0xaaaaaaaaffffffff, 0xaaaaaaaaffffffff, 0xaaaaaaaaffffffff,
    0xaaaaaaaa7fffffff, 0xaaaaaaaa7fffffff, 0xaaaaaaaa7fffffff, 0xaaaaaaaa80000000,
    0xaaaaaaaa80000000, 0xaaaaaaaa80000000, 0x123456789abcdef0, 0x123456789abcdef0,
};

static uint64_t test_data_bin_src2[] = {
    0x000000000812fada, 0x0000000000012341, 0xfffffffffffedcbf, 0x0000000000000000,
    0xffffffffffffffff, 0x0000000000000001, 0x0000000000000002, 0x0000000000000000,
    0x0000000000000001, 0xffffffffffffffff, 0xffffffffffffffff, 0x0000000000000001,
    0xfffffffffffffffe, 0xeeeeeeee0812fada, 0xeeeeeeee00012341, 0xeeeeeeeefffedcbf,
    0xeeeeeeee00000000, 0xeeeeeeeeffffffff, 0xeeeeeeee00000001, 0xeeeeeeee00000002,
    0xeeeeeeee00000000, 0xeeeeeeee00000001, 0xeeeeeeeeffffffff, 0xeeeeeeeeffffffff,
    0xeeeeeeee00000001, 0xeeeeeeeefffffffe, 0x123456789abcdef0, 0xfedcba9876543210,
};

#define TEST2_BIN(CHECK, EXEC, INSN) \
    TEST2_DATA(CHECK, EXEC, INSN, test_data_bin_src1, test_data_bin_src2, glue(INSN, _expect))

EXPECT_DATA(bitrevs, uint32_t) {
    0x1e6a2c48, 0x82c48000, 0xffffffff, 0xffffffff, 0x00000000, 0x1e6a2c48,
    0x82c48000, 0xffffffff, 0xfffffffe, 0x00000001, 0x0f7b3d59,
};
EXPECT_DATA(bitrevd, uint64_t) {
    0x1e6a2c4800000000, 0x82c4800000000000, 0xffffffffffffffff, 0xfffffffffffffffe,
    0x0000000000000001, 0x1e6a2c4855555555, 0x82c4800055555555, 0xffffffff55555555,
    0xfffffffe55555555, 0x0000000155555555, 0x0f7b3d591e6a2c48,
};
EXPECT_DATA(lzcnts, uint32_t) {
    0x00000003, 0x0000000f, 0x00000000, 0x00000000, 0x00000020, 0x00000003,
    0x0000000f, 0x00000000, 0x00000001, 0x00000000, 0x00000000,
};
EXPECT_DATA(lzcntd, uint64_t) {
    0x0000000000000023, 0x000000000000002f, 0x0000000000000000, 0x0000000000000001,
    0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
    0x0000000000000000, 0x0000000000000000, 0x0000000000000003,
};
EXPECT_DATA(popcnts, uint32_t) {
    0x0000000d, 0x00000006, 0x00000020, 0x00000020, 0x00000000, 0x0000000d,
    0x00000006, 0x00000020, 0x0000001f, 0x00000001, 0x00000013,
};
EXPECT_DATA(popcntd, uint64_t) {
    0x000000000000000d, 0x0000000000000006, 0x0000000000000040, 0x000000000000003f,
    0x0000000000000001, 0x000000000000001d, 0x0000000000000016, 0x0000000000000030,
    0x000000000000002f, 0x0000000000000011, 0x0000000000000020,
};
EXPECT_DATA(pmulubhh, uint64_t) {
    0x0000000002b6fd96, 0x00000000000008f4, 0x000000000000f70c, 0x0000000000000000,
    0xffffffffffffffff, 0x0000000000000001, 0x0000000000000002, 0x0000000000000000,
    0x0000000000000001, 0xffffffffffffffff, 0x0000000000000000, 0x0000000000000000,
    0x0000000000000000, 0xfecdfc8802b6fd96, 0x0000ffef000008f4, 0x0000ffef0000f70c,
    0xeeffeeff00000000, 0xeeffeeffffffffff, 0xeeffeeff00000001, 0xeeffeeff00000002,
    0xf788eeff00000000, 0xf788eeff00000001, 0xf788eeffffffffff, 0xf777000000000000,
    0xf777000000000000, 0xf777000000000000, 0x0af33f80a82fe101, 0xff50cd08669d2eef,
};

int main(int argc, char *argv[]) {
    TEST1_BIT(CHECK1_0134, EXEC_R, bitrevs);
    TEST1_BIT(CHECK1_0134, EXEC_R, bitrevd);

    TEST1_BIT(CHECK1_14, EXEC_R, lzcnts);
    TEST1_BIT(CHECK1_14, EXEC_R, lzcntd);
    TEST1_BIT(CHECK1_14, EXEC_R, popcnts);
    TEST1_BIT(CHECK1_14, EXEC_R, popcntd);

    TEST2_BIN(CHECK2_14, EXEC_RR, pmulubhh);

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
