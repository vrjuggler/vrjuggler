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

#ifndef _VPR_SOCKET_STREAM_IMPL_NSPR_H_
#define _VPR_SOCKET_STREAM_IMPL_NSPR_H_

#include <vpr/vprConfig.h>
#include <string>
#include <prio.h>

#include <vpr/md/NSPR/IO/Socket/SocketImplNSPR.h>
#include <vpr/Util/Debug.h>

namespace vpr
{

/** \class SocketStreamImplNSPR SocketStreamImplNSPR.h vpr/IO/Socket/SocketStream.h
 *
 * NSPR implementation of the stream-oriented socket interface.  This is used
 * in conjunction with vpr::SocketConfiguration to create the typedef
 * vpr::SocketStream.
 */
class VPR_CLASS_API SocketStreamImplNSPR : public SocketImplNSPR
{
public:
   // ========================================================================
   // vpr::SocketStreamImp implementation.
   // ========================================================================

   /**
    * Default constructor.  This initializes the member variables.
    */
   SocketStreamImplNSPR();

   /**
    * Constructor.  This takes the local and remote addresses for this socket.
    * The local address is the address to which this socket will be bound.
    * The remote address is the address which with this socket will
    * communicate.
    *
    * @post The member variables are initialized to default values.  The
    *       socket type is set to vpr::SocketTypes::STREAM.
    *
    * @param localAddr  The local address to which this socket will be bound.
    * @param remoteAddr The remote address whith which this socket will
    *                   communicate.
    *
    * @see bind, connect
    */
   SocketStreamImplNSPR(const vpr::InetAddr& localAddr,
                        const vpr::InetAddr& remoteAddr);

   /**
    * Copy constructor.
    * XXX: We need to have a reference count here
    */
   SocketStreamImplNSPR(const SocketStreamImplNSPR& sock)
      : SocketImplNSPR(sock)
   {
      /* Just call base class */ ;
   }

   /**
    * Listens on the socket for incoming connection requests.
    *
    * @pre The socket has been opened and bound to the address in
    *      \c mLocalAddr.
    * @post The socket is in a listening state waiting for incoming
    *       connection requests.
    *
    * @param backlog The maximum length of the queue of pending connections.
    *
    * @throw vpr::SocketException If the socket could not be put into a
    *                             listening state.
    *
    * @see open, bind
    */
   void listen(const int backlog = 5);

   /**
    * Accepts an incoming connection request and returns the connected socket
    * to the caller in the given socket object reference.
    *
    * @pre This socket is open, bound, and in a listening state.
    * @post When a connection is established, the given socket \p sock will
    *       be set up to communicate with the newly accepted connection.
    *
    * @param sock    A reference to a vpr::SocketStream object that will be
    *                used to return the newly connected socket.
    * @param timeout The length of time to wait for the accept call to return.
    *
    * @throw vpr::WouldBlockException
    *           If this is a non-blocking socket, and there are no waiting
    *           connection requests.
    * @throw vpr::TimeoutException
    *           If no connection requests arrived within the given timeout
    *           period.
    * @throw vpr::SocketException
    *           If the connection was not accepted because of an error.
    *
    * @see open, bind, listen
    */
   void accept(SocketStreamImplNSPR& sock,
               const vpr::Interval timeout = vpr::Interval::NoTimeout);
};

} // End of vpr namespace


#endif   /* _VJ_SOCKET_STREAM_IMPL_NSPR_H_ */
