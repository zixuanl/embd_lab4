// userConst.h: declares the global variables Usermode.S uses
//
// Authors: Tianyi Huang <tianyih@andrew.cmu.edu>
//          Zixuan Liu <zixuanl@andrew.cmu.edu>
//	    Jianan Lu <jiananl@andrew.cmu.edu>
// Date:    10/23/2013

#ifndef USERCONST_H
#define USERCONST_H

#include <types.h>

/*
// mode change
extern uint32_t oldspsr;			// stores the old svc spsr
extern uint32_t oldsp;				// stores the old svc sp
extern uint32_t link;				// stores the old svc link register, which has the address of next instruction after calling user app in kernel
*/

extern uint32_t rr8;				// stores r8 before swi
// return status
extern int value;				// stores return value to S_Handler

#endif
