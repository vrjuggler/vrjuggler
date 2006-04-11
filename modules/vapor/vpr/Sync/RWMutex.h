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

#ifndef _VPR_RW_MUTEX_H_
#define _VPR_RW_MUTEX_H_

#include <vpr/vprConfig.h>
#include <vpr/Sync/Semaphore.h>
#include <vpr/Sync/CondVar.h>
#include <vpr/Sync/Mutex.h>

#if VPR_THREAD_DOMAIN_INCLUDE == VPR_DOMAIN_NSPR
#  include <vpr/md/NSPR/Sync/RWMutexNSPR.h>
#elif (VPR_THREAD_DOMAIN_INCLUDE == VPR_DOMAIN_POSIX) || \
      (VPR_THREAD_DOMAIN_INCLUDE == VPR_DOMAIN_IRIX_SPROC)

namespace vpr
{

/**
 * vpr::RWMutex wrapper.
 *
 * @date 1-31-97
 */
class VPR_CLASS_API RWMutex
{
public:
   RWMutex ()
      : waitingReaders(&stateLock), numWaitingReaders(0),
        waitingWriters(&stateLock), numWaitingWriters(0), refCount(0)
   {
      /* Do nothing. */ ;
   }

   ~RWMutex (void)
   {
      /* Do nothing. */ ;
   }

   /**
    * Locks the mutex.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the mutex is acquired.
    *         vpr::ReturnStatus::Fail is returned upon error.
    */
   vpr::ReturnStatus acquire()
   {
      return acquireWrite();
   }

   /// Acquires a read mutex.
   vpr::ReturnStatus acquireRead(void);

   /// Acquires a write mutex.
   vpr::ReturnStatus acquireWrite(void);

   /**
    * Tries to acquire the mutex.
    * Wait until the semaphore value is greater than 0.
    * Then decrement by 1 and return.
    * P operation.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the mutex is acquired.
    *         vpr::ReturnStatus::Fail is returned if the mutex is not
    *         acquired.
    */
   vpr::ReturnStatus tryAcquire ()
   {
      return tryAcquireWrite();
   }

   /// Tries to acquire a read mutex.
   vpr::ReturnStatus tryAcquireRead (void);

   /// Tries to acquire a write mutex.
   vpr::ReturnStatus tryAcquireWrite (void);

   /**
    * Releases the mutex.
    *
    * @return vpr::ReturnStatus::Succeed is returned on success;
    *         vpr::ReturnStatus::Fail on error.
    */
   vpr::ReturnStatus release(void);

   /**
    *Tests the current lock status.
    *
    * @return 0 is returned to indicate that the mutex is not locked.<br>
    *         1 is returned when the mutex is locked.
    */
   int test()
   {
      return stateLock.test();
   }

   /// Dumps the mutex debug stuff and current state.
   void dump (FILE* dest = stderr,
              const char* message = "\n------ Mutex Dump -----\n") const
   {
      stateLock.dump();
   }

protected:
   Mutex stateLock;        /**< Serialize access to internal state */
   CondVar waitingReaders; /**< Reader threads waiting to acquire the lock */
   int numWaitingReaders;  /**< Number of waiting readers */

   CondVar waitingWriters; /**< Writer threads waiting to acquire the lock */
   int numWaitingWriters;  /**< Number of waiting writers */

   /**
    * Value is -1 if writer has the lock, else this keeps track of the
    * number of readers holding the lock.
    */
   int refCount;

   // = Prevent assignment and initialization.
   void operator= (const RWMutex &) {}
   RWMutex (const RWMutex &) {}
};

} // End of vpr namespace

#endif  /* ! VPR_USE_NSPR */


#endif /* _VPR_RW_MUTEX_H_ */
