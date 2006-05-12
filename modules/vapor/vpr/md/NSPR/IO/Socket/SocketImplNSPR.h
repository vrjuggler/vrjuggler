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
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

#ifndef _VPR_SOCKET_IMPL_NSPR_H_
#define _VPR_SOCKET_IMPL_NSPR_H_

#include <vpr/vprConfig.h>

#include <string>
#include <vector>
#include <prio.h>
#include <prerror.h>

#include <vpr/IO/Socket/InetAddr.h>
#include <vpr/md/NSPR/NSPRHelpers.h>
#include <vpr/IO/Socket/SocketTypes.h>
#include <vpr/IO/Socket/SocketOptions.h>
#include <vpr/IO/IOSys.h>

#include <vpr/Util/Debug.h>

namespace vpr
{

/** \class SocketImplNSPR SocketImplNSPR.h vpr/IO/Socket/Socket.h
 *
 * NSPR implementation of the base socket interface.  This is used in
 * conjunction with vpr::SocketConfiguration to create the typedef
 * vpr::Socket.
 */
class VPR_CLASS_API SocketImplNSPR
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
   const std::string& getName() const
   {
      return mName;
   }

   /**
    * Opens this socket.  This creates a new socket using the domain and type
    * options set through member variables.
    *
    * @pre \c mDomain and \c mType have been set to values recognized.
    * @post A new socket is created.
    *
    * @throw vpr::SocketException If this socket could not be opened.
    * @throw vpr::IOException     If the blocking state could not be set.
    */
   void open();

   /**
    * Closes this socket.
    *
    * @pre The socket is open.
    * @post An attempt is made to close this socket.  The resulting status is
    *       returned to the caller.  If this socket is closed, \c mOpen is set
    *       to \c false.
    *
    * @throws vpr::IOException If this socket could not be closed.
    */
   void close();

   /**
    * Gets the open state of this socket.
    *
    * @post The boolean value giving the open state is returned to the caller.
    *
    * @return \c true is returned if this socket is open; \c false otherwise.
    */
   bool isOpen() const
   {
      return mOpen;
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
    * @pre The socket is open, and mLocalAddr has been initialized properly.
    * @post The socket is bound to the address in mLocalAddr.
    *
    * @throws vpr::SocketException If this socket could not be bound.
    */
   void bind();

   /**
    * Returns the contained handle.
    */
   vpr::IOSys::Handle getHandle() const
   {
      return mHandle;
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
    * Sets the blocking state for this socket.
    *
    * @pre This socket is not already open (isOpen() returns \c false).
    * @post Processes will block (or not) when accessing this socket.
    *
    * @param blocking The new blocking state.  A value of \c true indicates
    *                 that this will be a blocking socket.  A value of \c false
    *                 indicates that it will be a non-blocking socket.
    *
    * @throw vpr::SocketException If trying to call after a blocking
    *                             call has already been made.
    * @throw vpr::IOException     If the blocking state could not be set.
    *
    * @see isOpen, open
    */
   void setBlocking(bool blocking);

   /**
    * Gets the current blocking state for this socket.
    *
    * @return \c true is returned if this socket is in blocking mode.
    *         Otherwise, \c false is returned.
    */
   bool isBlocking() const
   {
      return mBlocking;
   }

   /**
    * Connects this socket on the client side to the server side.  For a
    * datagram socket, this makes the address given to the constructor the
    * default destination for all packets.  For a stream socket, this has
    * the effect of establishing a connection with the destination.
    *
    * @pre This socket is open.
    * @post This socket is connected to the address in \c mLocalAddr.  For a
    *       stream socket, this means that a connection for future
    *       communication has been established.  For a datagram socket, the
    *       default destination for all packets is now \c mLocalAddr.
    *
    * @throw ConnectionResetException If connection is reset.
    * @throw NoRouteToHostException   If a route to host does not exist.
    * @throw UnknownHostException     If host does not exist.
    * @throw IOException              If network is down.
    * @throw vpr::WouldBlockException If the socket is in non-blocking mode,
    *                                 and the write operation could not be
    *                                 completed.
    * @throw vpr::TimeoutException    If the write could not begin within the
    *                                 timeout interval.
    * @throw vpr::SocketException     If could not connect.
    */
   void connect(const vpr::Interval timeout = vpr::Interval::NoTimeout);

   /**
    * Gets the status of a possibly connected socket.
    *
    * @return \c true if this socket is connected to a remote address.
    * @return \c false if this socket is not currently connect (the other side
    *         may have disconnected).
    * @throws nothing
    */
   bool isConnected() const
   {
      // If it is not open, then it can't be connected.
      if ( mConnectCalled )
      {
         int num_avail = PR_Available(mHandle);
         if ( num_avail == 0 )
         {
            PRPollDesc poll_desc;
            poll_desc.fd = mHandle;
            poll_desc.in_flags = PR_POLL_READ;

            PR_Poll(&poll_desc, 1, PR_INTERVAL_NO_WAIT);
            if ( poll_desc.out_flags & PR_POLL_READ )
            {
               return false;             // Opened, but not connected
            }
         }

         return true;        // Either have data, or are waiting for it
      }
      else
      {
         return false;           // Not open --> not connected
      }
   }

   /**
    * Gets the type of this socket (for example, vpr::SocketTypes::STREAM).
    *
    * @pre The socket implementation pointer is valid.
    * @post The socket type for this socket is returned to the caller.
    *
    * @return A vpr::SocketTypes::Type value giving the socket type for
    *         this socket.
    */
   const SocketTypes::Type& getType() const
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
    * @throw vpr::SocketException If the local address could not be changed.
    *                             This occurs when this socket is already
    *                             bound.
    *
    * @see isBound, bind
    */
   void setLocalAddr(const vpr::InetAddr& addr)
   {
      if ( mBound )
      {
         std::string err_msg(
            "[vpr::SocketImplNSPR::setLocalAddr()] Can't set address of bound socket."
         );
         vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL) << err_msg << std::endl
                                                   << vprDEBUG_FLUSH;
         throw SocketException(err_msg, VPR_LOCATION);
      }
      else
      {
         mLocalAddr = addr;
      }
   }

   /**
    * Returns the remote address with which this socket will communicate.
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
    * @throws vpr::SocketException If the remote address could not be changed
    *                              because the socket is already connected.
    *
    * @see isConnected, connect
    */
   void setRemoteAddr(const vpr::InetAddr& addr)
   {
      if ( mConnectCalled )
      {
         std::string err_msg(
            "[vpr::SocketImplNSPR::setRemoteAddr()] Can't set address of bound socket."
         );
         vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL) << err_msg << std::endl
                                                   << vprDEBUG_FLUSH;
         throw SocketException(err_msg, VPR_LOCATION);
      }
   }

   /**
    * Implementation of the read() template method.  This reads at most the
    * specified number of bytes from this socket into the given buffer.
    *
    * @pre The device is open for reading, and the buffer is at least
    *      \p length bytes long.
    * @post The given buffer has length bytes copied into it from the device,
    *       and the number of bytes read successfully is returned to the
    *       caller via the \p bytesRead parameter.
    *
    * @param buffer    A pointer to the buffer where the device's buffer
    *                  contents are to be stored.
    * @param length    The number of bytes to be read.
    * @param bytesRead The number of bytes read into the buffer.
    * @param timeout   The maximum amount of time to wait for data to be
    *                  available for reading.  This argument is optional and
    *                  defaults to vpr::Interval::NoTimeout.
    *
    * @throw vpr::SocketException     If this socket is not connected.
    * @throw vpr::WouldBlockException If the file is in non-blocking mode,
    *                                 and there is no data to read.
    * @throw vpr::TimeoutException    If the read could not begin within the
    *                                 timeout interval.
    * @throw vpr::IOException         If the read operation failed.
    */
   void read_i(void* buffer, const vpr::Uint32 length, vpr::Uint32& bytesRead,
               const vpr::Interval timeout = vpr::Interval::NoTimeout);

   /**
    * Implementation of the readn() template method.  This reads exactly the
    * specified number of bytes from this socket into the given buffer.
    *
    * @pre The device is open for reading, and the buffer is at least
    *      \p length bytes long.
    * @post The given buffer has \p length bytes copied into it from the
    *       device, and the number of bytes read successfully is returned to
    *       the caller via the \p bytesRead parameter.
    *
    * @param buffer    A pointer to the buffer where the device's buffer
    *                  contents are to be stored.
    * @param length    The number of bytes to be read.
    * @param bytesRead The number of bytes read into the buffer.
    * @param timeout   The maximum amount of time to wait for data to be
    *                  available for reading.  This argument is optional and
    *                  defaults to vpr::Interval::NoTimeout.
    *
    * @throw vpr::SocketException If this socket is not connected.
    * @throw vpr::EOFException    If end of file or end of stream has been
    *                             reached unexpectedly during input.
    * @throw vpr::IOException     If an error ocured while reading.
    */
   void readn_i(void* buffer, const vpr::Uint32 length, vpr::Uint32& bytesRead,
                const vpr::Interval timeout = vpr::Interval::NoTimeout);

   /**
    * Implementation of the write() template method.  This writes the buffer
    * to the socket.
    *
    * @pre The device is open for writing.
    * @post The given buffer is written to the I/O device, and the number
    *       of bytes written successfully is returned to the caller via the
    *       \p bytesWritten parameter.
    *
    * @param buffer       A pointer to the buffer to be written.
    * @param length       The length of the buffer.
    * @param bytesWritten The number of bytes written to the device.
    * @param timeout      The maximum amount of time to wait for data to be
    *                     available for writing.  This argument is optional
    *                     and defaults to vpr::Interval::NoTimeout.
    *
    * @throw ConnectionResetException If connection is reset.
    * @throw NoRouteToHostException   If a route to host does not exist.
    * @throw UnknownHostException     If host does not exist.
    * @throw IOException              If network is down.
    * @throw vpr::WouldBlockException If the socket is in non-blocking mode,
    *                                 and the write operation could not be
    *                                 completed.
    * @throw vpr::TimeoutException    If the write could not begin within the
    *                                 timeout interval.
    * @throw vpr::SocketException     If the write operation failed.
    * @throw vpr::IOException         If the socket write operation failed.
    */
   void write_i(const void* buffer, const vpr::Uint32 length,
                vpr::Uint32& bytesWritten,
                const vpr::Interval timeout = vpr::Interval::NoTimeout);

   /**
    * Returns the number of bytes available to be read from this socket's
    * receive buffer.
    */
   vpr::Uint32 availableBytes() const
   {
      return PR_Available(mHandle);
   }

   /**
    * Retrieves the value for the given option as set on this socket.
    *
    * @param option The option to be queried.
    * @param data   A data buffer that will be used to store the value of the
    *               given option.
    *
    * @throw vpr::SocketException If the value for the given option
    *                             could not be retrieved.
    */
   void getOption(const vpr::SocketOptions::Types option,
                  vpr::SocketOptions::Data& data) const;

   /**
    * Sets a value for the given option on this socket using the given data
    * block.
    *
    * @param option The option whose value will be set.
    * @param data   A data buffer containing the value to be used in setting
    *               this socket option.
    *
    * @throws vpr::SocketException if the value for the given option
    *         could not be set.
    */
   void setOption(const vpr::SocketOptions::Types option,
                  const vpr::SocketOptions::Data& data);

   /**
    * Destructor.
    *
    * @post Closes this socket and deallocates any resources associated with
    *       this socket.
    */
   ~SocketImplNSPR();

protected:
   /**
    * Default constructor.  This just initializes member variables to
    * reasonable defaults.
    *
    * @post The member variables are initialized accordingly to reasonable
    *       defaults.
    *
    * @param sockType The type of socket.
    */
   SocketImplNSPR(const SocketTypes::Type sockType);

   /**
    * Standard constructor.  This takes two vpr::InetAddr objects, a local
    * address and a remote address.
    *
    * @post The member variables are initialized with the given values.
    *
    * @param localAddr  The local address for this socket.
    * @param remoteAddr The remote address for this socket.
    * @param sockType   The type of socket.
    */
   SocketImplNSPR(const InetAddr& localAddr, const InetAddr& remoteAddr,
                  const SocketTypes::Type sockType);

   /**
    * Copy constructor.
    * XXX: We need to have a reference count here
    */
   SocketImplNSPR(const SocketImplNSPR& sock)
      : mName(sock.mName)
      , mHandle(sock.mHandle)
      , mLocalAddr(sock.mLocalAddr)
      , mRemoteAddr(sock.mRemoteAddr)
      , mType(sock.mType)
      , mOpen(sock.mOpen)
      , mBound(sock.mBound)
      , mConnectCalled(sock.mConnectCalled)
      , mOpenBlocking(sock.mOpenBlocking)
      , mBlocking(sock.mBlocking)
      , mBlockingFixed(sock.mBlockingFixed)
   {
   }

   std::string       mName;
   PRFileDesc*       mHandle;     /**< Handle to the socket */
   vpr::InetAddr     mLocalAddr;  /**< The local site's address structure */
   vpr::InetAddr     mRemoteAddr; /**< The remote site's address structure */

   vpr::SocketTypes::Type mType;  /**< Socket type */

   bool mOpen;
   bool mBound;          /**< Is this socket bound to a port yet (connect
                               and bind do this) */
   bool mConnectCalled;
   bool mOpenBlocking;
   bool mBlocking;
   bool mBlockingFixed;
};

} // End of vpr namespace


#endif   /* _VPR_SOCKET_IMPL_NSPR_H_ */
