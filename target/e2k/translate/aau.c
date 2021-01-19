#include "qemu/osdep.h"
#include "qemu.h"
#include "exec/log.h"
#include "translate.h"

typedef struct {
    int chan;
    union {
        struct {
            uint16_t am: 1;
            uint16_t ind: 5;
            uint16_t area: 6;
            uint16_t opc: 3;
            uint16_t be: 1;
        };
        uint16_t aas;
    };
    uint8_t dst;
} Instr;

static void gen_load_prefetch_program(DisasContext *ctx)
{
    gen_helper_aau_load_program(cpu_env);
}

static void gen_aau_result_reg64(DisasContext *ctx, Instr *instr, TCGv_i64 dst)
{
    AauResult *res = &ctx->aau_results[instr->chan];
    res->type = AAU_RESULT_REG64;
    res->dst = instr->dst;
    res->index = e2k_get_temp_i32(ctx);
    res->v64 = dst;
    if (IS_REGULAR(instr->dst)) {
        res->dst = instr->dst;
    } else {
        res->dst = 0;
        e2k_gen_reg_index(ctx, res->index, instr->dst);
    }
}

static void gen_aau_result_reg32(DisasContext *ctx, Instr *instr, TCGv_i32 dst)
{
    AauResult *res = &ctx->aau_results[instr->chan];
    res->type = AAU_RESULT_REG32;
    res->dst = instr->dst;
    res->index = e2k_get_temp_i32(ctx);
    res->v32 = dst;
    if (IS_REGULAR(instr->dst)) {
        res->dst = instr->dst;
    } else {
        res->dst = 0;
        e2k_gen_reg_index(ctx, res->index, instr->dst);
    }
}

static void gen_mova_i32(DisasContext *ctx, Instr *instr, TCGv ptr)
{
    MemOp memop = instr->be ? MO_BE : MO_LE;
    TCGv_i32 dst = e2k_get_temp_i32(ctx);

    switch(instr->opc) {
    case 1: memop |= MO_8; break; /* movab */
    case 2: memop |= MO_16; break; /* movah */
    case 3: memop |= MO_32; break; /* movaw */
    default:
        g_assert_not_reached();
        break;
    }

    tcg_gen_qemu_ld_i32(dst, ptr, 0, memop);
    gen_aau_result_reg32(ctx, instr, dst);
}

static void gen_mova_i64(DisasContext *ctx, Instr *instr, TCGv ptr)
{
    TCGv_i64 dst = e2k_get_temp_i64(ctx);

    tcg_gen_qemu_ld_i64(dst, ptr, 0, instr->be ? MO_BEQ : MO_LEQ);
    gen_aau_result_reg64(ctx, instr, dst);
}

static inline void gen_mova_ptr(TCGv ret, Instr *instr)
{
    TCGv_i32 t0 = tcg_const_i32(instr->chan);
    TCGv_i32 t1 = tcg_const_i32(instr->area);
    TCGv t2 = tcg_temp_new();

    gen_helper_mova_ptr(t2, cpu_env, t0, t1);
    tcg_gen_addi_tl(ret, t2, instr->ind);

    tcg_temp_free(t2);
    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
}
static void gen_mova(DisasContext *ctx, Instr *instr)
{
    TCGv t5 = tcg_temp_new();

    ctx->aau_am[instr->chan] = instr->am ? instr->area : -1;
    // TODO: check ind has proper alignment
    // TODO: check ind is less than mrng
    gen_mova_ptr(t5, instr);

    switch(instr->opc) {
    case 1: /* movab */
    case 2: /* movah */
    case 3: /* movaw */
        gen_mova_i32(ctx, instr, t5);
        break;
    case 4: /* movad */
        gen_mova_i64(ctx, instr, t5);
        break;
    case 5: /* movaq */
        qemu_log_mask(LOG_UNIMP, "0x%lx: movaq is not implemented\n", ctx->pc);
        abort();
        break;
    case 7: /* movaqp */
        qemu_log_mask(LOG_UNIMP, "0x%lx: movaqp is not implemented\n", ctx->pc);
        abort();
        break;
    default:
        e2k_tr_gen_exception(ctx, E2K_EXCP_ILLOPC);
        break;
    }

    tcg_temp_free(t5);
}

static inline void gen_aau_am(DisasContext *ctx, int chan, int area)
{
    TCGv_i32 t0 = tcg_const_i32(chan);
    TCGv_i32 t1 = tcg_const_i32(area);

    gen_helper_aau_am(cpu_env, t0, t1);
    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
}

void e2k_aau_execute(DisasContext *ctx)
{
    const UnpackedBundle *bundle = &ctx->bundle;
    unsigned int i;

    for (i = 0; i < 4; i++) {
        Instr instr = { 0 };
        AauResult *res = &ctx->aau_results[i];

        instr.chan = i;
        instr.aas = bundle->aas[i + 2];
        instr.dst = extract16(bundle->aas[i / 2], ((i & 1) ^ 1) * 8, 8);

        if (!bundle->aas_present[i + 2] || instr.opc == 0) {
            ctx->aau_am[i] = -1;
            res->type = AAU_RESULT_NONE;
            continue;
        }

        gen_mova(ctx, &instr);
    }

    /* bap */
    if (ctx->bundle.ss & (1 << 28)) {
        gen_load_prefetch_program(ctx);
    }
}

void e2k_aau_commit(DisasContext *ctx)
{
    unsigned int i;

    for (i = 0; i < 4; i++) {
        AauResult *res = &ctx->aau_results[i];

        if (res->type == AAU_RESULT_REG32 || res->type == AAU_RESULT_REG64) {
            if (IS_REGULAR(res->dst)) {
                e2k_gen_reg_index_from_wregi(res->index, GET_REGULAR(res->dst));
            }
        }
    }

    for (i = 0; i < 4; i++) {
        AauResult *res = &ctx->aau_results[i];
        TCGv_i32 zero = tcg_const_i32(0);

        // TODO: aau.tags
        switch(res->type) {
        case AAU_RESULT_REG32: {
            TCGv_i64 t0 = tcg_temp_new_i64();

            /* mova{b,h,w} always write to reg64 */
            tcg_gen_extu_i32_i64(t0, res->v32);
            e2k_gen_reg_tag_write_i32(zero, res->index);
            e2k_gen_reg_write_i64(t0, res->index);
            tcg_temp_free_i64(t0);
            break;
        }
        case AAU_RESULT_REG64:
            e2k_gen_reg_tag_write_i64(zero, res->index);
            e2k_gen_reg_write_i64(res->v64, res->index);
            break;
        default:
            break;
        }

        tcg_temp_free_i32(zero);
    }

    for (i = 0; i < 4; i++) {
        int area = ctx->aau_am[i];
        if (area == -1 || ((i == 1 || i == 3) && ctx->aau_am[i - 1] == area)) {
            continue;
        }
        gen_aau_am(ctx, i, area);
    }
}
