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

#ifndef _VPR_Guard_h_
#define _VPR_Guard_h_

#include <vpr/vprConfig.h>
#include <vpr/Util/ReturnStatus.h>


namespace vpr
{

/** \class Guard Guard.h vpr/Sync/Guard.h
 *
 * Guard wrapper.
 *
 * @date January 31, 1997
 */
template<class LOCK_TYPE>
class Guard
{
public:
   /**
    * Acquires the lock implicitly.
    * If \p block is 1 then use a blocking mutex acquisition operationg.
    */
   Guard(LOCK_TYPE &lock, int block = 1) : mTheLock(&lock)
   {
      mLockStatus = block ? acquire().success() : tryAcquire().success();
   }

   /** Releases the lock. */
   ~Guard()
   {
      if ( mLockStatus )
      {
         mTheLock->release();
      }
   }

   /**
    * @return \c true is returned if this guard is locked.
    * @return \c false is returned if this guard is not locked.
    */
   const bool& locked()
   {
      return mLockStatus;
   }

   /** Acquires the lock. */
   vpr::ReturnStatus acquire()
   {
      vpr::ReturnStatus status = mTheLock->acquire();
      mLockStatus = status.success();
      return status;
   }

   /** Tries to acquire lock. */
   vpr::ReturnStatus tryAcquire()
   {
      vpr::ReturnStatus status = mTheLock->tryAcquire();
      mLockStatus = status.success();
      return status;
   }

   /** Explicity releases the lock. */
   vpr::ReturnStatus release()
   {
      mLockStatus = false;
      return mTheLock->release();
   }

private:
   LOCK_TYPE* mTheLock;    /**< The lock that we are using */
   bool       mLockStatus; /**< Are we locked or not */
};

} // End of vpr namespace


#endif
