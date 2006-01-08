/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
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

#ifndef BANDWIDTHSTATADAPTER_H
#define BANDWIDTHSTATADAPTER_H

#include <vpr/vprConfig.h>

namespace vpr
{

/** \class IOStatsStrategyAdapter IOStatsStrategyAdapter.h vpr/IO/Stats/IOStatsStrategyAdapter.h
 *
 * This is a template adapter that combines two I/O Stats strategies together
 * into a single class.  It multiply inherits a new class from the two classes
 * to create a single new class.  It overrides the virtual functions read_s(),
 * readn_s(), and write_s() to create new functions that call the parent
 * functions in the order that they were specified as template parameters.
 * All types here must be based on the vpr::BaseIOStatsStrategy_i interface.
 *
 * @param BASE_ONE The first base class.
 * @param BASE_TWO The second base class.
 */
template < class BASE_ONE, class BASE_TWO >
class IOStatsStrategyAdapter : public BASE_ONE, public BASE_TWO
{
public:
   virtual void read_s(void* buffer, const vpr::Uint32 length,
                       vpr::Uint32& bytesRead,
                       const vpr::Interval timeout = vpr::Interval::NoTimeout)
   {
      BASE_ONE::read_s(buffer, length, bytesRead, timeout);
      BASE_TWO::read_s(buffer, length, bytesRead, timeout);
   }

   virtual void readn_s(void* buffer, const vpr::Uint32 length,
                        vpr::Uint32& bytesRead,
                        const vpr::Interval timeout = vpr::Interval::NoTimeout)
   {
      BASE_ONE::readn_s(buffer, length, bytesRead, timeout);
      BASE_TWO::readn_s(buffer, length, bytesRead, timeout);
   }

   virtual void write_s(const void* buffer, const vpr::Uint32 length,
                        vpr::Uint32& bytesWritten,
                        const vpr::Interval timeout = vpr::Interval::NoTimeout)
   {
      BASE_ONE::write_s(buffer, length, bytesWritten, timeout);
      BASE_TWO::write_s(buffer, length, bytesWritten, timeout);
   }
};

} // namespace vpr


#endif //BANDWIDTHSTATADAPTER_H
