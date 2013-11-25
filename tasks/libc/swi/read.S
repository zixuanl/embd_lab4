/** @file read.S
 *
 * @brief read sycall wrapper
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 07 Oct 2007 01:53:16 -0400
 * 
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-10-29
 */

#include <asm.h>
#include <bits/swi.h>
                                               
        .file   "read.S"
        .text
                        
        .global read    
read:        
        stmfd   sp!, {r4-r11}   @ as a called routine of APCS, preserve r4 - r11 (but in fact we won't use them)
	mov     r4,  sp
	mov	r5,  lr        
        swi     READ_SWI        @ use syscall read, argumens r0, r1, r2 are used
	mov	lr,  r5
	mov     sp,  r4 
        movs    r1, r0          @ check return value  
        movmi   r0, #-1         @ if r0 < 0, r0 = -1  
        ldrmi   r2, =errno      @ load the address of global variable errno                     
        rsbmi   r3, r1, #0      @ set errno = -r0     
        strmi   r3, [r2]        @ store the value back to errno                    
        ldmfd   sp!, {r4-r11}   @ reload the preserved registers
        bx      lr              @ return 	mov     r4,  sp
