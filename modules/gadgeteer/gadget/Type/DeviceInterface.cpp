/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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
#include <gadget/Type/DeviceInterface.h>
#include <gadget/InputManager.h>

namespace gadget
{

BaseDeviceInterface::BaseDeviceInterface()
: mProxyPtr(NULL), mProxyName("UnInitialized"), mNameSet(false)
{
   BaseDeviceInterface::addDevInterface(this);    // Keep referense to the interface
}

BaseDeviceInterface::~BaseDeviceInterface()
{
   removeDevInterface(this);     // Remove it from the list of active interfaces;
}


void BaseDeviceInterface::init(std::string proxyName)
{
   mProxyName = proxyName;    // Set the name
   mNameSet = true;
   refresh();                 // Refresh the name
}


//! NOTE: If the interface does not have an initialized mProxyName, then don't try to refresh it
void BaseDeviceInterface::refresh()
{
   Proxy* prev_proxy_ptr = mProxyPtr;    // Keep track of previous value

   // If it is not initialized, then don't try
   if(!mNameSet)
   { return; }

   mProxyPtr = InputManager::instance()->getProxy(mProxyName); //XXX: Kernel::instance()->getInputManager()->getProxy(mProxyName);

   if (NULL == mProxyPtr)
   {
      vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL)
         << "WARNING: BaseDeviceInterface::refresh: could not find proxy: "
         << mProxyName.c_str() << std::endl << vprDEBUG_FLUSH;
      vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL)
         << "         Make sure the proxy exists in the current configuration."
         << std::endl << vprDEBUG_FLUSH;
      vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL)
         << "   referencing device interface will be stupified to point at dummy device."
         << std::endl << vprDEBUG_FLUSH;
   }
   else if((NULL != mProxyPtr) && (NULL == prev_proxy_ptr))   // ASSERT: We have just gotten a valid proxy to point to
   {
      vprDEBUG(vprDBG_ALL,vprDBG_WARNING_LVL)
         << "vjBaseDeviceInterface::refresh: Success: Now able to find proxy: "
         << mProxyName.c_str() << std::endl << vprDEBUG_FLUSH;
   }
}


void BaseDeviceInterface::addDevInterface(BaseDeviceInterface* dev)
{ mAllocatedDevices.push_back(dev); }

void BaseDeviceInterface::removeDevInterface(BaseDeviceInterface* dev)
{ mAllocatedDevices.push_back(dev); }

void BaseDeviceInterface::refreshAllDevices()
{
   for(unsigned int i=0;i<mAllocatedDevices.size();i++)
   {
      BaseDeviceInterface* dev = mAllocatedDevices[i];
      dev->refresh();
   }
}

std::vector<BaseDeviceInterface*> BaseDeviceInterface::mAllocatedDevices = std::vector<BaseDeviceInterface*>();

};
