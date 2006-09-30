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
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

#include <assert.h>
#include <ulocks.h>
#include <boost/noncopyable.hpp>

#include <vpr/Sync/LockException.h>


namespace vpr
{

class MemPoolSGI;

/** \class SemaphoreSGI SemaphoreSGI.h vpr/Sync/Semaphore.h
 *
 * Semaphore wrapper for the SGI systems.
 * This class encapsulates the behavior of a semaphore variable.
 * This is typedef'd to vpr::Semaphore.
 *
 * @date January 20, 1997
 */
class SemaphoreSGI : boost::noncopyable
{
public:
   /**
    * Constructor.
    *
    * @post The semaphore variable for the class is initilized as an
    *       unnamed semaphore.
    *
    * @param initialValue The initial number of resources controlled by the
    *                     semaphore.  If not specified, the default value is 1.
    *
    * @throw vpr::ResourceException is thrown if the semaphore cannot be
    *        allocated.
    */
   SemaphoreSGI(const int initialValue = 1);

   ~SemaphoreSGI();

   /**
    * Locks this semaphore.
    *
    * @post The calling thread either acquires the semaphore until release()
    *       is called, or the caller is put at the tail of a wait and is
    *       suspended until such time as it can be freed and allowed to acquire
    *       the semaphore itself.
    *
    * @throw vpr::LockException is thrown if the current thread has already
    *        locked this semaphore.
    */
   void acquire()
   {
      const int result = uspsema(mSema);

      if ( result == -1 && ERANGE == errno )
      {
         throw vpr::LockException("Semaphore queue overflowed", VPR_LOCATION);
      }

      assert(result == 0);
   }

   /**
    * Acquires a read lock on a resource protected by this semaphore.
    *
    * @post The calling thread either acquires the semaphore until release()
    *       is called, or the caller is put at the tail of a wait and is
    *       suspended until such time as it can be freed and allowed to acquire
    *       the semaphore itself.
    *
    * @note There is no special read semaphore for now.
    */
   void acquireRead()
   {
      this->acquire();     // No special "read" semaphore -- For now
   }

   /**
    * Acquires a write lock on a resource protected by this semaphore.
    *
    * @post The calling thread either acquires the semaphore until release()
    *       is called, or the caller is put at the tail of a wait and is
    *       suspended until such time as it can be freed and allowed to acquire
    *       the semaphore itself.
    *
    * @note There is no special write semaphore for now.
    */
   void acquireWrite()
   {
      return this->acquire();     // No special "write" semaphore -- For now
   }

   /**
    * Tries to acquire the a resource lock immediately (does not block).
    *
    * @post If the semaphore could be acquired by the caller, the caller
    *       gets control of the semaphore.  If the semaphore was already
    *       locked, the routine returns immediately without suspending the
    *       calling thread.
    *
    * @return \c true is returned if the lock is acquired, and \c false is
    *         returned if the semaphore is already locked.
    */
   bool tryAcquire()
   {
      return uscpsema(mSema) == 1;
   }

   /**
    * Tries to acquire a read lock on a resource (does not block).
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
    * Tries to acquire a write lock on a resource (does not block).
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
    * Releases a resource lock.
    *
    * @pre The semaphore should have been locked before being released.
    * @post The semaphore is released and the thread at the head of the
    *       wait queue is allowed to execute again.
    */
   void release()
   {
      const int result = usvsema(mSema);
      assert(result == 0);
   }

   /**
    * Resets the resource count for this semaphore.
    *
    * @post This semaphore's count is set to the specified value.
    *
    * @param val The value to which the semaphore is reset.
    *
    * @note If processes are waiting on the semaphore, the results are
    *       undefined.
    */
   void reset(const int val)
   {
      const int result = usinitsema(mSema, val);
      assert(result == 0);
   }

   /**
    * Dumps the semaphore debug stuff and current state.
    *
    * @post All important data and debugging information related to this
    *       semaphore is dumped to the specified file descriptor (or to
    *       stderr if none is given).
    *
    * @param dest    File descriptor to which the output will be written.
    *                It defaults to stderr if no descriptor is specified.
    * @param message Message printed out before the output is dumped.
    */
   void dump(FILE* dest = stderr,
             const char* message = "\n------ Semaphore Status -----\n") const
   {
      usdumpsema(mSema, dest, message);
   }

protected:
   usema_t* mSema;

   // Problem here.  Fork will not like these.
   static MemPoolSGI* mSemaphorePool;
   static int* mAttachedCounter;
};

} // End of vpr namespace


#endif /* _VPR_SemaphoreSGI_h_ */
