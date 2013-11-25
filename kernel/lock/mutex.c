/**
 * @file mutex.c
 *
 * @brief Implements mutices.
 *
 * Authors: Tianyi Huang <tianyih@andrew.cmu.edu>
 *          Zixuan Liu <zixuanl@andrew.cmu.edu>
 *	    Jianan Lu <jiananl@andrew.cmu.edu>
 * Date:    11/23/2013
 * 
 * @date  
 */

//#define DEBUG_MUTEX

#include <lock.h>
#include <task.h>
#include <sched.h>
#include <bits/errno.h>
#include <arm/psr.h>
#include <arm/exception.h>
//#ifdef DEBUG_MUTEX
#include <exports.h> // temp
//#endif

mutex_t gtMutex[OS_NUM_MUTEX];


// kernel init mutex
void mutex_init()
{
	// init mutex to be available for users
	int index;
	for (index = 0; index < OS_NUM_MUTEX; index++) {
		gtMutex[index].bAvailable = 1;
		gtMutex[index].pHolding_Tcb = (void *)0;
		gtMutex[index].bLock = 0;
		gtMutex[index].pSleep_queue = (void *)0;
	}
}

// create mutex
int mutex_create_syscall(void)
{
	int index;
	for (index = 0; index < OS_NUM_MUTEX; index++) {

		// if the kernel has already set up the mutex
		if (gtMutex[index].bAvailable == 1) {

			// grab it 
			gtMutex[index].bAvailable = 0;

			// return the mutex id
			return index;
		}
	}

	// reach here if there is no mutex available
	return -ENOMEM;
}

// lock mutex
int mutex_lock_syscall(int mutex  __attribute__((unused)))
{
	// in order to make the lock operation atomic, disable interrupts, no preemption during mutex_lock
	disable_interrupts();

	// if mutex invalid
	if (mutex >= OS_NUM_MUTEX || gtMutex[mutex].bAvailable == 1) {
		enable_interrupts();		
		return EINVAL;
	}
	
	// if current task is already holding the mutex
	if (gtMutex[mutex].pHolding_Tcb == get_cur_tcb()) {
		enable_interrupts();
		return EDEADLOCK;
	}
	
	// if blocked, add the current task to sleep queue and also remove it from runqueue
	if (gtMutex[mutex].bLock == 1) {

		// use tmp to traverse the sleep list
		tcb_t** tmp = &(gtMutex[mutex].pSleep_queue);
		while (1) {
	
			// if the sleep list is empty 
			if (*tmp == (void *)0) {
				gtMutex[mutex].pSleep_queue = runqueue_remove(get_cur_prio());
				break;
			}

			// reach the end of the list
			if((*tmp)->sleep_queue == (void *)0) {
				(*tmp)->sleep_queue = runqueue_remove(get_cur_prio());
				((*tmp)->sleep_queue)->sleep_queue = (void *)0;
				break;
			}

			tmp = (tcb_t **)&((*tmp)->sleep_queue);
		}
	
		//test
		/*
		printf("blocked!!!!!!!!!\n");
		printf("mutex holding tcb: %d\n", (gtMutex[mutex].pHolding_Tcb)->native_prio);
		printf("mutex sleep queue\n");
		
		tcb_t* tmp2 = gtMutex[mutex].pSleep_queue;
		while (tmp2 != (void *)0) {
			printf("sleep %d\n", tmp2->native_prio);
			tmp2 = tmp2->sleep_queue;	
		}
		*/

		// before content switch, enable interrupts
		enable_interrupts();

		// if blocked, switch to other tasks
		dispatch_save();

		// after return, becoming atomic again
		disable_interrupts();
	}

	// if not blocked, current task holds this mutex		
	gtMutex[mutex].bLock = 1;
	gtMutex[mutex].pHolding_Tcb = get_cur_tcb();
	
	// upon leaving, enable interrupts
	enable_interrupts();

	// successful lock, return 0
	return 0;

}


// unlock mutex
int mutex_unlock_syscall(int mutex  __attribute__((unused)))
{
	// in order to make the lock operation atomic, disable interrupts, no preemption during mutex_lock
	disable_interrupts();

	// if mutex invalid
	if (mutex >= OS_NUM_MUTEX || gtMutex[mutex].bAvailable == 1) {
		enable_interrupts();
		return EINVAL;
	}

	// if the current task does not hold the mutex
	if (gtMutex[mutex].pHolding_Tcb != get_cur_tcb()) {
		enable_interrupts();
		return EPERM;
	}
	
	// first tcb in pSleep_queue
	tcb_t* nextTcb = gtMutex[mutex].pSleep_queue;

	// if there are no other tcbs blocked
	if (nextTcb == (void *)0) {
		gtMutex[mutex].bLock = 0;
		gtMutex[mutex].pHolding_Tcb = (void *)0;
	}
	else {
		// if there are other tcbs blocked, update sleep queue
		gtMutex[mutex].pSleep_queue = (gtMutex[mutex].pSleep_queue)->sleep_queue;
		nextTcb->sleep_queue = (void *)0;
		gtMutex[mutex].pHolding_Tcb = nextTcb;
		runqueue_add(nextTcb, nextTcb->native_prio);

	}

	// upon leaving, enable interrupts
	enable_interrupts();

	// success unlock
	return 0;
}

