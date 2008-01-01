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
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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

#include <sstream>

#include <vpr/Util/ResourceException.h>
#include <vpr/md/NSPR/NSPRHelpers.h>
#include <vpr/md/NSPR/Sync/RWMutexNSPR.h>


namespace vpr
{

RWMutexNSPR::RWMutexNSPR()
   : mLocked(false)
   , mRwLock(NULL)
{
   // Note the second argument "VPR RW Mutex" is for debug purposes only
   mRwLock = PR_NewRWLock(0, "VPR RW Mutex");

   if ( NULL == mRwLock )
   {
      std::ostringstream msg_stream;
      NSPR_PrintError("Read-write mutex allocation failed: ", msg_stream);
      throw vpr::ResourceException(msg_stream.str(), VPR_LOCATION);
   }
}

bool RWMutexNSPR::tryAcquireRead()
{
   bool locked(false);

/*
   stateLock.acquire();
   {
      if ( ! (mRefCount == -1 || numWaitingWriters > 0) )
      {
         mRefCount++;
         locked = true;
      }
   }
   stateLock.release();
*/

   throw vpr::Exception("Method not implemented!", VPR_LOCATION);

   return locked;
}

bool RWMutexNSPR::tryAcquireWrite()
{
   bool locked(false);

/*
   stateLock.acquire();
   {
      if ( mRefCount == 0 )
      {
         mRefCount = -1;
         locked = true;
      }
   }
   stateLock.release();
*/

   throw vpr::Exception("Method not implemented!", VPR_LOCATION);

   return locked;
}

} // End of vpr namespace
