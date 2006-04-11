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

#ifndef _CLUSTER_SWAP_LOCK_TCP_PLUGIN_H
#define _CLUSTER_SWAP_LOCK_TCP_PLUGIN_H

#include <cluster/Plugins/PluginConfig.h>

#include <map>
#include <vpr/Util/Singleton.h>

// Must implement the Abstract Base Class in order to be a manager used on the ClusterNetwork
#include <cluster/ClusterPlugin.h>

// Remove these includes when we move the handlePacket method to the cpp file.
#include <cluster/Packets/Packet.h>
#include <cluster/ClusterNetwork/ClusterNode.h>

#include <jccl/Config/ConfigChunk.h>
#include <jccl/Config/ConfigChunkPtr.h>
#include <jccl/RTRC/ConfigChunkHandler.h>


extern "C" GADGET_CLUSTER_PLUGIN_API(cluster::ClusterPlugin*) initPlugin();

namespace cluster
{
class GADGET_CLUSTER_PLUGIN_CLASS_API SwapLockTCPPlugin
   : public cluster::ClusterPlugin
{
   vprSingletonHeader( SwapLockTCPPlugin );
public:
   SwapLockTCPPlugin()  : mPluginGUID("5edfc033-1b3e-4741-b0e0-6ebb47967644"),
      SYNC_SIGNAL('G'), read_timeout(1000,vpr::Interval::Msec), mIsMaster(false), mActive(false)
   {;}

   virtual ~SwapLockTCPPlugin()
   {
      delete mSyncServerSocket;
   }
   
   /**
    * Get the GUID associated with this plugin.
    */
   vpr::GUID getPluginGUID()
   {
      return mPluginGUID;
   }

   /**
    * Handle a incoming packet.
    */
   virtual void handlePacket(Packet* packet, ClusterNode* node);

   /**
    * Virtual function that is unused by this plug since we are only 
    * concerned with creating a SwapLock barrier.
    */
   virtual void preDraw();
   
   /**
    * Virtual function that is unused by this plug since we are only 
    * concerned with creating a SwapLock barrier.
    */
   virtual void postPostFrame();
   
   /** Returns the status of SwapLockTCPPlugin(Not Used)
    *
    *  @return true If plugin is completly configured
    *               and has no pending tasks.
    *  
    */
   virtual bool isPluginReady();
   
   /**
    * Create a barrier that will be used to have SwapLock syncronization.
    */
   virtual bool createBarrier();  

   /**
    * Returns the name of this plugin.
    */
   virtual std::string getPluginName()
   {
      return(std::string("SwapLockTCPPlugin"));
   }

   /** Add the pending chunk to the configuration.
    *  PRE: configCanHandle (chunk) == true.
    *  @return true iff chunk was successfully added to configuration.
    */
   bool configAdd(jccl::ConfigChunkPtr chunk);
   
   /** Remove the pending chunk from the current configuration.
    *  PRE: configCanHandle (chunk) == true.
    *  @return true iff the chunk (and any objects it represented)
    *          were successfully removed.
    */
   bool configRemove(jccl::ConfigChunkPtr chunk);

   /** Checks if this handler can process chunk.
    *  Typically, an implementation of handler will check the chunk's
    *  description name/token to decide if it knows how to deal with
    *  it.
    *  @return true iff this handler can process chunk.
    */
   bool configCanHandle(jccl::ConfigChunkPtr chunk);
private:
   /**
    * Helper function that determaines if the given ConfigChunk is a SwapLockTCPPlugin.
    */
   bool recognizeSwapLockTCPPluginConfig(jccl::ConfigChunkPtr chunk);
   
   /**
    * Returns the string representation of the chunk type used for the SwapLockTCPPlugin
    */   
   static std::string getChunkType() { return std::string( "SwapLockTCPPlugin" ); }
   
   /**
    * Attempts to connect to the sync master.
    *
    * @post -If we successfully connect to the sync master, mActive will become true.
    */
   vpr::ReturnStatus ConnectToMasterSocket();

   /**
    * Start listening for incoming connection requests.
    *
    * Note: This will only be used by the barrier master.
    */
   bool startListening();
   
   /**
    * Control loop that is run by mAcceptThread in order to accept incoming connection requests.
    *
    * Note: This will only be used by the barrier master.
    */
   void acceptLoop(void* nullParam);
   
   /**
    * Sends the "complete" signal to all sync slaves.
    */
   void masterSend();

   /**
    * Waits for "ready" signals from all sync slaves.
    */
   void masterReceive();

   /**
    * Sends "ready" signal to sync master.
    */
   void slaveSend();

   /**
    * Waits for "complete" signal from sync master.
    */
   void slaveReceive();
private:
   vpr::Thread*                     mAcceptThread;       /**< Thread that listens for incoming connections. */
   vpr::InetAddr                    mListenAddr;         /**< Address to listen for incoming connections on. */
   
   vpr::SocketStream*               mSyncServerSocket;   /**< SocketStream for the sync master. */
   std::vector<vpr::SocketStream*>  mSyncClients;        /**< Vector of all sync slave's SocketStreams. */
   vpr::Mutex                       mSyncClientsLock;    /**< Mutex used to ensure thread safty while adding sync slaves */
   
   std::string                      mBarrierMasterHostname; /**< Hostname of the sync master. */
   vpr::Uint16                      mTCPport;               /**< TCP Port that the sync master is listening on. */

   vpr::GUID                        mPluginGUID;         /**< GUID for this ClusterPlugin */

   const vpr::Uint8                 SYNC_SIGNAL;         /**< Character that will be sent for each signal. */
   const vpr::Interval              read_timeout;        /**< Maximum time to wait for the barrier before slipping. */
   
   bool                             mIsMaster;           /**< Are we the sync master? */
   bool                             mActive;             /**< Is the plugin ready to be used? */
};

} // end namespace

#endif
