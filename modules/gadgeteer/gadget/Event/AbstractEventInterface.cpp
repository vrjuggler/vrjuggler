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

#include <algorithm>
#include <boost/bind.hpp>

#include <vpr/Util/Assert.h>

#include <gadget/Event/AbstractEventInterface.h>


namespace gadget
{

std::vector<AbstractEventInterface*> AbstractEventInterface::sAllocatedInterfaces;

AbstractEventInterface::AbstractEventInterface()
{
   sAllocatedInterfaces.push_back(this);
}

AbstractEventInterface::~AbstractEventInterface()
{
   // Attempt to find the device, if found, erase it, if not, then assert
   std::vector<AbstractEventInterface*>::iterator i =
      std::find(sAllocatedInterfaces.begin(), sAllocatedInterfaces.end(),
                this);
   vprASSERT(i != sAllocatedInterfaces.end());
   sAllocatedInterfaces.erase(i);
}

void AbstractEventInterface::refreshAllInterfaces()
{
   std::for_each(sAllocatedInterfaces.begin(), sAllocatedInterfaces.end(),
                 boost::bind(&AbstractEventInterface::refresh, _1));
}

}
