/** @file ctx_switch.c
*
* @brief C wrappers around assembly context switch routines.
*
 * Authors: Tianyi Huang <tianyih@andrew.cmu.edu>
 *          Zixuan Liu <zixuanl@andrew.cmu.edu>
 *	    Jianan Lu <jiananl@andrew.cmu.edu>
 * Date:    11/23/2013
*/
 

#include <types.h>
#include <assert.h>

#include <config.h>
#include <kernel.h>
#include "sched_i.h"

#ifdef DEBUG_MUTEX
#include <exports.h>
#endif

static __attribute__((unused)) tcb_t* cur_tcb; /* use this if needed */

/**
* @brief Initialize the current TCB and priority.
*
* Set the initialization thread's priority to IDLE so that anything
* will preempt it when dispatching the first task.
*/
void dispatch_init(tcb_t* idle __attribute__((unused)))
{
        cur_tcb = idle;
}


/**
* @brief Context switch to the highest priority task while saving off the
* current task state.
*
* This function needs to be externally synchronized.
* We could be switching from the idle task. The priority searcher has been tuned
* to return IDLE_PRIO for a completely empty run_queue case.
*/
void dispatch_save(void)
{
	// use temp to store the current tcb and then update cur_tcb
        tcb_t* temp_tcb = cur_tcb;
        tcb_t* highest_tcb;
        highest_tcb = &system_tcb[highest_prio()];                                /*get the task of the highest priority*/
        cur_tcb = highest_tcb;                                                        /*get the current task*/

	// content switch to the highest priority task and save the current one 
        ctx_switch_full(&(highest_tcb->context), &(temp_tcb->context));                /*switch context*/
}

/**
* @brief Context switch to the highest priority task that is not this task --
* don't save the current task state.
*
* There is always an idle task to switch to.
*/
void dispatch_nosave(void)
{
        // get highest prio task and update cur_tcb
	tcb_t* highest_tcb;
        highest_tcb = &system_tcb[highest_prio()];                                /*get the task of the highest priority*/
        cur_tcb = highest_tcb;

	// content switch to the highest priority task without saving the current one
        ctx_switch_half(&(highest_tcb->context));                                /*switch context*/
}


/**
* @brief Context switch to the highest priority task that is not this task --
* and save the current task but don't mark is runnable.
*
* There is always an idle task to switch to.
*/
void dispatch_sleep(void)
{
        tcb_t* temp_tcb = cur_tcb;
        tcb_t* highest_tcb;
        highest_tcb = &system_tcb[highest_prio()];                                /*get the task of the highest priority*/
        cur_tcb = highest_tcb;                                                        /*get the current task*/

	// content switch to the highest priority task and save the current one
        ctx_switch_full(&(highest_tcb->context), &(temp_tcb->context));                /*switch context*/
}

/**
* @brief Returns the priority value of the current task.
*/
uint8_t get_cur_prio(void)
{
        return cur_tcb->native_prio;
}

/**
* @brief Returns the TCB of the current task.
*/
tcb_t* get_cur_tcb(void)
{
        return cur_tcb;
}
