/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
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
