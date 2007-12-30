/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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
   : TypedProxy<Digital>(deviceName)
   , mUnitNum(unitNum)
   , mData(0)
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

bool DigitalProxy::config(jccl::ConfigElementPtr element)
{
vpr::DebugOutputGuard dbg_output(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL,
                              std::string("----------- configuring DIGITAL PROXY -----------------\n"),
                              std::string("----------- exit: configuring digital proxy -----------\n"));

   vprASSERT(element->getID() == getElementType());

   if( ! Proxy::config(element) )
   {
      return false;
   }

   mUnitNum = element->getProperty<int>("unit");
   mDeviceName = element->getProperty<std::string>("device");

   refresh();
   return true;
}



void DigitalProxy::updateData()
{
   if (!isStupefied())
   {
      int old_state = mData.getDigital();

      // Make sure dependencies are updated.
      getProxiedInputDevice()->updateDataIfNeeded();

      mData = mTypedDevice->getDigitalData(mUnitNum);
      int new_state = mData.getDigital();

      if (Digital::OFF == old_state)
      {
         if (new_state)     // Button now pressed
            mData = Digital::TOGGLE_ON;
         else              // Button now released
            mData = Digital::OFF;
      }
      else if (Digital::ON == old_state)
      {
         if (new_state)     // Button now pressed
            mData = Digital::ON;
         else              // Button now released
            mData = Digital::TOGGLE_OFF;
      }
      else if (Digital::TOGGLE_ON == old_state)
      {
         if (new_state)     // Button now pressed
            mData = Digital::ON;
         else              // Button now released
            mData = Digital::TOGGLE_OFF;
      }
      else if (Digital::TOGGLE_OFF == old_state)
      {
         if (new_state)     // Button now pressed
            mData = Digital::TOGGLE_ON;
         else              // Button now released
            mData = Digital::OFF;
      }
   }
}

vpr::Interval DigitalProxy::getTimeStamp() const
{
   return mData.getTime();
}

} // End of gadget namespace
