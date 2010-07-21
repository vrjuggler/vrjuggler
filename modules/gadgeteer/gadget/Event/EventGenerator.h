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
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _GADGET_EVENT_GENERATOR_H_
#define _GADGET_EVENT_GENERATOR_H_

#include <gadget/gadgetConfig.h>

#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>

#include <gadget/Event/EventGeneratorPtr.h>


namespace gadget
{

class GADGET_CLASS_API EventGenerator
   : public boost::enable_shared_from_this<EventGenerator>
   , private boost::noncopyable
{
protected:
   EventGenerator();

public:
   virtual ~EventGenerator();

   virtual void emitEvents() = 0;
};

}


#endif /* _GADGET_EVENT_GENERATOR_H_ */
