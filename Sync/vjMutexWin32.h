#ifndef _vjMutexWin32_h_
#define _vjMutexWin32_h_
//----------------------------------------------
// vjMutexWin32
//
// Purpose: (also called a lock)
//    Mutex wrapper for Win32 systems
//    Used for critical section protection
//
// Author:
//	Andy Himberger
//
// Date: 11-7-97
//-----------------------------------------------
#include <windows.h>
#include <SharedMem/vjMemPool.h>
#include <SharedMem/vjMemPoolWin32.h>

//: Mutex wrapper for Win32 systems.
//!PUBLIC_API:
class vjMutexWin32
{
public:
    vjMutexWin32 ()
    {
        // BUG: Possible race condition here
        if(mutexPool == NULL) {
            mutexPool = new vjMemPoolWin32(65536, 32, "memMutexPoolSGIXXXXXX");
            attachedCounter = static_cast<int*>(mutexPool->allocate(sizeof(int)));
            *attachedCounter = 0;
        }
		
        *attachedCounter = *attachedCounter + 1;	    // Track how many mutexes are allocated
        cerr << "vjMutexWin32::vjMutexWin32: attachedCounter: " << *attachedCounter << endl;

        // ----- Allocate the mutex ----- //
        mutex = CreateMutex(NULL,FALSE,NULL);
    }

    ~vjMutexWin32(void)
    {
        // ---- Delete the mutex --- //
        CloseHandle(mutex);

        // ---- Deal with the pool --- //
        *attachedCounter = *attachedCounter - 1;	    // Track how many mutexes are allocated

        cerr << "vjMutexWin32::~vjMutexWin32: attachedCounter: " << *attachedCounter << endl;

        if(*attachedCounter == 0)
        {
            mutexPool->deallocate(attachedCounter);
            attachedCounter = NULL;
            delete mutexPool;
            mutexPool = NULL;
        }

    }

    //---------------------------------------------------------
    // int aquire()
    //
    // PURPOSE:
    //   Lock the mutex
    // RETURNS:
    //   1 - Aquired
    //  -1 - Error
    //---------------------------------------------------------
    int acquire() const
    {
		DWORD dw = WaitForSingleObject(mutex,INFINITE);
		if (dw == WAIT_OBJECT_0)
		{
			return 1;
		}
        return -1;
    }

    //----------------------------------------------------------
    //  Aquire a read mutex
    //----------------------------------------------------------
    int acquireRead() const
    {
        return this->acquire();	    // No special "read" semaphore -- For now
    }

    //----------------------------------------------------------
    //  Aquire a write mutex
    //----------------------------------------------------------
    int acquireWrite() const
    {
        return this->acquire();	    // No special "write" semaphore -- For now
    }

    //---------------------------------------------------------
    // int tryAquire()
    //
    // PURPOSE:
    //   Try to acquire the lock.
    //   Returns immediately even if we don't aquire the lock.
    // RETURNS:
    //   1 - Aquired
    //   0 - Not Aquired
    //---------------------------------------------------------
    int tryAcquire () const
    {
        DWORD dw = WaitForSingleObject(mutex,0);
		if (dw == WAIT_OBJECT_0)
		{
			return 1;
		}
		return 0;	
    }

    //----------------------------------------------------------
    //  Try to aquire a read mutex
    //----------------------------------------------------------
    int tryAcquireRead () const
    {
        return this->tryAcquire();
    }

    //----------------------------------------------------------
    //  Try to aquire a write mutex
    //----------------------------------------------------------
    int tryAcquireWrite () const
    {
        return this->tryAcquire();
    }

    //---------------------------------------------------------
    // int release()
    //
    // PURPOSE:
    //   Release the mutex.
    // RETURNS:
    //   0 - Success
    //  -1 - Error
    //---------------------------------------------------------
    int release() const
    {
        return ReleaseMutex(mutex);
    }

    //------------------------------------------------------
    // int test()
    //
    // PURPOSE:
    //	Test the current lock status
    // RETURNS:
    //  0 - Not locked
    //  1 - Locked
    //------------------------------------------------------
    int test()
    {
		DWORD dw = WaitForSingleObject(mutex,0);
		if (dw == WAIT_OBJECT_0)
		{
			ReleaseMutex(mutex);
			return 0;
		}
        return 1;
    }


    //---------------------------------------------------------
    // void dump()
    //
    // PURPOSE:
    //  Dump the mutex debug stuff and current state.
    //---------------------------------------------------------
    void dump (FILE* dest = stderr, const char* message = "\n------ Mutex Dump -----\n") const
    {
		cout << "Mutex::dump" << endl;
    }


protected:
    HANDLE mutex;

    // = Prevent assignment and initialization.
    void operator= (const vjMutexWin32 &) {}
    vjMutexWin32 (const vjMutexWin32 &) {}

    static vjMemPoolWin32* mutexPool;
    static int* attachedCounter;
};

vjMemPoolWin32* vjMutexWin32::mutexPool = NULL;
int* vjMutexWin32::attachedCounter = NULL;

#endif
