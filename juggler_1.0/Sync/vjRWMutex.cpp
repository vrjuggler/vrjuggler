/*
 *  File:	    $Name$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#include <vjConfig.h>

#include <Sync/vjRWMutex.h>


//----------------------------------------------------------
// Acquire a read mutex.
//----------------------------------------------------------
int
vjRWMutex::acquireRead () {
    int retVal = 0;

    if (stateLock.acquire() == -1)
        retVal = -1;			// Didn't get the lock
    else
    {
        // let those writers who are waiting have first shot
        while(refCount < 0 || numWaitingWriters > 0)
        {
            numWaitingReaders++;	    // Another one waiting
            waitingReaders.wait();	    // So wait until something changes
            numWaitingReaders--;	    //
        }
    }

    if (retVal == 0) {
        refCount++;
        stateLock.release();
    }

    return retVal;
}

//----------------------------------------------------------
// Acquire a write mutex.
//----------------------------------------------------------
int
vjRWMutex::acquireWrite() {
    int retVal = 0;

    if (stateLock.acquire() == -1)
        retVal = -1;			// Didn't get the lock
    else
    {
        while(refCount != 0)	    // While there are readers
        {
            numWaitingWriters++;	// One more waiting
            waitingWriters.wait();	// Wait for soemthing to change
            numWaitingWriters--;	// Not waiting any more
        }
    }

    if(retVal == 0)
    {
        refCount = -1;		// Tell everyone that there is a writer
        stateLock.release();
    }

    return retVal;
}

//----------------------------------------------------------
// Try to acquire a read mutex.
//----------------------------------------------------------
int
vjRWMutex::tryAcquireRead () {
    int retVal = -1;

    if (stateLock.acquire() != -1)
    {
        if(refCount == -1 || numWaitingWriters >0)
            retVal = -1;
        else
        {
            refCount++;
            retVal = 0;
        }
        stateLock.release();
    }
    return retVal;
}

//----------------------------------------------------------
// Try to acquire a write mutex.
//----------------------------------------------------------
int
vjRWMutex::tryAcquireWrite () {
    int retVal = -1;

    if (stateLock.acquire() != -1)
    {
        if(refCount != 0)
            retVal = -1;
        else
        {
            refCount = -1;
            retVal = 0;
        }
        stateLock.release();
    }

    return retVal;
}

//---------------------------------------------------------
// Release the mutex.
//
// RETURNS:  0 - Success
// RETURNS: -1 - Error
//---------------------------------------------------------
int
vjRWMutex::release () {
    int retVal = 0;

    if (stateLock.acquire() == -1)
        return -1;

    if(refCount > 0)	    // We have a reader to release
        refCount--;
    else if (refCount == -1)	// We have writer
        refCount = 0;
    else			// We have an error
        cerr << "vjRWMutex::release: Should not have refCount of 0!!!" << endl;

    // Preference to writers
    if (numWaitingWriters > 0)
    {
        retVal = waitingWriters.signal();
    }
    else if (numWaitingReaders > 0)
    {
        retVal = waitingReaders.broadcast();
    }
    else
        retVal = 0;

    stateLock.release();

    return retVal;	
}
