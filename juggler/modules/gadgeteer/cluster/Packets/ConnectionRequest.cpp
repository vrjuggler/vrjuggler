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
#include <gadget/Util/Debug.h>
#include <cluster/Packets/ConnectionRequest.h>
#include <cluster/ClusterNetwork/ClusterNetwork.h>
#include <cluster/ClusterNetwork/ClusterNode.h>


namespace cluster
{
   ConnectionRequest::ConnectionRequest(Header* packet_head, vpr::SocketStream* stream)
   {
      recv(packet_head,stream);
      parse();
   }
   ConnectionRequest::ConnectionRequest(std::string host_name, vpr::Uint16 port, std::string manager_id)
   {
      // Given the input, create the packet and then serialize the packet(which includes the header)
      // - Set member variables
      // - Create the correct packet header
      // - Serialize the packet

      // Device Request Vars
      mHostname = host_name;
      mPort = port;
      mManagerId = manager_id;
      
      // string -> string.size()
      // Uint8 -> 1
      // Uint16 -> 2
      // Uint32 -> 4
      
      // 2 + mHostname.size() + 2 + 2 + mMacnagerId.size()

      
      // Header vars (Create Header)
      mHeader = new Header(Header::RIM_PACKET,
                                      Header::RIM_CONNECTION_REQ,
                                      Header::RIM_PACKET_HEAD_SIZE 
                                      + 2 
                                      + mHostname.size()
                                      + 2 /*mPort*/
                                      + 2
                                      + mManagerId.size(),0);
      serialize();
   }
   void ConnectionRequest::serialize()
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
         
         // Host Name
//      mPacketWriter->writeUint16(mHostname.size());
      mPacketWriter->writeString(mHostname);
               
         // Port
      mPacketWriter->writeUint16(mPort);
         
         // ManagerID
//      mPacketWriter->writeUint16(mManagerId.size());
      mPacketWriter->writeString(mManagerId);
         
      //
      // =============== Packet Specific =================
   }
   void ConnectionRequest::parse()
   {
      // =============== Packet Specific =================
      //

         // Host Name
//      vpr::Uint16 temp_string_len = mPacketReader->readUint16();
//      mHostname = mPacketReader->readString(temp_string_len);
      mHostname = mPacketReader->readString();
         
         // Port
      mPort = mPacketReader->readUint16();

         // Manager ID
//      temp_string_len = mPacketReader->readUint16();
//      mManagerId = mPacketReader->readString(temp_string_len);
      mManagerId = mPacketReader->readString();


      //
      // =============== Packet Specific =================
   }
   void ConnectionRequest::printData(int debug_level)
   {
      vprDEBUG_BEGIN(gadgetDBG_RIM,debug_level) 
         <<  clrOutBOLD(clrYELLOW,"==== Connection Request Packet Data ====\n") << vprDEBUG_FLUSH;
      
      Packet::printData(debug_level);

      vprDEBUG(gadgetDBG_RIM,debug_level) 
         << clrOutBOLD(clrYELLOW, "Host Name:    ") << mHostname
         << std::endl << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_RIM,debug_level) 
         << clrOutBOLD(clrYELLOW, "Port:         ") << mPort
         << std::endl << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_RIM,debug_level) 
         << clrOutBOLD(clrYELLOW, "Manager ID:   ") << mManagerId
         << std::endl << vprDEBUG_FLUSH;

      vprDEBUG_END(gadgetDBG_RIM,debug_level) 
         <<  clrOutBOLD(clrYELLOW,"========================================\n") << vprDEBUG_FLUSH;

      /*
      Packet::printData();
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) 
      << clrOutBOLD(clrCYAN,"\n==== Connection Request Packet Data ====")
      << "\nHost Name:    " << mHostname
      << "\nPort:         " << mPort
      << "\nManager ID:   " << mManagerId << std::endl
      << vprDEBUG_FLUSH;      
      */
   }
   
   bool ConnectionRequest::action(ClusterNode* node)
   {
      // - Check if ClusterNode already exists
      //   - If true delete old one (In case the other end has lost the connection)
      // - Create a ClusterNode
      //   -(Set isConnected on new node)
      // - Send a Connection_Ack

      // MUST ADD ANOTHER FUNCTION HERE SINCE A CLUSTER NODE IS NOT CALLING THIS
      // RIM GETS THIS FROM LISTNING
      // We could make the RIM responsible for creating the new ClusterNode when
      // it receives a network connection then all we need to do is:
      //   - Save remote ManagerId
      if (node == NULL)
      {
         return false;
      }

      node->setHostname(mHostname);
      node->setPort(mPort);
      node->setRemoteManagerId(mManagerId);
      // Does not do anything here since it all happens in the acceptLoop
      //node->setConnected(true);
      
      return true;
   }

}   // end namespace gadget
