/* LinSched -- The Linux Scheduler Simulator
 * Copyright (C) 2008  John M. Calandrino
 * E-mail: jmc@cs.unc.edu
 *
 * This file contains Linux variables and functions that have been "defined
 * away" or exist here in a modified form to avoid including an entire Linux
 * source file that might otherwise lead to a "cascade" of dependency issues.
 * It also includes certain LinSched variables to which some Linux functions
 * and definitions now map.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program (see COPYING); if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

/* To support printing debugging and other messages to the console. */
#include <stdio.h>
/* linsched variables and functions */








/* Knn */




// Number of parameters
#define NPARAM 3
// Size of data
#define MAX_ENTRY 5
// No of scheduling classes
#define NDECISIONS 20

struct Data{
    int params[NPARAM];
    int decision;
    int time;
};

int* calc_dist(int*, struct Data*);   
int* sort_dist(int*);
int find_max(int *,int);
int sq(int a){return a*a;};
int* find_decisions(int *,int *,struct Data*, int);
void init(struct Data*);
int assign_class(int);
void print_arr(int*,int);
void read_data(struct Data*, int*,char*);
int knn(int *,int);
   
int knn(int *query,int K)
{
    int i,j;
    struct Data* set_of_data;
    int* dist;
    int *sorted_dist;
    int* set_of_decisions;
    int decision;
        
    set_of_data = (struct Data*)malloc( (MAX_ENTRY)*sizeof(struct Data) );

    // Input
    read_data(set_of_data,query,"inp");
    // Calculate Distance
    dist = calc_dist(query, set_of_data);   
    // Sort distance
    sorted_dist = sort_dist(dist);
    // Find decisions for first k elements
    set_of_decisions =  find_decisions(sorted_dist, dist, set_of_data, K);
    // Find max of decisions and return it
    decision = find_max(set_of_decisions,K);
    return decision;
}
int* calc_dist(int *query, struct Data* set_of_data)
{
    int i,j;
    int *dist = (int *)malloc(MAX_ENTRY*sizeof(int));
    
    for(i=0;i<MAX_ENTRY;i++){
        dist[i] = 0;
        for(j=0;j<NPARAM;j++){
            dist[i] += sq( query[j] - set_of_data[i].params[j]);
        }
    }
    return dist;
}
int* sort_dist(int* dist)
{
    int i,j,temp=0;
    int *sorted_dist = (int *)malloc(MAX_ENTRY*sizeof(int));
    for(i=0;i<MAX_ENTRY;i++)sorted_dist[i]=dist[i];
    for(i=0;i<MAX_ENTRY;i++){
        for(j=0;j<MAX_ENTRY;j++){
            if(sorted_dist[i]<sorted_dist[j])
            {
                temp = sorted_dist[i];
                sorted_dist[i] = sorted_dist[j];
                sorted_dist[j] = temp;
            }
        }
    }
    return sorted_dist;
}

int* find_decisions(int *sorted_dist,int *dist,struct Data* set_of_data, int k_n)
{
    int *set_of_decisions = (int *)malloc(k_n*sizeof(int));
    int i,j,x=0;
    for(i=0;i<k_n;i++){
        for(j=0;j<MAX_ENTRY;j++){
            if(sorted_dist[i]==dist[j]){
                set_of_decisions[x++]=set_of_data[j].decision;
                break;
            }
        }
    }
    return set_of_decisions;
}

int find_max(int *set_of_decisions,int k_n)
{
    int decision,i,max_count,c;
    int *count_decisions = malloc(NDECISIONS*sizeof(int));
    for(i=0;i<NDECISIONS;i++)count_decisions[i]=0;
    decision = -1;max_count = -1;
    for(i=0;i<k_n;i++){
        c = ++count_decisions[ set_of_decisions[i] ];
        if(c>max_count){max_count=c;decision=set_of_decisions[i];}
    }
    return decision;
}

void data_init(struct Data* d)
{
    int i=0;
    for(i=0;i<NPARAM;i++) d->params[i] = 0;
    d->decision = 0;
    d->time = 0;
}
int assign_class(int nticks)
{
    int class;
    // TODO:Assign class to ticks
    return class;
}

void read_data(struct Data* set_of_data, int* query, char *filename)
{
    FILE *inp;
    int i,j;
    inp = fopen(filename,"r");
    for(i=0;i<MAX_ENTRY;i++)
    {
        for(j=0;j<NPARAM;j++)
            fscanf(inp,"%d",&set_of_data[i].params[j]);
        fscanf(inp,"%d",&set_of_data[i].decision);
    }
}

void print_arr(int *arr,int size){int i;for(i=0;i<size;i++)printf("%d ",arr[i]);printf("\n");};




int __linsched_curr_cpu = 0;
struct task_struct *__linsched_tasks[LINSCHED_MAX_TASKS];
int curr_task_id = 0;

void linsched_change_cpu(int cpu) { __linsched_curr_cpu = cpu; }
void linsched_init_cpus(void) { /* Look at Tong's work to init. */ }
void linsched_init(void)
{
	/* Initialize platform. For now, this does nothing, since
	 * the number of CPUs is known, and we do not yet support
	 * MC/SMT/NUMA. Scheduling decisions are not dependent on
	 * asymmetries yet, either, but that could change.
	 */
	linsched_init_cpus();

	/* Initialize random number generator. */
	linsched_random_init(LINSCHED_RAND_SEED);

	/* Change context to "boot" cpu and boot kernel. */
	linsched_change_cpu(0);
	start_kernel();
}

unsigned long long time_start;
unsigned long long time_end;
int sim_param;
int sim_param_count = 0;

void linsched_default_callback(void) { }
void linsched_exit_callback(void) { do_exit(0); }
void linsched_announce_callback_bin(void)
{
	long i;
	long my_time;
	//my_time = sys_time();
	//printf("time is %lu",my_time);
	printf("CPU %d / t = %u: Task 0x%x scheduled.\n", smp_processor_id(),
               (unsigned int)jiffies, (unsigned int)current);

        		
	for(i = 0;i<100; i++)
	{
	
	
	}

	sim_param_count++;
	
	if (sim_param == sim_param_count)
	{
		time_end = sched_clock();
		printf("Turn around time %llu\n", time_start-time_end);
		do_exit(0);
        }




//	do_exit(0);
	//my_time = sys_time();
        //printf("time is %lu",my_time);
}

void linsched_announce_callback(void)
{
        long i;
        long my_time;
        //my_time = sys_time();
        //printf("time is %lu",my_time);
        printf("CPU %d / t = %u: Task 0x%x scheduled.\n", smp_processor_id(),
               (unsigned int)jiffies, (unsigned int)current);


        for(i = 0;i<100; i++)
        {


        }

}








void linsched_announce_callback_1(void)
{
        long i;
        long my_time;
        //my_time = sys_time();
        //printf("time is %lu",my_time);
        printf("CPU %d / t = %u: Task 0x%x scheduled.\n", smp_processor_id(),
               (unsigned int)jiffies, (unsigned int)current);


        for(i = 0; i<100;i++)
        {


        }

        do_exit(0);
        //my_time = sys_time();
        //printf("time is %lu",my_time);
}









void linsched_disable_migrations(void)
{
	int i;

	for (i = 0; i < curr_task_id; i++)
		set_cpus_allowed(__linsched_tasks[i],
				 cpumask_of_cpu(
					 task_cpu(__linsched_tasks[i])));
}

void linsched_enable_migrations(void)
{
	int i;

	for (i = 0; i < curr_task_id; i++)
		set_cpus_allowed(__linsched_tasks[i], CPU_MASK_ALL);
}

/* Needed for migration to work correctly. */
void linsched_run_sim(int sim_ticks);

/* Force a migration of task to the dest_cpu.
 * If migr is set, allow migrations after the forced migration... otherwise,
 * do not allow them. (We need to disable migrations so that the forced
 * migration takes place correctly.)
 * Returns old cpu of task.
 */
int linsched_force_migration(struct task_struct *task, int dest_cpu, int migr)
{
	int old_cpu = task_cpu(task);
	
	linsched_disable_migrations();
	set_cpus_allowed(task, cpumask_of_cpu(dest_cpu));
	linsched_change_cpu(old_cpu);
	schedule();
	linsched_change_cpu(dest_cpu);
	schedule();
	if (migr)
		linsched_enable_migrations();

	return old_cpu;
}

/* Return the task in position ta
sk_id in the task array.
 * No error checking, so be careful!
 */
struct task_struct *linsched_get_task(int task_id)
{
	return __linsched_tasks[task_id];
}

struct task_struct *__linsched_create_task(void (*callback)(void))
{
	struct task_struct *newtask =
		(struct task_struct *)do_fork(0, 0, 0, 0, 0, 0, callback);

	/* Allow task to run on any CPU. */

	set_cpus_allowed(newtask, CPU_MASK_ALL);

	return newtask;
}


/* New Version */

struct task_struct *__linsched_create_task_binary(void (*callback)(void), unsigned long time_slice)
{
        struct task_struct *newtask =
                (struct task_struct *)do_fork_binary(0, 0, 0, 0, 0, 0, callback, time_slice);

        set_cpus_allowed(newtask, CPU_MASK_ALL);

        return newtask;
}











/* Create a normal task with the specified callback and 
 * nice value of niceval, which determines its priority.
 */
void linsched_create_normal_task(void (*callback)(void), int niceval)
{
	struct sched_param params;

	/* If we cannot support any more tasks, return. */
	if (curr_task_id >= LINSCHED_MAX_TASKS)
		return;

	/* Create "normal" task and set its nice value. */
	__linsched_tasks[curr_task_id] = __linsched_create_task(callback);
	params.sched_priority = 0;
	sys_sched_setscheduler(__linsched_tasks[curr_task_id], SCHED_NORMAL,
			       &params);
	set_user_nice(__linsched_tasks[curr_task_id], niceval);

	/* Print message. */
	printf("Created normal task 0x%x with nice value %d.\n",
	       (unsigned int)__linsched_tasks[curr_task_id], niceval);

	/* Increment task id. */
	curr_task_id++;
}






/* NEW VERSION
 */
void linsched_create_normal_task_binary(struct elf_binary* binary, int niceval)
{
        struct sched_param params;
        unsigned long long time_slice;
        int query[3];
	
	query[0] = binary->size;
	query[1] = binary->bss;
	query[2] = binary->input_size; 
	/* If we cannot support any more tasks, return. */
        if (curr_task_id >= LINSCHED_MAX_TASKS)
                return;
	time_start = sched_clock();
		
	sim_param = (query[0]+query[1]+query[2])/(30*2);	
	time_slice = knn(&query[0], 3);
	time_slice *= 1000000;
	//time_slice = 500000000;
        /* Create "normal" task and set its nice value. */
        __linsched_tasks[curr_task_id] = __linsched_create_task_binary(binary->callback, time_slice);
        params.sched_priority = 0;
        


	sys_sched_setscheduler(__linsched_tasks[curr_task_id], SCHED_NORMAL,
                               &params);
        set_user_nice(__linsched_tasks[curr_task_id], niceval);

        /* Print message. */
        printf("Created normal task 0x%x with nice value %d.\n",
               (unsigned int)__linsched_tasks[curr_task_id], niceval);

        /* Increment task id. */
        curr_task_id++;



}


/* Create a batch task with the specified callback and 
 * nice value of niceval, which determines its priority.
 */
void linsched_create_batch_task(void (*callback)(void), int niceval)
{
	struct sched_param params;

	/* If we cannot support any more tasks, return. */
	if (curr_task_id >= LINSCHED_MAX_TASKS)
		return;

	/* Create "batch" task and set its nice value. */
	__linsched_tasks[curr_task_id] = __linsched_create_task(callback);
	params.sched_priority = 0;
	sys_sched_setscheduler(__linsched_tasks[curr_task_id], SCHED_BATCH,
			       &params);
	set_user_nice(__linsched_tasks[curr_task_id], niceval);

	/* Print message. */
	printf("Created batch task 0x%x with nice value %d.\n",
	       (unsigned int)__linsched_tasks[curr_task_id], niceval);

	/* Increment task id. */
	curr_task_id++;
}

/* Create a FIFO real-time task with the specified callback and priority. */
void linsched_create_RTfifo_task(void (*callback)(void), int prio)
{
	struct sched_param params;

	/* If we cannot support any more tasks, return. */
	if (curr_task_id >= LINSCHED_MAX_TASKS)
		return;

	/* Create FIFO real-time task and set its priority. */
	__linsched_tasks[curr_task_id] = __linsched_create_task(callback);
	params.sched_priority = prio;
	sys_sched_setscheduler(__linsched_tasks[curr_task_id], SCHED_FIFO,
			       &params);

        /* Print message. */
	printf("Created FIFO real-time task 0x%x with priority %d.\n",
	       (unsigned int)__linsched_tasks[curr_task_id], prio);

	/* Increment task id. */
	curr_task_id++;
}

/* Create a RR real-time task with the specified callback and priority. */
void linsched_create_RTrr_task(void (*callback)(void), int prio)
{
	struct sched_param params;

	/* If we cannot support any more tasks, return. */
	if (curr_task_id >= LINSCHED_MAX_TASKS)
		return;

	/* Create RR real-time task and set its priority. */
	__linsched_tasks[curr_task_id] = __linsched_create_task(callback);
	params.sched_priority = prio;
	sys_sched_setscheduler(__linsched_tasks[curr_task_id], SCHED_RR,
			       &params);

        /* Print message. */
	printf("Created RR real-time task 0x%x with priority %d.\n",
	       (unsigned int)__linsched_tasks[curr_task_id], prio);

	/* Increment task id. */
	curr_task_id++;
}

void linsched_yield(void)
{
	/* If the current task is not the idle task, yield. */
	if (current != idle_task(smp_processor_id()))
		yield();
}

/* Borrowed from drivers/net/hamradio/dmascc.c. */
static unsigned long __linsched_rand;
void linsched_random_init(int seed) { __linsched_rand = seed; }
unsigned long linsched_random(void)
{
        /* See "Numerical Recipes in C", second edition, p. 284 */
        __linsched_rand = __linsched_rand * 1664525L + 1013904223L;
        return (unsigned long)(__linsched_rand >> 24);
}

void linsched_run_sim(int sim_ticks)
{
	/* Run a simulation for some number of ticks. Each tick,
	 * scheduling and load balancing decisions are made. The
	 * order in which CPUs make their scheduler_tick calls
	 * is randomized. Obviously, we could create tasks,
	 * change priorities, etc., at certain ticks if we desired,
	 * rather than just running a simple simulation.
	 * (Tasks can also be removed by having them exit.)
	 */
	/* NOTE: The per-CPU "tick" is never disabled, like it might be in a
	 * real system, when a CPU goes idle. Since even the most current
	 * version of Linux maintains a periodic tick when there is
	 * actual work to do, and disabling the tick when idle would
	 * not change anything about how the scheduler behaves
	 * (it only conserves energy, which is not going to help us here),
	 * there is no need.
	 */
	

//	printf("Yeah-first_run\n");
	int initial_jiffies = jiffies;
	for (jiffies = initial_jiffies;
	     jiffies < initial_jiffies + sim_ticks;
	     jiffies++) {
		cpumask_t cpu_processed_map = CPU_MASK_NONE;
		while (!cpus_full(cpu_processed_map)) {
			int active_cpu;
			
			/* Determine next active CPU, and set as processed. */ 
			do {
				active_cpu = linsched_random() % NR_CPUS;
				//active_cpu = 1;

			} while (cpu_isset(active_cpu, cpu_processed_map));
			cpu_set(active_cpu, cpu_processed_map);

			/* Call scheduler_tick for that CPU. */
			linsched_change_cpu(active_cpu);
//			printf("Mainsimulation\n");
			scheduler_tick(); /* may trigger a schedule() call */

			/* First time executing a task? Do not need to
			 * call schedule_tail, since we are not actually
			 * performing a "real" context switch.
			 */
		}
	}
}

/* Linux variables and functions */

struct group_info init_groups = { .usage = ATOMIC_INIT(2) };
DEFINE_PER_CPU(struct task_struct *, current_task) = &init_task;
unsigned long volatile __jiffy_data jiffies = INITIAL_JIFFIES;
cpumask_t cpu_online_map = CPU_MASK_ALL;
cpumask_t cpu_possible_map = CPU_MASK_ALL;
unsigned int cpu_khz; // needs correct initialization depending on core speeds
__cacheline_aligned DEFINE_RWLOCK(tasklist_lock); 
struct task_struct *kthreadd_task;

void __lockfunc _spin_lock(spinlock_t *lock) { }
void __lockfunc _spin_unlock(spinlock_t *lock) { }
void __lockfunc _spin_lock_irq(spinlock_t *lock) { }
void __lockfunc _spin_unlock_irq(spinlock_t *lock) { }
unsigned long __lockfunc _spin_lock_irqsave(spinlock_t *lock) { return 0; }
void __lockfunc _spin_unlock_irqrestore(spinlock_t *lock,
		unsigned long flags) { }
void __lockfunc _read_lock(rwlock_t *lock) { }
void __lockfunc _read_unlock(rwlock_t *lock) { }
int __lockfunc _spin_trylock(spinlock_t *lock) { return 1; }
int __lockfunc __reacquire_kernel_lock(void) { return 0; }
void __lockfunc __release_kernel_lock(void) { }
void lock_acquire(struct lockdep_map *lock, unsigned int subclass,
  		int trylock, int read, int check, unsigned long ip) { }
void lock_release(struct lockdep_map *lock, int nested, unsigned long ip) { }
void inline fastcall __sched mutex_lock(struct mutex *lock) { }
void fastcall __sched mutex_unlock(struct mutex *lock) { }
void rt_mutex_adjust_pi(struct task_struct *task) { }
int rt_mutex_getprio(struct task_struct *task) { return task->normal_prio; }
void trace_hardirqs_on(void) { }
void trace_hardirqs_off(void) { }
void fastcall add_preempt_count(int val) { }
void fastcall sub_preempt_count(int val) { }

asmlinkage int printk(const char *fmt, ...) { return 0; }
void dump_stack(void) { }
struct sighand_struct *lock_task_sighand(struct task_struct *tsk,
		unsigned long *flags) { return tsk->sighand; }
unsigned int debug_smp_processor_id(void) { return __linsched_curr_cpu; }
int capable(int cap) { return 1; }
void fire_sched_out_preempt_notifiers(struct task_struct *curr,
		struct task_struct *next) { }
void fire_sched_in_preempt_notifiers(struct task_struct *curr) { }

/* These functions do not copy to and from user space anymore, so
 * they are just memory copy functions now.
 */
unsigned long
copy_from_user(void *to, const void __user *from, unsigned long n)
{
        memcpy(to, from, n);
        return 0;
}
unsigned long
copy_to_user(void __user *to, const void *from, unsigned long n)
{
        memcpy(to, from, n);
        return 0;
}

/* find_task_by_pid_type: just a typecast is performed,
 * no actual mapping/hashing.
 */
struct task_struct *find_task_by_pid_type(int type, pid_t nr)
{
        return (struct task_struct*)nr;
}

/* These functions have been redefined so that the static function
 * run_rebalance_domains in sched.c can be called from here
 * through a function pointer.
 */
//void (*run_rebalance_domains)(struct softirq_action*);
//void open_softirq(int nr, void (*action)(struct softirq_action*),
//		void *data) { run_rebalance_domains = action; }
//void fastcall raise_softirq(unsigned int nr) { (*run_rebalance_domains)(0); }

void ktime_get_ts(struct timespec *ts)
{
	unsigned long long curr_ns = sched_clock();
	ts->tv_sec = curr_ns / NSEC_PER_SEC;
	ts->tv_nsec = curr_ns % NSEC_PER_SEC;
	if (ts->tv_nsec < 0) {
		ts->tv_sec--;
		ts->tv_nsec += NSEC_PER_SEC;
	}
}

