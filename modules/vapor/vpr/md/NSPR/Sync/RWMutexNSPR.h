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
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

#ifndef _VPR_RW_MUTEX_NSPR_H_
#define _VPR_RW_MUTEX_NSPR_H_

#include <vpr/vprConfig.h>

#include <stdio.h>
#include <prrwlock.h>

#include <vpr/Util/ReturnStatus.h>


namespace vpr
{

/** \class RWMutexNSPR RWMutexNSPR.h vpr/Sync/RWMutex.h
 *
 * Read/write mutex implementation using NSPR read/write mutexes (PRRWLock).
 * This is typedef'd to vpr::RWMutex.
 */
class VPR_CLASS_API RWMutexNSPR
{
public:
   RWMutexNSPR() : mRwLock(NULL)
   {
      // Note the second argument "VPR RW Mutex" is for debug purposes only
      mRwLock = PR_NewRWLock(0, "VPR RW Mutex");
   }

   ~RWMutexNSPR()
   {
      PR_DestroyRWLock(mRwLock);
   }

   /**
    * Locks the mutex.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the mutex is acquired.
    * @return vpr::ReturnStatus::Fail is returned upon error.
    */
   vpr::ReturnStatus acquire()
   {
      return acquireWrite();
   }

   /**
    * Acquires a read mutex.
    */
   vpr::ReturnStatus acquireRead()
   {
      PR_RWLock_Rlock(mRwLock);
      return vpr::ReturnStatus();
   }

   /**
    * Acquires a write mutex.
    */
   vpr::ReturnStatus acquireWrite()
   {
      PR_RWLock_Wlock(mRwLock);
      return vpr::ReturnStatus();
   }

   /**
    * Tries to acquire the mutex.
    * Wait until the semaphore value is greater than 0.
    * Then decrement by 1 and return.
    * P operation.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the mutex is acquired.
    * @return vpr::ReturnStatus::Fail is returned if the mutex is not
    *         acquired.
    */
   vpr::ReturnStatus tryAcquire()
   {
      return tryAcquireWrite();
   }

   /**
    * Tries to acquire a read mutex.
    */
   vpr::ReturnStatus tryAcquireRead();

   /**
    * Tries to acquire a write mutex.
    */
   vpr::ReturnStatus tryAcquireWrite();

   /**
    * Releases the mutex.
    *
    * @return vpr::ReturnStatus::Succeed is returned on success.
    * @return vpr::ReturnStatus::Fail is returned on error.
    */
   vpr::ReturnStatus release()
   {
      PR_RWLock_Unlock(mRwLock);
      return vpr::ReturnStatus();
   }

   /**
    * Tests the current lock status.
    *
    * @return 0 is returned if this mutex is not locked.
    * @return 1 is returned if this mutex is locked.
    */
/*
    int test()
    {
        return stateLock.test();
    }
*/

protected:
   int numWaitingReaders;    /**< Number of waiting readers. */
   int numWaitingWriters;    /**< Number of waiting writers. */

   /**
    * Value is -1 if writer has the lock, else this keeps track of the
    * number of readers holding the lock.
    */
   int mRefCount;

   PRRWLock* mRwLock;

   // = Prevent assignment and initialization.
   void operator= (const RWMutexNSPR &)
   {
      /* Do nothing. */ ;
   }

   RWMutexNSPR(const RWMutexNSPR &)
   {
      /* Do nothing. */ ;
   }
};

} // End of vpr namespace


#endif  /* _VPR_RW_MUTEX_NSPR_H_ */
