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
} Mova;

static void gen_load_prefetch_program(DisasContext *ctx)
{
    gen_helper_aau_load_program(cpu_env);
}

static void gen_aau_result(DisasContext *ctx, Mova *instr, TCGv_i64 dst,
    TCGv_i32 tag)
{
    AauResult *res = &ctx->aau_results[instr->chan];
    res->is_set = true;
    res->index = e2k_get_temp_i32(ctx);
    res->value = dst;
    res->tag = tag;
    if (IS_REGULAR(instr->dst)) {
        res->dst = instr->dst;
    } else {
        res->dst = 0;
        e2k_gen_reg_index(ctx, res->index, instr->dst);
    }
}

static void gen_checked_ld(DisasContext *ctx, Mova *instr, TCGv ptr)
{
    TCGLabel *l0 = gen_new_label();
    TCGLabel *l1 = gen_new_label();
    TCGv_i32 tag = e2k_get_temp_i32(ctx);
    TCGv_i64 dst = e2k_get_temp_i64(ctx);
    MemOp memop = instr->be ? MO_BE : MO_LE;

    switch(instr->opc) {
    case 1: memop |= MO_8; break; /* movab */
    case 2: memop |= MO_16; break; /* movah */
    case 3: memop |= MO_32; break; /* movaw */
    case 4: memop |= MO_64; break; /* movad */
    default:
        g_assert_not_reached();
        break;
    }

    tcg_gen_brcondi_tl(TCG_COND_NE, ptr, 0, l0);

    /* if address is invalid */
    tcg_gen_movi_i32(tag, E2K_TAG_NON_NUMBER64);
    tcg_gen_movi_i64(dst, E2K_MOVA_RESULT_INVALID);
    tcg_gen_br(l1);

    /* if address is valid */
    gen_set_label(l0);
    tcg_gen_movi_i32(tag, E2K_TAG_NUMBER64);
    tcg_gen_qemu_ld_i64(dst, ptr, 0, memop);

    gen_set_label(l1);
    gen_aau_result(ctx, instr, dst, tag);
}

static inline void gen_mova_ptr(TCGv ret, Mova *instr)
{
    TCGv_i32 t0 = tcg_const_i32(instr->chan);
    TCGv_i32 t1 = tcg_const_i32(instr->area);
    TCGv_i32 t2 = tcg_const_i32(instr->ind);
    gen_helper_mova_ptr(ret, cpu_env, t0, t1, t2);
    tcg_temp_free_i32(t2);
    tcg_temp_free_i32(t1);
    tcg_temp_free_i32(t0);
}

static void gen_mova(DisasContext *ctx, Mova *instr)
{
    /* branch in gen_checked_ld */
    TCGv t5 = tcg_temp_local_new();

    ctx->aau_am[instr->chan] = instr->am ? instr->area : -1;
    // TODO: check ind has proper alignment
    // TODO: check ind is less than mrng
    gen_mova_ptr(t5, instr);

    switch(instr->opc) {
    case 1: /* movab */
    case 2: /* movah */
    case 3: /* movaw */
    case 4: /* movad */
        gen_checked_ld(ctx, instr, t5);
        break;
    case 5: /* movaq */
        e2k_todo_illop(ctx, "movaq");
        break;
    case 7: /* movaqp */
        e2k_todo_illop(ctx, "movaqp");
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
        Mova instr = { 0 };
        AauResult *res = &ctx->aau_results[i];

        instr.chan = i;
        instr.aas = bundle->aas[i + 2];
        instr.dst = extract16(bundle->aas[i / 2], ((i & 1) ^ 1) * 8, 8);

        if (!bundle->aas_present[i + 2] || instr.opc == 0) {
            ctx->aau_am[i] = -1;
            res->is_set = false;
            continue;
        }

        // TODO: invalid value if addr is unaligned
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

        if (res->is_set) {
            if (IS_REGULAR(res->dst)) {
                e2k_gen_reg_index_from_wregi(res->index, GET_REGULAR(res->dst));
            }
        }
    }

    for (i = 0; i < 4; i++) {
        AauResult *res = &ctx->aau_results[i];

        // TODO: aau.tags
        if (res->is_set) {
            e2k_gen_reg_tag_write_i64(res->tag, res->index);
            e2k_gen_reg_write_i64(res->value, res->index);
        }
    }

    for (i = 0; i < 4; i++) {
        int area = ctx->aau_am[i];
        if (area == -1 || ((i == 1 || i == 3) && ctx->aau_am[i - 1] == area)) {
            continue;
        }
        gen_aau_am(ctx, i, area);
    }
}
