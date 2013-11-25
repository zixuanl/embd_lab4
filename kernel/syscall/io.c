/** @file io.c
 *
 * @brief Kernel I/O syscall implementations
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 14 Oct 2007 00:07:38 -0400
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date   2008-11-16
 */

#include <types.h>
#include <bits/errno.h>
#include <bits/fileno.h>
#include <arm/physmem.h>
#include <syscall.h>
#include <exports.h>
#include <kernel.h>
#include <stack.h>

#define EOT_CHAR 0x04
#define DEL_CHAR 0x7f
#define BUF_SIZE 10

/* Read count bytes (or less) from fd into the buffer buf. */
ssize_t read_syscall(int fd __attribute__((unused)), char *buf __attribute__((unused)), size_t count __attribute__((unused)))
{
	// Check if it is reading from the STDIO
	if (fd != STDIN_FILENO) {                                                
                return -EBADF; 
        }
	
	// Check if the range of the buffer exits out side the range of writable memory (SDRAM)
	if (buf < (char *)LEGEL_MEM_START_ADDR || (buf + count) > (char *)LEGEL_MEM_END_ADDR)
		return -EFAULT;

	// Check if the range of the buffer exits in some writable memory that is not writable  by a user application, namely u-boot code, heap, u-boot global data struct, abort stack and supervisor stack
	if ((buf + count) > (char *)KERNEL_START_ADDR)
		return -EFAULT;

	// Read from STDIN and store the input into buf. 
	uint32_t i;
	for (i = 0; i < count; i++) {
		while(!tstc())
			continue;
			
		buf[i] = getc();
		
		// Check if the user enters an EOT
		if(buf[i] == EOT_CHAR)
	        {
			return i;
	        }

		// Check if the user enters a backspace
		else if(buf[i] == DEL_CHAR)
		{
			// Check if the backspace is the first character
			if(i != 0)
			{
				i -= 2;
				puts("\b \b");
			}
			else
				i--;
		}
		// Check if the user enters a newline or carriage return
		else
		{
			if((buf[i] == '\n') || (buf[i] == '\r'))
			{
				putc('\n'); 
				return ++i;
			}
			putc(buf[i]);	
		}				
	}// end input from STDIN
	// If the buffer is full and the last element is not a '\n' or '\r', then print a new line in the end
	if((i == BUF_SIZE) && (buf[count-1] != '\n') && (buf[count-1] != '\r'))
		putc('\n');
	return i;
	
}

/* Write count bytes to fd from the buffer buf. */
ssize_t write_syscall(int fd  __attribute__((unused)), const char *buf  __attribute__((unused)), size_t count  __attribute__((unused)))
{

  // Check if it is writing to the STDOUT
	if (fd != STDOUT_FILENO) {
                return -EBADF;  
	}

	// Check if the range of the buffer exits out side the range of readable memory (StrataFlash ROM or SDRAM)
	if (buf < (char *)LEGEL_MEM_START_ADDR || (buf + count) > (char *)LEGEL_MEM_END_ADDR)
		return -EFAULT;
	
	// Check if the range of the buffer exits in some readable memory that is not readable by a user application, namely abort stack and supervisor stack
	if ((buf < (char *)ABORT_STACK_END_ADDR && (buf + count) > (char *)ABORT_STACK_END_ADDR) || (buf < (char *)KERNEL_START_ADDR && (buf+count) > (char *)KERNEL_START_ADDR))
		return -EFAULT;

	uint32_t i;
	// Check if the last character is a carriage return; if so, print an newline instead in the last
	if(buf[count-1]=='\r')
	{
		for(i = 0; i < (count-1); i++)
			putc(buf[i]);
		printf("\n");
	}
	// Writing to the STDOUT from buf
	else
	{
		for(i = 0; i < count; i++)
			putc(buf[i]);
	}
	return count;
	
}

