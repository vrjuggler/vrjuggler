/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ****************** <VPR heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VPR_CondVarGeneric_h_
#define _VPR_CondVarGeneric_h_

#include <vpr/vprConfig.h>

#include <iostream>
#include <iomanip>

#ifdef VPR_OS_Win32
#  include <process.h>
#endif

#include <sys/types.h>

#ifdef HAVE_UNISTD_H
#  include <unistd.h>
#endif

#include <vpr/Sync/Semaphore.h>
#include <vpr/Sync/Mutex.h>
#include <vpr/Util/Debug.h>
#include <vpr/Util/Interval.h>


namespace vpr
{

/**
 * Condition Variable wrapper for the any system.
 *
 *    Condition variables allow a locked test waiting
 *  for specific conditions to be satisfied.  For
 *  example waiting for a flag to be set or for a
 *  counter to fall below a certain value.
 *
 * Example:
 *    (Insert Example here)
 *
 * @date 1-29-97
 */
class VPR_CLASS_API CondVarGeneric
{
public:
   /**
    * Constructor.
    *
    * @param mutex A pointer to a user specified mutex
    *             if not specified, uses internal mutex.
    */
   CondVarGeneric(Mutex* mutex = NULL)
   {
      if ( mutex == NULL )
      {
         mutex = &defaultMutex;
      }

      condMutex = mutex;
      waiters = 0;

      std::cerr << "------------------------------------\n"
                << "  vpr::CondVarGeneric: DOES NOT WORK\n"
                << "------------------------------------\n";
   }

   /**
    * Waits for possible condition change.
    *
    * @post The condition has been modifed, but may not be satisfied.
    * @note The call blocks until a condition has been signaled
    */
   vpr::ReturnStatus wait(vpr::Interval time_to_wait = vpr::Interval::NoTimeout);

   /**
    * Signals a condition change.
    * This call tells all waiters that the condition has changed.
    * They can then check to see if it now sarisfies the condition
    */
   vpr::ReturnStatus signal ()
   {
      std::cerr << std::setw(5) << getpid() << "  Signal" << std::endl;
      // ASSERT:  We have been locked
      if ( condMutex->test() == 0 )    // Not locked
      {
         std::cerr << " vpr::CondVarGeneric::signal: Mutex was not locked when signal called!!!" << std::endl;
      }

      if ( waiters > 0 )
      {
         return sema.release();
      }
      else
      {
         return vpr::ReturnStatus();
      }
   }

   /**
    * Signals all waiting threads.
    * This releases all waiting threads.
    */
   vpr::ReturnStatus broadcast ()
   {
      // ASSERT:  We have been locked
      if ( condMutex->test() == 0 )    // Not locked
      {
         std::cerr << " vpr::CondVarGeneric::broadcast: Mutex was not locked when broadcase called!!!" << std::endl;
      }

      for ( int i = waiters;i>0;i-- )
      {
         sema.release();
      }

      return vpr::ReturnStatus();
   }

   /// Acquires the condition lock.
   vpr::ReturnStatus acquire()
   {
      return condMutex->acquire();
   }

   /// Tries to acquire the condition lock.
   vpr::ReturnStatus tryAcquire()
   {
      return condMutex->tryAcquire();
   }

   /// Releases the condition lock.
   vpr::ReturnStatus release()
   {
      return condMutex->release();
   }

   /**
    * Explicitly set the mutex to use.
    *
    * @note NEVER call except to initialize explicitly.
    */
   void setMutex(Mutex* mutex)
   {
      mutex->release();       // NOT exactly correct, but just make sure not to leave it locked
      condMutex = mutex;
   }

   void dump (void) const
   {
      vprDEBUG_BEGIN(vprDBG_ALL,0)
         << "------------- vpr::CondVarGeneric::Dump ---------\n"
         << vprDEBUG_FLUSH;
      vprDEBUG(vprDBG_ALL,0) << "waiters: " << waiters << std::endl
                             << vprDEBUG_FLUSH;
      condMutex->dump();
      vprDEBUG_END(vprDBG_ALL,0) << "-----------------------------------\n"
                                 << vprDEBUG_FLUSH;
   }

private:
   // --- These make up the "condition variable" ---- ///
   Semaphore sema;     //! Condition variable.
   long waiters;       //! The number of processes waiting

   Mutex* condMutex;   //! Mutex for the condition variable - User specified
   Mutex defaultMutex; //! Mutex to use if user does not specify one

   // = Prevent assignment and initialization.
   void operator= (const CondVarGeneric&)
   {
      ;
   }

   CondVarGeneric (const CondVarGeneric &c)
   {
      ;
   }
};

} // End of vpr namespace


#endif
