.set SYSCALL32_TRAPNUM, 0x1
.set SYSCALL64_TRAPNUM, 0x3

.set __NR_exit,  0x1
.set __NR_write, 0x4

.macro e2k_syscall_1 trapnum sysnum v1
{
    addd 0x0, \sysnum, %b[0]
    addd 0x0, \v1, %b[1]
    sdisp %ctpr1, \trapnum
}
{
    call %ctpr1, wbs = 0x4
}
.endm

.macro e2k_syscall_2 trapnum sysnum v1 v2
{
    sdisp %ctpr1, \trapnum
    addd 0x0, \sysnum, %b[0]
    addd 0x0, \v1, %b[1]
    addd 0x0, \v2, %b[2]
}
{
    call %ctpr1, wbs = 0x4
}
.endm

.macro e2k_syscall_3 trapnum sysnum v1 v2 v3
{
    sdisp %ctpr1, \trapnum
    addd 0x0, \sysnum, %b[0]
    addd 0x0, \v1, %b[1]
    addd 0x0, \v2, %b[2]
    addd 0x0, \v3, %b[3]
}
{
    call %ctpr1, wbs = 0x4
}
.endm

.macro e2k_syscall_4 trapnum sysnum v1 v2 v3 v4
{
    sdisp %ctpr1, \trapnum
    addd 0x0, \sysnum, %b[0]
    addd 0x0, \v1, %b[1]
    addd 0x0, \v2, %b[2]
    addd 0x0, \v3, %b[3]
    addd 0x0, \v4, %b[4]
}
{
    call %ctpr1, wbs = 0x4
}
.endm

.macro e2k_syscall_5 trapnum sysnum v1 v2 v3 v4 v5
{
    sdisp %ctpr1, \trapnum
    addd 0x0, \sysnum, %b[0]
    addd 0x0, \v1, %b[1]
    addd 0x0, \v2, %b[2]
    addd 0x0, \v3, %b[3]
    addd 0x0, \v4, %b[4]
    addd 0x0, \v5, %b[5]
}
{
    call %ctpr1, wbs = 0x4
}
.endm

.macro e2k_syscall_6 trapnum sysnum v1 v2 v3 v4 v5 v6
{
    sdisp %ctpr1, \trapnum
    addd 0x0, \sysnum, %b[0]
    addd 0x0, \v1, %b[1]
    addd 0x0, \v2, %b[2]
    addd 0x0, \v3, %b[3]
    addd 0x0, \v4, %b[4]
    addd 0x0, \v5, %b[5]
}
{
    addd 0x0, \v6, %b[5]
    call %ctpr1, wbs = 0x4
}
.endm

.macro e2k_syscall_7 trapnum sysnum v1 v2 v3 v4 v5 v6 v7
{
    sdisp %ctpr1, \trapnum
    addd 0x0, \sysnum, %b[0]
    addd 0x0, \v1, %b[1]
    addd 0x0, \v2, %b[2]
    addd 0x0, \v3, %b[3]
    addd 0x0, \v4, %b[4]
    addd 0x0, \v5, %b[5]
}
{
    addd 0x0, \v6, %b[6]
    addd 0x0, \v7, %b[7]
    call %ctpr1, wbs = 0x4
}
.endm

.macro assert op reg1 reg2 counter_reg
{
    \op \reg1, \reg2, %pred0
    addd 0x0, \counter_reg, %b[0]
    disp %ctpr1, _test_fail
}
{
    addd 0x1, \counter_reg, \counter_reg
    call %ctpr1, wbs = 0x4 ? ~%pred0
}
.endm

_test_fail:
{
    setwd wsz = 0x8, nfx = 0x0, dbl = 0x0
    setbn rsz = 0x3, rbs = 0x4, rcur = 0x0
}
    e2k_syscall_3 SYSCALL64_TRAPNUM, __NR_write, 0, [ _test_failed_str ], _test_failed_strlen
    e2k_syscall_1 SYSCALL64_TRAPNUM, __NR_exit, %dr0
