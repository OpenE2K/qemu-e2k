#ifndef EXEC_H
#define EXEC_H

/*****************************************************************************/
/* Wrappers for 64-bit registers */
/*****************************************************************************/

#define EXEC_R_0134(INSN, RES, SRC1) asm( \
    "\t{\n" \
    "\t    " #INSN ",0 %[src1], %0\n" \
    "\t    " #INSN ",1 %[src1], %1\n" \
    "\t    " #INSN ",3 %[src1], %2\n" \
    "\t    " #INSN ",4 %[src1], %3\n" \
    "\t}" \
    : "+r"(RES[0]), \
      "+r"(RES[1]), \
      "+r"(RES[2]), \
      "+r"(RES[3]) \
    : [src1]"r"(SRC1) \
)

#define EXEC_R_14(INSN, RES, SRC1) asm( \
    "\t{\n" \
    "\t    " #INSN ",1 %[src1], %0\n" \
    "\t    " #INSN ",4 %[src1], %1\n" \
    "\t}" \
    : "+r"(RES[0]), \
      "+r"(RES[1])  \
    : [src1]"r"(SRC1) \
)

#define EXEC_R_5(INSN, RES, SRC1) asm( \
    "\t{\n" \
    "\t    " #INSN ",5 %[src1], %0\n" \
    "\t}" \
    : "+r"(RES[0]) \
    : [src1]"r"(SRC1) \
)

#define EXEC_RR_03(INSN, RES, SRC1, SRC2) asm( \
    "\t{\n" \
    "\t    " #INSN ",0 %[src1], %[src2], %0\n" \
    "\t    " #INSN ",3 %[src1], %[src2], %1\n" \
    "\t}" \
    : "+r"(RES[0]), \
      "+r"(RES[1])  \
    : [src1]"r"(SRC1), \
      [src2]"r"(SRC2)  \
)

#define EXEC_RR_14(INSN, RES, SRC1, SRC2) asm( \
    "\t{\n" \
    "\t    " #INSN ",1 %[src1], %[src2], %0\n" \
    "\t    " #INSN ",4 %[src1], %[src2], %1\n" \
    "\t}" \
    : "+r"(RES[0]), \
      "+r"(RES[1])  \
    : [src1]"r"(SRC1), \
      [src2]"r"(SRC2)  \
)

#define EXEC_RR_25(INSN, RES, SRC1, SRC2) asm( \
    "\t{\n" \
    "\t    " #INSN ",2 %[src1], %[src2], %0\n" \
    "\t    " #INSN ",5 %[src1], %[src2], %1\n" \
    "\t}" \
    : "+r"(RES[0]), \
      "+r"(RES[1])  \
    : [src1]"r"(SRC1), \
      [src2]"r"(SRC2)  \
)

#define EXEC_RI_14(INSN, RES, SRC1, SRC2) asm( \
    "\t{\n" \
    "\t    " #INSN ",1 %[src1], %[src2], %0\n" \
    "\t    " #INSN ",4 %[src1], %[src2], %1\n" \
    "\t}" \
    : "+r"(RES[0]), \
      "+r"(RES[1])  \
    : [src1]"r"(SRC1), \
      [src2]"i"(SRC2)  \
)

#define EXEC_RR_0134(INSN, RES, SRC1, SRC2) asm( \
    "\t{\n" \
    "\t    " #INSN ",0 %[src1], %[src2], %0\n" \
    "\t    " #INSN ",1 %[src1], %[src2], %1\n" \
    "\t    " #INSN ",3 %[src1], %[src2], %2\n" \
    "\t    " #INSN ",4 %[src1], %[src2], %3\n" \
    "\t}" \
    : "+r"(RES[0]), \
      "+r"(RES[1]), \
      "+r"(RES[2]), \
      "+r"(RES[3])  \
    : [src1]"r"(SRC1), \
      [src2]"r"(SRC2)  \
)

#define EXEC_CMP_0134(INSN, RES, SRC1, SRC2) asm( \
    "\t{\n" \
    "\t    " #INSN ",0 %[src1], %[src2], %%pred0\n" \
    "\t    " #INSN ",1 %[src1], %[src2], %%pred1\n" \
    "\t    " #INSN ",3 %[src1], %[src2], %%pred2\n" \
    "\t    " #INSN ",4 %[src1], %[src2], %%pred3\n" \
    "\t}\n" \
    "\t{\n" \
    "\t    merged,0 0, 1, %0, %%pred0\n" \
    "\t    merged,1 0, 1, %1, %%pred1\n" \
    "\t    merged,3 0, 1, %2, %%pred2\n" \
    "\t    merged,4 0, 1, %3, %%pred3\n" \
    "\t}" \
    : "+r"(RES[0]), \
      "+r"(RES[1]), \
      "+r"(RES[2]), \
      "+r"(RES[3])  \
    : [src1]"rI"(SRC1), \
      [src2]"ri"(SRC2)  \
    : "pred0", "pred1", "pred2", "pred3" \
)

#define EXEC2_012345(INSN, RES, C1, SRC1, C2, SRC2) asm( \
    "\t{\n" \
    "\t    " #INSN ",0 %[src1], %[src2], %0\n" \
    "\t    " #INSN ",1 %[src1], %[src2], %1\n" \
    "\t    " #INSN ",2 %[src1], %[src2], %2\n" \
    "\t    " #INSN ",3 %[src1], %[src2], %3\n" \
    "\t    " #INSN ",4 %[src1], %[src2], %4\n" \
    "\t    " #INSN ",5 %[src1], %[src2], %5\n" \
    "\t}" \
    : "+r"(RES[0]), \
      "+r"(RES[1]), \
      "+r"(RES[2]), \
      "+r"(RES[3]), \
      "+r"(RES[4]), \
      "+r"(RES[5]) \
    : [src1]C1(SRC1), \
      [src2]C2(SRC2) \
)

#define EXEC_RR_012345(INSN, RES, SRC1, SRC2) \
    EXEC2_012345(INSN, RES, "r", SRC1, "r", SRC2)

#define EXEC_IR_012345(INSN, RES, SRC1, SRC2) \
    EXEC2_012345(INSN, RES, "I", SRC1, "r", SRC2)

#define EXEC_RI_012345(INSN, RES, SRC1, SRC2) \
    EXEC2_012345(INSN, RES, "r", SRC1, "i", SRC2)

#define EXEC2_5(INSN, RES, C1, SRC1, C2, SRC2) asm( \
    "\t{\n" \
    "\t    " #INSN ",5 %[src1], %[src2], %0\n" \
    "\t}" \
    : "+r"(RES[0]) \
    : [src1]C1(SRC1), \
      [src2]C2(SRC2) \
)

#define EXEC_RR_5(INSN, RES, SRC1, SRC2) \
    EXEC2_5(INSN, RES, "r", SRC1, "r", SRC2)

#define EXEC_RRI_14(INSN, RES, SRC1, SRC2, SRC3) asm( \
    "\t{\n" \
    "\t    " #INSN ",1 %[src1], %[src2], %[src3], %0\n" \
    "\t    " #INSN ",4 %[src1], %[src2], %[src3], %1\n" \
    "\t}" \
    : "+r"(RES[0]), \
      "+r"(RES[1]) \
    : [src1]"r"(SRC1), \
      [src2]"r"(SRC2), \
      [src3]"i"(SRC3) \
    : "pred0" \
)

#define EXEC_RRR_0134(INSN, RES, SRC1, SRC2, SRC3) asm( \
    "\t{\n" \
    "\t    " #INSN ",0 %[src1], %[src2], %[src3], %0\n" \
    "\t    " #INSN ",1 %[src1], %[src2], %[src3], %1\n" \
    "\t    " #INSN ",3 %[src1], %[src2], %[src3], %2\n" \
    "\t    " #INSN ",4 %[src1], %[src2], %[src3], %3\n" \
    "\t}" \
    : "+r"(RES[0]), \
      "+r"(RES[1]), \
      "+r"(RES[2]), \
      "+r"(RES[3])  \
    : [src1]"r"(SRC1), \
      [src2]"r"(SRC2), \
      [src3]"r"(SRC3)  \
)

#define EXEC_RRR_14(INSN, RES, SRC1, SRC2, SRC3) asm( \
    "\t{\n" \
    "\t    " #INSN ",1 %[src1], %[src2], %[src3], %0\n" \
    "\t    " #INSN ",4 %[src1], %[src2], %[src3], %1\n" \
    "\t}" \
    : "+r"(RES[0]), \
      "+r"(RES[1])  \
    : [src1]"r"(SRC1), \
      [src2]"r"(SRC2), \
      [src3]"r"(SRC3)  \
)

#define EXEC_RRR_25(INSN, RES, SRC1, SRC2, SRC3) asm( \
    "\t{\n" \
    "\t    " #INSN ",2 %[src1], %[src2], %[src3], %0\n" \
    "\t    " #INSN ",5 %[src1], %[src2], %[src3], %1\n" \
    "\t}" \
    : "+r"(RES[0]), \
      "+r"(RES[1])  \
    : [src1]"r"(SRC1), \
      [src2]"r"(SRC2), \
      [src3]"r"(SRC3)  \
)

/*****************************************************************************/
/* Wrappers for 80-bit registers */
/*****************************************************************************/

#define EXEC_DX_03(INSN, RES, SRC1) do { \
    asm( \
        "\t{\n" \
        "\t    movif,0 %[src1_l], %[src1_h], %%g16\n" \
        "\t    nop 1\n" \
        "\t}\n" \
        "\t{\n" \
        "\t    " #INSN ",0 %%g16, %0\n" \
        "\t    " #INSN ",3 %%g16, %2\n" \
        "\t}\n" \
        "\t{\n" \
        "\t    addd,0 0, 0, %1\n" \
        "\t    addd,1 0, 0, %3\n" \
        "\t}\n" \
        : "+r"((RES)[0]), \
          "+r"((RES)[1]), \
          "+r"((RES)[2]), \
          "+r"((RES)[3])  \
        : [src1_l]"r"((SRC1)[0]), \
          [src1_h]"r"((SRC1)[1])  \
        : "g16" \
    ); \
} while (0)

#define EXEC_DX_14(INSN, RES, SRC1) do { \
    asm( \
        "\t{\n" \
        "\t    movif,0 %[src1_l], %[src1_h], %%g16\n" \
        "\t    nop 1\n" \
        "\t}\n" \
        "\t{\n" \
        "\t    " #INSN ",1 %%g16, %0\n" \
        "\t    " #INSN ",4 %%g16, %2\n" \
        "\t}\n" \
        "\t{\n" \
        "\t    addd,0 0, 0, %1\n" \
        "\t    addd,1 0, 0, %3\n" \
        "\t}\n" \
        : "+r"((RES)[0]), \
          "+r"((RES)[1]), \
          "+r"((RES)[2]), \
          "+r"((RES)[3])  \
        : [src1_l]"r"((SRC1)[0]), \
          [src1_h]"r"((SRC1)[1])  \
        : "g16" \
    ); \
} while (0)

#define EXEC_DX_0134(INSN, RES, SRC1) do { \
    asm( \
        "\t{\n" \
        "\t    movif,0 %[src1_l], %[src1_h], %%g16\n" \
        "\t    nop 1\n" \
        "\t}\n" \
        "\t{\n" \
        "\t    " #INSN ",0 %%g16, %0\n" \
        "\t    " #INSN ",1 %%g16, %2\n" \
        "\t    " #INSN ",3 %%g16, %4\n" \
        "\t    " #INSN ",4 %%g16, %6\n" \
        "\t}\n" \
        "\t{\n" \
        "\t    addd,0 0, 0, %1\n" \
        "\t    addd,1 0, 0, %3\n" \
        "\t    addd,2 0, 0, %5\n" \
        "\t    addd,3 0, 0, %7\n" \
        "\t}" \
        : "+r"((RES)[0]), \
          "+r"((RES)[1]), \
          "+r"((RES)[2]), \
          "+r"((RES)[3]), \
          "+r"((RES)[4]), \
          "+r"((RES)[5]), \
          "+r"((RES)[6]), \
          "+r"((RES)[7])  \
        : [src1_l]"r"((SRC1)[0]), \
          [src1_h]"r"((SRC1)[1])  \
        : "g16" \
    ); \
} while (0)

#define EXEC_XX_0134(INSN, RES, SRC1) do { \
    asm( \
        "\t{\n" \
        "\t    movif,0 %[src1_l], %[src1_h], %%g16\n" \
        "\t    nop 1\n" \
        "\t}\n" \
        "\t{\n" \
        "\t    " #INSN ",0 %%g16, %0\n" \
        "\t    " #INSN ",1 %%g16, %2\n" \
        "\t    " #INSN ",3 %%g16, %4\n" \
        "\t    " #INSN ",4 %%g16, %6\n" \
        "\t}\n" \
        "\t{\n" \
        "\t    movfi,1 %0, %1\n" \
        "\t    movfi,4 %2, %3\n" \
        "\t}\n" \
        "\t{\n" \
        "\t    movfi,1 %4, %5\n" \
        "\t    movfi,4 %6, %7\n" \
        "\t}" \
        : "+r"((RES)[0]), \
          "+r"((RES)[1]), \
          "+r"((RES)[2]), \
          "+r"((RES)[3]), \
          "+r"((RES)[4]), \
          "+r"((RES)[5]), \
          "+r"((RES)[6]), \
          "+r"((RES)[7])  \
        : [src1_l]"r"((SRC1)[0]), \
          [src1_h]"r"((SRC1)[1])  \
        : "g16" \
    ); \
} while (0)

#define EXEC_DXX_5(INSN, RES, SRC1, SRC2) do { \
    asm( \
        "\t{\n" \
        "\t    movif,0 %[src1_l], %[src1_h], %%g16\n" \
        "\t    movif,1 %[src2_l], %[src2_h], %%g17\n" \
        "\t    nop 1\n" \
        "\t}\n" \
        "\t{\n" \
        "\t    " #INSN ",5 %%g16, %%g17, %0\n" \
        "\t}\n" \
        "\t    addd,0 0, 0, %1\n" \
        : "+r"((RES)[0]), \
          "+r"((RES)[1])  \
        : [src1_l]"r"((SRC1)[0]), \
          [src1_h]"r"((SRC1)[1]), \
          [src2_l]"r"((SRC2)[0]), \
          [src2_h]"r"((SRC2)[1])  \
        : "g16", "g17" \
    ); \
} while (0)

#define EXEC_XXX_5(INSN, RES, SRC1, SRC2) do { \
    asm( \
        "\t{\n" \
        "\t    movif,0 %[src1_l], %[src1_h], %%g16\n" \
        "\t    movif,1 %[src2_l], %[src2_h], %%g17\n" \
        "\t    nop 1\n" \
        "\t}\n" \
        "\t{\n" \
        "\t    " #INSN ",5 %%g16, %%g17, %0\n" \
        "\t}\n" \
        "\t    movfi,1 %0, %1\n" \
        : "+r"((RES)[0]), \
          "+r"((RES)[1])  \
        : [src1_l]"r"((SRC1)[0]), \
          [src1_h]"r"((SRC1)[1]), \
          [src2_l]"r"((SRC2)[0]), \
          [src2_h]"r"((SRC2)[1])  \
        : "g16", "g17" \
    ); \
} while (0)

#define EXEC_DXX_0134(INSN, RES, SRC1, SRC2) do { \
    asm( \
        "\t{\n" \
        "\t    movif,0 %[src1_l], %[src1_h], %%g16\n" \
        "\t    movif,1 %[src2_l], %[src2_h], %%g17\n" \
        "\t    nop 1\n" \
        "\t}\n" \
        "\t{\n" \
        "\t    " #INSN ",0 %%g16, %%g17, %0\n" \
        "\t    " #INSN ",1 %%g16, %%g17, %2\n" \
        "\t    " #INSN ",3 %%g16, %%g17, %4\n" \
        "\t    " #INSN ",4 %%g16, %%g17, %6\n" \
        "\t}\n" \
        "\t{\n" \
        "\t    addd,0 0, 0, %1\n" \
        "\t    addd,1 0, 0, %3\n" \
        "\t    addd,2 0, 0, %5\n" \
        "\t    addd,3 0, 0, %7\n" \
        "\t}\n" \
        : "+r"((RES)[0]), \
          "+r"((RES)[1]), \
          "+r"((RES)[2]), \
          "+r"((RES)[3]), \
          "+r"((RES)[4]), \
          "+r"((RES)[5]), \
          "+r"((RES)[6]), \
          "+r"((RES)[7])  \
        : [src1_l]"r"((SRC1)[0]), \
          [src1_h]"r"((SRC1)[1]), \
          [src2_l]"r"((SRC2)[0]), \
          [src2_h]"r"((SRC2)[1])  \
        : "g16", "g17" \
    ); \
} while (0)

#define EXEC_XXX_03(INSN, RES, SRC1, SRC2) do { \
    asm( \
        "\t{\n" \
        "\t    movif,0 %[src1_l], %[src1_h], %%g16\n" \
        "\t    movif,1 %[src2_l], %[src2_h], %%g17\n" \
        "\t    nop 1\n" \
        "\t}\n" \
        "\t{\n" \
        "\t    " #INSN ",0 %%g16, %%g17, %0\n" \
        "\t    " #INSN ",3 %%g16, %%g17, %2\n" \
        "\t}\n" \
        "\t{\n" \
        "\t    movfi,1 %0, %1\n" \
        "\t    movfi,4 %2, %3\n" \
        "\t}\n" \
        : "+r"((RES)[0]), \
          "+r"((RES)[1]), \
          "+r"((RES)[2]), \
          "+r"((RES)[3])  \
        : [src1_l]"r"((SRC1)[0]), \
          [src1_h]"r"((SRC1)[1]), \
          [src2_l]"r"((SRC2)[0]), \
          [src2_h]"r"((SRC2)[1])  \
        : "g16", "g17" \
    ); \
} while (0)

#define EXEC_XXX_14(INSN, RES, SRC1, SRC2) do { \
    asm( \
        "\t{\n" \
        "\t    movif,0 %[src1_l], %[src1_h], %%g16\n" \
        "\t    movif,1 %[src2_l], %[src2_h], %%g17\n" \
        "\t    nop 1\n" \
        "\t}\n" \
        "\t{\n" \
        "\t    " #INSN ",1 %%g16, %%g17, %0\n" \
        "\t    " #INSN ",4 %%g16, %%g17, %2\n" \
        "\t}\n" \
        "\t{\n" \
        "\t    movfi,1 %0, %1\n" \
        "\t    movfi,4 %2, %3\n" \
        "\t}\n" \
        : "+r"((RES)[0]), \
          "+r"((RES)[1]), \
          "+r"((RES)[2]), \
          "+r"((RES)[3])  \
        : [src1_l]"r"((SRC1)[0]), \
          [src1_h]"r"((SRC1)[1]), \
          [src2_l]"r"((SRC2)[0]), \
          [src2_h]"r"((SRC2)[1])  \
        : "g16", "g17" \
    ); \
} while (0)

#define EXEC_XXX_0134(INSN, RES, SRC1, SRC2) do { \
    asm( \
        "\t{\n" \
        "\t    movif,0 %[src1_l], %[src1_h], %%g16\n" \
        "\t    movif,1 %[src2_l], %[src2_h], %%g17\n" \
        "\t    nop 1\n" \
        "\t}\n" \
        "\t{\n" \
        "\t    " #INSN ",0 %%g16, %%g17, %0\n" \
        "\t    " #INSN ",1 %%g16, %%g17, %2\n" \
        "\t    " #INSN ",3 %%g16, %%g17, %4\n" \
        "\t    " #INSN ",4 %%g16, %%g17, %6\n" \
        "\t}\n" \
        "\t{\n" \
        "\t    movfi,1 %0, %1\n" \
        "\t    movfi,4 %2, %3\n" \
        "\t}\n" \
        "\t{\n" \
        "\t    movfi,1 %4, %5\n" \
        "\t    movfi,4 %6, %7\n" \
        "\t}" \
        : "+r"((RES)[0]), \
          "+r"((RES)[1]), \
          "+r"((RES)[2]), \
          "+r"((RES)[3]), \
          "+r"((RES)[4]), \
          "+r"((RES)[5]), \
          "+r"((RES)[6]), \
          "+r"((RES)[7])  \
        : [src1_l]"r"((SRC1)[0]), \
          [src1_h]"r"((SRC1)[1]), \
          [src2_l]"r"((SRC2)[0]), \
          [src2_h]"r"((SRC2)[1])  \
        : "g16", "g17" \
    ); \
} while (0)

/*****************************************************************************/
/* Wrappers for 128-bit registers */
/*****************************************************************************/

#define EXEC_DQ_0134(INSN, RES, SRC1) do { \
    asm( \
        "\t{\n" \
        "\t    qppackdl,0 %[src1_h], %[src1_l], %%g16\n" \
        "\t    nop 1\n" \
        "\t}\n" \
        "\t{\n" \
        "\t    " #INSN ",0 %%g16, %0\n" \
        "\t    " #INSN ",1 %%g16, %2\n" \
        "\t    " #INSN ",3 %%g16, %4\n" \
        "\t    " #INSN ",4 %%g16, %6\n" \
        "\t}\n" \
        "\t{\n" \
        "\t    addd,0 0, 0, %1\n" \
        "\t    addd,1 0, 0, %3\n" \
        "\t    addd,3 0, 0, %5\n" \
        "\t    addd,4 0, 0, %7\n" \
        "\t}" \
        : "+r"((RES)[0]), \
          "+r"((RES)[1]), \
          "+r"((RES)[2]), \
          "+r"((RES)[3]), \
          "+r"((RES)[4]), \
          "+r"((RES)[5]), \
          "+r"((RES)[6]), \
          "+r"((RES)[7])  \
        : [src1_l]"r"((SRC1)[0]), \
          [src1_h]"r"((SRC1)[1])  \
        : "g16" \
    ); \
} while (0)

#define EXEC_QQ_0134(INSN, RES, SRC1) do { \
    asm( \
        "\t{\n" \
        "\t    qppackdl,0 %[src1_h], %[src1_l], %%g16\n" \
        "\t    nop 1\n" \
        "\t}\n" \
        "\t{\n" \
        "\t    " #INSN ",0 %%g16, %0\n" \
        "\t    " #INSN ",1 %%g16, %2\n" \
        "\t    " #INSN ",3 %%g16, %4\n" \
        "\t    " #INSN ",4 %%g16, %6\n" \
        "\t}\n" \
        "\t{\n" \
        "\t    qpswitchd,0 %0, %1\n" \
        "\t    qpswitchd,1 %2, %3\n" \
        "\t    qpswitchd,3 %4, %5\n" \
        "\t    qpswitchd,4 %6, %7\n" \
        "\t}" \
        : "+r"((RES)[0]), \
          "+r"((RES)[1]), \
          "+r"((RES)[2]), \
          "+r"((RES)[3]), \
          "+r"((RES)[4]), \
          "+r"((RES)[5]), \
          "+r"((RES)[6]), \
          "+r"((RES)[7])  \
        : [src1_l]"r"((SRC1)[0]), \
          [src1_h]"r"((SRC1)[1])  \
        : "g16" \
    ); \
} while (0)

#define EXEC_QQQ_03(INSN, RES, SRC1, SRC2) do { \
    asm( \
        "\t{\n" \
        "\t    qppackdl,0 %[src1_h], %[src1_l], %%g16\n" \
        "\t    qppackdl,1 %[src2_h], %[src2_l], %%g17\n" \
        "\t    nop 1\n" \
        "\t}\n" \
        "\t{\n" \
        "\t    " #INSN ",0 %%g16, %%g17, %0\n" \
        "\t    " #INSN ",3 %%g16, %%g17, %2\n" \
        "\t}\n" \
        "\t{\n" \
        "\t    qpswitchd,0 %0, %1\n" \
        "\t    qpswitchd,1 %2, %3\n" \
        "\t}" \
        : "+r"((RES)[0]), \
          "+r"((RES)[1]), \
          "+r"((RES)[2]), \
          "+r"((RES)[3])  \
        : [src1_l]"r"((SRC1)[0]), \
          [src1_h]"r"((SRC1)[1]), \
          [src2_l]"r"((SRC2)[0]), \
          [src2_h]"r"((SRC2)[1])  \
        : "g16", "g17" \
    ); \
} while (0)

#define EXEC_DQQ_14(INSN, RES, SRC1, SRC2) do { \
    asm( \
        "\t{\n" \
        "\t    qppackdl,0 %[src1_h], %[src1_l], %%g16\n" \
        "\t    qppackdl,1 %[src2_h], %[src2_l], %%g17\n" \
        "\t    nop 1\n" \
        "\t}\n" \
        "\t{\n" \
        "\t    " #INSN ",1 %%g16, %%g17, %0\n" \
        "\t    " #INSN ",4 %%g16, %%g17, %2\n" \
        "\t}\n" \
        "\t{\n" \
        "\t    addd,0 0, 0, %1\n" \
        "\t    addd,1 0, 0, %3\n" \
        "\t}" \
        : "+r"((RES)[0]), \
          "+r"((RES)[1]), \
          "+r"((RES)[2]), \
          "+r"((RES)[3])  \
        : [src1_l]"r"((SRC1)[0]), \
          [src1_h]"r"((SRC1)[1]), \
          [src2_l]"r"((SRC2)[0]), \
          [src2_h]"r"((SRC2)[1])  \
        : "g16", "g17" \
    ); \
} while (0)

#define EXEC_QQQ_14(INSN, RES, SRC1, SRC2) do { \
    asm( \
        "\t{\n" \
        "\t    qppackdl,0 %[src1_h], %[src1_l], %%g16\n" \
        "\t    qppackdl,1 %[src2_h], %[src2_l], %%g17\n" \
        "\t    nop 1\n" \
        "\t}\n" \
        "\t{\n" \
        "\t    " #INSN ",1 %%g16, %%g17, %0\n" \
        "\t    " #INSN ",4 %%g16, %%g17, %2\n" \
        "\t}\n" \
        "\t{\n" \
        "\t    qpswitchd,0 %0, %1\n" \
        "\t    qpswitchd,1 %2, %3\n" \
        "\t}" \
        : "+r"((RES)[0]), \
          "+r"((RES)[1]), \
          "+r"((RES)[2]), \
          "+r"((RES)[3])  \
        : [src1_l]"r"((SRC1)[0]), \
          [src1_h]"r"((SRC1)[1]), \
          [src2_l]"r"((SRC2)[0]), \
          [src2_h]"r"((SRC2)[1])  \
        : "g16", "g17" \
    ); \
} while (0)

#define EXEC_DQQ_0134(INSN, RES, SRC1, SRC2) do { \
    asm( \
        "\t{\n" \
        "\t    qppackdl,0 %[src1_h], %[src1_l], %%g16\n" \
        "\t    qppackdl,1 %[src2_h], %[src2_l], %%g17\n" \
        "\t    nop 1\n" \
        "\t}\n" \
        "\t{\n" \
        "\t    " #INSN ",0 %%g16, %%g17, %0\n" \
        "\t    " #INSN ",1 %%g16, %%g17, %2\n" \
        "\t    " #INSN ",3 %%g16, %%g17, %4\n" \
        "\t    " #INSN ",4 %%g16, %%g17, %6\n" \
        "\t}\n" \
        "\t{\n" \
        "\t    addd,0 0, 0, %1\n" \
        "\t    addd,1 0, 0, %3\n" \
        "\t    addd,3 0, 0, %5\n" \
        "\t    addd,4 0, 0, %7\n" \
        "\t}" \
        : "+r"((RES)[0]), \
          "+r"((RES)[1]), \
          "+r"((RES)[2]), \
          "+r"((RES)[3]), \
          "+r"((RES)[4]), \
          "+r"((RES)[5]), \
          "+r"((RES)[6]), \
          "+r"((RES)[7])  \
        : [src1_l]"r"((SRC1)[0]), \
          [src1_h]"r"((SRC1)[1]), \
          [src2_l]"r"((SRC2)[0]), \
          [src2_h]"r"((SRC2)[1])  \
        : "g16", "g17" \
    ); \
} while (0)

#define EXEC_QQQ_0134(INSN, RES, SRC1, SRC2) do { \
    asm( \
        "\t{\n" \
        "\t    qppackdl,0 %[src1_h], %[src1_l], %%g16\n" \
        "\t    qppackdl,1 %[src2_h], %[src2_l], %%g17\n" \
        "\t    nop 1\n" \
        "\t}\n" \
        "\t{\n" \
        "\t    " #INSN ",0 %%g16, %%g17, %0\n" \
        "\t    " #INSN ",1 %%g16, %%g17, %2\n" \
        "\t    " #INSN ",3 %%g16, %%g17, %4\n" \
        "\t    " #INSN ",4 %%g16, %%g17, %6\n" \
        "\t}\n" \
        "\t{\n" \
        "\t    qpswitchd,0 %0, %1\n" \
        "\t    qpswitchd,1 %2, %3\n" \
        "\t    qpswitchd,3 %4, %5\n" \
        "\t    qpswitchd,4 %6, %7\n" \
        "\t}" \
        : "+r"((RES)[0]), \
          "+r"((RES)[1]), \
          "+r"((RES)[2]), \
          "+r"((RES)[3]), \
          "+r"((RES)[4]), \
          "+r"((RES)[5]), \
          "+r"((RES)[6]), \
          "+r"((RES)[7])  \
        : [src1_l]"r"((SRC1)[0]), \
          [src1_h]"r"((SRC1)[1]), \
          [src2_l]"r"((SRC2)[0]), \
          [src2_h]"r"((SRC2)[1])  \
        : "g16", "g17" \
    ); \
} while (0)

#define EXEC_QQQ_012345(INSN, RES, SRC1, SRC2) do { \
    asm( \
        "\t{\n" \
        "\t    qppackdl,0 %[src1_h], %[src1_l], %%g16\n" \
        "\t    qppackdl,1 %[src2_h], %[src2_l], %%g17\n" \
        "\t    nop 1\n" \
        "\t}\n" \
        "\t{\n" \
        "\t    " #INSN ",0 %%g16, %%g17, %0\n" \
        "\t    " #INSN ",1 %%g16, %%g17, %2\n" \
        "\t    " #INSN ",2 %%g16, %%g17, %4\n" \
        "\t    " #INSN ",3 %%g16, %%g17, %6\n" \
        "\t    " #INSN ",4 %%g16, %%g17, %8\n" \
        "\t    " #INSN ",5 %%g16, %%g17, %10\n" \
        "\t}\n" \
        "\t{\n" \
        "\t    qpswitchd,0 %0, %1\n" \
        "\t    qpswitchd,1 %2, %3\n" \
        "\t    qpswitchd,3 %4, %5\n" \
        "\t    qpswitchd,4 %6, %7\n" \
        "\t}" \
        "\t{\n" \
        "\t    qpswitchd,0 %8, %9\n" \
        "\t    qpswitchd,1 %10, %11\n" \
        "\t}" \
        : "+r"((RES)[0]), \
          "+r"((RES)[1]), \
          "+r"((RES)[2]), \
          "+r"((RES)[3]), \
          "+r"((RES)[4]), \
          "+r"((RES)[5]), \
          "+r"((RES)[6]), \
          "+r"((RES)[7]), \
          "+r"((RES)[8]), \
          "+r"((RES)[9]), \
          "+r"((RES)[10]), \
          "+r"((RES)[11])  \
        : [src1_l]"r"((SRC1)[0]), \
          [src1_h]"r"((SRC1)[1]), \
          [src2_l]"r"((SRC2)[0]), \
          [src2_h]"r"((SRC2)[1])  \
        : "g16", "g17" \
    ); \
} while (0)

#define EXEC_QQQQ_0134(INSN, RES, SRC1, SRC2, SRC3) do { \
    asm( \
        "\t{\n" \
        "\t    qppackdl,0 %[src1_h], %[src1_l], %%g16\n" \
        "\t    qppackdl,1 %[src2_h], %[src2_l], %%g17\n" \
        "\t    qppackdl,3 %[src3_h], %[src3_l], %%g18\n" \
        "\t    nop 1\n" \
        "\t}\n" \
        "\t{\n" \
        "\t    " #INSN ",0 %%g16, %%g17, %%g18, %0\n" \
        "\t    " #INSN ",1 %%g16, %%g17, %%g18, %2\n" \
        "\t    " #INSN ",3 %%g16, %%g17, %%g18, %4\n" \
        "\t    " #INSN ",4 %%g16, %%g17, %%g18, %6\n" \
        "\t}\n" \
        "\t{\n" \
        "\t    qpswitchd,0 %0, %1\n" \
        "\t    qpswitchd,1 %2, %3\n" \
        "\t    qpswitchd,3 %4, %5\n" \
        "\t    qpswitchd,4 %6, %7\n" \
        "\t}" \
        : "+r"((RES)[0]), \
          "+r"((RES)[1]), \
          "+r"((RES)[2]), \
          "+r"((RES)[3]), \
          "+r"((RES)[4]), \
          "+r"((RES)[5]), \
          "+r"((RES)[6]), \
          "+r"((RES)[7])  \
        : [src1_l]"r"((SRC1)[0]), \
          [src1_h]"r"((SRC1)[1]), \
          [src2_l]"r"((SRC2)[0]), \
          [src2_h]"r"((SRC2)[1]), \
          [src3_l]"r"((SRC3)[0]), \
          [src3_h]"r"((SRC3)[1])  \
        : "g16", "g17" \
    ); \
} while (0)

#define EXEC_QQQQ_012345(INSN, RES, SRC1, SRC2, SRC3) do { \
    asm( \
        "\t{\n" \
        "\t    qppackdl,0 %[src1_h], %[src1_l], %%g16\n" \
        "\t    qppackdl,1 %[src2_h], %[src2_l], %%g17\n" \
        "\t    qppackdl,3 %[src3_h], %[src3_l], %%g18\n" \
        "\t    nop 1\n" \
        "\t}\n" \
        "\t{\n" \
        "\t    " #INSN ",0 %%g16, %%g17, %%g18, %0\n" \
        "\t    " #INSN ",1 %%g16, %%g17, %%g18, %2\n" \
        "\t    " #INSN ",2 %%g16, %%g17, %%g18, %4\n" \
        "\t    " #INSN ",3 %%g16, %%g17, %%g18, %6\n" \
        "\t    " #INSN ",4 %%g16, %%g17, %%g18, %8\n" \
        "\t    " #INSN ",5 %%g16, %%g17, %%g18, %10\n" \
        "\t}\n" \
        "\t{\n" \
        "\t    qpswitchd,0 %0, %1\n" \
        "\t    qpswitchd,1 %2, %3\n" \
        "\t    qpswitchd,3 %4, %5\n" \
        "\t    qpswitchd,4 %6, %7\n" \
        "\t}" \
        "\t{\n" \
        "\t    qpswitchd,0 %8, %9\n" \
        "\t    qpswitchd,1 %10, %11\n" \
        "\t}" \
        : "+r"((RES)[0]), \
          "+r"((RES)[1]), \
          "+r"((RES)[2]), \
          "+r"((RES)[3]), \
          "+r"((RES)[4]), \
          "+r"((RES)[5]), \
          "+r"((RES)[6]), \
          "+r"((RES)[7]), \
          "+r"((RES)[8]), \
          "+r"((RES)[9]), \
          "+r"((RES)[10]), \
          "+r"((RES)[11])  \
        : [src1_l]"r"((SRC1)[0]), \
          [src1_h]"r"((SRC1)[1]), \
          [src2_l]"r"((SRC2)[0]), \
          [src2_h]"r"((SRC2)[1]), \
          [src3_l]"r"((SRC3)[0]), \
          [src3_h]"r"((SRC3)[1])  \
        : "g16", "g17" \
    ); \
} while (0)

#endif /* EXEC_H */
