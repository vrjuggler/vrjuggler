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

#ifndef _vprSemaphoreSGI_h_
#define _vprSemaphoreSGI_h_

#include <vpr/vprConfig.h>
#include <ulocks.h>

#include <vpr/md/SPROC/SharedMem/MemPool.h>
#include <vpr/Util/ReturnStatus.h>


namespace vpr {

//----------------------------------------------
//: Semaphore wrapper for the SGI systems
//
// This class encapsulates the behavior of a semaphore variable.
//
// Author:
//  Allen Bierbaum
//
// Date: 1-20-97
//-----------------------------------------------
//!PUBLIC_API:
class SemaphoreSGI
{
public:
   //---------------------------------------------------------
   //: Constructor
   // Default to initial Value = 1
   // That means taht semaphore initialy is available.
   //---------------------------------------------------------
   SemaphoreSGI (int initialValue = 1)
   {
      // BUG:
      if (semaphorePool == NULL)
      {
         semaphorePool = new MemPoolSGI(65536, 32,
                                        "/var/tmp/memSemaphorePoolSGIXXXXXX");
         attachedCounter = static_cast<int*>(semaphorePool->allocate(sizeof(int)));
         *attachedCounter = 0;
      }
      *attachedCounter = *attachedCounter + 1;      // Track how many semaphores are allocated

//      DebugLock.acquire();
//      vprDEBUG << " vpr::SemaphoreSGI::SemaphoreSGI: attachedCounter: "
//               << *attachedCounter << endl << vprDEBUG_FLUSH;
//      DebugLock.release();

      // ----- Allocate the semaphore ----- //
      sema = usnewsema(semaphorePool->getArena(), initialValue);
   }

   ~SemaphoreSGI(void)
   {
      // ---- Delete the semaphore --- //
      usfreesema(sema, semaphorePool->getArena());

      // ---- Deal with the pool --- //
      *attachedCounter = *attachedCounter - 1;     // Track how many Semaphore are allocated

//      DebugLock.acquire();
//      vprDEBUG << "vpr::SemaphoreSGI::~SemaphoreSGI: attachedCounter: "
//               << *attachedCounter << endl << vprDEBUG_FLUSH;
//      DebugLock.release();

      if (*attachedCounter == 0)
      {
         semaphorePool->deallocate(attachedCounter);
         attachedCounter = NULL;
         delete semaphorePool;
         semaphorePool = NULL;
      }
   }

   //---------------------------------------------------------
   //: Lock the semaphore.
   //
   //! RETURNS:  1 - Acquired
   //! RETURNS: -1 - Error
   //---------------------------------------------------------
   vpr::ReturnStatus acquire() const
   {
      vpr::ReturnStatus status;

      if ( uspsema(sema) < 0 )
      {
         std::cerr << "vpr::SemphoreSGI::ERROR:" << std::endl;
         status.setCode(vpr::ReturnStatus::Fail);
      }

      return status;
   }

   //----------------------------------------------------------
   //: Acquire a read semaphore.
   //----------------------------------------------------------
   vpr::ReturnStatus acquireRead() const
   {
      return this->acquire();     // No special "read" semaphore -- For now
   }

   //----------------------------------------------------------
   //: Acquire a write semaphore.
   //----------------------------------------------------------
   vpr::ReturnStatus acquireWrite() const
   {
      return this->acquire();     // No special "write" semaphore -- For now
   }

   //---------------------------------------------------------
   //: Try to acquire the semaphore immediately.  Does not
   //+ block.
   //
   //! RETURNS: 1 - Acquired
   //! RETURNS: 0 - Not acquired
   //---------------------------------------------------------
   vpr::ReturnStatus tryAcquire () const
   {
      if ( uscpsema(sema) == 1 )
      {
         return vpr::ReturnStatus();
      }
      else
      {
         return vpr::ReturnStatus(vpr::ReturnStatus::Fail);
      }
   }

   //----------------------------------------------------------
   //: Try to acquire a read semaphore.
   //----------------------------------------------------------
   vpr::ReturnStatus tryAcquireRead () const
   {
      return this->tryAcquire();
   }

   //----------------------------------------------------------
   //: Try to acquire a write semaphore.
   //----------------------------------------------------------
   vpr::ReturnStatus tryAcquireWrite () const
   {
      return this->tryAcquire();
   }

   //---------------------------------------------------------
   //: Release the semaphore.
   //
   //! RETURNS:  0 - Succeed
   //! RETURNS: -1 - Error
   //---------------------------------------------------------
   vpr::ReturnStatus release() const
   {
      vpr::ReturnStatus status;

      if( usvsema(sema) < 0 )
      {
         std::cerr << "vpr::SemaphoreSGI::ERROR:" << std::endl;
         status.setCode(vpr::ReturnStatus::Fail);
      }

      return status;
   }

   //---------------------------------------------------------
   //: Reset the semaphore.
   //
   //! RETURNS:  0 - Succeed
   //! RETURNS: -1 - Error
   //
   //! NOTE: If processes are waiting on the semaphore,
   //+ the results are undefined.
   //---------------------------------------------------------
   vpr::ReturnStatus reset(int val)
   {
      if ( usinitsema(sema, val) == 0 )
      {
         return vpr::ReturnStatus();
      }
      else
      {
         return vpr::ReturnStatus(vpr::ReturnStatus::Fail);
      }
   }

   //---------------------------------------------------------
   //: Dump the semaphore debug stuff and current state.
   //---------------------------------------------------------
   void dump (FILE* dest = stderr,
              const char* message = "\n------ Semaphore Dump -----\n") const
   {
      usdumpsema(sema, dest, message);
   }


protected:
   usema_t* sema;

   // = Prevent assignment and initialization.
   void operator= (const SemaphoreSGI &) {;}
   SemaphoreSGI (const SemaphoreSGI &) {}

   // Problem here.  Fork will not like these.
   static MemPoolSGI* semaphorePool;
   static int* attachedCounter;
};

}; // End of vpr namespace

#endif
