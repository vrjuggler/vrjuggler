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

#ifndef _VPR_SIM_SOCKET_MANAGER_H_
#define _VPR_SIM_SOCKET_MANAGER_H_

#include <vpr/vprConfig.h>

#include <list>

#ifdef VPR_HASH_MAP_INCLUDE
#  include VPR_HASH_MAP_INCLUDE
#else
#  include <map>
#endif

#include <string>
#include <utility>

#include <vpr/vprTypes.h>
#include <vpr/IO/Socket/SocketTypes.h>
#include <vpr/IO/Socket/InetAddr.h>
#include <vpr/Sync/Mutex.h>

#include <vpr/md/SIM/IO/Socket/InetAddrSIM.h> /* SIM InetAddr implementation */
#include <vpr/md/SIM/Network/NetworkGraph.h>
#include <vpr/md/SIM/Network/MessagePtr.h>


namespace vpr
{

class Interval;
class SocketImplSIM;
class SocketStreamImplSIM;

namespace sim
{

class Controller;

/** \class SocketManager SocketManager.h vpr/md/SIM/SocketManager.h
 *
 * Abstraction for C-style sockets.  This is used by Plexus sim sockets, and
 * in the overall sim socket scheme, this sits at the operating system level.
 * This roughly corresponds to the data link or transport layer.
 *
 * It provides a way to bind address and send messages.
 */
class VPR_CLASS_API SocketManager
{
public:
   SocketManager() : mActive(false)
   {
      /* Do nothing. */ ;
   }

   void setActive()
   {
      mActive = true;
   }

   // ===========
   // connector:
   // ===========

   /**
    * If the socket is of type SOCK_STREAM, then this call attempts to make
    * a connection to another socket.  The other socket is specified by
    * name, which is an address in the communications space of the socket.
    *
    * If the socket is of type SOCK_DGRAM, then this call
    * specifies the peer with which the socket is to be associated; this
    * address is that to which datagrams are to be sent, and the only address
    * from which datagrams are to be received.
    *
    * Each communications space interprets the name parameter in its own way.
    * Generally, stream sockets may successfully connect only once; datagram
    * sockets may use connect multiple times to change their association.
    * Datagram sockets may dissolve the association by connecting to an
    * invalid address, such as a zero-filled address.
    */
   vpr::ReturnStatus connect(vpr::SocketImplSIM* localSock,
                             const vpr::InetAddrSIM& remoteName,
                             NetworkGraph::VertexListPtr& path,
                             vpr::Interval timeout);

   // ================
   // acceptor funcs:
   // ================

   /**
    * Bind ties the given adddress to the given socket.  In reality, this
    * registers the given socket object with the simulator and makes it
    * possible to look up the socket by its address.
    *
    * Assign the socket to the network somwhere
    * Case localAddr = InetAddr::Any  ==> 0x7F000001 with random port
    * Case localAddr = Have IP and port=0 (ie. Give me a random port. I don't care)
    * Case localAddr = Have IP and port != 0 (bind me to that exact one)
    *
    * @post If the binding process succeeds, the given socket is assigned a
    *       node in the network graph.
    *
    * @param handle    The socket object to be bound (this must have it's local addr assigned already)
    *
    * @return vpr::ReturnStatus::Success is returned if the handle could be
    *         bound to the given address.  vpr::ReturnStatus::Failure is
    *         returned otherwise.
    */
   vpr::ReturnStatus bind(vpr::SocketImplSIM* handle);

   /**
    * Unbinds the given socket from its registered address.  Once the socket
    * is unbound, it should close.  For sockets that are not bound, no action
    * is taken.
    *
    * @post The bound address is released so another socket may bind to it.
    *
    * @param handle The socket object to be unbound from its registered address.
    *
    * @return vpr::ReturnStatus::Success is returned if the handle could be
    *         unbound from its local address.  vpr::ReturnStatus::Failure is
    *         returned otherwise.
    */
   vpr::ReturnStatus unbind(vpr::SocketImplSIM* handle);

   /**
    * Sets a socket s to listen for connections.
    *
    * @pre The listen call applies only to sockets of type SOCK_STREAM.
    *
    * @return vpr::ReturnStatus::Success for success, vpr::ReturnStatus::Fail
    *         for error.
    */
   vpr::ReturnStatus listen(vpr::SocketStreamImplSIM* handle,
                            const int backlog = 5);

// extra stuff...
public:
   void findRoute(vpr::SocketImplSIM* src_sock, vpr::SocketImplSIM* dest_sock);

   /**
    * Puts the given message on its way to its destination.  This call should
    * be used for "reliable" messages or messages that know where they are
    * going.
    */
   void sendMessage(vpr::sim::MessagePtr msg);

   /**
    * Defines the path from the source socket to the destination address for
    * the given message.  This call should be used for "unreliable" messages
    * only.  For example, the given destination address may be unreachable,
    * and thus the message will not be delivered.
    */
   void sendMessageTo(vpr::sim::MessagePtr msg,
                      const vpr::SocketImplSIM* src_sock,
                      const vpr::InetAddrSIM& dest_addr);

   /**
    * Is there someone listening on the address?
    */
   bool isListening(const vpr::InetAddrSIM& address);

protected:  // -- Internal helpers -- //
  /** Assign the given socket to a node on the actual network
   * This is based on the following address scheme for assignment
   *
   * Assign the socket to the network somwhere
   * Case localAddr = InetAddr::Any  ==> 0x7F000001 with random port
   * Case localAddr = Have IP and port=0 (ie. Give me a random port. I don't care)
   * Case localAddr = Have IP and port != 0 (bind me to that exact one)
   */
   vpr::ReturnStatus assignToNode(vpr::SocketImplSIM* handle);

   /**
    * Unassigns the socket from the node that it is bound to.
    * This is basically the opposite of assignToNode (and unbind if you will).
    */
   vpr::ReturnStatus unassignFromNode(vpr::SocketImplSIM* handle);

   /**
    * Make sure that the local data structures know about the node at the given
    * address.
    *
    * @pre \p ipAddr is a valid address in the network.
    * @return vpr::ReturnStatus::Succeed is returned on successful completion.
    */
   vpr::ReturnStatus ensureNetworkNodeIsRegistered(const vpr::Uint32& ipAddr);


   //vpr::sim::NetworkGraph::net_vertex_t getLocalhostVertex(void);

   /** Get the ip address value for the "localhost" node that we are using */
   //vpr::Uint32 getLocalhostIpAddrValue();

protected:
   // These two have to be here because Visual C++ will try to make them
   // exported public symbols.  This causes problems because copying vpr::Mutex
   // objects is not allowed.  We do not want to copy SocketManager instances
   // anyway, so this should be fine.
   SocketManager (const SocketManager& o) {;}
   void operator= (const SocketManager& o) {;}

   bool mActive;

#ifdef VPR_HASH_MAP_INCLUDE
   typedef std::hash_map<vpr::InetAddr, vpr::SocketStreamImplSIM*, vpr::InetAddr::hash> listener_map_t;
   typedef std::hash_map<vpr::Uint32, vpr::sim::NetworkNodePtr> node_map_t;
#else
   typedef std::map<vpr::InetAddr, vpr::SocketStreamImplSIM* > listener_map_t;
   typedef std::map<vpr::Uint32, vpr::sim::NetworkNodePtr> node_map_t;
#endif

   /**
    * This is a list of sockets that have gone into a listening state.
    * Used to track which sockets are currently in a listening state.
    * XXX: Could move this into the actually socket as a socket state instead.
    */
   listener_map_t mListenerList;
   vpr::Mutex mListenerListMutex;


   /**
    * List of the nodes in the system indexed by IP address
    *
    * @note Local host will have two mappings, one for the "localhost" address
    *       and one for the node's real address.
    */
   node_map_t     mNetworkNodes;
   vpr::Mutex     mNetworkNodesMutex;
};

} // namespace sim

} // namespace vpr


#endif
