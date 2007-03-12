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

/**
 * \file
 *
 * @note This file must be included by vpr/Sync/Semaphore.h, not the other way
 *       around.
 */

#ifndef _VPR_SEMAPHORE_POSIX_H_
#define _VPR_SEMAPHORE_POSIX_H_

#include <vpr/vprConfig.h>

#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <semaphore.h>
#include <errno.h>
#include <stdio.h>

#include <vpr/Util/ReturnStatus.h>

#if defined(VPR_OS_Darwin) && VPR_OS_RELEASE_MAJOR <= 8
#define VPR_USE_NAMED_SEMAPHORE 1
#endif

#ifdef VPR_USE_NAMED_SEMAPHORE
#include <string.h>
#endif

namespace vpr
{

/** \class SemaphorePosix SemaphorePosix.h vpr/Sync/Semaphore.h
 *
 * Semaphore wrapper for POSIX.4-compliant systems.  This is typedef'd to
 * vpr::Semaphore.
 */
class SemaphorePosix
{
public:
   /**
    * Constructor for vpr::SemaphorePosix class.
    *
    * @post The semaphore variable for this object is initialized as an
    *       unnamed semaphore on all platforms except Darwin (Mac OS X).
    *       On Darwin 7.5 and earlier, unnamed semaphores are not
    *       supported.
    *
    * @param initialValue The initial number of resources controlled by
    *                     the semaphore.  If not specified, the default
    *                     value is 1.
    */
   SemaphorePosix(int initialValue = 1)
   {
#ifdef VPR_USE_NAMED_SEMAPHORE
      // Use strdup(3) here so that mktemp(3) can modify the memory.  Trying
      // to modify a constant string would be bad.
      // NOTE: The memory allocated by strdup(3) will be released in the
      // destructor.
      mSemaFile = mktemp(strdup("/tmp/vprsema.XXXXXX"));

      // ----- Allocate the named semaphore ----- //
      mSema = sem_open(mSemaFile, O_CREAT, 0600, initialValue);

      if ( mSema == (sem_t*) SEM_FAILED )
      {
         perror("sem_init() error");
      }
#else
      // ----- Allocate the unnamed semaphore ----- //
      mSema = (sem_t*) malloc(sizeof(sem_t));

      if ( sem_init(mSema, 0, initialValue) != 0 )
      {
         perror("sem_init() error");
      }
#endif
   }

   /**
    * Destructor for vpr::SemaphorePosix class.
    *
    * @post The resources used by the semaphore variable are freed.
    */
   ~SemaphorePosix()
   {
      // ---- Delete the semaphore --- //
#ifdef VPR_USE_NAMED_SEMAPHORE
      if ( sem_close(mSema) != 0 )
      {
         perror("sem_close() error");
      }

      sem_unlink(mSemaFile);
      free(mSemaFile);
#else
      if ( sem_destroy(mSema) != 0 )
      {
         perror("sem_destroy() error");
      }

      free(mSema);
#endif
   }

   /**
    * Locks this semaphore.
    *
    * @post The calling thread either acquires the semaphore until
    *       release() is called, or the caller is put at the tail of a wait
    *       and is suspended until such time as it can be freed and allowed
    *       to acquire the semaphore itself.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the lock is acquired.
    * @return vpr::ReturnStatus::Fail is returnd if an error occurred.
    */
   vpr::ReturnStatus acquire() const
   {
      int result;

      do
      {
         result = sem_wait(mSema);
      }
      while ( result == -1 && errno == EINTR );

      if ( result == 0 )
      {
         return vpr::ReturnStatus();
      }
      else
      {
         perror("sem_wait() error");
         return vpr::ReturnStatus(vpr::ReturnStatus::Fail);
      }
   }

   /**
    * Acquires a read lock on this semaphore.
    *
    * @post The calling thread either acquires the semaphore until
    *       release() is called, or the caller is put at the tail of a wait
    *       and is suspended until such time as it can be freed and allowed
    *       to acquire the semaphore itself.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the read lock is
    *         acquired.
    * @return vpr::ReturnStatus::Fail is returnd if an error occurred.
    *
    * @note There is no special read lock for now.
    */
   vpr::ReturnStatus acquireRead() const
   {
      return this->acquire();
   }

   /**
    * Acquires a write lock on this semaphore.
    *
    * @post The calling thread either acquires the semaphore until
    *       release() is called, or the caller is put at the tail of a wait
    *       and is suspended until such time as it can be freed and allowed
    *       to acquire the semaphore itself.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the write lock is
    *         acquired.
    * @return vpr::ReturnStatus::Fail is returnd if an error occurred.
    *
    * @note There is no special write lock for now.
    */
   vpr::ReturnStatus acquireWrite() const
   {
      return this->acquire();
   }

   /**
    * Tries to acquire the semaphore immediately (does not block).
    *
    * @post If the semaphore could be acquired by the caller, the caller
    *       gets control of the semaphore.  If the semaphore was already
    *       locked, the routine returns immediately without suspending the
    *       calling thread.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the lock on this
    *         semaphore is acquired.
    * @return vpr::ReturnStatus::Fail is returned if the lock is not acquired.
    */
   vpr::ReturnStatus tryAcquire() const
   {
      if ( sem_trywait(mSema) == 0 )
      {
         return vpr::ReturnStatus();
      }
      else
      {
         return vpr::ReturnStatus(vpr::ReturnStatus::Fail);
      }
   }

   /**
    * Tries to acquire a read lock on this semaphore (does not block).
    *
    * @post If the semaphore could be acquired by the caller, the caller
    *       gets control of the semaphore.  If the semaphore was already
    *       locked, the routine returns immediately without suspending the
    *       calling thread.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the lock on this
    *         semaphore is acquired.
    * @return vpr::ReturnStatus::Fail is returned if the lock is not acquired.
    */
   vpr::ReturnStatus tryAcquireRead() const
   {
      return this->tryAcquire();
   }

   /**
    * Tries to acquire a write lock on this semaphore (does not block).
    *
    * @post If the semaphore could be acquired by the caller, the caller
    *       gets control of the semaphore.  If the semaphore was already
    *       locked, the routine returns immediately without suspending the
    *       calling thread.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the lock on this
    *         semaphore is acquired.
    * @return vpr::ReturnStatus::Fail is returned if the lock is not acquired.
    */
   vpr::ReturnStatus tryAcquireWrite() const
   {
      return this->tryAcquire();
   }

   /**
    * Releases this semaphore.
    *
    * @pre The semaphore should have been locked before being released.
    * @post The semaphore is released and the thread at the haed of the
    *       wait queue is allowed to execute again.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the lock on this
    *         semaphore is released.
    * @return vpr::ReturnStatus::Fail is returned if an error occurred.
    */
   vpr::ReturnStatus release() const
   {
      if ( sem_post(mSema) == 0 )
      {
         return vpr::ReturnStatus();
      }
      else
      {
         return vpr::ReturnStatus(vpr::ReturnStatus::Fail);
      }
   }

   /**
    * Resets the semaphore using the given value.
    *
    * @post The semaphore's count is set to the specified value.
    *
    * @param val The value to which the semaphore is reset.
    *
    * @return vpr::ReturnStatus::Succeed is returned if this semaphore is
    *         reset with the given value successfully.
    * @return vpr::ReturnStatus::Fail is returned if this semaphore could not
    *         be reset.
    *
    * @note If processes are waiting on the semaphore, the results are
    *       undefined.
    */
   vpr::ReturnStatus reset(int val)
   {
      vpr::ReturnStatus status;

#ifdef VPR_USE_NAMED_SEMAPHORE
      // First destroy the current semaphore.
      sem_unlink(mSemaFile);

      // Now recreate it with the new value in val.
      mSema = sem_open(mSemaFile, O_CREAT, 0600, val);

      if ( mSema == (sem_t*) SEM_FAILED )
      {
         perror("sem_init() error");
         status.setCode(vpr::ReturnStatus::Fail);
      }
#else
      // First destroy the current semaphore.
      sem_destroy(mSema);

      // Now recreate it with the new value in val.
      if ( sem_init(mSema, 0, val) != 0 )
      {
         status.setCode(vpr::ReturnStatus::Fail);
      }
#endif

      return status;
   }

   /**
    * Dumps the semaphore debug stuff and current state.
    *
    * @post All important data and debugging information related to the
    *       semaphore is dumped to the specified file descriptor (or to
    *       stderr if none is given).
    *
    * @param dest    File descriptor to which the output will be written.
    *                It defaults to stderr if no descriptor is specified.
    * @param message Message printed out before the output is dumped.
    */
   void dump(FILE* dest = stderr,
             const char* message = "\n------ Semaphore Dump -----\n") const
   {
      int value;
      sem_getvalue(mSema, &value);
      fprintf(dest, "%s", message);
      fprintf(dest, "Current semaphore value: %d", value);
   }

protected:
#ifdef VPR_USE_NAMED_SEMAPHORE
   char* mSemaFile;
#endif
   sem_t* mSema;   /**< Semaphore variable for the class. */

   // Prevent assignment and initialization.
   void operator=(const SemaphorePosix &)
   {
      /* Do nothing. */ ;
   }

   SemaphorePosix(const SemaphorePosix &)
   {
      /* Do nothing. */ ;
   }
};

} // End of vpr namespace

#undef VPR_USE_NAMED_SEMAPHORE

#endif  /* _VPR_SEMAPHORE_POSIX_H_ */
