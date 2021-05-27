_stop:
    e2k_syscall_1 SYSCALL64_TRAPNUM, __NR_exit, 0

_test_failed_str:
    .ascii "TEST FAILED\n\0"
    .set _test_failed_strlen, . - _test_failed_str
