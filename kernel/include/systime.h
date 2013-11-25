// systime.h: declare system time
//
// Authors: Tianyi Huang <tianyih@andrew.cmu.edu>
//          Zixuan Liu <zixuanl@andrew.cmu.edu>
//	    Jianan Lu <jiananl@andrew.cmu.edu>
// Date:    10/23/2013

#ifndef SYSTIME_H
#define SYSTIME_H

#define SYSTIME_PRECISION 10
#define FREQ_MS 3250
// system time
extern volatile unsigned int system_time;		// maintains a global variable as system time

#endif
