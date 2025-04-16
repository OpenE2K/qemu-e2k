#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

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
    SKIP_GEN    = 1 << 0,
    FLAGS_X     = 1 << 1,
    FLAGS_Q     = 1 << 2,
    FLAGS_XQ    = FLAGS_X | FLAGS_Q,
} flags_t;

typedef struct alc_test {
    const char *insn;
    const char *comment;
    alc_t channels;
    flags_t flags;
    uint64_t result[12];
    int i;
} alc_test_t;

void parse_args(int argc, char *argv[]);
void skip_gen(bool skip);
bool is_failed(void);

alc_test_t test_start(const char *insn, const char *comment, alc_t channels, flags_t flags);
void test_end(alc_test_t *test);
void test_report(
    alc_test_t *test,
    const uint64_t *expected,
    const uint64_t *src1,
    const uint64_t *src2,
    const uint64_t *src3,
    const uint64_t *src4
);

#define EXEC1_REPORT(TEST, EXEC, INSN, SRC1, EXPECTED) \
    uint64_t src1 = SRC1; \
    uint64_t expected = EXPECTED; \
    EXEC(INSN, (TEST).result, SRC1); \
    test_report(&TEST, &expected, &src1, NULL, NULL, NULL)

#define EXEC2_REPORT(TEST, EXEC, INSN, SRC1, SRC2, EXPECTED) do { \
    uint64_t src1 = SRC1, src2 = SRC2; \
    uint64_t expected = EXPECTED; \
    EXEC(INSN, (TEST).result, SRC1, SRC2); \
    test_report(&TEST, &expected, &src1, &src2, NULL, NULL); \
} while(0)

#define EXEC3_REPORT(TEST, EXEC, INSN, SRC1, SRC2, SRC3, EXPECTED) do { \
    uint64_t src1 = SRC1, src2 = SRC2, src3 = SRC3; \
    uint64_t expected = EXPECTED; \
    EXEC(INSN, (TEST).result, SRC1, SRC2, SRC3); \
    test_report(&TEST, &expected, &src1, &src2, &src3, NULL); \
} while(0)

#define EXEC_REPORT(TEST, EXEC, INSN, SRC1, SRC2, SRC3, SRC4, EXPECTED) do { \
    uint64_t src1 = SRC1, src2 = SRC2, src3 = SRC3, src4 = SRC4; \
    uint64_t expected = EXPECTED; \
    EXEC(INSN, (TEST).result, SRC1, SRC2, SRC3, SRC4); \
    test_report(&TEST, &expected, &src1, &src2, &src3, &src4); \
} while(0)

#define DUMMY_EXPECT(TEST, EXPECT, I) (TEST)->result[0]

#ifdef DUMP_ONLY
#define GET_EXPECT DUMMY_EXPECT
#else
#define GET_EXPECT(TEST, EXPECT, I) (EXPECT)[I]
#endif

#define TEST1(EXEC, CHAN, INSN, SRC1, EXPECT, GET_EXPECT) do { \
    alc_test_t test = test_start(#INSN, NULL, glue(ALC, CHAN), 0); \
    const uint64_t *src1 = (const void *) (SRC1); \
    for (int i = 0; i < ARRAY_LEN(SRC1); ++i) { \
        glue3(EXEC, _, CHAN)(INSN, test.result, src1[i]); \
        uint64_t expected = GET_EXPECT(&test, EXPECT, i); \
        test_report(&test, &expected, &src1[i], NULL, NULL, NULL); \
    } \
    test_end(&test); \
} while(0)

#define DUMP1(EXEC, CHAN, INSN, SRC1) \
    TEST1(EXEC, CHAN, INSN, SRC1, NULL, DUMMY_EXPECT)

#define CHECK1(EXEC, CHAN, INSN, SRC1) \
    TEST1(EXEC, CHAN, INSN, SRC1, glue(INSN, _expect), GET_EXPECT)

#define TEST2(EXEC, CHAN, INSN, SRC1, SRC2, EXPECT, GET_EXPECT) do { \
    ASSERT_ARRAY_LEN_EQ(SRC1, SRC2); \
    alc_test_t test = test_start(#INSN, NULL, glue(ALC, CHAN), 0); \
    const uint64_t *src1 = (const void *) (SRC1); \
    const uint64_t *src2 = (const void *) (SRC2); \
    for (int i = 0; i < ARRAY_LEN(SRC1); ++i) { \
        glue3(EXEC, _, CHAN)(INSN, test.result, src1[i], src2[i]); \
        uint64_t expected = GET_EXPECT(&test, EXPECT, i); \
        test_report(&test, &expected, &src1[i], &src2[i], NULL, NULL); \
    } \
    test_end(&test); \
} while(0)

#define DUMP2(EXEC, CHAN, INSN, SRC1, SRC2) \
    TEST2(EXEC, CHAN, INSN, SRC1, SRC2, NULL, DUMMY_EXPECT)

#define CHECK2(EXEC, CHAN, INSN, SRC1, SRC2) \
    TEST2(EXEC, CHAN, INSN, SRC1, SRC2, glue(INSN, _expect), GET_EXPECT)

#define TEST2_CARTESIAN(EXEC, CHAN, INSN, SRC1, SRC2, EXPECT, GET_EXPECT) do {\
    alc_test_t test = test_start(#INSN, NULL, glue(ALC, CHAN), 0); \
    const uint64_t *src1 = (const void *) (SRC1); \
    const uint64_t *src2 = (const void *) (SRC2); \
    for (int i = 0, k = 0; i < ARRAY_LEN(SRC1); ++i) { \
        for (int j = 0; j < ARRAY_LEN(SRC2); ++j, ++k) { \
            glue3(EXEC, _, CHAN)(INSN, test.result, src1[i], src2[j]); \
            uint64_t expected = GET_EXPECT(&test, EXPECT, k); \
            test_report(&test, &expected, &src1[i], &src2[j], NULL, NULL); \
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
    alc_test_t test = test_start(#INSN, NULL, glue(ALC, CHAN), 0); \
    const uint64_t *src1 = (const void *) (SRC1); \
    const uint64_t *src2 = (const void *) (SRC2); \
    const uint64_t *src3 = (const void *) (SRC3); \
    for (int i = 0; i < ARRAY_LEN(SRC1); ++i) { \
        glue3(EXEC, _, CHAN)(INSN, test.result, src1[i], src2[i], src3[i]); \
        uint64_t expected = GET_EXPECT(&test, EXPECT, i); \
        test_report(&test, &expected, &src1[i], &src2[i], &src3[i], NULL); \
    } \
    test_end(&test); \
} while(0)

#define DUMP3(EXEC, CHAN, INSN, SRC1, SRC2, SRC3) \
    TEST3(EXEC, CHAN, INSN, SRC1, SRC2, SRC3, NULL, DUMMY_EXPECT)

#define CHECK3(EXEC, CHAN, INSN, SRC1, SRC2, SRC3) \
    TEST3(EXEC, CHAN, INSN, SRC1, SRC2, SRC3, glue(INSN, _expect), GET_EXPECT)

#endif /* TEST_H */
