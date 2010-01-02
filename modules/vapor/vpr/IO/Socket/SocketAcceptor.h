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
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VPR_SOCK_ACCEPTOR_H_
#define _VPR_SOCK_ACCEPTOR_H_

#include <vpr/vprConfig.h>
#include <vpr/IO/Socket/SocketStream.h>
#include <vpr/IO/Socket/InetAddr.h>

namespace vpr
{

/** \class SocketAcceptor SocketAcceptor.h vpr/IO/Socket/SocketAcceptor.h
 *
 * Socket connection acceptor factory.
 * Wraps all the nitty-gritty details of accepting a connection.
 */
class VPR_CLASS_API SocketAcceptor
{
public:
   /**
    * Default constructor.  This does nothing.  If this constructor is used
    * when creating an acceptor, the open() method must be called.
    *
    * @see open
    */
   SocketAcceptor()
   {
      ;
   }

   /**
    * Destructor.  This makes sure that the accepting socket gets closed.
    */
   ~SocketAcceptor()
   {
      if ( mSocket.isOpen() )
      {
         mSocket.close();
      }
   }

   /**
    * Constructs Acceptor to accept connections on the given address.
    * - Opens the socket automatically
    *
    * @post This acceptor is open and bound to the given address.  It is ready
    *       to accept incoming connections.
    *
    * @param addr      The local address to which this acceptor will be bound.
    * @param reuseAddr A flag stating whether or not to set the "reuse
    *                  address" option for this socket.  This parameter is
    *                  optional and defaults to true.
    * @param backlog   The maximum allowed size for the queue of pending
    *                  connections.
    */
   SocketAcceptor(const vpr::InetAddr& addr, bool reuseAddr = true,
                  const int backlog = 5)
   {
      open(addr, reuseAddr, backlog);
   }

   /**
    * Opens the socket for accepting a connection.
    * - Binds the connection and starts listening.
    *
    * @pre  This acceptor is not already open.
    * @post This acceptor is open and bound to the given address.  It is ready
    *       to accept incoming connections.
    *
    * @param addr      The local address to which this acceptor will be bound.
    * @param reuseAddr A flag stating whether or not to set the "reuse
    *                  address" option for this socket.  This parameter is
    *                  optional and defaults to true.
    * @param backlog   The maximum allowed size for the queue of pending
    *                  connections.
    */
   void open(const vpr::InetAddr& addr, bool reuseAddr = true,
                          const int backlog = 5);

   /**
    * Accepts a new connection.  Creates a new socket on the connection and
    * returns it.
    *
    * @pre This acceptor is open and listening for incoming connections.
    * @post A connected socket is returned via the by-reference parameter
    *       \p sock if a connection is successfully accepted.
    *
    * @param sock    A reference to a vpr::SocketStream object that will be
    *                used as storage for the connected socket object if a
    *                connection can be established.
    * @param timeout The amount of time to wait for an incoming connection
    *                request.  This argument is optional and default to
    *                vpr::Interval::NoTimeout.
    *
    * @throws vpr::WouldBlockException if this is a non-blocking socket,
    *         and there are no waiting connection requests.
    * @throws vpr::TimeoutException if no connection requests arrived within
    *         the given timeout period.
    * @throws vpr::SocketException if the connection was not accepted because
    *         of an error.
    */
   void accept(vpr::SocketStream& sock,
               vpr::Interval timeout = vpr::Interval::NoTimeout)
   {
      vprASSERT(mSocket.isOpen());

      mSocket.accept(sock, timeout);
   }

   /**
    * Closes the accepting socket.
    * @throws vpr::IOException if the operation failed.
    */
   void close()
   {
      mSocket.close();
   }

   /**
    * Gets the member socket that is being used.
    */
   vpr::SocketStream& getSocket()
   {
      return mSocket;
   }

   vpr::IOSys::Handle getHandle()
   {
      return mSocket.getHandle();
   }

private:
   vpr::SocketStream    mSocket;
};

}


#endif /* _VPR_SOCK_ACCEPTOR_H_ */
