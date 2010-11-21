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
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VPR_SOCKET_BRIDGE_H_
#define _VPR_SOCKET_BRIDGE_H_
// NOTE: this is the bridge class for use with Socket.h

#include <vpr/vprConfig.h>

#include <string>

#include <vpr/IO/BlockIO.h> // base class.
#include <vpr/IO/IOException.h>
#include <vpr/IO/IOSys.h>
#include <vpr/IO/Socket/SocketOptions.h>

#include <vpr/Util/Interval.h>
#include <vpr/Util/Assert.h>

#include <vpr/IO/Stats/BaseIOStatsStrategy.h>

#include <boost/smart_ptr.hpp>

#include <typeinfo>


namespace vpr
{

/** \class Socket_t Socket_t.h vpr/IO/Socket/Socket_t.h
 *
 * Cross-platform block-based socket interface.  vpr::Socket objects cannot be
 * instantiated.  Instead, see vpr::SocketStream and vpr::SocketDatagram.
 * Given a SocketConfiguration declaration, this class is typedef'd to
 * vpr::Socket.
 *
 * @param SockConfig_ The SocketConfiguration type that matches the
 *                    platform-specific socket implementation to use.
 *
 * @see vpr::SocketStream_t, vpr::SocketDatagram_t, vpr::SocketImplNSPR,
 *      vpr::SocketImplBSD
 *
 * @note vpr::SocketBasicOpt and vpr::SocketIpOpt were folded into this class
 *       in version 2.1.10. User-level code will most likely not be affected
 *       by this difference.
 */
//template<class RealSocketImpl, class IO_STATS_STRATEGY = NullIOStatsStrategy>
template<class SockConfig_>
class Socket_t : public vpr::BlockIO
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
    * @return An object containing the "name" of this socket.
    */
   virtual const std::string& getName() const
   {
      return mSocketImpl->getName();
   }

   /**
    * Opens the socket.
    *
    * @pre The socket is not already open.
    * @post An attempt is made to open the socket.  The resulting status is
    *       returned to the caller.  If the socket is opened, \c mOpen is set
    *       to true.
    *
    * @throws vpr::IOException if the socket could not be opened.
    * @see FileHandleImplUNIX::open()
    */
   void open()
   {
      mSocketImpl->open();
   }

   /**
    * Closes the socket.
    *
    * @pre The socket is open.
    * @post An attempt is made to close the socket.  The resulting status
    *       is returned to the caller.  If the socket is closed, \c mOpen
    *       is set to false.
    *
    * @throws vpr::IOException if the socke could not be closed.
    * @see FileHandleImplUNIX::close()
    */
   void close()
   {
      mSocketImpl->close();
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
   bool isOpen() const
   {
      return mSocketImpl->isOpen();
   }

   /**
    * Gets the bound state of this socket.
    *
    * @pre None.
    * @post The boolean value giving the bound state is returned
    *
    * @return true is returned if this socket is bound;
    *         false otherwise.
    */
   bool isBound() const
   {
      return mSocketImpl->isBound();
   }

   /**
    * Gets the status of a possibly connected socket.
    *
    * @return \c true is returned if this socket is connected to a remote
    *         address.
    * @return \c false is returned if this socket is not currently connected
    *         (the other wise may have disconnected).
    */
   bool isConnected() const
   {
      return mSocketImpl->isConnected();
   }

   /** Gets the handle to this socket. */
   vpr::IOSys::Handle getHandle() const
   {
      return mSocketImpl->getHandle();
   }

   /**
    * Queries if the blocking state for this socket is fixed and can no
    * longer be changed.
    */
   bool isBlockingFixed() const
   {
      return mSocketImpl->isBlockingFixed();
   }

   /**
    * Reconfigures the socket so that it is in blocking or non-blocking mode
    * depending on the value of the parameter.  If the socket has not been
    * opened yet, it will be opened in blocking or non-blocking mode
    * appropriately when open() is called.
    *
    * @post Processes will block when accessing the socket.
    *
    * @param blocking A value of true indicates that the socket should be
    *                 configure to use blocking I/O.  A value of false
    *                 indicates that it should use non-blocking I/O.
    *
    * @throws vpr::SocketException if trying to call after a clocking
    *         call has already been made.
    * @throws vpr::IOException if the blocking state could not be set.
    *
    * @see FileHandleImplUNIX::setBlocking()
    * @see isOpen, open
    */
   void setBlocking(bool blocking)
   {
      mSocketImpl->setBlocking(blocking);
   }

   /**
    * Gets the current blocking state for the socket.
    *
    * @pre The blocking mode on the internal socket implementation is set
    *      correctly.
    *
    * @return true is returned if the socket is in blocking mode.  Otherwise,
    *         false is returned.
    */
   bool isBlocking() const
   {
      return mSocketImpl->isBlocking();
   }

   /**
    * Binds this socket to the address in the host address member variable.
    *
    * @pre The socket is open.
    * @post The socket is bound to the address defined in the constructor.
    *
    * @throws vpr::SocketException if socket could not be bound.
    */
   void bind()
   {
      mSocketImpl->bind();
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
    * @post The socket is connected to the address in \c mLocalAddr.  For a
    *       stream socket, this means that a connection for future
    *       communication has been established.  For a datagram socket, the
    *       default destination for all packets is now \c mLocalAddr.
    *
    * @throws ConnectionResetException if connection is reset.
    * @throws NoRouteToHostException if a route to host does not exist.
    * @throws UnknownHostException if host does not exist.
    * @throws IOException if network is down.
    * @throws vpr::WouldBlockException if the handle is in non-blocking mode,
    *         and the write operation could not be completed.
    * @throws vpr::TimeoutException if the write could not begin within the
    *         timeout interval.
    * @throws vpr::SocketException if could not connect.
    */
   void connect(const vpr::Interval& timeout = vpr::Interval::NoTimeout)
   {
      mSocketImpl->connect(timeout);
   }

   /**
    * Receives at most the specified number of bytes from the socket into the
    * given buffer.
    *
    * @pre The socket is open for reading, and the buffer is at least
    *      length bytes long.
    * @post The given buffer has length bytes copied into it from
    *       the socket, and the number of bytes read successfully is returned
    *       to the caller.
    *
    * @param buffer  A pointer to the buffer where the socket's buffer
    *                contents are to be stored.
    * @param length  The number of bytes to be read.
    * @param timeout The maximum amount of time to wait for data to be
    *                available for reading.  This argument is optional and
    *                defaults to vpr::Interval::NoTimeout.
    *
    * @return The number of bytes read into the buffer is returned.
    *
    * @throws vpr::SocketException if the socket is not connected.
    * @throws vpr::WouldBlockException if the file is in non-blocking mode,
    *         and there is no data to read.
    * @throws vpr::TimeoutException if the read could not begin within the
    *         timeout interval.
    * @throws vpr::IOException if the read operation failed.
    */
   vpr::Uint32 recv(void* buffer, const vpr::Uint32 length,
                    const vpr::Interval& timeout = vpr::Interval::NoTimeout)
   {
      return read(buffer, length, timeout);
   }

   /**
    * Receives at most the specified number of bytes from the socket into the
    * given buffer.
    *
    * @pre The socket is open for reading, and the buffer is at least
    *      \p length bytes long.
    * @post The given buffer has length bytes copied into it from the socket,
    *       and the number of bytes read successfully is returned to the
    *       caller.
    *
    * @param buffer  A reference to the buffer (a std::string object) where
    *                the socket's buffer contents are to be stored.
    * @param length  The number of bytes to be read.
    * @param timeout The maximum amount of time to wait for data to be
    *                available for reading.  This argument is optional and
    *                defaults to vpr::Interval::NoTimeout.
    *
    * @return The number of bytes read into the buffer is returned.
    *
    * @throws vpr::SocketException if the socket is not connected.
    * @throws vpr::WouldBlockException if the file is in non-blocking mode,
    *         and there is no data to read.
    * @throws vpr::TimeoutException if the read could not begin within the
    *         timeout interval.
    * @throws vpr::IOException if the read operation failed.
    */
   vpr::Uint32 recv(std::string& buffer, const vpr::Uint32 length,
                    const vpr::Interval& timeout = vpr::Interval::NoTimeout)
   {
      return read(buffer, length, timeout);
   }

   /**
    * Receives at most the specified number of bytes from the socket into the
    * given buffer.
    *
    * @pre The socket is open for reading, and the buffer is at least
    *      \p length bytes long.
    * @post The given buffer has length bytes copied into it from the socket,
    *       and the number of bytes read successfully is returned to the
    *       caller.
    *
    * @param buffer  A pointer to the buffer (a vector of bytes) where the
    *                socket's buffer contents are to be stored.
    * @param length  The number of bytes to be read.
    * @param timeout The maximum amount of time to wait for data to be
    *                available for reading.  This argument is optional and
    *                defaults to vpr::Interval::NoTimeout.
    *
    * @return The number of bytes read into the buffer is returned.
    *
    * @throws vpr::SocketException if the socket is not connected.
    * @throws vpr::WouldBlockException if the file is in non-blocking mode,
    *         and there is no data to read.
    * @throws vpr::TimeoutException if the read could not begin within the
    *         timeout interval.
    * @throws vpr::IOException if the read operation failed.
    */
   vpr::Uint32 recv(std::vector<vpr::Uint8>& buffer, const vpr::Uint32 length,
                    const vpr::Interval& timeout = vpr::Interval::NoTimeout)
   {
      return read(buffer, length, timeout);
   }

   /**
    * Receives exactly the specified number of bytes from the socket into the
    * given buffer.
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
    *                available for reading.  This argument is optional and
    *                defaults to vpr::Interval::NoTimeout.
    *
    * @return The number of bytes read into the buffer is returned.
    *
    * @throws vpr::SocketException if socket is not connected.
    * @throws vpr::EOFException if end of file or end of stream has been
    *         reached unexpectedly during input.
    * @throws vpr::IOException if an error ocured while reading.
    */
   vpr::Uint32 recvn(void* buffer, const vpr::Uint32 length,
                     const vpr::Interval& timeout = vpr::Interval::NoTimeout)
   {
      return readn(buffer, length, timeout);
   }

   /**
    * Receives exactly the specified number of bytes from the socket into the
    * given buffer.
    *
    * @pre The socket is open for reading, and the buffer is at least
    *      \p length bytes long.
    * @post The given buffer has length bytes copied into it from the socket,
    *       and the number of bytes read successfully is returned to the
    *       caller.
    *
    * @param buffer  A reference to the buffer (a std::string object) where
    *                the socket's buffer contents are to be stored.
    * @param length  The number of bytes to be read.
    * @param timeout The maximum amount of time to wait for data to be
    *                available for reading.  This argument is optional and
    *                defaults to vpr::Interval::NoTimeout.
    *
    * @return The number of bytes read into the buffer is returned.
    *
    * @throws vpr::SocketException if socket is not connected.
    * @throws vpr::EOFException if end of file or end of stream has been
    *         reached unexpectedly during input.
    * @throws vpr::IOException if an error ocured while reading.
    */
   vpr::Uint32 recvn(std::string& buffer, const vpr::Uint32 length,
                     const vpr::Interval& timeout = vpr::Interval::NoTimeout)
   {
      return readn(buffer, length, timeout);
   }

   /**
    * Receives exactly the specified number of bytes from the socket into the
    * given buffer.
    *
    * @pre The socket is open for reading, and the buffer is at least
    *      \p length bytes long.
    * @post The given buffer has length bytes copied into it from the socket,
    *       and the number of bytes read successfully is returned to the
    *       caller.
    *
    * @param buffer  A pointer to the buffer (a vector of bytes) where the
    *                socket's buffer contents are to be stored.
    * @param length  The number of bytes to be read.
    * @param timeout The maximum amount of time to wait for data to be
    *                available for reading.  This argument is optional and
    *                defaults to vpr::Interval::NoTimeout.
    *
    * @return The number of bytes read into the buffer is returned.
    *
    * @throws vpr::SocketException if socket is not connected.
    * @throws vpr::EOFException if end of file or end of stream has been
    *         reached unexpectedly during input.
    * @throws vpr::IOException if an error ocured while reading.
    */
   vpr::Uint32 recvn(std::vector<vpr::Uint8>& buffer, const vpr::Uint32 length,
                     const vpr::Interval& timeout = vpr::Interval::NoTimeout)
   {
      return readn(buffer, length, timeout);
   }

   /** Returns the number of avaiable bytes for reading. */
   virtual vpr::Uint32 availableBytes() const
   {
      return mSocketImpl->availableBytes();
   }

   /**
    * Sends the buffer to the remote side of the socket.
    *
    * @pre The socket is open for writing.
    * @post The given buffer is written to the socket, and the number of bytes
    *       written successfully is returned to the caller.
    *
    * @param buffer       A pointer to the buffer to be written.
    * @param length       The length of the buffer.
    * @param timeout      The maximum amount of time to wait for data to be
    *                     available for writing.  This argument is optional
    *                     and defaults to vpr::Interval::NoTimeout.
    *
    * @return The number of bytes written to the socket is returned.
    *
    * @throws ConnectionResetException if connection is reset.
    * @throws NoRouteToHostException if a route to host does not exist.
    * @throws UnknownHostException if host does not exist.
    * @throws IOException if network is down.
    * @throws vpr::WouldBlockException if the handle is in non-blocking mode,
    *         and the write operation could not be completed.
    * @throws vpr::TimeoutException if the write could not begin within the
    *         timeout interval.
    * @throws vpr::SocketException if the write operation failed.
    * @throws vpr::IOException if the file handle write operation failed.
    */
   vpr::Uint32 send(const void* buffer, const vpr::Uint32 length,
                    const vpr::Interval& timeout = vpr::Interval::NoTimeout)
   {
      return write(buffer, length, timeout);
   }

   /**
    * Sends the buffer to the remote side of the socket.
    *
    * @pre The socket is open for writing.
    * @post The given buffer is written to the socket, and the number of bytes
    *       written successfully is returned to the caller.
    *
    * @param buffer  A reference to the buffer (a std::string object) to be
    *                written.
    * @param length  The length of the buffer.
    * @param timeout The maximum amount of time to wait for data to be
    *                available for writing.  This argument is optional and
    *                defaults to vpr::Interval::NoTimeout.
    *
    * @return The number of bytes written to the socket is returned.
    *
    * @throws ConnectionResetException if connection is reset.
    * @throws NoRouteToHostException if a route to host does not exist.
    * @throws UnknownHostException if host does not exist.
    * @throws IOException if network is down.
    * @throws vpr::WouldBlockException if the handle is in non-blocking mode,
    *         and the write operation could not be completed.
    * @throws vpr::TimeoutException if the write could not begin within the
    *         timeout interval.
    * @throws vpr::SocketException if the write operation failed.
    * @throws vpr::IOException if the file handle write operation failed.
    */
   vpr::Uint32 send(const std::string& buffer, const vpr::Uint32 length,
                    const vpr::Interval& timeout = vpr::Interval::NoTimeout)
   {
      vprASSERT(length <= buffer.size() && "length was bigger than the data given");
      return write(buffer, length, timeout);
   }

   /**
    * Sends the buffer to the remote side of the socket.
    *
    * @pre The socket is open for writing.
    * @post The given buffer is written to the socket, and the number of bytes
    *       written successfully is returned to the caller.
    *
    * @param buffer  A reference to the buffer (a std::string object) to be
    *                written.
    * @param length  The length of the buffer.
    * @param timeout The maximum amount of time to wait for data to be
    *                available for writing.  This argument is optional and
    *                and defaults to vpr::Interval::NoTimeout.
    *
    * @return The number of bytes written to the socket is returned.
    *
    * @throws ConnectionResetException if connection is reset.
    * @throws NoRouteToHostException if a route to host does not exist.
    * @throws UnknownHostException if host does not exist.
    * @throws IOException if network is down.
    * @throws vpr::WouldBlockException if the handle is in non-blocking mode,
    *         and the write operation could not be completed.
    * @throws vpr::TimeoutException if the write could not begin within the
    *         timeout interval.
    * @throws vpr::SocketException if the write operation failed.
    * @throws vpr::IOException if the file handle write operation failed.
    */
   vpr::Uint32 send(const std::vector<vpr::Uint8>& buffer,
                    const vpr::Uint32 length,
                    const vpr::Interval& timeout = vpr::Interval::NoTimeout)
   {
      vprASSERT(length <= buffer.size() && "length was bigger than the data given");
      return write(buffer, length, timeout);
   }

   /**
    * Gets the type of this socket (e.g., vpr::SocketTypes::STREAM).
    *
    * @pre The socket implementation pointer is valid.
    * @post The socket type for \c mSocketImpl is returned to the caller.
    *
    * @return A vpr::SocketTypes::Type value giving the socket type for
    *         this socket.
    */
   const vpr::SocketTypes::Type& getType() const
   {
      return mSocketImpl->getType();
   }

   const InetAddr& getLocalAddr() const
   {
      return mSocketImpl->getLocalAddr();
   }

   void setLocalAddr(const InetAddr& addr)
   {
      mSocketImpl->setLocalAddr(addr);
   }

   const InetAddr& getRemoteAddr() const
   {
      return mSocketImpl->getRemoteAddr();
   }

   void setRemoteAddr(const InetAddr& addr)
   {
      mSocketImpl->setRemoteAddr(addr);
   }

   /** @name Socket Options */
   //@{
   /**
    * @throw vpr::SocketException Thrown if the operation failed.
    */
   bool getKeepAlive() const
   {
      vpr::SocketOptions::Data option;

      getOption(vpr::SocketOptions::KeepAlive, option);

      return option.keep_alive;
   }

   /**
    * @throw vpr::SocketException Thrown if the operation failed.
    */
   void setKeepAlive(const bool enableVal)
   {
      vpr::SocketOptions::Data option;
      option.keep_alive = enableVal;
      setOption(vpr::SocketOptions::KeepAlive, option);
   }

   /**
    * @throw vpr::SocketException Thrown if the operation failed.
    */
   void getLingerOnClose(bool& enabled, int& lingerSec) const
   {
      vpr::SocketOptions::Data opt;

      getOption(vpr::SocketOptions::Linger, opt);

      enabled   = opt.linger.enabled;
      lingerSec = opt.linger.seconds;
   }

   /**
    * @throw vpr::SocketException Thrown if the operation failed.
    */
   void setLingerOnClose(const bool enableVal,
                         const int lingerSec)
   {
      vpr::SocketOptions::Data opt;

      opt.linger.enabled = enableVal;
      opt.linger.seconds = lingerSec;

      setOption(vpr::SocketOptions::Linger, opt);
   }

   /**
    * @throw vpr::SocketException Thrown if the operation failed.
    */
   size_t getRecvBufferSize() const
   {
      vpr::SocketOptions::Data opt;

      getOption(vpr::SocketOptions::RecvBufferSize, opt);

      return opt.recv_buffer_size;
   }

   /**
    * @throw vpr::SocketException Thrown if the operation failed.
    */
   void setRecvBufferSize(const Int32 size)
   {
      vpr::SocketOptions::Data opt;

      opt.recv_buffer_size = size;

      setOption(vpr::SocketOptions::RecvBufferSize, opt);
   }

   /**
    * @throw vpr::SocketException Thrown if the operation failed.
    */
   size_t getSendBufferSize() const
   {
      vpr::SocketOptions::Data opt;

      getOption(vpr::SocketOptions::SendBufferSize, opt);

      return opt.send_buffer_size;
   }

   /**
    * @throw vpr::SocketException Thrown if the operation failed.
    */
   void setSendBufferSize(const Int32 size)
   {
      vpr::SocketOptions::Data opt;

      opt.send_buffer_size = size;

      setOption(vpr::SocketOptions::SendBufferSize, opt);
   }

   /**
    * @throw vpr::SocketException Thrown if the operation failed.
    */
   bool getReuseAddr() const
   {
      vpr::SocketOptions::Data option;

      getOption(vpr::SocketOptions::ReuseAddr, option);

      return option.reuse_addr;
   }

   /**
    * Enables reuse of the address that will be bound by the socket.
    *
    * @pre The socket has been opened, but bind() has not been called.
    *
    * @throw vpr::SocketException Thrown if the operation failed.
    */
   void setReuseAddr(const bool enableVal)
   {
      vpr::SocketOptions::Data option;
      option.reuse_addr = enableVal;
      setOption(vpr::SocketOptions::ReuseAddr, option);
   }

   /**
    * @throw vpr::SocketException Thrown if the operation failed.
    */
   vpr::SocketOptions::TypeOfService getTypeOfService() const
   {
      vpr::SocketOptions::Data option;

      getOption(vpr::SocketOptions::IpTypeOfService, option);
      return option.type_of_service;
   }

   /**
    * @throw vpr::SocketException Thrown if the operation failed.
    */
   void setTypeOfService(const vpr::SocketOptions::TypeOfService tos)
   {
      vpr::SocketOptions::Data option;
      option.type_of_service = tos;
      setOption(vpr::SocketOptions::IpTypeOfService, option);
   }

   /**
    * @throw vpr::SocketException Thrown if the operation failed.
    */
   vpr::Int32 getTimeToLive() const
   {
      vpr::SocketOptions::Data option;

      getOption(vpr::SocketOptions::IpTimeToLive, option);
      return option.ip_ttl;
   }

   /**
    * @throw vpr::SocketException Thrown if the operation failed.
    */
   void setTimeToLive(const vpr::Int32 ttl)
   {
      vpr::SocketOptions::Data option;
      option.ip_ttl = ttl;
      setOption(vpr::SocketOptions::IpTimeToLive, option);
   }
   //@}

protected:
   /**
    * Default constructor.  The socket address is set to "INADDRY_ANY", and
    * the implementation socket is initialized to NULL.
    *
    * @pre None.
    * @post "INADDR_ANY" is passed on to the vpr::BlockIO constructor, and
    *       \c mSocketImpl is set to NULL.
    */
   Socket_t()
      : vpr::BlockIO()
   {
      initSocket_t();
   }

   /**
    * Constructor.  The socket address is set to the given address, and the
    * implementation socket is initialized to NULL.
    *
    * @pre None.
    * @post address is passed on to the vpr::BlockIO constructor, and
    *       \c mSocketImpl is set to \c NULL.
    *
    * @param address The address string for this socket object.
    */
   Socket_t(const std::string& address)
       : vpr::BlockIO(address)
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
   virtual ~Socket_t()
   {
/*
      if(mSocketImpl != NULL)
         delete mSocketImpl;
*/

      //mSocketImpl = NULL;  // Don't destory because it is a member of the child class
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
         BaseIOStatsStrategy* new_strategy = new DefaultIOStatsStrategy;
         this->setIOStatStrategy(new_strategy);
      }
   }

   /**
    * Implementation of the read() template method.  This reads at
    * most the specified number of bytes from the socket into the given
    * buffer.
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
    * @throws vpr::SocketException if the socket is not connected.
    * @throws vpr::WouldBlockException if the file is in non-blocking mode,
    *         and there is no data to read.
    * @throws vpr::TimeoutException if the read could not begin within the
    *         timeout interval.
    * @throws vpr::IOException if the read operation failed.
    */
   virtual vpr::Uint32 read_i(void* buffer, const vpr::Uint32 length,
                              const vpr::Interval& timeout)
   {
      return mSocketImpl->read_i(buffer, length, timeout);
   }

   /**
    * Implementation of the readn() template method.  This reads
    * exactly the specified number of bytes from the socket into the given
    * buffer.
    *
    * @pre The socket is open for reading, and the buffer is at least
    *      \p length bytes long.
    * @post The given buffer has length bytes copied into it from the socket,
    *       and the number of bytes read successfully is returned to the
    *       caller.
    *
    * @param buffer    A pointer to the buffer where the socket's buffer
    *                  contents are to be stored.
    * @param length    The number of bytes to be read.
    * @param timeout   The maximum amount of time to wait for data to be
    *                  available for reading.
    *
    * @return The number of bytes read into the buffer is returned.
    *
    * @throws vpr::SocketException if socket is not connected.
    * @throws vpr::EOFException if end of file or end of stream has been
    *         reached unexpectedly during input.
    * @throws vpr::IOException if an error ocured while reading.
    */
   virtual vpr::Uint32 readn_i(void* buffer, const vpr::Uint32 length,
                               const vpr::Interval& timeout)
   {
      return mSocketImpl->readn_i(buffer, length, timeout);
   }

   /**
    * Implementation of the write() template method.  This writes
    * the buffer to the socket.
    *
    * @pre The socket is open for writing.
    * @post The given buffer is written to the socket, and the number of bytes
    *       written successfully is returned to the caller.
    *
    * @param buffer  A pointer to the buffer to be written.
    * @param length  The length of the buffer.
    * @param timeout The maximum amount of time to wait for data to be
    *                available for writing.
    *
    * @throws ConnectionResetException if connection is reset.
    * @throws NoRouteToHostException if a route to host does not exist.
    * @throws UnknownHostException if host does not exist.
    * @throws IOException if network is down.
    * @throws vpr::WouldBlockException if the handle is in non-blocking mode,
    *         and the write operation could not be completed.
    * @throws vpr::TimeoutException if the write could not begin within the
    *         timeout interval.
    * @throws vpr::SocketException if the write operation failed.
    * @throws vpr::IOException if the file handle write operation failed.
    */
   virtual vpr::Uint32 write_i(const void* buffer, const vpr::Uint32 length,
                               const vpr::Interval& timeout)
   {
      return mSocketImpl->write_i(buffer, length, timeout);
   }

   /**
    * Retrieves the value for the given option as set on the socket.
    *
    * @param option The option to be queried.
    * @param data   A data buffer that will be used to store the value of the
    *               given option.
    *
    * @throw vpr::SocketException
    *           Thrown if the value for the given option could not be
    *           retrieved.
    */
   void getOption(const vpr::SocketOptions::Types option,
                  vpr::SocketOptions::Data& data) const
   {
      mSocketImpl->getOption(option, data);
   }

   /**
    * Sets a value for the given option on the socket using the given data
    * block.
    *
    * @param option The option whose value will be set.
    * @param data   A data buffer containing the value to be used in setting
    *               the socket option.
    *
    * @throw vpr::SocketException
    *           Thrown if the value for the given option could not be set.
    */
   void setOption(const vpr::SocketOptions::Types option,
                  const vpr::SocketOptions::Data& data)
   {
      mSocketImpl->setOption(option, data);
   }

// Put in back door for simulator
#if VPR_IO_DOMAIN_INCLUDE == VPR_DOMAIN_SIMULATOR
public:
#endif
   /// Platform-specific socket implementation object
   boost::shared_ptr<SocketImpl> mSocketImpl;
};

} // End of vpr namespace


#endif  /* _VPR_SOCKET_BRIDGE_H_ */
