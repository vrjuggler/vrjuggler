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
#include <gadget/Type/AnalogProxy.h>


namespace gadget
{

AnalogProxy::AnalogProxy(const std::string& deviceName, const int unitNum)
   : TypedProxy<Analog>(deviceName)
   , mUnitNum(unitNum)
   , mRawData(-1.0f)
   , mData(0.0f)
{
   /* Do nothing. */ ;
}

AnalogProxyPtr AnalogProxy::create(const std::string& deviceName,
                                   const int unitNum)
{
   return AnalogProxyPtr(new AnalogProxy(deviceName, unitNum));
}

AnalogProxy::~AnalogProxy()
{
   /* Do nothing. */ ;
}

void AnalogProxy::updateData()
{
   if ( ! isStupefied() )
   {
      // Make sure dependencies are updated.
      getProxiedInputDevice()->updateDataIfNeeded();

      mRawData = mTypedDevice->getAnalogData(mUnitNum);
      mData    = mTypedDevice->normalize(mRawData.getAnalog());
   }
}

std::string AnalogProxy::getElementType()
{
   return "analog_proxy";
}

vpr::Interval AnalogProxy::getTimeStamp() const
{
   return mRawData.getTime();
}

bool AnalogProxy::config(jccl::ConfigElementPtr element)
{
vpr::DebugOutputGuard dbg_output(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL,
                                 std::string("----------- configuring ANALOG PROXY -----------------\n"),
                                 std::string("----------- exit: configuring analog proxy -----------\n"));
   vprASSERT(element->getID() == getElementType());

   if ( ! Proxy::config(element) )
   {
      return false;
   }

   mUnitNum = element->getProperty<int>("unit");
   mDeviceName = element->getProperty<std::string>("device");

   refresh();     // Refresh the device now that we have something to point at

   return true;
}

} // End of gadget namespace
