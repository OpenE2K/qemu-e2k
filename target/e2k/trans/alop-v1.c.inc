/*
 * E2K ISA version 1 translation
 *
 * SPDX-License-Identifier: GPL-2.0-only
 */

#define compat_format_alf1          ALOPF1
#define compat_format_alf1_mas      ALOPF1_MAS
#define compat_format_alf1_lit8     ALOPF11_LIT8
#define compat_format_alf2          ALOPF2
#define compat_format_alf2_pshufh   ALOPF12_PSHUFH
#define compat_format_alf2_icalld   ALOPF12_ICALLD
#define compat_format_alf3_mas      ALOPF3
#define compat_format_alf7          ALOPF7
#define compat_format_alf8          ALOPF8
#define compat_format_alf10_mas     ALOPF10
#define compat_format_alf15         ALOPF15
#define compat_format_alf16         ALOPF16
#define compat_format_alf21         ALOPF21
#define compat_format_alf21_log     ALOPF21
#define compat_format(F) glue(compat_format_, F)

#define compat_args_alf1(R, S1, S2, S3)         glue(ARGS_, glue3(S1, S2, R))
#define compat_args_alf1_mas(R, S1, S2, S3)     glue(ARGS_, glue3(S1, S2, R))
#define compat_args_alf1_lit8(R, S1, S2, S3)    glue(ARGS_, glue3(S1, S2, R))
#define compat_args_alf2(R, S1, S2, S3)         glue(ARGS_, glue(S2, R))
#define compat_args_alf2_pshufh(R, S1, S2, S3)  glue(ARGS_, glue(S2, R))
#define compat_args_alf2_icalld(R, S1, S2, S3)  glue(ARGS_, glue(S2, R))
#define compat_args_alf3_mas(R, S1, S2, S3)     glue(ARGS_, glue3(S1, S2, R))
#define compat_args_alf7(R, S1, S2, S3)         glue(ARGS_, glue3(S1, S2, R))
#define compat_args_alf8(R, S1, S2, S3)         glue(ARGS_, glue(S2, R))
#define compat_args_alf10_mas(R, S1, S2, S3)    glue(ARGS_, R)
#define compat_args_alf15(R_, S1, S2, S3)       glue(ARGS_, glue(S2, R))
#define compat_args_alf16(R, S1, S2, S3)        glue(ARGS_, R)
#define compat_args_alf21(R, S1, S2, S3)        glue(ARGS_, glue4(S1, S2, S3, R))
#define compat_args_alf21_log(R, S1, S2, S3)    glue(ARGS_, glue4(S1, S2, S3, R))
#define compat_args(F, R, S1, S2, S3) glue(compat_args_, F)(R, S1, S2, S3)

#define lower_B b
#define lower_S s
#define lower_D d
#define lower_X x
#define lower_P q
#define lower_Q q
#define lower(S) glue(lower_, S)

#define compat_args2_alf1(R, S1, S2, S3)         glue3(R, S1, S2)
#define compat_args2_alf1_mas(R, S1, S2, S3)
#define compat_args2_alf1_lit8(R, S1, S2, S3)    glue3(R, S1, S2)
#define compat_args2_alf2(R, S1, S2, S3)         glue(R, S2)
#define compat_args2_alf2_pshufh(R, S1, S2, S3)  glue(R, S2)
#define compat_args2_alf3_mas(R, S1, S2, S3)
#define compat_args2_alf7(R, S1, S2, S3)         glue3(R, S1, S2)
#define compat_args2_alf8(R, S1, S2, S3)         glue(R, S2)
#define compat_args2_alf21(R, S1, S2, S3)        glue4(R, S1, S2, S3)
#define compat_args2_alf21_log(R, S1, S2, S3)    glue4(R, S1, S2, S3)
#define compat_args2(F, R, S1, S2, S3) glue(compat_args2_, F)(lower(R), lower(S1), lower(S2), lower(S3))

#define V_MIN(X) ((X) & 0xff)
#define V_MAX(X) ((X) >> 8)
#define R(MIN, MAX) (((MAX) << 8) | (MIN))
#define V1_2 R(1, 2)
#define V1_3 R(1, 3)

static bool check_version(Alop *alop, uint16_t versions[6])
{
    uint16_t min_ver = V_MIN(versions[alop->chan]); \
    uint16_t max_ver = V_MAX(versions[alop->chan]); \
    return alop->ctx->version >= min_ver && (max_ver == 0 || alop->ctx->version <= max_ver);
}

#define TRANS_BASE(NAME, C0, C1, C2, C3, C4, C5, R, S1, S2, S3, F, CODE) \
    static bool glue(trans_, NAME)(DisasContext *ctx, glue(arg_, F) *a) \
    { \
        uint16_t versions[6] = { C0, C1, C2, C3, C4, C5 }; \
        Alop *alop = &ctx->alops[ctx->cur_alop]; \
        if (!check_version(alop, versions)) { \
            return false; \
        } else if (ctx->only_check) { \
            alop->format = compat_format(F); \
            alop->args = compat_args(F, R, S1, S2, S3); \
        } else { \
            CODE; \
        } \
        return true; \
    }

#define TRANSW(NAME, C0, C1, C2, C3, C4, C5, R, S1, S2, S3, F, E, GEN) \
    TRANS_BASE(NAME, C0, C1, C2, C3, C4, C5, R, S1, S2, S3, F, \
        { glue4(gen_, F, E, compat_args2(F, R, S1, S2, S3))(alop, GEN); })

#define TRANSC(NAME, C0, C1, C2, C3, C4, C5, R, S1, S2, S3, F, WRAP, OP1, OP2) \
    TRANS_BASE(NAME, C0, C1, C2, C3, C4, C5, R, S1, S2, S3, F, \
        { WRAP(alop, OP1, OP2); })

#define TRANSM(NAME, C0, C1, C2, C3, C4, C5, R, S1, S2, S3, F, ADDR, GEN, MEMOP, BASE) \
    TRANS_BASE(NAME, C0, C1, C2, C3, C4, C5, R, S1, S2, S3, F, \
        { glue(gen_, F)(alop, ADDR, GEN, MEMOP, BASE); })

#define TRANS_(NAME, C0, C1, C2, C3, C4, C5, R, S1, S2, S3, F, WRAP, GEN) \
    TRANS_BASE(NAME, C0, C1, C2, C3, C4, C5, R, S1, S2, S3, F, { WRAP(alop, GEN); })

#define TRANSD(NAME, C0, C1, C2, C3, C4, C5, R, S1, S2, S3, F, GEN) \
    TRANS_BASE(NAME, C0, C1, C2, C3, C4, C5, R, S1, S2, S3, F, { GEN(alop); })

// special version:
//   * set old opcode (to reuse old code)
//   * direct call
#define TRANSA(NAME, C0, C1, C2, C3, C4, C5, R, S1, S2, S3, F, OP, GEN) \
    static bool glue(trans_, NAME)(DisasContext *ctx, glue(arg_, F) *a) \
    { \
        uint16_t versions[6] = { C0, C1, C2, C3, C4, C5 }; \
        Alop *alop = &ctx->alops[ctx->cur_alop]; \
        if (!check_version(alop, versions)) { \
            return false; \
        } else if (ctx->only_check) { \
            alop->op = OP; \
            alop->format = compat_format(F); \
            alop->args = compat_args(F, R, S1, S2, S3); \
        } else { \
            GEN(alop); \
        } \
        return true; \
    }

TRANSW(andw,            1, 1, 1, 1, 1, 1, S,S,S,0, alf1, _, tcg_gen_and_i32);
TRANSW(andd,            1, 1, 1, 1, 1, 1, D,D,D,0, alf1, _, tcg_gen_and_i64);
TRANSW(andnw,           1, 1, 1, 1, 1, 1, S,S,S,0, alf1, _, gen_andn_i32);
TRANSW(andnd,           1, 1, 1, 1, 1, 1, D,D,D,0, alf1, _, gen_andn_i64);
TRANSW(orw,             1, 1, 1, 1, 1, 1, S,S,S,0, alf1, _, tcg_gen_or_i32);
TRANSW(ord,             1, 1, 1, 1, 1, 1, D,D,D,0, alf1, _, tcg_gen_or_i64);
TRANSW(ornw,            1, 1, 1, 1, 1, 1, S,S,S,0, alf1, _, gen_orn_i32);
TRANSW(ornd,            1, 1, 1, 1, 1, 1, D,D,D,0, alf1, _, gen_orn_i64);
TRANSW(xorw,            1, 1, 1, 1, 1, 1, S,S,S,0, alf1, _, tcg_gen_xor_i32);
TRANSW(xord,            1, 1, 1, 1, 1, 1, D,D,D,0, alf1, _, tcg_gen_xor_i64);
TRANSW(xornw,           1, 1, 1, 1, 1, 1, S,S,S,0, alf1, _, gen_xorn_i32);
TRANSW(xornd,           1, 1, 1, 1, 1, 1, D,D,D,0, alf1, _, gen_xorn_i64);
TRANSW(sxtb,            1, 1, 1, 1, 1, 1, D,0,S,0, alf2, _, gen_sxtb);
TRANSW(sxth,            1, 1, 1, 1, 1, 1, D,0,S,0, alf2, _, gen_sxth);
TRANSW(sxtw,            1, 1, 1, 1, 1, 1, D,0,S,0, alf2, _, gen_sxtw);
TRANSW(zxtb,            1, 1, 1, 1, 1, 1, D,0,S,0, alf2, _, gen_zxtb);
TRANSW(zxth,            1, 1, 1, 1, 1, 1, D,0,S,0, alf2, _, gen_zxth);
TRANSW(zxtw,            1, 1, 1, 1, 1, 1, D,0,S,0, alf2, _, gen_zxtw);
TRANSW(sxt,             1, 1, 1, 1, 1, 1, D,S,S,0, alf1, _, gen_helper_sxt);
TRANSD(mergew,          1, 1, 1, 1, 1, 1, S,S,S,0, alf1,    gen_merges);
TRANSD(merged,          1, 1, 1, 1, 1, 1, D,D,D,0, alf1,    gen_merged);
TRANSW(addw,            1, 1, 1, 1, 1, 1, S,S,S,0, alf1, _, tcg_gen_add_i32);
TRANSW(addd,            1, 1, 1, 1, 1, 1, D,D,D,0, alf1, _, tcg_gen_add_i64);
TRANSW(negw,            1, 1, 1, 1, 1, 1, S,S,S,0, alf2, _, tcg_gen_neg_i32);
TRANSW(subw,            1, 1, 1, 1, 1, 1, S,S,S,0, alf1, _, tcg_gen_sub_i32);
TRANSW(negd,            1, 1, 1, 1, 1, 1, D,D,D,0, alf2, _, tcg_gen_neg_i64);
TRANSW(subd,            1, 1, 1, 1, 1, 1, D,D,D,0, alf1, _, tcg_gen_sub_i64);
TRANSW(sclw,            1, 1, 1, 1, 1, 1, S,S,S,0, alf1, _, tcg_gen_rotl_i32);
TRANSW(scld,            1, 1, 1, 1, 1, 1, D,D,D,0, alf1, _, tcg_gen_rotl_i64);
TRANSW(scrw,            1, 1, 1, 1, 1, 1, S,S,S,0, alf1, _, tcg_gen_rotr_i32);
TRANSW(scrd,            1, 1, 1, 1, 1, 1, D,D,D,0, alf1, _, tcg_gen_rotr_i64);
TRANSW(shlw,            1, 1, 1, 1, 1, 1, S,S,S,0, alf1, _, tcg_gen_shl_i32);
TRANSW(shld,            1, 1, 1, 1, 1, 1, D,D,D,0, alf1, _, tcg_gen_shl_i64);
TRANSW(shrw,            1, 1, 1, 1, 1, 1, S,S,S,0, alf1, _, tcg_gen_shr_i32);
TRANSW(shrd,            1, 1, 1, 1, 1, 1, D,D,D,0, alf1, _, tcg_gen_shr_i64);
TRANSW(sarw,            1, 1, 1, 1, 1, 1, S,S,S,0, alf1, _, tcg_gen_sar_i32);
TRANSW(sard,            1, 1, 1, 1, 1, 1, D,D,D,0, alf1, _, tcg_gen_sar_i64);
TRANSD(getfw,           1, 1, 1, 1, 1, 1, S,S,S,0, alf1,    gen_getfs);
TRANSD(getfd,           1, 1, 1, 1, 1, 1, D,D,D,0, alf1,    gen_getfd);
TRANSW(cmpowb,          1, 1, 0, 1, 1, 0, S,S,S,0, alf7, _, gen_cmposb);
TRANSW(cmpbwb,          1, 1, 0, 1, 1, 0, S,S,S,0, alf7, _, gen_cmpbsb);
TRANSW(cmpewb,          1, 1, 0, 1, 1, 0, S,S,S,0, alf7, _, gen_cmpesb);
TRANSW(cmpbewb,         1, 1, 0, 1, 1, 0, S,S,S,0, alf7, _, gen_cmpbesb);
TRANSW(cmpswb,          1, 1, 0, 1, 1, 0, S,S,S,0, alf7, _, gen_cmpssb);
TRANSW(cmppwb,          1, 1, 0, 1, 1, 0, S,S,S,0, alf7, _, gen_cmppsb);
TRANSW(cmplwb,          1, 1, 0, 1, 1, 0, S,S,S,0, alf7, _, gen_cmplsb);
TRANSW(cmplewb,         1, 1, 0, 1, 1, 0, S,S,S,0, alf7, _, gen_cmplesb);
TRANSW(cmpodb,          1, 1, 0, 1, 1, 0, D,D,D,0, alf7, _, gen_cmpodb);
TRANSW(cmpbdb,          1, 1, 0, 1, 1, 0, D,D,D,0, alf7, _, gen_cmpbdb);
TRANSW(cmpedb,          1, 1, 0, 1, 1, 0, D,D,D,0, alf7, _, gen_cmpedb);
TRANSW(cmpbedb,         1, 1, 0, 1, 1, 0, D,D,D,0, alf7, _, gen_cmpbedb);
TRANSW(cmpsdb,          1, 1, 0, 1, 1, 0, D,D,D,0, alf7, _, gen_cmpsdb);
TRANSW(cmppdb,          1, 1, 0, 1, 1, 0, D,D,D,0, alf7, _, gen_cmppdb);
TRANSW(cmpldb,          1, 1, 0, 1, 1, 0, D,D,D,0, alf7, _, gen_cmpldb);
TRANSW(cmpledb,         1, 1, 0, 1, 1, 0, D,D,D,0, alf7, _, gen_cmpledb);
TRANSW(cmpandewb,       1, 1, 0, 1, 1, 0, S,S,S,0, alf7, _, gen_cmpandesb);
TRANSW(cmpandswb,       1, 1, 0, 1, 1, 0, S,S,S,0, alf7, _, gen_cmpandssb);
TRANSW(cmpandpwb,       1, 1, 0, 1, 1, 0, S,S,S,0, alf7, _, gen_cmpandpsb);
TRANSW(cmpandlewb,      1, 1, 0, 1, 1, 0, S,S,S,0, alf7, _, gen_cmpandlesb);
TRANSW(cmpandedb,       1, 1, 0, 1, 1, 0, D,D,D,0, alf7, _, gen_cmpandedb);
TRANSW(cmpandsdb,       1, 1, 0, 1, 1, 0, D,D,D,0, alf7, _, gen_cmpandsdb);
TRANSW(cmpandpdb,       1, 1, 0, 1, 1, 0, D,D,D,0, alf7, _, gen_cmpandpdb);
TRANSW(cmpandledb,      1, 1, 0, 1, 1, 0, D,D,D,0, alf7, _, gen_cmpandledb);
TRANSD(cctopo,          1, 0, 0, 1, 0, 0, B,0,S,0, alf8,    gen_cctopo);
TRANSD(cctopb,          1, 0, 0, 1, 0, 0, B,0,S,0, alf8,    gen_cctopb);
TRANSD(cctope,          1, 0, 0, 1, 0, 0, B,0,S,0, alf8,    gen_cctope);
TRANSD(cctopbe,         1, 0, 0, 1, 0, 0, B,0,S,0, alf8,    gen_cctopbe);
TRANSD(cctops,          1, 0, 0, 1, 0, 0, B,0,S,0, alf8,    gen_cctops);
TRANSD(cctopp,          1, 0, 0, 1, 0, 0, B,0,S,0, alf8,    gen_cctopp);
TRANSD(cctopl,          1, 0, 0, 1, 0, 0, B,0,S,0, alf8,    gen_cctopl);
TRANSD(cctople,         1, 0, 0, 1, 0, 0, B,0,S,0, alf8,    gen_cctople);
TRANSM(ldb,             1, 0, 1, 1, 0, 1, D,D,D,0, alf1_mas, gen_addr_i64, gen_ld_raw_i64, MO_UB, ADDR_FLAT); 
TRANSM(ldh,             1, 0, 1, 1, 0, 1, D,D,D,0, alf1_mas, gen_addr_i64, gen_ld_raw_i64, MO_UW, ADDR_FLAT); 
TRANSM(ldw,             1, 0, 1, 1, 0, 1, D,D,D,0, alf1_mas, gen_addr_i64, gen_ld_raw_i64, MO_UL, ADDR_FLAT); 
TRANSM(ldd,             1, 0, 1, 1, 0, 1, D,D,D,0, alf1_mas, gen_addr_i64, gen_ld_raw_i64, MO_UQ, ADDR_FLAT); 
// should be illop if not e2k32
#if 1 || defined(TARGET_E2K32)
TRANSM(ldcsb,           1, 0, 1, 1, 0, 1, D,S,S,0, alf1_mas, gen_addr_i32, gen_ld_raw_i64, MO_UB, ADDR_CS);
TRANSM(ldcsh,           1, 0, 1, 1, 0, 1, D,S,S,0, alf1_mas, gen_addr_i32, gen_ld_raw_i64, MO_UW, ADDR_CS);
TRANSM(ldcsw,           1, 0, 1, 1, 0, 1, D,S,S,0, alf1_mas, gen_addr_i32, gen_ld_raw_i64, MO_UL, ADDR_CS);
TRANSM(ldcsd,           1, 0, 1, 1, 0, 1, D,S,S,0, alf1_mas, gen_addr_i32, gen_ld_raw_i64, MO_UQ, ADDR_CS);
TRANSM(lddsb,           1, 0, 1, 1, 0, 1, D,S,S,0, alf1_mas, gen_addr_i32, gen_ld_raw_i64, MO_UB, ADDR_DS);
TRANSM(lddsh,           1, 0, 1, 1, 0, 1, D,S,S,0, alf1_mas, gen_addr_i32, gen_ld_raw_i64, MO_UW, ADDR_DS);
TRANSM(lddsw,           1, 0, 1, 1, 0, 1, D,S,S,0, alf1_mas, gen_addr_i32, gen_ld_raw_i64, MO_UL, ADDR_DS);
TRANSM(lddsd,           1, 0, 1, 1, 0, 1, D,S,S,0, alf1_mas, gen_addr_i32, gen_ld_raw_i64, MO_UQ, ADDR_DS);
TRANSM(ldesb,           1, 0, 1, 1, 0, 1, D,S,S,0, alf1_mas, gen_addr_i32, gen_ld_raw_i64, MO_UB, ADDR_ES);
TRANSM(ldesh,           1, 0, 1, 1, 0, 1, D,S,S,0, alf1_mas, gen_addr_i32, gen_ld_raw_i64, MO_UW, ADDR_ES);
TRANSM(ldesw,           1, 0, 1, 1, 0, 1, D,S,S,0, alf1_mas, gen_addr_i32, gen_ld_raw_i64, MO_UL, ADDR_ES);
TRANSM(ldesd,           1, 0, 1, 1, 0, 1, D,S,S,0, alf1_mas, gen_addr_i32, gen_ld_raw_i64, MO_UQ, ADDR_ES);
TRANSM(ldfsb,           1, 0, 1, 1, 0, 1, D,S,S,0, alf1_mas, gen_addr_i32, gen_ld_raw_i64, MO_UB, ADDR_FS);
TRANSM(ldfsh,           1, 0, 1, 1, 0, 1, D,S,S,0, alf1_mas, gen_addr_i32, gen_ld_raw_i64, MO_UW, ADDR_FS);
TRANSM(ldfsw,           1, 0, 1, 1, 0, 1, D,S,S,0, alf1_mas, gen_addr_i32, gen_ld_raw_i64, MO_UL, ADDR_FS);
TRANSM(ldfsd,           1, 0, 1, 1, 0, 1, D,S,S,0, alf1_mas, gen_addr_i32, gen_ld_raw_i64, MO_UQ, ADDR_FS);
//ldfsq,               1, 0, 1, 1, 0, 1, Q,D,D,0, alf11_mas, AF_PAIR
TRANSM(ldgsb,           1, 0, 1, 1, 0, 1, D,S,S,0, alf1_mas, gen_addr_i32, gen_ld_raw_i64, MO_UB, ADDR_GS);
TRANSM(ldgsh,           1, 0, 1, 1, 0, 1, D,S,S,0, alf1_mas, gen_addr_i32, gen_ld_raw_i64, MO_UW, ADDR_GS);
TRANSM(ldgsw,           1, 0, 1, 1, 0, 1, D,S,S,0, alf1_mas, gen_addr_i32, gen_ld_raw_i64, MO_UL, ADDR_GS);
TRANSM(ldgsd,           1, 0, 1, 1, 0, 1, D,S,S,0, alf1_mas, gen_addr_i32, gen_ld_raw_i64, MO_UQ, ADDR_GS);
//ldgsq,               1, 0, 1, 1, 0, 1, Q,D,D,0, alf11_mas, AF_PAIR
TRANSM(ldssb,           1, 0, 1, 1, 0, 1, D,S,S,0, alf1_mas, gen_addr_i32, gen_ld_raw_i64, MO_UB, ADDR_SS);
TRANSM(ldssh,           1, 0, 1, 1, 0, 1, D,S,S,0, alf1_mas, gen_addr_i32, gen_ld_raw_i64, MO_UW, ADDR_SS);
TRANSM(ldssw,           1, 0, 1, 1, 0, 1, D,S,S,0, alf1_mas, gen_addr_i32, gen_ld_raw_i64, MO_UL, ADDR_SS);
TRANSM(ldssd,           1, 0, 1, 1, 0, 1, D,S,S,0, alf1_mas, gen_addr_i32, gen_ld_raw_i64, MO_UQ, ADDR_SS);
//ldssq,               1, 0, 1, 1, 0, 1, Q,D,D,0, alf11_mas, AF_PAIR
TRANSM(ldgdb,           1, 0, 1, 1, 0, 1, D,S,S,0, alf1_mas, gen_addr_i32, gen_ld_raw_i64, MO_UB, ADDR_GD);
TRANSM(ldgdh,           1, 0, 1, 1, 0, 1, D,S,S,0, alf1_mas, gen_addr_i32, gen_ld_raw_i64, MO_UW, ADDR_GD);
TRANSM(ldgdw,           1, 0, 1, 1, 0, 1, D,S,S,0, alf1_mas, gen_addr_i32, gen_ld_raw_i64, MO_UL, ADDR_GD);
TRANSM(ldgdd,           1, 0, 1, 1, 0, 1, D,S,S,0, alf1_mas, gen_addr_i32, gen_ld_raw_i64, MO_UQ, ADDR_GD);
//ldgdq,               1, 0, 1, 1, 0, 1, Q,S,S,0, alf11_mas, AF_PAIR
#endif
//ldcudb,              1, 0, 1, 1, 0, 1, D,S,S,0, alf11_mas
//ldcudh,              1, 0, 1, 1, 0, 1, D,S,S,0, alf11_mas
//ldcudw,              1, 0, 1, 1, 0, 1, D,S,S,0, alf11_mas
//ldcudd,              1, 0, 1, 1, 0, 1, D,S,S,0, alf11_mas
//ldcudq,              1, 0, 1, 1, 0, 1, Q,S,S,0, alf11_mas, AF_PAIR
//ldapb,               1, 0, 1, 1, 0, 1, D,Q,S,0, alf11_mas
//ldaph,               1, 0, 1, 1, 0, 1, D,Q,S,0, alf11_mas
//ldapw,               1, 0, 1, 1, 0, 1, D,Q,S,0, alf11_mas
//ldapq,               1, 0, 1, 1, 0, 1, Q,Q,S,0, alf11_mas, AF_PAIR
//ldodrb,              1, 0, 1, 1, 0, 1, D,Q,S,0, alf11_mas, AF_REMOVED_IN_V3
//ldodrh,              1, 0, 1, 1, 0, 1, D,Q,S,0, alf11_mas, AF_REMOVED_IN_V3
//ldodrw,              1, 0, 1, 1, 0, 1, D,Q,S,0, alf11_mas, AF_REMOVED_IN_V3
//ldodrd,              1, 0, 1, 1, 0, 1, D,Q,S,0, alf11_mas, AF_REMOVED_IN_V3
//ldodrq,              1, 0, 1, 1, 0, 1, Q,Q,S,0, alf11_mas, AF_PAIR | AF_REMOVED_IN_V3
//ldodwb,              1, 0, 1, 1, 0, 1, D,Q,S,0, alf11_mas, AF_REMOVED_IN_V3
//ldodwh,              1, 0, 1, 1, 0, 1, D,Q,S,0, alf11_mas, AF_REMOVED_IN_V3
//ldodww,              1, 0, 1, 1, 0, 1, D,Q,S,0, alf11_mas, AF_REMOVED_IN_V3
//ldodwd,              1, 0, 1, 1, 0, 1, D,Q,S,0, alf11_mas, AF_REMOVED_IN_V3
//ldodwq,              1, 0, 1, 1, 0, 1, Q,Q,S,0, alf11_mas, AF_PAIR | AF_REMOVED_IN_V3
//ldodpb,              1, 0, 1, 1, 0, 1, D,Q,S,0, alf11_mas, AF_REMOVED_IN_V3
//ldodph,              1, 0, 1, 1, 0, 1, D,Q,S,0, alf11_mas, AF_REMOVED_IN_V3
//ldodpw,              1, 0, 1, 1, 0, 1, D,Q,S,0, alf11_mas, AF_REMOVED_IN_V3
//ldodpd,              1, 0, 1, 1, 0, 1, D,Q,S,0, alf11_mas, AF_REMOVED_IN_V3
//ldodpq,              1, 0, 1, 1, 0, 1, Q,Q,S,0, alf11_mas, AF_PAIR | AF_REMOVED_IN_V3
TRANSM(stb,             0, 0, 1, 0, 0, 1, S,D,D,0, alf3_mas, gen_addr_i64, gen_st_raw_i32, MO_UB, ADDR_FLAT);
TRANSM(sth,             0, 0, 1, 0, 0, 1, S,D,D,0, alf3_mas, gen_addr_i64, gen_st_raw_i32, MO_UW, ADDR_FLAT);
TRANSM(stw,             0, 0, 1, 0, 0, 1, S,D,D,0, alf3_mas, gen_addr_i64, gen_st_raw_i32, MO_UL, ADDR_FLAT);
TRANSM(std,             0, 0, 1, 0, 0, 1, D,D,D,0, alf3_mas, gen_addr_i64, gen_st_raw_i64, MO_UQ, ADDR_FLAT);
//stq,                 0, 0, 5, 0, 0, 5, Q,D,D,0, alf13_mas, AF_PAIR
// should be illop if not e2k32
#if 1 || defined(TARGET_E2K32)
TRANSM(stcsb,           0, 0, 1, 0, 0, 1, S,S,S,0, alf3_mas, gen_addr_i32, gen_st_raw_i32, MO_UB, ADDR_CS);
TRANSM(stcsh,           0, 0, 1, 0, 0, 1, S,S,S,0, alf3_mas, gen_addr_i32, gen_st_raw_i32, MO_UW, ADDR_CS);
TRANSM(stcsw,           0, 0, 1, 0, 0, 1, S,S,S,0, alf3_mas, gen_addr_i32, gen_st_raw_i32, MO_UL, ADDR_CS);
TRANSM(stcsd,           0, 0, 1, 0, 0, 1, D,S,S,0, alf3_mas, gen_addr_i32, gen_st_raw_i64, MO_UQ, ADDR_CS);
//stcsq,               0, 0, 1, 0, 0, 1, Q,S,S,0, alf13_mas, AF_PAIR
TRANSM(stdsb,           0, 0, 1, 0, 0, 1, S,S,S,0, alf3_mas, gen_addr_i32, gen_st_raw_i32, MO_UB, ADDR_DS);
TRANSM(stdsh,           0, 0, 1, 0, 0, 1, S,S,S,0, alf3_mas, gen_addr_i32, gen_st_raw_i32, MO_UW, ADDR_DS);
TRANSM(stdsw,           0, 0, 1, 0, 0, 1, S,S,S,0, alf3_mas, gen_addr_i32, gen_st_raw_i32, MO_UL, ADDR_DS);
TRANSM(stdsd,           0, 0, 1, 0, 0, 1, D,S,S,0, alf3_mas, gen_addr_i32, gen_st_raw_i64, MO_UQ, ADDR_DS);
//stdsq,               0, 0, 1, 0, 0, 1, Q,S,S,0, alf13_mas, AF_PAIR
TRANSM(stesb,           0, 0, 1, 0, 0, 1, S,S,S,0, alf3_mas, gen_addr_i32, gen_st_raw_i32, MO_UB, ADDR_ES);
TRANSM(stesh,           0, 0, 1, 0, 0, 1, S,S,S,0, alf3_mas, gen_addr_i32, gen_st_raw_i32, MO_UW, ADDR_ES);
TRANSM(stesw,           0, 0, 1, 0, 0, 1, S,S,S,0, alf3_mas, gen_addr_i32, gen_st_raw_i32, MO_UL, ADDR_ES);
TRANSM(stesd,           0, 0, 1, 0, 0, 1, D,S,S,0, alf3_mas, gen_addr_i32, gen_st_raw_i64, MO_UQ, ADDR_ES);
//stesq,               0, 0, 1, 0, 0, 1, Q,S,S,0, alf13_mas, AF_PAIR
TRANSM(stfsb,           0, 0, 1, 0, 0, 1, S,S,S,0, alf3_mas, gen_addr_i32, gen_st_raw_i32, MO_UB, ADDR_FS);
TRANSM(stfsh,           0, 0, 1, 0, 0, 1, S,S,S,0, alf3_mas, gen_addr_i32, gen_st_raw_i32, MO_UW, ADDR_FS);
TRANSM(stfsw,           0, 0, 1, 0, 0, 1, S,S,S,0, alf3_mas, gen_addr_i32, gen_st_raw_i32, MO_UL, ADDR_FS);
TRANSM(stfsd,           0, 0, 1, 0, 0, 1, D,S,S,0, alf3_mas, gen_addr_i32, gen_st_raw_i64, MO_UQ, ADDR_FS);
//stfsq,               0, 0, 1, 0, 0, 1, Q,S,S,0, alf13_mas, AF_PAIR
TRANSM(stgsb,           0, 0, 1, 0, 0, 1, S,S,S,0, alf3_mas, gen_addr_i32, gen_st_raw_i32, MO_UB, ADDR_GS);
TRANSM(stgsh,           0, 0, 1, 0, 0, 1, S,S,S,0, alf3_mas, gen_addr_i32, gen_st_raw_i32, MO_UW, ADDR_GS);
TRANSM(stgsw,           0, 0, 1, 0, 0, 1, S,S,S,0, alf3_mas, gen_addr_i32, gen_st_raw_i32, MO_UL, ADDR_GS);
TRANSM(stgsd,           0, 0, 1, 0, 0, 1, D,S,S,0, alf3_mas, gen_addr_i32, gen_st_raw_i64, MO_UQ, ADDR_GS);
//stgsq,               0, 0, 1, 0, 0, 1, Q,S,S,0, alf13_mas, AF_PAIR
TRANSM(stssb,           0, 0, 1, 0, 0, 1, S,S,S,0, alf3_mas, gen_addr_i32, gen_st_raw_i32, MO_UB, ADDR_SS);
TRANSM(stssh,           0, 0, 1, 0, 0, 1, S,S,S,0, alf3_mas, gen_addr_i32, gen_st_raw_i32, MO_UW, ADDR_SS);
TRANSM(stssw,           0, 0, 1, 0, 0, 1, S,S,S,0, alf3_mas, gen_addr_i32, gen_st_raw_i32, MO_UL, ADDR_SS);
TRANSM(stssd,           0, 0, 1, 0, 0, 1, D,S,S,0, alf3_mas, gen_addr_i32, gen_st_raw_i64, MO_UQ, ADDR_SS);
//stssq,               0, 0, 1, 0, 0, 1, Q,S,S,0, alf13_mas, AF_PAIR
TRANSM(stgdb,           0, 0, 1, 0, 0, 1, S,S,S,0, alf3_mas, gen_addr_i32, gen_st_raw_i32, MO_UB, ADDR_GD);
TRANSM(stgdh,           0, 0, 1, 0, 0, 1, S,S,S,0, alf3_mas, gen_addr_i32, gen_st_raw_i32, MO_UW, ADDR_GD);
TRANSM(stgdw,           0, 0, 1, 0, 0, 1, S,S,S,0, alf3_mas, gen_addr_i32, gen_st_raw_i32, MO_UL, ADDR_GD);
TRANSM(stgdd,           0, 0, 1, 0, 0, 1, D,S,S,0, alf3_mas, gen_addr_i32, gen_st_raw_i64, MO_UQ, ADDR_GD);
//stgdq,               0, 0, 1, 0, 0, 1, Q,S,S,0, alf13_mas, AF_PAIR
#endif
//stodrb,              0, 0, 1, 0, 0, 1, S,Q,S,0, alf13_mas, AF_REMOVED_IN_V3
//stodrh,              0, 0, 1, 0, 0, 1, S,Q,S,0, alf13_mas, AF_REMOVED_IN_V3
//stodrw,              0, 0, 1, 0, 0, 1, S,Q,S,0, alf13_mas, AF_REMOVED_IN_V3
//stodrd,              0, 0, 1, 0, 0, 1, D,Q,S,0, alf13_mas, AF_REMOVED_IN_V3
//stodrq,              0, 0, 1, 0, 0, 1, Q,Q,S,0, alf13_mas, AF_PAIR | AF_REMOVED_IN_V3
//stodwb,              0, 0, 1, 0, 0, 1, S,Q,S,0, alf13_mas, AF_REMOVED_IN_V3
//stodwh,              0, 0, 1, 0, 0, 1, S,Q,S,0, alf13_mas, AF_REMOVED_IN_V3
//stodww,              0, 0, 1, 0, 0, 1, S,Q,S,0, alf13_mas, AF_REMOVED_IN_V3
//stodwd,              0, 0, 1, 0, 0, 1, D,Q,S,0, alf13_mas, AF_REMOVED_IN_V3
//stodwq,              0, 0, 1, 0, 0, 1, Q,Q,S,0, alf13_mas, AF_PAIR | AF_REMOVED_IN_V3
//stodpb,              0, 0, 1, 0, 0, 1, S,Q,S,0, alf13_mas, AF_REMOVED_IN_V3
//stodph,              0, 0, 1, 0, 0, 1, S,Q,S,0, alf13_mas, AF_REMOVED_IN_V3
//stodpw,              0, 0, 1, 0, 0, 1, S,Q,S,0, alf13_mas, AF_REMOVED_IN_V3
//stodpd,              0, 0, 1, 0, 0, 1, D,Q,S,0, alf13_mas, AF_REMOVED_IN_V3
//stodpq,              0, 0, 1, 0, 0, 1, Q,Q,S,0, alf13_mas, AF_PAIR | AF_REMOVED_IN_V3

TRANSA(staab,           0, 0, 1, 0, 0, 1, S,0,0,0, alf10_mas, OP_STAAB, gen_staab);
TRANSA(staah,           0, 0, 1, 0, 0, 1, S,0,0,0, alf10_mas, OP_STAAH, gen_staah);
TRANSA(staaw,           0, 0, 1, 0, 0, 1, S,0,0,0, alf10_mas, OP_STAAW, gen_staaw);
TRANSA(staad,           0, 0, 1, 0, 0, 1, D,0,0,0, alf10_mas, OP_STAAD, gen_staad);
TRANSA(staaq,           0, 0, 1, 0, 0, 1, Q,0,0,0, alf10_mas, OP_STAAQ, gen_staaq);
//stapb,               0, 0, 1, 0, 0, 1, S,Q,S,0, alf13_mas
//staph,               0, 0, 1, 0, 0, 1, S,Q,S,0, alf13_mas
//stapw,               0, 0, 1, 0, 0, 1, S,Q,S,0, alf13_mas
//stapd,               0, 0, 1, 0, 0, 1, D,Q,S,0, alf13_mas
//stapq,               0, 0, 1, 0, 0, 1, Q,Q,S,0, alf13_mas, AF_PAIR
//ldrd,                1, 0, 1, 1, 0, 1, D,D,D,0, alf11
//strd,                0, 0, 1, 0, 0, 1, D,D,D,0, alf13_mas
//aaurrw,              0, 0, 1, 0, 0, 1, S,0,0,0, aaurr
//aaurrd,              0, 0, 1, 0, 0, 1, D,0,0,0, aaurr
//aaurrq,              0, 0, 1, 0, 0, 1, Q,0,0,0, aaurr, AF_PAIR
//aaurww,              0, 0, 1, 0, 0, 1, S,0,0,0, aaurw
//aaurwd,              0, 0, 1, 0, 0, 1, D,0,0,0, aaurw
//aaurwq,              0, 0, 1, 0, 0, 1, Q,0,0,0, aaurw, AF_PAIR
TRANSA(gettags,         0, 0, 1, 0, 0, 1, S,0,S,0, alf2,     OP_GETTAGS, gen_gettags);
TRANSA(gettagd,         0, 0, 1, 0, 0, 1, S,0,D,0, alf2,     OP_GETTAGD, gen_gettagd);
TRANSA(puttags,         0, 0, 1, 0, 0, 1, S,S,S,0, alf1_mas, OP_PUTTAGS, gen_puttags);
TRANSA(puttagd,         0, 0, 1, 0, 0, 1, D,D,S,0, alf1_mas, OP_PUTTAGD, gen_puttagd);
//gettc,               1, 0, 0, 0, 0, 0, D,0,S,0, alf12
//puttc,               1, 0, 0, 0, 0, 0, D,D,D,0, alf11
//invtc,               1, 0, 0, 0, 0, 0, D,0,S,0, alf12
TRANSA(movts,           1, 1, 0, 1, 1, 0, S,0,S,0, alf2, OP_MOVTS,    gen_movts);
TRANSA(movtcs,          1, 1, 0, 1, 1, 0, S,0,S,0, alf2, OP_MOVTCS,   gen_movtcs);
//TRANSA(movtrs,          1, 1, 0, 1, 1, 0, S,0,S,0, alf2, OP_MOVTRS,   gen_movtrs);
//TRANSA(movtrcs,         1, 1, 0, 1, 1, 0, S,0,S,0, alf2, OP_MOVTRCS,  gen_movtrcs);
TRANSA(movtd,           1, 1, 0, 1, 1, 0, D,0,D,0, alf2, OP_MOVTD,    gen_movtd);
TRANSA(movtcd,          1, 1, 0, 1, 1, 0, D,0,D,0, alf2, OP_MOVTCD,   gen_movtcd);
//TRANSA(movtrd,          1, 1, 0, 1, 1, 0, D,0,D,0, alf2, OP_MOVTRD,   gen_movtrd);
//TRANSA(movtrcd,         1, 1, 0, 1, 1, 0, D,0,D,0, alf2, OP_MOVTRCD,  gen_movtrcd);
TRANSA(movtq,           1, 1, 0, 1, 1, 0, Q,0,Q,0, alf2, OP_MOVTQ,  gen_movtq);
TRANSA(movtcq,          1, 1, 0, 1, 1, 0, Q,0,Q,0, alf2, OP_MOVTCQ, gen_movtcq);
//movtrq,              1, 1, 0, 1, 1, 0, Q,0,Q,0, alf22, AF_PAIR
//movtrcq,             1, 1, 0, 1, 1, 0, Q,0,Q,0, alf22, AF_PAIR
//getsap,              1, 1, 0, 0, 0, 0, Q,0,S,0, alf2, AF_PAIR
//cudtoap,             1, 1, 0, 1, 1, 0, Q,0,S,0, alf2, AF_PAIR
//gdtoap,              1, 1, 0, 1, 1, 0, Q,0,S,0, alf2, AF_PAIR
//vfsi,                0, 1, 0, 0, 1, 0, D,S,D,0, alf1
TRANSA(getpl,           1, 0, 0, 1, 0, 0, D,0,S,0, alf2, OP_GETPL, gen_getpl);
TRANSW(fxcmpeqsb,       1, 1, 0, 1, 1, 0, D,X,S,0, alf7, _env_, gen_helper_fxcmpeqs);
TRANSW(fxcmpltsb,       1, 1, 0, 1, 1, 0, D,X,S,0, alf7, _env_, gen_helper_fxcmplts);
TRANSW(fxcmplesb,       1, 1, 0, 1, 1, 0, D,X,S,0, alf7, _env_, gen_helper_fxcmples);
TRANSW(fxcmpuodsb,      1, 1, 0, 1, 1, 0, D,X,S,0, alf7, _env_, gen_helper_fxcmpuods);
TRANSW(fxcmpneqsb,      1, 1, 0, 1, 1, 0, D,X,S,0, alf7, _env_, gen_helper_fxcmpneqs);
TRANSW(fxcmpnltsb,      1, 1, 0, 1, 1, 0, D,X,S,0, alf7, _env_, gen_helper_fxcmpnlts);
TRANSW(fxcmpnlesb,      1, 1, 0, 1, 1, 0, D,X,S,0, alf7, _env_, gen_helper_fxcmpnles);
TRANSW(fxcmpodsb,       1, 1, 0, 1, 1, 0, D,X,S,0, alf7, _env_, gen_helper_fxcmpods);
TRANSW(fxcmpeqdb,       1, 1, 0, 1, 1, 0, D,X,D,0, alf7, _env_, gen_helper_fxcmpeqd);
TRANSW(fxcmpltdb,       1, 1, 0, 1, 1, 0, D,X,D,0, alf7, _env_, gen_helper_fxcmpltd);
TRANSW(fxcmpledb,       1, 1, 0, 1, 1, 0, D,X,D,0, alf7, _env_, gen_helper_fxcmpled);
TRANSW(fxcmpuoddb,      1, 1, 0, 1, 1, 0, D,X,D,0, alf7, _env_, gen_helper_fxcmpuodd);
TRANSW(fxcmpneqdb,      1, 1, 0, 1, 1, 0, D,X,D,0, alf7, _env_, gen_helper_fxcmpneqd);
TRANSW(fxcmpnltdb,      1, 1, 0, 1, 1, 0, D,X,D,0, alf7, _env_, gen_helper_fxcmpnltd);
TRANSW(fxcmpnledb,      1, 1, 0, 1, 1, 0, D,X,D,0, alf7, _env_, gen_helper_fxcmpnled);
TRANSW(fxcmpoddb,       1, 1, 0, 1, 1, 0, D,X,D,0, alf7, _env_, gen_helper_fxcmpodd);
TRANSW(fxcmpeqxb,       1, 1, 0, 1, 1, 0, D,X,X,0, alf7, _env_, gen_helper_fxcmpeqx);
TRANSW(fxcmpltxb,       1, 1, 0, 1, 1, 0, D,X,X,0, alf7, _env_, gen_helper_fxcmpltx);
TRANSW(fxcmplexb,       1, 1, 0, 1, 1, 0, D,X,X,0, alf7, _env_, gen_helper_fxcmplex);
TRANSW(fxcmpuodxb,      1, 1, 0, 1, 1, 0, D,X,X,0, alf7, _env_, gen_helper_fxcmpuodx);
TRANSW(fxcmpneqxb,      1, 1, 0, 1, 1, 0, D,X,X,0, alf7, _env_, gen_helper_fxcmpneqx);
TRANSW(fxcmpnltxb,      1, 1, 0, 1, 1, 0, D,X,X,0, alf7, _env_, gen_helper_fxcmpnltx);
TRANSW(fxcmpnlexb,      1, 1, 0, 1, 1, 0, D,X,X,0, alf7, _env_, gen_helper_fxcmpnlex);
TRANSW(fxcmpodxb,       1, 1, 0, 1, 1, 0, D,X,X,0, alf7, _env_, gen_helper_fxcmpodx);
TRANSW(fcmpeqsb,        1, 1, 0, 1, 1, 0, S,S,S,0, alf7, _env_, gen_helper_fcmpeqs);
TRANSW(fcmpltsb,        1, 1, 0, 1, 1, 0, S,S,S,0, alf7, _env_, gen_helper_fcmplts);
TRANSW(fcmplesb,        1, 1, 0, 1, 1, 0, S,S,S,0, alf7, _env_, gen_helper_fcmples);
TRANSW(fcmpuodsb,       1, 1, 0, 1, 1, 0, S,S,S,0, alf7, _env_, gen_helper_fcmpuods);
TRANSW(fcmpneqsb,       1, 1, 0, 1, 1, 0, S,S,S,0, alf7, _env_, gen_helper_fcmpneqs);
TRANSW(fcmpnltsb,       1, 1, 0, 1, 1, 0, S,S,S,0, alf7, _env_, gen_helper_fcmpnlts);
TRANSW(fcmpnlesb,       1, 1, 0, 1, 1, 0, S,S,S,0, alf7, _env_, gen_helper_fcmpnles);
TRANSW(fcmpodsb,        1, 1, 0, 1, 1, 0, S,S,S,0, alf7, _env_, gen_helper_fcmpods);
TRANSW(fcmpeqdb,        1, 1, 0, 1, 1, 0, D,D,D,0, alf7, _env_, gen_helper_fcmpeqd);
TRANSW(fcmpltdb,        1, 1, 0, 1, 1, 0, D,D,D,0, alf7, _env_, gen_helper_fcmpltd);
TRANSW(fcmpledb,        1, 1, 0, 1, 1, 0, D,D,D,0, alf7, _env_, gen_helper_fcmpled);
TRANSW(fcmpuoddb,       1, 1, 0, 1, 1, 0, D,D,D,0, alf7, _env_, gen_helper_fcmpuodd);
TRANSW(fcmpneqdb,       1, 1, 0, 1, 1, 0, D,D,D,0, alf7, _env_, gen_helper_fcmpneqd);
TRANSW(fcmpnltdb,       1, 1, 0, 1, 1, 0, D,D,D,0, alf7, _env_, gen_helper_fcmpnltd);
TRANSW(fcmpnledb,       1, 1, 0, 1, 1, 0, D,D,D,0, alf7, _env_, gen_helper_fcmpnled);
TRANSW(fcmpoddb,        1, 1, 0, 1, 1, 0, D,D,D,0, alf7, _env_, gen_helper_fcmpodd);
TRANSW(fadds,           1, 1, 4, 1, 1, 4, S,S,S,0, alf1, _env_, gen_helper_fadds);
TRANSW(faddd,           1, 1, 4, 1, 1, 4, D,D,D,0, alf1, _env_, gen_helper_faddd);
TRANSW(fsubs,           1, 1, 4, 1, 1, 4, S,S,S,0, alf1, _env_, gen_helper_fsubs);
TRANSW(fsubd,           1, 1, 4, 1, 1, 4, D,D,D,0, alf1, _env_, gen_helper_fsubd);
TRANSW(fmins,           1, 1, 0, 1, 1, 0, S,S,S,0, alf1, _env_, gen_helper_fmins);
TRANSW(fmind,           1, 1, 0, 1, 1, 0, D,D,D,0, alf1, _env_, gen_helper_fmind);
TRANSW(fmaxs,           1, 1, 0, 1, 1, 0, S,S,S,0, alf1, _env_, gen_helper_fmaxs);
TRANSW(fmaxd,           1, 1, 0, 1, 1, 0, D,D,D,0, alf1, _env_, gen_helper_fmaxd);
TRANSW(fmuls,           1, 1, 4, 1, 1, 4, S,S,S,0, alf1, _env_, gen_helper_fmuls);
TRANSW(fmuld,           1, 1, 4, 1, 1, 4, D,D,D,0, alf1, _env_, gen_helper_fmuld);
TRANSW(fstoiw,          1, 1, 0, 1, 1, 0, S,0,S,0, alf2, _env_, gen_helper_fstois);
TRANSW(fstoiwtr,        1, 1, 0, 1, 1, 0, S,0,S,0, alf2, _env_, gen_helper_fstoistr);
TRANSW(iwtofs,          1, 1, 0, 1, 1, 0, S,0,S,0, alf2, _env_, gen_helper_istofs);
TRANSW(fdtoid,          1, 1, 0, 1, 1, 0, D,0,D,0, alf2, _env_, gen_helper_fdtoid);
TRANSW(fxtoid,          1, 1, 0, 1, 1, 0, D,0,X,0, alf2, _env_, gen_helper_fxtoid);
TRANSW(idtofd,          1, 1, 0, 1, 1, 0, D,0,D,0, alf2, _env_, gen_helper_idtofd);
TRANSW(idtofx,          1, 1, 0, 1, 1, 0, X,0,D,0, alf2, _env_, gen_helper_idtofx);
TRANSW(fxtofd,          1, 1, 0, 1, 1, 0, D,0,X,0, alf2, _env_, gen_helper_fxtofd);
TRANSW(fdtofx,          1, 1, 0, 1, 1, 0, X,0,D,0, alf2, _env_, gen_helper_fdtofx);
TRANSW(pfstoiw,         1, 1, 0, 1, 1, 0, D,0,D,0, alf2, _env_, gen_helper_pfstois);
TRANSW(pfstoiwtr,       1, 1, 0, 1, 1, 0, D,0,D,0, alf2, _env_, gen_helper_pfstoistr);
TRANSW(piwtofs,         1, 1, 0, 1, 1, 0, D,0,D,0, alf2, _env_, gen_helper_pistofs);
TRANSW(fstoid,          1, 1, 0, 1, 1, 0, D,0,S,0, alf2, _env_, gen_helper_fstoid);
TRANSW(iwtofd,          1, 1, 0, 1, 1, 0, D,0,S,0, alf2, _env_, gen_helper_istofd);
TRANSW(iwtofx,          1, 1, 0, 1, 1, 0, X,0,S,0, alf2, _env_, gen_helper_istofx);
TRANSW(fstofd,          1, 1, 0, 1, 1, 0, D,0,S,0, alf2, _env_, gen_helper_fstofd);
TRANSW(fstofx,          1, 1, 0, 1, 1, 0, X,0,S,0, alf2, _env_, gen_helper_fstofx);
TRANSW(pfstofd,         1, 1, 0, 1, 1, 0, D,0,S,0, alf2, _env_, gen_helper_fstofd);
TRANSW(fdtoiw,          1, 1, 0, 1, 1, 0, S,0,D,0, alf2, _env_, gen_helper_fdtois);
TRANSW(fxtoiw,          1, 1, 0, 1, 1, 0, S,0,X,0, alf2, _env_, gen_helper_fxtois);
TRANSW(fdtoiwtr,        1, 1, 0, 1, 1, 0, S,0,D,0, alf2, _env_, gen_helper_fdtoistr);
TRANSW(idtofs,          1, 1, 0, 1, 1, 0, S,0,D,0, alf2, _env_, gen_helper_idtofs);
TRANSW(fdtofs,          1, 1, 0, 1, 1, 0, S,0,D,0, alf2, _env_, gen_helper_fdtofs);
TRANSW(fxtofs,          1, 1, 0, 1, 1, 0, S,0,X,0, alf2, _env_, gen_helper_fxtofs);
TRANSW(pfdtoiw,         1, 1, 0, 1, 1, 0, S,0,D,0, alf2, _env_, gen_helper_fdtois);
TRANSW(pfdtoiwtr,       1, 1, 0, 1, 1, 0, S,0,D,0, alf2, _env_, gen_helper_fdtoistr);
TRANSW(pfdtofs,         1, 1, 0, 1, 1, 0, S,0,D,0, alf2, _env_, gen_helper_fdtofs);
TRANSW(udivw,           0, 0, 0, 0, 0, 1, S,S,S,0, alf1, _tag_, gen_udivs);
TRANSW(udivd,           0, 0, 0, 0, 0, 1, D,D,D,0, alf1, _tag_, gen_udivd);
TRANSW(sdivw,           0, 0, 0, 0, 0, 1, S,S,S,0, alf1, _tag_, gen_sdivs);
TRANSW(sdivd,           0, 0, 0, 0, 0, 1, D,D,D,0, alf1, _tag_, gen_sdivd);
TRANSW(udivx,           0, 0, 0, 0, 0, 1, S,D,S,0, alf1, _tag_, gen_udivx);
TRANSW(umodx,           0, 0, 0, 0, 0, 1, S,D,S,0, alf1, _tag_, gen_umodx);
TRANSW(sdivx,           0, 0, 0, 0, 0, 1, S,D,S,0, alf1, _tag_, gen_sdivx);
TRANSW(smodx,           0, 0, 0, 0, 0, 1, S,D,S,0, alf1, _tag_, gen_smodx);
TRANSW(fxaddss,         1, 1, 0, 1, 1, 0, S,X,S,0, alf1, _env_, gen_helper_fxaddss);
TRANSW(fxadddd,         1, 1, 0, 1, 1, 0, D,X,D,0, alf1, _env_, gen_helper_fxadddd);
TRANSW(fxaddsx,         1, 1, 0, 1, 1, 0, X,X,S,0, alf1, _env_, gen_helper_fxaddsx);
TRANSW(fxadddx,         1, 1, 0, 1, 1, 0, X,X,D,0, alf1, _env_, gen_helper_fxadddx);
TRANSW(fxaddxs,         1, 1, 0, 1, 1, 0, S,X,X,0, alf1, _env_, gen_helper_fxaddxs);
TRANSW(fxaddxd,         1, 1, 0, 1, 1, 0, D,X,X,0, alf1, _env_, gen_helper_fxaddxd);
TRANSW(fxaddxx,         1, 1, 0, 1, 1, 0, X,X,X,0, alf1, _env_, gen_helper_fxaddxx);
TRANSW(fxdivss,         0, 0, 0, 0, 0, 1, S,X,S,0, alf1, _env_, gen_helper_fxdivss);
TRANSW(fxsubss,         1, 1, 0, 1, 1, 0, S,X,S,0, alf1, _env_, gen_helper_fxsubss);
TRANSW(fxdivdd,         0, 0, 0, 0, 0, 1, D,X,D,0, alf1, _env_, gen_helper_fxdivdd);
TRANSW(fxsubdd,         1, 1, 0, 1, 1, 0, D,X,D,0, alf1, _env_, gen_helper_fxsubdd);
TRANSW(fxdivsx,         0, 0, 0, 0, 0, 1, X,X,S,0, alf1, _env_, gen_helper_fxdivsx);
TRANSW(fxsubsx,         1, 1, 0, 1, 1, 0, X,X,S,0, alf1, _env_, gen_helper_fxsubsx);
TRANSW(fxdivdx,         0, 0, 0, 0, 0, 1, X,X,D,0, alf1, _env_, gen_helper_fxdivdx);
TRANSW(fxsubdx,         1, 1, 0, 1, 1, 0, X,X,D,0, alf1, _env_, gen_helper_fxsubdx);
TRANSW(fxdivxs,         0, 0, 0, 0, 0, 1, S,X,X,0, alf1, _env_, gen_helper_fxdivxs);
TRANSW(fxsubxs,         1, 1, 0, 1, 1, 0, S,X,X,0, alf1, _env_, gen_helper_fxsubxs);
TRANSW(fxdivxd,         0, 0, 0, 0, 0, 1, D,X,X,0, alf1, _env_, gen_helper_fxdivxd);
TRANSW(fxsubxd,         1, 1, 0, 1, 1, 0, D,X,X,0, alf1, _env_, gen_helper_fxsubxd);
TRANSW(fxdivxx,         0, 0, 0, 0, 0, 1, X,X,X,0, alf1, _env_, gen_helper_fxdivxx);
TRANSW(fxsubxx,         1, 1, 0, 1, 1, 0, X,X,X,0, alf1, _env_, gen_helper_fxsubxx);
TRANSW(fxmulss,         1, 1, 0, 1, 1, 0, S,X,S,0, alf1, _env_, gen_helper_fxmulss);
TRANSW(fxmuldd,         1, 1, 0, 1, 1, 0, D,X,D,0, alf1, _env_, gen_helper_fxmuldd);
TRANSW(fxmulsx,         1, 1, 0, 1, 1, 0, X,X,S,0, alf1, _env_, gen_helper_fxmulsx);
TRANSW(fxmuldx,         1, 1, 0, 1, 1, 0, X,X,D,0, alf1, _env_, gen_helper_fxmuldx);
TRANSW(fxmulxs,         1, 1, 0, 1, 1, 0, S,X,X,0, alf1, _env_, gen_helper_fxmulxs);
TRANSW(fxmulxd,         1, 1, 0, 1, 1, 0, D,X,X,0, alf1, _env_, gen_helper_fxmulxd);
TRANSW(fxmulxx,         1, 1, 0, 1, 1, 0, X,X,X,0, alf1, _env_, gen_helper_fxmulxx);
TRANSW(fxrsubss,        1, 1, 0, 1, 1, 0, S,X,S,0, alf1, _env_, gen_helper_fxrsubss);
TRANSW(fxrsubdd,        1, 1, 0, 1, 1, 0, D,X,D,0, alf1, _env_, gen_helper_fxrsubdd);
TRANSW(fxrsubsx,        1, 1, 0, 1, 1, 0, X,X,S,0, alf1, _env_, gen_helper_fxrsubsx);
TRANSW(fxrsubdx,        1, 1, 0, 1, 1, 0, X,X,D,0, alf1, _env_, gen_helper_fxrsubdx);
TRANSW(fxsqrtisx,       0, 0, 0, 0, 0, 1, X,S,X,0, alf1, _,     gen_fxsqrtisx);
TRANSW(fxsqrtidx,       0, 0, 0, 0, 0, 1, X,D,X,0, alf1, _,     gen_fxsqrtidx);
TRANSW(fxsqrtixx,       0, 0, 0, 0, 0, 1, X,X,X,0, alf1, _,     gen_fxsqrtixx);
TRANSW(fxsqrtuxx,       0, 0, 0, 0, 0, 1, X,X,X,0, alf1, _,     gen_fxsqrtuxx);
TRANSW(fxsqrtusx,       0, 0, 0, 0, 0, 1, X,S,X,0, alf1, _,     gen_fxsqrtusx);
TRANSW(fxsqrtudx,       0, 0, 0, 0, 0, 1, X,D,X,0, alf1, _,     gen_fxsqrtudx);
TRANSW(fxsqrttxx,       0, 0, 0, 0, 0, 1, X,X,X,0, alf1, _env_, gen_helper_fxsqrttxx);
TRANSW(fxsqrttsx,       0, 0, 0, 0, 0, 1, X,S,X,0, alf1, _env_, gen_helper_fxsqrttsx);
TRANSW(fxsqrttdx,       0, 0, 0, 0, 0, 1, X,D,X,0, alf1, _env_, gen_helper_fxsqrttdx);
TRANSW(fxdivtss,        0, 0, 0, 0, 0, 1, S,S,X,0, alf1, _env_, gen_helper_fxdivtss);
TRANSW(fxdivtdd,        0, 0, 0, 0, 0, 1, D,D,X,0, alf1, _env_, gen_helper_fxdivtdd);
TRANSW(fxdivtsx,        0, 0, 0, 0, 0, 1, X,S,X,0, alf1, _env_, gen_helper_fxdivtsx);
TRANSW(fxdivtdx,        0, 0, 0, 0, 0, 1, X,D,X,0, alf1, _env_, gen_helper_fxdivtdx);
TRANSW(movfi,           2, 1, 0, 2, 1, 0, S,0,X,0, alf2, _,     gen_movfi);
TRANSW(movif,           2, 1, 0, 2, 1, 0, X,D,S,0, alf1, _,     gen_movif);
TRANSW(pminub,          1, 0, 0, 1, 0, 0, D,D,D,0, alf1, _, gen_helper_pminub);
TRANSW(pminsh,          1, 0, 0, 1, 0, 0, D,D,D,0, alf1, _, gen_helper_pminsh);
TRANSW(pmaxub,          1, 0, 0, 1, 0, 0, D,D,D,0, alf1, _, gen_helper_pmaxub);
TRANSW(pmaxsh,          1, 0, 0, 1, 0, 0, D,D,D,0, alf1, _, gen_helper_pmaxsh);
TRANSW(punpckhbh,       0, 1, 0, 0, 1, 0, D,D,D,0, alf1, _, gen_helper_punpckhbh);
TRANSW(punpcklbh,       0, 1, 0, 0, 1, 0, D,D,D,0, alf1, _, gen_helper_punpcklbh);
TRANSW(punpckhhw,       0, 1, 0, 0, 1, 0, D,D,D,0, alf1, _, gen_helper_punpckhhw);
TRANSW(punpcklhw,       0, 1, 0, 0, 1, 0, D,D,D,0, alf1, _, gen_helper_punpcklhw);
TRANSW(punpckhwd,       0, 1, 0, 0, 1, 0, D,D,D,0, alf1, _, gen_helper_punpckhwd);
TRANSW(punpcklwd,       0, 1, 0, 0, 1, 0, D,D,D,0, alf1, _, gen_helper_punpcklwd);
TRANSW(pmovmskps,       0, 1, 0, 0, 1, 0, D,D,D,0, alf1, _, gen_helper_pmovmskps);
TRANSW(pmovmskpd,       0, 1, 0, 0, 1, 0, D,D,D,0, alf1, _, gen_helper_pmovmskpd);
TRANSW(packsshb,        0, 1, 0, 0, 1, 0, D,D,D,0, alf1, _, gen_helper_packsshb);
TRANSW(packushb,        0, 1, 0, 0, 1, 0, D,D,D,0, alf1, _, gen_helper_packushb);
TRANSW(packsswh,        0, 1, 0, 0, 1, 0, D,D,D,0, alf1, _, gen_helper_packsswh);
TRANSW(paddb,           1, 0, 0, 1, 0, 0, D,D,D,0, alf1, _, tcg_gen_vec_add8_i64);
TRANSW(paddh,           1, 0, 0, 1, 0, 0, D,D,D,0, alf1, _, tcg_gen_vec_add16_i64);
TRANSW(paddw,           1, 0, 0, 1, 0, 0, D,D,D,0, alf1, _, tcg_gen_vec_add32_i64);
TRANSW(paddd,           1, 0, 0, 1, 0, 0, D,D,D,0, alf1, _, tcg_gen_add_i64);
TRANSW(paddsb,          1, 0, 0, 1, 0, 0, D,D,D,0, alf1, _, gen_helper_paddsb);
TRANSW(paddsh,          1, 0, 0, 1, 0, 0, D,D,D,0, alf1, _, gen_helper_paddsh);
TRANSW(pmovmskb,        0, 1, 0, 0, 1, 0, D,D,D,0, alf1, _, gen_helper_pmovmskb);
TRANSW(paddusb,         1, 0, 0, 1, 0, 0, D,D,D,0, alf1, _, gen_helper_paddusb);
TRANSW(paddush,         1, 0, 0, 1, 0, 0, D,D,D,0, alf1, _, gen_helper_paddush);
TRANSW(psubb,           1, 0, 0, 1, 0, 0, D,D,D,0, alf1, _, tcg_gen_vec_sub8_i64);
TRANSW(psubh,           1, 0, 0, 1, 0, 0, D,D,D,0, alf1, _, tcg_gen_vec_sub16_i64);
TRANSW(psubw,           1, 0, 0, 1, 0, 0, D,D,D,0, alf1, _, tcg_gen_vec_sub32_i64);
TRANSW(psubd,           1, 0, 0, 1, 0, 0, D,D,D,0, alf1, _, tcg_gen_sub_i64);
TRANSW(psubsb,          1, 0, 0, 1, 0, 0, D,D,D,0, alf1, _, gen_helper_psubsb);
TRANSW(psubsh,          1, 0, 0, 1, 0, 0, D,D,D,0, alf1, _, gen_helper_psubsh);
TRANSW(psubusb,         1, 0, 0, 1, 0, 0, D,D,D,0, alf1, _, gen_helper_psubusb);
TRANSW(psubush,         1, 0, 0, 1, 0, 0, D,D,D,0, alf1, _, gen_helper_psubush);
TRANSW(psrlqh,          0, 1, 0, 0, 1, 0, D,D,D,0, alf1_lit8, _, gen_psrlqh);
TRANSW(psrlql,          0, 1, 0, 0, 1, 0, D,D,D,0, alf1_lit8, _, gen_psrlql);
TRANSW(psllqh,          0, 1, 0, 0, 1, 0, D,D,D,0, alf1_lit8, _, gen_psllqh);
TRANSW(psllql,          0, 1, 0, 0, 1, 0, D,D,D,0, alf1_lit8, _, gen_psllql);
TRANSW(psrlw,           3, 1, 0, 3, 1, 0, D,D,D,0, alf1, _, gen_helper_psrlw);
TRANSW(psrlh,           3, 1, 0, 3, 1, 0, D,D,D,0, alf1, _, gen_helper_psrlh);
TRANSW(psraw,           3, 1, 0, 3, 1, 0, D,D,D,0, alf1, _, gen_helper_psraw);
TRANSW(psrah,           3, 1, 0, 3, 1, 0, D,D,D,0, alf1, _, gen_helper_psrah);
TRANSW(psllw,           3, 1, 0, 3, 1, 0, D,D,D,0, alf1, _, gen_helper_psllw);
TRANSW(psllh,           3, 1, 0, 3, 1, 0, D,D,D,0, alf1, _, gen_helper_psllh);
TRANSW(pshufw,          0, 1, 0, 0, 1, 0, D,D,D,0, alf1_lit8, _, gen_pshufw);
TRANSW(pshufh,          0, 1, 0, 0, 1, 0, D,0,D,0, alf2_pshufh, _, gen_helper_pshufh);
TRANSW(pcmpeqb,         1, 0, 0, 1, 0, 0, D,D,D,0, alf1, _, gen_helper_pcmpeqb);
TRANSW(pcmpeqh,         1, 0, 0, 1, 0, 0, D,D,D,0, alf1, _, gen_helper_pcmpeqh);
TRANSW(pcmpeqw,         1, 0, 0, 1, 0, 0, D,D,D,0, alf1, _, gen_helper_pcmpeqw);
TRANSW(pcmpgtb,         1, 0, 0, 1, 0, 0, D,D,D,0, alf1, _, gen_helper_pcmpgtb);
TRANSW(pcmpgth,         1, 0, 0, 1, 0, 0, D,D,D,0, alf1, _, gen_helper_pcmpgth);
TRANSW(pcmpgtw,         1, 0, 0, 1, 0, 0, D,D,D,0, alf1, _, gen_helper_pcmpgtw);
TRANSW(pmulhh,          0, 1, 0, 0, 1, 0, D,D,D,0, alf1, _, gen_helper_pmulhh);
TRANSW(pmullh,          0, 1, 0, 0, 1, 0, D,D,D,0, alf1, _, gen_helper_pmullh);
TRANSW(pmaddh,          0, 1, 0, 0, 1, 0, D,D,D,0, alf1, _, gen_helper_pmaddh);
TRANSW(pmulhuh,         0, 1, 0, 0, 1, 0, D,D,D,0, alf1, _, gen_helper_pmulhuh);
TRANSW(psadbw,          0, 1, 0, 0, 1, 0, D,D,D,0, alf1, _, gen_helper_psadbw);
TRANSW(pextrh,          0, 1, 0, 0, 1, 0, D,D,D,0, alf1_lit8, _, gen_pextrh);
TRANSW(pinsh,           0, 1, 0, 0, 1, 0, D,D,D,0, alf1_lit8, _, gen_pinsh);
TRANSW(pavgusb,         1, 0, 0, 1, 0, 0, D,D,D,0, alf1, _, gen_helper_pavgusb);
TRANSW(pavgush,         1, 0, 0, 1, 0, 0, D,D,D,0, alf1, _, gen_helper_pavgush);
TRANSW(mulw,            1, 1, 0, 1, 1, 0, S,S,S,0, alf1, _, tcg_gen_mul_i32);
TRANSW(muld,            1, 1, 0, 1, 1, 0, D,D,D,0, alf1, _, tcg_gen_mul_i64);
TRANSW(umulx,           1, 1, 0, 1, 1, 0, D,S,S,0, alf1, _, gen_umulx);
TRANSW(smulx,           1, 1, 0, 1, 1, 0, D,S,S,0, alf1, _, gen_smulx);
TRANSW(insfw,           1, 1, 0, 1, 1, 0, S,S,S,S, alf21, _, gen_insfs);
TRANSD(insfd,           1, 1, 0, 1, 1, 0, D,D,D,D, alf21, gen_insfd);
TRANSW(fxcmpudsf,       1, 1, 0, 1, 1, 0, S,X,S,0, alf1, _env_, gen_helper_fxcmpudsf);
TRANSW(fxcmpodsf,       1, 1, 0, 1, 1, 0, S,X,S,0, alf1, _env_, gen_helper_fxcmpodsf);
TRANSW(fxcmpuddf,       1, 1, 0, 1, 1, 0, S,X,D,0, alf1, _env_, gen_helper_fxcmpuddf);
TRANSW(fxcmpoddf,       1, 1, 0, 1, 1, 0, S,X,D,0, alf1, _env_, gen_helper_fxcmpoddf);
TRANSW(fxcmpudxf,       1, 1, 0, 1, 1, 0, S,X,X,0, alf1, _env_, gen_helper_fxcmpudxf);
TRANSW(fxcmpodxf,       1, 1, 0, 1, 1, 0, S,X,X,0, alf1, _env_, gen_helper_fxcmpodxf);
TRANSW(fcmpeqs,         1, 1, 0, 1, 1, 0, S,S,S,0, alf1, _env_, gen_helper_fcmpeqs);
TRANSW(fcmplts,         1, 1, 0, 1, 1, 0, S,S,S,0, alf1, _env_, gen_helper_fcmplts);
TRANSW(fcmples,         1, 1, 0, 1, 1, 0, S,S,S,0, alf1, _env_, gen_helper_fcmples);
TRANSW(fcmpuods,        1, 1, 0, 1, 1, 0, S,S,S,0, alf1, _env_, gen_helper_fcmpuods);
TRANSW(fcmpneqs,        1, 1, 0, 1, 1, 0, S,S,S,0, alf1, _env_, gen_helper_fcmpneqs);
TRANSW(fcmpnlts,        1, 1, 0, 1, 1, 0, S,S,S,0, alf1, _env_, gen_helper_fcmpnlts);
TRANSW(fcmpnles,        1, 1, 0, 1, 1, 0, S,S,S,0, alf1, _env_, gen_helper_fcmpnles);
TRANSW(fcmpods,         1, 1, 0, 1, 1, 0, S,S,S,0, alf1, _env_, gen_helper_fcmpods);
TRANSW(fcmpeqd,         1, 1, 0, 1, 1, 0, D,D,D,0, alf1, _env_, gen_helper_fcmpeqd);
TRANSW(fcmpltd,         1, 1, 0, 1, 1, 0, D,D,D,0, alf1, _env_, gen_helper_fcmpltd);
TRANSW(fcmpled,         1, 1, 0, 1, 1, 0, D,D,D,0, alf1, _env_, gen_helper_fcmpled);
TRANSW(fcmpuodd,        1, 1, 0, 1, 1, 0, D,D,D,0, alf1, _env_, gen_helper_fcmpuodd);
TRANSW(fcmpneqd,        1, 1, 0, 1, 1, 0, D,D,D,0, alf1, _env_, gen_helper_fcmpneqd);
TRANSW(fcmpnltd,        1, 1, 0, 1, 1, 0, D,D,D,0, alf1, _env_, gen_helper_fcmpnltd);
TRANSW(fcmpnled,        1, 1, 0, 1, 1, 0, D,D,D,0, alf1, _env_, gen_helper_fcmpnled);
TRANSW(fcmpodd,         1, 1, 0, 1, 1, 0, D,D,D,0, alf1, _env_, gen_helper_fcmpodd);
TRANSW(fcmpudsf,        1, 1, 0, 1, 1, 0, S,S,S,0, alf1, _env_, gen_helper_fcmpudsf);
TRANSW(fcmpodsf,        1, 1, 0, 1, 1, 0, S,S,S,0, alf1, _env_, gen_helper_fcmpodsf);
TRANSW(fcmpuddf,        1, 1, 0, 1, 1, 0, S,D,D,0, alf1, _env_, gen_helper_fcmpuddf);
TRANSW(fcmpoddf,        1, 1, 0, 1, 1, 0, S,D,D,0, alf1, _env_, gen_helper_fcmpoddf);
TRANSW(pfadds,          1, 1, 4, 1, 1, 4, D,D,D,0, alf1, _env_, gen_helper_pfadds);
TRANSW(pfaddd,          1, 1, 4, 1, 1, 4, D,D,D,0, alf1, _env_, gen_helper_faddd);
TRANSW(pfsubs,          1, 1, 4, 1, 1, 4, D,D,D,0, alf1, _env_, gen_helper_pfsubs);
TRANSW(pfsubd,          1, 1, 4, 1, 1, 4, D,D,D,0, alf1, _env_, gen_helper_fsubd);
TRANSW(pfmins,          1, 1, 0, 1, 1, 0, D,D,D,0, alf1, _env_, gen_helper_pfmins);
TRANSW(pfmind,          1, 1, 0, 1, 1, 0, D,D,D,0, alf1, _env_, gen_helper_fmind);
TRANSW(pfmaxs,          1, 1, 0, 1, 1, 0, D,D,D,0, alf1, _env_, gen_helper_pfmaxs);
TRANSW(pfmaxd,          1, 1, 0, 1, 1, 0, D,D,D,0, alf1, _env_, gen_helper_fmaxd);
TRANSW(pfmuls,          1, 1, 4, 1, 1, 4, D,D,D,0, alf1, _env_, gen_helper_pfmuls);
TRANSW(pfmuld,          1, 1, 0, 1, 1, 0, D,D,D,0, alf1, _env_, gen_helper_fmuld);
TRANSW(pfcmpeqs,        1, 1, 0, 1, 1, 0, D,D,D,0, alf1, _env_, gen_helper_pfcmpeqs);
TRANSW(pfcmplts,        1, 1, 0, 1, 1, 0, D,D,D,0, alf1, _env_, gen_helper_pfcmplts);
TRANSW(pfcmples,        1, 1, 0, 1, 1, 0, D,D,D,0, alf1, _env_, gen_helper_pfcmples);
TRANSW(pfcmpuods,       1, 1, 0, 1, 1, 0, D,D,D,0, alf1, _env_, gen_helper_pfcmpuods);
TRANSW(pfcmpneqs,       1, 1, 0, 1, 1, 0, D,D,D,0, alf1, _env_, gen_helper_pfcmpneqs);
TRANSW(pfcmpnlts,       1, 1, 0, 1, 1, 0, D,D,D,0, alf1, _env_, gen_helper_pfcmpnlts);
TRANSW(pfcmpnles,       1, 1, 0, 1, 1, 0, D,D,D,0, alf1, _env_, gen_helper_pfcmpnles);
TRANSW(pfcmpods,        1, 1, 0, 1, 1, 0, D,D,D,0, alf1, _env_, gen_helper_pfcmpods);
TRANSW(pfcmpeqd,        1, 1, 0, 1, 1, 0, D,D,D,0, alf1, _env_, gen_helper_fcmpeqd);
TRANSW(pfcmpltd,        1, 1, 0, 1, 1, 0, D,D,D,0, alf1, _env_, gen_helper_fcmpltd);
TRANSW(pfcmpled,        1, 1, 0, 1, 1, 0, D,D,D,0, alf1, _env_, gen_helper_fcmpled);
TRANSW(pfcmpuodd,       1, 1, 0, 1, 1, 0, D,D,D,0, alf1, _env_, gen_helper_fcmpuodd);
TRANSW(pfcmpneqd,       1, 1, 0, 1, 1, 0, D,D,D,0, alf1, _env_, gen_helper_fcmpneqd);
TRANSW(pfcmpnltd,       1, 1, 0, 1, 1, 0, D,D,D,0, alf1, _env_, gen_helper_fcmpnltd);
TRANSW(pfcmpnled,       1, 1, 0, 1, 1, 0, D,D,D,0, alf1, _env_, gen_helper_fcmpnled);
TRANSW(pfcmpodd,        1, 1, 0, 1, 1, 0, D,D,D,0, alf1, _env_, gen_helper_fcmpodd);
TRANSW(pandd,           1, 1, 0, 1, 1, 0, D,D,D,0, alf1, _,     tcg_gen_and_i64);
TRANSW(pandnd,          1, 1, 0, 1, 1, 0, D,D,D,0, alf1, _,     gen_andn_i64);
TRANSW(pord,            1, 1, 0, 1, 1, 0, D,D,D,0, alf1, _,     tcg_gen_or_i64);
TRANSW(pxord,           1, 1, 0, 1, 1, 0, D,D,D,0, alf1, _,     tcg_gen_xor_i64);
TRANSW(psrld,           0, 1, 0, 0, 1, 0, D,D,D,0, alf1, _,     gen_psrld);
TRANSW(pslld,           0, 1, 0, 0, 1, 0, D,D,D,0, alf1, _,     gen_pslld);
TRANSW(fdivs,           0, 0, 0, 0, 0, 1, S,S,S,0, alf1, _env_, gen_helper_fdivs);
TRANSW(fdivd,           0, 0, 0, 0, 0, 1, D,D,D,0, alf1, _env_, gen_helper_fdivd);
TRANSW(pfdivs,          0, 0, 0, 0, 0, 1, S,S,S,0, alf1, _env_, gen_helper_fdivs);
TRANSW(pfdivd,          0, 0, 0, 0, 0, 1, D,D,D,0, alf1, _env_, gen_helper_fdivd);
TRANSW(fsqrts,          0, 0, 0, 0, 0, 1, S,0,S,0, alf2, _env_, gen_helper_fsqrts);
TRANSW(fsqrtid,         0, 0, 0, 0, 0, 1, D,0,D,0, alf2, _,     gen_fsqrtid);
TRANSW(pfsqrts,         0, 0, 0, 0, 0, 1, S,0,S,0, alf2, _env_, gen_helper_fsqrts);
TRANSW(frcps,           0, 0, 0, 0, 0, 1, S,0,S,0, alf2, _env_, gen_helper_frcps);
TRANSW(fsqrttd,         0, 0, 0, 0, 0, 1, D,D,D,0, alf1, _env_, gen_helper_fsqrttd);
TRANSW(pfsqrttd,        0, 0, 0, 0, 0, 1, D,D,D,0, alf1, _env_, gen_helper_fsqrttd);
TRANSW(frsqrts,         0, 0, 0, 0, 0, 1, S,0,S,0, alf2, _env_, gen_helper_frsqrts);
TRANSD(rww,             1, 0, 0, 0, 0, 0, 0,0,S,0, alf15, gen_rws);
TRANSD(rwd,             1, 0, 0, 0, 0, 0, 0,0,D,0, alf15, gen_rwd);
TRANSD(rrw,             1, 0, 0, 0, 0, 0, S,0,0,0, alf16, gen_rrs);
TRANSD(rrd,             1, 0, 0, 0, 0, 0, D,0,0,0, alf16, gen_rrd);
TRANSW(getsp,           1, 0, 0, 0, 0, 0, D,0,S,0, alf2, _env_, gen_helper_getsp);
//aptoap,              1, 1, 0, 1, 1, 0, Q,Q,S,0, alf11, AF_PAIR
//getva,               1, 0, 0, 1, 0, 0, D,Q,S,0, alf11
//tdtomp,              1, 1, 0, 1, 1, 0, D,D,D,0, alf11, AF_REMOVED_IN_V3
//odtoap,              1, 1, 0, 1, 1, 0, Q,Q,D,0, alf11, AF_REMOVED_IN_V3
//cast,                1, 1, 0, 1, 1, 0, Q,Q,Q,0, alf11, AF_REMOVED_IN_V3
//gettd,               1, 0, 0, 1, 0, 0, D,0,D,0, alf12, AF_REMOVED_IN_V3
//getsod,              1, 1, 0, 1, 1, 0, Q,0,Q,0, alf12, AF_REMOVED_IN_V3

//andw_fb,             1, 0, 0, 1, 0, 0, S,S,S,0, alf11
//andnw_fb,            1, 0, 0, 1, 0, 0, S,S,S,0, alf11
//orw_fb,              1, 0, 0, 1, 0, 0, S,S,S,0, alf11
//ornw_fb,             1, 0, 0, 1, 0, 0, S,S,S,0, alf11
//xorw_fb,             1, 0, 0, 1, 0, 0, S,S,S,0, alf11
//xornw_fb,            1, 0, 0, 1, 0, 0, S,S,S,0, alf11
//addw_fb,             1, 0, 0, 1, 0, 0, S,S,S,0, alf11
//subw_fb,             1, 0, 0, 1, 0, 0, S,S,S,0, alf11
//sclw_fb,             1, 0, 0, 1, 0, 0, S,S,S,S, alf21
//scrw_fb,             1, 0, 0, 1, 0, 0, S,S,S,S, alf21
//shlw_fb,             1, 0, 0, 1, 0, 0, S,S,S,S, alf21
//shrw_fb,             1, 0, 0, 1, 0, 0, S,S,S,S, alf21
//sarw_fb,             1, 0, 0, 1, 0, 0, S,S,S,S, alf21
//umulx_fb,            1, 0, 0, 1, 0, 0, S,S,S,0, alf11
//smulx_fb,            1, 0, 0, 1, 0, 0, S,S,S,0, alf11
//incw_fb,             1, 0, 0, 1, 0, 0, S,S,S,S, alf21
//decw_fb,             1, 0, 0, 1, 0, 0, S,S,S,S, alf21
//andw_fh,             1, 0, 0, 1, 0, 0, S,S,S,0, alf11
//andnw_fh,            1, 0, 0, 1, 0, 0, S,S,S,0, alf11
//orw_fh,              1, 0, 0, 1, 0, 0, S,S,S,0, alf11
//ornw_fh,             1, 0, 0, 1, 0, 0, S,S,S,0, alf11
//xorw_fh,             1, 0, 0, 1, 0, 0, S,S,S,0, alf11
//xornw_fh,            1, 0, 0, 1, 0, 0, S,S,S,0, alf11
//addw_fh,             1, 0, 0, 1, 0, 0, S,S,S,0, alf11
//subw_fh,             1, 0, 0, 1, 0, 0, S,S,S,0, alf11
//sclw_fh,             1, 0, 0, 1, 0, 0, S,S,S,S, alf21
//scrw_fh,             1, 0, 0, 1, 0, 0, S,S,S,S, alf21
//shlw_fh,             1, 0, 0, 1, 0, 0, S,S,S,S, alf21
//shrw_fh,             1, 0, 0, 1, 0, 0, S,S,S,S, alf21
//sarw_fh,             1, 0, 0, 1, 0, 0, S,S,S,S, alf21
//umulx_fh,            1, 0, 0, 1, 0, 0, S,S,S,0, alf11
//smulx_fh,            1, 0, 0, 1, 0, 0, S,S,S,0, alf11
//incw_fh,             1, 0, 0, 1, 0, 0, S,S,S,S, alf21
//decw_fh,             1, 0, 0, 1, 0, 0, S,S,S,S, alf21
//andw_fw,             1, 0, 0, 1, 0, 0, S,S,S,0, alf11
//andnw_fw,            1, 0, 0, 1, 0, 0, S,S,S,0, alf11
//orw_fw,              1, 0, 0, 1, 0, 0, S,S,S,0, alf11
//ornw_fw,             1, 0, 0, 1, 0, 0, S,S,S,0, alf11
//xorw_fw,             1, 0, 0, 1, 0, 0, S,S,S,0, alf11
//xornw_fw,            1, 0, 0, 1, 0, 0, S,S,S,0, alf11
//addw_fw,             1, 0, 0, 1, 0, 0, S,S,S,0, alf11
//subw_fw,             1, 0, 0, 1, 0, 0, S,S,S,0, alf11
//sclw_fw,             1, 0, 0, 1, 0, 0, S,S,S,S, alf21
//scrw_fw,             1, 0, 0, 1, 0, 0, S,S,S,S, alf21
//shlw_fw,             1, 0, 0, 1, 0, 0, S,S,S,S, alf21
//shrw_fw,             1, 0, 0, 1, 0, 0, S,S,S,S, alf21
//sarw_fw,             1, 0, 0, 1, 0, 0, S,S,S,S, alf21
//umulx_fw,            1, 0, 0, 1, 0, 0, S,S,S,0, alf11
//smulx_fw,            1, 0, 0, 1, 0, 0, S,S,S,0, alf11
//incw_fw,             1, 0, 0, 1, 0, 0, S,S,S,S, alf21
//decw_fw,             1, 0, 0, 1, 0, 0, S,S,S,S, alf21

#define TRANS_ICOMB(NAME, C0, C1, C2, C3, C4, C5, OP2) \
    TRANSC(glue3(and_,   NAME, w), C0, C1, C2, C3, C4, C5, S,S,S,S, alf21, gen_icomb_i32, ICOMB_AND,   OP2); \
    TRANSC(glue3(and_,   NAME, d), C0, C1, C2, C3, C4, C5, D,D,D,D, alf21, gen_icomb_i64, ICOMB_AND,   OP2); \
    TRANSC(glue3(andn_,  NAME, w), C0, C1, C2, C3, C4, C5, S,S,S,S, alf21, gen_icomb_i32, ICOMB_ANDN,  OP2); \
    TRANSC(glue3(andn_,  NAME, d), C0, C1, C2, C3, C4, C5, D,D,D,D, alf21, gen_icomb_i64, ICOMB_ANDN,  OP2); \
    TRANSC(glue3(or_,    NAME, w), C0, C1, C2, C3, C4, C5, S,S,S,S, alf21, gen_icomb_i32, ICOMB_OR,    OP2); \
    TRANSC(glue3(or_,    NAME, d), C0, C1, C2, C3, C4, C5, D,D,D,D, alf21, gen_icomb_i64, ICOMB_OR,    OP2); \
    TRANSC(glue3(orn_,   NAME, w), C0, C1, C2, C3, C4, C5, S,S,S,S, alf21, gen_icomb_i32, ICOMB_ORN,   OP2); \
    TRANSC(glue3(orn_,   NAME, d), C0, C1, C2, C3, C4, C5, D,D,D,D, alf21, gen_icomb_i64, ICOMB_ORN,   OP2); \
    TRANSC(glue3(xor_,   NAME, w), C0, C1, C2, C3, C4, C5, S,S,S,S, alf21, gen_icomb_i32, ICOMB_XOR,   OP2); \
    TRANSC(glue3(xor_,   NAME, d), C0, C1, C2, C3, C4, C5, D,D,D,D, alf21, gen_icomb_i64, ICOMB_XOR,   OP2); \
    TRANSC(glue3(xorn_,  NAME, w), C0, C1, C2, C3, C4, C5, S,S,S,S, alf21, gen_icomb_i32, ICOMB_XORN,  OP2); \
    TRANSC(glue3(xorn_,  NAME, d), C0, C1, C2, C3, C4, C5, D,D,D,D, alf21, gen_icomb_i64, ICOMB_XORN,  OP2); \
    TRANSC(glue3(merge_, NAME, w), C0, C1, C2, C3, C4, C5, S,S,S,S, alf21, gen_icomb_i32, ICOMB_MERGE, OP2); \
    TRANSC(glue3(merge_, NAME, d), C0, C1, C2, C3, C4, C5, D,D,D,D, alf21, gen_icomb_i64, ICOMB_MERGE, OP2); \
    TRANSC(glue3(add_,   NAME, w), C0, C1, C2, C3, C4, C5, S,S,S,S, alf21, gen_icomb_i32, ICOMB_ADD,   OP2); \
    TRANSC(glue3(add_,   NAME, d), C0, C1, C2, C3, C4, C5, D,D,D,D, alf21, gen_icomb_i64, ICOMB_ADD,   OP2); \
    TRANSC(glue3(sub_,   NAME, w), C0, C1, C2, C3, C4, C5, S,S,S,S, alf21, gen_icomb_i32, ICOMB_SUB,   OP2); \
    TRANSC(glue3(sub_,   NAME, d), C0, C1, C2, C3, C4, C5, D,D,D,D, alf21, gen_icomb_i64, ICOMB_SUB,   OP2); \
    TRANSC(glue3(scl_,   NAME, w), C0, C1, C2, C3, C4, C5, S,S,S,S, alf21, gen_icomb_i32, ICOMB_SCL,   OP2); \
    TRANSC(glue3(scl_,   NAME, d), C0, C1, C2, C3, C4, C5, D,D,D,D, alf21, gen_icomb_i64, ICOMB_SCL,   OP2); \
    TRANSC(glue3(scr_,   NAME, w), C0, C1, C2, C3, C4, C5, S,S,S,S, alf21, gen_icomb_i32, ICOMB_SCR,   OP2); \
    TRANSC(glue3(scr_,   NAME, d), C0, C1, C2, C3, C4, C5, D,D,D,D, alf21, gen_icomb_i64, ICOMB_SCR,   OP2); \
    TRANSC(glue3(shl_,   NAME, w), C0, C1, C2, C3, C4, C5, S,S,S,S, alf21, gen_icomb_i32, ICOMB_SHL,   OP2); \
    TRANSC(glue3(shl_,   NAME, d), C0, C1, C2, C3, C4, C5, D,D,D,D, alf21, gen_icomb_i64, ICOMB_SHL,   OP2); \
    TRANSC(glue3(shr_,   NAME, w), C0, C1, C2, C3, C4, C5, S,S,S,S, alf21, gen_icomb_i32, ICOMB_SHR,   OP2); \
    TRANSC(glue3(shr_,   NAME, d), C0, C1, C2, C3, C4, C5, D,D,D,D, alf21, gen_icomb_i64, ICOMB_SHR,   OP2); \
    TRANSC(glue3(sar_,   NAME, w), C0, C1, C2, C3, C4, C5, S,S,S,S, alf21, gen_icomb_i32, ICOMB_SAR,   OP2); \
    TRANSC(glue3(sar_,   NAME, d), C0, C1, C2, C3, C4, C5, D,D,D,D, alf21, gen_icomb_i64, ICOMB_SAR,   OP2); \
    TRANSC(glue3(getf_,  NAME, w), C0, C1, C2, C3, C4, C5, S,S,S,S, alf21, gen_icomb_i32, ICOMB_GETF,  OP2); \
    TRANSC(glue3(getf_,  NAME, d), C0, C1, C2, C3, C4, C5, D,D,D,D, alf21, gen_icomb_i64, ICOMB_GETF,  OP2)

TRANS_ICOMB(and,    0, 1, 0, 0, 1, 0, ICOMB_AND);
TRANS_ICOMB(andn,   0, 1, 0, 0, 1, 0, ICOMB_ANDN);
TRANS_ICOMB(or,     0, 1, 0, 0, 1, 0, ICOMB_OR);
TRANS_ICOMB(orn,    0, 1, 0, 0, 1, 0, ICOMB_ORN);
TRANS_ICOMB(xor,    0, 1, 0, 0, 1, 0, ICOMB_XOR);
TRANS_ICOMB(xorn,   0, 1, 0, 0, 1, 0, ICOMB_XORN);
TRANS_ICOMB(rsub,   0, 1, 0, 0, 1, 0, ICOMB_RSUB);
TRANS_ICOMB(add,    0, 1, 0, 0, 1, 0, ICOMB_ADD);
TRANS_ICOMB(sub,    0, 1, 0, 0, 1, 0, ICOMB_SUB);

TRANS_ICOMB(merge,  0, V1_2, 0, 0, V1_2, 0, ICOMB_MERGE);
TRANS_ICOMB(scl,    0, V1_2, 0, 0, V1_2, 0, ICOMB_SCL);
TRANS_ICOMB(scr,    0, V1_2, 0, 0, V1_2, 0, ICOMB_SCR);
TRANS_ICOMB(shl,    0, V1_2, 0, 0, V1_2, 0, ICOMB_SHL);
TRANS_ICOMB(shr,    0, V1_2, 0, 0, V1_2, 0, ICOMB_SHR);
TRANS_ICOMB(sar,    0, V1_2, 0, 0, V1_2, 0, ICOMB_SAR);
TRANS_ICOMB(getf,   0, V1_2, 0, 0, V1_2, 0, ICOMB_GETF);

TRANSC(fmul_adds,   1, 1, 4, 1, 1, 4, S,S,S,S, alf21, gen_fcomb_i32, FCOMB_MUL, FCOMB_ADD);
TRANSC(fmul_addd,   1, 1, 4, 1, 1, 4, D,D,D,D, alf21, gen_fcomb_i64, FCOMB_MUL, FCOMB_ADD);
TRANSC(fmul_subs,   1, 1, 4, 1, 1, 4, S,S,S,S, alf21, gen_fcomb_i32, FCOMB_MUL, FCOMB_SUB);
TRANSC(fmul_subd,   1, 1, 4, 1, 1, 4, D,D,D,D, alf21, gen_fcomb_i64, FCOMB_MUL, FCOMB_SUB);
TRANSC(fmul_rsubs,  1, 1, 4, 1, 1, 4, S,S,S,S, alf21, gen_fcomb_i32, FCOMB_MUL, FCOMB_RSUB);
TRANSC(fmul_rsubd,  1, 1, 4, 1, 1, 4, D,D,D,D, alf21, gen_fcomb_i64, FCOMB_MUL, FCOMB_RSUB);

TRANSC(pfmul_adds,  1, 1, 4, 1, 1, 4, D,D,D,D, alf21, gen_pfcomb_i32, FCOMB_MUL, FCOMB_ADD);
TRANSC(pfmul_addd,  1, 1, 4, 1, 1, 4, D,D,D,D, alf21, gen_pfcomb_i64, FCOMB_MUL, FCOMB_ADD);
TRANSC(pfmul_subs,  1, 1, 4, 1, 1, 4, D,D,D,D, alf21, gen_pfcomb_i32, FCOMB_MUL, FCOMB_SUB);
TRANSC(pfmul_subd,  1, 1, 4, 1, 1, 4, D,D,D,D, alf21, gen_pfcomb_i64, FCOMB_MUL, FCOMB_SUB);
TRANSC(pfmul_rsubs, 1, 1, 4, 1, 1, 4, D,D,D,D, alf21, gen_pfcomb_i32, FCOMB_MUL, FCOMB_RSUB);
TRANSC(pfmul_rsubd, 1, 1, 4, 1, 1, 4, D,D,D,D, alf21, gen_pfcomb_i64, FCOMB_MUL, FCOMB_RSUB);

TRANSC(fadd_muls,   V1_2, V1_2, 0, V1_2, V1_2, 0, S,S,S,S, alf21, gen_fcomb_i32, FCOMB_ADD, FCOMB_MUL);
TRANSC(fadd_muld,   V1_2, V1_2, 0, V1_2, V1_2, 0, D,D,D,D, alf21, gen_fcomb_i64, FCOMB_ADD, FCOMB_MUL);
TRANSC(fsub_muls,   V1_2, V1_2, 0, V1_2, V1_2, 0, S,S,S,S, alf21, gen_fcomb_i32, FCOMB_SUB, FCOMB_MUL);
TRANSC(fsub_muld,   V1_2, V1_2, 0, V1_2, V1_2, 0, D,D,D,D, alf21, gen_fcomb_i64, FCOMB_SUB, FCOMB_MUL);

TRANSC(pfadd_muls,  V1_2, V1_2, 0, V1_2, V1_2, 0, D,D,D,D, alf21, gen_pfcomb_i32, FCOMB_ADD, FCOMB_MUL);
TRANSC(pfadd_muld,  V1_2, V1_2, 0, V1_2, V1_2, 0, D,D,D,D, alf21, gen_pfcomb_i64, FCOMB_ADD, FCOMB_MUL);
TRANSC(pfsub_muls,  V1_2, V1_2, 0, V1_2, V1_2, 0, D,D,D,D, alf21, gen_pfcomb_i32, FCOMB_SUB, FCOMB_MUL);
TRANSC(pfsub_muld,  V1_2, V1_2, 0, V1_2, V1_2, 0, D,D,D,D, alf21, gen_pfcomb_i64, FCOMB_SUB, FCOMB_MUL);
