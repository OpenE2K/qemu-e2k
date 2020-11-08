 
#include <sys/types.h>

#define COMMON_PART                                                     \
  const char *name;                                                     \
  int (* parse_args) (char **, const struct e2k_opcode_templ *);        \
  const char * (* merge) (struct e2k_opcode_templ *, const struct e2k_opcode_templ *)

#define ALOPF1			1
#define ALOPF2			2
#define ALOPF3			3
#define ALOPF7			4
#define ALOPF8			5
#define ALOPF10			6
#define ALOPF11			7
#define ALOPF11_LIT8		8
#define ALOPF11_MERGE		9
#define ALOPF12			10
#define ALOPF12_PSHUFH		11
#define ALOPF12_IBRANCHD	12
#define ALOPF12_ICALLD		13
#define ALOPF13			14
#define ALOPF15			15
#define ALOPF16			16
#define ALOPF21			17
#define ALOPF21_MERGE		18
#define ALOPF22			19
#define MERGE			20
#define MMURR			21
#define MMURW			22
#define AAURR			23
#define AAURW			24
#define ALOPF17			25

#define ALF_PART                                \
  u_int8_t alopf;                               \
  int need_mas;                                 \
  u_int8_t opc;                                 \
  int allowed_channels[6]


#define MAS             1
#define NO_MAS          0

/* ALES.opc2 values. See B.1.2 in iset.single.  */
#define EXT     0x1
#define EXT1    0x2
#define EXT2	0x3
#define FLB     0x4
#define FLH     0x5
#define FLW     0x6
#define FLD     0x7
#define ICMB0   0x8
#define ICMB1   0x9
#define ICMB2   0xA
#define ICMB3   0xB

#define FCMB0   0xC
#define FCMB1   0xD
#define PFCMB0  0XE
#define PFCMB1  0xF
#define LCMBD0  0x10
#define LCMBD1  0x11
#define LCMBQ0  0x12
#define LCMBQ1  0x13
#define QPFCMB0 0x16
#define QPFCMB1 0x17


/* ALES.opce values.  */
#define NONE    0xc0


/* It seems that LAS doesn't support %xr's, that's
   why everywhere where an x-args is supposed we
   use a d-one. */
typedef enum {
  SINGLE,
  DOUBLE,
  QUAD,
  QPACKED
} e2k_register_format;


#define ARGS_S          SINGLE
#define ARGS_D          DOUBLE
#define ARGS_Q          QUAD
#define ARGS_P		QPACKED

#define ARGS_SS         {SINGLE, SINGLE}
#define ARGS_SD         {SINGLE, DOUBLE}
#define ARGS_SQ         {SINGLE, QUAD}
#define ARGS_DS         {DOUBLE, SINGLE}
#define ARGS_DD         {DOUBLE, DOUBLE}
#define ARGS_DQ         {DOUBLE, QUAD}
#define ARGS_DP         {DOUBLE, QPACKED}
#define ARGS_QS         {QUAD, SINGLE}
#define ARGS_PS         {QPACKED, SINGLE}
#define ARGS_QD         {QUAD, DOUBLE}
#define ARGS_PD         {QPACKED, DOUBLE}
#define ARGS_QQ         {QUAD, QUAD}
#define ARGS_PP         {QPACKED, QPACKED}

#define ARGS_SSS        {SINGLE, SINGLE, SINGLE}
#define ARGS_SSD        {SINGLE, SINGLE, DOUBLE}
#define ARGS_SSQ        {SINGLE, SINGLE, QUAD}
#define ARGS_SSP        {SINGLE, SINGLE, QPACKED}
#define ARGS_SDD        {SINGLE, DOUBLE, DOUBLE}
#define ARGS_DSS        {DOUBLE, SINGLE, SINGLE}
#define ARGS_DSD        {DOUBLE, SINGLE, DOUBLE}
#define ARGS_DDS        {DOUBLE, DOUBLE, SINGLE}
#define ARGS_DDD        {DOUBLE, DOUBLE, DOUBLE}
#define ARGS_DDQ        {DOUBLE, DOUBLE, QUAD}
#define ARGS_DDP        {DOUBLE, DOUBLE, QPACKED}
#define ARGS_DQQ        {DOUBLE, QUAD, QUAD}
#define ARGS_DPP        {DOUBLE, QPACKED, QPACKED}
#define ARGS_QSS        {QUAD, SINGLE, SINGLE}
#define ARGS_QSD        {QUAD, SINGLE, DOUBLE}
#define ARGS_QSQ        {QUAD, SINGLE, QUAD}
#define ARGS_PSP        {QPACKED, SINGLE, QPACKED}
#define ARGS_QSP        {QUAD, SINGLE, QPACKED}
#define ARGS_QDQ        {QUAD, DOUBLE, QUAD}
#define ARGS_PDP        {QPACKED, DOUBLE, QPACKED}
#define ARGS_QQD        {QUAD, QUAD, DOUBLE}
#define ARGS_PPD        {QPACKED, QPACKED, DOUBLE}
#define ARGS_QQQ        {QUAD, QUAD, QUAD}
#define ARGS_PPP        {QPACKED, QPACKED, QPACKED}

#define ARGS_SSSS       {SINGLE, SINGLE, SINGLE, SINGLE}
#define ARGS_DDSD       {DOUBLE, DOUBLE, SINGLE, DOUBLE}
#define ARGS_DDDD       {DOUBLE, DOUBLE, DOUBLE, DOUBLE}
#define ARGS_QQQQ       {QUAD, QUAD, QUAD, QUAD}
#define ARGS_PPPP       {QPACKED, QPACKED, QPACKED, QPACKED}

#define ALL_SINGLE {SINGLE, SINGLE, SINGLE}
#define ALL_DOUBLE {DOUBLE, DOUBLE, DOUBLE}



typedef struct e2k_opcode_templ
{
  COMMON_PART;
} e2k_opcode_templ;

typedef struct
{
  COMMON_PART;
  ALF_PART;  
} e2k_alf_opcode_templ;



#define ALF1_PART                               \
  e2k_register_format arg_fmt[3]

typedef struct
{
  COMMON_PART;
  ALF_PART;
  ALF1_PART;
} e2k_alf1_opcode_templ;


#define ALF2_PART                               \
  e2k_register_format arg_fmt[2];               \
  u_int8_t opce


typedef struct
{
  COMMON_PART;
  ALF_PART;
  ALF2_PART;
} e2k_alf2_opcode_templ;



#define ALF3_PART                               \
  e2k_register_format arg_fmt[3]

typedef struct
{
  COMMON_PART;
  ALF_PART;
  ALF3_PART;
} e2k_alf3_opcode_templ;


#define ALOPF12_PART                            \
  e2k_register_format arg_fmt[2];               \
  u_int8_t opce;                                \
  u_int8_t ales_opce;                           \
  u_int8_t ales_opc2

typedef struct
{
  COMMON_PART;
  ALF_PART;
  ALOPF12_PART;
} e2k_alopf12_opcode_templ;



#define ALOPF13_PART                            \
  e2k_register_format arg_fmt[3];               \
  u_int8_t ales_opce;                           \
  u_int8_t ales_opc2

typedef struct
{
  COMMON_PART;
  ALF_PART;
  ALOPF13_PART;
} e2k_alopf13_opcode_templ;



#define ALOPF15_PART                               \
  e2k_register_format arg_fmt

typedef struct
{
  COMMON_PART;
  ALF_PART;
  ALOPF15_PART;
} e2k_alopf15_opcode_templ;

typedef e2k_alopf15_opcode_templ e2k_alopf16_opcode_templ;

#define CMPsb           0x20
#define CMPdb           0x21
#define CMPANDsb        0x22
#define FXCMPxb         0x2b
#define FCMPdb          0x2f

typedef enum {
  CMPOPCE_0 = 0,
  CMPOPCE_1,
  CMPOPCE_2,
  CMPOPCE_3,
  CMPOPCE_4,
  CMPOPCE_5,
  CMPOPCE_6,
  CMPOPCE_7
} cmpopce_t;

#define ALF7_PART                               \
  e2k_register_format arg_fmt[2];               \
  cmpopce_t cmpopce;				\
  int implicit_nops;

typedef struct
{
  COMMON_PART;
  ALF_PART;
  ALF7_PART;
} e2k_alf7_opcode_templ;

#define ALF9_PART                               \
  e2k_register_format arg_fmt;

typedef struct
{
  COMMON_PART;
  ALF_PART;
  ALF9_PART;
} e2k_alf9_opcode_templ;


#define ALF10_PART                              \
  e2k_register_format arg_fmt;

typedef struct
{
  COMMON_PART;
  ALF_PART;
  ALF10_PART;
} e2k_alf10_opcode_templ;


#define ALF8_PART                               \
  e2k_register_format arg_fmt;                  \
  cmpopce_t cmpopce


typedef struct
{
  COMMON_PART;
  ALF_PART;
  ALF8_PART;
} e2k_alf8_opcode_templ;

#define ALOPF11_PART                            \
  e2k_register_format arg_fmt[3];               \
  u_int8_t ales_opce;                           \
  u_int8_t ales_opc2[6];                        \
  int explicit_ales25_v4

typedef struct
{
  COMMON_PART;
  ALF_PART;
  ALOPF11_PART;
} e2k_alopf11_opcode_templ;


#define ALOPF11_LIT8_PART                       \
  u_int8_t max_lit8;                            \
  const char *warn

typedef struct
{
  COMMON_PART;
  ALF_PART;
  ALOPF11_PART;
  ALOPF11_LIT8_PART;
} e2k_alopf11_lit8_opcode_templ;


#define ALOPF21_PART                            \
  e2k_register_format arg_fmt[4];               \
  u_int8_t ales_opc2;

typedef struct
{
  COMMON_PART;
  ALF_PART;
  ALOPF21_PART;
} e2k_alopf21_opcode_templ;


#define NO_LABEL        0
#define EXPECT_LABEL    1

#define NO_CTPR         {0, 0, 0}
#define CTPR2           {0, 1, 0}
#define CTPR3           {0, 0, 1}
#define ALL_CTPRS       {1, 1, 1}

typedef struct
{
  COMMON_PART;
  u_int8_t ctp_opc;
  int allowed_ctprs[3];
  int label_expected;
} e2k_copf2_opcode_templ;

typedef struct
{
  COMMON_PART;
  unsigned id;
} e2k_setcmd_opcode_templ;

#define MOVA_PART                               \
  u_int16_t opc;                                \
  e2k_register_format arg_fmt;

typedef struct
{
  COMMON_PART;
  MOVA_PART;
} e2k_mova_opcode_templ;




static int parse_alf_args (char **s, const e2k_opcode_templ *t) { return 0; }
static int parse_copf2_args (char **s, const e2k_opcode_templ *t) { return 0; }
static int parse_pref_args (char **s, const e2k_opcode_templ *t) { return 0; }
static int parse_copf4_args (char **s, const e2k_opcode_templ *t) { return 0; }
static int parse_nop_args (char **s, const e2k_opcode_templ *t) { return 0; }
static int parse_setcmd_args (char **s, const e2k_opcode_templ *t) { return 0; }
static int parse_setsft_args (char **s, const e2k_opcode_templ *t) { return 0; }
static int parse_wait_args (char **s, const e2k_opcode_templ *t) { return 0; }
static int parse_ct_args (char **s, const e2k_opcode_templ *t) { return 0; }
static int parse_hcall_args (char **s, const e2k_opcode_templ *t) { return 0; }
static int parse_ipd_args (char **s, const e2k_opcode_templ *t) { return 0; }

static int parse_loop_mode_args (char **s, const e2k_opcode_templ *t) { return 0; }
static int parse_alc_args (char **s, const e2k_opcode_templ *t) { return 0; }
static int parse_abn_args (char **s, const e2k_opcode_templ *t) { return 0; }
static int parse_abp_args (char **s, const e2k_opcode_templ *t) { return 0; }
static int parse_abg_args (char **s, const e2k_opcode_templ *t) { return 0; }
static int parse_bap_args (char **s, const e2k_opcode_templ *t) { return 0; }
static int parse_eap_args (char **s, const e2k_opcode_templ *t) { return 0; }


static int parse_pass_args (char **s, const e2k_opcode_templ *t) { return 0; }
static int parse_andp_args (char **s, const e2k_opcode_templ *t) { return 0; }
static int parse_landp_args (char **s, const e2k_opcode_templ *t) { return 0; }
static int parse_ibranch_args (char **s, const e2k_opcode_templ *t) { return 0; }
static int parse_done_hret_glaunch_args (char **s, const e2k_opcode_templ *t) { return 0; }

static int parse_incr_args (char **s, const e2k_opcode_templ *t) { return 0; }
static int parse_mova_args (char **s, const e2k_opcode_templ *t) { return 0; }
static int parse_fapb_args (char **s, const e2k_opcode_templ *t) { return 0; }
static int parse_movep_args (char **s, const e2k_opcode_templ *t) { return 0; }

static int parse_flushts_args (char **s, const e2k_opcode_templ *t) { return 0; }

static int parse_cpl_args (char **s, const e2k_opcode_templ *t) { return 0; }

static int parse_set_mark_args (char **s, const e2k_opcode_templ *t) { return 0; }
static int parse_vfdi_args (char **s, const e2k_opcode_templ *t) { return 0; }



extern struct e2k_opcode_templ *e2k_opcode_templs[];
extern size_t e2k_num_opcodes;


typedef enum {
  WINDOW,
  BASED,
  GLOBAL,
  SPECIAL,
  AASTI
} e2k_register_type;

typedef struct {
  u_int8_t idx;
  e2k_register_type type;
  e2k_register_format fmt;
} e2k_generic_register;


typedef enum {
  LITERAL_4 = -3,
  LITERAL_5,
  /* LITERAL_8 should be used exclusively for encoding ALEF2.opce
     in PSHUF{W,H}.  */
  LITERAL_8,
  LITERAL_16,
  LITERAL_32,
  LITERAL_64
} e2k_literal_size;

typedef struct {
  e2k_literal_size size;
  int sgnd;
} e2k_literal_format;


typedef struct
{
  int negated;
  int pred_num;
  int pred_fld;
} e2k_pred;

/* The maximal possible number of ALCes in the wide instruction.  */
#define ALS_CHANNELS_NUMBER     6

#define opc_field                                                       \
  struct {                                                              \
    u_int8_t cop : 7;                                                   \
    u_int8_t spec : 1;                                                  \
  } opc

#define GENERIC_ALS                                                     \
  /* May be 1, 2, 3, 5, 6, 7, 8, 9, 10 for appropriate ALF's. */        \
  int fmt;                                                              \
  /* Pointer to a function which will finalize this ALS after all long	\
     literals in the containing wide instruction have been		\
     accommodated.  */							\
  int (* finish) (struct e2k_als *);                                    \
  /* The value of ALS which should be encoded.  */			\
  union {                                                               \
    struct {                                                            \
      u_int8_t dst, src2, src1;                                         \
      opc_field;                                                        \
    } alf1;                                                             \
    struct {                                                            \
      u_int8_t dst, src2, opce;                                         \
      opc_field;                                                        \
    } alf2;                                                             \
    struct {                                                            \
      u_int8_t src3, src2, src1;                                        \
      opc_field;                                                        \
    } alf3;                                                             \
    struct {                                                            \
      u_int8_t regn, src2, opce;                                        \
      opc_field;                                                        \
    } alf5;                                                             \
    struct {                                                            \
      u_int8_t dst, none, regn;                                         \
      opc_field;                                                        \
    } alf6;                                                             \
    struct {                                                            \
      struct {                                                          \
        u_int8_t pdst : 5;                                              \
        u_int8_t cmpopce : 3;                                           \
      } dst2;                                                           \
      u_int8_t src2, src1;                                              \
      opc_field;                                                        \
    } alf7;                                                             \
    struct {                                                            \
      struct {                                                          \
        u_int8_t pdst : 5;                                              \
        u_int8_t cmpopce : 3;                                           \
      } dst2;                                                           \
      u_int8_t src2, opce;                                              \
      opc_field;                                                        \
    } alf8;                                                             \
    struct {                                                            \
      u_int8_t dst;                                                     \
      u_int8_t opce1_lo;                                                \
      u_int8_t opce1_hi;                                                \
      opc_field;                                                        \
    } alf9;                                                             \
    struct {                                                            \
      u_int8_t src3;                                                    \
      u_int8_t opce1_lo;                                                \
      u_int8_t opce1_hi;                                                \
      opc_field;                                                        \
    } alf10;                                                            \
    u_int32_t val;                                                      \
  } u[2];                                                               \
                                                                        \
  /* Therse two are used for quad ops occupying two channels. */        \
  unsigned real_als_nmb;                                                \
  /* The first element in real_alses will always be the minor           \
     channel number. I want the user to be able to write                \
     stapq,5 instead of stapq,2. */                                     \
  unsigned real_alses[6][2];                                            \
                                                                        \
  /* This means that ALS{j,k}.src1 should contain the same value        \
     in both channels. This is required to encode LDAPQ and STAPQ       \
     properly.  */                                                      \
  int same_src1_quad;                                                   \
                                                                        \
  /* The number of valid placements. It can be 6 at maximum, which	\
     corresponds to ALC0, . . , ALC5. */			        \
  int plcmnt_nmb;                                                       \
  int pos[ALS_CHANNELS_NUMBER];                                         \
  /* The most optimal index in pos[].  */				\
  int optimal_plcmnt_idx

typedef union {
  struct {
    u_int8_t src3;
    u_int8_t opc2;
  } alef1;

  struct {
    u_int8_t opce;
    u_int8_t opc2;
  } alef2;

  u_int16_t hword;
} e2k_ales;


extern void init_opcode_templs (void);

extern int mcpu;
