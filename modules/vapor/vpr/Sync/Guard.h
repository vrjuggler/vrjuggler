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
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
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
//#include <vpr/Sync/Semaphore.h>
//#include <vpr/Sync/Mutex.h>


namespace vpr {

/**
 * Guard wrapper.
 *
 * @author Allen Bierbaum
 *
 * @date 1-31-97
 */
template <class LOCK>
class Guard
{
public:
   /**
    * Acquires the lock implicitly.
    * If block = 1 then use a blocking acquire.
    */
   Guard(LOCK &lock, int block = 1)
   : theLock(&lock)
   {
      lockStatus = block ? acquire() : tryAcquire();
   }

   /// Releases the lock.
   ~Guard()
   {
      if (lockStatus >= 0)
      {   // != -1
         theLock->release();
      }
   }

   /**
    * @return 1 is returned if this guard is locked.<br>
    *         0 is returned if this guard is not locked.
    */
   int locked() {
      return (lockStatus >= 0);  // != -1
   }

   /// Acquires the lock.
   int acquire()
   {
      return theLock->acquire();
   }

   /// Tries to acquire lock.
   int tryAcquire()
   {
      return theLock->tryAcquire();
   }

   /// Explicity releases the lock.
   int release()
   {
      return theLock->release();
   }


private:
   LOCK* theLock;	//! The lock that we are using
   int   lockStatus;	//! Are we locked or not
};

}; // End of vpr namespace


#endif
