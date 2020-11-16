#include <asm/e2k_api.h>
#include <asm/unistd.h>

#define MSG "Hello, world!\n"

void _start(void) {
	__E2K_SYSCALL_3(LINUX_SYSCALL64_TRAPNUM, __NR_write, 1, MSG, sizeof(MSG));
	__E2K_SYSCALL_1(LINUX_SYSCALL64_TRAPNUM, __NR_exit, 0);
}
