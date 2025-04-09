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

static uint64_t pshift_src1[] = {
    0x1234567812345678, 0x123456789abcdef0, 0xfafafafafafafafa, 0xffffffffffffffff,
};

static uint64_t pshift_src2[] = {
    0xcccccccceeeeeeee, 0x1122334455667788, 0xaabbaabbaabbaabb, 0x0000000000000000,
};

#ifndef DUMP_ONLY
#include "test-vec-expect.inc"
#endif

#define CHECK_RI_14(TEST, INSN, SRC1, SRC2, K) do { \
    EXEC_RI_14(INSN, (TEST)->result, SRC1, SRC2, 0, 0); \
    uint64_t expected = GET_EXPECT(TEST, glue(INSN, _expect), K); \
    test_report(TEST, expected, SRC1, SRC2, 0, 0); \
} while(0)

#define CHECK_RRI_14(TEST, INSN, SRC1, SRC2, SRC3, K) do { \
    EXEC_RRI_14(INSN, (TEST)->result, SRC1, SRC2, SRC3, 0); \
    uint64_t expected = GET_EXPECT(TEST, glue(INSN, _expect), K); \
    test_report(TEST, expected, SRC1, SRC2, SRC3, 0); \
} while(0)

static void test_v1_packed(void) {
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
}

static void test_v1_shuffle(void) {
    do {
        alc_test_t test = test_start("pshufw", NULL, ALC14, HAS_SRC123);
        uint64_t src1 = 0x1122334455667788;
        uint64_t src2 = 0xffeeddccbbaa9900;
        int k = 0;
        CHECK_RRI_14(&test, pshufw, src1, src2, 0x0, k); ++k;
        CHECK_RRI_14(&test, pshufw, src1, src2, 0x1, k); ++k;
        CHECK_RRI_14(&test, pshufw, src1, src2, 0x2, k); ++k;
        CHECK_RRI_14(&test, pshufw, src1, src2, 0x3, k); ++k;
        CHECK_RRI_14(&test, pshufw, src1, src2, 0x4, k); ++k;
        CHECK_RRI_14(&test, pshufw, src1, src2, 0x5, k); ++k;
        CHECK_RRI_14(&test, pshufw, src1, src2, 0x6, k); ++k;
        CHECK_RRI_14(&test, pshufw, src1, src2, 0x7, k); ++k;
        CHECK_RRI_14(&test, pshufw, src1, src2, 0x8, k); ++k;
        CHECK_RRI_14(&test, pshufw, src1, src2, 0x9, k); ++k;
        CHECK_RRI_14(&test, pshufw, src1, src2, 0xa, k); ++k;
        CHECK_RRI_14(&test, pshufw, src1, src2, 0xb, k); ++k;
        CHECK_RRI_14(&test, pshufw, src1, src2, 0xc, k); ++k;
        CHECK_RRI_14(&test, pshufw, src1, src2, 0xd, k); ++k;
        CHECK_RRI_14(&test, pshufw, src1, src2, 0xe, k); ++k;
        CHECK_RRI_14(&test, pshufw, src1, src2, 0xf, k); ++k;
        test_end(&test);
    } while(0);

    do {
        alc_test_t test = test_start("pshufh", NULL, ALC14, HAS_SRC12);
        uint64_t src1 = 0x1122334455667788;
        int k = 0;
        CHECK_RI_14(&test, pshufh, src1, 0x00, k); ++k;
        CHECK_RI_14(&test, pshufh, src1, 0x01, k); ++k;
        CHECK_RI_14(&test, pshufh, src1, 0x02, k); ++k;
        CHECK_RI_14(&test, pshufh, src1, 0x03, k); ++k;
        CHECK_RI_14(&test, pshufh, src1, 0x04, k); ++k;
        CHECK_RI_14(&test, pshufh, src1, 0x05, k); ++k;
        CHECK_RI_14(&test, pshufh, src1, 0x06, k); ++k;
        CHECK_RI_14(&test, pshufh, src1, 0x07, k); ++k;
        CHECK_RI_14(&test, pshufh, src1, 0x08, k); ++k;
        CHECK_RI_14(&test, pshufh, src1, 0x09, k); ++k;
        CHECK_RI_14(&test, pshufh, src1, 0x0a, k); ++k;
        CHECK_RI_14(&test, pshufh, src1, 0x0b, k); ++k;
        CHECK_RI_14(&test, pshufh, src1, 0x0c, k); ++k;
        CHECK_RI_14(&test, pshufh, src1, 0x0d, k); ++k;
        CHECK_RI_14(&test, pshufh, src1, 0x0e, k); ++k;
        CHECK_RI_14(&test, pshufh, src1, 0x0f, k); ++k;
        CHECK_RI_14(&test, pshufh, src1, 0x00, k); ++k;
        CHECK_RI_14(&test, pshufh, src1, 0x10, k); ++k;
        CHECK_RI_14(&test, pshufh, src1, 0x20, k); ++k;
        CHECK_RI_14(&test, pshufh, src1, 0x30, k); ++k;
        CHECK_RI_14(&test, pshufh, src1, 0x40, k); ++k;
        CHECK_RI_14(&test, pshufh, src1, 0x50, k); ++k;
        CHECK_RI_14(&test, pshufh, src1, 0x60, k); ++k;
        CHECK_RI_14(&test, pshufh, src1, 0x70, k); ++k;
        CHECK_RI_14(&test, pshufh, src1, 0x80, k); ++k;
        CHECK_RI_14(&test, pshufh, src1, 0x90, k); ++k;
        CHECK_RI_14(&test, pshufh, src1, 0xa0, k); ++k;
        CHECK_RI_14(&test, pshufh, src1, 0xb0, k); ++k;
        CHECK_RI_14(&test, pshufh, src1, 0xc0, k); ++k;
        CHECK_RI_14(&test, pshufh, src1, 0xd0, k); ++k;
        CHECK_RI_14(&test, pshufh, src1, 0xe0, k); ++k;
        CHECK_RI_14(&test, pshufh, src1, 0xf0, k); ++k;
        CHECK_RI_14(&test, pshufh, src1, 0xf7, k); ++k;
        CHECK_RI_14(&test, pshufh, src1, 0x7f, k); ++k;
        CHECK_RI_14(&test, pshufh, src1, 0x7f, k); ++k;
        CHECK_RI_14(&test, pshufh, src1, 0xf7, k); ++k;
        test_end(&test);
    } while(0);

    do {
        alc_test_t test = test_start("pextrh", NULL, ALC14, HAS_SRC123);
        uint64_t src1 = 0x1122334455667788;
        uint64_t src2 = 0xffeeddccbbaa9900;
        int k = 0;
        CHECK_RRI_14(&test, pextrh, src1, src2, 0x0, k); ++k;
        CHECK_RRI_14(&test, pextrh, src1, src2, 0x1, k); ++k;
        CHECK_RRI_14(&test, pextrh, src1, src2, 0x2, k); ++k;
        CHECK_RRI_14(&test, pextrh, src1, src2, 0x3, k); ++k;
        CHECK_RRI_14(&test, pextrh, src1, src2, 0x4, k); ++k;
        CHECK_RRI_14(&test, pextrh, src1, src2, 0x5, k); ++k;
        CHECK_RRI_14(&test, pextrh, src1, src2, 0x6, k); ++k;
        CHECK_RRI_14(&test, pextrh, src1, src2, 0x7, k); ++k;
        test_end(&test);
    } while(0);

    do {
        alc_test_t test = test_start("pinsh", NULL, ALC14, HAS_SRC123);
        uint64_t src1 = 0x1122334455667788;
        uint64_t src2 = 0xffeeddccbbaa9900;
        int k = 0;
        CHECK_RRI_14(&test, pinsh, src1, src2, 0x0, k); ++k;
        CHECK_RRI_14(&test, pinsh, src1, src2, 0x1, k); ++k;
        CHECK_RRI_14(&test, pinsh, src1, src2, 0x2, k); ++k;
        CHECK_RRI_14(&test, pinsh, src1, src2, 0x3, k); ++k;
        test_end(&test);
    } while(0);
}

#define EXEC_PSHIFT_ITER(INSN, SRC1, SRC2, SRC3, K) do { \
    EXEC_RRI_14(INSN, test.result, SRC1[i], SRC2[i], SRC3, 0); \
    uint64_t expected = GET_EXPECT(&test, glue(INSN, _expect), K); \
    test_report(&test, expected, SRC1[i], SRC2[i], SRC3, 0); \
} while(0)

#define CHECK3_PSHIFT_7(INSN, SRC1, SRC2) do { \
    ASSERT_ARRAY_LEN_EQ(SRC1, SRC2); \
    alc_test_t test = test_start(#INSN, NULL, ALC14, HAS_SRC123); \
    for (int i = 0, k = 0; i < ARRAY_LEN(SRC1); ++i) { \
        EXEC_PSHIFT_ITER(INSN, SRC1, SRC2, 0, k); ++k; \
        EXEC_PSHIFT_ITER(INSN, SRC1, SRC2, 1, k); ++k; \
        EXEC_PSHIFT_ITER(INSN, SRC1, SRC2, 3, k); ++k; \
        EXEC_PSHIFT_ITER(INSN, SRC1, SRC2, 4, k); ++k; \
        EXEC_PSHIFT_ITER(INSN, SRC1, SRC2, 6, k); ++k; \
        EXEC_PSHIFT_ITER(INSN, SRC1, SRC2, 7, k); ++k; \
    } \
    test_end(&test); \
} while(0)

#define CHECK3_PSHIFT_15(INSN, SRC1, SRC2) do { \
    ASSERT_ARRAY_LEN_EQ(SRC1, SRC2); \
    alc_test_t test = test_start(#INSN, NULL, ALC14, HAS_SRC123); \
    for (int i = 0, k = 0; i < ARRAY_LEN(SRC1); ++i) { \
        EXEC_PSHIFT_ITER(INSN, SRC1, SRC2,  0, k); ++k; \
        EXEC_PSHIFT_ITER(INSN, SRC1, SRC2,  1, k); ++k; \
        EXEC_PSHIFT_ITER(INSN, SRC1, SRC2,  7, k); ++k; \
        EXEC_PSHIFT_ITER(INSN, SRC1, SRC2, 12, k); ++k; \
        EXEC_PSHIFT_ITER(INSN, SRC1, SRC2, 13, k); ++k; \
        EXEC_PSHIFT_ITER(INSN, SRC1, SRC2, 15, k); ++k; \
    } \
    test_end(&test); \
} while(0)

static void test_v1_shift(void) {
    CHECK2_CARTESIAN(EXEC_RR, 14, psrlw, shift_src1, shift_src2);
    CHECK2_CARTESIAN(EXEC_RR, 14, psrlh, shift_src1, shift_src2);
    CHECK2_CARTESIAN(EXEC_RR, 14, psraw, shift_src1, shift_src2);
    CHECK2_CARTESIAN(EXEC_RR, 14, psrah, shift_src1, shift_src2);
    CHECK2_CARTESIAN(EXEC_RR, 14, psllw, shift_src1, shift_src2);
    CHECK2_CARTESIAN(EXEC_RR, 14, psllh, shift_src1, shift_src2);
    CHECK2_CARTESIAN(EXEC_RR, 14, psrld, shift_src1, shift_src2);
    CHECK2_CARTESIAN(EXEC_RR, 14, pslld, shift_src1, shift_src2);

    CHECK3_PSHIFT_7(psrlqh, pshift_src1, pshift_src2);
    CHECK3_PSHIFT_7(psllql, pshift_src1, pshift_src2);

    CHECK3_PSHIFT_15(psrlql, pshift_src1, pshift_src2);
    CHECK3_PSHIFT_15(psllqh, pshift_src1, pshift_src2);
}

static void test_v1(void) {
    test_v1_packed();
    test_v1_shuffle();
    test_v1_shift();
}

static uint64_t pshufb_src1[] = {
    0x0000000000000000, 0xffffffffffffffff, 0x1122334455667788, 0x1122334455667788,
    0x1122334455667788, 0x1122334455667788, 0x1122334455667788, 0x1122334455667788,
    0x1122334455667788, 0x1122334455667788, 0x1122334455667788, 0x1122334455667788,
    0x1122334455667788, 0x1122334455667788,
};

static uint64_t pshufb_src2[] = {
    0x0000000000000000, 0xffffffffffffffff, 0x99aabbccddeeff00, 0x99aabbccddeeff00,
    0x99aabbccddeeff00, 0x99aabbccddeeff00, 0x99aabbccddeeff00, 0x99aabbccddeeff00,
    0x99aabbccddeeff00, 0x99aabbccddeeff00, 0x99aabbccddeeff00, 0x99aabbccddeeff00,
    0x99aabbccddeeff00, 0x99aabbccddeeff00,
};

static uint64_t pshufb_src3[] = {
    0x00ff9f8fe0c0a080, 0x00ff9f8fe0c0a080, 0x0706050403020100, 0x0f0e0d0c0b0a0908,
    0x1716151413121110, 0x1f1e1d1c1b1a1918, 0x2726252423222120, 0x2f2e2d2c2b2a2928,
    0x4746454443424140, 0x4f4e4d4c4b4a4948, 0x5756555453525150, 0x5f5e5d5c5b5a5958,
    0x77767574737271f0, 0x7f7e7d7c7b7a79f8,
};

static uint64_t pmerge_src1[] = {
    0x1122334455667788, 0x1122334455667788, 0x1122334455667788, 0x1122334455667788,
};

static uint64_t pmerge_src2[] = {
    0x99aabbccddeeff00, 0x99aabbccddeeff00, 0x99aabbccddeeff00, 0x99aabbccddeeff00,
};

static uint64_t pmerge_src3[] = {
    0x0800ffe0c0807f0f, 0xffff00000000ffff, 0xff00ff0000ffff00, 0x00ffff00ff000fff,
};

static void test_v2(void) {
    CHECK2(EXEC_RR, 14, pmulubhh, int_src1, int_src2);

    CHECK3(EXEC_RRR, 0134, pshufb, pshufb_src1, pshufb_src2, pshufb_src3);
    CHECK3(EXEC_RRR, 0134, pmerge, pmerge_src1, pmerge_src2, pmerge_src3);
}

static void test_v3(void) {
    CHECK2(EXEC_RR, 03, pminsb,     int_src1, int_src2);
    CHECK2(EXEC_RR, 03, pminuh,     int_src1, int_src2);
    CHECK2(EXEC_RR, 03, pmaxsb,     int_src1, int_src2);
    CHECK2(EXEC_RR, 03, pmaxuh,     int_src1, int_src2);
    CHECK2(EXEC_RR, 03, pcmpeqd,    int_src1, int_src2);
    CHECK2(EXEC_RR, 03, pcmpgtd,    int_src1, int_src2);
    CHECK2(EXEC_RR, 03, pminuw,     int_src1, int_src2);
    CHECK2(EXEC_RR, 03, pminsw,     int_src1, int_src2);
    CHECK2(EXEC_RR, 03, pmaxuw,     int_src1, int_src2);
    CHECK2(EXEC_RR, 03, pmaxsw,     int_src1, int_src2);
    CHECK2(EXEC_RR, 03, phaddh,     int_src1, int_src2);
    CHECK2(EXEC_RR, 03, phaddw,     int_src1, int_src2);
    CHECK2(EXEC_RR, 03, phaddsh,    int_src1, int_src2);
    CHECK2(EXEC_RR, 03, phsubh,     int_src1, int_src2);
    CHECK2(EXEC_RR, 03, phsubw,     int_src1, int_src2);
    CHECK2(EXEC_RR, 03, phsubsh,    int_src1, int_src2);
    CHECK2(EXEC_RR, 03, psignb,     int_src1, int_src2);
    CHECK2(EXEC_RR, 03, psignh,     int_src1, int_src2);
    CHECK2(EXEC_RR, 03, psignw,     int_src1, int_src2);

    CHECK2(EXEC_RR, 14, mpsadbh,    int_src1, int_src2);
    CHECK2(EXEC_RR, 14, pmaddubsh,  int_src1, int_src2);
    CHECK2(EXEC_RR, 14, pmulhrsh,   int_src1, int_src2);
    CHECK2(EXEC_RR, 14, phminposuh, int_src1, int_src2);
    CHECK2(EXEC_RR, 14, packuswh,   int_src1, int_src2);

    // new channels
    skip_gen(true);
    CHECK2_CARTESIAN(EXEC_RR, 03, psrlw, shift_src1, shift_src2);
    CHECK2_CARTESIAN(EXEC_RR, 03, psrlh, shift_src1, shift_src2);
    CHECK2_CARTESIAN(EXEC_RR, 03, psraw, shift_src1, shift_src2);
    CHECK2_CARTESIAN(EXEC_RR, 03, psrah, shift_src1, shift_src2);
    CHECK2_CARTESIAN(EXEC_RR, 03, psllw, shift_src1, shift_src2);
    CHECK2_CARTESIAN(EXEC_RR, 03, psllh, shift_src1, shift_src2);
    skip_gen(false);
}

int main(int argc, char *argv[]) {
    parse_args(argc, argv);

    test_v1();
    test_v2();
    test_v3();

    return is_failed();
}
