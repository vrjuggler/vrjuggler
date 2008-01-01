/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 ****************** <VPR heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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

#include <vpr/vprConfig.h>

#include <stdlib.h>
#include <boost/concept_check.hpp>
#include <boost/utility.hpp>
#include <vpr/Util/Debug.h>
#include <vpr/Util/Assert.h>

#include <vpr/md/SIM/Controller.h>
#include <vpr/md/SIM/IO/Socket/SocketImplSIM.h>
#include <vpr/md/SIM/IO/Socket/SocketStreamImplSIM.h>
#include <vpr/md/SIM/IO/Socket/SocketDatagramImplSIM.h>
#include <vpr/md/SIM/Network/NetworkGraph.h>
#include <vpr/md/SIM/SocketManager.h>


namespace vpr
{

namespace sim
{

   const vpr::Uint32 LocalHostIpAddrValue(0x7F000001);

   // ===========
   // connector:
   // ===========

   // If the socket is of type SOCK_STREAM, then this call attempts to make
   //   a connection to another socket.  The other socket is specified by
   //   name, which is an address in the communications space of the socket.
   //
   // If the socket is of type SOCK_DGRAM, then this call
   //   specifies the peer with which the socket is to be associated; this
   //   address is that to which datagrams are to be sent, and the only address
   //   from which datagrams are to be received.
   //
   // Each communications space interprets the name parameter in its own way.
   //   Generally, stream sockets may successfully connect only once; datagram
   //   sockets may use connect multiple times to change their association.
   //   Datagram sockets may dissolve the association by connecting to an
   //   invalid address, such as a zero-filled address.
   vpr::ReturnStatus SocketManager::connect( vpr::SocketImplSIM* localSock,
                                             const vpr::InetAddrSIM& remoteName,
                                             NetworkGraph::VertexListPtr& path,
                                             vpr::Interval timeout )
   {
      boost::ignore_unused_variable_warning(timeout);
      vprASSERT(mActive && "Socket manager not activated yet");
      vprASSERT(localSock->isBound() && "Precondition is that socket is bound");
      vprASSERT(localSock->mLocalAddr != vpr::InetAddr::AnyAddr && "Must be bound to an address");

      vpr::ReturnStatus status;

      vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
         <<"connect( "<<remoteName<<" )\n"<<vprDEBUG_FLUSH;

      vpr::SocketStreamImplSIM* local_stream_socket;
      local_stream_socket = dynamic_cast<vpr::SocketStreamImplSIM*>(localSock);

      // If we have a vpr::SocketStreamImplSIM* object, we perform a real
      // simulated connection.
      if ( local_stream_socket != NULL )
      {
         if ( isListening( remoteName ) )                // If remote side is accepting connections
         {
            vpr::SocketImplSIM* remote_sock;

            mListenerListMutex.acquire();
            {
               vprASSERT(mListenerList.find(remoteName) != mListenerList.end());
               remote_sock = (*mListenerList.find(remoteName)).second;
            }
            mListenerListMutex.release();

            vpr::SocketStreamImplSIM* remote_stream_socket
                  = dynamic_cast<vpr::SocketStreamImplSIM*>(remote_sock);
            vprASSERT(NULL != remote_stream_socket && "We should have a stream socket, but don't");

            // Queue us up in the listening socket's connection queue.
            status = remote_stream_socket->addConnector(localSock);
            vprASSERT(!status.failure() && "Failed to add connector");

            vpr::sim::Controller* controller = vpr::sim::Controller::instance();
            controller->addConnectionEvent(controller->getClock().getCurrentTime(), remote_sock);
         }
         else
         {
            vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
               << "vpr::sim::SocketManager: Cannot connect, no one listening on "
               << remoteName << std::endl << vprDEBUG_FLUSH;

            vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL) << "--------- Listening list ---------\n" << vprDEBUG_FLUSH;
            for(listener_map_t::iterator i=mListenerList.begin();
                i != mListenerList.end(); ++i)
            { vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL) << (*i).second->getLocalAddr() << std::endl << vprDEBUG_FLUSH; }
            vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL) << "---- [End] Listening list ---------\n" << vprDEBUG_FLUSH;

            vprASSERT(false && "Tried to connect to a non-listening node");
            status.setCode(vpr::ReturnStatus::Fail);
         }
      }
      // We are dealing with datagram sockets.
      else
      {
         vpr::SocketImplSIM* remote_sock;
         vpr::SocketDatagramImplSIM* remote_dgram_socket;

         mListenerListMutex.acquire();
         {
            vprASSERT(mListenerList.find(remoteName) != mListenerList.end());
            remote_sock = (*mListenerList.find(remoteName)).second;
         }
         mListenerListMutex.release();

         remote_dgram_socket = dynamic_cast<vpr::SocketDatagramImplSIM*>(remote_sock);

         // Ensure that we really did get a datagram socket.  Basically, the
         // only reason dgram_socket exists is for this conditional.
         vprASSERT( remote_dgram_socket != NULL );

         // We have to cast away the const-ness so that we can add the
         // connector information.
         localSock->completeConnection( remote_sock );
         // XXX: Don't tell the other side that it is connected to us
         //remote_dgram_socket->completeConnection(localSock);

         // Now find the shortest path between u and v.
         const NetworkGraph::net_vertex_t& u = localSock->getNetworkNode();
         const NetworkGraph::net_vertex_t& v = remote_dgram_socket->getNetworkNode();
         path = vpr::sim::Controller::instance()->getNetworkGraph().getShortestPath(u, v);
      }

      vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL) << "DONE: connect()\n"
                                             << vprDEBUG_FLUSH;
      return status;
   }


   /* Assign the socket to the network somwhere
   * Case localAddr = InetAddr::Any  ==> 0x7F000001 with random port
   * Case localAddr = Have IP and port=0 (ie. Give me a random port. I don't care)
   * Case localAddr = Have IP and port != 0 (bind me to that exact one)
   */
   vpr::ReturnStatus SocketManager::bind( vpr::SocketImplSIM* handle )
   {
      vprASSERT(handle->isOpen() && !handle->isBound() && "Against preconditions to have a handle not open or already bound");
      vprDEBUG_BEGIN(vprDBG_ALL, vprDBG_STATE_LVL) << "bind(" << handle << ")\n" << vprDEBUG_FLUSH;

      vpr::ReturnStatus ret_stat = assignToNode(handle);

      vprDEBUG_END(vprDBG_ALL, vprDBG_STATE_LVL) << "bind complete\n" << vprDEBUG_FLUSH;

      return ret_stat;
   }

   vpr::ReturnStatus SocketManager::unbind( vpr::SocketImplSIM* handle )
   {
      vpr::ReturnStatus status;

      vprASSERT( handle->isBound() && "Can't unbind and unbound handle");

      vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
           << "Unbinding handle (" << handle << " --> "
           << handle->getLocalAddr() << ")\n" << vprDEBUG_FLUSH;

      // -- Unassign from node
      status = unassignFromNode( handle );

      return status;
   }


   // Add socket to the listening list
   vpr::ReturnStatus SocketManager::listen( vpr::SocketStreamImplSIM* handle,
                                            const int backlog )
   {
      boost::ignore_unused_variable_warning(backlog);
      vpr::ReturnStatus status;

      vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL) <<"SocketManager::listen: Added listner: " << handle->getLocalAddr() << std::endl
                                             << vprDEBUG_FLUSH;
      vprASSERT( handle->isBound() && "Can't listen on unbound socket");
      vprASSERT( handle->getType() == vpr::SocketTypes::STREAM && "Trying to listen with not stream socket");

      // enter self into list for accepting connections.
      mListenerListMutex.acquire();
      {
         mListenerList[handle->getLocalAddr()] = handle;
      }
      mListenerListMutex.release();

      vprASSERT( isListening(handle->getLocalAddr()) && "Didn't add listener" );

      return status;
   }


   void SocketManager::findRoute (vpr::SocketImplSIM* src_sock,
                                  vpr::SocketImplSIM* dest_sock)
   {
      vprASSERT(mActive && "Socket manager not activated!");

      const NetworkGraph::net_vertex_t& u = src_sock->getNetworkNode();
      const NetworkGraph::net_vertex_t& v = dest_sock->getNetworkNode();
      NetworkGraph::VertexListPtr path;
      vpr::sim::NetworkGraph& net_graph =
         vpr::sim::Controller::instance()->getNetworkGraph();

      // Now find the shortest path between u and v.
      path = net_graph.getShortestPath(u, v);
      src_sock->setPathToPeer(path);
      dest_sock->setPathToPeer(net_graph.reversePath(path));
   }

   void SocketManager::sendMessage (vpr::sim::MessagePtr msg)
   {
      // There is no point in doing anything if we do not have a controller
      // for the simulation.
      vprASSERT(mActive && "Socket manager not activated yet");

      vprDEBUG(vprDBG_ALL, vprDBG_HVERB_LVL)
         << "SocketManager::sendMessage(): Sending message from "
         << msg->getSourceSocket()->getLocalAddr() << " to "
         << msg->getDestinationSocket()->getLocalAddr() << "\n"
         << vprDEBUG_FLUSH;

      NetworkGraph::net_vertex_t first_hop(msg->getNextHop());

      bool end_of_path;
      msg->incNextHop(end_of_path);

      vpr::sim::Controller* controller = vpr::sim::Controller::instance();

      if ( ! end_of_path )
      {
         NetworkGraph::net_vertex_t second_hop(msg->getNextHop());
         NetworkGraph::net_edge_t first_edge;
         bool found;
         vpr::sim::NetworkGraph& net_graph = controller->getNetworkGraph();

         boost::tie(first_edge, found) = net_graph.getEdge(first_hop, second_hop);

         if ( found )
         {
            NetworkLine::LineDirection dir;

            NetworkLine& first_edge_prop = net_graph.getLineProperty(first_edge);

            // Determine the direction for the message on this line.
            dir = net_graph.isSource(first_hop, first_edge) ? NetworkLine::FORWARD : NetworkLine::REVERSE;
            first_edge_prop.calculateMessageEventTimes(msg, vpr::Interval::now(),
                                                       dir);

            vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
               << "SocketManager::sendMessage(): Starting message ["
               << msg->getSourceSocket()->getLocalAddr() << " ==> "
               << msg->getDestinationSocket()->getLocalAddr() << "] on wire "
               << first_edge_prop.getNetworkAddressString() << ": ("
               << msg->whenStartOnWire().getBaseVal() << ", "
               << msg->whenFullyOnWire().getBaseVal() << ", "
               << msg->whenArrivesFully().getBaseVal() << ")\n"
               << vprDEBUG_FLUSH;

            first_edge_prop.addMessage(msg, dir);
            controller->addMessageEvent(msg->whenArrivesFully(), first_edge,
                                        dir);
         }
      }
      // This is a loopback, so we can just deliver the message without going
      // through the network.
      else
      {
         vprASSERT(msg->getSourceSocket()->getLocalAddr().getAddressValue() == msg->getDestinationSocket()->getLocalAddr().getAddressValue()
                   && "Could not get second node in message's path");
         vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)
            << "SocketManager::sendMessage(): Loopback--delivering to "
            << msg->getDestinationSocket()->getLocalAddr() << "("
            << msg->getDestinationSocket() << ") immediately\n"
            << vprDEBUG_FLUSH;
         msg->getDestinationSocket()->addArrivedMessage(msg);
         controller->addLocalhostDeliveryEvent(controller->getClock().getCurrentTime(),
                                               msg->getDestinationSocket());
      }
   }

   void SocketManager::sendMessageTo (vpr::sim::MessagePtr msg,
                                      const vpr::SocketImplSIM* src_sock,
                                      const vpr::InetAddrSIM& dest_addr)
   {
      vprASSERT(src_sock->getType() == vpr::SocketTypes::DATAGRAM && "Only datagram sockets may use sendMessageTo()");

      vpr::ReturnStatus status;
      NetworkGraph::net_vertex_t dest_node;
      vpr::sim::NetworkGraph& net_graph =
         vpr::sim::Controller::instance()->getNetworkGraph();

      status = net_graph.getNodeWithAddr(dest_addr.getAddressValue(), dest_node);

      if ( status.success() )
      {
         NetworkNodePtr dest_node_prop;

         dest_node_prop = net_graph.getNodeProperty(dest_node);

         if ( dest_node_prop->hasSocket(dest_addr.getPort(), vpr::SocketTypes::DATAGRAM) )
         {
            vpr::SocketImplSIM* peer;
            NetworkGraph::VertexListPtr path;

            peer = dest_node_prop->getSocket( dest_addr.getPort(), vpr::SocketTypes::DATAGRAM);

            vprASSERT(src_sock->isBound() && "Trying to send from socket not bound.");
            path = net_graph.getShortestPath(src_sock->getNetworkNode(), dest_node);
            msg->setPath(path, src_sock, peer);

            // At this point, the message is ready to go, so we can send it in
            // the same manner as a "reliable" message.
            sendMessage(msg);
         }
      }
   }


   // is there someone listening on the address?
   bool SocketManager::isListening( const vpr::InetAddrSIM& address )
   {
      bool status;

      mListenerListMutex.acquire();
      {
         status = (mListenerList.find( address) != mListenerList.end());
      }
      mListenerListMutex.release();

      return status;
   }



   // --------------------------------------------------------
   //             INTERNAL HELPERS
   // --------------------------------------------------------

   vpr::ReturnStatus SocketManager::assignToNode (vpr::SocketImplSIM* handle)
   {
      vpr::ReturnStatus status;
      NetworkGraph net_graph = Controller::instance()->getNetworkGraph();

      // --- Get the local address setup correctly --- //
      vpr::InetAddrSIM local_addr;
      local_addr = handle->getLocalAddr();

      // If any addr, then set it to local host
      // Case localAddr = InetAddr::Any  ==> "localhost" with random port
      if(local_addr == vpr::InetAddr::AnyAddr )
      {
         local_addr.setAddress(LocalHostIpAddrValue, 0);
      }

      // Make sure that we know about the node of the given address
      ensureNetworkNodeIsRegistered(local_addr.getAddressValue() );
      vprASSERT( mNetworkNodes.find(local_addr.getAddressValue()) != mNetworkNodes.end());

      // --- Bind the socket to an actual node in the graph -- //
      // - Get the node
      // - Register this socket with that node
      NetworkNodePtr net_node = mNetworkNodes[local_addr.getAddressValue()];

      // - Deal with port assignment issues
      // Case localAddr = Have IP and port=0 (ie. Give me a random port. I don't care)
      // Case localAddr = Have IP and port != 0 (bind me to that exact one)
      if(local_addr.getPort() == 0)
      {
         vpr::Uint32 port_num;
         if(handle->getType() == vpr::SocketTypes::STREAM)
         {  port_num = net_node->getUnassignedTcpPortNumber(); }
         else
         {  port_num = net_node->getUnassignedUdpPortNumber(); }
         local_addr.setPort(port_num);
      }

      // Set the final local address and add the socket to the node
      handle->setLocalAddr(local_addr);
      net_node->addSocket( handle );

      // Store the network node vertex in the socket for fast access
      NetworkGraph::net_vertex_t node_vertex;
      status = net_graph.getNodeWithAddr(local_addr.getAddressValue(), node_vertex);
      vprASSERT(status.success());
      handle->setNetworkNode( node_vertex );

      vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
         << "SocketManager::assignToNode(): Assigned node:  local_addr:" << local_addr
         << "   type: " << ((handle->getType()==vpr::SocketTypes::STREAM) ? "STREAM" : "DATAGRAM")
         << "  --> node: " << net_node->getIpAddressString() << std::endl <<  vprDEBUG_FLUSH;

      return status;
   }

   vpr::ReturnStatus SocketManager::unassignFromNode (vpr::SocketImplSIM* handle)
   {
      vpr::ReturnStatus status;
      vpr::InetAddrSIM local_addr = handle->getLocalAddr();
      vprASSERT( mNetworkNodes.find(local_addr.getAddressValue()) != mNetworkNodes.end());

      // Get the network node
      NetworkNodePtr net_node = mNetworkNodes[local_addr.getAddressValue()];

      // Remove the socket from the node
      status = net_node->removeSocket( handle );
      vprASSERT(status.success());

      return status;
   }

vpr::ReturnStatus SocketManager::ensureNetworkNodeIsRegistered(const vpr::Uint32& ipAddr)
{
   vpr::ReturnStatus ret_stat(vpr::ReturnStatus::Succeed);

   if( mNetworkNodes.find(ipAddr) == mNetworkNodes.end())    // If not registered yet
   {
      NetworkGraph::net_vertex_t node_vertex;
      NetworkGraph net_graph = Controller::instance()->getNetworkGraph();

      ret_stat = net_graph.getNodeWithAddr(ipAddr, node_vertex);
      vprASSERT(ret_stat.success() && "Specified IP address does not actually exist in the sim net graph.");

      NetworkNodePtr node_prop = net_graph.getNodeProperty(node_vertex);
      vprASSERT(node_prop.get() != NULL);

      mNetworkNodes.insert( std::pair<node_map_t::key_type, node_map_t::data_type>(ipAddr, node_prop) );
   }

   return ret_stat;
}


} // End of sim namespace

} // End of vpr namespace

