#include "qemu/osdep.h"
#include "qemu.h"
#include "exec/log.h"
#include "translate.h"

typedef struct {
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

void e2k_aau_execute(DisasContext *ctx)
{
    const UnpackedBundle *bundle = &ctx->bundle;
    unsigned int i;

    for (i = 0; i < 4; i++) {
        Instr instr = { 0 };
        AauResult *res = &ctx->aau_results[i];

        instr.aas = bundle->aas[i + 2];
        instr.dst = extract16(bundle->aas[i / 2], ((i & 1) ^ 1) * 8, 8);
        if (!bundle->aas_present[i + 2] || instr.opc == 0) {
            res->type = AAU_RESULT_NONE;
            continue;
        }

        if (instr.be) {
            // TODO: aas.be
            qemu_log_mask(LOG_UNIMP, "0x%lx: aas.be is not implemented\n",
                ctx->pc);
            abort();
        }

        if (instr.am) {
            // TODO: aas.am
            qemu_log_mask(LOG_UNIMP, "0x%lx: aas.be is not implemented\n",
                ctx->pc);
            abort();
        }

        switch(instr.opc) {
        case 1:
            // TODO: movab
            qemu_log_mask(LOG_UNIMP, "0x%lx: movab is not implemented\n",
                ctx->pc);
            abort();
            break;
        case 2:
            // TODO: movah
            qemu_log_mask(LOG_UNIMP, "0x%lx: movah is not implemented\n",
                ctx->pc);
            abort();
            break;
        case 3:
            // TODO: movaw
            qemu_log_mask(LOG_UNIMP, "0x%lx: movaw is not implemented\n",
                ctx->pc);
            abort();
            break;
        case 4:
            // TODO: movad
            qemu_log_mask(LOG_UNIMP, "0x%lx: movad is not implemented\n",
                ctx->pc);
            abort();
            break;
        case 5:
            // TODO: movaq
            qemu_log_mask(LOG_UNIMP, "0x%lx: movaq is not implemented\n",
                ctx->pc);
            abort();
            break;
        case 7:
            // TODO: movaqp
            qemu_log_mask(LOG_UNIMP, "0x%lx: movaqp is not implemented\n",
                ctx->pc);
            abort();
            break;
        default:
            e2k_tr_gen_exception(ctx, E2K_EXCP_ILLOPC);
            return;
        }
    }
}

void e2k_aau_commit(DisasContext *ctx)
{
    unsigned int i;

    for (i = 0; i < 4; i++) {
        AauResult *res = &ctx->aau_results[i];
        TCGv_i32 zero = tcg_const_i32(0);

        // TODO: aau.tags
        switch(res->type) {
        case AAU_RESULT_REG32:
            e2k_gen_reg_tag_write_i32(zero, res->index);
            e2k_gen_reg_write_i32(res->v32, res->index);
            break;
        case AAU_RESULT_REG64:
            e2k_gen_reg_tag_write_i64(zero, res->index);
            e2k_gen_reg_write_i64(res->v64, res->index);
            break;
        default:
            break;
        }

        tcg_temp_free_i32(zero);
    }
}
