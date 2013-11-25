/** @file kernel.h
 *
 * @brief Main kernel -- primary interface.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date   2008-11-17
 */

#ifndef KERNEL_H
#define KERNEL_H

#include <types.h>

extern uint32_t global_data;
int enter_user_mode(void);
int valid_addr(const void* start, size_t count, uintptr_t base, uintptr_t bound);


// Assembly function declaration
void Usermode(int argc, char *argv[]);		// declares Usermode which prepares for and runs user app


#endif /* KERNEL_H */
