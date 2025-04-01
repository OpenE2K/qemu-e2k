#ifndef TESTS_TCG_E2K_E2K_H
#define TESTS_TCG_E2K_E2K_H

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

static int total_fails = 0;
#define is_failed() (total_fails > 0)

#define EXEC1(INSN, S2, C2) ({ \
    uint64_t res = 0; \
    asm(#INSN " %[src2], %[dst]" \
        : [dst]"=r"(res) \
        : [src2] #C2 ((uint64_t) S2) \
    ); \
    res; \
})

#define EXEC2(INSN, S1, S2, C1, C2) ({ \
    uint64_t res = 0; \
    asm(#INSN " %[src1], %[src2], %[dst]" \
        : [dst]"=r"(res) \
        : [src1] #C1 ((uint64_t) S1), \
          [src2] #C2 ((uint64_t) S2) \
    ); \
    res; \
})

#define EXEC3(INSN, S1, S2, S3, C1, C2, C3) ({ \
    uint64_t res = 0; \
    asm(#INSN " %[src1], %[src2], %[src3], %[dst]" \
        : [dst]"=r"(res) \
        : [src1] #C1 ((uint64_t) S1), \
          [src2] #C2 ((uint64_t) S2), \
          [src3] #C3 ((uint64_t) S3) \
    ); \
    res; \
})

#define EXEC_X(INSN, S2) EXEC1(INSN, S2, ri)
#define EXEC_R(INSN, S2) EXEC1(INSN, S2, r)
#define EXEC_XX(INSN, S1, S2) EXEC2(INSN, S1, S2, rI, ri)
#define EXEC_RR(INSN, S1, S2) EXEC2(INSN, S1, S2, r, r)
#define EXEC_XXX(INSN, S1, S2, S3) EXEC3(INSN, S1, S2, S3, rI, ri, r)
#define EXEC_RRR(INSN, S1, S2, S3) EXEC3(INSN, S1, S2, S3, r, r, r)

#define INSN_FMT "%-16s"
#define RES_FMT " %016" PRIx64 " ="
#define SRC_FMT " %016" PRIx64

#define AS_U64(X) ((uint64_t) (X))
#define RES AS_U64
#define SRC AS_U64

#define DUMP1(EXEC, INSN, S2) ({ \
    uint64_t res = 0; \
    res = EXEC(INSN, S2); \
    printf(INSN_FMT RES_FMT SRC_FMT "\n", \
        #INSN, RES(res), SRC(S2)); \
    res; \
})

#define DUMP2(EXEC, INSN, S1, S2) ({ \
    uint64_t res = 0; \
    res = EXEC(INSN, S1, S2); \
    printf(INSN_FMT RES_FMT SRC_FMT SRC_FMT "\n", \
        #INSN, RES(res), SRC(S1), SRC(S2)); \
    res; \
})

#define DUMP3(EXEC, INSN, S1, S2, S3) ({ \
    uint64_t res = 0; \
    res = EXEC(INSN, S1, S2, S3); \
    printf(INSN_FMT RES_FMT SRC_FMT SRC_FMT SRC_FMT "\n", \
        #INSN, RES(res), SRC(S1), SRC(S2), SRC(S3)); \
    res; \
})

#ifdef NO_CHECKS
#define CHECK1(EXEC, INSN, S2, EXPECT) \
    DUMP1(EXEC, INSN, S2)
#define CHECK2(EXEC, INSN, S1, S2, EXPECT) \
    DUMP2(EXEC, INSN, S1, S2)
#define CHECK3(EXEC, INSN, S1, S2, S3, EXPECT) \
    DUMP3(EXEC, INSN, S1, S2, S3)
#else /* CHECKS */
#define CHECK1(EXEC, INSN, S2, EXPECT) ({ \
    uint64_t res = DUMP1(EXEC, INSN, S2); \
    if (res != EXPECT) { \
        total_fails += 1; \
        fprintf(stderr, "Failed at %s:%d\n" \
            "  " INSN_FMT RES_FMT SRC_FMT "\n" \
            "  " INSN_FMT SRC_FMT "\n", \
            __FILE__, __LINE__, \
            #INSN, RES(res), SRC(S2), \
            "expected", SRC(EXPECT)); \
    } \
})

#define CHECK2(EXEC, INSN, S1, S2, EXPECT) ({ \
    uint64_t res = DUMP2(EXEC, INSN, S1, S2); \
    if (res != EXPECT) { \
        total_fails += 1; \
        fprintf(stderr, "Failed at %s:%d\n" \
            "  " INSN_FMT RES_FMT SRC_FMT SRC_FMT "\n" \
            "  " INSN_FMT SRC_FMT "\n", \
            __FILE__, __LINE__, \
            #INSN, RES(res), SRC(S1), SRC(S2), \
            "expected", SRC(EXPECT)); \
    } \
})

#define CHECK3(EXEC, INSN, S1, S2, S3, EXPECT) ({ \
    uint64_t res = DUMP3(EXEC, INSN, S1, S2, S3); \
    if (res != EXPECT) { \
        total_fails += 1; \
        fprintf(stderr, "Failed at %s:%d\n" \
            "  " INSN_FMT RES_FMT SRC_FMT SRC_FMT SRC_FMT "\n" \
            "  " INSN_FMT SRC_FMT "\n", \
            __FILE__, __LINE__, \
            #INSN, RES(res), SRC(S1), SRC(S2), SRC(S3), \
            "expected", SRC(EXPECT)); \
    } \
})
#endif /* CHECKS */

#define CHECK1_32(EXEC, INSN, S2, EXPECT) \
    CHECK1(EXEC, INSN, S2, (uint32_t) (EXPECT))

#define CHECK2_32(EXEC, INSN, S1, S2, EXPECT) \
    CHECK2(EXEC, INSN, S1, S2, (uint32_t) (EXPECT))

#define CHECK3_32(EXEC, INSN, S1, S2, S3, EXPECT) \
    CHECK3(EXEC, INSN, S1, S2, S3, (uint32_t) (EXPECT))

#endif /* TESTS_TCG_E2K_E2K_H */
