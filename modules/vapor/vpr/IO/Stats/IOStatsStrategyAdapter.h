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
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
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
#include <vpr/Util/Status.h>

namespace vpr
{

/**
 * This is a template adapter that combines two IO Stats strategies together into a single class
 * 
 * It multiply inherits a new class from the two classes to create a single new class
 *
 * It overrides the virtual functions read_i, readn_i, and write_i to create new functions that call
 * the parent functions in the order that they were specified as template parameters
 * 
 * It does this by over riding the virtual functions read_i, readn_i, and write_i. 
 *
 * All types here must be based on the BaseIOStatsStrategy_i interface
 */
template < class BASE_ONE, class BASE_TWO >
class IOStatsStrategyAdapter : public BASE_ONE, public BASE_TWO
{
public:
    
   
   virtual void read_s(Status& status,
                         void* buffer, const vpr::Uint32 length,
                          vpr::Uint32& bytes_read,
                          const vpr::Interval timeout = vpr::Interval::NoTimeout)
   {
      BASE_ONE::read_s(status, buffer, length, bytes_read, timeout);
      BASE_TWO::read_s(status, buffer, length, bytes_read, timeout);
   }

   virtual void readn_s(Status& status,
                          void* buffer, const vpr::Uint32 length,
                           vpr::Uint32& bytes_read,
                           const vpr::Interval timeout = vpr::Interval::NoTimeout)
   {
      BASE_ONE::readn_s(status, buffer, length, bytes_read, timeout);
      BASE_TWO::readn_s(status, buffer, length, bytes_read, timeout);
   }

   virtual void write_s(Status& status,
                          const void* buffer, const vpr::Uint32 length,
                           vpr::Uint32& bytes_written,
                           const vpr::Interval timeout = vpr::Interval::NoTimeout)
   {
      BASE_ONE::write_s(status, buffer, length, bytes_written, timeout);
      BASE_TWO::write_s(status, buffer, length, bytes_written, timeout);
   }
};

}; // namespace vpr

#endif //BANDWIDTHSTATADAPTER_H

