/* @file main.c
 *
 * @brief kernel main
 *
 * @author 
 *	   
 *	   
 * @date   
 */
#include <exports.h>
#include <kernel.h>
#include <task.h>
#include <sched.h>
#include <device.h>
#include <assert.h>
#include <hijack.h>

uint32_t global_data;
tcb_t kernel_tcb[4]; 

int kmain(int argc __attribute__((unused)), char** argv  __attribute__((unused)), uint32_t table)
{

	app_startup();
	global_data = table;
	/* add your code up to assert statement */
	

	// hijack
	if (install_handler() != 0) {		// "Wire in" my own SWI handler
		puts("Bad Code!\n");
		while(1);
	}
			
	if (install_irqhandler() != 0) {	// wire in irq handlers
		puts("Bad Code!\n");
		while(1);
	}
	
	// user
	Usermode(argc, argv);			// Change the mode to user mode, set the user stack and jump to user program. 

		
	assert(0);        /* should never get here */
}


