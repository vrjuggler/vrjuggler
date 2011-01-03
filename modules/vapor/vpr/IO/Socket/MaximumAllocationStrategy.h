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
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VPR_SOCkET_MAXIMUM_ALLOCATION_STRATEGY_H_
#define _VPR_SOCkET_MAXIMUM_ALLOCATION_STRATEGY_H_

#include <vpr/vprConfig.h>

#include <vector>


namespace vpr
{

/** \class MaximumAllocationStrategy MaximumAllocationStrategy.h vpr/IO/Socket/MaximumAllocationStrategy.h
 *
 * A memory allocation strategy for use with vpr::NoPushWriter that keeps
 * track of the maximum amount of memory allocated while the socket is in the
 * no-push ("corked") state. When queried, the maximum memory allocation value
 * thus far is returned as the amount to allocate. This strategy is suitable
 * for a case when a socket is used for writing roughly the same amount of
 * data repeatedly. Copying due to reallocation will be minimized (once the
 * maximum send size while corked is determined), but memory usage will be
 * less efficient if there is significant deviation of data written to the
 * socket while corked.
 *
 * @since 2.1.9
 */
class VPR_CLASS_API MaximumAllocationStrategy
{
public:
   MaximumAllocationStrategy();

   /**
    * Returns the maximum of \c mMaxAllocSize and the sum of \p curBufferSize
    * and \p increaseAmt.
    *
    * @post \c mMaxAllocSize holds the maximum value returned by this strategy
    *       thus far.
    */
   size_t operator()(const size_t curBufferSize, const size_t increaseAmt);

private:
   size_t mMaxAllocSize;
};

}


#endif /* _VPR_SOCkET_MAXIMUM_ALLOCATION_STRATEGY_H_ */
