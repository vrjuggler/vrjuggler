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
 *   Patrick Hartling
 * --------------------------------------------------------------------------
 * NOTES:
 *    - This file (vjCondNSPR.h) must be included by vjCond.h, not the
 *      other way around.
 * --------------------------------------------------------------------------
 */

#ifndef _VJ_COND_NSPR_H_
#define _VJ_COND_NSPR_H_


#include <vjConfig.h>

#include <prcvar.h>

#include <VPR/md/NSPR/vjMutexNSPR.h>
#include <Utils/vjDebug.h>


//: Condition variable wrapper for NSPR condition variables.
//!PUBLIC_API:
class vjCondNSPR {
public:
    // -----------------------------------------------------------------------
    //: Constructor for vjCondNSPR class.
    //
    //! PRE: None.
    //! POST: The condition variable is intialized, and the mutex variable
    //+       associated with it is defined.  These two steps must be done
    //+       before any other member functions can use them.
    //
    //! ARGS: mutex - Pointer to a vjMutexNSPR variable that is used in
    //+               association with the condition variable in this class
    //+               (optional).
    // -----------------------------------------------------------------------
    vjCondNSPR (vjMutexNSPR* mutex = NULL) {
        // If the caller did not specify a mutex variable to use with
        // the condition variable, use mDefaultMutex.
        if ( mutex == NULL ) {
            mCondMutex = &mDefaultMutex;
        } else {
            mCondMutex = mutex;
        }

        // Initialize the condition variable.
        mCondVar = PR_NewCondVar(mCondMutex->mMutex);

        vjASSERT(mCondVar != NULL);
    }

    // -----------------------------------------------------------------------
    //: Destructor for vjCondNSPR class.
    //
    //! PRE: The condition variable is no longer in use.
    //! POST: The condition variable is destroyed.
    // -----------------------------------------------------------------------
    ~vjCondNSPR (void) {
        PR_DestroyCondVar(mCondVar);
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
    inline int
    wait (void) {
        PRStatus retval;

        // ASSERT:  We have been locked.
        retval = PR_WaitCondVar(mCondVar, PR_INTERVAL_NO_TIMEOUT);

        // XXX: Use error status to print a message before the assertion.
        vjASSERT(retval != PR_FAILURE);

        return retval;
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
        PRStatus retval;

        // ASSERT:  We have been locked
        retval = PR_NotifyCondVar(mCondVar);
        vjASSERT(retval != PR_FAILURE);

        return retval;
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
        PRStatus retval;

        // ASSERT:  We have been locked
        retval = PR_NotifyAllCondVar(mCondVar);
        vjASSERT(retval != PR_FAILURE);

        return retval;
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
    //! ARGS: mutex - Pointer to a vjMutexNSPR variable that is used in
    //+               association with the condition variable in this class.
    //
    //! NOTE: NEVER call except to initialize explicitly.
    // -----------------------------------------------------------------------
    inline void
    setMutex (vjMutexNSPR* mutex) {
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
        std::cerr << "------------- vjCondNSPR::Dump ---------\n"
                  << "Not Implemented yet.\n";
    }

private:
    PRCondVar*		mCondVar;	//: Condition variable
    vjMutexNSPR*	mCondMutex;	//: Mutex for the condition variable

    vjMutexNSPR		mDefaultMutex;	//: A default mutex variable

    // = Prevent assignment and initialization.
    void operator= (const vjCondNSPR&) {
        /* Do nothing.*/ ;
    }

    vjCondNSPR (const vjCondNSPR &c) {
        /* Do nothing.*/ ;
    }
};

#endif	/* _VJ_COND_NSPR_H_ */
