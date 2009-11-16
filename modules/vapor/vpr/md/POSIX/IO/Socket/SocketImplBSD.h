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
 * VR Juggler is (C) Copyright 1998-2009 by Iowa State University
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

#ifndef _VPR_SOCKET_IMPL_BSD_H_
#define _VPR_SOCKET_IMPL_BSD_H_

#include <vpr/vprConfig.h>

#include <string>
#include <vector>

#include <vpr/IO/IOSys.h>
#include <vpr/md/POSIX/IO/FileHandleImplUNIX.h>
#include <vpr/IO/IOException.h>
#include <vpr/IO/Socket/InetAddr.h>
#include <vpr/IO/Socket/SocketOptions.h>
#include <vpr/IO/Socket/SocketException.h>


namespace vpr
{

/** \class SocketImplBSD SocketImplBSD.h vpr/IO/Socket/Socket.h
 *
 * Basic socket wrapper implementation for BSD sockets.  This is used in
 * conjunction with vpr::SocketConfiguration to create the typedef
 * vpr::Socket.
 */
class VPR_CLASS_API SocketImplBSD
{
public:
   // ========================================================================
   // vpr::BlockIO basics.
   // ========================================================================

   /**
    * Gets the "name" of this socket.  It is typically the address of the
    * peer host.
    *
    * @return An object containing the "name" of this socket.
    */
   const std::string& getName() const;

   /**
    * Opens the socket.  This creates a new socket using the domain and type
    * options set through member variables.
    *
    * @pre mDomain and mType have been set to values recognized by the
    *      \c socket(2) system call.
    * @post A new socket is created with its file handle stored in the
    *       mFdesc member variable.
    *
    * @throws vpr::SocketException if the socket could not be opened.
    * @throws vpr::IOException if the blocking state could not be set.
    * @see FileHandleImplUNIX::setBlocking()
    */
   void open();

   /**
    * Close the socket.
    *
    * @pre The socket is open.
    * @post An attempt is made to close the socket.  The resulting status is
    *       returned to the caller.
    *       The local tracking of connected state and bound status are reset.
    *
    * @throws vpr::IOException if the socket could not be closed.
    * @see FileHandleImplUNIX::close()
    */
   void close();

   /**
    * Gets the open state of this socket.
    *
    * @post The boolean value giving the open state is returned to the
    *       caller.
    *
    * @return \c true is returned if this socket is open; \c false otherwise.
    */
   bool isOpen() const
   {
      return mHandle->isOpen();
   }

   /**
    * Gets the bound state of this socket.
    *
    * @post The boolean value giving the bound state is returned to the caller.
    *
    * @return \c true is returned if this socket is bound; \c false otherwise.
    */
   bool isBound() const
   {
      return mBound;
   }

   /**
    * Binds this socket to the address in the host address member variable.
    *
    * @pre The socket is open, and \c mLocalAddr has been initialized properly.
    * @post The socket is bound to the address in \c mLocalAddr.
    *
    * @throws vpr::SocketException if socket could not be bound.
    */
   void bind();

   /** Returns the contained handle. */
   vpr::IOSys::Handle getHandle() const
   {
      return mHandle->getHandle();
   }

   /**
    * Queries if the blocking state for this socket is fixed and can no
    * longer be changed.
    */
   bool isBlockingFixed() const
   {
      return mBlockingFixed;
   }

   /**
    * Reconfigures the socket so that it is in blocking or non-blocking mode.
    * If this socket has not been opened yet, it will be opened in blocking
    * or non-blocking mode appropriately when open() is called.
    *
    * @pre This socket is not already open (isOpen() returns \c false).
    * @post Processes will block (or not) when accessing the socket.
    *
    * @param blocking A value of \c true indicates that the socket will be
    *                 configured to use blocking I/O.  A value of \c false
    *                 indicates that it will use non-blocking I/O.
    *
    * @throws vpr::SocketException if trying to call after a clocking
    *         call has already been made.
    * @throws vpr::IOException if the blocking state could not be set.
    *
    * @see FileHandleImplUNIX::setBlocking()
    * @see isOpen, open
    */
   void setBlocking(bool blocking);

   /**
    * Gets the current blocking state for the socket.
    *
    * @return \c true is returned if the socket is in blocking mode.
    *         Otherwise, \c false is returned.
    */
   bool isBlocking() const
   {
      return mHandle->isBlocking();
   }

   // ========================================================================
   // vpr::Socket interface.
   // ========================================================================

   /**
    * Connects the socket on the client side to the server side.  For a
    * datagram socket, this makes the address given to the constructor the
    * default destination for all packets.  For a stream socket, this has
    * the effect of establishing a connection with the destination.
    *
    * @pre The socket is open.
    * @post blocking: The socket is connected to the address in \c mLocalAddr.
    *         For a stream socket, this means that a connection for future
    *         communication has been established.  For a datagram socket, the
    *         default destination for all packets is now \c mLocalAddr.
    *       non-blocking: A connection has been started and may have completed
    *         or caused an error.
    * @note timeout value does not affect blocking socket connects.
    * @todo Make timeout value apply to blocking sockets (by making non-blocking
    *       for duration of connection).
    *
    * @throws vpr::ConnectionResetException if connection is reset.
    * @throws vpr::NoRouteToHostException if a route to host does not exist.
    * @throws vpr::UnknownHostException if host does not exist.
    * @throws vpr::TimeoutException if the connection could not be completed
    *         in the given amount of time. This closes the socket before throw.
    * @throws vpr::SocketException if could not connect.
    */
   void connect(const vpr::Interval& timeout = vpr::Interval::NoTimeout);

   /**
    * Gets the status of a possibly connected socket.
    *
    * @pre This socket is opened and has been connected to a peer.
    *
    * @return \c true is returned if this socket is still connected.
    * @return \c false is returned if this socket is not currently connected.
    */
   bool isConnected() const;

   /**
    * Gets the type of this socket (for example, vpr::SocketTypes::STREAM).
    *
    * @pre The socket implementation pointer is valid.
    * @post The socket type for this socket is returned to the caller.
    *
    * @return A vpr::SocketTypes::Type value giving the socket type for this
    *         socket.
    */
   const vpr::SocketTypes::Type& getType() const
   {
      return mType;
   }

   /**
    * Returns the local address to which this socket will be or is bound.
    */
   const vpr::InetAddr& getLocalAddr() const
   {
      return mLocalAddr;
   }

   /**
    * Changes the local address for this socket.
    *
    * @pre This socket is not already bound (isBound() returns \c false).
    * @post On successful completion, \c mLocalAddr is updated to use the given
    *       vpr::InetAddr object.
    *
    * @throws vpr::SocketException if the local address could not be changed.
    *         This occurs when this socket is already bound.
    *
    * @see isBound, bind
    */
   void setLocalAddr(const vpr::InetAddr& addr);

   /**
    * Returns the remote address for this socket.  This is typically the
    * address to which this socket is connected.
    */
   const vpr::InetAddr& getRemoteAddr() const
   {
      return mRemoteAddr;
   }

   /**
    * Changes the remote address for this socket.
    *
    * @pre This socket is not already connected (isConnected() returns
    *      \c false).
    * @post On successful completion, \c mRemoteAddr is updated to use the
    *       given vpr::InetAddr object.
    *
    * @throws vpr::SocketException if the remote address could not be changed
    *         because the socket is already connected.
    *
    * @see isConnected, connect
    */
   void setRemoteAddr(const vpr::InetAddr& addr);

   /**
    * Implementation of the read() template method.  This reads at most the
    * specified number of bytes from the socket into the given buffer.
    *
    * @pre The socket is open for reading, and the buffer is at least
    *      \p length bytes long.
    * @post The given buffer has length bytes copied into it from the socket,
    *       and the number of bytes read successfully is returned to the
    *       caller.
    *
    * @param buffer  A pointer to the buffer where the socket's buffer
    *                contents are to be stored.
    * @param length  The number of bytes to be read.
    * @param timeout The maximum amount of time to wait for data to be
    *                available for reading.
    *
    * @return The number of bytes read into the buffer is returned.
    *
    * @throw vpr::SocketException
    *           Thrown if the socket is not connected.
    * @throw vpr::WouldBlockException
    *           Thrown if the file is in non-blocking mode, and there is no
    *           data to read.
    * @throw vpr::TimeoutException
    *           Thrown if the read could not begin within the timeout
    *           interval.
    * @throw vpr::IOException
    *           Thrown if the read operation failed.
    */
   vpr::Uint32 read_i(void* buffer, const vpr::Uint32 length,
                      const vpr::Interval& timeout);

   /**
    * Implementation of the readn() template method.  This reads exactly the
    * specified number of bytes from the socket into the given buffer.
    *
    * @pre The socket is open for reading, and the buffer is at least
    *      \p length bytes long.
    * @post The given buffer has \p length bytes copied into it from the
    *       socket, and the number of bytes read successfully is returned to
    *       the caller.
    *
    * @param buffer  A pointer to the buffer where the socket's buffer
    *                contents are to be stored.
    * @param length  The number of bytes to be read.
    * @param timeout The maximum amount of time to wait for data to be
    *                available for reading.
    *
    * @return The number of bytes read into the buffer is returned.
    *
    * @throw vpr::SocketException
    *           Thrown if socket is not connected.
    * @throw vpr::EOFException
    *           Thrown if end of file or end of stream has been reached
    *           unexpectedly during input.
    * @throw vpr::IOException
    *           Thrown if an error ocured while reading.
    */
   vpr::Uint32 readn_i(void* buffer, const vpr::Uint32 length,
                       const vpr::Interval& timeout);

   /**
    * Implementation of the write() template method.  This writes the buffer
    * to the socket.
    *
    * @pre The socket is open for writing.
    * @post The given buffer is written to the I/O socket, and the number
    *       of bytes written successfully is returned to the caller.
    *
    * @param buffer  A pointer to the buffer to be written.
    * @param length  The length of the buffer.
    * @param timeout The maximum amount of time to wait for data to be
    *                available for writing.
    *
    * @return The number of bytes written to the socket is returned.
    *
    * @throw vpr::ConnectionResetException
    *           Thrown if connection is reset.
    * @throw vpr::NoRouteToHostException
    *           Thrown if a route to host does not exist.
    * @throw vpr::UnknownHostException
    *           Thrown if host does not exist.
    * @throw vpr::IOException
    *           Thrown if the network is down.
    * @throw vpr::WouldBlockException
    *           Thrown if the handle is in non-blocking mode, and the write
    *           operation could not be completed.
    * @throw vpr::TimeoutException
    *           Thrown if the write could not begin within the timeout
    *           interval.
    * @throw vpr::SocketException
    *           Thrown if the write operation failed.
    * @throw vpr::IOException
    *           Thrown if the file handle write operation failed.
    */
   virtual vpr::Uint32 write_i(const void* buffer, const vpr::Uint32 length,
                               const vpr::Interval& timeout);

   vpr::Uint32 availableBytes() const
   {
      return mHandle->availableBytes();
   }

   /**
    * Retrieves the value for the given option as set on the socket.
    *
    * @param option The option to be queried.
    * @param data   A data buffer that will be used to store the value of the
    *               given option.
    *
    * @throws vpr::SocketException if the value for the given option
    *         could not be retrieved.
    */
   void getOption(const vpr::SocketOptions::Types option,
                  struct vpr::SocketOptions::Data& data) const;

   /**
    * Sets a value for the given option on the socket using the given data
    * block.
    *
    * @param option The option whose value will be set.
    * @param data   A data buffer containing the value to be used in setting
    *               the socket option.
    *
    * @throws vpr::SocketException if the value for the given option
    *         could not be set.
    */
   void setOption(const vpr::SocketOptions::Types option,
                  const struct vpr::SocketOptions::Data& data);

   /**
    * Destructor.  This releases the memory allocated for mHandle (if it is
    * non-NULL).
    *
    * @pre If mHandle is NULL, its memory has already been deleted.
    * @post The memory for mHandle is deleted.
    */
   virtual ~SocketImplBSD();

protected:
  /**
    * Default constructor.  This just initializes member variables to
    * reasonable defaults.
    *
    * @post The member variables are initialized accordingly to reasonable
    *       defaults.
    *
    * @param sockType    The type for this socket (stream, datagram, etc.).
    */
   SocketImplBSD(const vpr::SocketTypes::Type sockType);

   /**
    * Standard constructor.  This takes two vpr::InetAddr objects, a local
    * address and a remote address.
    *
    * @post The member variables are initialized with the given values.
    *
    * @param localAddr  The local address for the socket.
    * @param remoteAddr The remote address for the socket.
    * @param sockType   The type for this socket (stream, datagram, etc.).
    */
   SocketImplBSD(const vpr::InetAddr& localAddr,
                 const vpr::InetAddr& remoteAddr,
                 const vpr::SocketTypes::Type sockType);

protected:
   bool mOpenBlocking;     /**< Used for working around socket(2) semantics */
   bool mBound;            /**< True if we have been successfully bound to a port. */
   bool mConnectCalled;    /**< A connection call has been issued and is processing. */
   bool mBlockingFixed;    /**< Blocking state is fixed and can't be changed. */

   vpr::FileHandleImplUNIX* mHandle; /**< The OS handle for this socket */

   vpr::InetAddr mLocalAddr;    /**< The local site's address structure */
   vpr::InetAddr mRemoteAddr;   /**< The remote site's address structure */

   vpr::SocketTypes::Type mType; /**< The type for this socket */
};

} // End of vpr namespace


#endif   /* _VPR_SOCKET_IMPL_BSD_H_ */
