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

#include <boost/concept_check.hpp>

#include <gadget/Util/Debug.h>

#include <cluster/Packets/DeviceRequest.h>
#include <cluster/Packets/PacketFactory.h>

namespace cluster
{
   CLUSTER_REGISTER_CLUSTER_PACKET_CREATOR(DeviceRequest);   
   
   DeviceRequest::DeviceRequest(const vpr::GUID& plugin_guid, const std::string& device_name)
   {
      // Set the local member variables using the given values.
      mDeviceName = device_name;
      mPluginId = plugin_guid;
      
      // Create a Header for this packet with the correect type and size.
      mHeader = new Header(Header::RIM_PACKET,
                                      Header::RIM_DEVICE_REQ,
                                      Header::RIM_PACKET_HEAD_SIZE 
                                      + 16 /*Plugin GUID*/
                                      + vpr::BufferObjectReader::STRING_LENGTH_SIZE
                                      + mDeviceName.size(),
                                      0/*Field not curently used*/);
      // Serialize the given data.
      serialize();
   }

   void DeviceRequest::serialize()
   {
      // Clear the data stream.
      mPacketWriter->getData()->clear();
      mPacketWriter->setCurPos(0);

      // Serialize the header.
      mHeader->serializeHeader();
      
      // Serialize plugin GUID.
      mPluginId.writeObject(mPacketWriter);

      // Serialize the name of the requested device.
      mPacketWriter->writeString(mDeviceName);
   }
   void DeviceRequest::parse(vpr::BufferObjectReader* reader)
   {   
      // De-Serialize plugin GUID
      mPluginId.readObject(reader);

      // De-Serialize the name of the requested device.
      mDeviceName = reader->readString();
   }
   
   void DeviceRequest::printData(int debug_level)
   {
      vprDEBUG_BEGIN(gadgetDBG_RIM,debug_level) 
         <<  clrOutBOLD(clrYELLOW,"==== Device Request Packet Data ====\n") << vprDEBUG_FLUSH;
      
      Packet::printData(debug_level);

      vprDEBUG(gadgetDBG_RIM,debug_level) 
         << clrOutBOLD(clrYELLOW, "Plugin GUID: ") << mPluginId.toString()
         << std::endl << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_RIM,debug_level) 
         << clrOutBOLD(clrYELLOW, "Device Name: ") << mDeviceName
         << std::endl << vprDEBUG_FLUSH;

      vprDEBUG_END(gadgetDBG_RIM,debug_level) 
         <<  clrOutBOLD(clrYELLOW,"====================================\n") << vprDEBUG_FLUSH;
   }                                                                                          
}   // end namespace gadget
