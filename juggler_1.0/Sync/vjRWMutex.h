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
    // int aquire()
    //
    // PURPOSE:
    //:   Lock the mutex
    //! RETVAL:   1 - Aquired
    //! RETVAL:  -1 - Error
    //---------------------------------------------------------
    int acquire()
    {
        return aquireWrite();
    }

    //----------------------------------------------------------
    //:  Aquire a read mutex
    //----------------------------------------------------------
    int acquireRead()
    {
        int retVal = 0;
        if (stateLock.aquire() == -1)
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
    //:  Aquire a write mutex
    //----------------------------------------------------------
    int acquireWrite()
    {
        int retVal = 0;
        if (stateLock.aquire() == -1)
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

    //---------------------------------------------------------
    // int tryAquire()
    //
    // PURPOSE:
    //:  Trys to aquire the mutex.
    //   Wait until the semaphore value is greater than 0.
    //   Then decrement by 1 and return.
    //   P operation.
    // RETURNS:
    //!RETVAL:   1 - Aquired
    //!RETVAL:   0 - Not Aquired
    //---------------------------------------------------------
    int tryAcquire ()
    {
        return tryAcquireWrite();
    }

    //----------------------------------------------------------
    //:  Try to aquire a read mutex
    //----------------------------------------------------------
    int tryAcquireRead ()
    {
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
    //:  Try to aquire a write mutex
    //----------------------------------------------------------
    int tryAcquireWrite ()
    {
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
    // int release()
    //
    // PURPOSE:
    //:   Release the mutex.
    // RETURNS:
    //!RETVAL:   0 - Success
    //!RETVAL:  -1 - Error
    //---------------------------------------------------------
    int release()
    {
        if (stateLock.acquire() == -1)
            return -1;

        if(refCount > 0)	    // We have a reader to release
            refCount--;
        else if (refCount == -1)	// We have writer
            refCount = 0;
        else			// We have an error
            cerr << "vjRWMutex::release: Should not have refCount of 0!!!" << endl;
	
        int retVal = 0;

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

    //------------------------------------------------------
    // int test()
    //
    // PURPOSE:
    //:	Test the current lock status
    // RETURNS:
    //!RETVAL:  0 - Not locked
    //!RETVAL:  1 - Locked
    //------------------------------------------------------
    int test()
    {
        return stateLock.test();
    }


    //---------------------------------------------------------
    // void dump()
    //
    // PURPOSE:
    //:  Dump the mutex debug stuff and current state.
    //---------------------------------------------------------
    void dump (FILE* dest = stderr, const char* message = "\n------ Mutex Dump -----\n") const
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
