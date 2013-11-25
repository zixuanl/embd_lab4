/** @file run_queue.c
 * 
 * @brief Run queue maintainence routines.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-21
 */

#include <types.h>
#include <assert.h>

#include <kernel.h>
#include <sched.h>
#include "sched_i.h"

#include <exports.h>

static tcb_t* run_list[OS_MAX_TASKS]  __attribute__((unused));

/* A high bit in this bitmap means that the task whose priority is
 * equal to the bit number of the high bit is runnable.
 */
static uint8_t run_bits[OS_MAX_TASKS/8] __attribute__((unused));

/* This is a trie structure.  Tasks are grouped in groups of 8.  If any task
 * in a particular group is runnable, the corresponding group flag is set.
 * Since we can only have 64 possible tasks, a single byte can represent the
 * run bits of all 8 groups.
 */
static uint8_t group_run_bits __attribute__((unused));

/* This unmap table finds the bit position of the lowest bit in a given byte
 * Useful for doing reverse lookup.
 */
static uint8_t prio_unmap_table[]  __attribute__((unused)) =
{

0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0
};

/**
 * @brief Clears the run-queues and sets them all to empty.
 */
void runqueue_init(void)
{
	//puts("runqueue_init\n");
	// clear group_run_bits and run_bits
	group_run_bits = 0;

	int i;
	for (i = 0; i < OS_MAX_TASKS/8; i++) {
		run_bits[i] = 0;
	}
	
	// clear the run-queues
	for (i = 0; i < OS_MAX_TASKS; i++) {
		run_list[i] = (void *)0;
	}
	
}

/**
 * @brief Adds the thread identified by the given TCB to the runqueue at
 * a given priority.
 *
 * The native priority of the thread need not be the specified priority.  The
 * only requirement is that the run queue for that priority is empty.  This
 * function needs to be externally synchronized.
 */
void runqueue_add(tcb_t* tcb  __attribute__((unused)), uint8_t prio  __attribute__((unused)))
{
	//printf("runqueue_add, prio: %d\n", (int)prio);
	// set group_run_bits and run_bits
	uint8_t OSTCBY = (prio >> 3) & 0x07;
	uint8_t OSTCBX = prio & 0x07;

	group_run_bits = group_run_bits | (1 << OSTCBY);
	run_bits[OSTCBY] = run_bits[OSTCBY] | (1 << OSTCBX);
	
	// insert it to the corresponding runqueue
	//tcb_t *oldheader = run_list[prio];
	run_list[prio] = tcb;
	//tcb. = oldheader;
	/*
	printf("x: %d\n", OSTCBX);
	printf("y: %d\n", OSTCBY);
	printf("run_bits[0]: %d\n", run_bits[0]);
	printf("run_bits[7]: %d\n", run_bits[7]);
	*/
	
}


/**
 * @brief Empty the run queue of the given priority.
 *
 * @return  The tcb at enqueued at the given priority.
 *
 * This function needs to be externally synchronized.
 */
tcb_t* runqueue_remove(uint8_t prio  __attribute__((unused)))
{
	//puts("runqueue_remove\n");
	tcb_t *tcb = run_list[prio];
	run_list[prio] = (void *)0;

	uint8_t OSTCBY = (prio >> 3) & 0x07;
	uint8_t OSTCBX = prio & 0x07;
	run_bits[OSTCBY] = run_bits[OSTCBY] & (~(1 << OSTCBX));
	if (run_bits[OSTCBY] == 0) {
		group_run_bits = group_run_bits & (~(1 << OSTCBY));
	}
/*
	printf("x: %d\n", OSTCBX);
	printf("y: %d\n", OSTCBY);
	printf("run_bits[0]: %d\n", run_bits[0]);
	printf("run_bits[7]: %d\n", run_bits[7]);
*/
	return tcb; // fix this; dummy return to prevent warning messages	
}

/**
 * @brief This function examines the run bits and the run queue and returns the
 * priority of the runnable task with the highest priority (lower number).
 */
uint8_t highest_prio(void)
{
	//puts("highest_prio\n");
	uint8_t prio;
	uint8_t x,y;
	y = prio_unmap_table[group_run_bits];
	x = prio_unmap_table[run_bits[y]];
	prio = (y << 3) + x;
	return prio; // fix this; dummy return to prevent warning messages	
}
