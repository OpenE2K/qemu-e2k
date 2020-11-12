// include/exec/helper-proto.h needs this file

DEF_HELPER_2(raise_exception, noreturn, env, int)
DEF_HELPER_2(check_syscall_ctpr, void, env, i64)
