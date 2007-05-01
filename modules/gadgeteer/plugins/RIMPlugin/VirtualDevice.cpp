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

#include <cluster/PluginConfig.h>
#include <plugins/RIMPlugin/VirtualDevice.h>

namespace cluster
{

VirtualDevice::VirtualDevice(const std::string& name, const vpr::GUID& id, const std::string& baseType, 
                             const std::string& hostname, gadget::InputPtr device)
   : mName(name)
   , mBaseType(baseType)
   , mRemoteHostname(hostname)
   , mId(id)
   , mDevice(device)
{}

VirtualDevice::~VirtualDevice()
{;}

void VirtualDevice::debugDump(int debug_level)
{
   vpr::DebugOutputGuard dbg_output(gadgetDBG_RIM,debug_level,
                              std::string("-------------- VirtualDevice --------------\n"),
                              std::string("-----------------------------------------\n"));

   vprDEBUG(gadgetDBG_RIM,debug_level) << "Local ID: " << mId.toString() << std::endl << vprDEBUG_FLUSH; 
   vprDEBUG(gadgetDBG_RIM,debug_level) << "Name:     " << mName << std::endl << vprDEBUG_FLUSH;
   vprDEBUG(gadgetDBG_RIM,debug_level) << "BaseType: " << mBaseType << std::endl << vprDEBUG_FLUSH;
}

} // End of cluster namespace
