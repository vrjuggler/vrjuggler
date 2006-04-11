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

/*
 * --------------------------------------------------------------------------
 * NOTES:
 *    - This file (MutexNSPR.h) must be included by vpr/Sync/Mutex.h, not the
 *      other way around.
 * ----------------------------------------------------------------------------
 */

#ifndef _VPR_MUTEX_NSPR_H_
#define _VPR_MUTEX_NSPR_H_

#include <vpr/vprConfig.h>

#include <stdio.h>
#include <stdlib.h>
#include <prlock.h>

#include <vpr/Util/Assert.h>
#include <vpr/Util/ReturnStatus.h>


namespace vpr
{

/**
 * Mutex wrapper for NSPR locks.
 */
class VPR_CLASS_API MutexNSPR
{
public:
   /**
    * Constructor.
    *
    * @pre None.
    * @post This mutex is initialized for use.  It must be initialized before
    *       any other member functions can do anything with it.
    */
   MutexNSPR (void) : mLocked(0)
   {
      // ----- Allocate the mutex ----- //
      mMutex = PR_NewLock();
      vprASSERT(mMutex != NULL);
   }

   /**
    * Destructor.
    *
    * @pre No thread should be in a lock-specific function.
    * @post The mutex variable is destroyed.
    */
   ~MutexNSPR (void)
   {
      PR_DestroyLock(mMutex);
   }

   /**
    * Locks this mutex.
    *
    * @pre None.
    * @post A lock on this mutex is acquired by the caller.  If a lock has
    *       already been acquired by another process/thread, the caller blocks
    *       until the mutex has been freed.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the lock is acquired
    *         successfully.  vpr::ReturnStatus::Fail is returned otherwise.
    */
   vpr::ReturnStatus acquire (void)
   {
      PR_Lock(mMutex);
      mLocked = 1;

      return vpr::ReturnStatus();
   }

   /**
    * Acquires a read lock.
    *
    * @pre None.
    * @post A lock on this mutex is acquired by the caller.  If a lock has
    *       already been acquired by another process/thread, the caller
    *       blocks until the mutex has been freed.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the read lock is
    *         acquired successfully.  vpr::ReturnStatus::Fail is returned
    *         otherwise.
    *
    * @note No special read mutex has been defined for now.
    */
   vpr::ReturnStatus acquireRead (void)
   {
      return this->acquire();
   }

   /**
    * Acquires a write lock.
    *
    * @pre None.
    * @post A lock on this mutex is acquired by the caller.  If a lock has
    *       already been acquired by another process/thread, the caller blocks
    *       until the mutex has been freed.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the write lock is
    *         acquired successfully.  vpr::ReturnStatus::Fail is returned
    *         otherwise.
    *
    * @note No special write mutex has been defined for now.
    */
   vpr::ReturnStatus acquireWrite (void)
   {
      return this->acquire();
   }

   /**
    * Tries to acquire a lock on this mutex (does not block).
    *
    * @pre None.
    * @post A lock on this mutex is acquired by the caller.  If a lock has
    *       already been acquired by another process/thread, the caller
    *       returns does not wait for it to be unlocked.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the lock is acquired.
    *         vpr::ReturnStatus::Fail is returned if another thread is
    *         holding the lock already.
    */
   vpr::ReturnStatus tryAcquire (void)
   {
      // XXX: Possible race condition exists in this function implementation
      if ( mLocked == 0 )
      {
         this->acquire();
         return vpr::ReturnStatus();
      }
      else
      {
         return vpr::ReturnStatus(vpr::ReturnStatus::Fail);
      }
   }

   /**
    * Tries to acquire a read lock (does not block).
    *
    * @pre None.
    * @post A lock on this mutex is acquired by the caller.  If a lock has
    *       already been acquired by another process/thread, the caller
    *       returns does not wait for it to be unlocked.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the read lock is
    *         acquired.  vpr::ReturnStatus::Fail is returned if another thread
    *         is holding the lock already.
    */
   vpr::ReturnStatus tryAcquireRead (void)
   {
      return this->tryAcquire();
   }

   /**
    * Tries to acquire a write lock (does not block).
    *
    * @pre None.
    * @post A lock on this mutex is acquired by the caller.  If a lock has
    *       already been acquired by another process/thread, the caller returns
    *       does not wait for it to be unlocked.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the write lock is
    *         acquired.  vpr::ReturnStatus::Fail is returned if another thread
    *         is holding the lock already.
    */
   vpr::ReturnStatus tryAcquireWrite (void)
   {
      return this->tryAcquire();
   }

   /**
    * Releases the mutex.
    *
    * @pre This mutex must be locked.
    * @post This mutex is unlocked.
    *
    * @return vpr::ReturnStatus::Succeed is returned if this mutex is unlocked
    *         successfully.  vpr::ReturnStatus::Fail is returned otherwise.
    */
   vpr::ReturnStatus release (void)
   {
      mLocked = 0;
      if ( PR_Unlock(mMutex) == PR_SUCCESS )
      {
         return vpr::ReturnStatus();
      }
      else
      {
         return vpr::ReturnStatus(vpr::ReturnStatus::Fail);
      }
   }

   /**
    * Tests the current lock status.
    *
    * @pre None.
    * @post The state of this mutex is returned.
    *
    * @return 0 is returned if this mutex is not locked.  1 is returned if it
    *         is locked.
    */
   int test (void)
   {
      return mLocked;
   }

   /**
    * Dumps the mutex debug stuff and current state.
    *
    * @pre None.
    * @post All important data and debugging information related to the
    *       mutex are dumped to the specified file descriptor (or to stderr
    *       if none is given).
    *
    * @param dest    File descriptor to which the output will be written.
    *                It defaults to stderr if no descriptor is specified.
    * @param message Message printed out before the output is dumped.
    */
   void dump (FILE* dest = stderr,
              const char* message = "\n------ Mutex Dump -----\n") const
   {
      fprintf(dest, "%s", message);
      fprintf(dest, "This is not currently implemented ...\n");
      // More needed ...
   }

   // Allow the vpr::CondNSPR class to access the private and protected
   // members of this class.  Specifically, direct access is needed to the
   // mutex variable.
   friend class CondVarNSPR;

protected:
   PRLock* mMutex;     /**<  Mutex variable for the class. */
   int     mLocked;    /**< */

   // = Prevent assignment and initialization.
   MutexNSPR& operator= (const MutexNSPR& r)
   {
      mMutex = r.mMutex;
      mLocked = r.mLocked;
      return *this;
   }

   MutexNSPR (const MutexNSPR &)
   {
      /* Do nothing. */ ;
   }
};

} // End of vpr namespace


#endif   /* ifdef _VPR_MUTEX_NSPR_H_ */
