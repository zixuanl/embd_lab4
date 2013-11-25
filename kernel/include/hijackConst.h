/* 
 * hijackConst.h: declares constants and functions used by hijack.c 
 *
 * Authors: Tianyi Huang <tianyih@andrew.cmu.edu>
 *          Zixuan Liu <zixuanl@andrew.cmu.edu>
 *	    Jianan Lu <jiananl@andrew.cmu.edu>
 * Date:    11/09/2013
 */

#ifndef HIJACKCONST_H
#define HIJACKCONST_H

// const
#define VECTORTABLE_SWI_ENTRY 0x08		// swi entry address: 0x08
#define VECTORTABLE_IRQ_ENTRY 0x18
#define PC_CURRENTADDR_OFFSET 8			// pc_addr = current_addr + 8
#define LOAD_PC_PC_4_ENCODING 0xe51ff004	// encoding of 'ldr pc, [pc, #-4]'
#define LOAD_PC_PC_PLUS_OFFSET_MASK 0xe59ff000
#define LOAD_PC_PC_MINUS_OFFSET_MASK 0xe51ff000

// variables
int *iaddr_1;					// address of the first instruction we altered
int *iaddr_2;					// address of the second instruction we altered
int instruction1;				// content of the first instruction we altered
int instruction2;				// content of the second instruction we altered
int *irq_iaddr_1;				// address of the first instruction we altered (for IRQ)
int *irq_iaddr_2;				// address of the second instruction we altered (for IRQ)
int irq_instruction1;				// content of the first instruction we altered (for IRQ)
int irq_instruction2;				// content of the second instruction we altered (for IRQ)


// declares assembly functions
void S_Handler(int);				// declares S_Handler
void IRQ_Handler();				// declares IRQ handler

#endif
