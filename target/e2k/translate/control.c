#include "qemu/osdep.h"
#include "qemu.h"
#include "exec/log.h"
#include "translate.h"

static inline void gen_alc_dec(DisasContext *dc, TCGCond cond)
{
    TCGv_i64 one = tcg_const_i64(1);
    TCGv_i32 zero = tcg_const_i32(0);
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i32 t1 = tcg_temp_new_i32();
    TCGv_i32 t2 = tcg_temp_new_i32();
    TCGv_i32 t3 = tcg_temp_new_i32();
    TCGv_i32 t4 = tcg_temp_new_i32();
    TCGv_i32 t5 = tcg_temp_new_i32();
    TCGv_i32 t6 = tcg_temp_new_i32();
    TCGv_i64 t7 = tcg_temp_new_i64();

    tcg_gen_extrl_i64_i32(t0, e2k_cs.lsr);
    tcg_gen_subi_i32(t1, t0, 1);
    tcg_gen_movcond_i32(TCG_COND_LTU, t2, t1, t0, t1, t0);
    tcg_gen_setcondi_i32(TCG_COND_EQ, t3, t1, 0);
    tcg_gen_shli_i32(t4, t3, LSR_OVER_OFF - 32);
    tcg_gen_extrh_i64_i32(t5, e2k_cs.lsr);
    tcg_gen_or_i32(t6, t5, t4);
    tcg_gen_concat_i32_i64(t7, t2, t6);
    tcg_gen_movcond_i64(cond, e2k_cs.lsr,
        dc->jmp.cond, one,
        t7, e2k_cs.lsr);

    tcg_temp_free_i64(t7);
    tcg_temp_free_i32(t6);
    tcg_temp_free_i32(t5);
    tcg_temp_free_i32(t4);
    tcg_temp_free_i32(t3);
    tcg_temp_free_i32(t2);
    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
    tcg_temp_free_i32(zero);
    tcg_temp_free_i64(one);
}

static inline void gen_abp_inc(DisasContext *dc, TCGCond cond)
{
    TCGv_i64 t0 = tcg_temp_new_i64();
    TCGv_i64 t1 = tcg_temp_new_i64();
    TCGv_i64 one = tcg_const_i64(1);

    tcg_gen_subi_i64(t0, e2k_cs.pcur, 1);
    tcg_gen_umin_i64(t1, t0, e2k_cs.psz);
    tcg_gen_movcond_i64(cond, e2k_cs.pcur, dc->jmp.cond, one, t1, e2k_cs.pcur);

    tcg_temp_free_i64(one);
    tcg_temp_free_i64(t1);
    tcg_temp_free_i64(t0);
}

static inline void gen_abn_inc(DisasContext *dc, TCGCond cond)
{
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i32 t1 = tcg_temp_new_i32();
    TCGv_i32 t2 = tcg_temp_new_i32();
    TCGv_i32 t3 = tcg_temp_new_i32();
    TCGv_i32 one = tcg_const_i32(1);

    tcg_gen_subi_i32(t0, e2k_cs.rcur, 1);
    tcg_gen_umin_i32(t2, t0, e2k_cs.rsz);
    tcg_gen_extrl_i64_i32(t3, dc->jmp.cond);
    tcg_gen_movcond_i32(cond, e2k_cs.rcur,
        t3, one,
        t2, e2k_cs.rcur);

    tcg_temp_free_i32(one);
    tcg_temp_free_i32(t3);
    tcg_temp_free_i32(t2);
    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
}

void e2k_win_commit(DisasContext *dc)
{
    // Change windowing registers after commit is done.
    uint32_t ss = dc->bundle.ss;
//    unsigned int vfdi = (ss & 0x04000000) >> 26;
//    unsigned int abg = (ss & 0x01800000) >> 23;

    if (GET_BIT(ss, 16)) {
        gen_alc_dec(dc, TCG_COND_EQ);
    }
    if (GET_BIT(ss, 17)) {
        gen_alc_dec(dc, TCG_COND_NE);
    }
    if (GET_BIT(ss, 18)) {
        gen_abp_inc(dc, TCG_COND_EQ);
    }
    if (GET_BIT(ss, 19)) {
        gen_abp_inc(dc, TCG_COND_NE);
    }
    if (GET_BIT(ss, 21)) {
        gen_abn_inc(dc, TCG_COND_EQ);
    }
    if (GET_BIT(ss, 22)) {
        gen_abn_inc(dc, TCG_COND_NE);
    }
}

static inline void gen_is_last_iter(TCGv_i64 ret)
{
    TCGv_i64 t0 = tcg_temp_new_i64();
    TCGv_i64 t1 = tcg_temp_new_i64();
    TCGv_i64 t2 = tcg_temp_new_i64();

    e2k_gen_lcnt(t0);
    tcg_gen_setcondi_i64(TCG_COND_LTU, t1, t0, 2);
    tcg_gen_extract_i64(t2, e2k_cs.lsr, LSR_VLC_OFF, 1);
    tcg_gen_and_i64(ret, t1, t2);

    tcg_temp_free(t2);
    tcg_temp_free(t1);
    tcg_temp_free(t0);
}

static inline void gen_is_loop_end(TCGv_i64 ret)
{
    TCGv_i64 t0 = tcg_temp_new_i64();
    TCGv_i64 t1 = tcg_temp_new_i64();
    TCGv_i64 t2 = tcg_temp_new_i64();

    e2k_gen_ecnt(t0);
    tcg_gen_setcondi_i64(TCG_COND_EQ, t1, t0, 0);
    gen_is_last_iter(t2);
    tcg_gen_and_i64(ret, t1, t2);

    tcg_temp_free_i64(t2);
    tcg_temp_free_i64(t1);
    tcg_temp_free_i64(t0);
}

static void gen_cs0(DisasContext *dc)
{
    typedef enum {
        NOTHING,
        IBRANCH,
        PREF,
        PUTTSD,
        DONE,
        HRET,
        GLAUNCH,
        DISP,
        SDISP,
        GETTSD,
        LDISP,
        RETURN
    } cs0_type;

    static cs0_type cs0_ops[4][4] =  {
        {IBRANCH, PREF, PUTTSD, DONE},
        {DISP, NOTHING, SDISP, GETTSD},
        {DISP, LDISP, SDISP, GETTSD},
        {DISP, NOTHING, SDISP, RETURN}
    };
    const UnpackedBundle *bundle = &dc->bundle;
    uint32_t cs0 = bundle->cs0;

    unsigned int ctpr = (cs0 & 0xc0000000) >> 30;
    unsigned int ctp_opc = (cs0 & 0x30000000) >> 28;
    unsigned int param_type = (cs0 & 0x00000007);
    cs0_type type = cs0_ops[ctpr][ctp_opc];

    if (type == RETURN && param_type == 1) {
        type = GETTSD;
    } else if (type == DONE) {
        if (param_type == 3) {
            type = HRET;
        } else if (param_type == 4) {
            type = GLAUNCH;
        }
    }

    if (type == IBRANCH || type == DONE || type == HRET || type == GLAUNCH) {
        /* IBRANCH, DONE, HRET and GLAUNCH are special because they require SS
           to be properly encoded.  */
        if (!bundle->ss_present || (bundle->ss & 0x00000c00))
        {
            // TODO: exeption invalid bundle
            abort();
        /* Don't output either of the aforementioned instructions under "never"
           condition. Don't disassemble CS0 being a part of HCALL. Unlike ldis
           HCALL is currently disassembled on behalf of CS1.  */
        } else if ((bundle->ss & 0x1ff)
              && !(bundle->cs1_present
              /* CS1.opc == CALL */
              && (bundle->cs1 & 0xf0000000) >> 28 == 5
              /* CS1.param.ctopc == HCALL  */
              && (bundle->cs1 & 0x380) >> 7 == 2))
        {
            if (type == IBRANCH) {
                /* C0F2 has `disp' field. In `C0F1' it's called `param'. Is this
                   the only difference between these two formats?  Funnily enough,
                   DONE is also C0F2 and thus has `disp', though it obviously
                   makes no sense for it.  */
                unsigned int disp = (cs0 & 0x0fffffff);
                /* Calculate a signed displacement in bytes. */
                int sdisp = ((int) (disp << 4)) >> 1;
                target_ulong tgt = dc->pc + sdisp;
                tcg_gen_movi_tl(dc->jmp.dest, tgt);
            }
        }
    } else {
        /* Note that according to Table B.4.1 it's possible to obtain
    `      gettsd %ctpr{1,2} with an invalid value for CS0.param.type.  */
        if (type == GETTSD && param_type != 1) {
            // invalid
            abort();
        }
        int ipd = GET_FIELD(bundle->ss, 30, 31);
        if (type == DISP || type == LDISP) {
            unsigned int disp = GET_FIELD(cs0, 0, 27);
            /* Calculate a signed displacement in bytes. */
            int sdisp = ((int) (disp << 4)) >> 1;
            uint64_t reg = (dc->pc + sdisp) |
                ((uint64_t) CTPR_TAG_DISP << CTPR_TAG_OFF) |
                ((uint64_t) ipd << CTPR_IPD_OFF);
            if (type == LDISP) {
                reg |= (uint64_t) CTPR_OPC_LDISP << CTPR_OPC_OFF;
            }
            tcg_gen_movi_tl(e2k_cs.ctprs[ctpr], reg);
        } else if (type == SDISP) {
            unsigned int disp = GET_FIELD(cs0, 0, 27) << 11;
            /* FIXME: trap address */
            target_ulong base = ((uint64_t) 0xe2 << 40) | disp;
            uint64_t reg = (dc->pc + base) |
                ((uint64_t) CTPR_TAG_SDISP << CTPR_TAG_OFF) |
                ((uint64_t) ipd << CTPR_IPD_OFF);
            tcg_gen_movi_tl(e2k_cs.ctprs[ctpr], reg);
        }

        if (/* Note that RETURN is said to be COPF1. I can't understand what its
              `CS0.param' is needed for: all of the bits except the three
               lowermost ones are undefined, while the latter also known as "type"
               field should be filled in with zeroes.  */
            type == RETURN
            /* GETTSD has as meaningless `CS0.param' as RETURN. The only
               difference is that its `CS0.param.type' should be equal to `1'. I
               wonder if I should check for that and output something like
               "invalid gettsd" if this turns out not to be the case . . .  */
            || type == GETTSD)
        {
            // TODO
        }

        if (type == SDISP) {
//            my_printf (", 0x%x", cs0 & 0x1f);
        } else if (type == DISP
                    || type == LDISP
                    || type == PUTTSD)
        {
//            unsigned int disp = (cs0 & 0x0fffffff);
//            int sgnd_disp = ((int) (disp << 4)) >> 1;
            /* PUTTSD obviously doesn't take %ctpr{j} parameter. TODO: beware of
               an optional predicate which may control its execution which is
               encoded via `SS.ctcond.psrc' and `SS.ts_opc == PUTTSDC{P,N}' in
               case of `SS.type == 1' (see C.21.4). I wonder if `ct %ctpr<j>'
               encoded in `SS.ctop' under the same `SS.ctcond' takes an effect in
               such a case.  */
//            my_printf ("%s0x%llx", type == PUTTSD ? "" : ", ",
            /* FIXME: this way I ensure that it'll work correctly
               both on 32 and 64-bit hosts.  */
//                (unsigned long long) (instr_addr + sgnd_disp));
        }

        if (type == PREF) {
//            unsigned int pdisp = (bundle->cs0 & 0x0ffffff0) >> 4;
//            unsigned int ipd = (bundle->cs0 & 0x00000008) >> 3;
//            unsigned int prefr = bundle->cs0 & 0x00000007;
        }
    }
}

static void gen_cs1(DisasContext *dc)
{
    enum {
        SETR0,
        SETR1,
        SETEI,
        WAIT,
        SETBR,
        CALL,
        MAS_OPC,
        FLUSHR,
        BG
    };

    const UnpackedBundle *bundle = &dc->bundle;
    unsigned int cs1 = bundle->cs1;
    unsigned int opc = (cs1 & 0xf0000000) >> 28;

    if (opc == SETR0 || opc == SETR1 || opc == SETBR) {
        unsigned int setbp = (cs1 & 0x08000000) >> 27;
        unsigned int setbn = (cs1 & 0x04000000) >> 26;

        /* Try to follow the same order of these instructions as in LDIS.
        Presumably `vfrpsz' should come first, while `setbp' should be placed
        between `setwd' and `setbn', but this is to be verified. I don't have
        a binary with these commands by hand right now.  */

        if (opc == SETR1) {
            if (! bundle->lts_present[0]) {
//                my_printf ("<bogus vfrpsz>");
            } else {
                /* Find out if VFRPSZ is always encoded together with SETWD. This
                seems to be the case even if no SETWD has been explicitly
                specified.  */
//                unsigned int rpsz = (bundle->lts[0] & 0x0001f000) >> 12;
//                my_printf ("vfrpsz rpsz = 0x%x", rpsz);
            }
        }

        // FIXME: Should windowing registers be precomputed or not?

        if (opc == SETR0 || opc == SETR1) {
            if (! bundle->lts_present[0]) {
                // TODO: <bogus setwd>
                abort();
            } else {
                uint32_t lts0 = bundle->lts[0];
                tcg_gen_movi_i32(e2k_cs.wsz, GET_FIELD(lts0, 5, 11));
                tcg_gen_movi_i32(e2k_cs.nfx, GET_BIT(lts0, 4));

                if (dc->version >= 3) {
                    tcg_gen_movi_i32(e2k_cs.dbl, GET_BIT(lts0, 3));
                }
            }
        }

        if (setbn) {
            unsigned int rcur = (cs1 & 0x0003f000) >> 12;
            unsigned int rsz = (cs1 & 0x00000fc0) >> 6;
            unsigned int rbs = cs1 & 0x0000003f;

            tcg_gen_movi_i32(e2k_cs.rcur, rcur);
            tcg_gen_movi_i32(e2k_cs.rsz, rsz);
            tcg_gen_movi_i32(e2k_cs.rbs, rbs);
        }

        if (setbp) {
            tcg_gen_movi_i64(e2k_cs.psz, GET_FIELD(cs1, 18, 23));
        }
    } else if (opc == SETEI) {
        /* Verify that CS1.param.sft = CS1.param[27] is equal to zero as required
        in C.14.3.  */
        unsigned int sft = (cs1 & 0x08000000) >> 27;
//        unsigned int eir = (cs1 & 0x000000ff);

        if (sft) {
//            my_printf ("%s", mcpu >= 2 ? "setsft" : "unimp");
        } else {
//            my_printf ("setei 0x%x", eir);
        }
    } else if (opc == WAIT) {
//        unsigned int ma_c = (cs1 & 0x00000020) >> 5;
//        unsigned int fl_c = (cs1 & 0x00000010) >> 4;
        unsigned int ld_c = (cs1 & 0x00000008) >> 3;
        unsigned int st_c = (cs1 & 0x00000004) >> 2;
//        unsigned int all_e = (cs1 & 0x00000002) >> 1;
//        unsigned int all_c = cs1 & 0x00000001;

        if (dc->version >= 5) {
            /* `sa{l,s}' fields are `elbrus-v5'-specific. Each of them makes sense
            only in the presence of `{ld,st}_c == 1' respectively.  */
            if (ld_c) {
//                unsigned int sal = (cs1 & 0x00000100) >> 8;
//                my_printf ("sal = %d, ", sal);
            }

            if (st_c) {
//                unsigned int sas = (cs1 & 0x00000080) >> 7;
//                my_printf ("sas = %d, ", sas);
            }
        }

        if (dc->version >= 2) {
          /* `trap' field was introduced starting from `elbrus-v2'.  */
//            unsigned int trap = (cs1 & 0x00000040) >> 6;
//          my_printf ("trap = %d, ", trap);
        }

//      my_printf ("ma_c = %d, fl_c = %d, ld_c = %d, st_c = %d, all_e = %d, "
//                 "all_c = %d", ma_c, fl_c, ld_c, st_c, all_e, all_c);
    } else if (opc == CALL) {
        unsigned int ctop = (bundle->ss & 0x00000c00) >> 10;
        /* In C.17.4 it's said that other bits in CS1.param except for the
           seven lowermost ones are ignored.  */
        unsigned int wbs = cs1 & 0x7f;

        if (ctop) {
            dc->is_call = true;
            tcg_gen_movi_i32(e2k_cs.syscall_wbs, wbs);
//            my_printf ("call %%ctpr%d, wbs = 0x%x", ctop, wbs);
//            print_ctcond (info, instr->ss & 0x1ff);
        } else {
            unsigned int cs1_ctopc = (cs1 & 0x380) >> 7;
            /* CS1.param.ctpopc == HCALL. CS0 is required to encode HCALL.  */
            if (cs1_ctopc == 2 && bundle->cs0_present) {
                unsigned int cs0 = bundle->cs0;
                unsigned int cs0_opc = (cs0 & 0xf0000000) >> 28;
                /* CS0.opc == HCALL, which means
                CS0.opc.ctpr == CS0.opc.ctp_opc == 0 */
                if (cs0_opc == 0) {
//                    unsigned int hdisp = (cs0 & 0x1e) >> 1;
//                    my_printf ("hcall 0x%x, wbs = 0x%x", hdisp, wbs);
//                    print_ctcond (info, instr->ss & 0x1ff);
                }
            } else {
//                my_printf ("<bogus call>");
            }
        }
    } else if (opc == MAS_OPC) {
        /* Note that LDIS doesn't print it out as a standalone instruction.  */
//        unsigned int mas = cs1 & 0x0fffffff;

//        my_printf ("mas 0x%x", mas);
    } else if (opc == FLUSHR) {
        /* . . . these stupid engineers off! FLUSHR seems to be responsible for
           encoding both `flushr' and `flushc'. Moreover, according to their
           logic it should be possible to encode them simultaneously.  */

        /* Check for `CS1.param.flr'.  */
        if (cs1 & 0x00000001) {
//            my_printf ("flushr");
        }

        /* Check for `CS1.param.flc'.  */
        if (cs1 & 0x00000002) {
//          my_printf ("flushc");
        }
    } else if (opc == BG) {
        /* Hopefully, `vfbg' is the only instruction encoded by BG. I'm currently
           unable to find other ones in `iset-v5.single' at least . . .  */
//        unsigned int chkm4 = (cs1 & 0x00010000) >> 16;
//        unsigned int dmask = (cs1 & 0x0000ff00) >> 8;
//        unsigned int umsk = cs1 & 0x000000ff;

        /* Print its fields in the order proposed in C.14.10.  */
//        my_printf ("vfbg umask = 0x%x, dmask = 0x%x, chkm4 = 0x%x",
//                   umsk, dmask, chkm4);
    } else {
//        my_printf ("unimp");
        abort();
    }
}

static void gen_jmp(DisasContext *dc)
{
    unsigned int psrc = GET_FIELD(dc->bundle.ss, 0, 4);
    unsigned int cond_type = GET_FIELD(dc->bundle.ss, 5, 8);
    unsigned int ctpr = GET_FIELD(dc->bundle.ss, 10, 11);

    if (cond_type == 1) {
        dc->base.is_jmp = STATIC_JUMP;
        tcg_gen_movi_i64(dc->jmp.cond, 1);
    } else {
        /* TODO: single assign */
        TCGv_i64 cond = tcg_temp_new_i64();

        dc->base.is_jmp = DYNAMIC_JUMP;

        if (cond_type == 2 || cond_type == 6 || cond_type == 0xf) {
            /* if pred is true */
            tcg_gen_mov_i64(cond, e2k_get_preg(dc, psrc));
        } else if (cond_type == 3 || cond_type == 7 || cond_type == 0xe) {
            /* if pred is false */
            TCGv_i64 one = tcg_const_i64(1);
            TCGv_i64 zero = tcg_const_i64(0);
            tcg_gen_movcond_i64(TCG_COND_EQ, cond,
                e2k_get_preg(dc, psrc), zero,
                one, zero);
            tcg_temp_free_i64(zero);
            tcg_temp_free_i64(one);
        }

        /* TODO: other kinds of conditions */

        if (cond_type == 4 || cond_type == 6 || cond_type == 0xe) {
            TCGv_i64 is_loop_end = tcg_temp_new_i64();

            gen_is_loop_end(is_loop_end);

            if (cond_type == 6 || cond_type == 0xe) {
                tcg_gen_or_i64(cond, cond, is_loop_end);
            } else {
                tcg_gen_mov_i64(cond, is_loop_end);
            }

            tcg_temp_free_i64(is_loop_end);
        }

        if (cond_type == 5 || cond_type == 7 || cond_type == 0xf) {
            TCGv_i64 is_loop_end = tcg_temp_new_i64();
            TCGv_i64 t0 = tcg_temp_new_i64();

            gen_is_loop_end(is_loop_end);
            tcg_gen_setcondi_i64(TCG_COND_EQ, t0, is_loop_end, 0);

            if(cond_type == 7 || cond_type == 0xf) {
                tcg_gen_and_i64(cond, cond, t0);
            } else {
                tcg_gen_mov_i64(cond, t0);
            }

            tcg_temp_free_i64(t0);
            tcg_temp_free_i64(is_loop_end);
        }

        if (cond_type == 8) {
            // %MLOCK
            /* It's not clearly said in C.17.1.2 of iset-vX.single if the uppermost
               fourth bit in `psrc' has any meaning at all.  */
            if (psrc & 0xf) {
    //            static const int conv[] = {0, 1, 3, 4};
                int i;

                // %dt_al
                for (i = 0; i < 4; i++) {
                    if (psrc & (1 << i)) {
                        // i
                    }
                }
            }
        }

        /* `lock_cond || pl_cond' control transfer conditions.  */
        if (cond_type == 9) {
            unsigned int type = (psrc & 0x18) >> 3;
            if (type == 0) {
    //            static const int cmp_num_to_alc[] = {0, 1, 3, 4};
    //            unsigned int cmp_num = (psrc & 0x6) >> 1;
    //            unsigned int neg = psrc & 0x1;

    //            my_printf ("%%MLOCK || %s%%cmp%d", neg ? "~" : "",
    //                cmp_num_to_alc[cmp_num]);
            } else if (type == 1) {
    //            unsigned int cmp_jk = (psrc & 0x4) >> 2;
    //            unsigned int negj = (psrc & 0x2) >> 1;
    //            unsigned int negk = psrc & 0x1;

    //            my_printf ("%%MLOCK || %s%%cmp%d || %s%%cmp%d",
    //                     negj ? "~" : "", cmp_jk == 0 ? 0 : 3,
    //                     negk ? "~" : "", cmp_jk == 0 ? 1 : 4);
            } else if (type == 2) {
    //            unsigned int clp_num = (psrc & 0x6) >> 1;
    //            unsigned int neg = psrc & 0x1;

                // "%%MLOCK || %s%%clp%d", neg ? "~" : "", clp_num
            }
        }

        if (cond_type >= 0xa && cond_type <= 0xd) {
            // reserved condition type
            qemu_log_mask(LOG_UNIMP, "Undefined control transfer type %#x\n", cond_type);
            abort();
        }

        tcg_gen_mov_i64(dc->jmp.cond, cond);

        tcg_temp_free_i64(cond);
    }

    /* TODO: check CPU behavior if present ibranch and ctpr is not zero */

    /* TODO: different kinds of ct */
    if (ctpr != 0) {
        if (dc->is_call) {
            /* TODO: call save state */
            dc->call_ctpr = ctpr;
            dc->base.is_jmp = DISAS_CALL;
            return;
        }
        TCGv_i64 t0 = tcg_temp_new_i64();

        tcg_gen_andi_i64(t0, e2k_cs.ctprs[ctpr], GEN_MASK(0, 47));
        tcg_gen_mov_tl(dc->jmp.dest, t0);

        tcg_temp_free_i64(t0);
    }
}

void e2k_control_gen(DisasContext *dc)
{
    if (dc->bundle.cs0_present) {
        gen_cs0(dc);
    }

    if (dc->bundle.cs1_present) {
        gen_cs1(dc);
    }

    if (dc->bundle.ss_present) {
        gen_jmp(dc);
    }
}
