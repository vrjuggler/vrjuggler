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

/**
 * Mutex wrapper for POSIX-compliant systems using pthreads mutex variables
 * for the implementation.
 *
 * @author Patrick Hartling
 */
class MutexPosix {
public:
    /**
     * Constructor for vpr::MutexPosix class.
     *
     * @pre None.
     * @post The mutex variable is initialized for use.  It must be
     *       initialized before any other member functions can do anything
     *       with it.
     */
    MutexPosix (void) {
        // Initialize the mutex.
#ifdef _PTHREADS_DRAFT_4
        pthread_mutex_init(&mMutex, pthread_mutexattr_default);
#else
        pthread_mutex_init(&mMutex, NULL);
#endif
    }

    /**
     * Destructor for vpr::MutexPosix class.
     *
     * @pre The mutex variable should be unlocked before being destroyed,
     *      but if it is not, this routine will unlock it and then destroy
     *      it.
     * @post The mutex variable is destroyed and unlocked if necessary.
     */
    ~MutexPosix (void) {
        // Destroy the mutex.
        if ( pthread_mutex_destroy(&mMutex) == -1 ) {
            pthread_mutex_unlock(&mMutex);
            pthread_mutex_destroy(&mMutex);
        }
    }

    /**
     * Locks this mutex.
     *
     * @pre None.
     * @post A lock on the mutex variable is acquired by the caller.  If a
     *       lock has already been acquired by another process/thread, the
     *       caller blocks until the mutex has been freed.
     *
     * @return 1 is returned if the lock is acquired.<br>
     *         -1 is returned if an error occurred.
     */
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

    /**
     * Acquires a read lock on this mutex.
     *
     * @pre None.
     * @post A lock on the mutex variable is acquired by the caller.  If a
     *       lock has already been acquired by another process/thread, the
     *       caller blocks until the mutex has been freed.
     *
     * @return 1 is returned if the lock is acquired.<br>
     *         -1 is returned if an error occurred.
     *
     * @note No special read lock has been defined for now.
     */
    inline int
    acquireRead (void) {
        return this->acquire();
    }

    /**
     * Acquires a write lock on this mutex.
     *
     * @pre None.
     * @post A lock on the mutex variable is acquired by the caller.  If a
     *       lock has already been acquired by another process/thread, the
     *       caller blocks until the mutex has been freed.
     *
     * @return 1 is returned if the lock is acquired.<br>
     *         -1 is returned if an error occurred.
     *
     * @note No special write lock has been defined for now.
     */
    inline int
    acquireWrite (void) {
        return this->acquire();
    }

    /**
     * Tries to acquire a lock on this mutex variable (does not block).
     *
     * @pre None.
     * @post A lock on the mutex variable is acquired by the caller.  If a
     *       lock has already been acquired by another process/thread, the
     *       caller returns does not wait for it to be unlocked.
     *
     * @return 1 is returned if the lock is acquired.<br>
     *         0 is returned if the mutex is already locked.
     */
    inline int
    tryAcquire (void) {
        if ( pthread_mutex_trylock(&mMutex) == 0 ) {
            return 1;
        } else {
            return 0;
        }
    }

    /**
     * Tries to acquire a read lock on this mutex (does not block).
     *
     * @pre None.
     * @post A lock on the mutex variable is acquired by the caller.  If a
     *       lock has already been acquired by another process/thread, the
     *       caller returns does not wait for it to be unlocked.
     *
     * @return 1 is returned if the read lock is acquired.<br>
     *         0 is returned if the mutex is already locked.
     */
    inline int
    tryAcquireRead (void) {
        return this->tryAcquire();
    }

    /**
     * Tries to acquire a write lock on this mutex (does not block).
     *
     * @pre None.
     * @post A lock on the mutex variable is acquired by the caller.  If a
     *       lock has already been acquired by another process/thread, the
     *       caller returns does not wait for it to be unlocked.
     *
     * @return 1 is returned if the write lock is acquired.<br>
     *         0 is returned if the mutex is already locked.
     */
    inline int
    tryAcquireWrite (void) {
        return this->tryAcquire();
    }

    /**
     * Releases this mutex.
     *
     * @pre The mutex variable must be locked.
     * @post The mutex variable is unlocked.
     *
     * @return 0 is returned if the lock is released successfully.<br>
     *         -1 is returned otherwise.
     */
    inline int
    release (void) {
        return pthread_mutex_unlock(&mMutex);
    }

    /**
     * Tests the current lock status.
     *
     * @pre None.
     * @post The state of the mutex variable is returned.
     *
     * @return 0 is returned if this mutex is not currently locked.<br>
     *         0 is returned if this mutex is locked.
     */
    int test(void);

    /**
     * Dumps the mutex debug stuff and current state.
     *
     * @pre None.
     * @post All important data and debugging information related to the
     *       mutex are dumped to the specified file descriptor (or to stderr
     *       if none is given).
     *
     * @param dest    File descriptor to which the output will be written.
     *                It defaults to stderr if no descriptor is specified.
     * @param message Message printed out before the output is dumped.
     */
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
    pthread_mutex_t mMutex;    /**< Mutex variable for the class. */

    // = Prevent assignment and initialization.
    void operator= (const MutexPosix &) {;}
    MutexPosix (const MutexPosix &) {;}
};

}; // End of vpr namespace


#endif  /* ifdef _VPR_MUTEX_POSIX_H_ */
