/** @file lock.h
 *
 * @brief Declaration of locking and synchronization primitives.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date   2008-12-01
 *
 * @author Junsung Kim <junsungk@cmu.edu>
 *	   Veeren Mandalia <vmandali@andrew.cmu.edu>
 *	   Vikram Gupta <vikramg@andrew.cmu.edu>
 * @date   Sat, 06 Dec 2008 05:15:00 -0400
 */

#ifndef _LOCK_H
#define _LOCK_H
int mutex_create(void);
int mutex_lock(int mutex);
int mutex_unlock(int mutex);
#endif // _LOCK_H
