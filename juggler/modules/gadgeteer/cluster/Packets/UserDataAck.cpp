/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2002 by Iowa State University
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
#include <cluster/Packets/UserDataAck.h>
#include <cluster/ClusterNetwork/ClusterNetwork.h>
#include <cluster/ClusterNetwork/ClusterNode.h>
#include <cluster/ClusterManager.h>
#include <cluster/Plugins/ApplicationDataManager/ApplicationDataManager.h>
#include <gadget/InputManager.h>
#include <jccl/RTRC/ConfigManager.h>
#include <gadget/Util/Debug.h>

namespace cluster
{
   UserDataAck::UserDataAck(Header* packet_head, vpr::SocketStream* stream)
   {
      recv(packet_head,stream);
      parse();
   }
   UserDataAck::UserDataAck(vpr::Uint16& device_id, std::string& device_name, bool ack)
   {
      // Given the input, create the packet and then serialize the packet(which includes the header)
      // - Set member variables
      // - Create the correct packet header
      // - Serialize the packet

      // =============== Packet Specific =================
      //

      // Device Request Vars
      mDeviceId = device_id;
      mDeviceName = device_name;
      mAck = ack;
      mHostname = ClusterNetwork::instance()->getLocalHostname();
      
      // string -> string.size()
      // Uint8 -> 1
      // Uint16 -> 2
      // Uint32 -> 4

      // 2 + 2 + 2 + mDeviceName.size() + 2 + mDeviceBaseType() + 1

      // Header vars (Create Header)
      mHeader = new Header(Header::RIM_PACKET,
                                      Header::RIM_USERDATA_ACK,
                                      Header::RIM_PACKET_HEAD_SIZE 
                                      + 2 /*mDeviceID*/
                                      + 2 /*value of size*/+ mDeviceName.size() /*length of mDeviceName*/
                                      + 2 /*value of size*/+ mHostname.size() /*length of mDeviceBaseType*/
                                      + 1 /*mAck*/,0);                      
      //
      // =============== Packet Specific =================

      serialize();
   }
   void UserDataAck::serialize()
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
               // Remote and Local ID's
      mPacketWriter->writeUint16(mDeviceId);
      
         // Device Name
//      mPacketWriter->writeUint16(mDeviceName.size());
      mPacketWriter->writeString(mDeviceName);
      
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
   void UserDataAck::parse()
   {
      // =============== Packet Specific =================
      //
         // Remote and Local ID's
      mDeviceId = mPacketReader->readUint16();

         // Device Name
//      vpr::Uint16 temp_string_len = mPacketReader->readUint16();
//      mDeviceName = mPacketReader->readString(temp_string_len);
      mDeviceName = mPacketReader->readString();

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

   bool UserDataAck::action(ClusterNode* node)
   {
/*      // -If ACK
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
         node->removeDeviceRequest(mDeviceName);

         if (RemoteInputManager::instance()->getVirtualDevice(mDeviceName) != NULL)
         {
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrRED, "ERROR:") 
               << "Somehow we already have a virtual device named: " << mDeviceName << std::endl << vprDEBUG_FLUSH;
         }                                                                  
         else
         {
            RemoteInputManager::instance()->addVirtualDevice(mDeviceId, mDeviceName, mDeviceBaseType, mHostname);
            // Tell the input Mangager that we may now have the device it was trying to point to
            gadget::InputManager::instance()->refreshAllProxies();      
         }
      }
      else
      {
         // Do Nothing Since we will just re-try later
         //RemoteInputManager::instance()->createPendingConfigRemoveAndAdd(mDeviceName);
         //jccl::ConfigManager::instance()->delayStalePendingList();
      }
*/    
      if (mAck)
      {
         ApplicationDataManager::instance()->removePendingUserDataRequest(mDeviceName);
      }
      return true;
   }


   void UserDataAck::printData(int debug_level)
   {
      vprDEBUG_BEGIN(gadgetDBG_RIM,debug_level) 
         <<  clrOutBOLD(clrYELLOW,"==== User Data Ack Packet Data ====\n") << vprDEBUG_FLUSH;
      
      Packet::printData(debug_level);

      vprDEBUG(gadgetDBG_RIM,debug_level) 
         << clrOutBOLD(clrYELLOW, "Device ID:        ") << mDeviceId
         << std::endl << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_RIM,debug_level) 
         << clrOutBOLD(clrYELLOW, "Device Name:      ") << mDeviceName
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
