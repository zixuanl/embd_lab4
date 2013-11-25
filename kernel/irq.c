/* 
 * irq.c: irq setup and handler
 *
 * Authors: Tianyi Huang <tianyih@andrew.cmu.edu>
 *          Zixuan Liu <zixuanl@andrew.cmu.edu>
 *	    Jianan Lu <jiananl@andrew.cmu.edu>
 * Date:    11/22/2013
 */
#include <exports.h>
#include <systime.h>
#include <irq.h>
#include <arm/interrupt.h>
#include <arm/timer.h>
#include <arm/reg.h>
#include <device.h>

void setup();
void IRQ_Handler();
void C_IRQ_Handler();


// Setup for the interrupt controller and timer
void setup() {
	// Setup the interrupt controller
	reg_write(INT_ICMR_ADDR, 0x04000000);		// only enable the interrupt of os timer0
	reg_write(INT_ICLR_ADDR, 0);			// set all interrupt to IRQ

	// Setup the timer
	reg_write(OSTMR_OSCR_ADDR, 0);			// reset the OSCR
	reg_write(OSTMR_OSMR_ADDR(0), FREQ_MS * SYSTIME_PRECISION);		// set the initial OSMR0 to 32500, so it will trigger an interrupt every 10ms

	reg_write(OSTMR_OIER_ADDR, OSTMR_OIER_E0);	// only enable the interrupt of OSMR0
	reg_write(OSTMR_OSSR_ADDR, OSTMR_OSSR_M0);	// clear the bit initially

}


void C_IRQ_Handler() {
	system_time++;
	reg_write(OSTMR_OSCR_ADDR, 0);			// reset the OSCR
	reg_set(OSTMR_OSSR_ADDR, 1);
	dev_update(system_time);
}
