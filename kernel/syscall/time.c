/** @file time.c: timer system calls.
 *
 * @brief Kernel timer based syscall implementations
 *
 * Authors: Tianyi Huang <tianyih@andrew.cmu.edu>
 *          Zixuan Liu <zixuanl@andrew.cmu.edu>
 *	    Jianan Lu <jiananl@andrew.cmu.edu>
 * Date:    11/20/2013
 */

#include <types.h>
#include <config.h>
#include <bits/errno.h>
#include <arm/timer.h>
#include <syscall.h>
#include <systime.h>
#include <exports.h>

// returns the current system time
unsigned long time_syscall(void)
{
 	return system_time;
}



/** @brief Waits in a tight loop for atleast the given number of milliseconds.
 *
 * @param msec  The number of milliseconds to sleep.
 *
 * 
 */
void sleep_syscall(unsigned long msec  __attribute__((unused)))
{
	//printf("in sleep\n");
	if (msec == 0)
		return;

	unsigned int start_time = system_time;
	unsigned int target = (msec + SYSTIME_PRECISION * start_time)/SYSTIME_PRECISION;

	while(target > system_time) {
		
	}
}
