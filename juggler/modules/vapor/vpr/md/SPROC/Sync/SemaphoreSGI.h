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

#ifndef _VPR_SemaphoreSGI_h_
#define _VPR_SemaphoreSGI_h_

#include <vpr/vprConfig.h>
#include <ulocks.h>

#include <vpr/md/SPROC/SharedMem/MemPool.h>
#include <vpr/Util/ReturnStatus.h>


namespace vpr
{

/**
 * Semaphore wrapper for the SGI systems
 * This class encapsulates the behavior of a semaphore variable.
 *
 * @date 1-20-1997
 */
class SemaphoreSGI
{
public:
   /**
    * Constructor.
    *
    * @pre None.
    * @post The semaphore variable for the class is initilized as an
    *       unnamed semaphore.
    *
    * @param initialValue The initial number of resources controlled by the
    *                     semaphore.  If not specified, the default value is 1.
    */
   SemaphoreSGI (int initialValue = 1)
   {
      // BUG:
      if (semaphorePool == NULL)
      {
         semaphorePool = new MemPoolSGI(65536, 32,
                                        "/var/tmp/memSemaphorePoolSGIXXXXXX");
         attachedCounter = static_cast<int*>(semaphorePool->allocate(sizeof(int)));
         *attachedCounter = 0;
      }
      *attachedCounter = *attachedCounter + 1;      // Track how many semaphores are allocated

//      DebugLock.acquire();
//      vprDEBUG << " vpr::SemaphoreSGI::SemaphoreSGI: attachedCounter: "
//               << *attachedCounter << endl << vprDEBUG_FLUSH;
//      DebugLock.release();

      // ----- Allocate the semaphore ----- //
      sema = usnewsema(semaphorePool->getArena(), initialValue);
   }

   ~SemaphoreSGI(void)
   {
      // ---- Delete the semaphore --- //
      usfreesema(sema, semaphorePool->getArena());

      // ---- Deal with the pool --- //
      *attachedCounter = *attachedCounter - 1;     // Track how many Semaphore are allocated

//      DebugLock.acquire();
//      vprDEBUG << "vpr::SemaphoreSGI::~SemaphoreSGI: attachedCounter: "
//               << *attachedCounter << endl << vprDEBUG_FLUSH;
//      DebugLock.release();

      if (*attachedCounter == 0)
      {
         semaphorePool->deallocate(attachedCounter);
         attachedCounter = NULL;
         delete semaphorePool;
         semaphorePool = NULL;
      }
   }

   /**
    * Locks this semaphore.
    *
    * @pre None.
    * @post The calling thread either acquires the semaphore until release()
    *       is called, or the caller is put at the tail of a wait and is
    *       suspended until such time as it can be freed and allowed to acquire
    *       the semaphore itself.
    *
    * @return vpr::ReturnStatus::Succeed is returned if a resource lock is
    *         acquired.  vpr::ReturnStatus::Fail is returned otherwise.
    */
   vpr::ReturnStatus acquire() const
   {
      vpr::ReturnStatus status;

      if ( uspsema(sema) < 0 )
      {
         std::cerr << "vpr::SemphoreSGI::ERROR:" << std::endl;
         status.setCode(vpr::ReturnStatus::Fail);
      }

      return status;
   }

   /**
    * Acquires a read lock on a resource protected by this semaphore.
    *
    * @pre None.
    * @post The calling thread either acquires the semaphore until release()
    *       is called, or the caller is put at the tail of a wait and is
    *       suspended until such time as it can be freed and allowed to acquire
    *       the semaphore itself.
    *
    * @return vpr::ReturnStatus::Succeed is returned if a resource read lock is
    *         acquired.  vpr::ReturnStatus::Fail is returned otherwise.
    *
    * @note There is no special read semaphore for now.
    */
   vpr::ReturnStatus acquireRead() const
   {
      return this->acquire();     // No special "read" semaphore -- For now
   }

   /**
    * Acquires a write lock on a resource protected by this semaphore.
    *
    * @pre None.
    * @post The calling thread either acquires the semaphore until release()
    *       is called, or the caller is put at the tail of a wait and is
    *       suspended until such time as it can be freed and allowed to acquire
    *       the semaphore itself.
    *
    * @return vpr::ReturnStatus::Succeed is returned if a resource write lock
    *         is acquired.  vpr::ReturnStatus::Fail is returned otherwise.
    *
    * @note There is no special write semaphore for now.
    */
   vpr::ReturnStatus acquireWrite() const
   {
      return this->acquire();     // No special "write" semaphore -- For now
   }

   /**
    * Tries to acquire the a resource lock immediately (does not block).
    *
    * @pre None.
    * @post If the semaphore could be acquired by the caller, the caller
    *       gets control of the semaphore.  If the semaphore was already
    *       locked, the routine returns immediately without suspending the
    *       calling thread.
    *
    * @return vpr::ReturnStatus::Succeed is returned if a lock is acquired.
    *         vpr::ReturnStatus::Fail is returned if no resource could be
    *         locked without blocking.
    */
   vpr::ReturnStatus tryAcquire () const
   {
      if ( uscpsema(sema) == 1 )
      {
         return vpr::ReturnStatus();
      }
      else
      {
         return vpr::ReturnStatus(vpr::ReturnStatus::Fail);
      }
   }

   /**
    * Tries to acquire a read lock on a resource (does not block).
    *
    * @pre None.
    * @post If the semaphore could be acquired by the caller, the caller
    *       gets control of the semaphore.  If the semaphore was already
    *       locked, the routine returns immediately without suspending the
    *       calling thread.
    *
    * @return vpr::ReturnStatus::Succeed is returned if a read lock is
    *         acquired.  vpr::ReturnStatus::Fail is returned if no resource
    *         could be locked without blocking.
    */
   vpr::ReturnStatus tryAcquireRead () const
   {
      return this->tryAcquire();
   }

   /**
    * Tries to acquire a write lock on a resource (does not block).
    *
    * @pre None.
    * @post If the semaphore could be acquired by the caller, the caller
    *       gets control of the semaphore.  If the semaphore was already
    *       locked, the routine returns immediately without suspending the
    *       calling thread.
    *
    * @return vpr::ReturnStatus::Succeed is returned if a write lock is
    *         acquired.  vpr::ReturnStatus::Fail is returned if no resource
    *         could be locked without blocking.
    */
   vpr::ReturnStatus tryAcquireWrite () const
   {
      return this->tryAcquire();
   }

   /**
    * Releases a resource lock.
    *
    * @pre The semaphore should have been locked before being released.
    * @post The semaphore is released and the thread at the head of the
    *       wait queue is allowed to execute again.
    *
    * @return vpr::ReturnStatus::Succeed is returned if a resource is
    *         unlocked successfully.  vpr::ReturnStatus::Fail is returned
    *         otherwise.
    */
   vpr::ReturnStatus release() const
   {
      vpr::ReturnStatus status;

      if( usvsema(sema) < 0 )
      {
         std::cerr << "vpr::SemaphoreSGI::ERROR:" << std::endl;
         status.setCode(vpr::ReturnStatus::Fail);
      }

      return status;
   }

   /**
    * Resets the resource count for this semaphore.
    *
    * @pre None.
    * @post This semaphore's count is set to the specified value.
    *
    * @param val - The value to which the semaphore is reset.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the resource count is
    *         reset successfully.  vpr::ReturnStatus::Fail is returned
    *         otherwise.
    *
    * @note If processes are waiting on the semaphore, the results are
    *       undefined.
    */
   vpr::ReturnStatus reset(int val)
   {
      if ( usinitsema(sema, val) == 0 )
      {
         return vpr::ReturnStatus();
      }
      else
      {
         return vpr::ReturnStatus(vpr::ReturnStatus::Fail);
      }
   }

   /**
    * Dumps the semaphore debug stuff and current state.
    *
    * @pre None.
    * @post All important data and debugging information related to this
    *       semaphore is dumped to the specified file descriptor (or to
    *       stderr if none is given).
    *
    * @param dest    File descriptor to which the output will be written.
    *                It defaults to stderr if no descriptor is specified.
    * @param message Message printed out before the output is dumped.
    */
   void dump (FILE* dest = stderr,
              const char* message = "\n------ Semaphore Dump -----\n") const
   {
      usdumpsema(sema, dest, message);
   }

protected:
   usema_t* sema;

   // = Prevent assignment and initialization.
   void operator= (const SemaphoreSGI &) {;}
   SemaphoreSGI (const SemaphoreSGI &) {}

   // Problem here.  Fork will not like these.
   static MemPoolSGI* semaphorePool;
   static int* attachedCounter;
};

} // End of vpr namespace


#endif /* _VPR_SemaphoreSGI_h_ */
