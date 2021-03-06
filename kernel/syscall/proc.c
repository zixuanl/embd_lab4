/** @file proc.c
 * 
 * @brief Implementation of `process' syscalls
 *
 * Authors: Tianyi Huang <tianyih@andrew.cmu.edu>
 *          Zixuan Liu <zixuanl@andrew.cmu.edu>
 *	    Jianan Lu <jiananl@andrew.cmu.edu>
 * Date:    11/23/2013
 */

#include <exports.h>
#include <bits/errno.h>
#include <config.h>
#include <kernel.h>
#include <syscall.h>
#include <sched.h>


#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/physmem.h>
#include <device.h>
#include <task.h>

/* func: task_exchange
 * 	 used as a helper to swap two tasks in the tasks[], in order to sort the tasks 
 */
void task_exchange (task_t * task1, task_t * task2) 
{
	// tmp acts as an intermediate 
	task_t tmp_task;

	tmp_task.lambda = task1->lambda;
	tmp_task.data = task1->data;
	tmp_task.stack_pos = task1->stack_pos;
	tmp_task.C = task1->C;
	tmp_task.T = task1->T;

	// swap task 1 and task 2
	task1->lambda = task2->lambda;
	task1->data = task2->data;
	task1->stack_pos = task2->stack_pos;
	task1->C = task2->C;
	task1->T = task2->T;

	task2->lambda = tmp_task.lambda;
	task2->data = tmp_task.data;
	task2->stack_pos = tmp_task.stack_pos;
	task2->C = tmp_task.C;
	task2->T = tmp_task.T;

}

int task_create_syscall(task_t* tasks, size_t num_tasks)
{
	size_t i, j;
	size_t min;
	uint32_t tmp1;
	uint32_t tmp2;
	uint32_t tmp3;
	float sum = 0;
	
	// error checking for num_tasks
	if (num_tasks > OS_MAX_TASKS)
		return EINVAL;
	tmp3 = (uint32_t) tasks;

	if ( tmp3 > USR_END_ADDR || tmp3 < USR_START_ADDR ) 
		return EFAULT;


	// test func addr and stack addr
	for (i = 0; i < num_tasks; i++)
	{	
		tmp1 = (uint32_t)tasks[i].stack_pos;
		tmp2 = (uint32_t)tasks[i].lambda;

		/* check the user_stack position and task_fun address 
		 * leave the first 0x10000 for user program
		 * a valid stack position should be between 0xa0010000 and 0xa3000000
		 * a valid task_fun address should be between 0xa0000000 and 0xa3000000
		 */
		if ( tmp1 > USR_END_ADDR || tmp1 < USR_START_ADDR )
			return EFAULT;
		if ( tmp2 > USR_END_ADDR || tmp2 < USR_START_ADDR )
			return EFAULT;
		sum = tasks[i].C/tasks[i].T;
	}
	
	/* test whether task set is schedulable */
	if (sum > 1)
		return ESCHED;

	// sort tasks according to their freq
	for (i = 0; i < num_tasks; i++) {
		min = i;
		for (j = i+1; j < num_tasks; j++ ) {
			if(tasks[min].T > tasks[j].T)
				min = j;
		}
		task_exchange(&tasks[i], &tasks[min]);
	}

	// init devices
	dev_init();

	// construct tcbs
	allocate_tasks(&tasks, num_tasks);

	// upon success, it will not return 
	return 1;
}

int event_wait_syscall(unsigned int dev)
{
	// test if dev number is valid
	if (dev > NUM_DEVICES - 1) {
		return EINVAL;
	}
	
	// wait for device
	dev_wait(dev);

	// if success, return 0 
	return 0;
}

/* An invalid syscall causes the kernel to exit. */
void invalid_syscall(unsigned int call_num)
{
	printf("Kernel panic: invalid syscall -- 0x%08x\n", call_num);
	disable_interrupts();
	while(1);
}
