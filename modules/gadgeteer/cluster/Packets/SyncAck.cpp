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

#include <gadget/Util/Debug.h>

#include <cluster/Packets/SyncAck.h>
#include <cluster/Packets/PacketFactory.h>


namespace cluster
{
   CLUSTER_REGISTER_CLUSTER_PACKET_CREATOR(SyncAck);

   SyncAck::SyncAck(const std::string& host_name, const vpr::Uint16& port, const bool ack)
   {
      // Set the local member variables using the given values.
      mHostname = host_name;
      mPort = port;
      mAck = ack;

      // Create a Header for this packet with the correect type and size.
      mHeader = new Header(Header::RIM_PACKET,
                                      Header::RIM_SYNC_ACK,
                                      Header::RIM_PACKET_HEAD_SIZE 
                                      + vpr::BufferObjectReader::STRING_LENGTH_SIZE
                                      + mHostname.size()
                                      + 2 /*mPort*/
                                      + 1 /*mAck*/,
                                      0/*Field not curently used*/);
      // Serialize the given data.
      serialize();
   }

   void SyncAck::serialize()
   {
      // Clear the data stream.
      mPacketWriter->getData()->clear();
      mPacketWriter->setCurPos(0);

      // Serialize the header.
      mHeader->serializeHeader();
      
      // Serialize the hostname of the machine acknowledging the SyncRequest.
      mPacketWriter->writeString(mHostname);
               
      // Serialize the listening port of the machine acknowledging the SyncRequest.
      mPacketWriter->writeUint16(mPort);
         
      // Serialize the Ack boolean
      mPacketWriter->writeBool(mAck);
   }

   void SyncAck::parse(vpr::BufferObjectReader* reader)
   {
      // De-Serialize the hostname of the machine acknowledging the SyncRequest.
      mHostname = reader->readString();
         
      // De-Serialize the listening port of the machine acknowledging the SyncRequest.
      mPort = reader->readUint16();

      // De-Serialize the Ack boolean
      mAck = reader->readBool();
   }

   void SyncAck::printData(int debug_level)
   {
      vprDEBUG_BEGIN(gadgetDBG_RIM,debug_level) 
         <<  clrOutBOLD(clrYELLOW,"==== Sync Ack Packet Data ====\n") << vprDEBUG_FLUSH;
      
      Packet::printData(debug_level);

      vprDEBUG(gadgetDBG_RIM,debug_level) 
         << clrOutBOLD(clrYELLOW, "Host Name:    ") << mHostname
         << std::endl << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_RIM,debug_level) 
         << clrOutBOLD(clrYELLOW, "Port:         ") << mPort
         << std::endl << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_RIM,debug_level) 
         << clrOutBOLD(clrYELLOW, "Ack or Nack:  ") << (mAck ? "Ack" : "Nack")
         << std::endl << vprDEBUG_FLUSH;

      vprDEBUG_END(gadgetDBG_RIM,debug_level) 
         <<  clrOutBOLD(clrYELLOW,"====================================\n") << vprDEBUG_FLUSH;
   }
}   // end namespace gadget
