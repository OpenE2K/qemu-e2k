#ifndef EXEC_H
#define EXEC_H

#define EXEC_R_0134(INSN, RES, SRC1, SRC2, SRC3, SRC4) asm( \
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

#define EXEC_R_14(INSN, RES, SRC1, SRC2, SRC3, SRC4) asm( \
    "\t{\n" \
    "\t    " #INSN ",1 %[src1], %0\n" \
    "\t    " #INSN ",4 %[src1], %1\n" \
    "\t}" \
    : "+r"(RES[0]), \
      "+r"(RES[1])  \
    : [src1]"r"(SRC1) \
)

#define EXEC_R_5(INSN, RES, SRC1, SRC2, SRC3, SRC4) asm( \
    "\t{\n" \
    "\t    " #INSN ",5 %[src1], %0\n" \
    "\t}" \
    : "+r"(RES[0]) \
    : [src1]"r"(SRC1) \
)

#define EXEC_RR_03(INSN, RES, SRC1, SRC2, SRC3, SRC4) asm( \
    "\t{\n" \
    "\t    " #INSN ",0 %[src1], %[src2], %0\n" \
    "\t    " #INSN ",3 %[src1], %[src2], %1\n" \
    "\t}" \
    : "+r"(RES[0]), \
      "+r"(RES[1])  \
    : [src1]"r"(SRC1), \
      [src2]"r"(SRC2)  \
)

#define EXEC_RR_14(INSN, RES, SRC1, SRC2, SRC3, SRC4) asm( \
    "\t{\n" \
    "\t    " #INSN ",1 %[src1], %[src2], %0\n" \
    "\t    " #INSN ",4 %[src1], %[src2], %1\n" \
    "\t}" \
    : "+r"(RES[0]), \
      "+r"(RES[1])  \
    : [src1]"r"(SRC1), \
      [src2]"r"(SRC2)  \
)

#define EXEC_RR_25(INSN, RES, SRC1, SRC2, SRC3, SRC4) asm( \
    "\t{\n" \
    "\t    " #INSN ",2 %[src1], %[src2], %0\n" \
    "\t    " #INSN ",5 %[src1], %[src2], %1\n" \
    "\t}" \
    : "+r"(RES[0]), \
      "+r"(RES[1])  \
    : [src1]"r"(SRC1), \
      [src2]"r"(SRC2)  \
)

#define EXEC_RI_14(INSN, RES, SRC1, SRC2, SRC3, SRC4) asm( \
    "\t{\n" \
    "\t    " #INSN ",1 %[src1], %[src2], %0\n" \
    "\t    " #INSN ",4 %[src1], %[src2], %1\n" \
    "\t}" \
    : "+r"(RES[0]), \
      "+r"(RES[1])  \
    : [src1]"r"(SRC1), \
      [src2]"i"(SRC2)  \
)

#define EXEC_RR_0134(INSN, RES, SRC1, SRC2, SRC3, SRC4) asm( \
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

#define EXEC_CMP_0134(INSN, RES, SRC1, SRC2, SRC3, SRC4) asm( \
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
      [src2]"ri"(SRC2), \
      [src3]"ri"(SRC3)  \
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

#define EXEC_RR_012345(INSN, RES, SRC1, SRC2, SRC3, SRC4) \
    EXEC2_012345(INSN, RES, "r", SRC1, "r", SRC2)

#define EXEC_IR_012345(INSN, RES, SRC1, SRC2, SRC3, SRC4) \
    EXEC2_012345(INSN, RES, "I", SRC1, "r", SRC2)

#define EXEC_RI_012345(INSN, RES, SRC1, SRC2, SRC3, SRC4) \
    EXEC2_012345(INSN, RES, "r", SRC1, "i", SRC2)

#define EXEC2_5(INSN, RES, C1, SRC1, C2, SRC2) asm( \
    "\t{\n" \
    "\t    " #INSN ",5 %[src1], %[src2], %0\n" \
    "\t}" \
    : "+r"(RES[0]) \
    : [src1]C1(SRC1), \
      [src2]C2(SRC2) \
)

#define EXEC_RR_5(INSN, RES, SRC1, SRC2, SRC3, SRC4) \
    EXEC2_5(INSN, RES, "r", SRC1, "r", SRC2)

#define EXEC_RRI_14(INSN, RES, SRC1, SRC2, SRC3, SRC4) asm( \
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

#define EXEC_RRR_0134(INSN, RES, SRC1, SRC2, SRC3, SRC4) asm( \
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

#define EXEC_RRR_14(INSN, RES, SRC1, SRC2, SRC3, SRC4) asm( \
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

#define EXEC_RRR_25(INSN, RES, SRC1, SRC2, SRC3, SRC4) asm( \
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

#endif /* EXEC_H */
