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
//	Allen Bierbaum
//
// Date: 1-20-97
//-----------------------------------------------

#include <config.h>
#include <ulocks.h>
#include <SharedMem/vjMemPool.h>
#include <SharedMem/vjMemPoolSGI.h>

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
        *attachedCounter = *attachedCounter + 1;	    // Track how many mutexes are allocated
        //vjDEBUG << " vjMutexSGI:: vjMutexSGI: attachedCounter: " << *attachedCounter << endl << vjDEBUG_FLUSH;
      
        // ----- Allocate the mutex ----- //
        mutex = usnewlock(mutexPool->getArena());
    }

    ~ vjMutexSGI(void)
    {
        // ---- Delete the mutex --- //
        usfreelock(mutex, mutexPool->getArena());
    
        // ---- Deal with the pool --- //
        *attachedCounter = *attachedCounter - 1;	    // Track how many mutexes are allocated  
    
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
        return ussetlock(mutex);
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
        return uscsetlock(mutex, 100);	    // Try 100 spins.
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
        return usunsetlock(mutex);   
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
        return ustestlock(mutex);
    }

  
    //---------------------------------------------------------
    // void dump()
    //
    // PURPOSE:
    //  Dump the mutex debug stuff and current state.
    //---------------------------------------------------------
    void dump (FILE* dest = stderr, const char* message = "\n------ Mutex Dump -----\n") const
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
