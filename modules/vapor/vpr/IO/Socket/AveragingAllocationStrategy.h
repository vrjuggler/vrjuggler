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

#ifndef _VPR_SOCkET_AVERAGING_ALLOCATION_STRATEGY_H_
#define _VPR_SOCkET_AVERAGING_ALLOCATION_STRATEGY_H_

#include <vpr/vprConfig.h>

#include <vector>


namespace vpr
{

/** \class AveragingAllocationStrategy AveragingAllocationStrategy.h vpr/IO/Socket/AveragingAllocationStrategy.h
 *
 * This allocation strategy, designed specifically for use with
 * vpr::NoPushWriter, keeps a sliding window of allocation sizes. When
 * called, the maximum of the average allocation size and the required
 * allocation size is returned. The intention is that this class can be used
 * when many write operations are performed on a socket with the sizes of
 * those writes varying frequently. Ideally, the averaging will allocate a
 * buffer that is big enough during the corked time frame that it will not
 * require reaallocation during that time and will not be too much larger
 * than is strictly necessary.
 *
 * @note Using an average to achieve the stated goals may be naive.
 *
 * @since 2.1.11
 */
class VPR_CLASS_API AveragingAllocationStrategy
{
public:
   typedef std::vector<size_t>::size_type size_type;

   AveragingAllocationStrategy(const size_type windowSize = 100);

   /**
    * Returns the larger of the sum of \p curBufferSize and \p increaseAmt
    * and the average of all previous allocations within the sliding window.
    * More concisely, this method returns the following:
    *
    * \code
    * max(curBufferSize + increaseAmt, computeAverage())
    * \endcode
    *
    * @post \c mAllocOffset is incremented by 1. The value returned is
    *       recorded in \c mAllocSizes at entry \c mIndex (the value at the
    *       time of invocation). \c mIndex is then incremented by 1 but kept
    *       within the range [0,mWindowSize). 
    *
    * @return The amount of memory to allocate to satisfy the request is
    *         returned.
    */
   size_t operator()(const size_t curBufferSize, const size_t increaseAmt);

private:
   /**
    * Computes the average allocation size using the number of allocation
    * sizes stored in \c mAllocSizes.
    */
   size_t computeAverage() const;

   size_type mWindowSize;

   /** @name Mean Allocation Computational Data */
   //@{
   size_type           mIndex;
   std::vector<size_t> mAllocSizes;
   size_type           mAllocOffset;
   //@}
};

}


#endif /* _VPR_SOCkET_AVERAGING_ALLOCATION_STRATEGY_H_ */
