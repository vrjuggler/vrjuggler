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

#include <vpr/IO/Stats/BaseIOStatsStrategy.h>

#include <typeinfo>


namespace vpr {

/**
 * Cross-platform block-based socket interface.  <code>vpr::Socket</code>
 * objects cannot be instantiated.  Instead, see <code>vpr::SocketStream</code>
 * and <code>vpr::SocketDatagram</code>.
 *
 * @author Patrick Hartling
 * @author Allen Bierbaum
 * @author Kevin Meinert
 *
 * @see vpr::SocketStream_t
 * @see vpr::SocketDatagram_t
 */
template<class RealSocketImpl, class IO_STATS_STRATEGY = NullIOStatsStrategy>
class Socket_t : public vpr::BlockIO {
public:
    // ========================================================================
    // Block I/O interface.
    // ========================================================================

    /**
     * Gets the "name" of this socket.  It is typically the address of the
     * peer host.
     *
     * @pre None.
     * @post
     *
     * @return An object containing the "name" of this socket.*/
    virtual const std::string&
    getName (void) {
        return m_socket_imp->getName();
    }

    /**
     * Sets the blocking flags so that the socket is opened in blocking mode.
     *
     * @pre None.
     * @post The open flags are updated so that when the socket is opened, it
     *       is opened in blocking mode.  If the socket is already open, this
     *       has no effect.
     */
    inline void
    setOpenBlocking (void) {
        m_socket_imp->setOpenBlocking();
    }

    /**
     * Sets the blocking flags so that the socket is opened in non-blocking
     * mode.
     *
     * @pre None.
     * @post The open flags are updated so that when the socket is opened, it
     *       is opened in non-blocking mode.  If the socket is already open,
     *       this has no effect.
     */
    inline void
    setOpenNonBlocking (void) {
        m_socket_imp->setOpenNonBlocking();
    }

    /**
     * Opens the socket.
     *
     * @pre The socket is not already open.
     * @post An attempt is made to open the socket.  The resulting status is
     *       returned to the caller.  If the socket is opened, m_open is set
     *       to true.
     *
     * @return <code>vpr::Status::Success</code> is returned if this socket is
     *         opened successfully.<br>
     *         <code>vpr::Status::Failure</code> is returned otherwise.
     */
    inline vpr::Status
    open (void) {
        return m_socket_imp->open();
    }

    /**
     * Closes the socket.
     *
     * @pre The socket is open.
     * @post An attempt is made to close the socket.  The resulting status
     *       is returned to the caller.  If the socket is closed, m_open
     *       is set to false.
     *
     * @return <code>vpr::Status::Success</code> is returned if this socket is
     *         closed successfully.<br>
     *         <code>vpr::Status::Failure</code> is returned otherwise.
     */
    inline vpr::Status
    close (void) {
        return m_socket_imp->close();
    }

    /**
     * Gets the open state of this socket.
     *
     * @pre None.
     * @post The boolean value giving the open state is returned to the
     *       caller.
     *
     * @return <code>true</code> is returned if this socket is open;
     *         <code>false</code> otherwise.
     */
    inline bool
    isOpen (void) {
        return m_socket_imp->isOpen();
    }

    /**
     * Gets the status of a possibly connected socket
     *
     * @pre None
     *
     * @return <code>true</code> is returned if this socket is connected to a
     *         remote address.<br>
     *         <code>false</code> is returned if this socket is not currently
     *         connected (the other wise may have disconnected).
     */
    bool isConnected()
    {
        return m_socket_imp->isConnected();
    }

    /// Gets the handle to this socket
    IOSys::Handle getHandle()
    {
        return m_socket_imp->getHandle();
    }

    /**
     * Reconfigures the socket so that it is in blocking mode.
     *
     * @pre The socket is open.
     * @post Processes will block when accessing the socket.
     *
     * @return <code>vpr::Status::Success</code> is returned if the socket's
     *         blocking mode is set to blocking.<br>
     *         <code>vpr::Status::Failure</code> is returned otherwise.
     */
    inline vpr::Status
    enableBlocking (void) {
        return m_socket_imp->enableBlocking();
    }

    /**
     * Reconfigures the socket so that it is in non-blocking mode.
     *
     * @pre The socket is open.
     * @post Processes will not block when accessing the socket.
     *
     * @return <code>vpr::Status::Success</code> is returned if the socket's
     *         blocking mode is set to non-blocking.<br>
     *         <code>vpr::Status::Failure</code> is returned otherwise.
     */
    inline vpr::Status
    enableNonBlocking (void) {
        return m_socket_imp->enableNonBlocking();
    }

    /**
     * Gets the current blocking state for the socket.
     *
     * @pre m_blocking is set correctly
     * @post
     *
     * @return <code>true</code> is returned if the socket is in blocking
     *         mode.  Otherwise, <code>false</code> is returned.
     */
    inline bool
    getBlocking (void) const {
        return m_socket_imp->getBlocking();
    }

    /**
     * Gets the current non-blocking state for the socket.
     *
     * @pre m_blocking is set correctly
     * @post
     *
     * @return <code>true</code> is returned if the socket is in non-blocking
     *         mode.  Otherwise, <code>false</code> is returned.
     */
    inline bool
    getNonBlocking (void) const {
        return m_socket_imp->getNonBlocking();
    }

    /**
     * Binds this socket to the address in the host address member variable.
     *
     * @pre The socket is open.
     * @post The socket is bound to the address defined in the constructor.
     *
     * @return <code>vpr::Status::Success</code> is returned if this socket was
     *         bound to its designated local address successfully.<br>
     *         <code>vpr::Status::Failure</code> is returned otherwise.
     */
    inline vpr::Status
    bind (void) {
        return m_socket_imp->bind();
    }

    // ========================================================================
    // Socket interface.
    // ========================================================================

    /**
     * Connects the socket on the client side to the server side.  For a
     * datagram socket, this makes the address given to the constructor the
     * default destination for all packets.  For a stream socket, this has
     * the effect of establishing a connection with the destination.
     *
     * @pre The socket is open.
     * @post The socket is connected to the address in m_host_addr.  For a
     *       stream socket, this means that a connection for future
     *       communication has been established.  For a datagram socket, the
     *       default destination for all packets is now m_host_addr.
     *
     * @return <code>vpr::Status::Success</code> is returned if the connection
     *         succeeded.<br>
     *         <code>vpr::Status::InProgress</code> is returned if this is a
     *         non-blocking socket and the connection is still in progress.
     *         The connection will be completed "in the background".<br>
     *         <code>vpr::Status::Timeout</code> is returned if the connection
     *         could not be made within the given timeout interval.<br>
     *         <code>vpr::Status::Failure</code> is returned if the connection
     *         could not be made.
     */
    inline vpr::Status
    connect (const vpr::Interval timeout = vpr::Interval::NoTimeout) {
        return m_socket_imp->connect(timeout);
    }

    /**
     * Receives at most the specified number of bytes from the socket into the
     * given buffer.
     *
     * @pre The socket is open for reading, and the buffer is at least
     *      <code>length</code> bytes long.
     * @post The given buffer has <code>length</code> bytes copied into it from
     *       the socket, and the number of bytes read successfully is returned
     *       to the caller via the <code>bytes_read</code> argument.
     *
     * @return <code>vpr::Status::Success</code> is returned if the read
     *         operation completed successfully.<br>
     *         <code>vpr::Status::Success</code> is returned if the read
     *         operation failed.<br>
     *         <code>vpr::Status::InProgress</code> if the device is in
     *         non-blocking mode, and the read operation is in progress.<br>
     *         <code>vpr::Status::Timeout</code> is returned if the read
     *         could not begin within the timeout interval.
     * @param buffer     A pointer to the buffer where the socket's buffer
     * contents are to be stored.
     * @param length     The number of bytes to be read.
     * @param bytes_read The number of bytes read into the buffer.
     * @param timeout    The maximum amount of time to wait for data to be
     * available for reading.  This argument is optional and defaults to
     * <code>vpr::Interval::NoTimeout</code>.
     */
    inline vpr::Status
    recv (void* buffer, const size_t length, ssize_t& bytes_read,
          const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
        return read(buffer, length, bytes_read, timeout);
    }

    /**
     * Receives at most the specified number of bytes from the socket into the
     * given buffer.
     *
     * @pre The device is open for reading, and the buffer is at least
     *      <code>length</code> bytes long.
     * @post The given buffer has <code>length</code> bytes copied into it
     *       from the device, and the number of bytes read successfully is
     *       returned to the caller via the <code>bytes_read</code> argument.
     *
     * @param buffer     A reference to the buffer (a <code>std::string</code>
     *                   object) where the device's buffer contents are to be
     *                   stored.
     * @param length     The number of bytes to be read.
     * @param bytes_read The number of bytes read into the buffer.
     * @param timeout    The maximum amount of time to wait for data to be
     *                   available for reading.  This argument is optional
     *                   and defaults to <code>vpr::Interval::NoTimeout</code>.
     *
     * @return <code>vpr::Status::Success</code> is returned if the read
     *         operation completed successfully.<br>
     *         <code>vpr::Status::Success</code> is returned if the read
     *         operation failed.<br>
     *         <code>vpr::Status::InProgress</code> if the device is in
     *         non-blocking mode, and the read operation is in progress.<br>
     *         <code>vpr::Status::Timeout</code> is returned if the read
     *         could not begin within the timeout interval.
     */
    inline vpr::Status
    recv (std::string& buffer, const size_t length, ssize_t& bytes_read,
          const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
       return read(buffer, length, bytes_read, timeout);
    }

    /**
     * Receives at most the specified number of bytes from the socket into the
     * given buffer.
     *
     * @pre The device is open for reading, and the buffer is at least
     *      <code>length</code> bytes long.
     * @post The given buffer has <code>length</code> bytes copied into it
     *       from the device, and the number of bytes read successfully is
     *       returned to the caller via the <code>bytes_read</code> argument.
     *
     * @param buffer     A pointer to the buffer (a vector of
     *                   <code>char</code>s) where the device's buffer
     *                   contents are to be stored.
     * @param length     The number of bytes to be read.
     * @param bytes_read The number of bytes read into the buffer.
     * @param timeout    The maximum amount of time to wait for data to be
     *                   available for reading.  This argument is optional
     *                   and defaults to <code>vpr::Interval::NoTimeout</code>.
     *
     * @return <code>vpr::Status::Success</code> is returned if the read
     *         operation completed successfully.<br>
     *         <code>vpr::Status::Success</code> is returned if the read
     *         operation failed.<br>
     *         <code>vpr::Status::InProgress</code> if the device is in
     *         non-blocking mode, and the read operation is in progress.<br>
     *         <code>vpr::Status::Timeout</code> is returned if the read
     *         could not begin within the timeout interval.
     */
    inline vpr::Status
    recv (std::vector<vpr::Uint8>& buffer, const size_t length,
          ssize_t& bytes_read,
          const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
       return read(buffer, length, bytes_read, timeout);
    }

    /**
     * Receives exactly the specified number of bytes from the socket into the
     * given buffer.
     *
     * @pre The device is open for reading, and the buffer is at least
     *      <code>length</code> bytes long.
     * @post The given buffer has <code>length</code> bytes copied into it from
     *       the device, and the number of bytes read successfully is returned
     *       to the caller via the <code>bytes_read</code> parameter.
     *
     * @param buffer     A pointer to the buffer where the device's buffer
     *                   contents are to be stored.
     * @param length     The number of bytes to be read.
     * @param bytes_read The number of bytes read into the buffer.
     * @param timeout    The maximum amount of time to wait for data to be
     *                   available for reading.  This argument is optional
     *                   and defaults to <code>vpr::Interval::NoTimeout</code>.
     *
     * @return <code>vpr::Status::Success</code> is returned if the read
     *         operation completed successfully.<br>
     *         <code>vpr::Status::Success</code> is returned if the read
     *         operation failed.<br>
     *         <code>vpr::Status::InProgress</code> if the device is in
     *         non-blocking mode, and the read operation is in progress.
     */
    inline vpr::Status
    recvn (void* buffer, const size_t length, ssize_t& bytes_read,
           const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
        return readn(buffer, length, bytes_read, timeout);
    }

    /**
     * Receives exactly the specified number of bytes from the socket into the
     * given buffer.
     *
     * @pre The device is open for reading, and the buffer is at least
     *      <code>length</code> bytes long.
     * @post The given buffer has <code>length</code> bytes copied into it from
     *       the device, and the number of bytes read successfully is returned
     *       to the caller via the <code>bytes_read</code> parameter.
     *
     * @param buffer     A reference to the buffer (a <code>std::string</code>
     *                   object) where the device's buffer contents are to be
     *                   stored.
     * @param length     The number of bytes to be read.
     * @param bytes_read The number of bytes read into the buffer.
     * @param timeout    The maximum amount of time to wait for data to be
     *                   available for reading.  This argument is optional
     *                   and defaults to <code>vpr::Interval::NoTimeout</code>.
     *
     * @return <code>vpr::Status::Success</code> is returned if the read
     *         operation completed successfully.
     *         <code>vpr::Status::Success</code> is returned if the read
     *         operation failed.  <code>vpr::Status::InProgress</code> if the
     *         device is in non-blocking mode, and the read operation is in
     *         progress.
     */
    inline vpr::Status
    recvn (std::string& buffer, const size_t length, ssize_t& bytes_read,
           const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
        return readn(buffer, length, bytes_read, timeout);
    }

    /**
     * Receives exactly the specified number of bytes from the socket into the
     * given buffer.
     *
     * @pre The device is open for reading, and the buffer is at least
     *      <code>length</code> bytes long.
     * @post The given buffer has <code>length</code> bytes copied into it from
     *       the device, and the number of bytes read successfully is returned
     *       to the caller.
     *
     * @param buffer     A pointer to the buffer (a vector of
     *                   <code>char</code>s) where the device's buffer contents
     *                   are to be stored.
     * @param length     The number of bytes to be read.
     * @param bytes_read The number of bytes read into the buffer.
     * @param timeout    The maximum amount of time to wait for data to be
     *                   available for reading.  This argument is optional
     *                   and defaults to <code>vpr::Interval::NoTimeout</code>.
     *
     * @return <code>vpr::Status::Success</code> is returned if the read
     *         operation completed successfully.<br>
     *         <code>vpr::Status::Success</code> is returned if the read
     *         operation failed.<br>
     *         <code>vpr::Status::InProgress</code> if the device is in
     *         non-blocking mode, and the read operation is in progress.<br>
     *         <code>vpr::Status::Timeout</code> is returned if the read
     *         could not begin within the timeout interval.
     */
    inline vpr::Status
    recvn (std::vector<vpr::Uint8>& buffer, const size_t length,
           ssize_t& bytes_read,
           const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
        return readn(buffer, length, bytes_read, timeout);
    }

    /**
     * Sends the buffer to the remote side of the socket.
     *
     * @pre The device is open for writing.
     * @post The given buffer is written to the I/O device, and the number of
     *       bytes written successfully is returned to the caller via the
     *       <code>bytes_written</code> parameter.
     *
     * @param buffer        A pointer to the buffer to be written.
     * @param length        The length of the buffer.
     * @param bytes_written The number of bytes written to the device.
     * @param timeout       The maximum amount of time to wait for data to be
     *                      available for writing.  This argument is optional
     *                      and defaults to
     *                      <code>vpr::Interval::NoTimeout</code>.
     *
     * @return <code>vpr::Status::Success</code> is returned if the write
     *         operation completed successfully.<br>
     *         <code>vpr::Status::Failure</code> is returned if the write
     *         operation failed.
     *         <code>vpr::Status::InProgress</code> is returned if the handle
     *         is in non-blocking mode, and the write operation is in
     *         progress.<br>
     *         <code>vpr::Status::Timeout</code> is returned if the write
     *         could not begin within the timeout interval.
     */
    inline vpr::Status
    send (const void* buffer, const size_t length, ssize_t& bytes_written,
          const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
        return write(buffer, length, bytes_written, timeout);
    }

    /**
     * Sends the buffer to the remote side of the socket.
     *
     * @pre The device is open for writing.
     * @post The given buffer is written to the I/O device, and the number of
     *       bytes written successfully is returned to the caller via the
     *       <code>bytes_written</code> parameter.
     *
     * @param buffer        A reference to the buffer (a std::string object)
     *                      to be written.
     * @param length        The length of the buffer.
     * @param bytes_written The number of bytes written to the device.
     * @param timeout       The maximum amount of time to wait for data to be
     *                      available for writing.  This argument is optional
     *                      and defaults to
     *                      <code>vpr::Interval::NoTimeout</code>.
     *
     * @return <code>vpr::Status::Success</code> is returned if the write
     *         operation completed successfully.<br>
     *         <code>vpr::Status::Failure</code> is returned if the write
     *         operation failed.
     *         <code>vpr::Status::InProgress</code> is returned if the handle
     *         is in non-blocking mode, and the write operation is in
     *         progress.<br>
     *         <code>vpr::Status::Timeout</code> is returned if the write
     *         could not begin within the timeout interval.
     */
    inline vpr::Status
    send (const std::string& buffer, const size_t length,
          ssize_t& bytes_written,
          const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
       vprASSERT( length <= buffer.size() && "length was bigger than the data given" );
       return write(buffer, length, bytes_written, timeout);
    }

    /**
     * Sends the buffer to the remote side of the socket.
     *
     * @pre The device is open for writing.
     * @post The given buffer is written to the I/O device, and the number of
     *       bytes written successfully is returned to the caller via the
     *       <code>bytes_written</code> parameter.
     *
     * @param buffer        A reference to the buffer (a std::string object)
     *                      to be written.
     * @param length        The length of the buffer.
     * @param bytes_written The number of bytes written to the device.
     * @param timeout       The maximum amount of time to wait for data to be
     *                      available for writing.  This argument is optional
     *                      and defaults to
     *                      <code>vpr::Interval::NoTimeout</code>.
     *
     * @return <code>vpr::Status::Success</code> is returned if the write
     *         operation completed successfully.<br>
     *         <code>vpr::Status::Failure</code> is returned if the write
     *         operation failed.
     *         <code>vpr::Status::InProgress</code> is returned if the handle
     *         is in non-blocking mode, and the write operation is in
     *         progress.<br>
     *         <code>vpr::Status::Timeout</code> is returned if the write
     *         could not begin within the timeout interval.
     * @pre The device is open for writing.
     * @post The given buffer is written to the I/O device, and the number of
     *       bytes written successfully is returned to the caller via the
     *       <code>bytes_written</code> parameter.
     *
     * @param buffer        A pointer to the buffer (a vector of chars) to
     *                      be written.
     * @param length        The length of the buffer.
     * @param bytes_written The number of bytes written to the device.
     * @param timeout       The maximum amount of time to wait for data to be
     *                      available for writing.  This argument is optional
     *                      and defaults to
     *                      <code>vpr::Interval::NoTimeout</code>.
     *
     * @return <code>vpr::Status::Success</code> is returned if the write
     *         operation completed successfully.<br>
     *         <code>vpr::Status::Failure</code> is returned if the write
     *         operation failed.
     *         <code>vpr::Status::InProgress</code> is returned if the handle
     *         is in non-blocking mode, and the write operation is in
     *         progress.<br>
     *         <code>vpr::Status::Timeout</code> is returned if the write
     *         could not begin within the timeout interval.
     */
    inline vpr::Status
    send (const std::vector<vpr::Uint8>& buffer, const size_t length,
          ssize_t& bytes_written,
          const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
       vprASSERT( length <= buffer.size() && "length was bigger than the data given" );
       return write(buffer, length, bytes_written,timeout);
    }

    /**
     * Gets the type of this socket (e.g., vpr::SocketTypes::STREAM).
     *
     * @pre The socket implementation pointer is valid.
     * @post The socket type for m_socket_imp is returned to the caller.
     *
     * @return A vpr::SocketTypes::Type value giving the socket type for
     *         this socket.
     */
    inline const vpr::SocketTypes::Type&
    getType (void) const {
        return m_socket_imp->getType();
    }

    inline const InetAddr&
    getLocalAddr (void) const {
        return m_socket_imp->getLocalAddr();
    }

    inline vpr::Status
    setLocalAddr (const InetAddr& addr) {
      return m_socket_imp->setLocalAddr(addr);
    }

    inline const InetAddr&
    getRemoteAddr (void) const {
        return m_socket_imp->getRemoteAddr();
    }

    inline vpr::Status
    setRemoteAddr (const InetAddr& addr) {
       return m_socket_imp->setRemoteAddr(addr);
    }

    /**
     *
     */
    inline vpr::Status
    getKeepAlive (bool& enabled) const {
        return m_socket_imp->getKeepAlive(enabled);
    }

    /**
     *
     */
    inline vpr::Status
    setKeepAlive (const bool enable_val) {
        return m_socket_imp->setKeepAlive(enable_val);
    }

    /**
     *
     */
    inline vpr::Status
    getLingerOnClose (bool& enabled, int& linger_sec) const {
        return m_socket_imp->getLingerOnClose(enabled, linger_sec);
    }

    /**
     *
     */
    inline vpr::Status
    setLingerOnClose (const bool enable_val, const int linger_sec) {
        return m_socket_imp->setLingerOnClose(enable_val, linger_sec);
    }

    /**
     *
     */
    inline vpr::Status
    getRecvBufferSize (Int32& size) const {
        return m_socket_imp->getRecvBufferSize(size);
    }

    /**
     *
     */
    inline vpr::Status
    setRecvBufferSize (const Int32 size) {
        return m_socket_imp->setRecvBufferSize(size);
    }

    /**
     *  the default max size that you can send using this socket
     *  you can change the max size with setSendBufferSize
     */
    inline vpr::Status
    getSendBufferSize (int& size) const {
        return m_socket_imp->getSendBufferSize(size);
    }

    /**
     *
     */
    inline vpr::Status
    setSendBufferSize (const Int32 size) {
        return m_socket_imp->setSendBufferSize(size);
    }

    /**
     *
     */
    inline vpr::Status
    getReuseAddr (bool& enabled) const {
        return m_socket_imp->getReuseAddr(enabled);
    }

    /**
     * Enables reuse of the address that will be bound by this socket.
     *
     * @pre The socket has been opened, but bind() has not been called.
     */
    inline vpr::Status
    setReuseAddr (const bool enable_val) {
        return m_socket_imp->setReuseAddr(enable_val);
    }

#ifdef FIX_TOS_LATER
    /**
     *
     */
    inline vpr::Status
    getTypeOfService (SocketOptions::TypeOfService& tos) {
        return m_socket_imp->getTypeOfService(tos);
    }

    /**
     *
     */
    inline vpr::Status
    setTypeOfService (const SocketOptions::TypeOfService& tos) {
        return m_socket_imp->setTypeOfService(tos);
    }
#endif

    /**
     *
     */
    inline vpr::Status
    getTimeToLive (Int32& ttl) {
        return m_socket_imp->getTimeToLive(ttl);
    }

    /**
     *
     */
    inline vpr::Status
    setTimeToLive (const Int32 ttl) {
        return m_socket_imp->setTimeToLive(ttl);
    }

protected:
    /**
     * Default constructor.  The socket address is set to "INADDRY_ANY", and
     * the implementation socket is initialized to NULL.
     *
     * @pre None.
     * @post "INADDR_ANY" is passed on to the vpr::BlockIO constructor, and
     *       m_socket_imp is set to NULL.
     */
    Socket_t (void)
        : vpr::BlockIO(std::string("INADDR_ANY")), m_socket_imp(NULL)
    {
       initSocket_t();
    }

    /**
     * Constructor.  The socket address is set to the given address, and the
     * implementation socket is initialized to NULL.
     *
     * @pre None.
     * @post address is passed on to the vpr::BlockIO constructor, and
     *       m_socket_imp is set to NULL.
     *
     * @param address The address string for this socket object.
     */
    Socket_t (const std::string& address)
        : vpr::BlockIO(address), m_socket_imp(NULL)
    {
       initSocket_t();
    }

    /**
     * Destructor.  If the socket implementation object pointer is non-NULL,
     * its memory is released.
     *
     * @pre None.
     * @post If m_socket_imp is non-NULL, its memory is released.
     */
    virtual ~Socket_t (void) {
        ;
    }

    /**
     * Helper function to capture code common between constructors
     */
    void initSocket_t()
    {
        // Allocate stats stategy object
       // First, use template programming magic.  If the stategy is the null one, then don't allocate one
       // NOTE: Because all the info is static the compiler "should" optimize the following if statement away
       if( IO_STATS_STRATEGY::IS_NULL == 0)     // If we have a real one
       {
          //std::cout << "Allocating strategy: " << typeid(IO_STATS_STRATEGY).name() << std::endl;
          BaseIOStatsStrategy* new_strategy = new IO_STATS_STRATEGY;
          this->setIOStatStrategy(new_strategy);
       }
    }

    /**
     * Implementation of the <code>read</code> template method.  This reads at
     * most the specified number of bytes from the socket into the given
     * buffer.
     *
     * @pre The device is open for reading, and the buffer is at least
     *      <code>length</code> bytes long.
     * @post The given buffer has length bytes copied into it from the device,
     *       and the number of bytes read successfully is returned to the
     *       caller via the <code>bytes_read</code> parameter.
     *
     * @param buffer     A pointer to the buffer where the device's buffer
     *                   contents are to be stored.
     * @param length     The number of bytes to be read.
     * @param bytes_read The number of bytes read into the buffer.
     * @param timeout    The maximum amount of time to wait for data to be
     *                   available for reading.  This argument is optional and
     *                   defaults to <code>vpr::Interval::NoTimeout</code>
     *
     * @return vpr::Status::Success is returned if the read operation
     *         completed successfully.<br>
     *         vpr::Status::InProgress if the device is in non-blocking mode,
     *         and the read operation is in progress.<br>
     *         vpr::Status::Timeout is returned if the read could not begin
     *         within the timeout interval.<br>
     *         vpr::Status::Failure is returned if the read operation failed.
     */
    virtual vpr::Status
    read_i (void* buffer, const size_t length, ssize_t& bytes_read,
            const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
        return m_socket_imp->read(buffer, length, bytes_read, timeout);
    }

    /**
     * Implementation of the <code>readn</code> template method.  This reads
     * exactly the specified number of bytes from the socket into the given
     * buffer.
     *
     * @pre The device is open for reading, and the buffer is at least
     *      <code>length</code> bytes long.
     * @post The given buffer has <code>length</code> bytes copied into
     *       it from the device, and the number of bytes read successfully
     *       is returned to the caller via the <code>bytes_read</code>
     *       parameter.
     *
     * @param buffer     A pointer to the buffer where the device's buffer
     *                   contents are to be stored.
     * @param length     The number of bytes to be read.
     * @param bytes_read The number of bytes read into the buffer.
     * @param timeout    The maximum amount of time to wait for data to be
     *                   available for reading.  This argument is optional and
     *                   defaults to <code>vpr::Interval::NoTimeout</code>
     *
     * @return vpr::Status::Success is returned if the read operation completed successfully.
     * @return vpr::Status::InProgress if the device is in
     *         non-blocking mode, and the read operation is in progress.
     * @return vpr::Status::Timeout is returned if the read
     *         could not begin within the timeout interval.
     * @return vpr::Status::Failure is returned if the read operation failed.     
     */
    virtual vpr::Status
    readn_i (void* buffer, const size_t length, ssize_t& bytes_read,
             const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
        return m_socket_imp->readn(buffer, length, bytes_read, timeout);
    }

    /**
     * Implementation of the <code>write</code> template method.  This writes
     * the buffer to the socket.
     *
     * @pre The device is open for writing.
     * @post The given buffer is written to the I/O device, and the number
     *       of bytes written successfully is returned to the caller via the
     *       <code>bytes_written</code> parameter.
     *
     * @param buffer        A pointer to the buffer to be written.
     * @param length        The length of the buffer.
     * @param bytes_written The number of bytes written to the device.
     * @param timeout       The maximum amount of time to wait for data to be
     *                      available for writing.  This argument is optional
     *                      and defaults to
     *                      <code>vpr::Interval::NoTimeout</code>.
     *
     * @return vpr::Status::Success is returned if the write operation
     *         completed successfully.<br>
     *         vpr::Status::InProgress is returned if the handle is in
     *         non-blocking mode, and the write operation is in progress.<br>
     *         vpr::Status::Timeout is returned if the write could not begin
     *         within the timeout interval.<br>
     *         vpr::Status::Failure is returned if the write operation failed.
     */
    virtual vpr::Status
    write_i (const void* buffer, const size_t length, ssize_t& bytes_written,
             const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
        return m_socket_imp->write(buffer, length, bytes_written, timeout);
    }

    /// Platform-specific socket implementation object
    RealSocketImpl* m_socket_imp;
};

}; // End of vpr namespace


#endif  /* _VPR_SOCKET_BRIDGE_H_ */
