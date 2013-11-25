/** @file time.S
 *
 * @brief time sycall wrapper
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-10-31
 */

#include <asm.h>
#include <bits/swi.h>

	.file "time.S"

FUNC(time)
	stmfd   sp!, {r4-r11}   @ as a called routine of APCS, preserve r4 - r11 (but in fact we won't use them)
	mov     r4,  sp
	mov	r5,  lr         
        swi     TIME_SWI        @ use syscall read, no arguments is used 
	mov	lr,  r5
	mov     sp,  r4                    
        ldmfd   sp!, {r4-r11}   @ reload the preserved registers
        bx      lr              @ return 
