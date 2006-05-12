/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

#include <boost/concept_check.hpp>

#include <gadget/Util/Debug.h>
#include <cluster/Packets/ApplicationDataAck.h>
#include <cluster/Packets/PacketFactory.h>

namespace cluster
{
   CLUSTER_REGISTER_CLUSTER_PACKET_CREATOR(ApplicationDataAck);
   
   ApplicationDataAck::ApplicationDataAck(const vpr::GUID& plugin_guid, 
                                          const vpr::GUID& id, 
                                          const bool ack)
   {
      // Set the local member variables using the given values.
      mPluginId = plugin_guid;
      mId = id;
      mAck = ack;
      
      // Create a Header for this packet with the correect type and size.
      mHeader = new Header(Header::RIM_PACKET,
                                      Header::RIM_APPDATA_ACK,
                                      Header::RIM_PACKET_HEAD_SIZE 
                                      + 16/*Plugin GUID*/
                                      + 16/*Object GUID*/
                                      + 1 /*mAck*/,
                                      0/*Field not curently used*/);
      // Serialize the given data.
      serialize();
   }
   void ApplicationDataAck::serialize()
   {
      // Clear the data stream.
      mPacketWriter->getData()->clear();
      mPacketWriter->setCurPos(0);

      // Serialize the header.
      mHeader->serializeHeader();
      
      // Serialize plugin GUID
      mPluginId.writeObject(mPacketWriter);

      // Serialize ApplicationData object GUID
      mId.writeObject(mPacketWriter);

      // Serialize the Ack boolean
      mPacketWriter->writeBool(mAck);
   }

   void ApplicationDataAck::parse(vpr::BufferObjectReader* reader)
   {
      // De-Serialize plugin GUID
      mPluginId.readObject(reader);
      
      // De-Serialize ApplicationData object GUID
      mId.readObject(reader);

      // De-Serialize the Ack boolean
      mAck = reader->readBool();
   }

   void ApplicationDataAck::printData(int debug_level)
   {
      vprDEBUG_BEGIN(gadgetDBG_RIM,debug_level) 
         <<  clrOutBOLD(clrYELLOW,"==== ApplicationData Ack Packet ====\n") << vprDEBUG_FLUSH;
      
      Packet::printData(debug_level);

      vprDEBUG(gadgetDBG_RIM,debug_level) 
         << clrOutBOLD(clrYELLOW, "Plugin GUID:        ") << mPluginId.toString()
         << std::endl << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_RIM,debug_level) 
         << clrOutBOLD(clrYELLOW, "Object GUID:        ") << mId.toString()
         << std::endl << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_RIM,debug_level) 
         << clrOutBOLD(clrYELLOW, "Ack or Nack:      ") << (mAck ? "Ack" : "Nack")  << std::endl
         << std::endl << vprDEBUG_FLUSH;

      vprDEBUG_END(gadgetDBG_RIM,debug_level) 
         <<  clrOutBOLD(clrYELLOW,"================================\n") << vprDEBUG_FLUSH;      
   }

}   // end namespace gadget
