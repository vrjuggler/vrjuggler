/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ****************** <VPR heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/


/*
 * --------------------------------------------------------------------------
 * Author:
 *   Patrick Hartling
 * ----------------------------------------------------------------------------
 * NOTES:
 *    - This file (MutexNSPR.h) must be included by vpr/Sync/Mutex.h, not the
 *      other way around.
 * ----------------------------------------------------------------------------
 */

#ifndef _VPR_MUTEX_NSPR_H_
#define _VPR_MUTEX_NSPR_H_

#include <vpr/vprConfig.h>

#include <stdio.h>
#include <stdlib.h>
#include <prlock.h>

#include <vpr/Util/Assert.h>


namespace vpr {

//: Mutex wrapper for NSPR locks.

//!PUBLIC_API:
class VPR_CLASS_API MutexNSPR {
public:
    // -----------------------------------------------------------------------
    //: Constructor for vpr::MutexNSPR class.
    //
    //! PRE: None.
    //! POST: The mutex variable is initialized for use.  It must be
    //+       initialized before any other member functions can do anything
    //+       with it.
    // -----------------------------------------------------------------------
    MutexNSPR (void)
        : mLocked(0)
    {
        // ----- Allocate the mutex ----- //
        mMutex = PR_NewLock();
        vprASSERT(mMutex != NULL);
    }

    // -----------------------------------------------------------------------
    //: Destructor for vpr::MutexNSPR class.
    //
    //! PRE: No thread should be in a lock-specific function.
    //! POST: The mutex variable is destroyed.
    // -----------------------------------------------------------------------
    ~MutexNSPR (void) {
        PR_DestroyLock(mMutex);
    }

    // -----------------------------------------------------------------------
    //: Lock the mutex.
    //
    //! PRE: None.
    //! POST: A lock on the mutex variable is acquired by the caller.  If a
    //+       lock has already been acquired by another process/thread, the
    //+       caller blocks until the mutex has been freed.
    //
    //! RETURNS: 1 - Lock acquired
    // -----------------------------------------------------------------------
    inline int
    acquire (void) {
        PR_Lock(mMutex);
        mLocked = 1;

        return 1;
    }

    // -----------------------------------------------------------------------
    //: Acquire a read mutex lock.
    //
    //! PRE: None.
    //! POST: A lock on the mutex variable is acquired by the caller.  If a
    //+       lock has already been acquired by another process/thread, the
    //+       caller blocks until the mutex has been freed.
    //
    //! RETURNS:  1 - Lock acquired
    //
    //! NOTE: No special read mutex has been defined for now.
    // -----------------------------------------------------------------------
    inline int
    acquireRead (void) {
        return this->acquire();
    }

    // -----------------------------------------------------------------------
    //: Acquire a write mutex lock.
    //
    //! PRE: None.
    //! POST: A lock on the mutex variable is acquired by the caller.  If a
    //+       lock has already been acquired by another process/thread, the
    //+       caller blocks until the mutex has been freed.
    //
    //! RETURNS:  1 - Acquired
    //
    //! NOTE: No special write mutex has been defined for now.
    // -----------------------------------------------------------------------
    inline int
    acquireWrite (void) {
        return this->acquire();
    }

    // -----------------------------------------------------------------------
    //: Try to acquire a lock on the mutex variable (does not block).
    //
    //! PRE: None.
    //! POST: A lock on the mutex variable is acquired by the caller.  If a
    //+       lock has already been acquired by another process/thread, the
    //+       caller returns does not wait for it to be unlocked.
    //
    //! RETURNS: 1 - Acquired
    //! RETURNS: 0 - Mutex is busy
    // -----------------------------------------------------------------------
    inline int
    tryAcquire (void) {
        if ( mLocked == 0 ) {
            this->acquire();
            return 1;
        } else {
            return 0;
        }
    }

    // -----------------------------------------------------------------------
    //: Try to acquire a read mutex lock (does not block).
    //
    //! PRE: None.
    //! POST: A lock on the mutex variable is acquired by the caller.  If a
    //+       lock has already been acquired by another process/thread, the
    //+       caller returns does not wait for it to be unlocked.
    //
    //! RETURNS: 1 - Acquired
    //! RETURNS: 0 - Mutex is busy
    // -----------------------------------------------------------------------
    inline int
    tryAcquireRead (void) {
        return this->tryAcquire();
    }

    // -----------------------------------------------------------------------
    //: Try to acquire a write mutex (does not block)..
    //
    //! PRE: None.
    //! POST: A lock on the mutex variable is acquired by the caller.  If a
    //+       lock has already been acquired by another process/thread, the
    //+       caller returns does not wait for it to be unlocked.
    //
    //! RETURNS: 1 - Acquired
    //! RETURNS: 0 - Mutex is busy
    // -----------------------------------------------------------------------
    inline int
    tryAcquireWrite (void) {
        return this->tryAcquire();
    }

    // -----------------------------------------------------------------------
    //: Release the mutex.
    //
    //! PRE: The mutex variable must be locked.
    //! POST: The mutex variable is unlocked.
    //
    //! RETURNS:  0 - Succeed
    //! RETURNS: -1 - Error
    // -----------------------------------------------------------------------
    inline int
    release (void) {
        mLocked = 0;
        return PR_Unlock(mMutex);
    }

    // -----------------------------------------------------------------------
    //: Test the current lock status.
    //
    //! PRE: None.
    //! POST: The state of the mutex variable is returned.
    //
    //! RETURNS: 0 - Not locked
    //! RETURNS: 1 - Locked
    // -----------------------------------------------------------------------
    inline int
    test (void) {
        return mLocked;
    }

    // -----------------------------------------------------------------------
    //: Dump the mutex debug stuff and current state.
    //
    //! PRE: None.
    //! POST: All important data and debugging information related to the
    //+       mutex are dumped to the specified file descriptor (or to stderr
    //+       if none is given).
    //
    //! ARGS: dest - File descriptor to which the output will be written.
    //+              It defaults to stderr if no descriptor is specified.
    //! ARGS: message - Message printed out before the output is dumped.
    // -----------------------------------------------------------------------
    void
    dump (FILE* dest = stderr,
          const char* message = "\n------ Mutex Dump -----\n") const
    {
        fprintf(dest, "%s", message);
        fprintf(dest, "This is not currently implemented ...\n");
        // More needed ...
    }

    // Allow the vpr::CondNSPR class to access the private and protected
    // members of this class.  Specifically, direct access is needed to the
    // mutex variable.
    friend class CondVarNSPR;

protected:
    PRLock* mMutex;     //: Mutex variable for the class.
    int     mLocked;    //:

    // = Prevent assignment and initialization.
    void operator= (const MutexNSPR &) {
        /* Do nothing. */ ;
    }

    MutexNSPR (const MutexNSPR &) {
        /* Do nothing. */ ;
    }
};

}; // End of vpr namespace


#endif	/* ifdef _VJ_MUTEX_NSPR_H_ */
