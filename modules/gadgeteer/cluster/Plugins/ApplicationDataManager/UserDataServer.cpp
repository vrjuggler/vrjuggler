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

#include <gadget/gadgetConfig.h>
#include <gadget/Util/Debug.h>
#include <cluster/Plugins/ApplicationDataManager/UserDataServer.h> // my header...
#include <cluster/ClusterNetwork/ClusterNode.h>
#include <cluster/SerializableData.h>

namespace cluster
{
   UserDataServer::UserDataServer(const std::string& name,  SerializableData* user_data) 
   {
      mName = name;
      
      mUserData = user_data;

      mDataPacket = new DataPacket();
      mDeviceData = new std::vector<vpr::Uint8>;      
      mBufferObjectWriter = new vpr::BufferObjectWriter(mDeviceData);
   }
   UserDataServer::~UserDataServer()
   {;}

   void UserDataServer::send()
   {
      lockClients();
      //--send to all nodes in the map
      //WE MUST NEVER USE THE BASE CLASS's SEND()
      for (std::map<cluster::ClusterNode*,vpr::Uint16>::iterator i = mClients.begin();
           i != mClients.end() ; i++)
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) << "Sending data to: " 
            << (*i).first->getName() << std::endl << vprDEBUG_FLUSH;
         try
         {
            // XXX Might have to change this to use ClusterNode's send
            //     method soon.
            mDataPacket->send((*i).first->getSockStream(), (*i).second, mDeviceData);
         }
         catch(cluster::ClusterException cluster_exception)
         {
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "UserDataServer::send() Caught an exception!" 
               << std::endl << vprDEBUG_FLUSH;
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrSetBOLD(clrRED)
               << cluster_exception.getMessage() << clrRESET
               << std::endl << vprDEBUG_FLUSH;

            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << 
               "UserDataServer::send() We have lost our connection to: " << (*i).first->getName() << ":" << (*i).first->getPort()
               << std::endl << vprDEBUG_FLUSH;
            
            (*i).first->setConnected(ClusterNode::DISCONNECTED);
            debugDump(vprDBG_CONFIG_LVL);
         }
      }
      unlockClients();
   }
   void UserDataServer::updateLocalData()
   {
      // -BufferObjectWriter
      mBufferObjectWriter->getData()->clear();
      mBufferObjectWriter->setCurPos(0);

      mUserData->writeObject(mBufferObjectWriter);

   }

   void UserDataServer::addClient(ClusterNode* new_client_node, vpr::Uint16& remote_id)
   {
      vprASSERT(0 == mClientsLock.test());
      vprASSERT(new_client_node != NULL && "You can not add a new client that is NULL");
      lockClients();

      mClients[new_client_node] = remote_id;
      
      unlockClients();
   }
   
   void UserDataServer::removeClient(const std::string& host_name)
   {
      vprASSERT(0 == mClientsLock.test());
      lockClients();
   
      for (std::map<cluster::ClusterNode*,vpr::Uint16>::iterator i = mClients.begin() ; 
            i!= mClients.end() ; i++)
      {
         if ((*i).first->getHostname() == host_name)
         {
            mClients.erase(i);
            unlockClients();
            return;
         }
      }
      unlockClients();
   }
   
   void UserDataServer::debugDump(int debug_level)
   {
      vprASSERT(0 == mClientsLock.test());
      lockClients();

      vpr::DebugOutputGuard dbg_output(gadgetDBG_RIM,debug_level,
                                 std::string("-------------- UserDataServer --------------\n"),
                                 std::string("------------------------------------------\n"));

      vprDEBUG(gadgetDBG_RIM,debug_level) << "Name:     " << mName << std::endl << vprDEBUG_FLUSH;
      
      { // Used simply to make the following DebugOutputGuard go out of scope 
         vpr::DebugOutputGuard dbg_output2(gadgetDBG_RIM,debug_level,
                           std::string("------------ Clients ------------\n"),
                           std::string("---------------------------------\n"));
         for (std::map<cluster::ClusterNode*,vpr::Uint16>::iterator i = mClients.begin() ; 
               i!= mClients.end() ; i++)
         {
            vprDEBUG(gadgetDBG_RIM,debug_level) << "-------- " << (*i).first->getName() << " --------" << std::endl << vprDEBUG_FLUSH;
            vprDEBUG(gadgetDBG_RIM,debug_level) << "       Hostname: " << (*i).first->getHostname() << std::endl << vprDEBUG_FLUSH;
            vprDEBUG(gadgetDBG_RIM,debug_level) << "----------------------------------" << std::endl << vprDEBUG_FLUSH;
         }
      }
      unlockClients();
   }

} // End of gadget namespace
