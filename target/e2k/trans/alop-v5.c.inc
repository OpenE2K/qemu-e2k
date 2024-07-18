/*
 * E2K ISA version 5 translation
 *
 * SPDX-License-Identifier: GPL-2.0-only
 */

//ldq,                 5, 0, 5, 5, 0, 5, Q,D,D,0, alf1_mas, AF_PAIR
TRANSM(ldqp,                5, 0, 5, 5, 0, 5, P,D,D,0, alf1_mas, gen_addr_i64, gen_ld_raw_i128, MO_UO, ADDR_FLAT);
TRANSM(ldcsqp,              5, 0, 5, 5, 0, 5, P,D,D,0, alf1_mas, gen_addr_i64, gen_ld_raw_i128, MO_UO, ADDR_CS);
TRANSM(lddsqp,              5, 0, 5, 5, 0, 5, P,D,D,0, alf1_mas, gen_addr_i64, gen_ld_raw_i128, MO_UO, ADDR_DS);
TRANSM(ldesqp,              5, 0, 5, 5, 0, 5, P,D,D,0, alf1_mas, gen_addr_i64, gen_ld_raw_i128, MO_UO, ADDR_ES);
TRANSM(ldfsqp,              5, 0, 5, 5, 0, 5, P,D,D,0, alf1_mas, gen_addr_i64, gen_ld_raw_i128, MO_UO, ADDR_FS);
TRANSM(ldgsqp,              5, 0, 5, 5, 0, 5, P,D,D,0, alf1_mas, gen_addr_i64, gen_ld_raw_i128, MO_UO, ADDR_GS);
TRANSM(ldssqp,              5, 0, 5, 5, 0, 5, P,D,D,0, alf1_mas, gen_addr_i64, gen_ld_raw_i128, MO_UO, ADDR_SS);
TRANSM(ldgdqp,              5, 0, 5, 5, 0, 5, P,D,D,0, alf1_mas, gen_addr_i64, gen_ld_raw_i128, MO_UO, ADDR_GD);
//ldcudqp,             5, 0, 5, 5, 0, 5, P,D,D,0, alf1_mas
//ldapqp,              5, 0, 5, 5, 0, 5, P,D,D,0, alf1_mas
TRANSM(stqp,                0, 0, 5, 0, 0, 5, P,D,D,0, alf3_mas, gen_addr_i64, gen_st_raw_i128, MO_UO, ADDR_FLAT);
TRANSA(staaqp,              0, 0, 5, 0, 0, 5, P,0,0,0, alf10_mas, OP_STAAQP, gen_staaqp);
TRANSM(stcsqp,              0, 0, 5, 0, 0, 5, P,D,D,0, alf3_mas, gen_addr_i64, gen_st_raw_i128, MO_UO, ADDR_CS);
TRANSM(stdsqp,              0, 0, 5, 0, 0, 5, P,D,D,0, alf3_mas, gen_addr_i64, gen_st_raw_i128, MO_UO, ADDR_DS);
TRANSM(stesqp,              0, 0, 5, 0, 0, 5, P,D,D,0, alf3_mas, gen_addr_i64, gen_st_raw_i128, MO_UO, ADDR_ES);
TRANSM(stfsqp,              0, 0, 5, 0, 0, 5, P,D,D,0, alf3_mas, gen_addr_i64, gen_st_raw_i128, MO_UO, ADDR_FS);
TRANSM(stgsqp,              0, 0, 5, 0, 0, 5, P,D,D,0, alf3_mas, gen_addr_i64, gen_st_raw_i128, MO_UO, ADDR_GS);
TRANSM(stssqp,              0, 0, 5, 0, 0, 5, P,D,D,0, alf3_mas, gen_addr_i64, gen_st_raw_i128, MO_UO, ADDR_SS);
TRANSM(stgdqp,              0, 0, 5, 0, 0, 5, P,S,S,0, alf3_mas, gen_addr_i64, gen_st_raw_i128, MO_UO, ADDR_GD);
//stapqp,              0, 0, 5, 0, 0, 5, P,Q,S,0, alf3_mas
TRANSM(stmqp,               0, 0, 5, 0, 0, 5, P,D,D,0, alf3_mas, gen_addr_src1_i64, gen_stm_raw_i128, MO_UO, ADDR_FLAT);
TRANSM(stcsmqp,             0, 0, 5, 0, 0, 5, P,D,D,0, alf3_mas, gen_addr_src1_i32, gen_stm_raw_i128, MO_UO, ADDR_CS);
TRANSM(stdsmqp,             0, 0, 5, 0, 0, 5, P,D,D,0, alf3_mas, gen_addr_src1_i32, gen_stm_raw_i128, MO_UO, ADDR_DS);
TRANSM(stesmqp,             0, 0, 5, 0, 0, 5, P,D,D,0, alf3_mas, gen_addr_src1_i32, gen_stm_raw_i128, MO_UO, ADDR_ES);
TRANSM(stfsmqp,             0, 0, 5, 0, 0, 5, P,D,D,0, alf3_mas, gen_addr_src1_i32, gen_stm_raw_i128, MO_UO, ADDR_FS);
TRANSM(stgsmqp,             0, 0, 5, 0, 0, 5, P,D,D,0, alf3_mas, gen_addr_src1_i32, gen_stm_raw_i128, MO_UO, ADDR_GS);
TRANSM(stssmqp,             0, 0, 5, 0, 0, 5, P,D,D,0, alf3_mas, gen_addr_src1_i32, gen_stm_raw_i128, MO_UO, ADDR_SS);
TRANSM(stgdmqp,             0, 0, 5, 0, 0, 5, P,S,S,0, alf3_mas, gen_addr_src1_i32, gen_stm_raw_i128, MO_UO, ADDR_GD);
//stapmqp,             0, 0, 5, 0, 0, 5, P,Q,S,0, alf13_mas
//ldrqp,               5, 0, 5, 5, 0, 5, P,D,D,0, alf1_mas
//strqp,               0, 0, 5, 0, 0, 5, P,D,D,0, alf3_mas
TRANSA(puttagqp,            5, 5, 0, 5, 5, 0, P,P,S,0, alf1,  OP_PUTTAGQP, gen_puttagqp);

TRANSW(pmullw,              0, 5, 0, 0, 5, 0, D,D,D,0, alf1,  _, gen_helper_pmullw);
TRANSW(psrcw,               5, 5, 0, 5, 5, 0, D,D,D,0, alf1,  _, gen_helper_psrcw);
TRANSW(psrcd,               5, 5, 0, 5, 5, 0, D,D,D,0, alf1,  _, gen_helper_psrcd);

TRANSW(addcd,               0, 5, 0, 0, 5, 0, D,D,D,S, alf21, _, gen_addcd);
TRANSW(addcd_c,             0, 5, 0, 0, 5, 0, D,D,D,S, alf21, _, gen_addcd_c);
TRANSW(subcd,               0, 5, 0, 0, 5, 0, D,D,D,S, alf21, _, gen_subcd);
TRANSW(subcd_c,             0, 5, 0, 0, 5, 0, D,D,D,S, alf21, _, gen_subcd_c);
TRANSW(getfzs,              5, 5, 5, 5, 5, 5, S,S,S,0, alf1,  _, gen_getfzs);
TRANSW(getfzd,              5, 5, 5, 5, 5, 5, D,D,D,0, alf1,  _, gen_getfzd);
TRANSW(qpsrlw,              5, 5, 0, 5, 5, 0, P,P,D,0, alf1,  _, gen_qpsrlw);
TRANSW(qpsrlh,              5, 5, 0, 5, 5, 0, P,P,D,0, alf1,  _, gen_qpsrlh);
TRANSW(qpsraw,              5, 5, 0, 5, 5, 0, P,P,D,0, alf1,  _, gen_qpsraw);
TRANSW(qpsrah,              5, 5, 0, 5, 5, 0, P,P,D,0, alf1,  _, gen_qpsrah);
TRANSW(qpsllw,              5, 5, 0, 5, 5, 0, P,P,D,0, alf1,  _, gen_qpsllw);
TRANSW(qpsllh,              5, 5, 0, 5, 5, 0, P,P,D,0, alf1,  _, gen_qpsllh);
TRANSW(qpsrld,              5, 5, 0, 5, 5, 0, P,P,D,0, alf1,  _, gen_qpsrld);
TRANSW(qpslld,              5, 5, 0, 5, 5, 0, P,P,D,0, alf1,  _, gen_qpslld);
TRANSW(qpand,               5, 5, 0, 5, 5, 0, P,P,P,0, alf1,  _, gen_qpand);
TRANSW(qpandn,              5, 5, 0, 5, 5, 0, P,P,P,0, alf1,  _, gen_qpandn);
TRANSW(qpor,                5, 5, 0, 5, 5, 0, P,P,P,0, alf1,  _, gen_qpor);
TRANSW(qpxor,               5, 5, 0, 5, 5, 0, P,P,P,0, alf1,  _, gen_qpxor);
TRANSW(qpminuw,             5, 0, 0, 5, 0, 0, P,P,P,0, alf1,  _, gen_qpminuw);
TRANSW(qpminsw,             5, 0, 0, 5, 0, 0, P,P,P,0, alf1,  _, gen_qpminsw);
TRANSW(qpmaxuw,             5, 0, 0, 5, 0, 0, P,P,P,0, alf1,  _, gen_qpmaxuw);
TRANSW(qpmaxsw,             5, 0, 0, 5, 0, 0, P,P,P,0, alf1,  _, gen_qpmaxsw);
TRANSW(qpcmpeqd,            5, 0, 0, 5, 0, 0, P,P,P,0, alf1,  _, gen_qpcmpeqd);
TRANSW(qphaddh,             5, 0, 0, 5, 0, 0, P,P,P,0, alf1,  _, gen_helper_qphaddh);
TRANSW(qphaddw,             5, 0, 0, 5, 0, 0, P,P,P,0, alf1,  _, gen_helper_qphaddw);
TRANSW(qphaddsh,            5, 0, 0, 5, 0, 0, P,P,P,0, alf1,  _, gen_helper_qphaddsh);
TRANSW(qpcmpgtd,            5, 0, 0, 5, 0, 0, P,P,P,0, alf1,  _, gen_qpcmpgtd);
TRANSW(qphsubh,             5, 0, 0, 5, 0, 0, P,P,P,0, alf1,  _, gen_helper_qphsubh);
TRANSW(qphsubw,             5, 0, 0, 5, 0, 0, P,P,P,0, alf1,  _, gen_helper_qphsubw);
TRANSW(qphsubsh,            5, 0, 0, 5, 0, 0, P,P,P,0, alf1,  _, gen_helper_qphsubsh);
TRANSW(qpsrcw,              5, 5, 0, 5, 5, 0, P,P,D,0, alf1,  _, gen_qpsrcw);
TRANSW(qpsrcd,              5, 5, 0, 5, 5, 0, P,P,D,0, alf1,  _, gen_qpsrcd);
TRANSW(qpmsk2sgnb,          5, 5, 0, 5, 5, 0, P,P,S,0, alf1,  _, gen_helper_qpmsk2sgnb);
TRANSW(qpsignb,             5, 0, 0, 5, 0, 0, P,P,P,0, alf1,  _, gen_qpsignb);
TRANSW(qpsignh,             5, 0, 0, 5, 0, 0, P,P,P,0, alf1,  _, gen_qpsignh);
TRANSW(qpsignw,             5, 0, 0, 5, 0, 0, P,P,P,0, alf1,  _, gen_qpsignw);
TRANSW(qpsgn2mskb,          5, 5, 0, 5, 5, 0, S,0,P,0, alf2,  _, gen_helper_qpsgn2mskb);
TRANSW(qppackdl,            5, 5, 0, 5, 5, 0, P,D,D,0, alf1,  _, gen_qppackdl);
TRANSW(qpswitchw,           5, 5, 0, 5, 5, 0, P,0,P,0, alf2,  _, gen_qpswitchw);
TRANSW(qpswitchd,           5, 5, 0, 5, 5, 0, P,0,P,0, alf2,  _, gen_qpswitchd);
TRANSW(qpminub,             5, 0, 0, 5, 0, 0, P,P,P,0, alf1,  _, gen_qpminub);
TRANSW(qpminsh,             5, 0, 0, 5, 0, 0, P,P,P,0, alf1,  _, gen_qpminsh);
TRANSW(qpmaxub,             5, 0, 0, 5, 0, 0, P,P,P,0, alf1,  _, gen_qpmaxub);
TRANSW(qpmaxsh,             5, 0, 0, 5, 0, 0, P,P,P,0, alf1,  _, gen_qpmaxsh);
TRANSW(qpminsb,             5, 0, 0, 5, 0, 0, P,P,P,0, alf1,  _, gen_qpminsb);
TRANSW(qpminuh,             5, 0, 0, 5, 0, 0, P,P,P,0, alf1,  _, gen_qpminuh);
TRANSW(qpmaxsb,             5, 0, 0, 5, 0, 0, P,P,P,0, alf1,  _, gen_qpmaxsb);
TRANSW(qpmaxuh,             5, 0, 0, 5, 0, 0, P,P,P,0, alf1,  _, gen_qpmaxuh);
TRANSW(qpacksshb,           0, 5, 0, 0, 5, 0, P,P,P,0, alf1,  _, gen_qpacksshb);
TRANSW(qpaddb,              5, 0, 0, 5, 0, 0, P,P,P,0, alf1,  _, gen_qpaddb);
TRANSW(qpackushb,           0, 5, 0, 0, 5, 0, P,P,P,0, alf1,  _, gen_qpackushb);
TRANSW(qpaddh,              5, 0, 0, 5, 0, 0, P,P,P,0, alf1,  _, gen_qpaddh);
TRANSW(qpacksswh,           0, 5, 0, 0, 5, 0, P,P,P,0, alf1,  _, gen_qpacksswh);
TRANSW(qpaddsb,             5, 0, 0, 5, 0, 0, P,P,P,0, alf1,  _, gen_qpaddsb);
TRANSW(qpackuswh,           0, 5, 0, 0, 5, 0, P,P,P,0, alf1,  _, gen_qpackuswh);
TRANSW(qpaddsh,             5, 0, 0, 5, 0, 0, P,P,P,0, alf1,  _, gen_qpaddsh);
TRANSW(qpaddusb,            5, 0, 0, 5, 0, 0, P,P,P,0, alf1,  _, gen_qpaddusb);
TRANSW(qpaddush,            5, 0, 0, 5, 0, 0, P,P,P,0, alf1,  _, gen_qpaddush);
TRANSW(qpaddw,              5, 0, 0, 5, 0, 0, P,P,P,0, alf1,  _, gen_qpaddw);
TRANSW(qpaddd,              5, 0, 0, 5, 0, 0, P,P,P,0, alf1,  _, gen_qpaddd);
TRANSW(qpsubb,              5, 0, 0, 5, 0, 0, P,P,P,0, alf1,  _, gen_qpsubb);
TRANSW(qpsubh,              5, 0, 0, 5, 0, 0, P,P,P,0, alf1,  _, gen_qpsubh);
TRANSW(qpsubsb,             5, 0, 0, 5, 0, 0, P,P,P,0, alf1,  _, gen_qpsubsb);
TRANSW(qpsubsh,             5, 0, 0, 5, 0, 0, P,P,P,0, alf1,  _, gen_qpsubsh);
TRANSW(qpsubusb,            5, 0, 0, 5, 0, 0, P,P,P,0, alf1,  _, gen_qpsubusb);
TRANSW(qpsubush,            5, 0, 0, 5, 0, 0, P,P,P,0, alf1,  _, gen_qpsubush);
TRANSW(qpsubw,              5, 0, 0, 5, 0, 0, P,P,P,0, alf1,  _, gen_qpsubw);
TRANSW(qpsubd,              5, 0, 0, 5, 0, 0, P,P,P,0, alf1,  _, gen_qpsubd);
TRANSW(qpcmpeqb,            5, 0, 0, 5, 0, 0, P,P,P,0, alf1,  _, gen_qpcmpeqb);
TRANSW(qpmulhh,             0, 5, 0, 0, 5, 0, P,P,P,0, alf1,  _, gen_qpmulhh);
TRANSW(qpcmpeqh,            5, 0, 0, 5, 0, 0, P,P,P,0, alf1,  _, gen_qpcmpeqh);
TRANSW(qpmullh,             0, 5, 0, 0, 5, 0, P,P,P,0, alf1,  _, gen_qpmullh);
TRANSW(qpcmpeqw,            5, 0, 0, 5, 0, 0, P,P,P,0, alf1,  _, gen_qpcmpeqw);
TRANSW(qpmaddh,             0, 5, 0, 0, 5, 0, P,P,P,0, alf1,  _, gen_qpmaddh);
TRANSW(qpcmpgtb,            5, 0, 0, 5, 0, 0, P,P,P,0, alf1,  _, gen_qpcmpgtb);
TRANSW(qpmulhuh,            0, 5, 0, 0, 5, 0, P,P,P,0, alf1,  _, gen_qpmulhuh);
TRANSW(qpcmpgth,            5, 0, 0, 5, 0, 0, P,P,P,0, alf1,  _, gen_qpcmpgth);
TRANSW(qpsadbw,             0, 5, 0, 0, 5, 0, P,P,P,0, alf1,  _, gen_qpsadbw);
TRANSW(qpcmpgtw,            5, 0, 0, 5, 0, 0, P,P,P,0, alf1,  _, gen_qpcmpgtw);
TRANSW(qpmulubhh,           0, 5, 0, 0, 5, 0, P,D,P,0, alf1,  _, gen_helper_qpmulubhh);
TRANSW(qpavgusb,            5, 0, 0, 5, 0, 0, P,P,P,0, alf1,  _, gen_qpavgusb);
TRANSW(qpmullw,             0, 5, 0, 0, 5, 0, P,P,P,0, alf1,  _, gen_qpmullw);
TRANSW(qpavgush,            5, 0, 0, 5, 0, 0, P,P,P,0, alf1,  _, gen_qpavgush);
TRANSW(qpmaddubsh,          0, 5, 0, 0, 5, 0, P,P,P,0, alf1,  _, gen_qpmaddubsh);
TRANSW(qpmulhrsh,           0, 5, 0, 0, 5, 0, P,P,P,0, alf1,  _, gen_qpmulhrsh);
TRANSW(qphminposuh,         0, 5, 0, 0, 5, 0, D,P,P,0, alf1,  _, gen_helper_qphminposuh);
TRANSW(qpmpsadbh,           0, 5, 0, 0, 5, 0, P,P,S,0, alf1,  _, gen_helper_qpmpsadbh);
TRANSW(qpfstois,            5, 5, 0, 5, 5, 0, P,0,P,0, alf2,  _env_, gen_qpfstois);
TRANSW(qpfstoistr,          5, 5, 0, 5, 5, 0, P,0,P,0, alf2,  _env_, gen_qpfstoistr);
TRANSW(qpistofs,            5, 5, 0, 5, 5, 0, P,0,P,0, alf2,  _env_, gen_qpistofs);
TRANSW(qpfstoid,            5, 5, 0, 5, 5, 0, P,0,D,0, alf2,  _env_, gen_helper_qpfstoid);
TRANSW(qpfstoidtr,          5, 5, 0, 5, 5, 0, P,0,D,0, alf2,  _env_, gen_helper_qpfstoidtr);
TRANSW(qpistofd,            5, 5, 0, 5, 5, 0, P,0,D,0, alf2,  _env_, gen_helper_qpistofd);
TRANSW(qpfstofd,            5, 5, 0, 5, 5, 0, P,0,D,0, alf2,  _env_, gen_helper_qpfstofd);
TRANSW(qpfdtois,            5, 5, 0, 5, 5, 0, D,0,P,0, alf2,  _env_, gen_helper_qpfdtois);
TRANSW(qpfdtoistr,          5, 5, 0, 5, 5, 0, D,0,P,0, alf2,  _env_, gen_helper_qpfdtoistr);
TRANSW(qpidtofs,            5, 5, 0, 5, 5, 0, D,0,P,0, alf2,  _env_, gen_helper_qpidtofs);
TRANSW(qpfdtofs,            5, 5, 0, 5, 5, 0, D,0,P,0, alf2,  _env_, gen_helper_qpfdtofs);
TRANSW(qpfdtoid,            5, 5, 0, 5, 5, 0, P,0,P,0, alf2,  _env_, gen_qpfdtoid);
TRANSW(qpfdtoidtr,          5, 5, 0, 5, 5, 0, P,0,P,0, alf2,  _env_, gen_qpfdtoidtr);
TRANSW(qpidtofd,            5, 5, 0, 5, 5, 0, P,0,P,0, alf2,  _env_, gen_qpidtofd);
TRANSW(qpfstoifs,           5, 5, 0, 5, 5, 0, P,D,P,0, alf1,  _env_, gen_qpfstoifs);
TRANSW(qpfdtoifd,           5, 5, 0, 5, 5, 0, P,D,P,0, alf1,  _env_, gen_qpfdtoifd);
TRANSW(qpfadds,             5, 5, 5, 5, 5, 5, P,P,P,0, alf1,  _env_, gen_qpfadds);
TRANSW(qpfaddd,             5, 5, 5, 5, 5, 5, P,P,P,0, alf1,  _env_, gen_qpfaddd);
TRANSW(qpfsubs,             5, 5, 5, 5, 5, 5, P,P,P,0, alf1,  _env_, gen_qpfsubs);
TRANSW(qpfsubd,             5, 5, 5, 5, 5, 5, P,P,P,0, alf1,  _env_, gen_qpfsubd);
TRANSW(qpfmins,             5, 5, 0, 5, 5, 0, P,P,P,0, alf1,  _env_, gen_qpfmins);
TRANSW(qpfmind,             5, 5, 0, 5, 5, 0, P,P,P,0, alf1,  _env_, gen_qpfmind);
TRANSW(qpfmaxs,             5, 5, 0, 5, 5, 0, P,P,P,0, alf1,  _env_, gen_qpfmaxs);
TRANSW(qpfmaxd,             5, 5, 0, 5, 5, 0, P,P,P,0, alf1,  _env_, gen_qpfmaxd);
TRANSW(qpfmuls,             5, 5, 5, 5, 5, 5, P,P,P,0, alf1,  _env_, gen_qpfmuls);
TRANSW(qpfmuld,             5, 5, 5, 5, 5, 5, P,P,P,0, alf1,  _env_, gen_qpfmuld);
TRANSW(qpfcmpeqs,           5, 5, 0, 5, 5, 0, P,P,P,0, alf1,  _env_, gen_qpfcmpeqs);
TRANSW(qpfcmplts,           5, 5, 0, 5, 5, 0, P,P,P,0, alf1,  _env_, gen_qpfcmplts);
TRANSW(qpfcmples,           5, 5, 0, 5, 5, 0, P,P,P,0, alf1,  _env_, gen_qpfcmples);
TRANSW(qpfcmpuods,          5, 5, 0, 5, 5, 0, P,P,P,0, alf1,  _env_, gen_qpfcmpuods);
TRANSW(qpfcmpneqs,          5, 5, 0, 5, 5, 0, P,P,P,0, alf1,  _env_, gen_qpfcmpneqs);
TRANSW(qpfcmpnlts,          5, 5, 0, 5, 5, 0, P,P,P,0, alf1,  _env_, gen_qpfcmpnlts);
TRANSW(qpfcmpnles,          5, 5, 0, 5, 5, 0, P,P,P,0, alf1,  _env_, gen_qpfcmpnles);
TRANSW(qpfcmpods,           5, 5, 0, 5, 5, 0, P,P,P,0, alf1,  _env_, gen_qpfcmpods);
TRANSW(qpfcmpeqd,           5, 5, 0, 5, 5, 0, P,P,P,0, alf1,  _env_, gen_qpfcmpeqd);
TRANSW(qpfcmpltd,           5, 5, 0, 5, 5, 0, P,P,P,0, alf1,  _env_, gen_qpfcmpltd);
TRANSW(qpfcmpled,           5, 5, 0, 5, 5, 0, P,P,P,0, alf1,  _env_, gen_qpfcmpled);
TRANSW(qpfcmpuodd,          5, 5, 0, 5, 5, 0, P,P,P,0, alf1,  _env_, gen_qpfcmpuodd);
TRANSW(qpfcmpneqd,          5, 5, 0, 5, 5, 0, P,P,P,0, alf1,  _env_, gen_qpfcmpneqd);
TRANSW(qpfcmpnltd,          5, 5, 0, 5, 5, 0, P,P,P,0, alf1,  _env_, gen_qpfcmpnltd);
TRANSW(qpfcmpnled,          5, 5, 0, 5, 5, 0, P,P,P,0, alf1,  _env_, gen_qpfcmpnled);
TRANSW(qpfcmpodd,           5, 5, 0, 5, 5, 0, P,P,P,0, alf1,  _env_, gen_qpfcmpodd);
TRANSW(qpfhadds,            5, 5, 5, 5, 5, 5, P,P,P,0, alf1,  _env_, gen_helper_qpfhadds);
TRANSW(qpfhsubs,            5, 5, 5, 5, 5, 5, P,P,P,0, alf1,  _env_, gen_helper_qpfhsubs);
TRANSW(qpfaddsubs,          5, 5, 5, 5, 5, 5, P,P,P,0, alf1,  _env_, gen_qpfaddsubs);
TRANSW(qpfaddsubd,          5, 5, 5, 5, 5, 5, P,P,P,0, alf1,  _env_, gen_qpfaddsubd);
//plog_and,            5, 5, 0, 5, 5, 0, D,D,D,D, alf21, AF_ALIAS
//plog_xor,            5, 5, 0, 5, 5, 0, D,D,D,D, alf21, AF_ALIAS
//plog_sel3,           5, 5, 0, 5, 5, 0, D,D,D,D, alf21, AF_ALIAS
//plog_mjr,            5, 5, 0, 5, 5, 0, D,D,D,D, alf21, AF_ALIAS
//plog_or,             5, 5, 0, 5, 5, 0, D,D,D,D, alf21, AF_ALIAS
TRANSW(plog,                5, 5, 0, 5, 5, 0, D,D,D,D, alf21_log, _, gen_plog);
//qplog_and,           5, 5, 7, 5, 5, 7, P,P,P,P, alf21_lt3, AF_ALIAS
//qplog_xor,           5, 5, 7, 5, 5, 7, P,P,P,P, alf21_lt3, AF_ALIAS
//qplog_sel3,          5, 5, 7, 5, 5, 7, P,P,P,P, alf21_lt3, AF_ALIAS
//qplog_mjr,           5, 5, 7, 5, 5, 7, P,P,P,P, alf21_lt3, AF_ALIAS
//qplog_or,            5, 5, 7, 5, 5, 7, P,P,P,P, alf21_lt3, AF_ALIAS
TRANSW(qplog,               5, 5, 7, 5, 5, 7, P,P,P,P, alf21_log, _, gen_qplog);
TRANSW(qppermb,             5, 5, 0, 5, 5, 0, P,P,P,P, alf21, _, gen_helper_qppermb);
TRANSW(qpshufb,             5, 5, 0, 5, 5, 0, P,P,P,P, alf21, _, gen_helper_qpshufb);
TRANSW(qpmerge,             5, 5, 0, 5, 5, 0, P,P,P,P, alf21, _, gen_qpmerge);

TRANSC(qpfadd_adds,         5, 5, 5, 5, 5, 5, P,P,P,P, alf21, gen_qpfcomb_i32, FCOMB_ADD,    FCOMB_ADD);
TRANSC(qpfadd_addd,         5, 5, 5, 5, 5, 5, P,P,P,P, alf21, gen_qpfcomb_i64, FCOMB_ADD,    FCOMB_ADD);
TRANSC(qpfsub_adds,         5, 5, 5, 5, 5, 5, P,P,P,P, alf21, gen_qpfcomb_i32, FCOMB_SUB,    FCOMB_ADD);
TRANSC(qpfsub_addd,         5, 5, 5, 5, 5, 5, P,P,P,P, alf21, gen_qpfcomb_i64, FCOMB_SUB,    FCOMB_ADD);
TRANSC(qpfhadd_adds,        5, 5, 5, 5, 5, 5, P,P,P,P, alf21, gen_qpfcomb_i32, FCOMB_HADD,   FCOMB_ADD);
TRANSC(qpfhsub_adds,        5, 5, 5, 5, 5, 5, P,P,P,P, alf21, gen_qpfcomb_i32, FCOMB_HSUB,   FCOMB_ADD);
TRANSC(qpfmul_adds,         5, 5, 5, 5, 5, 5, P,P,P,P, alf21, gen_qpfcomb_i32, FCOMB_MUL,    FCOMB_ADD);
TRANSC(qpfmul_addd,         5, 5, 5, 5, 5, 5, P,P,P,P, alf21, gen_qpfcomb_i64, FCOMB_MUL,    FCOMB_ADD);
TRANSC(qpfaddsub_adds,      5, 5, 5, 5, 5, 5, P,P,P,P, alf21, gen_qpfcomb_i32, FCOMB_ADDSUB, FCOMB_ADD);
TRANSC(qpfaddsub_addd,      5, 5, 5, 5, 5, 5, P,P,P,P, alf21, gen_qpfcomb_i64, FCOMB_ADDSUB, FCOMB_ADD);
TRANSC(qpfadd_subs,         5, 5, 5, 5, 5, 5, P,P,P,P, alf21, gen_qpfcomb_i32, FCOMB_ADD,    FCOMB_SUB);
TRANSC(qpfadd_subd,         5, 5, 5, 5, 5, 5, P,P,P,P, alf21, gen_qpfcomb_i64, FCOMB_ADD,    FCOMB_SUB);
TRANSC(qpfsub_subs,         5, 5, 5, 5, 5, 5, P,P,P,P, alf21, gen_qpfcomb_i32, FCOMB_SUB,    FCOMB_SUB);
TRANSC(qpfsub_subd,         5, 5, 5, 5, 5, 5, P,P,P,P, alf21, gen_qpfcomb_i64, FCOMB_SUB,    FCOMB_SUB);
TRANSC(qpfhadd_subs,        5, 5, 5, 5, 5, 5, P,P,P,P, alf21, gen_qpfcomb_i32, FCOMB_HADD,   FCOMB_SUB);
TRANSC(qpfhsub_subs,        5, 5, 5, 5, 5, 5, P,P,P,P, alf21, gen_qpfcomb_i32, FCOMB_HSUB,   FCOMB_SUB);
TRANSC(qpfmul_subs,         5, 5, 5, 5, 5, 5, P,P,P,P, alf21, gen_qpfcomb_i32, FCOMB_MUL,    FCOMB_SUB);
TRANSC(qpfmul_subd,         5, 5, 5, 5, 5, 5, P,P,P,P, alf21, gen_qpfcomb_i64, FCOMB_MUL,    FCOMB_SUB);
TRANSC(qpfaddsub_subs,      5, 5, 5, 5, 5, 5, P,P,P,P, alf21, gen_qpfcomb_i32, FCOMB_ADDSUB, FCOMB_SUB);
TRANSC(qpfaddsub_subd,      5, 5, 5, 5, 5, 5, P,P,P,P, alf21, gen_qpfcomb_i64, FCOMB_ADDSUB, FCOMB_SUB);
TRANSC(qpfadd_hadds,        5, 5, 5, 5, 5, 5, P,P,P,P, alf21, gen_qpfcomb_i32, FCOMB_ADD,    FCOMB_HADD);
TRANSC(qpfsub_hadds,        5, 5, 5, 5, 5, 5, P,P,P,P, alf21, gen_qpfcomb_i32, FCOMB_SUB,    FCOMB_HADD);
TRANSC(qpfhadd_hadds,       5, 5, 5, 5, 5, 5, P,P,P,P, alf21, gen_qpfcomb_i32, FCOMB_HADD,   FCOMB_HADD);
TRANSC(qpfhsub_hadds,       5, 5, 5, 5, 5, 5, P,P,P,P, alf21, gen_qpfcomb_i32, FCOMB_HSUB,   FCOMB_HADD);
TRANSC(qpfmul_hadds,        5, 5, 5, 5, 5, 5, P,P,P,P, alf21, gen_qpfcomb_i32, FCOMB_MUL,    FCOMB_HADD);
TRANSC(qpfaddsub_hadds,     5, 5, 5, 5, 5, 5, P,P,P,P, alf21, gen_qpfcomb_i32, FCOMB_ADDSUB, FCOMB_HADD);
TRANSC(qpfadd_hsubs,        5, 5, 5, 5, 5, 5, P,P,P,P, alf21, gen_qpfcomb_i32, FCOMB_ADD,    FCOMB_HSUB);
TRANSC(qpfsub_hsubs,        5, 5, 5, 5, 5, 5, P,P,P,P, alf21, gen_qpfcomb_i32, FCOMB_SUB,    FCOMB_HSUB);
TRANSC(qpfhadd_hsubs,       5, 5, 5, 5, 5, 5, P,P,P,P, alf21, gen_qpfcomb_i32, FCOMB_HADD,   FCOMB_HSUB);
TRANSC(qpfhsub_hsubs,       5, 5, 5, 5, 5, 5, P,P,P,P, alf21, gen_qpfcomb_i32, FCOMB_HSUB,   FCOMB_HSUB);
TRANSC(qpfmul_hsubs,        5, 5, 5, 5, 5, 5, P,P,P,P, alf21, gen_qpfcomb_i32, FCOMB_MUL,    FCOMB_HSUB);
TRANSC(qpfaddsub_hsubs,     5, 5, 5, 5, 5, 5, P,P,P,P, alf21, gen_qpfcomb_i32, FCOMB_ADDSUB, FCOMB_HSUB);
TRANSC(qpfadd_rsubs,        5, 5, 5, 5, 5, 5, P,P,P,P, alf21, gen_qpfcomb_i32, FCOMB_ADD,    FCOMB_RSUB);
TRANSC(qpfadd_rsubd,        5, 5, 5, 5, 5, 5, P,P,P,P, alf21, gen_qpfcomb_i64, FCOMB_ADD,    FCOMB_RSUB);
TRANSC(qpfsub_rsubs,        5, 5, 5, 5, 5, 5, P,P,P,P, alf21, gen_qpfcomb_i32, FCOMB_SUB,    FCOMB_RSUB);
TRANSC(qpfsub_rsubd,        5, 5, 5, 5, 5, 5, P,P,P,P, alf21, gen_qpfcomb_i64, FCOMB_SUB,    FCOMB_RSUB);
TRANSC(qpfhadd_rsubs,       5, 5, 5, 5, 5, 5, P,P,P,P, alf21, gen_qpfcomb_i32, FCOMB_HADD,   FCOMB_RSUB);
TRANSC(qpfhsub_rsubs,       5, 5, 5, 5, 5, 5, P,P,P,P, alf21, gen_qpfcomb_i32, FCOMB_HSUB,   FCOMB_RSUB);
TRANSC(qpfmul_rsubs,        5, 5, 5, 5, 5, 5, P,P,P,P, alf21, gen_qpfcomb_i32, FCOMB_MUL,    FCOMB_RSUB);
TRANSC(qpfmul_rsubd,        5, 5, 5, 5, 5, 5, P,P,P,P, alf21, gen_qpfcomb_i64, FCOMB_MUL,    FCOMB_RSUB);
TRANSC(qpfaddsub_rsubs,     5, 5, 5, 5, 5, 5, P,P,P,P, alf21, gen_qpfcomb_i32, FCOMB_ADDSUB, FCOMB_RSUB);
TRANSC(qpfaddsub_rsubd,     5, 5, 5, 5, 5, 5, P,P,P,P, alf21, gen_qpfcomb_i64, FCOMB_ADDSUB, FCOMB_RSUB);
TRANSC(qpfadd_addsubs,      5, 5, 5, 5, 5, 5, P,P,P,P, alf21, gen_qpfcomb_i32, FCOMB_ADD,    FCOMB_ADDSUB);
TRANSC(qpfadd_addsubd,      5, 5, 5, 5, 5, 5, P,P,P,P, alf21, gen_qpfcomb_i64, FCOMB_ADD,    FCOMB_ADDSUB);
TRANSC(qpfsub_addsubs,      5, 5, 5, 5, 5, 5, P,P,P,P, alf21, gen_qpfcomb_i32, FCOMB_SUB,    FCOMB_ADDSUB);
TRANSC(qpfsub_addsubd,      5, 5, 5, 5, 5, 5, P,P,P,P, alf21, gen_qpfcomb_i64, FCOMB_SUB,    FCOMB_ADDSUB);
TRANSC(qpfhadd_addsubs,     5, 5, 5, 5, 5, 5, P,P,P,P, alf21, gen_qpfcomb_i32, FCOMB_HADD,   FCOMB_ADDSUB);
TRANSC(qpfhsub_addsubs,     5, 5, 5, 5, 5, 5, P,P,P,P, alf21, gen_qpfcomb_i32, FCOMB_HSUB,   FCOMB_ADDSUB);
TRANSC(qpfmul_addsubs,      5, 5, 5, 5, 5, 5, P,P,P,P, alf21, gen_qpfcomb_i32, FCOMB_MUL,    FCOMB_ADDSUB);
TRANSC(qpfmul_addsubd,      5, 5, 5, 5, 5, 5, P,P,P,P, alf21, gen_qpfcomb_i64, FCOMB_MUL,    FCOMB_ADDSUB);
TRANSC(qpfaddsub_addsubs,   5, 5, 5, 5, 5, 5, P,P,P,P, alf21, gen_qpfcomb_i32, FCOMB_ADDSUB, FCOMB_ADDSUB);
TRANSC(qpfaddsub_addsubd,   5, 5, 5, 5, 5, 5, P,P,P,P, alf21, gen_qpfcomb_i64, FCOMB_ADDSUB, FCOMB_ADDSUB);
