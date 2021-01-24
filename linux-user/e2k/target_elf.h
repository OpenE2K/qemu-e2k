#ifndef E2K_TARGET_ELF_H
#define E2K_TARGET_ELF_H

#define E2K_ELF_IPD_MASK  ((1U << 1)|(1U << 0))
#define E2K_ELF_X86APP    (1U << 2)
#define E2K_ELF_4MB_PAGES (1U << 3)
#define E2K_ELF_INCOMPAT  (1U << 4)
#define E2K_ELF_PM        (1U << 5)

#define E2K_ELF_OLD_MACH(x) (((x) >> 28) & 7)
#define E2K_ELF_MACH(x)     (((x) >> 24) & 255)

#define E2K_MACH_BASE 0
#define E2K_MACH_EV1  1
#define E2K_MACH_EV2  2
#define E2K_MACH_EV3  3
#define E2K_MACH_EV4  4
#define E2K_MACH_EV5  5
#define E2K_MACH_EV6  6

/* elbrus-v4 based processors */
#define E2K_MACH_8C     19
#define E2K_MACH_1CPLUS 20

const char *cpu_get_model(uint32_t eflags);

#endif /* E2K_TARGET_ELF_H */
