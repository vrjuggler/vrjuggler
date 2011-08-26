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
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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

#include <vpr/vprConfig.h>

#include <stdio.h>
#include <string.h>
#include <sstream>
#include <sys/types.h>
#include <errno.h>
#include <boost/concept_check.hpp>
#include <boost/static_assert.hpp>

#include <vpr/IO/Socket/ConnectionResetException.h>
#include <vpr/IO/Socket/ConnectionRefusedException.h>
#include <vpr/IO/Socket/UnknownHostException.h>
#include <vpr/IO/Socket/NoRouteToHostException.h>
#include <vpr/IO/IOException.h>
#include <vpr/IO/TimeoutException.h>
#include <vpr/IO/WouldBlockException.h>
#include <vpr/Util/Debug.h>
#include <vpr/md/BOOST/IO/Socket/SocketImplBOOST.h>


namespace vpr
{

#if defined(HAVE_CORKABLE_TCP)
#  if defined(TCP_CORK)
const int VPR_TCP_CORK(TCP_CORK);
#  elif defined(TCP_NOPUSH)
const int VPR_TCP_CORK(TCP_NOPUSH);
#  else
// If HAVE_CORKABLE_TCP is defined, either TCP_CORK or TCP_NOPUSH must be
// defined.
BOOST_STATIC_ASSERT(false);
#  endif
#endif

// Given an error number (or errno) build up an exception with the
// correct type and error string and throw it.
//
// This helper comes in handy since we have to throw exceptions from so many
// places in the socket implementation.
void buildAndThrowException(const std::string& prefix,
                            const std::string& location, int error_number = -2)
{
   if ( -2 == error_number )
   {
      error_number = errno;
   }

   /** Build error string. */
   const char* err_str(strerror(error_number));
   std::string err_string;

   if ( NULL != err_str )
   {
      err_string = std::string(err_str);
   }

   // Build and throw exception
   if ( ECONNREFUSED == error_number )
   {
      std::ostringstream msg_stream;
      msg_stream << prefix << "Connection refused: " << err_string;
      throw vpr::ConnectionRefusedException(msg_stream.str(), location);
   }
   else if ( ECONNRESET == error_number )
   {
      std::ostringstream msg_stream;
      msg_stream << prefix << "Connection reset: " << err_string;
      throw vpr::ConnectionResetException(msg_stream.str(), location);
   }
   else if ( EHOSTUNREACH == error_number )
   {
      std::ostringstream msg_stream;
      msg_stream << prefix << "No route to host: " << err_string;
      throw vpr::NoRouteToHostException(msg_stream.str(), location);
   }
   else if ( ENETDOWN == error_number )
   {
      std::ostringstream msg_stream;
      msg_stream << prefix << "Network is down: " << err_string;
      throw vpr::SocketException(msg_stream.str(), location);
   }
   else
   {
      std::ostringstream msg_stream;
      msg_stream << prefix << "Error: " << err_string;
      throw vpr::SocketException(msg_stream.str(), location);
   }
}

}

namespace vpr
{

// ============================================================================
// Public methods.
// ============================================================================

const std::string& SocketImplBOOST::getName() const
{
   return mName;
}

bool SocketImplBOOST::isOpen() const
{
   switch (mType)
   {
      case vpr::SocketTypes::STREAM:
         return mTcpSocket->is_open();
         break;
      case vpr::SocketTypes::DATAGRAM:
         return mUdpSocket->is_open();
         break;
      case vpr::SocketTypes::RAW:
         throw SocketException("RAW socket type not supported.", VPR_LOCATION);
         break;
      default:
      {
         std::stringstream msg_stream;
         msg_stream << "[vpr::SocketImplBOOST] ERROR: Unknown socket type "
                    << "value: " << unsigned(mType);
         throw SocketException(msg_stream.str(), VPR_LOCATION);
         break;
      }
   }
}

vpr::Uint32 SocketImplBOOST::availableBytes() const
{
   switch (mType)
   {
      case vpr::SocketTypes::STREAM:
         return mTcpSocket->available();
         break;
      case vpr::SocketTypes::DATAGRAM:
         return mUdpSocket->available();
         break;
      default:
      {
         std::stringstream msg_stream;
         msg_stream << "[vpr::SocketImplBOOST] ERROR: Unknown socket type "
            << "value: " << unsigned(mType);
         throw SocketException(msg_stream.str(), VPR_LOCATION);
         return 0;
         break;
      }
   }
}

// Open the socket.  This creates a new socket using the domain and type
// options set through member variables.
void SocketImplBOOST::open()
{
   boost::system::error_code ec;
   switch (mType)
   {
      case vpr::SocketTypes::STREAM:
         //mTcpSocket = new boost::asio::ip::tcp::socket(mIOService);
         mTcpSocket->open(boost::asio::ip::tcp::v4(), ec);
         break;
      case vpr::SocketTypes::DATAGRAM:
         //mUdpSocket = new boost::asio::ip::udp::socket(mIOService);
         mUdpSocket->open(boost::asio::ip::udp::v4(), ec);
         break;
      case vpr::SocketTypes::RAW:
         throw SocketException("RAW socket type not supported.", VPR_LOCATION);
         break;
      default:
      {
         std::stringstream msg_stream;
         msg_stream << "[vpr::SocketImplBOOST] ERROR: Unknown socket type "
                    << "value: " << unsigned(mType);
         throw SocketException(msg_stream.str(), VPR_LOCATION);
         break;
      }
   }

   if (ec)
   {
      std::stringstream msg_stream;
      msg_stream << "[vpr::SocketImplBOOST] ERROR: Unable to open socket ";
      throw SocketException(msg_stream.str(), VPR_LOCATION);
   }

   mOpen = true;
   setBlocking(mOpenBlocking);
}

void SocketImplBOOST::close()
{
   // Reset the local state tracking to initial state
   // since we are not connected or bound any more.
   // This allows the socket to be reused.
   mBound = false;
   mOpen = false;
   mConnectCalled = false;
   mBlockingFixed = false;
   mInBlockingCall = false;
}

// Reconfigures the socket so that it is in blocking mode.
void SocketImplBOOST::setBlocking(bool blocking)
{
   vprASSERT(! mBlockingFixed &&
             "Cannot change blocking state after a blocking call!");

   if ( ! mOpen )
   {
      mOpenBlocking = blocking;
   }
   else
   {
      boost::asio::socket_base::non_blocking_io command(!blocking);
      boost::system::error_code ec;

      switch (mType)
      {
         case vpr::SocketTypes::STREAM:
            mTcpSocket->io_control(command, ec);
            break;
         case vpr::SocketTypes::DATAGRAM:
            mUdpSocket->io_control(command, ec);
            break;
         case vpr::SocketTypes::RAW:
            throw SocketException("RAW socket type not supported.",
                                  VPR_LOCATION);
            break;
         default:
         {
            std::stringstream msg_stream;
            msg_stream << "[vpr::SocketImplBOOST] ERROR: Unknown socket type "
                       << "value: " << unsigned(mType);
            throw SocketException(msg_stream.str(), VPR_LOCATION);
            break;
         }
      }
   }
}

// Bind this socket to the address in the host address member variable.
void SocketImplBOOST::bind()
{
   boost::system::error_code ec;

   switch (mType)
   {
      case vpr::SocketTypes::STREAM:
         mTcpSocket->bind(mLocalAddr.mTcpAddr, ec);
         break;
      case vpr::SocketTypes::DATAGRAM:
         mUdpSocket->bind(mLocalAddr.mUdpAddr, ec);
         break;
      case vpr::SocketTypes::RAW:
         throw SocketException("RAW socket type not supported.", VPR_LOCATION);
         break;
      default:
         {
            std::stringstream msg_stream;
            msg_stream << "[vpr::SocketImplBOOST] ERROR: Unknown socket type "
                       << "value: " << unsigned(mType);
            throw SocketException(msg_stream.str(), VPR_LOCATION);
            break;
         }
   }


   if ( ec )
   {
      std::ostringstream ex_text;
      ex_text << "[vpr::SocketImplBOOST::bind()] " << " addr: ["
              << mLocalAddr.mTcpAddr << "] ";
      buildAndThrowException(ex_text.str(), VPR_LOCATION);
   }
   else
   {
      mBound = true;
   }
}

// Connect the socket on the client side to the server side.  For a datagram
// socket, this makes the address given to the constructor the default
// destination for all packets.  For a stream socket, this has the effect of
// establishing a connection with the destination.
void SocketImplBOOST::connect(const vpr::Interval& timeout)
{
   boost::system::error_code ec;

   // Attempt to connect to the address in mAddr.
   switch (mType)
   {
      case vpr::SocketTypes::STREAM:
         mTcpSocket->connect(mRemoteAddr.mTcpAddr, ec);
         break;
      case vpr::SocketTypes::DATAGRAM:
         mUdpSocket->connect(mRemoteAddr.mUdpAddr, ec);
         break;
   }

   // If connect(2) failed, print an error message explaining why and return
   // error status.
   if ( ec )
   {
      buildAndThrowException("[vpr::SocketImplBOOST::connect()] ",
                                VPR_LOCATION, ec.value());
   }
   // Otherwise, return success.
   else
   {
      mBound            = true;
      mConnectCalled    = true;
      mBlockingFixed    = true;
   }

   // Fill in the local address if has not already been assigned.
   if ( mConnectCalled && vpr::InetAddr::AnyAddr == mLocalAddr )
   {
      boost::asio::ip::tcp::endpoint tcpEndpoint;
      boost::asio::ip::udp::endpoint udpEndpoint;
      boost::system::error_code ec;

      switch (mType)
      {
         case vpr::SocketTypes::STREAM:
            tcpEndpoint = mTcpSocket->local_endpoint(ec);
            if (!ec)
               mLocalAddr.mTcpAddr = tcpEndpoint;
            break;
         case vpr::SocketTypes::DATAGRAM:
            udpEndpoint = mUdpSocket->local_endpoint(ec);
            if (!ec)
               mLocalAddr.mUdpAddr = udpEndpoint;
            break;
      }
   }
   else
   {
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << "Failed to get sock name: " << strerror(errno) << std::endl
         << vprDEBUG_FLUSH;
   }
}

// Idea:
// - If have read or write and there are no socket errors, then we are
//   connected
bool SocketImplBOOST::isConnected() const
{
   if ( isOpen() && mConnectCalled )
   {
      return true;
   }

   return false;
}

void SocketImplBOOST::setLocalAddr(const InetAddr& addr)
{
   if ( mBound || mConnectCalled )
   {
      std::ostringstream msg_stream;
      msg_stream << "[vpr::SocketImplBOOST::setLocalAddr] Can't set address of "
                 << "a bound or connected socket.";
      throw SocketException(msg_stream.str(), VPR_LOCATION);
   }
   else
   {
      mLocalAddr = addr;
   }
}

void SocketImplBOOST::setRemoteAddr(const InetAddr& addr)
{
   if ( mConnectCalled )
   {
      throw SocketException(
         "Can not set remote address on a connected socket.", VPR_LOCATION
      );
   }
   else
   {
      mRemoteAddr = addr;
   }
}

vpr::Uint32 SocketImplBOOST::read_i(void* buffer, const vpr::Uint32 length,
                                    const vpr::Interval& timeout)
{
   boost::system::error_code ec;
   //boost::asio::buffer boostBuffer = boost::asio::buffer(buffer, length);
   vpr::Uint32 bytes(0);
   mBlockingFixed = true;
   mInBlockingCall = true;

   switch (mType)
   {
      case vpr::SocketTypes::STREAM:
         bytes = boost::asio::read(*mTcpSocket, boost::asio::buffer(buffer, length));
         break;
      case vpr::SocketTypes::DATAGRAM:
         bytes = mUdpSocket->receive(boost::asio::buffer(buffer, length));
         break;
   }

   if ( 0 == bytes )
   {
      throw ConnectionResetException("Socket disconnected cleanly.",
                                     VPR_LOCATION);
   }

   mInBlockingCall = false;
   return bytes;
}

vpr::Uint32 SocketImplBOOST::readn_i(void* buffer, const vpr::Uint32 length,
                                     const vpr::Interval& timeout)
{
   boost::system::error_code ec;
   vpr::Uint32 bytes(0);
   mBlockingFixed = true;
   mInBlockingCall = true;

   switch (mType)
   {
      case vpr::SocketTypes::STREAM:
         bytes = boost::asio::read(*mTcpSocket, boost::asio::buffer(buffer, length));
         break;
      case vpr::SocketTypes::DATAGRAM:
         bytes = mUdpSocket->receive(boost::asio::buffer(buffer, length));
         break;
   }

   if ( 0 == bytes )
   {
      int val = ec.value();
      std::string msg = ec.message();
      throw ConnectionResetException("Socket disconnected cleanly.",
                                     VPR_LOCATION);
   }

   mInBlockingCall = false;
   return bytes;
}

vpr::Uint32 SocketImplBOOST::write_i(const void* buffer,
                                     const vpr::Uint32 length,
                                     const vpr::Interval& timeout)
{
   vpr::Uint32 bytes_written(0);
   mBlockingFixed = true;
   mInBlockingCall = true;
   size_t sent;

   switch (mType)
   {
      case vpr::SocketTypes::STREAM:
         sent = mTcpSocket->send(boost::asio::buffer(buffer, length));
         break;
      case vpr::SocketTypes::DATAGRAM:
         sent = mUdpSocket->send(boost::asio::buffer(buffer, length));
         break;
      case vpr::SocketTypes::RAW:
         throw SocketException("RAW socket type not supported.", VPR_LOCATION);
         break;
      default:
      {
         std::stringstream msg_stream;
         msg_stream << "[vpr::SocketImplBOOST] ERROR: Unknown socket type "
                    << "value: " << unsigned(mType);
         throw SocketException(msg_stream.str(), VPR_LOCATION);
         break;
      }
   }

   bytes_written = sent;
   mInBlockingCall = false;
   return bytes_written;
}


void SocketImplBOOST::getLinger(vpr::SocketOptions::Data& data) const
{
   boost::asio::socket_base::linger option;
   boost::system::error_code ec;
   switch ( mType )
   {
   case vpr::SocketTypes::STREAM:
      mTcpSocket->get_option(option, ec);
      break;
   case vpr::SocketTypes::DATAGRAM:
      mUdpSocket->get_option(option, ec);
      break;
   }

   if ( !ec )
   {
      data.linger.enabled = option.enabled();
      data.linger.seconds = option.timeout();
   }
   else
   {
      std::ostringstream msg_stream;
      msg_stream << "[vpr::SocketImplBOOST::getOption()] ERROR: Could not get "
                 << "socket option for socket " << getName() << ": "
                 << strerror(errno);
      throw SocketException(msg_stream.str(), VPR_LOCATION);
   }
}

void SocketImplBOOST::setLinger(const vpr::SocketOptions::Data& data)
{
   boost::asio::socket_base::linger option(data.linger.enabled,
                                           data.linger.seconds);
   boost::system::error_code ec;

   switch ( mType )
   {
   case vpr::SocketTypes::STREAM:
      mTcpSocket->set_option(option, ec);
      break;
   case vpr::SocketTypes::DATAGRAM:
      mUdpSocket->set_option(option, ec);
      break;
   }

   if ( ec )
   {
      std::ostringstream msg_stream;
      msg_stream << "[vpr::SocketImplBOOST::setOption()] ERROR: Could not set "
                 << "socket option for socket " << getName() << ": "
                 << strerror(errno);
      throw SocketException(msg_stream.str(), VPR_LOCATION);
   }
}

void SocketImplBOOST::getReuseAddr(vpr::SocketOptions::Data& data) const
{
   boost::asio::socket_base::reuse_address option;
   boost::system::error_code ec;
   switch ( mType )
   {
   case vpr::SocketTypes::STREAM:
      mTcpSocket->get_option(option, ec);
      break;
   case vpr::SocketTypes::DATAGRAM:
      mUdpSocket->get_option(option, ec);
      break;
   }

   if ( !ec )
   {
      data.reuse_addr = option.value();
   }
   else
   {
      std::ostringstream msg_stream;
      msg_stream << "[vpr::SocketImplBOOST::getOption()] ERROR: Could not get "
                 << "socket option for socket " << getName() << ": "
                 << strerror(errno);
      throw SocketException(msg_stream.str(), VPR_LOCATION);
   }
}

void SocketImplBOOST::setReuseAddr(const vpr::SocketOptions::Data& data)
{
   boost::asio::socket_base::reuse_address option(data.reuse_addr);
   boost::system::error_code ec;

   switch ( mType )
   {
   case vpr::SocketTypes::STREAM:
      mTcpSocket->set_option(option, ec);
      break;
   case vpr::SocketTypes::DATAGRAM:
      mUdpSocket->set_option(option, ec);
      break;
   }

   if ( ec )
   {
      std::cout << ec.value() << " : " << ec.message() << std::endl;
      std::ostringstream msg_stream;
      msg_stream << "[vpr::SocketImplBOOST::setOption()] ERROR: Could not set "
                 << "socket option for socket " << getName() << ": "
                 << strerror(errno);
      throw SocketException(msg_stream.str(), VPR_LOCATION);
   }
}


void SocketImplBOOST::getKeepAlive(vpr::SocketOptions::Data& data) const
{
   boost::asio::socket_base::keep_alive option;
   boost::system::error_code ec;
   switch ( mType )
   {
   case vpr::SocketTypes::STREAM:
      mTcpSocket->get_option(option, ec);
      break;
   case vpr::SocketTypes::DATAGRAM:
      mUdpSocket->get_option(option, ec);
      break;
   }

   if ( !ec )
   {
      data.keep_alive = option.value();
   }
   else
   {
      std::ostringstream msg_stream;
      msg_stream << "[vpr::SocketImplBOOST::getOption()] ERROR: Could not get "
                 << "socket option for socket " << getName() << ": "
                 << strerror(errno);
      throw SocketException(msg_stream.str(), VPR_LOCATION);
   }
}

void SocketImplBOOST::setKeepAlive(const vpr::SocketOptions::Data& data)
{
   boost::asio::socket_base::keep_alive option(data.keep_alive);
   boost::system::error_code ec;

   switch ( mType )
   {
   case vpr::SocketTypes::STREAM:
      mTcpSocket->set_option(option, ec);
      break;
   case vpr::SocketTypes::DATAGRAM:
      mUdpSocket->set_option(option, ec);
      break;
   }

   if ( ec )
   {
      std::ostringstream msg_stream;
      msg_stream << "[vpr::SocketImplBOOST::setOption()] ERROR: Could not set "
                 << "socket option for socket " << getName() << ": "
                 << strerror(errno);
      throw SocketException(msg_stream.str(), VPR_LOCATION);
   }
}

void SocketImplBOOST::getReceiveBufferSize(vpr::SocketOptions::Data& data)
   const
{
   boost::asio::socket_base::receive_buffer_size option;
   boost::system::error_code ec;
   switch ( mType )
   {
   case vpr::SocketTypes::STREAM:
      mTcpSocket->get_option(option, ec);
      break;
   case vpr::SocketTypes::DATAGRAM:
      mUdpSocket->get_option(option, ec);
      break;
   }

   if ( !ec )
   {
      data.recv_buffer_size = option.value();
   }
   else
   {
      std::ostringstream msg_stream;
      msg_stream << "[vpr::SocketImplBOOST::getOption()] ERROR: Could not get "
                 << "socket option for socket " << getName() << ": "
                 << strerror(errno);
      throw SocketException(msg_stream.str(), VPR_LOCATION);
   }
}

void
SocketImplBOOST::setReceiveBufferSize(const vpr::SocketOptions::Data& data)
{
   boost::asio::socket_base::receive_buffer_size option(data.recv_buffer_size);
   boost::system::error_code ec;

   switch ( mType )
   {
   case vpr::SocketTypes::STREAM:
      mTcpSocket->set_option(option, ec);
      break;
   case vpr::SocketTypes::DATAGRAM:
      mUdpSocket->set_option(option, ec);
      break;
   }

   if ( ec )
   {
      std::ostringstream msg_stream;
      msg_stream << "[vpr::SocketImplBOOST::setOption()] ERROR: Could not set "
                 << "socket option for socket " << getName() << ": "
                 << strerror(errno);
      throw SocketException(msg_stream.str(), VPR_LOCATION);
   }
}

void SocketImplBOOST::getSendBufferSize(vpr::SocketOptions::Data& data) const
{
   boost::asio::socket_base::send_buffer_size option;
   boost::system::error_code ec;
   switch ( mType )
   {
   case vpr::SocketTypes::STREAM:
      mTcpSocket->get_option(option);
      break;
   case vpr::SocketTypes::DATAGRAM:
      mUdpSocket->get_option(option);
      break;
   }

   if ( !ec )
   {
      data.send_buffer_size = option.value();
   }
   else
   {
      std::ostringstream msg_stream;
      msg_stream << "[vpr::SocketImplBOOST::getOption()] ERROR: Could not get "
                 << "socket option for socket " << getName() << ": "
                 << strerror(errno);
      throw SocketException(msg_stream.str(), VPR_LOCATION);
   }
}

void SocketImplBOOST::setSendBufferSize(const vpr::SocketOptions::Data& data)
{
   boost::asio::socket_base::send_buffer_size option(data.send_buffer_size);
   boost::system::error_code ec;

   switch ( mType )
   {
   case vpr::SocketTypes::STREAM:
      mTcpSocket->set_option(option, ec);
      break;
   case vpr::SocketTypes::DATAGRAM:
      mUdpSocket->set_option(option, ec);
      break;
   }

   if ( ec )
   {
      std::ostringstream msg_stream;
      msg_stream << "[vpr::SocketImplBOOST::setOption()] ERROR: Could not set "
                 << "socket option for socket " << getName() << ": "
                 << strerror(errno);
      throw SocketException(msg_stream.str(), VPR_LOCATION);
   }
}


void SocketImplBOOST::getBroadcast(vpr::SocketOptions::Data& data) const
{
   boost::asio::socket_base::broadcast option;
   boost::system::error_code ec;
   switch ( mType )
   {
   case vpr::SocketTypes::STREAM:
      mTcpSocket->get_option(option);
      break;
   case vpr::SocketTypes::DATAGRAM:
      mUdpSocket->get_option(option);
      break;
   }

   if ( !ec )
   {
      data.broadcast = option.value();
   }
   else
   {
      std::ostringstream msg_stream;
      msg_stream << "[vpr::SocketImplBOOST::getOption()] ERROR: Could not get "
                 << "socket option for socket " << getName() << ": "
                 << strerror(errno);
      throw SocketException(msg_stream.str(), VPR_LOCATION);
   }
}

void SocketImplBOOST::setBroadcast(const vpr::SocketOptions::Data& data)
{
   boost::asio::socket_base::broadcast option(data.broadcast);
   boost::system::error_code ec;

   switch ( mType )
   {
   case vpr::SocketTypes::STREAM:
      mTcpSocket->set_option(option, ec);
      break;
   case vpr::SocketTypes::DATAGRAM:
      mUdpSocket->set_option(option, ec);
      break;
   }

   if ( ec )
   {
      std::ostringstream msg_stream;
      msg_stream << "[vpr::SocketImplBOOST::setOption()] ERROR: Could not set "
                 << "socket option for socket " << getName() << ": "
                 << strerror(errno);
      throw SocketException(msg_stream.str(), VPR_LOCATION);
   }
}

void SocketImplBOOST::getUniTTL(vpr::SocketOptions::Data& data) const
{
   boost::asio::ip::unicast::hops option;
   boost::system::error_code ec;
   switch ( mType )
   {
   case vpr::SocketTypes::STREAM:
      mTcpSocket->get_option(option);
      break;
   case vpr::SocketTypes::DATAGRAM:
      mUdpSocket->get_option(option);
      break;
   }

   if ( !ec )
   {
      data.ip_ttl = option.value();
   }
   else
   {
      std::ostringstream msg_stream;
      msg_stream << "[vpr::SocketImplBOOST::getOption()] ERROR: Could not get "
                 << "socket option for socket " << getName() << ": "
                 << strerror(errno);
      throw SocketException(msg_stream.str(), VPR_LOCATION);
   }
}

void SocketImplBOOST::setUniTTL(const vpr::SocketOptions::Data& data)
{
   boost::asio::ip::unicast::hops option(data.ip_ttl);
   boost::system::error_code ec;

   switch ( mType )
   {
   case vpr::SocketTypes::STREAM:
      mTcpSocket->set_option(option, ec);
      break;
   case vpr::SocketTypes::DATAGRAM:
      mUdpSocket->set_option(option, ec);
      break;
   }

   if ( ec )
   {
      std::ostringstream msg_stream;
      msg_stream << "[vpr::SocketImplBOOST::setOption()] ERROR: Could not set "
                 << "socket option for socket " << getName() << ": "
                 << strerror(errno);
      throw SocketException(msg_stream.str(), VPR_LOCATION);
   }
}

void SocketImplBOOST::getMcastTTL(vpr::SocketOptions::Data& data) const
{
   boost::asio::ip::multicast::hops option;
   boost::system::error_code ec;
   switch ( mType )
   {
   case vpr::SocketTypes::STREAM:
      mTcpSocket->get_option(option);
      break;
   case vpr::SocketTypes::DATAGRAM:
      mUdpSocket->get_option(option);
      break;
   }

   if ( !ec )
   {
      data.mcast_ttl = option.value();
   }
   else
   {
      std::ostringstream msg_stream;
      msg_stream << "[vpr::SocketImplBOOST::getOption()] ERROR: Could not get "
                 << "socket option for socket " << getName() << ": "
                 << strerror(errno);
      throw SocketException(msg_stream.str(), VPR_LOCATION);
   }
}

void SocketImplBOOST::setMcastTTL(const vpr::SocketOptions::Data& data)
{
   boost::asio::ip::multicast::hops option(data.mcast_ttl);
   boost::system::error_code ec;

   switch ( mType )
   {
   case vpr::SocketTypes::STREAM:
      mTcpSocket->set_option(option, ec);
      break;
   case vpr::SocketTypes::DATAGRAM:
      mUdpSocket->set_option(option, ec);
      break;
   }

   if ( ec )
   {
      std::ostringstream msg_stream;
      msg_stream << "[vpr::SocketImplBOOST::setOption()] ERROR: Could not set "
                 << "socket option for socket " << getName() << ": "
                 << strerror(errno);
      throw SocketException(msg_stream.str(), VPR_LOCATION);
   }
}

void SocketImplBOOST::getMcastLoopback(vpr::SocketOptions::Data& data)
   const
{
   boost::asio::ip::multicast::enable_loopback option;
   boost::system::error_code ec;
   switch ( mType )
   {
   case vpr::SocketTypes::STREAM:
      mTcpSocket->get_option(option);
      break;
   case vpr::SocketTypes::DATAGRAM:
      mUdpSocket->get_option(option);
      break;
   }

   if ( !ec )
   {
      data.mcast_loopback = option.value();
   }
   else
   {
      std::ostringstream msg_stream;
      msg_stream << "[vpr::SocketImplBOOST::getOption()] ERROR: Could not get "
                 << "socket option for socket " << getName() << ": "
                 << strerror(errno);
      throw SocketException(msg_stream.str(), VPR_LOCATION);
   }
}

void SocketImplBOOST::setMcastLoopback(const vpr::SocketOptions::Data& data)
{
   boost::asio::ip::multicast::enable_loopback option(data.mcast_loopback);
   boost::system::error_code ec;

   switch ( mType )
   {
   case vpr::SocketTypes::STREAM:
      mTcpSocket->set_option(option, ec);
      break;
   case vpr::SocketTypes::DATAGRAM:
      mUdpSocket->set_option(option, ec);
      break;
   }

   if ( ec )
   {
      std::ostringstream msg_stream;
      msg_stream << "[vpr::SocketImplBOOST::setOption()] ERROR: Could not set "
                 << "socket option for socket " << getName() << ": "
                 << strerror(errno);
      throw SocketException(msg_stream.str(), VPR_LOCATION);
   }
}

void SocketImplBOOST::getNoDelay(vpr::SocketOptions::Data& data) const
{
   boost::asio::ip::tcp::no_delay option;
   boost::system::error_code ec;
   switch ( mType )
   {
   case vpr::SocketTypes::STREAM:
      mTcpSocket->get_option(option);
      break;
   case vpr::SocketTypes::DATAGRAM:
      mUdpSocket->get_option(option);
      break;
   }

   if ( !ec )
   {
      data.no_delay = option.value();
   }
   else
   {
      std::ostringstream msg_stream;
      msg_stream << "[vpr::SocketImplBOOST::getOption()] ERROR: Could not get "
                 << "socket option for socket " << getName() << ": "
                 << strerror(errno);
      throw SocketException(msg_stream.str(), VPR_LOCATION);
   }
}

void SocketImplBOOST::setNoDelay(const vpr::SocketOptions::Data& data)
{
   boost::asio::ip::tcp::no_delay option(data.no_delay);
   boost::system::error_code ec;

   switch ( mType )
   {
   case vpr::SocketTypes::STREAM:
      mTcpSocket->set_option(option, ec);
      break;
   case vpr::SocketTypes::DATAGRAM:
      mUdpSocket->set_option(option, ec);
      break;
   }

   if ( ec )
   {
      std::ostringstream msg_stream;
      msg_stream << "[vpr::SocketImplBOOST::setOption()] ERROR: Could not set "
                 << "socket option for socket " << getName() << ": "
                 << strerror(errno);
      throw SocketException(msg_stream.str(), VPR_LOCATION);
   }
}

void SocketImplBOOST::setMcastAddMember(const vpr::SocketOptions::Data& data)
{
   boost::asio::ip::address multi_addr =
      boost::asio::ip::address::from_string(data.mcast_add_member.getMulticastAddr().getAddressString());
   boost::asio::ip::multicast::join_group option(multi_addr);
   boost::system::error_code ec;

   switch ( mType )
   {
   case vpr::SocketTypes::STREAM:
      mTcpSocket->set_option(option, ec);
      break;
   case vpr::SocketTypes::DATAGRAM:
      mUdpSocket->set_option(option, ec);
      break;
   }

   if ( ec )
   {
      std::ostringstream msg_stream;
      msg_stream << "[vpr::SocketImplBOOST::setOption()] ERROR: Could not set "
                 << "socket option for socket " << getName() << ": "
                 << strerror(errno);
      throw SocketException(msg_stream.str(), VPR_LOCATION);
   }
}

void SocketImplBOOST::setMcastDropMember(const vpr::SocketOptions::Data& data)
{
   boost::asio::ip::address multi_addr =
      boost::asio::ip::address::from_string(data.mcast_drop_member.getMulticastAddr().getAddressString());
   boost::asio::ip::multicast::leave_group option(multi_addr);
   boost::system::error_code ec;

   switch ( mType )
   {
   case vpr::SocketTypes::STREAM:
      mTcpSocket->set_option(option, ec);
      break;
   case vpr::SocketTypes::DATAGRAM:
      mUdpSocket->set_option(option, ec);
      break;
   }

   if ( ec )
   {
      std::ostringstream msg_stream;
      msg_stream << "[vpr::SocketImplBOOST::setOption()] ERROR: Could not set "
                 << "socket option for socket " << getName() << ": "
                 << strerror(errno);
      throw SocketException(msg_stream.str(), VPR_LOCATION);
   }
}

void SocketImplBOOST::setMcastInterface(const vpr::SocketOptions::Data& data)
{

}

void SocketImplBOOST::getNoPush(vpr::SocketOptions::Data& data) const
{
   // TODO Implement get TCP_CORK in BOOST using native()

   /*
   boost::asio::ip::tcp::no_delay option;
   boost::system::error_code ec;
   switch ( mType )
   {
   case vpr::SocketTypes::STREAM:
      mTcpSocket->get_option(option);
      break;
   case vpr::SocketTypes::DATAGRAM:
      mUdpSocket->get_option(option);
      break;
   }

   if ( !ec )
   {
      data.no_delay = option.value();
   }
   else
   {
      std::ostringstream msg_stream;
      msg_stream << "[vpr::SocketImplBOOST::getOption()] ERROR: Could not get "
                 << "socket option for socket " << getName() << ": "
                 << strerror(errno);
      throw SocketException(msg_stream.str(), VPR_LOCATION);
   }
   */
}

void SocketImplBOOST::setNoPush(const vpr::SocketOptions::Data& data)
{
   // TODO Implement set TCP_CORK in BOOST using native()

   /*
   boost::asio::ip::multicast::leave_group option(multi_addr);
   boost::system::error_code ec;

   switch ( mType )
   {
   case vpr::SocketTypes::STREAM:
      mTcpSocket->set_option(option, ec);
      break;
   case vpr::SocketTypes::DATAGRAM:
      mUdpSocket->set_option(option, ec);
      break;
   }

   if ( ec )
   {
      std::ostringstream msg_stream;
      msg_stream << "[vpr::SocketImplBOOST::setOption()] ERROR: Could not set "
                 << "socket option for socket " << getName() << ": "
                 << strerror(errno);
      throw SocketException(msg_stream.str(), VPR_LOCATION);
   }
   */
}

void SocketImplBOOST::getOption(const vpr::SocketOptions::Types option,
                                vpr::SocketOptions::Data& data) const
{
   switch ( option )
   {
      // Socket-level options.
      case vpr::SocketOptions::Linger:
         getLinger(data);
         break;
      case vpr::SocketOptions::ReuseAddr:
         getReuseAddr(data);
         break;
      case vpr::SocketOptions::KeepAlive:
         getKeepAlive(data);
         break;
      case vpr::SocketOptions::RecvBufferSize:
         getReceiveBufferSize(data);
         break;
      case vpr::SocketOptions::SendBufferSize:
         getSendBufferSize(data);
         break;
      case vpr::SocketOptions::Broadcast:
         getBroadcast(data);
         break;

      // IP-level options.
      case vpr::SocketOptions::IpTimeToLive:
         getUniTTL(data);
         break;
      case vpr::SocketOptions::IpTypeOfService:
         // TODO add IP_TOS support with BOOST
         break;
      case vpr::SocketOptions::AddMember:
      case vpr::SocketOptions::DropMember:
         fprintf(stderr,
                 "[vpr::SocketImplBOOST] Cannot get add/drop member socket option!\n");
         break;
      case vpr::SocketOptions::McastInterface:
         fprintf(stderr,
                 "[vpr::SocketImplBOOST] Cannot get multicast interface socket option!\n");
         break;
      case vpr::SocketOptions::McastTimeToLive:
         getMcastTTL(data);
         break;
      case vpr::SocketOptions::McastLoopback:
         getMcastLoopback(data);
         break;

      // TCP-level options.
      case vpr::SocketOptions::NoDelay:
         getNoDelay(data);
         break;
      case vpr::SocketOptions::MaxSegment:
         fprintf(stderr,
                 "[vpr::SocketImplBOOST] Cannot get max segment socket option!\n");
         break;
      case vpr::SocketOptions::NoPush:
#if defined(HAVE_CORKABLE_TCP)
         getNoPush(data);
#else
         // Maybe this should be a compile-time assertion.
         vprASSERT(false && "Should not have gotten here without TCP corking");
#endif
         break;
   }
}

void SocketImplBOOST::setOption(const vpr::SocketOptions::Types option,
                                const vpr::SocketOptions::Data& data)
{
   switch ( option )
   {
      // Socket-level options.
      case vpr::SocketOptions::Linger:
         setLinger(data);
         break;
      case vpr::SocketOptions::ReuseAddr:
         setReuseAddr(data);
         break;
      case vpr::SocketOptions::KeepAlive:
         setKeepAlive(data);
         break;
      case vpr::SocketOptions::RecvBufferSize:
         setReceiveBufferSize(data);
         break;
      case vpr::SocketOptions::SendBufferSize:
         setSendBufferSize(data);
         break;
      case vpr::SocketOptions::Broadcast:
         setBroadcast(data);
         break;


      // IP-level options.
      case vpr::SocketOptions::IpTimeToLive:
         setUniTTL(data);
         break;
      case vpr::SocketOptions::IpTypeOfService:
         // TODO Add IP_TOS for BOOST
         break;
      case vpr::SocketOptions::AddMember:
         setMcastAddMember(data);
         break;
      case vpr::SocketOptions::DropMember:
         setMcastDropMember(data);
         break;
      case vpr::SocketOptions::McastInterface:
         setMcastInterface(data);
         break;
      case vpr::SocketOptions::McastTimeToLive:
         setMcastTTL(data);
         break;
      case vpr::SocketOptions::McastLoopback:
         setMcastLoopback(data);
         break;

      // TCP-level options.
      case vpr::SocketOptions::NoDelay:
         setNoDelay(data);
         break;
      case vpr::SocketOptions::MaxSegment:
         break;
      case vpr::SocketOptions::NoPush:
#if defined(HAVE_CORKABLE_TCP)
         setNoPush(data);
#else
         // Maybe this should be a compile-time assertion.
         vprASSERT(false && "Should not have gotten here without TCP corking");
#endif
         break;

      // Unsetable
      case vpr::SocketOptions::Error:
         break;
   }
}

SocketImplBOOST::~SocketImplBOOST()
{
   switch ( mType )
   {
   case vpr::SocketTypes::STREAM:
      delete mTcpSocket;
      break;
   case vpr::SocketTypes::DATAGRAM:
      delete mUdpSocket;
      break;
   }
}

// ============================================================================
// Protected methods.
// ============================================================================

SocketImplBOOST::SocketImplBOOST(const vpr::SocketTypes::Type sockType)
   : mOpenBlocking(true)
   , mBound(false)
   , mConnectCalled(false)
   , mBlockingFixed(false)
   , mType(sockType)
   , mInBlockingCall(false)
   , mBytesRead(0)
{
   switch ( mType )
   {
   case vpr::SocketTypes::STREAM:
      mTcpSocket = new boost::asio::ip::tcp::socket(mIOService);
      break;
   case vpr::SocketTypes::DATAGRAM:
      mUdpSocket = new boost::asio::ip::udp::socket(mIOService);
      break;
   }
}

SocketImplBOOST::SocketImplBOOST(const vpr::InetAddr& localAddr,
                                 const vpr::InetAddr& remoteAddr,
                                 const vpr::SocketTypes::Type sockType)
   : mOpenBlocking(true)
   , mBound(false)
   , mConnectCalled(false)
   , mBlockingFixed(false)
   , mLocalAddr(localAddr)
   , mRemoteAddr(remoteAddr)
   , mType(sockType)
   , mInBlockingCall(false)
   , mBytesRead(0)
{
   switch ( mType )
   {
   case vpr::SocketTypes::STREAM:
      mTcpSocket = new boost::asio::ip::tcp::socket(mIOService);
      break;
   case vpr::SocketTypes::DATAGRAM:
      mUdpSocket = new boost::asio::ip::udp::socket(mIOService);
      break;
   }
}

} // End of vpr namespace
