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
#include <cluster/Packets/UserDataRequest.h>
#include <gadget/Util/Debug.h>

#include <cluster/ClusterManager.h>
#include <cluster/Plugins/ApplicationDataManager/ApplicationDataManager.h>
#include <cluster/Plugins/ApplicationDataManager/UserDataServer.h>
#include <cluster/Packets/UserDataAck.h>
#include <cluster/ClusterNetwork/ClusterNode.h>

namespace cluster
{
   UserDataRequest::UserDataRequest(Header* packet_head, vpr::SocketStream* stream)
   {
      recv(packet_head,stream);
      parse();
   }
   UserDataRequest::UserDataRequest(vpr::Uint16 sender_id, std::string user_data_name)
   {
      // Given the input, create the packet and then serialize the packet(which includes the header)
      // - Set member variables
      // - Create the correct packet header
      // - Serialize the packet

      // Device Request Vars
      mSenderId = sender_id;
      mUserDataName = user_data_name;
      
      // Header vars (Create Header)
      mHeader = new Header(Header::RIM_PACKET,
                                      Header::RIM_USERDATA_REQ,
                                      Header::RIM_PACKET_HEAD_SIZE + 2 + 2 + mUserDataName.size(),
                                      0);
      serialize();
   }
   void UserDataRequest::serialize()
   {
      // - Clear
      // - Write Header
      // - Write data
      
      mPacketWriter->getData()->clear();
      mPacketWriter->setCurPos(0);

      // Create the header information
      mHeader->serializeHeader();
      
      // Remote Device ID
      mPacketWriter->writeUint16(mSenderId);
      // Device Name
//      mPacketWriter->writeUint16(mUserDataName.size());      
      mPacketWriter->writeString(mUserDataName);
   }
   void UserDataRequest::parse()
   {
      // Remote Device ID
      mSenderId = mPacketReader->readUint16();
   
      // Device Name
//      vpr::Uint16 temp_name_len = mPacketReader->readUint16();
//      mUserDataName = mPacketReader->readString(temp_name_len);
      mUserDataName = mPacketReader->readString();
   }
   
   bool UserDataRequest::action(ClusterNode* node)
   {
      UserDataServer* temp_userdata_server = ApplicationDataManager::instance()->getUserDataServer(mUserDataName);
      UserDataAck* temp_userdata_ack = NULL;
      if (temp_userdata_server != NULL)
      {
         temp_userdata_server->addClient(node, mSenderId);
         temp_userdata_ack = new UserDataAck(mSenderId , mUserDataName, true);
         node->send(temp_userdata_ack);
      }
      else
      {
         temp_userdata_ack = new UserDataAck(mSenderId , mUserDataName, false);
         node->send(temp_userdata_ack);
      }
      delete temp_userdata_ack;
      return true;
   }

   void UserDataRequest::printData(int debug_level)
   {
      vprDEBUG_BEGIN(gadgetDBG_RIM,debug_level) 
         <<  clrOutBOLD(clrYELLOW,"==== User Data Packet Data ====\n") << vprDEBUG_FLUSH;
      
      Packet::printData(debug_level);

      vprDEBUG(gadgetDBG_RIM,debug_level) 
         << clrOutBOLD(clrYELLOW, "Sender ID:   ") << mSenderId
         << std::endl << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_RIM,debug_level) 
         << clrOutBOLD(clrYELLOW, "User Data Name: ") << mUserDataName
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
