#include <inttypes.h>
#include <string.h>
#include <assert.h>
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
    assert(!(test->flags & FLAGS_XQ) && "use test_report_ptr");
    test_report_ptr(test, &expected,
        test->flags & HAS_SRC1 ? &src1 : NULL,
        test->flags & HAS_SRC2 ? &src2 : NULL,
        test->flags & HAS_SRC3 ? &src3 : NULL,
        test->flags & HAS_SRC4 ? &src4 : NULL
    );
}

static inline void print_value(FILE *out, flags_t flags, const uint64_t *x) {
    if (!x) {
        return;
    }
    if (flags & FLAGS_Q) {
        fprintf(out, " %016" PRIx64 ":%016" PRIx64, x[1], x[0]);
    } else if (flags & FLAGS_X) {
        fprintf(out, " %04x:%016" PRIx64, (uint16_t) x[1], x[0]);
    } else {
        fprintf(out, " %016" PRIx64, x[0]);
    }
}

void test_report_ptr(
    alc_test_t *test,
    const uint64_t *expected,
    const uint64_t *src1,
    const uint64_t *src2,
    const uint64_t *src3,
    const uint64_t *src4
) {
    bool fail = false;

    if (generate_expect) {
        int max = test->flags & FLAGS_Q ? 1 : (test->flags & FLAGS_X ? 2 : 3);
        if (test->flags & SKIP_GEN)
            goto skip;
        if (test->i == 0)
            printf("   ");
        if (test->flags & FLAGS_Q) {
            printf(" 0x%016" PRIx64 ", 0x%016" PRIx64 ",", test->result[0], test->result[1]);
        } else if (test->flags & FLAGS_X) {
            printf(" 0x%016" PRIx64 ", 0x%04x,", test->result[0], (uint16_t) test->result[1]);
        } else {
            printf(" 0x%016" PRIx64 ",", test->result[0]);
        }
        if (test->i >= max) {
            test->i = 0;
            printf("\n");
        } else {
            test->i += 1;
        }
    } else {
        printf(" ");
        print_value(stdout, test->flags, test->result);
        printf(" =");
        print_value(stdout, test->flags, src1);
        print_value(stdout, test->flags, src2);
        print_value(stdout, test->flags, src3);
        print_value(stdout, test->flags, src4);
        printf("\n");
    }

skip:
    for (int i = 0, j = 0; j < 6; ++j) {
        if ((test->channels & (ALC0 << j)) == 0) {
            continue;
        }
        if (test->flags & FLAGS_XQ) {
            if (test->result[i + 0] != expected[0] || test->result[i + 1] != expected[1]) {
                fail = true;
                break;
            }
            i += 2;
        } else {
            if (test->result[i] != expected[0]) {
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
    print_value(stderr, test->flags, src1);
    print_value(stderr, test->flags, src2);
    print_value(stderr, test->flags, src3);
    print_value(stderr, test->flags, src4);
    fprintf(stderr, "\n");

    fprintf(stderr, "  expected:");
    print_value(stderr, test->flags, expected);
    fprintf(stderr, "\n");

    for (int i = 0, j = 0; j < 6; ++j) {
        if (test->channels & (ALC0 << j)) {
            const char *err = "";
            if (test->result[i] != expected[0]) {
                err = " <<<< invalid result";
            }
            fprintf(stderr, "      alc%d:", j);
            print_value(stderr, test->flags, &test->result[i]);
            i += test->flags & FLAGS_XQ ? 2 : 1;
            fprintf(stderr, "%s\n", err);
        }
    }
    fprintf(stderr, "\n");

done:
    memset(test->result, 0, sizeof(test->result));
}
