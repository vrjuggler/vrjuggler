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

#include <gadget/gadgetConfig.h>
#include <jccl/Config/ConfigElement.h>
#include <gadget/Util/Debug.h>
#include <gadget/Type/GestureProxy.h>


namespace gadget
{

GestureProxy::GestureProxy(const std::string& deviceName)
   : TypedProxy<Gesture>(deviceName)
{
   /* Do nothing. */ ;
}

GestureProxyPtr GestureProxy::create(const std::string& deviceName)
{
   return GestureProxyPtr(new GestureProxy(deviceName));
}

GestureProxy::~GestureProxy()
{
   /* Do nothing. */ ;
}

vpr::Interval GestureProxy::getTimeStamp() const
{
   // XXX: Broken for now, this is a case similar to the KeyboardMouse type in
   //      that it does not point to one data element like digital, analog,
   //      and position.
   return vpr::Interval();
}

std::string GestureProxy::getElementType()
{
   return "gesture_proxy";
}

bool GestureProxy::config(jccl::ConfigElementPtr element)
{
vpr::DebugOutputGuard dbg_output(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL,
      std::string("------------------ GESTURE PROXY config() -----------------\n"),
      std::string("\n"));

   vprASSERT(element->getID() == getElementType());

   if ( ! Proxy::config(element) )
   {
      return false;
   }

   mDeviceName = element->getProperty<std::string>("device");

   refresh();

   return true;
}

} // End of gadget namespace
