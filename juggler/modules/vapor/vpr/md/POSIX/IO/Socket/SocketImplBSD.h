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

#ifndef _VPR_SOCKET_IMPL_BSD_H_
#define _VPR_SOCKET_IMPL_BSD_H_

#include <vpr/vprConfig.h>

#include <sys/types.h>
#include <netinet/in.h>
#include <string>
#include <vector>

#include <vpr/IO/BlockIO.h>
#include <vpr/IO/IOSys.h>
#include <vpr/md/POSIX/IO/FileHandleImplUNIX.h>
#include <vpr/IO/Socket/InetAddr.h>
#include <vpr/IO/Socket/SocketIpOpt.h>
#include <vpr/Util/Assert.h>


namespace vpr {

/**
 * Basic socket wrapper implementation for BSD sockets.
 *
 * @author Patrick Hartling
 */
class SocketImplBSD : public BlockIO, public SocketIpOpt
{
public:
    // ========================================================================
    // vpr::BlockIO overrides.
    // ========================================================================

    /**
     * Opens the socket.  This creates a new socket using the domain and type
     * options set through member variables.
     *
     * @pre m_domain and m_type have been set to values recognized by the
     *      socket(2) system call.
     * @post A new socket is created with its file handle stored in the
     *       m_fdesc member variable.
     *
     * @return vpr::Status::Success is returned if the socket was opened
     *         successfully.<br>
     *         vpr::Status::Failure is returned otherwise.
     */
    virtual Status open(void);

    /**
     * Close the socket.
     *
     * @pre The socket is open.
     * @post An attempt is made to close the socket.  The resulting status is
     *       returned to the caller.
     *
     * @return vpr::Status::Success is returned if the socket was closed
     *         successfully; vpr::Status::Failure otherwise.
     */
    inline virtual Status
    close (void) {
        Status retval;

        retval = m_handle->close();
        m_open = (retval.success() ? false : true);

        return retval;
    }

    /**
     * Binds this socket to the address in the host address member variable.
     *
     * @pre The socket is open, and m_host_addr has been initialized properly.
     * @post The socket is bound to the address in m_host_addr.
     *
     * @return vpr::Status::Sucess is returned if the socket was bound to the
     *         address successfully.<br>
     *         vpr::Status::Failure is returned if the socket could not be
     *         bound to the address in m_host_addr.
     */
    virtual Status bind(void);

    /**
     * Returns the contained handle.
     */
    inline IOSys::Handle
    getHandle (void) {
       return m_handle->m_fdesc;
    }

    /**
     * Reconfigures the socket so that it is in blocking mode.
     *
     * @pre The socket is open.
     * @post Processes will block when accessing the socket.
     *
     * @return vpr::Status::Success is returned if the blocking mode was
     *         changed successfully; vpr::Status::Failure otherwise.
     */
    virtual Status enableBlocking(void);

    /**
     * Reconfigures the socket so that it is in non-blocking mode.
     *
     * @pre The socket is open.
     * @post Processes will not block when accessing the socket.
     *
     * @return vpr::Status::Success is returned if the blocking mode was
     *         changed successfully; vpr::Status::Failure otherwise.
     */
    virtual Status enableNonBlocking(void);

    // ========================================================================
    // vpr::SocketImp interface implementation.
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
     * @return vpr::Status::Success is returned if the connection
     *         succeeded.<br>
     *         vpr::Status::InProgress is returned if this is a non-blocking
     *         socket and the connection is still in progress.  The connection
     *         will be completed "in the background".<br>
     *         vpr::Status::Timeout is returned if the connection could not be
     *         made within the given timeout interval.<br>
     *         vpr::Status::Failure is returned if the connection could not be
     *         made.
     */
    virtual Status connect(vpr::Interval timeout = vpr::Interval::NoTimeout);

    /**
     * Gets the type of this socket (e.g., vpr::SocketTypes::STREAM).
     *
     * @pre The socket implementation pointer is valid.
     * @post The socket type for this socket is returned to the caller.
     *
     * @return A vpr::SocketTypes::Type value giving the socket type for this
     *         socket.
     */
    inline const SocketTypes::Type&
    getType (void) const {
        return m_type;
    }

    /**
     * Gets the type of this socket (e.g., vpr::SocketTypes::STREAM).
     *
     * @pre The socket implementation pointer is valid.
     * @post The socket type for this socket is returned to the caller.
     *
     * @return A vpr::SocketTypes::Type value giving the socket type for this
     *         socket.
     */
    inline const InetAddr&
    getLocalAddr (void) const {
        return m_local_addr;
    }

    /**
     * Changes the local address for this socket.
     *
     * @pre This socket is not already bound.
     * @post m_local_addr is updated to use the given vpr::InetAddr object.
     */
    Status setLocalAddr(const InetAddr& addr);

    /**
     * Returns the remote address for this socket.  This is typically the
     * address to which this socket is connected.
     */
    inline const InetAddr&
    getRemoteAddr (void) const {
        return m_remote_addr;
    }

    /**
     * Changes the remote address for this socket.
     *
     * @pre This socket is not already connected.
     * @post m_local_addr is updated to use the given vpr::InetAddr object.
     */
    Status setRemoteAddr(const InetAddr& addr);

protected:
    /**
     * Default constructor.  This just initializes member variables to
     * reasonable defaults.
     *
     * @post The member variables are initialized accordingly to reasonable
     *       defaults.
     */
    SocketImplBSD (const SocketTypes::Type sock_type)
        : BlockIO(std::string("INADDR_ANY")), m_bound(false),
          m_connected(false), m_handle(NULL), m_type(sock_type),
          m_blocking_fixed(false)
    {
        m_handle = new FileHandleImplUNIX("INADDR_ANY");
    }

    /**
     * Standard constructor.  This takes two InetAddr objects, a local address
     * and a remote address.
     *
     * @post The member variables are initialized with the given values.
     *
     * @param local_addr  The local address for the socket.
     * @param remote_addr The remote address for the socket.
     * @param sock_type   The type for this socket (stream, datagram, etc.).
     */
    SocketImplBSD (const InetAddr& local_addr, const InetAddr& remote_addr,
                   const SocketTypes::Type sock_type)
        : BlockIO(std::string("INADDR_ANY")), m_bound(false),
          m_connected(false), m_handle(NULL), m_local_addr(local_addr),
          m_remote_addr(remote_addr), m_type(sock_type),
          m_blocking_fixed(false)
    {
        m_handle = new FileHandleImplUNIX(m_name);
    }

    /**
     * Destructor.  This releases the memory allocated for m_handle (if it is
     * non-NULL).
     *
     * @pre If m_handle is NULL, its memory has already been deleted.
     * @post The memory for m_handle is deleted.
     */
    virtual ~SocketImplBSD(void);

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
     *                   defaults to vpr::Interval::NoTimeout.
     *
     * @return vpr::Status::Success is returned if the read operation
     *         completed successfully.<br>
     *         vpr::Status::InProgress if the device is in non-blocking mode,
     *         and the read operation is in progress.<br>
     *         vpr::Status::Timeout is returned if the read could not begin
     *         within the timeout interval.<br>
     *         vpr::Status::Failure is returned if the read operation failed.
     */
    virtual Status
    read_i (void* buffer, const size_t length, ssize_t& bytes_read,
            const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
        m_blocking_fixed = true;
        return m_handle->read(buffer, length, bytes_read, timeout);
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
     * @return vpr::Status::Success is returned if the read operation
     *         completed successfully.<br>
     *         vpr::Status::InProgress if the device is in non-blocking mode,
     *         and the read operation is in progress.<br>
     *         vpr::Status::Timeout is returned if the read could not begin
     *         within the timeout interval.<br>
     *         vpr::Status::Failure is returned if the read operation failed.
     */
    virtual Status
    readn_i (void* buffer, const size_t length, ssize_t& bytes_read,
             const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
        m_blocking_fixed = true;
        return m_handle->readn(buffer, length, bytes_read, timeout);
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
    virtual Status
    write_i (const void* buffer, const size_t length, ssize_t& bytes_written,
             const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
        m_blocking_fixed = true;
        return m_handle->write(buffer, length, bytes_written, timeout);
    }

    virtual Status getOption(const SocketOptions::Types option,
                             struct SocketOptions::Data& data);

    virtual Status setOption(const SocketOptions::Types option,
                             const struct SocketOptions::Data& data);

    bool                m_bound;
    bool                m_connected;
    FileHandleImplUNIX* m_handle;      /**< The OS handle for this socket */
    InetAddr            m_local_addr;  /**< The local site's address structure */
    InetAddr            m_remote_addr; /**< The remote site's address structure */
    SocketTypes::Type   m_type;        /**< The type for this socket */

    bool                m_blocking_fixed;
};

}; // End of vpr namespace


#endif   /* _VPR_SOCKET_IMPL_BSD_H_ */
