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

#include <vpr/vprConfig.h>

#include <stdio.h>
#include <string.h>
#include <prinrval.h>
#include <prio.h>
#include <prerror.h>

#include <vpr/md/NSPR/IO/Socket/SocketImplNSPR.h>
#include <vpr/md/NSPR/NSPRHelpers.h>

namespace vpr {

// ============================================================================
// Public methods.
// ============================================================================

// ----------------------------------------------------------------------------
// Open the socket.  This creates a new socket using the domain and type
// options set through member variables.
// ----------------------------------------------------------------------------
vpr::Status
SocketImplNSPR::open () {
   vpr::Status retval;
   PRFileDesc* new_sock = NULL;

   if(NULL != m_handle) {
      retval.setCode(vpr::Status::Failure);
   }
   else {
      // NSPR has not concept of domain in socket creation
      // switch (m_local_addr.getFamily())

      switch (m_type) {
        case vpr::SocketTypes::STREAM:
          new_sock = PR_NewTCPSocket();
          break;
        case vpr::SocketTypes::DATAGRAM:
          new_sock = PR_NewUDPSocket();
          break;
        default:
          fprintf(stderr,
                  "[vpr::SocketImplNSPR] ERROR: Unknown socket type value %d\n",
                  m_local_addr.getFamily());
          break;
      }

      // If socket(2) failed, print an error message and return error status.
      if ( new_sock == NULL ) {
         NSPR_PrintError("[vpr::SocketImplNSPR] Could not create socket");
         retval.setCode(vpr::Status::Failure);
      }
      // Otherwise, return success.
      else {
         m_handle = new_sock;
         m_open = true;

         if ( ! m_open_blocking ) {
            enableNonBlocking();
         }
      }
   }

   return retval;
}

// ----------------------------------------------------------------------------
// Close the socket.
// ----------------------------------------------------------------------------
vpr::Status
SocketImplNSPR::close () {
    vpr::Status retval;
    PRStatus status;

    status = PR_Close(m_handle);

    if (status == PR_SUCCESS) {
       m_open = false;
       m_bound = false;
       retval.setCode(vpr::Status::Success);
    }
    else {
       retval.setCode(vpr::Status::Failure);
    }

    return retval;
}

// ----------------------------------------------------------------------------
// Bind this socket to the address in the host address member variable.
// ----------------------------------------------------------------------------
vpr::Status
SocketImplNSPR::bind () {
    vpr::Status retval;
    PRStatus status;

    // Bind the socket to the address in m_local_addr.
    status = PR_Bind(m_handle, m_local_addr.getPRNetAddr());

    // If that fails, print an error and return error status.
    if ( status == PR_FAILURE )
    {
       retval.setCode(vpr::Status::Failure);
       NSPR_PrintError("SocketImplNSPR::bind: Failed to bind.");
    }
    // Otherwise, return success.
    else {
        m_bound = true;
    }

    return retval;
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
vpr::Status
SocketImplNSPR::enableBlocking () {

   assert( m_open && "precondition says you must open() the socket first" );

   vpr::Status retval;

   if (m_blocking_fixed) {
       vprDEBUG(0,0) << "NSPRSocketImpl::enableBlocking: Can't enable blocking after blocking call\n"
                     << vprDEBUG_FLUSH;
       retval.setCode(vpr::Status::Failure);
   }
   else {
      PRStatus status;
      PRSocketOptionData option_data;
      option_data.option = PR_SockOpt_Nonblocking;
      option_data.value.non_blocking = false;

      status = PR_SetSocketOption(m_handle, &option_data);

      // If that fails, print an error and return error status.
      if ( status == PR_FAILURE )
      {
         NSPR_PrintError("SocketImplNSPR::enableBlocking: Failed to set.");
         retval.setCode(vpr::Status::Failure);
      }
      else {
         m_blocking = true;
      }
   }

   return retval;
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
vpr::Status
SocketImplNSPR::enableNonBlocking () {
   vpr::Status retval;

   assert( m_open && "precondition says you must open() the socket first" );

   if(m_blocking_fixed)
   {
      vprDEBUG(0,0) << "NSPRSocketImpl::enableBlocking: Can't diable blocking after blocking call\n"
                    << vprDEBUG_FLUSH;
      retval.setCode(vpr::Status::Failure);
   }
   else {
      PRStatus status;
      PRSocketOptionData option_data;
      option_data.option = PR_SockOpt_Nonblocking;
      option_data.value.non_blocking = true;

      status = PR_SetSocketOption(m_handle, &option_data);

      // If that fails, print an error and return error status.
      if ( status == PR_FAILURE )
      {
         NSPR_PrintError("SocketImplNSPR::enableNonBlocking: Failed to set.");
         retval.setCode(vpr::Status::Failure);
      }
      else {
         m_blocking = false;
      }
   }

   return retval;
}

// ----------------------------------------------------------------------------
// Connect the socket on the client side to the server side.  For a datagram
// socket, this makes the address given to the constructor the default
// destination for all packets.  For a stream socket, this has the effect of
// establishing a connection with the destination.
// ----------------------------------------------------------------------------
vpr::Status
SocketImplNSPR::connect (vpr::Interval timeout) {
   vpr::Status retval;
   PRStatus status;

   if(m_bound)
   {
      vprDEBUG(0,0) << "SocketImplNSPR::connect: Socket alreay bound.  Can't connect"
                    << vprDEBUG_FLUSH;
      retval.setCode(vpr::Status::Failure);
   }
   else {
      // Attempt to connect to the address in m_addr.
      status = PR_Connect(m_handle, m_remote_addr.getPRNetAddr(),
                          NSPR_getInterval(timeout) );

      if ( status == PR_FAILURE )
      {
         PRInt32 err;

         err = PR_GetError();

         if ( err == PR_WOULD_BLOCK_ERROR || err == PR_IN_PROGRESS_ERROR  ) {
            retval.setCode( vpr::Status::InProgress );
         }
         else if ( err == PR_IO_TIMEOUT_ERROR ) {
            retval.setCode(vpr::Status::Timeout);
         }
         else {
            NSPR_PrintError("SocketImplNSPR::connect: Failed to connect.");
            retval.setCode(vpr::Status::Failure);
         }
      }
      // Otherwise, return success.
      else
      {
         m_bound = true;
         m_blocking_fixed = true;
      }
   }

   return retval;
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
vpr::Status
SocketImplNSPR::read_i (void* buffer, const vpr::Uint32 length,
                        vpr::Uint32& bytes_read,
                        const vpr::Interval timeout)
{
   vpr::Status retval;
   PRInt32 bytes;

   m_blocking_fixed = true;

   bytes = PR_Recv(m_handle, buffer, length, 0, NSPR_getInterval(timeout));

   // -1 indicates failure which includes PR_WOULD_BLOCK_ERROR.
   if ( bytes_read == -1 ) {
      PRErrorCode err_code = PR_GetError();

      bytes_read = 0;

      if ( err_code == PR_WOULD_BLOCK_ERROR ) {
         retval.setCode(vpr::Status::WouldBlock);
      }
      else if ( err_code == PR_IO_TIMEOUT_ERROR ) {
         retval.setCode(vpr::Status::Timeout);
      }
      else {
         retval.setCode(vpr::Status::Failure);
      }
   }
   else {
      bytes_read = bytes;
   }

   return retval;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
vpr::Status
SocketImplNSPR::readn_i (void* buffer, const vpr::Uint32 length,
                         vpr::Uint32& bytes_read,
                         const vpr::Interval timeout)
{
   vpr::Status retval;
   PRInt32 bytes;

   m_blocking_fixed = true;

   bytes = PR_Recv(m_handle, buffer, length, 0, NSPR_getInterval(timeout));

   // -1 indicates failure which includes PR_WOULD_BLOCK_ERROR.
   if ( bytes == -1 ) {
      PRErrorCode err_code = PR_GetError();

      bytes_read = 0;

      if ( err_code == PR_WOULD_BLOCK_ERROR ) {
         retval.setCode(vpr::Status::WouldBlock);
      }
      else if ( err_code == PR_IO_TIMEOUT_ERROR ) {
         retval.setCode(vpr::Status::Timeout);
      }
      else {
         retval.setCode(vpr::Status::Failure);
      }
   }
   else {
      bytes_read = bytes;
   }

   return retval;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
vpr::Status
SocketImplNSPR::write_i (const void* buffer, const vpr::Uint32 length,
                         vpr::Uint32& bytes_written,
                         const vpr::Interval timeout)
{
   vpr::Status retval;
   PRInt32 bytes;

   m_blocking_fixed = true;

   bytes = PR_Send(m_handle, buffer, length, 0, NSPR_getInterval(timeout));

   if ( bytes == -1 ) {
      PRErrorCode err_code = PR_GetError();

      bytes_written = 0;

      if ( err_code == PR_WOULD_BLOCK_ERROR ) {
         retval.setCode(vpr::Status::WouldBlock);
      }
      else if ( err_code == PR_IO_TIMEOUT_ERROR ) {
         retval.setCode(vpr::Status::Timeout);
      }
      else {
         retval.setCode(vpr::Status::Failure);
      }
   }
   else {
      bytes_written = bytes;
   }

   return retval;
}

/**
 *
 */
vpr::Status
SocketImplNSPR::getOption (const vpr::SocketOptions::Types option,
                           struct vpr::SocketOptions::Data& data)
{
    vpr::Status retval;
    PRSocketOptionData opt_data;
    bool get_opt;

    // If this is true, PR_GetSocketOption() will be called.
    get_opt = true;

    switch (option) {
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

    if ( get_opt ) {
        PRStatus status;

        status = PR_GetSocketOption(m_handle, &opt_data);

        if ( status == PR_SUCCESS ) {
            // This extracts the information from the union passed to
            // PR_GetSocketOption() and puts it in our friendly
            // vpr::SocketOptions::Data object.
            switch (option) {
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
                switch (opt_data.value.ip_tos) {
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
//                data.mcast_if = vpr::InetAddr(opt_data.value.mcast_if);
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
            }
        }
        else {
            retval.setCode(vpr::Status::Failure);
            NSPR_PrintError("[vpr::SocketImplNSPR] ERROR: Could not get socket option for socket");
        }
    }
    else {
        retval.setCode(vpr::Status::Failure);
    }

    return retval;
}

/**
 *
 */
vpr::Status
SocketImplNSPR::setOption (const vpr::SocketOptions::Types option,
                           const struct vpr::SocketOptions::Data& data)
{
    PRSocketOptionData opt_data;

    switch (option) {
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

        switch (data.type_of_service) {
          case vpr::SocketOptions::LowDelay:
//            opt_data.value.tos = ???;
            break;
          case vpr::SocketOptions::Throughput:
//            opt_data.value.tos = ???;
            break;
          case vpr::SocketOptions::Reliability:
//            opt_data.value.tos = ???;
            break;
          case vpr::SocketOptions::LowCost:
#ifdef IPTOS_LOWCOST
//            opt_data.value.tos = ???;
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
//        opt_data.value.mcast_req.mcaddr = data.mcast_add_member.getMulticastAddr().getAddressValue();
//        opt_data.value.mcast_req.ifaddr = data.mcast_add_member.getInterfaceAddr().getAddressValue();
        break;
      case vpr::SocketOptions::DropMember:
        opt_data.option = PR_SockOpt_DropMember;
//        opt_data.value.mcast_req.mcaddr = data.mcast_drop_member.getMulticastAddr().getAddressValue();
//        opt_data.value.mcast_req.ifaddr = data.mcast_drop_member.getInterfaceAddr().getAddressValue();
        break;
      case vpr::SocketOptions::McastInterface:
        opt_data.option         = PR_SockOpt_McastInterface;
//        opt_data.value.mcast_if = data.mcast_if.getAddressValue();
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

    vprASSERT((m_handle != NULL) && "Trying to set option on NULL handle");
    if(m_handle == NULL) {
        return vpr::Status(vpr::Status::Failure);
    }
    else {
        if ( PR_SetSocketOption(m_handle, &opt_data) == PR_SUCCESS ) {
            return vpr::Status();
        }
        else {
            return vpr::Status(vpr::Status::Failure);
        }
    }
}

// ============================================================================
// Protected methods.
// ============================================================================

// ----------------------------------------------------------------------------
// Default constructor.  This just initializes member variables to reasonable
// defaults.
// ----------------------------------------------------------------------------
SocketImplNSPR::SocketImplNSPR (const vpr::SocketTypes::Type sock_type)
    : m_name("INADDR_ANY"), m_handle(NULL), m_type(sock_type), m_open(false),
      m_open_blocking(true), m_bound(false), m_blocking(true),
      m_blocking_fixed(false)
{
    /* Do nothing. */ ;
}

// ----------------------------------------------------------------------------
// Standard constructor.  This takes the given address (a string containing a
// hostname or an IP address), port, domain and type and stores the values in
// the member variables for use when opening the socket and performing
// communications.
// ----------------------------------------------------------------------------
SocketImplNSPR::SocketImplNSPR (const vpr::InetAddr& local_addr,
                                const vpr::InetAddr& remote_addr,
                                const vpr::SocketTypes::Type sock_type)
    : m_handle(NULL), m_local_addr(local_addr), m_remote_addr(remote_addr),
      m_type(sock_type), m_open(false), m_open_blocking(true), m_bound(false),
      m_blocking(true), m_blocking_fixed(false)
{
    m_name = m_local_addr.getAddressString();
}

// ----------------------------------------------------------------------------
// Destructor.  This currently does nothing.
// ----------------------------------------------------------------------------
SocketImplNSPR::~SocketImplNSPR ()
{
    if ( m_handle != NULL ) {
       // PRClose(m_handle);     // XXX: Let it dangle
       m_handle = NULL;
    }
}

}; // End of vpr namespace
