#define f80 i128
#define vec i128

DEF_HELPER_2(raise_exception, noreturn, env, int)

DEF_HELPER_4(call, void, env, i64, int, tl)
DEF_HELPER_2(prep_return, i64, env, int)
DEF_HELPER_1(return, void, env)

DEF_HELPER_4(setwd, void, env, int, int, int)

DEF_HELPER_FLAGS_3(probe_read_access,   TCG_CALL_NO_RWG_SE, int, env, tl, int)
DEF_HELPER_FLAGS_3(probe_write_access,  TCG_CALL_NO_RWG_SE, int, env, tl, int)

DEF_HELPER_FLAGS_2(aau_load_program,    TCG_CALL_NO_RWG, void, env, i64)
DEF_HELPER_FLAGS_6(mova_ptr,            TCG_CALL_NO_RWG, tl, env, int, int, int, int, int)
DEF_HELPER_FLAGS_3(aau_am,              TCG_CALL_NO_RWG, void, env, int, int)
DEF_HELPER_FLAGS_4(aaurwd_aad,          TCG_CALL_NO_RWG, void, env, i32, i64, i32)
DEF_HELPER_FLAGS_4(aaurwq_aad,          TCG_CALL_NO_RWG, void, env, i32, i128, i32)
DEF_HELPER_FLAGS_4(aaurwd_aaind,        TCG_CALL_NO_RWG, void, env, i32, i32, i32)
DEF_HELPER_FLAGS_4(aaurwd_aasti,        TCG_CALL_NO_RWG, void, env, i32, i32, i32)
DEF_HELPER_FLAGS_4(aaurwd_aaincr,       TCG_CALL_NO_RWG, void, env, i32, i32, i32)
DEF_HELPER_FLAGS_3(aad_ptr,             TCG_CALL_NO_RWG_SE, i64, env, i32, i32)
DEF_HELPER_FLAGS_4(aasti_incr,          TCG_CALL_NO_RWG, void, env, i32, i32, i32)

DEF_HELPER_4(dam_lock_addr, void, env, i64, int, int)
DEF_HELPER_4(dam_unlock_addr, int, env, i64, int, int)

DEF_HELPER_FLAGS_2(getsp, TCG_CALL_NO_RWG, i64, env, i32)

DEF_HELPER_FLAGS_2(state_reg_get, TCG_CALL_NO_WG_SE, i64, env, int)
DEF_HELPER_3(state_reg_set, void, env, i64, int)

DEF_HELPER_FLAGS_2(sxt, TCG_CALL_NO_RWG_SE, i64, i32, i32)
DEF_HELPER_FLAGS_2(getfs_v1, TCG_CALL_NO_RWG_SE, i32, i32, i32)
DEF_HELPER_FLAGS_2(getfd_v1, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(getfs_v5, TCG_CALL_NO_RWG_SE, i32, i32, i32)
DEF_HELPER_FLAGS_2(getfd_v5, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(clmull, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(clmulh, TCG_CALL_NO_RWG_SE, i64, i64, i64)

/* Packed Min/Max */
DEF_HELPER_FLAGS_2(pminub, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(pminsb, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(pminuh, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(pminsh, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(pminuw, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(pminsw, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(pmaxub, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(pmaxsb, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(pmaxuh, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(pmaxsh, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(pmaxuw, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(pmaxsw, TCG_CALL_NO_RWG_SE, i64, i64, i64)

/* Packed Cmp */
DEF_HELPER_FLAGS_2(pcmpeqb, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(pcmpeqh, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(pcmpeqw, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(pcmpeqd, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(pcmpgtb, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(pcmpgth, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(pcmpgtw, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(pcmpgtd, TCG_CALL_NO_RWG_SE, i64, i64, i64)

/* Pached Horizontal Add */
DEF_HELPER_FLAGS_2(phaddh,  TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(phaddw,  TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(phaddsh, TCG_CALL_NO_RWG_SE, i64, i64, i64)

DEF_HELPER_FLAGS_2(qphaddh,  TCG_CALL_NO_RWG, vec, vec, vec)
DEF_HELPER_FLAGS_2(qphaddw,  TCG_CALL_NO_RWG, vec, vec, vec)
DEF_HELPER_FLAGS_2(qphaddsh, TCG_CALL_NO_RWG, vec, vec, vec)

/* Packed Horizontal Sub */
DEF_HELPER_FLAGS_2(phsubh,  TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(phsubw,  TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(phsubsh, TCG_CALL_NO_RWG_SE, i64, i64, i64)

DEF_HELPER_FLAGS_2(qphsubh,  TCG_CALL_NO_RWG, vec, vec, vec)
DEF_HELPER_FLAGS_2(qphsubw,  TCG_CALL_NO_RWG, vec, vec, vec)
DEF_HELPER_FLAGS_2(qphsubsh, TCG_CALL_NO_RWG, vec, vec, vec)

/* Packed Add using saturation */
DEF_HELPER_FLAGS_2(paddsb,  TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(paddsh,  TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(paddusb, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(paddush, TCG_CALL_NO_RWG_SE, i64, i64, i64)

/* Packed Sub using saturation */
DEF_HELPER_FLAGS_2(psubsb,  TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(psubsh,  TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(psubusb, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(psubush, TCG_CALL_NO_RWG_SE, i64, i64, i64)

/* Packed shifts */
DEF_HELPER_FLAGS_2(psllh, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(psllw, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(pslld, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(psrlh, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(psrlw, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(psrld, TCG_CALL_NO_RWG_SE, i64, i64, i64)

/* Packed shifts with sign */
DEF_HELPER_FLAGS_2(psrah, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(psraw, TCG_CALL_NO_RWG_SE, i64, i64, i64)

/* Packed Mul */
DEF_HELPER_FLAGS_2(pmaddh,    TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(pmaddubsh, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(pmulhh,    TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(pmullh,    TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(pmulhuh,   TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(pmulubhh,  TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(pmulhrsh,  TCG_CALL_NO_RWG_SE, i64, i64, i64)

/* Packed Sign Mul */
DEF_HELPER_FLAGS_2(psignb, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(psignh, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(psignw, TCG_CALL_NO_RWG_SE, i64, i64, i64)

/* Packed Move Mask */
DEF_HELPER_FLAGS_2(pmovmskb,  TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(pmovmskps, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(pmovmskpd, TCG_CALL_NO_RWG_SE, i64, i64, i64)

/* Packed packs */
DEF_HELPER_FLAGS_2(packsshb, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(packushb, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(packsswh, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(packuswh, TCG_CALL_NO_RWG_SE, i64, i64, i64)

/* Packed unpacks */
DEF_HELPER_FLAGS_2(punpcklbh, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(punpcklhw, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(punpcklwd, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(punpckhbh, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(punpckhhw, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(punpckhwd, TCG_CALL_NO_RWG_SE, i64, i64, i64)

/* Packed shuffle */
DEF_HELPER_FLAGS_3(pshufb, TCG_CALL_NO_RWG_SE, i64, i64, i64, i64)
DEF_HELPER_FLAGS_3(pmerge, TCG_CALL_NO_RWG_SE, i64, i64, i64, i64)
DEF_HELPER_FLAGS_2(pshufh, TCG_CALL_NO_RWG_SE, i64, i64, i32)
DEF_HELPER_FLAGS_3(pshufw, TCG_CALL_NO_RWG_SE, i64, i64, i64, i32)

/* Packed uncategorized */
DEF_HELPER_FLAGS_2(psadbw,     TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(pavgusb,    TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(pavgush,    TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(phminposuh, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(mpsadbh,    TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_4(plog,       TCG_CALL_NO_RWG_SE, i64, i32, i64, i64, i64)

DEF_HELPER_FLAGS_2(qpmpsadbh,   TCG_CALL_NO_RWG,    vec, vec, i32)
DEF_HELPER_FLAGS_2(qpmulubhh,   TCG_CALL_NO_RWG,    vec, i64, vec)
DEF_HELPER_FLAGS_2(qphminposuh, TCG_CALL_NO_RWG_SE, i64, vec, vec)
DEF_HELPER_FLAGS_1(qpsgn2mskb,  TCG_CALL_NO_RWG_SE, i32, vec)
DEF_HELPER_FLAGS_2(qpmsk2sgnb,  TCG_CALL_NO_RWG,    vec, vec, i32)
DEF_HELPER_FLAGS_3(qppermb,     TCG_CALL_NO_RWG,    vec, vec, vec, vec)
DEF_HELPER_FLAGS_3(qpshufb,     TCG_CALL_NO_RWG,    vec, vec, vec, vec)

/* Float 32/64 Ops */
DEF_HELPER_FLAGS_3(fadds,     TCG_CALL_NO_RWG, i32, env, i32, i32)
DEF_HELPER_FLAGS_3(fsubs,     TCG_CALL_NO_RWG, i32, env, i32, i32)
DEF_HELPER_FLAGS_3(fmuls,     TCG_CALL_NO_RWG, i32, env, i32, i32)
DEF_HELPER_FLAGS_3(fdivs,     TCG_CALL_NO_RWG, i32, env, i32, i32)
DEF_HELPER_FLAGS_3(fmins,     TCG_CALL_NO_RWG, i32, env, i32, i32)
DEF_HELPER_FLAGS_3(fmaxs,     TCG_CALL_NO_RWG, i32, env, i32, i32)
DEF_HELPER_FLAGS_3(fscales,   TCG_CALL_NO_RWG, i32, env, i32, i32)

DEF_HELPER_FLAGS_3(faddd,     TCG_CALL_NO_RWG, i64, env, i64, i64)
DEF_HELPER_FLAGS_3(fsubd,     TCG_CALL_NO_RWG, i64, env, i64, i64)
DEF_HELPER_FLAGS_3(fmuld,     TCG_CALL_NO_RWG, i64, env, i64, i64)
DEF_HELPER_FLAGS_3(fdivd,     TCG_CALL_NO_RWG, i64, env, i64, i64)
DEF_HELPER_FLAGS_3(fmind,     TCG_CALL_NO_RWG, i64, env, i64, i64)
DEF_HELPER_FLAGS_3(fmaxd,     TCG_CALL_NO_RWG, i64, env, i64, i64)
DEF_HELPER_FLAGS_3(fscaled,   TCG_CALL_NO_RWG, i64, env, i64, i32)

DEF_HELPER_FLAGS_2(frcps,     TCG_CALL_NO_RWG, i32, env, i32)
DEF_HELPER_FLAGS_2(fsqrts,    TCG_CALL_NO_RWG, i32, env, i32)
DEF_HELPER_FLAGS_2(frsqrts,   TCG_CALL_NO_RWG, i32, env, i32)
DEF_HELPER_FLAGS_3(fsqrttd,   TCG_CALL_NO_RWG, i64, env, i64, i64)

/* Packed Float ops */
DEF_HELPER_FLAGS_3(pfadds,    TCG_CALL_NO_RWG, i64, env, i64, i64)
DEF_HELPER_FLAGS_3(pfsubs,    TCG_CALL_NO_RWG, i64, env, i64, i64)
DEF_HELPER_FLAGS_3(pfmuls,    TCG_CALL_NO_RWG, i64, env, i64, i64)
DEF_HELPER_FLAGS_3(pfmaxs,    TCG_CALL_NO_RWG, i64, env, i64, i64)
DEF_HELPER_FLAGS_3(pfmins,    TCG_CALL_NO_RWG, i64, env, i64, i64)
DEF_HELPER_FLAGS_3(pfhadds,   TCG_CALL_NO_RWG, i64, env, i64, i64)
DEF_HELPER_FLAGS_3(pfhsubs,   TCG_CALL_NO_RWG, i64, env, i64, i64)
DEF_HELPER_FLAGS_3(pfaddsubs, TCG_CALL_NO_RWG, i64, env, i64, i64)
DEF_HELPER_FLAGS_3(qpfhadds,  TCG_CALL_NO_RWG, vec, env, vec, vec)
DEF_HELPER_FLAGS_3(qpfhsubs,  TCG_CALL_NO_RWG, vec, env, vec, vec)

DEF_HELPER_FLAGS_3(pfstoifs,  TCG_CALL_NO_RWG, i64, env, i64, i64)
DEF_HELPER_FLAGS_2(pistofs,   TCG_CALL_NO_RWG, i64, env, i64)
DEF_HELPER_FLAGS_2(pfstois,   TCG_CALL_NO_RWG, i64, env, i64)
DEF_HELPER_FLAGS_2(pfstoistr, TCG_CALL_NO_RWG, i64, env, i64)

DEF_HELPER_FLAGS_2(qpfstoid,   TCG_CALL_NO_RWG, vec, env, i64)
DEF_HELPER_FLAGS_2(qpfstoidtr, TCG_CALL_NO_RWG, vec, env, i64)
DEF_HELPER_FLAGS_2(qpistofd,   TCG_CALL_NO_RWG, vec, env, i64)
DEF_HELPER_FLAGS_2(qpfstofd,   TCG_CALL_NO_RWG, vec, env, i64)

DEF_HELPER_FLAGS_2(qpfdtois,   TCG_CALL_NO_RWG, i64, env, vec)
DEF_HELPER_FLAGS_2(qpfdtoistr, TCG_CALL_NO_RWG, i64, env, vec)
DEF_HELPER_FLAGS_2(qpidtofs,   TCG_CALL_NO_RWG, i64, env, vec)
DEF_HELPER_FLAGS_2(qpfdtofs,   TCG_CALL_NO_RWG, i64, env, vec)

DEF_HELPER_FLAGS_3(pfcmpeqs,  TCG_CALL_NO_RWG, i64, env, i64, i64)
DEF_HELPER_FLAGS_3(pfcmplts,  TCG_CALL_NO_RWG, i64, env, i64, i64)
DEF_HELPER_FLAGS_3(pfcmples,  TCG_CALL_NO_RWG, i64, env, i64, i64)
DEF_HELPER_FLAGS_3(pfcmpuods, TCG_CALL_NO_RWG, i64, env, i64, i64)
DEF_HELPER_FLAGS_3(pfcmpneqs, TCG_CALL_NO_RWG, i64, env, i64, i64)
DEF_HELPER_FLAGS_3(pfcmpnlts, TCG_CALL_NO_RWG, i64, env, i64, i64)
DEF_HELPER_FLAGS_3(pfcmpnles, TCG_CALL_NO_RWG, i64, env, i64, i64)
DEF_HELPER_FLAGS_3(pfcmpods,  TCG_CALL_NO_RWG, i64, env, i64, i64)

/* FMA */
DEF_HELPER_FLAGS_4(fmas,      TCG_CALL_NO_RWG, i32, env, i32, i32, i32)
DEF_HELPER_FLAGS_4(fmss,      TCG_CALL_NO_RWG, i32, env, i32, i32, i32)
DEF_HELPER_FLAGS_4(fnmas,     TCG_CALL_NO_RWG, i32, env, i32, i32, i32)
DEF_HELPER_FLAGS_4(fnmss,     TCG_CALL_NO_RWG, i32, env, i32, i32, i32)

DEF_HELPER_FLAGS_4(fmad,      TCG_CALL_NO_RWG, i64, env, i64, i64, i64)
DEF_HELPER_FLAGS_4(fmsd,      TCG_CALL_NO_RWG, i64, env, i64, i64, i64)
DEF_HELPER_FLAGS_4(fnmad,     TCG_CALL_NO_RWG, i64, env, i64, i64, i64)
DEF_HELPER_FLAGS_4(fnmsd,     TCG_CALL_NO_RWG, i64, env, i64, i64, i64)

DEF_HELPER_FLAGS_4(qpfmas,    TCG_CALL_NO_RWG, vec, env, vec, vec, vec)
DEF_HELPER_FLAGS_4(qpfmss,    TCG_CALL_NO_RWG, vec, env, vec, vec, vec)
DEF_HELPER_FLAGS_4(qpfnmas,   TCG_CALL_NO_RWG, vec, env, vec, vec, vec)
DEF_HELPER_FLAGS_4(qpfnmss,   TCG_CALL_NO_RWG, vec, env, vec, vec, vec)
DEF_HELPER_FLAGS_4(qpfmass,   TCG_CALL_NO_RWG, vec, env, vec, vec, vec)
DEF_HELPER_FLAGS_4(qpfmsas,   TCG_CALL_NO_RWG, vec, env, vec, vec, vec)

DEF_HELPER_FLAGS_4(qpfmad,    TCG_CALL_NO_RWG, vec, env, vec, vec, vec)
DEF_HELPER_FLAGS_4(qpfmsd,    TCG_CALL_NO_RWG, vec, env, vec, vec, vec)
DEF_HELPER_FLAGS_4(qpfnmad,   TCG_CALL_NO_RWG, vec, env, vec, vec, vec)
DEF_HELPER_FLAGS_4(qpfnmsd,   TCG_CALL_NO_RWG, vec, env, vec, vec, vec)
DEF_HELPER_FLAGS_4(qpfmasd,   TCG_CALL_NO_RWG, vec, env, vec, vec, vec)
DEF_HELPER_FLAGS_4(qpfmsad,   TCG_CALL_NO_RWG, vec, env, vec, vec, vec)

/* Float x80 ops */
#define DEF_FX_OP2(op) \
    DEF_HELPER_FLAGS_3(glue(op, ss), TCG_CALL_NO_RWG, i32, env, f80, i32) \
    DEF_HELPER_FLAGS_3(glue(op, dd), TCG_CALL_NO_RWG, i64, env, f80, i64) \
    DEF_HELPER_FLAGS_3(glue(op, sx), TCG_CALL_NO_RWG, f80, env, f80, i32) \
    DEF_HELPER_FLAGS_3(glue(op, dx), TCG_CALL_NO_RWG, f80, env, f80, i64) \
    DEF_HELPER_FLAGS_3(glue(op, xx), TCG_CALL_NO_RWG, f80, env, f80, f80) \
    DEF_HELPER_FLAGS_3(glue(op, xd), TCG_CALL_NO_RWG, i64, env, f80, f80) \
    DEF_HELPER_FLAGS_3(glue(op, xs), TCG_CALL_NO_RWG, i32, env, f80, f80)

DEF_FX_OP2(fxadd)
DEF_FX_OP2(fxsub)
DEF_FX_OP2(fxmul)
DEF_FX_OP2(fxdiv)

#undef DEF_FX_OP2

DEF_HELPER_FLAGS_3(fxdivtss, TCG_CALL_NO_RWG, f32, env, f32, f80)
DEF_HELPER_FLAGS_3(fxdivtdd, TCG_CALL_NO_RWG, f64, env, f64, f80)
DEF_HELPER_FLAGS_3(fxdivtsx, TCG_CALL_NO_RWG, f80, env, f32, f80)
DEF_HELPER_FLAGS_3(fxdivtdx, TCG_CALL_NO_RWG, f80, env, f64, f80)

DEF_HELPER_FLAGS_3(fxrsubss, TCG_CALL_NO_RWG, i32, env, f80, i32)
DEF_HELPER_FLAGS_3(fxrsubdd, TCG_CALL_NO_RWG, i64, env, f80, i64)
DEF_HELPER_FLAGS_3(fxrsubsx, TCG_CALL_NO_RWG, f80, env, f80, i32)
DEF_HELPER_FLAGS_3(fxrsubdx, TCG_CALL_NO_RWG, f80, env, f80, i64)

DEF_HELPER_FLAGS_3(fxsqrttsx, TCG_CALL_NO_RWG, f80, env, i32, f80)
DEF_HELPER_FLAGS_3(fxsqrttdx, TCG_CALL_NO_RWG, f80, env, i64, f80)
DEF_HELPER_FLAGS_3(fxsqrttxx, TCG_CALL_NO_RWG, f80, env, f80, f80)

DEF_HELPER_FLAGS_3(fxscalesx, TCG_CALL_NO_RWG, f80, env, f80, i32)

DEF_HELPER_FLAGS_3(fxcmpodsf, TCG_CALL_NO_RWG, i32, env, f80, f32)
DEF_HELPER_FLAGS_3(fxcmpudsf, TCG_CALL_NO_RWG, i32, env, f80, f32)
DEF_HELPER_FLAGS_3(fxcmpoddf, TCG_CALL_NO_RWG, i32, env, f80, f64)
DEF_HELPER_FLAGS_3(fxcmpuddf, TCG_CALL_NO_RWG, i32, env, f80, f64)
DEF_HELPER_FLAGS_3(fxcmpodxf, TCG_CALL_NO_RWG, i32, env, f80, f80)
DEF_HELPER_FLAGS_3(fxcmpudxf, TCG_CALL_NO_RWG, i32, env, f80, f80)

/* Float Comparisons */
#define DEF_HELPER_FP_CMP(P, S, R, A, B) \
    DEF_HELPER_FLAGS_3(glue(glue(P, cmpeq),  S), TCG_CALL_NO_RWG, R, env, A, B) \
    DEF_HELPER_FLAGS_3(glue(glue(P, cmpneq), S), TCG_CALL_NO_RWG, R, env, A, B) \
    DEF_HELPER_FLAGS_3(glue(glue(P, cmple),  S), TCG_CALL_NO_RWG, R, env, A, B) \
    DEF_HELPER_FLAGS_3(glue(glue(P, cmpnle), S), TCG_CALL_NO_RWG, R, env, A, B) \
    DEF_HELPER_FLAGS_3(glue(glue(P, cmplt),  S), TCG_CALL_NO_RWG, R, env, A, B) \
    DEF_HELPER_FLAGS_3(glue(glue(P, cmpnlt), S), TCG_CALL_NO_RWG, R, env, A, B) \
    DEF_HELPER_FLAGS_3(glue(glue(P, cmpuod), S), TCG_CALL_NO_RWG, R, env, A, B) \
    DEF_HELPER_FLAGS_3(glue(glue(P, cmpod),  S), TCG_CALL_NO_RWG, R, env, A, B)

DEF_HELPER_FP_CMP(f,  s, i32, i32, i32)
DEF_HELPER_FP_CMP(f,  d, i64, i64, i64)
DEF_HELPER_FP_CMP(fx, s, i64, f80, i32)
DEF_HELPER_FP_CMP(fx, d, i64, f80, i64)
DEF_HELPER_FP_CMP(fx, x, i64, f80, f80)

#undef DEF_HELPER_FP_CMP

/* Float Flag Comparisons */
DEF_HELPER_FLAGS_3(fcmpodsf, TCG_CALL_NO_RWG, i32, env, i32, i32)
DEF_HELPER_FLAGS_3(fcmpudsf, TCG_CALL_NO_RWG, i32, env, i32, i32)
DEF_HELPER_FLAGS_3(fcmpoddf, TCG_CALL_NO_RWG, i32, env, i64, i64)
DEF_HELPER_FLAGS_3(fcmpuddf, TCG_CALL_NO_RWG, i32, env, i64, i64)

/* Float Conversions */
DEF_HELPER_FLAGS_2(fstofd,   TCG_CALL_NO_RWG, i64, env, i32)
DEF_HELPER_FLAGS_2(fstofx,   TCG_CALL_NO_RWG, f80, env, i32)

DEF_HELPER_FLAGS_2(fdtofs,   TCG_CALL_NO_RWG, i32, env, i64)
DEF_HELPER_FLAGS_2(fdtofx,   TCG_CALL_NO_RWG, f80, env, i64)

DEF_HELPER_FLAGS_2(fxtofs,   TCG_CALL_NO_RWG, i32, env, f80)
DEF_HELPER_FLAGS_2(fxtofd,   TCG_CALL_NO_RWG, i64, env, f80)

DEF_HELPER_FLAGS_2(istofs,   TCG_CALL_NO_RWG, i32, env, i32)
DEF_HELPER_FLAGS_2(istofd,   TCG_CALL_NO_RWG, i64, env, i32)
DEF_HELPER_FLAGS_2(istofx,   TCG_CALL_NO_RWG, f80, env, i32)

DEF_HELPER_FLAGS_2(idtofs,   TCG_CALL_NO_RWG, i32, env, i64)
DEF_HELPER_FLAGS_2(idtofd,   TCG_CALL_NO_RWG, i64, env, i64)
DEF_HELPER_FLAGS_2(idtofx,   TCG_CALL_NO_RWG, f80, env, i64)

DEF_HELPER_FLAGS_2(fstois,   TCG_CALL_NO_RWG, i32, env, i32)
DEF_HELPER_FLAGS_2(fstoid,   TCG_CALL_NO_RWG, i64, env, i32)
DEF_HELPER_FLAGS_2(fstoistr, TCG_CALL_NO_RWG, i32, env, i32)
DEF_HELPER_FLAGS_2(fstoidtr, TCG_CALL_NO_RWG, i64, env, i32)

DEF_HELPER_FLAGS_2(fdtois,   TCG_CALL_NO_RWG, i32, env, i64)
DEF_HELPER_FLAGS_2(fdtoid,   TCG_CALL_NO_RWG, i64, env, i64)
DEF_HELPER_FLAGS_2(fdtoistr, TCG_CALL_NO_RWG, i32, env, i64)
DEF_HELPER_FLAGS_2(fdtoidtr, TCG_CALL_NO_RWG, i64, env, i64)

DEF_HELPER_FLAGS_2(fxtois,   TCG_CALL_NO_RWG, i32, env, f80)
DEF_HELPER_FLAGS_2(fxtoistr, TCG_CALL_NO_RWG, i32, env, f80)
DEF_HELPER_FLAGS_2(fxtoid,   TCG_CALL_NO_RWG, i64, env, f80)
DEF_HELPER_FLAGS_2(fxtoidtr, TCG_CALL_NO_RWG, i64, env, f80)

/* Float Rounding */
DEF_HELPER_FLAGS_3(fstoifs,  TCG_CALL_NO_RWG, i32, env, i32, i32)
DEF_HELPER_FLAGS_3(fdtoifd,  TCG_CALL_NO_RWG, i64, env, i64, i64)

DEF_HELPER_FLAGS_5(stmqp5,      TCG_CALL_NO_RWG, void, env, tl, vec, i32, i32)
DEF_HELPER_FLAGS_5(stmqp6,      TCG_CALL_NO_RWG, void, env, tl, vec, i32, i32)
DEF_HELPER_FLAGS_6(stmqp_mlock, TCG_CALL_NO_RWG, i32, env, tl, vec, i32, i32, vec)

#undef f80
#undef vec
