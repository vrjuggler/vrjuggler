/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
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
 */


#include <Input/InputManager/vjDeviceInterface.h>
#include <Kernel/vjKernel.h>
#include <Input/InputManager/vjInputManager.h>


vjDeviceInterface::vjDeviceInterface()
: mProxyIndex(-1)
{
   mProxyName = "UnInitialized";
   mNameSet = false;
   vjDeviceInterface::addDevInterface(this);    // Keep referense to the interface
}

vjDeviceInterface::~vjDeviceInterface()
{
   removeDevInterface(this);     // Remove it from the list of active interfaces;
}


void vjDeviceInterface::init(std::string proxyName)
{
   mProxyName = proxyName;    // Set the name
   mNameSet = true;
   refresh();                 // Refresh the name
}


//! NOTE: If the interface does not have an initialized mProxyName, then don't try to refresh it
void vjDeviceInterface::refresh()
{
   int prev_proxy_index = mProxyIndex;    // Keep track of previous value

   // If it is not initialized, then don't try
   if(!mNameSet)
   { return; }

   mProxyIndex = vjKernel::instance()->getInputManager()->getProxyIndex(mProxyName);
   if (mProxyIndex == -1)
   {
      vjDEBUG(vjDBG_ALL,vjDBG_CONFIG_LVL) << "WARNING: vjDeviceInterface::refresh: could not find proxy: " << mProxyName.c_str() << endl << vjDEBUG_FLUSH;
      vjDEBUG(vjDBG_ALL,vjDBG_CONFIG_LVL) << "         Make sure the proxy exists in the current configuration." << endl << vjDEBUG_FLUSH;
      vjDEBUG(vjDBG_ALL,vjDBG_CONFIG_LVL) << "   referencing device interface will be stupified to point at dummy device." << endl << vjDEBUG_FLUSH;
   }
   else if((mProxyIndex != -1) && (prev_proxy_index == -1))   // ASSERT: We have just gotten a valid proxy to point to
   {
      vjDEBUG(vjDBG_ALL,vjDBG_CONFIG_LVL) << "vjDeviceInterface::refresh: Success: Now able to find proxy: " << mProxyName.c_str() << endl << vjDEBUG_FLUSH;
   }
}


void vjDeviceInterface::addDevInterface(vjDeviceInterface* dev)
{ mAllocatedDevices.push_back(dev); }

void vjDeviceInterface::removeDevInterface(vjDeviceInterface* dev)
{ mAllocatedDevices.push_back(dev); }

void vjDeviceInterface::refreshAllDevices()
{
   for(unsigned int i=0;i<mAllocatedDevices.size();i++)
      mAllocatedDevices[i]->refresh();
}

std::vector<vjDeviceInterface*> vjDeviceInterface::mAllocatedDevices = std::vector<vjDeviceInterface*>();
