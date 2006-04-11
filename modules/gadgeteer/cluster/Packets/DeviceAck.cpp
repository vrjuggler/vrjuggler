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
#include <cluster/Packets/DeviceAck.h>
#include <cluster/ClusterNetwork/ClusterNetwork.h>
#include <cluster/ClusterNetwork/ClusterNode.h>
#include <cluster/Plugins/RemoteInputManager/RemoteInputManager.h>
#include <gadget/InputManager.h>
#include <jccl/RTRC/ConfigManager.h>
#include <gadget/Util/Debug.h>

namespace cluster
{
   DeviceAck::DeviceAck(Header* packet_head, vpr::SocketStream* stream)
   {
      recv(packet_head,stream);
      parse();
   }
   DeviceAck::DeviceAck(vpr::GUID& id, std::string& device_name, 
                              std::string& device_base_type, bool ack)
   {
      // Given the input, create the packet and then serialize the packet(which includes the header)
      // - Set member variables
      // - Create the correct packet header
      // - Serialize the packet

      // =============== Packet Specific =================
      //

      // Device Request Vars
      mId = id;
      mDeviceName = device_name;
      mDeviceBaseType = device_base_type;
      mAck = ack;
      mHostname = ClusterNetwork::instance()->getLocalHostname();
      
      // string -> string.size()
      // Uint8 -> 1
      // Uint16 -> 2
      // Uint32 -> 4

      // 2 + 2 + 2 + mDeviceName.size() + 2 + mDeviceBaseType() + 1

      // Header vars (Create Header)
      mHeader = new Header(Header::RIM_PACKET,
                                      Header::RIM_DEVICE_ACK,
                                      Header::RIM_PACKET_HEAD_SIZE 
                                      + 16 /*mID*/
                                      + 2 /*value of size*/+ mDeviceName.size() /*length of mDeviceName*/
                                      + 2 /*value of size*/+ mDeviceBaseType.size() /*length of mDeviceBaseType*/
                                      + 2 /*value of size*/+ mHostname.size() /*length of mDeviceBaseType*/
                                      + 1 /*mAck*/,0);                      
      //
      // =============== Packet Specific =================

      serialize();
   }
   void DeviceAck::serialize()
   {
      // - Clear
      // - Write Header
      // - Write data
      
      mPacketWriter->getData()->clear();
      mPacketWriter->setCurPos(0);

      // Create the header information
      mHeader->serializeHeader();
      
      // =============== Packet Specific =================
      //
      
      // mId
      mId.writeObject(mPacketWriter);
      
         // Device Name
//      mPacketWriter->writeUint16(mDeviceName.size());
      mPacketWriter->writeString(mDeviceName);
      
         // Base Type
//      mPacketWriter->writeUint16(mDeviceBaseType.size());
      mPacketWriter->writeString(mDeviceBaseType);

         // Hostname of the machine that the device on
//      mPacketWriter->writeUint16(mHostname.length());
      mPacketWriter->writeString(mHostname);

         // Ack
      mPacketWriter->writeBool(mAck);

      

/*      // Remote Device ID
      mPacketWriter->writeUint16(mSenderId);
      // Device Name
//      mPacketWriter->writeUint16(mDeviceName.size());      
      mPacketWriter->writeString(mDeviceName);
*/
      //
      // =============== Packet Specific =================
   }
   void DeviceAck::parse()
   {
      // =============== Packet Specific =================
      //
      
      // mId
      mId.readObject(mPacketReader);
         
      // Device Name
//      vpr::Uint16 temp_string_len = mPacketReader->readUint16();
//      mDeviceName = mPacketReader->readString(temp_string_len);
      mDeviceName = mPacketReader->readString();

         // Base Type
//      temp_string_len = mPacketReader->readUint16();
//      mDeviceBaseType = mPacketReader->readString(temp_string_len);
      mDeviceBaseType = mPacketReader->readString();

         // Hostname of the machine that the device on
//      temp_string_len = mPacketReader->readUint16();
//      mHostname = mPacketReader->readString(temp_string_len);
      mHostname = mPacketReader->readString();

         // Ack
      mAck = mPacketReader->readBool();

/*
      // Remote Device ID
      mSenderId = mPacketReader->readUint16();
   
      // Device Name
//      vpr::Uint16 temp_name_len = mPacketReader->readUint16();
//      mDeviceName = mPacketReader->readString(temp_name_len);
      mDeviceName = mPacketReader->readString();
*/


      //
      // =============== Packet Specific =================
   }

   bool DeviceAck::action(ClusterNode* node)
   {
      // -If ACK
      //   -Create VirtualDevice
      // -If Nack
      //   -Do nothing(let the config manager worry about re-trying)
      if (node == NULL)
      {
         return false;
      }
      if (mAck)
      {
         //vpr::ReturnStatus status =  RemoteInputManager::instance()->addVirtualDevice(mDeviceId, mDeviceName, mDeviceBaseType);
         RemoteInputManager::instance()->removePendingDeviceRequest(mDeviceName);

         if (RemoteInputManager::instance()->getVirtualDevice(mDeviceName) != NULL)
         {
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrRED, "ERROR:") 
               << "Somehow we already have a virtual device named: " << mDeviceName << std::endl << vprDEBUG_FLUSH;
         }                                                                  
         else
         {
            RemoteInputManager::instance()->addVirtualDevice(mId, mDeviceName, mDeviceBaseType, mHostname);
            // Tell the input Mangager that we may now have the device it was trying to point to
            gadget::InputManager::instance()->refreshAllProxies();      
         }
      }
      else
      {  //XXX: FIX
         // Do Nothing Since we will just re-try later
         //RemoteInputManager::instance()->createPendingConfigRemoveAndAdd(mDeviceName);
         //jccl::ConfigManager::instance()->delayStalePendingList();
      }
            
      return true;
   }


   void DeviceAck::printData(int debug_level)
   {
      vprDEBUG_BEGIN(gadgetDBG_RIM,debug_level) 
         <<  clrOutBOLD(clrYELLOW,"==== Device Ack Packet Data ====\n") << vprDEBUG_FLUSH;
      
      Packet::printData(debug_level);

      vprDEBUG(gadgetDBG_RIM,debug_level) 
         << clrOutBOLD(clrYELLOW, "Device ID:        ") << mId.toString()
         << std::endl << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_RIM,debug_level) 
         << clrOutBOLD(clrYELLOW, "Device Name:      ") << mDeviceName
         << std::endl << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_RIM,debug_level) 
         << clrOutBOLD(clrYELLOW, "Device Base Type: ") << mDeviceBaseType 
         << std::endl << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_RIM,debug_level) 
         << clrOutBOLD(clrYELLOW, "Remote Hostname:  ") << mHostname
         << std::endl << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_RIM,debug_level) 
         << clrOutBOLD(clrYELLOW, "Ack or Nack:      ") << (mAck ? "Ack" : "Nack")  << std::endl
         << std::endl << vprDEBUG_FLUSH;

      vprDEBUG_END(gadgetDBG_RIM,debug_level) 
         <<  clrOutBOLD(clrYELLOW,"================================\n") << vprDEBUG_FLUSH;
      
      /*
      // =============== Packet Specific =================
      //                                                
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) 
      << clrOutBOLD(clrCYAN,"\n==== Device Ack Packet Data ====")
      << "\nDevice ID: " << mDeviceId
      << "\nDevice Name:      " << mDeviceName
      << "\nDevice Base Type: " << mDeviceBaseType 
      << "\nAck or Nack:      " << (mAck ? "Ack" : "Nack")  << std::endl
      << vprDEBUG_FLUSH;      
      //
      // =============== Packet Specific =================
      */
   }

}   // end namespace gadget
