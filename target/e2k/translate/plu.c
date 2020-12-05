#include "qemu/osdep.h"
#include "qemu.h"
#include "exec/log.h"
#include "translate.h"

static void gen_get_lp(TCGv_i32 ret, uint16_t clp, int offset, TCGv_i32 lp[7])
{
    int p = GET_FIELD(clp, offset, 3);
    int neg = GET_BIT(clp, offset + 3);

    tcg_gen_xori_i32(ret, lp[p], neg);
}

void e2k_gen_cond_i32(DisasContext *ctx, TCGv_i32 ret, uint8_t psrc)
{
    if (!GET_BIT(psrc, 6)) {
        if (GET_FIELD(psrc, 0, 6) == 0) {
            e2k_gen_lcntex(ret);
        } else {
            // TODO: spred
            abort();
        }
    } else if (GET_FIELD(psrc, 5, 2) == 0x40) {
        int val = GET_FIELD(psrc, 0, 5);
        if (val == 0) {
            // TODO: bgrpred
            abort();
        } else if (val <= 15) {
            // TODO: rndpred
            abort();
        } else {
            e2k_tr_gen_exception(ctx, E2K_EXCP_ILLOPN);
        }
    } else {
        int reg = GET_FIELD(psrc, 0, 5);
        TCGv_i64 t0 = tcg_temp_new_i64();

        e2k_gen_preg(t0, reg);
        tcg_gen_extrl_i64_i32(ret, t0);

        tcg_temp_free_i64(t0);
    }
}

static inline void scan_needed(const UnpackedBundle *bundle, int need[7])
{
    bool once_more = true;
    unsigned int i;

    for (i = 0; i < 3; i++) {
        if (bundle->pls_present[i] && GET_BIT(bundle->pls[i], 5)) {
            need[4 + i] = 1;
        }
    }

    while (once_more) {
        once_more = false;

        for (i = 0; i < 3; i++) {
            int p0, p1;

            if (need[4 + i] != 1) {
                continue;
            }

            p0 = GET_FIELD(bundle->pls[i], 10, 3);
            p1 = GET_FIELD(bundle->pls[i], 6, 3);

            if (p0 < 7 && need[p0] == 0) {
                need[p0] = 1;

                if (p0 >= 4) {
                    once_more = true;
                }
            }

            if (p1 < 7 && need[p1] == 0) {
                need[p1] = 1;

                if (p1 >= 4) {
                    once_more = true;
                }
            }

            need[4 + i] = 2;
        }
    }
}

void e2k_plu_execute(DisasContext *ctx)
{
    const UnpackedBundle *bundle = &ctx->bundle;
    int need[7] = { 0 };
    unsigned int i;
    TCGv_i32 lp[7];

    scan_needed(bundle, need);

    for (i = 0; i < 7; i++) {
        if (need[i]) {
            lp[i] = tcg_temp_new_i32();
        }
    }

    for (i = 0; i < 3; i++) {
        ctx->pl_results[i].reg = -1;

        if (!bundle->pls_present[i]) {
            continue;
        }

        if (i < 2) {
            if (need[i * 2]) {
                int elp = GET_FIELD(bundle->pls[i], 24, 7);
                e2k_gen_cond_i32(ctx, lp[i * 2], elp);
            }

            if (need[i * 2 + 1]) {
                int elp = GET_FIELD(bundle->pls[i], 16, 7);
                e2k_gen_cond_i32(ctx, lp[i * 2 + 1], elp);
            }
        }

        if (need[4 + i]) {
            uint16_t clp = GET_FIELD(bundle->pls[i], 0, 16);
            int opc = GET_FIELD(clp, 14, 2);
            TCGv_i32 p0 = tcg_temp_new_i32();
            TCGv_i32 p1 = tcg_temp_new_i32();
            int vdst = GET_BIT(clp, 5);
            int pdst = GET_FIELD(clp, 0, 5);

            // TODO: check clp arg
            // {C/M}LP0 0, 1             => 4
            // {C/M}LP1 0, 1, 2, 3, 4    => 5
            // {C/M}LP2 0, 1, 2, 3, 4, 5 => 6
            // maximal cascading is 2

            gen_get_lp(p0, clp, 10, lp);
            gen_get_lp(p1, clp, 6, lp);

            if (vdst) {
                ctx->pl_results[i].reg = pdst;
                ctx->pl_results[i].value = e2k_get_temp_i32(ctx);
            }

            switch (opc) {
            case 0: /* andp */
                // FIXME: what is the difference between `andp` and `landp`?
            case 1: /* landp */
                tcg_gen_and_i32(lp[4 + i], p0, p1);
                if (vdst) {
                    tcg_gen_mov_i32(ctx->pl_results[i].value, lp[4 + i]);
                }
                break;
            case 3: { /* movep */
                // FIXME: clp cannot read result of movep???
                tcg_gen_and_i32(lp[4 + i], p0, p1);

                if (vdst) {
                    TCGv_i32 one = tcg_const_i32(1);
                    TCGv_i64 t0 = tcg_temp_new_i64();
                    TCGv_i32 t1 = tcg_temp_new_i32();

                    e2k_gen_preg(t0, pdst);
                    tcg_gen_extrl_i64_i32(t1, t0);
                    tcg_gen_movcond_i32(TCG_COND_EQ, ctx->pl_results[i].value,
                        p0, one, p1, t1);

                    tcg_temp_free_i32(t1);
                    tcg_temp_free_i64(t0);
                    tcg_temp_free_i32(one);
                }
                break;
            }
            default:
                abort();
                break;
            }
        }
    }

    for (i = 0; i < 7; i++) {
        if (need[i]) {
            tcg_temp_free_i32(lp[i]);
        }
    }
}

void e2k_plu_commit(DisasContext *ctx)
{
    unsigned int i;

    for (i = 0; i < 3; i++) {
        TCGv_i64 t0;

        if (ctx->pl_results[i].reg < 0) {
            continue;
        }

        t0 = tcg_temp_new_i64();

        tcg_gen_extu_i32_i64(t0, ctx->pl_results[i].value);
        e2k_gen_store_preg(ctx->pl_results[i].reg, t0);

        tcg_temp_free_i64(t0);
    }
}
