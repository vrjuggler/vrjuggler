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


#ifndef _vjMutexSGI_h_
#define _vjMutexSGI_h_
//----------------------------------------------
//  vjMutexSGI
//
// Purpose: (also called a lock)
//    Mutex wrapper for the SGI systems
//    Used for critical section protection
//
// Author:
// Allen Bierbaum
//
// Date: 1-20-97
//-----------------------------------------------

#include <vjConfig.h>
#include <ulocks.h>
#include <SharedMem/vjMemPool.h>
#include <SharedMem/vjMemPoolSGI.h>
#include <Kernel/vjAssert.h>

//: Mutex wrapper for the SGI systems.  Used for critical section protection.
//!PUBLIC_API:
class  vjMutexSGI
{
public:
     vjMutexSGI ()
    {
        // BUG: Possible race condition here
        if(mutexPool == NULL) {
            mutexPool = new vjMemPoolSGI(65536, 32, "/var/tmp/memMutexPoolSGIXXXXXX");
            attachedCounter = static_cast<int*>(mutexPool->allocate(sizeof(int)));
            *attachedCounter = 0;
        }
        *attachedCounter = *attachedCounter + 1;       // Track how many mutexes are allocated
        //vjDEBUG << " vjMutexSGI:: vjMutexSGI: attachedCounter: " << *attachedCounter << endl << vjDEBUG_FLUSH;

        // ----- Allocate the mutex ----- //
        mutex = usnewlock(mutexPool->getArena());
        vjASSERT( mutex != NULL && "in vjMutexSGI::vjMutexSGI() mutex is NULL" );
    }

    ~ vjMutexSGI(void)
    {
        // ---- Delete the mutex --- //
        usfreelock(mutex, mutexPool->getArena());

        // ---- Deal with the pool --- //
        *attachedCounter = *attachedCounter - 1;       // Track how many mutexes are allocated

        //vjDEBUG << " vjMutexSGI::~ vjMutexSGI: attachedCounter: " << *attachedCounter << endl << vjDEBUG_FLUSH;

        if(*attachedCounter == 0)
        {
            mutexPool->deallocate(attachedCounter);
            attachedCounter = NULL;
            delete mutexPool;
            mutexPool = NULL;
        }

    }

    //---------------------------------------------------------
    //: Lock the mutex.
    //
    //! RETURNS:  1 - Acquired
    //! RETURNS: -1 - Error
    //---------------------------------------------------------
    int acquire() const
    {
       vjASSERT( mutex != NULL && "in vjMutexSGI::aquire() mutex is NULL" );
        return ussetlock( mutex );
    }

    //----------------------------------------------------------
    //: Acquire a read mutex.
    //----------------------------------------------------------
    int acquireRead() const
    {
        return this->acquire();      // No special "read" semaphore -- For now
    }

    //----------------------------------------------------------
    //: Acquire a write mutex.
    //----------------------------------------------------------
    int acquireWrite() const
    {
        return this->acquire();      // No special "write" semaphore -- For now
    }

    //---------------------------------------------------------
    //: Try to acquire the lock.  Returns immediately even if
    //+ we don't acquire the lock.
    //
    //! RETURNS: 1 - Acquired
    //! RETURNS: 0 - Not acquired
    //---------------------------------------------------------
    int tryAcquire () const
    {
        return uscsetlock(mutex, 100);     // Try 100 spins.
    }

    //----------------------------------------------------------
    //: Try to acquire a read mutex.
    //----------------------------------------------------------
    int tryAcquireRead () const
    {
        return this->tryAcquire();
    }

    //----------------------------------------------------------
    //: Try to acquire a write mutex.
    //----------------------------------------------------------
    int tryAcquireWrite () const
    {
        return this->tryAcquire();
    }

    //---------------------------------------------------------
    //: Release the mutex.
    //
    //! RETURNS:  0 - Success
    //! RETURNS: -1 - Error
    //---------------------------------------------------------
    int release() const
    {
        return usunsetlock(mutex);
    }

    //------------------------------------------------------
    //: Test the current lock status.
    //
    //! RETURNS: 0 - Not locked
    //! RETURNS: 1 - Locked
    //------------------------------------------------------
    int test()
    {
        return ustestlock(mutex);
    }

    //---------------------------------------------------------
    //: Dump the mutex debug stuff and current state.
    //---------------------------------------------------------
    void dump (FILE* dest = stdout,
               const char* message = "\n------ Mutex Dump -----\n") const
    {
        usdumplock(mutex, dest, message);
    }

protected:
    ulock_t mutex;

    // = Prevent assignment and initialization.
    void operator= (const  vjMutexSGI &) {}
     vjMutexSGI (const  vjMutexSGI &) {}

    static vjMemPoolSGI* mutexPool;
    static int* attachedCounter;
};

#endif
