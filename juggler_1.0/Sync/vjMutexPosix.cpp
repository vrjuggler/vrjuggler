/*
 *  File:	    $Name$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#include <vjConfig.h>

#include <Sync/vjMutexPosix.h>


// ---------------------------------------------------------------------------
// Test the current lock status.
//
// PRE: None.
// POST: The state of the mutex variable is returned.
//
// RETURNS: 0 - Not locked
// RETURNS: 1 - Locked
// ---------------------------------------------------------------------------
int
vjMutexPosix::test (void) {
    int ret_val;

    ret_val = pthread_mutex_trylock(mMutex);

    // The mutex is not currently locked if ret_val is 0.
    if ( ret_val == 0 ) {
        return 1;
    }
    // If the return value from pthread_mutex_trylock() is 1, then this
    // process now has a lock on mutex.  Therefore, no other process could
    // have held a lock on it, so unlock mutex and return 0.
    else if ( ret_val == 1 ) {
        pthread_mutex_unlock(mMutex);
        return 0;
    }

    return ret_val;
}
