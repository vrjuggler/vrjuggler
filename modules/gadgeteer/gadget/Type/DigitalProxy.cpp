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

#include <gadget/gadgetConfig.h>
#include <jccl/Config/ConfigElement.h>
#include <gadget/Util/Debug.h>
#include <gadget/Type/DigitalProxy.h>

namespace gadget
{

DigitalProxy::DigitalProxy(const std::string& deviceName, const int unitNum) 
   : base_type(deviceName, unitNum)
{
   /* Do nothing. */ ;
}

DigitalProxyPtr DigitalProxy::create(const std::string& deviceName,
                                     const int unitNum)
{
   return DigitalProxyPtr(new DigitalProxy(deviceName, unitNum));
}

DigitalProxy::~DigitalProxy()
{
   ;
}

std::string DigitalProxy::getElementType()
{
   return "digital_proxy";
}

void DigitalProxy::updateData()
{
   if (! isStupefied())
   {
      const DigitalState::State old_state(mData.getValue());

      // Make sure dependencies are updated.
      getProxiedInputDevice()->updateDataIfNeeded();

      mData = mTypedDevice->getDigitalData(mUnit);
      const DigitalState::State new_state(mData.getValue());

      switch (old_state)
      {
         case DigitalState::OFF:
            // Digital::TOGGLE_ON -> Button now pressed
            // Digital::OFF       -> Button still released
            mData = new_state ? DigitalState::TOGGLE_ON : DigitalState::OFF;
            break;
         case DigitalState::ON:
            // Digital::ON         -> Button still pressed
            // Digital::TOGGLE_OFF -> Button now released
            mData = new_state ? DigitalState::ON : DigitalState::TOGGLE_OFF;
            break;
         case DigitalState::TOGGLE_ON:
            // Digital::ON         -> Button still pressed
            // Digital::TOGGLE_OFF -> Button now released
            mData = new_state ? DigitalState::ON : DigitalState::TOGGLE_OFF;
            break;
         case DigitalState::TOGGLE_OFF:
            // Digital::TOGGLE_ON -> Button now pressed
            // Digital::OFF       -> Button still released
            mData = new_state ? DigitalState::TOGGLE_ON : DigitalState::OFF;
            break;
      }
   }
}

} // End of gadget namespace
