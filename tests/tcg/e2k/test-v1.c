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

static uint64_t int_src3[] = {
    0x123456789abcdef0, 0xfffffffffffedcbf, 0x0000000000012341, 0x0000000000000000,
    0x0000000000000002, 0x0000000000000001, 0xffffffffffffffff, 0x0000000000000001,
    0x0000000000000000, 0xffffffffffffffff, 0xfffffffffffffffe, 0xffffffffffffffff,
    0x0000000000000001, 0xeeeeeeee00012341, 0xeeeeeeee0812fada, 0xeeeeeeeefffedcbf,
    0x0000000000000000, 0xeeeeeeee00000002, 0xeeeeeeee00000001, 0xeeeeeeeeffffffff,
    0xeeeeeeee00000001, 0xeeeeeeee00000000, 0xeeeeeeeeffffffff, 0xeeeeeeeefffffffe,
    0xeeeeeeeeffffffff, 0xeeeeeeee00000001, 0xfedcba9876543210, 0x123456789abcdef0,
};

static uint64_t div_src1[] = {
    0x0000000012345678, 0x0000000000012341, 0xffffffffffffffff, 0x7fffffffffffffff,
    0x8000000000000000, 0xaaaaaaaa12345678, 0xaaaaaaaa00012341, 0xaaaaaaaaffffffff,
    0xaaaaaaaaffffffff, 0xaaaaaaaa7fffffff, 0xaaaaaaaa80000000, 0x123456789abcdef0,
};

static uint64_t div_src2[] = {
    0x000000000812fada, 0x0000000000012341, 0xfffffffffffedcbf, 0x0000000000000001,
    0x0000000000000002, 0x0000000000000001, 0xfffffffffffffffe, 0xeeeeeeee0812fada,
    0xeeeeeeee00012341, 0xeeeeeeeefffedcbf, 0xeeeeeeeeffffffff, 0xeeeeeeee00000001,
    0xeeeeeeee00000002, 0xeeeeeeeefffffffe, 0x123456789abcdef0, 0xfedcba9876543210,
};

static uint64_t udivx_src1[] = {
    0x0000000012345678, 0x0000000000012341, 0x123456789abcdefa, 0x8000000080000000,
};

static uint64_t udivx_src2[] = {
    0xaaaaaaaa00000123, 0xaaaaaaaa00000001, 0xaaaaaaaa7fffffff, 0xaaaaaaaafffffff0,
};

static uint64_t sdivx_src1[] = {
    0x0000000012345678, 0x0000000000012341, 0x123456789abcdefa, 0xffffffffffffffff,
};

static uint64_t sdivx_src2[] = {
    0xaaaaaaaa00000123, 0xaaaaaaaa00000001, 0xaaaaaaaa7fffffff, 0xaaaaaaaa7fffffff,
};

static uint64_t shift_src1[] = {
    0x0000000000000001, 0x0000000000000008, 0x000000007fffffff, 0x0000000080000000,
    0x0000000080000001, 0x00000000ffffffff, 0xeeeeeeee00000001, 0xeeeeeeee00000008,
    0xeeeeeeee7fffffff, 0xeeeeeeee80000000, 0xeeeeeeee80000001, 0xeeeeeeeeffffffff,
    0x7fffffffffffffff, 0x8000000000000000, 0x8000000000000001, 0xffffffffffffffff,
};

static uint64_t shift_src2[] = {
    0, 1, 2, 31, 32, 47, 48, 63, 64,
};

static uint64_t cmp_src[] = {
    0x0000000000000000, 0x0000000000000001, 0x000000007fffffff, 0x0000000080000000,
    0x00000000deadbeef, 0x00000000ffffffff, 0x7fffffffffffffff, 0x8000000000000000,
    0xdeadbeefdeadbeef, 0xffffffffffffffff,
};

#include "test-v1-expect.inc"

static void test_int(void) {
    CHECK2(EXEC_RR, 012345, ands,   int_src1, int_src2);
    CHECK2(EXEC_RR, 012345, andns,  int_src1, int_src2);
    CHECK2(EXEC_RR, 012345, ors,    int_src1, int_src2);
    CHECK2(EXEC_RR, 012345, orns,   int_src1, int_src2);
    CHECK2(EXEC_RR, 012345, xors,   int_src1, int_src2);
    CHECK2(EXEC_RR, 012345, xorns,  int_src1, int_src2);
    CHECK2(EXEC_RR, 012345, adds,   int_src1, int_src2);
    CHECK2(EXEC_RR, 012345, subs,   int_src1, int_src2);

    CHECK2(EXEC_RR, 012345, andd,   int_src1, int_src2);
    CHECK2(EXEC_RR, 012345, andnd,  int_src1, int_src2);
    CHECK2(EXEC_RR, 012345, ord,    int_src1, int_src2);
    CHECK2(EXEC_RR, 012345, ornd,   int_src1, int_src2);
    CHECK2(EXEC_RR, 012345, xord,   int_src1, int_src2);
    CHECK2(EXEC_RR, 012345, xornd,  int_src1, int_src2);
    CHECK2(EXEC_RR, 012345, addd,   int_src1, int_src2);
    CHECK2(EXEC_RR, 012345, subd,   int_src1, int_src2);

    CHECK2(EXEC_RR, 0134, muls,     int_src1, int_src2);
    CHECK2(EXEC_RR, 0134, muld,     int_src1, int_src2);
    CHECK2(EXEC_RR, 0134, umulx,    int_src1, int_src2);
    CHECK2(EXEC_RR, 0134, smulx,    int_src1, int_src2);

    CHECK2_CARTESIAN(EXEC_RR, 5, udivs, div_src1, div_src2);
    CHECK2_CARTESIAN(EXEC_RR, 5, udivd, div_src1, div_src2);
    CHECK2_CARTESIAN(EXEC_RR, 5, sdivs, div_src1, div_src2);
    CHECK2_CARTESIAN(EXEC_RR, 5, sdivd, div_src1, div_src2);

    CHECK2(EXEC_RR, 5, udivx, udivx_src1, udivx_src2);
    CHECK2(EXEC_RR, 5, umodx, udivx_src1, udivx_src2);
    CHECK2(EXEC_RR, 5, sdivx, sdivx_src1, sdivx_src2);
    CHECK2(EXEC_RR, 5, smodx, sdivx_src1, sdivx_src2);
}

#define CHECK_SXT(EXEC, CHAN, COMMENT) do { \
    alc_test_t test = test_start("sxt", COMMENT, glue(ALC, CHAN), SKIP_GEN | HAS_SRC1 | HAS_SRC2); \
    EXEC2_REPORT(test, glue3(EXEC, _, CHAN), sxt, 0, 0x7fff7f7f, 0x000000000000007f); \
    EXEC2_REPORT(test, glue3(EXEC, _, CHAN), sxt, 1, 0x7fff7f7f, 0x0000000000007f7f); \
    EXEC2_REPORT(test, glue3(EXEC, _, CHAN), sxt, 2, 0x7fff7f7f, 0x000000007fff7f7f); \
    EXEC2_REPORT(test, glue3(EXEC, _, CHAN), sxt, 3, 0x7fff7f7f, 0x000000007fff7f7f); \
    EXEC2_REPORT(test, glue3(EXEC, _, CHAN), sxt, 4, 0x7fff7f7f, 0x000000000000007f); \
    EXEC2_REPORT(test, glue3(EXEC, _, CHAN), sxt, 5, 0x7fff7f7f, 0x0000000000007f7f); \
    EXEC2_REPORT(test, glue3(EXEC, _, CHAN), sxt, 6, 0x7fff7f7f, 0x000000007fff7f7f); \
    EXEC2_REPORT(test, glue3(EXEC, _, CHAN), sxt, 7, 0x7fff7f7f, 0x000000007fff7f7f); \
    EXEC2_REPORT(test, glue3(EXEC, _, CHAN), sxt, 0, 0x80008080, 0xffffffffffffff80); \
    EXEC2_REPORT(test, glue3(EXEC, _, CHAN), sxt, 1, 0x80008080, 0xffffffffffff8080); \
    EXEC2_REPORT(test, glue3(EXEC, _, CHAN), sxt, 2, 0x80008080, 0xffffffff80008080); \
    EXEC2_REPORT(test, glue3(EXEC, _, CHAN), sxt, 3, 0x80008080, 0xffffffff80008080); \
    EXEC2_REPORT(test, glue3(EXEC, _, CHAN), sxt, 4, 0x80008080, 0x0000000000000080); \
    EXEC2_REPORT(test, glue3(EXEC, _, CHAN), sxt, 5, 0x80008080, 0x0000000000008080); \
    EXEC2_REPORT(test, glue3(EXEC, _, CHAN), sxt, 6, 0x80008080, 0x0000000080008080); \
    EXEC2_REPORT(test, glue3(EXEC, _, CHAN), sxt, 7, 0x80008080, 0x0000000080008080); \
    test_end(&test); \
} while(0)

static void test_sxt(void) {
    CHECK_SXT(EXEC_IR, 012345, "imm"); // specialized for imm
    CHECK_SXT(EXEC_RR, 012345, "reg"); // helper
}

static void test_shift(void) {
    CHECK2_CARTESIAN(EXEC_RR, 012345, shls, shift_src1, shift_src2);
    CHECK2_CARTESIAN(EXEC_RR, 012345, shrs, shift_src1, shift_src2);
    CHECK2_CARTESIAN(EXEC_RR, 012345, sars, shift_src1, shift_src2);
    CHECK2_CARTESIAN(EXEC_RR, 012345, scls, shift_src1, shift_src2);
    CHECK2_CARTESIAN(EXEC_RR, 012345, scrs, shift_src1, shift_src2);
    CHECK2_CARTESIAN(EXEC_RR, 012345, shld, shift_src1, shift_src2);
    CHECK2_CARTESIAN(EXEC_RR, 012345, shrd, shift_src1, shift_src2);
    CHECK2_CARTESIAN(EXEC_RR, 012345, sard, shift_src1, shift_src2);
    CHECK2_CARTESIAN(EXEC_RR, 012345, scld, shift_src1, shift_src2);
    CHECK2_CARTESIAN(EXEC_RR, 012345, scrd, shift_src1, shift_src2);
}

#define EXEC_MERGE(INSN, RES, SRC1, SRC2, SRC3, SRC4) asm( \
    "\t\tcmpedb 1, %[src3], %%pred0\n" \
    "\t{\n" \
    "\t    " #INSN ",0 %[src1], %[src2], %0, %%pred0\n" \
    "\t    " #INSN ",1 %[src1], %[src2], %1, %%pred0\n" \
    "\t    " #INSN ",2 %[src1], %[src2], %2, %%pred0\n" \
    "\t    " #INSN ",3 %[src1], %[src2], %3, %%pred0\n" \
    "\t    " #INSN ",4 %[src1], %[src2], %4, %%pred0\n" \
    "\t    " #INSN ",5 %[src1], %[src2], %5, %%pred0\n" \
    "\t}" \
    : "+r"(RES[0]), \
      "+r"(RES[1]), \
      "+r"(RES[2]), \
      "+r"(RES[3]), \
      "+r"(RES[4]), \
      "+r"(RES[5]) \
    : [src1]"rI"(SRC1), \
      [src2]"ri"(SRC2), \
      [src3]"ri"(SRC3)  \
    : "pred0" \
)

static void test_merge(void) {
    do {
        alc_test_t test = test_start("merges", NULL, ALC012345, SKIP_GEN | HAS_SRC123);
        EXEC_REPORT(test, EXEC_MERGE, merges, 1, -1, 0, 0, 0x00000001);
        EXEC_REPORT(test, EXEC_MERGE, merges, 1, -1, 1, 0, 0xffffffff);
        test_end(&test);
    } while(0);

    do {
        alc_test_t test = test_start("merged", NULL, ALC012345, SKIP_GEN | HAS_SRC123);
        EXEC_REPORT(test, EXEC_MERGE, merged, 1, -1ULL, 0, 0, 0x0000000000000001);
        EXEC_REPORT(test, EXEC_MERGE, merged, 1, -1ULL, 1, 0, 0xffffffffffffffff);
        test_end(&test);
    } while(0);
}

#define CHECK_GETF(EXEC, INSN, COMMENT, TYPE, IS_64) do { \
    alc_test_t test = test_start(#INSN, COMMENT, ALC012345, SKIP_GEN | HAS_SRC12); \
    EXEC2_REPORT(test, EXEC, INSN, 0x0000000000000000, FIELD( 0, 0, 0, 0), (TYPE) 0x0000000000000000); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD( 0, 4, 0, 0), (TYPE) 0x0000000000000000); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD( 4, 4, 0, 0), (TYPE) 0x0000000000000001); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD( 8, 4, 0, 0), (TYPE) 0x0000000000000002); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD(12, 4, 0, 0), (TYPE) 0x0000000000000003); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD(16, 4, 0, 0), (TYPE) 0x0000000000000004); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD(20, 4, 0, 0), (TYPE) 0x0000000000000005); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD(24, 4, 0, 0), (TYPE) 0x0000000000000006); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD(28, 4, 0, 0), (TYPE) 0x0000000000000008); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD( 0, 4, 1, 0), (TYPE) 0x0000000000000000); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD( 4, 4, 1, 0), (TYPE) 0x0000000000000001); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD( 8, 4, 1, 1), (TYPE) 0x0000000000000002); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD(12, 4, 1, 1), (TYPE) 0x0000000000000003); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD(16, 4, 1, 2), (TYPE) 0x0000000000000004); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD(20, 4, 1, 2), (TYPE) 0x0000000000000005); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD(24, 4, 1, 3), (TYPE) 0x0000000000000006); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD(28, 4, 1, 3), (TYPE) 0xfffffffffffffff8); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD( 0, 8, 0, 0), (TYPE) 0x0000000000000010); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD( 8, 8, 0, 0), (TYPE) 0x0000000000000032); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD(16, 8, 0, 0), (TYPE) 0x0000000000000054); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD(24, 8, 0, 0), (TYPE) 0x0000000000000086); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD( 0, 8, 1, 0), (TYPE) 0x0000000000000010); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD( 8, 8, 1, 1), (TYPE) 0x0000000000000032); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD(16, 8, 1, 2), (TYPE) 0x0000000000000054); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD(24, 8, 1, 3), (TYPE) 0xffffffffffffff86); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD( 0,16, 0, 0), (TYPE) 0x0000000000003210); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD(16,16, 0, 0), (TYPE) 0x0000000000008654); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD( 0,16, 1, 0), (TYPE) 0x0000000000003210); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD(16,16, 1, 3), (TYPE) 0xffffffffffff8654); \
    if (IS_64) { \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD(32, 4, 0, 0), (TYPE) 0x0000000000000007); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD(36, 4, 0, 0), (TYPE) 0x0000000000000009); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD(40, 4, 0, 0), (TYPE) 0x000000000000000a); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD(44, 4, 0, 0), (TYPE) 0x000000000000000b); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD(48, 4, 0, 0), (TYPE) 0x000000000000000c); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD(52, 4, 0, 0), (TYPE) 0x000000000000000d); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD(56, 4, 0, 0), (TYPE) 0x000000000000000e); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD(60, 4, 0, 0), (TYPE) 0x000000000000000f); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD(32, 4, 1, 4), (TYPE) 0x0000000000000007); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD(36, 4, 1, 4), (TYPE) 0xfffffffffffffff9); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD(40, 4, 1, 5), (TYPE) 0xfffffffffffffffa); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD(44, 4, 1, 5), (TYPE) 0xfffffffffffffffb); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD(48, 4, 1, 6), (TYPE) 0xfffffffffffffffc); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD(52, 4, 1, 6), (TYPE) 0xfffffffffffffffd); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD(56, 4, 1, 7), (TYPE) 0xfffffffffffffffe); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD(60, 4, 1, 7), (TYPE) 0xffffffffffffffff); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD(32, 8, 0, 0), (TYPE) 0x0000000000000097); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD(40, 8, 0, 0), (TYPE) 0x00000000000000ba); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD(48, 8, 0, 0), (TYPE) 0x00000000000000dc); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD(56, 8, 0, 0), (TYPE) 0x00000000000000fe); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD(32, 8, 1, 4), (TYPE) 0xffffffffffffff97); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD(40, 8, 1, 5), (TYPE) 0xffffffffffffffba); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD(48, 8, 1, 6), (TYPE) 0xffffffffffffffdc); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD(56, 8, 1, 7), (TYPE) 0xfffffffffffffffe); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD(32,16, 0, 0), (TYPE) 0x000000000000ba97); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD(48,16, 0, 0), (TYPE) 0x000000000000fedc); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD(32,16, 1, 4), (TYPE) 0xffffffffffffba97); \
    EXEC2_REPORT(test, EXEC, INSN, 0xfedcba9786543210, FIELD(48,16, 1, 6), (TYPE) 0xfffffffffffffedc); \
    } \
    test_end(&test); \
} while(0)

static void test_getf(void) {
    // specialized for literal
    CHECK_GETF(EXEC_RI_012345, getfs, "imm", uint32_t, 0);
    CHECK_GETF(EXEC_RI_012345, getfd, "imm", uint64_t, 1);
    // helper
    CHECK_GETF(EXEC_RR_012345, getfs, "reg", uint32_t, 0);
    CHECK_GETF(EXEC_RR_012345, getfd, "reg", uint64_t, 1);

#if 0
// TODO: e2k getfs/getfd forward compatibility issue
#if __iset__ >= 5
    // v5+ does not follow older rules for src2[15:13]
    CHECK2_ALL(EXEC, INSN, 0x0000000000808080, FIELD( 0,32, 1, 0), (TYPE) 0x0000000000808080);
    CHECK2_ALL(EXEC, INSN, 0x0000000000808080, FIELD( 0,32, 1, 1), (TYPE) 0x0000000000808080);
    CHECK2_ALL(EXEC, INSN, 0x0000000000808080, FIELD( 0,32, 1, 2), (TYPE) 0x0000000000808080);
    CHECK2_ALL(EXEC, INSN, 0x0000000000808080, FIELD( 0,32, 1, 3), (TYPE) 0x0000000000808080);

    CHECK2_ALL(EXEC, INSN, 0x0000000000010101, FIELD( 1,32, 1, 0), (TYPE) 0x0000000000008080);
    CHECK2_ALL(EXEC, INSN, 0x0000000000010101, FIELD( 1,32, 1, 1), (TYPE) 0x0000000000008080);
    CHECK2_ALL(EXEC, INSN, 0x0000000000010101, FIELD( 1,32, 1, 2), (TYPE) 0x0000000000008080);
    CHECK2_ALL(EXEC, INSN, 0x0000000000010101, FIELD( 1,32, 1, 3), (TYPE) 0x0000000000008080);
#else
    CHECK2_ALL(EXEC, INSN, 0x0000000000808080, FIELD( 0,32, 1, 0), (TYPE) 0xffffffff00808080);
    CHECK2_ALL(EXEC, INSN, 0x0000000000808080, FIELD( 0,32, 1, 1), (TYPE) 0xffffffff00808080);
    CHECK2_ALL(EXEC, INSN, 0x0000000000808080, FIELD( 0,32, 1, 2), (TYPE) 0xffffffff00808080);
    CHECK2_ALL(EXEC, INSN, 0x0000000000808080, FIELD( 0,32, 1, 3), (TYPE) 0x0000000000808080);

    CHECK2_ALL(EXEC, INSN, 0x0000000000010101, FIELD( 1,32, 1, 0), (TYPE) 0xffffffff00008080);
    CHECK2_ALL(EXEC, INSN, 0x0000000000010101, FIELD( 1,32, 1, 1), (TYPE) 0xffffffff00008080);
    CHECK2_ALL(EXEC, INSN, 0x0000000000010101, FIELD( 1,32, 1, 2), (TYPE) 0xffffffff00008080);
    CHECK2_ALL(EXEC, INSN, 0x0000000000010101, FIELD( 1,32, 1, 3), (TYPE) 0x0000000000008080);
#endif
#endif
}

#define EXEC_CMP_0134(INSN, RES, SRC1, SRC2, SRC3, SRC4) asm( \
    "\t{\n" \
    "\t    " #INSN ",0 %[src1], %[src2], %%pred0\n" \
    "\t    " #INSN ",1 %[src1], %[src2], %%pred1\n" \
    "\t    " #INSN ",3 %[src1], %[src2], %%pred2\n" \
    "\t    " #INSN ",4 %[src1], %[src2], %%pred3\n" \
    "\t}\n" \
    "\t{\n" \
    "\t    merged,0 0, 1, %0, %%pred0\n" \
    "\t    merged,1 0, 1, %1, %%pred1\n" \
    "\t    merged,3 0, 1, %2, %%pred2\n" \
    "\t    merged,4 0, 1, %3, %%pred3\n" \
    "\t}" \
    : "+r"(RES[0]), \
      "+r"(RES[1]), \
      "+r"(RES[2]), \
      "+r"(RES[3])  \
    : [src1]"rI"(SRC1), \
      [src2]"ri"(SRC2), \
      [src3]"ri"(SRC3)  \
    : "pred0", "pred1", "pred2", "pred3" \
)

static void test_cmp(void) {
    CHECK2_CARTESIAN(EXEC_CMP, 0134, cmposb,     cmp_src, cmp_src);
    CHECK2_CARTESIAN(EXEC_CMP, 0134, cmpbsb,     cmp_src, cmp_src);
    CHECK2_CARTESIAN(EXEC_CMP, 0134, cmpesb,     cmp_src, cmp_src);
    CHECK2_CARTESIAN(EXEC_CMP, 0134, cmpbesb,    cmp_src, cmp_src);
    CHECK2_CARTESIAN(EXEC_CMP, 0134, cmpssb,     cmp_src, cmp_src);
    CHECK2_CARTESIAN(EXEC_CMP, 0134, cmppsb,     cmp_src, cmp_src);
    CHECK2_CARTESIAN(EXEC_CMP, 0134, cmplsb,     cmp_src, cmp_src);
    CHECK2_CARTESIAN(EXEC_CMP, 0134, cmplesb,    cmp_src, cmp_src);

    CHECK2_CARTESIAN(EXEC_CMP, 0134, cmpodb,     cmp_src, cmp_src);
    CHECK2_CARTESIAN(EXEC_CMP, 0134, cmpbdb,     cmp_src, cmp_src);
    CHECK2_CARTESIAN(EXEC_CMP, 0134, cmpedb,     cmp_src, cmp_src);
    CHECK2_CARTESIAN(EXEC_CMP, 0134, cmpbedb,    cmp_src, cmp_src);
    CHECK2_CARTESIAN(EXEC_CMP, 0134, cmpsdb,     cmp_src, cmp_src);
    CHECK2_CARTESIAN(EXEC_CMP, 0134, cmppdb,     cmp_src, cmp_src);
    CHECK2_CARTESIAN(EXEC_CMP, 0134, cmpldb,     cmp_src, cmp_src);
    CHECK2_CARTESIAN(EXEC_CMP, 0134, cmpledb,    cmp_src, cmp_src);
}

static void test_cmpand(void) {
    CHECK2_CARTESIAN(EXEC_CMP, 0134, cmpandesb,  cmp_src, cmp_src);
    CHECK2_CARTESIAN(EXEC_CMP, 0134, cmpandssb,  cmp_src, cmp_src);
    CHECK2_CARTESIAN(EXEC_CMP, 0134, cmpandpsb,  cmp_src, cmp_src);
    CHECK2_CARTESIAN(EXEC_CMP, 0134, cmpandlesb, cmp_src, cmp_src);

    CHECK2_CARTESIAN(EXEC_CMP, 0134, cmpandedb,  cmp_src, cmp_src);
    CHECK2_CARTESIAN(EXEC_CMP, 0134, cmpandsdb,  cmp_src, cmp_src);
    CHECK2_CARTESIAN(EXEC_CMP, 0134, cmpandpdb,  cmp_src, cmp_src);
    CHECK2_CARTESIAN(EXEC_CMP, 0134, cmpandledb, cmp_src, cmp_src);
}

#define EXEC_RRR_14(INSN, RES, SRC1, SRC2, SRC3, SRC4) asm( \
    "\t{\n" \
    "\t    " #INSN ",1 %[src1], %[src2], %[src3], %0\n" \
    "\t    " #INSN ",4 %[src1], %[src2], %[src3], %1\n" \
    "\t}" \
    : "+r"(RES[0]), \
      "+r"(RES[1])  \
    : [src1]"r"(SRC1), \
      [src2]"r"(SRC2), \
      [src3]"r"(SRC3)  \
)

#define TEST_COMB(EXEC, CHAN, OP) do {\
    CHECK3(EXEC, CHAN, glue(and_,  OP), int_src1, int_src2, int_src3); \
    CHECK3(EXEC, CHAN, glue(andn_, OP), int_src1, int_src2, int_src3); \
    CHECK3(EXEC, CHAN, glue(or_,   OP), int_src1, int_src2, int_src3); \
    CHECK3(EXEC, CHAN, glue(orn_,  OP), int_src1, int_src2, int_src3); \
    CHECK3(EXEC, CHAN, glue(xor_,  OP), int_src1, int_src2, int_src3); \
    CHECK3(EXEC, CHAN, glue(xorn_, OP), int_src1, int_src2, int_src3); \
    CHECK3(EXEC, CHAN, glue(add_,  OP), int_src1, int_src2, int_src3); \
    CHECK3(EXEC, CHAN, glue(sub_,  OP), int_src1, int_src2, int_src3); \
    CHECK3(EXEC, CHAN, glue(scl_,  OP), int_src1, int_src2, int_src3); \
    CHECK3(EXEC, CHAN, glue(scr_,  OP), int_src1, int_src2, int_src3); \
    CHECK3(EXEC, CHAN, glue(shl_,  OP), int_src1, int_src2, int_src3); \
    CHECK3(EXEC, CHAN, glue(shr_,  OP), int_src1, int_src2, int_src3); \
    CHECK3(EXEC, CHAN, glue(sar_,  OP), int_src1, int_src2, int_src3); \
} while(0)

static void test_comb(void) {
    TEST_COMB(EXEC_RRR, 14, ands);
    TEST_COMB(EXEC_RRR, 14, andns);
    TEST_COMB(EXEC_RRR, 14, ors);
    TEST_COMB(EXEC_RRR, 14, orns);
    TEST_COMB(EXEC_RRR, 14, xors);
    TEST_COMB(EXEC_RRR, 14, xorns);
    TEST_COMB(EXEC_RRR, 14, rsubs);
    TEST_COMB(EXEC_RRR, 14, adds);
    TEST_COMB(EXEC_RRR, 14, subs);

    TEST_COMB(EXEC_RRR, 14, andd);
    TEST_COMB(EXEC_RRR, 14, andnd);
    TEST_COMB(EXEC_RRR, 14, ord);
    TEST_COMB(EXEC_RRR, 14, ornd);
    TEST_COMB(EXEC_RRR, 14, xord);
    TEST_COMB(EXEC_RRR, 14, xornd);
    TEST_COMB(EXEC_RRR, 14, rsubd);
    TEST_COMB(EXEC_RRR, 14, addd);
    TEST_COMB(EXEC_RRR, 14, subd);

    // TODO: e2k comb merge_*
    //
    // merge_ands
    // merge_andd
    // merge_andns
    // merge_andnd
    // merge_ors
    // merge_ord
    // merge_orns
    // merge_ornd
    // merge_xors
    // merge_xord
    // merge_xorns
    // merge_xornd
    // merge_rsubs
    // merge_rsubd
    // merge_adds
    // merge_addd
    // merge_subs
    // merge_subd

    // TODO: e2k comb getf_*
    //
    // getf_ands
    // getf_andd
    // getf_andns
    // getf_andnd
    // getf_ors
    // getf_ord
    // getf_orns
    // getf_ornd
    // getf_xors
    // getf_xord
    // getf_xorns
    // getf_xornd
    // getf_rsubs
    // getf_rsubd
    // getf_adds
    // getf_addd
    // getf_subs
    // getf_subd

    // TODO: implemented, v1 only, no support in toolchain
    //
    // and_merges
    // and_merged
    // andn_merges
    // andn_merged
    // or_merges
    // or_merged
    // orn_merges
    // orn_merged
    // xor_merges
    // xor_merged
    // xorn_merges
    // xorn_merged
    // merge_merges
    // merge_merged
    // add_merges
    // add_merged
    // sub_merges
    // sub_merged
    // scl_merges
    // scl_merged
    // scr_merges
    // scr_merged
    // shl_merges
    // shl_merged
    // shr_merges
    // shr_merged
    // sar_merges
    // sar_merged
    // getf_merges
    // getf_merged
    // and_scls
    // and_scld
    // andn_scls
    // andn_scld
    // or_scls
    // or_scld
    // orn_scls
    // orn_scld
    // xor_scls
    // xor_scld
    // xorn_scls
    // xorn_scld
    // merge_scls
    // merge_scld
    // add_scls
    // add_scld
    // sub_scls
    // sub_scld
    // scl_scls
    // scl_scld
    // scr_scls
    // scr_scld
    // shl_scls
    // shl_scld
    // shr_scls
    // shr_scld
    // sar_scls
    // sar_scld
    // getf_scls
    // getf_scld
    // and_scrs
    // and_scrd
    // andn_scrs
    // andn_scrd
    // or_scrs
    // or_scrd
    // orn_scrs
    // orn_scrd
    // xor_scrs
    // xor_scrd
    // xorn_scrs
    // xorn_scrd
    // merge_scrs
    // merge_scrd
    // add_scrs
    // add_scrd
    // sub_scrs
    // sub_scrd
    // scl_scrs
    // scl_scrd
    // scr_scrs
    // scr_scrd
    // shl_scrs
    // shl_scrd
    // shr_scrs
    // shr_scrd
    // sar_scrs
    // sar_scrd
    // getf_scrs
    // getf_scrd
    // and_shls
    // and_shld
    // andn_shls
    // andn_shld
    // or_shls
    // or_shld
    // orn_shls
    // orn_shld
    // xor_shls
    // xor_shld
    // xorn_shls
    // xorn_shld
    // merge_shls
    // merge_shld
    // add_shls
    // add_shld
    // sub_shls
    // sub_shld
    // scl_shls
    // scl_shld
    // scr_shls
    // scr_shld
    // shl_shls
    // shl_shld
    // shr_shls
    // shr_shld
    // sar_shls
    // sar_shld
    // getf_shls
    // getf_shld
    // and_shrs
    // and_shrd
    // andn_shrs
    // andn_shrd
    // or_shrs
    // or_shrd
    // orn_shrs
    // orn_shrd
    // xor_shrs
    // xor_shrd
    // xorn_shrs
    // xorn_shrd
    // merge_shrs
    // merge_shrd
    // add_shrs
    // add_shrd
    // sub_shrs
    // sub_shrd
    // scl_shrs
    // scl_shrd
    // scr_shrs
    // scr_shrd
    // shl_shrs
    // shl_shrd
    // shr_shrs
    // shr_shrd
    // sar_shrs
    // sar_shrd
    // getf_shrs
    // getf_shrd
    // and_sars
    // and_sard
    // andn_sars
    // andn_sard
    // or_sars
    // or_sard
    // orn_sars
    // orn_sard
    // xor_sars
    // xor_sard
    // xorn_sars
    // xorn_sard
    // merge_sars
    // merge_sard
    // add_sars
    // add_sard
    // sub_sars
    // sub_sard
    // scl_sars
    // scl_sard
    // scr_sars
    // scr_sard
    // shl_sars
    // shl_sard
    // shr_sars
    // shr_sard
    // sar_sars
    // sar_sard
    // getf_sars
    // getf_sard
    // and_getfs
    // and_getfd
    // andn_getfs
    // andn_getfd
    // or_getfs
    // or_getfd
    // orn_getfs
    // orn_getfd
    // xor_getfs
    // xor_getfd
    // xorn_getfs
    // xorn_getfd
    // merge_getfs
    // merge_getfd
    // add_getfs
    // add_getfd
    // sub_getfs
    // sub_getfd
    // scl_getfs
    // scl_getfd
    // scr_getfs
    // scr_getfd
    // shl_getfs
    // shl_getfd
    // shr_getfs
    // shr_getfd
    // sar_getfs
    // sar_getfd
    // getf_getfs
    // getf_getfd

    // TODO: binary translation for x86
    //
    // ands_fb
    // andns_fb
    // ors_fb
    // orns_fb
    // xors_fb
    // xorns_fb
    // adds_fb
    // subs_fb
    // scls_fb
    // scrs_fb
    // shls_fb
    // shrs_fb
    // sars_fb
    // umulx_fb
    // smulx_fb
    // incs_fb
    // decs_fb
    // ands_fh
    // andns_fh
    // ors_fh
    // orns_fh
    // xors_fh
    // xorns_fh
    // adds_fh
    // subs_fh
    // scls_fh
    // scrs_fh
    // shls_fh
    // shrs_fh
    // sars_fh
    // umulx_fh
    // smulx_fh
    // incs_fh
    // decs_fh
    // ands_fw
    // andns_fw
    // ors_fw
    // orns_fw
    // xors_fw
    // xorns_fw
    // adds_fw
    // subs_fw
    // scls_fw
    // scrs_fw
    // shls_fw
    // shrs_fw
    // sars_fw
    // umulx_fw
    // smulx_fw
    // incs_fw
    // decs_fw
}

static void test_packed(void) {
    CHECK2(EXEC_RR, 0134, pandd,    int_src1, int_src2);
    CHECK2(EXEC_RR, 0134, pandnd,   int_src1, int_src2);
    CHECK2(EXEC_RR, 0134, pord,     int_src1, int_src2);
    CHECK2(EXEC_RR, 0134, pxord,    int_src1, int_src2);

    CHECK2(EXEC_RR, 03, pminub,     int_src1, int_src2);
    CHECK2(EXEC_RR, 03, pminsh,     int_src1, int_src2);
    CHECK2(EXEC_RR, 03, pmaxub,     int_src1, int_src2);
    CHECK2(EXEC_RR, 03, pmaxsh,     int_src1, int_src2);
    CHECK2(EXEC_RR, 03, paddb,      int_src1, int_src2);
    CHECK2(EXEC_RR, 03, paddh,      int_src1, int_src2);
    CHECK2(EXEC_RR, 03, paddsb,     int_src1, int_src2);
    CHECK2(EXEC_RR, 03, paddsh,     int_src1, int_src2);
    CHECK2(EXEC_RR, 03, paddusb,    int_src1, int_src2);
    CHECK2(EXEC_RR, 03, paddush,    int_src1, int_src2);
    CHECK2(EXEC_RR, 03, paddw,      int_src1, int_src2);
    CHECK2(EXEC_RR, 03, paddd,      int_src1, int_src2);
    CHECK2(EXEC_RR, 03, psubb,      int_src1, int_src2);
    CHECK2(EXEC_RR, 03, psubh,      int_src1, int_src2);
    CHECK2(EXEC_RR, 03, psubsb,     int_src1, int_src2);
    CHECK2(EXEC_RR, 03, psubsh,     int_src1, int_src2);
    CHECK2(EXEC_RR, 03, psubusb,    int_src1, int_src2);
    CHECK2(EXEC_RR, 03, psubush,    int_src1, int_src2);
    CHECK2(EXEC_RR, 03, psubw,      int_src1, int_src2);
    CHECK2(EXEC_RR, 03, psubd,      int_src1, int_src2);

    CHECK2(EXEC_RR, 03, pcmpeqb,    int_src1, int_src2);
    CHECK2(EXEC_RR, 03, pcmpeqh,    int_src1, int_src2);
    CHECK2(EXEC_RR, 03, pcmpeqw,    int_src1, int_src2);
    CHECK2(EXEC_RR, 03, pcmpgtb,    int_src1, int_src2);
    CHECK2(EXEC_RR, 03, pcmpgth,    int_src1, int_src2);
    CHECK2(EXEC_RR, 03, pcmpgtw,    int_src1, int_src2);
    CHECK2(EXEC_RR, 03, pavgusb,    int_src1, int_src2);
    CHECK2(EXEC_RR, 03, pavgush,    int_src1, int_src2);

    CHECK2(EXEC_RR, 14, punpckhbh,  int_src1, int_src2);
    CHECK2(EXEC_RR, 14, punpcklbh,  int_src1, int_src2);
    CHECK2(EXEC_RR, 14, punpckhhw,  int_src1, int_src2);
    CHECK2(EXEC_RR, 14, punpcklhw,  int_src1, int_src2);
    CHECK2(EXEC_RR, 14, punpckhwd,  int_src1, int_src2);
    CHECK2(EXEC_RR, 14, punpcklwd,  int_src1, int_src2);
    CHECK2(EXEC_RR, 14, pmovmskps,  int_src1, int_src2);
    CHECK2(EXEC_RR, 14, pmovmskpd,  int_src1, int_src2);
    CHECK2(EXEC_RR, 14, packsshb,   int_src1, int_src2);
    CHECK2(EXEC_RR, 14, packushb,   int_src1, int_src2);
    CHECK2(EXEC_RR, 14, packsswh,   int_src1, int_src2);
    CHECK2(EXEC_RR, 14, pmovmskb,   int_src1, int_src2);

    CHECK2(EXEC_RR, 14, pmulhh,     int_src1, int_src2);
    CHECK2(EXEC_RR, 14, pmullh,     int_src1, int_src2);
    CHECK2(EXEC_RR, 14, pmaddh,     int_src1, int_src2);
    CHECK2(EXEC_RR, 14, pmulhuh,    int_src1, int_src2);
    CHECK2(EXEC_RR, 14, psadbw,     int_src1, int_src2);

    // TODO: psrlqh
    // TODO: psrlql
    // TODO: psllqh
    // TODO: psllql
    // TODO: psrlw
    // TODO: psrlh
    // TODO: psraw
    // TODO: psrah
    // TODO: psllw
    // TODO: psllh
    // TODO: psrld
    // TODO: pslld

    // TODO: pshufw
    // TODO: pshufh

    // TODO: pextrh
    // TODO: pinsh
}

int main(int argc, char *argv[]) {
    parse_args(argc, argv);

    test_int();
    test_sxt();
    test_shift();
    test_merge();
    test_getf();
    test_cmp();
    test_cmpand();
    test_comb();
    test_packed();

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

    // TODO: insfs
    // TODO: insfd

    // TODO: movtq
    // TODO: movtcq
    // TODO: movtrq
    // TODO: movtrcq

    return is_failed();
}
