#include "qemu/osdep.h"
#include "target/ppc/cpu.h"
#include "hw/ppc/spapr.h"
#include "hw/ppc/spapr_cpu_core.h"
#include "hw/ppc/spapr_xive.h"
#include "hw/ppc/xive.h"
#include "target/ppc/kvm_ppc.h"

int kvmppc_xive_pre_save(SpaprXive *xive)
{
    g_assert_not_reached();
}

int kvmppc_xive_post_load(SpaprXive *xive, int version_id)
{
    g_assert_not_reached();
}

void kvmppc_xive_synchronize_state(SpaprXive *xive, Error **errp)
{
    g_assert_not_reached();
}

int kvmppc_xive_source_reset_one(XiveSource *xsrc, int srcno, Error **errp)
{
    g_assert_not_reached();
}

void kvmppc_xive_source_set_irq(void *opaque, int srcno, int val)
{
    g_assert_not_reached();
}

int kvmppc_xive_connect(SpaprInterruptController *intc, uint32_t nr_servers,
                        Error **errp)
{
    g_assert_not_reached();
}

void kvmppc_xive_disconnect(SpaprInterruptController *intc)
{
    g_assert_not_reached();
}

int kvmppc_xive_set_source_config(SpaprXive *xive, uint32_t lisn, XiveEAS *eas,
                                  Error **errp)
{
    g_assert_not_reached();
}

int kvmppc_xive_set_queue_config(SpaprXive *xive, uint8_t end_blk,
                                 uint32_t end_idx, XiveEND *end,
                                 Error **errp)
{
    g_assert_not_reached();
}

int kvmppc_xive_get_queue_config(SpaprXive *xive, uint8_t end_blk,
                                 uint32_t end_idx, XiveEND *end,
                                 Error **errp)
{
    g_assert_not_reached();
}

uint64_t kvmppc_xive_esb_rw(XiveSource *xsrc, int srcno, uint32_t offset,
                            uint64_t data, bool write)
{
    g_assert_not_reached();
}

void kvmppc_xive_sync_source(SpaprXive *xive, uint32_t lisn, Error **errp)
{
    g_assert_not_reached();
}

void kvmppc_xive_reset(SpaprXive *xive, Error **errp)
{
    g_assert_not_reached();
}

int kvmppc_xive_cpu_synchronize_state(XiveTCTX *tctx, Error **errp)
{
    g_assert_not_reached();
}

int kvmppc_xive_cpu_connect(XiveTCTX *tctx, Error **errp)
{
    g_assert_not_reached();
}

int kvmppc_xive_cpu_get_state(XiveTCTX *tctx, Error **errp)
{
    g_assert_not_reached();
}

int kvmppc_xive_cpu_set_state(XiveTCTX *tctx, Error **errp)
{
    g_assert_not_reached();
}
