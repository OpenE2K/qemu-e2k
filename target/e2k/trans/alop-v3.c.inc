TRANSW(umulhd,     3, 3, 0, 3, 3, 0, D,D,D,0, alf1, _,     gen_umulhd);
TRANSW(smulhd,     3, 3, 0, 3, 3, 0, D,D,D,0, alf1, _,     gen_smulhd);
TRANSW(pminsb,     3, 0, 0, 3, 0, 0, D,D,D,0, alf1, _,     gen_helper_pminsb);
TRANSW(pminuh,     3, 0, 0, 3, 0, 0, D,D,D,0, alf1, _,     gen_helper_pminuh);
TRANSW(pmaxsb,     3, 0, 0, 3, 0, 0, D,D,D,0, alf1, _,     gen_helper_pmaxsb);
TRANSW(pmaxuh,     3, 0, 0, 3, 0, 0, D,D,D,0, alf1, _,     gen_helper_pmaxuh);
TRANSW(pcmpeqd,    3, 0, 0, 3, 0, 0, D,D,D,0, alf1, _,     gen_helper_pcmpeqd);
TRANSW(pcmpgtd,    3, 0, 0, 3, 0, 0, D,D,D,0, alf1, _,     gen_helper_pcmpgtd);
TRANSW(mpsadbh,    0, 3, 0, 0, 3, 0, D,D,D,0, alf1, _,     gen_helper_mpsadbh);
TRANSW(pmaddubsh,  0, 3, 0, 0, 3, 0, D,D,D,0, alf1, _,     gen_helper_pmaddubsh);
TRANSW(pmulhrsh,   0, 3, 0, 0, 3, 0, D,D,D,0, alf1, _,     gen_helper_pmulhrsh);
TRANSW(phminposuh, 0, 3, 0, 0, 3, 0, D,D,D,0, alf1, _,     gen_helper_phminposuh);
TRANSW(packuswh,   0, 3, 0, 0, 3, 0, D,D,D,0, alf1, _,     gen_helper_packuswh);
TRANSW(fstoifs,    3, 3, 0, 3, 3, 0, S,S,S,0, alf1, _env_, gen_helper_fstoifs);
TRANSW(fdtoifd,    3, 3, 0, 3, 3, 0, D,D,D,0, alf1, _env_, gen_helper_fdtoifd);
TRANSW(pfstoifs,   3, 3, 0, 3, 3, 0, D,D,D,0, alf1, _env_, gen_helper_pfstoifs);
TRANSW(pfdtoifd,   3, 3, 0, 3, 3, 0, D,D,D,0, alf1, _env_, gen_helper_fdtoifd);
TRANSW(pfhadds,    3, 3, 4, 3, 3, 4, D,D,D,0, alf1, _env_, gen_helper_pfhadds);
TRANSW(pfhsubs,    3, 3, 4, 3, 3, 4, D,D,D,0, alf1, _env_, gen_helper_pfhsubs);
TRANSW(pfaddsubs,  3, 3, 4, 3, 3, 4, D,D,D,0, alf1, _env_, gen_helper_pfaddsubs);
TRANSW(pminuw,     3, 0, 0, 3, 0, 0, D,D,D,0, alf1, _,     gen_helper_pminuw);
TRANSW(pminsw,     3, 0, 0, 3, 0, 0, D,D,D,0, alf1, _,     gen_helper_pminsw);
TRANSW(pmaxuw,     3, 0, 0, 3, 0, 0, D,D,D,0, alf1, _,     gen_helper_pmaxuw);
TRANSW(pmaxsw,     3, 0, 0, 3, 0, 0, D,D,D,0, alf1, _,     gen_helper_pmaxsw);
TRANSW(phaddh,     3, 0, 0, 3, 0, 0, D,D,D,0, alf1, _,     gen_helper_phaddh);
TRANSW(phaddw,     3, 0, 0, 3, 0, 0, D,D,D,0, alf1, _,     gen_helper_phaddw);
TRANSW(phaddsh,    3, 0, 0, 3, 0, 0, D,D,D,0, alf1, _,     gen_helper_phaddsh);
TRANSW(phsubh,     3, 0, 0, 3, 0, 0, D,D,D,0, alf1, _,     gen_helper_phsubh);
TRANSW(phsubw,     3, 0, 0, 3, 0, 0, D,D,D,0, alf1, _,     gen_helper_phsubw);
TRANSW(phsubsh,    3, 0, 0, 3, 0, 0, D,D,D,0, alf1, _,     gen_helper_phsubsh);
TRANSW(psignb,     3, 0, 0, 3, 0, 0, D,D,D,0, alf1, _,     gen_helper_psignb);
TRANSW(psignh,     3, 0, 0, 3, 0, 0, D,D,D,0, alf1, _,     gen_helper_psignh);
TRANSW(psignw,     3, 0, 0, 3, 0, 0, D,D,D,0, alf1, _,     gen_helper_psignw);

//puttst,              0, 0, 0, 3, 0, 0, D,0,D,0, alf12

//andd_fd,             3, 0, 0, 3, 0, 0, S,D,D,0, alf11
//andnd_fd,            3, 0, 0, 3, 0, 0, S,D,D,0, alf11
//ord_fd,              3, 0, 0, 3, 0, 0, S,D,D,0, alf11
//ornd_fd,             3, 0, 0, 3, 0, 0, S,D,D,0, alf11
//xord_fd,             3, 0, 0, 3, 0, 0, S,D,D,0, alf11
//xornd_fd,            3, 0, 0, 3, 0, 0, S,D,D,0, alf11
//addd_fd,             3, 0, 0, 3, 0, 0, S,D,D,0, alf11
//subd_fd,             3, 0, 0, 3, 0, 0, S,D,D,0, alf11
//scld_fd,             3, 0, 0, 3, 0, 0, S,D,D,0, alf21
//scrd_fd,             3, 0, 0, 3, 0, 0, S,D,D,0, alf21
//shld_fd,             3, 0, 0, 3, 0, 0, S,D,D,0, alf21
//shrd_fd,             3, 0, 0, 3, 0, 0, S,D,D,0, alf21
//sard_fd,             3, 0, 0, 3, 0, 0, S,D,D,0, alf21
//incd_fd,             3, 0, 0, 3, 0, 0, S,D,D,0, alf21
//decd_fd,             3, 0, 0, 3, 0, 0, S,D,D,0, alf21

TRANSC(pfhadd_adds,      3, 3, 4, 3, 3, 4, D,D,D,D, alf21, gen_pfcomb_i32, FCOMB_HADD,   FCOMB_ADD);
TRANSC(pfhsub_adds,      3, 3, 4, 3, 3, 4, D,D,D,D, alf21, gen_pfcomb_i32, FCOMB_HSUB,   FCOMB_ADD);
TRANSC(pfaddsub_adds,    3, 3, 4, 3, 3, 4, D,D,D,D, alf21, gen_pfcomb_i32, FCOMB_ADDSUB, FCOMB_ADD);
TRANSC(pfhadd_subs,      3, 3, 4, 3, 3, 4, D,D,D,D, alf21, gen_pfcomb_i32, FCOMB_HADD,   FCOMB_SUB);
TRANSC(pfhsub_subs,      3, 3, 4, 3, 3, 4, D,D,D,D, alf21, gen_pfcomb_i32, FCOMB_HSUB,   FCOMB_SUB);
TRANSC(pfaddsub_subs,    3, 3, 4, 3, 3, 4, D,D,D,D, alf21, gen_pfcomb_i32, FCOMB_ADDSUB, FCOMB_SUB);
TRANSC(pfadd_hadds,      3, 3, 4, 3, 3, 4, D,D,D,D, alf21, gen_pfcomb_i32, FCOMB_ADD,    FCOMB_HADD);
TRANSC(pfsub_hadds,      3, 3, 4, 3, 3, 4, D,D,D,D, alf21, gen_pfcomb_i32, FCOMB_SUB,    FCOMB_HADD);
TRANSC(pfhadd_hadds,     3, 3, 4, 3, 3, 4, D,D,D,D, alf21, gen_pfcomb_i32, FCOMB_HADD,   FCOMB_HADD);
TRANSC(pfhsub_hadds,     3, 3, 4, 3, 3, 4, D,D,D,D, alf21, gen_pfcomb_i32, FCOMB_HSUB,   FCOMB_HADD);
TRANSC(pfmul_hadds,      3, 3, 4, 3, 3, 4, D,D,D,D, alf21, gen_pfcomb_i32, FCOMB_MUL,    FCOMB_HADD);
TRANSC(pfaddsub_hadds,   3, 3, 4, 3, 3, 4, D,D,D,D, alf21, gen_pfcomb_i32, FCOMB_ADDSUB, FCOMB_HADD);
TRANSC(pfadd_hsubs,      3, 3, 4, 3, 3, 4, D,D,D,D, alf21, gen_pfcomb_i32, FCOMB_ADD,    FCOMB_HSUB);
TRANSC(pfsub_hsubs,      3, 3, 4, 3, 3, 4, D,D,D,D, alf21, gen_pfcomb_i32, FCOMB_SUB,    FCOMB_HSUB);
TRANSC(pfhadd_hsubs,     3, 3, 4, 3, 3, 4, D,D,D,D, alf21, gen_pfcomb_i32, FCOMB_HADD,   FCOMB_HSUB);
TRANSC(pfhsub_hsubs,     3, 3, 4, 3, 3, 4, D,D,D,D, alf21, gen_pfcomb_i32, FCOMB_HSUB,   FCOMB_HSUB);
TRANSC(pfmul_hsubs,      3, 3, 4, 3, 3, 4, D,D,D,D, alf21, gen_pfcomb_i32, FCOMB_MUL,    FCOMB_HSUB);
TRANSC(pfaddsub_hsubs,   3, 3, 4, 3, 3, 4, D,D,D,D, alf21, gen_pfcomb_i32, FCOMB_ADDSUB, FCOMB_HSUB);
TRANSC(pfhadd_rsubs,     3, 3, 4, 3, 3, 4, D,D,D,D, alf21, gen_pfcomb_i32, FCOMB_HADD,   FCOMB_RSUB);
TRANSC(pfhsub_rsubs,     3, 3, 4, 3, 3, 4, D,D,D,D, alf21, gen_pfcomb_i32, FCOMB_HSUB,   FCOMB_RSUB);
TRANSC(pfaddsub_rsubs,   3, 3, 4, 3, 3, 4, D,D,D,D, alf21, gen_pfcomb_i32, FCOMB_ADDSUB, FCOMB_RSUB);
TRANSC(pfadd_addsubs,    3, 3, 4, 3, 3, 4, D,D,D,D, alf21, gen_pfcomb_i32, FCOMB_ADD,    FCOMB_ADDSUB);
TRANSC(pfsub_addsubs,    3, 3, 4, 3, 3, 4, D,D,D,D, alf21, gen_pfcomb_i32, FCOMB_SUB,    FCOMB_ADDSUB);
TRANSC(pfhadd_addsubs,   3, 3, 4, 3, 3, 4, D,D,D,D, alf21, gen_pfcomb_i32, FCOMB_HADD,   FCOMB_ADDSUB);
TRANSC(pfhsub_addsubs,   3, 3, 4, 3, 3, 4, D,D,D,D, alf21, gen_pfcomb_i32, FCOMB_HSUB,   FCOMB_ADDSUB);
TRANSC(pfmul_addsubs,    3, 3, 4, 3, 3, 4, D,D,D,D, alf21, gen_pfcomb_i32, FCOMB_MUL,    FCOMB_ADDSUB);
TRANSC(pfaddsub_addsubs, 3, 3, 4, 3, 3, 4, D,D,D,D, alf21, gen_pfcomb_i32, FCOMB_ADDSUB, FCOMB_ADDSUB);
