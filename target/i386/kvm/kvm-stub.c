/*
 * x86 KVM stubs
 */

#include "qemu/osdep.h"
#include "cpu.h"
#include "kvm_i386.h"

uint32_t kvm_arch_get_supported_cpuid(KVMState *env, uint32_t function,
                                      uint32_t index, int reg)
{
    g_assert_not_reached();
}

void kvm_arch_reset_vcpu(X86CPU *cs)
{
    g_assert_not_reached();
}

void kvm_arch_after_reset_vcpu(X86CPU *cpu)
{
    g_assert_not_reached();
}

void kvm_arch_do_init_vcpu(X86CPU *cs)
{
    g_assert_not_reached();
}

uint64_t kvm_arch_get_supported_msr_feature(KVMState *s, uint32_t index)
{
    g_assert_not_reached();
}

void kvm_request_xsave_components(X86CPU *cpu, uint64_t mask)
{
    g_assert_not_reached();
}

void kvm_set_max_apic_id(uint32_t max_apic_id)
{
    g_assert_not_reached();
}

bool kvm_has_smm(void)
{
    g_assert_not_reached();
}

bool kvm_enable_sgx_provisioning(KVMState *s)
{
    g_assert_not_reached();
}
