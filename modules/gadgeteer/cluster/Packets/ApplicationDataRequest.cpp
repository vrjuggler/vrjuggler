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
#include <cluster/Packets/ApplicationDataRequest.h>
#include <gadget/Util/Debug.h>

#include <cluster/ClusterManager.h>
#include <cluster/Plugins/ApplicationDataManager/ApplicationDataManager.h>
#include <cluster/Plugins/ApplicationDataManager/ApplicationDataServer.h>
#include <cluster/Packets/ApplicationDataAck.h>
#include <cluster/ClusterNetwork/ClusterNode.h>

namespace cluster
{
   ApplicationDataRequest::ApplicationDataRequest(Header* packet_head, vpr::SocketStream* stream)
   {
      recv(packet_head,stream);
      parse();
   }
   ApplicationDataRequest::ApplicationDataRequest(vpr::GUID id)
   {
      // Given the input, create the packet and then serialize the packet(which includes the header)
      // - Set member variables
      // - Create the correct packet header
      // - Serialize the packet

      // ApplicationData GUID
      mId = id;

      // Header vars (Create Header)
      mHeader = new Header(Header::RIM_PACKET,
                                      Header::RIM_APPDATA_REQ,
                                      Header::RIM_PACKET_HEAD_SIZE + 16/*Size of GUID*/,
                                      0);
      serialize();
   }
   void ApplicationDataRequest::serialize()
   {
      // - Clear
      // - Write Header
      // - Write data
      
      mPacketWriter->getData()->clear();
      mPacketWriter->setCurPos(0);

      // Create the header information
      mHeader->serializeHeader();
      
      mId.writeObject(mPacketWriter);
   }
   void ApplicationDataRequest::parse()
   {
      mId.readObject(mPacketReader);
   }
   
   bool ApplicationDataRequest::action(ClusterNode* node)
   {
      ApplicationDataServer* temp_app_data_server = ApplicationDataManager::instance()->getApplicationDataServer(mId);
      ApplicationDataAck* temp_ack = NULL;
      if (temp_app_data_server != NULL)
      {
         temp_app_data_server->addClient(node);
         temp_ack = new ApplicationDataAck(mId, true);
         node->send(temp_ack);
      }
      else
      {
         temp_ack = new ApplicationDataAck(mId, false);
         node->send(temp_ack);
      }
      delete temp_ack;
      return true;
   }

   void ApplicationDataRequest::printData(int debug_level)
   {
      vprDEBUG_BEGIN(gadgetDBG_RIM,debug_level) 
         <<  clrOutBOLD(clrYELLOW,"==== User Data Packet Data ====\n") << vprDEBUG_FLUSH;
      
      Packet::printData(debug_level);

      vprDEBUG(gadgetDBG_RIM,debug_level) 
         << clrOutBOLD(clrYELLOW, "GUID:   ") << mId.toString()
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
