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

#ifndef _VPR_MutexSGI_h_
#define _VPR_MutexSGI_h_

#include <vpr/vprConfig.h>
#include <string.h>
#include <ulocks.h>
#include <errno.h>
#include <vpr/md/SPROC/SharedMem/MemPool.h>
#include <vpr/Util/Assert.h>
#include <vpr/Util/ReturnStatus.h>


namespace vpr
{

/** \class MutexSGI MutexSGI.h vpr/Sync/Mutex.h
 *
 * Mutex wrapper for the SGI systems.  Used for critical section protection.
 * This is typedef'd to vpr::Mutex.
 *
 * @date January 20, 1997
 */
class MutexSGI
{
public:
   MutexSGI() : mMutex(NULL)
   {
      // BUG: Possible race condition here
      if ( mMutexPool == NULL )
      {
         mMutexPool = new MemPoolSGI(65536, 32,
                                     "/var/tmp/memMutexPoolSGIXXXXXX");
         mAttachedCounter = static_cast<int*>(mMutexPool->allocate(sizeof(int)));
         *mAttachedCounter = 0;
      }

      // Track how many mutexes are allocated
      *mAttachedCounter = *mAttachedCounter + 1;
//      vprDEBUG << " vpr::MutexSGI::MutexSGI: mAttachedCounter: "
//               << *mAttachedCounter << endl << vprDEBUG_FLUSH;

      // ----- Allocate the mutex ----- //
      mMutex = usnewlock(mMutexPool->getArena());

      if ( NULL == mMutex )
      {
         std::cerr << "ERROR: Failed to allocate new mutex -- "
                   << strerror(errno) << std::endl;
         vprASSERT(mMutex != NULL && "in vpr::MutexSGI::MutexSGI() mMutex is NULL");
      }
   }

   ~MutexSGI()
   {
      // ---- Delete the mutex --- //
      usfreelock(mMutex, mMutexPool->getArena());

      // ---- Deal with the pool --- //

      // Track how many mutexes are allocated
      *mAttachedCounter = *mAttachedCounter - 1;

//      vprDEBUG << "vpr::MutexSGI::~MutexSGI: mAttachedCounter: "
//               << *mAttachedCounter << endl << vprDEBUG_FLUSH;

      if ( *mAttachedCounter == 0 )
      {
         mMutexPool->deallocate(mAttachedCounter);
         mAttachedCounter = NULL;
         delete mMutexPool;
         mMutexPool = NULL;
      }
   }

   /**
    * Locks this mutex.
    *
    * @post A lock on this mutex is acquired by the caller.  If a lock has
    *       already been acquired by another process/thread, the caller blocks
    *       until the mutex has been freed.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the lock is acquired
    *         successfully.  vpr::ReturnStatus::Fail is returned otherwise.
    */
   vpr::ReturnStatus acquire() const
   {
      vprASSERT(mMutex != NULL && "in vpr::MutexSGI::aquire() mMutex is NULL");
      if ( ussetlock(mMutex) == 1 )
      {
         return vpr::ReturnStatus();
      }
      else
      {
         return vpr::ReturnStatus(vpr::ReturnStatus::Fail);
      }
   }

   /**
    * Acquires a read lock.
    *
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
   vpr::ReturnStatus acquireRead() const
   {
      return this->acquire();      // No special "read" semaphore -- For now
   }

   /**
    * Acquires a write lock.
    *
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
   vpr::ReturnStatus acquireWrite() const
   {
      return this->acquire();      // No special "write" semaphore -- For now
   }

   /**
    * Tries to acquire a lock on this mutex (does not block).
    *
    * @post A lock on this mutex is acquired by the caller.  If a lock has
    *       already been acquired by another process/thread, the caller
    *       returns does not wait for it to be unlocked.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the lock is acquired.
    *         vpr::ReturnStatus::Fail is returned if another thread is
    *         holding the lock already.
    */
   vpr::ReturnStatus tryAcquire() const
   {
      // Try 100 spins.
      if ( uscsetlock(mMutex, 100) == 1 )
      {
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
    * @post A lock on this mutex is acquired by the caller.  If a lock has
    *       already been acquired by another process/thread, the caller
    *       returns does not wait for it to be unlocked.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the read lock is
    *         acquired.  vpr::ReturnStatus::Fail is returned if another thread
    *         is holding the lock already.
    */
   vpr::ReturnStatus tryAcquireRead() const
   {
      return this->tryAcquire();
   }

   /**
    * Tries to acquire a write lock (does not block).
    *
    * @post A lock on this mutex is acquired by the caller.  If a lock has
    *       already been acquired by another process/thread, the caller returns
    *       does not wait for it to be unlocked.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the write lock is
    *         acquired.  vpr::ReturnStatus::Fail is returned if another thread
    *         is holding the lock already.
    */
   vpr::ReturnStatus tryAcquireWrite() const
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
   vpr::ReturnStatus release() const
   {
      if ( usunsetlock(mMutex) == 0 )
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
    * @post The state of this mutex is returned.
    *
    * @return 0 is returned if this mutex is not locked.  1 is returned if it
    *         is locked.
    */
   int test() const
   {
      return ustestlock(const_cast<ulock_t>(mMutex));
   }

   /**
    * Dumps the mutex debug stuff and current state.
    *
    * @post All important data and debugging information related to the
    *       mutex are dumped to the specified file descriptor (or to stderr
    *       if none is given).
    *
    * @param dest    File descriptor to which the output will be written.
    *                It defaults to stderr if no descriptor is specified.
    * @param message Message printed out before the output is dumped.
    */
   void dump(FILE* dest = stdout,
             const char* message = "\n------ Mutex Status -----\n") const
   {
      usdumplock(mMutex, dest, message);
   }

protected:
   ulock_t mMutex;

   // = Prevent assignment and initialization.
   void operator= (const MutexSGI &) {;}
   MutexSGI (const MutexSGI &) {;}

   static MemPoolSGI* mMutexPool;
   static int* mAttachedCounter;
};

} // End of vpr namespace


#endif /* _VPR_MutexSGI_h_ */
