#ifndef _vjNullMutex_h_
#define _vjNullMutex_h_
//----------------------------------------------
// vjNullMutex
//
// Purpose:
//    Null mutex wrapper.  Used to pass a do nothing
//	mutex as a template type.
//
// Author:
//	Allen Bierbaum
//
// Date: 1-21-97
//-----------------------------------------------

#include <config.h>
#include <iostream.h>

class vjNullMutex
{
public:
    vjNullMutex () {}
    ~vjNullMutex() {}
  
  
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
        return 1;
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
        return 1;	    // Try 100 spins.
    }

    //----------------------------------------------------------
    //  Try to aquire a read mutex
    //----------------------------------------------------------
    int tryacquire_read () const
    {
        return this->tryAcquire();
    }
  
    //----------------------------------------------------------
    //  Try to aquire a write mutex
    //----------------------------------------------------------
    int tryacquire_write () const
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
        return 0;   
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
        return 0;     // Just return 0 since it is a null lock
    }

  
    //---------------------------------------------------------
    // void dump()
    //
    // PURPOSE:
    //  Dump the mutex debug stuff and current state.
    //---------------------------------------------------------
#ifndef WIN32 
    void dump (FILE* dest = stderr, const char* message = "\n------ Mutex Dump -----\n") const
    {
        cerr << message << "NULL Mutex\n";  
    }
#endif  

protected:
    // = Prevent assignment and initialization.
    void operator= (const vjNullMutex &) {}
    vjNullMutex (const vjNullMutex &) {}
};

#endif
