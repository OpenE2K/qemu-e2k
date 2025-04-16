#include "test.h"

static bool generate_expect = false;
static flags_t global_flags = 0;
static int total_fails = 0;

bool is_failed(void) {
    return total_fails > 0;
}

void skip_gen(bool skip) {
    global_flags = skip ? (global_flags | SKIP_GEN) : (global_flags & ~SKIP_GEN);
}

void parse_args(int argc, char *argv[]) {
    if (argc > 1) {
        generate_expect = strcmp(argv[1], "-g") == 0;
    }
    if (generate_expect) {
        printf("/* AUTO-GENERATED! DO NOT EDIT! */\n\n");
    }
}

alc_test_t test_start(const char *insn, const char *comment, alc_t channels, flags_t flags) {
    alc_test_t test = { 0 };
    test.insn = insn;
    test.comment = comment;
    test.channels = channels;
    test.flags = global_flags | flags;
    test.i = 0;
    if (generate_expect) {
        if (test.flags & SKIP_GEN)
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

void test_end(alc_test_t *test) {
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

void test_report(
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
