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

//#include <gadget/gadgetConfig.h>
#include <boost/concept_check.hpp>

#include <gadget/Util/Debug.h>

#include <gadget/InputManager.h>
#include <gadget/Type/Input.h>
#include <cluster/ClusterNetwork/ClusterNode.h>
#include <jccl/RTRC/ConfigManager.h>

#include <cluster/Packets/DeviceRequest.h>

namespace cluster
{
   DeviceRequest::DeviceRequest(Header* packet_head, vpr::SocketStream* stream)
   {
      recv(packet_head,stream);
      parse();
   }
   DeviceRequest::DeviceRequest(const std::string& device_name, const vpr::GUID& plugin_guid)
   {
      // Given the input, create the packet and then serialize the packet(which includes the header)
      // - Set member variables
      // - Create the correct packet header
      // - Serialize the packet

      // Device Request Vars
      mDeviceName = device_name;
      mPluginId = plugin_guid;
      
      // Header vars (Create Header)
      mHeader = new Header(Header::RIM_PACKET,
                                      Header::RIM_DEVICE_REQ,
                                      Header::RIM_PACKET_HEAD_SIZE 
                                       + 16 /*Plugin GUID*/
                                       + 2 /*Size mDeviceName*/
                                       + mDeviceName.size(),
                                      0);
      serialize();
   }
   void DeviceRequest::serialize()
   {
      // - Clear
      // - Write Header
      // - Write data
      
      mPacketWriter->getData()->clear();
      mPacketWriter->setCurPos(0);

      // Create the header information
      mHeader->serializeHeader();
      
      // Serialize plugin GUID
      mPluginId.writeObject(mPacketWriter);

      // Device Name
      mPacketWriter->writeString(mDeviceName);
   }
   void DeviceRequest::parse()
   {   
      // De-Serialize plugin GUID
      mPluginId.readObject(mPacketReader);

      // De-Serialize Device Name
      mDeviceName = mPacketReader->readString();
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

   /*   Packet::printData();
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) 
      << clrOutBOLD(clrCYAN,"\n==== Device Request Packet Data ====")
      << "\nSender ID:    " << mSenderId
      << "\nDevice Name: " << mDeviceName << std::endl
      << vprDEBUG_FLUSH;      
   */
   }

}   // end namespace gadget
