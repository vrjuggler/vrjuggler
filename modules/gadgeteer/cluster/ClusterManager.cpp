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

// UserData
#include <cluster/SerializableData.h>
#include <cluster/Plugins/ApplicationDataManager/UserDataServer.h>
#include <cluster/Packets/UserDataRequest.h>


namespace cluster
{
   vprSingletonImp( ClusterManager );

   ClusterManager::ClusterManager()
   {
      jccl::ConfigManager::instance()->addConfigChunkHandler(ClusterNetwork::instance());
      jccl::DependencyManager::instance()->registerChecker(new ClusterDepChecker());
   }
   ClusterManager::~ClusterManager()
   {
      ;
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
         }
         
         return(true);
      }
	  else
	  {
	     vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) 
            << clrOutBOLD(clrRED,"[ClusterManager::ConfigAdd] ERROR, Something is seriously wrong, we should never get here\n")
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
		return( recognizeClusterManagerConfig(chunk) );

	}
    
	bool ClusterManager::recognizeClusterManagerConfig(jccl::ConfigChunkPtr chunk)
	{
      /*if (chunk->getDescToken() == ClusterManager::getChunkType())
      {
         std::string sync_machine = chunk->getProperty<std::string>("sync_machine");
         std::cout << "RIMChunk: " << chunk->getName() << std::endl;
         std::cout << "Sync Machine: " << sync_machine << std::endl;

         jccl::ConfigChunkPtr device_host_ptr = chunk->getProperty<jccl::ConfigChunkPtr>("sync_machine",0);
         std::cout << "SYNC MACHINE !!!!!!!!!!!!!!!!!!!!! " << device_host_ptr->getName() << std::endl;
      } */
   	  return(chunk->getDescToken() == ClusterManager::getChunkType());
	}

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
         std::cout << "[ClusterManager] Adding a Manager!" << std::endl;
      }
   }
   void ClusterManager::removePlugin(ClusterPlugin* old_manager)
   {
      vpr::Guard<vpr::Mutex> guard(mPluginsLock);
      
      for (std::list<ClusterPlugin*>::iterator i = mPlugins.begin();
           i != mPlugins.end() ; i++)
      {
         if ((*i) == old_manager)
         {
            mPlugins.erase(i);
            jccl::ConfigManager::instance()->removeConfigChunkHandler(*i);
            std::cout << "[ClusterManager] Removing a Manager!" << std::endl;
            return;
         }
      }
   }
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

   void ClusterManager::preDraw()
   {
      bool updateNeeded = false;
      vpr::Guard<vpr::Mutex> guard(mPluginsLock);
      
      for (std::list<ClusterPlugin*>::iterator i = mPlugins.begin();
           i != mPlugins.end() ; i++)
      {
         if ((*i)->isActive())
         {
            (*i)->preDraw();
            updateNeeded = true;
         }                  
      }
//      if (updateNeeded)
//      {
         sendEndBlocksAndSignalUpdate();
//      }                                 
   }
   void ClusterManager::postPostFrame()
   {
      bool updateNeeded = false;
      vpr::Guard<vpr::Mutex> guard(mPluginsLock);
      
      for (std::list<ClusterPlugin*>::iterator i = mPlugins.begin();
           i != mPlugins.end() ; i++)
      {
         if ((*i)->isActive())
         {
            (*i)->postPostFrame();      
            updateNeeded = true;
         }
      }  
//      if (updateNeeded)
//      {
         sendEndBlocksAndSignalUpdate();
//      }                                 
   }
} // End of gadget namespace
