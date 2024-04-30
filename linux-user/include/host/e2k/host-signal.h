/*
 * host-signal.h: signal info dependent on the host architecture
 *
 * Copyright (c) 2024 Denis Drakhnia <numas13@gmail.com>
 *
 * This work is licensed under the terms of the GNU LGPL, version 2.1 or later.
 * See the COPYING file in the top-level directory.
 */

#ifndef E2K_HOST_SIGNAL_H
#define E2K_HOST_SIGNAL_H

/* The third argument to a SA_SIGINFO handler is ucontext_t. */
typedef ucontext_t host_sigcontext;

static inline uintptr_t host_signal_pc(host_sigcontext *uc)
{
    return uc->uc_mcontext.cr0_hi;
}

static inline void host_signal_set_pc(host_sigcontext *uc, uintptr_t pc)
{
    uc->uc_mcontext.cr0_hi = pc & ~7;
}

static inline void *host_signal_mask(host_sigcontext *uc)
{
    return &uc->uc_sigmask;
}

#define E2K_HS_SS_BIT (1 << 12)

#define E2K_HS_HAS_CS(h, i) (((h) & (1 << (14 + (i)))) != 0)
#define E2K_HS_HAS_ALS(h, i) (((h) & (1 << (26 + (i)))) != 0)
#define E2K_HS_HAS_ALES(h, i) (((h) & (1 << (20 + (i)))) != 0)

static inline bool e2k_check_store(const uint32_t *syll)
{
    uint32_t hs, als, ales25 = 0x01c001c0;
    int half_offset, syll_count = 1;

    /* bundle header */
    hs = syll[0];

    /* offset to half-syllables */
    half_offset = (hs & 15) + 1;

    if (hs & E2K_HS_SS_BIT) {
        syll_count += 1;
    }

    for (int i = 0; i < 6; ++i) {
        if (E2K_HS_HAS_ALS(hs, i)) {
            syll_count += 1;
        }
    }

    for (int i = 0; i < 2; ++i) {
        if (E2K_HS_HAS_CS(hs, i)) {
            syll_count += 1;
        }
    }

    if (half_offset > syll_count) {
        ales25 = syll[half_offset - (E2K_HS_HAS_CS(hs, 1) ? 2 : 1)];
    }

    syll += hs & E2K_HS_SS_BIT ? 2 : 1; /* skip HS and SS */

    for (int i = 0; i < 6; ++i) {
        if (!E2K_HS_HAS_ALS(hs, i)) {
            continue;
        }

        /* store can be only in ALS2 or ALS5 */
        if (i != 2 && i != 5) {
            syll += 1;
        }

        als = *syll++;

        if (E2K_HS_HAS_ALES(hs, i)) {
            uint16_t ales = extract32(ales25, i == 2 ? 16 : 0, 16);

            switch (extract16(ales, 8, 8)) {
            case 0x01: /* EXT */
                switch (extract32(als, 24, 7)) {
                case 0x02: /* stcsq */
                case 0x03: /* stdsq */
                case 0x04: /* stesq */
                case 0x05: /* stfsq */
                case 0x06: /* stgsq */
                case 0x07: /* stssq */
                case 0x1b: /* strd */
                case 0x24: /* stgdb */
                case 0x25: /* stgdh */
                case 0x26: /* stgdw */
                case 0x27: /* stgdd */
                case 0x28: /* stapb */
                case 0x29: /* staph */
                case 0x2a: /* stapw */
                case 0x2b: /* stapd */
                case 0x2c: /* stodrb */
                case 0x2d: /* stodrh */
                case 0x2e: /* stodrw */
                case 0x2f: /* stodrd */
                case 0x30: /* stodwb */
                case 0x31: /* stodwh */
                case 0x32: /* stodww */
                case 0x33: /* stodwd */
                case 0x34: /* stodpb */
                case 0x35: /* stodph */
                case 0x36: /* stodpw */
                case 0x37: /* stodpd */
                case 0x39: /* stgdq */
                case 0x3a: /* stapq */
                case 0x3b: /* stodrq */
                case 0x3c: /* stodwq */
                case 0x3d: /* stodpq */
                case 0x3f: /* staaq */
                    return true;
                }
                break;
            case 0x02: /* EXT1 */
                switch (extract32(als, 24, 7)) {
                case 0x11: /* stmqp */
                case 0x12: /* stcsmqp */
                case 0x13: /* stdsmqp */
                case 0x14: /* stesmqp */
                case 0x15: /* stfsmqp */
                case 0x16: /* stgsmqp */
                case 0x17: /* stssmqp */
                case 0x19: /* stgdmqp */
                case 0x1a: /* stapmqp */
                case 0x20: /* staaqp */
                case 0x21: /* stqp */
                case 0x22: /* stcsqp */
                case 0x23: /* stdsqp */
                case 0x24: /* stesqp */
                case 0x25: /* stfsqp */
                case 0x26: /* stgsqp */
                case 0x27: /* stssqp */
                case 0x29: /* stgdqp */
                case 0x2a: /* stapqp */
                case 0x2b: /* strqp */
                    return true;
                }
                break;
            }
        } else {
            switch (extract32(als, 24, 7)) {
            case 0x24: /* stb */
            case 0x25: /* sth */
            case 0x26: /* stw */
            case 0x27: /* std */
            case 0x28: /* stcsb */
            case 0x29: /* stcsh */
            case 0x2a: /* stcsw */
            case 0x2b: /* stcsd */
            case 0x2c: /* stdsb */
            case 0x2d: /* stdsh */
            case 0x2e: /* stdsw */
            case 0x2f: /* stdsd */
            case 0x30: /* stesb */
            case 0x31: /* stesh */
            case 0x32: /* stesw */
            case 0x33: /* stesd */
            case 0x34: /* stfsb */
            case 0x35: /* stfsh */
            case 0x36: /* stfsw */
            case 0x37: /* stfsd */
            case 0x38: /* stgsb */
            case 0x39: /* stgsh */
            case 0x3a: /* stgsw */
            case 0x3b: /* stgsd */
            case 0x3c: /* stssb */
            case 0x3d: /* stssh */
            case 0x3e: /* stssw */
            case 0x3f: /* stssd */
                return true;
            }
        }
    }

    return false;
}

#define	e2k_exc_data_page_num       29

#define E2K_TIR_IP(lo) ((lo) & ((1ULL << 48) - 1))
#define E2K_TIR_HAS_EXCP(hi, i) (((hi) & (1ULL << (i))) != 0)
#define E2K_TIR_HAS_ALS(hi, i) (((hi) & (1ULL << (44 + (i)))) != 0)

static inline bool host_signal_write(siginfo_t *info, host_sigcontext *uc)
{
    for (int i = 0; i <= uc->uc_mcontext.nr_TIRs; ++i) {
        uint64_t tir_hi = uc->uc_mcontext.tir_hi[i];

        if (!E2K_TIR_HAS_EXCP(tir_hi, e2k_exc_data_page_num)) {
            continue;
        }

        if (E2K_TIR_HAS_ALS(tir_hi, 2) || E2K_TIR_HAS_ALS(tir_hi, 5)) {
            uint64_t pc = E2K_TIR_IP(uc->uc_mcontext.tir_lo[i]);
            if (e2k_check_store ((const uint32_t *) pc)) {
                return true;
            }
        }
    }

    return false;
}

#endif /* E2K_HOST_SIGNAL_H */
