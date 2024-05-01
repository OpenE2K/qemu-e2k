#include "qemu/osdep.h"

#include "kvm_loongarch.h"

int  kvm_loongarch_set_interrupt(LoongArchCPU *cpu, int irq, int level)
{
    g_assert_not_reached();
}

void kvm_arch_reset_vcpu(CPUState *cs)
{
    g_assert_not_reached();
}
