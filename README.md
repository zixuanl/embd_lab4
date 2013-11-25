/*
 * Overall review of Lab4.part1
 *
 * Authors: Tianyi Huang <tianyih@andrew.cmu.edu>
 *          Zixuan Liu <zixuanl@andrew.cmu.edu>
 *  	    Jianan Lu <jiananl@andrew.cmu.edu>
 * Date:    11/25/2013
 */

 Firstly, we ported previous code from the Gravelv kernel to the Gravelv2 kernel.
 We added the wrapper for new added system call (mutex_** Function), task_create
 and event_wait. Especially, we stored the sp and lr of each task before enter the 
 real swi. 

 Secondly, we wrote the code for new added system call and the corresponding code
 for task management. In task_create, we sorted the user-entered tasks by their device
 period(considered as priority) and then converted the user-entered tasks to tcb that are 
 used for task management.Then we allocated the tasks as well as the idle task and put 
 the tasks into run_queue and set corrsponding run_bit and run_groupbit(used as ready queue). 
 Until now, the initialization for task management is ready. After context switch, the 
 highest priority task will be running.
 
 Thirdly, we dealed with the context switch. There are two possible ways a task can be 
 preempted. One is the task program is over for this time period and called event_wait. 
 The other is higher priority task is added to the runlist. And higher priority task will 
 be running firstly.If the task is over for one period, it will be added to the device_sleep 
 queue and be removed from the run_queue with corresponding bit set clear.Then do the 
 context switch When the time matches certain device's next_match time. All the tasks 
 that are in the device's sleep_queue will be awaken. Then do the context switch. In this 
 way, higher priority task may be added into the runlist and it will preempt the current 
 running task with lower priority.
 	
 Lastly, we did the concurrency control through mutexes. In mutex_** function, we disable 
 the interrupt to make it like a atomic one. When a particular task wants to hold a mutex 
 and the mutex is free to use. The holder of the mutex is set to be this task and the mutex
 is set to be blocked. If the mutex is not free when a particular task wants to hold it,
 the program will do the context switch and add this task to the mutex sleep_queue and 
 remove it from the run_queue. When the mutex is freed by a certain task, the first waiting
 task will be woken up and mutex will be handed over to that task atomically.


 
