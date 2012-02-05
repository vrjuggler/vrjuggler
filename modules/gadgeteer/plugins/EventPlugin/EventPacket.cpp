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

#include <cluster/PluginConfig.h>

#include <gadget/Util/Debug.h>

#include <plugins/EventPlugin/EventPacket.h>
#include <cluster/Packets/PacketFactory.h>

#include <vpr/System.h>
#include <vpr/Util/Assert.h>


namespace cluster
{

CLUSTER_REGISTER_CLUSTER_PACKET_CREATOR(EventPacket);

EventPacket::EventPacket()
   : Packet(vpr::GUID())
{;}

EventPacket::EventPacket(const vpr::GUID& pluginId, const std::string& name)
   : Packet(pluginId)
   , mName(name)
{      
   // Create a Header for this packet with the correect type and size.
   mHeader = Header::create(Header::RIM_PACKET,
                            Header::RIM_DATA_PACKET,
                            Header::RIM_PACKET_HEAD_SIZE 
                            + 16 /*Plugin GUID*/
                            + vpr::BufferObjectReader::STRING_LENGTH_SIZE
                            + mName.size()
                            + 0 /* Empty now, but needs updated later. */,
                            0/*Field not curently used*/);

   // Serialize the given data.
   serialize();
}

EventPacketPtr EventPacket::create()
{
   return EventPacketPtr(new EventPacket());
}

EventPacketPtr EventPacket::create(const vpr::GUID& pluginId, const std::string& name)
{
   return EventPacketPtr(new EventPacket(pluginId, name));
}
   
EventPacket::~EventPacket()
{
}

void EventPacket::serialize()
{
   // Clear data stream since header is at beginning
   mPacketWriter->getData()->clear();
   mPacketWriter->setCurPos( 0 );

   // Serialize plugin GUID.
   mPluginId.writeObject(mPacketWriter);

   mPacketWriter->writeString(mName);

   // Serialize the header.
   mHeader->prependSerializedHeader(mPacketWriter);
}

void EventPacket::parse()
{
   mPacketReader->setCurPos(0);

   // De-Serialize plugin GUID
   mPluginId.readObject(mPacketReader);

   mName = mPacketReader->readString();
}

void EventPacket::printData(int debugLevel) const
{
   vprDEBUG_BEGIN(gadgetDBG_RIM,debugLevel) 
      <<  clrOutBOLD(clrYELLOW,"==== Event Packet ====\n") << vprDEBUG_FLUSH;
   
   Packet::printData(debugLevel);

   vprDEBUG(gadgetDBG_RIM,debugLevel) 
      << clrOutBOLD(clrYELLOW, "Plugin ID: ") << mPluginId.toString()
      << std::endl << vprDEBUG_FLUSH;
   vprDEBUG(gadgetDBG_RIM,debugLevel) 
      << clrOutBOLD(clrYELLOW, "Name: ") << mName
      << std::endl << vprDEBUG_FLUSH;

   vprDEBUG_END(gadgetDBG_RIM,debugLevel) 
      <<  clrOutBOLD(clrYELLOW,"============================\n") << vprDEBUG_FLUSH;
}

} // end namespace cluster
