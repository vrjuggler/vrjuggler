/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ****************** <VPR heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
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
#include <boost/utility.hpp>
#include <vpr/vpr.h>
#include <vpr/System.h>
#include <vpr/Util/Debug.h>
#include <vpr/Util/Assert.h>
#include <vpr/Sync/Guard.h>

#include <vpr/md/SIM/Controller.h>
#include <vpr/md/SIM/SocketManager.h>
#include <vpr/md/SIM/IO/Socket/SocketImplSIM.h>
#include <vpr/md/SIM/IO/Socket/SocketStreamImplSIM.h>
#include <vpr/md/SIM/IO/Socket/SocketDatagramImplSIM.h>
#include <vpr/md/SIM/Network/NetworkGraph.h>


namespace vpr
{

namespace sim
{
   bool SocketManager::hasActiveSockets ()
   {
      bool status;

      mBindListSockMutexTCP.acquire();
      {
         status = (! mBindListSockTCP.empty());
      }
      mBindListSockMutexTCP.release();

      if ( ! status )
      {
         mBindListSockMutexUDP.acquire();
         {
            status = (! mBindListSockUDP.empty());
         }
         mBindListSockMutexUDP.release();
      }

      return status;
   }

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
   vpr::ReturnStatus SocketManager::connect( const vpr::SocketImplSIM* local_socket,
                                             vpr::SocketImplSIM** remote_socket,
                                             const vpr::InetAddrSIM& remoteName,
                                             NetworkGraph::VertexListPtr& path,
                                             vpr::Interval timeout )
   {
      vprASSERT(mActive && "Socket manager not activated yet");

      vpr::ReturnStatus status;

      vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
         <<"connect( "<<remoteName<<" )\n"<<vprDEBUG_FLUSH;

      const vpr::SocketStreamImplSIM* stream_socket;
      stream_socket = dynamic_cast<const vpr::SocketStreamImplSIM*>(local_socket);

      // If we have a vpr::SocketStreamImplSIM* object, we perform a real
      // simulated connection.
      if ( stream_socket != NULL )
      {
         if ( isListening( remoteName ) )
         {
            const vpr::SocketImplSIM* sim_sock;

            mBindListAddrMutexTCP.acquire();
            {
               sim_sock = mBindListAddrTCP[remoteName];
            }
            mBindListAddrMutexTCP.release();

            stream_socket = dynamic_cast<const vpr::SocketStreamImplSIM*>(sim_sock);

            // Queue us up in the listening socket's connection queue.
            if ( stream_socket != NULL && isListening(stream_socket) )
            {
               vpr::SocketImplSIM* sock;

               // We have to cast away the const-ness so that we can add the
               // connector information.
               sock = const_cast<vpr::SocketImplSIM*>(sim_sock);

               status = sock->addConnector(local_socket, remote_socket);
               vprASSERT(! status.failure() && "Failed to add connector");

               // The local socket needs an address now if it does not already
               // have one.
               if ( vpr::InetAddr::AnyAddr == local_socket->mLocalAddr )
               {
                  vpr::SocketImplSIM* temp_sock;

                  temp_sock = const_cast<vpr::SocketImplSIM*>(local_socket);
//                  assignToNode(temp_sock);
                  bindUnusedPort(temp_sock, temp_sock->mLocalAddr);
               }
            }
            else
            {
               vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
                  << "vpr::sim::SocketManager: Cannot connect, no one listening on "
                  << remoteName << std::endl << vprDEBUG_FLUSH;
               status.setCode(vpr::ReturnStatus::Fail);
            }
         }
         else
         {
            vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
               << "vpr::sim::SocketManager: Cannot connect, no one listening on "
               << remoteName << std::endl << vprDEBUG_FLUSH;
            status.setCode(vpr::ReturnStatus::Fail);
         }
      }
      // We are dealing with datagram sockets.
      else
      {
         const vpr::SocketImplSIM* sim_sock;
         const vpr::SocketDatagramImplSIM* dgram_socket;

         mBindListAddrMutexUDP.acquire();
         {
            sim_sock = mBindListAddrUDP[remoteName];
         }
         mBindListAddrMutexUDP.release();

         dgram_socket = dynamic_cast<const vpr::SocketDatagramImplSIM*>(sim_sock);

         // Ensure that we really did get a datagram socket.  Basically, the
         // only reason dgram_socket exists is for this conditional.
         if ( dgram_socket != NULL )
         {
            vpr::SocketImplSIM *mod_local_sock, *mod_remote_sock;

            // We have to cast away the const-ness so that we can add the
            // connector information.
            mod_local_sock        = const_cast<vpr::SocketImplSIM*>(local_socket);
            mod_remote_sock       = const_cast<vpr::SocketImplSIM*>(sim_sock);
            mod_local_sock->mPeer = mod_remote_sock;
            *remote_socket        = mod_remote_sock;

            // Now find the shortest path between u and v.
            const NetworkGraph::net_vertex_t& u = local_socket->getNetworkNode();
            const NetworkGraph::net_vertex_t& v = (*remote_socket)->getNetworkNode();
            path = vpr::sim::Controller::instance()->getNetworkGraph().getShortestPath(u, v);

            // The local socket needs an address now if it does not already
            // have one.
            if ( vpr::InetAddr::AnyAddr == local_socket->mLocalAddr )
            {
               vpr::SocketImplSIM* temp_sock;

               temp_sock = const_cast<vpr::SocketImplSIM*>(local_socket);
               bindUnusedPort(temp_sock, temp_sock->mLocalAddr);
            }
         }
         else
         {
            vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
               << "vpr::sim::SocketManager: Cannot set " << remoteName
               << " to default destination\n" << vprDEBUG_FLUSH;
            status.setCode(vpr::ReturnStatus::Fail);
         }
      }

      vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL) << "DONE: connect()\n"
                                             << vprDEBUG_FLUSH;

      return status;
   }

   // Bind assigns a name to an unnamed socket.  When a socket is created with
   //  open() it exists in a name space (address family) but has no name
   //  assigned.  Bind requests that name be assigned to the socket.
   //
   // NOTE: bind is mostly useful when you are accepting connections (listen())
   vpr::ReturnStatus SocketManager::bind( const vpr::SocketImplSIM* handle,
                                          const vpr::InetAddrSIM& localName )
   {
      vpr::ReturnStatus status;

      vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL) << "bind(" << handle << ", "
                                             << localName << ")\n"
                                             << vprDEBUG_FLUSH;

      if (handle->isOpen() &&
          isBound( handle ) == false &&
          isBound(localName, handle->getType()) == false)
      {
         _bind( handle, localName );
         vprASSERT( isBound( handle ) && "_bind failed on handle." );
         vprASSERT(isBound(localName, handle->getType()) && "_bind failed on address.");
      }
      else
      {
         if (handle->isOpen() == false)
         {
            vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
               << "vpr::sim::SocketManager: handle is not open, call open() before bind()\n"
               << vprDEBUG_FLUSH;
         }
         else if (isBound( localName, handle->getType() ) == true &&
                  isBound( handle ) == true)
         {
            vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
               << "vpr::sim::SocketManager: handle(" << handle << ") and address("
               << localName << ") already bound\n"
               << vprDEBUG_FLUSH;
         }
         else if (isBound( handle ) == true)
         {
            vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
               << "vpr::sim::SocketManager: specified handle already bound to an address\n"
               << vprDEBUG_FLUSH;
         }
         else if (isBound( localName, handle->getType() ) == true)
         {
            vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
               << "vpr::sim::SocketManager: Address " << localName
               << " is already bound by socket "
               << (handle->getType() == vpr::SocketTypes::STREAM ? mBindListAddrTCP[localName]
                                                                 : mBindListAddrUDP[localName])
               << " (couldn't bind sock " << handle << ")\n"
               << vprDEBUG_FLUSH;
         }
         else
         {
            vprASSERT( false && "unexpected error. deal with it." );
         }

         status.setCode(vpr::ReturnStatus::Fail);
      }

      return status;
   }

   vpr::ReturnStatus SocketManager::unbind( const vpr::SocketImplSIM* handle )
   {
      vpr::ReturnStatus status;

      if ( isBound(handle) )
      {
         vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
            << "Unbinding handle(" << handle << ")\n" << vprDEBUG_FLUSH;

         status = _unbind(handle);
         vprASSERT((! isBound(handle) && status.success()) && "_unbind failed");

         vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
            << "Unbind for handle(" << handle << ") done\n" << vprDEBUG_FLUSH;
     }

      return status;
   }

   // set a socket s to listen for connections
   // a willingness to accept incoming connections and a queue limit for incoming
   //  connections are specified with listen(2), and then each new connection is
   //  accepted with accept(2).  The listen call applies only to sockets of type
   //  SOCK_STREAM.
   // returns true for success
   // returns false for error
   vpr::ReturnStatus SocketManager::listen( const vpr::SocketStreamImplSIM* handle,
                                         const int backlog )
   {
      vpr::ReturnStatus status;

      vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL) <<"listen( "<<handle<<" )\n"
                                             <<vprDEBUG_FLUSH;
      if (isBound( handle ))
      {
         // enter self into list for accepting connections.
         mListenerListMutex.acquire();
         {
            mBindListSockMutexTCP.acquire();
            {
               mListenerList[mBindListSockTCP[handle]] =
                  std::pair<const vpr::SocketStreamImplSIM*, int>(handle,
                                                                  backlog);
            }
            mBindListSockMutexTCP.release();
         }
         mListenerListMutex.release();
      }
      else
      {
         vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
            << "vpr::sim::SocketManager: specified handle is not bound to an address\n"
            << vprDEBUG_FLUSH;

         status.setCode(vpr::ReturnStatus::Fail);
      }

      return status;
   }

   vpr::ReturnStatus SocketManager::assignToNode (vpr::SocketImplSIM* handle)
   {
      timeval cur_time;
      vpr::System::gettimeofday(&cur_time);
      srand(cur_time.tv_usec);
      vpr::ReturnStatus status;
      bool attached = false;

      vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
         << "SocketManager::assignToNode(): Looking up a random node for "
         << handle << "\n" << vprDEBUG_FLUSH;

      vpr::sim::NetworkGraph& net_graph =
         vpr::sim::Controller::instance()->getNetworkGraph();

      vpr::Uint32 node_count = net_graph.getNodeCount();
      vprASSERT(node_count > 0 && "No nodes in the network!");

      // Ensure that we get a network node that is not already attached to a
      // sim socket.
      while ( ! attached )
      {
         // Generate a random number to use for the network node to which the
         // socket pointed to by handle will be attached.
         int n = rand() % node_count;

         // Assign a randomly chosen node to the socket that has just been
         // bound to an address.
         // XXX: This is not 100% correct.  Ideally, I think it would be best
         // if the address to which the socket is bound corresponded to the
         // node in the graph.  This could lead to better grouping of nodes as
         // would occur on a real network.
         NetworkGraph::net_vertex_t node = net_graph.getNode(n);
         NetworkNode node_prop = net_graph.getNodeProperty(node);

         if ( ! node_prop.hasSocket(handle->getLocalAddr().getPort(),
                                    handle->getType()) )
         {
            vpr::InetAddrSIM new_addr;

            vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
               << "SocketManager::assignToNode(): Using node "
               << node_prop.getIpAddressString() << "\n" << vprDEBUG_FLUSH;
            handle->setNetworkNode(node);

            // Set the socket's IP address to match its node.
            new_addr.setAddress(node_prop.getIpAddress(),
                                handle->getLocalAddr().getPort());
            status = handle->setLocalAddr(new_addr);
            vprASSERT(status.success() && "Failed to assign new address to socket");
            node_prop.addSocket(handle);

            // Ensure that the property associated with node n is updated
            // properly.  What a pain.
            net_graph.setNodeProperty(node, node_prop);

            attached = true;
         }
      }

      return status;
   }

   vpr::ReturnStatus SocketManager::assignToNode (vpr::SocketImplSIM* handle,
                                                  const vpr::InetAddrSIM& addr)
   {
      vpr::ReturnStatus status;
      NetworkGraph::net_vertex_t node;
      vpr::sim::NetworkGraph& net_graph =
         vpr::sim::Controller::instance()->getNetworkGraph();

      // 0x7F000001 is the integer value for 127.0.0.1.
      if ( 0x7F000001 == addr.getAddressValue() )
      {
         node = getLocalhost();
      }
      else
      {
         vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
            << "SocketManager::assignToNode(): Trying to find node with address "
            << addr << "\n" << vprDEBUG_FLUSH;
         status = net_graph.getNodeWithAddr(addr.getAddressValue(), node);
      }

      if ( status.success() )
      {
         vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)
            << "SocketManager::assignToNode(): Found node!\n" << vprDEBUG_FLUSH;
         NetworkNode node_prop = net_graph.getNodeProperty(node);
         handle->setNetworkNode(node);
         node_prop.addSocket(handle);
         net_graph.setNodeProperty(node, node_prop);
      }
      // Fall back on a randomly assigned node.
      else
      {
         vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)
            << "SocketManager::assignToNode(): Falling back on random lookup\n"
            << vprDEBUG_FLUSH;
         status = assignToNode(handle);
      }

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

      if ( ! end_of_path )
      {
         NetworkGraph::net_vertex_t second_hop(msg->getNextHop());
         NetworkGraph::net_edge_t first_edge;
         bool found;
         vpr::sim::Controller* controller = vpr::sim::Controller::instance();
         vpr::sim::NetworkGraph& net_graph = controller->getNetworkGraph();

         boost::tie(first_edge, found) = net_graph.getEdge(first_hop, second_hop);

         if ( found )
         {
            NetworkLine first_edge_prop;
            NetworkLine::LineDirection dir;

            first_edge_prop = net_graph.getLineProperty(first_edge);

            // Determine the direction for the message on this line.
            dir = net_graph.isSource(first_hop, first_edge) ? NetworkLine::FORWARD : NetworkLine::REVERSE;
            first_edge_prop.calculateMessageEventTimes(msg, vpr::Interval::now(),
                                                       dir);

            vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
               << "SocketManager::sendMessage(): Starting message on wire "
               << first_edge_prop.getNetworkAddressString() << ": ("
               << msg->whenStartOnWire().getBaseVal() << ", "
               << msg->whenFullyOnWire().getBaseVal() << ", "
               << msg->whenArrivesFully().getBaseVal() << ")\n"
               << vprDEBUG_FLUSH;

            first_edge_prop.addMessage(msg, dir);
            net_graph.setLineProperty(first_edge, first_edge_prop);
            controller->addEvent(msg->whenArrivesFully(), first_edge, dir);
         }
      }
      // This is a loopback, so we can just deliver the message without going
      // through the network.
      else
      {
         vprASSERT(msg->getSourceSocket()->getLocalAddr().getAddressValue() == msg->getDestinationSocket()->getLocalAddr().getAddressValue() && "Could not get second node in message's path");
         vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)
            << "SocketManager::sendMessage(): Loopback--delivering to "
            << msg->getDestinationSocket()->getLocalAddr() << "("
            << msg->getDestinationSocket() << ") immediately\n"
            << vprDEBUG_FLUSH;
         msg->getDestinationSocket()->addArrivedMessage(msg);
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

      if ( 0x7F000001 == dest_addr.getAddressValue() )
      {
         dest_node = getLocalhost();
      }
      else
      {
         status = net_graph.getNodeWithAddr(dest_addr.getAddressValue(), dest_node);
      }

      if ( status.success() )
      {
         NetworkNode dest_node_prop;

         dest_node_prop = net_graph.getNodeProperty(dest_node);

         if ( dest_node_prop.hasSocket(dest_addr.getPort(), vpr::SocketTypes::DATAGRAM) )
         {
            vpr::SocketImplSIM* peer;
            NetworkGraph::VertexListPtr path;

            mBindListAddrMutexUDP.acquire();
            {
               peer =
                  const_cast<vpr::SocketImplSIM*>(mBindListAddrUDP[dest_addr]);
            }
            mBindListAddrMutexUDP.release();

            vprASSERT(src_sock->mNodeAssigned && "Trying to send from socket not assigned to a node!");
            path = net_graph.getShortestPath(src_sock->getNetworkNode(), dest_node);
            msg->setPath(path, src_sock, peer);

            // At this point, the message is ready to go, so we can send it in
            // the same manner as a "reliable" message.
            sendMessage(msg);
         }
      }
   }

   // is the socket bound to an address?
   bool SocketManager::isBound( const vpr::SocketImplSIM* handle )
   {
      bool status;

      switch (handle->getType())
      {
         case vpr::SocketTypes::DATAGRAM:
            mBindListSockMutexUDP.acquire();
            {
               status = mBindListSockUDP.count(handle) > 0;
            }
            mBindListSockMutexUDP.release();
            break;
         case vpr::SocketTypes::STREAM:
            mBindListSockMutexTCP.acquire();
            {
               status = mBindListSockTCP.count(handle) > 0;
            }
            mBindListSockMutexTCP.release();
            break;
      }

      return handle->isOpen() && status;
   }

   // is the address bound to a socket?
   bool SocketManager::isBound( const vpr::InetAddrSIM& addr,
                                const vpr::SocketTypes::Type addr_type )
   {
      bool status = false;

      switch (addr_type)
      {
         case vpr::SocketTypes::DATAGRAM:
            mBindListAddrMutexUDP.acquire();
            {
               status = mBindListAddrUDP.count(addr) > 0;
            }
            mBindListAddrMutexUDP.release();
            break;
         case vpr::SocketTypes::STREAM:
            mBindListAddrMutexTCP.acquire();
            {
               status = mBindListAddrTCP.count(addr) > 0;
            }
            mBindListAddrMutexTCP.release();
            break;
      }

      return status;
   }

   // is there someone listening on the address?
   bool SocketManager::isListening( const vpr::InetAddrSIM& address )
   {
      bool status;

      mListenerListMutex.acquire();
      {
         status = mListenerList.count( address ) > 0;
      }
      mListenerListMutex.release();

      return status;
   }

   // is this socket in a listening state?
   bool SocketManager::isListening( const vpr::SocketStreamImplSIM* handle )
   {
      bool status = false;

      if (isBound( handle ))
      {
         mListenerListMutex.acquire();
         {
            mBindListSockMutexTCP.acquire();
            {
               status = mListenerList.count(mBindListSockTCP[handle]) > 0;
            }
            mBindListSockMutexTCP.release();
         }
         mListenerListMutex.release();
      }

      return status;
   }

   vpr::InetAddrSIM SocketManager::getBoundAddress( const vpr::SocketImplSIM* handle )
   {
      vpr::InetAddrSIM addr;

      vprASSERT( isBound( handle ) && "need to check isBound before using" );

      switch (handle->getType())
      {
         case vpr::SocketTypes::DATAGRAM:
            mBindListSockMutexUDP.acquire();
            {
               vprASSERT( mBindListSockUDP.count( handle ) > 0 && "if handle is bound, then it should appear in the bindlist, but it doesn't" );
               addr = mBindListSockUDP[handle];
            }
            mBindListSockMutexUDP.release();
            break;
         case vpr::SocketTypes::STREAM:
            mBindListSockMutexTCP.acquire();
            {
               vprASSERT( mBindListSockTCP.count( handle ) > 0 && "if handle is bound, then it should appear in the bindlist, but it doesn't" );
               addr = mBindListSockTCP[handle];
            }
            mBindListSockMutexTCP.release();
            break;
      }

      return addr;
   }

   vpr::ReturnStatus SocketManager::bindUnusedPort( vpr::SocketImplSIM* sock,
                                                    vpr::InetAddrSIM& addr )
   {
      vpr::Guard<vpr::Mutex> guard(mPortMutex);

      addr.setPort(genUnusedPort(sock->getType()));

      return bind(sock, addr);
   }

   void SocketManager::_bind( const vpr::SocketImplSIM* handle,
                              const vpr::InetAddrSIM& addr )
   {
      switch (handle->getType())
      {
         case vpr::SocketTypes::DATAGRAM:
            mBindListSockMutexUDP.acquire();
            {
               mBindListSockUDP[handle] = addr;
            }
            mBindListSockMutexUDP.release();

            mBindListAddrMutexUDP.acquire();
            {
               mBindListAddrUDP[addr] = handle;
            }
            mBindListAddrMutexUDP.release();

            break;
         case vpr::SocketTypes::STREAM:
            mBindListSockMutexTCP.acquire();
            {
               mBindListSockTCP[handle] = addr;
            }
            mBindListSockMutexTCP.release();

            mBindListAddrMutexTCP.acquire();
            {
               mBindListAddrTCP[addr] = handle;
            }
            mBindListAddrMutexTCP.release();

            break;
      }
      vprASSERT( isBound( handle ) == true );
      vprASSERT( isBound( addr, handle->getType() ) == true );

   }

   vpr::ReturnStatus SocketManager::_unbind( const vpr::SocketImplSIM* handle )
   {
      vpr::InetAddrSIM addr;
      const vpr::SocketImplSIM* hand;
      vpr::ReturnStatus status;

      vprASSERT( isBound( handle ) == true );

      switch (handle->getType())
      {
         case vpr::SocketTypes::DATAGRAM:
            mBindListSockMutexUDP.acquire();
            {
               vprASSERT( isBound( mBindListSockUDP[handle], handle->getType() ) == true );

               addr = mBindListSockUDP[handle];
               hand = handle;                // Temporary storage ...

               vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
                  << "_unbind() unmapping: " << mBindListSockUDP[handle] << " "
                  << handle << "\n" << vprDEBUG_FLUSH;

               mBindListAddrMutexUDP.acquire();
               {
                  mBindListAddrUDP.erase(mBindListSockUDP[handle]);
               }
               mBindListAddrMutexUDP.release();

               mBindListSockUDP.erase( handle );
            }
            mBindListSockMutexUDP.release();
            break;
         case vpr::SocketTypes::STREAM:
            mBindListSockMutexTCP.acquire();
            {
               vprASSERT( isBound( mBindListSockTCP[handle], handle->getType() ) == true );

               addr = mBindListSockTCP[handle];
               hand = handle;                // Temporary storage ...

               vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
                  << "_unbind() unmapping: " << mBindListSockTCP[handle] << " "
                  << handle << "\n" << vprDEBUG_FLUSH;

               mBindListAddrMutexTCP.acquire();
               {
                  mBindListAddrTCP.erase(mBindListSockTCP[handle]);
               }
               mBindListAddrMutexTCP.release();

               mBindListSockTCP.erase( handle );
            }
            mBindListSockMutexTCP.release();
            break;
      }

      vpr::sim::NetworkGraph& net_graph =
         vpr::sim::Controller::instance()->getNetworkGraph();
      NetworkNode node_prop = net_graph.getNodeProperty(handle->getNetworkNode());

      status = node_prop.removeSocket(handle);
      vprASSERT(status.success() && "Tried to remove a socket from the wrong node");

      net_graph.setNodeProperty(handle->getNetworkNode(), node_prop);

      vprASSERT( isBound( hand ) == false );
      vprASSERT( isBound( addr, hand->getType() ) == false );

      return status;
   }

   vpr::sim::NetworkGraph::net_vertex_t SocketManager::getLocalhost ()
   {
      vpr::sim::NetworkGraph& net_graph =
         vpr::sim::Controller::instance()->getNetworkGraph();
      vpr::Uint32 last_node = net_graph.getNodeCount() - 1;
      vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
         << "SocketManager::getLocalhost(): Using last node (#" << last_node
         << ") for localhost\n" << vprDEBUG_FLUSH;
      return net_graph.getNode(last_node);
   }

   // XXX: This needs to be rethough since we can now have 2^64 port numbers
   // (2^32 at each node, and 2^32 nodes).
   // return an unused port at some existing address, or 0 for error.
   vpr::Uint32 SocketManager::genUnusedPort (const vpr::SocketTypes::Type addr_type)
   {
      //vpr::Uint32 full = -1;
      vpr::Uint32 full = 0xFFFF;    // Max 32 bit value
      vpr::InetAddrSIM address;

      switch (addr_type)
      {
         case vpr::SocketTypes::DATAGRAM:
            {
               // This guard must be used because return is called from within
               // the for loop.
               vpr::Guard<vpr::Mutex> guard(mBindListAddrMutexUDP);

               // NOTE: this is pretty dumb, but it works.
               for ( vpr::Uint32 x = 1; x < full; ++x)
               {
                  address.setPort(x);

                  if ( mBindListAddrUDP.count(address) == 0 )
                  {
                     return x;
                  }
               }
            }
            vprASSERT( false && "should never fail" );
            break;
         case vpr::SocketTypes::STREAM:
            {
               // This guard must be used because return is called from within
               // the for loop.
               vpr::Guard<vpr::Mutex> guard(mBindListAddrMutexTCP);

               // NOTE: this is pretty dumb, but it works.
               for ( vpr::Uint32 x = 1; x < full; ++x)
               {
                  address.setPort(x);

                  if ( mBindListAddrTCP.count(address) == 0 )
                  {
                     return x;
                  }
               }
            }
            vprASSERT( false && "should never fail" );
            break;
      }
      return 0; //error
   }

} // End of sim namespace

} // End of vpr namespace
