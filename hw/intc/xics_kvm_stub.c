#include "qemu/osdep.h"
#include "qapi/error.h"
#include "trace.h"
#include "sysemu/kvm.h"
#include "hw/ppc/spapr.h"
#include "hw/ppc/spapr_cpu_core.h"
#include "hw/ppc/xics.h"
#include "hw/ppc/xics_spapr.h"
#include "target/ppc/kvm_ppc.h"
#include "qemu/config-file.h"
#include "qemu/error-report.h"

#include <sys/ioctl.h>

void icp_synchronize_state(ICPState *icp)
{
    g_assert_not_reached();
}

void icp_get_kvm_state(ICPState *icp)
{
    g_assert_not_reached();
}

void ics_get_kvm_state(ICSState *ics)
{
    g_assert_not_reached();
}

int icp_set_kvm_state(ICPState *icp, Error **errp)
{
    g_assert_not_reached();
}

int ics_set_kvm_state(ICSState *ics, Error **errp)
{
    g_assert_not_reached();
}

int ics_set_kvm_state_one(ICSState *ics, int srcno, Error **errp)
{
    g_assert_not_reached();
}

void icp_kvm_realize(DeviceState *dev, Error **errp)
{
    g_assert_not_reached();
}

void ics_kvm_set_irq(ICSState *ics, int srcno, int val)
{
    g_assert_not_reached();
}

void ics_synchronize_state(ICSState *ics)
{
    g_assert_not_reached();
}

int xics_kvm_connect(SpaprInterruptController *intc, uint32_t nr_servers,
                     Error **errp)
{
    g_assert_not_reached();
}

void xics_kvm_disconnect(SpaprInterruptController *intc)
{
    g_assert_not_reached();
}
