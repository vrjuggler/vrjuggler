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

#ifndef _VPR_CondGeneric_h_
#define _VPR_CondGeneric_h_

#include <vprConfig.h>

#ifdef VPR_OS_Win32
#   include <process.h>
#endif

#include <sys/types.h>

#ifdef HAVE_UNISTD_H
#   include <unistd.h>
#endif

#include <Sync/Semaphore.h>
#include <Sync/Mutex.h>
#include <Utils/Debug.h>


namespace vpr {

//----------------------------------------------
//  vpr::CondGeneric
//
// Purpose:
//:   Condition Variable wrapper for the any system
//
//    Condition variables allow a locked test waiting
//  for specific conditions to be satisfied.  For
//  example waiting for a flag to be set or for a
//  counter to fall below a certain value.
//
// Example:
//    (Insert Example here)
//
// Author:
//	Allen Bierbaum
//
// Date: 1-29-97
//!PUBLIC_API:
//-----------------------------------------------
class CondGeneric
{
public:
   //: Constructor
   //! ARGS: mutex - a pointer to a user specified mutex
   //+               if not specified, uses internal mutex
   CondGeneric(Mutex* mutex = NULL)
   {
      if (mutex == NULL)
         mutex = &defaultMutex;

      condMutex = mutex;
      waiters = 0;

      std::cerr << "-----------------------------------\n"
                << "  vpr::CondGeneric: DOES NOT WORK\n"
                << "-----------------------------------\n";
   }

   //: Wait for possible condition change
   //! POST: The condition has been modifed, but may not be satisfied.
   //! NOTE: The call blocks until a condition has been signaled
   int wait(void);

   //: Signal a condition change
   // This call tells all waiters that the condition has changed.
   // They can then check to see if it now sarisfies the condition
   int signal ()
   {
      std::cerr << std::setw(5) << getpid() << "  Signal" << std::endl;
      // ASSERT:  We have been locked
      if (condMutex->test() == 0)    // Not locked
         std::cerr << " vpr::CondGeneric::signal: Mutex was not locked when signal called!!!" << std::endl;

      if (waiters > 0)
      {
         int ret_val = sema.release();
         return ret_val;
      }
      else
         return 0;
   }

   //: Signal all waiting threads
   // This releases all waiting threads.
   int broadcast ()
   {
      // ASSERT:  We have been locked
      if (condMutex->test() == 0)    // Not locked
         std::cerr << " vpr::CondGeneric::broadcast: Mutex was not locked when broadcase called!!!" << std::endl;

      for (int i = waiters;i>0;i--)
         sema.release();

      return 0;
   }

   //: Acquire the condition lock.
   int acquire()
   {
      return condMutex->acquire();
   }

   //: Try to acquire the condition lock.
   int tryAcquire()
   {
      return condMutex->tryAcquire();
   }

   //: Release the condition lock.
   int release()
   {
      return condMutex->release();
   }

   //: Explicitly set the mutex to use.
   //! NOTE: NEVER call except to initialize explicitly.
   void setMutex(Mutex* mutex)
   {
      mutex->release();       // NOT exactly correct, but just make sure not to leave it locked
      condMutex = mutex;
   }

   void dump (void) const
   {
      vprDEBUG_BEGIN(vprDBG_ALL,0)
          << "------------- vpr::CondGeneric::Dump ---------\n"
          << vprDEBUG_FLUSH;
      vprDEBUG(vprDBG_ALL,0) << "waiters: " << waiters << std::endl
                             << vprDEBUG_FLUSH;
      condMutex->dump();
      vprDEBUG_END(vprDBG_ALL,0) << "-----------------------------------\n"
                                 << vprDEBUG_FLUSH;
   }


private:
   // --- These make up the "condition variable" ---- ///
   Semaphore sema;     // Condition variable.
   long waiters;       //: The number of processes waiting

   Mutex* condMutex;   //: Mutex for the condition variable - User specified
   Mutex defaultMutex; //: Mutex to use if user does not specify one

   // = Prevent assignment and initialization.
   void operator= (const CondGeneric&) {;}
   CondGeneric (const CondGeneric &c) {;}
};

}; // End of vpr namespace


#endif
