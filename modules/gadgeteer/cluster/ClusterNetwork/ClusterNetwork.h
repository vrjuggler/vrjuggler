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

#ifndef _CLUSTER_CLUSTER_NETWORK_H
#define _CLUSTER_CLUSTER_NETWORK_H

#include <gadget/gadgetConfig.h>
#include <vpr/Util/Singleton.h>
#include <vpr/Sync/Mutex.h>
#include <vpr/IO/Socket/InetAddr.h>
#include <jccl/RTRC/ConfigChunkHandler.h>
#include <jccl/Config/ConfigChunkPtr.h>

#include <vpr/IO/Socket/SocketStream.h>
//#include <list>

namespace vpr
{
   class ReturnStatus;
}
namespace jccl
{
   class ConfigChunk;
}

namespace cluster
{

class ClusterNode;
class Packet;

class GADGET_CLASS_API ClusterNetwork : public jccl::ConfigChunkHandler
{
   vprSingletonHeader( ClusterNetwork );
public:
   ClusterNetwork();
   virtual ~ClusterNetwork();

   void handlePacket(Packet* packet, ClusterNode* node);
     
   bool isClusterNetworkReady();

   void updateNewConnections();

   /** Locks the list of ClusterNodes
    *
    *  This function blocks until it can lock the std::vector of
    *  ClusterNodes.
    *
    *  The caller of this method must call unlockActive() when it
    *  is finished viewing/modifying the active list.
    */
   void lockClusterNodes()
   { mClusterNodesLock.acquire(); }

   /** Unlocks the list of ClusterNodes
    *
    *  The method releases the lock on the std::map.of
    *  ClusterNodes
    *
    *  The caller of this method must have previously locked the 
    *  ClusterNodes list with lockClusterNodes().
    */
   void unlockClusterNodes()
   { mClusterNodesLock.release(); }

private:  
   /**
    * Creates a ClusterNode with the given parameters and adds
    * this new node the std::map of ClusterNodes.
    *
    * The caller of this method mustlock the ClusterNodes list
    * first by callinf lockClusterNodes()
    */
   vpr::ReturnStatus addClusterNode(const std::string& name, const std::string& host_name, 
                                    const vpr::Uint16& port, vpr::SocketStream* socket_stream = NULL);

   /**
    * Adds the given ClusterNode to the std::map of ClusterNodes
    *
    * The caller of this method mustlock the ClusterNodes list
    * first by callinf lockClusterNodes()
    */
   void addClusterNode(ClusterNode* node);

   /**
    * Removes the ClusterNode with the given hostname
    */
   void removeClusterNode(const std::string& node_hostname);

public:  
   /** Returns the ClusterNode with the given hostname
    *  If no ClusterNode with this hostname exists, NULL is returned.
    */   
   cluster::ClusterNode* getClusterNodeByHostname(const std::string& host_name);
   
   /** Returns the ClusterNode with the given name
    *  If no ClusterNode with this name exists, NULL is returned.
    */   
   cluster::ClusterNode* getClusterNodeByName(const std::string& node_name);
   
   /**
    * Get the number of nodes in the current cluster configuration.
    */
   int getNumClusterNodes()
   { return mClusterNodes.size(); }

   /** Print out debug information abour all nodes in the current 
    *  cluster configuration
    *
    *  @pre The caller of this method must NOT have locked the pending list.
    */
   void debugDumpClusterNodes(int debug_level);

   /** Get an iterator to the beginning of the ClusterNodes std::vector.
    *  The caller of this method must have locked the ClusterNodes list.
    */
   std::vector<cluster::ClusterNode*>::iterator getClusterNodesBegin()
   {
      vprASSERT(1 == mClusterNodesLock.test());
      return mClusterNodes.begin();
   }
   
   /** Get an iterator to the end of the ClusterNodes std::vector.
    *  The caller of this method must have locked the ClusterNodes list.
    */
   std::vector<cluster::ClusterNode*>::iterator getClusterNodesEnd()
   {
      vprASSERT(1 == mClusterNodesLock.test());
      return mClusterNodes.end();
   }

public:
   /** Locks the list of PendingNodes
    *
    *  This function blocks until it can lock the std::vector of
    *  PendingNodes.
    *
    *  The caller of this method must call unlockActive() when it
    *  is finished viewing/modifying the active list.
    */
   void lockPendingNodes()
   { mPendingNodesLock.acquire(); }
   
   /** Unlocks the list of PendingNodes
    *
    *  The method releases the lock on the std::map.of
    *  PendingNodes
    *
    *  The caller of this method must have previously locked the PendingNodes
    *  list with lockPendingNodes().
    */   
   void unlockPendingNodes()
   { mPendingNodesLock.release(); }

public:
   /** Adds the given ClusterNode to a list of pending nodes
    *  that are still trying to gain a connection.
    */
   void addPendingNode(ClusterNode* node);
   
   /** Removes the ClusterNode with the given hostname from
    *  the list of PendingNodes.
    */
   void removePendingNode(std::string hostname);

   /** Returns the Pending node with the given hostname.
    *  If the is not a node with the given hotname, NULL is returned.
    */
   ClusterNode* getPendingNode(std::string host_name);

private:      
   /**
    * Return the number of ClusterNodes in the Pending Nodes list.
    */
   vpr::Uint16 getNumPendingNodes();
   
   /**
    * Attempt to connect to all ClusterNodes in the PendingNodes list.
    */
   bool attemptPendingNodes();

   
   /////////////////////////////
   //    ACCEPTING METHODS    //
   /////////////////////////////
private:
   /**
    * Start the listening thread on the given port
    */
   bool startListening(const int& listen_port);

   /**
    * Control Loop used to continuously recieve incoming connections.
    */
   void acceptLoop(void* nullParam);           
   
   /////////////////////////////
   //    UPDATING METHODS     //
   /////////////////////////////
private:   
   /**
    * Kill the listen thread and the update thread
    */
   void shutdown();


   //////////////////////////
   //    CONFIG METHODS    //
   //////////////////////////
public:
   /**
    * Determine if the given ConfigChunk is a MachineSpecific chunk.
    */
   bool recognizeClusterMachineConfig(jccl::ConfigChunkPtr chunk);

   /**
    * Determine if we can handle the given ConfigChunk.
    */
   bool configCanHandle(jccl::ConfigChunkPtr chunk);

   /**
    * Configure the given ConfigChunk because it was just added to
    * the active configuration.
    * 
    * @return true  If we successfully configured the given MachineSpecific Chunk.
    * @return false If we failed to configure the given MachineSpecific Chunk.
    */
   bool configAdd(jccl::ConfigChunkPtr chunk);

   /**
    * Remove the given ConfigChunk from the active configuration .
    * 
    * @return true  If we successfully removed the given MachineSpecific Chunk.
    * @return false If we failed to removed the given MachineSpecific Chunk.
    */
   bool configRemove(jccl::ConfigChunkPtr chunk);
   
   /**
    * Return the hostname of the local machine.
    */
   std::string getLocalHostname() { return mLocalHostnameShort; }

   /**
    * Return the chunk type for MachineSpecific chunks that we configure here.
    */
   static std::string getMachineSpecificChunkType() { return std::string( "MachineSpecific" ); }

private:   
   std::vector<cluster::ClusterNode*>     mClusterNodes;       /**< Current configuration.     */
   vpr::Mutex                             mClusterNodesLock;   /**< Lock on active config list.*/
   
   std::vector<cluster::ClusterNode*>     mPendingNodes;       /**< Vector of Pending Nodes */
   vpr::Mutex                             mPendingNodesLock;   /**< Lock the mPendingNodes list */
   
   vpr::Thread*                           mUpdateThread;       /**< Thread to continually attempt pending nodes */
   
   vpr::Thread*                           mAcceptThread;       /**< Thread that listens for incoming connections. */
   vpr::InetAddr                          mListenAddr;         /**< Address to listen for incoming connections on. */
   std::string                            mLocalHostnameShort; /**< Local hostname with domain included */
   std::string                            mLocalHostnameLong;  /**< Local hostname without domain */
};

} // end namespace gadget

#endif
