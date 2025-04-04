#include <stdbool.h>
#include "test-e2k.h"

#define TEST2_DEFAULT_DATA(CHECK, EXEC, INSN) \
    TEST2_DATA(CHECK, EXEC, INSN, test_data, glue(INSN, _expect))

static struct e2k_test_data test_data[] = {
#include "test-data-int.inc"
};

EXPECT_DATA(ands, uint32_t) {
    0x00105258, 0x00012341, 0x00000001, 0x00000000, 0xffffffff, 0x00000001,
    0x00000002, 0x00000000, 0x00000001, 0xffffffff, 0x00000000, 0x00000000,
    0x00000000, 0x00105258, 0x00012341, 0x00000001, 0x00000000, 0xffffffff,
    0x00000001, 0x00000002, 0x00000000, 0x00000001, 0x7fffffff, 0x80000000,
    0x00000000, 0x80000000, 0x9abcdef0, 0x12141210,
};
EXPECT_DATA(andns, uint32_t) {
    0x12240420, 0x00000000, 0x00012340, 0xffffffff, 0x00000000, 0xfffffffe,
    0xfffffffd, 0xffffffff, 0xfffffffe, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x12240420, 0x00000000, 0x00012340, 0xffffffff, 0x00000000,
    0xfffffffe, 0xfffffffd, 0x7fffffff, 0x7ffffffe, 0x00000000, 0x00000000,
    0x80000000, 0x00000000, 0x00000000, 0x88a8cce0
};
EXPECT_DATA(ors, uint32_t) {
    0x1a36fefa, 0x00012341, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0x00000001,
    0xfffffffe, 0x1a36fefa, 0x00012341, 0xffffffff, 0xffffffff, 0xffffffff,
    0xffffffff, 0xffffffff, 0x7fffffff, 0x7fffffff, 0xffffffff, 0xffffffff,
    0x80000001, 0xfffffffe, 0x9abcdef0, 0xfefcfef0,
};
EXPECT_DATA(orns, uint32_t) {
    0xf7fd577d, 0xffffffff, 0x00012341, 0xffffffff, 0xffffffff, 0xffffffff,
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0x00000000, 0xfffffffe,
    0x00000001, 0xf7fd577d, 0xffffffff, 0x00012341, 0xffffffff, 0xffffffff,
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0x7fffffff, 0x80000000,
    0xfffffffe, 0x80000001, 0xffffffff, 0x9bbfdfff,
};
EXPECT_DATA(xors, uint32_t) {
    0x1a26aca2, 0x00000000, 0xfffffffe, 0xffffffff, 0x00000000, 0xfffffffe,
    0xfffffffd, 0xffffffff, 0xfffffffe, 0x00000000, 0xffffffff, 0x00000001,
    0xfffffffe, 0x1a26aca2, 0x00000000, 0xfffffffe, 0xffffffff, 0x00000000,
    0xfffffffe, 0xfffffffd, 0x7fffffff, 0x7ffffffe, 0x80000000, 0x7fffffff,
    0x80000001, 0x7ffffffe, 0x00000000, 0xece8ece0,
};
EXPECT_DATA(xorns, uint32_t) {
    0xe5d9535d, 0xffffffff, 0x00000001, 0x00000000, 0xffffffff, 0x00000001,
    0x00000002, 0x00000000, 0x00000001, 0xffffffff, 0x00000000, 0xfffffffe,
    0x00000001, 0xe5d9535d, 0xffffffff, 0x00000001, 0x00000000, 0xffffffff,
    0x00000001, 0x00000002, 0x80000000, 0x80000001, 0x7fffffff, 0x80000000,
    0x7ffffffe, 0x80000001, 0xffffffff, 0x1317131f,
};
EXPECT_DATA(adds, uint32_t) {
    0x1a475152, 0x00024682, 0x00000000, 0xffffffff, 0xfffffffe, 0x00000000,
    0x00000001, 0xffffffff, 0x00000000, 0xfffffffe, 0xffffffff, 0x00000001,
    0xfffffffe, 0x1a475152, 0x00024682, 0x00000000, 0xffffffff, 0xfffffffe,
    0x00000000, 0x00000001, 0x7fffffff, 0x80000000, 0x7ffffffe, 0x7fffffff,
    0x80000001, 0x7ffffffe, 0x3579bde0, 0x11111100,
};
EXPECT_DATA(subs, uint32_t) {
    0x0a215b9e, 0x00000000, 0x00024682, 0xffffffff, 0x00000000, 0xfffffffe,
    0xfffffffd, 0xffffffff, 0xfffffffe, 0x00000000, 0x00000001, 0xffffffff,
    0x00000002, 0x0a215b9e, 0x00000000, 0x00024682, 0xffffffff, 0x00000000,
    0xfffffffe, 0xfffffffd, 0x7fffffff, 0x7ffffffe, 0x80000000, 0x80000001,
    0x7fffffff, 0x80000002, 0x00000000, 0x2468ace0,
};
EXPECT_DATA(andd, uint64_t) {
    0x0000000000105258, 0x0000000000012341, 0x0000000000000001, 0x0000000000000000,
    0xffffffffffffffff, 0x0000000000000001, 0x0000000000000002, 0x0000000000000000,
    0x0000000000000001, 0x7fffffffffffffff, 0x8000000000000000, 0x0000000000000000,
    0x8000000000000000, 0xaaaaaaaa00105258, 0xaaaaaaaa00012341, 0xaaaaaaaa00000001,
    0xaaaaaaaa00000000, 0xaaaaaaaaffffffff, 0xaaaaaaaa00000001, 0xaaaaaaaa00000002,
    0xaaaaaaaa00000000, 0xaaaaaaaa00000001, 0xaaaaaaaa7fffffff, 0xaaaaaaaa80000000,
    0xaaaaaaaa00000000, 0xaaaaaaaa80000000, 0x123456789abcdef0, 0x1214121812141210,
};
EXPECT_DATA(andnd, uint64_t) {
    0x0000000012240420, 0x0000000000000000, 0x0000000000012340, 0xffffffffffffffff,
    0x0000000000000000, 0xfffffffffffffffe, 0xfffffffffffffffd, 0x7fffffffffffffff,
    0x7ffffffffffffffe, 0x0000000000000000, 0x0000000000000000, 0x8000000000000000,
    0x0000000000000000, 0x0000000012240420, 0x0000000000000000, 0x0000000000012340,
    0x00000000ffffffff, 0x0000000000000000, 0x00000000fffffffe, 0x00000000fffffffd,
    0x000000007fffffff, 0x000000007ffffffe, 0x0000000000000000, 0x0000000000000000,
    0x0000000080000000, 0x0000000000000000, 0x0000000000000000, 0x0020446088a8cce0,
};
EXPECT_DATA(ord, uint64_t) {
    0x000000001a36fefa, 0x0000000000012341, 0xffffffffffffffff, 0xffffffffffffffff,
    0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0x7fffffffffffffff,
    0x7fffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0x8000000000000001,
    0xfffffffffffffffe, 0xeeeeeeee1a36fefa, 0xeeeeeeee00012341, 0xeeeeeeeeffffffff,
    0xeeeeeeeeffffffff, 0xeeeeeeeeffffffff, 0xeeeeeeeeffffffff, 0xeeeeeeeeffffffff,
    0xeeeeeeee7fffffff, 0xeeeeeeee7fffffff, 0xeeeeeeeeffffffff, 0xeeeeeeeeffffffff,
    0xeeeeeeee80000001, 0xeeeeeeeefffffffe, 0x123456789abcdef0, 0xfefcfef8fefcfef0,
};
EXPECT_DATA(ornd, uint64_t) {
    0xfffffffff7fd577d, 0xffffffffffffffff, 0x0000000000012341, 0xffffffffffffffff,
    0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff,
    0xffffffffffffffff, 0x7fffffffffffffff, 0x8000000000000000, 0xfffffffffffffffe,
    0x8000000000000001, 0xbbbbbbbbf7fd577d, 0xbbbbbbbbffffffff, 0xbbbbbbbb00012341,
    0xbbbbbbbbffffffff, 0xbbbbbbbbffffffff, 0xbbbbbbbbffffffff, 0xbbbbbbbbffffffff,
    0xbbbbbbbbffffffff, 0xbbbbbbbbffffffff, 0xbbbbbbbb7fffffff, 0xbbbbbbbb80000000,
    0xbbbbbbbbfffffffe, 0xbbbbbbbb80000001, 0xffffffffffffffff, 0x1337577f9bbfdfff,
};
EXPECT_DATA(xord, uint64_t) {
    0x000000001a26aca2, 0x0000000000000000, 0xfffffffffffffffe, 0xffffffffffffffff,
    0x0000000000000000, 0xfffffffffffffffe, 0xfffffffffffffffd, 0x7fffffffffffffff,
    0x7ffffffffffffffe, 0x8000000000000000, 0x7fffffffffffffff, 0x8000000000000001,
    0x7ffffffffffffffe, 0x444444441a26aca2, 0x4444444400000000, 0x44444444fffffffe,
    0x44444444ffffffff, 0x4444444400000000, 0x44444444fffffffe, 0x44444444fffffffd,
    0x444444447fffffff, 0x444444447ffffffe, 0x4444444480000000, 0x444444447fffffff,
    0x4444444480000001, 0x444444447ffffffe, 0x0000000000000000, 0xece8ece0ece8ece0,
};
EXPECT_DATA(xornd, uint64_t) {
    0xffffffffe5d9535d, 0xffffffffffffffff, 0x0000000000000001, 0x0000000000000000,
    0xffffffffffffffff, 0x0000000000000001, 0x0000000000000002, 0x8000000000000000,
    0x8000000000000001, 0x7fffffffffffffff, 0x8000000000000000, 0x7ffffffffffffffe,
    0x8000000000000001, 0xbbbbbbbbe5d9535d, 0xbbbbbbbbffffffff, 0xbbbbbbbb00000001,
    0xbbbbbbbb00000000, 0xbbbbbbbbffffffff, 0xbbbbbbbb00000001, 0xbbbbbbbb00000002,
    0xbbbbbbbb80000000, 0xbbbbbbbb80000001, 0xbbbbbbbb7fffffff, 0xbbbbbbbb80000000,
    0xbbbbbbbb7ffffffe, 0xbbbbbbbb80000001, 0xffffffffffffffff, 0x1317131f1317131f,
};
EXPECT_DATA(addd, uint64_t) {
    0x000000001a475152, 0x0000000000024682, 0x0000000000000000, 0xffffffffffffffff,
    0xfffffffffffffffe, 0x0000000000000000, 0x0000000000000001, 0x7fffffffffffffff,
    0x8000000000000000, 0x7ffffffffffffffe, 0x7fffffffffffffff, 0x8000000000000001,
    0x7ffffffffffffffe, 0x999999981a475152, 0x9999999800024682, 0x9999999900000000,
    0x99999998ffffffff, 0x99999999fffffffe, 0x9999999900000000, 0x9999999900000001,
    0x999999987fffffff, 0x9999999880000000, 0x999999997ffffffe, 0x999999997fffffff,
    0x9999999880000001, 0x999999997ffffffe, 0x2468acf13579bde0, 0x1111111111111100,
};
EXPECT_DATA(subd, uint64_t) {
    0x000000000a215b9e, 0x0000000000000000, 0x0000000000024682, 0xffffffffffffffff,
    0x0000000000000000, 0xfffffffffffffffe, 0xfffffffffffffffd, 0x7fffffffffffffff,
    0x7ffffffffffffffe, 0x8000000000000000, 0x8000000000000001, 0x7fffffffffffffff,
    0x8000000000000002, 0xbbbbbbbc0a215b9e, 0xbbbbbbbc00000000, 0xbbbbbbbb00024682,
    0xbbbbbbbcffffffff, 0xbbbbbbbc00000000, 0xbbbbbbbcfffffffe, 0xbbbbbbbcfffffffd,
    0xbbbbbbbc7fffffff, 0xbbbbbbbc7ffffffe, 0xbbbbbbbb80000000, 0xbbbbbbbb80000001,
    0xbbbbbbbc7fffffff, 0xbbbbbbbb80000002, 0x0000000000000000, 0x13579be02468ace0,
};

static void test_basic(void) {
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
}

#define TEST_SXT(EXEC) \
    CHECK2_ALL(EXEC, sxt, 0, 0x7fff7f7f, 0x000000000000007f); \
    CHECK2_ALL(EXEC, sxt, 1, 0x7fff7f7f, 0x0000000000007f7f); \
    CHECK2_ALL(EXEC, sxt, 2, 0x7fff7f7f, 0x000000007fff7f7f); \
    CHECK2_ALL(EXEC, sxt, 3, 0x7fff7f7f, 0x000000007fff7f7f); \
    CHECK2_ALL(EXEC, sxt, 4, 0x7fff7f7f, 0x000000000000007f); \
    CHECK2_ALL(EXEC, sxt, 5, 0x7fff7f7f, 0x0000000000007f7f); \
    CHECK2_ALL(EXEC, sxt, 6, 0x7fff7f7f, 0x000000007fff7f7f); \
    CHECK2_ALL(EXEC, sxt, 7, 0x7fff7f7f, 0x000000007fff7f7f); \
    CHECK2_ALL(EXEC, sxt, 0, 0x80008080, 0xffffffffffffff80); \
    CHECK2_ALL(EXEC, sxt, 1, 0x80008080, 0xffffffffffff8080); \
    CHECK2_ALL(EXEC, sxt, 2, 0x80008080, 0xffffffff80008080); \
    CHECK2_ALL(EXEC, sxt, 3, 0x80008080, 0xffffffff80008080); \
    CHECK2_ALL(EXEC, sxt, 4, 0x80008080, 0x0000000000000080); \
    CHECK2_ALL(EXEC, sxt, 5, 0x80008080, 0x0000000000008080); \
    CHECK2_ALL(EXEC, sxt, 6, 0x80008080, 0x0000000080008080); \
    CHECK2_ALL(EXEC, sxt, 7, 0x80008080, 0x0000000080008080)

static void test_sxt(void) {
    // implemented in a helper
    GROUP("sxt reg");
    TEST_SXT(EXEC_RR);

    // specialized for imm
    GROUP("sxt imm");
    TEST_SXT(EXEC_IR);
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

static struct e2k_test_data cmp_test_data[] = {
#include "test-data-cmp.inc"
};

#define TEST_CMP(INSN) \
    TEST2_DATA(CHECK2_0134, EXEC_CMP_XX, INSN, cmp_test_data, glue(INSN, _expect))

static void test_cmpsb(void) {
    EXPECT_DATA(cmposb, bool) {
        0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 1, 1, 1, 1, 0, 1, 1,
        0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
        0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    };
    EXPECT_DATA(cmpbsb, bool) {
        0, 1, 1, 1, 1, 1, 1, 0, 1, 1,
        0, 0, 1, 1, 1, 1, 1, 0, 1, 1,
        0, 0, 0, 1, 1, 1, 1, 0, 1, 1,
        0, 0, 0, 0, 1, 1, 1, 0, 1, 1,
        0, 0, 0, 0, 0, 1, 1, 0, 0, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 1, 1, 0, 1, 1,
        0, 0, 0, 0, 0, 1, 1, 0, 0, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    };
    EXPECT_DATA(cmpesb, bool) {
        1, 0, 0, 0, 0, 0, 0, 1, 0, 0,
        0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 1, 0, 0, 0, 1, 0,
        0, 0, 0, 0, 0, 1, 1, 0, 0, 1,
        0, 0, 0, 0, 0, 1, 1, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 1, 0, 0,
        0, 0, 0, 0, 1, 0, 0, 0, 1, 0,
        0, 0, 0, 0, 0, 1, 1, 0, 0, 1,
    };
    EXPECT_DATA(cmpbesb, bool) {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        0, 1, 1, 1, 1, 1, 1, 0, 1, 1,
        0, 0, 1, 1, 1, 1, 1, 0, 1, 1,
        0, 0, 0, 1, 1, 1, 1, 0, 1, 1,
        0, 0, 0, 0, 1, 1, 1, 0, 1, 1,
        0, 0, 0, 0, 0, 1, 1, 0, 0, 1,
        0, 0, 0, 0, 0, 1, 1, 0, 0, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        0, 0, 0, 0, 1, 1, 1, 0, 1, 1,
        0, 0, 0, 0, 0, 1, 1, 0, 0, 1,
    };
    EXPECT_DATA(cmpssb, bool) {
        0, 1, 1, 1, 0, 0, 0, 0, 0, 0,
        0, 0, 1, 1, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 1, 1, 1, 1, 0, 1, 1,
        1, 0, 0, 0, 1, 1, 1, 1, 1, 1,
        1, 1, 0, 0, 0, 1, 1, 1, 0, 1,
        1, 1, 1, 0, 0, 0, 0, 1, 0, 0,
        1, 1, 1, 0, 0, 0, 0, 1, 0, 0,
        0, 1, 1, 1, 0, 0, 0, 0, 0, 0,
        1, 1, 0, 0, 0, 1, 1, 1, 0, 1,
        1, 1, 1, 0, 0, 0, 0, 1, 0, 0,
    };
    EXPECT_DATA(cmppsb, bool) {
        1, 1, 0, 1, 1, 0, 0, 1, 1, 0,
        0, 1, 0, 0, 1, 0, 0, 0, 1, 0,
        1, 0, 1, 1, 0, 1, 1, 1, 0, 1,
        1, 1, 0, 1, 1, 0, 0, 1, 1, 0,
        0, 1, 1, 0, 1, 1, 1, 0, 1, 1,
        1, 0, 1, 1, 0, 1, 1, 1, 0, 1,
        1, 0, 1, 1, 0, 1, 1, 1, 0, 1,
        1, 1, 0, 1, 1, 0, 0, 1, 1, 0,
        0, 1, 1, 0, 1, 1, 1, 0, 1, 1,
        1, 0, 1, 1, 0, 1, 1, 1, 0, 1,
    };
    EXPECT_DATA(cmplsb, bool) {
        0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 0, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 0, 0, 1, 1, 1, 0, 1,
        1, 1, 1, 0, 0, 0, 0, 1, 0, 0,
        1, 1, 1, 0, 0, 0, 0, 1, 0, 0,
        0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 0, 0, 1, 1, 1, 0, 1,
        1, 1, 1, 0, 0, 0, 0, 1, 0, 0,
    };
    EXPECT_DATA(cmplesb, bool) {
        1, 1, 1, 0, 0, 0, 0, 1, 0, 0,
        0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 0, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 0, 0, 1, 1, 1, 0, 1,
        1, 1, 1, 0, 0, 1, 1, 1, 0, 1,
        1, 1, 1, 0, 0, 0, 0, 1, 0, 0,
        1, 1, 1, 0, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 0, 0, 1, 1, 1, 0, 1,
    };

    TEST_CMP(cmposb);
    TEST_CMP(cmpbsb);
    TEST_CMP(cmpesb);
    TEST_CMP(cmpbesb);
    TEST_CMP(cmpssb);
    TEST_CMP(cmppsb);
    TEST_CMP(cmplsb);
    TEST_CMP(cmplesb);
}

static void test_cmpdb(void) {
    EXPECT_DATA(cmpodb, bool) {
        0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
        0, 1, 1, 1, 1, 1, 1, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    };
    EXPECT_DATA(cmpbdb, bool) {
        0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
        0, 0, 0, 1, 1, 1, 1, 1, 1, 1,
        0, 0, 0, 0, 1, 1, 1, 1, 1, 1,
        0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
        0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
        0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    };
    EXPECT_DATA(cmpedb, bool) {
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    };
    EXPECT_DATA(cmpbedb, bool) {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
        0, 0, 0, 1, 1, 1, 1, 1, 1, 1,
        0, 0, 0, 0, 1, 1, 1, 1, 1, 1,
        0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
        0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
        0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    };
    EXPECT_DATA(cmpsdb, bool) {
        0, 1, 1, 1, 1, 1, 1, 1, 0, 0,
        0, 0, 1, 1, 1, 1, 1, 1, 0, 0,
        0, 0, 0, 1, 1, 1, 1, 1, 0, 0,
        0, 0, 0, 0, 1, 1, 1, 1, 0, 0,
        0, 0, 0, 0, 0, 1, 1, 1, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 1, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 1, 1,
        1, 1, 1, 1, 1, 1, 0, 0, 0, 1,
        1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
    };
    EXPECT_DATA(cmppdb, bool) {
        1, 1, 0, 1, 1, 0, 0, 1, 1, 0,
        0, 1, 0, 0, 1, 0, 0, 0, 1, 0,
        1, 0, 1, 1, 0, 1, 1, 1, 0, 1,
        1, 1, 0, 1, 1, 0, 0, 1, 1, 0,
        0, 1, 1, 0, 1, 1, 1, 0, 1, 1,
        1, 0, 1, 1, 0, 1, 1, 1, 0, 1,
        1, 0, 1, 1, 0, 1, 1, 1, 0, 1,
        1, 1, 0, 1, 1, 0, 0, 1, 1, 0,
        0, 1, 1, 0, 1, 1, 1, 0, 1, 1,
        1, 0, 1, 1, 0, 1, 1, 1, 0, 1,
    };
    EXPECT_DATA(cmpldb, bool) {
        0, 1, 1, 1, 1, 1, 1, 0, 0, 0,
        0, 0, 1, 1, 1, 1, 1, 0, 0, 0,
        0, 0, 0, 1, 1, 1, 1, 0, 0, 0,
        0, 0, 0, 0, 1, 1, 1, 0, 0, 0,
        0, 0, 0, 0, 0, 1, 1, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 1, 1, 1, 0, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 0, 0, 1,
        1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
    };
    EXPECT_DATA(cmpledb, bool) {
        1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
        0, 1, 1, 1, 1, 1, 1, 0, 0, 0,
        0, 0, 1, 1, 1, 1, 1, 0, 0, 0,
        0, 0, 0, 1, 1, 1, 1, 0, 0, 0,
        0, 0, 0, 0, 1, 1, 1, 0, 0, 0,
        0, 0, 0, 0, 0, 1, 1, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 0, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 0, 0, 1,
    };

    TEST_CMP(cmpodb);
    TEST_CMP(cmpbdb);
    TEST_CMP(cmpedb);
    TEST_CMP(cmpbedb);
    TEST_CMP(cmpsdb);
    TEST_CMP(cmppdb);
    TEST_CMP(cmpldb);
    TEST_CMP(cmpledb);
}

static void test_cmpandsb(void) {
    EXPECT_DATA(cmpandesb, bool) {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 1, 0, 0, 0, 1, 0, 0,
        1, 0, 0, 1, 0, 0, 0, 1, 0, 0,
        1, 1, 1, 0, 0, 0, 0, 1, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 1, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 1, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 1, 0, 0,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 0, 0, 0, 0, 1, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 1, 0, 0,
    };
    EXPECT_DATA(cmpandssb, bool) {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 1, 1, 1, 1, 0, 1, 1,
        0, 0, 0, 1, 1, 1, 1, 0, 1, 1,
        0, 0, 0, 1, 1, 1, 1, 0, 1, 1,
        0, 0, 0, 1, 1, 1, 1, 0, 1, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 1, 1, 1, 1, 0, 1, 1,
        0, 0, 0, 1, 1, 1, 1, 0, 1, 1,
    };
    EXPECT_DATA(cmpandpsb, bool) {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 1, 0, 0, 0, 1, 0, 0,
        1, 0, 1, 1, 0, 1, 1, 1, 0, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 1, 0, 0, 0, 1, 0, 0,
        1, 0, 1, 1, 0, 1, 1, 1, 0, 1,
        1, 0, 1, 1, 0, 1, 1, 1, 0, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 1, 0, 0, 0, 1, 0, 0,
        1, 0, 1, 1, 0, 1, 1, 1, 0, 1,
    };
    EXPECT_DATA(cmpandlesb, bool) {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 1, 0, 0, 0, 1, 0, 0,
        1, 0, 0, 1, 0, 0, 0, 1, 0, 0,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 1, 1, 1, 1, 1, 1, 1,
    };

    TEST_CMP(cmpandesb);
    TEST_CMP(cmpandssb);
    TEST_CMP(cmpandpsb);
    TEST_CMP(cmpandlesb);
}

static void test_cmpanddb(void) {
    EXPECT_DATA(cmpandedb, bool) {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 1, 0, 0, 0, 1, 0, 0,
        1, 0, 0, 1, 0, 0, 0, 1, 0, 0,
        1, 1, 1, 0, 0, 0, 0, 1, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 1, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 1, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 1, 0, 0,
        1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    };
    EXPECT_DATA(cmpandsdb, bool) {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
        0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
        0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
    };
    EXPECT_DATA(cmpandpdb, bool) {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 1, 0, 0, 0, 1, 0, 0,
        1, 0, 1, 1, 0, 1, 1, 1, 0, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 1, 0, 0, 0, 1, 0, 0,
        1, 0, 1, 1, 0, 1, 1, 1, 0, 1,
        1, 0, 1, 1, 0, 1, 1, 1, 0, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 1, 0, 0, 0, 1, 0, 0,
        1, 0, 1, 1, 0, 1, 1, 1, 0, 1,
    };
    EXPECT_DATA(cmpandledb, bool) {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 1, 0, 0, 0, 1, 0, 0,
        1, 0, 0, 1, 0, 0, 0, 1, 0, 0,
        1, 1, 1, 0, 0, 0, 0, 1, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 1, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 1, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 1, 0, 0,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 0, 0, 0, 0, 1, 1, 1,
        1, 0, 0, 0, 0, 0, 0, 1, 1, 1,
    };

    TEST_CMP(cmpandedb);
    TEST_CMP(cmpandsdb);
    TEST_CMP(cmpandpdb);
    TEST_CMP(cmpandledb);
}

// TODO: cctopo
// TODO: cctopb
// TODO: cctope
// TODO: cctopbe
// TODO: cctops
// TODO: cctopp
// TODO: cctopl
// TODO: cctople

// TODO: movts
// TODO: movtcs
// TODO: movtrs
// TODO: movtrcs
// TODO: movtd
// TODO: movtcd
// TODO: movtrd
// TODO: movtrcd

// TODO: getpl

// TODO: rws
// TODO: rwd
// TODO: rrs
// TODO: rrd

// TODO: udivs
// TODO: udivd
// TODO: sdivs
// TODO: sdivd
// TODO: udivx
// TODO: umodx
// TODO: sdivx
// TODO: smodx

// TODO: muls
// TODO: muld
// TODO: umulx
// TODO: smulx

// TODO: movtq
// TODO: movtcq
// TODO: movtrq
// TODO: movtrcq

int main(int argc, char *argv[]) {
    test_basic();
    test_sxt();
    test_merge();
    test_shift();
    test_getfs();
    test_getfd();
    test_cmpsb();
    test_cmpdb();
    test_cmpandsb();
    test_cmpanddb();

    return is_failed();
}
