/*
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"

#include "kvm_s390x.h"

int kvm_s390_get_protected_dump(void)
{
    return false;
}

void kvm_s390_program_interrupt(S390CPU *cpu, uint16_t code)
{
    g_assert_not_reached();
}

void kvm_s390_reset_vcpu_clear(S390CPU *cpu)
{
    g_assert_not_reached();
}

void kvm_s390_reset_vcpu_normal(S390CPU *cpu)
{
    g_assert_not_reached();
}

void kvm_s390_reset_vcpu_initial(S390CPU *cpu)
{
    g_assert_not_reached();
}

#if defined(MCST_LCC_SUCKS) && !defined(CONFIG_SOFTMMU)
void run_on_cpu(CPUState *cpu, run_on_cpu_func func, run_on_cpu_data data)
{
    g_assert_not_reached();
}
#endif

int kvm_s390_vcpu_interrupt_post_load(S390CPU *cpu)
{
    g_assert_not_reached();
}

void kvm_s390_access_exception(S390CPU *cpu, uint16_t code, uint64_t te_code)
{
    g_assert_not_reached();
}

int kvm_s390_mem_op(S390CPU *cpu, vaddr addr, uint8_t ar, void *hostbuf,
                    int len, bool is_write)
{
    g_assert_not_reached();
}

int kvm_s390_mem_op_pv(S390CPU *cpu, vaddr addr, void *hostbuf, int len,
                       bool is_write)
{
    g_assert_not_reached();
}

int kvm_s390_set_cpu_state(S390CPU *cpu, uint8_t cpu_state)
{
    g_assert_not_reached();
}

int kvm_s390_set_mem_limit(uint64_t new_limit, uint64_t *hw_limit)
{
    g_assert_not_reached();
}

void kvm_s390_set_max_pagesize(uint64_t pagesize, Error **errp)
{
    g_assert_not_reached();
}

void kvm_s390_cmma_reset(void)
{
    g_assert_not_reached();
}

int kvm_s390_assign_subch_ioeventfd(EventNotifier *notifier, uint32_t sch,
                                    int vq, bool assign)
{
    g_assert_not_reached();
}

void kvm_s390_crypto_reset(void)
{
    g_assert_not_reached();
}

void kvm_s390_enable_css_support(S390CPU *cpu)
{
    g_assert_not_reached();
}

void kvm_s390_set_diag318(CPUState *cs, uint64_t diag318_info)
{
    g_assert_not_reached();
}

int kvm_s390_topology_set_mtcr(uint64_t attr)
{
    g_assert_not_reached();
}

void kvm_s390_restart_interrupt(S390CPU *cpu)
{
    g_assert_not_reached();
}

void kvm_s390_stop_interrupt(S390CPU *cpu)
{
    g_assert_not_reached();
}

int s390_pv_kvm_init(ConfidentialGuestSupport *cgs, Error **errp)
{
    g_assert_not_reached();
}

void kvm_s390_vcpu_interrupt_pre_save(S390CPU *cpu)
{
    g_assert_not_reached();
}
