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

#include <cluster/Plugins/SwapLockPlugin/SwapLockPlugin.h> // my header...
#include <cluster/Plugins/SwapLockPlugin/ClusterBarrier.h>

#include <gadget/Util/Debug.h>
#include <cluster/ClusterDepChecker.h>

#include <cluster/ClusterManager.h>
#include <cluster/ClusterNetwork/ClusterNetwork.h>
#include <cluster/ClusterNetwork/ClusterNode.h>

cluster::ClusterPlugin* initPlugin()
{
   return cluster::SwapLockPlugin::instance();
}

namespace cluster
{
   vprSingletonImp( SwapLockPlugin );
   
   SwapLockPlugin::SwapLockPlugin() : mPluginGUID("5edfc033-1b3e-4741-b0e0-6ebb47967644"),
      SYNC_SIGNAL('G'), read_timeout(5,vpr::Interval::Sec)
   {
      // This is done by the ClusterManager
      //jccl::DependencyManager::instance()->registerChecker(new ClusterDepChecker());
      mFrameNumber = 0;
   }
   SwapLockPlugin::~SwapLockPlugin()
   {;}
   
   /** Add the pending chunk to the configuration.
   *  PRE: configCanHandle (chunk) == true.
   *  @return true iff chunk was successfully added to configuration.
   */
   bool SwapLockPlugin::configAdd(jccl::ConfigChunkPtr chunk)
   {
      // -If the cluster manager has been configured.

      if(!ClusterManager::instance()->isClusterActive())
      {
         // XXX: This could be made into a dependancy also.
         return false;
      }

      /////////////////////////////////////////
      //  SwapLock Barrier Stuff
      //
      // -Set flag we have started configuring the cluster
      // -Get Sync Machine Chunk Name
      // -Get ChunkPtr to this chunk
      // -Get the Hostname of this node

      std::string barrier_machine_chunk_name = chunk->getProperty<std::string>(std::string("sync_server"));
      jccl::ConfigChunkPtr barrier_machine_chunk = ClusterManager::instance()->getConfigChunkPointer(barrier_machine_chunk_name);
      vprASSERT(NULL != barrier_machine_chunk.get() && "ConfigManager Chunk MUST have a barrier_master.");
      mBarrierMasterHostname = barrier_machine_chunk->getProperty<std::string>(std::string("host_name"));

      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrCYAN,"[StartBarrierPlugin] ")
         << "SwapLock Master Chunk Name is: " << barrier_machine_chunk_name << std::endl << vprDEBUG_FLUSH;         
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrCYAN,"[StartBarrierPlugin] ")
         << "SwapLock Master Hostname is: " << mBarrierMasterHostname << std::endl << vprDEBUG_FLUSH;         
      // Starting Barrier Stuff
      /////////////////////////////////////         
      
      if (mBarrierMasterHostname == ClusterNetwork::instance()->getLocalHostname())
      {
         mBarrierMaster = true;
         mSlaves = ClusterManager::instance()->getClusterNodes();     
         for (std::vector<std::string>::iterator i = mSlaves.begin() ; i != mSlaves.end() ; i++)
         {
            ClusterNode* temp_node = ClusterNetwork::instance()->getClusterNodeByHostname(*i);
            if (NULL != temp_node)
            {
               mSlaveNodes.push_back(temp_node);
            }
         }         
      }
      else
      {
         mBarrierMaster = false;
         mMasterNode = ClusterNetwork::instance()->getClusterNodeByHostname(mBarrierMasterHostname);
      }

      return true;
   }
   
   
   /** Remove the pending chunk from the current configuration.
   *  PRE: configCanHandle (chunk) == true.
   *  @return true iff the chunk (and any objects it represented)
   *          were successfully removed.
   */
   bool SwapLockPlugin::configRemove(jccl::ConfigChunkPtr chunk)
   {
      if (recognizeSwapLockPluginConfig(chunk))
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "[SwapLockPlugin] Disabling SwapLock: " << chunk->getName() 
         << "\n" << vprDEBUG_FLUSH;
         return(true);
      }
      else
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "[SwapLockPlugin::configRemove] ERROR, Something is seriously wrong, we should never get here\n"
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
   bool SwapLockPlugin::configCanHandle(jccl::ConfigChunkPtr chunk)
   {
      return( recognizeSwapLockPluginConfig(chunk) );   
   }
      
   bool SwapLockPlugin::recognizeSwapLockPluginConfig(jccl::ConfigChunkPtr chunk)
   {
      return(chunk->getDescToken() == getChunkType());
   }

   void SwapLockPlugin::preDraw()
   {;
   }
   void SwapLockPlugin::postPostFrame()
   {;
   }
   
   
   bool SwapLockPlugin::isPluginReady()
   {
      // We could do some sort of signal here I guess?
      return (true);
   }
   
   
   bool SwapLockPlugin::createBarrier()
   {
      // If Slave
      // - send ready to Master
      // - read ready from Server
      // Else if Master
      // - for each slave
      //   - read ready
      // - for each slave
      //   - send ready

      //vpr::Interval first_time, second_time;
      //first_time.setNow();

      if (mBarrierMaster)
      {
         MasterReceive();
         MasterSend();
      }
      else
      {
         SlaveSend();
         SlaveReceive();
      }
      //second_time.setNow();
      //vpr::Interval diff_time(second_time-first_time);
      //vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) << clrSetBOLD(clrCYAN) << "Latency: " 
      //   << diff_time.getBaseVal() << " usecs\n"<< clrRESET << vprDEBUG_FLUSH;

      return(true);
   }

   /////////////// Actual Communication  //////////////////////////

   void SwapLockPlugin::MasterSend()
   {
      //vprASSERT(mSyncClients!=NULL && "Sync Clients Vector is NULL!");
      //vprASSERT(mSwapLockActive==true && "Barrier is not active!");
      
      vpr::Uint32 bytes_read;

      for (std::vector<ClusterNode*>::iterator i = mSlaveNodes.begin();
        i != mSlaveNodes.end();i++)
      {
         (*i)->getSockStream()->send(&SYNC_SIGNAL , 1, bytes_read, vpr::Interval::NoWait);
      }

   }
   void SwapLockPlugin::MasterReceive()
   {
      //vprASSERT(mSwapLockActive==true && "Barrier is not active!");
      
      vpr::Uint32 bytes_read;
      vpr::Uint8 temp;
      for (std::vector<ClusterNode*>::iterator i = mSlaveNodes.begin();
        i != mSlaveNodes.end();i++)
      {
         if((*i)->getSockStream()->recv(&temp , 1, bytes_read, read_timeout) == vpr::ReturnStatus::Timeout)
         {
            static int numTimeouts = 0;
            numTimeouts++;
            vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) << "ClusterBarrierTCP: Received a timeout from a cluster node, it was removed" 
               << std::endl << vprDEBUG_FLUSH;
            if (numTimeouts > 5)
            {
               mSlaveNodes.erase(i);
               vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) << "ClusterBarrierTCP: Received too many timeouts from a cluster node,"
                  <<" so it was removed from the list of machines to syncronize with." 
                  << std::endl << vprDEBUG_FLUSH;
               vpr::System::sleep(1);
            }
         }
         //vprASSERT(1==bytes_read && "ClusterBarrierTCP: Master Barrier received timeout");
      } 
   }
   void SwapLockPlugin::SlaveSend()
   {
      vprASSERT(mMasterNode!=NULL && "mSyncServer is NULL!");
      //vprASSERT(mSwapLockActive==true && "Barrier is not active!");
      
      vpr::Uint32 bytes_read;
      mMasterNode->getSockStream()->send(&SYNC_SIGNAL , 1, bytes_read, vpr::Interval::NoWait);
   }
   void SwapLockPlugin::SlaveReceive()
   {
      vprASSERT(mMasterNode!=NULL && "mSyncServer is NULL!");
      //vprASSERT(mSwapLockActive==true && "Barrier is not active!");

      vpr::Uint32 bytes_read;
      vpr::Uint8 temp;
      if (!mMasterNode->getSockStream()->recv(&temp , 1, bytes_read, read_timeout).success())
      {
         std::cout << "[ClusterBarrierTCP] SYNC ERROR" << std::endl;
         vpr::System::sleep(1);
      }
      //vprASSERT(1==bytes_read && "ClusterBarrierTCP: Slave Barrier received timeout");
   }


} // End of gadget namespace
