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

#include <cluster/Plugins/SwapLockWiredPlugin/SwapLockWiredPlugin.h> // my header...

#include <gadget/Util/Debug.h>
#include <cluster/ClusterDepChecker.h>

#include <cluster/ClusterManager.h>
#include <cluster/ClusterNetwork/ClusterNetwork.h>
#include <cluster/ClusterNetwork/ClusterNode.h>

//#include <cluster/Plugins/SwapLockPlugin/ClusterBarrier.h>
//#include <cluster/ClusterNetwork/ClusterNetwork.h>
#include <cluster/Packets/PacketFactory.h>
#include <cluster/Packets/SyncRequest.h>
#include <cluster/Packets/SyncAck.h>

#include <vpr/Thread/Thread.h>

#include <vpr/Util/ReturnStatus.h>
#include <vpr/IO/Socket/SocketStream.h>
#include <vpr/IO/Port/SerialPort.h>

#include <boost/concept_check.hpp>


cluster::ClusterPlugin* initPlugin()
{
   return cluster::SwapLockWiredPlugin::instance();
}

namespace cluster
{
   vprSingletonImp( SwapLockWiredPlugin );

   /** Add the pending chunk to the configuration.
   *  PRE: configCanHandle (chunk) == true.
   *  @return true iff chunk was successfully added to configuration.
   */
   bool SwapLockWiredPlugin::configAdd(jccl::ConfigChunkPtr chunk)
   {
      // We need to make sure that the ClusterManager has been configured so that we
      // can ensure that the barrier master's config chunk has also been configured.
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
      
      //mTCPport = chunk->getProperty<int>(std::string("listen_port"));

      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrCYAN,"[SwapLockWiredPlugin] ")
         << "SwapLock Master Chunk Name is: " << barrier_machine_chunk_name << std::endl << vprDEBUG_FLUSH;         
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrCYAN,"[SwapLockWiredPlugin] ")
         << "SwapLock Master Hostname is: " << mBarrierMasterHostname << std::endl << vprDEBUG_FLUSH;         
      // Starting Barrier Stuff
      /////////////////////////////////////         
      
      if (mBarrierMasterHostname == ClusterNetwork::instance()->getLocalHostname())
      {
         mIsMaster = true;
         mActive = true;
      }
      else
      {
         mIsMaster = false;
         mActive = true;
      }
      Init();
      return true;
   }
   
   /** Remove the pending chunk from the current configuration.
   *  PRE: configCanHandle (chunk) == true.
   *  @return true iff the chunk (and any objects it represented)
   *          were successfully removed.
   */
   bool SwapLockWiredPlugin::configRemove(jccl::ConfigChunkPtr chunk)
   {
      if (recognizeSwapLockWiredPluginConfig(chunk))
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "[SwapLockWiredPlugin] Disabling SwapLock: " << chunk->getName() 
         << "\n" << vprDEBUG_FLUSH;
         return(true);
      }
      else
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "[SwapLockWiredPlugin::configRemove] ERROR, Something is seriously wrong, we should never get here\n"
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
   bool SwapLockWiredPlugin::configCanHandle(jccl::ConfigChunkPtr chunk)
   {
      return( recognizeSwapLockWiredPluginConfig(chunk) );   
   }
      
   bool SwapLockWiredPlugin::recognizeSwapLockWiredPluginConfig(jccl::ConfigChunkPtr chunk)
   {
      return(chunk->getDescToken() == getChunkType());
   }

   void SwapLockWiredPlugin::preDraw()
   {;
   }
   void SwapLockWiredPlugin::postPostFrame()
   {;
   }

   void SwapLockWiredPlugin::handlePacket(Packet* packet, ClusterNode* node)
   {
      boost::ignore_unused_variable_warning(packet);
      boost::ignore_unused_variable_warning(node);
   }     

   bool SwapLockWiredPlugin::isPluginReady()
   {
      // We could do some sort of signal here I guess?
      return (true);
   }
   bool SwapLockWiredPlugin::createBarrier()
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

      if (mIsMaster)
      {
         masterReceive();
         masterSend();
      }
      else
      {
         slaveSend();
         slaveReceive();
      }

      //second_time.setNow();
      //vpr::Interval diff_time(second_time-first_time);
      //vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) << clrSetBOLD(clrCYAN) << "Latency: " 
      //   << diff_time.getBaseVal() << " usecs\n"<< clrRESET << vprDEBUG_FLUSH;

      return(true);
   }

   vpr::ReturnStatus SwapLockWiredPlugin::Init()
   {
	   vpr::ReturnStatus status;
	   status = ConnectToWiredParallel(); 
	   while(status != vpr::ReturnStatus::Succeed)
	   {
	      status = ConnectToWiredParallel();
	      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "SwapLockWiredPlugin::ConnectToWiredParallel() Barrier could not be initialized, FileHandle = " 
		      << mWire << "\n" << vprDEBUG_FLUSH;
	      
	   }
	   mActive = true;
	   vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "SwapLockWiredPlugin::ConnectToWiredParallel() Barrier has been Init() is success!" << vprDEBUG_FLUSH;
	   return(status); 
   }

   vpr::ReturnStatus SwapLockWiredPlugin::ConnectToWiredParallel()
   {
      mWire = open("/dev/wiredparallel",O_RDWR);      
      if (mWire != -1)
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) 
		 << clrOutBOLD(clrGREEN,"SwapLockWiredPlugin::ConnectToWiredParallel() Successfully connected to /dev/wiredParallel\n") 
		 << vprDEBUG_FLUSH;
      }
      else
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) 
		 << clrOutBOLD(clrRED,"SwapLockWiredPlugin::ConnectToWiredParallel() ERROR: Could not connect to /dev/wiredparallel\n") 
		 << vprDEBUG_FLUSH;
         return(vpr::ReturnStatus::Fail);
      }      
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "SwapLockWiredPlugin::ConnectToWiredParallel() Setting initial value to 0\n" << vprDEBUG_FLUSH;
      
      char send_char = 255;
      // Keep in mind that the hardware will invert pin 15 on the recveing side, so writeing 255 means to go low.
      write(mWire,&send_char,1);
      return(vpr::ReturnStatus::Succeed);
   }
     // MasterReceive should act identical to SlaveSend since we have no concept of Master/Server with this Sync Method
   void SwapLockWiredPlugin::masterReceive()
   {
      slaveSend();
   }
     // MasterSend should act identical to MasterReceive since we have no concept of Master/Server with this Sync Method
   void SwapLockWiredPlugin::masterSend()
   {
      slaveReceive();
   }
   void SwapLockWiredPlugin::slaveSend()
   {
      vprASSERT(mActive==true && "Barrier is not active!");
      // Send Signal stating that we are at the barrier waiting
      // Keep in mind that the hardware will invert pin 15 on the recveing side, so writeing 0 means to go high.
      char send_char = 0;
      write(mWire,&send_char,1);
   }
   void SwapLockWiredPlugin::slaveReceive()
   {
      // - Wait for signal
      // - Set the local signal wire low again
      
      vprASSERT(mActive==true && "Barrier is not active!");
          
      char buf;
      
        // Mask the value in the Parallel Port register to care 
        // only are about bit 4, which has the decimal value of 8
      read(mWire,&buf,1);
      buf=buf&0x8;
      while((int)buf != 0)
      {
          read(mWire,&buf,1);
          buf=buf&0x8;
      }
      
      // Set the local signal wire low again   
      char send_char = 255; 
      // Keep in mind that the hardware will invert pin 15 on the recveing side, so writeing 255 means to go low.
      write(mWire,&send_char,1);
   }

} // End of cluster namespace
