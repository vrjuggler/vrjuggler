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
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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

#include <sys/types.h>
#include <netinet/in.h>
#include <string>
#include <vector>

#include <vpr/IO/IOSys.h>
#include <vpr/md/POSIX/IO/FileHandleImplUNIX.h>
#include <vpr/IO/Socket/InetAddr.h>
#include <vpr/IO/Socket/SocketOptions.h>
#include <vpr/Util/Assert.h>
#include <vpr/Util/ReturnStatus.h>


namespace vpr
{

/**
 * Basic socket wrapper implementation for BSD sockets.
 */
class SocketImplBSD
{
public:
   // ========================================================================
   // vpr::BlockIO basics.
   // ========================================================================

   /**
    * Gets the "name" of this socket.  It is typically the address of the
    * peer host.
    *
    * @pre None.
    * @post
    *
    * @return An object containing the "name" of this socket.
    */
   const std::string& getName (void)
   {
      return mHandle->getName();
   }

   /**
    * Sets the blocking flags so that the socket is opened in blocking mode.
    *
    * @pre None.
    * @post The open flags are updated so that when the socket is opened, it
    *       is opened in blocking mode.  If the socket is already open, this
    *       has no effect.
    */
   void setOpenBlocking (void)
   {
      mOpenBlocking = true;
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
   void setOpenNonBlocking (void)
   {
      mOpenBlocking = false;
   }

   /**
    * Opens the socket.  This creates a new socket using the domain and type
    * options set through member variables.
    *
    * @pre mDomain and mType have been set to values recognized by the
    *      socket(2) system call.
    * @post A new socket is created with its file handle stored in the
    *       mFdesc member variable.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the socket was opened
    *         successfully.<br>
    *         vpr::ReturnStatus::Fail is returned otherwise.
    */
   vpr::ReturnStatus open(void);

   /**
    * Close the socket.
    *
    * @pre The socket is open.
    * @post An attempt is made to close the socket.  The resulting status is
    *       returned to the caller.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the socket was closed
    *         successfully; vpr::ReturnStatus::Fail otherwise.
    */
   vpr::ReturnStatus close (void)
   {
      vpr::ReturnStatus retval;

      retval = mHandle->close();
      mOpen = (retval.success() ? false : true);

      return retval;
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
   bool isOpen (void)
   {
      return mOpen;
   }

   bool isBound() const
   {  return mBound; }


   /**
    * Binds this socket to the address in the host address member variable.
    *
    * @pre The socket is open, and mLocalAddr has been initialized properly.
    * @post The socket is bound to the address in mLocalAddr.
    *
    * @return vpr::ReturnStatus::Sucess is returned if the socket was bound to
    *         the address successfully.<br>
    *         vpr::ReturnStatus::Fail is returned if the socket could not be
    *         bound to the address in mLocalAddr.
    */
   vpr::ReturnStatus bind(void);

   /// Returns the contained handle.
   vpr::IOSys::Handle getHandle (void)
   {
      return mHandle->getHandle();
   }

   /**
    * Queries if the blocking state for this socket is fixed and can no
    * longer be changed.
    */
   bool isBlockingFixed (void)
   {
      return mBlockingFixed;
   }

   /**
    * Reconfigures the socket so that it is in blocking mode.
    *
    * @pre The socket is open.
    * @post Processes will block when accessing the socket.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the blocking mode was
    *         changed successfully; vpr::ReturnStatus::Fail otherwise.
    */
   vpr::ReturnStatus enableBlocking(void);

   /**
    * Reconfigures the socket so that it is in non-blocking mode.
    *
    * @pre The socket is open.
    * @post Processes will not block when accessing the socket.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the blocking mode was
    *         changed successfully; vpr::ReturnStatus::Fail otherwise.
    */
   vpr::ReturnStatus enableNonBlocking(void);

   /**
    * Gets the current blocking state for the socket.
    *
    * @return <code>true</code> is returned if the socket is in blocking
    *         mode.  Otherwise, <code>false</code> is returned.
    */
   bool getBlocking (void) const
   {
      return mHandle->getBlocking();
   }

   /**
    * Gets the current non-blocking state for the socket.
    *
    * @return <code>true</code> is returned if the socket is in non-blocking
    *         mode.  Otherwise, <code>false</code> is returned.
    */
   bool getNonBlocking (void) const
   {
      return mHandle->getNonBlocking();
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
    * @post The socket is connected to the address in mLocalAddr.  For a
    *       stream socket, this means that a connection for future
    *       communication has been established.  For a datagram socket, the
    *       default destination for all packets is now mLocalAddr.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the connection
    *         succeeded.<br>
    *         vpr::ReturnStatus::InProgress is returned if this is a
    *         non-blocking socket and the connection is still in progress.  The
    *         connection will be completed "in the background".<br>
    *         vpr::ReturnStatus::Timeout is returned if the connection could
    *         not be made within the given timeout interval.<br>
    *         vpr::ReturnStatus::Fail is returned if the connection could not
    *         be made.
    */
   vpr::ReturnStatus connect(vpr::Interval timeout = vpr::Interval::NoTimeout);

   /**
    * Gets the status of a possibly connected socket.
    *
    * @pre This socket is opened and has been connected to a peer.
    *
    * @return true is returned if this socket is still connected.<br>
    *         false is returned if this socket is not currently connected.
    */
   bool isConnected(void);

   /**
    * Gets the type of this socket (e.g., vpr::SocketTypes::STREAM).
    *
    * @pre The socket implementation pointer is valid.
    * @post The socket type for this socket is returned to the caller.
    *
    * @return A vpr::SocketTypes::Type value giving the socket type for this
    *         socket.
    */
   const vpr::SocketTypes::Type& getType (void) const
   {
      return mType;
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
   const vpr::InetAddr& getLocalAddr (void) const
   {
      return mLocalAddr;
   }

   /**
    * Changes the local address for this socket.
    *
    * @pre This socket is not already bound.
    * @post mLocalAddr is updated to use the given vpr::InetAddr object.
    */
   vpr::ReturnStatus setLocalAddr(const vpr::InetAddr& addr);

   /**
    * Returns the remote address for this socket.  This is typically the
    * address to which this socket is connected.
    */
   const vpr::InetAddr& getRemoteAddr (void) const
   {
      return mRemoteAddr;
   }

   /**
    * Changes the remote address for this socket.
    *
    * @pre This socket is not already connected.
    * @post mLocalAddr is updated to use the given vpr::InetAddr object.
    */
   vpr::ReturnStatus setRemoteAddr(const vpr::InetAddr& addr);

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
    * @return vpr::ReturnStatus::Succeed is returned if the read operation
    *         completed successfully.<br>
    *         vpr::ReturnStatus::WouldBlock if the device is in non-blocking
    *         mode, and there is no data to read.<br>
    *         vpr::ReturnStatus::Timeout is returned if the read could not
    *         begin within the timeout interval.<br>
    *         vpr::ReturnStatus::Fail is returned if the read operation failed.
    */
   vpr::ReturnStatus read_i(void* buffer, const vpr::Uint32 length,
                            vpr::Uint32& bytes_read,
                            const vpr::Interval timeout = vpr::Interval::NoTimeout);

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
    * @return vpr::ReturnStatus::Succeed is returned if the read operation
    *         completed successfully.<br>
    *         vpr::ReturnStatus::WouldBlock if the device is in non-blocking
    *         mode, and there is no data to read.<br>
    *         vpr::ReturnStatus::Timeout is returned if the read could not
    *         begin within the timeout interval.<br>
    *         vpr::ReturnStatus::Fail is returned if the read operation failed.
    */
   vpr::ReturnStatus readn_i(void* buffer, const vpr::Uint32 length,
                             vpr::Uint32& bytes_read,
                             const vpr::Interval timeout = vpr::Interval::NoTimeout);

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
    * @return vpr::ReturnStatus::Succeed is returned if the write operation
    *         completed successfully.<br>
    *         vpr::ReturnStatus::WouldBlock is returned if the handle is in
    *         non-blocking mode, and the write operation could not be
    *         completed.<br>
    *         vpr::ReturnStatus::Timeout is returned if the write could not
    *         begin within the timeout interval.<br>
    *         vpr::ReturnStatus::Fail is returned if the write operation
    *         failed.
    */
   vpr::ReturnStatus write_i(const void* buffer, const vpr::Uint32 length,
                             vpr::Uint32& bytes_written,
                             const vpr::Interval timeout = vpr::Interval::NoTimeout);

   vpr::Uint32 availableBytes (void)
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
    * @return vpr::ReturnStatus::Succeed is returned if the value for the given
    *         option was retrieved successfully.<br>
    *         vpr::ReturnStatus;:Fail is returned otherwise.
    */
   vpr::ReturnStatus getOption(const vpr::SocketOptions::Types option,
                               struct vpr::SocketOptions::Data& data);

   /**
    * Sets a value for the given option on the socket using the given data
    * block.
    *
    * @param option The option whose value will be set.
    * @param data   A data buffer containing the value to be used in setting
    *               the socket option.
    */
   vpr::ReturnStatus setOption(const vpr::SocketOptions::Types option,
                               const struct vpr::SocketOptions::Data& data);

   /**
    * Destructor.  This releases the memory allocated for mHandle (if it is
    * non-NULL).
    *
    * @pre If mHandle is NULL, its memory has already been deleted.
    * @post The memory for mHandle is deleted.
    */
   ~SocketImplBSD(void);

protected:
  /**
    * Default constructor.  This just initializes member variables to
    * reasonable defaults.
    *
    * @post The member variables are initialized accordingly to reasonable
    *       defaults.
    */
   SocketImplBSD (const vpr::SocketTypes::Type sock_type)
      : mOpen(false), mOpenBlocking(true), mBound(false),
        mConnected(false), mBlockingFixed(false), mHandle(NULL),
        mType(sock_type)
   {
      mHandle = new FileHandleImplUNIX();
   }

   /**
    * Standard constructor.  This takes two vpr::InetAddr objects, a local
    * address and a remote address.
    *
    * @post The member variables are initialized with the given values.
    *
    * @param local_addr  The local address for the socket.
    * @param remote_addr The remote address for the socket.
    * @param sock_type   The type for this socket (stream, datagram, etc.).
    */
   SocketImplBSD (const vpr::InetAddr& local_addr,
                  const vpr::InetAddr& remote_addr,
                  const vpr::SocketTypes::Type sock_type)
      : mOpen(false), mOpenBlocking(true), mBound(false),
        mConnected(false), mBlockingFixed(false), mHandle(NULL),
        mLocalAddr(local_addr), mRemoteAddr(remote_addr), mType(sock_type)
   {
      mHandle = new FileHandleImplUNIX(remote_addr.getAddressString());
   }

protected:
   // XXX: This class should not need mOpen and should instead use the one
   // in FileHandleUNIX.  For some reason, doing that causes all kinds of
   // problems in SocketStreamImplBSD::accept().
   bool mOpen;
   bool mOpenBlocking;
   bool mBound;
   bool mConnected;
   bool mBlockingFixed;

   vpr::FileHandleImplUNIX* mHandle; /**< The OS handle for this socket */

   vpr::InetAddr mLocalAddr;    /**< The local site's address structure */
   vpr::InetAddr mRemoteAddr;   /**< The remote site's address structure */

   vpr::SocketTypes::Type mType; /**< The type for this socket */
};

} // End of vpr namespace


#endif   /* _VPR_SOCKET_IMPL_BSD_H_ */
