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
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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

#ifndef _VPR_SOCkET_SIMPLE_ALLOCATION_STRATEGIES_H_
#define _VPR_SOCkET_SIMPLE_ALLOCATION_STRATEGIES_H_

#include <vpr/vprConfig.h>


namespace vpr
{

/**
 * Returns the sum of \p curBufferSize and \p increaseAmt thus indicating
 * that only the minimum amount of memory should be allocated to fulfill the
 * request.
 *
 * @see vpr::NoPushWriter
 * @see vpr::StreamSocket
 *
 * @since 2.1.9
 */
inline size_t minimalAllocationStrategy(const size_t curBufferSize,
                                        const size_t increaseAmt)
{
   return curBufferSize + increaseAmt;
}

/**
 * Returns the smaller of two possible sizes: doubling the current buffer size
 * or doubling the sum of the current buffer size and the increase amount.
 * Which of these is returned is determined by comparing the doubled current
 * buffer size against the needed amount of space for the additional bytes. If
 * doubling the current buffer size is sufficient, then that value is returned.
 * Otherwise, the doubled sum of the current buffer size and the increase
 * amount is returned.
 *
 * @return A value strictly larger than \p curBufferSize + \p increaseAmt.
 *
 * @since 2.1.9
 */
inline size_t doublingAllocationStrategy(const size_t curBufferSize,
                                         const size_t increaseAmt)
{
   return (curBufferSize * 2 > curBufferSize + increaseAmt) ?
             curBufferSize * 2 : (curBufferSize + increaseAmt) * 2;
}

}


#endif /* _VPR_SOCkET_SIMPLE_ALLOCATION_STRATEGIES_H_ */
