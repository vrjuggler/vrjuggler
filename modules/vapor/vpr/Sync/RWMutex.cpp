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

#include <vpr/vprConfig.h>

#include <vpr/Sync/RWMutex.h>


namespace vpr {

/// Acquires a read mutex.
vpr::ReturnStatus RWMutex::acquireRead ()
{
   vpr::ReturnStatus status;

   if ( stateLock.acquire().failure() )
   {
      status.setCode(vpr::ReturnStatus::Fail);  // Didn't get the lock
   }
   else
   {
      // let those writers who are waiting have first shot
      while ( refCount < 0 || numWaitingWriters > 0 )
      {
         numWaitingReaders++;        // Another one waiting
         waitingReaders.wait();      // So wait until something changes
         numWaitingReaders--;        //
      }
   }

   if ( status.success() )
   {
      refCount++;
      stateLock.release();
   }

   return status;
}

/// Acquire a write mutex.
vpr::ReturnStatus RWMutex::acquireWrite()
{
   vpr::ReturnStatus status;

   if ( stateLock.acquire().failure() )
   {
      status.setCode(vpr::ReturnStatus::Fail);    // Didn't get the lock
   }
   else
   {
      while ( refCount != 0 )        // While there are readers
      {
         numWaitingWriters++;    // One more waiting
         waitingWriters.wait();  // Wait for soemthing to change
         numWaitingWriters--;    // Not waiting any more
      }
   }

   if ( status.success() )
   {
      refCount = -1;      // Tell everyone that there is a writer
      stateLock.release();
   }

   return status;
}

/// Tries to acquire a read mutex.
vpr::ReturnStatus RWMutex::tryAcquireRead ()
{
   vpr::ReturnStatus status(vpr::ReturnStatus::Fail);

   if ( stateLock.acquire().success() )
   {
      if ( ! (refCount == -1 || numWaitingWriters > 0) )
      {
         refCount++;
         status.setCode(vpr::ReturnStatus::Succeed);
      }
      stateLock.release();
   }
   return status;
}

/// Tries to acquire a write mutex.
vpr::ReturnStatus RWMutex::tryAcquireWrite ()
{
   vpr::ReturnStatus status(vpr::ReturnStatus::Fail);

   if ( stateLock.acquire().success() )
   {
      if ( refCount == 0 )
      {
         refCount = -1;
         status.setCode(vpr::ReturnStatus::Succeed);
      }
      stateLock.release();
   }

   return status;
}

/// Releases the mutex.
vpr::ReturnStatus RWMutex::release ()
{
   vpr::ReturnStatus status;

   if ( stateLock.acquire().failure() )
      return vpr::ReturnStatus(vpr::ReturnStatus::Fail);

   if ( refCount > 0 )        // We have a reader to release
      refCount--;
   else if ( refCount == -1 )    // We have writer
      refCount = 0;
   else            // We have an error
      std::cerr << "vpr::RWMutex::release: Should not have refCount of 0!!!"
      << std::endl;

   // Preference to writers
   if ( numWaitingWriters > 0 )
   {
      status = waitingWriters.signal();
   }
   else if ( numWaitingReaders > 0 )
   {
      status = waitingReaders.broadcast();
   }
   else
   {
      status.setCode(vpr::ReturnStatus::Succeed);
   }

   stateLock.release();

   return status;
}

} // End of vpr namespace
