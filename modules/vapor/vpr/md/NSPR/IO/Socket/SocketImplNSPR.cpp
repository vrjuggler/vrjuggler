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
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

#include <vpr/vprConfig.h>

#include <stdio.h>
#include <string.h>
#include <prinrval.h>
#include <prio.h>
#include <prerror.h>

#include <vpr/md/NSPR/IO/Socket/SocketImplNSPR.h>
#include <vpr/Util/Error.h>

namespace vpr
{

// ============================================================================
// Public methods.
// ============================================================================

// Open the socket.  This creates a new socket using the domain and type
// options set through member variables.
vpr::ReturnStatus SocketImplNSPR::open()
{
   vpr::ReturnStatus retval;
   PRFileDesc* new_sock = NULL;

   if(NULL != mHandle)
   {
      retval.setCode(vpr::ReturnStatus::Fail);
   }
   else
   {
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
        default:
          fprintf(stderr,
                  "[vpr::SocketImplNSPR] ERROR: Unknown socket type value %d\n",
                  mLocalAddr.getFamily());
          break;
      }

      // If socket(2) failed, print an error message and return error status.
      if ( new_sock == NULL )
      {
         vpr::Error::outputCurrentError(std::cerr, "[vpr::SocketImplNSPR] Could not create socket");
         retval.setCode(vpr::ReturnStatus::Fail);
      }
      // Otherwise, return success.
      else
      {
         mHandle = new_sock;
         mOpen = true;

         // Now that this socket is open, we can set the blocking state.
         setBlocking(mOpenBlocking);

         //vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL) << "Socket open: handle:[" << mHandle << "]\n" << vprDEBUG_FLUSH;
      }
   }

   return retval;
}

// Close the socket.
vpr::ReturnStatus SocketImplNSPR::close()
{
   vpr::ReturnStatus retval(vpr::ReturnStatus::Succeed);      // Default to success
   PRStatus status;

   if(NULL != mHandle)
   {
      //vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL) << "Socket close: handle:[" << mHandle << "] (Socket now invalid)\n" << vprDEBUG_FLUSH;

      status = PR_Close(mHandle);

      mHandle = NULL;                // mHandle points to shrapnel now, so write over it
      mOpen = false;                 // Even if failed, we must scrap the socket and start over
      mBound = false;
      mConnected = false;
      mBlockingFixed = false;

      if (status == PR_SUCCESS)
      {
         //mOpen = false;
         //mBound = false;
         retval.setCode(vpr::ReturnStatus::Succeed);
      }
      else
      {
         retval.setCode(vpr::ReturnStatus::Fail);
      }
   }

   return retval;
}

// Bind this socket to the address in the host address member variable.
vpr::ReturnStatus SocketImplNSPR::bind()
{
   vpr::ReturnStatus retval;
   PRStatus status;

   vprASSERT((true == mOpen) && "Trying to bind an un-opened socket");
   vprASSERT((mHandle != NULL) && "Trying to bind with NULL handle");

   // Bind the socket to the address in mLocalAddr.
   status = PR_Bind(mHandle, mLocalAddr.getPRNetAddr());

   // If that fails, print an error and return error status.
   if ( status == PR_FAILURE )
   {
      retval.setCode(vpr::ReturnStatus::Fail);
      vpr::Error::outputCurrentError(std::cerr, "SocketImplNSPR::bind: Failed to bind.");
   }
   // Otherwise, return success.
   else
   {
       mBound = true;
   }

   return retval;
}

vpr::ReturnStatus SocketImplNSPR::setBlocking(bool blocking)
{
   vprASSERT(! mBlockingFixed && "Can't change blocking state after blocking call");

   vpr::ReturnStatus retval;

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
            vpr::Error::outputCurrentError(std::cerr,
                                           "SocketImplNSPR::setBlocking: Failed to set.");
            retval.setCode(vpr::ReturnStatus::Fail);
         }
         else
         {
            mBlocking = blocking;
         }
      }
      else
      {
         retval.setCode(vpr::ReturnStatus::Fail);
      }
   }

   return retval;
}

// Connect the socket on the client side to the server side.  For a datagram
// socket, this makes the address given to the constructor the default
// destination for all packets.  For a stream socket, this has the effect of
// establishing a connection with the destination.
vpr::ReturnStatus SocketImplNSPR::connect(vpr::Interval timeout)
{
   vpr::ReturnStatus retval;
   PRStatus status;

   vprASSERT((true == mOpen) && "Trying to connect an un-opened socket");
   vprASSERT((mHandle != NULL) && "Trying to connect with NULL handle");

   if(mConnected)
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL) << "SocketImplNSPR::connect: Socket already connected.  Can't connect again"
                    << vprDEBUG_FLUSH;
      retval.setCode(vpr::ReturnStatus::Fail);
   }
   else
   {
      // Attempt to connect to the address in mAddr.
      status = PR_Connect(mHandle, mRemoteAddr.getPRNetAddr(),
                          NSPR_getInterval(timeout) );

      if ( status == PR_FAILURE )
      {
         PRInt32 err;

         err = PR_GetError();

         // This is a non-blocking connection.
         if ( err == PR_WOULD_BLOCK_ERROR || err == PR_IN_PROGRESS_ERROR  )
         {
            if ( vpr::Interval::NoWait == timeout )
            {
               retval.setCode( vpr::ReturnStatus::InProgress );
            }
            // Use the timeout to wait for the connection to complete.
            else
            {
               PRPollDesc poll_desc;

               poll_desc.fd       = mHandle;
               poll_desc.in_flags = PR_POLL_WRITE | PR_POLL_EXCEPT;

               PR_Poll(&poll_desc, 1, NSPR_getInterval(timeout));

               // If the out flags don't have PR_POLL_WRITE, then we timed out.
               // Since we are a non-blocking socket, we tell the caller that
               // the connection is still in progress.
               if ( ! (poll_desc.out_flags & PR_POLL_WRITE) )
               {
                  retval.setCode(vpr::ReturnStatus::InProgress);
               }
            }

            mBound         = true;
            mConnected     = true;
            mBlockingFixed = true;
         }
         else if ( err == PR_IO_TIMEOUT_ERROR )
         {
            retval.setCode(vpr::ReturnStatus::Timeout);
         }
         else
         {
            vpr::Error::outputCurrentError(std::cerr, "SocketImplNSPR::connect: Failed to connect.");
            retval.setCode(vpr::ReturnStatus::Fail);
         }
      }
      // Otherwise, return success.
      else
      {
         mBound = true;
         mConnected = true;
         mBlockingFixed = true;
      }
   }

   // Fill in the local address if has not already been assigned.
   if ( mConnected && vpr::InetAddr::AnyAddr == mLocalAddr )
   {
      PRStatus status;

      status = PR_GetSockName(mHandle, mLocalAddr.getPRNetAddr());

      if ( status == PR_SUCCESS )
      {
/* XXX: This doesn't compile on IRIX, and I don't know why.
          vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL) << "Connected, local address is "
                                        << mLocalAddr << std::endl
                                        << vprDEBUG_FLUSH;
*/
      }
      else
      {
          vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL) << "Failed to get local socket name\n"
                                          << vprDEBUG_FLUSH;
      }
   }

   return retval;
}

vpr::ReturnStatus SocketImplNSPR::read_i(void* buffer,
                                         const vpr::Uint32 length,
                                         vpr::Uint32& bytesRead,
                                         const vpr::Interval timeout)
{
   if(mHandle == NULL)
   {
      return vpr::ReturnStatus::Fail;
   }

   vpr::ReturnStatus retval;
   PRInt32 bytes;

   mBlockingFixed = true;

   bytes = PR_Recv(mHandle, buffer, length, 0, NSPR_getInterval(timeout));

   if( bytes > 0)     // Successful read
   {
      bytesRead = bytes;
   }
   else if ( bytes == -1 )      // -1 indicates failure which includes PR_WOULD_BLOCK_ERROR.
   {
      PRErrorCode err_code = PR_GetError();
      vpr::Error::outputCurrentError(std::cerr, "SocketImplNSPR::read_i::Error -->");

      bytesRead = 0;

      if ( err_code == PR_WOULD_BLOCK_ERROR )
      {
         retval.setCode(vpr::ReturnStatus::WouldBlock);
      }
      else if ( err_code == PR_IO_TIMEOUT_ERROR )
      {
         retval.setCode(vpr::ReturnStatus::Timeout);
      }
      else if ( err_code == PR_CONNECT_RESET_ERROR ||
                err_code == PR_SOCKET_SHUTDOWN_ERROR ||
                err_code == PR_CONNECT_ABORTED_ERROR )
      {
         retval.setCode(vpr::ReturnStatus::NotConnected);
      }
      else
      {
         retval.setCode(vpr::ReturnStatus::Fail);
      }
   }
   else if( bytes == 0)    // Indicates that the network connection is closed
   {
      bytesRead = bytes;
      retval.setCode(vpr::ReturnStatus::NotConnected);    // Set status to indicate connection is closed
   }

   return retval;
}

vpr::ReturnStatus SocketImplNSPR::readn_i(void* buffer,
                                          const vpr::Uint32 length,
                                          vpr::Uint32& bytesRead,
                                          const vpr::Interval timeout)
{
   if(mHandle == NULL)
   {
      return vpr::ReturnStatus::Fail;
   }

   vpr::ReturnStatus retval;
   PRInt32 bytes;            // Number of bytes read each time
   vpr::Uint32 bytes_left(length);

   bytesRead = 0;
   mBlockingFixed = true;

   while ( bytes_left > 0 )
   {
      bytes = PR_Recv(mHandle, buffer, bytes_left, 0,
                      NSPR_getInterval(timeout));

      // We are in an error state.
      if ( bytes > 0 )
      {
         buffer = (void*) ((char*) buffer + bytes);
         bytes_left -= bytes;
         bytesRead  += bytes;
      }
      else if ( bytes < 0 )
      {
         PRErrorCode err_code = PR_GetError();

         // Non-blocking socket, but this is basically a blocking call.  We
         // just keep reading.
         if ( err_code == PR_WOULD_BLOCK_ERROR )
         {
            continue;
         }
         // The last read took longer than we wanted.
         else if ( err_code == PR_IO_TIMEOUT_ERROR )
         {
            retval.setCode(vpr::ReturnStatus::Timeout);
            return retval;
         }
         // We lost the connection.  We're done.
         else if ( err_code == PR_CONNECT_RESET_ERROR ||
                   err_code == PR_SOCKET_SHUTDOWN_ERROR ||
                   err_code == PR_CONNECT_ABORTED_ERROR )
         {
            retval.setCode(vpr::ReturnStatus::NotConnected);
            return retval;
         }
         // Unrecognized error.
         else
         {
            retval.setCode(vpr::ReturnStatus::Fail);
            return retval;
         }
      }
      // Read 0 bytes.
      else
      {
         retval.setCode(vpr::ReturnStatus::NotConnected);    // Set status to indicate connection is closed
         return retval;
      }
   }

   return retval;
}

vpr::ReturnStatus SocketImplNSPR::write_i(const void* buffer,
                                          const vpr::Uint32 length,
                                          vpr::Uint32& bytesWritten,
                                          const vpr::Interval timeout)
{
   if(mHandle == NULL)
      return vpr::ReturnStatus::Fail;

   vpr::ReturnStatus retval;
   PRInt32 bytes;

   mBlockingFixed = true;

   bytes = PR_Send(mHandle, buffer, length, 0, NSPR_getInterval(timeout));

   if ( bytes == -1 )
   {
      PRErrorCode err_code = PR_GetError();
      vpr::Error::outputCurrentError(std::cerr, "SocketImplNspr::write_i: Error --> ");

      bytesWritten = 0;

      if ( err_code == PR_WOULD_BLOCK_ERROR )
      {
         retval.setCode(vpr::ReturnStatus::WouldBlock);
      }
      else if ( err_code == PR_IO_TIMEOUT_ERROR )
      {
         retval.setCode(vpr::ReturnStatus::Timeout);
      }
      else if ( err_code == PR_NOT_CONNECTED_ERROR ||
                err_code == PR_CONNECT_RESET_ERROR ||
                err_code == PR_SOCKET_SHUTDOWN_ERROR ||
                err_code == PR_CONNECT_ABORTED_ERROR )
      {
         retval.setCode(vpr::ReturnStatus::NotConnected);
      }
      else
      {
         retval.setCode(vpr::ReturnStatus::Fail);
      }
   }
   else
   {
      bytesWritten = bytes;
   }

   return retval;
}

vpr::ReturnStatus SocketImplNSPR::getOption(const vpr::SocketOptions::Types option,
                                            vpr::SocketOptions::Data& data)
   const
{
   vpr::ReturnStatus retval;
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
               data.linger.seconds = PR_IntervalToSeconds(opt_data.value.linger.linger);
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
            case vpr::SocketOptions::AddMember:
            case vpr::SocketOptions::DropMember:
            default:
               vprASSERT(false && "Socket option handled incorrectly.");  // Should never get here
               break;
         }
      }
      else
      {
         retval.setCode(vpr::ReturnStatus::Fail);
         vpr::Error::outputCurrentError(std::cerr, "[vpr::SocketImplNSPR] ERROR: Could not get socket option for socket");
      }
   }
   else
   {
      retval.setCode(vpr::ReturnStatus::Fail);
   }

   return retval;
}

vpr::ReturnStatus SocketImplNSPR::setOption(const vpr::SocketOptions::Types option,
                                            const vpr::SocketOptions::Data& data)
{
   PRSocketOptionData opt_data;

   switch (option)
   {
      case vpr::SocketOptions::Linger:
         opt_data.option                = PR_SockOpt_Linger;
         opt_data.value.linger.polarity = data.linger.enabled;
         opt_data.value.linger.linger   = PR_SecondsToInterval(data.linger.seconds);
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
   }

   vprASSERT((mHandle != NULL) && "Trying to set option on NULL handle");
   if(mHandle == NULL)
   {
      return vpr::ReturnStatus(vpr::ReturnStatus::Fail);
   }
   else
   {
      if ( PR_SetSocketOption(mHandle, &opt_data) == PR_SUCCESS )
      {
         return vpr::ReturnStatus();
      }
      else
      {
         return vpr::ReturnStatus(vpr::ReturnStatus::Fail);
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
   , mConnected(false)
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
   , mConnected(false)
   , mOpenBlocking(true)
   , mBlocking(true)
   , mBlockingFixed(false)
{
   mName = mLocalAddr.getAddressString();
}

// Destructor.  This currently does nothing.
SocketImplNSPR::~SocketImplNSPR()
{
   if ( mHandle != NULL )
   {
      vpr::ReturnStatus status = close();      // Close the socket
      vprASSERT(status.success() && "Failed to close socket in SocketImplNSPR destructor");
      mHandle = NULL;
   }
}

} // End of vpr namespace
