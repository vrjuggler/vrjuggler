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

#include <gadget/Util/Debug.h>
#include <cluster/ClusterDepChecker.h>
#include <cluster/ClusterManager.h>
#include <cluster/ClusterNetwork/ClusterNetwork.h>
#include <cluster/ClusterNetwork/ClusterNode.h>
#include <gadget/Type/DeviceFactory.h>
#include <jccl/RTRC/ConfigManager.h>

#include <boost/concept_check.hpp>

namespace cluster
{

bool ClusterDepChecker::depSatisfied(jccl::ConfigChunkPtr chunk)
{
   if (chunk->getDescToken() == ClusterNetwork::getMachineSpecificChunkType())
   {
      // Machine Specific Chunks should have no dependencies since we are simply inserting
      // the embedded chunks into the pending list. This is to fix errors like the embedded
      // keyboard window in a DisplayWindow would always create a dependancy loop
      debugOutDependencies(chunk,vprDBG_WARNING_LVL);
      return true;
   }
   else if (cluster::ClusterManager::instance()->recognizeRemoteDeviceConfig(chunk))
   {
      // Remote devices should have no dependencies since we are not actually configuring anything,
      // we are only creating a data structure that we can determine without any other chunks
      // Virtual devices should not have any dependencies

      // RemoteDeviceConfig has only two dependencies
      //   - deivceHost exists in Active List
      //   - ClusterNode exists and is connected
      //   - Remote Device needs to be configured

      bool pass = true;

      jccl::ConfigManager* cfg_mgr = jccl::ConfigManager::instance();
      cluster::ClusterNetwork* cluster_net = cluster::ClusterNetwork::instance();

      // deviceHost exists in active configuration
      std::string device_host = chunk->getProperty<std::string>("deviceHost");
      ClusterNode* node = cluster_net->getClusterNodeByName(device_host);

      if(!cfg_mgr->isChunkInActiveList(device_host))
      {
         pass = false;
         vprDEBUG(gadgetDBG_RIM,vprDBG_WARNING_LVL) << "[ClusterDepChecker] Host node's ConfigChunk("
            << device_host << ") is not in the Active List yet.\n" << vprDEBUG_FLUSH;
      }
      else if(node == NULL)
      {
         pass = false;
         vprDEBUG(gadgetDBG_RIM,vprDBG_WARNING_LVL) << "[ClusterDepChecker] ClusterNode("
               << device_host << ") is NULL, so it is not in the Active List yet.\n" << vprDEBUG_FLUSH;
      }
      else if(ClusterNode::DISCONNECTED == node->getConnected())
      {
         pass = false;
         vprDEBUG(gadgetDBG_RIM,vprDBG_WARNING_LVL) << "[ClusterDepChecker] Adding Pending Node??" << vprDEBUG_FLUSH;
         ClusterNetwork::instance()->addPendingNode(node);

         vprDEBUG(gadgetDBG_RIM,vprDBG_WARNING_LVL) << "[ClusterDepChecker] ClusterNode("
            << device_host << ") not connected yet.\n" << vprDEBUG_FLUSH;
      }
      //Not needed, we kind of just did it above
      // debugOutDependencies(chunk,vprDBG_WARNING_LVL);

      return pass;
   }
   /*else if (cluster::ClusterManager::instance()->recognizeClusterManagerConfig(chunk))
   {
      // We need the following for the Cluster to be setup correctly
      // - All Nodes connected(All listed machines connected)
      // - All Devices configured

      // HERE
      // - No MachineSpecific Chunks Pending


      int number_nodes = chunk->getNum("cluster_nodes");
      for (int i = 0 ; i < number_nodes ; i++)
      {
         std::string node_name = chunk->getProperty<std::string>("cluster_nodes",i);
         jccl::ConfigChunkPtr node_chunk = ClusterManager::instance()->getConfigChunkPointer(node_name);
         if (node_chunk == NULL)
         {
            // Node not in current configuration
            jccl::ConfigManager::instance()->delayStalePendingList();
            std::cout << "L" << std::flush;
            return(false);
         }
         std::string host_name = node_chunk->getProperty<std::string>("host_name");
         if (host_name != ClusterNetwork::instance()->getLocalHostname())
         {
            ClusterNode* node = ClusterNetwork::instance()->getClusterNodeByName(node_name);
            if (node == NULL)
            {
               jccl::ConfigManager::instance()->delayStalePendingList();
               std::cout << "N" << std::flush;
               return(false);
            }
            else if (!node->isConnected())
            {
               jccl::ConfigManager::instance()->delayStalePendingList();
               std::cout << "D" << std::flush;
               return(false);
            }
         }
      }
      return(true);
   }*/
   else
   {
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "ERROR, Something is seriously wrong, we should never get here\n"
      << vprDEBUG_FLUSH;
      return(true);
   }
}


// We need to handle Machine Specific Chunks because in reality they have no dependencies
bool ClusterDepChecker::canHandle(jccl::ConfigChunkPtr chunk)
{
   return (chunk->getDescToken() == ClusterNetwork::getMachineSpecificChunkType() ||
           cluster::ClusterManager::instance()->recognizeRemoteDeviceConfig(chunk) /*||
           cluster::ClusterManager::instance()->recognizeClusterManagerConfig(chunk)*/ );
}

void ClusterDepChecker::debugOutDependencies(jccl::ConfigChunkPtr chunk,int dbg_lvl)
{
   boost::ignore_unused_variable_warning(dbg_lvl);

   if (chunk->getDescToken() == ClusterNetwork::getMachineSpecificChunkType())
   {
      // Machine Specific Chunks should have no dependencies since we are simply inserting
      // the embedded chunks into the pending list
      //vprDEBUG(vprDBG_ALL,dbg_lvl) << "MachineSpecific Chunks have NO Deps!!!\n" << vprDEBUG_FLUSH;
   }
   else if (cluster::ClusterManager::instance()->recognizeRemoteDeviceConfig(chunk))
   {
      // Remote devices should have no dependencies since we are not actually configuring anything,
      // we are only creating a data structure that we can determine without any other chunks
      //vprDEBUG(vprDBG_ALL,dbg_lvl) << "Virtual Device: " << chunk->getName()
      //   << " has NO Deps since it is Virtual!!!\n" << vprDEBUG_FLUSH;
   }
   /*else if (cluster::ClusterManager::instance()->recognizeClusterManagerConfig(chunk))
   {
      //int number_nodes = chunk->getNum("cluster_nodes");
      //for (int i = 0 ; i < number_nodes ; i++)
      //{
      //   std::string node_name = chunk->getProperty<std::string>("cluster_nodes",i);
      //   jccl::ConfigChunkPtr node_chunk = RmoteInputManager::instance()->getConfigChunkPointer(node_name);
      //   if (node_chunk == NULL)
      //   {
      //      vprDEBUG(vprDBG_ALL, dbg_lvl) << node_name << " Node not in current configuration."
      //         << std::endl << vprDEBUG_FLUSH;
      //      return;
      //   }
      //   std::string host_name = node_chunk->getProperty<std::string>("host_name");
      //   if (host_name != ClusterNetwork::instance()->getLocalHostname())
      //   {
      //      ClusterNode* node = ClusterNetwork::instance()->getClusterNodeByName(node_name);
      //      if (node == NULL)
      //      {
      //         vprDEBUG(vprDBG_ALL, dbg_lvl) << node_name << " does not exist in ClusterNetwork."
      //            << std::endl << vprDEBUG_FLUSH;
      //         return;
      //      }
      //      else if (!node->isConnected())
      //      {
      //         node->debugDump(dbg_lvl);
      //         return;
      //      }
      //   }
      //}
   }*/
   else
   {
      //vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "ERROR, Something is seriously wrong, we should never get here\n"
      //   << vprDEBUG_FLUSH;
   }

}

}  // end namespace gadget
