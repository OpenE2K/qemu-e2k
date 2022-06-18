#ifndef E2K_TARGET_SYSCALL_H
#define E2K_TARGET_SYSCALL_H

#define UNAME_MACHINE "e2k"
#define UNAME_MINIMUM_RELEASE "2.6.32"

#define	E2K_MAXNR	128			/* The total number of */
						/* quad-NRs */
#define	E2K_MAXGR	16			/* The total number of global */
						/* quad-NRs */
#define	E2K_MAXSR	(E2K_MAXNR - E2K_MAXGR)	/* The total number of stack */
						/* quad-NRs */
#define	E2K_MAXNR_d	(E2K_MAXNR * 2)		/* The total number of */
						/* double-NRs */
#define	E2K_MAXGR_d	(E2K_MAXGR * 2)		/* The total number of global */
						/* double-NRs */
#define	E2K_MAXSR_d	(E2K_MAXSR * 2)		/* The total number of stack */
						/* double-NRs */

#define E2K_DEFAULT_PCS_SIZE (TARGET_PAGE_SIZE)
#define E2K_DEFAULT_PS_SIZE (TARGET_PAGE_SIZE * 4)

typedef uint64_t e2k_greg_t; // double word

struct target_pt_regs {
    /* special registers */
    uint64_t wd; // Current window descriptor (WD)
        
    uint64_t sbr; // User Stack Base Register (USBR/SBR)
                  // SBR - contains the base (top) virtual address of the current User Stack area.
    
    // uint64_t tr; // current type register
    
    E2KPsp pcsp;
    E2KPsp psp;

    uint32_t psr; // Processor State Register (PSR)
    uint32_t upsr; // User processor status register (UPSR)
    uint64_t ip; // instruction pointer
    uint64_t nip; // next instruction pointer
    
    uint64_t ctpr1; // Control Transfer Preparation Register (CTPR)
    uint64_t ctpr2;
    uint64_t ctpr3;
    
    uint32_t pfpfr; // Packed Floating Point Flag Register (PFPFR)
    uint32_t fpcr; // Floating point control register (FPCR)
    uint32_t fpsr; // Floating point state register (FPSR)
    
    // borrowed from Embox OS
    uint64_t lsr; // Loop status register (LSR)
    uint64_t ilcr; // Loop counter register (ILCR)
    
    uint64_t dr0;
    
    uint64_t cr0_hi;
    uint64_t cr1_lo;
    uint64_t cr1_hi;
    uint64_t pcsp_hi;
    uint64_t pcsp_lo;
    uint64_t usd_lo; // User data
    uint64_t usd_hi;
    
    uint64_t gbase[32];
    uint16_t gext[32];
};

// FIXME: Is it right place for these constants?
#if TARGET_LONG_BITS == 64
#define TARGET_PAGE_OFFSET 0x0000d00000000000UL
#define TARGET_TASK_SIZE TARGET_PAGE_OFFSET
#else
#define TARGET_TASK_SIZE 0xe0000000UL
#endif

/* modes for sys_access_hw_stacks */
enum {
    READ_CHAIN_STACK,
    READ_PROCEDURE_STACK,
    WRITE_PROCEDURE_STACK,
    GET_CHAIN_STACK_OFFSET,
    GET_CHAIN_STACK_SIZE,
    GET_PROCEDURE_STACK_SIZE,
    READ_CHAIN_STACK_EX,
    READ_PROCEDURE_STACK_EX,
    WRITE_PROCEDURE_STACK_EX,
    WRITE_CHAIN_STACK_EX,
};

#endif /* E2K_TARGET_SYSCALL_H */
