/*
 * Copyright (c) 2021 Alibek Omarov
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2 or later, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * Ripped from Linux MCST fork
 */

/*
 * 0x0:
 * 0x1: extended command line
 */
#define BOOTBLOCK_VER			0x1

#define	KSTRMAX_SIZE			128
#define	KSTRMAX_SIZE_EX			512
#define	BIOS_INFO_SIGN_SIZE		8
#define KERNEL_ARGS_STRING_EX_SIGN_SIZE	22
#define	BOOT_VER_STR_SIZE		128
#define	BOOTBLOCK_SIZE			0x1000	/* 1 PAGE_SIZE */
#define	X86BOOT_SIGNATURE		0x8086
#define	ROMLOADER_SIGNATURE		0xe200
#define	KVM_GUEST_SIGNATURE		0x20e2
#define	BIOS_INFO_SIGNATURE		"E2KBIOS"
#define	KVM_INFO_SIGNATURE		"E2KKVM"
#define KERNEL_ARGS_STRING_EX_SIGNATURE "KERNEL_ARGS_STRING_EX"
#define BOOT_KERNEL_ARGS_STRING_EX_SIGNATURE	\
boot_va_to_pa(KERNEL_ARGS_STRING_EX_SIGNATURE)

/*
 * Below is boot information that comes out of the x86 code of Linux/E2K
 * loader proto.
 */

/* L_MAX_NODE_PHYS_BANKS = 4 sometimes is not enough, so we increase it to
 * an arbitary value (8 now). The old L_MAX_NODE_PHYS_BANKS we rename to
 * L_MAX_NODE_PHYS_BANKS_FUSTY and take in mind for boot_info compatibility.
 *
 * L_MAX_NODE_PHYS_BANKS_FUSTY and L_MAX_MEM_NUMNODES describe max size of
 * array of memory banks on all nodes and should be in accordance with old value
 * of L_MAX_PHYS_BANKS for compatibility with boot_info old structure (bank)
 * size, so L_MAX_NODE_PHYS_BANKS_FUSTY * L_MAX_MEM_NUMNODES should be
 * equal to 32.
 */
#define L_MAX_NODE_PHYS_BANKS		64	/* max number of memory banks */
/* on one node */
#define L_MAX_NODE_PHYS_BANKS_FUSTY	4	/* fusty max number of memory */
/* banks on one node */
#define L_MAX_PHYS_BANKS_EX		64	/* max number of memory banks */
/* in banks_ex field of */
/* boot_info */
#define L_MAX_MEM_NUMNODES		8	/* max number of nodes in the */
/* list of memory banks on */
/* each node */
#define	L_MAX_BUSY_AREAS	4	/* max number of busy areas */
/* occupied by BIOS and should be */
/* kept unchanged by kernel to */
/* support recovery mode */

#ifndef	__ASSEMBLY__

typedef struct bank_info {
    __u64	address;	/* start address of bank */
    __u64	size;		/* size of bank in bytes */
} bank_info_t;

typedef struct node_banks {
    bank_info_t banks[L_MAX_NODE_PHYS_BANKS_FUSTY];	/* memory banks array */
    /* of a node */
} node_banks_t;

typedef struct boot_times {
    __u64 arch;
    __u64 unpack;
    __u64 pci;
    __u64 drivers1;
    __u64 drivers2;
    __u64 menu;
    __u64 sm;
    __u64 kernel;
    __u64 reserved[8];
} boot_times_t;

typedef struct bios_info {
    __u8	signature[BIOS_INFO_SIGN_SIZE];		/* signature, */
    /* 'E2KBIOS' */
    __u8	boot_ver[BOOT_VER_STR_SIZE];		/* boot version */
    __u8	mb_type;				/* mother board type */
    __u8	chipset_type;				/* chipset type */
    __u8	cpu_type;				/* cpu type */
    __u8	kernel_args_string_ex[KSTRMAX_SIZE_EX];	/* extended command */
    /* line of kernel */
    /* used to pass */
    /* command line */
    /* from e2k BIOS */
    __u8	reserved1;				/* reserved1 */
    __u32	cache_lines_damaged;			/* number of damaged */
    /* cache lines */
    __u64	nodes_mem_slabs_deprecated[52];		/* array of slabs */
    /* accessible memory */
    /* on each node */
    /* accessible memory */
    /* on each node */
    bank_info_t banks_ex[L_MAX_PHYS_BANKS_EX];	/* extended array of */
    /* descriptors of */
    /* banks of available */
    /* physical memory */
    __u64	devtree;				/* devtree pointer */
    __u32	bootlog_addr;				/* bootlog address */
    __u32	bootlog_len;				/* bootlog length */
    __u8	uuid[16];				/* UUID boot device */
} bios_info_t;

typedef struct boot_info {
    __u16	signature;	/* signature, 0x8086 */
    __u8	target_mdl;	/* target cpu model number */
    __u8	reserved1;	/* reserved1 */
    __u16	reserved2;	/* reserved2 */
    __u8	vga_mode;	/* vga mode */
    __u8	num_of_banks;	/* number of available physical memory banks */
    /* see below bank array */
    /* total number on all nodes or 0 */
    __u64	kernel_base;	/* base address to load kernel image */
    /* if 0 then BIOS can load at any address */
    /* but address should be large page size */
    /* aligned - 4 Mb */
    __u64	kernel_size;	/* kernel image byte's size */
    __u64	ramdisk_base;	/* base address to load RAM-disk */
    /* now not used */
    __u64	ramdisk_size;	/* RAM-disk byte's size */
    
    __u16	num_of_cpus;	/* number of started physical CPU(s) */
    __u16	mach_flags;	/* machine identifacition flags */
    /* should be set by our romloader and BIOS */
    __u16	num_of_busy;	/* number of busy areas occupied by BIOS */
    /* see below busy array */
    __u16	num_of_nodes;	/* number of nodes on NUMA system */
    __u64	mp_table_base;	/* MP-table base address */
    __u64	serial_base;	/* base address of serial port for Am85c30 */
    /* Used for debugging purpose */
    __u64	nodes_map;	/* online nodes map */
    __u64	mach_serialn;	/* serial number of the machine */
    __u8	mac_addr[6];	/* base MAC address for ethernet cards */
    __u16	reserved3;	/* reserved3 */
    
    char	kernel_args_string[KSTRMAX_SIZE]; /* command line of kernel */
    /* used to pass command line */
    /* from e2k BIOS */
    node_banks_t	nodes_mem[L_MAX_MEM_NUMNODES];	/* array of */
    /* descriptors of banks of */
    /* available physical memory */
    /* on each node */
    bank_info_t	busy[L_MAX_BUSY_AREAS];	/* descriptors of areas */
    /* occupied by BIOS, all this */
    /* shoud be kept in system */
    /* recovery mode */
    u64		cntp_info_deprecated[32];	/* control points */
    /* info to save and */
    /* restore them state */
    u64		dmp_deprecated[20];	/* Info for future work of */
    /* dump analyzer */
    __u64		reserved4[13];	/* reserved4 */
    __u8		mb_name[16];	/* Motherboard product name */
    __u32		reserved5;	/* reserved5 */
    __u32		kernel_csum;	/* kernel image control sum */
    bios_info_t	bios;		/* extended BIOS info */
    /* SHOULD BE LAST ITEM into this */
    /* structure */
} boot_info_t;

typedef struct bootblock_struct {
    boot_info_t	info;			/* general kernel<->BIOS info */
    __u8					/* zip area to make size of */
    /* bootblock struct - constant */
    gap[BOOTBLOCK_SIZE -
    sizeof (boot_info_t)  -
    sizeof (boot_times_t) -
    1 -		/* u8  : bootblock_ver */
    4 -		/* u32 : reserved1 */
    2 -		/* u16 : kernel_flags */
    1 -		/* u8  : reserved2 */
    5 -		/* u8  : number of cnt points */
    /* u8  : current # of cnt point */
    /* u8  : number of cnt points */
    /*	 ready in the memory */
    /* u8  : number of cnt points */
    /*	 saved on the disk */
    /* u8  : all control points */
    /*	 is created */
    8 -		/* u64 : dump sector */
    8 -		/* u64 : cnt point sector */
    2 -		/* u16 : dump device */
    2 -		/* u16 : cnt point device */
    2 -		/* u16 : boot_flags */
    2];		/* u16 : x86_marker */
    __u8		bootblock_ver;		/* bootblock version number */
    __u32		reserved1;		/* reserved1 */
    boot_times_t	boot_times;		/* boot load times */
    __u16		kernel_flags;		/* kernel flags, boot should */
    /* not modify it */
    __u8		reserved2;		/* reserved2 */
    
    __u8	cnt_points_num_deprecated;	/* number of control points */
    /* all memory will be devided */
    /* on this number of parts */
    __u8	cur_cnt_point_deprecated;	/* current # of active */
    /* control point (running */
    /* part) */
    __u8	mem_cnt_points_deprecated;	/* number of started control */
    /* points (ready in the memory) */
    __u8	disk_cnt_points_deprecated;	/* number of control points */
    /* saved on the disk (ready  */
    /* to be loaded from disk) */
    __u8	cnt_points_created_deprecated;	/* all control points created */
    /* in the memory and on disk */
    __u64	dump_sector_deprecated;		/* start sector # to dump */
    /* physical memory */
    __u64	cnt_point_sector_deprecated;	/* start sector # to save */
    /* restore control points */
    __u16	dump_dev_deprecated;		/* disk # to dump memory */
    __u16	cnt_point_dev_deprecated;	/* disk # for save/restore */
    /* control point */
    
    __u16		boot_flags;		/* boot flags: if non */
    /* zero then this structure */
    /* is recovery info */
    /* structure instead of boot */
    /* info structure */
    __u16		x86_marker;		/* marker of the end of x86 */
    /* boot block (0xAA55) */
} bootblock_struct_t;

/*
 * The mother board types
 */

#define MB_TYPE_MIN                     0
#define MB_TYPE_E2K_BASE                0x00
#define MB_TYPE_ES2_BASE                (MB_TYPE_E2K_BASE + 20)
#define MB_TYPE_E1CP_BASE		(MB_TYPE_E2K_BASE + 50)
#define	MB_TYPE_ES4_BASE		(MB_TYPE_E2K_BASE + 70)
#define MB_TYPE_E8C_BASE		(MB_TYPE_E2K_BASE + 80)
#define MB_TYPE_MAX			a0

#define MB_TYPE_ES2_PLATO1		(MB_TYPE_ES2_BASE + 0)
#define MB_TYPE_ES2_BUTTERFLY		(MB_TYPE_ES2_BASE + 1)
#define MB_TYPE_ES2_RTC_FM33256		(MB_TYPE_ES2_BASE + 2)	/* FM332aa56 rtc */
#define MB_TYPE_ES2_RTC_CY14B101P	(MB_TYPE_ES2_BASE + 3)	/* CY14B101P rtc */
#define MB_TYPE_ES2_APORIA		(MB_TYPE_ES2_BASE + 5)  /* APORIA */
#define MB_TYPE_ES2_NT			(MB_TYPE_ES2_BASE + 6)  /* Nosimyi terminal */
/* Use this when CLKRs are not synchronized across the system */
#define MB_TYPE_ES2_RTC_CY14B101P_MULTICLOCK (MB_TYPE_ES2_BASE + 7)
#define MB_TYPE_ES2_CUB_COM		(MB_TYPE_ES2_BASE + 8)
#define MB_TYPE_ES2_MBCUB_C		(MB_TYPE_ES2_BASE + 11)
#define MB_TYPE_ES2_MB3S1_C		(MB_TYPE_ES2_BUTTERFLY)
#define	MB_TYPE_ES2_MB3S_C_K		(MB_TYPE_ES2_BASE + 14)
#define	MB_TYPE_ES2_MGA3D		(MB_TYPE_ES2_BASE + 15)
#define	MB_TYPE_ES2_BC_M4211		(MB_TYPE_ES2_BASE + 16)
#define	MB_TYPE_ES2_EL2S4		(MB_TYPE_ES2_BASE + 17)
/* By default all mb_versions > MB_TYPE_ES2_EL2S4 
 * have cy14b101p rt clock. If no - fix is_cy14b101p_exist()
 * in arch/l/kernel/i2c-spi/core.c
 */

#define MB_TYPE_E1CP_PMC	(MB_TYPE_E1CP_BASE + 0)	/* E1CP with PMC */
#define MB_TYPE_E1CP_IOHUB2_RAZBRAKOVSCHIK	/* IOHUB2 razbrakovschik */ \
(MB_TYPE_E1CP_BASE + 1)
#define MB_TYPE_MBE1C_PC	(MB_TYPE_E1CP_BASE + 2)	/* E1CP with PMC */

#define	MB_TYPE_ES4_MBE2S_PC	(MB_TYPE_ES4_BASE + 0)
#define	MB_TYPE_ES4_PC401	(MB_TYPE_ES4_BASE + 1)

#define MB_TYPE_E8C		(MB_TYPE_E8C_BASE + 0)


/*
 * The cpu types
 */

#define CPU_TYPE_E2S		0x03	/* E2S */
#define CPU_TYPE_ES2_DSP	0x04	/* E2C+ */
#define CPU_TYPE_ES2_RU		0x06	/* E2C Micron */
#define CPU_TYPE_E8C		0x07	/* E8C */
#define CPU_TYPE_E1CP		0x08	/* E1C+ */
#define CPU_TYPE_E8C2		0x09	/* E8C */
#define CPU_TYPE_E12C		0xa	/* E12C */
#define CPU_TYPE_E16C		0xb	/* E16C */
#define CPU_TYPE_E2C3		0xc	/* E2C3 */

#define CPU_TYPE_SIMUL		0x3e    /* simulator */

#define	CPU_TYPE_MASK		0x3f	/* mask of CPU type */
#define	PROC_TYPE_MASK		0xc0	/* mask of MicroProcessor type */

#define	GET_CPU_TYPE(type)	(((type) & CPU_TYPE_MASK) >> 0)

/*
 * The cpu types names
 */

#define GET_CPU_TYPE_NAME(type_field)			\
({							\
    unsigned char type = GET_CPU_TYPE(type_field);	\
    char *name;					\
    \
    switch (type) {					\
        case CPU_TYPE_E2S:				\
            name = "E2S";				\
            break;					\
        case CPU_TYPE_ES2_DSP:				\
            name = "E2C+DSP";			\
            break;					\
        case CPU_TYPE_ES2_RU:				\
            name = "E1C";				\
            break;					\
        case CPU_TYPE_E8C:				\
            name = "E8C";				\
            break;					\
        case CPU_TYPE_E1CP:				\
            name = "E1C+";				\
            break;					\
        case CPU_TYPE_E8C2:				\
            name = "E8C2";				\
            break;					\
        case CPU_TYPE_E12C:				\
            name = "E12C";				\
            break;					\
        case CPU_TYPE_E16C:				\
            name = "E16C";				\
            break;					\
        case CPU_TYPE_E2C3:				\
            name = "E2C3";				\
            break;					\
        case CPU_TYPE_SIMUL:				\
            name = "SIMUL";				\
            break;					\
        default:					\
            name = "unknown";			\
    }						\
    \
    name;						\
})

/*
 * The mother board types names
 */

#define GET_MB_TYPE_NAME(type)				\
({							\
    char *name;					\
    \
    switch (type) {					\
        case MB_TYPE_ES2_MB3S1_C:			\
            name = "MB3S1/C";			\
            break;					\
        case MB_TYPE_ES2_MBCUB_C:			\
        case MB_TYPE_ES2_PLATO1:			\
            name = "MBKUB/C";			\
            break;					\
        case MB_TYPE_ES2_MB3S_C_K:			\
            name = "MB3S/C-K";			\
            break;					\
        case MB_TYPE_ES2_NT:				\
            name = "NT-ELBRUS-S";			\
            break;					\
        case MB_TYPE_ES2_CUB_COM:			\
            name = "CUB-COM";			\
            break;					\
        case MB_TYPE_ES2_RTC_FM33256:			\
            name = "MONOCUB+FM33256";		\
            break;					\
        case MB_TYPE_ES2_RTC_CY14B101P:			\
            name = "MONOCUB";			\
            break;					\
        case MB_TYPE_ES2_RTC_CY14B101P_MULTICLOCK:	\
            name = "MP1C1/V";			\
            break;					\
        case MB_TYPE_ES2_EL2S4:				\
            name = "EL2S4";				\
            break;					\
        case MB_TYPE_ES2_MGA3D:				\
            name = "MGA3D";				\
            break;					\
        case MB_TYPE_ES2_BC_M4211:			\
            name = "BC-M4211";			\
            break;					\
        case MB_TYPE_E1CP_PMC:				\
            name = "E1C+ PMC";			\
            break;					\
        case MB_TYPE_E1CP_IOHUB2_RAZBRAKOVSCHIK:	\
            name = "IOHUB2 razbrakovschik";		\
            break;					\
        case MB_TYPE_MBE1C_PC:				\
            name = "MBE1C-PC";			\
            break;					\
        case MB_TYPE_ES4_MBE2S_PC:			\
            name = "MBE2S-PC";			\
            break;					\
        case MB_TYPE_ES4_PC401:				\
            name = "PC-401";			\
            break;					\
        case MB_TYPE_E8C:				\
            name = "E8C";				\
            break;					\
        default:					\
            name = "unknown";			\
    }						\
    \
    name;						\
})

#define GET_MB_USED_IN(type)					\
({								\
    char *name;						\
    \
    switch (type) {						\
        case MB_TYPE_ES2_PLATO1:				\
            name = "Plato with softreset error";		\
            break;						\
        case MB_TYPE_ES2_MBCUB_C:				\
            name = "APM VK-2, APM VK-120, BV632, BV631";	\
            break;						\
        case MB_TYPE_ES2_MB3S1_C:				\
            name = "ELBRUS-3C-CVS, ELBRUS-3C";		\
            break;						\
        case MB_TYPE_ES2_RTC_FM33256:				\
            name = "MONOCUB+FM33256";			\
            break;						\
        case MB_TYPE_ES2_RTC_CY14B101P:				\
            name = "MONOCUB-M, MONOCUB-PC";			\
            break;						\
        default:						\
            name = NULL;					\
    }							\
    \
    name;							\
})
