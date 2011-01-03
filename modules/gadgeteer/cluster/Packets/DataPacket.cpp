/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
#include <gadget/Util/Debug.h>

#include <cluster/Packets/DataPacket.h>
#include <cluster/Packets/PacketFactory.h>

#include <vpr/System.h>
#include <vpr/Util/Assert.h>


namespace cluster
{

CLUSTER_REGISTER_CLUSTER_PACKET_CREATOR(DataPacket);

DataPacket::DataPacket()
   : Packet(vpr::GUID())
{;}

DataPacket::DataPacket(const vpr::GUID& pluginId, const vpr::GUID& objectId)
   : Packet(pluginId)
   , mObjectId(objectId)
{      
   // Create a Header for this packet with the correect type and size.
   mHeader = Header::create(Header::RIM_PACKET,
                            Header::RIM_DATA_PACKET,
                            Header::RIM_PACKET_HEAD_SIZE 
                            + 16 /*Plugin GUID*/
                            + 16 /*Object GUID*/
                            + 0 /* Empty now, but needs updated later. */,
                            0/*Field not curently used*/);

   // NOTE: We don't serialize here because we want to reuse the data packet.
}

DataPacketPtr DataPacket::create()
{
   return DataPacketPtr(new DataPacket());
}

DataPacketPtr DataPacket::create(const vpr::GUID& pluginId, const vpr::GUID& objectId)
{
   return DataPacketPtr(new DataPacket(pluginId, objectId));
}
   
DataPacket::~DataPacket()
{
}

void DataPacket::serialize(vpr::SerializableObject& object)
{
   // Clear data stream since header is at beginning
   mPacketWriter->getData()->clear();
   mPacketWriter->setCurPos( 0 );

   // Serialize plugin GUID.
   mPluginId.writeObject(mPacketWriter);

   // Serialize device GUID.
   mObjectId.writeObject(mPacketWriter);

   object.writeObject(mPacketWriter);

   // Serialize the header.
   mHeader->prependSerializedHeader(mPacketWriter);
}

void DataPacket::parse()
{
   mPacketReader->setCurPos(0);

   // De-Serialize plugin GUID
   mPluginId.readObject(mPacketReader);

   // De-Serialize plugin GUID
   mObjectId.readObject(mPacketReader);

   // There is extra data on the buffer for the object itself.
}

void DataPacket::printData(int debugLevel) const
{
   vprDEBUG_BEGIN(gadgetDBG_RIM,debugLevel) 
      <<  clrOutBOLD(clrYELLOW,"==== Device Data Packet ====\n") << vprDEBUG_FLUSH;
   
   Packet::printData(debugLevel);

   vprDEBUG(gadgetDBG_RIM,debugLevel) 
      << clrOutBOLD(clrYELLOW, "Plugin ID: ") << mPluginId.toString()
      << std::endl << vprDEBUG_FLUSH;
   vprDEBUG(gadgetDBG_RIM,debugLevel) 
      << clrOutBOLD(clrYELLOW, "Object ID: ") << mObjectId.toString()
      << std::endl << vprDEBUG_FLUSH;

   vprDEBUG_END(gadgetDBG_RIM,debugLevel) 
      <<  clrOutBOLD(clrYELLOW,"============================\n") << vprDEBUG_FLUSH;
}

} // end namespace cluster
