#ifndef TEST_H
#define TEST_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>

#define glue(a, b) a ## b
#define glue3(a, b, c) a ## b ## c

#define ARRAY_LEN(X) (sizeof(X) / sizeof(X[0]))

#define ASSERT_ARRAY_LEN_EQ(X, Y) do { \
    if (ARRAY_LEN(X) != ARRAY_LEN(Y)) { \
        fprintf(stderr, "%s:%d array length %s:%u != %s:%u\n", __FILE__, __LINE__, \
                #X, (unsigned) ARRAY_LEN(X), #Y, (unsigned) ARRAY_LEN(Y)); \
        abort(); \
    } \
} while(0)

// Selector for getf{s,d}.
#define FIELD(O, L, S, B) ((O) | ((L) << 6) | ((S) << 12) | ((B) << 13))

static bool generate_expect = false;

static void parse_args(int argc, char *argv[]) {
    if (argc > 1) {
        generate_expect = strcmp(argv[1], "-g") == 0;
    }
    if (generate_expect) {
        printf("/* AUTO-GENERATED! DO NOT EDIT! */\n\n");
    }
}

static int total_fails = 0;

#define is_failed() (total_fails > 0)

typedef enum {
    ALC0 = 1 << 0,
    ALC1 = 1 << 1,
    ALC2 = 1 << 2,
    ALC3 = 1 << 3,
    ALC4 = 1 << 4,
    ALC5 = 1 << 5,
    ALC03 = ALC0 | ALC3,
    ALC14 = ALC1 | ALC4,
    ALC25 = ALC2 | ALC5,
    ALC0134 = ALC03 | ALC14,
    ALC0235 = ALC03 | ALC25,
    ALC012345 = ALC03 | ALC14 | ALC25,
} alc_t;

typedef enum {
    HAS_SRC1    = 1 << 0,
    HAS_SRC2    = 1 << 1,
    HAS_SRC3    = 1 << 2,
    HAS_SRC4    = 1 << 3,
    HAS_SRC12   = HAS_SRC1 | HAS_SRC2,
    HAS_SRC123  = HAS_SRC1 | HAS_SRC2 | HAS_SRC3,
    HAS_SRC1234 = HAS_SRC1 | HAS_SRC2 | HAS_SRC3 | HAS_SRC4,
    SKIP_GEN    = 1 << 4,
} flags_t;

typedef struct alc_test {
    const char *insn;
    const char *comment;
    alc_t channels;
    flags_t flags;
    uint64_t result[6];
    int i;
} alc_test_t;

static alc_test_t test_start(const char *insn, const char *comment, alc_t channels, flags_t flags) {
    alc_test_t test = { 0 };
    test.insn = insn;
    test.comment = comment;
    test.channels = channels;
    test.flags = flags;
    test.i = 0;
    if (generate_expect) {
        if (flags & SKIP_GEN)
            return test;
        if (comment) {
            printf("// %s\n", comment);
        }
        printf("static uint64_t %s_expect[] = {\n", insn);
    } else {
        if (comment) {
            printf("# %s %s\n", insn, comment);
        } else {
            printf("# %s\n", insn);
        }
    }
    return test;
}

static void test_end(alc_test_t *test) {
    if (generate_expect) {
        if (!(test->flags & SKIP_GEN)) {
            if (test->i != 0)
                printf("\n");
            printf("};\n");
        }
    } else {
        printf("\n");
    }
}

static void test_report(
    alc_test_t *test,
    uint64_t expected,
    uint64_t src1,
    uint64_t src2,
    uint64_t src3,
    uint64_t src4
) {
    bool fail = false;

    if (generate_expect) {
        if (test->flags & SKIP_GEN)
            goto skip;
        if (test->i == 0)
            printf("   ");
        printf(" 0x%016" PRIx64 ",", test->result[0]);
        if (test->i >= 3) {
            test->i = 0;
            printf("\n");
        } else {
            test->i += 1;
        }
    } else {
        printf("  %016" PRIx64 " =", test->result[0]);
        if (test->flags & HAS_SRC1) printf(" %016" PRIx64, src1);
        if (test->flags & HAS_SRC2) printf(" %016" PRIx64, src2);
        if (test->flags & HAS_SRC3) printf(" %016" PRIx64, src3);
        if (test->flags & HAS_SRC4) printf(" %016" PRIx64, src4);
        printf("\n");
    }

skip:
    for (int i = 0, j = 0; j < 6; ++j) {
        if (test->channels & (ALC0 << j)) {
            if (test->result[i] != expected) {
                fail = true;
                break;
            }
            i += 1;
        }
    }

    if (!fail)
        goto done;

    total_fails += 1;

    if (test->comment) {
        fprintf(stderr, "FAILED: invalid result \"%s %s\"\n", test->insn, test->comment);
    } else {
        fprintf(stderr, "FAILED: invalid result \"%s\"\n", test->insn);
    }
    fprintf(stderr, "  operands:");
    if (test->flags & HAS_SRC1) fprintf(stderr, " %016" PRIx64, src1);
    if (test->flags & HAS_SRC2) fprintf(stderr, " %016" PRIx64, src2);
    if (test->flags & HAS_SRC3) fprintf(stderr, " %016" PRIx64, src3);
    if (test->flags & HAS_SRC4) fprintf(stderr, " %016" PRIx64, src4);
    fprintf(stderr, "\n");

    fprintf(stderr, "  expected: %016" PRIx64 "\n", expected);
    for (int i = 0, j = 0; j < 6; ++j) {
        if (test->channels & (ALC0 << j)) {
            const char *err = "";
            if (test->result[i] != expected) {
                err = " <<<< invalid result";
            }
            fprintf(stderr, "      alc%d: %016" PRIx64 "%s\n", j, test->result[i], err);
            i += 1;
        }
    }
    fprintf(stderr, "\n");

done:
    memset(test->result, 0, sizeof(test->result));
}

#define EXEC_R_0134(INSN, RES, SRC1, SRC2, SRC3, SRC4) asm( \
    "\t{\n" \
    "\t    " #INSN ",0 %[src1], %0\n" \
    "\t    " #INSN ",1 %[src1], %1\n" \
    "\t    " #INSN ",3 %[src1], %2\n" \
    "\t    " #INSN ",4 %[src1], %3\n" \
    "\t}" \
    : "+r"(RES[0]), \
      "+r"(RES[1]), \
      "+r"(RES[2]), \
      "+r"(RES[3]) \
    : [src1]"r"(SRC1) \
)

#define EXEC_R_14(INSN, RES, SRC1, SRC2, SRC3, SRC4) asm( \
    "\t{\n" \
    "\t    " #INSN ",1 %[src1], %0\n" \
    "\t    " #INSN ",4 %[src1], %1\n" \
    "\t}" \
    : "+r"(RES[0]), \
      "+r"(RES[1])  \
    : [src1]"r"(SRC1) \
)

#define EXEC_R_5(INSN, RES, SRC1, SRC2, SRC3, SRC4) asm( \
    "\t{\n" \
    "\t    " #INSN ",5 %[src1], %0\n" \
    "\t}" \
    : "+r"(RES[0]) \
    : [src1]"r"(SRC1) \
)

#define EXEC_RR_03(INSN, RES, SRC1, SRC2, SRC3, SRC4) asm( \
    "\t{\n" \
    "\t    " #INSN ",0 %[src1], %[src2], %0\n" \
    "\t    " #INSN ",3 %[src1], %[src2], %1\n" \
    "\t}" \
    : "+r"(RES[0]), \
      "+r"(RES[1])  \
    : [src1]"r"(SRC1), \
      [src2]"r"(SRC2)  \
)

#define EXEC_RR_14(INSN, RES, SRC1, SRC2, SRC3, SRC4) asm( \
    "\t{\n" \
    "\t    " #INSN ",1 %[src1], %[src2], %0\n" \
    "\t    " #INSN ",4 %[src1], %[src2], %1\n" \
    "\t}" \
    : "+r"(RES[0]), \
      "+r"(RES[1])  \
    : [src1]"r"(SRC1), \
      [src2]"r"(SRC2)  \
)

#define EXEC_RI_14(INSN, RES, SRC1, SRC2, SRC3, SRC4) asm( \
    "\t{\n" \
    "\t    " #INSN ",1 %[src1], %[src2], %0\n" \
    "\t    " #INSN ",4 %[src1], %[src2], %1\n" \
    "\t}" \
    : "+r"(RES[0]), \
      "+r"(RES[1])  \
    : [src1]"r"(SRC1), \
      [src2]"i"(SRC2)  \
)

#define EXEC_RR_0134(INSN, RES, SRC1, SRC2, SRC3, SRC4) asm( \
    "\t{\n" \
    "\t    " #INSN ",0 %[src1], %[src2], %0\n" \
    "\t    " #INSN ",1 %[src1], %[src2], %1\n" \
    "\t    " #INSN ",3 %[src1], %[src2], %2\n" \
    "\t    " #INSN ",4 %[src1], %[src2], %3\n" \
    "\t}" \
    : "+r"(RES[0]), \
      "+r"(RES[1]), \
      "+r"(RES[2]), \
      "+r"(RES[3])  \
    : [src1]"r"(SRC1), \
      [src2]"r"(SRC2)  \
)

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

#define EXEC2_012345(INSN, RES, C1, SRC1, C2, SRC2) asm( \
    "\t{\n" \
    "\t    " #INSN ",0 %[src1], %[src2], %0\n" \
    "\t    " #INSN ",1 %[src1], %[src2], %1\n" \
    "\t    " #INSN ",2 %[src1], %[src2], %2\n" \
    "\t    " #INSN ",3 %[src1], %[src2], %3\n" \
    "\t    " #INSN ",4 %[src1], %[src2], %4\n" \
    "\t    " #INSN ",5 %[src1], %[src2], %5\n" \
    "\t}" \
    : "+r"(RES[0]), \
      "+r"(RES[1]), \
      "+r"(RES[2]), \
      "+r"(RES[3]), \
      "+r"(RES[4]), \
      "+r"(RES[5]) \
    : [src1]C1(SRC1), \
      [src2]C2(SRC2) \
)

#define EXEC_RR_012345(INSN, RES, SRC1, SRC2, SRC3, SRC4) \
    EXEC2_012345(INSN, RES, "r", SRC1, "r", SRC2)

#define EXEC_IR_012345(INSN, RES, SRC1, SRC2, SRC3, SRC4) \
    EXEC2_012345(INSN, RES, "I", SRC1, "r", SRC2)

#define EXEC_RI_012345(INSN, RES, SRC1, SRC2, SRC3, SRC4) \
    EXEC2_012345(INSN, RES, "r", SRC1, "i", SRC2)

#define EXEC2_5(INSN, RES, C1, SRC1, C2, SRC2) asm( \
    "\t{\n" \
    "\t    " #INSN ",5 %[src1], %[src2], %0\n" \
    "\t}" \
    : "+r"(RES[0]) \
    : [src1]C1(SRC1), \
      [src2]C2(SRC2) \
)

#define EXEC_RR_5(INSN, RES, SRC1, SRC2, SRC3, SRC4) \
    EXEC2_5(INSN, RES, "r", SRC1, "r", SRC2)

#define EXEC_RRI_14(INSN, RES, SRC1, SRC2, SRC3, SRC4) asm( \
    "\t{\n" \
    "\t    " #INSN ",1 %[src1], %[src2], %[src3], %0\n" \
    "\t    " #INSN ",4 %[src1], %[src2], %[src3], %1\n" \
    "\t}" \
    : "+r"(RES[0]), \
      "+r"(RES[1]) \
    : [src1]"r"(SRC1), \
      [src2]"r"(SRC2), \
      [src3]"i"(SRC3) \
    : "pred0" \
)

#define EXEC_RRR_0134(INSN, RES, SRC1, SRC2, SRC3, SRC4) asm( \
    "\t{\n" \
    "\t    " #INSN ",0 %[src1], %[src2], %[src3], %0\n" \
    "\t    " #INSN ",1 %[src1], %[src2], %[src3], %1\n" \
    "\t    " #INSN ",3 %[src1], %[src2], %[src3], %2\n" \
    "\t    " #INSN ",4 %[src1], %[src2], %[src3], %3\n" \
    "\t}" \
    : "+r"(RES[0]), \
      "+r"(RES[1]), \
      "+r"(RES[2]), \
      "+r"(RES[3])  \
    : [src1]"r"(SRC1), \
      [src2]"r"(SRC2), \
      [src3]"r"(SRC3)  \
)

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

#define EXEC_REPORT(TEST, EXEC, INSN, SRC1, SRC2, SRC3, SRC4, EXPECTED) \
    EXEC(INSN, (TEST).result, SRC1, SRC2, SRC3, SRC4); \
    test_report(&TEST, EXPECTED, SRC1, SRC2, SRC3, SRC4)

#define EXEC1_REPORT(TEST, EXEC, INSN, SRC1, EXPECTED) \
    EXEC(INSN, (TEST).result, SRC1, 0, 0, 0); \
    test_report(&TEST, EXPECTED, SRC1, 0, 0, 0)

#define EXEC2_REPORT(TEST, EXEC, INSN, SRC1, SRC2, EXPECTED) \
    EXEC(INSN, (TEST).result, SRC1, SRC2, 0, 0); \
    test_report(&TEST, EXPECTED, SRC1, SRC2, 0, 0)

#define EXEC3_REPORT(TEST, EXEC, INSN, SRC1, SRC2, SRC3, EXPECTED) \
    EXEC(INSN, (TEST).result, SRC1, SRC2, SRC3, 0); \
    test_report(&TEST, EXPECTED, SRC1, SRC2, SRC3, 0)

#define DUMMY_EXPECT(TEST, EXPECT, I) (TEST)->result[0]

#ifdef DUMP_ONLY
#define GET_EXPECT DUMMY_EXPECT
#else
#define GET_EXPECT(TEST, EXPECT, I) (EXPECT)[I]
#endif

#define TEST1(EXEC, CHAN, INSN, SRC1, EXPECT, GET_EXPECT) do { \
    alc_test_t test = test_start(#INSN, NULL, glue(ALC, CHAN), HAS_SRC1); \
    const uint64_t *src1 = (const void *) (SRC1); \
    for (int i = 0; i < ARRAY_LEN(SRC1); ++i) { \
        glue3(EXEC, _, CHAN)(INSN, test.result, src1[i], 0, 0, 0); \
        uint64_t expected = GET_EXPECT(&test, EXPECT, i); \
        test_report(&test, expected, src1[i], 0, 0, 0); \
    } \
    test_end(&test); \
} while(0)

#define DUMP1(EXEC, CHAN, INSN, SRC1) \
    TEST1(EXEC, CHAN, INSN, SRC1, NULL, DUMMY_EXPECT)

#define CHECK1(EXEC, CHAN, INSN, SRC1) \
    TEST1(EXEC, CHAN, INSN, SRC1, glue(INSN, _expect), GET_EXPECT)

#define TEST2(EXEC, CHAN, INSN, SRC1, SRC2, EXPECT, GET_EXPECT) do { \
    ASSERT_ARRAY_LEN_EQ(SRC1, SRC2); \
    alc_test_t test = test_start(#INSN, NULL, glue(ALC, CHAN), HAS_SRC12); \
    const uint64_t *src1 = (const void *) (SRC1); \
    const uint64_t *src2 = (const void *) (SRC2); \
    for (int i = 0; i < ARRAY_LEN(SRC1); ++i) { \
        glue3(EXEC, _, CHAN)(INSN, test.result, src1[i], src2[i], 0, 0); \
        uint64_t expected = GET_EXPECT(&test, EXPECT, i); \
        test_report(&test, expected, src1[i], src2[i], 0, 0); \
    } \
    test_end(&test); \
} while(0)

#define DUMP2(EXEC, CHAN, INSN, SRC1, SRC2) \
    TEST2(EXEC, CHAN, INSN, SRC1, SRC2, NULL, DUMMY_EXPECT)

#define CHECK2(EXEC, CHAN, INSN, SRC1, SRC2) \
    TEST2(EXEC, CHAN, INSN, SRC1, SRC2, glue(INSN, _expect), GET_EXPECT)

#define TEST2_CARTESIAN(EXEC, CHAN, INSN, SRC1, SRC2, EXPECT, GET_EXPECT) do {\
    alc_test_t test = test_start(#INSN, NULL, glue(ALC, CHAN), HAS_SRC12); \
    const uint64_t *src1 = (const void *) (SRC1); \
    const uint64_t *src2 = (const void *) (SRC2); \
    for (int i = 0, k = 0; i < ARRAY_LEN(SRC1); ++i) { \
        for (int j = 0; j < ARRAY_LEN(SRC2); ++j, ++k) { \
            glue3(EXEC, _, CHAN)(INSN, test.result, src1[i], src2[j], 0, 0); \
            uint64_t expected = GET_EXPECT(&test, EXPECT, k); \
            test_report(&test, expected, src1[i], src2[j], 0, 0); \
        } \
    } \
    test_end(&test); \
} while(0)

#define DUMP2_CARTESIAN(EXEC, CHAN, INSN, SRC1, SRC2) \
    TEST2_CARTESIAN(EXEC, CHAN, INSN, SRC1, SRC2, NULL, DUMMY_EXPECT)

#define CHECK2_CARTESIAN(EXEC, CHAN, INSN, SRC1, SRC2) \
    TEST2_CARTESIAN(EXEC, CHAN, INSN, SRC1, SRC2, glue(INSN, _expect), GET_EXPECT)

#define TEST3(EXEC, CHAN, INSN, SRC1, SRC2, SRC3, EXPECT, GET_EXPECT) do { \
    ASSERT_ARRAY_LEN_EQ(SRC1, SRC2); \
    ASSERT_ARRAY_LEN_EQ(SRC1, SRC3); \
    alc_test_t test = test_start(#INSN, NULL, glue(ALC, CHAN), HAS_SRC123); \
    const uint64_t *src1 = (const void *) (SRC1); \
    const uint64_t *src2 = (const void *) (SRC2); \
    const uint64_t *src3 = (const void *) (SRC3); \
    for (int i = 0; i < ARRAY_LEN(SRC1); ++i) { \
        glue3(EXEC, _, CHAN)(INSN, test.result, src1[i], src2[i], src3[i], 0); \
        uint64_t expected = GET_EXPECT(&test, EXPECT, i); \
        test_report(&test, expected, src1[i], src2[i], src3[i], 0); \
    } \
    test_end(&test); \
} while(0)

#define DUMP3(EXEC, CHAN, INSN, SRC1, SRC2, SRC3) \
    TEST3(EXEC, CHAN, INSN, SRC1, SRC2, SRC3, NULL, DUMMY_EXPECT)

#define CHECK3(EXEC, CHAN, INSN, SRC1, SRC2, SRC3) \
    TEST3(EXEC, CHAN, INSN, SRC1, SRC2, SRC3, glue(INSN, _expect), GET_EXPECT)

#endif /* TEST_H */
