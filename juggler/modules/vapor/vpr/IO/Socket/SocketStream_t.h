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

// ----------------------------------------------------------------------------
//: Cross-platform stream socket class.
// ----------------------------------------------------------------------------
//! PUBLIC_API:
template<class RealSocketStreamImpl, class RealSocketStreamImplParent>
class SocketStream_t : public Socket_t<RealSocketStreamImplParent> {
public:
    // ------------------------------------------------------------------------
    //: Default constructor.
    // ------------------------------------------------------------------------
    SocketStream_t (void)
        : Socket_t<RealSocketStreamImplParent>(), m_socket_stream_imp()
    {
        m_socket_imp = &m_socket_stream_imp;
    }

    // ------------------------------------------------------------------------
    //: Constructor.  This takes a reference to a vpr::InetAddr object giving
    //+ the local socket address and a reference to a vpr::InetAddr object
    //+ giving the remote address.
    //
    //! PRE: addr is a reference to a valid vpr::InetAddr object.
    //! POST: A socket is created using the contents of addr.
    //
    //! ARGS: local_addr  - A reference to a vpr::InetAddr object for the
    //+                     local socket address.
    //! ARGS: remote_addr - A reference to a vpr::InetAddr object for the
    //+                     remote socket address.
    // ------------------------------------------------------------------------
    SocketStream_t (InetAddr local_addr, InetAddr remote_addr)
        : Socket_t<RealSocketStreamImplParent>(),
          m_socket_stream_imp(local_addr, remote_addr)
    {
        m_socket_imp = &m_socket_stream_imp;
    }

    // ------------------------------------------------------------------------
    // Copy constructor.
    // ------------------------------------------------------------------------
    SocketStream_t (const SocketStream_t& sock)
        : m_socket_stream_imp(sock.m_socket_stream_imp)
    {
        m_socket_imp = &m_socket_stream_imp;
    }

    // ------------------------------------------------------------------------
    //: Destructor.  This currently does nothing.
    //
    //! PRE: None.
    //! POST: None.
    // ------------------------------------------------------------------------
    virtual ~SocketStream_t (void) {
        /* Do nothing. */ ;
    }

    // ------------------------------------------------------------------------
    //: Listen on the socket for incoming connection requests.
    //
    //! PRE: The socket has been opened and bound to the address in m_addr.
    //! POST: The socket is in a listening state waiting for incoming
    //+       connection requests.
    //
    //! ARGS: backlog - The maximum length of th queue of pending connections.
    //
    //! RETURNS: true  - The socket is in a listening state.
    //! RETURNS: false - The socket could not be put into a listening state.
    //+                  An error message is printed explaining what went
    //+                  wrong.
    // ------------------------------------------------------------------------
    inline vpr::Status
    listen (const int backlog = 5) {
        return m_socket_stream_imp.listen(backlog);
    }

    // ------------------------------------------------------------------------
    //: Accept an incoming connection request and return the connected socket
    //+ to the caller in the given socket object reference.
    //
    //! PRE: The socket is open and is in a listening state.
    //! POST:
    //
    //! ARGS: sock    - A reference to a vpr::SocketStream object that will
    //+                 be used to return the connected socket created.
    //! ARGS: timeout - The length of time to wait for the accept call to
    //+                 return.
    //
    //! RETURNS: vpr::Status::Success    - The incoming request has been
    //+                                    handled, and the given SocketStream
    //+                                    object is a valid, connected
    //+                                    socket.
    //! RETURNS: vpr::Status;:WouldBlock - This is a non-blocking socket, and
    //+                                    there are no waiting connection
    //+                                    requests.
    //! RETURNS: vpr::Status::Timeout    - No connection requests arrived
    //+                                    within the given timeout period.
    //! RETURNS: vpr::Status::Failure    - The accept failed.  The given
    //+                                    SocketStream object is not
    //+                                    modified.
    // ------------------------------------------------------------------------
    inline Status
    accept (SocketStream_t& sock,
            const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
        return m_socket_stream_imp.accept(sock.m_socket_stream_imp, timeout);
    }

    // ------------------------------------------------------------------------
    //: Open a socket to be used for the server side communications.  This is
    //+ provided to automate the repeated tasks executed when a server is
    //+ set up.  It handles opening the socket, binding the address and going
    //+ into listening mode.
    //
    //! PRE: The member variables have been initialized properly.
    //! POST: The socket is in the listening state waiting for incoming
    //+       connection requests.
    //
    //! ARGS: reuse_addr - Enable or disable reuse of the address being bound.
    //+                    This argument is optional and defaults to true.
    //! ARGS: backlog    - The maximum length of the pending connection queue.
    //
    //! RETURNS: true  - The server socket is in the listening state.
    //! RETURNS: false - The server socket could not be set up.  An error
    //+                  message is printed explaining what went wrong.
    // ------------------------------------------------------------------------
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
    getMaxSegmentSize (Int32& size) const {
        return m_socket_stream_imp.getMaxSegmentSize(size);
    }

    /**
     *
     */
    inline vpr::Status
    setMaxSegmentSize (const Int32 size) {
        return m_socket_stream_imp.setMaxSegmentSize(size);
    }

    /**
     *
     */
    inline vpr::Status
    getNoDelay (bool& enabled) const {
        return m_socket_stream_imp.getNoDelay(enabled);
    }

    /**
     *
     */
    inline vpr::Status
    setNoDelay (const bool enable_val) {
        return m_socket_stream_imp.setNoDelay(enable_val);
    }

protected:
    // ------------------------------------------------------------------------
    //: Constructor.  Create a vpr::SocketStream object using the given
    //+ vpr::SocketStreamImpl object pointer.  This is needed by accept().
    //
    //! PRE: sock_imp points to a valid vpr::SocketStreamImpl object.
    //! POST: sock_imp is copied into m_socket_stream_imp.
    //
    //! ARGS: sock_imp - A pointer to a vpr::SocketStreamImpl object.
    // ------------------------------------------------------------------------
    SocketStream_t (RealSocketStreamImpl* sock_imp)
        : Socket_t<RealSocketStreamImplParent>(), m_socket_stream_imp(*sock_imp)
    {
        m_socket_imp = &m_socket_stream_imp;
    }

    RealSocketStreamImpl m_socket_stream_imp; //: Platform-specific stream
                                              //+ socket implementation
};

}; // End of vpr namespace

#endif // _VPR_SOCKET_STREAM_BRIDGE_H_
