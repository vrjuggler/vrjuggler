#ifndef _vjSemaphoreSGI_h_
#define _vjSemaphoreSGI_h_

#include <config.h>
#include <ulocks.h>
#include <SharedMem/vjMemPool.h>
#include <SharedMem/vjMemPoolSGI.h>
#include <Kernel/vjDebug.h>
#include <Threads/vjThread.h>

//----------------------------------------------
//: Semaphore wrapper for the SGI systems
//
// Author:
//	Allen Bierbaum
//
// Date: 1-20-97
//-----------------------------------------------
class vjSemaphoreSGI
{
public:
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

      DebugLock.acquire();
      //vjDEBUG << vjThread::self() << " vjSemaphoreSGI::vjSemaphoreSGI: attachedCounter: " << *attachedCounter << endl;
      DebugLock.release();

      // ----- Allocate the semaphore ----- //
      sema = usnewsema(semaphorePool->getArena(), initialValue);
   }

   ~vjSemaphoreSGI(void)
   {
      // ---- Delete the semaphore --- //
      usfreesema(sema, semaphorePool->getArena());

      // ---- Deal with the pool --- //
      *attachedCounter = *attachedCounter - 1;     // Track how many Semaphore are allocated  

      DebugLock.acquire();
      //vjDEBUG << vjThread::self() << " vjSemaphoreSGI::~vjSemaphoreSGI: attachedCounter: " << *attachedCounter << endl;
      DebugLock.release();

      if (*attachedCounter == 0)
      {
         semaphorePool->deallocate(attachedCounter);
         attachedCounter = NULL;
         delete semaphorePool;
         semaphorePool = NULL;
      }

   }

   //---------------------------------------------------------
   // int aquire()
   //
   // PURPOSE:
   //   Lock the Semaphore
   // RETURNS:
   //   1 - Aquired
   //  -1 - Error
   //---------------------------------------------------------
   int acquire() const
   {
      return uspsema(sema);     
   }

//----------------------------------------------------------
//  Acquire a read Semaphore
//----------------------------------------------------------
   int acquireRead() const
   {
      return this->acquire();     // No special "read" semaphore -- For now
   }

//----------------------------------------------------------
//  Acquire a write Semaphore
//----------------------------------------------------------
   int acquireWrite() const
   {
      return this->acquire();     // No special "write" semaphore -- For now
   }

//---------------------------------------------------------
// int tryAcquire()
//
// PURPOSE:
//   Try to acquire the semaphore immediately.  Does not block.
// RETURNS:
//   1 - Aquired
//   0 - Not Aquired
//---------------------------------------------------------
   int tryAcquire () const
   {
      return uscpsema(sema);
   }

//----------------------------------------------------------
//  Try to aquire a read Semaphore
//----------------------------------------------------------
   int tryAcquireRead () const
   {
      return this->tryAcquire();
   }

//----------------------------------------------------------
//  Try to aquire a write Semaphore
//----------------------------------------------------------
   int tryAcquireWrite () const
   {
      return this->tryAcquire();
   }

//---------------------------------------------------------
// int release()
//
// PURPOSE:
//   Release the Semaphore. 
// RETURNS:
//   0 - Success
//  -1 - Error
//---------------------------------------------------------
   int release() const
   {
      return usvsema(sema);   
   }

//---------------------------------------------------------
// int reset(int val)
//
// PURPOSE:
//   Reset the Semaphore. 
// RETURNS:
//   0 - Success
//  -1 - Error
// NOTE:
//   If processes are waiting on the semaphore, the results are undefined
//---------------------------------------------------------
   int release(int val)
   {
      return usinitsema(sema, val);   
   }

//---------------------------------------------------------
// void dump()
//
// PURPOSE:
//  Dump the semaphore debug stuff and current state.
//---------------------------------------------------------
   void dump (FILE* dest = stderr, const char* message = "\n------ Semaphore Dump -----\n") const
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
