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

#ifndef _VPR_SOCKET_IMPL_WINSOCK_H_
#define _VPR_SOCKET_IMPL_WINSOCK_H_

#include <vpr/vprConfig.h>

#include <winsock2.h>
#include <string>
#include <vector>

#include <vpr/IO/BlockIO.h>
#include <vpr/IO/IOSys.h>
#include <vpr/IO/Socket/InetAddr.h>
#include <vpr/IO/Socket/SocketIpOpt.h>


namespace vpr {

class VPR_CLASS_API SocketImplWinSock : public BlockIO, public SocketIpOpt {
public:
    // ========================================================================
    // vpr::BlockIO overrides.
    // ========================================================================

    // ------------------------------------------------------------------------
    // Open the socket.  This creates a new socket using the domain and type
    // options set through member variables.
    //
    // PRE: m_domain and m_type have been set to values recognized by the
    //      socket() call.
    // POST: A new socket is created with its file handle stored in the
    //       m_sockfd member variable.
    //
    // Returns:
    //     true  - The socket was opened successfully.
    //     false - The socket could not be opened for some reason (an error
    //             message is printed explaining why).
    // ------------------------------------------------------------------------
    virtual Status open(void);

    // ------------------------------------------------------------------------
    //: Close the socket.
    //
    //! PRE: The socket is open.
    //! POST: An attempt is made to close the socket.  The resulting status is
    //+       returned to the caller.  If the socket is closed, m_open is set
    //+       to false.
    //
    //! RETURNS: true  - The socket was closed successfully.
    //! RETURNS: false - The socket could not be closed for some reason.
    // ------------------------------------------------------------------------
    virtual Status close(void);

    // ------------------------------------------------------------------------
    // Bind this socket to the address in the host address member variable.
    //
    // PRE: The socket is open, and m_host_addr has been initialized properly.
    // POST: The socket is bound to the address in m_host_addr.
    //
    // Returns:
    //     true  - The socket was bound to the address successfully.
    //     false - The socket could not be bound to the address in
    //             m_host_addr.  An error message is printed explaining what
    //             went wrong.
    // ------------------------------------------------------------------------
    virtual Status bind(void);

    // ------------------------------------------------------------------------
    //: Return the contained handle
    // ------------------------------------------------------------------------
    inline IOSys::Handle
    getHandle (void) {
       return m_sockfd;
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual Status
    enableBlocking (void) {
        u_long non_blocking_mode;
        Status status;

        non_blocking_mode = 0;    // XXX: What is the right value?
        m_blocking        = true;

        if ( ioctlsocket(m_sockfd, FIONBIO, &non_blocking_mode) != 0 ) {
            status.setCode(Status::Failure);
        }

        return status;
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual Status
    enableNonBlocking (void) {
        u_long non_blocking_mode;
        Status status;

        non_blocking_mode = 1;
        m_blocking        = false;

        if ( ioctlsocket(m_sockfd, FIONBIO, &non_blocking_mode) != 0 ) {
        }

        return status;
    }

    // ========================================================================
    // vpr::Socket interface implementation.
    // ========================================================================

    // ------------------------------------------------------------------------
    // Connect the socket on the client side to the server side.  For a
    // datagram socket, this makes the address given to the constructor the
    // default destination for all packets.  For a stream socket, this has
    // the effect of establishing a connection with the destination.
    //
    // PRE: The socket is open.
    // POST: The socket is connected to the address in m_host_addr.  For a
    //       stream socket, this means that a connection for future
    //       communication has been established.  For a datagram socket, the
    //       default destination for all packets is now m_host_addr.
    //
    // Returns:
    //     true  - The connection was made.
    //     false - The connect could not be made.  An error message is
    //             printed explaining what happened.
    // ------------------------------------------------------------------------
    virtual Status connect(void);

    // ------------------------------------------------------------------------
    //: Get the type of this socket (e.g., vpr::SocketTypes::STREAM).
    //
    //! PRE: The socket implementation pointer is valid.
    //! POST: The socket type for m_socket_imp is returned to the caller.
    //
    //! RETURNS: A vpr::SocketTypes::Type value giving the socket type for
    //+          this socket.
    // ------------------------------------------------------------------------
    inline const SocketTypes::Type&
    getType (void) const {
        return m_type;
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline const InetAddr&
    getLocalAddr (void) const {
        return m_local_addr;
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status
    setLocalAddr(const InetAddr& addr) {
        Status status;

        if (this->m_open) {
            status.setCode(vpr::Status::Failure);
        }
        else {
            m_local_addr = addr;
        }

        return status;
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline const InetAddr&
    getRemoteAddr (void) const {
        return m_remote_addr;
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline Status
    setRemoteAddr (const InetAddr& addr) {
        Status retval;

        if ( m_open ) {
            retval.setCode(Status::Failure);
        }
        else {
            m_remote_addr = addr;
        }

        return retval;
    }

protected:
    // ------------------------------------------------------------------------
    // Default constructor.  This just initializes member variables to
    // reasonable defaults.
    //
    // PRE: None.
    // POST: The member variables are initialized accordingly to reasonable
    //       defaults.
    // ------------------------------------------------------------------------
    SocketImplWinSock(void);

    // ------------------------------------------------------------------------
    // Standard constructor.  This takes two InetAddr objects, a local address
    // and a remote address.
    //
    // PRE: None.
    // POST: The member variables are initialized with the given values.
    //
    // Arguments:
    //     local_addr  - The local address for the socket.
    //     remote_addr - The remote address for the socket.
    // ------------------------------------------------------------------------
    SocketImplWinSock(const InetAddr& local_addr, const InetAddr& remote_addr,
                      const SocketTypes::Type type);

    // ------------------------------------------------------------------------
    // Destructor.  This currently does nothing.
    //
    // PRE: None.
    // POST: None.
    // ------------------------------------------------------------------------
    virtual ~SocketImplWinSock(void);

    // ------------------------------------------------------------------------
    //: Do the WinSock initialization required before any socket stuff can
    //+ happen.
    //
    //! PRE: None.
    //! POST: The WinSock startup operation is performed.
    // ------------------------------------------------------------------------
    void init(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual Status
    read_i (void* buffer, const size_t length, ssize_t& bytes_read) {
        return recv(buffer, length, 0, bytes_read);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual Status
    readn_i (void* buffer, const size_t length, ssize_t& bytes_read) {
        return recvn(buffer, length, 0, bytes_read);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual Status
    write_i (const void* buffer, const size_t length, ssize_t& bytes_written) {
        return send(buffer, length, 0, bytes_written);
    }

    // ------------------------------------------------------------------------
    // Receive the specified number of bytes from the remote site to which the
    // local side is connected.
    //
    // PRE: The socket is open and connect() has been called.  buffer is at
    //      least length bytes in size.
    // POST: length bytes are read from the remote site and stored in the
    //       given buffer.  The number of bytes read is returned to the
    //       caller.
    //
    // Arguments:
    //     buffer - A pointer to the buffer used for storing the bytes
    //              received from the remote site.
    //     length - The number of bytes to receive from the remote site.
    //     flags  - Flags to use when receiving the data.  This is optional
    //              and defaults to 0.
    //
    // Returns:
    //     >-1 - The number of bytes received.
    //      -1 - Something went wrong when trying to receive the data.
    // ------------------------------------------------------------------------
    virtual Status recv(void* buffer, const size_t length, const int flags,
                        ssize_t& bytes_read);

    // ------------------------------------------------------------------------
    //! Read exactly the specified number of bytes from the socket into the
    //+ given buffer.
    //
    //! PRE: The socket is valid and open, and the buffer is at least length
    //+      bytes long.
    //! POST: The given buffer has length bytes copied into it from the file
    //+       handle, and the number of bytes read successfully is returned to
    //+       the caller.
    //
    //! ARGS: buffer - A pointer to the buffer where the file contents are to
    //                 be stored.
    //! ARGS: length - The number of bytes to be read.
    //
    //! RETURNS: >-1 - The number of bytes successfully read from the socket.
    //! RETURNS:  -1 - An error occurred when reading.
    // ------------------------------------------------------------------------
    virtual Status recvn(void* buffer, const size_t length, const int flags,
                         ssize_t& bytes_read);

    // ------------------------------------------------------------------------
    // Send the specified number of bytes contained in the given buffer from
    // the local side to the remote site to which we are connected.
    //
    // PRE: The socket is open and connect() has been called.  buffer is at
    //      least length bytes in size.
    // POST: length bytes are sent from the buffer on the local site to the
    //       remote site.  The number of bytes read is returned to the caller.
    //
    // Arguments:
    //     buffer - A pointer to the buffer containing the bytes to be sent.
    //     length - The number of bytes to sent to the remote site.
    //     flags  - Flags to use when sending the data.  This is optional and
    //              defaults to 0.
    //
    // Returns:
    //     >-1 - The number of bytes received.
    //      -1 - Something went wrong when trying to receive the data.
    // ------------------------------------------------------------------------
    virtual Status send(const void* buffer, const size_t length,
                        const int flags, ssize_t& bytes_sent);

    /**
     *
     */
    virtual Status getOption(const SocketOptions::Types option,
                             struct SocketOptions::Data& data);

    /**
     *
     */
    virtual Status setOption(const SocketOptions::Types option,
                             const struct SocketOptions::Data& data);

    SOCKET            m_sockfd;
    bool              m_bound;
    bool              m_connected;
    InetAddr          m_local_addr;
    InetAddr          m_remote_addr;
    SocketTypes::Type m_type;
};

}; // End of vpr namespace


#endif   /* _VPR_SOCKET_IMPL_WINSOCK_H_ */
