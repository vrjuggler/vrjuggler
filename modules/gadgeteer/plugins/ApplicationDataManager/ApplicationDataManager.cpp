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

#include <cluster/Plugins/ApplicationDataManager/ApplicationDataManager.h> // my header...

#include <cluster/ClusterNetwork/ClusterNetwork.h>
#include <cluster/ClusterNetwork/ClusterNode.h>

#include <cluster/Packets/EndBlock.h>

//XXX: Need to remove this since we can not depend on another plug-in
//     This is currently here so that we can generate ID's that will
//     not overlap between RIM & ADM. These ID's are checked in the 
//     DataPacket->action() method
#include <cluster/Plugins/RemoteInputManager/RemoteInputManager.h>                                                                    
#include <gadget/Type/DeviceFactory.h>

#include <gadget/Util/Debug.h>
#include <cluster/ClusterDepChecker.h>

//#include <jccl/Config/ConfigChunk.h>
//#include <jccl/Config/ConfigChunkPtr.h>
#include <jccl/RTRC/ConfigManager.h>
#include <jccl/RTRC/DependencyManager.h>

// UserData
#include <cluster/SerializableData.h>
#include <cluster/Plugins/ApplicationDataManager/UserDataServer.h>
#include <cluster/Packets/UserDataRequest.h>


namespace cluster
{
   vprSingletonImp( ApplicationDataManager );

	ApplicationDataManager::ApplicationDataManager()
	{
       // This is done by the ClusterManager
	   //jccl::DependencyManager::instance()->registerChecker(new ClusterDepChecker());
       mFrameNumber = 0;
    }
	ApplicationDataManager::~ApplicationDataManager()
	{
		;
	}

	/** Add the pending chunk to the configuration.
	 *  PRE: configCanHandle (chunk) == true.
	 *  @return true iff chunk was successfully added to configuration.
	 */
	bool ApplicationDataManager::configAdd(jccl::ConfigChunkPtr chunk)
	{
      if (recognizeApplicationDataManagerConfig(chunk))
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrCYAN,"[ApplicationDataManager] ")
            << "Configure the Cluster: " << chunk->getName() 
            << "\n" << vprDEBUG_FLUSH;
         return(true);
      }
	  else
	  {
	     vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) 
            << clrOutBOLD(clrRED,"[ApplicationDataManager::ConfigAdd] ERROR, Something is seriously wrong, we should never get here\n")
			<< vprDEBUG_FLUSH;
         return(true);
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
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "[ApplicationDataManager] Shutdown the Cluster: " << chunk->getName() 
            << "\n" << vprDEBUG_FLUSH;
         return(true);
      }
	  else
	  {
	     vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "[ApplicationDataManager::configRemove] ERROR, Something is seriously wrong, we should never get here\n"
		    << vprDEBUG_FLUSH;
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
		return( recognizeApplicationDataManagerConfig(chunk) );

	}
    
	bool ApplicationDataManager::recognizeApplicationDataManagerConfig(jccl::ConfigChunkPtr chunk)
	{
      //if (chunk->getDescToken() == ApplicationDataManager::getChunkType())
      //{
      //   std::string sync_machine = chunk->getProperty<std::string>("sync_machine");
      //   std::cout << "RIMChunk: " << chunk->getName() << std::endl;
      //   std::cout << "Sync Machine: " << sync_machine << std::endl;
      //
      //   jccl::ConfigChunkPtr device_host_ptr = chunk->getProperty<jccl::ConfigChunkPtr>("sync_machine",0);
      //   std::cout << "SYNC MACHINE !!!!!!!!!!!!!!!!!!!!! " << device_host_ptr->getName() << std::endl;
      //}
   	  return(chunk->getDescToken() == ApplicationDataManager::getChunkType());
	}

    
    void ApplicationDataManager::preDraw()
    {
       updateAll();
    }
    void ApplicationDataManager::postPostFrame()
    {;
    }
    
    bool ApplicationDataManager::isPluginReady()
    {
       return (0 == mPendingUserDataRequests.size());
    }

    void ApplicationDataManager::updateAll()
    {
         // State should be PREFRAME for all nodes
       //ClusterNetwork::instance()->setStateAll(ClusterNode::PREFRAME);
         
       mFrameNumber++;
       
       //////////////////////////////////////////////////////////////////////////////
       //                              Send UserData                               //
       //////////////////////////////////////////////////////////////////////////////
       
       //vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
       //   << clrOutBOLD(clrMAGENTA,"[ApplicationDataManager]")
       //   << "Sending UserData.\n" << vprDEBUG_FLUSH;

       mUserDataServersLock.acquire();
       std::map<std::string, UserDataServer*>::iterator userdata_servers_begin = mUserDataServers.begin();
       std::map<std::string, UserDataServer*>::iterator userdata_servers_end = mUserDataServers.end();
       for (std::map<std::string, UserDataServer*>::iterator i = userdata_servers_begin; i != userdata_servers_end; i++)
       {
          (*i).second->updateLocalData();
          (*i).second->send();
       }
       mUserDataServersLock.release();
      
      //////////////////////////////////////////////////////////////////////////////
      //                            Send UserData Reqs                            //
      //////////////////////////////////////////////////////////////////////////////
      
      //vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
      //   << clrOutBOLD(clrMAGENTA,"[ApplicationDataManager]")
      //   << "Sending UserData Requests.\n" << vprDEBUG_FLUSH;

         // Send UserData Requests
      sendUserDataRequests();    //Only happens if Connected
   }

   void ApplicationDataManager::sendUserDataRequests()
   {
      vpr::Guard<vpr::Mutex> guard(mPendingUserDataRequestsLock);

      std::map<UserDataRequest*, std::string>::iterator begin = mPendingUserDataRequests.begin();
      std::map<UserDataRequest*, std::string>::iterator end   = mPendingUserDataRequests.end();
      std::map<UserDataRequest*, std::string>::iterator i;
      for (i = begin ; i != end ; i++)
      {
         ClusterNode* temp_node = ClusterNetwork::instance()->getClusterNodeByHostname((*i).second);
         if (temp_node != NULL && temp_node->isConnected())
         {
            temp_node->send((*i).first);
         }
      }
   }

   UserDataServer* ApplicationDataManager::getUserDataServer(std::string name)
   {
      vpr::Guard<vpr::Mutex> guard(mUserDataServersLock);
      
      UserDataServer* temp_userdata_server = mUserDataServers[name];
      return(temp_userdata_server);
   }
    
   SerializableData* ApplicationDataManager::getRemoteUserData(std::string name)
   {
      return(NULL);
   }
    
   SerializableData* ApplicationDataManager::getRemoteUserData(vpr::Uint16 id)
   {
      vpr::Guard<vpr::Mutex> guard(mRemoteUserDataLock);

      SerializableData* temp_remote_userdata = mRemoteUserData[id];
      return(temp_remote_userdata);
   }

   void ApplicationDataManager::addPendingUserDataRequest(UserDataRequest* new_userdata_req, std::string hostname)
   {
      vpr::Guard<vpr::Mutex> guard(mPendingUserDataRequestsLock);

      mPendingUserDataRequests[new_userdata_req] = hostname;
   }

   void ApplicationDataManager::removePendingUserDataRequest(std::string userdata_name)
   {
      vpr::Guard<vpr::Mutex> guard(mPendingUserDataRequestsLock);
      std::map<UserDataRequest*, std::string>::iterator begin = mPendingUserDataRequests.begin();
      std::map<UserDataRequest*, std::string>::iterator end = mPendingUserDataRequests.end();
      std::map<UserDataRequest*, std::string>::iterator i;
      for ( i = begin ; i != end ; i++)
      {
         if ((*i).first->getUserDataName() == userdata_name)
         {
            mPendingUserDataRequests.erase(i);
            return;
         }
      }
   }

    void ApplicationDataManager::addUserData(SerializableData* new_user_data)
    {
       std::string name = new_user_data->getAttrib<std::string>("cluster.userdata.name");
       std::string hostname = new_user_data->getAttrib<std::string>("cluster.userdata.hostname");

       vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) 
          << clrOutBOLD(clrMAGENTA,"[ApplicationDataManager]")
          << " Adding UserData: " << name
          << std::endl << vprDEBUG_FLUSH;

       setActive(true);

      if (hostname != "")
      {
         if (hostname == ClusterNetwork::instance()->getLocalHostname())
         {
            vpr::Guard<vpr::Mutex> guard(mUserDataServersLock);         

            new_user_data->setAttrib("cluster.userdata.local", true);
            UserDataServer* new_userdata_server = new UserDataServer(name,new_user_data);                        
            mUserDataServers[name] = new_userdata_server;
         }
         else
         {
               // Get a new device_id
            vpr::Uint16 new_id = RemoteInputManager::instance()->generateLocalId();
               
               // Create a UserDataRequest
            UserDataRequest* new_userdata_req = 
            new UserDataRequest( /*mUserDataIdGen.generateNewId()*/new_id, name);
            addPendingUserDataRequest(new_userdata_req, hostname);
             
               // Add UserData to Remote Vector
            vpr::Guard<vpr::Mutex> guard(mRemoteUserDataLock);
            mRemoteUserData[new_id] = new_user_data;
         }
      }
       //if (!new_user_data->getAttrib<bool>("cluster.userdat.local"))
       //{
          // Send Device Request
          // Local Cluster Nodes
          // Create New ClusterNode
          // Add Node to Pending
          // Add 

          // WE ARE WILL HAVE TO MOVE THIS INTO configAdd and worry about it there????
          // BUT IF WE DO NOT HAVE A CONFIG CHUNK THEN WE WILL NEED TO SIMPLY LOOK AT
          // THIS TABLE WHEN WE CONNECTED TO A REMOTE NODE....WE WILL HAVE 



          // WE CAN DO THIS AT CONFIGADD FOR CLUSTER NODE

          //ClusterNode* remote_node = ClusterNetwork::instance()->getPendingNode(hostname);
          //if ( remote_node == NULL)
          //{
          //   ClusterNetwork::instance()->lockClusterNodes();
          //   ClusterNode* remote_node = ClusterNetwork::instance()->getClusterNodeByHostname(hostname);
          //   ClusterNetwork::instance()->unlockClusterNodes();
          //
          //   ClusterNetwork::instance()->addPendingNode(remote_node);
          //   
          //}
          //UserDataRequest* user_data_request = new UserDataRequest(generateLocalId(), new_user_data->getName());
          //remote_node->addDeviceRequest(user_data_request);
       //}
       
       //dumpUserData();
    }

    void ApplicationDataManager::removeUserData(SerializableData* old_user_data)
    {
       std::string name = old_user_data->getAttrib<std::string>("cluster.userdata.name");
       std::string hostname = old_user_data->getAttrib<std::string>("cluster.userdata.hostname");

       vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) 
          << clrOutBOLD(clrMAGENTA,"[ApplicationDataManager]")
          << " Removing UserData: " << name
          << std::endl << vprDEBUG_FLUSH;

       // Remove Active UserData
/*       vpr::Guard<vpr::Mutex> guard(mUserDataLock);
       for (std::vector<cluster::SerializableData*>::iterator i = mUserData.begin() ;
            i != mUserData.end() ; i++)
       {
          if ((*i) == old_user_data)
          {
             mUserData.erase(i);
             return;
          }
       }
*/
       // Remove Pending UserData
    }

    void ApplicationDataManager::dumpUserData()
    {
       vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) 
          << clrOutBOLD(clrMAGENTA,"[ApplicationDataManager]")
          << " Listing All Application Level UserData"
          << std::endl << vprDEBUG_FLUSH;

/*       vpr::Guard<vpr::Mutex> guard(mUserDataLock);
       for (std::vector<cluster::SerializableData*>::iterator i = mUserData.begin() ;
            i != mUserData.end() ; i++)
       {
          std::string name = (*i)->getAttrib<std::string>("cluster.userdata.name");
          std::string hostname = (*i)->getAttrib<std::string>("cluster.userdata.hostname");
          bool local = (*i)->getAttrib<bool>("cluster.userdata.local");

          vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << name
          << " - " << (local ? "Local" : "Remote") << std::endl << vprDEBUG_FLUSH;
       }
*/      
    }
} // End of gadget namespace
