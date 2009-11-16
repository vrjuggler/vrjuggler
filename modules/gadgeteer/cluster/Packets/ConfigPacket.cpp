/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2009 by Iowa State University
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

#include <gadget/Util/Debug.h>
#include <cluster/Packets/ConfigPacket.h>
#include <cluster/Packets/PacketFactory.h>

namespace cluster
{

CLUSTER_REGISTER_CLUSTER_PACKET_CREATOR(ConfigPacket);

ConfigPacket::ConfigPacket()
   : Packet(vpr::GUID("f3ea94e2-82fc-43f6-a57f-474d3fd1d6eb"))
{}

ConfigPacket::ConfigPacket(const std::string config, const vpr::Uint16 type)
   : Packet(vpr::GUID("f3ea94e2-82fc-43f6-a57f-474d3fd1d6eb"))
   , mConfig(config)
   , mType(type)
{
   // Create a Header for this packet with the correect type and size.
   mHeader = Header::create(Header::RIM_PACKET,
                            getPacketFactoryType(),
                            Header::RIM_PACKET_HEAD_SIZE 
                            + vpr::BufferObjectReader::STRING_LENGTH_SIZE
                            + mConfig.size()
                            + 2 /*mType*/,
                            0/*Field not curently used*/);

   // Serialize the given data.
   serialize();
}

ConfigPacketPtr ConfigPacket::create()
{
   return ConfigPacketPtr(new ConfigPacket());
}

ConfigPacketPtr ConfigPacket::create(const std::string config, const vpr::Uint16 type)
{
   return ConfigPacketPtr(new ConfigPacket(config, type));
}

void ConfigPacket::serialize()
{
   // Clear the data stream.
   mPacketWriter->getData()->clear();
   mPacketWriter->setCurPos(0);

   mPacketWriter->writeString(mConfig);
   mPacketWriter->writeUint16(mType);

   // Serialize the header.
   mHeader->prependSerializedHeader(mPacketWriter);
}

void ConfigPacket::parse()
{
   mPacketReader->setCurPos(0);

   mConfig = mPacketReader->readString();
   mType = mPacketReader->readUint16();
}

void ConfigPacket::printData(int debugLevel) const
{
   vprDEBUG_BEGIN(gadgetDBG_RIM,debugLevel) 
      <<  clrOutBOLD(clrYELLOW,"==== Config Packet Data ====\n") << vprDEBUG_FLUSH;
   
   Packet::printData(debugLevel);

   vprDEBUG(gadgetDBG_RIM,debugLevel) 
      << clrOutBOLD(clrYELLOW, "Config:       ") << mConfig
      << std::endl << vprDEBUG_FLUSH;
   vprDEBUG(gadgetDBG_RIM,debugLevel) 
      << clrOutBOLD(clrYELLOW, "Type          ") << mType
      << std::endl << vprDEBUG_FLUSH;

   vprDEBUG_END(gadgetDBG_RIM,debugLevel) 
      <<  clrOutBOLD(clrYELLOW,"====================================\n") << vprDEBUG_FLUSH;
}

}   // end namespace gadget
