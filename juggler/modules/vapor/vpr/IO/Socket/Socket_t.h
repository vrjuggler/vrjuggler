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

#ifndef _VPR_SOCKET_BRIDGE_H_
#define _VPR_SOCKET_BRIDGE_H_
// NOTE: this is the bridge class for use with Socket.h

#include <vpr/vprConfig.h>

#include <string>

#include <vpr/IO/BlockIO.h> // base class.
#include <vpr/IO/IOSys.h>

#include <vpr/Util/Interval.h>
#include <vpr/Util/Status.h>
#include <vpr/Util/Assert.h>


namespace vpr {

// ----------------------------------------------------------------------------
//: Cross-platform block-based socket interface.  vpr::Socket objects cannot
//+ be instantiated.  Instead, see vpr::SocketStream and vpr::SocketDatagram.
// ----------------------------------------------------------------------------
//!PUBLIC_API:
template<class RealSocketImpl>
class Socket_t : public BlockIO {
public:
    // ========================================================================
    // Block I/O interface.
    // ========================================================================

    // ------------------------------------------------------------------------
    //: Get the "name" of this socket.  It is typically the address of the
    //+ peer host.
    //
    //! PRE: None.
    //! POST:
    //
    //! RETURNS: An object containing the "name" of this socket.
    // ------------------------------------------------------------------------
    virtual const std::string&
    getName (void) {
        return m_socket_imp->getName();
    }

    // ------------------------------------------------------------------------
    //: Set the open flags so that the socket is opened in read-only mode.
    //
    //! PRE: None.
    //! POST: The open flags are updated so that when the socket is opened, it
    //+       is opened in read-only mode.  If the socket is already open,
    //+       this has no effect.
    // ------------------------------------------------------------------------
    inline void
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
    inline void
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
    inline void
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
    inline void
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
    inline void
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
    inline vpr::Status
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
    inline vpr::Status
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
    inline bool
    isOpen (void) {
        return m_socket_imp->isOpen();
    }

    // ------------------------------------------------------------------
    //: Get the status of a possibly connected socket
    //
    //! PRE: None
    //! RETURNS: true - The socket is connected to a remote addr
    //! RETURNS: false - The socket is not currently connect (the other side may have disconnected)
    // -----------------------------------------------------------------
    bool isConnected()
    {
        return m_socket_imp->isConnected();
    }

    //: Get the handle to this socket
    IOSys::Handle getHandle()
    {
        return m_socket_imp->getHandle();
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
    inline vpr::Status
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
    inline vpr::Status
    enableNonBlocking (void) {
        return m_socket_imp->enableNonBlocking();
    }

    // ------------------------------------------------------------------------
    //: Get the current blocking state for the socket.
    //
    //! PRE: m_blocking is set correctly
    //! POST:
    //
    //! RETURNS: true  - The socket is in blocking mode.
    //! RETURNS: false - The socket is in non-blocking mode.
    // ------------------------------------------------------------------------
    inline bool
    getBlocking (void) const {
        return m_socket_imp->getBlocking();
    }

    // ------------------------------------------------------------------------
    //: Get the current non-blocking state for the socket.
    //
    //! PRE: m_blocking is set correctly
    //! POST:
    //
    //! RETURNS: true  - The socket is in non-blocking mode.
    //! RETURNS: false - The socket is in blocking mode.
    // ------------------------------------------------------------------------
    inline bool
    getNonBlocking (void) const {
        return m_socket_imp->getNonBlocking();
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
    inline vpr::Status
    bind (void) {
        return m_socket_imp->bind();
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
    inline vpr::Status
    connect (const vpr::Interval timeout = vpr::Interval::NoTimeout) {
        return m_socket_imp->connect(timeout);
    }

    // ------------------------------------------------------------------------
    //: Receive at most the specified number of bytes from the socket into the
    //+ given buffer.
    //
    //! PRE: The socket is open for reading, and the buffer is at least length
    //+      bytes long.
    //! POST: The given buffer has length bytes copied into it from the
    //+       socket, and the number of bytes read successfully is returned to
    //+       the caller.
    //
    //! ARGS: buffer - A pointer to the buffer where the socket's buffer
    //+                contents are to be stored.
    //! ARGS: length - The number of bytes to be read.
    //
    //! RETURNS: >-1 - The number of bytes successfully read from the socket.
    //! RETURNS:  -1 - An error occurred when reading.
    // ------------------------------------------------------------------------
    inline Status
    recv (void* buffer, const size_t length, ssize_t& bytes_read, const vpr::Interval timeout = vpr::Interval::NoTimeout) {
        return read(buffer, length, bytes_read, timeout);
    }

    // ------------------------------------------------------------------------
    //: Receive at most the specified number of bytes from the socket into the
    //+ given buffer.
    //
    //! PRE: The socket is open for reading, and the buffer is at least length
    //+      bytes long.
    //! POST: The given buffer has length bytes copied into it from the
    //+       socket, and the number of bytes read successfully is returned to
    //+       the caller.
    //
    //! ARGS: buffer - A reference to the buffer (a std::string object) where
    //+                the socket's buffer contents are to be stored.
    //! ARGS: length - The number of bytes to be read.
    //
    //! RETURNS: >-1 - The number of bytes successfully read from the socket.
    //! RETURNS:  -1 - An error occurred when reading.
    // ------------------------------------------------------------------------
    inline Status
    recv (std::string& buffer, const size_t length, ssize_t& bytes_read, const vpr::Interval timeout = vpr::Interval::NoTimeout) {
       return read(buffer, length, bytes_read, timeout);
    }

    // ------------------------------------------------------------------------
    //: Receive at most the specified number of bytes from the socket into the
    //+ given buffer.
    //
    //! PRE: The socket is open for reading, and the buffer is at least length
    //+      bytes long.
    //! POST: The given buffer has length bytes copied into it from the
    //+       socket, and the number of bytes read successfully is returned to
    //+       the caller.
    //
    //! ARGS: buffer - A reference to the buffer (a std::vector<vpr::Uint8> object)
    //+                where the socket's buffer contents are to be stored.
    //! ARGS: length - The number of bytes to be read.
    //
    //! RETURNS: >-1 - The number of bytes successfully read from the socket.
    //! RETURNS:  -1 - An error occurred when reading.
    // ------------------------------------------------------------------------
    inline Status
    recv (std::vector<vpr::Uint8>& buffer, const size_t length, ssize_t& bytes_read, const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
       return read(buffer, length, bytes_read, timeout);
    }

    // ------------------------------------------------------------------------
    //: Read exactly the specified number of bytes from the socket into the
    //+ given buffer.
    //
    //! PRE: The socket is open for reading, and the buffer is at least length
    //+      bytes long.
    //! POST: The given buffer has length bytes copied into it from the
    //+       socket, and the number of bytes read successfully is returned to
    //+       the caller.
    //
    //! ARGS: buffer - A pointer to the buffer where the socket's buffer
    //+                contents are to be stored.
    //! ARGS: length - The number of bytes to be read.
    //
    //! RETURNS: >-1 - The number of bytes successfully read from the socket.
    //! RETURNS:  -1 - An error occurred when reading.
    // ------------------------------------------------------------------------
    inline Status
    recvn (void* buffer, const size_t length, ssize_t& bytes_read, const vpr::Interval timeout = vpr::Interval::NoTimeout) {
        return readn(buffer, length, bytes_read, timeout);
    }

    // ------------------------------------------------------------------------
    //: Read exactly the specified number of bytes from the socket into the
    //+ given buffer.
    //
    //! PRE: The socket is open for reading, and the buffer is at least length
    //+      bytes long.
    //! POST: The given buffer has length bytes copied into it from the
    //+       socket, and the number of bytes read successfully is returned to
    //+       the caller.
    //
    //! ARGS: buffer - A reference to the buffer (a std::string object) where
    //+                the socket's buffer contents are to be stored.
    //! ARGS: length - The number of bytes to be read.
    //
    //! RETURNS: >-1 - The number of bytes successfully read from the socket.
    //! RETURNS:  -1 - An error occurred when reading.
    // ------------------------------------------------------------------------
    inline Status
    recvn (std::string& buffer, const size_t length, ssize_t& bytes_read, const vpr::Interval timeout = vpr::Interval::NoTimeout) {
        return readn(buffer, length, bytes_read, timeout);
    }

    // ------------------------------------------------------------------------
    //: Read exactly the specified number of bytes from the socket into the
    //+ given buffer.
    //
    //! PRE: The socket is open for reading, and the buffer is at least length
    //+      bytes long.
    //! POST: The given buffer has length bytes copied into it from the
    //+       socket, and the number of bytes read successfully is returned to
    //+       the caller.
    //
    //! ARGS: buffer - A reference to the buffer (a std::vector<vpr::Uint8> object)
    //+                where the socket's buffer contents are to be stored.
    //! ARGS: length - The number of bytes to be read.
    //
    //! RETURNS: >-1 - The number of bytes successfully read from the socket.
    //! RETURNS:  -1 - An error occurred when reading.
    // ------------------------------------------------------------------------
    inline Status
    recvn (std::vector<vpr::Uint8>& buffer, const size_t length, ssize_t& bytes_read, const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
        return readn(buffer, length, bytes_read, timeout);
    }

    // ------------------------------------------------------------------------
    //: Send the buffer to the remote side of the socket.
    //
    //! PRE: The socket is open for writing.
    //! POST: The given buffer is written to the socket, and the number of
    //+       bytes written successfully is returned to the caller.
    //
    //! ARGS: buffer - A pointer to the buffer to be written.
    //! ARGS: length - The length of the buffer.
    //
    //! RETURNS: >-1 - The number of bytes successfully written to the socket.
    //! RETURNS:  -1 - An error occurred when writing.
    // ------------------------------------------------------------------------
    inline Status
    send (const void* buffer, const size_t length, ssize_t& bytes_written, const vpr::Interval timeout = vpr::Interval::NoTimeout) {
        return write(buffer, length, bytes_written, timeout);
    }

    // ------------------------------------------------------------------------
    //: Send the buffer to the remote side of the socket.
    //
    //! PRE: The socket is open for writing.
    //! POST: The given buffer is written to the socket, and the number of
    //+       bytes written successfully is returned to the caller.
    //
    //! ARGS: buffer - A reference to the buffer (a std::string object) to be
    //+                written.
    //! ARGS: length - The length of the buffer.
    //
    //! RETURNS: >-1 - The number of bytes successfully written to the socket.
    //! RETURNS:  -1 - An error occurred when writing.
    // ------------------------------------------------------------------------
    inline Status
    send (const std::string& buffer, const size_t length,
          ssize_t& bytes_written,
          const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
       vprASSERT( length <= buffer.size() && "length was bigger than the data given" );
       return write(buffer, length, bytes_written, timeout);
    }

    // ------------------------------------------------------------------------
    //: Send the buffer to the remote side of the socket.
    //
    //! PRE: The socket is open for writing.
    //! POST: The given buffer is written to the socket, and the number of
    //+       bytes written successfully is returned to the caller.
    //
    //! ARGS: buffer - A reference to the buffer (a std::vector<vpr::Uint8> object)
    //+                to be written.
    //! ARGS: length - The length of the buffer.
    //
    //! RETURNS: >-1 - The number of bytes successfully written to the socket.
    //! RETURNS:  -1 - An error occurred when writing.
    // ------------------------------------------------------------------------
    inline Status
    send (const std::vector<vpr::Uint8>& buffer, const size_t length,
          ssize_t& bytes_written,
          const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
       vprASSERT( length <= buffer.size() && "length was bigger than the data given" );
       return write(buffer, length, bytes_written,timeout);
    }

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
        return m_socket_imp->getType();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline const InetAddr&
    getLocalAddr (void) const {
        return m_socket_imp->getLocalAddr();
    }

    inline Status
    setLocalAddr (const InetAddr& addr) {
      return m_socket_imp->setLocalAddr(addr);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline const InetAddr&
    getRemoteAddr (void) const {
        return m_socket_imp->getRemoteAddr();
    }

    inline Status
    setRemoteAddr (const InetAddr& addr) {
       return m_socket_imp->setRemoteAddr(addr);
    }

    /**
     *
     */
    inline Status
    getKeepAlive (bool& enabled) const {
        return m_socket_imp->getKeepAlive(enabled);
    }

    /**
     *
     */
    inline Status
    setKeepAlive (const bool enable_val) {
        return m_socket_imp->setKeepAlive(enable_val);
    }

    /**
     *
     */
    inline Status
    getLingerOnClose (bool& enabled, int& linger_sec) const {
        return m_socket_imp->getLingerOnClose(enabled, linger_sec);
    }

    /**
     *
     */
    inline Status
    setLingerOnClose (const bool enable_val, const int linger_sec) {
        return m_socket_imp->setLingerOnClose(enable_val, linger_sec);
    }

    /**
     *
     */
    inline Status
    getRecvBufferSize (Int32& size) const {
        return m_socket_imp->getRecvBufferSize(size);
    }

    /**
     *
     */
    inline Status
    setRecvBufferSize (const Int32 size) {
        return m_socket_imp->setRecvBufferSize(size);
    }

    /**
     *  the default max size that you can send using this socket
     *  you can change the max size with setSendBufferSize
     */
    inline Status
    getSendBufferSize (int& size) const {
        return m_socket_imp->getSendBufferSize(size);
    }

    /**
     *
     */
    inline Status
    setSendBufferSize (const Int32 size) {
        return m_socket_imp->setSendBufferSize(size);
    }

    /**
     *
     */
    inline Status
    getReuseAddr (bool& enabled) const {
        return m_socket_imp->getReuseAddr(enabled);
    }

    /**
     * Enable reuse of the address that will be bound by this socket.
     *
     * PRE: The socket has been opened, but bind() has not been called.
     */
    inline Status
    setReuseAddr (const bool enable_val) {
        return m_socket_imp->setReuseAddr(enable_val);
    }

#ifdef FIX_TOS_LATER
    /**
     *
     */
    inline Status
    getTypeOfService (SocketOptions::TypeOfService& tos) {
        return m_socket_imp->getTypeOfService(tos);
    }

    /**
     *
     */
    inline Status
    setTypeOfService (const SocketOptions::TypeOfService& tos) {
        return m_socket_imp->setTypeOfService(tos);
    }
#endif

    /**
     *
     */
    inline Status
    getTimeToLive (Int32& ttl) {
        return m_socket_imp->getTimeToLive(ttl);
    }

    /**
     *
     */
    inline Status
    setTimeToLive (const Int32 ttl) {
        return m_socket_imp->setTimeToLive(ttl);
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
    virtual Status
    read_i (void* buffer, const size_t length,
            ssize_t& bytes_read, const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
        return m_socket_imp->read(buffer, length, bytes_read, timeout);
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
    virtual Status
    readn_i (void* buffer, const size_t length,
             ssize_t& bytes_read, const vpr::Interval timeout = vpr::Interval::NoTimeout) {
        return m_socket_imp->readn(buffer, length, bytes_read, timeout);
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
    virtual Status
    write_i (const void* buffer, const size_t length,
             ssize_t& bytes_written, const vpr::Interval timeout = vpr::Interval::NoTimeout) {
        return m_socket_imp->write(buffer, length, bytes_written, timeout);
    }

    RealSocketImpl* m_socket_imp; //: Platform-specific socket implementation
                                  //+ object
};

}; // End of vpr namespace


#endif  /* _VPR_SOCKET_BRIDGE_H_ */
