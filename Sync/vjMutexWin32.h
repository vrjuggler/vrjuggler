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
//#include <SharedMem/vjMemPool.h>
//#include <SharedMem/vjMemPoolWin32.h>

//: Mutex wrapper for Win32 systems.
//!PUBLIC_API:
class vjMutexWin32
{
public:
    vjMutexWin32 ()
    {        
        // ----- Allocate the mutex ----- //
        // NULL - No security
        // FALSE - We don't want to qcquire it
        // NULL - Unamed version
        mutex = CreateMutex(NULL,FALSE,NULL);
        mLocked = false;                  // We don't have it locked yet
    }

    ~vjMutexWin32(void)
    {
        // ---- Delete the mutex --- //
        CloseHandle(mutex);
    }

    //---------------------------------------------------------
    //: Lock the mutex.
    //
    //! RETURNS:  1 - Acquired
    //! RETURNS: -1 - Error
    //---------------------------------------------------------
    int acquire()
    {
		DWORD dw = WaitForSingleObject(mutex,INFINITE);
		if (dw == WAIT_OBJECT_0)
		{
			mLocked = true;
         return 1;
		}
      else
      {
        return -1;
      }
    }

    //----------------------------------------------------------
    //: Acquire a read mutex.
    //----------------------------------------------------------
    int acquireRead()
    {
        return this->acquire();	    // No special "read" semaphore -- For now
    }

    //----------------------------------------------------------
    //: Acquire a write mutex.
    //----------------------------------------------------------
    int acquireWrite()
    {
        return this->acquire();	    // No special "write" semaphore -- For now
    }

    //---------------------------------------------------------
    //: Try to acquire the lock.  Returns immediately even if
    //+ we don't acquire the lock.
    //
    //! RETURNS: 1 - Acquired
    //! RETURNS: 0 - Not Acquired
    //---------------------------------------------------------
    int tryAcquire ()
    {
      DWORD dw = WaitForSingleObject(mutex,0);
		if (dw == WAIT_OBJECT_0)
		{
			mLocked = true;
         return 1;
		}
		return 0;	
    }

    //----------------------------------------------------------
    //: Try to acquire a read mutex.
    //----------------------------------------------------------
    int tryAcquireRead ()
    {
        return this->tryAcquire();
    }

    //----------------------------------------------------------
    //: Try to acquire a write mutex.
    //----------------------------------------------------------
    int tryAcquireWrite ()
    {
        return this->tryAcquire();
    }

    //---------------------------------------------------------
    //: Release the mutex.
    //
    //! RETURNS:  0 - Success
    //! RETURNS: -1 - Error
    //---------------------------------------------------------
    int release()
    {
       int ret_val = ReleaseMutex(mutex);
       
       if(ret_val == 1)
          mLocked = false;       // If failure, I will keep it locked

       return ret_val;
    }

    //------------------------------------------------------
    //: Test the current lock status.
    //
    //! NOTE: Since Win32 allows the same thread to 
    //+        acqurie a lock multiple times, we need to do
    //+        some extra stuff
    //
    //! RETURNS: 0 - Not locked
    //! RETURNS: 1 - Locked
    //------------------------------------------------------
    int test() const
    {
       if(mLocked == true)
          return 1;
       else
          return 0;
    }


    //---------------------------------------------------------
    //: Dump the mutex debug stuff and current state.
    //---------------------------------------------------------
    void dump (FILE* dest = stderr, const char* message = "\n------ Mutex Dump -----\n") const
    {
        std::cout << "Mutex::dump\nNot implemented on Win32" << std::endl;
    }


protected:
    HANDLE mutex;
    
      // We need this variable because win32 allows multiple
      // acquisitions by the same thread.  In order for us to
      // determine if the thread is currently locked we need to
      // test this variable
      // Notice that the only place this variable is touched is
      // in acquire and release where we implicitly have protection
    bool   mLocked;     // Says wether the mutex is locked
                        
    // = Prevent assignment and initialization.
    void operator= (const vjMutexWin32 &) {}
    vjMutexWin32 (const vjMutexWin32 &) {}
};


#endif
