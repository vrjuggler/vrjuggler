/*
 *  File:          $RCSfile$
 *  Date modified: $Date$
 *  Version:       $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                             Patrick Hartling
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                  Copyright (C) - 1997, 1998, 1999, 2000
 *              Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#include <vjConfig.h>

#include <errno.h>

#include <Sync/vjMutexPosix.h>
#include <Kernel/vjDebug.h>


// ---------------------------------------------------------------------------
// Lock the mutex.
//
// PRE: None.
// POST: A lock on the mutex variable is acquired by the caller.  If a lock
//       has already been acquired by another process/thread, the caller
//       blocks until the mutex has been freed.
//
// RETURNS:  1 - Lock acquired
// RETURNS: -1 - Error
// ---------------------------------------------------------------------------
int
vjMutexPosix::acquire (void) const {
    int retval;

    retval = pthread_mutex_lock(mMutex) ;

    if ( retval == 0 ) {
        return 1;
    }
#ifdef _DEBUG
    else if ( retval == EDEADLK ) {
        vjDEBUG(vjDBG_ALL, vjDBG_CRITICAL_LVL)
            << "Tried to lock mutex twice (vjMutexPosix.cpp:52)!\n"
            << vjDEBUG_FLUSH;

        return -1;
    }
#endif
    else {
        return -1;
    }
}

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

    // If the return value from pthread_mutex_trylock() is 0, then this
    // process now has a lock on mutex.  Therefore, no other process could
    // have held a lock on it, so unlock the mutex and return 0.
    if ( ret_val == 0 ) {
        pthread_mutex_unlock(mMutex);
        ret_val = 0;
    }
    // The mutex is currently locked by some thread if ret_val is non-zero.
    else {
        ret_val = 1;
    }

    return ret_val;
}
