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
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

#ifndef _VPR_SOCKET_STREAM_IMPL_BSD_H_
#define _VPR_SOCKET_STREAM_IMPL_BSD_H_

#include <vpr/vprConfig.h>

#include <string>

#include <vpr/md/POSIX/IO/Socket/SocketImplBSD.h>


namespace vpr
{

/** \class SocketStreamImplBSD SocketStreamImplBSD.h vpr/IO/Socket/SocketStream.h
 *
 * Implementation of the stream socket wrapper using BSD sockets.  This is
 * used in conjunction with vpr::SocketConfiguration to create the typedef
 * vpr::SocketStream.
 */
class SocketStreamImplBSD : public SocketImplBSD
{
public:
   // ========================================================================
   // vpr::SocketStream interface.
   // ========================================================================

   /**
    * Constructor.
    *
    * @pre None.
    * @post The member variables are initialized with the mType variable in
    *       particular set to vpr::SocketTypes::STREAM.
    */
   SocketStreamImplBSD();

   /**
    * Constructs a stream socket using the given addresses as defaults for
    * communication channels.  This takes the address (either hostname or IP
    * address) of a remote site and a port and stores the values for later use
    * in the member variables of the object.
    *
    * @post The member variables are initialized with the type in particular
    *       set to vpr::SocketTypes::STREAM.
    *
    * @param localAddr  The local address for this socket.  This is used for
    *                   binding the socket.
    * @param remoteAddr The remote address for this socket.  This is used to
    *                   specify the connection addres for this socket.
    */
   SocketStreamImplBSD(const InetAddr& localAddr, const InetAddr& remoteAddr);

   /**
    * Copy constructor.
    *
    * @post This socket is a copy of the given socket.
    */
   SocketStreamImplBSD(const SocketStreamImplBSD& sock);

   /**
    * Puts this socket into the listening state where it listens for
    * incoming connection requests.
    *
    * @pre The socket has been opened and bound to the address in
    *      \c mLocalAddr.
    * @post The socket is in a listening state waiting for incoming
    *       connection requests.
    *
    * @param backlog The maximum length of th queue of pending connections.
    *
    * @return vpr::ReturnStatus::Succeed is returned if this socket is now in a
    *         listening state.
    * @return vpr::ReturnStatus::Fail is returned otherwise.
    */
   vpr::ReturnStatus listen(const int backlog = 5);

   /**
    * Accepts an incoming connection request and returns the connected socket
    * to the caller in the given socket object reference.
    *
    * @pre This socket is open, bound, and in a listening state.
    * @post When a connection is established, the given socket \p sock will
    *       be set up to communicate with the newly accepted connection.
    *
    * @param sock    A reference to a vpr::SocketStream object that will
    *                be used to return the newly connected socket.
    * @param timeout The length of time to wait for the accept call to
    *                return.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the new connection was
    *         accepted succesfully.
    * @return vpr::ReturnStatus::WouldBlock is returned if this is a
    *         non-blocking socket, and there are no waiting connection
    *         requests.
    * @return vpr::ReturnStatus::Timeout is returned when no connections
    *         requests arrived within the given timeout period.
    * @return vpr::ReturnStatus::Fail is returned if the connection was not
    *         accepted.  An error message is printed explaining what went
    *         wrong.
    *
    * @see open, bind, listen
    */
   vpr::ReturnStatus accept(SocketStreamImplBSD& sock,
                            vpr::Interval timeout = vpr::Interval::NoTimeout);
};

} // End of vpr namespace


#endif  /* _VJ_SOCKET_STREAM_IMPL_BSD_H_ */
