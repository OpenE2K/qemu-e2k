#include "qemu/osdep.h"
#include "qemu/log.h"
#include "cpu.h"
#include "helper-tcg.h"
#include "exec/exec-all.h"
#include "qemu/host-utils.h"
#include "exec/helper-proto.h"

static FloatRoundMode rm[] = {
    float_round_nearest_even,
    float_round_down,
    float_round_up,
    float_round_to_zero,
};

static inline int fpu_flags(int flags)
{
    return (flags & float_flag_invalid          ? FP_IE : 0) |
           (flags & float_flag_divbyzero        ? FP_ZE : 0) |
           (flags & float_flag_overflow         ? FP_OE : 0) |
           (flags & float_flag_underflow        ? FP_UE : 0) |
           (flags & float_flag_inexact          ? FP_PE : 0) |
           (flags & float_flag_input_denormal   ? FP_DE : 0);
}

static inline void fx_merge_exception_flags(CPUE2KState *env, int old_flags)
{
    int new_flags = get_float_exception_flags(&env->fx_status);
    float_raise(old_flags, &env->fx_status);
    env->fpsr.ef |= fpu_flags(new_flags);
    if (env->fpsr.ef & (~env->fpcr.em & FP_EM)) {
        env->fpsr.es = 1;
        env->fpsr.b  = 1;
    }
}

static inline void fp_merge_exception_flags(CPUE2KState *env, int old_flags)
{
    int new_flags = get_float_exception_flags(&env->fp_status);
    float_raise(old_flags, &env->fp_status);
    env->pfpfr.ef |= fpu_flags(new_flags);
}

static inline int save_exception_flags(float_status *s)
{
    int old_flags = get_float_exception_flags(s);
    set_float_exception_flags(0, s);
    return old_flags;
}

#define fx_save_exception_flags(e) save_exception_flags(&(e)->fx_status)
#define fp_save_exception_flags(e) save_exception_flags(&(e)->fp_status)

void e2k_update_fx_status(CPUE2KState *env)
{
    int x;
    
    set_float_rounding_mode(rm[env->fpcr.rc], &env->fx_status);
        
    switch(env->fpcr.pc) {
        case FPCR_PC_XP: x = floatx80_precision_x; break;
        case FPCR_PC_DP: x = floatx80_precision_d; break;
        case FPCR_PC_SP: x = floatx80_precision_s; break;
        case FPCR_PC_RESERVED:
        default:
            qemu_log_mask(LOG_UNIMP, "unknown precision mode 0x%x\n", env->fpcr.pc);
            abort();
    }
    
    set_floatx80_rounding_precision(x, &env->fx_status);
}

void e2k_update_fp_status(CPUE2KState *env)
{
    set_float_rounding_mode(rm[env->pfpfr.rc], &env->fp_status);
}

#define TOIF_RC_CURRENT        0x4
#define TOIF_RC_IGNORE_INEXACT 0x8

static inline void toif_set_round_mode(uint32_t flags, float_status *s)
{
    if ((flags & TOIF_RC_CURRENT) == 0) {
        set_float_rounding_mode(rm[flags & FP_RC_CHOP], s);
    }
}

static inline void toif_clear_inexact(uint32_t flags, float_status *s)
{
    if(flags & TOIF_RC_IGNORE_INEXACT) {
        int new_flags = get_float_exception_flags(s);
        new_flags = new_flags & (~float_flag_inexact);
        set_float_exception_flags(new_flags, s);
    }
}

#ifdef TARGET_E2K_PRECISE_FSQRTID
uint64 HELPER(fsqrtid)(CPUE2KState *env, uint64_t x)
{
#error implement
    return x;
}
#endif

#define IMPL_FSCALE(name, ty, exp_len, exp_off, mul, cvt) \
    ty HELPER(name)(CPUE2KState *env, ty src1, uint32_t src2) \
    { \
        int32_t p = (int32_t) src2; \
        ty max = (1 << exp_len) - 1; \
        int32_t bias = max >> 1; \
        ty exp; \
        ty m = 0; \
        if (p <= -bias) { \
            exp = 0; \
            p += bias + exp_off - 1; \
            if (p >= 0 && p < exp_off) { \
                m = 1ULL << p; \
            } else { \
                m = p == -1; \
            } \
        } else if (p > bias) { \
            exp = max; \
        } else { \
            exp = bias + p; \
        } \
        ty s2 = (exp << exp_off) | m; \
        return mul(env, src1, s2); \
    }

IMPL_FSCALE(fscaled, uint64_t, 11, 52, helper_fmuld, uint64_to_float64)
IMPL_FSCALE(fscales, uint32_t, 8, 23, helper_fmuls, uint32_to_float32)

void HELPER(fxscalesx)(floatx80 *r, CPUE2KState *env, floatx80 *a, uint32_t b)
{
    floatx80 v;
    int32_t p = (int32_t) b;
    uint16_t max = (1 << 15) - 1;
    int16_t bias = max >> 1;
    if (p <= -bias) {
        v.high = 0;
        p += bias + 62;
        if (p >= 0 && p < 63) {
            v.low = 1ULL << p;
        } else {
            v.low = p == -1;
        }
    } else if (p > bias) {
        v.low = 1UL << 63;
        v.high = max;
    } else {
        v.low = 1UL << 63;
        v.high = bias + p;
    }
    helper_fxmulxx(r, env, a, &v);
}

#define type_name_i32 int32
#define type_name_i64 int64
#define type_name_f32 float32
#define type_name_f64 float64
#define type_name_f80 floatx80
#define type_name(S) glue(type_name_, S)

#define arg_type_i32 uint32_t
#define arg_type_i64 uint64_t
#define arg_type_f32 uint32_t
#define arg_type_f64 uint64_t
#define arg_type_f80 floatx80 *
#define arg_type(S) glue(arg_type_, S)

#define ret_arg_i32
#define ret_arg_i64
#define ret_arg_f32
#define ret_arg_f64
#define ret_arg_f80 floatx80 *ret,
#define ret_arg(S) glue(ret_arg_, S)

#define ret_type_i32 uint32_t
#define ret_type_i64 uint64_t
#define ret_type_f32 uint32_t
#define ret_type_f64 uint64_t
#define ret_type_f80 void
#define ret_type(S) glue(ret_type_, S)

#define make_i32(v) (v)
#define make_i64(v) (v)
#define make_f32(v) make_float32(v)
#define make_f64(v) make_float64(v)
#define make_f80(v) (*(v))
#define make(S, v) glue(make_, S)(v)

#define float_val_f32(v) float32_val(v)
#define float_val_f64(v) float64_val(v)
#define float_val(S, v) glue(float_val_, S)(v)

#define type_i32 uint32_t
#define type_i64 uint64_t
#define type_f32 float32
#define type_f64 float64
#define type_f80 floatx80
#define type(S) glue(type_, S)

#define int32_to_int32(v, s) (v)
#define int64_to_int64(v, s) (v)
#define float32_to_float32(v, s) (v)
#define float64_to_float64(v, s) (v)
#define floatx80_to_floatx80(v, s) (v)
#define convert(F, T, v, s) glue3(type_name(F), _to_, type_name(T))(v, s)

#define ret_i32(v) return (v)
#define ret_i64(v) return (v)
#define ret_f32(v) return float32_val(v)
#define ret_f64(v) return float64_val(v)
#define ret_f80(v) *ret = v
#define ret(S, v) glue(ret_, S)(v)

#define fpu_mov(x, s) (x)
#define ident(x) (x)
#define not(x) (!(x))

#define IMPL_ALOPF2_FPU_BASIC(FPU, name, S2, R, T2, TR, op) \
    ret_type(R) HELPER(name)(ret_arg(R) CPUE2KState *env, arg_type(S2) s2) \
    { \
        int old_flags = glue(FPU, _save_exception_flags)(env); \
        float_status *s = &env->glue(FPU, _status); \
        type(T2) t2 = convert(S2, T2, make(S2, s2), s); \
        type(TR) tr = op(t2, s); \
        type(R) r = convert(TR, R, tr, s); \
        glue(FPU, _merge_exception_flags)(env, old_flags); \
        ret(R, r); \
    }

#define IMPL_ALOPF2_FPU(FPU, name, S2, R, op) \
    IMPL_ALOPF2_FPU_BASIC(FPU, name, S2, R, S2, R, op)

#define IMPL_ALOPF2_FP(name, S2, R, op) \
    IMPL_ALOPF2_FPU_BASIC(fp, name, R, S2, R, S2, op)

#define IMPL_ALOPF2_FPU_CVT_OP(FPU, name, S2, R, op) \
    ret_type(R) HELPER(name)(ret_arg(R) CPUE2KState *env, arg_type(S2) s2) \
    { \
        int old_flags = glue(FPU, _save_exception_flags)(env); \
        float_status *s = &env->glue(FPU, _status); \
        type(R) r = op(make(S2, s2), s); \
        glue(FPU, _merge_exception_flags)(env, old_flags); \
        ret(R, r); \
    }

#define IMPL_ALOPF2_FPU_CVT(FPU, name, S2, R) \
    ret_type(R) HELPER(name)(ret_arg(R) CPUE2KState *env, arg_type(S2) s2) \
    { \
        int old_flags = glue(FPU, _save_exception_flags)(env); \
        float_status *s = &env->glue(FPU, _status); \
        type(R) r = convert(S2, R, make(S2, s2), s); \
        glue(FPU, _merge_exception_flags)(env, old_flags); \
        ret(R, r); \
    }

#define IMPL_ALOPF1_FPU(FPU, name, S1, S2, R, T1, T2, TR, op) \
    ret_type(R) HELPER(name)(ret_arg(R) CPUE2KState *env, \
        arg_type(S1) s1, arg_type(S2) s2) \
    { \
        int old_flags = glue(FPU, _save_exception_flags)(env); \
        float_status *s = &env->glue(FPU, _status); \
        type(T1) t1 = convert(S1, T1, make(S1, s1), s); \
        type(T2) t2 = convert(S2, T2, make(S2, s2), s); \
        type(TR) tr = op(t1, t2, s); \
        type(R) r = convert(TR, R, tr, s); \
        glue(FPU, _merge_exception_flags)(env, old_flags); \
        ret(R, r); \
    }

#define IMPL_ALOPF1_FX(name, S1, S2, R, op) \
    IMPL_ALOPF1_FPU(fx, name, S1, S2, R, f80, f80, f80, op)

#define IMPL_ALOPF1_FP(name, S1, S2, R, op) \
    IMPL_ALOPF1_FPU(fp, name, S1, S2, R, S1, S2, R, op)

#define IMPL_ALOPF1_FPU_CMP_BASIC(FPU, name, S1, S2, R, T1, T2, TR, op1, op2) \
    ret_type(R) HELPER(name)(ret_arg(R) CPUE2KState *env, \
        arg_type(S1) s1, arg_type(S2) s2) \
    { \
        int old_flags = glue(FPU, _save_exception_flags)(env); \
        float_status *s = &env->glue(FPU, _status); \
        type(T1) t1 = convert(S1, T1, make(S1, s1), s); \
        type(T2) t2 = convert(S2, T2, make(S2, s2), s); \
        type(TR) tr = op2(op1(t1, t2, s)) ? -1 : 0; \
        type(R) r = convert(TR, R, tr, s); \
        glue(FPU, _merge_exception_flags)(env, old_flags); \
        ret(R, r); \
    }

#define IMPL_ALOPF1_FPU_CMP(FPU, name, S1, S2, R, T, op1, op2) \
    IMPL_ALOPF1_FPU_CMP_BASIC(FPU, name, S1, S2, R, T, T, R, op1, op2)

#define IMPL_ALOPF1_FP_S(name, S1, S2, R, op) \
    ret_type(R) HELPER(name)(CPUE2KState *env, arg_type(S1) s1, \
        arg_type(S2) s2) \
    { \
        int old_flags = fp_save_exception_flags(env); \
        type(R) r = op(R, make(S1, s1), make(S2, s2), &env->fp_status); \
        fp_merge_exception_flags(env, old_flags); \
        return float_val(R, r); \
    }

#define IMPL_ALOPF21_FPU(FPU, name, S1, S2, S3, R, T1, T2, T3, TR, op) \
    ret_type(R) HELPER(name)(ret_arg(R) CPUE2KState *env, \
        arg_type(S1) s1, arg_type(S2) s2, arg_type(S3) s3) \
    { \
        int old_flags = glue(FPU, _save_exception_flags)(env); \
        float_status *s = &env->glue(FPU, _status); \
        type(T1) t1 = convert(S1, T1, make(S1, s1), s); \
        type(T2) t2 = convert(S2, T2, make(S2, s2), s); \
        type(T3) t3 = convert(S3, T3, make(S3, s3), s); \
        type(TR) tr = op(t1, t2, t3, s); \
        type(R) r = convert(TR, R, tr, s); \
        glue(FPU, _merge_exception_flags)(env, old_flags); \
        ret(R, r); \
    }

#define IMPL_ALOPF21_FP(name, T, op) \
    IMPL_ALOPF21_FPU(fp, name, T, T, T, T, T, T, T, T, op)

#define IMPL_ALOPF1_FX_MANY(name, op) \
    IMPL_ALOPF1_FX(glue3(fx, name, ss), f80, f32, f32, op) \
    IMPL_ALOPF1_FX(glue3(fx, name, dd), f80, f64, f64, op) \
    IMPL_ALOPF1_FX(glue3(fx, name, sx), f80, f32, f80, op) \
    IMPL_ALOPF1_FX(glue3(fx, name, dx), f80, f64, f80, op) \
    IMPL_ALOPF1_FX(glue3(fx, name, xx), f80, f80, f80, op) \
    IMPL_ALOPF1_FX(glue3(fx, name, xd), f80, f80, f64, op) \
    IMPL_ALOPF1_FX(glue3(fx, name, xs), f80, f80, f32, op)

IMPL_ALOPF1_FX_MANY(add, floatx80_add)
IMPL_ALOPF1_FX_MANY(sub, floatx80_sub)
IMPL_ALOPF1_FX_MANY(mul, floatx80_mul)
IMPL_ALOPF1_FX_MANY(div, floatx80_div)

#define floatx80_rsub(x, y, s) floatx80_sub(y, x, s)

IMPL_ALOPF1_FX(fxrsubss, f80, f32, f32, floatx80_rsub)
IMPL_ALOPF1_FX(fxrsubdd, f80, f64, f64, floatx80_rsub)
IMPL_ALOPF1_FX(fxrsubsx, f80, f32, f80, floatx80_rsub)
IMPL_ALOPF1_FX(fxrsubdx, f80, f64, f80, floatx80_rsub)

IMPL_ALOPF1_FX(fxdivtss, f32, f80, f32, floatx80_div)
IMPL_ALOPF1_FX(fxdivtdd, f64, f80, f64, floatx80_div)
IMPL_ALOPF1_FX(fxdivtsx, f32, f80, f80, floatx80_div)
IMPL_ALOPF1_FX(fxdivtdx, f64, f80, f80, floatx80_div)

static floatx80 fxsqrtt(floatx80 a, floatx80 b, float_status *s)
{
    // TODO: fxsqrtt
    return floatx80_sqrt(a, s);
}

IMPL_ALOPF1_FX(fxsqrttsx, f32, f80, f80, fxsqrtt)
IMPL_ALOPF1_FX(fxsqrttdx, f64, f80, f80, fxsqrtt)
IMPL_ALOPF1_FX(fxsqrttxx, f80, f80, f80, fxsqrtt)

#define CMPODF_BASIC(p, x, y, s) ({ \
    uint32_t ret = 0x45; \
    if (!glue(p, _is_any_nan)(x) && !glue(p, _is_any_nan)(y)) { \
        FloatRelation relation = glue(p, _compare_quiet)(x, y, s); \
        switch (relation) { \
        case float_relation_greater: ret = 0x00; break; \
        case float_relation_less: ret = 0x01; break; \
        case float_relation_equal: ret = 0x40; break; \
        case float_relation_unordered: ret = 0x45; break; \
        } \
    } \
    ret;\
})

#define FXCMPODF(x, y, s) CMPODF_BASIC(floatx80, x, y, s)
#define FCMPODDF(x, y, s) CMPODF_BASIC(float64, x, y, s)
#define FCMPODSF(x, y, s) CMPODF_BASIC(float32, x, y, s)
// TODO: cmpudf
#define FXCMPUDF FXCMPODF
#define FCMPUDSF FCMPODSF
#define FCMPUDDF FCMPODDF

#define IMPL_CMPF_FX(name, S1, S2, op) \
    IMPL_ALOPF1_FPU(fx, name, S1, S2, i32, f80, f80, i32, op)

IMPL_CMPF_FX(fxcmpodsf, f80, f32, FXCMPODF)
IMPL_CMPF_FX(fxcmpoddf, f80, f64, FXCMPODF)
IMPL_CMPF_FX(fxcmpodxf, f80, f80, FXCMPODF)

IMPL_CMPF_FX(fxcmpudsf, f80, f32, FXCMPUDF)
IMPL_CMPF_FX(fxcmpuddf, f80, f64, FXCMPUDF)
IMPL_CMPF_FX(fxcmpudxf, f80, f80, FXCMPUDF)

#define IMPL_CMPF_FP(name, T, R, op) \
    IMPL_ALOPF1_FPU(fp, name, T, T, R, T, T, R, op)

IMPL_CMPF_FP(fcmpodsf, f32, i32, FCMPODSF)
IMPL_CMPF_FP(fcmpoddf, f64, i32, FCMPODDF)

IMPL_CMPF_FP(fcmpudsf, f32, i32, FCMPUDSF)
IMPL_CMPF_FP(fcmpuddf, f64, i32, FCMPUDDF)

#define cmp_op(T, op) glue3(type_name(T), _, op)

#define IMPL_ALOPF1_FPU_CMP_ALL(F, P, S, T, R, A, B) \
    IMPL_ALOPF1_FPU_CMP(F, glue3(P, cmpeq,  S), A, B, R, T, cmp_op(T, eq), ident) \
    IMPL_ALOPF1_FPU_CMP(F, glue3(P, cmpneq, S), A, B, R, T, cmp_op(T, eq), not) \
    IMPL_ALOPF1_FPU_CMP(F, glue3(P, cmple,  S), A, B, R, T, cmp_op(T, le), ident) \
    IMPL_ALOPF1_FPU_CMP(F, glue3(P, cmpnle, S), A, B, R, T, cmp_op(T, le), not) \
    IMPL_ALOPF1_FPU_CMP(F, glue3(P, cmplt,  S), A, B, R, T, cmp_op(T, lt), ident) \
    IMPL_ALOPF1_FPU_CMP(F, glue3(P, cmpnlt, S), A, B, R, T, cmp_op(T, lt), not) \
    IMPL_ALOPF1_FPU_CMP(F, glue3(P, cmpuod, S), A, B, R, T, cmp_op(T, unordered), ident) \
    IMPL_ALOPF1_FPU_CMP(F, glue3(P, cmpod,  S), A, B, R, T, cmp_op(T, unordered), not)

IMPL_ALOPF1_FPU_CMP_ALL(fp, f,  s, f32, i32, f32, f32)
IMPL_ALOPF1_FPU_CMP_ALL(fp, f,  d, f64, i64, f64, f64)
IMPL_ALOPF1_FPU_CMP_ALL(fx, fx, s, f80, i64, f80, f32)
IMPL_ALOPF1_FPU_CMP_ALL(fx, fx, d, f80, i64, f80, f64)
IMPL_ALOPF1_FPU_CMP_ALL(fx, fx, x, f80, i64, f80, f80)

IMPL_ALOPF2_FPU_CVT(fp, fstofd, f32, f64)
IMPL_ALOPF2_FPU_CVT(fx, fstofx, f32, f80)
IMPL_ALOPF2_FPU_CVT(fp, fdtofs, f64, f32)
IMPL_ALOPF2_FPU_CVT(fx, fdtofx, f64, f80)
IMPL_ALOPF2_FPU_CVT(fx, fxtofs, f80, f32)
IMPL_ALOPF2_FPU_CVT(fx, fxtofd, f80, f64)

IMPL_ALOPF2_FPU_CVT(fp, istofs, i32, f32)
IMPL_ALOPF2_FPU_CVT(fp, istofd, i32, f64)
IMPL_ALOPF2_FPU_CVT(fx, istofx, i32, f80)

IMPL_ALOPF2_FPU_CVT(fp, idtofs, i64, f32)
IMPL_ALOPF2_FPU_CVT(fp, idtofd, i64, f64)
IMPL_ALOPF2_FPU_CVT(fx, idtofx, i64, f80)

/*
 * I assume e2k behaves like x86.
 *
 * e2k/x86 mandates that we return the indefinite integer value for the result
 * of any float-to-integer conversion that raises the 'invalid' exception.
 * Wrap the softfloat functions to get this behaviour.
 */
#define WRAP_F2I(RET, func, T, defval) \
    static RET glue(e2k_, func)(T x, float_status *s) \
    { \
        int oldflags, newflags; \
        RET r; \
        \
        oldflags = get_float_exception_flags(s); \
        set_float_exception_flags(0, s); \
        r = func(x, s); \
        newflags = get_float_exception_flags(s); \
        if (newflags & float_flag_invalid) { \
            r = defval; \
        } \
        set_float_exception_flags(newflags | oldflags, s); \
        return r; \
    }

WRAP_F2I(int32_t, float32_to_int32,                float32,  INT32_MIN)
WRAP_F2I(int32_t, float32_to_int32_round_to_zero,  float32,  INT32_MIN)
WRAP_F2I(int64_t, float32_to_int64,                float32,  INT64_MIN)
WRAP_F2I(int64_t, float32_to_int64_round_to_zero,  float32,  INT64_MIN)
WRAP_F2I(int32_t, float64_to_int32,                float64,  INT32_MIN)
WRAP_F2I(int32_t, float64_to_int32_round_to_zero,  float64,  INT32_MIN)
WRAP_F2I(int64_t, float64_to_int64,                float64,  INT64_MIN)
WRAP_F2I(int64_t, float64_to_int64_round_to_zero,  float64,  INT64_MIN)
WRAP_F2I(int32_t, floatx80_to_int32,               floatx80, INT32_MIN)
WRAP_F2I(int32_t, floatx80_to_int32_round_to_zero, floatx80, INT32_MIN)
WRAP_F2I(int64_t, floatx80_to_int64,               floatx80, INT64_MIN)
WRAP_F2I(int64_t, floatx80_to_int64_round_to_zero, floatx80, INT64_MIN)

IMPL_ALOPF2_FPU_CVT_OP(fp, fstois, f32, i32, e2k_float32_to_int32)
IMPL_ALOPF2_FPU_CVT_OP(fp, fdtois, f64, i32, e2k_float64_to_int32)
IMPL_ALOPF2_FPU_CVT_OP(fx, fxtois, f80, i32, e2k_floatx80_to_int32)
IMPL_ALOPF2_FPU_CVT_OP(fp, fstoid, f32, i64, e2k_float32_to_int64)
IMPL_ALOPF2_FPU_CVT_OP(fp, fdtoid, f64, i64, e2k_float64_to_int64)
IMPL_ALOPF2_FPU_CVT_OP(fx, fxtoid, f80, i64, e2k_floatx80_to_int64)

IMPL_ALOPF2_FPU_CVT_OP(fp, fstoistr, f32, i32, e2k_float32_to_int32_round_to_zero)
IMPL_ALOPF2_FPU_CVT_OP(fp, fstoidtr, f32, i64, e2k_float32_to_int64_round_to_zero)
IMPL_ALOPF2_FPU_CVT_OP(fp, fdtoistr, f64, i32, e2k_float64_to_int32_round_to_zero)
IMPL_ALOPF2_FPU_CVT_OP(fp, fdtoidtr, f64, i64, e2k_float64_to_int64_round_to_zero)
IMPL_ALOPF2_FPU_CVT_OP(fx, fxtoistr, f80, i32, e2k_floatx80_to_int32_round_to_zero)
IMPL_ALOPF2_FPU_CVT_OP(fx, fxtoidtr, f80, i64, e2k_floatx80_to_int64_round_to_zero)

#define IMPL_FTOIF(name, A, B) \
    static type(B) name(type(A) flags, type(B) f, float_status *s) \
    { \
        FloatRoundMode oldrm = get_float_rounding_mode(s); \
        type(B) ret; \
        \
        toif_set_round_mode(flags, s); \
        ret = glue(type_name(B), _round_to_int)(f, s); \
        set_float_rounding_mode(oldrm, s); \
        toif_clear_inexact(flags, s); \
        return ret; \
    }

IMPL_FTOIF(fstoifs, i32, f32)
IMPL_FTOIF(fdtoifd, i64, f64)

IMPL_ALOPF1_FP(fstoifs, i32, f32, f32, fstoifs)
IMPL_ALOPF1_FP(fdtoifd, i64, f64, f64, fdtoifd)

/* TODO: frcps: test if valid, test exception flags */
#define frcps(a, s) float32_div(float32_one, a, s)
#define frsqrts(a, s) float32_div(float32_one, float32_sqrt(a, s), s)

static float64 fsqrttd(float64 a, float64 b, float_status *s)
{
    // TODO: fxsqrtt
    return float64_sqrt(a, s);
}

/*
 * I assume e2k behaves like x86.
 *
 * Note that the choice of comparison op here is important to get the
 * special cases right: for min and max Intel specifies that (-0,0),
 * (NaN, anything) and (anything, NaN) return the second argument.
 */
#define FPU_MIN(T, a, b, s) \
    (glue(type_name(T), _lt)(a, b, s) ? (a) : (b))
#define FPU_MAX(T, a, b, s) \
    (glue(type_name(T), _lt)(b, a, s) ? (a) : (b))

#define IMPL_ALOPF1_FP_MANY(S, T) \
    IMPL_ALOPF1_FP(glue(fadd, S), T, T, T, glue(type_name(T), _add)) \
    IMPL_ALOPF1_FP(glue(fsub, S), T, T, T, glue(type_name(T), _sub)) \
    IMPL_ALOPF1_FP(glue(fmul, S), T, T, T, glue(type_name(T), _mul)) \
    IMPL_ALOPF1_FP(glue(fdiv, S), T, T, T, glue(type_name(T), _div)) \
    IMPL_ALOPF1_FP_S(glue(fmin, S), T, T, T, FPU_MIN) \
    IMPL_ALOPF1_FP_S(glue(fmax, S), T, T, T, FPU_MAX)

IMPL_ALOPF1_FP_MANY(s, f32)
IMPL_ALOPF1_FP_MANY(d, f64)

IMPL_ALOPF2_FP(fsqrts,  f32, f32, float32_sqrt)
IMPL_ALOPF2_FP(frcps,   f32, f32, frcps)
IMPL_ALOPF2_FP(frsqrts, f32, f32, frsqrts)
IMPL_ALOPF1_FP(fsqrttd, f64, f64, f64, fsqrttd)

#define FMA(T, x, y, z, f, s) \
    glue(type_name(T), _muladd)(x, y, z, f, s)

#define FMAS(x, y, z, s) FMA(f32, x, y, z, 0, s)
#define FMAD(x, y, z, s) FMA(f64, x, y, z, 0, s)
#define FMSS(x, y, z, s) FMA(f32, x, y, z, float_muladd_negate_c, s)
#define FMSD(x, y, z, s) FMA(f64, x, y, z, float_muladd_negate_c, s)
#define FNMAS(x, y, z, s) FMA(f32, x, y, z, float_muladd_negate_product, s)
#define FNMAD(x, y, z, s) FMA(f64, x, y, z, float_muladd_negate_product, s)
#define FNMSS(x, y, z, s) FMA(f32, x, y, z, float_muladd_negate_product | float_muladd_negate_c, s)
#define FNMSD(x, y, z, s) FMA(f64, x, y, z, float_muladd_negate_product | float_muladd_negate_c, s)

IMPL_ALOPF21_FP(fmas,  f32, FMAS)
IMPL_ALOPF21_FP(fmss,  f32, FMSS)
IMPL_ALOPF21_FP(fnmas, f32, FNMAS)
IMPL_ALOPF21_FP(fnmss, f32, FNMSS)
IMPL_ALOPF21_FP(fmad,  f64, FMAD)
IMPL_ALOPF21_FP(fmsd,  f64, FMSD)
IMPL_ALOPF21_FP(fnmad, f64, FNMAD)
IMPL_ALOPF21_FP(fnmsd, f64, FNMSD)

#define qp_len_uw 4
#define qp_len_ud 2
#define qp_len(T) glue(qp_len_, T)

#define IMPL_OP3_QP_ENV(name, T, F, code) \
    void HELPER(name)(E2KReg *r, CPUE2KState *env, E2KReg *s1, \
        E2KReg *s2, E2KReg *s3) \
    { \
        int old_flags = fp_save_exception_flags(env); \
        int i; \
        \
        for (i = 0; i < qp_len(T); i++) { \
            type(F) a = make(F, s1->T[i]); \
            type(F) b = make(F, s2->T[i]); \
            type(F) c = make(F, s3->T[i]); \
            { code; } \
        } \
        \
        fp_merge_exception_flags(env, old_flags); \
    }

#define IMPL_OP3_QP_ENV_OP(name, T, F, op) \
    IMPL_OP3_QP_ENV(name, T, F, { \
        r->T[i] = float_val(F, op(a, b, c, &env->fp_status)); \
    })

IMPL_OP3_QP_ENV_OP(qpfmas,  uw, f32, FMAS)
IMPL_OP3_QP_ENV_OP(qpfmss,  uw, f32, FMSS)
IMPL_OP3_QP_ENV_OP(qpfnmas, uw, f32, FNMAS)
IMPL_OP3_QP_ENV_OP(qpfnmss, uw, f32, FNMSS)
IMPL_OP3_QP_ENV_OP(qpfmad,  ud, f64, FMAD)
IMPL_OP3_QP_ENV_OP(qpfmsd,  ud, f64, FMSD)
IMPL_OP3_QP_ENV_OP(qpfnmad, ud, f64, FNMAD)
IMPL_OP3_QP_ENV_OP(qpfnmsd, ud, f64, FNMSD)

#define IMPL_OP3_QP_ENV_OP_ALT(name, T, F, op1, op2) \
    IMPL_OP3_QP_ENV(name, T, F, { \
        r->T[i] = float_val(F, i & 1 ? op2(a, b, c, &env->fp_status) : \
            op1(a, b, c, &env->fp_status)); \
    })

IMPL_OP3_QP_ENV_OP_ALT(qpfmass, uw, f32, FMAS, FMSS)
IMPL_OP3_QP_ENV_OP_ALT(qpfmsas, uw, f32, FMSS, FMAS)
IMPL_OP3_QP_ENV_OP_ALT(qpfmasd, ud, f64, FMAD, FMSD)
IMPL_OP3_QP_ENV_OP_ALT(qpfmsad, ud, f64, FMSD, FMAD)
