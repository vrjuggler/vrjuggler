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
#include <cluster/Packets/ApplicationDataRequest.h>


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
       return (0 == mPendingApplicationDataRequests.size());
    }

    void ApplicationDataManager::updateAll()
    {
         // State should be PREFRAME for all nodes
       //ClusterNetwork::instance()->setStateAll(ClusterNode::PREFRAME);
         
       mFrameNumber++;
       
       //////////////////////////////////////////////////////////////////////////////
       //                              Send ApplicationData                               //
       //////////////////////////////////////////////////////////////////////////////
       
       //vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
       //   << clrOutBOLD(clrMAGENTA,"[ApplicationDataManager]")
       //   << "Sending ApplicationData.\n" << vprDEBUG_FLUSH;

       mApplicationDataServersLock.acquire();
       std::map<vpr::GUID, ApplicationDataServer*>::iterator ApplicationData_servers_begin = mApplicationDataServers.begin();
       std::map<vpr::GUID, ApplicationDataServer*>::iterator ApplicationData_servers_end = mApplicationDataServers.end();
       for (std::map<vpr::GUID, ApplicationDataServer*>::iterator i = ApplicationData_servers_begin; i != ApplicationData_servers_end; i++)
       {
          (*i).second->updateLocalData();
          (*i).second->send();
       }
       mApplicationDataServersLock.release();
      
      //////////////////////////////////////////////////////////////////////////////
      //                            Send ApplicationData Reqs                            //
      //////////////////////////////////////////////////////////////////////////////
      
      //vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
      //   << clrOutBOLD(clrMAGENTA,"[ApplicationDataManager]")
      //   << "Sending ApplicationData Requests.\n" << vprDEBUG_FLUSH;

         // Send ApplicationData Requests
      sendApplicationDataRequests();    //Only happens if Connected
   }

   void ApplicationDataManager::sendApplicationDataRequests()
   {
      vpr::Guard<vpr::Mutex> guard(mPendingApplicationDataRequestsLock);

      std::map<ApplicationDataRequest*, std::string>::iterator begin = mPendingApplicationDataRequests.begin();
      std::map<ApplicationDataRequest*, std::string>::iterator end   = mPendingApplicationDataRequests.end();
      std::map<ApplicationDataRequest*, std::string>::iterator i;
      for (i = begin ; i != end ; i++)
      {
         ClusterNode* temp_node = ClusterNetwork::instance()->getClusterNodeByHostname((*i).second);
         if (temp_node != NULL && temp_node->isConnected())
         {
            temp_node->send((*i).first);
         }
      }
   }

   ApplicationDataServer* ApplicationDataManager::getApplicationDataServer(vpr::GUID id)
   {
      vpr::Guard<vpr::Mutex> guard(mApplicationDataServersLock);
      
      ApplicationDataServer* temp_app_data_server = mApplicationDataServers[id];
      return(temp_app_data_server);
   }
        
   ApplicationData* ApplicationDataManager::getRemoteApplicationData(vpr::GUID id)
   {
      vpr::Guard<vpr::Mutex> guard(mRemoteApplicationDataLock);

      ApplicationData* temp_remote_app_data = mRemoteApplicationData[id];
      return(temp_remote_app_data);
   }

   void ApplicationDataManager::addPendingApplicationDataRequest(ApplicationDataRequest* new_req, std::string hostname)
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

      for ( i = begin ; i != end ; i++)
      {
         if ((*i).first->getId() == guid)
         {
            mPendingApplicationDataRequests.erase(i);
            return;
         }
      }
   }

   void ApplicationDataManager::addApplicationData(ApplicationData* new_app_data)
   {
      vpr::GUID id = new_app_data->getId();
      std::string hostname = new_app_data->getHostname();

      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) 
         << clrOutBOLD(clrMAGENTA,"[ApplicationDataManager]")
         << " Adding ApplicationData: " << id.toString()
         << std::endl << vprDEBUG_FLUSH;

      setActive(true);

      if (hostname != "")
      {
         if (hostname == ClusterNetwork::instance()->getLocalHostname())
         {
            vpr::Guard<vpr::Mutex> guard(mApplicationDataServersLock);         

            new_app_data->setIsLocal(true);
            
            ApplicationDataServer* new_appdata_server = new ApplicationDataServer(id,new_app_data);                        
            mApplicationDataServers[id] = new_appdata_server;
         }
         else
         {               
               // Create a ApplicationDataRequest
            ApplicationDataRequest* new_appdata_req = new ApplicationDataRequest(id);
            addPendingApplicationDataRequest(new_appdata_req, hostname);
             
               // Add ApplicationData to Remote Vector
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

       // Remove Active ApplicationData
/*       vpr::Guard<vpr::Mutex> guard(mApplicationDataLock);
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
       // Remove Pending ApplicationData
    }

    void ApplicationDataManager::dumpApplicationData()
    {
       vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) 
          << clrOutBOLD(clrMAGENTA,"[ApplicationDataManager]")
          << " Listing All Application Level ApplicationData"
          << std::endl << vprDEBUG_FLUSH;

/*       vpr::Guard<vpr::Mutex> guard(mApplicationDataLock);
       for (std::vector<cluster::ApplicationData*>::iterator i = mApplicationData.begin() ;
            i != mApplicationData.end() ; i++)
       {
          std::string name = (*i)->getAttrib<std::string>("cluster.ApplicationData.name");
          std::string hostname = (*i)->getAttrib<std::string>("cluster.ApplicationData.hostname");
          bool local = (*i)->getAttrib<bool>("cluster.ApplicationData.local");

          vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << name
          << " - " << (local ? "Local" : "Remote") << std::endl << vprDEBUG_FLUSH;
       }
*/      
    }
} // End of gadget namespace
