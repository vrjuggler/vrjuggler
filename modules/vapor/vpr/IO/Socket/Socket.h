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

#ifndef _VPR_SOCKET_H_
#define _VPR_SOCKET_H_

#include <string>

#include <IO/BlockIO.h>
#include <IO/Socket/SocketTypes.h>


namespace vpr {

// ----------------------------------------------------------------------------
//: Cross-platform block-based socket interface.  vpr::Socket objects cannot
//+ be instantiated.  Instead, see vpr::SocketStream and vpr::SocketDatagram.
// ----------------------------------------------------------------------------
//!PUBLIC_API:
template<class RealSocketImp>
class Socket_t : public BlockIO {
public:
    // ========================================================================
    // Block I/O interface.
    // ========================================================================

    // ------------------------------------------------------------------------
    //: Set the open flags so that the socket is opened in read-only mode.
    //
    //! PRE: None.
    //! POST: The open flags are updated so that when the socket is opened, it
    //+       is opened in read-only mode.  If the socket is already open,
    //+       this has no effect.
    // ------------------------------------------------------------------------
    inline virtual void
    setOpenReadOnly (void) {
        m_socket_imp->setOpenReadOnly();
    }

    // ------------------------------------------------------------------------
    //: Set the open flags so that the socket is opened in write-only mode.
    //
    //! PRE: None.
    //! POST: The open flags are updated so that when the socket is opened, it
    //+       is opened in write-only mode.  If the socket is already open,
    //+       this has no effect.
    // ------------------------------------------------------------------------
    inline virtual void
    setOpenWriteOnly (void) {
        m_socket_imp->setOpenWriteOnly();
    }

    // ------------------------------------------------------------------------
    //: Set the open flags so that the socket is opened in read/write mode.
    //
    //! PRE: None.
    //! POST: The open flags are updated so that when the socket is opened, it
    //+       is opened in read/write mode.  If the socket is already open,
    //+       this has no effect.
    // ------------------------------------------------------------------------
    inline virtual void
    setOpenReadWrite (void) {
        m_socket_imp->setOpenReadWrite();
    }

    // ------------------------------------------------------------------------
    //: Set the blocking flags so that the socket is opened in blocking mode.
    //
    //! PRE: None.
    //! POST: The open flags are updated so that when the socket is opened, it
    //+       is opened in blocking mode.  If the socket is already open, this
    //+       has no effect.
    // ------------------------------------------------------------------------
    inline virtual void
    setOpenBlocking (void) {
        m_socket_imp->setOpenBlocking();
    }

    // ------------------------------------------------------------------------
    //: Set the blocking flags so that the socket is opened in non-blocking
    //+ mode.
    //
    //! PRE: None.
    //! POST: The open flags are updated so that when the socket is opened, it
    //+       is opened in non-blocking mode.  If the socket is already open,
    //+       this has no effect.
    // ------------------------------------------------------------------------
    inline virtual void
    setOpenNonBlocking (void) {
        m_socket_imp->setOpenNonBlocking();
    }

    // ------------------------------------------------------------------------
    //: Open the socket.
    //
    //! PRE: The socket is not already open.
    //! POST: An attempt is made to open the socket.  The resulting status is
    //+       returned to the caller.  If the socket is opened, m_open is set
    //+       to true.
    //
    //! RETURNS: true  - The socket was opened successfully.
    //! RETURNS: false - The socket could not be opened for some reason.
    // ------------------------------------------------------------------------
    inline virtual bool
    open (void) {
        return m_socket_imp->open();
    }

    // ------------------------------------------------------------------------
    //: Close the socket.
    //
    //! PRE: The socket is open.
    //! POST: An attempt is made to close the socket.  The resulting status
    //+       is returned to the caller.  If the socket is closed, m_open
    //+       is set to false.
    //
    //! RETURNS: true  - The socket was closed successfully.
    // ! RETURNS:false - The socket could not be closed for some reason.
    // ------------------------------------------------------------------------
    virtual bool
    close (void) {
        return m_socket_imp->close();
    }

    // ------------------------------------------------------------------------
    //: Get the open state of this socket.
    //
    //! PRE: None.
    //! POST: The boolean value giving the open state is returned to the
    //+       caller.
    //
    //! RETURNS: true  - The socket is in the open state.
    //! RETURNS: false - The socket is in the closed state.
    // ------------------------------------------------------------------------
    inline virtual bool
    isOpen (void) {
        return m_socket_imp->isOpen();
    }

    // ------------------------------------------------------------------------
    //: Reconfigure the socket so that it is in blocking mode.
    //
    //! PRE: The socket is open.
    //! POST: Processes will block when accessing the socket.
    //
    //! RETURNS: true  - The blocking mode was changed successfully.
    //! RETURNS: false - The blocking mode could not be changed for some
    //+                  reason.
    // ------------------------------------------------------------------------
    inline virtual bool
    enableBlocking (void) {
        return m_socket_imp->enableBlocking();
    }

    // ------------------------------------------------------------------------
    //: Reconfigure the socket so that it is in non-blocking mode.
    //
    //! PRE: The socket is open.
    //! POST: Processes will not block when accessing the socket.
    //
    //! RETURNS: true  - The blocking mode was changed successfully.
    //! RETURNS: false - The blocking mode could not be changed for some
    //+                  reason.
    // ------------------------------------------------------------------------
    inline virtual bool
    enableNonBlocking (void) {
        return m_socket_imp->enableNonBlocking();
    }

    // ------------------------------------------------------------------------
    //+ Bind this socket to the address in the host address member variable.
    //
    //! PRE: The socket is open.
    //! POST: The socket is bound to the address defined in the constructor.
    //
    //! RETURNS: true  - The socket was bound to the address successfully.
    //! RETURNS: false - The socket could not be bound to the address.  An
    //+                  error message is printed explaining what went wrong.
    // ------------------------------------------------------------------------
    inline virtual bool
    bind (void) {
        return m_socket_imp->bind();
    }

    // ------------------------------------------------------------------------
    //: Read at most the specified number of bytes from the socket into the
    //+ given buffer.
    //
    //! PRE: The socket implementation pointer is valid, and the buffer is at
    //+      least length bytes long.
    //! POST: The given buffer has length bytes copied into it from the socket
    //+       bufffer, and the number of bytes read successfully is returned to
    //+       the caller.
    //
    //! ARGS: buffer - A pointer to the buffer where the socket's buffer
    //+                contents are to be stored.
    //! ARGBS: length - The number of bytes to be read.
    //
    //! RETURNS: >-1 - The number of bytes successfully read from the socket.
    //! RETURNS:  -1 - An error occurred when reading.
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    read (void* buffer, const size_t length) {
        return m_socket_imp->read(buffer, length);
    }

    // ------------------------------------------------------------------------
    //: Read exactly the specified number of bytes from the socket into the
    //+ given buffer.
    //
    //! PRE: The socket implementation pointer is valid, and the buffer is at
    //+      least length bytes long.
    //! POST: The given buffer has length bytes copied into it from the socket
    //+       buffer, and the number of bytes read successfully is returned to
    //+       the caller.
    //
    //! ARGS: buffer - A pointer to the buffer where the socket's buffer
    //+                contents are to be stored.
    //! ARGS: length - The number of bytes to be read.
    //
    //! RETURNS: >-1 - The number of bytes successfully read from the socket.
    //! RETURNS:  -1 - An error occurred when reading.
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    readn (void* buffer, const size_t length) {
        return m_socket_imp->readn(buffer, length);
    }

    // ------------------------------------------------------------------------
    //: Write the buffer to the socket.
    //
    //! PRE: The socket implementation pointer is valid.
    //! POST: The given buffer is written to the socket, and the number of
    //+       bytes written successfully is returned to the caller.
    //
    //! ARGS: buffer - A pointer to the buffer to be written.
    //! ARGS: length - The length of the buffer.
    //
    //! RETURNS: >-1 - The number of bytes successfully written to the socket.
    //! RETURNS:  -1 - An error occurred when writing.
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    write (const void* buffer, const size_t length) {
        return m_socket_imp->write(buffer, length);
    }

    // ========================================================================
    // Socket interface.
    // ========================================================================

    // ------------------------------------------------------------------------
    //: Connect the socket on the client side to the server side.  For a
    //+ datagram socket, this makes the address given to the constructor the
    //+ default destination for all packets.  For a stream socket, this has
    //+ the effect of establishing a connection with the destination.
    //
    //! PRE: The socket is open.
    //! POST: The socket is connected to the address in m_host_addr.  For a
    //+       stream socket, this means that a connection for future
    //+       communication has been established.  For a datagram socket, the
    //+       default destination for all packets is now m_host_addr.
    //
    //! RETURNS: true  - The connection was made.
    //! RETURNS: false - The connect could not be made.  An error message is
    //+                  printed explaining what happened.
    // ------------------------------------------------------------------------
    inline virtual bool
    connect (void) {
        return m_socket_imp->connect();
    }

    // ------------------------------------------------------------------------
    //: Get the port for this socket.
    //
    //! PRE: The socket implementation pointer is valid.
    //! POST: The port number for m_socket_imp is returned to the caller.
    //
    //! RETURNS: An unsigned short (16-bit value) giving the port number for
    //+          this socket.
    // ------------------------------------------------------------------------
    inline unsigned short
    getPort (void) const {
        return m_socket_imp->getPort();
    }

protected:
    // ------------------------------------------------------------------------
    //: Default constructor.  The socket address is set to "INADDRY_ANY", and
    //+ the implementation socket is initialized to NULL.
    //
    //! PRE: None.
    //! POST: "INADDR_ANY" is passed on to the vpr::BlockIO constructor, and
    //+       m_socket_imp is set to NULL.
    // ------------------------------------------------------------------------
    Socket_t (void)
        : BlockIO(std::string("INADDR_ANY")), m_socket_imp(NULL)
    {
        /* Do nothing. */ ;
    }

    // ------------------------------------------------------------------------
    //: Constructor.  The socket address is set to the given address, and the
    //+ implementation socket is initialized to NULL.
    //
    //! PRE: None.
    //! POST: address is passed on to the vpr::BlockIO constructor, and
    //+       m_socket_imp is set to NULL.
    //
    //! ARGS: address - The address string for this socket object.
    // ------------------------------------------------------------------------
    Socket_t (const std::string& address)
        : BlockIO(address), m_socket_imp(NULL)
    {
        /* Do nothing. */ ;
    }

    // ------------------------------------------------------------------------
    //: Destructor.  If the socket implementation object pointer is non-NULL,
    //+ its memory is released.
    //
    //! PRE: None.
    //! POST: If m_socket_imp is non-NULL, its memory is released.
    // ------------------------------------------------------------------------
    virtual ~Socket_t (void) {
        /* Do nothing. */ ;
    }

    RealSocketImp* m_socket_imp; //: Platform-specific socket implementation
                                 //+ object
};

}; // End of vpr namespace

#if defined(VPR_USE_NSPR)
#   include <md/NSPR/SocketImpNSPR.h>

namespace vpr {
  typedef Socket_t<SocketImpNSPR> Socket;
};
#elif defined(VPR_USE_WIN32)
#   include <md/WIN32/SocketImpWinSock.h>

namespace vpr {
  typedef Socket_t<SocketImpWinSock> Socket;
};
#else
#   include <md/POSIX/SocketImpBSD.h>

namespace vpr {
  typedef Socket_t<SocketImpBSD> Socket;
};
#endif


#endif  /* _VPR_SOCKET_H_ */
