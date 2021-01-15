#include "qemu/osdep.h"
#include "qemu/log.h"
#include "cpu.h"
#include "exec/exec-all.h"
#include "qemu/host-utils.h"
#include "exec/helper-proto.h"

#define glue3(x, y, z) glue(glue(x, y), z)
#define glue4(x, y, z, w) glue(glue3(x, y, z), w)
#define deref(x) *(x)
#define no_cvt(x) (x) /* when in correct type */

static inline void fpu_set_exception(CPUE2KState *env, int mask)
{
    env->fpsr.ef |= mask;
    if (env->fpsr.ef & (~env->fpcr.em & FPCR_EM)) {
        env->fpsr.es = 1;
        env->fpsr.b  = 1;
    }
}

static inline int save_exception_flags(CPUE2KState *env)
{
    int old_flags = get_float_exception_flags(&env->fp_status);
    set_float_exception_flags(0, &env->fp_status);
    return old_flags;
}

static inline void merge_exception_flags(CPUE2KState *env, int old_flags)
{
    int new_flags = get_float_exception_flags(&env->fp_status);
    float_raise((uint8_t)old_flags, &env->fp_status);
    fpu_set_exception(env,
                      ((new_flags & float_flag_invalid ? FPSR_IE : 0) |
                       (new_flags & float_flag_divbyzero ? FPSR_ZE : 0) |
                       (new_flags & float_flag_overflow ? FPSR_OE : 0) |
                       (new_flags & float_flag_underflow ? FPSR_UE : 0) |
                       (new_flags & float_flag_inexact ? FPSR_PE : 0) |
                       (new_flags & float_flag_input_denormal ? FPSR_DE : 0)));
}

void e2k_update_fp_status(CPUE2KState *env)
{
    FloatRoundMode rm;
    int x;
    
    switch(env->fpcr.rc) {
        case FPCR_RC_UP:
            rm = float_round_up;
            break;
        case FPCR_RC_DOWN:
            rm = float_round_down;
            break;
        case FPCR_RC_CHOP:
            rm = float_round_to_zero;
            break;
        case FPCR_RC_NEAR:
            rm = float_round_nearest_even;
            break;
        default:
            qemu_log_mask(LOG_UNIMP, "unknown rounding mode 0x%x\n", env->fpcr.rc);
            abort();
    }
    
    set_float_rounding_mode(rm, &env->fp_status);
        
    switch(env->fpcr.pc) {
        case FPCR_PC_XP: x = 80; break;
        case FPCR_PC_DP: x = 64; break;
        case FPCR_PC_SP: x = 32; break;
        case FPCR_PC_RESERVED:
        default:
            qemu_log_mask(LOG_UNIMP, "unknown precision mode 0x%x\n", env->fpcr.pc);
            abort();
    }
    
    set_floatx80_rounding_precision(x, &env->fp_status);
}

#define GENERATE_SIMPLE_FLOAT2_OP(name, function, size) \
    uint##size##_t HELPER(name)(CPUE2KState *env, uint##size##_t x, uint##size##_t y) \
    { \
        int old_flags = save_exception_flags(env); \
        float##size z = float##size##_##function (make_float##size (x), make_float##size (y), &env->fp_status); \
        merge_exception_flags(env, old_flags); \
        return float##size##_val(z); \
    }

#define GENERATE_SIMPLE_FLOAT2_OPS_32_64(name, function) \
    GENERATE_SIMPLE_FLOAT2_OP(glue(name, s), function, 32) \
    GENERATE_SIMPLE_FLOAT2_OP(glue(name, d), function, 64)

#define GENERATE_CMP_FLOAT2_OP(ret_type, name, expr, op, in_type, cvt_macro) \
    ret_type HELPER(name)(CPUE2KState *env, in_type x, in_type y) \
    { \
        int old_flags = save_exception_flags(env); \
        ret_type z = expr op(cvt_macro(x), cvt_macro(y), &env->fp_status); \
        merge_exception_flags(env, old_flags); \
        return z ? -1 : 0; \
    }

#define GENERATE_CMP_FLOAT2_OPS_32_64_80(name, expr, op) \
    GENERATE_CMP_FLOAT2_OP(uint32_t, glue3(f, name, s), expr, glue(float32_, op), uint32_t, make_float32) \
    GENERATE_CMP_FLOAT2_OP(uint64_t, glue3(f, name, d), expr, glue(float64_, op), uint64_t, make_float32) \
    GENERATE_CMP_FLOAT2_OP(uint64_t, glue3(fx, name, x), expr, glue(floatx80_, op), floatx80*, deref)

#define GENERATE_CVT_FLOAT1_OP(name, from_t, to_t, size_from, size_to, func_from, func_to) \
    size_to HELPER(name)(CPUE2KState *env, size_from x) \
    {\
        int old_flags = save_exception_flags(env); \
        size_to z = func_to( glue3(from_t, _to_, to_t) (func_from(x), &env->fp_status) );\
        merge_exception_flags(env, old_flags); \
        return z; \
    }

GENERATE_SIMPLE_FLOAT2_OPS_32_64(fadd, add)
GENERATE_SIMPLE_FLOAT2_OPS_32_64(fsub, sub)
GENERATE_SIMPLE_FLOAT2_OPS_32_64(fmin, min)
GENERATE_SIMPLE_FLOAT2_OPS_32_64(fmax, max)
GENERATE_SIMPLE_FLOAT2_OPS_32_64(fmul, mul)
GENERATE_SIMPLE_FLOAT2_OPS_32_64(fdiv, div)
GENERATE_CMP_FLOAT2_OPS_32_64_80(cmpeq,   , eq)
GENERATE_CMP_FLOAT2_OPS_32_64_80(cmpneq, !, eq)
GENERATE_CMP_FLOAT2_OPS_32_64_80(cmple,   , le)
GENERATE_CMP_FLOAT2_OPS_32_64_80(cmpnle, !, le)
GENERATE_CMP_FLOAT2_OPS_32_64_80(cmplt,   , lt)
GENERATE_CMP_FLOAT2_OPS_32_64_80(cmpnlt, !, lt)
GENERATE_CMP_FLOAT2_OPS_32_64_80(cmpuod,  , unordered)
GENERATE_CMP_FLOAT2_OPS_32_64_80(cmpod,  !, unordered)

GENERATE_CVT_FLOAT1_OP(fstois,   float32, int32,   uint32_t, uint32_t, make_float32, no_cvt)
GENERATE_CVT_FLOAT1_OP(istofs,   int32,   float32, uint32_t, uint32_t, no_cvt, float32_val)
GENERATE_CVT_FLOAT1_OP(fstoistr, float32, int32_round_to_zero, uint32_t, uint32_t, make_float32, no_cvt)

GENERATE_CVT_FLOAT1_OP(fdtoid,   float64, int64,   uint64_t, uint64_t, make_float64, no_cvt)
GENERATE_CVT_FLOAT1_OP(idtofd,   int64,   float64, uint64_t, uint64_t, no_cvt, float64_val)
GENERATE_CVT_FLOAT1_OP(fdtoidtr, float64, int64_round_to_zero, uint64_t, uint64_t, make_float64, no_cvt)

GENERATE_CVT_FLOAT1_OP(fstofd,   float32, float64, uint32_t, uint64_t, make_float32, float64_val)
GENERATE_CVT_FLOAT1_OP(fstoid,   float32, int64,   uint32_t, uint64_t, make_float32, no_cvt)
GENERATE_CVT_FLOAT1_OP(istofd,   int32,   float64, uint32_t, uint64_t, no_cvt, float64_val)
GENERATE_CVT_FLOAT1_OP(fstoidtr, float32, int64_round_to_zero, uint32_t, uint64_t, make_float32, no_cvt)

GENERATE_CVT_FLOAT1_OP(fdtofs,   float64, float32, uint64_t, uint32_t, make_float64, float32_val)
GENERATE_CVT_FLOAT1_OP(fdtois,   float64, int32,   uint64_t, uint32_t, make_float64, no_cvt)
GENERATE_CVT_FLOAT1_OP(idtofs,   int64,   float32, uint64_t, uint32_t, no_cvt, float32_val)
GENERATE_CVT_FLOAT1_OP(fdtoistr, float64, int32_round_to_zero, uint64_t, uint32_t, make_float64, no_cvt)

GENERATE_CVT_FLOAT1_OP(fxtofs,   floatx80, float32, floatx80*, uint32_t, deref, float32_val)
GENERATE_CVT_FLOAT1_OP(fxtofd,   floatx80, float64, floatx80*, uint64_t, deref, float64_val)

void HELPER(fstofx)(floatx80 *ret, CPUE2KState *env, uint32_t x)
{
    int old_flags = save_exception_flags(env);
    *ret = float32_to_floatx80(make_float32(x), &env->fp_status);
    merge_exception_flags(env, old_flags);
}

void HELPER(fdtofx)(floatx80 *ret, CPUE2KState *env, uint64_t x)
{
    int old_flags = save_exception_flags(env);
    *ret = float64_to_floatx80(make_float64(x), &env->fp_status);
    merge_exception_flags(env, old_flags);
}

#define GEN_OP2_XX(name, op) \
    void HELPER(name)(CPUE2KState *env, floatx80 *x, floatx80 *y) \
    { \
        int old_flags = save_exception_flags(env); \
        *x = glue(floatx80_, op)(*x, *y, &env->fp_status); \
        merge_exception_flags(env, old_flags); \
    }

GEN_OP2_XX(fxaddxx, add)
GEN_OP2_XX(fxsubxx, sub)
GEN_OP2_XX(fxmulxx, mul)
GEN_OP2_XX(fxdivxx, div)

void HELPER(fxrsubxx)(CPUE2KState *env, floatx80 *x, floatx80 *y)
{
    int old_flags = save_exception_flags(env);
    *x = floatx80_sub(*y, *x, &env->fp_status);
    merge_exception_flags(env, old_flags);
}

#define GENERATE_FCMPODF(op, IN, F, cvt_macro) \
    uint32_t HELPER(op)(CPUE2KState *env, IN _x, IN _y) \
    { \
        int old_flags = save_exception_flags(env); \
        uint32_t ret; \
        F x = cvt_macro(_x); \
        F y = cvt_macro(_y); \
        if (glue(F, _is_any_nan)(x) || glue(F, _is_any_nan)(y)) { \
            ret = 0x45; \
        } else { \
            FloatRelation relation = glue(F, _compare_quiet)(x, y, &env->fp_status); \
            switch(relation) \
            { \
            case float_relation_less: \
                ret = 0x01; \
                break; \
            case float_relation_greater: \
                ret = 0x00; \
                break; \
            case float_relation_equal: \
                ret = 0x40; \
                break; \
            case float_relation_unordered: \
            default: \
                ret = 0x45; \
                break; \
            } \
        } \
        merge_exception_flags(env, old_flags); \
        return ret; \
    }
GENERATE_FCMPODF(fcmpodsf,  uint32_t, float32, make_float32)
GENERATE_FCMPODF(fcmpoddf,  uint64_t, float64, make_float64)
GENERATE_FCMPODF(fxcmpodxf, floatx80 *, floatx80, deref)

/* didn't found any difference between these instruction
   so keep it that way for now */
uint32_t HELPER(fcmpudsf)(CPUE2KState *env, float32 x, float32 y)
{
    return HELPER(fcmpodsf)(env, x, y);
}

uint32_t HELPER(fcmpuddf)(CPUE2KState *env, float64 x, float64 y)
{
    return HELPER(fcmpoddf)(env, x, y);
}

uint32_t HELPER(fxcmpudxf)(CPUE2KState *env, floatx80 *x, floatx80 *y)
{
    return HELPER(fxcmpodxf)(env, x, y);
}

#define TOIF_RC_CURRENT        0x4
#define TOIF_RC_IGNORE_INEXACT 0x8

static inline void toif_set_round_mode(CPUE2KState *env, uint32_t flags)
{
    if(flags <= FPCR_RC_CHOP) {
        FloatRoundMode rm;
        switch(flags) {
        case FPCR_RC_NEAR: rm = float_round_nearest_even; break;
        case FPCR_RC_DOWN: rm = float_round_down; break;
        case FPCR_RC_UP:   rm = float_round_up; break;
        case FPCR_RC_CHOP: rm = float_round_to_zero; break;
        }
        set_float_rounding_mode(rm, &env->fp_status);
    }
}

static inline void toif_clear_inexact(CPUE2KState *env, uint32_t flags) 
{
    if(flags & TOIF_RC_IGNORE_INEXACT) {
        int new_flags = get_float_exception_flags(&env->fp_status);
        new_flags = new_flags & (~float_flag_inexact);
        set_float_exception_flags(new_flags, &env->fp_status);
    }
}

uint32_t HELPER(fstoifs)(CPUE2KState *env, uint32_t flags, uint32_t f)
{
    int old_flags = save_exception_flags(env);
    FloatRoundMode oldrm = get_float_rounding_mode(&env->fp_status);
    uint32_t ret;
    toif_set_round_mode(env, flags);
    ret = float32_val(float32_round_to_int(make_float32(f), &env->fp_status));
    set_float_rounding_mode(oldrm, &env->fp_status);
    toif_clear_inexact(env, flags);
    merge_exception_flags(env, old_flags);
    return ret;
}

uint64_t HELPER(fdtoifd)(CPUE2KState *env, uint64_t flags, uint64_t f)
{
    int old_flags = save_exception_flags(env);
    FloatRoundMode oldrm = get_float_rounding_mode(&env->fp_status);
    uint64_t ret;
    toif_set_round_mode(env, flags);
    ret = float64_val(float64_round_to_int(make_float64(f), &env->fp_status));
    set_float_rounding_mode(oldrm, &env->fp_status);
    toif_clear_inexact(env, flags);
    merge_exception_flags(env, old_flags);
    return ret;
}

/* TODO: test if valid, test exception flags */
#if 0
uint32_t HELPER(frcps)(CPUE2KState *env, uint32_t x)
{
    int old_flags = save_exception_flags(env);
    uint32_t y = float32_div(float32_one, make_float32(x), &env->fp_status);
    merge_exception_flags(env, old_flags);
    return float32_val(y);
}

uint32_t HELPER(fsqrts)(CPUE2KState *env, uint32_t x)
{
    int old_flags = save_exception_flags(env);
    uint32_t y = float32_sqrt(make_float32(x), &env->fp_status);
    merge_exception_flags(env, old_flags);
    return float32_val(y);
}

uint32_t HELPER(frsqrts)(CPUE2KState *env, uint32_t x)
{
    int old_flags = save_exception_flags(env);
    uint32_t y = float32_div(float32_one,
                             float32_sqrt(make_float32(x), &env->fp_status),
                             &env->fp_status);
    merge_exception_flags(env, old_flags);
    return float32_val(z);
}
#endif
