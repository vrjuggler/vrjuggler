/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <cluster/PluginConfig.h>

#include <plugins/ApplicationBarrierManager/ApplicationBarrierManager.h>

#include <cluster/ClusterManager.h>
#include <gadget/Node.h>
#include <gadget/Util/Debug.h>
#include <gadget/gadgetParam.h>

#include <plugins/ApplicationBarrierManager/ApplicationBarrier.h>
#include <plugins/ApplicationBarrierManager/ApplicationBarrierServer.h>

#include <cluster/Packets/DataPacket.h>

extern "C"
{
   GADGET_CLUSTER_PLUGIN_EXPORT(vpr::Uint32) getGadgeteerVersion()
   {
      return __GADGET_version;
   }

   GADGET_CLUSTER_PLUGIN_EXPORT(void) initPlugin(cluster::ClusterManager* mgr)
   {
      mgr->addPlugin(cluster::ApplicationBarrierManager::create());
   }
}

namespace cluster
{

const vpr::GUID ApplicationBarrierManager::mPluginGUID("5edddf06-cf73-43ad-928e-318c3e67815e");

ApplicationBarrierManager::ApplicationBarrierManager()
{;}

ClusterPluginPtr ApplicationBarrierManager::create()
{
   return ClusterPluginPtr(new ApplicationBarrierManager());
}

ApplicationBarrierManager::~ApplicationBarrierManager()
{
   // Memory for each ApplicationBarrier object is handled by application.
   mObjects.clear();
   mServers.clear();
}

bool ApplicationBarrierManager::isPluginReady()
{
   return true;
}

void ApplicationBarrierManager::handlePacket(PacketPtr packet, gadget::NodePtr node)
{
   vprDEBUG(gadgetDBG_RIM, vprDBG_HVERB_LVL)
         << clrOutBOLD(clrMAGENTA,"[ApplicationBarrierManager::handlePacket()]")
         << "In handlePacket.\n" << vprDEBUG_FLUSH;

   if ( NULL != packet.get() && NULL != node.get() )
   {
      switch ( packet->getPacketType() )
      {
      case cluster::Header::RIM_DATA_PACKET:
      {
         DataPacketPtr data_packet = boost::dynamic_pointer_cast<DataPacket>(packet);
         vprASSERT(NULL != data_packet.get() && "Dynamic cast failed!");

         // Find the ApplicationBarrier Object that we have received data
         // for.
         ApplicationBarrier* barrier =
            getApplicationBarrier(data_packet->getObjectId());

         if (barrier != NULL)
         {
            // Parse the object's data using the temporary ObjectReader
            barrier->incWaitingNodes();
         }
         else
         {
            vprDEBUG(gadgetDBG_RIM,vprDBG_WARNING_LVL)
               << clrOutBOLD(clrCYAN,"[ApplicationBarrierManager] ")
               << "Got data for an unknown ApplicationBarrier object: "
               << data_packet->getObjectId() << std::endl << vprDEBUG_FLUSH;
         }
         break;
      }
      default:
         vprDEBUG(gadgetDBG_RIM,vprDBG_WARNING_LVL)
            << clrOutBOLD(clrCYAN,"[ApplicationBarrierManager] ")
            << "Don't know how to handle a packet of type: "
            << packet->getPacketType() << std::endl << vprDEBUG_FLUSH;
         break;
      }
   }
}

void ApplicationBarrierManager::postPostFrame()
{;}

void ApplicationBarrierManager::preDraw()
{;}

void ApplicationBarrierManager::sendWait(const vpr::GUID& id)
{
   vprDEBUG(gadgetDBG_RIM, vprDBG_HVERB_LVL)
         << clrOutBOLD(clrMAGENTA,"[ApplicationBarrierManager::sendWait()]")
         << "Sending wait for GUID: " << id.toString() << "\n" << vprDEBUG_FLUSH;

   assert(id != vpr::GUID::NullGUID);

   ApplicationBarrierServerPtr server = mServers[id];

   assert(server.get() != NULL);

   server->serializeAndSend();
}

void ApplicationBarrierManager::addSerializableObject(vpr::SerializableObject* object)
{
   ApplicationBarrier* new_app_data = static_cast<ApplicationBarrier*>(object);

   if (NULL == new_app_data)
   {
      throw ClusterException("Could not cast to ApplicationBarrier", VPR_LOCATION);
   }

   const vpr::GUID id = new_app_data->getId();

   vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
      << clrOutBOLD(clrMAGENTA,"[ApplicationBarrierManager]")
      << " Adding ApplicationBarrier: " << id.toString()
      << std::endl << vprDEBUG_FLUSH;

   mObjects[id] = new_app_data;

   ApplicationBarrierServerPtr new_server
      = ApplicationBarrierServer::create(id, new_app_data, mPluginGUID);
   mServers[id] = new_server;

   // This plugin is now being used(active)
   setActive(true);
}

void ApplicationBarrierManager::dumpApplicationBarrier()
{
   vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
      << clrOutBOLD(clrMAGENTA,"[ApplicationBarrierManager]")
      << " Listing All Application Level ApplicationBarrier"
      << std::endl << vprDEBUG_FLUSH;

   vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
      << "Application Data Servers:" << std::endl << vprDEBUG_FLUSH;

   for (server_map_t::iterator itr = mServers.begin(); itr != mServers.end(); itr++)
   {
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
         << "     Server: " << (*itr).first << std::endl << vprDEBUG_FLUSH;
   }

   vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
     << "Application Barrier Objects:" << std::endl << vprDEBUG_FLUSH;

   for (object_map_t::iterator itr = mObjects.begin(); itr != mObjects.end(); itr++)
   {
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
         << "GUID: " << (*itr).second->getId()
         << std::endl << vprDEBUG_FLUSH;
   }
}

} // End of gadget namespace
