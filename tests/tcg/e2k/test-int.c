#include "iset.h"
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
    0x1234567812345678, 0x8765432187654321, 0xaaaaaaaa55555555, 0xdeadbeef12345678,
};

static uint64_t shift_src2[] = {
    0, 1, 2, 31, 32, 47, 48, 63, 64,
};

static uint64_t cmp_src[] = {
    0x0000000000000000, 0x0000000000000001, 0x000000007fffffff, 0x0000000080000000,
    0x00000000deadbeef, 0x00000000ffffffff, 0x7fffffffffffffff, 0x8000000000000000,
    0xdeadbeefdeadbeef, 0xffffffffffffffff,
};

static uint64_t bit_src1[] = {
    0x0000000012345678, 0x0000000000012341, 0xffffffffffffffff, 0x7fffffffffffffff,
    0x8000000000000000, 0xaaaaaaaa12345678, 0xaaaaaaaa00012341, 0xaaaaaaaaffffffff,
    0xaaaaaaaa7fffffff, 0xaaaaaaaa80000000, 0x123456789abcdef0,
};

#ifndef DUMP_ONLY
#include "test-int-expect.inc"
#endif

static void test_v1_int(void) {
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

static void test_v1_sxt(void) {
    CHECK_SXT(EXEC_IR, 012345, "imm"); // specialized for imm
    CHECK_SXT(EXEC_RR, 012345, "reg"); // helper
}

static void test_v1_shift(void) {
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
    "\t\tcmpesb 1, %[src3], %%pred0\n" \
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

static void test_v1_merge(void) {
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

static void test_v1_getf(void) {
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

static void test_v1_cmp(void) {
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

static void test_v1_cmpand(void) {
    CHECK2_CARTESIAN(EXEC_CMP, 0134, cmpandesb,  cmp_src, cmp_src);
    CHECK2_CARTESIAN(EXEC_CMP, 0134, cmpandssb,  cmp_src, cmp_src);
    CHECK2_CARTESIAN(EXEC_CMP, 0134, cmpandpsb,  cmp_src, cmp_src);
    CHECK2_CARTESIAN(EXEC_CMP, 0134, cmpandlesb, cmp_src, cmp_src);

    CHECK2_CARTESIAN(EXEC_CMP, 0134, cmpandedb,  cmp_src, cmp_src);
    CHECK2_CARTESIAN(EXEC_CMP, 0134, cmpandsdb,  cmp_src, cmp_src);
    CHECK2_CARTESIAN(EXEC_CMP, 0134, cmpandpdb,  cmp_src, cmp_src);
    CHECK2_CARTESIAN(EXEC_CMP, 0134, cmpandledb, cmp_src, cmp_src);
}

#define EXEC_COMB_MERGE(INSN, RES, SRC1, SRC2, SRC3, SRC4) asm( \
    "\t\tcmpesb 1, %[src4], %%pred0\n" \
    "\t{\n" \
    "\t    " #INSN ",1 %[src1], %[src2], %[src3], %0, %%pred0\n" \
    "\t    " #INSN ",4 %[src1], %[src2], %[src3], %1, %%pred0\n" \
    "\t}" \
    : "+r"(RES[0]), \
      "+r"(RES[1]) \
    : [src1]"rI"(SRC1), \
      [src2]"ri"(SRC2), \
      [src3]"r"(SRC3), \
      [src4]"ri"(SRC4)  \
    : "pred0" \
)

#define TEST3_MERGE(INSN, SRC1, SRC2, SRC3, EXPECT, GET_EXPECT) do { \
    ASSERT_ARRAY_LEN_EQ(SRC1, SRC2); \
    ASSERT_ARRAY_LEN_EQ(SRC1, SRC3); \
    alc_test_t test = test_start(#INSN, NULL, ALC14, HAS_SRC1234); \
    for (int i = 0, k = 0; i < ARRAY_LEN(SRC1); ++i) { \
        for (int j = 0; j < 2; ++j, ++k) { \
            EXEC_COMB_MERGE(INSN, test.result, SRC1[i], SRC2[i], SRC3[i], j); \
            uint64_t expected = GET_EXPECT(&test, EXPECT, k); \
            test_report(&test, expected, SRC1[i], SRC2[i], SRC3[i], j); \
        } \
    } \
    test_end(&test); \
} while(0)

#define CHECK3_MERGE(INSN, SRC1, SRC2, SRC3) \
    TEST3_MERGE(INSN, SRC1, SRC2, SRC3, glue(INSN, _expect), GET_EXPECT)

#define TEST_COMB(OP) do { \
    CHECK3(EXEC_RRR, 14, glue(and_,  OP), int_src1, int_src2, int_src3); \
    CHECK3(EXEC_RRR, 14, glue(andn_, OP), int_src1, int_src2, int_src3); \
    CHECK3(EXEC_RRR, 14, glue(or_,   OP), int_src1, int_src2, int_src3); \
    CHECK3(EXEC_RRR, 14, glue(orn_,  OP), int_src1, int_src2, int_src3); \
    CHECK3(EXEC_RRR, 14, glue(xor_,  OP), int_src1, int_src2, int_src3); \
    CHECK3(EXEC_RRR, 14, glue(xorn_, OP), int_src1, int_src2, int_src3); \
    CHECK3_MERGE(glue(merge_, OP), int_src1, int_src2, int_src3); \
    CHECK3(EXEC_RRR, 14, glue(add_,  OP), int_src1, int_src2, int_src3); \
    CHECK3(EXEC_RRR, 14, glue(sub_,  OP), int_src1, int_src2, int_src3); \
    CHECK3(EXEC_RRR, 14, glue(scl_,  OP), int_src1, int_src2, int_src3); \
    CHECK3(EXEC_RRR, 14, glue(scr_,  OP), int_src1, int_src2, int_src3); \
    CHECK3(EXEC_RRR, 14, glue(shl_,  OP), int_src1, int_src2, int_src3); \
    CHECK3(EXEC_RRR, 14, glue(shr_,  OP), int_src1, int_src2, int_src3); \
    CHECK3(EXEC_RRR, 14, glue(sar_,  OP), int_src1, int_src2, int_src3); \
} while(0)

static void test_v1_comb(void) {
    TEST_COMB(ands);
    TEST_COMB(andns);
    TEST_COMB(ors);
    TEST_COMB(orns);
    TEST_COMB(xors);
    TEST_COMB(xorns);
    TEST_COMB(rsubs);
    TEST_COMB(adds);
    TEST_COMB(subs);

    TEST_COMB(andd);
    TEST_COMB(andnd);
    TEST_COMB(ord);
    TEST_COMB(ornd);
    TEST_COMB(xord);
    TEST_COMB(xornd);
    TEST_COMB(rsubd);
    TEST_COMB(addd);
    TEST_COMB(subd);

    // TODO: e2k comb getf_*

    // TODO: implemented, v1 only, no support in toolchain
    //
    // *_merges
    // *_merged
    // *_scls
    // *_scld
    // *_scrs
    // *_scrd
    // *_shls
    // *_shld
    // *_shrs
    // *_shrd
    // *_sars
    // *_sard
    // *_getfs
    // *_getfd

    // TODO: binary translation for x86
    //
    // *_fb
    // *_fh
    // *_fw
}

static void test_v1(void) {
    test_v1_int();
    test_v1_sxt();
    test_v1_shift();
    test_v1_merge();
    test_v1_getf();
    test_v1_cmp();
    test_v1_cmpand();
    test_v1_comb();

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
}

static void test_v2(void) {
    push_iset(2);

    CHECK1(EXEC_R, 0134, bitrevs, bit_src1);
    CHECK1(EXEC_R, 0134, bitrevd, bit_src1);

    CHECK1(EXEC_R, 14, lzcnts, bit_src1);
    CHECK1(EXEC_R, 14, lzcntd, bit_src1);
    CHECK1(EXEC_R, 14, popcnts, bit_src1);
    CHECK1(EXEC_R, 14, popcntd, bit_src1);

    pop_iset();
}

static void test_v3(void) {
    push_iset(3);

    CHECK2(EXEC_RR, 0134, umulhd, int_src1, int_src2);
    CHECK2(EXEC_RR, 0134, smulhd, int_src1, int_src2);

    // TODO: puttst

    // TODO: *_fd, x86 binary translation

    pop_iset();
}

static void test_v5(void) {
    push_iset(5);

    // TODO: getfzs
    // TODO: getfzd

    CHECK3(EXEC_RRR, 14, addcd,   int_src1, int_src2, int_src3);
    CHECK3(EXEC_RRR, 14, addcd_c, int_src1, int_src2, int_src3);
    CHECK3(EXEC_RRR, 14, subcd,   int_src1, int_src2, int_src3);
    CHECK3(EXEC_RRR, 14, subcd_c, int_src1, int_src2, int_src3);

    pop_iset();
}

static void test_v6(void) {
    push_iset(6);

    // TODO: clmull
    // TODO: clmulh
    // TODO: ibranchd
    // TODO: icalld

    pop_iset();
}

int main(int argc, char *argv[]) {
    parse_args(argc, argv);

    test_v1();
    if_iset(2, "v2") test_v2();
    if_iset(3, "v3") test_v3();
    if_iset(5, "v5") test_v5();
    if_iset(6, "v6") test_v6();

    return is_failed();
}
