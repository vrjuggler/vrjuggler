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

#include <boost/concept_check.hpp>

#include <cluster/Plugins/StartBarrierPlugin/StartBarrierPlugin.h> // my header...

#include <gadget/Util/Debug.h>
#include <cluster/ClusterDepChecker.h>

#include <cluster/ClusterManager.h>
#include <cluster/ClusterNetwork/ClusterNetwork.h>
#include <cluster/ClusterNetwork/ClusterNode.h>

#include <cluster/Packets/StartBlock.h>

cluster::ClusterPlugin* initPlugin()
{
   return cluster::StartBarrierPlugin::instance();
}

namespace cluster
{
   vprSingletonImp( StartBarrierPlugin );
   
   StartBarrierPlugin::StartBarrierPlugin()
      : mBarrierMaster(false)
      , mComplete(false)
      , mSlaveWaitingOnPlugins(false)
      , mSlowDownMaster(0)
      , mPluginGUID("566a50ff-5e73-43e0-a9a9-0fb62b76731a")
   {;}

   StartBarrierPlugin::~StartBarrierPlugin()
   {
   
   }

   void StartBarrierPlugin::handlePacket(Packet* packet, ClusterNode* node)
   {
      // On Barrier Recv
      // -If Master
      //   -Remove Pending slave
      //   -If all recved
      //     -Send responce to all nodes
      //     -Set Running TRUE
      // -Else
      //   -Set Running TRUE
      if ( NULL != packet && NULL != node )
      {

         if(packet->getPacketType() == cluster::Header::RIM_START_BLOCK)
         {
            //We are not actually using any data in this packet for now.
            
            //StartBlock* temp_start_block = dynamic_cast<StartBlock*>(packet);
            //vprASSERT(NULL != temp_start_block && "Dynamic cast failed!");
            if (isBarrierMaster())
            {
               removePendingBarrierSlave(node->getHostname());
               vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) << clrOutBOLD(clrCYAN,"[StartBarrierPlugin] ")
                  << "handlePacket() Master has received a START signal.\n" << vprDEBUG_FLUSH;         
            }
            else
            {
               vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) << clrOutBOLD(clrCYAN,"[StartBarrierPlugin] ")
                  << "handlePacket() Slave has finished start barrier\n" << vprDEBUG_FLUSH;         
               mComplete = true;
               ClusterManager::instance()->setClusterReady(true);
            }      
         }
         else
         {
            vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) << clrOutBOLD(clrRED,"StartBarrierPlugin::handlePacket() ERROR: ")
               << "We do not handle this type of packet.\n" << vprDEBUG_FLUSH;         
         }
      }
   }
   
   /** Add the pending chunk to the configuration.
   *  PRE: configCanHandle (chunk) == true.
   *  @return true iff chunk was successfully added to configuration.
   */
   bool StartBarrierPlugin::configAdd(jccl::ConfigChunkPtr chunk)
   {
      // -If the cluster manager has been configured.

      if(!ClusterManager::instance()->isClusterActive())
      {
         // XXX: This could be made into a dependancy also.
         return false;
      }

      mPendingSlaves = ClusterManager::instance()->getClusterNodes();
      mSlaves = ClusterManager::instance()->getClusterNodes();


      /////////////////////////////////////////
      //  Starting Barrier Stuff
      //
      // -Set flag we have started configuring the cluster
      // -Get Sync Machine Chunk Name
      // -Get ChunkPtr to this chunk
      // -Get the Hostname of this node

      std::string barrier_machine_chunk_name = chunk->getProperty<std::string>(std::string("start_master"));
      jccl::ConfigChunkPtr barrier_machine_chunk = ClusterManager::instance()->getConfigChunkPointer(barrier_machine_chunk_name);
      vprASSERT(NULL != barrier_machine_chunk.get() && "ConfigManager Chunk MUST have a barrier_master.");
      mBarrierMasterHostname = barrier_machine_chunk->getProperty<std::string>(std::string("host_name"));

      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrCYAN,"[StartBarrierPlugin] ")
         << "Start Master Chunk Name is: " << barrier_machine_chunk_name << std::endl << vprDEBUG_FLUSH;         
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrCYAN,"[StartBarrierPlugin] ")
         << "Start Master Hostname is: " << mBarrierMasterHostname << std::endl << vprDEBUG_FLUSH;         
      // Starting Barrier Stuff
      /////////////////////////////////////         

      if (mBarrierMasterHostname == ClusterNetwork::instance()->getLocalHostname())
      {
         mBarrierMaster = true;
      }
      else
      {
         mBarrierMaster = false;
      }
      
      return true;
   }
   
   
   /** Remove the pending chunk from the current configuration.
   *  PRE: configCanHandle (chunk) == true.
   *  @return true iff the chunk (and any objects it represented)
   *          were successfully removed.
   */
   bool StartBarrierPlugin::configRemove(jccl::ConfigChunkPtr chunk)
   {
      boost::ignore_unused_variable_warning(chunk);
      return false;
   }
   
   /** Checks if this handler can process chunk.
   *  Typically, an implementation of handler will check the chunk's
   *  description name/token to decide if it knows how to deal with
   *  it.
   *  @return true iff this handler can process chunk.
   */
   bool StartBarrierPlugin::configCanHandle(jccl::ConfigChunkPtr chunk)
   {
      return( recognizeStartBarrierPluginConfig(chunk) );   
   }
      
   bool StartBarrierPlugin::recognizeStartBarrierPluginConfig(jccl::ConfigChunkPtr chunk)
   {
      return(chunk->getDescToken() == getChunkType());
   }

   void StartBarrierPlugin::preDraw()
   {;
   }
   void StartBarrierPlugin::postPostFrame()
   {
      // -If we are not complete
      //   -If all other plugins are ready
      //     -If Slave
      //       -Find the barrier master
      //       -If connected
      //         -Send a start block
      //       -Else
      //         -Add barrier node to pending list
      // -Else
      //   -If number of pending start nodes is 0
      //     -Send a start block to all of them
      
      //This is where all the real work gets done
      if (!mComplete)
      {
         if (ClusterManager::instance()->pluginsReady())
         {
            if (!mBarrierMaster)
            {
               ClusterNode* barrier_master = ClusterNetwork::instance()->getClusterNodeByHostname(mBarrierMasterHostname);
               if (NULL == barrier_master)
               {
                  vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) 
                     << clrOutBOLD(clrRED,"[StartBarrierPlugin] Barrier machine configuration chunk not yet loaded.")
                     << std::endl << vprDEBUG_FLUSH;
               }
               else if (barrier_master->isConnected())
               {
                  //Send packet to server machine
                  StartBlock temp_start_block(getPluginGUID(), 0);
                  barrier_master->send(&temp_start_block);
                  vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) << clrOutBOLD(clrCYAN,"[StartBarrierPlugin] ")
                     << "Sending signal to start master: " << mBarrierMasterHostname << std::endl << vprDEBUG_FLUSH;
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
               vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) << clrOutBOLD(clrCYAN,"[StartBarrierPlugin] ")
                  << "Barrier Master waiting...\n" << vprDEBUG_FLUSH;         
               int num_pending_nodes = getPendingBarrierSlaves().size();
               if (0 == num_pending_nodes)
               {
                  mComplete = true;
                  ClusterManager::instance()->setClusterReady(true);
                  std::cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX DONE - list=0 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << std::endl;
                  
                  StartBlock temp_start_block(getPluginGUID(), 0);
   
                  //Send responce to all nodes
                  for (std::vector<std::string>::iterator i = mSlaves.begin();
                       i != mSlaves.end() ; i++)
                  {
                     // Dead lock since we are actually in a recursion of ClusterNodes
                     ClusterNode* node = ClusterNetwork::instance()->getClusterNodeByHostname(*i);
                     node->send(&temp_start_block);
                     vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) << clrOutBOLD(clrCYAN,"[StartBarrierPlugin] ")
                        << "Sending start signal to slave: " << (*i) << std::endl << vprDEBUG_FLUSH;              
                  }
               }//End (0==num_pending_nodes)
            }//End (mBarrierMaster)
         }//End (Plugins Ready)
      }//End (!mComplete)
   }

   bool StartBarrierPlugin::isPluginReady()
   {
      return true;
   }
} // End of gadget namespace
