/** @file lock.h
 *
 * @brief Declaration of locking and synchronization primitives.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date   2008-12-01
 *
 * Authors: Tianyi Huang <tianyih@andrew.cmu.edu>
 *          Zixuan Liu <zixuanl@andrew.cmu.edu>
 *	    Jianan Lu <jiananl@andrew.cmu.edu>
 * Date:    11/23/2013
 */

#include <types.h>
#include <task.h>

#ifndef _LOCK_H_
#define _LOCK_H_

struct mutex
{
	bool_e	bAvailable;		/* flag for availability */
	tcb_t*	pHolding_Tcb;	/* who are using this mutex */
	bool_e	bLock;			/* 1 for lock/0 for unlock */	
	tcb_t*	pSleep_queue;	/* list of applications waiting for this mutex */
};
typedef struct mutex mutex_t;

struct cond
{
	/* FILL THIS IN */
	/* is not handled in this lab */
};
typedef struct cond cond_t;



void mutex_init(void);	/* a function for initiating mutexes */
int mutex_create_syscall(void);
int mutex_lock_syscall(int mutex);
int mutex_unlock_syscall(int mutex);

#endif /* _LOCK_H_ */
