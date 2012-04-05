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

#include <gadget/gadgetConfig.h>
#include <jccl/Config/ConfigElement.h>
#include <gadget/Util/Debug.h>
#include <gadget/Type/HatProxy.h>

namespace gadget
{

HatProxy::HatProxy(const std::string& deviceName, const int unitNum) 
   : base_type(deviceName, unitNum)
{
   /* Do nothing. */ ;
}

HatProxyPtr HatProxy::create(const std::string& deviceName,
                             const int unitNum)
{
   return HatProxyPtr(new HatProxy(deviceName, unitNum));
}

HatProxy::~HatProxy()
{
   ;
}

std::string HatProxy::getElementType()
{
   return "hat_proxy";
}

void HatProxy::updateData()
{
   if (! isStupefied())
   {
      const HatState::State old_state(mData.getValue());

      // Make sure dependencies are updated.
      getProxiedInputDevice()->updateDataIfNeeded();

      mData = getNextState(old_state,
                           mTypedDevice->getHatData(mUnit).getValue());
   }
}

HatState::State
HatProxy::getNextState(const HatState::State prevState,
                       const HatState::State curState)
{
   /*
   HatState::State next_state(HatState::CENTERED);
   
   switch (prevState)
   {
      case DigitalState::CENTERED:
         // Digital::TOGGLE_ON -> Button now pressed
         // Digital::OFF       -> Button still released
         next_state = curState ? DigitalState::TOGGLE_ON : DigitalState::OFF;
         break;
      case DigitalState::ON:
         // Digital::ON         -> Button still pressed
         // Digital::TOGGLE_OFF -> Button now released
         next_state = curState ? DigitalState::ON : DigitalState::TOGGLE_OFF;
         break;
      case DigitalState::TOGGLE_ON:
         // Digital::ON         -> Button still pressed
         // Digital::TOGGLE_OFF -> Button now released
         next_state = curState ? DigitalState::ON : DigitalState::TOGGLE_OFF;
         break;
      case DigitalState::TOGGLE_OFF:
         // Digital::TOGGLE_ON -> Button now pressed
         // Digital::OFF       -> Button still released
         next_state = curState ? DigitalState::TOGGLE_ON : DigitalState::OFF;
         break;
   }
   
   return next_state;
   */
   return curState;
}

} // End of gadget namespace
