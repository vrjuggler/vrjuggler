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


#ifndef _VPR_RW_MUTEX_NSPR_H_
#define _VPR_RW_MUTEX_NSPR_H_

#include <vpr/vprConfig.h>

#include <stdio.h>
#include <prrwlock.h>


namespace vpr {

//!PUBLIC_API:
class VPR_CLASS_API RWMutexNSPR
{
public:
    RWMutexNSPR (void) : m_rw_lock(NULL) {
        m_rw_lock = PR_NewRWLock(1, "/var/tmp/test");
    }

    ~RWMutexNSPR (void) {
        PR_DestroyRWLock(m_rw_lock);
    }

    //---------------------------------------------------------
    //: Lock the mutex.
    //
    //! RETURNS:  1 - Acquired
    //! RETURNS: -1 - Error
    //---------------------------------------------------------
    inline int
    acquire(void) {
        return acquireWrite();
    }

    //----------------------------------------------------------
    //: Acquire a read mutex.
    //----------------------------------------------------------
    int
    acquireRead (void) {
        PR_RWLock_Rlock(m_rw_lock);
        return 1;
    }

    //----------------------------------------------------------
    //: Acquire a write mutex.
    //----------------------------------------------------------
    inline int
    acquireWrite (void) {
        PR_RWLock_Wlock(m_rw_lock);
        return 1;
    }

    //---------------------------------------------------------
    //: Trys to acquire the mutex.
    //  Wait until the semaphore value is greater than 0.
    //  Then decrement by 1 and return.
    //  P operation.
    //
    //! RETURNS: 1 - Acquired
    //! RETURNS: 0 - Not acquired
    //---------------------------------------------------------
    inline int
    tryAcquire (void) {
        return tryAcquireWrite();
    }

    //----------------------------------------------------------
    //: Try to acquire a read mutex.
    //----------------------------------------------------------
    int tryAcquireRead (void);

    //----------------------------------------------------------
    //: Try to acquire a write mutex.
    //----------------------------------------------------------
    int tryAcquireWrite (void);

    //---------------------------------------------------------
    //: Release the mutex.
    //
    //! RETURNS:  0 - Success
    //! RETURNS: -1 - Error
    //---------------------------------------------------------
    inline int
    release (void) {
        PR_RWLock_Unlock(m_rw_lock);
        return 0;
    }

    //------------------------------------------------------
    //:	Test the current lock status.
    //
    //! RETURNS: 0 - Not locked
    //! RETURNS: 1 - Locked
    //------------------------------------------------------
/*
    int test()
    {
        return stateLock.test();
    }
*/

    //---------------------------------------------------------
    //: Dump the mutex debug stuff and current state.
    //---------------------------------------------------------
    void dump (FILE* dest = stderr,
               const char* message = "\n------ Mutex Dump -----\n") const
    {
//        stateLock.dump();
    }

protected:
    int numWaitingReaders;    //: Number of waiting readers.
    int numWaitingWriters;    //: Number of waiting writers.

    // Value is -1 if writer has the lock, else this keeps track of the
    // number of readers holding the lock.
    int refCount;

    PRRWLock* m_rw_lock;

    // = Prevent assignment and initialization.
    void operator= (const RWMutexNSPR &) {
        /* Do nothing. */ ;
    }

    RWMutexNSPR (const RWMutexNSPR &) {
        /* Do nothing. */ ;
    }
};

}; // End of vpr namespace


#endif	/* _VPR_RW_MUTEX_H_ */
