#ifndef E2K_TRANSLATE_H
#define E2K_TRANSLATE_H

#include "exec/translator.h"

#define COND_NEVER 0

typedef enum AlesFlag {
    ALES_NONE = 0x00,
    ALES_PRESENT = 0x01,
    ALES_ALLOCATED = 0x02,
} AlesFlag;

typedef struct CPUE2KStateTCG {
    TCGv pc;
    TCGv ctprs[3];
    TCGv_i32 is_jmp;
    TCGv_i32 wbs;
    TCGv_i32 wsz;
    TCGv_i32 nfx;
    TCGv_i32 dbl;
    TCGv_i32 rbs;
    TCGv_i32 rsz;
    TCGv_i32 rcur;
    TCGv_i32 psz;
    TCGv_ptr win_ptr;
    TCGv_i64 wregs[WREGS_SIZE];
    TCGv_i64 gregs[32];
    TCGv_i64 pregs;
} CPUE2KStateTCG;

extern struct CPUE2KStateTCG e2k_cpu;

typedef struct UnpackedBundle {
  uint32_t hs;
  uint32_t ss;
  uint32_t als[6];
  uint32_t cs0;
  uint16_t ales[6];
  uint32_t cs1;
  uint16_t aas[6];
  uint32_t lts[4];
  uint32_t pls[3];
  uint32_t cds[3];

  bool ss_present;
  bool als_present[6];
  bool cs0_present;
  AlesFlag ales_present[6];
  bool cs1_present;
  bool aas_present[6];
  bool lts_present[4];
  bool pls_present[3];
  bool cds_present[3];
} UnpackedBundle;

enum ResultType {
    RESULT_NONE,
    RESULT_BASED_REG,
    RESULT_REGULAR_REG,
    RESULT_GLOBAL_REG,
    RESULT_PREG,
};

typedef struct {
    enum ResultType tag;
    union {
        struct {
            unsigned int i;
            TCGv_i64 v;
        } reg;
    } u;
} Result;

typedef struct DisasContext {
    DisasContextBase base;
    UnpackedBundle bundle;
    target_ulong pc;

    // Temporary values.
    TCGv_i32 t32[16];
    TCGv_i64 t64[16];
    TCGv ttl[8];
    // Allocated temporary values count.
    int t32_len;
    int t64_len;
    int ttl_len;

    Result alc[6];
    struct {
        // raw condition code from SS[8:0]
        unsigned int cond;
        TCGv dest;
    } jmp;
} DisasContext;

#endif
