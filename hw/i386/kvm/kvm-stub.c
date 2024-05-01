#include "qemu/osdep.h"
#include "hw/isa/i8259_internal.h"

void kvm_pc_setup_irq_routing(bool pci_enabled)
{
    g_assert_not_reached();
}

void kvmclock_create(bool create_always)
{
    g_assert_not_reached();
}

qemu_irq *kvm_i8259_init(ISABus *bus)
{
    g_assert_not_reached();
}
