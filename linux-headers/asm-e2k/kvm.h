#ifndef _ASM_E2K_KVM_H
#define _ASM_E2K_KVM_H

/*
 * KVM e2k specific structures and definitions
 *
 * Note: you must update KVM_ARCH_API_VERSION if you change this interface.
 */

#ifndef __ASSEMBLY__

#include <linux/types.h>
#include <linux/ioctl.h>

/*
 * e2k KVM api is not yet stable, so there is specific e2k arch
 * refinement of the interface in format yymmdd so that the version
 * number always monotonously increased
 */
#define KVM_ARCH_API_VERSION	210512

#define KVM_COALESCED_MMIO_PAGE_OFFSET 1

/* Select e2k specific features in <linux/kvm.h> */
#define	__KVM_HAVE_IRQ_LINE
#define __KVM_HAVE_DEVICE_ASSIGNMENT
#define __KVM_HAVE_USER_NMI
#define __KVM_HAVE_GUEST_DEBUG
#define __KVM_HAVE_MSIX
#define __KVM_HAVE_MCE
#define __KVM_HAVE_VCPU_EVENTS

/* KVM (for /dev/kvm fds) capabilities (especially for e2k arch) */
/* number of CAPs is advisedly very big to don't intersect with other arch'es */
/* Other arch'es CAPs can be amplified and appended in future */
/* The better place for these defines should be at arch-indep header */
/* include/uapi/linux/kvm.h as for all other arch'es */
#define	KVM_CAP_E2K_SV_VM	300	/* paravirtualized guest without any */
					/* hardware support */
#define	KVM_CAP_E2K_SW_PV_VM	301	/* paravirtualized kernel without any */
					/* hardware support and can be run */
					/* as host (hypervisor) and as guest */
					/* especial case to debug purposes */
#define	KVM_CAP_E2K_HV_VM	302	/* fully virtualized guest machines */
					/* using hardware extensions */
#define	KVM_CAP_E2K_HW_PV_VM	303	/* paravirtualized guest machines */
					/* using hardware extensions */

/* Flags are bits 63:32 of KVM_CREATE_VM argument */
#define	KVM_E2K_VM_TYPE_MASK	0x00000000ffffffffULL
#define	KVM_E2K_VM_FLAG_MASK	0xffffffff00000000ULL
/* VM types, to be used as argument to KVM_CREATE_VM */
#define	KVM_E2K_SV_VM_TYPE	0	/* software virtualized guest without */
					/* any hardware support */
					/* now it is default VM type because */
					/* of can be available for all CPUs */
#define	KVM_E2K_SW_PV_VM_TYPE	1	/* paravirtualized kernel without any */
					/* hardware support and can be run */
					/* as host (hypervisor) and as guest */
					/* especial case to debug purposes */
#define	KVM_E2K_HV_VM_TYPE	2	/* fully virtualized guest machines */
					/* using hardware extensions */
#define	KVM_E2K_HW_PV_VM_TYPE	3	/* paravirtualized guest machines */
					/* using hardware extensions */

#define	KVM_E2K_EPIC_VM_FLAG	0x100000000ULL	/* choose between paravirt */
						/* APIC and EPIC models */

/* KVM MMU capabilities */
#define	KVM_CAP_E2K_SHADOW_PT_MMU	310	/* is shadow PT enabled */
#define	KVM_CAP_E2K_TDP_MMU		311	/* is Two Dimensial Paging */
						/* mode enabled */

/* Architectural interrupt line count. */
#define KVM_NR_INTERRUPTS 256

/*
 * e2k arch-dependent limits for the nr of threads virtual CPUs
 */

/* KVM manage */
#define	KVM_VM_MAX_LIMIT	1024	/* max number of VM IDs at use */

/* VCPUs manage */
#define KVM_MAX_VCPUS		64

typedef struct kvm_memory_alias {
	__u32 slot;	/* this has a different namespace than memory slots */
	__u32 flags;
	__u64 guest_alias_addr;
	__u64 memory_size;
	__u64 target_addr;
} kvm_memory_alias_t;

/* arch e2k additions flags for kvm_memory_region::flags */
#define	KVM_MEM_ADD_TYPE	0x0010	/* region should be added with */
					/* type of memory */
#define KVM_MEM_VCPU_RAM	0x0020	/* memory region is common RAM */
#define KVM_MEM_VCPU_VRAM	0x0040	/* memory region is virtual */
					/* registers emulation memory */
#define	KVM_MEM_IO_VRAM		0x0080	/* memory region is virtual IO memory */
					/* to emulate ISA, VGA VRAM (low) */
#define	KVM_MEM_USER_RAM	0x0100	/* memory is mapped to user space of */
					/* host application */
					/* (in our case QEMU) */
#define	KVM_MEM_TYPE_MASK	(KVM_MEM_VCPU_RAM | KVM_MEM_VCPU_VRAM | \
				KVM_MEM_IO_VRAM | KVM_MEM_USER_RAM)

typedef enum {
	guest_ram_mem_type	= 0x01,	/* memory is common RAM (low & high) */
	guest_vram_mem_type	= 0x02,	/* memory is virtual registers */
					/* memory (VCPU, VMMU ... emulation) */
	guest_io_vram_mem_type	= 0x03,	/* memory is virtual IO memory */
					/* to emulate ISA, VGA-VRAM (low) */
	guest_user_ram_mem_type	= 0x04,	/* memory is mapped to user space of */
					/* host application (QEMU) */
} kvm_guest_mem_type_t;

#define	KVM_MAX_VRAM_AREAS	4	/* max number of VRAM areas */

/* for KVM_GET_IRQCHIP and KVM_SET_IRQCHIP */
struct kvm_pic_state {
	__u8 last_irr;	/* edge detection */
	__u8 irr;		/* interrupt request register */
	__u8 imr;		/* interrupt mask register */
	__u8 isr;		/* interrupt service register */
	__u8 priority_add;	/* highest irq priority */
	__u8 irq_base;
	__u8 read_reg_select;
	__u8 poll;
	__u8 special_mask;
	__u8 init_state;
	__u8 auto_eoi;
	__u8 rotate_on_auto_eoi;
	__u8 special_fully_nested_mode;
	__u8 init4;		/* true if 4 byte init */
	__u8 elcr;		/* PIIX edge/trigger selection */
	__u8 elcr_mask;
};

typedef struct kvm_kernel_area_shadow {
	__u32 slot;	/* this has a different namespace than memory and */
			/* alias slots */
	__u32 flags;
	__u64 kernel_addr;	/* host kernel area  base address */
	__u64 area_size;
	__u64 guest_shadow_addr;	/* guest kernel base address */
					/* should be alias of memory region */
} kvm_kernel_area_shadow_t;


#define KVM_IOAPIC_NUM_PINS  24

struct kvm_ioapic_state {
	__u64 base_address;
	__u32 ioregsel;
	__u32 id;
	__u32 irr;
	__u32 pad;
	union {
		__u64 bits;
		struct {
			__u8 vector;
			__u8 delivery_mode:3;
			__u8 dest_mode:1;
			__u8 delivery_status:1;
			__u8 polarity:1;
			__u8 remote_irr:1;
			__u8 trig_mode:1;
			__u8 mask:1;
			__u8 reserve:7;
			__u8 reserved[4];
			__u8 dest_id;
		} fields;
	} redirtbl[KVM_IOAPIC_NUM_PINS];
	__u32 node_id;
};

#define KVM_IOEPIC_NUM_PINS  64

#define KVM_IRQCHIP_IOAPIC	0
#define KVM_IRQCHIP_IOEPIC	1
#define KVM_NR_IRQCHIPS		2

/* for KVM_GET_REGS and KVM_SET_REGS */
struct kvm_regs {
	/* out (KVM_GET_REGS) / in (KVM_SET_REGS) */
	__u64 upsr;
};

/* for KVM_GET_LAPIC and KVM_SET_LAPIC */
#define KVM_APIC_REG_SIZE 0x400
struct kvm_lapic_state {
	char regs[KVM_APIC_REG_SIZE];
};

/* for KVM_GET_SREGS and KVM_SET_SREGS */
struct kvm_sregs {
	/* out (KVM_GET_SREGS) / in (KVM_SET_SREGS) */
	__u64 USD_lo, USD_hi;
	__u64 PSP_lo, PSP_hi;
	__u64 PCSP_lo, PCSP_hi;
	__u64 apic_base;
	__u64 interrupt_bitmap[(KVM_NR_INTERRUPTS + 63) / 64];
};

/* for KVM_GET_FPU and KVM_SET_FPU */
struct kvm_fpu {
};

struct kvm_debug_exit_arch {
	__u32 exception;
	__u32 pad;
	__u64 pc;
	__u64 dr6;
	__u64 dr7;
};

#define KVM_GUESTDBG_USE_SW_BP		0x00010000
#define KVM_GUESTDBG_USE_HW_BP		0x00020000
#define KVM_GUESTDBG_INJECT_DB		0x00040000
#define KVM_GUESTDBG_INJECT_BP		0x00080000

/* for KVM_SET_GUEST_DEBUG */
struct kvm_guest_debug_arch {
	__u64 debugreg[8];
};

/* When set in flags, include corresponding fields on KVM_SET_VCPU_EVENTS */
#define KVM_VCPUEVENT_VALID_NMI_PENDING	0x00000001
#define KVM_VCPUEVENT_VALID_SIPI_VECTOR	0x00000002

/* for KVM_GET/SET_VCPU_EVENTS */
struct kvm_vcpu_events {
	struct {
		__u8 injected;
		__u8 nr;
		__u8 has_error_code;
		__u8 pad;
		__u32 error_code;
	} exception;
	struct {
		__u8 injected;
		__u8 nr;
		__u8 soft;
		__u8 pad;
	} interrupt;
	struct {
		__u8 injected;
		__u8 pending;
		__u8 masked;
		__u8 pad;
	} nmi;
	__u32 sipi_vector;
	__u32 flags;
	__u32 reserved[10];
};

/* for KVM_GET_PIT and KVM_SET_PIT */
struct kvm_pit_channel_state {
	__u32 count; /* can be 65536 */
	__u16 latched_count;
	__u8 count_latched;
	__u8 status_latched;
	__u8 status;
	__u8 read_state;
	__u8 write_state;
	__u8 write_latch;
	__u8 rw_mode;
	__u8 mode;
	__u8 bcd;
	__u8 gate;
	__s64 count_load_time;
};

struct kvm_pit_state {
	struct kvm_pit_channel_state channels[3];
};
#define KVM_PIT_FLAGS_HPET_LEGACY  0x00000001
struct kvm_pit_state2 {
	struct kvm_pit_channel_state channels[3];
	__u32 flags;
	__u32 reserved[9];
};

struct kvm_reinject_control {
	__u8 pit_reinject;
	__u8 reserved[31];
};
/* definition of registers in kvm_run */
struct kvm_sync_regs {
};

/*
 * e2k arch specific kvm dev/vm/vcpu ioctl's
 */

/*
 * Guest machine info
 */
typedef struct kvm_guest_info {
	int	cpu_mdl;		/* guest CPU model (as at IDR) */
	int	cpu_rev;		/* guest CPU revision (as at IDR) */
	int	cpu_iset;		/* guest CPU instruction set version */
	_Bool	is_stranger;		/* guest is stranger type */
					/* of CPU/machine */
	_Bool	mmu_support_pt_v6;	/* guest MMU support new MMU Page */
					/* Tables structures V6 */
	_Bool	is_pv;			/* guest is paravirtualized */
					/* and should not be run as bare */
	unsigned long	features;	/* guest features */
					/* see details <asm/kvm/hypervisor.h> */
} kvm_guest_info_t;

typedef struct kvm_guest_area_alloc {
	void	*region;	/* guest memory region to allocate area */
				/* or NULL if any region */
	void	*area;		/* allocated area - result of ioctl() */
	unsigned long start;	/* start address to allocate */
				/* or 0 if any address */
	unsigned long size;	/* area size (bytes) */
	unsigned long align;	/* the area beginning align */
	unsigned long flags;	/* allocation flags and modes */
	kvm_guest_mem_type_t type; /* type of memory: RAM, VRAM */
} kvm_guest_area_alloc_t;

typedef struct kvm_guest_area_reserve {
	unsigned long start;	   /* start address to reserve */
	unsigned long size;	   /* area size (bytes) */
	kvm_guest_mem_type_t type; /* type of memory: RAM, VRAM */
} kvm_guest_area_reserve_t;

/* guest area allocation flags */
#define	KVM_ALLOC_AREA_PRESENT		0x00000001ULL
#define	KVM_ALLOC_AREA_LOCKED		0x00000002ULL
#define	KVM_ALLOC_AREA_ZEROED		0x00000004ULL
/* protections as part of flags */
#define	KVM_ALLOC_AREA_PROT_READ	0x00010000ULL
#define	KVM_ALLOC_AREA_PROT_WRITE	0x00020000ULL
#define	KVM_ALLOC_AREA_PROT_EXEC	0x00040000ULL
/* some additional features */
#define	KVM_ALLOC_AREA_HUGE		0x00100000ULL	/* prefered mapping */
							/* to huge pages */
#define	KVM_ALLOC_AREA_MAP_FLAGS	\
		(KVM_ALLOC_AREA_PROT_READ | KVM_ALLOC_AREA_PROT_WRITE | \
			KVM_ALLOC_AREA_PROT_EXEC | \
				KVM_ALLOC_AREA_HUGE)

/* guest addresses map */
#define	KVM_GUEST_PAGE_OFFSET		0x00000010
#define	KVM_GUEST_KERNEL_IMAGE_BASE	0x00000020
#define	KVM_GUEST_VCPU_VRAM_PHYS_BASE	0x00000040
#define	KVM_GUEST_VCPU_VRAM_VIRT_BASE	0x00000080
#define	KVM_GUEST_VCPU_VRAM_SIZE	0x00000100
#define	KVM_GUEST_IO_VRAM_PHYS_BASE	0x00000200
#define	KVM_GUEST_IO_VRAM_VIRT_BASE	0x00000400
#define	KVM_GUEST_IO_VRAM_SIZE		0x00000800
#define	KVM_HOST_PAGE_OFFSET		0x00001000
#define	KVM_HOST_KERNEL_IMAGE_BASE	0x00002000
#define	KVM_KERNEL_AREAS_SIZE		0x00004000
#define	KVM_SHADOW_KERNEL_IMAGE_BASE	0x00008000
#define	KVM_GUEST_IO_PORTS_BASE		0x00010000
#define	KVM_GUEST_NBSR_BASE_NODE_0	0x00020000
#define	KVM_GUEST_NBSR_BASE_NODE_1	0x00040000
#define	KVM_GUEST_NBSR_BASE_NODE_2	0x00080000
#define	KVM_GUEST_NBSR_BASE_NODE_3	0x00100000
#define	KVM_HOST_INFO_VRAM_SIZE		0x00200000

/* flags of IO ports area mapping for guest */
#define	KVM_IO_PORTS_MMAP	0x1ff00000000	/* > max physical memory */

#define	KVM_VCPU_MAX_GUEST_ARGS		4

typedef struct kvm_vcpu_guest_startup {
	char	*kernel_base;	/* base address of guest kernel image */
	long	kernel_size;	/* guest kernel image size */
	char	*entry_point;	/* entry point to startup guest image */
	int	args_num;	/* number of additional arguments to pass */
				/* to guest image */
	unsigned long args[KVM_VCPU_MAX_GUEST_ARGS];	/* arguments */
	unsigned long flags;	/* flags specifying guest properties */
				/* see details below */
	unsigned long trap_off;	/* trap entry (ttable #0) offset from guest */
				/* kernel image base */
} kvm_vcpu_guest_startup_t;

typedef enum kvm_pci_region_type {
	kvm_pci_undef_region_type,
	kvm_pci_io_type,	/* PCI IO ports region */
	kvm_pci_mem_type,	/* PCI memory region */
	kvm_pci_pref_mem_type,	/* PCI prefetchable memory region */
} kvm_pci_region_type_t;

typedef struct kvm_pci_region {
	int node_id;			/* the node # */
	kvm_pci_region_type_t type;	/* the region type: IO/MEM/PMEM */
	unsigned long base;		/* the base address of the region */
	unsigned long size;		/* the region size */
} kvm_pci_region_t;

typedef struct kvm_base_addr_node {
	int node_id;			/* the node # */
	unsigned long base;		/* the base address */
} kvm_base_addr_node_t;

typedef struct kvm_guest_nbsr_state {
	int node_id;
	unsigned int rt_pcim0;
	unsigned int rt_pcim1;
	unsigned int rt_pcim2;
	unsigned int rt_pcim3;
	unsigned int rt_pciio0;
	unsigned int rt_pciio1;
	unsigned int rt_pciio2;
	unsigned int rt_pciio3;
	unsigned int rt_pcimp_b0;
	unsigned int rt_pcimp_b1;
	unsigned int rt_pcimp_b2;
	unsigned int rt_pcimp_b3;
	unsigned int rt_pcimp_e0;
	unsigned int rt_pcimp_e1;
	unsigned int rt_pcimp_e2;
	unsigned int rt_pcimp_e3;
	unsigned int rt_pcicfgb;
	unsigned long rt_msi;
	unsigned int iommu_ctrl;
	unsigned long iommu_ptbar;
	unsigned long iommu_dtbar;
	unsigned long iommu_err;
	unsigned long iommu_err_info;
	unsigned int prepic_ctrl2;
	unsigned int prepic_err_stat;
	unsigned int prepic_err_int;
	unsigned int prepic_linp0;
	unsigned int prepic_linp1;
	unsigned int prepic_linp2;
	unsigned int prepic_linp3;
	unsigned int prepic_linp4;
	unsigned int prepic_linp5;
} kvm_guest_nbsr_state_t;

#endif	/* __ASSEMBLY__ */

/*
 * Flags specifying guest properties (see field flags above)
 */
#define	NATIVE_KERNEL_IMAGE_GUEST_FLAG		0x0000000000000001UL
#define	PARAVIRT_KERNEL_IMAGE_GUEST_FLAG	0x0000000000000002UL
#define	LINTEL_IMAGE_GUEST_FLAG			0x0000000000000100UL

#define	E2K_SYSCALL_TRAP_ENTRY_SIZE	(2 * 1024)	/* 2Kb */
#define	KVM_GUEST_STARTUP_SYSCALL_NUM	12	/* # of system call to launch */
						/* guest using such method */
#define	KVM_GUEST_STARTUP_ENTRY_NUM	32	/* # of trap table entry to */
						/* launch guest using direct */
						/* control transfer (call or */
						/* return) */
#define	KVM_PV_VCPU_TRAP_ENTRY_NUM	36	/* # of trap table entry to */
						/* launch trap handler of */
						/* paravirtualized guest */
						/* (same as ttable #0) */


#define GUEST_COMPAT_FAST_SYSCALL_TRAP_NUM	5
#define GUEST_FAST_SYSCALL_TRAP_NUM		6
#define GUEST_PROT_FAST_SYSCALL_TRAP_NUM	7

#ifndef __ASSEMBLY__

#define KVM_GET_ARCH_API_VERSION	_IO(KVMIO, 0xe1)
#define KVM_VCPU_THREAD_SETUP		_IO(KVMIO, 0xe0)
#define KVM_GET_GUEST_ADDRESS		_IOWR(KVMIO, 0xe2, unsigned long *)
#define KVM_RESET_E2K_VCPU		_IO(KVMIO, 0xe3)
#define KVM_ALLOC_GUEST_AREA		_IOWR(KVMIO, 0xe4,	\
						kvm_guest_area_alloc_t)
#define KVM_VCPU_GUEST_STARTUP		_IOW(KVMIO, 0xe5,	\
						kvm_vcpu_guest_startup_t)
#define KVM_SET_KERNEL_IMAGE_SHADOW	_IOW(KVMIO, 0xe6,	\
						kvm_kernel_area_shadow_t)
#define KVM_SET_IRQCHIP_BASE		_IOW(KVMIO, 0xe7, unsigned long)
#define	KVM_SET_SYS_TIMER_BASE		_IOW(KVMIO, 0xe8, unsigned long)
#define	KVM_SET_SPMC_CONF_BASE		_IOW(KVMIO, 0xe9, unsigned long)
#define KVM_RESERVE_GUEST_AREA		_IOWR(KVMIO, 0xea,	\
						kvm_guest_area_reserve_t)
#define	KVM_SET_SPMC_CONF_BASE_SPMC_IN_QEMU	\
					_IOW(KVMIO, 0xeb, unsigned long)
#define	KVM_SET_I2C_SPI_CONF_BASE	_IOW(KVMIO, 0xec, unsigned long)
#define KVM_SET_GUEST_INFO		_IOW(KVMIO, 0xed, unsigned long)
#define KVM_GET_NBSR_STATE		_IOR(KVMIO, 0xee,	\
						kvm_guest_nbsr_state_t)
#define	KVM_CREATE_SIC_NBSR		_IO(KVMIO, 0xef)
#define	KVM_SET_PCI_REGION		_IOW(KVMIO, 0xf0, kvm_pci_region_t)
#define	KVM_SET_COUNT_NUMA_NODES	_IOW(KVMIO, 0xf1, unsigned long)
#define	KVM_SET_MAX_NR_NODE_CPU		_IOW(KVMIO, 0xf2, unsigned long)
#define	KVM_SET_CEPIC_FREQUENCY		_IOW(KVMIO, 0xf3, unsigned long)
#define	KVM_SET_WD_PRESCALER_MULT	_IOW(KVMIO, 0xf4, unsigned long)
/* IOCTL 0xf5 reserved for Imagination GPU passthrough */
#define	KVM_SET_LEGACY_VGA_PASSTHROUGH	_IOW(KVMIO, 0xf6, unsigned long)

/* e2k-specific exit reasons from KVM to userspace assistance */
#define KVM_EXIT_E2K_NOTIFY_IO		33
#define KVM_EXIT_E2K_SHUTDOWN		36
#define KVM_EXIT_E2K_RESTART		37
#define KVM_EXIT_E2K_PANIC		38
#define	KVM_EXIT_E2K_INTR		39
#define	KVM_EXIT_E2K_UNKNOWN		44

#endif	/* __ASSEMBLY__ */

#endif /* _ASM_E2K_KVM_H */
