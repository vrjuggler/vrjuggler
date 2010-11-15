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

#include <gadget/gadgetConfig.h>
#include <jccl/Config/ConfigElement.h>
#include <gadget/Util/Debug.h>
#include <gadget/Type/CommandProxy.h>

namespace gadget
{

CommandProxy::CommandProxy(const std::string& deviceName,
                           const int unitNum)
   : base_type(deviceName, unitNum)
{
   /* Do nothing. */ ;
}

CommandProxyPtr CommandProxy::create(const std::string& deviceName,
                                     const int unitNum)
{
   return CommandProxyPtr(new CommandProxy(deviceName, unitNum));
}

CommandProxy::~CommandProxy()
{
   /* Do nothing. */ ;
}

std::string CommandProxy::getElementType()
{
   return "command_proxy";
}

void CommandProxy::updateData()
{
   if (!isStupefied())
   {
      // Make sure dependencies are updated.
      getProxiedInputDevice()->updateDataIfNeeded();
      mData = mTypedDevice->getCommandData(mUnit);
   }
}

} // End of gadget namespace
