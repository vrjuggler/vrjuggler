/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/


/*
 * --------------------------------------------------------------------------
 * Author:
 *   Patrick Hartling (based on vjCondSGI by Allen Bierbaum).
 * --------------------------------------------------------------------------
 * NOTES:
 *    - This file (vjCondPosix.h) must be included by vjCond.h, not the
 *      other way around.
 * --------------------------------------------------------------------------
 */

#ifndef _VJ_COND_POSIX_H_
#define _VJ_COND_POSIX_H_


#include <vjConfig.h>
#include <pthread.h>
#include <Sync/vjMutexPosix.h>


//: Condition variable wrapper for POSIX-compliant systems using pthreads
//+ condition variables for the implementation.
//!PUBLIC_API:
class vjCondPosix {
public:
    // -----------------------------------------------------------------------
    //: Constructor for vjCondPosix class.
    //
    //! PRE: None.
    //! POST: The condition variable is intialized, and the mutex variable
    //+       associated with it is defined.  These two steps must be done
    //+       before any other member functions can use them.
    //
    //! ARGS: mutex - Pointer to a vjMutexPosix variable that is used in
    //+               association with the condition variable in this class
    //+               (optional).
    // -----------------------------------------------------------------------
    vjCondPosix (vjMutexPosix* mutex = NULL) {
        // Initialize the condition variable.
#ifdef _PTHREADS_DRAFT_4
        pthread_cond_init(&mCondVar, pthread_condattr_default);
#else
        pthread_cond_init(&mCondVar, NULL);
#endif

        // If the caller did not specify a mutex variable to use with
        // the condition variable, use mDefaultMutex.
        if ( mutex == NULL ) {
            mutex = &mDefaultMutex;
        }

        mCondMutex = mutex;
    }

    // -----------------------------------------------------------------------
    //: Destructor for vjCondPosix class.
    //
    //! PRE: None.
    //! POST: The condition variable is destroyed.
    // -----------------------------------------------------------------------
    ~vjCondPosix (void) {
        pthread_cond_destroy(&mCondVar);
    }

    // -----------------------------------------------------------------------
    //: Block on a condition.
    //
    //! PRE: The mutex variable associated with the condition variable must
    //+      be locked.
    //! POST: The condition variable is locked.  If it was previously
    //+       locked, the caller blocks until signaled.
    //
    //! RETURNS:  0 - Successful completion
    //! RETURNS: -1 - Error
    // -----------------------------------------------------------------------
    int
    wait (void) {
        // ASSERT:  We have been locked

        // If not locked ...
        if ( mCondMutex->test() == 0 ) {
            std::cerr << "vjCondPosix::wait: INCORRECT USAGE: Mutex was not "
                      << "locked when wait invoked!!!\n";

            return -1;
        }

        // The mutex variable must be locked when passed to
        // pthread_cond_wait().
        return pthread_cond_wait(&mCondVar, &(mCondMutex->mMutex));
    }

    // -----------------------------------------------------------------------
    //: Signal a thread waiting on the condition variable.
    //
    //! PRE: The condition variable must be locked.
    //! POST: The condition variable is unlocked, and a signal is sent to a
    //+       thread waiting on it.
    //
    //! RETURNS:  0 - Successful completion
    //! RETURNS: -1 - Error
    // -----------------------------------------------------------------------
    inline int
    signal (void) {
        // ASSERT:  We have been locked
        return pthread_cond_signal(&mCondVar);
    }

    // -----------------------------------------------------------------------
    //: Signal all waiting threads.
    //
    //! PRE: The mutex variable associated with the condition variable
    //+      should be locked.
    //! POST: The condition variable is unlocked, and all waiting threads
    //+       are signaled of this event.
    //
    //! RETURNS:  0 - Successful completion
    //! RETURNS: -1 - Error
    // -----------------------------------------------------------------------
    inline int
    broadcast (void) {
        // ASSERT:  We have been locked

        // If not locked ...
        if ( mCondMutex->test() == 0 ) {
            std::cerr << "vjCondPosix::broadcast: Mutex was not locked when "
                      << "broadcast called!!!\n";
        }

        return pthread_cond_broadcast(&mCondVar);
    }

    // -----------------------------------------------------------------------
    //: Acquire a lock on the mutex variable associated with the condition
    //+ variable.
    //
    //! PRE: None.
    //! POST: A lock is acquired on the mutex variable associated with the
    //+      condition variable.  If a lock is acquired, the caller controls
    //+      the mutex variable.  If it was previously locked, the caller
    //+      blocks until it is unlocked.
    //
    //! RETURNS:  0 - Successful completion
    //! RETURNS: -1 - Error
    // -----------------------------------------------------------------------
    inline int
    acquire (void) {
        return mCondMutex->acquire();
    }

    // -----------------------------------------------------------------------
    //: Try to acquire a lock on the mutex variable associated with the
    //+ condition variable.
    //
    //! PRE: None.
    //! POST: If the mutex variable is not already locked, the caller
    //+       obtains a lock on it.  If it is already locked, the routine
    //+       returns immediately to the caller.
    //
    //! RETURNS:  0 - Successful completion
    //! RETURNS: -1 - Error
    // -----------------------------------------------------------------------
    inline int
    tryAcquire (void) {
        return mCondMutex->tryAcquire();
    }

    // -----------------------------------------------------------------------
    //: Release the lock on the mutex variable associated with the condition
    //+ variable.
    //
    //! PRE: None.
    //! POST: The lock held by the caller on the mutex variable is released.
    // -----------------------------------------------------------------------
    inline int
    release (void) {
        return mCondMutex->release();
    }

    // -----------------------------------------------------------------------
    //: Change the condition variable mutex to be the specifiec mutex
    //+ variable.
    //
    //! PRE: The specified mutex variable must be initialized.
    //! POST: The condition variable associated with the mutex variable is
    //+       reset to the specified variable.
    //
    //! ARGS: mutex - Pointer to a vjMutexPosix variable that is used in
    //+               association with the condition variable in this class.
    //
    //! NOTE: NEVER call except to initialize explicitly.
    // -----------------------------------------------------------------------
    inline void
    setMutex (vjMutexPosix* mutex) {
        // NOT exactly correct, but just make sure not to leave it locked
        mutex->release();
        mCondMutex = mutex;
    }

    // -----------------------------------------------------------------------
    //: Print out information about the condition variable to stderr.
    //
    //! PRE: None.
    //! POST: All important data and debugging information related to the
    //+       condition variable and its mutex are dumped to stderr.
    // -----------------------------------------------------------------------
    void
    dump (void) const {
        std::cerr << "------------- vjCondPosix::Dump ---------\n"
                  << "Not Implemented yet.\n";
    }

private:
    pthread_cond_t	mCondVar;	//: Condition variable
    vjMutexPosix*	mCondMutex;	//: Mutex for the condition variable
    vjMutexPosix	mDefaultMutex;	//: A default mutex variable

    // = Prevent assignment and initialization.
    void operator= (const vjCondPosix&) {}
    vjCondPosix (const vjCondPosix &c) {}
};

#endif	/* _VJ_COND_POSIX_H_ */
