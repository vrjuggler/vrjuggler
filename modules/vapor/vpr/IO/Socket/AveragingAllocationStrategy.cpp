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
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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

template<typename T>
inline T minValue(const T v1, const T v2)
{
   return v1 < v2 ? v1 : v2;
}

}

namespace vpr
{

AveragingAllocationStrategy::
AveragingAllocationStrategy(const size_type windowSize)
   : mWindowSize(windowSize)
   , mIndex(0)
   // mAllocSizes is set to a fixed size so that it is not necessary to resize
   // it as allocations occur, nor is it necessary to stop resizing it when
   // the number of allocations eventually equals windowSize.
   , mAllocSizes(windowSize, 0)
   // mAllocOffset is initialized to 1 because we add it to
   // mAllocSizes.begin() in order to compute the average of the allocations
   // made. See computeAverage().
   , mAllocOffset(1)
{
   /* Do nothing. */ ;
}

size_t AveragingAllocationStrategy::operator()(const size_t curBufferSize,
                                               const size_t increaseAmt)
{
   // The amount of memory to allocate is the larger of the requested size and
   // the average size computed from previous allocations.
   const size_t alloc_size(maxValue(curBufferSize + increaseAmt,
                                    computeAverage()));

   // Record the current amount allocated for future computations of the
   // average allocation amount.
   mAllocSizes[mIndex] = alloc_size;
   mIndex = (mIndex + 1) % mWindowSize;

   // Incrementing mAllocOffset beyond mWindowSize eliminates the need for it.
   // Perhaps there is a better way of keeping track of the number of
   // allocation sizes stored in mAllocSizes.
   ++mAllocOffset;

   return alloc_size;
}

size_t AveragingAllocationStrategy::computeAverage() const
{
   // The smaller of mAllocOffset and mWindowSize provide the number of
   // allocation sizes that have been stored in mAllocSizes.
   const size_type count(minValue(mAllocOffset, mWindowSize));
   const size_t sum = std::accumulate(mAllocSizes.begin(),
                                      mAllocSizes.begin() + count, 0);

   // Return the average allocation size.
   return sum / count;
}

}
