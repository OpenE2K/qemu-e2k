#include "qemu/osdep.h"
#include "qemu/log.h"
#include "cpu.h"
#include "exec/exec-all.h"
#include "qemu/host-utils.h"
#include "exec/helper-proto.h"

#define glue3(x, y, z) glue(glue(x, y), z)
#define deref(x) *(x)

static inline void fpu_set_exception(CPUE2KState *env, int mask)
{
    env->fpsr.ef |= mask;
    if (env->fpsr.ef & (~env->fpcr.em & FPCR_EM)) {
        env->fpsr.es = 1;
        env->fpsr.b  = 1;
    }
}

static inline uint8_t save_exception_flags(CPUE2KState *env)
{
    uint8_t old_flags = get_float_exception_flags(&env->fp_status);
    set_float_exception_flags(0, &env->fp_status);
    return old_flags;
}

static inline void merge_exception_flags(CPUE2KState *env, uint8_t old_flags)
{
    uint8_t new_flags = get_float_exception_flags(&env->fp_status);
    float_raise(old_flags, &env->fp_status);
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
    int x;
    
    switch(env->fpcr.rc) {
        case FPCR_RC_UP:
            x = float_round_up;
            break;
        case FPCR_RC_DOWN:
            x = float_round_down;
            break;
        case FPCR_RC_CHOP:
            x = float_round_to_zero;
            break;
        case FPCR_RC_NEAR:
            x = float_round_nearest_even;
            break;
        default:
            abort();
            break;
    }
    
    set_float_rounding_mode(x, &env->fp_status);
        
    switch(env->fpcr.pc) {
        case FPCR_PC_XP: x = 80; break;
        case FPCR_PC_DP: x = 64; break;
        case FPCR_PC_SP: x = 32; break;
        case FPCR_PC_RESERVED:
        default:
            abort(); 
            break;
    }
    
    set_floatx80_rounding_precision(x, &env->fp_status);
    
}

#define GENERATE_SIMPLE_FLOAT2_OP(name, function, size) \
    uint##size##_t HELPER(name)(CPUE2KState *env, uint##size##_t x, uint##size##_t y) \
    { \
        uint8_t old_flags = save_exception_flags(env); \
        float##size z = float##size##_##function (make_float##size (x), make_float##size (y), &env->fp_status); \
        merge_exception_flags(env, old_flags); \
        return float##size##_val(z); \
    }
    
#define GENERATE_CMP_FLOAT2_OP(name, function, expr, size) \
    uint##size##_t HELPER(name)(CPUE2KState *env, uint##size##_t x, uint##size##_t y) \
    { \
        uint8_t old_flags = save_exception_flags(env); \
        uint##size##_t z = expr float##size##_##function (make_float##size (x), make_float##size (y), &env->fp_status); \
        merge_exception_flags(env, old_flags); \
        return z; \
    }

#define no_cvt(x) (x) // when function already returns in correct type

#define GENERATE_CVT_FLOAT1_OP(name, from_t, to_t, size_from, size_to, func_from, func_to) \
    size_to HELPER(name)(CPUE2KState *env, size_from x) \
    {\
        uint8_t old_flags = save_exception_flags(env); \
        size_to z = func_to( glue3(from_t, _to_, to_t) (func_from(x), &env->fp_status) );\
        merge_exception_flags(env, old_flags); \
        return z; \
    }

#define GENERATE_SIMPLE_FLOAT2_OPS_32_64(name, function) \
    GENERATE_SIMPLE_FLOAT2_OP(glue(name, s), function, 32) \
    GENERATE_SIMPLE_FLOAT2_OP(glue(name, d), function, 64)

#define GENERATE_CMP_FLOAT2_OPS_32_64(name, function, expr) \
    GENERATE_CMP_FLOAT2_OP(glue(name, s), function, expr, 32) \
    GENERATE_CMP_FLOAT2_OP(glue(name, d), function, expr, 64)
        
GENERATE_SIMPLE_FLOAT2_OPS_32_64(fadd, add)
GENERATE_SIMPLE_FLOAT2_OPS_32_64(fsub, sub)
GENERATE_SIMPLE_FLOAT2_OPS_32_64(fmin, min)
GENERATE_SIMPLE_FLOAT2_OPS_32_64(fmax, max)
GENERATE_SIMPLE_FLOAT2_OPS_32_64(fmul, mul)
GENERATE_SIMPLE_FLOAT2_OPS_32_64(fdiv, div)
GENERATE_CMP_FLOAT2_OPS_32_64(fcmpeq,  eq,  )
GENERATE_CMP_FLOAT2_OPS_32_64(fcmpneq, eq, !)
GENERATE_CMP_FLOAT2_OPS_32_64(fcmple,  le,  )
GENERATE_CMP_FLOAT2_OPS_32_64(fcmpnle, le, !)
GENERATE_CMP_FLOAT2_OPS_32_64(fcmplt,  lt,  )
GENERATE_CMP_FLOAT2_OPS_32_64(fcmpnlt, lt, !)
GENERATE_CMP_FLOAT2_OPS_32_64(fcmpuod, unordered,  )
GENERATE_CMP_FLOAT2_OPS_32_64(fcmpod,  unordered, !)

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
    uint8_t old_flags = save_exception_flags(env);
    *ret = float32_to_floatx80(make_float32(x), &env->fp_status);
    merge_exception_flags(env, old_flags);
}

void HELPER(fdtofx)(floatx80 *ret, CPUE2KState *env, uint64_t x)
{
    uint8_t old_flags = save_exception_flags(env);
    *ret = float64_to_floatx80(make_float64(x), &env->fp_status);
    merge_exception_flags(env, old_flags);
}

#define GEN_OP2_XX(name, op) \
    void HELPER(name)(CPUE2KState *env, floatx80 *x, floatx80 *y) \
    { \
	    uint8_t old_flags = save_exception_flags(env); \
	    *x = glue(floatx80_, op)(*x, *y, &env->fp_status); \
	    merge_exception_flags(env, old_flags); \
    }

GEN_OP2_XX(fxaddxx, add)
GEN_OP2_XX(fxsubxx, sub)
GEN_OP2_XX(fxmulxx, mul)
GEN_OP2_XX(fxdivxx, div)

void HELPER(fxrsubxx)(CPUE2KState *env, floatx80 *x, floatx80 *y)
{
    uint8_t old_flags = save_exception_flags(env);
    *x = floatx80_sub(*y, *x, &env->fp_status);
    merge_exception_flags(env, old_flags);
}
