/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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

#include <plugins/ApplicationDataManager/ApplicationDataManager.h>

#include <cluster/ClusterManager.h>
#include <gadget/Node.h>
#include <gadget/Util/Debug.h>
#include <gadget/gadgetParam.h>

#include <plugins/ApplicationDataManager/ApplicationData.h>
#include <plugins/ApplicationDataManager/ApplicationDataServer.h>

#include <cluster/Packets/DataPacket.h>

extern "C"
{
   GADGET_CLUSTER_PLUGIN_EXPORT(vpr::Uint32) getGadgeteerVersion()
   {
      return __GADGET_version;
   }

   GADGET_CLUSTER_PLUGIN_EXPORT(void) initPlugin(cluster::ClusterManager* mgr)
   {
      mgr->addPlugin(cluster::ApplicationDataManager::create());
   }
}

namespace cluster
{

const vpr::GUID ApplicationDataManager::mPluginGUID("cc6ca39f-03f2-4779-aa4b-048f774ff9a5");

ApplicationDataManager::ApplicationDataManager()
{;}

ClusterPluginPtr ApplicationDataManager::create()
{
   return ClusterPluginPtr(new ApplicationDataManager());
}

ApplicationDataManager::~ApplicationDataManager()
{
   // Memory for each ApplicationData object is handled by application.
   mObjects.clear();
   mServers.clear();
}

bool ApplicationDataManager::isPluginReady()
{
   return true;
}

void ApplicationDataManager::handlePacket(PacketPtr packet, gadget::NodePtr node)
{
   if ( NULL != packet.get() && NULL != node.get() )
   {
      switch ( packet->getPacketType() )
      {
      case cluster::Header::RIM_DATA_PACKET:
      {
         DataPacketPtr data_packet = boost::dynamic_pointer_cast<DataPacket>(packet);
         vprASSERT(NULL != data_packet.get() && "Dynamic cast failed!");

         // Find the ApplicationData Object that we have received data
         // for.
         ApplicationData* user_data =
            getApplicationData(data_packet->getObjectId());

         if (user_data != NULL)
         {
            // Create a object reader to parse the object's data with
            vpr::BufferObjectReader* temp_reader =
               new vpr::BufferObjectReader(data_packet->getDeviceData());

            // Parse the object's data using the temporary ObjectReader
            user_data->readObject(temp_reader);
         }
         else
         {
            vprDEBUG(gadgetDBG_RIM,vprDBG_WARNING_LVL)
               << clrOutBOLD(clrCYAN,"[ApplicationDataManager] ")
               << "Got data for an unknown ApplicationData object: "
               << data_packet->getObjectId() << std::endl << vprDEBUG_FLUSH;
         }
         break;
      }
      default:
         vprDEBUG(gadgetDBG_RIM,vprDBG_WARNING_LVL)
            << clrOutBOLD(clrCYAN,"[ApplicationDataManager] ")
            << "Don't know how to handle a packet of type: "
            << packet->getPacketType() << std::endl << vprDEBUG_FLUSH;
         break;
      }
   }
}

void ApplicationDataManager::postPostFrame()
{;}

void ApplicationDataManager::preDraw()
{
   if (ClusterManager::instance()->isMaster())
   {
      // Send ApplicationData
      vprDEBUG(gadgetDBG_RIM, vprDBG_HVERB_LVL)
         << clrOutBOLD(clrMAGENTA,"[ApplicationDataManager::preDraw()]")
         << "Sending ApplicationData.\n" << vprDEBUG_FLUSH;

      // For each ApplicationDataServer on this node, serialize the data and send it to each of it's clients.
      for (server_map_t::iterator itr = mServers.begin(); itr != mServers.end(); itr++)
      {
         (*itr).second->serializeAndSend();
      }
   }
}

void ApplicationDataManager::sendRequests()
{;}

void ApplicationDataManager::addSerializableObject(vpr::SerializableObject* object)
{
   ApplicationData* new_app_data = static_cast<ApplicationData*>(object);

   if (NULL == new_app_data)
   {
      throw ClusterException("Could not cast to ApplicationData", VPR_LOCATION);
   }

   const vpr::GUID id = new_app_data->getId();

   // TODO: Remove hostname field from application data.
   const std::string hostname = new_app_data->getHostname();

   vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
      << clrOutBOLD(clrMAGENTA,"[ApplicationDataManager]")
      << " Adding ApplicationData: " << id.toString()
      << std::endl << vprDEBUG_FLUSH;

   mObjects[id] = new_app_data;

   if (ClusterManager::instance()->isMaster())
   {
      new_app_data->setIsLocal(true);
      ApplicationDataServerPtr new_server
         = ApplicationDataServer::create(id, new_app_data, mPluginGUID);
      mServers[id] = new_server;
   }
   else
   {
      new_app_data->setIsLocal(false);
   }

   // This plugin is now being used(active)
   setActive(true);
}

void ApplicationDataManager::dumpApplicationData()
{
   vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
      << clrOutBOLD(clrMAGENTA,"[ApplicationDataManager]")
      << " Listing All Application Level ApplicationData"
      << std::endl << vprDEBUG_FLUSH;

   vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
      << "Application Data Servers:" << std::endl << vprDEBUG_FLUSH;

   for (server_map_t::iterator itr = mServers.begin(); itr != mServers.end(); itr++)
   {
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
         << "     Server: " << (*itr).first << std::endl << vprDEBUG_FLUSH;
   }

   vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
     << "Application Data Objects:" << std::endl << vprDEBUG_FLUSH;

   for (object_map_t::iterator itr = mObjects.begin(); itr != mObjects.end(); itr++)
   {
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
         << "Hostname: " << (*itr).second->getHostname()
         << "GUID: " << (*itr).second->getId()
         << std::endl << vprDEBUG_FLUSH;
   }
}

} // End of gadget namespace
