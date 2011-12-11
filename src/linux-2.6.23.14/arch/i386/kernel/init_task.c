#include <linux/mm.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/init_task.h>
#include <linux/fs.h>
#include <linux/mqueue.h>

#include <asm/uaccess.h>
#include <asm/pgtable.h>
#include <asm/desc.h>

#ifndef __LINSCHED__
static struct fs_struct init_fs = INIT_FS;
static struct files_struct init_files = INIT_FILES;
static struct signal_struct init_signals = INIT_SIGNALS(init_signals);
static struct sighand_struct init_sighand = INIT_SIGHAND(init_sighand);
struct mm_struct init_mm = INIT_MM(init_mm);
#else
#include <linux/personality.h>
static struct fs_struct init_fs;
static struct files_struct init_files;
static struct signal_struct init_signals;
static struct sighand_struct init_sighand;
struct mm_struct init_mm;
struct nsproxy init_nsproxy;
struct pid init_struct_pid = INIT_STRUCT_PID;
struct user_struct root_user;
struct exec_domain default_exec_domain;
long do_no_restart_syscall(struct restart_block *param)
{
        return -EINTR;
}
#endif /* __LINSCHED__ */

EXPORT_SYMBOL(init_mm);

/*
 * Initial thread structure.
 *
 * We need to make sure that this is THREAD_SIZE aligned due to the
 * way process stacks are handled. This is done by having a special
 * "init_task" linker map entry..
 */
union thread_union init_thread_union 
	__attribute__((__section__(".data.init_task"))) =
		{ INIT_THREAD_INFO(init_task) };

/*
 * Initial task structure.
 *
 * All other task structs will be allocated on slabs in fork.c
 */
struct task_struct init_task = INIT_TASK(init_task);

EXPORT_SYMBOL(init_task);

#ifndef __LINSCHED__

/*
 * per-CPU TSS segments. Threads are completely 'soft' on Linux,
 * no more per-task TSS's.
 */ 
DEFINE_PER_CPU_SHARED_ALIGNED(struct tss_struct, init_tss) = INIT_TSS;

#endif /* __LINSCHED__ */
