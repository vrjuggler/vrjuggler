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

#include <vpr/vprConfig.h>

#include <vpr/Sync/LockException.h>
#include <vpr/Sync/RWMutex.h>


namespace vpr
{

// Acquires a read mutex.
void RWMutex::acquireRead()
{
   // If this operations throws an exception, it will propagate up to the
   // caller. If it does not throw an exception, then we have acquired our
   // lock.
   stateLock.acquire();
   {
      // let those writers who are waiting have first shot
      while ( refCount < 0 || numWaitingWriters > 0 )
      {
         numWaitingReaders++;        // Another one waiting
         waitingReaders.wait();      // So wait until something changes
         numWaitingReaders--;        //
      }

      refCount++;
   }
   stateLock.release();
}

// Acquire a write mutex.
void RWMutex::acquireWrite()
{
   // If this operations throws an exception, it will propagate up to the
   // caller. If it does not throw an exception, then we have acquired our
   // lock.
   stateLock.acquire();
   {
      while ( refCount != 0 )        // While there are readers
      {
         numWaitingWriters++;    // One more waiting
         waitingWriters.wait();  // Wait for soemthing to change
         numWaitingWriters--;    // Not waiting any more
      }

      refCount = -1;      // Tell everyone that there is a writer
   }
   stateLock.release();
}

// Tries to acquire a read mutex.
bool RWMutex::tryAcquireRead()
{
   bool locked(false);

   // If this operations throws an exception, it will propagate up to the
   // caller. If it does not throw an exception, then we have acquired our
   // lock.
   stateLock.acquire();
   {
      if ( ! (refCount == -1 || numWaitingWriters > 0) )
      {
         refCount++;
         locked = true;
      }
   }
   stateLock.release();

   return locked;
}

// Tries to acquire a write mutex.
bool RWMutex::tryAcquireWrite()
{
   bool locked(false);

   // If this operations throws an exception, it will propagate up to the
   // caller. If it does not throw an exception, then we have acquired our
   // lock.
   stateLock.acquire();
   {
      if ( refCount == 0 )
      {
         refCount = -1;
         locked = true;
      }
   }
   stateLock.release();

   return locked;
}

// Releases the mutex.
void RWMutex::release()
{
   // If this operations throws an exception, it will propagate up to the
   // caller. If it does not throw an exception, then we have acquired our
   // lock.
   stateLock.acquire();
   {
      if ( refCount > 0 )        // We have a reader to release
      {
         refCount--;
      }
      else if ( refCount == -1 )    // We have writer
      {
         refCount = 0;
      }
      else            // We have an error
      {
         throw vpr::LockException("Should not have a refCount of 0!",
                                  VPR_LOCATION);
      }

      // Preference to writers
      if ( numWaitingWriters > 0 )
      {
         waitingWriters.signal();
      }
      else if ( numWaitingReaders > 0 )
      {
         waitingReaders.broadcast();
      }
   }
   stateLock.release();
}

} // End of vpr namespace
