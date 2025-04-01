// #define NO_CHECKS

#include "test-e2k.h"

int main(int argc, char *argv[]) {
    DUMP1(EXEC_X, movtd, 0x123456789abcdef);
    DUMP2(EXEC_XX, adds, 1, -123);
    DUMP2(EXEC_XX, subs, 1, -123);
    DUMP3(EXEC_XXX, add_subs, 1, -1, 123);

    CHECK1(EXEC_X, movtd, 0x123456789abcdef, 0x123456789abcdef);
    CHECK2(EXEC_XX, adds, 1, -123, (uint32_t) -122);
    CHECK2(EXEC_XX, subs, 1, -123, 124);
    CHECK3(EXEC_XXX, add_subs, 1, -1, 123, 123);

    return is_failed();
}
