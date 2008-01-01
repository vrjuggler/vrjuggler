/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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

#include <vpr/IO/BufferObjectWriter.h>
#include <cluster/PluginConfig.h>
#include <cluster/Packets/DataPacket.h>
#include <cluster/ClusterManager.h>
#include <gadget/Node.h>
#include <gadget/Type/Input.h>
#include <gadget/Util/Debug.h>
#include <plugins/RIMPlugin/DeviceServer.h>

namespace cluster
{

DeviceServer::DeviceServer(const std::string& name, gadget::InputPtr device,
                           const vpr::GUID& pluginGuid)
   : mName(name)
   , mPluginGUID(pluginGuid)
   , mDevice(device)
   , mDataPacket()
{
   vpr::GUID temp;
   temp.generate();

   do
   {
      mId.generate();   // Generate a unique ID for this device
      vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
         << "[DeviceServer] Invalid GUID, generating a new one."
         << std::endl << vprDEBUG_FLUSH;
   }
   while(temp == mId);

   mDataPacket = cluster::DataPacket::create(pluginGuid, mId);
}

DeviceServerPtr DeviceServer::create(const std::string& name, gadget::InputPtr device,
                                     const vpr::GUID& pluginGuid)
{
   return DeviceServerPtr(new DeviceServer(name, device, pluginGuid));
}

DeviceServer::~DeviceServer()
{
}

void DeviceServer::send() const
{
   vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL)
      << clrOutBOLD(clrMAGENTA,"DeviceServer::send()")
      << "Sending Device Data for: " << getName() << std::endl
      << vprDEBUG_FLUSH;

   cluster::ClusterManager::instance()->getNetwork()->sendToAll(mDataPacket);
}

void DeviceServer::updateLocalData()
{
   vprASSERT(NULL != mDevice.get() && "Can't have a NULL device.");

   mDataPacket->serialize(*mDevice);

   // We must update the size of the actual data that we are going to send
   mDataPacket->getHeader()->setPacketLength(
      cluster::Header::RIM_PACKET_HEAD_SIZE
         + mDataPacket->getData().size()
   );

   // We must serialize the header again so that we can reset the size.
   mDataPacket->getHeader()->serializeHeader();
}

void DeviceServer::debugDump(int debugLevel) const
{
   vpr::DebugOutputGuard dbg_output(
      gadgetDBG_RIM, debugLevel,
      "-------------- DeviceServer --------------\n",
      "------------------------------------------\n"
   );

   vprDEBUG(gadgetDBG_RIM, debugLevel)
      << "Name:     " << mName << std::endl << vprDEBUG_FLUSH;
}

} // End of cluster namespace
