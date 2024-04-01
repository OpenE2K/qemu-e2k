/*
 * E2K specific proc functions for linux-user
 *
 * SPDX-License-Identifier: GPL-2.0-only
 */

#ifndef LINUX_USER_E2K_TARGET_PROC_H
#define LINUX_USER_E2K_TARGET_PROC_H

static int open_cpuinfo(CPUArchState *cpu_env, int fd)
{
    CPUE2KState *env = &env_archcpu(cpu_env)->env;
    int i, num_cpus, model, revision;

    model = extract32(env->def.idr, IDR_MDL_OFF, IDR_MDL_LEN);
    revision = extract32(env->def.idr, IDR_REV_OFF, IDR_REV_LEN);

    num_cpus = sysconf(_SC_NPROCESSORS_ONLN);
    for (i = 0; i < num_cpus; i++) {
        dprintf(fd, "processor\t: %d\n", i);
        dprintf(fd, "vendor_id\t: QEMU\n");
        dprintf(fd, "cpu family\t: %d\n", env->def.isa);
        dprintf(fd, "model\t\t: %d\n", model);
        dprintf(fd, "model name\t: %s\n", env->def.model_name);
        dprintf(fd, "revision\t: %d\n\n", revision);
    }

#if 0
    dprintf(fd, "cache0\t\t: level=1 type=Instruction scope=Private size=128K line_size=256 associativity=4\n");
    dprintf(fd, "cache1\t\t: level=1 type=Data scope=Private size=64K line_size=32 associativity=4\n");
    dprintf(fd, "cache2\t\t: level=2 type=Unified scope=Private size=512K line_size=64 associativity=4\n");
    dprintf(fd, "cache3\t\t: level=3 type=Unified scope=Shared size=16384K line_size=64 associativity=16\n");
#endif

    return 0;
}
#define HAVE_ARCH_PROC_CPUINFO

#endif /* LINUX_USER_E2K_TARGET_PROC_H */
