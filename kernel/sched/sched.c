/** @file sched.c
 * 
 * @brief Top level implementation of the scheduler.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-20
 */
#include <exports.h>

#include <types.h>
#include <assert.h>

#include <kernel.h>
#include <config.h>
#include "sched_i.h"

#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/physmem.h>

tcb_t system_tcb[OS_MAX_TASKS]; /*allocate memory for system TCBs */

void sched_init()
{
	unsigned int index;
	for  (index = 0; index < OS_MAX_TASKS; index++) {
		system_tcb[index].native_prio = 0;
		system_tcb[index].cur_prio = 0;
		system_tcb[index].context.r4 = 0;
		system_tcb[index].context.r5 = 0;
		system_tcb[index].context.r6 = 0;
		system_tcb[index].context.r7 = 0;
		system_tcb[index].context.r8 = 0;
		system_tcb[index].context.r9 = 0;
		system_tcb[index].context.r10 = 0;
		system_tcb[index].context.r11 = 0;
		system_tcb[index].context.sp = (void *)0;
		system_tcb[index].context.lr = (void *)0;
	}
}

/**
 * @brief This is the idle task that the system runs when no other task is runnable
 */
 
static void __attribute__((unused)) idle(void)
{
	 enable_interrupts();
	 while(1);
}

/**
 * @brief Allocate user-stacks and initializes the kernel contexts of the
 * given threads.
 *
 * This function assumes that:
 * - num_tasks < number of tasks allowed on the system.
 * - the tasks have already been deemed schedulable and have been appropriately
 *   scheduled.  In particular, this means that the task list is sorted in order
 *   of priority -- higher priority tasks come first.
 *
 * @param tasks  A list of scheduled task descriptors.
 * @param size   The number of tasks is the list.
 */
void allocate_tasks(task_t** tasks  __attribute__((unused)), size_t num_tasks  __attribute__((unused)))
{
	//puts("allocate_tasks\n");
	runqueue_init();
	sched_init();

	unsigned int index;
	for  (index = 0; index < num_tasks; index++) {
		system_tcb[index].native_prio = index;
		system_tcb[index].cur_prio = index;
		system_tcb[index].context.r4 = (uint32_t) (*tasks + index)->lambda;
		system_tcb[index].context.r5 = (uint32_t) (*tasks + index)->data;
		system_tcb[index].context.r6 = (uint32_t) (*tasks + index)->stack_pos;
		system_tcb[index].context.r7 = 0;
		system_tcb[index].context.r8 = 0;
		system_tcb[index].context.r9 = 0;
		system_tcb[index].context.r10 = 0;
		system_tcb[index].context.r11 = 0;
		system_tcb[index].context.sp = (void *)(0xa3800000 + index * 0x1000);
		system_tcb[index].context.lr = (void *)launch_task;
/*
		printf("r4: %x\n", system_tcb[index].context.r4);
		printf("r5: %x\n", system_tcb[index].context.r5);
		printf("r6: %x\n", system_tcb[index].context.r6);
		printf("sp: %x\n", (uint32_t) system_tcb[index].context.sp);
		printf("lr: %x\n", (uint32_t) system_tcb[index].context.lr);
*/
		runqueue_add(&system_tcb[index], system_tcb[index].native_prio);
	}

	system_tcb[IDLE_PRIO].native_prio = IDLE_PRIO;
	system_tcb[IDLE_PRIO].cur_prio = IDLE_PRIO;
	system_tcb[IDLE_PRIO].context.r4 = (uint32_t) idle;
	system_tcb[IDLE_PRIO].context.r5 = 0;
	system_tcb[IDLE_PRIO].context.r6 = 0;
	system_tcb[IDLE_PRIO].context.r7 = 0;
	system_tcb[IDLE_PRIO].context.r8 = 0;
	system_tcb[IDLE_PRIO].context.r9 = 0;
	system_tcb[IDLE_PRIO].context.r10 = 0;
	system_tcb[IDLE_PRIO].context.r11 = 0;
	system_tcb[IDLE_PRIO].context.sp = (void *)(0xa3800000 + IDLE_PRIO * 0x1000);
	system_tcb[IDLE_PRIO].context.lr = (void *)launch_task;
/*
		printf("r4: %x\n", system_tcb[IDLE_PRIO].context.r4);
		printf("r5: %x\n", system_tcb[IDLE_PRIO].context.r5);
		printf("r6: %x\n", system_tcb[IDLE_PRIO].context.r6);
		printf("sp: %x\n", (uint32_t) system_tcb[IDLE_PRIO].context.sp);
		printf("lr: %x\n", (uint32_t) system_tcb[IDLE_PRIO].context.lr);
*/
	runqueue_add(&system_tcb[IDLE_PRIO], system_tcb[IDLE_PRIO].native_prio);
	
	dispatch_init(&system_tcb[IDLE_PRIO]);
	dispatch_nosave();
}

