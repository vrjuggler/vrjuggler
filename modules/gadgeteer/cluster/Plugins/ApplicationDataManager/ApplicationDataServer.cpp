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

#include <gadget/gadgetConfig.h>
#include <gadget/Util/Debug.h>
#include <cluster/Plugins/ApplicationDataManager/ApplicationDataServer.h> // my header...
#include <cluster/ClusterNetwork/ClusterNode.h>
#include <cluster/Plugins/ApplicationDataManager/ApplicationData.h>

namespace cluster
{
   ApplicationDataServer::ApplicationDataServer(vpr::GUID guid,  ApplicationData* user_data) 
   {
      mId = guid;
      
      mApplicationData = user_data;

      mDataPacket = new DataPacket();
      mDeviceData = new std::vector<vpr::Uint8>;      
      mBufferObjectWriter = new vpr::BufferObjectWriter(mDeviceData);
   }

   ApplicationDataServer::~ApplicationDataServer()
   {;}

   void ApplicationDataServer::send()
   {
      lockClients();
      //--send to all nodes in the map
      //WE MUST NEVER USE THE BASE CLASS's SEND()
      for (std::vector<cluster::ClusterNode*>::iterator i = mClients.begin();
           i != mClients.end() ; i++)
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) << "Sending data to: " 
            << (*i)->getName() << std::endl << vprDEBUG_FLUSH;
         try
         {
            // XXX Might have to change this to use ClusterNode's send
            //     method soon.
            mDataPacket->send((*i)->getSockStream(), mId, mDeviceData);
         }
         catch(cluster::ClusterException cluster_exception)
         {
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "ApplicationDataServer::send() Caught an exception!" 
               << std::endl << vprDEBUG_FLUSH;
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrSetBOLD(clrRED)
               << cluster_exception.getMessage() << clrRESET
               << std::endl << vprDEBUG_FLUSH;

            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << 
               "ApplicationDataServer::send() We have lost our connection to: " << (*i)->getName() << ":" << (*i)->getPort()
               << std::endl << vprDEBUG_FLUSH;
            
            (*i)->setConnected(ClusterNode::DISCONNECTED);
            debugDump(vprDBG_CONFIG_LVL);
         }
      }
      unlockClients();
   }
   void ApplicationDataServer::updateLocalData()
   {
      // -BufferObjectWriter
      mBufferObjectWriter->getData()->clear();
      mBufferObjectWriter->setCurPos(0);

      mApplicationData->writeObject(mBufferObjectWriter);

   }

   void ApplicationDataServer::addClient(ClusterNode* new_client_node)
   {
      vprASSERT(0 == mClientsLock.test());
      vprASSERT(new_client_node != NULL && "You can not add a new client that is NULL");
      lockClients();

      mClients.push_back(new_client_node);
      
      unlockClients();
   }
   
/*   void ApplicationDataServer::removeClient(const std::string& host_name)
   {
      vprASSERT(0 == mClientsLock.test());
      lockClients();
   
      for (std::map<cluster::ClusterNode*,vpr::Uint16>::iterator i = mClients.begin() ; 
            i!= mClients.end() ; i++)
      {
         if ((*i)->getHostname() == host_name)
         {
            mClients.erase(i);
            unlockClients();
            return;
         }
      }
      unlockClients();
   }
  
*/
   void ApplicationDataServer::debugDump(int debug_level)
   {
/*      vprASSERT(0 == mClientsLock.test());
      lockClients();

      vpr::DebugOutputGuard dbg_output(gadgetDBG_RIM,debug_level,
                                 std::string("-------------- ApplicationDataServer --------------\n"),
                                 std::string("------------------------------------------\n"));

      vprDEBUG(gadgetDBG_RIM,debug_level) << "Name:     " << mId.toString() << std::endl << vprDEBUG_FLUSH;
      
      { // Used simply to make the following DebugOutputGuard go out of scope 
         vpr::DebugOutputGuard dbg_output2(gadgetDBG_RIM,debug_level,
                           std::string("------------ Clients ------------\n"),
                           std::string("---------------------------------\n"));
         for (std::map<cluster::ClusterNode*,vpr::Uint16>::iterator i = mClients.begin() ; 
               i!= mClients.end() ; i++)
         {
            vprDEBUG(gadgetDBG_RIM,debug_level) << "-------- " << (*i)->getName() << " --------" << std::endl << vprDEBUG_FLUSH;
            vprDEBUG(gadgetDBG_RIM,debug_level) << "       Hostname: " << (*i).->getHostname() << std::endl << vprDEBUG_FLUSH;
            vprDEBUG(gadgetDBG_RIM,debug_level) << "----------------------------------" << std::endl << vprDEBUG_FLUSH;
         }
      }
      unlockClients();
      
*/      
   }

} // End of gadget namespace
