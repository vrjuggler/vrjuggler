/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _GADGET_NETWORK_MANAGER_H
#define _GADGET_NETWORK_MANAGER_H

#include <gadget/gadgetConfig.h>

#include <boost/version.hpp>

#if defined(__GNUC__) && __GNUC__ >= 4
#  include <tr1/unordered_map>
#elif defined(_MSC_VER) && _MSC_VER >= 1500
#  include <unordered_map>
#elif BOOST_VERSION >= 103600
#  include <boost/unordered_map.hpp>
#elif defined(VPR_HASH_MAP_INCLUDE)
#  include VPR_HASH_MAP_INCLUDE
#else
#  include <map>
#endif

#include <vpr/IO/Socket/SocketStream.h>
#include <vpr/Util/GUID.h>

#include <jccl/RTRC/ConfigElementHandler.h>
#include <jccl/Config/ConfigElementPtr.h>

#include <gadget/NodePtr.h>
#include <gadget/Reactor.h>
#include <gadget/PacketHandlerPtr.h>
#include <cluster/Packets/PacketPtr.h>

namespace gadget
{

/** \class NetworkManager NetworkManager.h gadget/NetworkManager.h
 *
 * Network abstraction.
 */
class GADGET_CLASS_API NetworkManager : public jccl::ConfigElementHandler
{
public:
   typedef std::vector<NodePtr> node_list_t;

   /**
    * Construct an empty representation of a network.
    */
   NetworkManager();

   /**
    * Disconnect all nodes in network and release memory.
    */
   virtual ~NetworkManager();

private:
   NetworkManager(const NetworkManager& anm)
      : jccl::ConfigElementHandler(anm)
   {;}
   void operator=(const NetworkManager&)
   {;}
public:

   /**
    * Process an incoming packet.
    */
   void handlePacket(cluster::PacketPtr packet, NodePtr node);

   void update( const int temp);
   void barrier( bool master );
   
   /**
    * Optimize network traffic by gathering write calls.
    * uncorkNetwork() must be called once write calls are finished.
    */
   void corkNetwork();

   /**
    * Flush pending writes to the network.
    */
   void uncorkNetwork();

private:
   size_t setAllUpdated( const bool updated );
   size_t sendEndBlocks( const int temp );
   void updateAllNodes( const size_t numNodes );

public:
   /**
    * Creates a Node with the given parameters and adds
    * this new node the std::map of Nodes.
    *
    * The caller of this method mustlock the Nodes list
    * first by callinf lockNodes()
    */
   bool addNode(const std::string& name, const std::string& hostName,
                const vpr::Uint16& port,
                vpr::SocketStream* socketStream = NULL);

   /**
    * Adds the given Node to the std::map of Nodes
    *
    * The caller of this method mustlock the Nodes list
    * first by callinf lockNodes()
    */
   void addNode(NodePtr node);

   /**
    * Removes the Node with the given hostname
    */
   void removeNode(const std::string& nodeHostname);

public:
   /**
    * Returns the Node with the given hostname
    * If no Node with this hostname exists, NULL is returned.
    */
   NodePtr getNodeByHostname(const std::string& hostName);

   /**
    * Returns the Node with the given name
    * If no Node with this name exists, NULL is returned.
    */
   NodePtr getNodeByName(const std::string& nodeName);

   /**
    * Get the number of nodes in network.
    */
   size_t getNumNodes()
   {
      return mNodes.size();
   }

   /**
    * Print out debug information about all nodes.
    */
   void debugDumpNodes(int debugLevel);

   /**
    * Print the status of each node in the cluster.
    */
   void printStatus(int debugLevel);

   Reactor& getReactor()
   {
      return mReactor;
   }

   /**
    * Get an iterator to the beginning of the Nodes std::vector.
    * The caller of this method must have locked the Nodes list.
    */
   node_list_t::iterator getNodesBegin()
   {
      return mNodes.begin();
   }

   /**
    * Get an iterator to the end of the Nodes std::vector.
    * The caller of this method must have locked the Nodes list.
    */
   node_list_t::iterator getNodesEnd()
   {
      return mNodes.end();
   }

   /**
    * Get a reference to the list of Nodes.
    * The caller of this method must have locked the Nodes list.
    */
   node_list_t& getNodes()
   {
      return mNodes;
   }

   void waitForConnection(const vpr::Uint16 listenPort);
   bool connectToSlaves();

   /**
    * Send packet to all cluster nodes.
    *
    * @since 1.3.6
    */
   void sendToAll(cluster::PacketPtr packet);

protected:
   bool connectTo(NodePtr node);

public:
   /**
    * Kill the listen thread and the update thread
    */
   void shutdown();

public:
   /**
    * Determine if the given jccl::ConfigElement is a cluster_node element.
    */
   bool recognizeClusterMachineConfig(jccl::ConfigElementPtr element);

   /**
    * Determine if we can handle the given jccl::ConfigElement.
    */
   bool configCanHandle(jccl::ConfigElementPtr element);

   /**
    * Configure the given jccl::ConfigElement because it was just added to
    * the active configuration.
    *
    * @return true  If we successfully configured the given cluster_node
    *               element.
    * @return false If we failed to configure the given cluster_node element.
    */
   bool configAdd(jccl::ConfigElementPtr element);

   /**
    * Return the element type for cluster_node element that we configure
    * here.
    */
   static std::string getClusterNodeElementType();

   /**
    * Remove the given jccl::ConfigElement from the active configuration .
    *
    * @return true  If we successfully removed the given cluster_node element.
    * @return false If we failed to removed the given cluster_node element.
    */
   bool configRemove(jccl::ConfigElementPtr element);

   /**
    * Determine if the given hostname matches the local machine's hostname.
    */
   static bool isLocalHost(const std::string& testHostName);

   PacketHandlerPtr getHandlerByGUID(const vpr::GUID& handlerGuid);
   void addHandler(PacketHandlerPtr newHandler);

private:
   node_list_t                  mNodes;         /**< List of nodes in network. */
   vpr::InetAddr                mListenAddr;    /**< Address to listen for incoming connections on. */

#if defined(__GNUC__) && __GNUC__ >= 4 || defined(_MSC_VER) && _MSC_VER >= 1500
   typedef std::tr1::unordered_map<vpr::GUID
                                , PacketHandlerPtr
                                , vpr::GUID::hash> packet_handler_map_t;
#elif BOOST_VERSION >= 103600
   typedef boost::unordered_map<vpr::GUID
                              , PacketHandlerPtr
                              , vpr::GUID::hash> packet_handler_map_t;
#elif defined(VPR_HASH_MAP_INCLUDE)
   typedef std::hash_map<vpr::GUID, PacketHandlerPtr, vpr::GUID::hash>
      packet_handler_map_t;
#else
   typedef std::map<vpr::GUID, PacketHandlerPtr> packet_handler_map_t;
#endif

   packet_handler_map_t         mHandlerMap;
   Reactor                      mReactor;
};

} // end namespace gadget

#endif /*_GADGET_NETWORK_MANAGER_H*/
