/* 
 * sys.c: dispatch system calls. 
 *
 * Authors: Tianyi Huang <tianyih@andrew.cmu.edu>
 *          Zixuan Liu <zixuanl@andrew.cmu.edu>
 *	    Jianan Lu <jiananl@andrew.cmu.edu>
 * Date:    11/20/2013
 */

#include <bits/swi.h>
#include <bits/fileno.h>
#include <bits/errno.h>
#include <syscall.h>
#include <task.h>
#include <lock.h>
#include <exports.h>

int getSWI(int num, int *sp) {
	//printf("SWI, %x\n", num);
	switch(num) {
		// io
		case READ_SWI:			
			return read_syscall((int) *sp, (char *) *(sp+1), (unsigned int) *(sp+2));
		case WRITE_SWI:
			return write_syscall((int) *sp, (char *) *(sp+1), (unsigned int) *(sp+2));

		// time
		case TIME_SWI:
			return time_syscall();
		case SLEEP_SWI:
			sleep_syscall((unsigned int) *sp);
			return 0; 
		
		// sched
		case CREATE_SWI:
			return task_create_syscall((task_t*) *sp, (size_t) (*(sp+1)));
		case EVENT_WAIT:
			return event_wait_syscall((unsigned int) (*sp));

		// mutex
		case MUTEX_CREATE:
			return mutex_create_syscall();
		case MUTEX_LOCK:
			return mutex_lock_syscall((int) (*sp));
		case MUTEX_UNLOCK:
			return mutex_unlock_syscall((int) (*sp));

		// invalid syscall
		default:
			invalid_syscall((unsigned int) num);
			break; 
	}
	return 0;
}
