/**
 * @file device.c
 *
 * @brief Implements simulated devices.
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-12-01
 */

#include <types.h>
#include <assert.h>
#include <exports.h>
#include <task.h>
#include <sched.h>
#include <device.h>
#include <systime.h>
#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>

/**
 * @brief Fake device maintainence structure.
 * Since our tasks are periodic, we can represent 
 * tasks with logical devices. 
 * These logical devices should be signalled periodically 
 * so that you can instantiate a new job every time period.
 * Devices are signaled by calling dev_update 
 * on every timer interrupt. In dev_update check if it is 
 * time to create a tasks new job. If so, make the task runnable.
 * There is a wait queue for every device which contains the tcbs of
 * all tasks waiting on the device event to occur.
 */

struct dev
{
	tcb_t* sleep_queue;
	unsigned long   next_match;
};
typedef struct dev dev_t;

/* devices will be periodically signaled at the following frequencies */
const unsigned long dev_freq[NUM_DEVICES] = {50, 100, 200, 500,};
static dev_t devices[NUM_DEVICES];

/**
 * @brief Initialize the sleep queues and match values for all devices.
 */
void dev_init(void)
{
	//puts("dev_init\n");
	int i;
	for (i = 0; i < NUM_DEVICES; i++) {
		devices[i].sleep_queue = (void *)0;
		devices[i].next_match =system_time+dev_freq[i]; 
		//printf("next match: %lu\n", devices[i].next_match);	
	}
}


/**
 * @brief Puts a task to sleep on the sleep queue until the next
 * event is signalled for the device.
 *
 * @param dev  Devisece number.
 */
void dev_wait(unsigned int dev)
{
	//puts("dev_wait\n");
	tcb_t** tmp = &(devices[dev].sleep_queue);
	while (1) {
		if (*tmp == (void *)0) {
			devices[dev].sleep_queue = runqueue_remove(get_cur_prio());
			break;
		}
		if((*tmp)->sleep_queue == (void *)0) {
			(*tmp)->sleep_queue = runqueue_remove(get_cur_prio());
			((*tmp)->sleep_queue) = (void *)0;
			break;
		}
		tmp = &((*tmp)->sleep_queue);
	}
	dispatch_sleep();	
}


/**
 * @brief Signals the occurrence of an event on all applicable devices. 
 * This function should be called on timer interrupts to determine that 
 * the interrupt corresponds to the event frequency of a device. If the 
 * interrupt corresponded to the interrupt frequency of a device, this 
 * function should ensure that the task is made ready to run 
 */
void dev_update(unsigned long millis)
{	
	//puts("dev_update\n");
	if (millis % 50 == 0) {
		printf("dev_up, systime: %lu\n", millis);
	}
	int i;
	int flag = 0;
	for (i = 0; i < NUM_DEVICES; i++) {
		if( millis == devices[i].next_match) {
			devices[i].next_match = devices[i].next_match + dev_freq[i];
		
			while (devices[i].sleep_queue != (void *)0) {
				tcb_t* old_header = devices[i].sleep_queue;
				devices[i].sleep_queue = old_header->sleep_queue;
				old_header->sleep_queue = (void *)0;
				runqueue_add(old_header, old_header->native_prio);
				flag = 1;
			}
		}
	}
	if (flag == 1) {
		dispatch_save();
	}	
}

