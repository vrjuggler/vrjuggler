/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VPR_SOCKET_STREAM_IMPL_BSD_H_
#define _VPR_SOCKET_STREAM_IMPL_BSD_H_

#include <vpr/vprConfig.h>

#include <string>

#include <vpr/md/POSIX/IO/Socket/SocketImplBSD.h>
#include <vpr/IO/Socket/SocketStreamOpt.h>


namespace vpr {

/**
 * Implementation of the stream socket wrapper using BSD sockets.
 *
 * @author Patrick Hartling
 */
class SocketStreamImplBSD : public SocketImplBSD {
public:
    // ========================================================================
    // vpr::SocketStreamImp implementation.
    // ========================================================================

    /**
     * Constructor.  This takes the address (either hostname or IP address) of
     * a remote site and a port and stores the values for later use in the
     * member variables of the object.
     *
     * @pre None.
     * @post The member variables are initialized with the m_type variable in
     *       particular set to vpr::SocketTypes::SOCK_STREAM.
     */
    SocketStreamImplBSD (void)
        : SocketImplBSD(SocketTypes::STREAM)
    {
        /* Do nothing. */ ;
    }

    /**
     * Constructs a stream socket using the given addresses as defaults for
     * communication channels.
     *
     * @post The member variables are initialized with the type in particular
     *       set to vpr::SocketTypes::STREAM.
     *
     * @param local_addr  The local address for this socket.  This is used for
     *                    binding the socket.
     * @param remote_addr The remote address for this socket.  This is used to
     *                    specify the connection addres for this socket.
     */
    SocketStreamImplBSD (const InetAddr& local_addr,
                        const InetAddr& remote_addr)
        : SocketImplBSD(local_addr, remote_addr, SocketTypes::STREAM)
    {
        /* Do nothing. */ ;
    }

    /**
     * Copy constructor.
     *
     * @post This socket is a copy of the given socket.
     */
    SocketStreamImplBSD (const SocketStreamImplBSD& sock)
        : SocketImplBSD(SocketTypes::STREAM)
    {
        m_local_addr      = sock.m_local_addr;
        m_remote_addr     = sock.m_remote_addr;
        m_handle          = new FileHandleImplUNIX(sock.m_handle->getName());
        m_handle->m_fdesc = sock.m_handle->m_fdesc;
    }

    /**
     * Destructor.  This currently does nothing.
     */
    virtual ~SocketStreamImplBSD (void) {
        /* Do nothing. */ ;
    }

    /**
     * Puts this socket into the listening state where it listens for
     * incoming connection requests.
     *
     * @pre The socket has been opened and bound to the address in
     *      m_host_addr.
     * @post The socket is in a listening state waiting for incoming
     *       connection requests.
     *
     * @param backlog The maximum length of th queue of pending connections.
     *
     * @return vpr::Status::Success is returned if this socket is now in a
     *         listening state.<br>
     *         vpr::Status::Failure is returned otherwise.
     */
    virtual Status listen(const int backlog = 5);

    /**
     * Accepts an incoming connection request and return the connected socket
     * to the caller in the given socket object reference.
     *
     * @pre The socket is open and is in a listening state.
     * @post When a connection is established, the given vpr::SocketStream
     *       object is assigned the newly connected socket.
     *
     * @param sock    A reference to a vpr::SocketStream object that will
     *                be used to return the connected socket created.
     * @param timeout The length of time to wait for the accept call to
     *                return.
     *
     * @return vpr::Status::Success is returned if the incoming request has
     *         been handled, and the given SocketStream object is a valid,
     *         connected socket.<br>
     *         vpr::Status::WouldBlock is returned if this is a non-blocking
     *         socket, and there are no waiting connection requests.<br>
     *         vpr::Status::Timeout is returned when no connections requests
     *         arrived within the given timeout period.<br>
     *         vpr::Status::Failure is returned if the accept failed.  The
     *         given vpr::SocketStream object is not modified in this case.
     */
    virtual Status accept(SocketStreamImplBSD& sock,
                          vpr::Interval timeout = vpr::Interval::NoTimeout);
};

}; // End of vpr namespace


#endif	/* _VJ_SOCKET_STREAM_IMPL_BSD_H_ */
