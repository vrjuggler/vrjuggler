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

#include <cluster/ClusterNetwork/ClusterNetwork.h>
#include <cluster/ClusterNetwork/ClusterNode.h>

#include <cluster/Packets/EndBlock.h>
#include <cluster/ClusterPlugin.h>
#include <gadget/Type/DeviceFactory.h>

#include <gadget/Util/Debug.h>
#include <cluster/ClusterDepChecker.h>

//#include <jccl/Config/ConfigChunk.h>
//#include <jccl/Config/ConfigChunkPtr.h>
#include <jccl/RTRC/ConfigManager.h>
#include <jccl/RTRC/DependencyManager.h>

//NEED TO REMOVE THESE FILES
#include <cluster/Plugins/RemoteInputManager/RemoteInputManager.h>
#include <cluster/Plugins/ApplicationDataManager/ApplicationDataManager.h>
#include <cluster/Plugins/SwapLockPlugin/SwapLockPlugin.h>

#include <cluster/Packets/PacketFactory.h>
#include <cluster/Packets/StartBlock.h>


namespace cluster
{
   vprSingletonImp( ClusterManager );

   ClusterManager::ClusterManager()
   {
      jccl::ConfigManager::instance()->addConfigChunkHandler(ClusterNetwork::instance());
      jccl::DependencyManager::instance()->registerChecker(new ClusterDepChecker());
      
      //We start all nodes thinking they are masters
      //This makes us not actually do anything bellow
      //Until configuration occurs
      mRunning = false;
      mBarrierMaster = false;
   }
   ClusterManager::~ClusterManager()
   {
      ;
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
         // We should do this here, but since we do not add the manager until its configAdd
         // currently you can see the problem
         jccl::ConfigManager::instance()->addConfigChunkHandler(new_plugin);
         //We can still unregister it when removed below though
	     vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrCYAN,"[ClusterManager] ")
            << "Adding Plugin: " << new_plugin->getManagerName() <<std::endl << vprDEBUG_FLUSH;
      }
   }

   /**
    * Removes a plugin from the ClusterManager
    */
   void ClusterManager::removePlugin(ClusterPlugin* old_plugin)
   {
      vpr::Guard<vpr::Mutex> guard(mPluginsLock);
      
      for (std::list<ClusterPlugin*>::iterator i = mPlugins.begin();
           i != mPlugins.end() ; i++)
      {
         if ((*i) == old_plugin)
         {
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrCYAN,"[ClusterManager] ")
            << "Removing Plugin: " << old_plugin->getManagerName() <<std::endl << vprDEBUG_FLUSH;
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

   void ClusterManager::preDraw()
   {
      // Idea is to not create frame lock if we do not need to
      bool updateNeeded = false;
      vpr::Guard<vpr::Mutex> guard(mPluginsLock);
      
      for (std::list<ClusterPlugin*>::iterator i = mPlugins.begin();
           i != mPlugins.end() ; i++)
      {
         (*i)->preDraw();
         updateNeeded = true;
      }
      if (updateNeeded)
      {
         sendEndBlocksAndSignalUpdate();
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
      
      for (std::list<ClusterPlugin*>::iterator i = mPlugins.begin();
           i != mPlugins.end() ; i++)
      {
         (*i)->postPostFrame();      
         updateNeeded = true;
      }  
      if (updateNeeded)
      {
         sendEndBlocksAndSignalUpdate();
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

   void ClusterManager::sendEndBlocksAndSignalUpdate()
   {
      ClusterNetwork::instance()->lockClusterNodes();

      std::vector<cluster::ClusterNode*>::iterator begin_cluster_nodes = ClusterNetwork::instance()->getClusterNodesBegin();
      std::vector<cluster::ClusterNode*>::iterator end_cluster_nodes = ClusterNetwork::instance()->getClusterNodesEnd();
   
      cluster::EndBlock* temp_end_block = new EndBlock(0);
      

      for(std::vector<cluster::ClusterNode*>::iterator i=begin_cluster_nodes;i!=end_cluster_nodes;i++)
      {
         if ((*i)->isConnected())
         {
               // Send End Blocks to all connected ClusterNodes
            (*i)->send(temp_end_block);
               // Signal Update thread to read Network Packets
            (*i)->signalUpdate();
         }                         
      }
      for(std::vector<cluster::ClusterNode*>::iterator i=begin_cluster_nodes;i!=end_cluster_nodes;i++)
      {
         if ((*i)->isConnected())
         {
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
                  
         int num_plugins = chunk->getNum(std::string("clusterPlugin"));
         for (int i = 0 ; i < num_plugins ; i++)
         {
            std::string new_plugin = chunk->getProperty<std::string>(std::string("clusterPlugin"), i);
            std::cout << "New Plugin: " << new_plugin << std::endl;
            if (new_plugin == "RemoteInputManager")
            {
               RemoteInputManager::instance()->load();
            }
            else if (new_plugin == "ApplicationDataManager")
            {
               ApplicationDataManager::instance()->load();
            }
            else if (new_plugin == "SwapLockPlugin")
            {
               SwapLockPlugin::instance()->load();
            }
         }

///////////////////////////
         int num_nodes = chunk->getNum(std::string("cluster_nodes"));
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrCYAN,"[XXX] ")
            << "configAdd() Number of nodes: " << num_nodes
            << "\n" << vprDEBUG_FLUSH;         
         for (int i = 0 ; i < num_nodes ; i++)
         {
            std::string new_node = chunk->getProperty<std::string>(std::string("cluster_nodes"), i);            
                  vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrCYAN,"[XXX] ")
                     << "configAdd() New Node Name: " << new_node
                     << "\n" << vprDEBUG_FLUSH;                     
            jccl::ConfigChunkPtr new_node_chunk = getConfigChunkPointer(new_node);
            std::string new_node_hostname = new_node_chunk->getProperty<std::string>(std::string("host_name"));
                  vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrCYAN,"[XXX] ")
                     << "configAdd() New Node Hostname: " << new_node_hostname
                     << "\n" << vprDEBUG_FLUSH;         
            if (new_node_hostname != ClusterNetwork::instance()->getLocalHostname())
            {
                     vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrCYAN,"[XXX] ")
                        << "configAdd() Added Node since it is non-local\n" << vprDEBUG_FLUSH;         
               mPendingSlaves.push_back(new_node_hostname);
               mSlaves.push_back(new_node_hostname);
            }
         }
////////////////////////////
         
         /////////////////////////////////////////
         //  Starting Barrier Stuff
         //
         // -Set flag we have started configuring the cluster
         // -Get Sync Machine Chunk Name
         // -Get ChunkPtr to this chunk
         // -Get the Hostname of this node
         
         std::string barrier_machine_chunk_name = chunk->getProperty<std::string>(std::string("barrier_master"));
         jccl::ConfigChunkPtr barrier_machine_chunk = getConfigChunkPointer(barrier_machine_chunk_name);
         vprASSERT(NULL != barrier_machine_chunk.get() && "ConfigManager Chunk MUST have a barrier_master.");
         mBarrierMasterHostname = barrier_machine_chunk->getProperty<std::string>(std::string("host_name"));

         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrCYAN,"[XXX] ")
            << "Barrier Machine Chunk Name is: " << barrier_machine_chunk_name << std::endl << vprDEBUG_FLUSH;         
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrCYAN,"[XXX] ")
            << "Barrier Machine Hostname is: " << mBarrierMasterHostname << std::endl << vprDEBUG_FLUSH;         
         // Starting Barrier Stuff
         /////////////////////////////////////         
         
         if (mBarrierMasterHostname == ClusterNetwork::instance()->getLocalHostname())
         {
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrCYAN,"[XXX] ")
               << "ClusterManager::ConfigAdd() Barrier Master!\n" << vprDEBUG_FLUSH;         
            mBarrierMaster = true;
         }
         else
         {
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrCYAN,"[XXX] ")
               << "ClusterManager::ConfigAdd() Barrier Slave!\n" << vprDEBUG_FLUSH;         
            mBarrierMaster = false;
         }
         
         return(true);
      }
	  else
	  {
	     vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) 
            << clrOutBOLD(clrRED,"[ClusterManager::ConfigAdd] ERROR: ")
            << "Something is seriously wrong, we should not be handling this chunk\n"
			<< vprDEBUG_FLUSH;
         return(true);
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


   //////////////////////////////////////////////
   // STARTING BARRIER

   bool ClusterManager::isClusterReady()
   {
      // THIS CASE ASSUMES YOU WILL NEVR CALL
      // THIS FUNCTION IF YOU ARE NOT IN A CLUSTER
      
      //We start all nodes thinking they are masters
      //This makes us not actually do anything bellow
      //Until configuration occurs

      // -If NOT Running
      //   -If Slave
      //     -Find BarrierMaster ClusterNode
      //     -Send Barrier Signal
      //   -Else
      //     -Nothing

      if (!mRunning)
      {
         if (!mBarrierMaster)
         {
            ClusterNode* barrier_master = ClusterNetwork::instance()->getClusterNodeByHostname(mBarrierMasterHostname);
            if (NULL == barrier_master)
            {
               vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) 
                  << clrOutBOLD(clrRED,"[Start Barrier] isClusterReady() Barrier machine configuration not yet loaded.")
                  << std::endl << vprDEBUG_FLUSH;
            }
            else if (barrier_master->isConnected())
            {
               //Send packet to server machine
               StartBlock temp_start_block(0);
               temp_start_block.send(barrier_master->getSockStream());
               vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) << clrOutBOLD(clrCYAN,"[Start Barrier] ")
                  << "isClusterReady() Sending Barrier Signal to master: " << mBarrierMasterHostname << std::endl << vprDEBUG_FLUSH;
            }
            else
            {
               //If we are not connected and we are not in pending list, add to the pending list
               if (NULL == ClusterNetwork::instance()->getPendingNode(barrier_master->getHostname()))
               {
                  ClusterNetwork::instance()->addPendingNode(barrier_master);
               }

            }
         }
         else
         {
            vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) << clrOutBOLD(clrCYAN,"[XXX] ")
               << "isClusterReady() Barrier Master waiting...\n" << vprDEBUG_FLUSH;         
            
            int num_pending_nodes = getPendingBarrierSlaves().size();
            if (0 == num_pending_nodes)
            {
               StartBlock temp_start_block(0);

               //Send responce to all nodes
               for (std::vector<std::string>::iterator i = mSlaves.begin();
                    i != mSlaves.end() ; i++)
               {
                  // Dead lock since we are actually in a recursion of ClusterNodes
                  ClusterNode* node = ClusterNetwork::instance()->getClusterNodeByHostname(*i);
                  temp_start_block.send(node->getSockStream());
                  vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) << clrOutBOLD(clrCYAN,"[Start Barrier] ")
                     << "Sending Barrier Signal to slave: " << (*i) << std::endl << vprDEBUG_FLUSH;              
               }

               //Set running true
               ClusterManager::instance()->setRunning(true);
            }
         }
         // We will delay the server until the next call
         // to isClusterReady to better sync it with the 
         // slaves since they still have to read the 
         // packet of the network
         return false;
      }
      return true;
   }

   // STARTING BARRIER
   //////////////////////////////////////////////
   
   
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

} // End of gadget namespace
