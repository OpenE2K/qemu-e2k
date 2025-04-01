#include "test-e2k.h"

int main(int argc, char *argv[]) {
    DUMP1(EXEC_X, movtd, 0x123456789abcdef);
    DUMP2(EXEC_XX, adds, 1, -123);
    DUMP2(EXEC_XX, subs, 1, -123);
    DUMP3(EXEC_XXX, add_subs, 1, -1, 123);
    return 0;
}
