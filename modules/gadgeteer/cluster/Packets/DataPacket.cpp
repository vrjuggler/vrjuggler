/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <gadget/gadgetConfig.h>
#include <gadget/Util/Debug.h>

#include <cluster/Packets/DataPacket.h>
#include <cluster/ClusterManager.h>
#include <cluster/ClusterNetwork/ClusterNode.h>
#include <gadget/Type/Input.h>

#include <vpr/IO/Socket/SocketStream.h>
#include <vpr/System.h>
#include <vpr/Util/Assert.h>


namespace cluster
{
   DataPacket::DataPacket(Header* packet_head, vpr::SocketStream* stream)
   {
      recv(packet_head,stream);
      parse();
   }
   DataPacket::DataPacket()
   {
      // Given the input, create the packet and then serialize the packet(which includes the header)
      // - Set member variables
      // - Create the correct packet header
      // - Serialize the packet

      //vprASSERT(mDeviceData == NULL && "Device data vector can't equal NULL");

      // Header vars (Create Header)
      mHeader = new Header(Header::RIM_PACKET,
                                      Header::RIM_DATA_PACKET,
                                      Header::RIM_PACKET_HEAD_SIZE 
                                      + 16 /* Plugin GUID */
                                      + 16 /* GUID */
                                      + 0 /*mDeviceData->size()*/,
                                      0/* Frame ID*/);
      serialize();
   }
   
   vpr::ReturnStatus DataPacket::send(vpr::SocketStream* socket)
   {
      vprASSERT(false && "YOU SHOULD NOT BE USING THIS SEND FUNCTION");
      return(vpr::ReturnStatus::Fail);
   }

   void DataPacket::send(vpr::SocketStream* socket, const vpr::GUID& plugin_id, const vpr::GUID& device_id, std::vector<vpr::Uint8>* device_data)
   {
      // - Send header data
      // - Send Device ID
      // - Send Device Data
      // - Send packet data
      vprASSERT(socket != NULL && "SocketStream can't be NULL");
      vprASSERT(device_data != NULL && "Device Data can't be NULL");


      vpr::Uint32 bytes_written;
      mHeader->setPacketLength(Header::RIM_PACKET_HEAD_SIZE 
                                      + 16 /* Plugin GUID*/
                                      + 16 /* GUID*/
                                      + device_data->size());
      mHeader->serializeHeader();
      mHeader->send(socket);
      
      // Send the device ID
      ////////////////////////////////

      mPluginId = plugin_id;
      mId = device_id;
      
      vpr::BufferObjectWriter temp_writer;
      
      // Serialize the Plugin GUID
      mPluginId.writeObject((vpr::ObjectWriter*)&temp_writer);
      
      // Serialize the Device GUID
      mId.writeObject((vpr::ObjectWriter*)&temp_writer);

      // Send 32 bits since we are sending two GUIDs
      socket->send(*temp_writer.getData(),32,bytes_written);

      // Send 32 bits less then the packet size since we have already sent two GUIDs
      socket->send(*device_data,mHeader->getPacketLength()-Header::RIM_PACKET_HEAD_SIZE-32,bytes_written);
   }
   void DataPacket::serialize()
   {
      // Create the header information
      mHeader->serializeHeader();      
   }
   void DataPacket::parse()
   {
      // De-Serialize the plugin ID
      mPluginId.readObject(mPacketReader);

      // De-Serialize the device ID
      mId.readObject(mPacketReader);
   
      // WE MUST KEEP IN MIND THAT mData is not simply the device data but the device data with the
      // Virtual Device Data ID at the beginning
      
      //std::cout << "Size before shrink: " << mPacketReader->mData->size() << std::endl;
      //std::vector<vpr::Uint8>::iterator i;
      //i = mPacketReader->mData->begin();
      //i = mPacketReader->mData->erase(i);
      //i = mPacketReader->mData->begin();
      //i = mPacketReader->mData->erase(i);
      //std::cout << "Size after shrink: " << mPacketReader->mData->size() << std::endl;
   }
   
   bool DataPacket::action(ClusterNode* node)
   {
      return false;
   }
   
   void DataPacket::printData(int debug_level)
   {
      vprDEBUG_BEGIN(gadgetDBG_RIM,vprDBG_VERB_LVL) 
         <<  clrOutBOLD(clrYELLOW,"==== Device Data Packet ====\n") << vprDEBUG_FLUSH;
      
      Packet::printData(vprDBG_VERB_LVL);

      vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) 
         << clrOutBOLD(clrYELLOW, "Plugin ID: ") << mPluginId.toString()
         << std::endl << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) 
         << clrOutBOLD(clrYELLOW, "Object ID: ") << mId.toString()
         << std::endl << vprDEBUG_FLUSH;

      vprDEBUG_END(gadgetDBG_RIM,vprDBG_VERB_LVL) 
         <<  clrOutBOLD(clrYELLOW,"============================\n") << vprDEBUG_FLUSH;
   }

}   // end namespace gadget
