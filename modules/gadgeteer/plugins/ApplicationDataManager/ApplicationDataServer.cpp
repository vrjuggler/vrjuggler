/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

#include <cluster/PluginConfig.h>

#include <gadget/Util/Debug.h>

#include <plugins/ApplicationDataManager/ApplicationDataServer.h> // my header...
#include <plugins/ApplicationDataManager/ApplicationData.h>
#include <gadget/Node.h>
#include <cluster/Packets/DataPacket.h>

#include <vpr/IO/BufferObjectWriter.h>

#include <boost/concept_check.hpp>

namespace cluster
{
   ApplicationDataServer::ApplicationDataServer(const vpr::GUID& guid,  ApplicationData* user_data, const vpr::GUID& plugin_guid)
   {
      mApplicationData = user_data;

      // mDataPacket and mBufferObjectWriter both use mDeviceData as their data buffer.
      mDeviceData = new std::vector<vpr::Uint8>;
      
      // Create a DataPacket that will be updated and sent continually.
      mDataPacket = new DataPacket(plugin_guid, guid, mDeviceData);
      mBufferObjectWriter = new vpr::BufferObjectWriter(mDeviceData);
   }

   ApplicationDataServer::~ApplicationDataServer()
   {
      delete mApplicationData;
      
      // mDataPacket will clean up the memory that mDeviceData points
      // to since mDataPacket contains a reference to the ame memory.
      delete mDataPacket;
      // vpr::BufferObjectWritter does not release mDeviceData
      delete mBufferObjectWriter;
      mDeviceData = NULL;   
   }

   void ApplicationDataServer::serializeAndSend()
   {
      // Clear old data and reset the position of mBufferObjectWriter
      mBufferObjectWriter->getData()->clear();
      mBufferObjectWriter->setCurPos(0);

      // This updates the mApplicationData which both mBufferedObjectReader and mDevicePacket point to
      vpr::ReturnStatus status = mApplicationData->writeObject(mBufferObjectWriter);

      // If we do not successfully serialize the object, don't send it.
      // This allows the developer to create a object that only gets
      // sent when a change occurs.
      if (!status.success())
      {
         return;
      }

      // We must update the size of the actual data that we are going to send
      mDataPacket->getHeader()->setPacketLength(Header::RIM_PACKET_HEAD_SIZE 
                                       + 16 /*Plugin GUID*/
                                       + 16 /*Plugin GUID*/
                                       + mDeviceData->size());

      // We must serialize the header again so that we can reset the size.
      mDataPacket->getHeader()->serializeHeader();



      // Send the serialized data to each client
      vpr::Guard<vpr::Mutex> guard(mClientsLock);
      
      // Send DataPacket to all nodes in mClients
      for (std::vector<gadget::Node*>::iterator i = mClients.begin();
           i != mClients.end() ; i++)
      {
         //vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "Sending data to: "
         //   << (*i)->getName() << std::endl << vprDEBUG_FLUSH;
         try
         {
            (*i)->send(mDataPacket);
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

            // If we receive an exception because we have lost the connection we need
            // to flag the Node as disconnected and print debug information
            (*i)->setStatus(gadget::Node::DISCONNECTED);
            debugDump(vprDBG_CONFIG_LVL);
         }
      }
   }

   void ApplicationDataServer::addClient(gadget::Node* new_client_node)
   {
      vprASSERT(new_client_node != NULL && "You can not add a new client that is NULL");
      
      // Lock mutex to make this thread safe
      vpr::Guard<vpr::Mutex> guard(mClientsLock);

      mClients.push_back(new_client_node);
   }

   void ApplicationDataServer::removeClient(const std::string& host_name)
   {
      vpr::Guard<vpr::Mutex> guard(mClientsLock);

      for (std::vector<gadget::Node*>::iterator i = mClients.begin() ;
            i!= mClients.end() ; i++)
      {
         if ((*i)->getHostname() == host_name)
         {
            mClients.erase(i);
            return;
         }
      }
   }

   void ApplicationDataServer::debugDump(int debug_level)
   {
      vpr::Guard<vpr::Mutex> guard(mClientsLock);

      vpr::DebugOutputGuard dbg_output(gadgetDBG_RIM,debug_level,
                                 std::string("-------------- ApplicationDataServer --------------\n"),
                                 std::string("------------------------------------------\n"));

      vprDEBUG(gadgetDBG_RIM,debug_level) << "Name:     " << getId().toString() << std::endl << vprDEBUG_FLUSH;

      vpr::DebugOutputGuard dbg_output2(gadgetDBG_RIM,debug_level,
                        std::string("------------ Clients ------------\n"),
                        std::string("---------------------------------\n"));
      for (std::vector<gadget::Node*>::iterator i = mClients.begin() ;
            i!= mClients.end() ; i++)
      {
         vprDEBUG(gadgetDBG_RIM,debug_level) << "-------- " << (*i)->getName() << " --------" << std::endl << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_RIM,debug_level) << "       Hostname: " << (*i)->getHostname() << std::endl << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_RIM,debug_level) << "----------------------------------" << std::endl << vprDEBUG_FLUSH;
      }
   }
   
   vpr::GUID ApplicationDataServer::getId() 
   { 
      return(mApplicationData->getId()); 
   }

} // End of cluster namespace
