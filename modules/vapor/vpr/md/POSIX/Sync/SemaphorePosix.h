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
 *    - This file (SemaphorePosix.h) must be included by vpr/Sync/Semaphore.h,
 *      not the other way around.
 * ----------------------------------------------------------------------------
 */

#ifndef _VPR_SEMAPHORE_POSIX_H_
#define _VPR_SEMAPHORE_POSIX_H_

#include <vpr/vprConfig.h>

#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <semaphore.h>
#include <errno.h>
#include <stdio.h>


namespace vpr {

/**
 * Semaphore wrapper for POSIX.4-compliant systems.
 */
class SemaphorePosix {
public:
    /**
     * Constructor for vpr::SemaphorePosix class.
     *
     * @pre None.
     * @post The semaphore variable for the class is initilized as an
     *       unnamed semaphore.
     *
     * @param initialValue The initial number of resources controlled by
     *                     the semaphore.  If not specified, the default
     *                     value is 1.
     */
    SemaphorePosix (int initialValue = 1) {
        // ----- Allocate the unnamed semaphore ----- //
        mSema = (sem_t*) malloc(sizeof(sem_t));

        if ( sem_init(mSema, 0, initialValue) != 0 ) {
            perror("sem_init() error");
        }
    }

    /**
     * Destructor for vpr::SemaphorePosix class.
     *
     * @pre None.
     * @post The resources used by the semaphore variable are freed.
     */
    ~SemaphorePosix (void) {
        // ---- Delete the semaphore --- //
        if ( sem_destroy(mSema) != 0 ) {
            perror("sem_destroy() error");
        }

        free(mSema);
    }

    /**
     * Locks this semaphore.
     *
     * @pre None.
     * @post The calling thread either acquires the semaphore until
     *       release() is called, or the caller is put at the tail of a wait
     *       and is suspended until such time as it can be freed and allowed
     *       to acquire the semaphore itself.
     *
     * @return 1 is returned if the lock is acquired.<br>
     *         -1 is returnd if an error occurred.
     */
    inline int
    acquire (void) const {
        if ( sem_wait(mSema) == 0 ) {
            return 1;
        } else {
            perror("sem_wait() error");
            return -1;
        }
    }

    /**
     * Acquires a read lock on this semaphore.
     *
     * @pre None.
     * @post The calling thread either acquires the semaphore until
     *       release() is called, or the caller is put at the tail of a wait
     *       and is suspended until such time as it can be freed and allowed
     *       to acquire the semaphore itself.
     *
     * @return 1 is returned if the read lock is acquired.<br>
     *         -1 is returnd if an error occurred.
     *
     * @note There is no special read lock for now.
     */
    inline int
    acquireRead (void) const {
        return this->acquire();
    }

    /**
     * Acquires a write lock on this semaphore.
     *
     * @pre None.
     * @post The calling thread either acquires the semaphore until
     *       release() is called, or the caller is put at the tail of a wait
     *       and is suspended until such time as it can be freed and allowed
     *       to acquire the semaphore itself.
     *
     * @return 1 is returned if the write lock is acquired.<br>
     *         -1 is returnd if an error occurred.
     *
     * @note There is no special write lock for now.
     */
    inline int
    acquireWrite (void) const {
        return this->acquire();
    }

    /**
     * Tries to acquire the semaphore immediately (does not block).
     *
     * @pre None.
     * @post If the semaphore could be acquired by the caller, the caller
     *       gets control of the semaphore.  If the semaphore was already
     *       locked, the routine returns immediately without suspending the
     *       calling thread.
     *
     * @return 1 is returned if the lock on this semaphore is acquired.<br>
     *         0 is returned if the lock is not acquired.
     */
    inline int
    tryAcquire (void) const {
        return sem_trywait(mSema);
    }

    /**
     * Tries to acquire a read lock on this semaphore (does not block).
     *
     * @pre None.
     * @post If the semaphore could be acquired by the caller, the caller
     *       gets control of the semaphore.  If the semaphore was already
     *       locked, the routine returns immediately without suspending the
     *       calling thread.
     *
     * @return 1 is returned if the lock on this semaphore is acquired.<br>
     *         0 is returned if the lock is not acquired.
     */
    inline int
    tryAcquireRead (void) const {
        return this->tryAcquire();
    }

    /**
     * Tries to acquire a write lock on this semaphore (does not block).
     *
     * @pre None.
     * @post If the semaphore could be acquired by the caller, the caller
     *       gets control of the semaphore.  If the semaphore was already
     *       locked, the routine returns immediately without suspending the
     *       calling thread.
     *
     * @return 1 is returned if the lock on this semaphore is acquired.<br>
     *         0 is returned if the lock is not acquired.
     */
    inline int
    tryAcquireWrite (void) const {
        return this->tryAcquire();
    }

    /**
     * Releases this semaphore.
     *
     * @pre The semaphore should have been locked before being released.
     * @post The semaphore is released and the thread at the haed of the
     *       wait queue is allowed to execute again.
     *
     * @return 0 is returned if the lock on this semaphore is released.<br>
     *         -1 is returned if an error occurred.
     */
    inline int
    release (void) const {
        return sem_post(mSema);
    }

    /**
     * Resets the semaphore using the given value.
     *
     * @pre None.
     * @post The semaphore's count is set to the specified value.
     *
     * @param val The value to which the semaphore is reset.
     *
     * @return 0 is returned if this semaphore is reset with the given value
     *         successfully.<br>
     *         -1 is returned if this semaphore could not be reset.
     *
     * @note If processes are waiting on the semaphore, the results are
     *       undefined.
     */
    inline int
    reset (int val) {
        // First destroy the current semaphore.
        sem_destroy(mSema);

        // Now recreate it with the new value in val.
        return sem_init(mSema, 0, val);
    }

    /**
     * Dumps the semaphore debug stuff and current state.
     *
     * @pre None.
     * @post All important data and debugging information related to the
     *       semaphore is dumped to the specified file descriptor (or to
     *       stderr if none is given).
     *
     * @param dest    File descriptor to which the output will be written.
     *                It defaults to stderr if no descriptor is specified.
     * @param message Message printed out before the output is dumped.
     */
    void
    dump (FILE* dest = stderr,
          const char* message = "\n------ Semaphore Dump -----\n") const
    {
        int value;

        sem_getvalue(mSema, &value);

        fprintf(dest, "%s", message);
        fprintf(dest, "Current semaphore value: %d", value);
    }

protected:
    sem_t* mSema;	/**< Semaphore variable for the class. */

    // Prevent assignment and initialization.
    void operator= (const SemaphorePosix &) {;}
    SemaphorePosix (const SemaphorePosix &) {;}
};

}; // End of vpr namespace


#endif	/* _VPR_SEMAPHORE_POSIX_H_ */
