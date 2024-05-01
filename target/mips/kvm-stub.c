#include "qemu/osdep.h"

#include "cpu.h"
#include "kvm_mips.h"

void kvm_mips_reset_vcpu(MIPSCPU *cpu)
{
    g_assert_not_reached();
}

int kvm_mips_set_interrupt(MIPSCPU *cpu, int irq, int level)
{
    g_assert_not_reached();
}

int kvm_mips_set_ipi_interrupt(MIPSCPU *cpu, int irq, int level)
{
    g_assert_not_reached();
}
