#ifndef _vjBarrierSGI_h_
#define _vjBarrierSGI_h_
#include <vjConfig.h>
#include <ulocks.h>
#include <SharedMem/vjMemPool.h>
#include <SharedMem/vjMemPoolSGI.h>
#include <SharedMem/SharedType.h>
#include <Sync/vjMutex.h>
#include <Sync/vjGuard.h>


//----------------------------------------------
//:    Barrier wrapper for SGI.
//
//    A barrier is a primitive that allows several
//    processes to wait at a sync point.
//
// Author:
//	Allen Bierbaum
//
// Date: 1-30-97
//!PUBLIC_API:
//-----------------------------------------------
class  vjBarrierSGI
{
public:
   // Initialize the barrier to synchronize <count> threads.
   vjBarrierSGI (int count) : syncCount(count)
   {
      if (barrierPool == NULL)
      {
         barrierPool = new vjMemPoolSGI(65536, 32, "/var/tmp/memBarrierPoolSGIXXXXXX");
         attachedCounter = static_cast<int*>(barrierPool->allocate(sizeof(int)));
         *attachedCounter = 0;
      }
      *attachedCounter = *attachedCounter + 1;      // Track how many mutexes are allocated
      //vjDEBUG << " vjBarrierSGI:: vjBarrierSGI: attachedCounter: " << *attachedCounter << endl << vjDEBUG_FLUSH;

      // ----- Allocate the mutex ----- //
      theBarrier = new_barrier(barrierPool->getArena());
   }

   ~vjBarrierSGI()
   {
      // ---- Delete the barrier --- //
      free_barrier(theBarrier);

      // ---- Deal with the pool --- //
      *attachedCounter = *attachedCounter - 1;     // Track how many mutexes are allocated

      //vjDEBUG << " vjBarrierSGI::~ vjBarrierSGI: attachedCounter: " << *attachedCounter << endl << vjDEBUG_FLUSH;

      if (*attachedCounter == 0)
      {
         barrierPool->deallocate(attachedCounter);
         attachedCounter = NULL;
         delete barrierPool;
         barrierPool = NULL;
      }
   }

   // Block the caller until all <count> threads have called <wait> and
   // then allow all the caller threads to continue in parallel.
   int wait(int numProcs)
   {
      ;
   }

   int wait ()
   {
      barrier(theBarrier, syncCount);
      //init_barrier(theBarrier);	    // -- How do I reset the barrier -- //
      return 0;
   }

   //-----------------------------------------------------------
   // addProcess/removeProcess
   // PURPOSE:
   //	    Tell the barrier to increase/decrease the count of the number
   //	    of threads to syncronize
   //-----------------------------------------------------------
   void addProcess()
   {
      vjGuard<vjMutex> guard(mutex);
      syncCount++;
   }

   void removeProcess()
   {
      vjGuard<vjMutex> guard(mutex);
      syncCount--;
   }

   void setProcesses(int procs)
   {
      vjGuard<vjMutex> guard(mutex);
      syncCount = procs;
   }

   void dump (void) const
   {
      cerr << "\n vjBarrierSGI::dump: Not implemented.\n";
   }

protected:
   vjMutex    mutex;
   int      syncCount;
   barrier_t* theBarrier;

   // = Prevent assignment and initialization.
   void operator= (const  vjBarrierSGI &) {}
   vjBarrierSGI (const  vjBarrierSGI &) {}

   static vjMemPoolSGI* barrierPool;
   static int* attachedCounter;
};

MemPoolSGI*  vjBarrierSGI::barrierPool = NULL;
int*  vjBarrierSGI::attachedCounter = NULL;

#endif
