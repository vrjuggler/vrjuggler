/*
 *  File:	    $RCSfile$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                             Patrick Hartling
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                  Copyright (C) - 1997, 1998, 1999, 2000
 *              Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#ifndef _vjSemaphoreSGI_h_
#define _vjSemaphoreSGI_h_

#include <vjConfig.h>
#include <ulocks.h>
#include <SharedMem/vjMemPool.h>
#include <SharedMem/vjMemPoolSGI.h>

//----------------------------------------------
//: Semaphore wrapper for the SGI systems
//
// This class encapsulates the behavior of a semaphore variable.
//
// Author:
//	Allen Bierbaum
//
// Date: 1-20-97
//-----------------------------------------------
//!PUBLIC_API:
class vjSemaphoreSGI
{
public:
   //---------------------------------------------------------
   //: Constructor
   // Default to initial Value = 1
   // That means taht semaphore initialy is available.
   //---------------------------------------------------------
   vjSemaphoreSGI (int initialValue = 1)
   {
      // BUG:
      if (semaphorePool == NULL)
      {
         semaphorePool = new vjMemPoolSGI(65536, 32, "/var/tmp/memSemaphorePoolSGIXXXXXX");
         attachedCounter = static_cast<int*>(semaphorePool->allocate(sizeof(int)));
         *attachedCounter = 0;
      }
      *attachedCounter = *attachedCounter + 1;      // Track how many semaphores are allocated

      //DebugLock.acquire();
      //vjDEBUG << vjThread::self() << " vjSemaphoreSGI::vjSemaphoreSGI: attachedCounter: " << *attachedCounter << endl << vjDEBUG_FLUSH;
      //DebugLock.release();

      // ----- Allocate the semaphore ----- //
      sema = usnewsema(semaphorePool->getArena(), initialValue);
   }

   ~vjSemaphoreSGI(void)
   {
      // ---- Delete the semaphore --- //
      usfreesema(sema, semaphorePool->getArena());

      // ---- Deal with the pool --- //
      *attachedCounter = *attachedCounter - 1;     // Track how many Semaphore are allocated

      //DebugLock.acquire();
      //vjDEBUG << vjThread::self() << " vjSemaphoreSGI::~vjSemaphoreSGI: attachedCounter: " << *attachedCounter << endl << vjDEBUG_FLUSH;
      //DebugLock.release();

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
   int acquire() const
   {
      int ret_val = uspsema(sema);
      if(ret_val < 0)
         cerr << "vjSemphoreSGI::ERROR:" << endl;

      return ret_val;
   }

   //----------------------------------------------------------
   //: Acquire a read semaphore.
   //----------------------------------------------------------
   int acquireRead() const
   {
      return this->acquire();     // No special "read" semaphore -- For now
   }

   //----------------------------------------------------------
   //: Acquire a write semaphore.
   //----------------------------------------------------------
   int acquireWrite() const
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
   int tryAcquire () const
   {
      return uscpsema(sema);
   }

   //----------------------------------------------------------
   //: Try to acquire a read semaphore.
   //----------------------------------------------------------
   int tryAcquireRead () const
   {
      return this->tryAcquire();
   }

   //----------------------------------------------------------
   //: Try to acquire a write semaphore.
   //----------------------------------------------------------
   int tryAcquireWrite () const
   {
      return this->tryAcquire();
   }

   //---------------------------------------------------------
   //: Release the semaphore.
   //
   //! RETURNS:  0 - Success
   //! RETURNS: -1 - Error
   //---------------------------------------------------------
   int release() const
   {
      int ret_val = usvsema(sema);
      if(ret_val < 0)
         cerr << "vjSemaphoreSGI::ERROR:" << endl;

      return ret_val;
   }

   //---------------------------------------------------------
   //: Reset the semaphore.
   //
   //! RETURNS:  0 - Success
   //! RETURNS: -1 - Error
   //
   //! NOTE: If processes are waiting on the semaphore,
   //+ the results are undefined.
   //---------------------------------------------------------
   int reset(int val)
   {
      return usinitsema(sema, val);
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
   void operator= (const vjSemaphoreSGI &) {}
   vjSemaphoreSGI (const vjSemaphoreSGI &) {}

   // Problem here.  Fork will not like these.
   static vjMemPoolSGI* semaphorePool;
   static int* attachedCounter;
};

#endif
