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

#include <sstream>
#include <boost/concept_check.hpp>
#include <prinrval.h>
#include <prio.h>
#include <private/pprio.h>
#include <prerror.h>

#include <vpr/IO/Socket/ConnectionResetException.h>
#include <vpr/IO/Socket/ConnectionRefusedException.h>
#include <vpr/IO/Socket/UnknownHostException.h>
#include <vpr/IO/Socket/NoRouteToHostException.h>
#include <vpr/IO/IOException.h>
#include <vpr/IO/TimeoutException.h>
#include <vpr/IO/WouldBlockException.h>
#include <vpr/Util/Error.h>
#include <vpr/md/NSPR/NSPRHelpers.h>
#include <vpr/md/NSPR/IO/SelectorImplNSPR.h>
#include <vpr/md/NSPR/IO/Socket/SocketImplNSPR.h>


namespace vpr
{
namespace
{

// Given an error number (or errno) build up an exception with the
// correct type and error string and throw it.
//
// This helper comes in handy since we have to throw exceptions from so many places in
// the socket implementation.
void buildAndThrowException(PRFileDesc* handle, const PRErrorCode err_code,
                            const std::string& prefix,
                            const std::string& location)
{
#if ! defined(WINNT)
   boost::ignore_unused_variable_warning(handle);
#endif

   const std::string err_string = vpr::Error::getCurrentErrorMsg();

   // Build and throw exception
   if ( PR_IN_PROGRESS_ERROR == err_code ||
        PR_WOULD_BLOCK_ERROR == err_code )
   {
      throw vpr::WouldBlockException(
         prefix + "Operation still in progress: " + err_string, location
      );
   }
   else if ( PR_IS_CONNECTED_ERROR == err_code )
   {
      throw vpr::SocketException(
         prefix + "Socket already connected: " + err_string, location
      );
   }
   else if ( PR_CONNECT_REFUSED_ERROR == err_code )
   {
      throw vpr::ConnectionRefusedException(
         prefix + "Connection refused: " + err_string, location
      );
   }
   else if ( PR_CONNECT_RESET_ERROR == err_code ||
             PR_SOCKET_SHUTDOWN_ERROR == err_code ||
             PR_CONNECT_ABORTED_ERROR == err_code ||
             PR_NOT_CONNECTED_ERROR == err_code )
   {
      std::ostringstream msg_stream;

      if ( PR_CONNECT_RESET_ERROR == err_code )
      {
         msg_stream << "Connection reset: ";
      }
      else if ( PR_SOCKET_SHUTDOWN_ERROR == err_code )
      {
         msg_stream << "Socket shutdown: ";
      }
      else if ( PR_CONNECT_ABORTED_ERROR == err_code )
      {
         msg_stream << "Connection aborted: ";
      }
      else if ( PR_NOT_CONNECTED_ERROR == err_code )
      {
         msg_stream << "Not connected: ";
      }

      throw vpr::ConnectionResetException(
         prefix + msg_stream.str() + err_string, location
      );
   }
   else if ( PR_NETWORK_UNREACHABLE_ERROR == err_code )
   {
      throw vpr::NoRouteToHostException(
         prefix + "No route to host: " + err_string, location
      );
   }
   else if ( PR_CONNECT_TIMEOUT_ERROR == err_code ||
             PR_IO_TIMEOUT_ERROR == err_code )
   {
#if defined(WINNT)
      // Handle the case of a timeout error on an NT socket. We have to tell
      // NSPR to put the socket back into the right state. We do not need to
      // worry about whether the socket is blocking because the timeout is
      // ignored by non-blocking NSPR sockets.
      // NOTE: According to the NSPR tech note about this, PR_NT_CancelIo()
      // does not need to be called when PR_CONNECT_TIMEOUT_ERROR is returned.
      // See the following for more information:
      // http://www.mozilla.org/projects/nspr/tech-notes/ntiotimeoutinterrupt.html
      if ( PR_IO_TIMEOUT_ERROR == err_code )
      {
         PR_NT_CancelIo(handle);
      }
#endif

      throw vpr::TimeoutException(prefix + "Timeout: " + err_string, location);
   }
   else
   {
      throw vpr::SocketException(prefix + "Error: " + err_string, location);
   }
}

void buildAndThrowException(PRFileDesc* handle, const std::string& prefix,
                            const std::string& location)
{
   buildAndThrowException(handle, PR_GetError(), prefix, location);
}

}

// ============================================================================
// Public methods.
// ============================================================================

// Open the socket.  This creates a new socket using the domain and type
// options set through member variables.
void SocketImplNSPR::open()
{
   PRFileDesc* new_sock(NULL);

   if ( NULL != mHandle )
   {
      std::stringstream msg_stream;
      msg_stream << "[vpr::SocketImplNSPR::open] Socket " << getName()
                 << " has already been opened";

      throw SocketException(msg_stream.str(), VPR_LOCATION);
   }

   // NSPR has not concept of domain in socket creation
   // switch (mLocalAddr.getFamily())

   switch (mType)
   {
      case vpr::SocketTypes::STREAM:
         new_sock = PR_NewTCPSocket();
         break;
      case vpr::SocketTypes::DATAGRAM:
         new_sock = PR_NewUDPSocket();
         break;
      case vpr::SocketTypes::RAW:
         throw SocketException("RAW socket type not supported.", VPR_LOCATION);
         break;
      default:
         {
            std::stringstream msg_stream;
            msg_stream << "[vpr::SocketImplNSPR] ERROR: Unknown socket type "
                       << "value: " << unsigned(mType);
            throw SocketException(msg_stream.str(), VPR_LOCATION);
            break;
         }
   }

   // If socket(2) failed, print an error message and return error status.
   if ( new_sock == NULL )
   {
      buildAndThrowException(
         mHandle, "[vpr::SocketImplNSPR::open()] (" + getName() + ") ",
         VPR_LOCATION
      );
   }
   // Otherwise, return success.
   else
   {
      mHandle = new_sock;
      mOpen = true;

      // Now that this socket is open, we can set the blocking state.
      setBlocking(mOpenBlocking);

//      vprDEBUG(vprDBG_ALL, vprDBG_HEX_LVL)
//         << "[vpr::SocketImplNSPR::open()] handle = " << mHandle << "\n"
//         << vprDEBUG_FLUSH;
   }
}

// Close the socket.
void SocketImplNSPR::close()
{
   if ( NULL != mHandle )
   {
//      vprDEBUG(vprDBG_ALL, vprDBG_HEX_LVL)
//         << "[vpr::SocketImplNSPR::close()] handle = " << mHandle
//         << " (Socket now invalid)\n" << vprDEBUG_FLUSH;

      PRStatus status = PR_Close(mHandle);

      mHandle = NULL; // mHandle points to shrapnel now, so write over it
      mOpen = false;  // Even if failed, we must scrap the socket and start over
      mBound = false;
      mConnectCalled = false;
      mBlockingFixed = false;

      if ( status == PR_SUCCESS )
      {
         //mOpen = false;
         //mBound = false;
      }
      else
      {
         buildAndThrowException(
            mHandle, "[vpr::SocketImplNSPR::open()] Could not close socket:",
            VPR_LOCATION
         );
      }
   }
}

// Bind this socket to the address in the host address member variable.
void SocketImplNSPR::bind()
{
   vprASSERT(true == mOpen && "Trying to bind an un-opened socket");
   vprASSERT(mHandle != NULL && "Trying to bind with NULL handle");

   // Bind the socket to the address in mLocalAddr.
   PRStatus status = PR_Bind(mHandle, mLocalAddr.getPRNetAddr());

   // If that fails, print an error and return error status.
   if ( status == PR_FAILURE )
   {
      std::ostringstream ex_text;
      ex_text << "[vpr::SocketImplNSPR::bind()] " << " addr: '" << mLocalAddr
              << "' ";
      buildAndThrowException(mHandle, ex_text.str(), VPR_LOCATION);
   }
   // Otherwise, return success.
   else
   {
      mBound = true;
   }
}

void SocketImplNSPR::setBlocking(bool blocking)
{
   vprASSERT(! mBlockingFixed && "Can't change blocking state after blocking call");

   // If this socket has not been opened yet, set mOpenBlocking to the value
   // of blocking.  The actual change for the blocking state has to occur
   // after the socket has been opened.
   if ( ! mOpen )
   {
      mOpenBlocking = blocking;
   }
   // If this socket is already open, change the blocking state to match the
   // value of blocking.
   else
   {
      if ( ! mBlockingFixed )
      {
         PRStatus status;
         PRSocketOptionData option_data;
         option_data.option = PR_SockOpt_Nonblocking;

         if ( blocking )
         {
            option_data.value.non_blocking = false;
         }
         else
         {
            option_data.value.non_blocking = true;
         }

         status = PR_SetSocketOption(mHandle, &option_data);

         // If that fails, print an error and return error status.
         if ( status == PR_FAILURE )
         {
            std::stringstream msg_stream;
            msg_stream << "[vpr::SocketImplNSPR::setBlocking()] Failed to "
                       << "change blocking state: "
                       << vpr::Error::getCurrentErrorMsg();
            throw IOException(msg_stream.str(), VPR_LOCATION);
         }
         else
         {
            mBlocking = blocking;
         }
      }
      else
      {
         throw SocketException(
            "Cannot change blocking state after a blocking call.",
            VPR_LOCATION
         );
      }
   }
}

// Connect the socket on the client side to the server side.  For a datagram
// socket, this makes the address given to the constructor the default
// destination for all packets.  For a stream socket, this has the effect of
// establishing a connection with the destination.
void SocketImplNSPR::connect(const vpr::Interval& timeout)
{
   vprASSERT(true == mOpen && "Trying to connect an un-opened socket");
   vprASSERT(mHandle != NULL && "Trying to connect with NULL handle");

   if ( mConnectCalled )
   {
      std::string err_msg(
         "[vpr::SocketImplNSPR::connect()] Socket already connected. Can't connect again!"
      );

      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL) << err_msg << std::endl
                                                << vprDEBUG_FLUSH;
      throw SocketException(err_msg, VPR_LOCATION);
   }

#ifdef VPR_OS_Windows
   vprASSERT(vpr::InetAddr::AnyAddr != mRemoteAddr &&
             "INADDR_ANY is not a valid desination on Windows.");
#endif

   // Attempt to connect to the address in mAddr.
   PRStatus status = PR_Connect(mHandle, mRemoteAddr.getPRNetAddr(),
                                NSPR_getInterval(timeout));

   if ( status == PR_FAILURE )
   {
      const PRErrorCode err = PR_GetError();

      // This is a non-blocking connection.
      if ( (err == PR_WOULD_BLOCK_ERROR || err == PR_IN_PROGRESS_ERROR) &&
           ! isBlocking() )
      {
         // Use the timeout to wait for the connection to complete.
         if ( vpr::Interval::NoWait != timeout )
         {
            // Force socket to wait for timeout interval to expire before
            // returning. This provides a way for the caller to specify
            // that they want the connection process to block even with a
            // non-blocking socket.
            try
            {
               // Wait for read/write on the socket.
               SelectorImplNSPR selector;
               selector.addHandle(getHandle(), SelectorBase::Read |
                                                  SelectorBase::Write |
                                                  SelectorBase::Except |
                                                  SelectorBase::Error);
               vpr::Uint16 num_events(0);
               selector.select(num_events, timeout);

               vpr::Uint16 event = selector.getOut(getHandle());
               if ( SelectorBase::Except == event ||
                    SelectorBase::Error == event )
               {
                  close();
                  // XXX: This throws a WouldBlockException, even though it
                  //      shouldn't because the PR_Connect() call failed.
                  //buildAndThrowException(mHandle,
                  //                       "[vpr::SocketImplNSPR::connect()] ",
                  //                       VPR_LOCATION);
                  throw SocketException(
                     "[vpr::SocketImplNSPR::connect()] Non-Blocking socket with timeout failed: ",
                     VPR_LOCATION
                  );
               }

               mBound = true;
               mConnectCalled = true;
               mBlockingFixed = true;
            }
            // Select timed out, so the connect timed out
            catch (TimeoutException& te)
            {
               close();
               throw TimeoutException("Timeout while connecting.",
                                      VPR_LOCATION);
            }
         }
         else // non-blocking connect started
         {
            mBound         = true;
            mConnectCalled = true;
            mBlockingFixed = true;
         }
      }
      else
      {
         buildAndThrowException(mHandle, err,
                                "[vpr::SocketImplNSPR::connect()] ",
                                VPR_LOCATION);
      }
   }
   // Otherwise, return success.
   else
   {
      mBound         = true;
      mConnectCalled = true;
      mBlockingFixed = true;
   }

   // Fill in the local address if has not already been assigned.
   if ( mConnectCalled && vpr::InetAddr::AnyAddr == mLocalAddr )
   {
      PRStatus status;

      status = PR_GetSockName(mHandle, mLocalAddr.getPRNetAddr());

      if ( status == PR_SUCCESS )
      {
/* XXX: This doesn't compile on IRIX, and I don't know why.
         vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
            << "Connected, local address is " << mLocalAddr << std::endl
            << vprDEBUG_FLUSH;
*/
      }
      else
      {
          vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
            << "Failed to get local socket name\n" << vprDEBUG_FLUSH;
      }
   }
}

bool SocketImplNSPR::isConnected() const
{
   // If it is not open, then it can't be connected.
   if ( isOpen() && mConnectCalled )
   {
      try
      {
         SelectorImplNSPR selector;
         selector.addHandle(getHandle(), SelectorBase::Read |
                                            SelectorBase::Write |
                                            SelectorBase::Except |
                                            SelectorBase::Error);

         vpr::Uint16 num_events(0);
         selector.select(num_events, vpr::Interval::NoWait);

         const vpr::Uint16 event = selector.getOut(getHandle());
         if ( SelectorBase::Except == event || SelectorBase::Error == event )
         {
            //close();
            throw SocketException("[vpr::SocketImplNSPR::isConnected()] ",
                                  VPR_LOCATION);
         }

      }
      catch (vpr::TimeoutException& ex)
      {
         boost::ignore_unused_variable_warning(ex);
         return false;
      }

      return true;
   }

   return false;
}

vpr::Uint32 SocketImplNSPR::read_i(void* buffer, const vpr::Uint32 length,
                                   const vpr::Interval& timeout)
{
   vprASSERT(NULL != mHandle && "Can not read from a socket with a NULL handle.");

   vpr::Uint32 bytes_read(0);
   mBlockingFixed = true;

   const PRInt32 bytes = PR_Recv(mHandle, buffer, length, 0,
                                 NSPR_getInterval(timeout));

   // Successful read.
   if ( bytes > 0 )
   {
      bytes_read = bytes;
   }
   // -1 indicates failure which includes PR_WOULD_BLOCK_ERROR.
   else if ( bytes == -1 )
   {
      buildAndThrowException(mHandle, "[vpr::SocketImplNSPR::read_i()] ",
                             VPR_LOCATION);
   }
   // Indicates that the network connection is closed.
   else if ( bytes == 0 )
   {
      std::stringstream msg_stream;
      msg_stream << "Socket disconnected cleanly. ";
      const std::string nspr_err_msg(vpr::Error::getCurrentErrorMsg());

      if ( ! nspr_err_msg.empty() )
      {
         msg_stream << ": " << nspr_err_msg;
      }

      throw ConnectionResetException(msg_stream.str(), VPR_LOCATION);
   }

   return bytes_read;
}

vpr::Uint32 SocketImplNSPR::readn_i(void* buffer, const vpr::Uint32 length,
                                    const vpr::Interval& timeout)
{
   vprASSERT(NULL != mHandle && "Can not read from a socket with a NULL handle.");

   vpr::Uint32 bytes_left(length);

   vpr::Uint32 bytes_read(0);
   mBlockingFixed = true;

   while ( bytes_left > 0 )
   {
      const PRInt32 bytes = PR_Recv(mHandle, buffer, bytes_left, 0,
                                    NSPR_getInterval(timeout));

      // We are in an error state.
      if ( bytes > 0 )
      {
         buffer = (void*) ((char*) buffer + bytes);
         bytes_left -= bytes;
         bytes_read += bytes;
      }
      else if ( bytes < 0 )
      {
         const PRErrorCode err_code = PR_GetError();

         // Non-blocking socket, but this is basically a blocking call.  We
         // just keep reading.
         if ( err_code == PR_WOULD_BLOCK_ERROR )
         {
            continue;
         }

         buildAndThrowException(mHandle, err_code,
                                "[vpr::SocketImplNSPR::readn_i] ",
                                VPR_LOCATION);
      }
      // Read 0 bytes.
      else
      {
         // Throw an exception to indicate connection is closed.
         std::stringstream msg_stream;
         msg_stream << "Connection closed";
         const std::string nspr_err_msg(vpr::Error::getCurrentErrorMsg());

         if ( ! nspr_err_msg.empty() )
         {
            msg_stream << ": " << nspr_err_msg;
         }

         throw ConnectionResetException(msg_stream.str(), VPR_LOCATION);
      }
   }

   return bytes_read;
}

vpr::Uint32 SocketImplNSPR::write_i(const void* buffer,
                                    const vpr::Uint32 length,
                                    const vpr::Interval& timeout)
{
   vprASSERT(NULL != mHandle && "Can not write to a socket with a NULL handle.");

   vpr::Uint32 bytes_written(0);
   mBlockingFixed = true;

   const PRInt32 bytes = PR_Send(mHandle, buffer, length, 0,
                                 NSPR_getInterval(timeout));

   if ( bytes == -1 )
   {
      buildAndThrowException(
         mHandle, "[vpr::SocketImplNSPR::write_i()] (" + getName() + ") ",
         VPR_LOCATION
      );
   }
   else
   {
      bytes_written = bytes;
   }

   return bytes_written;
}

void SocketImplNSPR::getOption(const vpr::SocketOptions::Types option,
                               vpr::SocketOptions::Data& data)
   const
{
   PRSocketOptionData opt_data;
   bool get_opt;

   // If this is true, PR_GetSocketOption() will be called.
   get_opt = true;

   switch (option)
   {
      case vpr::SocketOptions::Linger:
         opt_data.option = PR_SockOpt_Linger;
         break;
      case vpr::SocketOptions::ReuseAddr:
         opt_data.option = PR_SockOpt_Reuseaddr;
         break;
      case vpr::SocketOptions::KeepAlive:
         opt_data.option = PR_SockOpt_Keepalive;
         break;
      case vpr::SocketOptions::RecvBufferSize:
         opt_data.option = PR_SockOpt_RecvBufferSize;
         break;
      case vpr::SocketOptions::SendBufferSize:
         opt_data.option = PR_SockOpt_SendBufferSize;
         break;
      case vpr::SocketOptions::Broadcast:
         opt_data.option = PR_SockOpt_Broadcast;
         break;
      case vpr::SocketOptions::IpTimeToLive:
         opt_data.option = PR_SockOpt_IpTimeToLive;
         break;
      case vpr::SocketOptions::IpTypeOfService:
         opt_data.option = PR_SockOpt_IpTypeOfService;
         break;
      case vpr::SocketOptions::AddMember:
      case vpr::SocketOptions::DropMember:
         fprintf(stderr,
                 "[vpr::SocketImplNSPR] Cannot get add/drop member socket option!\n");
         get_opt = false;
         break;
      case vpr::SocketOptions::McastInterface:
         opt_data.option = PR_SockOpt_McastInterface;
         break;
      case vpr::SocketOptions::McastTimeToLive:
         opt_data.option = PR_SockOpt_McastTimeToLive;
         break;
      case vpr::SocketOptions::McastLoopback:
         opt_data.option = PR_SockOpt_McastLoopback;
         break;
      case vpr::SocketOptions::NoDelay:
         opt_data.option = PR_SockOpt_NoDelay;
         break;
      case vpr::SocketOptions::MaxSegment:
         opt_data.option = PR_SockOpt_MaxSegment;
         break;
      case vpr::SocketOptions::NoPush:
         vprASSERT(false && "Should not have gotten here with NSPR sockets");
         break;
      default:
         throw SocketException("Unsupported option passed to getOption().",
                               VPR_LOCATION);
         break;
   }

   if ( get_opt )
   {
      PRStatus status;

      status = PR_GetSocketOption(mHandle, &opt_data);

      if ( status == PR_SUCCESS )
      {
         // This extracts the information from the union passed to
         // PR_GetSocketOption() and puts it in our friendly
         // vpr::SocketOptions::Data object.
         switch (option)
         {
            case vpr::SocketOptions::Linger:
               data.linger.enabled = opt_data.value.linger.polarity;
               data.linger.seconds =
                  PR_IntervalToSeconds(opt_data.value.linger.linger);
               break;
            case vpr::SocketOptions::ReuseAddr:
               data.reuse_addr = (opt_data.value.reuse_addr != 0 ? true
                                                                 : false);
               break;
            case vpr::SocketOptions::KeepAlive:
               data.keep_alive = (opt_data.value.keep_alive != 0 ? true
                                                                 : false);
               break;
            case vpr::SocketOptions::RecvBufferSize:
               data.recv_buffer_size = opt_data.value.recv_buffer_size;
               break;
            case vpr::SocketOptions::SendBufferSize:
               data.send_buffer_size = opt_data.value.send_buffer_size;
               break;
            case vpr::SocketOptions::Broadcast:
               data.broadcast = opt_data.value.broadcast;
               break;
            case vpr::SocketOptions::IpTimeToLive:
               data.ip_ttl = opt_data.value.ip_ttl;
               break;
            case vpr::SocketOptions::IpTypeOfService:
/*
               switch (opt_data.value.ip_tos)
               {
                  case IPTOS_LOWDELAY:
                     data.type_of_service = vpr::SocketOptions::LowDelay;
                     break;
                  case IPTOS_THROUGHPUT:
                     data.type_of_service = vpr::SocketOptions::Throughput;
                     break;
                  case IPTOS_RELIABILITY:
                     data.type_of_service = vpr::SocketOptions::Reliability;
                     break;
#ifdef IPTOS_LOWCOST
                  case IPTOS_LOWCOST:
                     data.type_of_service = vpr::SocketOptions::LowCost;
                     break;
#endif
               }
*/

               break;
            case vpr::SocketOptions::McastInterface:
//               data.mcast_if = vpr::InetAddr(opt_data.value.mcast_if);
               break;
            case vpr::SocketOptions::McastTimeToLive:
               data.mcast_ttl = opt_data.value.mcast_ttl;
               break;
            case vpr::SocketOptions::McastLoopback:
               data.mcast_loopback = opt_data.value.mcast_loopback;
               break;
            case vpr::SocketOptions::NoDelay:
               data.no_delay = (opt_data.value.no_delay != 0 ? true : false);
               break;
            case vpr::SocketOptions::MaxSegment:
               data.max_segment = opt_data.value.max_segment;
               break;
            case vpr::SocketOptions::NoPush:
               vprASSERT(false &&
                         "Should not have gotten here with NSPR sockets");
               break;
            case vpr::SocketOptions::AddMember:
            case vpr::SocketOptions::DropMember:
            default:
               // Should never get here
               vprASSERT(false && "Socket option handled incorrectly.");
               break;
         }
      }
      else
      {
         std::stringstream msg_stream;
         msg_stream << "[vpr::SocketImplNSPR::getOption()] ERROR: Could "
                    << "not get socket option for socket: "
                    << vpr::Error::getCurrentErrorMsg();
         throw SocketException(msg_stream.str(), VPR_LOCATION);
      }
   }
   else
   {
      throw SocketException("Unsupported use of PR_GetSocketOption()",
                            VPR_LOCATION);
   }
}

void SocketImplNSPR::setOption(const vpr::SocketOptions::Types option,
                               const vpr::SocketOptions::Data& data)
{
   PRSocketOptionData opt_data;

   switch (option)
   {
      case vpr::SocketOptions::Linger:
         opt_data.option                = PR_SockOpt_Linger;
         opt_data.value.linger.polarity = data.linger.enabled;
         opt_data.value.linger.linger   =
            PR_SecondsToInterval(data.linger.seconds);
         break;
      case vpr::SocketOptions::ReuseAddr:
         opt_data.option           = PR_SockOpt_Reuseaddr;
         opt_data.value.reuse_addr = (data.reuse_addr ? PR_TRUE : PR_FALSE);
         break;
      case vpr::SocketOptions::KeepAlive:
         opt_data.option           = PR_SockOpt_Keepalive;
         opt_data.value.keep_alive = (data.keep_alive ? PR_TRUE : PR_FALSE);
         break;
      case vpr::SocketOptions::RecvBufferSize:
         opt_data.option                 = PR_SockOpt_RecvBufferSize;
         opt_data.value.recv_buffer_size = data.recv_buffer_size;
         break;
      case vpr::SocketOptions::SendBufferSize:
         opt_data.option                 = PR_SockOpt_SendBufferSize;
         opt_data.value.send_buffer_size = data.send_buffer_size;
         break;
      case vpr::SocketOptions::Broadcast:
         opt_data.option                 = PR_SockOpt_Broadcast;
         opt_data.value.broadcast = data.broadcast;
         break;
      case vpr::SocketOptions::IpTimeToLive:
         opt_data.option       = PR_SockOpt_IpTimeToLive;
         opt_data.value.ip_ttl = data.ip_ttl;
         break;
      case vpr::SocketOptions::IpTypeOfService:
         opt_data.option = PR_SockOpt_IpTypeOfService;

         switch (data.type_of_service)
         {
            case vpr::SocketOptions::LowDelay:
//               opt_data.value.tos = ???;
               break;
            case vpr::SocketOptions::Throughput:
//               opt_data.value.tos = ???;
               break;
            case vpr::SocketOptions::Reliability:
//               opt_data.value.tos = ???;
               break;
            case vpr::SocketOptions::LowCost:
#ifdef IPTOS_LOWCOST
//               opt_data.value.tos = ???;
#else
               fprintf(stderr,
                       "[vpr::SocketImplNSPR] WARNING: This subsystem does not "
                       "support LowCost type of service!\n");
#endif
               break;
         }

         break;
      case vpr::SocketOptions::AddMember:
         opt_data.option = PR_SockOpt_AddMember;
//         opt_data.value.mcast_req.mcaddr = data.mcast_add_member.getMulticastAddr().getAddressValue();
//         opt_data.value.mcast_req.ifaddr = data.mcast_add_member.getInterfaceAddr().getAddressValue();
         break;
      case vpr::SocketOptions::DropMember:
         opt_data.option = PR_SockOpt_DropMember;
//         opt_data.value.mcast_req.mcaddr = data.mcast_drop_member.getMulticastAddr().getAddressValue();
//         opt_data.value.mcast_req.ifaddr = data.mcast_drop_member.getInterfaceAddr().getAddressValue();
         break;
      case vpr::SocketOptions::McastInterface:
         opt_data.option         = PR_SockOpt_McastInterface;
//         opt_data.value.mcast_if = data.mcast_if.getAddressValue();
         break;
      case vpr::SocketOptions::McastTimeToLive:
         opt_data.option          = PR_SockOpt_McastTimeToLive;
         opt_data.value.mcast_ttl = data.mcast_ttl;
         break;
      case vpr::SocketOptions::McastLoopback:
         opt_data.option               = PR_SockOpt_McastLoopback;
         opt_data.value.mcast_loopback = data.mcast_loopback;
         break;
      case vpr::SocketOptions::NoDelay:
         opt_data.option         = PR_SockOpt_NoDelay;
         opt_data.value.no_delay = (data.no_delay ? PR_TRUE : PR_FALSE);
         break;
      case vpr::SocketOptions::MaxSegment:
         opt_data.option            = PR_SockOpt_MaxSegment;
         opt_data.value.max_segment = data.max_segment;
         break;
      case vpr::SocketOptions::NoPush:
         vprASSERT(false && "Should not have gotten here with NSPR sockets");
         break;
      default:
         throw SocketException("Unsupported option passed to setOption().",
                               VPR_LOCATION);
         break;
   }

   if ( mHandle == NULL )
   {
      throw SocketException("Cannot set option on a NULL socket.",
                            VPR_LOCATION);
   }
   else
   {
      if ( PR_SetSocketOption(mHandle, &opt_data) != PR_SUCCESS )
      {
         std::stringstream msg_stream;
         msg_stream << "Failed to set socket option: "
                    << vpr::Error::getCurrentErrorMsg();
         throw SocketException(msg_stream.str(), VPR_LOCATION);
      }
   }
}

// ============================================================================
// Protected methods.
// ============================================================================

// Default constructor.  This just initializes member variables to reasonable
// defaults.
SocketImplNSPR::SocketImplNSPR(const vpr::SocketTypes::Type sockType)
   : mHandle(NULL)
   , mType(sockType)
   , mOpen(false)
   , mBound(false)
   , mConnectCalled(false)
   , mOpenBlocking(true)
   , mBlocking(true)
   , mBlockingFixed(false)
{
   /* Do nothing. */ ;
}

// Standard constructor.  This takes the given address (a string containing a
// hostname or an IP address), port, domain and type and stores the values in
// the member variables for use when opening the socket and performing
// communications.
SocketImplNSPR::SocketImplNSPR(const vpr::InetAddr& localAddr,
                               const vpr::InetAddr& remoteAddr,
                               const vpr::SocketTypes::Type sockType)
   : mHandle(NULL)
   , mLocalAddr(localAddr)
   , mRemoteAddr(remoteAddr)
   , mType(sockType)
   , mOpen(false)
   , mBound(false)
   , mConnectCalled(false)
   , mOpenBlocking(true)
   , mBlocking(true)
   , mBlockingFixed(false)
{
   mName = mLocalAddr.getAddressString();
}

// XXX: We need to have a reference count here
SocketImplNSPR::SocketImplNSPR(const SocketImplNSPR& sock)
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
   /* Do nothing. */ ;
}

// Destructor.  This currently does nothing.
SocketImplNSPR::~SocketImplNSPR()
{
   if ( mHandle != NULL )
   {
      try
      {
         close();      // Close the socket
         mHandle = NULL;
      }
      catch (IOException& ex)
      {
         vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
            << clrOutBOLD(clrRED, "ERROR")
            << ": Caught exception while trying to close socket\n"
            << ex.what() << std::endl << vprDEBUG_FLUSH;
         vprASSERT(false &&
                   "Failed to close socket in SocketImplNSPR destructor");
      }
   }

#ifdef VPR_DEBUG
   // Mark socket as destroyed
   mHandle = (PRFileDesc*)(0xDEADBEEF);
   mName = "Destructed";
#endif
}

} // End of vpr namespace
