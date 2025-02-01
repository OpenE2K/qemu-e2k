/*
 * E2K ISA version 4 translation
 *
 * SPDX-License-Identifier: GPL-2.0-only
 */

TRANSW(fscales,   0, 4, 0, 0, 4, 0, S,S,S,0, alf1, _env_, gen_helper_fscales);
TRANSW(fscaled,   0, 4, 0, 0, 4, 0, D,D,S,0, alf1, _env_, gen_helper_fscaled);
TRANSW(fxscalesx, 0, 4, 0, 0, 4, 0, X,X,S,0, alf1, _env_, gen_helper_fxscalesx);
