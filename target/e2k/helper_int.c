#include "qemu/osdep.h"
#include "qemu/log.h"
#include "cpu.h"
#include "helper-tcg.h"
#include "exec/exec-all.h"
#include "qemu/host-utils.h"
#include "exec/helper-proto.h"

uint64_t HELPER(sxt)(uint32_t s1, uint32_t s2)
{
    int size = MIN(32, 8 << (s1 & 3));
    return s1 & 4 ? extract32(s2, 0, size) : sextract64(s2, 0, size);
}

static uint64_t cr_read(CPUE2KState *env, size_t offset)
{
    target_ulong addr = env->pcsp.base + env->pcsp.index + offset;
    return cpu_ldq_le_data(env, addr);
}

uint64_t HELPER(state_reg_get)(CPUE2KState *env, int index)
{
    switch (index) {
    case SR_PSR:        return env->psr;
    case SR_WD:         return env_wd_get(env);
    case SR_CORE_MODE:  return env->core_mode;
    case SR_CWD:        return 0; /* ignore */
    case SR_PSP_HI:     return env->psp.hi;
    case SR_PSP_LO:     return env->psp.lo;
    case SR_PSHTP:      return 0; /* ignore */
    case SR_PCSP_HI:    return env->pcsp.hi;
    case SR_PCSP_LO:    return env->pcsp.lo;
    case SR_PCSHTP:     return 0; /* ignore */
    case SR_CTPR1:      return env->ctprs[0].raw;
    case SR_CTPR2:      return env->ctprs[1].raw;
    case SR_CTPR3:      return env->ctprs[2].raw;
    case SR_SBR:        return env->sbr;
    case SR_CUTD:
    case SR_EIR:
    case SR_TSD:        goto todo;
    case SR_CUIR:       return env->cuir;
    case SR_OSCUD_HI:   return env->oscud.hi;
    case SR_OSCUD_LO:   return env->oscud.lo;
    case SR_OSGD_HI:    return env->osgd.hi;
    case SR_OSGD_LO:    return env->osgd.lo;
    case SR_OSEM:       goto todo;
    case SR_USD_HI:     return env->usd.hi;
    case SR_USD_LO:     return env->usd.lo;
    case SR_TR:         goto todo;
    case SR_OSR0:       return env->osr0;
    case SR_CUD_HI:     return env->cud.hi;
    case SR_CUD_LO:     return env->cud.lo;
    case SR_GD_HI:      return env->gd.hi;
    case SR_GD_LO:      return env->gd.lo;
    case SR_CS_HI:      return env->cs.hi;
    case SR_CS_LO:      return env->cs.lo;
    case SR_DS_HI:      return env->ds.hi;
    case SR_DS_LO:      return env->ds.lo;
    case SR_ES_HI:      return env->es.hi;
    case SR_ES_LO:      return env->es.lo;
    case SR_FS_HI:      return env->fs.hi;
    case SR_FS_LO:      return env->fs.lo;
    case SR_GS_HI:      return env->gs.hi;
    case SR_GS_LO:      return env->gs.lo;
    case SR_SS_HI:      return env->ss.hi;
    case SR_SS_LO:      return env->ss.lo;
    case SR_DIBCR:
    case SR_DIMCR:
    case SR_DIBSR:
    case SR_DTCR:
    case SR_DIBAR0:
    case SR_DIBAR1:
    case SR_DIBAR2:
    case SR_DIBAR3:
    case SR_DIMAR0:
    case SR_DIMAR1:
    case SR_DTRAF:
    case SR_DTART:      goto todo;
    case SR_CR0_HI:     return cr_read(env, offsetof(E2KCrs, cr0_hi));
    case SR_CR0_LO:     return cr_read(env, offsetof(E2KCrs, cr0_lo));
    case SR_CR1_HI:     return cr_read(env, offsetof(E2KCrs, cr1.hi));
    case SR_CR1_LO:     return cr_read(env, offsetof(E2KCrs, cr1.lo));
    case SR_SCLKM1:
    case SR_SCLKM2:
    case SR_CU_HW0:     goto todo;
    case SR_UPSR:       return env->upsr;
    case SR_IP:         return env->ip;
    case SR_NIP:        return env->nip;
    case SR_LSR:        return env_lsr_get(env);
    case SR_PFPFR:      return env->pfpfr.raw;
    case SR_FPCR:       return env->fpcr.raw;
    case SR_FPSR:       return env->fpsr.raw;
    case SR_ILCR:       return env_ilcr_get(env);
    case SR_BR:         return env_br_get(env);
    case SR_BGR:        return env->bgr;
    case SR_IDR:        return env->idr;
    case SR_CLKR:       return cpu_get_host_ticks(); // FIXME
    case SR_RNDPR:
    case SR_SCLKR:
    case SR_TIR_HI:
    case SR_TIR_LO:
    case SR_RPR:
    case SR_SBBP:
    case SR_RPR_HI:
    case SR_UPSRM:      goto todo;
    case SR_LSR1:       return env->lsr_lcnt;
    case SR_ILCR1:      return env->ilcr_lcnt;
    default:
        /* The index is checked during translation. */
        g_assert_not_reached();
        break;
    }

todo:
    qemu_log("reading a state register %#x is not implemented\n", index);
    return 0;
}

#define CASE_RWAP_SET(e, f) \
    case glue(e, _HI): \
        env->f.hi = value; \
        break; \
    case glue(e, _LO): \
        env->f.lo = value & E2K_RWAP_LO_MASK; \
        break;

void HELPER(state_reg_set)(CPUE2KState *env, uint64_t value, int index)
{
    switch (index) {
    case SR_UPSR:
    case SR_LSR:
    case SR_ILCR:
    case SR_PFPFR:
    case SR_FPCR:
    case SR_FPSR:
    case SR_SCLKR:
    case SR_TIR_HI:
    case SR_TIR_LO:
    case SR_RPR:
    case SR_SBBP:
    case SR_RPR_HI:
    case SR_UPSRM:
    case SR_LSR1:
    case SR_ILCR1:
        break;
    case SR_CUIR:
        raise_exception(env, EXCP_ILLEGAL_OPCODE); /* FIXME */
        break;
    default:
        if ((env->psr & PSR_PM) == 0) {
            raise_exception(env, EXCP_PRIV_ACTION);
        }
        break;
    }

    switch (index) {
    case SR_PSR:
        env->psr = value & PSR_MASK;
        break;
    case SR_WD:
        env_wd_set(env, value);
        break;
    case SR_CORE_MODE:
        // FIXME: core_mode
        qemu_log("FIXME: core_mode is set to %#lx\n", value);
        env->core_mode = value;
        break;
    case SR_CWD:
        /* ignore */
        break;
    case SR_PSP_HI:
        env->psp.hi = value;
        break;
    case SR_PSP_LO:
        env->psp.lo = value & E2K_PSP_LO_MASK;
        break;
    case SR_PSHTP:
        /* ignore */
        break;
    case SR_PCSP_HI:
        env->pcsp.hi = value;
        break;
    case SR_PCSP_LO:
        env->pcsp.lo = value & E2K_PSP_LO_MASK;
        break;
    case SR_PCSHTP:
        /* ignore */
        break;
    case SR_CTPR1:
        env->ctprs[0].raw = value;
        break;
    case SR_CTPR2:
        env->ctprs[1].raw = value;
        break;
    case SR_CTPR3:
        env->ctprs[2].raw = value;
        break;
    case SR_SBR:
        env->sbr = value;
        break;
    case SR_CUTD:
    case SR_EIR:
    case SR_TSD:
        goto todo;
    case SR_CUIR:
        env->cuir = value & CUIR_MASK;
        break;
    case SR_OSEM:
    case SR_TR:
        goto todo;
    case SR_OSR0:
        env->osr0 = value;
        break;
    CASE_RWAP_SET(SR_OSCUD, oscud)
    CASE_RWAP_SET(SR_OSGD, osgd)
    CASE_RWAP_SET(SR_USD, usd)
    CASE_RWAP_SET(SR_CUD, cud)
    CASE_RWAP_SET(SR_GD, gd)
    CASE_RWAP_SET(SR_CS, cs)
    CASE_RWAP_SET(SR_DS, ds)
    CASE_RWAP_SET(SR_ES, es)
    CASE_RWAP_SET(SR_FS, fs)
    CASE_RWAP_SET(SR_GS, gs)
    CASE_RWAP_SET(SR_SS, ss)
    case SR_DIBCR:
    case SR_DIMCR:
    case SR_DIBSR:
    case SR_DTCR:
    case SR_DIBAR0:
    case SR_DIBAR1:
    case SR_DIBAR2:
    case SR_DIBAR3:
    case SR_DIMAR0:
    case SR_DIMAR1:
    case SR_DTRAF:
    case SR_DTART:
    case SR_CR0_HI:
    case SR_CR0_LO:
    case SR_CR1_HI:
    case SR_CR1_LO:
    case SR_SCLKM1:
    case SR_SCLKM2:
    case SR_CU_HW0:
        goto todo;
    case SR_UPSR:
        env->upsr = value & UPSR_MASK;
        break;
    case SR_LSR:
        env_lsr_set(env, value);
        env_ilcr_set(env, value);
        break;
    case SR_PFPFR:
        env->pfpfr.raw = value;
        e2k_update_fp_status(env);
        break;
    case SR_FPCR:
        env->fpcr.raw = value;
        e2k_update_fx_status(env);
        break;
    case SR_FPSR:
        env->fpsr.raw = value;
        break;
    case SR_ILCR:
        env->ilcr = (value >> LSR_ECNT_OFF) & ILCR_MASK;
        env->ilcr_lcnt = value & GEN_MASK(0, LSR_LCNT_LEN);
        break;
    case SR_BR:
        env_br_set(env, value);
        break;
    case SR_BGR:
        env->bgr = value & BGR_MASK;
        break;
    case SR_RNDPR:
    case SR_SCLKR:
    case SR_TIR_HI:
    case SR_TIR_LO:
    case SR_RPR:
    case SR_SBBP:
    case SR_RPR_HI:
    case SR_UPSRM:
        goto todo;
    case SR_LSR1:
        env->lsr_lcnt = value;
        env->ilcr_lcnt = value;
        break;
    case SR_ILCR1:
        env->ilcr_lcnt = value;
        break;
    default:
        /* The index is checked during translation. */
        g_assert_not_reached();
        break;
    }

    return;
todo:
    qemu_log("write to state register %#x is not implemented\n", index);
}

uint64_t HELPER(getsp)(CPUE2KState *env, uint32_t src2)
{
    int32_t s2 = src2 & ~0xf;

    env->usd.base += s2;
    env->usd.size -= s2;

    return env->usd.base;
}

void HELPER(dam_lock_addr)(CPUE2KState *env, uint64_t addr, int size, int reg)
{
    // TODO: helper_dam_lock_addr
}

int HELPER(dam_unlock_addr)(CPUE2KState *env, uint64_t addr, int size, int reg)
{
    // TODO: helper_dam_unlock_addr
    return 1;
}

uint64_t HELPER(clmull)(uint64_t s1, uint64_t s2)
{
    uint64_t r = 0;

    for (; s1; s1 >>= 1, s2 <<= 1) {
        r ^= s1 & 1 ? s2 : 0;
    }

    return r;
}

uint64_t HELPER(clmulh)(uint64_t s1, uint64_t s2)
{
    uint64_t r = 0;

    for (s2 >>= 1; s1; s1 <<= 1, s2 >>= 1) {
        r ^= s1 & (1ULL << 63) ? s2 : 0;
    }

    return r;
}
