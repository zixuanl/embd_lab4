/** @file proc.c
 * 
 * @brief Implementation of `process' syscalls
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 14 Oct 2007 00:07:38 -0400
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-12
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

void task_exchange (task_t * task1,task_t * task2) 
{
	task_t tmp_task;

	tmp_task.lambda = task1->lambda;
	tmp_task.data = task1->data;
	tmp_task.stack_pos = task1->stack_pos;
	tmp_task.C = task1->C;
	tmp_task.T = task1->T;

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
	//puts("in task_create\n");
	size_t i, j;
	size_t min;
	uint32_t tmp1;
	uint32_t tmp2;
	float sum = 0;
	if (num_tasks > OS_MAX_TASKS)
		return EINVAL;

	for (i = 0; i < num_tasks; i++)
	{	
		tmp1 = (uint32_t)tasks[i].stack_pos;
		tmp2 = (uint32_t)tasks[i].lambda;
		/* check the user_stack position and task_fun address 
		 * leave the first 0x10000 for user program
		 * a valid stack position should be between 0xa0010000 and 0xa3000000
		 * a valid task_fun address should be between 0xa0000000 and 0xa3000000
		 */
		if ( tmp1 > USR_STACK || tmp1 < (LOAD_ADDR + 0x10000))
			return EFAULT;
		if ( tmp2 > USR_END_ADDR || tmp2 < USR_START_ADDR )
			return EFAULT;
		sum = tasks[i].C/tasks[i].T;
	}
	
	/* test whether task set is schedulable */
	if (sum > 1)		
		return ESCHED;
	
	for (i = 0; i < num_tasks; i++) {
		min = i;
		for (j = i+1; j < num_tasks; j++ ) {
			if(tasks[min].T > tasks[j].T)
				min = j;
		}
		task_exchange(&tasks[i], &tasks[min]);
	}
	/*
	for (i = 0; i < num_tasks; i++) {
		printf("%lu\n", tasks[i].T);

	}
	*/
	dev_init();
	allocate_tasks(&tasks, num_tasks);
	return 1;
}

int event_wait_syscall(unsigned int dev)
{
	dev_wait(dev);
	return 1;
}

/* An invalid syscall causes the kernel to exit. */
void invalid_syscall(unsigned int call_num)
{
	printf("Kernel panic: invalid syscall -- 0x%08x\n", call_num);
	disable_interrupts();
	while(1);
}
