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

#include <cluster/Plugins/PluginConfig.h>

#include <cluster/Plugins/ApplicationDataManager/ApplicationDataManager.h> // my header...

#include <cluster/ClusterNetwork/ClusterNetwork.h>
#include <cluster/ClusterNetwork/ClusterNode.h>
#include <cluster/Packets/EndBlock.h>

#include <gadget/Type/DeviceFactory.h>

#include <gadget/Util/Debug.h>
#include <cluster/ClusterDepChecker.h>

//#include <jccl/Config/ConfigChunk.h>
//#include <jccl/Config/ConfigChunkPtr.h>
#include <jccl/RTRC/ConfigManager.h>
#include <jccl/RTRC/DependencyManager.h>

// ApplicationData
#include <cluster/Plugins/ApplicationDataManager/ApplicationData.h>
#include <cluster/Plugins/ApplicationDataManager/ApplicationDataServer.h>

#include <cluster/Packets/Packet.h>
#include <cluster/Packets/ApplicationDataRequest.h>
#include <cluster/Packets/ApplicationDataAck.h>
#include <cluster/Packets/DataPacket.h>

cluster::ClusterPlugin* initPlugin()
{
   return cluster::ApplicationDataManager::instance();
}

namespace cluster
{
   vprSingletonImp( ApplicationDataManager );

   ApplicationDataManager::ApplicationDataManager()
      : mPluginGUID("cc6ca39f-03f2-4779-aa4b-048f774ff9a5"),
      mFrameNumber(0)
   {;}

   ApplicationDataManager::~ApplicationDataManager()
   {;}

   bool ApplicationDataManager::isPluginReady()
   {
      // This plugin will be ready when it does not have any more PendingApplicationDataRequests
      vpr::Guard<vpr::Mutex> guard(mPendingApplicationDataRequestsLock);
      return(0 == mPendingApplicationDataRequests.size());
   }    

   void ApplicationDataManager::handlePacket(Packet* packet, ClusterNode* node)
   {
      if ( NULL != packet && NULL != node )
      {
         switch ( packet->getPacketType() )
         {
         case cluster::Header::RIM_DATA_PACKET:
            {
               DataPacket* temp_data_packet = dynamic_cast<DataPacket*>(packet);
               vprASSERT(NULL != temp_data_packet && "Dynamic cast failed!");
   
               // Find the ApplicationData Object that we have received data for.
               ApplicationData* user_data = getRemoteApplicationData(temp_data_packet->getObjectId());
               if (user_data != NULL)
               {                  
                  // Create a object reader to parse the object's data with
                  vpr::BufferObjectReader* temp_reader = new vpr::BufferObjectReader(temp_data_packet->getDeviceData());
                  
                  // Parse the object's data using the temporary ObjectReader
                  user_data->readObject(temp_reader);
               }
               break;
            }
         case cluster::Header::RIM_APPDATA_REQ:
         {
            ApplicationDataRequest* temp_request = dynamic_cast<ApplicationDataRequest*>(packet);
            vprASSERT(NULL != temp_request && "Dynamic cast failed!");

            // Find the ApplicationDataServer that is serving the data for this requested object.
            ApplicationDataServer* temp_app_data_server = getApplicationDataServer(temp_request->getId());
            ApplicationDataAck* temp_ack = NULL;
                        
            // -If a ApplicationDataServer for this object exists
            //   -Add the requesting node as a client
            //   -Respond with an ACK
            // -Else
            //   -Respond with an NACK
            // -Free the memory used be the ACK/NACK

            if (temp_app_data_server != NULL)
            {
               temp_app_data_server->addClient(node);
               temp_ack = new ApplicationDataAck(getPluginGUID(), temp_request->getId(), true);
            }
            else
            {
               temp_ack = new ApplicationDataAck(getPluginGUID(), temp_request->getId(), false);
            }
            node->send(temp_ack);
            delete temp_ack;
            break;
         }
         case cluster::Header::RIM_APPDATA_ACK:
         {
            ApplicationDataAck* temp_ack = dynamic_cast<ApplicationDataAck*>(packet);
            vprASSERT(NULL != temp_ack && "Dynamic cast failed!");

            // -If we received an ACK
            //   -Remove the coresponding ApplicationDataRequest from the pending list
            if (temp_ack->getAck())
            {
               removePendingApplicationDataRequest(temp_ack->getId());
            }         
            break;
         }
         default:
            {
               vprDEBUG(gadgetDBG_RIM,vprDBG_WARNING_LVL) << clrOutBOLD(clrCYAN,"[ApplicationDataManager] ")
                  << "Don't know how to handle a packet of type: " << packet->getPacketType() 
                  << std::endl << vprDEBUG_FLUSH;
               break;
            }
         } // End switch
      } // End if                 
    }

    /** Add the pending chunk to the configuration.
	 *  PRE: configCanHandle (chunk) == true.
	 *  @return true iff chunk was successfully added to configuration.
	 */
	bool ApplicationDataManager::configAdd(jccl::ConfigChunkPtr chunk)
	{
      if (recognizeApplicationDataManagerConfig(chunk))
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_WARNING_LVL) << clrOutBOLD(clrCYAN,"[ApplicationDataManager] ")
            << "The ApplicationDataMananger does not currently support a need for config chunks: " << chunk->getName() 
            << "\n" << vprDEBUG_FLUSH;
         return(true);
      }
      else if (recognizeApplicationDataConfig(chunk))
      {
         std::string guid_string = chunk->getProperty<std::string>("guid");
         vpr::GUID guid(guid_string);
         std::string hostname = chunk->getProperty<std::string>("hostname");
         
         vpr::Guard<vpr::Mutex> guard(mNeedsConfigedLock);
         std::vector<ApplicationData*>::iterator begin = mNeedsConfiged.begin();
         std::vector<ApplicationData*>::iterator end = mNeedsConfiged.end();
         std::vector<ApplicationData*>::iterator i;

         for (i = begin ; i != end ; i++)
         {
            if ((*i)->getId() == guid)
            {
               vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) 
                  << "[ApplicationDataManager::ConfigAdd()] " 
                  << "Configuring ApplicationData object " << guid.toString() << " on host: " 
                  << hostname << std::endl << vprDEBUG_FLUSH;

                  (*i)->setHostname(hostname);

                  if (hostname == ClusterNetwork::instance()->getLocalHostname())
                  {
                     // This application data is local.
                     (*i)->setIsLocal(true);

                     // Adding a new ApplicationData server.
                     vpr::Guard<vpr::Mutex> guard(mApplicationDataServersLock); 
                     ApplicationDataServer* new_appdata_server = new ApplicationDataServer(guid, (*i), mPluginGUID);
                     mApplicationDataServers[guid] = new_appdata_server;
                  }
                  else
                  {    
                     // This application data is not local.
                     (*i)->setIsLocal(false);

                     // Create a ApplicationDataRequest.
                     ApplicationDataRequest* new_appdata_req = new ApplicationDataRequest(getPluginGUID(), guid);

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

         return false;
      }
	  else
	  {
	     vprDEBUG(gadgetDBG_RIM,vprDBG_WARNING_LVL) 
            << "[ApplicationDataManager::ConfigAdd()] " 
            << clrOutBOLD(clrRED, "WARNING: ") << "Don't know how to handle the config chunk: " 
            << chunk->getName() << std::endl << vprDEBUG_FLUSH;
         return(false);
	  }
	}


   /** Remove the pending chunk from the current configuration.
    *  PRE: configCanHandle (chunk) == true.
    *  @return true iff the chunk (and any objects it represented)
    *          were successfully removed.
    */
   bool ApplicationDataManager::configRemove(jccl::ConfigChunkPtr chunk)
   {
      if (recognizeApplicationDataManagerConfig(chunk))
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_WARNING_LVL) << clrOutBOLD(clrCYAN,"[ApplicationDataManager] ")
            << "The ApplicationDataMananger does not currently support a need for config chunks: " << chunk->getName() 
            << "\n" << vprDEBUG_FLUSH;
         return(true);
      }
      else if (recognizeApplicationDataManagerConfig(chunk))
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_WARNING_LVL) << clrOutBOLD(clrCYAN,"[ApplicationDataManager] ")
            << "The ApplicationDataMananger does not currently support a need for config chunks: " << chunk->getName() 
            << "\n" << vprDEBUG_FLUSH;
         return(true);
      }
	  else
	  {
	     vprDEBUG(gadgetDBG_RIM,vprDBG_WARNING_LVL) 
            << "[ApplicationDataManager::ConfigRemove()] " 
            << clrOutBOLD(clrRED, "WARNING: ") << "Don't know how to handle the config chunk: " 
            << chunk->getName() << std::endl << vprDEBUG_FLUSH;
         return(false);
      }
   }

   /** Checks if this handler can process chunk.
    *  Typically, an implementation of handler will check the chunk's
    *  description name/token to decide if it knows how to deal with
    *  it.
    *  @return true iff this handler can process chunk.
    */
   bool ApplicationDataManager::configCanHandle(jccl::ConfigChunkPtr chunk)
   {
      return( recognizeApplicationDataManagerConfig(chunk) ||
              recognizeApplicationDataConfig(chunk) );
   }
    
   /**
    * Helper function that checks the type of a given chunk against 
    * the type that the ApplicationDataManager accepts.
    */
   bool ApplicationDataManager::recognizeApplicationDataManagerConfig(jccl::ConfigChunkPtr chunk)
   {
      return(chunk->getDescToken() == ApplicationDataManager::getChunkType());
   }  
   
   /**
    * Helper function that checks the type of a given chunk against 
    * the type that the ApplicationDataManager accepts.
    */
   bool ApplicationDataManager::recognizeApplicationDataConfig(jccl::ConfigChunkPtr chunk)
   {
      return(chunk->getDescToken() == "AppData");
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
      for (std::map<vpr::GUID, ApplicationDataServer*>::iterator i = application_data_servers_begin; i != application_data_servers_end; i++)
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
      for (i = begin ; i != end ; i++)
      {
         // Get the ClusterNode that the request is for.
         ClusterNode* temp_node = ClusterNetwork::instance()->getClusterNodeByHostname((*i).second);

         // If the node exists in the ClusterNetwork
         if (temp_node != NULL)
         {
            // If the ClusterNode is connected
            if (temp_node->isConnected())
            {
               // Send the request
               temp_node->send((*i).first);
            }
            else
            {
               // Else add the ClusterNode to the list of pending ClusterNodes so that we will connect to it.
               ClusterNetwork::instance()->addPendingNode(temp_node);
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
         if (hostname == ClusterNetwork::instance()->getLocalHostname())
         {
            // This application data is local.
            new_app_data->setIsLocal(true);
            
            // Adding a new ApplicationData server.
            vpr::Guard<vpr::Mutex> guard(mApplicationDataServersLock);         
            ApplicationDataServer* new_appdata_server = new ApplicationDataServer(id, new_app_data, mPluginGUID);
            mApplicationDataServers[id] = new_appdata_server;
         }
         else
         {    
            // This application data is not local.
            new_app_data->setIsLocal(false);

            // Create a ApplicationDataRequest.
            ApplicationDataRequest* new_appdata_req = new ApplicationDataRequest(getPluginGUID() ,id);

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
       for (std::vector<cluster::ApplicationData*>::iterator i = mApplicationData.begin() ;
            i != mApplicationData.end() ; i++)
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
       
       std::map<vpr::GUID, ApplicationDataServer*>::iterator ads_begin = mApplicationDataServers.begin();
       std::map<vpr::GUID, ApplicationDataServer*>::iterator ads_end = mApplicationDataServers.end();
       std::map<vpr::GUID, ApplicationDataServer*>::iterator ads;

       vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "ApplicationDataServers:" << std::endl << vprDEBUG_FLUSH;          
       
       for (ads = ads_begin ; ads != ads_end ; ads++)
       {
          vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "     Server: "
          << (*ads).first << std::endl << vprDEBUG_FLUSH;          
       }
       
       vpr::Guard<vpr::Mutex> rad_guard(mRemoteApplicationDataLock);
       
       std::map<vpr::GUID, ApplicationData*>::iterator rad_begin = mRemoteApplicationData.begin();
       std::map<vpr::GUID, ApplicationData*>::iterator rad_end = mRemoteApplicationData.end();
       std::map<vpr::GUID, ApplicationData*>::iterator rad;

       vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "Remote ApplicationData Objects:" << std::endl << vprDEBUG_FLUSH;          
       
       for (rad = rad_begin ; rad != rad_end ; rad++)
       {
          vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
               << "Hostname: " << (*rad).second->getHostname()
               << "GUID: " << (*rad).second->getId() 
               << std::endl << vprDEBUG_FLUSH;          
       }   
    }
} // End of gadget namespace
