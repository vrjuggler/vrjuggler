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
 *   Patrick Hartling.
 * --------------------------------------------------------------------------
 */

#ifndef _VPR_SEMAPHORE_NSPR_H_
#define _VPR_SEMAPHORE_NSPR_H_

#include <vpr/vprConfig.h>

#include <stdio.h>
#include <pratom.h>

#include <vpr/Sync/CondVar.h>


namespace vpr {

//: Wrapper for semaphores implemented using condition variables.

//!PUBLIC_API:
class VPR_CLASS_API SemaphoreNSPR {
public:
    // -----------------------------------------------------------------------
    //: Custructor for vpr::SemaphoreNSPR class.
    //
    //! PRE: None.
    //! POST: The semaphore variable for the class is initilized as an
    //+       unnamed semaphore.
    //
    //! ARGS: initialValue - The initial number of resources controlled by
    //+                      the semaphore.  If not specified, the default
    //+                      value is 1.
    // -----------------------------------------------------------------------
    SemaphoreNSPR (int initial_value = 1) {
        mCondVar = new CondVar;
        PR_AtomicSet(&mValue, initial_value);
    }

    // -----------------------------------------------------------------------
    //: Destructor for vpr::SemaphoreNSPR class.
    //
    //! PRE: None.
    //! POST: The resources used by the semaphore variable are freed.
    // -----------------------------------------------------------------------
    ~SemaphoreNSPR (void) {
        delete mCondVar;
    }

    // -----------------------------------------------------------------------
    //: Lock the semaphore.
    //
    //! PRE: None.
    //! POST: The calling thread either acquires the semaphore until
    //+       release() is called, or the caller is put at the tail of a wait
    //+       and is suspended until such time as it can be freed and allowed
    //+       to acquire the semaphore itself.
    //
    //! RETURNS:  1 - Lock acquired
    //! RETURNS: -1 - Error
    // -----------------------------------------------------------------------
    inline vpr::ReturnStatus
    acquire (void) {
        mCondVar->acquire();
            PR_AtomicDecrement(&mValue);

            while ( mValue < 0 ) {
                mCondVar->wait();
            }
        mCondVar->release();

        return vpr::ReturnStatus();
    }

    // -----------------------------------------------------------------------
    //: Acquire and lock a read semaphore.
    //
    //! PRE: None.
    //! POST: The calling thread either acquires the semaphore until
    //+       release() is called, or the caller is put at the tail of a wait
    //+       and is suspended until such time as it can be freed and allowed
    //+       to acquire the semaphore itself.
    //
    //! RETURNS:  1 - Lock acquired
    //! RETURNS: -1 - Error
    //
    //! NOTE: There is no special read semaphore for now.
    // -----------------------------------------------------------------------
    inline vpr::ReturnStatus
    acquireRead (void) {
        return this->acquire();
    }

    // -----------------------------------------------------------------------
    //: Acquire and lock a write semaphore.
    //
    //! PRE: None.
    //! POST: The calling thread either acquires the semaphore until
    //+       release() is called, or the caller is put at the tail of a wait
    //+       and is suspended until such time as it can be freed and allowed
    //+       to acquire the semaphore itself.
    //
    //! RETURNS:  1 - Lock acquired
    //! RETURNS: -1 - Error
    //
    //! NOTE: There is no special write semaphore for now.
    // -----------------------------------------------------------------------
    inline vpr::ReturnStatus
    acquireWrite (void) {
        return this->acquire();
    }

    // -----------------------------------------------------------------------
    //: Try to acquire the semaphore immediately (does not block).
    //
    //! PRE: None.
    //! POST: If the semaphore could be acquired by the caller, the caller
    //+       gets control of the semaphore.  If the semaphore was already
    //+       locked, the routine returns immediately without suspending the
    //+       calling thread.
    //
    //! RETURNS: 1 - Acquired
    //! RETURNS: 0 - Not acquired
    // -----------------------------------------------------------------------
    inline vpr::ReturnStatus
    tryAcquire (void) {
        vpr::ReturnStatus status(vpr::ReturnStatus::Fail);

        if ( mValue >= 0 ) {
            status = this->acquire();
        }

        return status;
    }

    // -----------------------------------------------------------------------
    //: Try to acquire a read semaphore (does not block).
    //
    //! PRE: None.
    //! POST: If the semaphore could be acquired by the caller, the caller
    //+       gets control of the semaphore.  If the semaphore was already
    //+       locked, the routine returns immediately without suspending the
    //+       calling thread.
    //
    //! RETURNS: 1 - Acquired
    //! RETURNS: 0 - Not acquired
    // -----------------------------------------------------------------------
    inline vpr::ReturnStatus
    tryAcquireRead (void) {
        return this->tryAcquire();
    }

    // -----------------------------------------------------------------------
    //: Try to acquire a write semaphore (does not block).
    //
    //! PRE: None.
    //! POST: If the semaphore could be acquired by the caller, the caller
    //+       gets control of the semaphore.  If the semaphore was already
    //+       locked, the routine returns immediately without suspending the
    //+       calling thread.
    //
    //! RETURNS: 1 - Acquired
    //! RETURNS: 0 - Not acquired
    // -----------------------------------------------------------------------
    inline vpr::ReturnStatus
    tryAcquireWrite (void) {
        return this->tryAcquire();
    }

    // -----------------------------------------------------------------------
    //: Release the semaphore.
    //
    //! PRE: The semaphore should have been locked before being released.
    //! POST: The semaphore is released and the thread at the haed of the
    //+       wait queue is allowed to execute again.
    //
    //! RETURNS:  0 - Succeed
    //! RETURNS: -1 - Error
    // -----------------------------------------------------------------------
    inline vpr::ReturnStatus
    release (void) {
        vpr::ReturnStatus status;

        mCondVar->acquire();
            PR_AtomicIncrement(&mValue);
            status = mCondVar->signal();
        mCondVar->release();

        return status;
    }

    // -----------------------------------------------------------------------
    //: Reset the semaphore.
    //
    //! PRE: None.
    //! POST: The semaphore's count is set to the specified value.
    //
    //! ARGS: val - The value to which the semaphore is reset.
    //
    //! RETURNS:  0 - Succeed
    //! RETURNS: -1 - Error
    //
    //! NOTE: If processes are waiting on the semaphore, the results are
    //+       undefined.
    // -----------------------------------------------------------------------
    inline vpr::ReturnStatus
    reset (int val) {
        vpr::ReturnStatus status;

        mCondVar->acquire();
            PR_AtomicSet(&mValue, val);
            status = mCondVar->broadcast();
        mCondVar->release();

        return status;
    }

    // -----------------------------------------------------------------------
    //: Dump the semaphore debug stuff and current state.
    //
    //! PRE: None.
    //! POST: All important data and debugging information related to the
    //+       semaphore is dumped to the specified file descriptor (or to
    //+       stderr if none is given).
    //
    //! ARGS: dest - File descriptor to which the output will be written.
    //+              It defaults to stderr if no descriptor is specified.
    //! ARGS: message - Message printed out before the output is dumped.
    // -----------------------------------------------------------------------
    void
    dump (FILE* dest = stderr,
          const char* message = "\n------ Semaphore Dump -----\n") const
    {
        fprintf(dest, "%s", message);
        fprintf(dest, "Current semaphore value: %d", mValue);
    }

protected:
    CondVar*  mCondVar;       //: Semaphore simulator variable for the class
    PRInt32   mValue;

    // Prevent assignment and initialization.
    void operator= (const SemaphoreNSPR &) {
        /* Do nothing. */ ;
    }

    SemaphoreNSPR (const SemaphoreNSPR &) {
        /* Do nothing. */ ;
    }
};

}; // End of vpr namespace


#endif  /* _VPR_SEMAPHORE_NSPR_H_ */
