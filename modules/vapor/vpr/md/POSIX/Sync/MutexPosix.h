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
 * ----------------------------------------------------------------------------
 * Author:
 *   Patrick Hartling (based on vpr::MutexSGI by Allen Bierbaum).
 * ----------------------------------------------------------------------------
 * NOTES:
 *    - This file (MutexPosix.h) must be included by vpr/Sync/Mutex.h, not the
 *      other way around.
 * ----------------------------------------------------------------------------
 */

#ifndef _VPR_MUTEX_POSIX_H_
#define _VPR_MUTEX_POSIX_H_

#include <vpr/vprConfig.h>

#include <stdio.h>
#include <pthread.h>
#include <errno.h>


namespace vpr {

//: Mutex wrapper for POSIX-compliant systems using pthreads mutex variables
//+ for the implementation.

//!PUBLIC_API:
class MutexPosix {
public:
    // -----------------------------------------------------------------------
    //: Constructor for vpr::MutexPosix class.
    //
    //! PRE: None.
    //! POST: The mutex variable is initialized for use.  It must be
    //+       initialized before any other member functions can do anything
    //+       with it.
    // -----------------------------------------------------------------------
    MutexPosix (void) {
        // Initialize the mutex.
#ifdef _PTHREADS_DRAFT_4
        pthread_mutex_init(&mMutex, pthread_mutexattr_default);
#else
        pthread_mutex_init(&mMutex, NULL);
#endif
    }

    // -----------------------------------------------------------------------
    //: Destructor for vpr::MutexPosix class.
    //
    //! PRE: The mutex variable should be unlocked before being destroyed,
    //+      but if it is not, this routine will unlock it and then destroy
    //+      it.
    //! POST: The mutex variable is destroyed and unlocked if necessary.
    // -----------------------------------------------------------------------
    ~MutexPosix (void) {
        // Destroy the mutex.
        if ( pthread_mutex_destroy(&mMutex) == -1 ) {
            pthread_mutex_unlock(&mMutex);
            pthread_mutex_destroy(&mMutex);
        }
    }

    // -----------------------------------------------------------------------
    //: Lock the mutex.
    //
    //! PRE: None.
    //! POST: A lock on the mutex variable is acquired by the caller.  If a
    //+       lock has already been acquired by another process/thread, the
    //+       caller blocks until the mutex has been freed.
    //
    //! RETURNS:  1 - Lock acquired
    //! RETURNS: -1 - Error
    // -----------------------------------------------------------------------
    inline int
    acquire (void) {
        int retval;

        retval = pthread_mutex_lock(&mMutex);

        // Locking succeeded.
        if ( retval == 0 ) {
            return 1;
        }
#ifdef _DEBUG
        // This thread tried to lock the mutex twice and a deadlock condition
        // was reported.
        else if ( retval == EDEADLK ) {
            perror("Tried to lock mutex twice (MutexPosix.h:110)");

            return -1;
        }
#endif
        // Some other error occurred.
        else {
            return -1;
        }
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
    //! RETURNS: -1 - Error
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
    //! RETURNS: -1 - Error
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
        if ( pthread_mutex_trylock(&mMutex) == 0 ) {
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
    //! RETURNS:  0 - Success
    //! RETURNS: -1 - Error
    // -----------------------------------------------------------------------
    inline int
    release (void) {
        return pthread_mutex_unlock(&mMutex);
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
    int test(void);

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

    // Allow the vpr::CondPosix class to access the private and protected
    // members of this class.  Specifically, direct access is needed to the
    // mutex variable.
    friend class CondVarPosix;

protected:
    pthread_mutex_t mMutex;    //: Mutex variable for the class.

    // = Prevent assignment and initialization.
    void operator= (const MutexPosix &) {;}
    MutexPosix (const MutexPosix &) {;}
};

}; // End of vpr namespace


#endif  /* ifdef _VPR_MUTEX_POSIX_H_ */
