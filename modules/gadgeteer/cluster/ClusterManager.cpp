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

#include <cluster/ClusterManager.h> // my header...

#include <vpr/DynLoad/LibraryFinder.h>
#include <vpr/Util/FileUtils.h>

#include <cluster/ClusterNetwork/ClusterNetwork.h>
#include <cluster/ClusterNetwork/ClusterNode.h>

#include <cluster/Packets/EndBlock.h>
#include <cluster/Packets/ConnectionRequest.h>
#include <cluster/Packets/ConnectionAck.h>

#include <cluster/ClusterPlugin.h>
#include <gadget/Type/DeviceFactory.h>

#include <gadget/Util/Debug.h>
#include <cluster/ClusterDepChecker.h>

//#include <jccl/Config/ConfigChunk.h>
//#include <jccl/Config/ConfigChunkPtr.h>
#include <jccl/RTRC/ConfigManager.h>
#include <jccl/RTRC/DependencyManager.h>

#include <cluster/Packets/Packet.h>
#include <cluster/Packets/PacketFactory.h>
#include <cluster/Packets/StartBlock.h>


namespace cluster
{
   vprSingletonImp( ClusterManager );

   ClusterManager::ClusterManager() : mClusterActive(false), mClusterReady(false)
   {
      jccl::ConfigManager::instance()->addConfigChunkHandler(ClusterNetwork::instance());
      jccl::DependencyManager::instance()->registerChecker(new ClusterDepChecker());      
   }
   ClusterManager::~ClusterManager()
   {
      ;
   }

   bool ClusterManager::isClusterReady()
   {
      // -If the cluster is active and not ready
      //   -If a StartBarrier ConfigChunk does not exist
      //    -Warn the user and set cluster ready
      //   -Get new value of mClusterReady from asking all plugins
      // -Return the new mClusterReady

      vpr::Guard<vpr::Mutex> ready_guard(mClusterReadyLock);
      vpr::Guard<vpr::Mutex> active_guard(mClusterActiveLock);

      if (mClusterActive && !mClusterReady)
      {
         if (!jccl::ConfigManager::instance()->isChunkTypeInActiveList("StartBarrierPlugin") &&
             !jccl::ConfigManager::instance()->isChunkTypeInPendingList("StartBarrierPlugin"))
         {
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) 
            << clrOutBOLD(clrRED, "StartBarrier not config chunk does not exist. ") 
            << clrOutBOLD(clrRED, "If your application depends on each node starting at the same ")
            << clrOutBOLD(clrRED, "time you should add a StartBarrierPlugin configuration chunk.")
            << std::endl << vprDEBUG_FLUSH;

            std::cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX DONE - ERROR XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << std::endl;

            mClusterReady = true;
         }
      }
      // Lock it here so that we can avoid confusion in pluginsReady()
      vpr::Guard<vpr::Mutex> guard(mPluginsLock);
      return(mClusterReady && pluginsReady());
   }

   bool ClusterManager::pluginsReady()
   {
      // Plugins are already locked since we only call this method from 
      // isClusterReady which is only called by StartBarrierPlugin::postPostFrame 
      // which has already locked the list of plugins.

      //vpr::Guard<vpr::Mutex> guard(mPluginsLock);

      for (std::list<ClusterPlugin*>::iterator i = mPlugins.begin();
           i != mPlugins.end() ; i++)
      {
         if (!(*i)->isPluginReady())
            return false;
      }

      return true;
   }
   
   void ClusterManager::recoverFromLostNode(ClusterNode* lost_node)
   {
      vpr::Guard<vpr::Mutex> guard(mPluginsLock);

      for (std::list<ClusterPlugin*>::iterator i = mPlugins.begin();
           i != mPlugins.end() ; i++)
      {
         (*i)->recoverFromLostNode(lost_node);
      }
   }

   void ClusterManager::handlePacket(Packet* packet, ClusterNode* node)
   {
      // If the ClusterManager should handle this packet, then do so.
      if (packet->getPacketType() == Header::RIM_END_BLOCK)
      {
         // -Set New State
         if (node == NULL)
         {
            return;
         }
         
         //We don't actually use the temp_end_block for anything here.
         // 
         //EndBlock* temp_end_block = dynamic_cast<EndBlock*>(packet);
         //vprASSERT(NULL != temp_end_block && "Dynamic cast failed!");
         
	 //vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrCYAN,"[ClusterManager] ")
         //   << "Recv EndBlock: " << temp_end_block->getTempVar() << std::endl << vprDEBUG_FLUSH;           
         
         node->setUpdated(true);
         return;
      }
      else if (packet->getPacketType() == Header::RIM_CONNECTION_REQ ||
               packet->getPacketType() == Header::RIM_CONNECTION_ACK)
      {
         ClusterNetwork::instance()->handlePacket(packet, node);
      }
      
      vpr::GUID plugin_guid = packet->getPluginId();
      
      ClusterPlugin* temp_plugin = getPluginByGUID(plugin_guid);
      
      if (NULL != temp_plugin)
      {
         temp_plugin->handlePacket(packet, node);
      }
      else
      {
	     vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrCYAN,"[ClusterManager] ")
            << "Plugin " << plugin_guid.toString() << " does not exist to handle this packet" 
            << std::endl << vprDEBUG_FLUSH;           
      }
   }

   /**
    * Adds a new plugin to the ClusterManager.
    */
   void ClusterManager::addPlugin(ClusterPlugin* new_plugin)
   {
      vpr::Guard<vpr::Mutex> guard(mPluginsLock);
      if (!doesPluginExist(new_plugin))
      {
         mPlugins.push_back(new_plugin);
         std::pair<vpr::GUID, ClusterPlugin*> p = std::make_pair(new_plugin->getPluginGUID(), new_plugin);
         mPluginMap.insert(p);

         // We should do this here, but since we do not add the manager until its configAdd
         // currently you can see the problem
         jccl::ConfigManager::instance()->addConfigChunkHandler(new_plugin);
         //We can still unregister it when removed below though
	     vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrCYAN,"[ClusterManager] ")
            << "Adding Plugin: " << new_plugin->getPluginName() <<std::endl << vprDEBUG_FLUSH;
      }
   }

   ClusterPlugin* ClusterManager::getPluginByGUID(const vpr::GUID& plugin_guid)
   {
      std::map<vpr::GUID, ClusterPlugin*>::const_iterator i = mPluginMap.find(plugin_guid);
      if(i != mPluginMap.end())
      {
         return ((*i).second);
      }
      return NULL;
   }
   
   /**
    * Removes a plugin from the ClusterManager
    */
   void ClusterManager::removePlugin(ClusterPlugin* old_plugin)
   {
      vpr::Guard<vpr::Mutex> guard(mPluginsLock);

      mPluginMap.erase(old_plugin->getPluginGUID());
      
      for (std::list<ClusterPlugin*>::iterator i = mPlugins.begin();
           i != mPlugins.end() ; i++)
      {
         if ((*i) == old_plugin)
         {
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrCYAN,"[ClusterManager] ")
            << "Removing Plugin: " << old_plugin->getPluginName() <<std::endl << vprDEBUG_FLUSH;
            mPlugins.erase(i);
            jccl::ConfigManager::instance()->removeConfigChunkHandler(*i);
            return;
         }
      }
   }

   /**
    * Checks if a plugin exists in the ClusterManager
    */
   bool ClusterManager::doesPluginExist(ClusterPlugin* old_manager)
   {
      vprASSERT(mPluginsLock.test() == 1 && "mManagers Lock must be aquired before calling ClusterManager::doesManagerExist()");
      
      for (std::list<ClusterPlugin*>::iterator i = mPlugins.begin();
           i != mPlugins.end() ; i++)
      {
         if ((*i) == old_manager)
         {
            return(true);
         }
      }
      return(false);
   }
   
   void ClusterManager::sendRequests()
   {
      // Idea is to not create frame lock if we do not need to
      bool updateNeeded = false;
      vpr::Guard<vpr::Mutex> guard(mPluginsLock);
      
//      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrCYAN,"[ClusterManager] ")
//         << "Send Requests\n" << vprDEBUG_FLUSH;         

      for (std::list<ClusterPlugin*>::iterator i = mPlugins.begin();
           i != mPlugins.end() ; i++)
      {
         (*i)->sendRequests();
         updateNeeded = true;
      }
      if (updateNeeded)
      {
         sendEndBlocksAndSignalUpdate(1);
      }
   }

   void ClusterManager::preDraw()
   {
      // Idea is to not create frame lock if we do not need to
      bool updateNeeded = false;
      vpr::Guard<vpr::Mutex> guard(mPluginsLock);
      
//      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrCYAN,"[ClusterManager] ")
//         << "PreDraw\n" << vprDEBUG_FLUSH;         
      
      for (std::list<ClusterPlugin*>::iterator i = mPlugins.begin();
           i != mPlugins.end() ; i++)
      {
         (*i)->preDraw();
         updateNeeded = true;
      }
      if (updateNeeded)
      {
         sendEndBlocksAndSignalUpdate(2);
      }
   }

   void ClusterManager::postPostFrame()
   {
      // -If not running
      //   -If all plugins ready
      //     - isClusterReady
      
      
      // Idea is to not create frame lock if we do not need to
      bool updateNeeded = false;
      vpr::Guard<vpr::Mutex> guard(mPluginsLock);
      
//      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrCYAN,"[ClusterManager] ")
//         << "postPostFrame\n" << vprDEBUG_FLUSH;         

      for (std::list<ClusterPlugin*>::iterator i = mPlugins.begin();
           i != mPlugins.end() ; i++)
      {
         (*i)->postPostFrame();      
         updateNeeded = true;
      }  
      if (updateNeeded)
      {
//         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrCYAN,"[ClusterManager] ")
//            << "Before End\n" << vprDEBUG_FLUSH;         
         sendEndBlocksAndSignalUpdate(3);
//         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrCYAN,"[ClusterManager] ")
//            << "After End\n" << vprDEBUG_FLUSH;         
      }
   }
   void ClusterManager::createBarrier()
   {  
      vpr::Guard<vpr::Mutex> guard(mPluginsLock);
   
      for (std::list<ClusterPlugin*>::iterator i = mPlugins.begin();
           i != mPlugins.end() ; i++)
      {
         //if ((*i)->isActive())
         //{  // As soon as we find a plug-in that creates 
            // a barrier, we can continue. Maybe not since 
            // this will not match up on different machines
            if((*i)->createBarrier())
            {
               return; 
            }
         //}
      }
   }

   void ClusterManager::sendEndBlocksAndSignalUpdate(int temp)
   {
      ClusterNetwork::instance()->lockClusterNodes();

      std::vector<cluster::ClusterNode*>::iterator begin_cluster_nodes = ClusterNetwork::instance()->getClusterNodesBegin();
      std::vector<cluster::ClusterNode*>::iterator end_cluster_nodes = ClusterNetwork::instance()->getClusterNodesEnd();
   
      cluster::EndBlock* temp_end_block = new EndBlock(temp);
      
//      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrCYAN,"[ClusterManager] ")
//         << "Send EndBlock: " << temp << std::endl << vprDEBUG_FLUSH;           


      for(std::vector<cluster::ClusterNode*>::iterator i=begin_cluster_nodes;i!=end_cluster_nodes;i++)
      {
         if ((*i)->isConnected())
         {
//            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrCYAN,"[ClusterManager] ")
//               << "Send: " << (*i)->getName() << "\n" << vprDEBUG_FLUSH;         

               // Send End Blocks to all connected ClusterNodes
            (*i)->send(temp_end_block);

//            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrCYAN,"[ClusterManager] ")
//               << "signal: " << (*i)->getName() << "\n" << vprDEBUG_FLUSH;         
               // Signal Update thread to read Network Packets
            (*i)->signalUpdate();
         }                         
      }
      for(std::vector<cluster::ClusterNode*>::iterator i=begin_cluster_nodes;i!=end_cluster_nodes;i++)
      {
         if ((*i)->isConnected())
         {
//            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrCYAN,"[ClusterManager] ")
//               << "Sync: " << (*i)->getName() << "\n" << vprDEBUG_FLUSH;         
            //Block waiting for all packets to be received
            (*i)->sync();
         }               
      }
      delete temp_end_block;
      ClusterNetwork::instance()->unlockClusterNodes();
   }

   bool ClusterManager::recognizeRemoteDeviceConfig(jccl::ConfigChunkPtr chunk)
   {  
     if ( gadget::DeviceFactory::instance()->recognizeDevice(chunk) &&  chunk->getNum("deviceHost") > 0 )
     {
        std::string device_host = chunk->getProperty<std::string>("deviceHost");
        //std::cout << "Checking: " << chunk->getName() << std::endl;
        if ( !device_host.empty() )
        {
           // THIS IS A HACK: find a better way to do this
           jccl::ConfigChunkPtr device_host_ptr = getConfigChunkPointer(device_host);
           if (device_host_ptr.get() != NULL)
           {
              std::string host_name = device_host_ptr->getProperty<std::string>("host_name");
              std::string local_host_name = cluster::ClusterNetwork::instance()->getLocalHostname();
              //std::cout << "Host Name: " << host_name << std::endl;
              //std::cout << "Local Host Name: " << local_host_name << std::endl;
              if (host_name != local_host_name)
              {
                 return(true);
              }// Device is on the local machine
           }// Could not find the deviceHost in the configuration
        }// Device is not a remote device since there is no name in the deviceHost field
     }// Else it is not a device, or does not have a deviceHost property
     return false;
   }
   
   bool ClusterManager::recognizeClusterManagerConfig(jccl::ConfigChunkPtr chunk)
   {
     return(chunk->getDescToken() == ClusterManager::getChunkType());
   }

   /** Add the pending chunk to the configuration.
    *  PRE: configCanHandle (chunk) == true.
    *  @return true iff chunk was successfully added to configuration.
    */
   bool ClusterManager::configAdd(jccl::ConfigChunkPtr chunk)
   {
      if (recognizeClusterManagerConfig(chunk))
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrCYAN,"[ClusterManager] ")
            << "Configure the Cluster: " << chunk->getName() 
            << "\n" << vprDEBUG_FLUSH;

         // Get a list of cluster nodes to use for this cluster.
         int num_nodes = chunk->getNum(std::string("cluster_nodes"));
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrCYAN,"[ClusterManager] ")
            << "configAdd() Number of nodes: " << num_nodes
            << "\n" << vprDEBUG_FLUSH;         
         for (int i = 0 ; i < num_nodes ; i++)
         {
            std::string new_node = chunk->getProperty<std::string>(std::string("cluster_nodes"), i);            
                  vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrCYAN,"[ClusterManager] ")
                     << "configAdd() New Node Name: " << new_node
                     << "\n" << vprDEBUG_FLUSH;                     
            jccl::ConfigChunkPtr new_node_chunk = getConfigChunkPointer(new_node);
            std::string new_node_hostname = new_node_chunk->getProperty<std::string>(std::string("host_name"));
                  vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrCYAN,"[ClusterManager] ")
                     << "configAdd() New Node Hostname: " << new_node_hostname
                     << "\n" << vprDEBUG_FLUSH;         
            if (new_node_hostname != ClusterNetwork::instance()->getLocalHostname())
            {
                  vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrCYAN,"[ClusterManager] ")
                     << "configAdd() Added Node since it is non-local\n" << vprDEBUG_FLUSH;         
            
                  vpr::Guard<vpr::Mutex> guard(mClusterNodesLock);
                  mClusterNodes.push_back(new_node_hostname);
            }          
         }

         //Dynamic Plugin Loading
         
         // --- Load ClusterPlugin dsos -- //
         // - Load individual plugins
         const std::string plugin_prop_name("clusterPlugin");
         int plugin_count = chunk->getNum(plugin_prop_name);
         std::string plugin_dso;

         for ( int i = 0; i < plugin_count; ++i )
         {
            plugin_dso =
               vpr::replaceEnvVars(chunk->getProperty<std::string>(plugin_prop_name, i));
            if(!plugin_dso.empty())
            {
               vprDEBUG(gadgetDBG_RIM, vprDBG_STATE_LVL)
                  << "InputMgr::config: Loading ClusterPlugin DSO '" << plugin_dso << "'\n" << vprDEBUG_FLUSH;

               // If any part of the driver loading fails, the object driver_library
               // will go out of scope at the end of this iteration, thereby freeing
               // the allocated memory.
               vpr::LibraryPtr plugin_library =
                  vpr::LibraryPtr(new vpr::Library(plugin_dso));
               this->loadDriverDSO(plugin_library);
            }
         }         
         
         vpr::Guard<vpr::Mutex> guard(mClusterActiveLock);
         mClusterActive = true;
         return true;
      }
	  else
	  {
	     vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) 
            << clrOutBOLD(clrRED,"[ClusterManager::ConfigAdd] ERROR: ")
            << "Something is seriously wrong, we should not be handling this chunk\n"
			<< vprDEBUG_FLUSH;
         return false;
	  }
	}


	/** Remove the pending chunk from the current configuration.
	 *  PRE: configCanHandle (chunk) == true.
	 *  @return true iff the chunk (and any objects it represented)
	 *          were successfully removed.
	 */
   bool ClusterManager::configRemove(jccl::ConfigChunkPtr chunk)
   {
      if (recognizeClusterManagerConfig(chunk))
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "[ClusterManager] Shutdown the Cluster: " << chunk->getName() 
            << "\n" << vprDEBUG_FLUSH;
         return(true);
      }
	  else
	  {
	     vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "[ClusterManager::configRemove] ERROR, Something is seriously wrong, we should never get here\n"
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
	bool ClusterManager::configCanHandle(jccl::ConfigChunkPtr chunk)
	{
		if (recognizeClusterManagerConfig(chunk))
        {
           return true;
        }

/*        for (std::list<ClusterPlugin*>::iterator i = mPlugins.begin();
             i != mPlugins.end() ; i++)
        {
           if ((*i)->configCanHandle(chunk))
           {
              return true ;
           }
        }
*/        
        return false;
	}
   
   
   // ---- Configuration Helper Functions ----   
   jccl::ConfigChunkPtr ClusterManager::getConfigChunkPointer(std::string& name)
   {
      jccl::ConfigManager* cfg_mgr = jccl::ConfigManager::instance();
      //cfg_mgr->lockPending();
      //cfg_mgr->unlockPending();
      for (std::list<jccl::ConfigManager::PendingChunk>::iterator i = cfg_mgr->getPendingBegin();
           i != cfg_mgr->getPendingEnd() ; i++)
      {
         if ((*i).mChunk->getName() == name)
         {
            return((*i).mChunk);
         }
      }
      cfg_mgr->lockActive();
      jccl::ConfigChunkPtr temp = cfg_mgr->getActiveConfig()->get(name);
      cfg_mgr->unlockActive();
      return(temp);

      //return(NULL); //HOW DO I DO THIS WITH BOOST SHARED POINTERS 
   }


vpr::ReturnStatus ClusterManager::loadDriverDSO(vpr::LibraryPtr driverDSO)
{
   vprASSERT(driverDSO.get() != NULL && "Invalid vpr::LibraryPtr object");

   const int lib_name_width(50);

   vprDEBUG(gadgetDBG_RIM, vprDBG_CONFIG_LVL)
      << "Loading plugin library: " << std::setiosflags(std::ios::right)
      << std::setfill(' ') << std::setw(lib_name_width) << driverDSO->getName()
      << std::resetiosflags(std::ios::right) << "     " << vprDEBUG_FLUSH;

   // Load the driver
   vpr::ReturnStatus status;
   status = driverDSO->load();

   if ( status.success() )
   {
      vprDEBUG(gadgetDBG_RIM, vprDBG_WARNING_LVL)
         << "Success DSO loaded.\n" << vprDEBUG_FLUSH;

      ClusterPlugin* (*creator)();

      creator = (ClusterPlugin* (*)()) driverDSO->findSymbol("initPlugin");

      if ( NULL != creator )
      {
         vprDEBUG_CONT(gadgetDBG_RIM,vprDBG_CONFIG_STATUS_LVL) << "[ " << clrSetNORM(clrGREEN) << "OK" << clrRESET << " ]\n" << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_RIM, vprDBG_WARNING_LVL)
            << "Got pointer to driver factory\n" << vprDEBUG_FLUSH;

         mPluginLibraries.push_back(driverDSO);
         ClusterPlugin* new_plugin = (*creator)();
         addPlugin(new_plugin);
      }
      else
      {
         vprDEBUG_CONT(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "[ " << clrSetNORM(clrRED) << "FAILED lookup" << clrRESET << " ]\n" << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_RIM, vprDBG_WARNING_LVL)
            << clrOutNORM(clrYELLOW, "WARNING")
            << ": Failed to look up factory function in driver DSO '"
            << driverDSO << "'\n" << vprDEBUG_FLUSH;
      }
   }
   else
   {
      vprDEBUG_CONT(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "[ " << clrSetNORM(clrRED) << "FAILED" << clrRESET << " ]\n" << vprDEBUG_FLUSH;
   }

   return status;
}


} // End of gadget namespace
