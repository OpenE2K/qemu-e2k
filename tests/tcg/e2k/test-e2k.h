#ifndef TESTS_TCG_E2K_E2K_H
#define TESTS_TCG_E2K_E2K_H

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#define glue(a, b) a ## b

#define ARRAY_SIZE(X) (sizeof(X) / sizeof(X[0]))

// Selector for getf{s,d}.
#define FIELD(O, L, S, B) ((O) | ((L) << 6) | ((S) << 12) | ((B) << 13))

struct e2k_test_data {
    uint64_t src1;
    uint64_t src2;
    uint64_t src3;
};

void report_fail1(const char *file, int line, const char *insn,
        int chan, uint64_t res, uint64_t expect,
        uint64_t src2);

void report_fail2(const char *file, int line, const char *insn,
        int chan, uint64_t res, uint64_t expect,
        uint64_t src1, uint64_t src2);

void report_fail2_merge(const char *file, int line, const char *insn,
        int chan, uint64_t res, uint64_t expect,
        uint64_t src1, uint64_t src2, int pred);

void report_fail3(const char *file, int line, const char *insn,
        int chan, uint64_t res, uint64_t expect,
        uint64_t src1, uint64_t src2, uint64_t src3);

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

#define EXEC2_MERGE(INSN, CHAN, S1, S2, PRED, C1, C2) ({ \
    uint64_t res = 0; \
    asm("cmpedb 1, " #PRED ", %%pred0\n\t" \
        #INSN "," #CHAN " %[src1], %[src2], %[dst], %%pred0" \
        : [dst]"+r"(res) \
        : [src1] #C1 ((uint64_t) S1), \
          [src2] #C2 ((uint64_t) S2) \
        : "pred0" \
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
#define EXEC_IR(INSN, CHAN, S1, S2) EXEC2(INSN, CHAN, S1, S2, I, r)
#define EXEC_RI(INSN, CHAN, S1, S2) EXEC2(INSN, CHAN, S1, S2, r, i)
#define EXEC_MERGE_XX(INSN, CHAN, S1, S2, PRED) EXEC2_MERGE(INSN, CHAN, S1, S2, PRED, rI, ri)
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

#define DUMP_FAILED_START(FILE, LINE) \
    fprintf(stderr, "Failed at %s:%d\n", FILE, LINE)

#define DUMP_FAILED_END(EXPECT) \
    fprintf(stderr, "\n    " INSN_FMT SRC_FMT "\n", "expected", SRC(EXPECT))

#define DUMP_RES(FILE, INSN, CHAN, X) \
    fprintf(FILE, "  %d " INSN_FMT RES_FMT, CHAN, #INSN, RES(X))
#define DUMP_SRC(FILE, X) \
    fprintf(FILE, SRC_FMT, SRC(X))
#define DUMP_PRED(FILE, X) \
    fprintf(FILE, " %d", (int) (X))
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

#define DUMP2_MERGE(EXEC, INSN, CHAN, S1, S2, PRED) ({ \
    uint64_t res = EXEC(INSN, CHAN, S1, S2, PRED); \
    DUMP_RES(stdout, INSN, CHAN, res); \
    DUMP_SRC(stdout, S1); \
    DUMP_SRC(stdout, S2); \
    DUMP_PRED(stdout, PRED); \
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

void report_fail1(const char *file, int line, const char *insn,
        int chan, uint64_t res, uint64_t expect,
        uint64_t src2)
{
#ifndef NO_CHECKS
    total_fails += 1;
    DUMP_FAILED_START(file, line);
    DUMP_RES(stderr, insn, chan, res);
    DUMP_SRC(stderr, src2);
    DUMP_FAILED_END(expect);
#endif
}

void report_fail2(const char *file, int line, const char *insn,
        int chan, uint64_t res, uint64_t expect,
        uint64_t src1, uint64_t src2)
{
#ifndef NO_CHECKS
    total_fails += 1;
    DUMP_FAILED_START(file, line);
    DUMP_RES(stderr, insn, chan, res);
    DUMP_SRC(stderr, src1);
    DUMP_SRC(stderr, src2);
    DUMP_FAILED_END(expect);
#endif
}

void report_fail2_merge(const char *file, int line, const char *insn,
        int chan, uint64_t res, uint64_t expect,
        uint64_t src1, uint64_t src2, int pred)
{
#ifndef NO_CHECKS
    total_fails += 1;
    DUMP_FAILED_START(file, line);
    DUMP_RES(stderr, insn, chan, res);
    DUMP_SRC(stderr, src1);
    DUMP_SRC(stderr, src2);
    DUMP_PRED(stderr, pred);
    DUMP_FAILED_END(expect);
#endif
}

void report_fail3(const char *file, int line, const char *insn,
        int chan, uint64_t res, uint64_t expect,
        uint64_t src1, uint64_t src2, uint64_t src3)
{
#ifndef NO_CHECKS
    total_fails += 1;
    DUMP_FAILED_START(file, line);
    DUMP_RES(stderr, insn, chan, res);
    DUMP_SRC(stderr, src1);
    DUMP_SRC(stderr, src2);
    DUMP_SRC(stderr, src3);
    DUMP_FAILED_END(expect);
#endif
}

#define CHECK1(EXEC, INSN, CHAN, S2, EXPECT) ({ \
    uint64_t res = DUMP1(EXEC, INSN, CHAN, S2); \
    if (res != EXPECT) \
        report_fail1(__FILE__, __LINE__, #INSN, CHAN, res, EXPECT, S1); \
})

#define CHECK2(EXEC, INSN, CHAN, S1, S2, EXPECT) ({ \
    uint64_t res = DUMP2(EXEC, INSN, CHAN, S1, S2); \
    if (res != EXPECT) \
        report_fail2(__FILE__, __LINE__, #INSN, CHAN, res, EXPECT, S1, S2); \
})

#define CHECK2_MERGE(EXEC, INSN, CHAN, S1, S2, PRED, EXPECT) ({ \
    uint64_t res = DUMP2_MERGE(EXEC, INSN, CHAN, S1, S2, PRED); \
    if (res != EXPECT) \
        report_fail2_merge(__FILE__, __LINE__, #INSN, CHAN, res, EXPECT, S1, S2, PRED); \
})

#define CHECK3(EXEC, INSN, CHAN, S1, S2, S3, EXPECT) ({ \
    uint64_t res = DUMP3(EXEC, INSN, CHAN, S1, S2, S3); \
    if (res != EXPECT) \
        report_fail3(__FILE__, __LINE__, #INSN, CHAN, res, EXPECT, S1, S2, S3); \
    } \
})

#define CHECK1_32(EXEC, INSN, CHAN, S2, EXPECT) \
    CHECK1(EXEC, INSN, CHAN, S2, (uint32_t) (EXPECT))

#define CHECK2_32(EXEC, INSN, CHAN, S1, S2, EXPECT) \
    CHECK2(EXEC, INSN, CHAN, S1, S2, (uint32_t) (EXPECT))

#define CHECK3_32(EXEC, INSN, CHAN, S1, S2, S3, EXPECT) \
    CHECK3(EXEC, INSN, CHAN, S1, S2, S3, (uint32_t) (EXPECT))

#endif /* TESTS_TCG_E2K_E2K_H */
