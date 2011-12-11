#ifndef __ARCH_I386_PERCPU__
#define __ARCH_I386_PERCPU__

#ifdef __ASSEMBLY__

/*
 * PER_CPU finds an address of a per-cpu variable.
 *
 * Args:
 *    var - variable name
 *    reg - 32bit register
 *
 * The resulting address is stored in the "reg" argument.
 *
 * Example:
 *    PER_CPU(cpu_gdt_descr, %ebx)
 */
#ifdef CONFIG_SMP
#define PER_CPU(var, reg)				\
	movl %fs:per_cpu__##this_cpu_off, reg;		\
	lea per_cpu__##var(reg), reg
#define PER_CPU_VAR(var)	%fs:per_cpu__##var
#else /* ! SMP */
#define PER_CPU(var, reg)			\
	movl $per_cpu__##var, reg
#define PER_CPU_VAR(var)	per_cpu__##var
#endif	/* SMP */

#else /* ...!ASSEMBLY */

#ifndef __LINSCHED__

/*
 * PER_CPU finds an address of a per-cpu variable.
 *
 * Args:
 *    var - variable name
 *    cpu - 32bit register containing the current CPU number
 *
 * The resulting address is stored in the "cpu" argument.
 *
 * Example:
 *    PER_CPU(cpu_gdt_descr, %ebx)
 */
#ifdef CONFIG_SMP
/* Same as generic implementation except for optimized local access. */
#define __GENERIC_PER_CPU

/* This is used for other cpus to find our section. */
extern unsigned long __per_cpu_offset[];

#define per_cpu_offset(x) (__per_cpu_offset[x])

/* Separate out the type, so (int[3], foo) works. */
#define DECLARE_PER_CPU(type, name) extern __typeof__(type) per_cpu__##name
#define DEFINE_PER_CPU(type, name) \
    __attribute__((__section__(".data.percpu"))) __typeof__(type) per_cpu__##name

#define DEFINE_PER_CPU_SHARED_ALIGNED(type, name)		\
    __attribute__((__section__(".data.percpu.shared_aligned"))) \
    __typeof__(type) per_cpu__##name				\
    ____cacheline_aligned_in_smp

/* We can use this directly for local CPU (faster). */
DECLARE_PER_CPU(unsigned long, this_cpu_off);

/* var is in discarded region: offset to particular copy we want */
#define per_cpu(var, cpu) (*({				\
	extern int simple_indentifier_##var(void);	\
	RELOC_HIDE(&per_cpu__##var, __per_cpu_offset[cpu]); }))

#define __raw_get_cpu_var(var) (*({					\
	extern int simple_indentifier_##var(void);			\
	RELOC_HIDE(&per_cpu__##var, x86_read_percpu(this_cpu_off));	\
}))

#define __get_cpu_var(var) __raw_get_cpu_var(var)

/* A macro to avoid #include hell... */
#define percpu_modcopy(pcpudst, src, size)			\
do {								\
	unsigned int __i;					\
	for_each_possible_cpu(__i)				\
		memcpy((pcpudst)+__per_cpu_offset[__i],		\
		       (src), (size));				\
} while (0)

#define EXPORT_PER_CPU_SYMBOL(var) EXPORT_SYMBOL(per_cpu__##var)
#define EXPORT_PER_CPU_SYMBOL_GPL(var) EXPORT_SYMBOL_GPL(per_cpu__##var)

/* fs segment starts at (positive) offset == __per_cpu_offset[cpu] */
#define __percpu_seg "%%fs:"
#else  /* !SMP */
#include <asm-generic/percpu.h>
#define __percpu_seg ""
#endif	/* SMP */

/* For arch-specific code, we can use direct single-insn ops (they
 * don't give an lvalue though). */
extern void __bad_percpu_size(void);

#define percpu_to_op(op,var,val)				\
	do {							\
		typedef typeof(var) T__;			\
		if (0) { T__ tmp__; tmp__ = (val); }		\
		switch (sizeof(var)) {				\
		case 1:						\
			asm(op "b %1,"__percpu_seg"%0"		\
			    : "+m" (var)			\
			    :"ri" ((T__)val));			\
			break;					\
		case 2:						\
			asm(op "w %1,"__percpu_seg"%0"		\
			    : "+m" (var)			\
			    :"ri" ((T__)val));			\
			break;					\
		case 4:						\
			asm(op "l %1,"__percpu_seg"%0"		\
			    : "+m" (var)			\
			    :"ri" ((T__)val));			\
			break;					\
		default: __bad_percpu_size();			\
		}						\
	} while (0)

#define percpu_from_op(op,var)					\
	({							\
		typeof(var) ret__;				\
		switch (sizeof(var)) {				\
		case 1:						\
			asm(op "b "__percpu_seg"%1,%0"		\
			    : "=r" (ret__)			\
			    : "m" (var));			\
			break;					\
		case 2:						\
			asm(op "w "__percpu_seg"%1,%0"		\
			    : "=r" (ret__)			\
			    : "m" (var));			\
			break;					\
		case 4:						\
			asm(op "l "__percpu_seg"%1,%0"		\
			    : "=r" (ret__)			\
			    : "m" (var));			\
			break;					\
		default: __bad_percpu_size();			\
		}						\
		ret__; })

#define x86_read_percpu(var) percpu_from_op("mov", per_cpu__##var)
#define x86_write_percpu(var,val) percpu_to_op("mov", per_cpu__##var, val)
#define x86_add_percpu(var,val) percpu_to_op("add", per_cpu__##var, val)
#define x86_sub_percpu(var,val) percpu_to_op("sub", per_cpu__##var, val)
#define x86_or_percpu(var,val) percpu_to_op("or", per_cpu__##var, val)

#else /* __LINSCHED__ */

#define __GENERIC_PER_CPU

/* This per-cpu stuff is kind of a mess right now. Declarations
 * and definitions work okay, assuming that you do some additional
 * work for *every* per-cpu variable in setup_per_cpu_areas. This
 * does not scale at all, and will need to be reworked at some point.
 * It's possible that the ultimate solution will be to completely
 * scrap the per-cpu stuff altogether, but that seems equally drastic.
 */

extern unsigned int debug_smp_processor_id(void);

#define DECLARE_PER_CPU(type, name) \
	extern __typeof__(type) *per_cpu__##name

/* per_cpu variables are pointers that become arrays, and
 * are the actual per_cpu variables referenced after
 * everything is setup correctly.
 * per_cpu_ptr variables are pointers to these pointers,
 * which are necessary so that we can initialize
 * static per_cpu variables in init/main.c.
 * per_cpu_size variables indicate the size of the
 * type, so that the pointers can be correctly initialized
 * even if the type is not defined in init/main.c.
 * per_cpu_ptr_init is the value to which each element
 * in this "array" should be initialized.
 * (Yes, this is ugly.)
 */
#define DEFINE_PER_CPU(type, name) \
    	__typeof__(type) *per_cpu__##name; \
	void **per_cpu_ptr__##name = (void *)&per_cpu__##name; \
	int per_cpu_size__##name = sizeof(__typeof__(type)); \
	__typeof__(type) per_cpu_init__##name

#define DEFINE_PER_CPU_SHARED_ALIGNED(type, name)	\
	DEFINE_PER_CPU(type, name)

#define DECLARE_PER_CPU_INIT_VARS(type, name) \
	extern void **per_cpu_ptr__##name; \
	extern int per_cpu_size__##name; \
	extern __typeof__(type) per_cpu_init__##name

#define ALLOC_PER_CPU_MEM(name) \
	*per_cpu_ptr__##name = malloc(per_cpu_size__##name*NR_CPUS)

#define INIT_PER_CPU_MEM(name, cpu) \
	memcpy(*per_cpu_ptr__##name + sizeof(void *)*cpu, \
	       &per_cpu_init__##name, per_cpu_size__##name)

#define per_cpu(var, cpu) (*(per_cpu__##var + cpu))

#define __raw_get_cpu_var(var) per_cpu(var, debug_smp_processor_id())

#define x86_read_percpu(var) __raw_get_cpu_var(var)

#define __get_cpu_var(var) __raw_get_cpu_var(var)

#define x86_write_percpu(var,val) do { __raw_get_cpu_var(var) = val; } while (0)

#endif /* __LINSCHED__ */

#endif /* !__ASSEMBLY__ */

#endif /* __ARCH_I386_PERCPU__ */
