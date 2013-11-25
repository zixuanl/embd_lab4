/** @file time.c
 *
 * @brief Kernel timer based syscall implementations
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date   2008-11-16
 */

#include <types.h>
#include <config.h>
#include <bits/errno.h>
#include <arm/timer.h>
#include <syscall.h>
#include <systime.h>

unsigned long time_syscall(void)
{
 	return system_time;
}



/** @brief Waits in a tight loop for atleast the given number of milliseconds.
 *
 * @param millis  The number of milliseconds to sleep.
 *
 * 
 */
void sleep_syscall(unsigned long msec  __attribute__((unused)))
{
	if (msec == 0)
		return;

	unsigned int start_time = system_time;
	unsigned int target = (msec + SYSTIME_PRECISION * start_time)/SYSTIME_PRECISION;

	while(target > system_time) {
		
	}
}
