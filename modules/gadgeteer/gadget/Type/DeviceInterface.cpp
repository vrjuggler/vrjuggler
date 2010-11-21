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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <iomanip>

#include <gadget/gadgetConfig.h>
#include <gadget/Type/DeviceInterface.h>
#include <gadget/InputManager.h>
#include <gadget/Type/Proxy.h>

namespace gadget
{

BaseDeviceInterface::BaseDeviceInterface()
   : mProxyPtr()
   , mProxyName("UnInitialized")
   , mNameSet(false)
{
   // Keep reference to the interface.
   BaseDeviceInterface::addDevInterface(this);
}

BaseDeviceInterface::~BaseDeviceInterface()
{
   // Remove it from the list of active interfaces.
   BaseDeviceInterface::removeDevInterface(this);
}


BaseDeviceInterface::BaseDeviceInterface(const BaseDeviceInterface& other)
   : mProxyPtr(other.mProxyPtr)
   , mProxyName(other.mProxyName)
   , mNameSet(other.mNameSet)
{
   // Keep reference to the interface.
   BaseDeviceInterface::addDevInterface(this);
}

void BaseDeviceInterface::init(const std::string& proxyName)
{
   mProxyName = proxyName;    // Set the name
   mNameSet = true;
   refresh();                 // Refresh the name
}

// NOTE: If the interface does not have an initialized mProxyName, then
// don't try to refresh it.
void BaseDeviceInterface::refresh()
{
   ProxyPtr prev_proxy_ptr = mProxyPtr;    // Keep track of previous value

   // If it is not initialized, then don't try
   if ( ! mNameSet )
   {
      return;
   }

   mProxyPtr = InputManager::instance()->getProxy(mProxyName);

   if (NULL == mProxyPtr.get())
   {
      vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL)
         << "WARNING: DeviceInterface::refresh: could not find proxy: "
         << mProxyName.c_str() << std::endl << vprDEBUG_FLUSH;
      vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL)
         << "         Make sure the proxy exists in the current configuration."
         << std::endl << vprDEBUG_FLUSH;
      vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL)
         << "   referencing device interface will be stupefied to point at dummy device."
         << std::endl << vprDEBUG_FLUSH;
   }
   // ASSERT: We have just gotten a valid proxy to point to
   else if((NULL != mProxyPtr.get()) && (NULL == prev_proxy_ptr.get()))
   {
      const int item_width(25+12);
      //const int type_width(20);

      //vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_STATUS_LVL)
      //   << "DeviceInterface now able to find proxy: "
      //   << mProxyName.c_str() << "               [ "
      //   << clrSetNORM(clrGREEN) << "OK" << clrRESET << " ]"
      //   << std::endl << vprDEBUG_FLUSH;

      //std::string device_name("");
      //Input* deviceptr = mProxyPtr->getProxiedInputDevice();
      //if (NULL != deviceptr)
      //{
      //   device_name = deviceptr->getInstanceName();
      //}

      vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_STATUS_LVL)
         << "DeviceInterface found proxy: "
         << std::setiosflags(std::ios::right)
         << std::setfill(' ') << std::setw(item_width) << mProxyName
         << std::resetiosflags(std::ios::right) << "  ";
      vprDEBUG_CONTnl(vprDBG_ALL,vprDBG_CONFIG_STATUS_LVL)
         << "[ " << clrSetNORM(clrGREEN) << "OK" << clrRESET << " ]";
      vprDEBUG_CONTnl(vprDBG_ALL,vprDBG_CONFIG_STATUS_LVL)
         << std::endl << vprDEBUG_FLUSH;
   }
}

void BaseDeviceInterface::addDevInterface(BaseDeviceInterface* dev)
{
   mAllocatedDevices.push_back(dev);
}

void BaseDeviceInterface::removeDevInterface(BaseDeviceInterface* dev)
{
   // Attempt to find the device, if found, erase it, if not, then assert
   std::vector<BaseDeviceInterface*>::iterator found_dev =
      std::find(mAllocatedDevices.begin(), mAllocatedDevices.end(), dev);
   vprASSERT(found_dev != mAllocatedDevices.end() &&
             "Tried to remove non-registered interface");

   if ( mAllocatedDevices.end() != found_dev )
   {
      mAllocatedDevices.erase(found_dev);
   }
}

void BaseDeviceInterface::refreshAllInterfaces()
{
   for ( unsigned int i = 0; i < mAllocatedDevices.size(); ++i )
   {
      BaseDeviceInterface* dev = mAllocatedDevices[i];
      dev->refresh();
   }
}

std::vector<BaseDeviceInterface*> BaseDeviceInterface::mAllocatedDevices;

} // End of gadget namespace
