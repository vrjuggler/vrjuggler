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

#ifndef _VPR_SOCKET_STREAM_BRIDGE_H_
#define _VPR_SOCKET_STREAM_BRIDGE_H_
// NOTE: this is the bridge class for use with SocketStream.h

#include <vpr/vprConfig.h>

#include <vpr/Util/Status.h>
#include <vpr/IO/Socket/Socket_t.h> /* base bridge class.. */

namespace vpr {

/**
 * Cross-platform stream socket class.
 *
 * @author Patrick Hartling
 * @author Allen Bierbaum
 * @author Kevin Meinert
 */
template<class RealSocketStreamImpl, class RealSocketStreamImplParent>
class SocketStream_t : public Socket_t<RealSocketStreamImplParent> {
public:
    /**
     * Default constructor.
     */
    SocketStream_t (void)
        : Socket_t<RealSocketStreamImplParent>(), m_socket_stream_imp()
    {
        m_socket_imp = &m_socket_stream_imp;
    }

    /**
     * Constructor.  This takes a reference to a vpr::InetAddr object giving
     * the local socket address and a reference to a vpr::InetAddr object
     * giving the remote address.
     *
     * @pre addr is a reference to a valid vpr::InetAddr object.
     * @post A socket is created using the contents of addr.
     *
     * @param local_addr  A reference to a vpr::InetAddr object for the
     *                     local socket address.
     * @param remote_addr A reference to a vpr::InetAddr object for the
     *                     remote socket address.
     */
    SocketStream_t (vpr::InetAddr local_addr, vpr::InetAddr remote_addr)
        : Socket_t<RealSocketStreamImplParent>(),
          m_socket_stream_imp(local_addr, remote_addr)
    {
        m_socket_imp = &m_socket_stream_imp;
    }

    /**
     * Copy constructor.
     *
     * @param sock The source stream socket to be copied into this object.
     */
    SocketStream_t (const SocketStream_t& sock)
        : m_socket_stream_imp(sock.m_socket_stream_imp)
    {
        m_socket_imp = &m_socket_stream_imp;
    }

    /**
     * Destructor.  This currently does nothing.
     *
     * @pre None.
     * @post None.
     */
    virtual ~SocketStream_t (void) {
        /* Do nothing. */ ;
    }

    /**
     * Puts this socket into the listening state where it listens for
     * incoming connection requests.
     *
     * @pre The socket has been opened and bound to the address in m_addr.
     * @post The socket is in a listening state waiting for incoming
     *       connection requests.
     *
     * @param backlog The maximum length of th queue of pending connections.
     *
     * @return vpr::Status::Success is returned if this socket is now in a
     *         listening state.<br>
     *         vpr::Status::Failure is returned otherwise.
     */
    inline vpr::Status
    listen (const int backlog = 5) {
        return m_socket_stream_imp.listen(backlog);
    }

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
    inline vpr::Status
    accept (SocketStream_t& sock,
            const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
        return m_socket_stream_imp.accept(sock.m_socket_stream_imp, timeout);
    }

    /**
     * Opens a socket to be used for the server side communications.  This is
     * provided to automate the repeated tasks executed when a server is
     * set up.  It handles opening the socket, binding the address and going
     * into listening mode.
     *
     * @pre The member variables have been initialized properly.
     * @post The socket is in the listening state waiting for incoming
     *       connection requests.
     *
     * @param reuse_addr Enable or disable reuse of the address being bound.
     *                   This argument is optional and defaults to true.
     * @param backlog    The maximum length of the pending connection queue.
     *
     * @return <code>vpr::Status::Success</code> is returned if the server
     *         socket is in the listening state and ready to accept incoming
     *         connection requests.<br>
     *         <code>vpr::Status::Failure</code> is returned if the server
     *         socket could not be set up.
     */
    inline vpr::Status
    openServer (const bool reuse_addr = true, const int backlog = 5) {
        vpr::Status status;

        // First, open the socket.
        status = open();

        if ( status.success() ) {
            status = setReuseAddr(reuse_addr);

            if ( status.success() ) {
                status = bind();

                // If that succeeded, bind to the internal address.
                if ( status.success() ) {
                    // Finally, if that succeeded, go into listening mode.
                    status = listen(backlog);
                }
            }
        }

        return status;
    }

    /**
     *
     */
    inline vpr::Status
    getMaxSegmentSize (vpr::Int32& size) const {
        return m_socket_stream_imp.getMaxSegmentSize(size);
    }

    /**
     *
     */
    inline vpr::Status
    setMaxSegmentSize (const vpr::Int32 size) {
        return m_socket_stream_imp.setMaxSegmentSize(size);
    }

    /**
     * Gets the current no-delay status for this socket.  If no-delay is true,
     * then the Nagel algorithm has been disabled.
     *
     * @param enabled A reference to a <code>bool</code> variable to be used as
     *                storage for the current no-delay enable state.  If the
     *                value is <code>true</code>, the Nagel algorithm is
     *                <i>not</i> being used to delay the transmission of TCP
     *                segements.  Otherwise, the Nagel alorithm is delaying
     *                the transmission.
     */
    inline vpr::Status
    getNoDelay (bool& enabled) const {
        return m_socket_stream_imp.getNoDelay(enabled);
    }

    /**
     * Sets the current no-delay status for this socket.  If no-delay is true,
     * then the Nagel algorithm will be disabled.
     *
     * @param enable_val The Boolean enable/disable state for no-delay on this
     *                   socket.
     */
    inline vpr::Status
    setNoDelay (const bool enable_val) {
        return m_socket_stream_imp.setNoDelay(enable_val);
    }

protected:
    /**
     * Constructor.  Create a vpr::SocketStream object using the given
     * vpr::SocketStreamImpl object pointer.  This is needed by accept().
     *
     * @pre sock_imp points to a valid vpr::SocketStreamImpl object.
     * @post sock_imp is copied into m_socket_stream_imp.
     *
     * @param sock_imp A pointer to a vpr::SocketStreamImpl object.
     */
    SocketStream_t (RealSocketStreamImpl* sock_imp)
        : Socket_t<RealSocketStreamImplParent>(), m_socket_stream_imp(*sock_imp)
    {
        m_socket_imp = &m_socket_stream_imp;
    }

    /// Platform-specific stream socket implementation
    RealSocketStreamImpl m_socket_stream_imp;
};

}; // End of vpr namespace

#endif // _VPR_SOCKET_STREAM_BRIDGE_H_
