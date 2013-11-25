
/* 
 * hijack.h: install and resume handlers for swi and irq 
 *
 * Authors: Tianyi Huang <tianyih@andrew.cmu.edu>
 *          Zixuan Liu <zixuanl@andrew.cmu.edu>
 *	    Jianan Lu <jiananl@andrew.cmu.edu>
 * Date:    11/09/2013
 */

#ifndef HIJACK_H
#define HIJACK_H

extern int install_handler();				// "Wire in" my own SWI handler

extern int install_irqhandler();



#endif
