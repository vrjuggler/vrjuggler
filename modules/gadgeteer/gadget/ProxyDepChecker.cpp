/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <gadget/gadgetConfig.h>

#include <gadget/Type/AnalogProxy.h>
#include <gadget/Type/DigitalProxy.h>
#include <gadget/Type/PositionProxy.h>
#include <gadget/Type/GloveProxy.h>
#include <gadget/Type/GestureProxy.h>
//#include <gadget/Type/EventWindowProxy.h>
#include <gadget/Type/KeyboardMouseProxy.h>
#include <gadget/ProxyDepChecker.h>

namespace gadget
{

bool ProxyDepChecker::canHandle(jccl::ConfigElementPtr element)
{
   std::string element_type(element->getID());

   bool ret_val;
   ret_val = ((element_type == AnalogProxy::getElementType()) ||
              (element_type == DigitalProxy::getElementType()) ||
              (element_type == GestureProxy::getElementType()) ||
              (element_type == GloveProxy::getElementType()) ||
              (element_type == KeyboardMouseProxy::getElementType()) ||
              (element_type == PositionProxy::getElementType()));

   return ret_val;
}


} // End of gadget namespace
