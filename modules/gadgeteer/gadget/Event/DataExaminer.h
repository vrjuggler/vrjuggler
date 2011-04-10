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

#ifndef _GADGET_EVENT_DATA_EXAMINER_H_
#define _GADGET_EVENT_DATA_EXAMINER_H_

#include <gadget/gadgetConfig.h>

#include <vector>


namespace gadget
{

namespace event
{

/**
 * @tparam EventTag       The tag identifying the kind of event. This is used
 *                        to provide a mechanism for specializing this type
 *                        based on specific needs.
 * @tparam DataType       The type of a sample received from a device proxy.
 * @tparam CollectionType The type that handles event collection. This is
 *                        intended to be the base class, but specializations
 *                        may find other ways of putting this to use.
 *
 * @since 2.1.16
 */
template<typename EventTag, typename DataType, typename CollectionType>
class DataExaminer
   : public CollectionType
{
public:
   /**
    * Stores every data sample received from a device proxy to be emitted
    * later as an event.
    */
   void examine(const DataType& d)
   {
      addEvent(d);
   }
};

}

}


#endif /* _GADGET_EVENT_DATA_EXAMINER_H_ */
