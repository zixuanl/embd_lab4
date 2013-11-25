/** @file syscall.h
 *
 * @brief Declares the kernel supported syscalls
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sat, 13 Oct 2007 23:54:45 -0400
 *
 * Authors: Tianyi Huang <tianyih@andrew.cmu.edu>
 *          Zixuan Liu <zixuanl@andrew.cmu.edu>
 *	    Jianan Lu <jiananl@andrew.cmu.edu>
 * Date:    11/23/2013
 *
 */

#ifndef SYSCALL_H
#define SYSCALL_H

#include <types.h>
#include <task.h>

int getSWI(int num, int *sp);

ssize_t read_syscall(int fd, char *buf, size_t count);
ssize_t write_syscall(int fd, const char *buf, size_t count);

unsigned long time_syscall(void);
void sleep_syscall(unsigned long millis);

void invalid_syscall(unsigned int num) __attribute__((noreturn));

int task_create_syscall(task_t* tasks, size_t num_tasks);
int event_wait_syscall(unsigned int dev);

#endif /* SYSCALL_H */
