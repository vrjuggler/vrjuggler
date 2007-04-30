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

#include <vpr/IO/BufferObjectWriter.h>
#include <cluster/PluginConfig.h>
#include <cluster/Packets/DataPacket.h>
#include <cluster/ClusterManager.h>
#include <gadget/AbstractNetworkManager.h>
#include <gadget/Node.h>
#include <gadget/Type/Input.h>
#include <gadget/Util/Debug.h>
#include <plugins/RIMPlugin/DeviceServer.h>

namespace cluster
{

DeviceServer::DeviceServer(const std::string& name, gadget::Input* device,
                           const vpr::GUID& pluginGuid)
   : mName(name)
   , mDevice(device)
   , mPluginGUID(pluginGuid)
   , mDataPacket(NULL)
   , mBufferObjectWriter(NULL)
   , mDeviceData(NULL)
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

   mDeviceData = new std::vector<vpr::Uint8>;
   mDataPacket = new cluster::DataPacket(pluginGuid, mId, mDeviceData);
   mBufferObjectWriter = new vpr::BufferObjectWriter(mDeviceData);
}

DeviceServer::~DeviceServer()
{
   delete mDataPacket;
   // mDataPacket will clean up the memory that mDeviceData points
   // to since mDataPacket contains a reference to the same memory.
   mDeviceData = NULL;
   delete mBufferObjectWriter;
   mBufferObjectWriter = NULL;
}

void DeviceServer::send() const
{
   vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL)
      << clrOutBOLD(clrMAGENTA,"DeviceServer::send()")
      << "Sending Device Data for: " << getName() << std::endl
      << vprDEBUG_FLUSH;

   gadget::AbstractNetworkManager::node_list_t nodes = cluster::ClusterManager::instance()->getNetwork()->getNodes();
   for (gadget::AbstractNetworkManager::node_list_t::iterator itr = nodes.begin(); itr != nodes.end(); itr++)
   {
      try
      {
         (*itr)->send(mDataPacket);
      }
      catch( cluster::ClusterException cluster_exception )
      {
         vprDEBUG( gadgetDBG_RIM, vprDBG_CONFIG_LVL )
            << "DeviceServer::send() Caught an exception!"
            << std::endl << vprDEBUG_FLUSH;
         vprDEBUG( gadgetDBG_RIM, vprDBG_CONFIG_LVL )
            << clrOutBOLD(clrRED, "ERROR:") << cluster_exception.what()
            << std::endl << vprDEBUG_FLUSH;
         vprDEBUG( gadgetDBG_RIM, vprDBG_CONFIG_LVL )
            << "DeviceServer::send() We have lost our connection to: "
            << (*itr)->getName() << ":" << (*itr)->getPort()
            << std::endl << vprDEBUG_FLUSH;

         (*itr)->setStatus( gadget::Node::DISCONNECTED );
         (*itr)->shutdown();

         debugDump( vprDBG_CONFIG_LVL );
      }
   }
}

void DeviceServer::updateLocalData()
{
   // -BufferObjectWriter
   mBufferObjectWriter->getData()->clear();
   mBufferObjectWriter->setCurPos(0);

   // This updates the mDeviceData which both mBufferedObjectReader and
   // mDevicePacket point to.
   mDevice->writeObject(mBufferObjectWriter);

   // We must update the size of the actual data that we are going to send
   mDataPacket->getHeader()->setPacketLength(
      cluster::Header::RIM_PACKET_HEAD_SIZE
         + 16 /*Plugin GUID*/
         + 16 /*Plugin GUID*/
         + mDeviceData->size()
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
