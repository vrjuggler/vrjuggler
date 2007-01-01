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

#ifndef _vprBarrierSGI_h_
#define _vprBarrierSGI_h_

#include <vpr/vprConfig.h>
#include <ulocks.h>
#include <vpr/md/SPROC/SharedMem/MemPool.h>
#include <vpr/Sync/Mutex.h>
#include <vpr/Sync/Guard.h>


namespace vpr
{

/** \class BarrierSGI BarrierSGI.h vpr/Sync/Barrier.h
 *
 * Barrier wrapper for SGI.
 * A barrier is a primitive that allows several processes to wait at a sync
 * point.  This is typedef'd to vpr::Barrier.
 *
 * @date January 30, 1997
 */
class BarrierSGI
{
public:
   /**
    * Initializes the barrier to synchronize count threads.
    */
   BarrierSGI(int count) : syncCount(count)
   {
      if (barrierPool == NULL)
      {
         barrierPool = new MemPoolSGI(65536, 32,
                                      "/var/tmp/memBarrierPoolSGIXXXXXX");
         attachedCounter = static_cast<int*>(barrierPool->allocate(sizeof(int)));
         *attachedCounter = 0;
      }
      *attachedCounter = *attachedCounter + 1;      // Track how many mutexes are allocated
      //vprDEBUG << "BarrierSGI::BarrierSGI: attachedCounter: " << *attachedCounter << std::endl << vprDEBUG_FLUSH;

      // ----- Allocate the mutex ----- //
      theBarrier = new_barrier(barrierPool->getArena());
   }

   ~BarrierSGI()
   {
      // ---- Delete the barrier --- //
      free_barrier(theBarrier);

      // ---- Deal with the pool --- //
      *attachedCounter = *attachedCounter - 1;     // Track how many mutexes are allocated

      //vprDEBUG << "BarrierSGI::~BarrierSGI: attachedCounter: " << *attachedCounter << std::endl << vprDEBUG_FLUSH;

      if (*attachedCounter == 0)
      {
         barrierPool->deallocate(attachedCounter);
         attachedCounter = NULL;
         delete barrierPool;
         barrierPool = NULL;
      }
   }

   /**
    * Blocks the caller until all numProcs threads have called wait() and
    * then allows all the caller threads to continue in parallel.
    */
   int wait(int numProcs)
   {
      return -1;
   }

   int wait()
   {
      barrier(theBarrier, syncCount);
      //init_barrier(theBarrier);       // -- How do I reset the barrier -- //
      return 0;
   }

   /**
    * Tells the barrier to increase the count of the number of threads to
    * syncronize.
    */
   void addProcess()
   {
      Guard<Mutex> guard(mutex);
      syncCount++;
   }

   /**
    * Tells the barrier to decrease the count of the number of threads to
    * syncronize.
    */
   void removeProcess()
   {
      Guard<Mutex> guard(mutex);
      syncCount--;
   }

   void setProcesses(int procs)
   {
      Guard<Mutex> guard(mutex);
      syncCount = procs;
   }

protected:
   Mutex    mutex;
   int      syncCount;
   barrier_t* theBarrier;

   // = Prevent assignment and initialization.
   void operator=(const BarrierSGI &) {;}
   BarrierSGI(const BarrierSGI &) {;}

   static MemPoolSGI* barrierPool;
   static int* attachedCounter;
};

} // End of vpr namespace

#endif
