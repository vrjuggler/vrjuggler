/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 */


#ifndef _VJGuard_h_
#define _VJGuard_h_
//----------------------------------------------
// vjGuard
//
// Purpose:
//    Guard wrapper
//
//
// Author:
//	Allen Bierbaum
//
// Date: 1-31-97
//-----------------------------------------------

#include <vjConfig.h>
#include <Sync/vjSemaphore.h>
#include <Sync/vjMutex.h>

//: Guard wrapper.
//!PUBLIC_API:
template <class LOCK>
class vjGuard
{
public:
   //: Acquire the lock implicitly.
   // If block = 1 then use a blocking acquire
   vjGuard(LOCK &lock, int block = 1)
   : theLock(&lock)
   {
      lockStatus = block ? acquire() : tryAcquire();
   }

   //: Release the lock
   ~vjGuard()
   {
      if (lockStatus >= 0)
      {   // != -1
         theLock->release();
      }
   }

   //! RETURNS: 1 - Locked
   //! RETURNS: 0 - Not locked
   int locked() {
      return (lockStatus >= 0);  // != -1
   }

   //: Acquire the lock
   int acquire()
   {
      return theLock->acquire();
   }

   //: Try to acquire lock
   int tryAcquire()
   {
      return theLock->tryAcquire();
   }

   //: Explicity release the lock
   int release()
   {
      return theLock->release();
   }


private:
   LOCK* theLock;	//: The lock that we are using
   int   lockStatus;	//: Are we locked or not
};

#endif
