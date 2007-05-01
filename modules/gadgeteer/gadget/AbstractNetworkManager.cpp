/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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

#include <gadget/gadgetConfig.h>

#include <algorithm>
#include <iomanip>

#include <vpr/IO/Socket/InetAddr.h>

#include <cluster/Packets/Header.h>
#include <cluster/Packets/Packet.h>
#include <cluster/Packets/PacketFactory.h>

#include <gadget/Node.h>
#include <gadget/PacketHandler.h>
#include <gadget/Util/Debug.h>

#include <jccl/Config/ConfigElement.h>
#include <jccl/RTRC/ConfigManager.h>

#include <gadget/AbstractNetworkManager.h>

#include <boost/concept_check.hpp>   /* for ignore_unused_variable_warning */

namespace gadget
{

AbstractNetworkManager::AbstractNetworkManager() 
   : mNodes(0), mHandlerMap()
{;}

AbstractNetworkManager::~AbstractNetworkManager()
{
   shutdown();
}

void AbstractNetworkManager::waitForConnection(const int& listen_port)
{
   mListenAddr.setPort( listen_port );

   vprDEBUG( gadgetDBG_NET_MGR, vprDBG_CONFIG_STATUS_LVL )
      << clrOutBOLD( clrBLUE, "[ClusterNetwork]" )
      << " Waiting for connection...\n" << vprDEBUG_FLUSH;

   // Create a socket to listen for incoming connections
   vpr::SocketStream sock( mListenAddr, vpr::InetAddr::AnyAddr );
   
   // Open in server mode.
   try
   {
      // Open server enabling reuse of bound address.
      sock.openServer(true);

      vprDEBUG( gadgetDBG_NET_MGR, vprDBG_CONFIG_STATUS_LVL )
      << clrOutBOLD( clrBLUE, "[AbstractNetworkManager]" )
      << " Listening on Port: " << mListenAddr.getPort()
      << std::endl << vprDEBUG_FLUSH;
   }
   catch (vpr::IOException&)
   {
      vprDEBUG( gadgetDBG_NET_MGR, vprDBG_CRITICAL_LVL )
         << clrSetBOLD( clrRED ) << "[AbstractNetworkManager]"
         << " Unable to open listening socket on port: "
         << mListenAddr.getPort() << std::endl
         << clrRESET << vprDEBUG_FLUSH;

      exit(0);
   }

   // Create a socketstream for new socket
   vpr::SocketStream* client_sock = new vpr::SocketStream();

   try
   {
      // Wait for a connection request.
      sock.accept( *client_sock, vpr::Interval::NoTimeout );

      vprDEBUG(gadgetDBG_NET_MGR, vprDBG_CONFIG_LVL)
         << clrOutBOLD(clrBLUE, "[AbstractNetworkManager]")
         << " Receiving a connection on Port: "
         << mListenAddr.getPort() << std::endl << vprDEBUG_FLUSH;

      // Optimize new socket for low latency communication
      client_sock->setNoDelay( true );

      // Get the hostname and port of the requesting host.
      const std::string remote_host_name(
         client_sock->getRemoteAddr().getHostname()
      );
      vpr::Uint16 port = client_sock->getRemoteAddr().getPort();

      vprDEBUG( gadgetDBG_NET_MGR, vprDBG_CONFIG_LVL )
         << clrOutBOLD( clrBLUE, "[AbstractNetworkManager]" )
         << " Received from: " << remote_host_name
         << ":" << port << std::endl << vprDEBUG_FLUSH;

      NodePtr remote_node = getNodeByHostname( remote_host_name );

      vprASSERT(NULL == remote_node && "We already know about the master node.");

      // Get address information about local host.
      const vpr::InetAddr local = vpr::InetAddr::getLocalHost();
      const std::string local_hostname = local.getHostname();

      vprDEBUG( gadgetDBG_NET_MGR, vprDBG_STATE_LVL )
         << clrOutBOLD( clrBLUE,"[Acceptor]" )
         << " Set SockStream and send responce."
         << std::endl << vprDEBUG_FLUSH;

      // Add the master node to the cluster network.
      addNode("master", remote_host_name, port, client_sock);
      remote_node = getNodeByHostname( remote_host_name );

      vprASSERT(NULL != remote_node && "Master node must exist now.");

      vprDEBUG( gadgetDBG_NET_MGR,vprDBG_STATE_LVL )
         << clrOutBOLD( clrBLUE,"[Acceptor]" )
         << " Set new Node as a NEWCONNECTION."
         << std::endl << vprDEBUG_FLUSH;

      // XXX: Should not need this since network is going
      //      to be fully connected before starting frame loop.

      // Since we have just recieved a new connection,
      // set the connected status as so. We are not
      // in a fully connected state until the begining
      // of the next frame in Acceptor::updateNewConnetions()
      // this is becuase we only want to start using a
      // new connection at the start of a new frame.
      remote_node->setStatus( Node::NEWCONNECTION );
      // Print the new state information about this node.
      remote_node->debugDump( vprDBG_CONFIG_LVL );
      
      // XXX: We need to fix this in the near future.
      //ClusterDelta cluster_delta;
      //cluster_delta.clientClusterDelta(requesting_node->getSockStream());

      // XXX: Should be alright.
      remote_node->setStatus( Node::CONNECTED );
      mReactor.addNode(remote_node);
   }
   catch (vpr::IOException&)
   {
      // Should never happen since timeout is infinite
      if (client_sock->isOpen())
      {
         client_sock->close();
      }
      delete client_sock;
      client_sock = new vpr::SocketStream;
   }
}

/**
 * Determine if the given hostname matches the local machine's hostname.
 */
bool AbstractNetworkManager::isLocalHost(const std::string& testHostName)
{
   // Resolve the address of the hostname given to test.
   vpr::InetAddr remote_address;
   remote_address.setAddress(testHostName, 0);

   vprDEBUG(gadgetDBG_NET_MGR,vprDBG_HVERB_LVL)
      << "+======= Resolved addresses for given hostname ======+"
      << std::endl << vprDEBUG_FLUSH;

   std::string remote_hostname;
   try
   {
      remote_hostname = remote_address.getHostname();
   }
   catch(std::exception&)
   {
      remote_hostname = "Error resolving remote hostname";
   }
   
   vprDEBUG( gadgetDBG_NET_MGR, vprDBG_HVERB_LVL )
         << "| Name: " << std::setw(25) << remote_hostname << " | "
         << std::setw(16) << remote_address.getAddressString() << " | "
         << std::endl << vprDEBUG_FLUSH;

   // Try to get all local interfaces.
   //   - Fall back on local host
   std::vector<vpr::InetAddr> local_interfaces;
   try
   {
      local_interfaces = vpr::InetAddr::getAllLocalAddrs();
   }
   catch (vpr::Exception& ex)
   {
      vprDEBUG(gadgetDBG_NET_MGR, vprDBG_WARNING_LVL)
         << clrOutBOLD(clrYELLOW, "WARNING: ")
         << "Failed to get list of local interfaces: "
         << ex.getExtendedDescription()
         << std::endl << vprDEBUG_FLUSH;
      try
      {
         vpr::InetAddr local_host = vpr::InetAddr::getLocalHost();
         local_interfaces.push_back(local_host);
      }
      catch (vpr::Exception& ex)
      {
         vprDEBUG(gadgetDBG_NET_MGR, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING: ")
            << "Failed to get local host: "
            << ex.getExtendedDescription()
            << std::endl << vprDEBUG_FLUSH;
      }
   }

   // Print debug information about all local interfaces.
   vprDEBUG( gadgetDBG_NET_MGR, vprDBG_HVERB_LVL )
      << "+================= Local interfaces =================+"
      << std::endl << vprDEBUG_FLUSH;
   for (std::vector<vpr::InetAddr>::iterator itr = local_interfaces.begin() ; itr != local_interfaces.end() ; ++itr)
   {
      std::string temp_hostname;
      try
      {
         temp_hostname = (*itr).getHostname();
      }
      catch(std::exception&)
      {
         temp_hostname = "Error getting hostname";
      }
      vprDEBUG( gadgetDBG_NET_MGR, vprDBG_HVERB_LVL )
         << "| Name: " << std::setw(25) << temp_hostname << " | "
         << std::setw(16) << (*itr).getAddressString() << " | "
         << std::endl << vprDEBUG_FLUSH;
   }

   vprDEBUG( gadgetDBG_NET_MGR, vprDBG_HVERB_LVL )
      << "+====================================================+"
      << std::endl << vprDEBUG_FLUSH;

   bool result(false);
   for (std::vector<vpr::InetAddr>::iterator itr = local_interfaces.begin() ; itr != local_interfaces.end() ; ++itr)
   {
      if ((*itr).getAddressValue() == remote_address.getAddressValue())
      {
         vprDEBUG( gadgetDBG_NET_MGR, vprDBG_HVERB_LVL )
            << "| We have a match.                  "
            << std::setw(16) << (*itr).getAddressString() << " |"
            << std::endl << vprDEBUG_FLUSH;
         result = true;
      }
      else
      {
         vprDEBUG( gadgetDBG_NET_MGR, vprDBG_HVERB_LVL )
            << "| NO match.                         "
            << std::setw(16) << (*itr).getAddressString() << " |"
            << std::endl << vprDEBUG_FLUSH;
      }
   }
   vprDEBUG( gadgetDBG_NET_MGR, vprDBG_HVERB_LVL )
      << "+====================================================+"
      << std::endl << vprDEBUG_FLUSH;

   return result;
}


void AbstractNetworkManager::handlePacket(cluster::PacketPtr packet, NodePtr node)
{
   //vprDEBUG( gadgetDBG_NET_MGR, 0 )
   //   << clrOutBOLD( clrBLUE, "[AbstractNetworkManager]" )
   //   << " Got packet."
   //   << std::endl << vprDEBUG_FLUSH;

   // If the ClusterManager should handle this packet, then do so.
   if (packet->getPacketType() == cluster::Header::RIM_END_BLOCK)
   {
      // -Set New State
      if (node == NULL)
      {
         return;
      }

      node->setUpdated( true );
      return;
   }
   else if (packet->getPacketType() == cluster::Header::RIM_CONNECTION_REQ ||
            packet->getPacketType() == cluster::Header::RIM_CONNECTION_ACK)
   {
      //handleLocalPacket(packet, node);
      
      vprDEBUG( gadgetDBG_NET_MGR, vprDBG_CRITICAL_LVL )
         << clrOutBOLD( clrRED, "[ERROR] " )
         << "RIM_CONNECTION_REQ & RIM_CONNECTION_ACK data packet types are depreciated."
         << std::endl << vprDEBUG_FLUSH;
      return;
   }

   vpr::GUID handler_guid = packet->getPluginId();

   PacketHandler* temp_handler = getHandlerByGUID( handler_guid );

   //vprDEBUG( gadgetDBG_NET_MGR, 0 )
   //   << clrOutBOLD( clrBLUE, "[AbstractNetworkManager]" )
   //   << " Got packet for: " << handler_guid.toString()
   //   << std::endl << vprDEBUG_FLUSH;

   if (NULL != temp_handler)
   {
      vprDEBUG( gadgetDBG_NET_MGR, vprDBG_HVERB_LVL )
         << clrOutBOLD(clrBLUE,"[AbstractNetworkManager]")
         << " Handler \"" << temp_handler->getHandlerName() << "\" will handle this packet."
         << std::endl << vprDEBUG_FLUSH;


      temp_handler->handlePacket( packet, node );
   }
   else
   {
      vprDEBUG( gadgetDBG_NET_MGR, vprDBG_CONFIG_LVL )
         << clrOutBOLD( clrBLUE, "[AbstractNetworkManager]" )
         << " Handler " << handler_guid.toString() << " does not exist to handle this packet."
         << std::endl << vprDEBUG_FLUSH;
   }
}

bool AbstractNetworkManager::addNode(const std::string& name,
                                     const std::string& host_name,
                                     const vpr::Uint16& port,
                                     vpr::SocketStream* socketStream)
{
   // -Create a new Node using the given information
   // -Add the new node to the AbstractNetworkManager
   
   vprDEBUG( gadgetDBG_NET_MGR, vprDBG_CONFIG_LVL )
      << clrOutBOLD(clrBLUE,"[AbstractNetworkManager]")
      << " Adding node: " << name
      << std::endl << vprDEBUG_FLUSH;

   NodePtr temp_node = NodePtr(new Node(name, host_name, port, socketStream, this));
   mNodes.push_back( temp_node );
   
   return true;
}

void AbstractNetworkManager::addNode(NodePtr node)
{
   // -Add the given node to the AbstractNetworkManager
   
   vprDEBUG( gadgetDBG_NET_MGR, vprDBG_CONFIG_LVL )
      << clrOutBOLD( clrBLUE, "[AbstractNetworkManager]" )
      << " Adding node: " << node->getName()
      << std::endl << vprDEBUG_FLUSH;

   mNodes.push_back( node );
}

struct HostnamePred
{
   HostnamePred(const std::string& nodeHostname)
      : mHostname(nodeHostname)
   {
      /* Do nothing. */ ;
   }

   bool operator()(gadget::NodePtr n)
   {
      return n->getHostname() == mHostname;
   }

   const std::string& mHostname;
};

void AbstractNetworkManager::removeNode(const std::string& nodeHostname)
{
   vprDEBUG( gadgetDBG_NET_MGR, vprDBG_CONFIG_LVL )
      << clrOutBOLD( clrBLUE, "[AbstractNetworkManager]" )
      << " Removing node: " << nodeHostname
      << std::endl << vprDEBUG_FLUSH;

   typedef std::vector<gadget::NodePtr>::iterator iter_t;
   HostnamePred pred(nodeHostname);
   iter_t n = std::find_if(mNodes.begin(), mNodes.end(), pred);

   if ( n != mNodes.end() )
   {
      mReactor.removeNode(*n);
      mNodes.erase(n);
   }
}
   
NodePtr AbstractNetworkManager::getNodeByHostname(const std::string& host_name)
{
   vpr::DebugOutputGuard dbg_output( gadgetDBG_NET_MGR, vprDBG_VERB_LVL,
      std::string("-------- getNodeByHostname() --------\n"),
      std::string("--------------------------------------------\n"));

   vpr::InetAddr searching_for_node;
   searching_for_node.setAddress( host_name, 0 );

   vprDEBUG( gadgetDBG_NET_MGR, vprDBG_VERB_LVL )
      << clrOutBOLD( clrBLUE, "[AbstractNetworkManager]" )
      << " Looking for Node with hostname: " << host_name
      << std::endl << vprDEBUG_FLUSH;

      
   // -Find Node with given hostname and return a pointer to it.
   // -If we do not find one, return NULL
   for (node_list_t::iterator itr = mNodes.begin(); itr != mNodes.end(); itr++)
   {
      vpr::InetAddr testing_node;
      testing_node.setAddress( (*itr)->getHostname(), 0 );
      
      vprDEBUG( gadgetDBG_NET_MGR, vprDBG_VERB_LVL )
         << clrOutBOLD( clrBLUE, "[AbstractNetworkManager]" )
         << " Searcing for: " << searching_for_node.getAddressString()
         << " Testing: " << testing_node.getAddressString()
         << std::endl << vprDEBUG_FLUSH;

      if (searching_for_node.getAddressString() == testing_node.getAddressString())
      {
         return(*itr);
      }
   }

   return NodePtr();
}

NodePtr AbstractNetworkManager::getNodeByName(const std::string& node_name)
{
   // -Find Node with given name and return a pointer to it.
   // -If we do not find one, return NULL
   for (node_list_t::iterator itr = mNodes.begin(); itr != mNodes.end(); itr++)
   {
      if ((*itr)->getName() == node_name)
      {
         return *itr;
      }
   }

   return NodePtr();
}

vpr::Uint16 AbstractNetworkManager::getNumPendingNodes()
{
   return 0;
}

bool AbstractNetworkManager::connectToSlaves()
{
   bool ret_val = false;

   for (node_list_t::iterator itr = mNodes.begin(); itr != mNodes.end(); itr++)
   {
      if (connectTo(*itr))
      {
         // If any of the nodes were successful connecting
         // then we should return true
         ret_val = true;
      }
   }
   return ret_val;
}

void AbstractNetworkManager::sendToAll(cluster::PacketPtr packet)
{
   for (node_list_t::iterator itr = mNodes.begin(); itr != mNodes.end(); itr++)
   {
      try
      {
         (*itr)->send(packet);
      }
      catch( cluster::ClusterException cluster_exception )
      {
         vprDEBUG( gadgetDBG_RIM, vprDBG_CONFIG_LVL )
            << "AbstractNetworkManager::sendToAll() Caught an exception!"
            << std::endl << vprDEBUG_FLUSH;
         vprDEBUG( gadgetDBG_RIM, vprDBG_CONFIG_LVL )
            << clrOutBOLD(clrRED, "ERROR:") << cluster_exception.what()
            << std::endl << vprDEBUG_FLUSH;
         vprDEBUG( gadgetDBG_RIM, vprDBG_CONFIG_LVL )
            << "DeviceServer::send() We have lost our connection to: "
            << (*itr)->getName() << ":" << (*itr)->getPort()
            << std::endl << vprDEBUG_FLUSH;

         (*itr)->setStatus( gadget::Node::DISCONNECTED );
         (*itr)->shutdown();

         debugDump( vprDBG_CONFIG_LVL );
      }
   }
}

bool AbstractNetworkManager::connectTo(NodePtr node)
{
   //vprASSERT( Node::PENDING == node->getStatus() &&
   //           "Can not connect to a node that is not pending." );

   vpr::SocketStream* sock_stream(NULL);
   vpr::InetAddr inet_addr;

   vprDEBUG( gadgetDBG_NET_MGR, vprDBG_CONFIG_STATUS_LVL)
      << clrOutBOLD( clrBLUE,"[AbstractNetworkManager]" )
      << " Attempting to connect to: " << node->getName()
      << std::endl << vprDEBUG_FLUSH;
   
   try
   {
      // Set the address that we want to connect to
      inet_addr.setAddress( node->getHostname(), node->getPort() );
   }
   catch (vpr::IOException&)
   {
      vprDEBUG( gadgetDBG_NET_MGR, vprDBG_CRITICAL_LVL )
         << clrOutBOLD( clrBLUE,"[AbstractNetworkManager]" )
         << clrOutBOLD( clrRED, " ERROR:" )
         << " Failed to set address" << std::endl << vprDEBUG_FLUSH;
      return false;
   }

   // Create a new socket stream to this address
   sock_stream = new vpr::SocketStream( vpr::InetAddr::AnyAddr, inet_addr );

   // If we can successfully open the socket and connect to the server
   sock_stream->open();

   vpr::Uint16 retry_count(0);
   while (!sock_stream->isConnected())
   {
      try
      {
         sock_stream->connect();

         vprDEBUG( gadgetDBG_NET_MGR, vprDBG_CONFIG_STATUS_LVL )
            << clrOutBOLD( clrBLUE,"[AbstractNetworkManager]" )
            << " Successfully connected to: "
            << node->getHostname() <<":"<< node->getPort()
            << std::endl << vprDEBUG_FLUSH;
      }
      catch (vpr::IOException& ex)
      {
         vpr::Uint16 level = vprDBG_CONFIG_STATUS_LVL;

         if (retry_count >= 50)
         { level = vprDBG_CRITICAL_LVL; }

         vprDEBUG( gadgetDBG_NET_MGR, level)
            << clrOutBOLD( clrBLUE,"[AbstractNetworkManager]" )
            << clrOutBOLD( clrRED, " ERROR:" )
            << " Could not connect to Node: "
            << node->getHostname() << " : " << node->getPort()
            << " retry: " << retry_count << std::endl
            << ex.getExtendedDescription() << std::endl << vprDEBUG_FLUSH;

         if (retry_count > 50)
         { throw; }

         retry_count++;
         vpr::System::sleep(1);
      }
   }

   sock_stream->setNoDelay( true );
   node->setSockStream( sock_stream );

   // XXX: Should be alright.
   node->setStatus( Node::CONNECTED );
   mReactor.addNode(node);

   return true;
}

bool AbstractNetworkManager::attemptPendingNodes()
{
   bool ret_val = false;

   return ret_val;
}

void AbstractNetworkManager::shutdown()
{
   for (node_list_t::iterator itr = mNodes.begin(); itr != mNodes.end(); itr++)
   {
       (*itr)->shutdown();
   }
}


void AbstractNetworkManager::debugDumpNodes(int debug_level)
{
   vpr::DebugOutputGuard dbg_output(gadgetDBG_NET_MGR,debug_level,
      std::string("-------------- Cluster Network --------------\n"),
      std::string("---------------------------------------------\n"));

   for(node_list_t::iterator itr = mNodes.begin(); itr != mNodes.end(); itr++)
   {
      (*itr)->debugDump( debug_level );
   }
}

bool AbstractNetworkManager::recognizeClusterMachineConfig(jccl::ConfigElementPtr element)
{
   return (element->getID() == getClusterNodeElementType());
}

bool AbstractNetworkManager::configCanHandle(jccl::ConfigElementPtr elm)
{
   return recognizeClusterMachineConfig(elm);
}

bool AbstractNetworkManager::configAdd(jccl::ConfigElementPtr elm)
{
   boost::ignore_unused_variable_warning(elm);
   return true;
}

bool AbstractNetworkManager::configRemove(jccl::ConfigElementPtr elm)
{
   boost::ignore_unused_variable_warning(elm);
   return true;
}

std::string AbstractNetworkManager::getClusterNodeElementType()
{
   return "cluster_node";
}

void AbstractNetworkManager::updateNewConnections()
{
}

PacketHandler* AbstractNetworkManager::getHandlerByGUID(const vpr::GUID& handler_guid)
{
   std::map<vpr::GUID, PacketHandler*>::const_iterator i = mHandlerMap.find( handler_guid );
   if (i != mHandlerMap.end())
   {
      return ((*i).second);
   }
   return NULL;
}

/**
 * Adds a new plugin to the ClusterManager.
 */
void AbstractNetworkManager::addHandler(PacketHandler* new_handler)
{
   std::pair<vpr::GUID, PacketHandler*> p 
      = std::make_pair( new_handler->getHandlerGUID(), new_handler );
   mHandlerMap.insert( p );

   vprDEBUG( gadgetDBG_NET_MGR, vprDBG_CONFIG_LVL )
      << clrOutBOLD( clrBLUE, "[Reactor] " )
      << "Adding Handler: " << new_handler->getHandlerName() << std::endl << vprDEBUG_FLUSH;
}

} // end namespace gadget
