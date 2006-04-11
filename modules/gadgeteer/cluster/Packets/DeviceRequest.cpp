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
#include <cluster/Packets/DeviceRequest.h>
#include <gadget/Util/Debug.h>

#include <gadget/InputManager.h>
#include <gadget/Type/Input.h>
#include <cluster/Plugins/RemoteInputManager/RemoteInputManager.h>
#include <cluster/Plugins/RemoteInputManager/DeviceServer.h>
#include <cluster/Packets/DeviceAck.h>
#include <cluster/ClusterNetwork/ClusterNode.h>
#include <jccl/RTRC/ConfigManager.h>

namespace cluster
{
   DeviceRequest::DeviceRequest(Header* packet_head, vpr::SocketStream* stream)
   {
      recv(packet_head,stream);
      parse();
   }
   DeviceRequest::DeviceRequest(std::string& device_name)
   {
      // Given the input, create the packet and then serialize the packet(which includes the header)
      // - Set member variables
      // - Create the correct packet header
      // - Serialize the packet

      // Device Request Vars
      mDeviceName = device_name;
      
      // Header vars (Create Header)
      mHeader = new Header(Header::RIM_PACKET,
                                      Header::RIM_DEVICE_REQ,
                                      Header::RIM_PACKET_HEAD_SIZE 
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
      
      // Device Name
//      mPacketWriter->writeUint16(mDeviceName.size());      
      mPacketWriter->writeString(mDeviceName);
   }
   void DeviceRequest::parse()
   {   
      // Device Name
//      vpr::Uint16 temp_name_len = mPacketReader->readUint16();
//      mDeviceName = mPacketReader->readString(temp_name_len);
      mDeviceName = mPacketReader->readString();
   }
   
   bool DeviceRequest::action(ClusterNode* node)
   {
      // -If the ConfigManager is done configuring all local devices
      //   -If the InputManager has the device
      //     -Check for DeviceServer
      //       -If not create one
      //     -Add ClusterNode to Device Server's clients
      //     -Send Device_ACK
      //   -Else
      //     -Send Device_NACK
      // -Else
      //   -Send Device_NACK


      // XXX REORDER SEARCH ORDER
      if (node == NULL)
      {
         return false;
      }
      if (jccl::ConfigManager::instance()->isPendingStale())
      {
         gadget::Input* temp_input_device = gadget::InputManager::instance()->getDevice(mDeviceName);
         if (temp_input_device != NULL)
         {
            DeviceServer* temp_device_server = RemoteInputManager::instance()->getDeviceServer(mDeviceName);
            if (NULL == temp_device_server)
            {
               RemoteInputManager::instance()->addDeviceServer(mDeviceName, temp_input_device);
               temp_device_server = RemoteInputManager::instance()->getDeviceServer(mDeviceName);
            }
            
            temp_device_server->addClient(node);
                        
            // Create a responce ACK
            std::string temp_string = temp_input_device->getBaseType();
            vpr::GUID   temp_guid   = temp_device_server->getId();
            DeviceAck* temp_ack = new DeviceAck(temp_guid, mDeviceName, temp_string, true);
//            temp_ack.send(node->getSockStream());
            node->send(temp_ack);
         }
         else
         {
            std::string temp_string = "";
            vpr::GUID empty_id;
            DeviceAck* temp_ack = new DeviceAck(empty_id, mDeviceName, temp_string/*BaseType*/, false);
//            temp_ack.send(node->getSockStream());
            node->send(temp_ack);
         }
      }
      else
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) 
            << clrOutBOLD(clrRED,"Pending List is not stale(Config Manager is still configuring the local system) ")
            << clrOutBOLD(clrRED,"So we can not process this device request right now.") << std::endl << vprDEBUG_FLUSH;            
         
         std::string temp_string = "";
         vpr::GUID empty_id;
         DeviceAck* temp_ack = new DeviceAck(empty_id, mDeviceName, temp_string/*BaseType*/, false);
//         temp_ack.send(node->getSockStream());
         node->send(temp_ack);
      }
      return true;
   }

   void DeviceRequest::printData(int debug_level)
   {
      vprDEBUG_BEGIN(gadgetDBG_RIM,debug_level) 
         <<  clrOutBOLD(clrYELLOW,"==== Device Request Packet Data ====\n") << vprDEBUG_FLUSH;
      
      Packet::printData(debug_level);

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
