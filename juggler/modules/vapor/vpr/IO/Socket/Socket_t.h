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
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
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

#ifndef _VPR_SOCKET_BRIDGE_H_
#define _VPR_SOCKET_BRIDGE_H_
// NOTE: this is the bridge class for use with Socket.h

#include <vpr/vprConfig.h>

#include <string>

#include <vpr/IO/BlockIO.h> // base class.
#include <vpr/IO/IOSys.h>
#include <vpr/IO/Socket/SocketOptions.h>
#include <vpr/IO/Socket/SocketIpOpt.h>

#include <vpr/Util/Interval.h>
#include <vpr/Util/ReturnStatus.h>
#include <vpr/Util/Assert.h>

#include <vpr/IO/Stats/BaseIOStatsStrategy.h>

#include <boost/smart_ptr.hpp>

#include <typeinfo>


namespace vpr
{

/**
 * Cross-platform block-based socket interface.  vpr::Socket
 * objects cannot be instantiated.  Instead, see vpr::SocketStream
 * and vpr::SocketDatagram.
 *
 * @see vpr::SocketStream_t
 * @see vpr::SocketDatagram_t
 */
//template<class RealSocketImpl, class IO_STATS_STRATEGY = NullIOStatsStrategy>
template<class SockConfig_>
class Socket_t : public vpr::BlockIO, public vpr::SocketIpOpt
{
public:
   typedef SockConfig_                             Config;
   typedef typename Config::SocketImpl             SocketImpl;
   typedef typename Config::SocketIOStatsStrategy  DefaultIOStatsStrategy;

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
    * @return An object containing the "name" of this socket.
    */
   virtual const std::string& getName (void)
   {
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
   void setOpenBlocking (void)
   {
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
   void setOpenNonBlocking (void)
   {
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
    * @return vpr::ReturnStatus::Succeed is returned if this socket is
    *         opened successfully.<br>
    *         vpr::ReturnStatus::Fail is returned otherwise.
    */
   vpr::ReturnStatus open (void)
   {
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
    * @return vpr::ReturnStatus::Succeed is returned if this socket is
    *         closed successfully.<br>
    *         vpr::ReturnStatus::Fail is returned otherwise.
    */
   vpr::ReturnStatus close (void)
   {
      return m_socket_imp->close();
   }

   /**
    * Gets the open state of this socket.
    *
    * @pre None.
    * @post The boolean value giving the open state is returned to the
    *       caller.
    *
    * @return true is returned if this socket is open;
    *         false otherwise.
    */
   bool isOpen (void)
   {
      return m_socket_imp->isOpen();
   }

   /**
    * Gets the status of a possibly connected socket
    *
    * @pre None
    *
    * @return true is returned if this socket is connected to a
    *         remote address.<br>
    *         false is returned if this socket is not currently
    *         connected (the other wise may have disconnected).
    */
   bool isConnected()
   {
      return m_socket_imp->isConnected();
   }

   /// Gets the handle to this socket
   vpr::IOSys::Handle getHandle()
   {
      return m_socket_imp->getHandle();
   }

   /**
    * Queries if the blocking state for this socket is fixed and can no
    * longer be changed.
    */
   bool isBlockingFixed (void)
   {
      return m_socket_imp->isBlockingFixed();
   }

   /**
    * Reconfigures the socket so that it is in blocking mode.
    *
    * @pre The socket is open.
    * @post Processes will block when accessing the socket.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the socket's
    *         blocking mode is set to blocking.<br>
    *         vpr::ReturnStatus::Fail is returned otherwise.
    */
   vpr::ReturnStatus enableBlocking (void)
   {
      return m_socket_imp->enableBlocking();
   }

   /**
    * Reconfigures the socket so that it is in non-blocking mode.
    *
    * @pre The socket is open.
    * @post Processes will not block when accessing the socket.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the socket's
    *         blocking mode is set to non-blocking.<br>
    *         vpr::ReturnStatus::Fail is returned otherwise.
    */
   vpr::ReturnStatus enableNonBlocking (void)
   {
      return m_socket_imp->enableNonBlocking();
   }

   /**
    * Gets the current blocking state for the socket.
    *
    * @pre m_blocking is set correctly
    * @post
    *
    * @return true is returned if the socket is in blocking
    *         mode.  Otherwise, false is returned.
    */
   bool getBlocking (void) const
   {
      return m_socket_imp->getBlocking();
   }

   /**
    * Gets the current non-blocking state for the socket.
    *
    * @pre m_blocking is set correctly
    * @post
    *
    * @return true is returned if the socket is in non-blocking
    *         mode.  Otherwise, false is returned.
    */
   bool getNonBlocking (void) const
   {
      return m_socket_imp->getNonBlocking();
   }

   /**
    * Binds this socket to the address in the host address member variable.
    *
    * @pre The socket is open.
    * @post The socket is bound to the address defined in the constructor.
    *
    * @return vpr::ReturnStatus::Succeed is returned if this socket was
    *         bound to its designated local address successfully.<br>
    *         vpr::ReturnStatus::Fail is returned otherwise.
    */
   vpr::ReturnStatus bind (void)
   {
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
    * @return vpr::ReturnStatus::Succeed is returned if the connection
    *         succeeded.<br>
    *         vpr::ReturnStatus::InProgress is returned if this is a
    *         non-blocking socket and the connection is still in progress.
    *         The connection will be completed "in the background".<br>
    *         vpr::ReturnStatus::Timeout is returned if the connection
    *         could not be made within the given timeout interval.<br>
    *         vpr::ReturnStatus::Fail is returned if the connection
    *         could not be made.
    */
   vpr::ReturnStatus connect (const vpr::Interval timeout = vpr::Interval::NoTimeout)
   {
      return m_socket_imp->connect(timeout);
   }

   /**
    * Receives at most the specified number of bytes from the socket into the
    * given buffer.
    *
    * @pre The socket is open for reading, and the buffer is at least
    *      length bytes long.
    * @post The given buffer has length bytes copied into it from
    *       the socket, and the number of bytes read successfully is returned
    *       to the caller via the bytes_read argument.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the read
    *         operation completed successfully.<br>
    *         vpr::ReturnStatus::Fail is returned if the read
    *         operation failed.<br>
    *         vpr::ReturnStatus::WouldBlock if the device is in
    *         non-blocking mode, and there is no data to receive.<br>
    *         vpr::ReturnStatus::Timeout is returned if the read
    *         could not begin within the timeout interval.
    *
    * @param buffer     A pointer to the buffer where the socket's buffer
    * contents are to be stored.
    * @param length     The number of bytes to be read.
    * @param bytes_read The number of bytes read into the buffer.
    * @param timeout    The maximum amount of time to wait for data to be
    * available for reading.  This argument is optional and defaults to
    * vpr::Interval::NoTimeout.
    */
   vpr::ReturnStatus recv (void* buffer, const vpr::Uint32 length,
                           vpr::Uint32& bytes_read,
                           const vpr::Interval timeout = vpr::Interval::NoTimeout)
   {
      return read(buffer, length, bytes_read, timeout);
   }

   /**
    * Receives at most the specified number of bytes from the socket into the
    * given buffer.
    *
    * @pre The device is open for reading, and the buffer is at least
    *      length bytes long.
    * @post The given buffer has length bytes copied into it
    *       from the device, and the number of bytes read successfully is
    *       returned to the caller via the bytes_read argument.
    *
    * @param buffer     A reference to the buffer (a std::string
    *                   object) where the device's buffer contents are to be
    *                   stored.
    * @param length     The number of bytes to be read.
    * @param bytes_read The number of bytes read into the buffer.
    * @param timeout    The maximum amount of time to wait for data to be
    *                   available for reading.  This argument is optional
    *                   and defaults to vpr::Interval::NoTimeout.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the read
    *         operation completed successfully.<br>
    *         vpr::ReturnStatus::Fail is returned if the read
    *         operation failed.<br>
    *         vpr::ReturnStatus::WouldBlock if the device is in
    *         non-blocking mode, and there is no data to receive.<br>
    *         vpr::ReturnStatus::Timeout is returned if the read
    *         could not begin within the timeout interval.
    */
   vpr::ReturnStatus recv (std::string& buffer, const vpr::Uint32 length,
                           vpr::Uint32& bytes_read,
                           const vpr::Interval timeout = vpr::Interval::NoTimeout)
   {
      return read(buffer, length, bytes_read, timeout);
   }

   /**
    * Receives at most the specified number of bytes from the socket into the
    * given buffer.
    *
    * @pre The device is open for reading, and the buffer is at least
    *      length bytes long.
    * @post The given buffer has length bytes copied into it
    *       from the device, and the number of bytes read successfully is
    *       returned to the caller via the bytes_read argument.
    *
    * @param buffer     A pointer to the buffer (a vector of
    *                   chars) where the device's buffer
    *                   contents are to be stored.
    * @param length     The number of bytes to be read.
    * @param bytes_read The number of bytes read into the buffer.
    * @param timeout    The maximum amount of time to wait for data to be
    *                   available for reading.  This argument is optional
    *                   and defaults to vpr::Interval::NoTimeout.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the read
    *         operation completed successfully.<br>
    *         vpr::ReturnStatus::Fail is returned if the read
    *         operation failed.<br>
    *         vpr::ReturnStatus::WouldBlock if the device is in
    *         non-blocking mode, and there is no data to receive.<br>
    *         vpr::ReturnStatus::Timeout is returned if the read
    *         could not begin within the timeout interval.
    */
   vpr::ReturnStatus recv (std::vector<vpr::Uint8>& buffer,
                           const vpr::Uint32 length,
                           vpr::Uint32& bytes_read,
                           const vpr::Interval timeout = vpr::Interval::NoTimeout)
   {
      return read(buffer, length, bytes_read, timeout);
   }

   /**
    * Receives exactly the specified number of bytes from the socket into the
    * given buffer.
    *
    * @pre The device is open for reading, and the buffer is at least
    *      length bytes long.
    * @post The given buffer has length bytes copied into it from
    *       the device, and the number of bytes read successfully is returned
    *       to the caller via the bytes_read parameter.
    *
    * @param buffer     A pointer to the buffer where the device's buffer
    *                   contents are to be stored.
    * @param length     The number of bytes to be read.
    * @param bytes_read The number of bytes read into the buffer.
    * @param timeout    The maximum amount of time to wait for data to be
    *                   available for reading.  This argument is optional
    *                   and defaults to vpr::Interval::NoTimeout.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the read
    *         operation completed successfully.<br>
    *         vpr::ReturnStatus::Fail is returned if the read
    *         operation failed.<br>
    *         vpr::ReturnStatus::WouldBlock if the device is in
    *         non-blocking mode, and there is no data to receive.
    */
   vpr::ReturnStatus recvn (void* buffer, const vpr::Uint32 length,
                            vpr::Uint32& bytes_read,
                            const vpr::Interval timeout = vpr::Interval::NoTimeout)
   {
      return readn(buffer, length, bytes_read, timeout);
   }

   /**
    * Receives exactly the specified number of bytes from the socket into the
    * given buffer.
    *
    * @pre The device is open for reading, and the buffer is at least
    *      length bytes long.
    * @post The given buffer has length bytes copied into it from
    *       the device, and the number of bytes read successfully is returned
    *       to the caller via the bytes_read parameter.
    *
    * @param buffer     A reference to the buffer (a std::string
    *                   object) where the device's buffer contents are to be
    *                   stored.
    * @param length     The number of bytes to be read.
    * @param bytes_read The number of bytes read into the buffer.
    * @param timeout    The maximum amount of time to wait for data to be
    *                   available for reading.  This argument is optional
    *                   and defaults to vpr::Interval::NoTimeout.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the read
    *         operation completed successfully.
    *         vpr::ReturnStatus::Fail is returned if the read
    *         operation failed.  vpr::ReturnStatus::WouldBlock if the
    *         device is in non-blocking mode, and there is no data to
    *         receive.
    */
   vpr::ReturnStatus recvn (std::string& buffer, const vpr::Uint32 length,
                            vpr::Uint32& bytes_read,
                            const vpr::Interval timeout = vpr::Interval::NoTimeout)
   {
      return readn(buffer, length, bytes_read, timeout);
   }

   /**
    * Receives exactly the specified number of bytes from the socket into the
    * given buffer.
    *
    * @pre The device is open for reading, and the buffer is at least
    *      length bytes long.
    * @post The given buffer has length bytes copied into it from
    *       the device, and the number of bytes read successfully is returned
    *       to the caller.
    *
    * @param buffer     A pointer to the buffer (a vector of
    *                   chars) where the device's buffer contents
    *                   are to be stored.
    * @param length     The number of bytes to be read.
    * @param bytes_read The number of bytes read into the buffer.
    * @param timeout    The maximum amount of time to wait for data to be
    *                   available for reading.  This argument is optional
    *                   and defaults to vpr::Interval::NoTimeout.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the read
    *         operation completed successfully.<br>
    *         vpr::ReturnStatus::Fail is returned if the read
    *         operation failed.<br>
    *         vpr::ReturnStatus::WouldBlock if the device is in
    *         non-blocking mode, and there is no data to receive.<br>
    *         vpr::ReturnStatus::Timeout is returned if the read
    *         could not begin within the timeout interval.
    */
   vpr::ReturnStatus recvn (std::vector<vpr::Uint8>& buffer,
                            const vpr::Uint32 length,
                            vpr::Uint32& bytes_read,
                            const vpr::Interval timeout = vpr::Interval::NoTimeout)
   {
      return readn(buffer, length, bytes_read, timeout);
   }

   /** Return the number of avaiable bytes for reading */
   virtual vpr::Uint32 availableBytes()
   {
      return m_socket_imp->availableBytes();
   }

   /**
    * Sends the buffer to the remote side of the socket.
    *
    * @pre The device is open for writing.
    * @post The given buffer is written to the I/O device, and the number of
    *       bytes written successfully is returned to the caller via the
    *       bytes_written parameter.
    *
    * @param buffer        A pointer to the buffer to be written.
    * @param length        The length of the buffer.
    * @param bytes_written The number of bytes written to the device.
    * @param timeout       The maximum amount of time to wait for data to be
    *                      available for writing.  This argument is optional
    *                      and defaults to
    *                      vpr::Interval::NoTimeout.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the write
    *         operation completed successfully.<br>
    *         vpr::ReturnStatus::Fail is returned if the write
    *         operation failed.
    *         vpr::ReturnStatus::WouldBlock is returned if the handle
    *         is in non-blocking mode, and the write operation could not be
    *         completed.<br>
    *         vpr::ReturnStatus::Timeout is returned if the write
    *         could not begin within the timeout interval.
    */
   vpr::ReturnStatus send (const void* buffer, const vpr::Uint32 length,
                           vpr::Uint32& bytes_written,
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
    *       bytes_written parameter.
    *
    * @param buffer        A reference to the buffer (a std::string object)
    *                      to be written.
    * @param length        The length of the buffer.
    * @param bytes_written The number of bytes written to the device.
    * @param timeout       The maximum amount of time to wait for data to be
    *                      available for writing.  This argument is optional
    *                      and defaults to
    *                      vpr::Interval::NoTimeout.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the write
    *         operation completed successfully.<br>
    *         vpr::ReturnStatus::Fail is returned if the write
    *         operation failed.
    *         vpr::ReturnStatus::WouldBlock is returned if the handle
    *         is in non-blocking mode, and the send operation could not be
    *         completed.<br>
    *         vpr::ReturnStatus::Timeout is returned if the write
    *         could not begin within the timeout interval.
    */
   vpr::ReturnStatus send (const std::string& buffer, const vpr::Uint32 length,
                           vpr::Uint32& bytes_written,
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
    *       bytes_written parameter.
    *
    * @param buffer        A reference to the buffer (a std::string object)
    *                      to be written.
    * @param length        The length of the buffer.
    * @param bytes_written The number of bytes written to the device.
    * @param timeout       The maximum amount of time to wait for data to be
    *                      available for writing.  This argument is optional
    *                      and defaults to
    *                      vpr::Interval::NoTimeout.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the write
    *         operation completed successfully.<br>
    *         vpr::ReturnStatus::Fail is returned if the write
    *         operation failed.
    *         vpr::ReturnStatus::WoudlBlock is returned if the handle
    *         is in non-blocking mode, and the send operation could not be
    *         completed.<br>
    *         vpr::ReturnStatus::Timeout is returned if the write
    *         could not begin within the timeout interval.
    */
   vpr::ReturnStatus send (const std::vector<vpr::Uint8>& buffer,
                           const vpr::Uint32 length,
                           vpr::Uint32& bytes_written,
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
   const vpr::SocketTypes::Type& getType (void) const
   {
      return m_socket_imp->getType();
   }

   const InetAddr& getLocalAddr (void) const
   {
      return m_socket_imp->getLocalAddr();
   }

   vpr::ReturnStatus setLocalAddr (const InetAddr& addr)
   {
      return m_socket_imp->setLocalAddr(addr);
   }

   const InetAddr& getRemoteAddr (void) const
   {
      return m_socket_imp->getRemoteAddr();
   }

   vpr::ReturnStatus setRemoteAddr (const InetAddr& addr)
   {
      return m_socket_imp->setRemoteAddr(addr);
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
      : vpr::BlockIO(), m_socket_imp(NULL)
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
    * @post internal socket_impl* == NULL
    */
   virtual ~Socket_t (void)
   {
/*
      if(m_socket_imp != NULL)
         delete m_socket_imp;
*/

      //m_socket_imp = NULL;  // Don't destory because it is a member of the child class
      //                      // it will get destroyed on it's own
   }

   /**
    * Helper function to capture code common between constructors
    */
   void initSocket_t()
   {
      // Allocate stats stategy object.
      // First, use template programming magic.  If the stategy is the null
      // one, then don't allocate one.
      // NOTE: Because all the info is static the compiler "should" optimize
      // the following if statement away.
      if( DefaultIOStatsStrategy::IS_NULL == 0)     // If we have a real one
      {
         //std::cout << "Allocating strategy: " << typeid(DefaultIOStatsStrategy).name() << std::endl;
         BaseIOStatsStrategy<vpr::BlockIO>* new_strategy = new DefaultIOStatsStrategy;
         this->setIOStatStrategy(new_strategy);
      }
   }

   /**
    * Implementation of the read template method.  This reads at
    * most the specified number of bytes from the socket into the given
    * buffer.
    *
    * @pre The device is open for reading, and the buffer is at least
    *      length bytes long.
    * @post The given buffer has length bytes copied into it from the device,
    *       and the number of bytes read successfully is returned to the
    *       caller via the bytes_read parameter.
    *
    * @param buffer     A pointer to the buffer where the device's buffer
    *                   contents are to be stored.
    * @param length     The number of bytes to be read.
    * @param bytes_read The number of bytes read into the buffer.
    * @param timeout    The maximum amount of time to wait for data to be
    *                   available for reading.  This argument is optional and
    *                   defaults to vpr::Interval::NoTimeout
    *
    * @return vpr::ReturnStatus::Succeed is returned if the read operation
    *         completed successfully.<br>
    *         vpr::ReturnStatus::WouldBlock if the device is in non-blocking
    *         mode, and there is no data to read.<br>
    *         vpr::ReturnStatus::Timeout is returned if the read could not
    *         begin within the timeout interval.<br>
    *         vpr::ReturnStatus::Fail is returned if the read operation failed.
    */
   virtual vpr::ReturnStatus read_i (void* buffer, const vpr::Uint32 length,
                                     vpr::Uint32& bytes_read,
                                     const vpr::Interval timeout = vpr::Interval::NoTimeout)
   {
      return m_socket_imp->read_i(buffer, length, bytes_read, timeout);
   }

   /**
    * Implementation of the readn template method.  This reads
    * exactly the specified number of bytes from the socket into the given
    * buffer.
    *
    * @pre The device is open for reading, and the buffer is at least
    *      length bytes long.
    * @post The given buffer has length bytes copied into
    *       it from the device, and the number of bytes read successfully
    *       is returned to the caller via the bytes_read
    *       parameter.
    *
    * @param buffer     A pointer to the buffer where the device's buffer
    *                   contents are to be stored.
    * @param length     The number of bytes to be read.
    * @param bytes_read The number of bytes read into the buffer.
    * @param timeout    The maximum amount of time to wait for data to be
    *                   available for reading.  This argument is optional and
    *                   defaults to vpr::Interval::NoTimeout
    *
    * @return vpr::ReturnStatus::Succeed is returned if the read operation
    *         completed successfully.<br>
    *         vpr::ReturnStatus::WouldBlock if the device is in non-blocking
    *         mode, and there is no data to read.<br>
    *         vpr::ReturnStatus::Timeout is returned if the read could not
    *         begin within the timeout interval.<br>
    *         vpr::ReturnStatus::Fail is returned if the read operation failed.
    */
   virtual vpr::ReturnStatus readn_i (void* buffer, const vpr::Uint32 length,
                                      vpr::Uint32& bytes_read,
                                      const vpr::Interval timeout = vpr::Interval::NoTimeout)
   {
      return m_socket_imp->readn_i(buffer, length, bytes_read, timeout);
   }

   /**
    * Implementation of the write template method.  This writes
    * the buffer to the socket.
    *
    * @pre The device is open for writing.
    * @post The given buffer is written to the I/O device, and the number
    *       of bytes written successfully is returned to the caller via the
    *       bytes_written parameter.
    *
    * @param buffer        A pointer to the buffer to be written.
    * @param length        The length of the buffer.
    * @param bytes_written The number of bytes written to the device.
    * @param timeout       The maximum amount of time to wait for data to be
    *                      available for writing.  This argument is optional
    *                      and defaults to
    *                      vpr::Interval::NoTimeout.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the write operation
    *         completed successfully.<br>
    *         vpr::ReturnStatus::WouldBlock is returned if the handle is in
    *         non-blocking mode, and the write could not be completed.<br>
    *         vpr::ReturnStatus::Timeout is returned if the write could not
    *         begin within the timeout interval.<br>
    *         vpr::ReturnStatus::Fail is returned if the write operation
    *         failed.
    */
   virtual vpr::ReturnStatus write_i (const void* buffer,
                                      const vpr::Uint32 length,
                                      vpr::Uint32& bytes_written,
                                      const vpr::Interval timeout = vpr::Interval::NoTimeout)
   {
      return m_socket_imp->write_i(buffer, length, bytes_written, timeout);
   }

   virtual vpr::ReturnStatus getOption (const vpr::SocketOptions::Types option,
                                        struct vpr::SocketOptions::Data& data)
   {
      return m_socket_imp->getOption(option, data);
   }

   virtual vpr::ReturnStatus setOption (const vpr::SocketOptions::Types option,
                                        const struct vpr::SocketOptions::Data& data)
   {
      return m_socket_imp->setOption(option, data);
   }

// Put in back door for simulator
#if VPR_IO_DOMAIN_INCLUDE == VPR_DOMAIN_SIMULATOR
public:
#endif
   /// Platform-specific socket implementation object
   //SocketImpl* m_socket_imp;
   boost::shared_ptr<SocketImpl> m_socket_imp;
};

} // End of vpr namespace


#endif  /* _VPR_SOCKET_BRIDGE_H_ */
