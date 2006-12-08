/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _GADGET_ABSTRACT_NETWORK_MANAGER_H
#define _GADGET_ABSTRACT_NETWORK_MANAGER_H

#include <gadget/gadgetConfig.h>

#include <map>

#include <vpr/IO/Socket/SocketStream.h>
#include <vpr/Util/GUID.h>

#include <jccl/RTRC/ConfigElementHandler.h>
#include <jccl/Config/ConfigElementPtr.h>


namespace cluster
{
   class Packet;
}

namespace gadget
{
class Node;
class PacketHandler;

/** \class AbstractNetworkManager AbstractNetworkManager.h gadget/AbstractNetworkManager.h
 *
 * Network abstraction.
 */
class GADGET_CLASS_API AbstractNetworkManager : public jccl::ConfigElementHandler
{
public:
   /**
    * Construct an empty representation of a network.
    */
   AbstractNetworkManager();

   /**
    * Disconnect all nodes in network and release memory.
    */
   virtual ~AbstractNetworkManager();

private:
   AbstractNetworkManager(const AbstractNetworkManager& anm)
      : jccl::ConfigElementHandler(anm)
   {;}
   void operator=(const AbstractNetworkManager&)
   {;}
public:

   /**
    * Process an incoming packet.
    */
   void handlePacket(cluster::Packet* packet, Node* node);

   void updateNewConnections();

   /**
    * Creates a Node with the given parameters and adds
    * this new node the std::map of Nodes.
    *
    * The caller of this method mustlock the Nodes list
    * first by callinf lockNodes()
    */
   bool addNode(const std::string& name, const std::string& host_name,
                const vpr::Uint16& port,
                vpr::SocketStream* socketStream = NULL);

   /**
    * Adds the given Node to the std::map of Nodes
    *
    * The caller of this method mustlock the Nodes list
    * first by callinf lockNodes()
    */
   void addNode(Node* node);

   /**
    * Removes the Node with the given hostname
    */
   void removeNode(const std::string& nodeHostname);

public:
   /**
    * Returns the Node with the given hostname
    * If no Node with this hostname exists, NULL is returned.
    */
   gadget::Node* getNodeByHostname(const std::string& host_name);

   /**
    * Returns the Node with the given name
    * If no Node with this name exists, NULL is returned.
    */
   gadget::Node* getNodeByName(const std::string& node_name);

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
   void debugDumpNodes(int debug_level);

   /**
    * Get an iterator to the beginning of the Nodes std::vector.
    * The caller of this method must have locked the Nodes list.
    */
   std::vector<gadget::Node*>::iterator getNodesBegin()
   {
      return mNodes.begin();
   }

   /**
    * Get an iterator to the end of the Nodes std::vector.
    * The caller of this method must have locked the Nodes list.
    */
   std::vector<gadget::Node*>::iterator getNodesEnd()
   {
      return mNodes.end();
   }

   /**
    * Return the number of Nodes in the Pending Nodes list.
    */
   vpr::Uint16 getNumPendingNodes();

private:
   /**
    * Attempt to connect to all Nodes in the PendingNodes list.
    */
   bool attemptPendingNodes();

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

   PacketHandler* getHandlerByGUID(const vpr::GUID& handler_guid);
   void addHandler(PacketHandler* new_handler);

   virtual bool attemptConnect(Node* node) = 0;
   virtual void startListening(int listen_port, bool accept_anonymous) = 0;

private:
   std::vector<gadget::Node*>    mNodes;         /**< List of nodes in network. */

   std::map<vpr::GUID, PacketHandler*>  mHandlerMap;
};

} // end namespace gadget

#endif
