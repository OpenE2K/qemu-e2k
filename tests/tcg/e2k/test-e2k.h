#ifndef TESTS_TCG_E2K_E2K_H
#define TESTS_TCG_E2K_E2K_H

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#define glue(a, b) a ## b

static int total_fails = 0;
#define is_failed() (total_fails > 0)

#define EXEC1(INSN, CHAN, S2, C2) ({ \
    uint64_t res = 0; \
    asm(#INSN "," #CHAN " %[src2], %[dst]" \
        : [dst]"+r"(res) \
        : [src2] #C2 ((uint64_t) S2) \
    ); \
    res; \
})

#define EXEC2(INSN, CHAN, S1, S2, C1, C2) ({ \
    uint64_t res = 0; \
    asm(#INSN "," #CHAN " %[src1], %[src2], %[dst]" \
        : [dst]"+r"(res) \
        : [src1] #C1 ((uint64_t) S1), \
          [src2] #C2 ((uint64_t) S2) \
    ); \
    res; \
})

#define EXEC3(INSN, CHAN, S1, S2, S3, C1, C2, C3) ({ \
    uint64_t res = 0; \
    asm(#INSN "," #CHAN " %[src1], %[src2], %[src3], %[dst]" \
        : [dst]"+r"(res) \
        : [src1] #C1 ((uint64_t) S1), \
          [src2] #C2 ((uint64_t) S2), \
          [src3] #C3 ((uint64_t) S3) \
    ); \
    res; \
})

#define EXEC_X(INSN, CHAN, S2) EXEC1(INSN, CHAN, S2, ri)
#define EXEC_R(INSN, CHAN, S2) EXEC1(INSN, CHAN, S2, r)
#define EXEC_XX(INSN, CHAN, S1, S2) EXEC2(INSN, CHAN, S1, S2, rI, ri)
#define EXEC_RR(INSN, CHAN, S1, S2) EXEC2(INSN, CHAN, S1, S2, r, r)
#define EXEC_XXX(INSN, CHAN, S1, S2, S3) EXEC3(INSN, CHAN, S1, S2, S3, rI, ri, r)
#define EXEC_RRR(INSN, CHAN, S1, S2, S3) EXEC3(INSN, CHAN, S1, S2, S3, r, r, r)

#define GROUP(NAME) \
    printf("\n# Checking %s...\n", NAME)

#define GROUPF(FMT, ...) \
    printf("\n# Checking " FMT "...\n", __VA_ARGS__)

#define INSN_FMT "%-16s"
#define RES_FMT " %016" PRIx64 " ="
#define SRC_FMT " %016" PRIx64

#define AS_U64(X) ((uint64_t) (X))
#define RES AS_U64
#define SRC AS_U64

#define DUMP_FAILED_START() \
    fprintf(stderr, "Failed at %s:%d\n", __FILE__, __LINE__)

#define DUMP_FAILED_END(EXPECT) \
    fprintf(stderr, "\n    " INSN_FMT SRC_FMT "\n", "expected", SRC(EXPECT))

#define DUMP_RES(FILE, INSN, CHAN, X) \
    fprintf(FILE, "  %d " INSN_FMT RES_FMT, CHAN, #INSN, RES(X))
#define DUMP_SRC(FILE, X) \
    fprintf(FILE, SRC_FMT, SRC(X))
#define DUMP_END(FILE) \
    fprintf(FILE, "\n")

#define DUMP1(EXEC, INSN, CHAN, S2) ({ \
    uint64_t res = EXEC(INSN, CHAN, S2); \
    DUMP_RES(stdout, INSN, CHAN, res); \
    DUMP_SRC(stdout, S2); \
    DUMP_END(stdout); \
    res; \
})

#define DUMP2(EXEC, INSN, CHAN, S1, S2) ({ \
    uint64_t res = EXEC(INSN, CHAN, S1, S2); \
    DUMP_RES(stdout, INSN, CHAN, res); \
    DUMP_SRC(stdout, S1); \
    DUMP_SRC(stdout, S2); \
    DUMP_END(stdout); \
    res; \
})

#define DUMP3(EXEC, INSN, CHAN, S1, S2, S3) ({ \
    uint64_t res = EXEC(INSN, CHAN, S1, S2, S3); \
    DUMP_RES(stdout, INSN, CHAN, res); \
    DUMP_SRC(stdout, S1); \
    DUMP_SRC(stdout, S2); \
    DUMP_SRC(stdout, S3); \
    DUMP_END(stdout); \
    res; \
})

#ifdef NO_CHECKS
#define CHECK1(EXEC, INSN, CHAN, S2, EXPECT) \
    DUMP1(EXEC, INSN, S2)
#define CHECK2(EXEC, INSN, CHAN, S1, S2, EXPECT) \
    DUMP2(EXEC, INSN, S1, S2)
#define CHECK3(EXEC, INSN, CHAN, S1, S2, S3, EXPECT) \
    DUMP3(EXEC, INSN, S1, S2, S3)
#else /* CHECKS */
#define CHECK1(EXEC, INSN, CHAN, S2, EXPECT) ({ \
    uint64_t res = DUMP1(EXEC, INSN, CHAN, S2); \
    if (res != EXPECT) { \
        total_fails += 1; \
        DUMP_FAILED_START(); \
        DUMP_RES(stderr, INSN, CHAN, res); \
        DUMP_SRC(stderr, S2); \
        DUMP_FAILED_END(EXPECT); \
    } \
})

#define CHECK2(EXEC, INSN, CHAN, S1, S2, EXPECT) ({ \
    uint64_t res = DUMP2(EXEC, INSN, CHAN, S1, S2); \
    if (res != EXPECT) { \
        total_fails += 1; \
        DUMP_FAILED_START(); \
        DUMP_RES(stderr, INSN, CHAN, res); \
        DUMP_SRC(stderr, S1); \
        DUMP_SRC(stderr, S2); \
        DUMP_FAILED_END(EXPECT); \
    } \
})

#define CHECK3(EXEC, INSN, CHAN, S1, S2, S3, EXPECT) ({ \
    uint64_t res = DUMP3(EXEC, INSN, CHAN, S1, S2, S3); \
    if (res != EXPECT) { \
        total_fails += 1; \
        DUMP_FAILED_START(); \
        DUMP_RES(stderr, INSN, CHAN, res); \
        DUMP_SRC(stderr, S1); \
        DUMP_SRC(stderr, S2); \
        DUMP_SRC(stderr, S3); \
        DUMP_FAILED_END(EXPECT); \
    } \
})
#endif /* CHECKS */

#define CHECK1_32(EXEC, INSN, CHAN, S2, EXPECT) \
    CHECK1(EXEC, INSN, CHAN, S2, (uint32_t) (EXPECT))

#define CHECK2_32(EXEC, INSN, CHAN, S1, S2, EXPECT) \
    CHECK2(EXEC, INSN, CHAN, S1, S2, (uint32_t) (EXPECT))

#define CHECK3_32(EXEC, INSN, CHAN, S1, S2, S3, EXPECT) \
    CHECK3(EXEC, INSN, CHAN, S1, S2, S3, (uint32_t) (EXPECT))

#endif /* TESTS_TCG_E2K_E2K_H */
