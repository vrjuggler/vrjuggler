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


#ifndef _VJRWMutex_h_
#define _VJRWMutex_h_

#include <vjConfig.h>
#include <Sync/vjSemaphore.h>
#include <Sync/vjCond.h>
#include <Sync/vjMutex.h>

//----------------------------------------------
// vjRWMutex
//
// Purpose:
//:    vjRWMutex wrapper
//
//
// Author:
//	Allen Bierbaum
//
// Date: 1-31-97
//-----------------------------------------------
//!PUBLIC_API:
class vjRWMutex
{
public:
    vjRWMutex () : waitingReaders(&stateLock),
        waitingWriters(&stateLock),
        refCount(0),
        numWaitingWriters(0),
        numWaitingReaders(0)
    {}

    ~vjRWMutex(void)
    {}

    //---------------------------------------------------------
    //: Lock the mutex.
    //
    //! RETURNS:  1 - Acquired
    //! RETURNS: -1 - Error
    //---------------------------------------------------------
    int acquire()
    {
        return acquireWrite();
    }

    //----------------------------------------------------------
    //: Acquire a read mutex.
    //----------------------------------------------------------
    int acquireRead(void);

    //----------------------------------------------------------
    //: Acquire a write mutex.
    //----------------------------------------------------------
    int acquireWrite(void);

    //---------------------------------------------------------
    //: Trys to acquire the mutex.
    //  Wait until the semaphore value is greater than 0.
    //  Then decrement by 1 and return.
    //  P operation.
    //
    //! RETURNS: 1 - Acquired
    //! RETURNS: 0 - Not acquired
    //---------------------------------------------------------
    int tryAcquire ()
    {
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
    int release(void);

    //------------------------------------------------------
    //:	Test the current lock status.
    //
    //! RETURNS: 0 - Not locked
    //! RETURNS: 1 - Locked
    //------------------------------------------------------
    int test()
    {
        return stateLock.test();
    }

    //---------------------------------------------------------
    //: Dump the mutex debug stuff and current state.
    //---------------------------------------------------------
    void dump (FILE* dest = stderr,
               const char* message = "\n------ Mutex Dump -----\n") const
    {
        stateLock.dump();
    }

protected:
    vjMutex stateLock;        //: Serialize access to internal state.
    vjCond waitingReaders;    //: Reader threads waiting to acquire the lock.
    int numWaitingReaders;    //: Number of waiting readers.

    vjCond waitingWriters;    //: Writer threads waiting to acquire the lock.
    int numWaitingWriters;    //: Number of waiting writers.

    // Value is -1 if writer has the lock, else this keeps track of the
    // number of readers holding the lock.
    int refCount;

    // = Prevent assignment and initialization.
    void operator= (const vjRWMutex &) {}
    vjRWMutex (const vjRWMutex &) {}
};

#endif
