/*
 * E2K ISA version 7 translation
 *
 * SPDX-License-Identifier: GPL-2.0-only
 */

//qpmgme,              7, 0, 0, 0, 0, 0, P,P,P,P, alf21
//qpaesltr,            0, 7, 0, 0, 0, 0, P,P,P,P, alf21
//qpaesiltr,           0, 7, 0, 0, 0, 0, P,P,P,P, alf21
//qpkuzltr,            0, 7, 0, 0, 0, 0, P,0,P,0, alf12
//qpsbgltrlo,          0, 7, 0, 0, 0, 0, P,P,P,0, alf11
//qpsbgltrhi,          0, 7, 0, 0, 0, 0, P,P,P,0, alf11
//qpaesebgn,           0, 0, 7, 0, 0, 0, P,P,P,P, alf21
//qpaesdbgn,           0, 0, 7, 0, 0, 0, P,P,P,P, alf21
//qpcm2b,              7, 7, 0, 7, 7, 0, P,0,S,0, alf12
//qpcm2h,              7, 7, 0, 7, 7, 0, P,0,S,0, alf12
//qpcm2w,              7, 7, 0, 7, 7, 0, P,0,S,0, alf12
//qpcm2d,              7, 7, 0, 7, 7, 0, P,0,S,0, alf12
//qpkuzebgn,           0, 0, 7, 0, 0, 0, P,P,P,P, alf21
//qps2cmh,             7, 7, 0, 7, 7, 0, S,P,P,0, alf11
//qps2cmw,             7, 7, 0, 7, 7, 0, S,P,P,0, alf11
//qps2cmd,             7, 7, 0, 7, 7, 0, S,P,P,0, alf11
//qps2mb,              7, 7, 0, 7, 7, 0, S,P,P,0, alf11
//qps2mh,              7, 7, 0, 7, 7, 0, S,P,P,0, alf11
//qps2mw,              7, 7, 0, 7, 7, 0, S,P,P,0, alf11
//qps2md,              7, 7, 0, 7, 7, 0, S,P,P,0, alf11
TRANSW(qpackhbss,           7, 7, 0, 7, 7, 0, P,P,P,0, alf1, _, gen_helper_qpacksshb);
TRANSW(qpackhbus,           7, 7, 0, 7, 7, 0, P,P,P,0, alf1, _, gen_helper_qpackushb);
TRANSW(qpackwhss,           7, 7, 0, 7, 7, 0, P,P,P,0, alf1, _, gen_helper_qpacksswh);
TRANSW(qpackwhus,           7, 7, 0, 7, 7, 0, P,P,P,0, alf1, _, gen_helper_qpackuswh);
//qpfstobf,            7, 7, 0, 7, 7, 0, P,P,P,0, alf11
//qpidotsbwss,         7, 7, 0, 7, 7, 0, P,P,P,P, alf21
//qpidotsbwus,         7, 7, 0, 7, 7, 0, P,P,P,P, alf21
//qpidotsbwuu,         7, 7, 0, 7, 7, 0, P,P,P,P, alf21
//qpidotshwss,         7, 7, 0, 7, 7, 0, P,P,P,P, alf21
//qpbfdots,            7, 7, 0, 7, 7, 0, P,P,P,P, alf21
//qptbl_0,             7, 7, 0, 7, 7, 0, P,P,P,P, alf21
//qptbl_1,             7, 7, 0, 7, 7, 0, P,P,P,P, alf21
//qptbl_2,             7, 7, 0, 7, 7, 0, P,P,P,P, alf21
//qptbl_3,             7, 7, 0, 7, 7, 0, P,P,P,P, alf21
//qptbl_4,             7, 7, 0, 7, 7, 0, P,P,P,P, alf21
//qptbl_5,             7, 7, 0, 7, 7, 0, P,P,P,P, alf21
//qptbl_6,             7, 7, 0, 7, 7, 0, P,P,P,P, alf21
//qptbl_7,             7, 7, 0, 7, 7, 0, P,P,P,P, alf21
//qpinss,              7, 7, 0, 7, 7, 0, P,P,S,P, alf21_lt3
//qpinsd,              7, 7, 0, 7, 7, 0, P,P,D,P, alf21_lt3
