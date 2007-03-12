/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 ****************** <VPR heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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

#include <stdio.h>
#include <semaphore.h>
#include <errno.h>
#include <assert.h>
#include <boost/concept_check.hpp>
#include <boost/noncopyable.hpp>

#include <vpr/Sync/DeadlockException.h>

#if defined(VPR_OS_Darwin) && VPR_OS_RELEASE_MAJOR <= 8
#define VPR_USE_NAMED_SEMAPHORE 1
#endif


namespace vpr
{

/** \class SemaphorePosix SemaphorePosix.h vpr/Sync/Semaphore.h
 *
 * Semaphore wrapper for POSIX.4-compliant systems.  This is typedef'd to
 * vpr::Semaphore.
 */
class SemaphorePosix : boost::noncopyable
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
    *
    * @throw vpr::ResourceException is thrown if the semaphore cannot be
    *        allocated.
    */
   SemaphorePosix(const int initialValue = 1);

   /**
    * Destructor for vpr::SemaphorePosix class.
    *
    * @post The resources used by the semaphore variable are freed.
    */
   ~SemaphorePosix();

   /**
    * Locks this semaphore.
    *
    * @post The calling thread either acquires the semaphore until
    *       release() is called, or the caller is put at the tail of a wait
    *       and is suspended until such time as it can be freed and allowed
    *       to acquire the semaphore itself.
    *
    * @throw vpr::DeadlockException is thrown if the current thread has
    *        already locked this semaphore.
    */
   void acquire()
   {
      int result;

      do
      {
         result = sem_wait(mSema);
      }
      while ( -1 == result && EINTR == errno );

      if ( -1 == result && EDEADLK == errno )
      {
         throw vpr::DeadlockException(
            "Tried to lock semaphore twice in the same thread", VPR_LOCATION
         );
      }

      assert(result == 0);
      boost::ignore_unused_variable_warning(result);
   }

   /**
    * Acquires a read lock on this semaphore.
    *
    * @post The calling thread either acquires the semaphore until
    *       release() is called, or the caller is put at the tail of a wait
    *       and is suspended until such time as it can be freed and allowed
    *       to acquire the semaphore itself.
    *
    * @note There is no special read lock for now.
    */
   void acquireRead()
   {
      this->acquire();
   }

   /**
    * Acquires a write lock on this semaphore.
    *
    * @post The calling thread either acquires the semaphore until
    *       release() is called, or the caller is put at the tail of a wait
    *       and is suspended until such time as it can be freed and allowed
    *       to acquire the semaphore itself.
    *
    * @note There is no special write lock for now.
    */
   void acquireWrite()
   {
      this->acquire();
   }

   /**
    * Tries to acquire the semaphore immediately (does not block).
    *
    * @post If the semaphore could be acquired by the caller, the caller
    *       gets control of the semaphore.  If the semaphore was already
    *       locked, the routine returns immediately without suspending the
    *       calling thread.
    *
    * @return \c true is returned if the lock is acquired, and \c false is
    *         returned if the semaphore is already locked.
    *
    * @throw vpr::DeadlockException is thrown if the current thread has
    *        already locked this semaphore.
    */
   bool tryAcquire()
   {
      int result;

      do
      {
         result = sem_trywait(mSema);
      }
      while ( -1 == result && EINTR == errno );

      if ( -1 == result && EDEADLK == errno )
      {
         throw vpr::DeadlockException(
            "Tried to lock semaphore twice in the same thread", VPR_LOCATION
         );
      }

#if defined(_DEBUG)
      // If we are in an error state other than EAGAIN, something is very
      // wrong.
      if ( -1 == result && EAGAIN != errno )
      {
         assert(false);
      }
#endif

      return result == 0;
   }

   /**
    * Tries to acquire a read lock on this semaphore (does not block).
    *
    * @post If the semaphore could be acquired by the caller, the caller
    *       gets control of the semaphore.  If the semaphore was already
    *       locked, the routine returns immediately without suspending the
    *       calling thread.
    *
    * @return \c true is returned if the lock is acquired, and \c false is
    *         returned if the semaphore is already locked.
    */
   bool tryAcquireRead()
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
    * @return \c true is returned if the lock is acquired, and \c false is
    *         returned if the semaphore is already locked.
    */
   bool tryAcquireWrite()
   {
      return this->tryAcquire();
   }

   /**
    * Releases this semaphore.
    *
    * @pre The semaphore should have been locked before being released.
    * @post The semaphore is released and the thread at the haed of the
    *       wait queue is allowed to execute again.
    */
   void release()
   {
      const int result = sem_post(mSema);
      assert(result == 0);
      boost::ignore_unused_variable_warning(result);
   }

   /**
    * Resets the semaphore using the given value.
    *
    * @post The semaphore's count is set to the specified value.
    *
    * @param val The value to which the semaphore is reset.
    *
    * @throw vpr::ResourceException is thrown if the semaphore cannot be
    *        reset.
    *
    * @note If processes are waiting on the semaphore, the results are
    *       undefined.
    */
   void reset(const int val);

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
};

} // End of vpr namespace

#undef VPR_USE_NAMED_SEMAPHORE

#endif  /* _VPR_SEMAPHORE_POSIX_H_ */
