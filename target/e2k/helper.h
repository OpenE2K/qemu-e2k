DEF_HELPER_2(raise_exception, noreturn, env, int)
DEF_HELPER_1(debug, void, env)
DEF_HELPER_2(prep_return, i64, env, int)
DEF_HELPER_1(return, void, env)
DEF_HELPER_4(call, void, env, i64, int, tl)
DEF_HELPER_2(sxt, i64, i64, i32)
DEF_HELPER_1(debug_i32, void, i32)
DEF_HELPER_1(debug_i64, void, i64)
DEF_HELPER_1(debug_ptr, void, ptr)
DEF_HELPER_2(state_reg_read_i64, i64, env, int)
DEF_HELPER_2(state_reg_read_i32, i32, env, int)
DEF_HELPER_3(state_reg_write_i64, void, env, int, i64)
DEF_HELPER_3(state_reg_write_i32, void, env, int, i32)
DEF_HELPER_2(getsp, i64, env, i32) /* FIXME: return tl? */
DEF_HELPER_1(break_restore_state, void, env)
DEF_HELPER_2(setwd, void, env, i32)
DEF_HELPER_2(probe_read_access, int, env, tl)
DEF_HELPER_2(probe_write_access, int, env, tl)
DEF_HELPER_3(packed_shuffle_i64, i64, i64, i64, i64)
DEF_HELPER_2(pcmpeqb, i64, i64, i64)
