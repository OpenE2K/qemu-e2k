#include "qemu/osdep.h"
#include "qemu.h"
#include "exec/log.h"
#include "translate.h"

static inline void gen_lcnt_dec(TCGv_i64 ret, TCGv_i64 lsr)
{
    TCGv_i32 zero = tcg_const_i32(0);
    TCGv_i32 lcnt = tcg_temp_new_i32();
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i32 t1 = tcg_temp_new_i32();
    TCGv_i32 t2 = tcg_temp_new_i32();
    TCGv_i32 t3 = tcg_temp_new_i32();
    TCGv_i32 t4 = tcg_temp_new_i32();
    TCGv_i32 t5 = tcg_temp_new_i32();

    tcg_gen_extrl_i64_i32(lcnt, lsr);
    tcg_gen_subi_i32(t0, lcnt, 1);
    tcg_gen_movcond_i32(TCG_COND_EQ, t1, lcnt, zero, zero, t0);

    /* overflow bit */
    tcg_gen_extrh_i64_i32(t2, lsr);
    tcg_gen_setcondi_i32(TCG_COND_EQ, t3, lcnt, 1);
    tcg_gen_shli_i32(t4, t3, LSR_OVER_OFF - 32);
    tcg_gen_or_i32(t5, t2, t4);

    tcg_gen_concat_i32_i64(ret, t1, t5);

    tcg_temp_free_i32(t5);
    tcg_temp_free_i32(t4);
    tcg_temp_free_i32(t3);
    tcg_temp_free_i32(t2);
    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
    tcg_temp_free_i32(lcnt);
    tcg_temp_free_i32(zero);
}

static void gen_movcond_flag_i32(TCGv_i32 ret, int flag, TCGv_i32 cond,
    TCGv_i32 v1, TCGv_i32 v2)
{
    TCGv_i32 one = tcg_const_i32(1);
    TCGCond c;

    switch (flag) {
    case 0x00:
        c = TCG_COND_NEVER;
        break;
    case 0x01:
        c = TCG_COND_EQ;
        break;
    case 0x02:
        c = TCG_COND_NE;
        break;
    case 0x03:
        c = TCG_COND_ALWAYS;
        break;
    default:
        g_assert_not_reached();
        break;
    }

    tcg_gen_movcond_i32(c, ret, cond, one, v1, v2);

    tcg_temp_free_i32(one);
}

static inline void gen_movcond_flag_i64(TCGv_i64 ret, int flag, TCGv_i64 cond,
    TCGv_i64 v1, TCGv_i64 v2)
{
    TCGv_i64 one = tcg_const_i64(1);
    TCGCond c;

    switch (flag) {
    case 0x00:
        c = TCG_COND_NEVER;
        break;
    case 0x01:
        c = TCG_COND_EQ;
        break;
    case 0x02:
        c = TCG_COND_NE;
        break;
    case 0x03:
        c = TCG_COND_ALWAYS;
        break;
    default:
        g_assert_not_reached();
        break;
    }

    tcg_gen_movcond_i64(c, ret, cond, one, v1, v2);

    tcg_temp_free_i64(one);
}

static inline void gen_dec_wrap(TCGv_i32 ret, TCGv_i32 cur, int n,
    TCGv_i32 size)
{
    TCGv_i32 t0 = tcg_temp_new_i32();

    tcg_gen_addi_i32(t0, size, n);
    tcg_gen_sub_i32(t0, t0, cur);
    tcg_gen_remu_i32(t0, t0, size);
    tcg_gen_sub_i32(ret, size, t0);

    tcg_temp_free_i32(t0);
}

static inline void gen_cur_dec(DisasContext *ctx, TCGv_i32 ret, int cond,
    TCGv_i32 cur, int n, TCGv_i32 size)
{
    TCGLabel *l0 = gen_new_label();
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i32 t1 = tcg_temp_new_i32();

    tcg_gen_brcondi_i32(TCG_COND_EQ, size, 0, l0);
    tcg_gen_trunc_tl_i32(t0, e2k_cs.ct_cond);
    gen_dec_wrap(t1, cur, n, size);
    gen_movcond_flag_i32(ret, cond, t0, t1, cur);
    gen_set_label(l0);

    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
}

void e2k_stubs_commit(DisasContext *ctx)
{
    uint32_t ss = ctx->bundle.ss;
//    unsigned int vfdi = (ss & 0x04000000) >> 26;
//    unsigned int abg = (ss & 0x01800000) >> 23;
    int alc = GET_FIELD(ss, 16, 2);
    int abp = GET_FIELD(ss, 18, 2);
    int abn = GET_FIELD(ss, 21, 2);
    int abg = GET_FIELD(ss, 23, 2);

    if (alc) {
        TCGv_i64 t0 = tcg_temp_new_i64();

        gen_lcnt_dec(t0, e2k_cs.lsr);
        gen_movcond_flag_i64(e2k_cs.lsr, alc, e2k_cs.ct_cond, t0, e2k_cs.lsr);
        tcg_temp_free_i64(t0);
    }

    if (abp) {
        TCGv_i32 t0 = tcg_temp_local_new_i32();
        tcg_gen_addi_i32(t0, e2k_cs.psize, 1);
        gen_cur_dec(ctx, e2k_cs.pcur, abp, e2k_cs.pcur, 1, t0);
        tcg_temp_free_i32(t0);
    }

    if (abn) {
        gen_cur_dec(ctx, e2k_cs.bcur, abn, e2k_cs.bcur, 2, e2k_cs.bsize);
    }

    switch(abg) {
    case 0x00:
        break;
    case 0x01:
        // TODO: abgd
        abort();
        break;
    case 0x02:
        // TODO: abgi
        abort();
        break;
    default:
        e2k_tr_gen_exception(ctx, E2K_EXCP_ILLOPC);
        break;
    }
}

static inline void gen_is_last_iter(TCGv ret)
{
    TCGv t0 = tcg_temp_new();
    TCGv t1 = tcg_temp_new();
    TCGv t2 = tcg_temp_new();

    e2k_gen_lcnt(t0);
    tcg_gen_setcondi_tl(TCG_COND_LTU, t1, t0, 2);
    tcg_gen_extract_tl(t2, e2k_cs.lsr, LSR_VLC_OFF, 1);
    tcg_gen_and_tl(ret, t1, t2);

    tcg_temp_free(t2);
    tcg_temp_free(t1);
    tcg_temp_free(t0);
}

static inline void gen_is_loop_end(TCGv ret)
{
    TCGv t0 = tcg_temp_new();
    TCGv t1 = tcg_temp_new();
    TCGv t2 = tcg_temp_new();

    e2k_gen_ecnt(t0);
    tcg_gen_setcondi_tl(TCG_COND_EQ, t1, t0, 0);
    gen_is_last_iter(t2);
    tcg_gen_and_tl(ret, t1, t2);

    tcg_temp_free(t2);
    tcg_temp_free(t1);
    tcg_temp_free(t0);
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
        if (!bundle->ss_present || (bundle->ss & 0x00000c00)) {
            e2k_tr_gen_exception(dc, E2K_EXCP_ILLOPC);
        } else if ((bundle->ss & 0x1ff)
              && !(bundle->cs1_present
              /* CS1.opc == CALL */
              && (bundle->cs1 & 0xf0000000) >> 28 == 5
              /* CS1.param.ctopc == HCALL  */
              && (bundle->cs1 & 0x380) >> 7 == 2))
        {
            if (type == IBRANCH && dc->ct.type == CT_NONE) {
                /* C0F2 has `disp' field. In `C0F1' it's called `param'. Is this
                   the only difference between these two formats?  Funnily enough,
                   DONE is also C0F2 and thus has `disp', though it obviously
                   makes no sense for it.  */
                uint32_t disp = (cs0 & 0x0fffffff);
                /* Calculate a signed displacement in bytes. */
                int32_t sdisp = ((int32_t) (disp << 4)) >> 1;
                dc->ct.type = CT_IBRANCH;
                dc->ct.u.target = dc->pc + sdisp;
            } else {
                e2k_tr_gen_exception(dc, E2K_EXCP_ILLOPC);
            }
        }
    } else {
        /* Note that according to Table B.4.1 it's possible to obtain
    `      gettsd %ctpr{1,2} with an invalid value for CS0.param.type.  */
        if (type == GETTSD && param_type != 1) {
            e2k_tr_gen_exception(dc, E2K_EXCP_ILLOPC);
        }
        int ipd = GET_FIELD(bundle->ss, 30, 2);
        if (type == DISP || type == LDISP) {
            unsigned int disp = GET_FIELD(cs0, 0, 28);
            /* Calculate a signed displacement in bytes. */
            int sdisp = ((int) (disp << 4)) >> 1;
            uint64_t reg = (dc->pc + sdisp) |
                ((uint64_t) CTPR_TAG_DISP << CTPR_TAG_OFF) |
                ((uint64_t) ipd << CTPR_IPD_OFF);
            if (type == LDISP) {
                reg |= (uint64_t) CTPR_OPC_LDISP << CTPR_OPC_OFF;
            }
            tcg_gen_movi_tl(e2k_cs.ctprs[ctpr - 1], reg);
        } else if (type == SDISP) {
            unsigned int disp = GET_FIELD(cs0, 0, 28) << 11;
            target_ulong base = ((uint64_t) 0xe2 << 40) | disp;
            uint64_t reg = (dc->pc + base) |
                ((uint64_t) CTPR_TAG_SDISP << CTPR_TAG_OFF) |
                ((uint64_t) ipd << CTPR_IPD_OFF);
            tcg_gen_movi_tl(e2k_cs.ctprs[ctpr - 1], reg);
        }

        /* Note that RETURN is said to be COPF1. I can't understand what its
           `CS0.param' is needed for: all of the bits except the three
           lowermost ones are undefined, while the latter also known as "type"
           field should be filled in with zeroes.  */
        if (type == RETURN) {
            TCGv_i32 t0 = tcg_const_i32(ipd);

            if (ctpr != 3) {
                e2k_tr_gen_exception(dc, E2K_EXCP_ILLOPN);
            }

            gen_helper_prep_return(e2k_cs.ctprs[2], cpu_env, t0);
            tcg_temp_free_i32(t0);
        }

        /* GETTSD has as meaningless `CS0.param' as RETURN. The only
           difference is that its `CS0.param.type' should be equal to `1'. I
           wonder if I should check for that and output something like
           "invalid gettsd" if this turns out not to be the case . . .  */
        if (type == GETTSD) {
            // TODO: gettsd
            abort();
        }

        if (type == PUTTSD) {
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
            // TODO: puttsd
            abort();
        }

        if (type == PREF) {
//            unsigned int pdisp = (bundle->cs0 & 0x0ffffff0) >> 4;
//            unsigned int ipd = (bundle->cs0 & 0x00000008) >> 3;
//            unsigned int prefr = bundle->cs0 & 0x00000007;
            // TODO: pref
            abort();
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

    if (opc == SETEI) {
        bool sft = GET_BIT(cs1, 27);

        if (sft) {
            if (dc->version >= 2) {
                // TODO: setsft
                abort();
            } else {
                e2k_tr_gen_exception(dc, E2K_EXCP_ILLOPC);
            }
        } else {
//            uint8_t eir = GET_FIELD_LEN(cs1, 0, 8);
            // TODO: setei
            abort();
        }
    } else if (opc == WAIT) {
        // TODO: wait
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

    qemu_log_mask(LOG_UNIMP, "0x%lx: wait is not implemented!\n", dc->pc);
//      my_printf ("ma_c = %d, fl_c = %d, ld_c = %d, st_c = %d, all_e = %d, "
//                 "all_c = %d", ma_c, fl_c, ld_c, st_c, all_e, all_c);
    } else if (opc == CALL) {
        unsigned int ctop = (bundle->ss & 0x00000c00) >> 10;
        if (ctop) {
            dc->ct.type = CT_CALL;
            dc->ct.wbs = GET_FIELD(cs1, 0, 7);
        } else {
            unsigned int cs1_ctopc = (cs1 & 0x380) >> 7;
            /* CS1.param.ctpopc == HCALL. CS0 is required to encode HCALL.  */
            if (cs1_ctopc == 2 && bundle->cs0_present &&
                dc->ct.type == CT_NONE)
            {
                unsigned int cs0 = bundle->cs0;
                unsigned int cs0_opc = (cs0 & 0xf0000000) >> 28;
                if (cs0_opc == 0) {
//                    unsigned int hdisp = (cs0 & 0x1e) >> 1;
                    // TODO: hcall hdisp, wbs ? cond
                    abort();
                }
            } else {
                e2k_tr_gen_exception(dc, E2K_EXCP_ILLOPC);
            }
        }
    } else if (opc == MAS_OPC) {
        dc->mas[0] = extract32(cs1, 21, 7);
        dc->mas[2] = extract32(cs1, 14, 7);
        dc->mas[3] = extract32(cs1, 7, 7);
        dc->mas[5] = extract32(cs1, 0, 7);
    } else if (opc == FLUSHR) {
        if (cs1 & 0x00000001) {
            // TODO: flushr
            abort();
        }

        if (cs1 & 0x00000002) {
            // TODO: flushc
            abort();
        }
    } else if (opc == BG) {
//        unsigned int chkm4 = (cs1 & 0x00010000) >> 16;
//        unsigned int dmask = (cs1 & 0x0000ff00) >> 8;
//        unsigned int umsk = cs1 & 0x000000ff;

        // TODO: vfbg
        abort();
    }
}

static void gen_jmp(DisasContext *dc)
{
    unsigned int psrc = GET_FIELD(dc->bundle.ss, 0, 5);
    unsigned int cond_type = GET_FIELD(dc->bundle.ss, 5, 4);
    unsigned int ctpr = GET_FIELD(dc->bundle.ss, 10, 2);

    if (ctpr != 0) {
        dc->ct.type = CT_JUMP;
        dc->ct.u.ctpr = e2k_cs.ctprs[ctpr - 1];
    }

    if (cond_type == 1) {
        tcg_gen_movi_tl(e2k_cs.ct_cond, 1);
    } else if (cond_type > 1) {
        TCGv preg = tcg_temp_new();
        TCGv loop_end = tcg_temp_new();
        TCGv not_loop_end = tcg_temp_new();
        TCGv cond = e2k_cs.ct_cond;
        TCGv_i64 t0 = tcg_temp_new_i64();

        dc->ct.is_branch = true;
        e2k_gen_preg(t0, psrc);
        tcg_gen_trunc_i64_tl(preg, t0);
        gen_is_loop_end(loop_end);
        tcg_gen_setcondi_tl(TCG_COND_NE, not_loop_end, loop_end, 1);

        switch (cond_type) {
        case 0x2:
        case 0x6:
        case 0xf:
            tcg_gen_mov_tl(cond, preg);
            break;
        case 0x3:
        case 0x7:
        case 0xe:
            tcg_gen_setcondi_tl(TCG_COND_NE, cond, preg, 1);
            break;
        default:
            break;
        }

        switch (cond_type) {
        case 0x4:
            tcg_gen_mov_tl(cond, loop_end);
            break;
        case 0x5:
            tcg_gen_mov_tl(cond, not_loop_end);
            break;
        case 0x6:
        case 0xe:
            tcg_gen_or_tl(cond, cond, loop_end);
            break;
        case 0x7:
        case 0xf:
            tcg_gen_and_tl(cond, cond, not_loop_end);
            break;
        default:
            break;
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


        tcg_temp_free_i64(t0);
        tcg_temp_free(not_loop_end);
        tcg_temp_free(loop_end);
        tcg_temp_free(preg);
    }
}

void e2k_control_window_change(DisasContext *dc)
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
    uint32_t cs1 = bundle->cs1;
    int opc = extract32(cs1, 28, 4);

    if (!dc->bundle.cs1_present) {
        return;
    }

    if (opc == SETR0 || opc == SETR1 || opc == SETBR) {
        bool setbp = (cs1 >> 27) & 1;
        bool setbn = (cs1 >> 26) & 1;

        if (opc == SETR1) {
            if (!bundle->lts_present[0]) {
                e2k_tr_gen_exception(dc, E2K_EXCP_ILLOPC);
            } else {
                /* Find out if VFRPSZ is always encoded together with SETWD. This
                seems to be the case even if no SETWD has been explicitly
                specified.  */
//                unsigned int rpsz = (bundle->lts[0] & 0x0001f000) >> 12;
                qemu_log_mask(LOG_UNIMP, "0x%lx: vfrpsz is not implemented!",
                    dc->pc);
            }
        }

        if (opc == SETR0 || opc == SETR1) {
            if (!bundle->lts_present[0]) {
                e2k_tr_gen_exception(dc, E2K_EXCP_ILLOPC);
            } else {
                TCGv_i32 lts = tcg_const_i32(bundle->lts[0]);

                gen_helper_setwd(cpu_env, lts);
                tcg_temp_free_i32(lts);
            }
        }

        if (setbn) {
            int rbs = GET_FIELD(cs1, BR_RBS_OFF, BR_RBS_LEN);
            int rsz = GET_FIELD(cs1, BR_RSZ_OFF, BR_RSZ_LEN);
            int rcur = GET_FIELD(cs1, BR_RCUR_OFF, BR_RCUR_LEN);

            tcg_gen_movi_i32(e2k_cs.boff, rbs * 2);
            tcg_gen_movi_i32(e2k_cs.bsize, (rsz + 1) * 2);
            tcg_gen_movi_i32(e2k_cs.bcur, rcur * 2);
        }

        if (setbp) {
            int psz = GET_FIELD(cs1, BR_PSZ_OFF, BR_PSZ_LEN);

            tcg_gen_movi_i32(e2k_cs.psize, psz);
            tcg_gen_movi_i32(e2k_cs.pcur, 0);
        }
    }
}

void e2k_control_execute(DisasContext *ctx)
{
    ctx->ct.type = CT_NONE;

    if (ctx->bundle.ss_present) {
        gen_jmp(ctx);
    }

    if (ctx->bundle.cs0_present) {
        gen_cs0(ctx);
    }

    if (ctx->bundle.cs1_present) {
        gen_cs1(ctx);
    }
}
