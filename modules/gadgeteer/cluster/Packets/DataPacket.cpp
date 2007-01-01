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

#include <gadget/gadgetConfig.h>
#include <boost/concept_check.hpp>
#include <gadget/Util/Debug.h>

#include <cluster/Packets/DataPacket.h>
#include <cluster/Packets/PacketFactory.h>

#include <vpr/System.h>
#include <vpr/Util/Assert.h>


namespace cluster
{
   CLUSTER_REGISTER_CLUSTER_PACKET_CREATOR(DataPacket);
   
   DataPacket::DataPacket() : mDeviceData(NULL)
   {;}

   DataPacket::DataPacket(const vpr::GUID& plugin_id, const vpr::GUID& object_id, std::vector<vpr::Uint8>* data)
         : mObjectId(object_id), mDeviceData(data)
   {      
      mPluginId = plugin_id;

      // Create a Header for this packet with the correect type and size.
      mHeader = new Header(Header::RIM_PACKET,
                                      Header::RIM_DATA_PACKET,
                                      Header::RIM_PACKET_HEAD_SIZE 
                                       + 16 /*Plugin GUID*/
                                       + 16 /*Plugin GUID*/
                                       + mDeviceData->size(),
                                      0/*Field not curently used*/);
      // Serialize the given data.
      serialize();
   }

   void DataPacket::serialize()
   {
      // Clear the data stream.
      mPacketWriter->getData()->clear();
      mPacketWriter->setCurPos(0);

      // Serialize the header.
      mHeader->serializeHeader();
      
      // Serialize plugin GUID.
      mPluginId.writeObject(mPacketWriter);
      
      // Serialize device GUID.
      mObjectId.writeObject(mPacketWriter);
      
      // mDeviceData is a pointer that points at the DeviceData located in the DeviceServer
      // this data will be updated every frame before sent.
   }

   void DataPacket::parse(vpr::BufferObjectReader* reader)
   {
      // De-Serialize plugin GUID
      mPluginId.readObject(reader);

      // De-Serialize plugin GUID
      mObjectId.readObject(reader);
            
      mDeviceData = new std::vector<vpr::Uint8>();

      unsigned int data_size = mHeader->getPacketLength() - Header::RIM_PACKET_HEAD_SIZE - 32;
      for(unsigned int i = 0 ; i < data_size ; i++)
      {
         mDeviceData->push_back(*(reader->readRaw(1)));
      }
   }

   void DataPacket::printData(int debug_level)
   {
      // NOTE: This should be removed if any of the below code ever puts
      // debug_level to use.
      
      boost::ignore_unused_variable_warning(debug_level);
      
      /*
      vprDEBUG_BEGIN(gadgetDBG_RIM,debug_level) 
         <<  clrOutBOLD(clrYELLOW,"==== Device Data Packet ====\n") << vprDEBUG_FLUSH;
      
      Packet::printData(debug_level);

      vprDEBUG(gadgetDBG_RIM,debug_level) 
         << clrOutBOLD(clrYELLOW, "Plugin ID: ") << mPluginId.toString()
         << std::endl << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_RIM,debug_level) 
         << clrOutBOLD(clrYELLOW, "Object ID: ") << mObjectId.toString()
         << std::endl << vprDEBUG_FLUSH;

      vprDEBUG_END(gadgetDBG_RIM,debug_level) 
         <<  clrOutBOLD(clrYELLOW,"============================\n") << vprDEBUG_FLUSH;
      */
   }  
}// end namespace cluster
