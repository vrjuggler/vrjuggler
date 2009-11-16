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
 * VR Juggler is (C) Copyright 1998-2009 by Iowa State University
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

#include <numeric>

#include <vpr/IO/Socket/AveragingAllocationStrategy.h>


namespace
{

template<typename T>
inline T maxValue(const T v1, const T v2)
{
   return v1 > v2 ? v1 : v2;
}

}

namespace vpr
{

AveragingAllocationStrategy::
AveragingAllocationStrategy(const size_type windowSize)
   : mWindowSize(windowSize)
   , mIndex(0)
{
   // Reserve the memory required for storing the sliding window of allocation
   // sizes so that the buffer does not need to grow as we add values to it.
   mAllocSizes.reserve(windowSize);
}

size_t AveragingAllocationStrategy::operator()(const size_t curBufferSize,
                                               const size_t increaseAmt)
{
   // The amount of memory to allocate is the larger of the requested size and
   // the average size computed from previous allocations.
   const size_t alloc_size(maxValue(curBufferSize + increaseAmt,
                                    computeAverage()));

   // Record the current amount allocated for future computations of the
   // average allocation amount. We accomplish this by first populating
   // mAllocSizes until its size equals mWindowSize. After that, we just
   // assign to the entry identified by mIndex.
   if ( mAllocSizes.size() < mWindowSize )
   {
      mAllocSizes.push_back(alloc_size);
   }
   else
   {
      mAllocSizes[mIndex] = alloc_size;
   }

   mIndex = (mIndex + 1) % mWindowSize;

   return alloc_size;
}

size_t AveragingAllocationStrategy::computeAverage() const
{
   size_t average(0);

   if ( ! mAllocSizes.empty() )
   {
      const size_t sum(std::accumulate(mAllocSizes.begin(),
                                       mAllocSizes.end(), 0));

      // Compute the average allocation size.
      average = sum / mAllocSizes.size();
   }

   return average;
}

}
