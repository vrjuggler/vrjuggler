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

#include <plugins/ApplicationDataManager/ApplicationDataManager.h>

#include <cluster/ClusterManager.h>
#include <cluster/ClusterNetwork.h>
#include <gadget/Node.h>
#include <cluster/Packets/EndBlock.h>

#include <gadget/Type/DeviceFactory.h>

#include <gadget/Util/Debug.h>
#include <gadget/gadgetParam.h>
#include <cluster/ClusterDepChecker.h>

#include <jccl/RTRC/ConfigManager.h>
#include <jccl/RTRC/DependencyManager.h>

#include <plugins/ApplicationDataManager/ApplicationData.h>
#include <plugins/ApplicationDataManager/ApplicationDataServer.h>

#include <cluster/Packets/Packet.h>
#include <cluster/Packets/ApplicationDataRequest.h>
#include <cluster/Packets/ApplicationDataAck.h>
#include <cluster/Packets/DataPacket.h>

extern "C"
{
   GADGET_CLUSTER_PLUGIN_EXPORT(vpr::Uint32) getGadgeteerVersion()
   {
      return __GADGET_version;
   }

   GADGET_CLUSTER_PLUGIN_EXPORT(void) initPlugin(cluster::ClusterManager* mgr)
   {
      mgr->addPlugin(new cluster::ApplicationDataManager());
   }
}

namespace cluster
{
   ApplicationDataManager::ApplicationDataManager()
      : mHandlerGUID("cc6ca39f-03f2-4779-aa4b-048f774ff9a5")
      , mFrameNumber(0)
   {;}

   ApplicationDataManager::~ApplicationDataManager()
   {;}

   bool ApplicationDataManager::isPluginReady()
   {
      // This plugin will be ready when it does not have any more
      // PendingApplicationDataRequests
      vpr::Guard<vpr::Mutex> guard(mPendingApplicationDataRequestsLock);
      return(0 == mPendingApplicationDataRequests.size());
   }

   void ApplicationDataManager::handlePacket(Packet* packet, gadget::Node* node)
   {
      if ( NULL != packet && NULL != node )
      {
         switch ( packet->getPacketType() )
         {
         case cluster::Header::RIM_DATA_PACKET:
         {
            DataPacket* temp_data_packet = dynamic_cast<DataPacket*>(packet);
            vprASSERT(NULL != temp_data_packet && "Dynamic cast failed!");

            // Find the ApplicationData Object that we have received data
            // for.
            ApplicationData* user_data =
               getRemoteApplicationData(temp_data_packet->getObjectId());
            if (user_data != NULL)
            {
               // Create a object reader to parse the object's data with
               vpr::BufferObjectReader* temp_reader =
                  new vpr::BufferObjectReader(temp_data_packet->getDeviceData());

               // Parse the object's data using the temporary ObjectReader
               user_data->readObject(temp_reader);
            }
            break;
         }
         case cluster::Header::RIM_APPDATA_REQ:
         {
            ApplicationDataRequest* temp_request =
               dynamic_cast<ApplicationDataRequest*>(packet);
            vprASSERT(NULL != temp_request && "Dynamic cast failed!");

            // Find the ApplicationDataServer that is serving the data for
            // this requested object.
            ApplicationDataServer* temp_app_data_server =
               getApplicationDataServer(temp_request->getId());
            ApplicationDataAck* temp_ack(NULL);

            // -If a ApplicationDataServer for this object exists
            //   -Add the requesting node as a client
            //   -Respond with an ACK
            // -Else
            //   -Respond with an NACK
            // -Free the memory used be the ACK/NACK

            if (temp_app_data_server != NULL)
            {
               temp_app_data_server->addClient(node);
               temp_ack = new ApplicationDataAck(getHandlerGUID(),
                                                 temp_request->getId(), true);
            }
            else
            {
               temp_ack = new ApplicationDataAck(getHandlerGUID(),
                                                 temp_request->getId(), false);
            }
            node->send(temp_ack);
            delete temp_ack;
            break;
         }
         case cluster::Header::RIM_APPDATA_ACK:
         {
            ApplicationDataAck* temp_ack =
               dynamic_cast<ApplicationDataAck*>(packet);
            vprASSERT(NULL != temp_ack && "Dynamic cast failed!");

            // -If we received an ACK
            //   -Remove the coresponding ApplicationDataRequest from the
            //    pending list
            if (temp_ack->getAck())
            {
               removePendingApplicationDataRequest(temp_ack->getId());
            }
            break;
         }
         default:
            vprDEBUG(gadgetDBG_RIM,vprDBG_WARNING_LVL)
               << clrOutBOLD(clrCYAN,"[ApplicationDataManager] ")
               << "Don't know how to handle a packet of type: "
               << packet->getPacketType() << std::endl << vprDEBUG_FLUSH;
            break;
         } // End switch
      } // End if
   }

   std::string ApplicationDataManager::getElementType()
   {
      return "application_data_manager";
   }

   /** Add the pending element to the configuration.
    *  @pre configCanHandle(element) == true.
    *  @return true iff element was successfully added to configuration.
    */
   bool ApplicationDataManager::configAdd(jccl::ConfigElementPtr element)
   {
      if (recognizeApplicationDataManagerConfig(element))
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_WARNING_LVL)
            << clrOutBOLD(clrCYAN,"[ApplicationDataManager] ")
            << "The ApplicationDataMananger does not currently support a "
            << "need for configuration element: " << element->getName()
            << "\n" << vprDEBUG_FLUSH;
         return(true);
      }
      else if (recognizeApplicationDataConfig(element))
      {
         std::string guid_string = element->getProperty<std::string>("guid");
         vpr::GUID guid(guid_string);
         std::string hostname = element->getProperty<std::string>("hostname");

         vpr::Guard<vpr::Mutex> guard(mNeedsConfigedLock);
         std::vector<ApplicationData*>::iterator begin = mNeedsConfiged.begin();
         std::vector<ApplicationData*>::iterator end = mNeedsConfiged.end();
         std::vector<ApplicationData*>::iterator i;

         for (i = begin ; i != end ; ++i)
         {
            if ((*i)->getId() == guid)
            {
               vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
                  << "[ApplicationDataManager::configAdd()] "
                  << "Configuring ApplicationData object " << guid.toString()
                  << " on host: " << hostname << std::endl << vprDEBUG_FLUSH;

               (*i)->setHostname(hostname);

               if (ClusterNetwork::isLocalHost(hostname))
               {
                  // This application data is local.
                  (*i)->setIsLocal(true);

                  // Adding a new ApplicationData server.
                  vpr::Guard<vpr::Mutex> guard(mApplicationDataServersLock);
                  ApplicationDataServer* new_appdata_server =
                     new ApplicationDataServer(guid, (*i), mHandlerGUID);
                  mApplicationDataServers[guid] = new_appdata_server;
               }
               else
               {
                  // This application data is not local.
                  (*i)->setIsLocal(false);

                  // Create a ApplicationDataRequest.
                  ApplicationDataRequest* new_appdata_req =
                     new ApplicationDataRequest(getHandlerGUID(), guid);

                  // Add ApplicationData request to pending list.
                  addPendingApplicationDataRequest(new_appdata_req, hostname);

                  // Add ApplicationData to Remote Vector.
                  vpr::Guard<vpr::Mutex> guard(mRemoteApplicationDataLock);
                  mRemoteApplicationData[guid] = (*i);
               }
            }
            mNeedsConfiged.erase(i);
            return true;
         }

         vprDEBUG(gadgetDBG_RIM, vprDBG_CONFIG_LVL)
            << "[ApplicationDataManager::configAdd()] "
            << "No unconfigured ApplicationData object found"
            << std::endl << vprDEBUG_FLUSH;

         return false;
      }
      else
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_WARNING_LVL)
            << "[ApplicationDataManager::configAdd()] "
            << clrOutBOLD(clrRED, "WARNING: ")
            << "Don't know how to handle the configuration element: "
            << element->getName() << std::endl << vprDEBUG_FLUSH;
         return(false);
      }
   }


  /** Remove the pending element from the current configuration.
   *  @pre configCanHandle(element) == true.
   *  @return true iff the element (and any objects it represented)
   *          were successfully removed.
   */
   bool ApplicationDataManager::configRemove(jccl::ConfigElementPtr element)
   {
      if (recognizeApplicationDataManagerConfig(element))
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_WARNING_LVL)
            << clrOutBOLD(clrCYAN,"[ApplicationDataManager] ")
            << "The ApplicationDataMananger does not currently support a "
            << "need for configuration element: " << element->getName()
            << "\n" << vprDEBUG_FLUSH;
         return(true);
      }
      else if (recognizeApplicationDataManagerConfig(element))
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_WARNING_LVL)
            << clrOutBOLD(clrCYAN,"[ApplicationDataManager] ")
            << "The ApplicationDataMananger does not currently support a "
            << "need for configuration element: " << element->getName()
            << "\n" << vprDEBUG_FLUSH;
         return(true);
      }
      else
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_WARNING_LVL)
            << "[ApplicationDataManager::ConfigRemove()] "
            << clrOutBOLD(clrRED, "WARNING: ")
            << "Don't know how to handle the configuration element: "
            << element->getName() << std::endl << vprDEBUG_FLUSH;
         return(false);
      }
   }

   /** Checks if this handler can process element.
    *  Typically, an implementation of handler will check the element's
    *  description name/token to decide if it knows how to deal with
    *  it.
    *  @return true iff this handler can process element.
    */
   bool ApplicationDataManager::configCanHandle(jccl::ConfigElementPtr element)
   {
      return( recognizeApplicationDataManagerConfig(element) ||
              recognizeApplicationDataConfig(element) );
   }

   /**
    * Helper function that checks the type of a given element against
    * the type that the ApplicationDataManager accepts.
    */
   bool ApplicationDataManager::recognizeApplicationDataManagerConfig(jccl::ConfigElementPtr element)
   {
      return(element->getID() == ApplicationDataManager::getElementType());
   }

   /**
    * Helper function that checks the type of a given element against
    * the type that the ApplicationDataManager accepts.
    */
   bool ApplicationDataManager::recognizeApplicationDataConfig(jccl::ConfigElementPtr element)
   {
      return(element->getID() == "application_data");
   }

   void ApplicationDataManager::postPostFrame()
   {;}

   void ApplicationDataManager::preDraw()
   {
      //mFrameNumber++;

      // Send ApplicationData
      vprDEBUG(gadgetDBG_RIM, vprDBG_HVERB_LVL)
         << clrOutBOLD(clrMAGENTA,"[ApplicationDataManager::preDraw()]")
         << "Sending ApplicationData.\n" << vprDEBUG_FLUSH;

      vpr::Guard<vpr::Mutex> guard(mApplicationDataServersLock);

      std::map<vpr::GUID, ApplicationDataServer*>::iterator application_data_servers_begin = mApplicationDataServers.begin();
      std::map<vpr::GUID, ApplicationDataServer*>::iterator application_data_servers_end = mApplicationDataServers.end();

      // For each ApplicationDataServer on this node, serialize the data and send it to each of it's clients.
      for (std::map<vpr::GUID, ApplicationDataServer*>::iterator i = application_data_servers_begin;
           i != application_data_servers_end;
           ++i)
      {
       (*i).second->serializeAndSend();
      }
   }

   void ApplicationDataManager::sendRequests()
   {
      // Send ApplicationData Requests
      vpr::Guard<vpr::Mutex> guard(mPendingApplicationDataRequestsLock);

      std::map<ApplicationDataRequest*, std::string>::iterator begin = mPendingApplicationDataRequests.begin();
      std::map<ApplicationDataRequest*, std::string>::iterator end   = mPendingApplicationDataRequests.end();
      std::map<ApplicationDataRequest*, std::string>::iterator i;

      // For each pending ApplicationData request
      for (i = begin ; i != end ; ++i)
      {
         // Get the Node that the request is for.
         gadget::Node* temp_node =
            ClusterManager::instance()->getNetwork()->getNodeByHostname((*i).second);

         // If the node exists in the ClusterNetwork
         if (temp_node != NULL)
         {
            // If the Node is connected
            if ( gadget::Node::CONNECTED == temp_node->getStatus() || 
                 gadget::Node::NEWCONNECTION == temp_node->getStatus() )
            {
               // Send the request
               temp_node->send((*i).first);
            }
            else
            {
               // Else add the Node to the list of pending Nodes so that we
               // will connect to it.
               temp_node->setStatus(gadget::Node::PENDING);
            } // End if
         } // End if
      } // End for
   }

   ApplicationDataServer* ApplicationDataManager::getApplicationDataServer(const vpr::GUID& guid)
   {
      vpr::Guard<vpr::Mutex> guard(mApplicationDataServersLock);

      std::map<vpr::GUID, ApplicationDataServer*>::iterator begin = mApplicationDataServers.begin();
      std::map<vpr::GUID, ApplicationDataServer*>::iterator end = mApplicationDataServers.end();
      std::map<vpr::GUID, ApplicationDataServer*>::iterator i;

      for ( i = begin ; i != end ; i++)
      {
         if ((*i).first == guid)
         {
            return((*i).second);
         }
      }

      return NULL;
   }

   ApplicationData* ApplicationDataManager::getRemoteApplicationData(const vpr::GUID& id)
   {
      vpr::Guard<vpr::Mutex> guard(mRemoteApplicationDataLock);

      ApplicationData* temp_remote_app_data = mRemoteApplicationData[id];
      return(temp_remote_app_data);
   }

   void ApplicationDataManager::addPendingApplicationDataRequest(ApplicationDataRequest* new_req, const std::string& hostname)
   {
      vpr::Guard<vpr::Mutex> guard(mPendingApplicationDataRequestsLock);

      mPendingApplicationDataRequests[new_req] = hostname;
   }

   void ApplicationDataManager::removePendingApplicationDataRequest(const vpr::GUID& guid)
   {
      vpr::Guard<vpr::Mutex> guard(mPendingApplicationDataRequestsLock);
      std::map<ApplicationDataRequest*, std::string>::iterator begin = mPendingApplicationDataRequests.begin();
      std::map<ApplicationDataRequest*, std::string>::iterator end = mPendingApplicationDataRequests.end();
      std::map<ApplicationDataRequest*, std::string>::iterator i;

      // For each pending ApplicationData request, if the requested object id equals the given, erase the request from the pwnding list.
      for ( i = begin ; i != end ; i++)
      {
         if ((*i).first->getId() == guid)
         {
            mPendingApplicationDataRequests.erase(i);
            return;
         }
      }
   }

   void ApplicationDataManager::addSerializableObject(vpr::SerializableObject* object)
   {
      ApplicationData* new_app_data = static_cast<ApplicationData*>(object);

      vpr::GUID id = new_app_data->getId();
      std::string hostname = new_app_data->getHostname();

      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
         << clrOutBOLD(clrMAGENTA,"[ApplicationDataManager]")
         << " Adding ApplicationData: " << id.toString()
         << std::endl << vprDEBUG_FLUSH;

      // This plugin is now being used(active)
      setActive(true);

      if ("" == hostname)
      {
         vpr::Guard<vpr::Mutex> guard(mNeedsConfigedLock);
         mNeedsConfiged.push_back(new_app_data);
      }
      else
      {
         if (ClusterNetwork::isLocalHost(hostname))
         {
            // This application data is local.
            new_app_data->setIsLocal(true);

            // Adding a new ApplicationData server.
            vpr::Guard<vpr::Mutex> guard(mApplicationDataServersLock);
            ApplicationDataServer* new_appdata_server = new ApplicationDataServer(id, new_app_data, mHandlerGUID);
            mApplicationDataServers[id] = new_appdata_server;
         }
         else
         {
            // This application data is not local.
            new_app_data->setIsLocal(false);

            // Create a ApplicationDataRequest.
            ApplicationDataRequest* new_appdata_req = new ApplicationDataRequest(getHandlerGUID() ,id);

            // Add ApplicationData request to pending list.
            addPendingApplicationDataRequest(new_appdata_req, hostname);

            // Add ApplicationData to Remote Vector.
            vpr::Guard<vpr::Mutex> guard(mRemoteApplicationDataLock);
            mRemoteApplicationData[id] = new_app_data;
         }
      }
   }

   void ApplicationDataManager::removeApplicationData(ApplicationData* old_app_data)
   {
      vpr::GUID id = old_app_data->getId();
      std::string hostname = old_app_data->getHostname();

      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
         << clrOutBOLD(clrMAGENTA,"[ApplicationDataManager]")
         << " Removing ApplicationData: " << id.toString()
         << std::endl << vprDEBUG_FLUSH;

      // Remove Pending ApplicationData
      removePendingApplicationDataRequest(old_app_data->getId());
      /*
      // Remove Active ApplicationData
      vpr::Guard<vpr::Mutex> guard(mApplicationDataLock);
      for (std::vector<cluster::ApplicationData*>::iterator i = mApplicationData.begin();
           i != mApplicationData.end();
           ++i)
      {
         if ((*i) == old_user_data)
         {
            mApplicationData.erase(i);
            return;
         }
      }
      */
   }

   void ApplicationDataManager::dumpApplicationData()
   {
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
         << clrOutBOLD(clrMAGENTA,"[ApplicationDataManager]")
         << " Listing All Application Level ApplicationData"
         << std::endl << vprDEBUG_FLUSH;

      vpr::Guard<vpr::Mutex> ads_guard(mApplicationDataServersLock);

      std::map<vpr::GUID, ApplicationDataServer*>::iterator ads_begin =
         mApplicationDataServers.begin();
      std::map<vpr::GUID, ApplicationDataServer*>::iterator ads_end =
         mApplicationDataServers.end();
      std::map<vpr::GUID, ApplicationDataServer*>::iterator ads;

      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
         << "ApplicationDataServers:" << std::endl << vprDEBUG_FLUSH;

      for (ads = ads_begin ; ads != ads_end ; ++ads)
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
            << "     Server: " << (*ads).first << std::endl << vprDEBUG_FLUSH;
      }

      vpr::Guard<vpr::Mutex> rad_guard(mRemoteApplicationDataLock);

      std::map<vpr::GUID, ApplicationData*>::iterator rad_begin =
        mRemoteApplicationData.begin();
      std::map<vpr::GUID, ApplicationData*>::iterator rad_end =
        mRemoteApplicationData.end();
      std::map<vpr::GUID, ApplicationData*>::iterator rad;

      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
        << "Remote ApplicationData Objects:" << std::endl << vprDEBUG_FLUSH;

      for (rad = rad_begin ; rad != rad_end ; ++rad)
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
            << "Hostname: " << (*rad).second->getHostname()
            << "GUID: " << (*rad).second->getId()
            << std::endl << vprDEBUG_FLUSH;
      }
   }
} // End of gadget namespace
