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

#ifndef _VPR_SIM_SOCKET_MANAGER_H_
#define _VPR_SIM_SOCKET_MANAGER_H_

#include <vpr/vprConfig.h>

#include <list>
#include <map>
#include <string>
#include <iostream>
#include <pair.h>

#include <vpr/vprTypes.h>
#include <vpr/IO/Socket/SocketTypes.h>
#include <vpr/Sync/Mutex.h>

#include <vpr/md/SIM/IO/Socket/InetAddrSIM.h> /* Plexus InetAddr implementation */
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

/**
 * Abstraction for C-style sockets.  This is used by Plexus sim sockets, and
 * in the overall sim socket scheme, this sits at the operating system level.
 * This roughly corresponds to the data link or transport layer.
 *
 * It provides a way to bind address and send messages.
 */
class VPR_CLASS_API SocketManager
{
public:
   SocketManager (void) : mActive(false)
   {
      /* Do nothing. */ ;
   }

   void setActive (void)
   {
      mActive = true;
   }

   /**
    * Queries if there are currently any active sockets registered with this
    * manager.
    */
   bool hasActiveSockets(void);

   // ===========
   // connector:
   // ===========

   /**
    * If the socket is of type SOCK_STREAM, then this call attempts to make
    *   a connection to another socket.  The other socket is specified by
    *   name, which is an address in the communications space of the socket.
    *
    * If the socket is of type SOCK_DGRAM, then this call
    *   specifies the peer with which the socket is to be associated; this
    *   address is that to which datagrams are to be sent, and the only address
    *   from which datagrams are to be received.
    *
    * Each communications space interprets the name parameter in its own way.
    *   Generally, stream sockets may successfully connect only once; datagram
    *   sockets may use connect multiple times to change their association.
    *   Datagram sockets may dissolve the association by connecting to an
    *   invalid address, such as a zero-filled address.
    */
   vpr::ReturnStatus connect( const vpr::SocketImplSIM* local_sock,
                              vpr::SocketImplSIM** remote_sock,
                              const vpr::InetAddrSIM& remoteName,
                              NetworkGraph::VertexListPtr& path,
                              vpr::Interval timeout );

   // ================
   // acceptor funcs:
   // ================

   /**
    * Bind ties the given adddress to the given socket.  In reality, this
    * registers the given socket object with the simulator and makes it
    * possible to look up the socket by its address.
    *
    * @post If the binding process succeeds, the given socket is assigned a
    *       node in the network graph.
    *
    * @param handle    The socket object to be bound.
    * @param localName The address to which the socket object will be bound.
    *
    * @return vpr::ReturnStatus::Success is returned if the handle could be
    *         bound to the given address.  vpr::ReturnStatus::Failure is
    *         returned otherwise.
    */
   vpr::ReturnStatus bind( const vpr::SocketImplSIM* handle,
                           const vpr::InetAddrSIM& localName );

   /**
    * Unbinds the given socket from its registered address.  Once the socket
    * is unbound, it should close.  For sockets that are not bound, no action
    * is taken.
    *
    * @post The bound address is released so that another socket may bind to
    *       it.
    *
    * @param handle The closed socket object to be unbound from its registered
    *        address.
    *
    * @return vpr::ReturnStatus::Success is returned if the handle could be unbound
    *         from its local address.  vpr::ReturnStatus::Failure is returned
    *         otherwise.
    */
   vpr::ReturnStatus unbind( const vpr::SocketImplSIM* handle );

   /**
    * Sets a socket s to listen for connections.
    * A willingness to accept incoming connections and a queue limit for
    * incoming connections are specified with listen(2), and then each new
    * connection is accepted with accept(2).  The listen call applies only
    * to sockets of type SOCK_STREAM.
    *
    * @return true for success, false for error
    */
   vpr::ReturnStatus listen( const vpr::SocketStreamImplSIM* handle,
                             const int backlog = 5 );

// extra stuff...
public:
   vpr::ReturnStatus assignToNode(vpr::SocketImplSIM* handle);

   vpr::ReturnStatus assignToNode(vpr::SocketImplSIM* handle,
                                  const vpr::InetAddrSIM& addr);

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
    * Is the socket bound to an address?
    */
   bool isBound( const vpr::SocketImplSIM* handle );

   /**
    * Is the address bound to a socket?
    */
   bool isBound(const vpr::InetAddrSIM& addr,
                const vpr::SocketTypes::Type addr_type);

   /**
    * Is there someone listening on the address?
    */
   bool isListening( const vpr::InetAddrSIM& address );

   /**
    * Is there someone listening on the address?
    */
   bool isListening( const vpr::SocketStreamImplSIM* address );

   vpr::InetAddrSIM getBoundAddress( const vpr::SocketImplSIM* handle );

   /**
    * Binds the given socket to the given address after assigning that address
    * a new port number.
    *
    * @pre The given socket is not already bound.
    * @post The socket is bound to the address, and the address has a unique
    *       port number.
    *
    * @param sock The socket to be bound.
    * @param addr The address to which the socket will ultimately be bound.
    *
    * @return vpr::ReturnStatus::Success is returned if the socket is bound
    *         successfully.<br>
    *         vpr::ReturnStatus::Failure is returned otherwise.
    */
   vpr::ReturnStatus bindUnusedPort( vpr::SocketImplSIM* sock,
                                     vpr::InetAddrSIM& addr );

private:
   /**
    * The "internal" bind method called by the public bind() method.  This one
    * actually performs the binding.  It is assumed that checks for the
    * validity of the original bind request have already happened so that it is
    * safe to find the socket to the address.
    *
    * @pre  The given socket is not already bound to an address, and no socket
    *       is bound to the given address.
    * @post The socket is bound to the given address, and a node in the network
    *       graph is assigned to the given socket.
    */
   void _bind( const vpr::SocketImplSIM* handle, const vpr::InetAddrSIM& addr );

   vpr::ReturnStatus _unbind(const vpr::SocketImplSIM* handle);

public:
   /**
    * Returns an unused port at some existing address, or 0 for error.
    */
   vpr::Uint32 genUnusedPort(const vpr::SocketTypes::Type addr_type);

private:
   vpr::sim::NetworkGraph::net_vertex_t getLocalhost(void);

   /**
    * Comparison function for vpr::InetAddrSIM
    */
   struct ltaddr
   {
      bool operator()( const vpr::InetAddrSIM& s1, const vpr::InetAddrSIM& s2)
         const
      {
         if (s1.getAddressValue() < s2.getAddressValue())
            return true;
         else if (s1.getAddressValue() > s2.getAddressValue())
            return false;
         else if (s1.getPort() < s2.getPort())
            return true;
         else
            return false;
      }
   };

   bool mActive;

   /** This is a list of sockets that have gone into a listening state
    * Used to track which sockets are currently in a listening state
    * XXX: Could move this into the actually socket as a socket state instead.
   */
   std::map<vpr::InetAddrSIM, std::pair<const vpr::SocketStreamImplSIM*, int>, ltaddr> mListenerList;
   vpr::Mutex mListenerListMutex;

   /**
    * @note Handles are doubly mapped between the two maps.  Use _bind() and
    *       _unbind() to access these.
    */
   std::map<const vpr::SocketImplSIM*, vpr::InetAddrSIM> mBindListSockUDP;
   std::map<vpr::InetAddrSIM, const vpr::SocketImplSIM*, ltaddr> mBindListAddrUDP;

   vpr::Mutex mBindListSockMutexUDP;
   vpr::Mutex mBindListAddrMutexUDP;

   std::map<const vpr::SocketImplSIM*, vpr::InetAddrSIM> mBindListSockTCP;
   std::map<vpr::InetAddrSIM, const vpr::SocketImplSIM*, ltaddr> mBindListAddrTCP;

   vpr::Mutex mBindListSockMutexTCP;
   vpr::Mutex mBindListAddrMutexTCP;

   vpr::Mutex mPortMutex;          /**< Mutex to protect port generation */
};

} // namespace sim

} // namespace vpr


#endif
