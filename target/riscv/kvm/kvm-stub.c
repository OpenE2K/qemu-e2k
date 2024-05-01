/*
 * RISC-V stubs of KVM hooks
 */

#include "qemu/osdep.h"
#include "cpu.h"
#include "kvm_riscv.h"

void kvm_riscv_reset_vcpu(RISCVCPU *cpu)
{
    g_assert_not_reached();
}

void kvm_riscv_set_irq(RISCVCPU *cpu, int irq, int level)
{
    g_assert_not_reached();
}

void riscv_kvm_aplic_request(void *opaque, int irq, int level)
{
    g_assert_not_reached();
}

void riscv_kvm_cpu_finalize_features(RISCVCPU *cpu, Error **errp)
{
    g_assert_not_reached();
}

uint64_t kvm_riscv_get_timebase_frequency(RISCVCPU *cs)
{
    g_assert_not_reached();
}

void kvm_riscv_aia_create(MachineState *machine, uint64_t group_shift,
                          uint64_t aia_irq_num, uint64_t aia_msi_num,
                          uint64_t aplic_base, uint64_t imsic_base,
                          uint64_t guest_num)
{
    g_assert_not_reached();
}
