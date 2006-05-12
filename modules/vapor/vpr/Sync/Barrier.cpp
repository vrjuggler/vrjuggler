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
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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
#include <vpr/Sync/Guard.h>
#include <vpr/Sync/Barrier.h>


namespace vpr
{

Barrier::Barrier(unsigned int count)
   : mThreshold(count)
   , mCount(count)
   , mGeneration(0)
{;}

Barrier::~Barrier()
{;}

// Block the caller until all <count> threads have called <wait> and then
// allow all the caller threads to continue in parallel.
bool Barrier::wait()
{
   Guard<CondVar> guard(mCond);

   unsigned int gen_temp(mGeneration);

   if (--mCount == 0)   // If last thread to sync
   {
      mGeneration++;          // Next generation
      mCount = mThreshold;    // Reset the count
      mCond.broadcast();      // Wake up all threads
      return true;
   }

   while(gen_temp == mGeneration)      // While generation has not increased...
   {
      mCond.wait();
   }

   return true;
}

void Barrier::addProcess()
{
   if(mCount != mThreshold)
   {
      std::cerr << "vpr::Barrier: Attempt to add process while in use."
                << std::endl;
   }
   else
   {
      mThreshold++;
      mCount = mThreshold;
   }
}

// Tells the barrier to decrease the count of the number of threads to
// syncronize.
void Barrier::removeProcess()
{
   if(mCount != mThreshold)
   {
      std::cerr << "vpr::Barrier: Attempt to remove process while in use."
                << std::endl;
   }
   else if(1 == mCount)
   {
      std::cerr << "vpr::Barrier: Attempt to reduce barrier to 0." << std::endl;
   }
   else
   {
      mThreshold++;
      mCount = mThreshold;
   }
}

} // End of vpr namespace
