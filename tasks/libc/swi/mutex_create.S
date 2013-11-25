/** @file time.S
 *
 * @brief time sycall wrapper
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-10-31
 */

#include <asm.h>
#include <bits/swi.h>
#include <errno.h>
	.file "mutex_create.S"

FUNC(mutex_create)
	stmfd   sp!, {r4-r11}   @ as a called routine of APCS, preserve r4 - r11 (but in fact we won't use them)
	mov     r4,  sp
	mov	r5,  lr          
        swi     MUTEX_CREATE	@ use syscall read, argument r0 is used   
	ldr	r6,  =errno
	str	r0,  [r6] 
	mov	lr,  r5
	mov     sp,  r4                 
        ldmfd   sp!, {r4-r11}   @ reload the preserved registers
        bx      lr              @ return
