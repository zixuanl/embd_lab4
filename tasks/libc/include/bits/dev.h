/** @file dev.h
 *
 * @brief Defines simulated "devices".
 *
 * These devices each have an associated condition variable that they signal
 * with the given periodicity.  Each condition variable also has an associated
 * mutex of the same index.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-10-31
 */

#ifndef _DEV_PARAMETER
#define _DEV_PARAMETER

#define NUM_DEVICES 63

#define PERIOD_DEV0 100
#define PERIOD_DEV1 200
#define PERIOD_DEV2 500
#define PERIOD_DEV3 50


#endif // _DEV_PARAMETER

