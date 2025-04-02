#include "test-e2k.h"
#include <stdint.h>

#define CHECK2_ALL(EXEC, INSN, S1, S2, EXPECT) \
    CHECK2(EXEC, INSN, 0, S1, S2, EXPECT); \
    CHECK2(EXEC, INSN, 1, S1, S2, EXPECT); \
    CHECK2(EXEC, INSN, 2, S1, S2, EXPECT); \
    CHECK2(EXEC, INSN, 3, S1, S2, EXPECT); \
    CHECK2(EXEC, INSN, 4, S1, S2, EXPECT); \
    CHECK2(EXEC, INSN, 5, S1, S2, EXPECT)

#define TEST2_DEFAULT_DATA(CHANNELS, EXEC, INSN) \
    GROUP(#INSN); \
    for (int i = 0; i < ARRAY_SIZE(test_data); ++i) { \
        CHANNELS(EXEC, INSN, test_data[i].src1, test_data[i].src2, glue(INSN, _expect)[i]); \
    }

static struct e2k_test_data test_data[] = {
    { 0x0000000012345678, 0x000000000812fada, 0 },
    { 0x0000000000012341, 0x0000000000012341, 0 },
    { 0x0000000000012341, 0xfffffffffffedcbf, 0 },
    { 0xffffffffffffffff, 0x0000000000000000, 0 },
    { 0xffffffffffffffff, 0xffffffffffffffff, 0 },
    { 0xffffffffffffffff, 0x0000000000000001, 0 },
    { 0xffffffffffffffff, 0x0000000000000002, 0 },
    { 0x7fffffffffffffff, 0x0000000000000000, 0 },
    { 0x7fffffffffffffff, 0x0000000000000001, 0 },
    { 0x7fffffffffffffff, 0xffffffffffffffff, 0 },
    { 0x8000000000000000, 0xffffffffffffffff, 0 },
    { 0x8000000000000000, 0x0000000000000001, 0 },
    { 0x8000000000000000, 0xfffffffffffffffe, 0 },
    { 0xaaaaaaaa12345678, 0xeeeeeeee0812fada, 0 },
    { 0xaaaaaaaa00012341, 0xeeeeeeee00012341, 0 },
    { 0xaaaaaaaa00012341, 0xeeeeeeeefffedcbf, 0 },
    { 0xaaaaaaaaffffffff, 0xeeeeeeee00000000, 0 },
    { 0xaaaaaaaaffffffff, 0xeeeeeeeeffffffff, 0 },
    { 0xaaaaaaaaffffffff, 0xeeeeeeee00000001, 0 },
    { 0xaaaaaaaaffffffff, 0xeeeeeeee00000002, 0 },
    { 0xaaaaaaaa7fffffff, 0xeeeeeeee00000000, 0 },
    { 0xaaaaaaaa7fffffff, 0xeeeeeeee00000001, 0 },
    { 0xaaaaaaaa7fffffff, 0xeeeeeeeeffffffff, 0 },
    { 0xaaaaaaaa80000000, 0xeeeeeeeeffffffff, 0 },
    { 0xaaaaaaaa80000000, 0xeeeeeeee00000001, 0 },
    { 0xaaaaaaaa80000000, 0xeeeeeeeefffffffe, 0 },
};

static uint32_t ands_expect[] = {
    0x00105258,
    0x00012341,
    0x00000001,
    0x00000000,
    0xffffffff,
    0x00000001,
    0x00000002,
    0x00000000,
    0x00000001,
    0xffffffff,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00105258,
    0x00012341,
    0x00000001,
    0x00000000,
    0xffffffff,
    0x00000001,
    0x00000002,
    0x00000000,
    0x00000001,
    0x7fffffff,
    0x80000000,
    0x00000000,
    0x80000000,
};

static uint32_t andns_expect[] = {
    0x12240420,
    0x00000000,
    0x00012340,
    0xffffffff,
    0x00000000,
    0xfffffffe,
    0xfffffffd,
    0xffffffff,
    0xfffffffe,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x12240420,
    0x00000000,
    0x00012340,
    0xffffffff,
    0x00000000,
    0xfffffffe,
    0xfffffffd,
    0x7fffffff,
    0x7ffffffe,
    0x00000000,
    0x00000000,
    0x80000000,
    0x00000000,
};

static uint32_t ors_expect[] = {
    0x1a36fefa,
    0x00012341,
    0xffffffff,
    0xffffffff,
    0xffffffff,
    0xffffffff,
    0xffffffff,
    0xffffffff,
    0xffffffff,
    0xffffffff,
    0xffffffff,
    0x00000001,
    0xfffffffe,
    0x1a36fefa,
    0x00012341,
    0xffffffff,
    0xffffffff,
    0xffffffff,
    0xffffffff,
    0xffffffff,
    0x7fffffff,
    0x7fffffff,
    0xffffffff,
    0xffffffff,
    0x80000001,
    0xfffffffe,
};

static uint32_t orns_expect[] = {
    0xf7fd577d,
    0xffffffff,
    0x00012341,
    0xffffffff,
    0xffffffff,
    0xffffffff,
    0xffffffff,
    0xffffffff,
    0xffffffff,
    0xffffffff,
    0x00000000,
    0xfffffffe,
    0x00000001,
    0xf7fd577d,
    0xffffffff,
    0x00012341,
    0xffffffff,
    0xffffffff,
    0xffffffff,
    0xffffffff,
    0xffffffff,
    0xffffffff,
    0x7fffffff,
    0x80000000,
    0xfffffffe,
    0x80000001,
};

static uint32_t xors_expect[] = {
    0x1a26aca2,
    0x00000000,
    0xfffffffe,
    0xffffffff,
    0x00000000,
    0xfffffffe,
    0xfffffffd,
    0xffffffff,
    0xfffffffe,
    0x00000000,
    0xffffffff,
    0x00000001,
    0xfffffffe,
    0x1a26aca2,
    0x00000000,
    0xfffffffe,
    0xffffffff,
    0x00000000,
    0xfffffffe,
    0xfffffffd,
    0x7fffffff,
    0x7ffffffe,
    0x80000000,
    0x7fffffff,
    0x80000001,
    0x7ffffffe,
};

static uint32_t xorns_expect[] = {
    0xe5d9535d,
    0xffffffff,
    0x00000001,
    0x00000000,
    0xffffffff,
    0x00000001,
    0x00000002,
    0x00000000,
    0x00000001,
    0xffffffff,
    0x00000000,
    0xfffffffe,
    0x00000001,
    0xe5d9535d,
    0xffffffff,
    0x00000001,
    0x00000000,
    0xffffffff,
    0x00000001,
    0x00000002,
    0x80000000,
    0x80000001,
    0x7fffffff,
    0x80000000,
    0x7ffffffe,
    0x80000001,
};

static uint32_t adds_expect[] = {
    0x1a475152,
    0x00024682,
    0x00000000,
    0xffffffff,
    0xfffffffe,
    0x00000000,
    0x00000001,
    0xffffffff,
    0x00000000,
    0xfffffffe,
    0xffffffff,
    0x00000001,
    0xfffffffe,
    0x1a475152,
    0x00024682,
    0x00000000,
    0xffffffff,
    0xfffffffe,
    0x00000000,
    0x00000001,
    0x7fffffff,
    0x80000000,
    0x7ffffffe,
    0x7fffffff,
    0x80000001,
    0x7ffffffe,
};

static uint32_t subs_expect[] = {
    0x0a215b9e,
    0x00000000,
    0x00024682,
    0xffffffff,
    0x00000000,
    0xfffffffe,
    0xfffffffd,
    0xffffffff,
    0xfffffffe,
    0x00000000,
    0x00000001,
    0xffffffff,
    0x00000002,
    0x0a215b9e,
    0x00000000,
    0x00024682,
    0xffffffff,
    0x00000000,
    0xfffffffe,
    0xfffffffd,
    0x7fffffff,
    0x7ffffffe,
    0x80000000,
    0x80000001,
    0x7fffffff,
    0x80000002,
};

static uint64_t andd_expect[] = {
    0x0000000000105258,
    0x0000000000012341,
    0x0000000000000001,
    0x0000000000000000,
    0xffffffffffffffff,
    0x0000000000000001,
    0x0000000000000002,
    0x0000000000000000,
    0x0000000000000001,
    0x7fffffffffffffff,
    0x8000000000000000,
    0x0000000000000000,
    0x8000000000000000,
    0xaaaaaaaa00105258,
    0xaaaaaaaa00012341,
    0xaaaaaaaa00000001,
    0xaaaaaaaa00000000,
    0xaaaaaaaaffffffff,
    0xaaaaaaaa00000001,
    0xaaaaaaaa00000002,
    0xaaaaaaaa00000000,
    0xaaaaaaaa00000001,
    0xaaaaaaaa7fffffff,
    0xaaaaaaaa80000000,
    0xaaaaaaaa00000000,
    0xaaaaaaaa80000000,
};

static uint64_t andnd_expect[] = {
    0x0000000012240420,
    0x0000000000000000,
    0x0000000000012340,
    0xffffffffffffffff,
    0x0000000000000000,
    0xfffffffffffffffe,
    0xfffffffffffffffd,
    0x7fffffffffffffff,
    0x7ffffffffffffffe,
    0x0000000000000000,
    0x0000000000000000,
    0x8000000000000000,
    0x0000000000000000,
    0x0000000012240420,
    0x0000000000000000,
    0x0000000000012340,
    0x00000000ffffffff,
    0x0000000000000000,
    0x00000000fffffffe,
    0x00000000fffffffd,
    0x000000007fffffff,
    0x000000007ffffffe,
    0x0000000000000000,
    0x0000000000000000,
    0x0000000080000000,
    0x0000000000000000,
};

static uint64_t ord_expect[] = {
    0x000000001a36fefa,
    0x0000000000012341,
    0xffffffffffffffff,
    0xffffffffffffffff,
    0xffffffffffffffff,
    0xffffffffffffffff,
    0xffffffffffffffff,
    0x7fffffffffffffff,
    0x7fffffffffffffff,
    0xffffffffffffffff,
    0xffffffffffffffff,
    0x8000000000000001,
    0xfffffffffffffffe,
    0xeeeeeeee1a36fefa,
    0xeeeeeeee00012341,
    0xeeeeeeeeffffffff,
    0xeeeeeeeeffffffff,
    0xeeeeeeeeffffffff,
    0xeeeeeeeeffffffff,
    0xeeeeeeeeffffffff,
    0xeeeeeeee7fffffff,
    0xeeeeeeee7fffffff,
    0xeeeeeeeeffffffff,
    0xeeeeeeeeffffffff,
    0xeeeeeeee80000001,
    0xeeeeeeeefffffffe,
};

static uint64_t ornd_expect[] = {
    0xfffffffff7fd577d,
    0xffffffffffffffff,
    0x0000000000012341,
    0xffffffffffffffff,
    0xffffffffffffffff,
    0xffffffffffffffff,
    0xffffffffffffffff,
    0xffffffffffffffff,
    0xffffffffffffffff,
    0x7fffffffffffffff,
    0x8000000000000000,
    0xfffffffffffffffe,
    0x8000000000000001,
    0xbbbbbbbbf7fd577d,
    0xbbbbbbbbffffffff,
    0xbbbbbbbb00012341,
    0xbbbbbbbbffffffff,
    0xbbbbbbbbffffffff,
    0xbbbbbbbbffffffff,
    0xbbbbbbbbffffffff,
    0xbbbbbbbbffffffff,
    0xbbbbbbbbffffffff,
    0xbbbbbbbb7fffffff,
    0xbbbbbbbb80000000,
    0xbbbbbbbbfffffffe,
    0xbbbbbbbb80000001,
};

static uint64_t xord_expect[] = {
    0x000000001a26aca2,
    0x0000000000000000,
    0xfffffffffffffffe,
    0xffffffffffffffff,
    0x0000000000000000,
    0xfffffffffffffffe,
    0xfffffffffffffffd,
    0x7fffffffffffffff,
    0x7ffffffffffffffe,
    0x8000000000000000,
    0x7fffffffffffffff,
    0x8000000000000001,
    0x7ffffffffffffffe,
    0x444444441a26aca2,
    0x4444444400000000,
    0x44444444fffffffe,
    0x44444444ffffffff,
    0x4444444400000000,
    0x44444444fffffffe,
    0x44444444fffffffd,
    0x444444447fffffff,
    0x444444447ffffffe,
    0x4444444480000000,
    0x444444447fffffff,
    0x4444444480000001,
    0x444444447ffffffe,
};

static uint64_t xornd_expect[] = {
    0xffffffffe5d9535d,
    0xffffffffffffffff,
    0x0000000000000001,
    0x0000000000000000,
    0xffffffffffffffff,
    0x0000000000000001,
    0x0000000000000002,
    0x8000000000000000,
    0x8000000000000001,
    0x7fffffffffffffff,
    0x8000000000000000,
    0x7ffffffffffffffe,
    0x8000000000000001,
    0xbbbbbbbbe5d9535d,
    0xbbbbbbbbffffffff,
    0xbbbbbbbb00000001,
    0xbbbbbbbb00000000,
    0xbbbbbbbbffffffff,
    0xbbbbbbbb00000001,
    0xbbbbbbbb00000002,
    0xbbbbbbbb80000000,
    0xbbbbbbbb80000001,
    0xbbbbbbbb7fffffff,
    0xbbbbbbbb80000000,
    0xbbbbbbbb7ffffffe,
    0xbbbbbbbb80000001,
};

static uint64_t addd_expect[] = {
    0x000000001a475152,
    0x0000000000024682,
    0x0000000000000000,
    0xffffffffffffffff,
    0xfffffffffffffffe,
    0x0000000000000000,
    0x0000000000000001,
    0x7fffffffffffffff,
    0x8000000000000000,
    0x7ffffffffffffffe,
    0x7fffffffffffffff,
    0x8000000000000001,
    0x7ffffffffffffffe,
    0x999999981a475152,
    0x9999999800024682,
    0x9999999900000000,
    0x99999998ffffffff,
    0x99999999fffffffe,
    0x9999999900000000,
    0x9999999900000001,
    0x999999987fffffff,
    0x9999999880000000,
    0x999999997ffffffe,
    0x999999997fffffff,
    0x9999999880000001,
    0x999999997ffffffe,
};

static uint64_t subd_expect[] = {
    0x000000000a215b9e,
    0x0000000000000000,
    0x0000000000024682,
    0xffffffffffffffff,
    0x0000000000000000,
    0xfffffffffffffffe,
    0xfffffffffffffffd,
    0x7fffffffffffffff,
    0x7ffffffffffffffe,
    0x8000000000000000,
    0x8000000000000001,
    0x7fffffffffffffff,
    0x8000000000000002,
    0xbbbbbbbc0a215b9e,
    0xbbbbbbbc00000000,
    0xbbbbbbbb00024682,
    0xbbbbbbbcffffffff,
    0xbbbbbbbc00000000,
    0xbbbbbbbcfffffffe,
    0xbbbbbbbcfffffffd,
    0xbbbbbbbc7fffffff,
    0xbbbbbbbc7ffffffe,
    0xbbbbbbbb80000000,
    0xbbbbbbbb80000001,
    0xbbbbbbbc7fffffff,
    0xbbbbbbbb80000002,
};

#define TEST_SXT(EXEC, CHAN) \
    CHECK2(EXEC, sxt, CHAN, 0, 0x7fff7f7f, 0x000000000000007f); \
    CHECK2(EXEC, sxt, CHAN, 1, 0x7fff7f7f, 0x0000000000007f7f); \
    CHECK2(EXEC, sxt, CHAN, 2, 0x7fff7f7f, 0x000000007fff7f7f); \
    CHECK2(EXEC, sxt, CHAN, 3, 0x7fff7f7f, 0x000000007fff7f7f); \
    CHECK2(EXEC, sxt, CHAN, 4, 0x7fff7f7f, 0x000000000000007f); \
    CHECK2(EXEC, sxt, CHAN, 5, 0x7fff7f7f, 0x0000000000007f7f); \
    CHECK2(EXEC, sxt, CHAN, 6, 0x7fff7f7f, 0x000000007fff7f7f); \
    CHECK2(EXEC, sxt, CHAN, 7, 0x7fff7f7f, 0x000000007fff7f7f); \
    CHECK2(EXEC, sxt, CHAN, 0, 0x80008080, 0xffffffffffffff80); \
    CHECK2(EXEC, sxt, CHAN, 1, 0x80008080, 0xffffffffffff8080); \
    CHECK2(EXEC, sxt, CHAN, 2, 0x80008080, 0xffffffff80008080); \
    CHECK2(EXEC, sxt, CHAN, 3, 0x80008080, 0xffffffff80008080); \
    CHECK2(EXEC, sxt, CHAN, 4, 0x80008080, 0x0000000000000080); \
    CHECK2(EXEC, sxt, CHAN, 5, 0x80008080, 0x0000000000008080); \
    CHECK2(EXEC, sxt, CHAN, 6, 0x80008080, 0x0000000080008080); \
    CHECK2(EXEC, sxt, CHAN, 7, 0x80008080, 0x0000000080008080)

static void test_sxt(void) {
    // implemented in a helper
    GROUP("sxt reg");
    TEST_SXT(EXEC_RR, 0);
    TEST_SXT(EXEC_RR, 1);
    TEST_SXT(EXEC_RR, 2);
    TEST_SXT(EXEC_RR, 3);
    TEST_SXT(EXEC_RR, 4);
    TEST_SXT(EXEC_RR, 5);

    // specialized for imm
    GROUP("sxt imm");
    TEST_SXT(EXEC_IR, 0);
    TEST_SXT(EXEC_IR, 1);
    TEST_SXT(EXEC_IR, 2);
    TEST_SXT(EXEC_IR, 3);
    TEST_SXT(EXEC_IR, 4);
    TEST_SXT(EXEC_IR, 5);
}

#define TEST_MERGE(INSN, CHAN, TYPE) \
    CHECK2_MERGE(EXEC_MERGE_XX, INSN, CHAN, -1, -2, 0, (TYPE) -1); \
    CHECK2_MERGE(EXEC_MERGE_XX, INSN, CHAN, -1, -2, 1, (TYPE) -2)

static void test_merge(void) {
    GROUP("merges");
    TEST_MERGE(merges, 0, uint32_t);
    TEST_MERGE(merges, 1, uint32_t);
    TEST_MERGE(merges, 2, uint32_t);
    TEST_MERGE(merges, 3, uint32_t);
    TEST_MERGE(merges, 4, uint32_t);
    TEST_MERGE(merges, 5, uint32_t);

    GROUP("merged");
    TEST_MERGE(merged, 0, uint64_t);
    TEST_MERGE(merged, 1, uint64_t);
    TEST_MERGE(merged, 2, uint64_t);
    TEST_MERGE(merged, 3, uint64_t);
    TEST_MERGE(merged, 4, uint64_t);
    TEST_MERGE(merged, 5, uint64_t);
}

static void test_shift(void) {
    GROUP("shls");
    CHECK2_ALL(EXEC_RR, shls, 1,  0, 1);
    CHECK2_ALL(EXEC_RR, shls, 1,  1, 2);
    CHECK2_ALL(EXEC_RR, shls, 1,  2, 4);
    CHECK2_ALL(EXEC_RR, shls, 1, 32, 1);

    GROUP("shrs");
    CHECK2_ALL(EXEC_RR, shrs, 8,  0, 8);
    CHECK2_ALL(EXEC_RR, shrs, 8,  1, 4);
    CHECK2_ALL(EXEC_RR, shrs, 8,  2, 2);
    CHECK2_ALL(EXEC_RR, shrs, 8, 32, 8);

    GROUP("sars");
    CHECK2_ALL(EXEC_RR, sars, 0x80000000,  0, 0x80000000);
    CHECK2_ALL(EXEC_RR, sars, 0x80000000,  1, 0xc0000000);
    CHECK2_ALL(EXEC_RR, sars, 0x80000000,  2, 0xe0000000);
    CHECK2_ALL(EXEC_RR, sars, 0x80000000, 31, 0xffffffff);
    CHECK2_ALL(EXEC_RR, sars, 0x80000000, 32, 0x80000000);

    GROUP("scls");
    CHECK2_ALL(EXEC_RR, scls, 0x80000001,  0, 0x80000001);
    CHECK2_ALL(EXEC_RR, scls, 0x80000001,  1, 0x00000003);
    CHECK2_ALL(EXEC_RR, scls, 0x80000001,  2, 0x00000006);
    CHECK2_ALL(EXEC_RR, scls, 0x80000001, 31, 0xc0000000);
    CHECK2_ALL(EXEC_RR, scls, 0x80000001, 32, 0x80000001);

    GROUP("scrs");
    CHECK2_ALL(EXEC_RR, scrs, 0x80000001,  0, 0x80000001);
    CHECK2_ALL(EXEC_RR, scrs, 0x80000001,  1, 0xc0000000);
    CHECK2_ALL(EXEC_RR, scrs, 0x80000001,  2, 0x60000000);
    CHECK2_ALL(EXEC_RR, scrs, 0x80000001, 31, 0x00000003);
    CHECK2_ALL(EXEC_RR, scrs, 0x80000001, 32, 0x80000001);

    GROUP("shld");
    CHECK2_ALL(EXEC_RR, shld, 1,  0, 1);
    CHECK2_ALL(EXEC_RR, shld, 1,  1, 2);
    CHECK2_ALL(EXEC_RR, shld, 1,  2, 4);
    CHECK2_ALL(EXEC_RR, shld, 1, 64, 1);

    GROUP("shrd");
    CHECK2_ALL(EXEC_RR, shrd, 8,  0, 8);
    CHECK2_ALL(EXEC_RR, shrd, 8,  1, 4);
    CHECK2_ALL(EXEC_RR, shrd, 8,  2, 2);
    CHECK2_ALL(EXEC_RR, shrd, 8, 64, 8);

    GROUP("sard");
    CHECK2_ALL(EXEC_RR, sard, 0x8000000000000000,  0, 0x8000000000000000);
    CHECK2_ALL(EXEC_RR, sard, 0x8000000000000000,  1, 0xc000000000000000);
    CHECK2_ALL(EXEC_RR, sard, 0x8000000000000000,  2, 0xe000000000000000);
    CHECK2_ALL(EXEC_RR, sard, 0x8000000000000000, 63, 0xffffffffffffffff);
    CHECK2_ALL(EXEC_RR, sard, 0x8000000000000000, 64, 0x8000000000000000);

    GROUP("scld");
    CHECK2_ALL(EXEC_RR, scld, 0x8000000000000001,  0, 0x8000000000000001);
    CHECK2_ALL(EXEC_RR, scld, 0x8000000000000001,  1, 0x0000000000000003);
    CHECK2_ALL(EXEC_RR, scld, 0x8000000000000001,  2, 0x0000000000000006);
    CHECK2_ALL(EXEC_RR, scld, 0x8000000000000001, 63, 0xc000000000000000);
    CHECK2_ALL(EXEC_RR, scld, 0x8000000000000001, 64, 0x8000000000000001);

    GROUP("scrd");
    CHECK2_ALL(EXEC_RR, scrd, 0x8000000000000001,  0, 0x8000000000000001);
    CHECK2_ALL(EXEC_RR, scrd, 0x8000000000000001,  1, 0xc000000000000000);
    CHECK2_ALL(EXEC_RR, scrd, 0x8000000000000001,  2, 0x6000000000000000);
    CHECK2_ALL(EXEC_RR, scrd, 0x8000000000000001, 63, 0x0000000000000003);
    CHECK2_ALL(EXEC_RR, scrd, 0x8000000000000001, 64, 0x8000000000000001);
}

static void test_getfs(void) {
#undef INSN64
#undef INSN
#define INSN getfs

#undef EXEC
#define EXEC EXEC_RR
GROUP("getfs reg, reg");
#include "test-getf.inc"

#undef EXEC
#define EXEC EXEC_RI
GROUP("getfs reg, imm");
#include "test-getf.inc"
}

static void test_getfd(void) {
#undef INSN64
#undef INSN
#define INSN64
#define INSN getfd

#undef EXEC
#define EXEC EXEC_RR
GROUP("getfd reg, reg");
#include "test-getf.inc"

#undef EXEC
#define EXEC EXEC_RI
GROUP("getfd reg, imm");
#include "test-getf.inc"
}

int main(int argc, char *argv[]) {
    TEST2_DEFAULT_DATA(CHECK2_ALL, EXEC_RR, ands);
    TEST2_DEFAULT_DATA(CHECK2_ALL, EXEC_RR, andns);
    TEST2_DEFAULT_DATA(CHECK2_ALL, EXEC_RR, ors);
    TEST2_DEFAULT_DATA(CHECK2_ALL, EXEC_RR, orns);
    TEST2_DEFAULT_DATA(CHECK2_ALL, EXEC_RR, xors);
    TEST2_DEFAULT_DATA(CHECK2_ALL, EXEC_RR, xorns);
    TEST2_DEFAULT_DATA(CHECK2_ALL, EXEC_RR, adds);
    TEST2_DEFAULT_DATA(CHECK2_ALL, EXEC_RR, subs);

    TEST2_DEFAULT_DATA(CHECK2_ALL, EXEC_RR, andd);
    TEST2_DEFAULT_DATA(CHECK2_ALL, EXEC_RR, andnd);
    TEST2_DEFAULT_DATA(CHECK2_ALL, EXEC_RR, ord);
    TEST2_DEFAULT_DATA(CHECK2_ALL, EXEC_RR, ornd);
    TEST2_DEFAULT_DATA(CHECK2_ALL, EXEC_RR, xord);
    TEST2_DEFAULT_DATA(CHECK2_ALL, EXEC_RR, xornd);
    TEST2_DEFAULT_DATA(CHECK2_ALL, EXEC_RR, addd);
    TEST2_DEFAULT_DATA(CHECK2_ALL, EXEC_RR, subd);

    test_sxt();
    test_merge();
    test_shift();
    test_getfs();
    test_getfd();

    return is_failed();
}
