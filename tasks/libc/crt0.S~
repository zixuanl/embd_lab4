/** @file crt0.S
 *
 * @brief Execution startup routine
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 07 Oct 2007 01:51:29 -0400
 */

#include <asm.h>

	.file "crt0.S"

	.text

	.global	_start

_start:
	ldr	r0, [sp]	@ load argc
	add	r1, sp, #4	@ load argv
	bl 	main		@ call main routine, return value at r0
  loop:
        b loop
