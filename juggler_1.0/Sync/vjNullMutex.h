/*
 *  File:	    $RCSfile$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
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

#include <vjConfig.h>

//: Null mutex wrapper.  Used to pass a "do nothing" mutex as a template type.
//!PUBLIC_API:
class vjNullMutex
{
public:
    vjNullMutex () {}
    ~vjNullMutex() {}


    //---------------------------------------------------------
    //: Lock the mutex.
    //
    //! RETURNS:  1 - Acquired
    //! RETURNS: -1 - Error
    //---------------------------------------------------------
    int acquire() const
    {
        return 1;
    }

    //----------------------------------------------------------
    //: Acquire a read mutex.
    //----------------------------------------------------------
    int acquireRead() const
    {
        return this->acquire();	    // No special "read" semaphore -- For now
    }

    //----------------------------------------------------------
    //: Acquire a write mutex.
    //----------------------------------------------------------
    int acquireWrite() const
    {
        return this->acquire();	    // No special "write" semaphore -- For now
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
        return 1;	    // Try 100 spins.
    }

    //----------------------------------------------------------
    //: Try to acquire a read mutex.
    //----------------------------------------------------------
    int tryacquire_read () const
    {
        return this->tryAcquire();
    }

    //----------------------------------------------------------
    //: Try to acquire a write mutex.
    //----------------------------------------------------------
    int tryacquire_write () const
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
        return 0;
    }

    //------------------------------------------------------
    //: Test the current lock status.
    //
    //! RETURNS: 0 - Not locked
    //! RETURNS: 1 - Locked
    //------------------------------------------------------
    int test()
    {
        return 0;     // Just return 0 since it is a null lock
    }

    //---------------------------------------------------------
    //: Dump the mutex debug stuff and current state.
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
