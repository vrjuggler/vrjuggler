/*
 *  File:          $RCSfile$
 *  Date modified: $Date$
 *  Version:       $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                             Patrick Hartling
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                  Copyright (C) - 1997, 1998, 1999, 2000
 *              Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#include <Input/InputManager/vjDeviceInterface.h>
#include <Kernel/vjKernel.h>
#include <Input/InputManager/vjInputManager.h>


vjDeviceInterface::vjDeviceInterface()
: mProxyIndex(-1)
{
   mProxyName = "UnInitialized";
   vjDeviceInterface::addDevInterface(this);    // Keep referense to the interface
}

vjDeviceInterface::~vjDeviceInterface()
{
   removeDevInterface(this);     // Remove it from the list of active interfaces;
}


void vjDeviceInterface::init(std::string proxyName)
{
   mProxyName = proxyName;    // Set the name
   refresh();                 // Refresh the name
}


//! NOTE: If the interface does not have an initialized mProxyName, then don't try to refresh it
void vjDeviceInterface::refresh()
{
   if(mProxyName == std::string("UnInitialized"))
   {
      ; // Don't refresh it
   }
   else
   {
      mProxyIndex = vjKernel::instance()->getInputManager()->getProxyIndex(mProxyName);
      if (mProxyIndex == -1)
      {
         vjDEBUG(vjDBG_ALL,0) << "ERROR: could not find proxy: " << mProxyName.c_str() << endl << vjDEBUG_FLUSH;
         vjASSERT(false);
         exit(1);
      }
   }
}


void vjDeviceInterface::addDevInterface(vjDeviceInterface* dev)
{ mAllocatedDevices.push_back(dev); }

void vjDeviceInterface::removeDevInterface(vjDeviceInterface* dev)
{ mAllocatedDevices.push_back(dev); }

void vjDeviceInterface::refreshAllDevices()
{
   for(int i=0;i<mAllocatedDevices.size();i++)
      mAllocatedDevices[i]->refresh();
}

std::vector<vjDeviceInterface*> vjDeviceInterface::mAllocatedDevices = std::vector<vjDeviceInterface*>();
