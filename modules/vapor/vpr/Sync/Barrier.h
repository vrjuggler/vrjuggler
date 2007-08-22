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

#ifndef _VPR_Barrier_h_
#define _VPR_Barrier_h_

/**
 * \file
 *
 * Include this file to get the full declaration of the platform-specific
 * class that is typedef'd to vpr::Barrier.
 */

#include <vpr/vprConfig.h>

#include <vpr/Sync/CondVar.h>
#include <vpr/Sync/Mutex.h>
#include <vpr/Sync/Guard.h>


namespace vpr
{

/** \class SubBarrier Barrier.h vpr/Sync/Barrier.h
 *
 * Helper class for vpr::Barrier.
 */
class SubBarrier
{
public:
   /**
    * Initialization.
    */
   SubBarrier(int count, Mutex* lock)
      : barrierFinished(lock)
      , runningThreads(count)
   {
   }

   CondVar barrierFinished; /**< True if this generation of the barrier is done */

   int runningThreads;  /**< Number of threads that are still running */
};

/** \class Barrier Barrier.h vpr/Sync/Barrier.h
 *
 * Implements "barrier synchronization" primitive.
 */
class Barrier
{
public:
   /**
    * Constructor for barrier.
    * Creates a barrier that will wait for \p count threads to synchronize.
    */
   Barrier(unsigned int count);

   /** Destructor. */
   ~Barrier();

   /**
    * Blocks the caller until all \c mCount threads have called wait() and
    * then allows all the caller threads to continue in parallel.
    */
   bool wait();

   /**
    * Tells the barrier to increase the count of the number of threads to
    * syncronize.
    */
   void addProcess();

   /**
    * Tells the barrier to decrease the count of the number of threads to
    * syncronize.
    */
   void removeProcess();

private:
   vpr::CondVar   mCond;   /**< Condition variable and mutex for the barrier. */
   unsigned int   mThreshold;
   unsigned int   mCount;
   unsigned int   mGeneration;

   // = Prevent assignment and initialization.
   void operator= (const Barrier &)
   {;}

   Barrier(const Barrier &)
   {;}
};

} // End of vpr namespace


#endif  /* _VPR_Barrier_h_ */
