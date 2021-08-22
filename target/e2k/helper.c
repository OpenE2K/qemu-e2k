#include "qemu/osdep.h"
#include "cpu.h"
#include "exec/exec-all.h"
#include "qemu/log.h"
#include "sysemu/runstate.h"
#include "exec/helper-proto.h"
#include "exec/address-spaces.h"
#include "helper-tcg.h"
#include "trace.h"
#include "hw/e2k/memmap.h"

static inline void reset_ctprs(CPUE2KState *env)
{
    unsigned int i;

    for (i = 0; i < 3; i++) {
        env->ctprs[i].tag = CTPR_TAG_NONE;
    }
}

static inline void ps_push(CPUE2KState *env, uint64_t value, uint8_t tag)
{
#ifndef CONFIG_USER_ONLY
    if ((env->psp.index + 8) > env->psp.size) {
        raise_exception(env, EXCP_PROC_STACK_BOUNDS);
    }
#endif

    cpu_stq_le_data(env, env->psp.base + env->psp.index, value);
    if (env->enable_tags) {
        cpu_stb_data(env, env->psp.base_tag + env->psp.index / 8, tag);
    }
    env->psp.index += 8;
}

static inline uint64_t ps_pop(CPUE2KState *env, uint8_t *ret_tag)
{
    if (env->psp.index < 8) {
        raise_exception(env, EXCP_PROC_STACK_BOUNDS);
    }
    env->psp.index -= 8;
    if (ret_tag != NULL) {
        if (env->enable_tags) {
            abi_ptr ptr = env->psp.base_tag + env->psp.index / 8;
            *ret_tag = cpu_ldub_data(env, ptr);
        } else {
            *ret_tag = 0;
        }
    }
    return cpu_ldq_le_data(env, env->psp.base + env->psp.index);
}


static void ps_spill(CPUE2KState *env, int n, bool fx)
{
    int i;

    if (env->version >= 5) {
        for (i = 0; i < n; i++) {
            ps_push(env, env->regs[i].lo, env->tags[i]);
            if (fx || E2K_FORCE_FX) {
                ps_push(env, env->regs[i].hi, 0);
            }
        }
    } else{
        for (i = 0; i < n; i += 2) {
            E2KReg r0 = env->regs[i + 0];
            E2KReg r1 = env->regs[i + 1];
            ps_push(env, r0.lo, env->tags[i]);
            ps_push(env, r1.lo, env->tags[i + 1]);
            if (fx || E2K_FORCE_FX) {
                ps_push(env, r0.hi, 0);
                ps_push(env, r1.hi, 0);
            }
        }
    }
}

static void ps_fill(CPUE2KState *env, int n, bool fx)
{
    int i;

    if (env->version >= 5) {
        for (i = n; i-- > 0;) {
            if (fx || E2K_FORCE_FX) {
                env->regs[i].hi = ps_pop(env, NULL);
            }
            env->regs[i].lo = ps_pop(env, &env->tags[i]);
        }
    } else {
        for (i = n; i > 0; i -= 2) {
            E2KReg *r0 = &env->regs[i - 1];
            E2KReg *r1 = &env->regs[i - 2];
            if (fx || E2K_FORCE_FX) {
                r0->hi = ps_pop(env, NULL);
                r1->hi = ps_pop(env, NULL);
            }
            r0->lo = ps_pop(env, &env->tags[i - 1]);
            r1->lo = ps_pop(env, &env->tags[i - 2]);
        }
    }
}

static void move_regs(CPUE2KState *env, int dst, int src, int n)
{
    memmove(&env->regs[dst], &env->regs[src], n * sizeof(env->regs[0]));
    if (env->enable_tags) {
        memmove(&env->tags[dst], &env->tags[src], n * sizeof(env->tags[0]));
    }
}

static void callee_window(CPUE2KState *env, int base, int size, bool fx)
{
    ps_spill(env, base, fx);
    move_regs(env, 0, base, size - base);
}

static void caller_window(CPUE2KState *env, int base, int params, bool fx)
{
    move_regs(env, base, 0, params);
    ps_fill(env, base, fx);
}

static void crs_write(CPUE2KState *env, target_ulong addr, E2KCrs *crs)
{
    cpu_stq_le_data(env, addr + offsetof(E2KCrs, cr0_lo), crs->cr0_lo);
    cpu_stq_le_data(env, addr + offsetof(E2KCrs, cr0_hi), crs->cr0_hi);
    cpu_stq_le_data(env, addr + offsetof(E2KCrs, cr1.lo), crs->cr1.lo);
    cpu_stq_le_data(env, addr + offsetof(E2KCrs, cr1.hi), crs->cr1.hi);
}

static void crs_read(CPUE2KState *env, target_ulong addr, E2KCrs *crs)
{
    crs->cr0_lo = cpu_ldq_le_data(env, addr + offsetof(E2KCrs, cr0_lo));
    crs->cr0_hi = cpu_ldq_le_data(env, addr + offsetof(E2KCrs, cr0_hi));
    crs->cr1.lo = cpu_ldq_le_data(env, addr + offsetof(E2KCrs, cr1.lo));
    crs->cr1.hi = cpu_ldq_le_data(env, addr + offsetof(E2KCrs, cr1.hi));
}

static void pcs_push(CPUE2KState *env, E2KCrs *crs)
{
#ifndef CONFIG_USER_ONLY
    if ((env->pcsp.index + sizeof(E2KCrs) * 2) > env->pcsp.size) {
        raise_exception(env, EXCP_CHAIN_STACK_BOUNDS);
    }
#endif

    env->pcsp.index += sizeof(E2KCrs);
    crs_write(env, env->pcsp.base + env->pcsp.index, crs);
}

static void pcs_pop(CPUE2KState *env, E2KCrs *crs)
{
    crs_read(env, env->pcsp.base + env->pcsp.index, crs);

    if (env->pcsp.index < sizeof(E2KCrs)) {
        raise_exception(env, EXCP_CHAIN_STACK_BOUNDS);
    } else {
        env->pcsp.index -= sizeof(E2KCrs);
    }
}

void e2k_proc_call(CPUE2KState *env, int base, target_ulong ret_ip,
    bool force_fx)
{
    E2KCrs crs;

    crs.cr0_lo = env->pregs;
    crs.cr0_hi = ret_ip & ~7;
    crs.cr1.wbs = base / 2;
    crs.cr1.wpsz = env->wd.psize / 2;
    crs.cr1.wfx = env->wd.fx;
    crs.cr1.wdbl = env->wdbl;
    crs.cr1.psr = env->psr;
    crs.cr1.cuir = env->cuir;
    crs.cr1.br = env_br_get(env);
    crs.cr1.ussz = env->usd.size >> 4;

    pcs_push(env, &crs);
    callee_window(env, base, env->wd.size, env->wd.fx || force_fx);

    env->wd.fx = true;
    env->wd.size -= base;
    env->wd.psize = env->wd.size;
}

void e2k_proc_return(CPUE2KState *env, bool force_fx)
{
    E2KCrs crs;
    int base;

    pcs_pop(env, &crs);
    base = crs.cr1.wbs * 2;

    caller_window(env, base, env->wd.psize, crs.cr1.wfx || force_fx);

    env->pregs = crs.cr0_lo;
    env->ip = crs.cr0_hi & ~7;
    env_br_set(env, crs.cr1.br);
    env->cuir = crs.cr1.cuir;
    env->psr = crs.cr1.psr;
    env->wd.size = env->wd.psize + base;
    env->wd.psize = crs.cr1.wpsz * 2;
    env->wd.fx = crs.cr1.wfx;
    env->wdbl = crs.cr1.wdbl;
    env->usd.size = crs.cr1.ussz << 4;
    env->usd.base = env->sbr - env->usd.size;
}

static inline void do_call(CPUE2KState *env, int wbs, target_ulong ret_ip)
{
    e2k_proc_call(env, wbs * 2, ret_ip, false);
    reset_ctprs(env);
}

void HELPER(call)(CPUE2KState *env, uint64_t ctpr_raw, int call_wbs,
    target_ulong pc_next)
{
    E2KCtpr ctpr = { .raw = ctpr_raw };

    switch (ctpr.tag) {
    case CTPR_TAG_DISP:
    case CTPR_TAG_SDISP:
        do_call(env, call_wbs, pc_next);
        env->ip = ctpr.base;
        break;
    default:
        raise_exception(env, EXCP_ILLEGAL_OPCODE);
        break;
    }
}

#ifdef CONFIG_USER_ONLY
void HELPER(expand_stacks)(CPUE2KState *env)
{
    if ((env->psp.size - env->psp.index) <= (E2K_REG_LEN * E2K_NR_COUNT * 4)) {
        raise_exception_ra(env, EXCP_PROC_STACK_BOUNDS, GETPC());
    }

    if ((env->pcsp.size - env->pcsp.index) <= (sizeof(E2KCrs) * 2)) {
        raise_exception_ra(env, EXCP_CHAIN_STACK_BOUNDS, GETPC());
    }
}
#endif /* CONFIG_USER_ONLY */

uint64_t HELPER(prep_return)(CPUE2KState *env, int ipd)
{
    E2KCtpr ret = { 0 };
    target_ulong addr = env->pcsp.base + env->pcsp.index + offsetof(E2KCrs, cr0_hi);
    uint64_t cr0_hi = cpu_ldq_le_data(env, addr) & ~7;

    ret.ipd = ipd;
    ret.base = cr0_hi;
    ret.tag = CTPR_TAG_RETURN;
#ifdef CONFIG_USER_ONLY
    ret.opc = cr0_hi == E2K_SIGRET_ADDR ? CTPR_OPC_SIGRET : 0;
#else
    // TODO: set ctpr.opc
    ret.opc = 0;
#endif

    return ret.raw;
}

void HELPER(return)(CPUE2KState *env)
{
    CtprOpc opc = env->ctprs[2].opc;

#ifdef CONFIG_USER_ONLY
    if (opc == CTPR_OPC_SIGRET) {
        CPUState *cs = env_cpu(env);
        env->wd.psize = 2;
        env->regs[0].lo = 119; /* TARGET_NR_sigreturn */
        env->tags[0] = E2K_TAG_NUMBER64;
        cs->exception_index = EXCP_SYSCALL;
        cpu_loop_exit(cs);
    }
#endif

    if (opc != 0) {
        qemu_log(TARGET_FMT_lx ": unknown return ctpr opc %d\n", env->ip, opc);
    }

    e2k_proc_return(env, false);
    reset_ctprs(env);
}

void QEMU_NORETURN raise_exception(CPUE2KState *env, int exception_index)
{
    raise_exception_ra(env, exception_index, 0);
}

void QEMU_NORETURN raise_exception_ra(CPUE2KState *env, int exception_index,
    uintptr_t retaddr)
{
    CPUState *cs = env_cpu(env);
    switch (exception_index) {
#ifdef CONFIG_USER_ONLY
    case EXCP_SYSCALL:
#endif
    case EXCP_PROC_STACK_BOUNDS:
    case EXCP_CHAIN_STACK_BOUNDS:
        /* ignore */
        break;
    default:
        e2k_proc_call(env, env->wd.size, env->ip, true);
        break;
    }
    cs->exception_index = exception_index;
    cpu_loop_exit_restore(cs, retaddr);
}

void HELPER(raise_exception)(CPUE2KState *env, int exception_index)
{
    raise_exception(env, exception_index);
}

void HELPER(setwd)(CPUE2KState *env, int wsz, int nfx, int dbl)
{
    int size, diff;

    size = wsz * 2;
    diff = size - env->wd.size;

    if (size < env->wd.psize) {
        raise_exception(env, EXCP_ILLEGAL_OPCODE);
    }

    if (diff > 0) {
// FIXME: zeroing registers is not needed, but useful for debugging
#if 0
        memset(&env->regs[env->wd.size], 0, diff * sizeof(env->regs[0]));
#endif
        if (env->enable_tags) {
            memset(&env->tags[env->wd.size], E2K_TAG_NON_NUMBER128, diff);
        }
    }

    env->wd.size = size;
    env->wd.fx = nfx == 0;
    env->wdbl = dbl;
}

#ifndef CONFIG_USER_ONLY
hwaddr e2k_get_phys_page_attrs_debug(CPUState *cs, vaddr addr, MemTxAttrs *attrs)
{
    E2KCPU *cpu = E2K_CPU(cs);
    CPUE2KState *env = &cpu->env;
    uint64_t pte;
    uint32_t page_offset;
    int page_size;

    if (!(env->mmu.cr & MMU_CR_TLB_EN)) {
        pte = addr & PTE_MASK;
        page_size = PTE_SIZE;
    } else {
        // TODO: page table walk
        abort();
    }

    pte &= ((1ULL << (env->version >= 6 ? 48 : 40)) - 1) & ~(page_size - 1);
    page_offset = (addr & TARGET_PAGE_MASK) & (page_size - 1);
    return pte | page_offset;
}

static int handle_mmu_fault(CPUState *cs, vaddr addr, int size,
    MMUAccessType access_type, int mmu_idx)
{
    E2KCPU *cpu = E2K_CPU(cs);
    CPUE2KState *env = &cpu->env;

    if (!(env->mmu.cr & MMU_CR_TLB_EN)) {
        int prot = PAGE_READ | PAGE_WRITE | PAGE_EXEC;

        tlb_set_page(cs, addr, addr, prot, mmu_idx, PTE_SIZE);
    } else {
        // TODO: page table walk
        abort();
    }

    return 0;
}
#endif

bool e2k_cpu_tlb_fill(CPUState *cs, vaddr address, int size,
     MMUAccessType access_type, int mmu_idx, bool probe, uintptr_t retaddr)
{
    E2KCPU *cpu = E2K_CPU(cs);
    CPUE2KState *env = &cpu->env;

#ifdef CONFIG_USER_ONLY
    raise_exception_ra(env, EXCP_DATA_PAGE, retaddr);
#else
    env->retaddr = retaddr;
    if (handle_mmu_fault(cs, address, size, access_type, mmu_idx)) {
        raise_exception_ra(env, EXCP_DATA_PAGE, retaddr);
    }
    return true;
#endif
}

void HELPER(break_restore_state)(CPUE2KState *env)
{
    e2k_proc_return(env, true);
    env->is_bp = false;
}

void HELPER(debug)(CPUE2KState *env)
{
    CPUState *cs = env_cpu(env);
    env->is_bp = true;
    e2k_proc_call(env, env->wd.size, env->ip, true);
    cs->exception_index = EXCP_DEBUG;
    cpu_loop_exit(cs);
}

#ifndef CONFIG_USER_ONLY
static AddressSpace *get_addressspace_by_addr(target_ulong *addr, CPUState *env, MemTxAttrs attrs)
{
    if (*addr >= E2K_IO_AREA_BASE && *addr < E2K_IO_AREA_BASE + E2K_IO_AREA_SIZE)
    {
        *addr -= E2K_IO_AREA_BASE;
        return &address_space_io;
    }
    
    return cpu_addressspace(env, attrs);
}

#define GEN_IO_LD_IMPL(T, f, op) \
    T HELPER(f)(CPUE2KState *env, target_ulong port) \
    { \
        MemTxAttrs attrs = MEMTXATTRS_UNSPECIFIED; \
        AddressSpace *as = get_addressspace_by_addr(&port, env_cpu(env), attrs); \
        T ret = address_space_ ## op (as, port, attrs, NULL); \
        trace_ioaddr_in(#f, port, ret); \
        return ret; \
    }

#define GET_IO_ST_IMPL(T, f, op) \
    void HELPER(f)(CPUE2KState *env, target_ulong port, T val) \
    { \
        MemTxAttrs attrs = MEMTXATTRS_UNSPECIFIED; \
        AddressSpace *as = get_addressspace_by_addr(&port, env_cpu(env), attrs); \
        address_space_ ## op (as, port, val, attrs, NULL); \
        trace_ioaddr_out(#f, port, val); \
    }

GEN_IO_LD_IMPL(uint64_t, inb, ldub)
GEN_IO_LD_IMPL(uint64_t, inh, lduw)
GEN_IO_LD_IMPL(uint64_t, inw, ldl)
GEN_IO_LD_IMPL(uint64_t, ind, ldq)
GET_IO_ST_IMPL(uint32_t, outb, stb)
GET_IO_ST_IMPL(uint32_t, outh, stw)
GET_IO_ST_IMPL(uint32_t, outw, stl)
GET_IO_ST_IMPL(uint64_t, outd, stq)

uint64_t HELPER(mmu_ind)(CPUE2KState *env, target_ulong reg)
{
    trace_mmu_ind(reg, 0);
    
    return 0;
}

void HELPER(mmu_outd)(CPUE2KState *env, target_ulong reg, uint64_t val)
{
    trace_mmu_outd(reg, val);
}

void cpu_report_tpr_access(CPUE2KState *env, TPRAccess access)
{
#if 0
    E2KCPU *cpu = env_archcpu(env);
    CPUState *cs = env_cpu(env);

    if (kvm_enabled() || whpx_enabled()) {
        env->tpr_access_type = access;

        cpu_interrupt(cs, CPU_INTERRUPT_TPR);
    } else if (tcg_enabled()) {
        cpu_restore_state(cs, cs->mem_io_pc, false);

        apic_handle_tpr_access_report(cpu->apic_state, env->eip, access);
    }
#else
    abort();
#endif
}
#endif
