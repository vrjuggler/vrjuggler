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

#include <stdio.h>
#include <string.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <errno.h>

#include <vpr/md/POSIX/IO/Socket/SocketImplBSD.h>
#include <vpr/Util/Debug.h>


// ============================================================================
// External global variables.
// ============================================================================
extern int errno;

namespace vpr {

// ============================================================================
// Public methods.
// ============================================================================

// ----------------------------------------------------------------------------
// Open the socket.  This creates a new socket using the domain and type
// options set through member variables.
// ----------------------------------------------------------------------------
Status
SocketImplBSD::open () {
    int domain, type, sock;
    Status retval;

    switch (m_local_addr.getFamily()) {
      case SocketTypes::LOCAL:
#ifdef PF_LOCAL
        domain = PF_LOCAL;
#else
        domain = PF_UNIX;
#endif
        break;
      case SocketTypes::INET:
        domain = PF_INET;
        break;
      case SocketTypes::INET6:
#ifdef PF_INET6
        domain = PF_INET6;
#else
        fprintf(stderr,
                "[vpr::SocketImplBSD] WARNING: IPv6 not supported on this host!\n");
#endif
        break;
      case SocketTypes::LINK:
#if defined(PF_LINK)
        domain = PF_LINK;
#elif defined(PF_RAW)
        domain = PF_RAW;
#else
        fprintf(stderr,
                "[vpr::SocketImplBSD] WARNING: Cannot use LINK sockets. domain %d\n",
                m_local_addr.getFamily());
#endif
        break;
      default:
        fprintf(stderr,
                "[vpr::SocketImplBSD] ERROR: Unknown socket domain value %d\n",
                m_local_addr.getFamily());
        break;
    }

    switch (m_type) {
      case SocketTypes::STREAM:
        type = SOCK_STREAM;
        break;
      case SocketTypes::DATAGRAM:
        type = SOCK_DGRAM;
        break;
      case SocketTypes::RAW:
        type = SOCK_RAW;
        break;
      default:
        fprintf(stderr,
                "[vpr::SocketImplBSD] ERROR: Unknown socket type value %d\n",
                m_local_addr.getFamily());
        break;
    }

    // Attempt to create a new socket using the values in m_local_addr and
    // m_type.
    sock = socket(domain, type, 0);

    // If socket(2) failed, print an error message and return error status.
    if ( sock == -1 ) {
        fprintf(stderr,
                "[vpr::SocketImplBSD] Could not create socket (%s): %s\n",
                m_name.c_str(), strerror(errno));
        retval.setCode(Status::Failure);
    }
    // Otherwise, return success.
    else {
        if ( m_handle == NULL ) {
            m_handle = new FileHandleImplUNIX(m_remote_addr.getAddressString());
        }

        m_handle->m_fdesc = sock;
        m_open = true;

        // Since socket(2) cannot open a socket in non-blocking mode, we call
        // enableNonBlocking() now if the socket is to be opened in
        // non-blocking mode.
        if ( ! m_open_blocking ) {
            retval = enableNonBlocking();
        }
    }

    return retval;
}

// ----------------------------------------------------------------------------
//: Reconfigure the socket so that it is in blocking mode.
// ----------------------------------------------------------------------------
Status
SocketImplBSD::enableBlocking (void) {
    vpr::Status status;
    vprASSERT(m_open && "precondition says you must open() the socket first");

    if ( m_blocking_fixed ) {
        vprDEBUG(0,0)
            << "[SocketImplBSD] Cannot enable blocking after a blocking call!\n"
            << vprDEBUG_FLUSH;
        status.setCode(Status::Failure);
    }
    else {
        status = m_handle->enableBlocking();

        if ( status.success() ) {
            m_blocking = true;
        }
    }

    return status;
}

// ----------------------------------------------------------------------------
//: Reconfigure the socket so that it is in non-blocking mode.
// ----------------------------------------------------------------------------
Status
SocketImplBSD::enableNonBlocking (void) {
    vpr::Status status;
    vprASSERT(m_open && "precondition says you must open() the socket first");

    if ( m_blocking_fixed ) {
        vprDEBUG(0,0)
            << "[SocketImplBSD] Cannot disable blocking after a blocking call!\n"
            << vprDEBUG_FLUSH;
        status.setCode(Status::Failure);
    }
    else {
        status = m_handle->enableNonBlocking();

        if ( status.success() ) {
            m_blocking = false;
        }
    }

    return status;
}

// ----------------------------------------------------------------------------
// Bind this socket to the address in the host address member variable.
// ----------------------------------------------------------------------------
Status
SocketImplBSD::bind () {
    Status retval;
    int status;

    // Bind the socket to the address in m_local_addr.
    status = ::bind(m_handle->m_fdesc, (struct sockaddr*) &m_local_addr.m_addr,
                    m_local_addr.size());

    // If that fails, print an error and return error status.
    if ( status == -1 ) {
        fprintf(stderr,
                "[vpr::SocketImplBSD] Cannot bind socket to address: %s\n",
                strerror(errno));
        retval.setCode(Status::Failure);
    }
    else {
        m_bound = true;
    }

    return retval;
}

// ----------------------------------------------------------------------------
// Connect the socket on the client side to the server side.  For a datagram
// socket, this makes the address given to the constructor the default
// destination for all packets.  For a stream socket, this has the effect of
// establishing a connection with the destination.
// ----------------------------------------------------------------------------
Status
SocketImplBSD::connect (vpr::Interval timeout) {
    Status retval;
    int status;

    if(vpr::Interval::NoTimeout != timeout)
       vprDEBUG(0,vprDBG_WARNING_LVL) << "Timeout not supported\n" << vprDEBUG_FLUSH;

    // Attempt to connect to the address in m_addr.
    status = ::connect(m_handle->m_fdesc,
                       (struct sockaddr*) &m_remote_addr.m_addr,
                       m_remote_addr.size());

    // If connect(2) failed, print an error message explaining why and return
    // error status.
    if ( status == -1 ) {
        // If this is a non-blocking connection, return vpr::Status::InProgress
        // to indicate that the connection will complete later.  I'm not sure
        // if it's safe to set m_connected and m_blocking_fixed at this
        // point, but they have to be set sometime.
        if ( errno == EINPROGRESS && ! m_blocking ) {
            retval.setCode(vpr::Status::InProgress);
            m_connected      = true;
            m_blocking_fixed = true;
        }
        else {
            fprintf(stderr, "[vpr::SocketImplBSD] Error connecting to %s: %s\n",
                    m_remote_addr.getAddressString().c_str(), strerror(errno));
            retval.setCode(vpr::Status::Failure);
        }
    }
    // Otherwise, return success.
    else {
        m_connected      = true;
        m_blocking_fixed = true;
    }

    return retval;
}

Status
SocketImplBSD::setLocalAddr (const InetAddr& addr) {
    Status status;

    if ( m_bound || m_connected ) {
        vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
            << "SocketImplBSD::setLocalAddr: Can't set address of a "
            << "bound or connected socket.\n" << vprDEBUG_FLUSH;
        status.setCode(Status::Failure);
    }
    else {
      m_local_addr = addr;
    }

    return status;
}

Status
SocketImplBSD::setRemoteAddr (const InetAddr& addr) {
    Status status;

    if ( m_connected ) {
        status.setCode(Status::Failure);
    }
    else {
        m_remote_addr = addr;
    }

    return status;
}

// ============================================================================
// Protected methods.
// ============================================================================

// ----------------------------------------------------------------------------
// Destructor.  This currently does nothing.
// ----------------------------------------------------------------------------
SocketImplBSD::~SocketImplBSD () {
    if ( m_handle != NULL ) {
        delete m_handle;
        m_handle = NULL;
    }
}

/**
 * Define a simple union used as the optval argument to [gs]etsockopt(2).
 */
union sockopt_data {
    Int32          enabled;
    size_t         size;
    struct linger  linger_val;
    struct ip_mreq mcast_req;
    struct in_addr mcast_if;
    Uint8          mcast_ttl;
    Uint8          mcast_loop;
};

/**
 *
 */
Status
SocketImplBSD::getOption (const SocketOptions::Types option,
                          struct SocketOptions::Data& data)
{
    int opt_name, opt_level, status;
    Status retval;
    socklen_t opt_size;
    union sockopt_data opt_data;

    switch (option) {
      // Socket-level options.
      case SocketOptions::Linger:
        opt_level = SOL_SOCKET;
        opt_name  = SO_LINGER;
        opt_size  = sizeof(opt_data.linger_val);
        break;
      case SocketOptions::ReuseAddr:
        opt_level = SOL_SOCKET;
        opt_name  = SO_REUSEADDR;
        opt_size  = sizeof(opt_data.enabled);
        break;
      case SocketOptions::KeepAlive:
        opt_level = SOL_SOCKET;
        opt_name  = SO_KEEPALIVE;
        opt_size  = sizeof(opt_data.enabled);
        break;
      case SocketOptions::RecvBufferSize:
        opt_level = SOL_SOCKET;
        opt_name  = SO_RCVBUF;
        opt_size  = sizeof(opt_data.size);
        break;
      case SocketOptions::SendBufferSize:
        opt_level = SOL_SOCKET;
        opt_name  = SO_SNDBUF;
        opt_size  = sizeof(opt_data.size);
        break;

      // IP-level options.
      case SocketOptions::IpTimeToLive:
        opt_level = IPPROTO_IP;
        opt_name  = IP_TTL;
        opt_size  = sizeof(opt_data.size);
        break;
      case SocketOptions::IpTypeOfService:
        opt_level = IPPROTO_IP;
        opt_name  = IP_TOS;
        opt_size  = sizeof(opt_data.size);
        break;
      case SocketOptions::AddMember:
        opt_level = IPPROTO_IP;
        opt_name  = IP_ADD_MEMBERSHIP;
        opt_size  = sizeof(opt_data.mcast_req);
        break;
      case SocketOptions::DropMember:
        opt_level = IPPROTO_IP;
        opt_name  = IP_DROP_MEMBERSHIP;
        opt_size  = sizeof(opt_data.mcast_req);
        break;
      case SocketOptions::McastInterface:
        opt_level = IPPROTO_IP;
        opt_name  = IP_MULTICAST_IF;
        opt_size  = sizeof(opt_data.mcast_if);
        break;
      case SocketOptions::McastTimeToLive:
        opt_level = IPPROTO_IP;
        opt_name  = IP_MULTICAST_TTL;
        opt_size  = sizeof(opt_data.mcast_ttl);
        break;
      case SocketOptions::McastLoopback:
        opt_level = IPPROTO_IP;
        opt_name  = IP_MULTICAST_LOOP;
        opt_size  = sizeof(opt_data.mcast_loop);
        break;

      // TCP-level options.
      case SocketOptions::NoDelay:
        opt_level = IPPROTO_TCP;
        opt_name  = TCP_NODELAY;
        opt_size  = sizeof(opt_data.enabled);
        break;
      case SocketOptions::MaxSegment:
        opt_level = IPPROTO_TCP;
        opt_name  = TCP_MAXSEG;
        opt_size  = sizeof(opt_data.size);
        break;
    }

    status = getsockopt(m_handle->m_fdesc, opt_level, opt_name,
                        (void*) &opt_data, &opt_size);

    if ( status == 0 ) {
        // This extracts the information from the union passed to getsockopt(2)
        // and puts it in our friendly SocketOptions::Data object.  This code
        // depends on the type of that object being a union!
        switch (option) {
          case SocketOptions::Linger:
            data.linger.enabled = (opt_data.linger_val.l_onoff != 0 ? true
                                                                    : false);
            data.linger.seconds = opt_data.linger_val.l_linger;
            break;
          case SocketOptions::ReuseAddr:
            data.reuse_addr = (opt_data.enabled != 0 ? true : false);
            break;
          case SocketOptions::KeepAlive:
            data.keep_alive = (opt_data.enabled != 0 ? true : false);
            break;
          case SocketOptions::RecvBufferSize:
            data.recv_buffer_size = opt_data.size;
            break;
          case SocketOptions::SendBufferSize:
            data.send_buffer_size = opt_data.size;
            break;
          case SocketOptions::IpTimeToLive:
            data.ip_ttl = opt_data.size;
            break;
          case SocketOptions::IpTypeOfService:
            switch (opt_data.size) {
              case IPTOS_LOWDELAY:
                data.type_of_service = SocketOptions::LowDelay;
                break;
              case IPTOS_THROUGHPUT:
                data.type_of_service = SocketOptions::Throughput;
                break;
              case IPTOS_RELIABILITY:
                data.type_of_service = SocketOptions::Reliability;
                break;
#ifdef IPTOS_LOWCOST
              case IPTOS_LOWCOST:
                data.type_of_service = SocketOptions::LowCost;
                break;
#endif
            }

            break;
          case SocketOptions::McastInterface:
            data.mcast_if = InetAddr();
            data.mcast_if.setAddress(opt_data.mcast_if.s_addr, 0);
            break;
          case SocketOptions::McastTimeToLive:
            data.mcast_ttl = opt_data.mcast_ttl;
            break;
          case SocketOptions::McastLoopback:
            data.mcast_loopback = opt_data.mcast_loop;
            break;
          case SocketOptions::NoDelay:
            data.no_delay = (opt_data.enabled != 0 ? true : false);
            break;
          case SocketOptions::MaxSegment:
            data.max_segment = opt_data.size;
            break;
        }
    }
    else {
        retval.setCode(Status::Failure);
        fprintf(stderr,
                "[vpr::SocketImplBSD] ERROR: Could not get socket option for socket %s: %s\n",
                m_handle->getName().c_str(), strerror(errno));
    }

    return retval;
}

/**
 *
 */
Status
SocketImplBSD::setOption (const SocketOptions::Types option,
                          const struct SocketOptions::Data& data)
{
    int opt_name, opt_level;
    socklen_t opt_size;
    union sockopt_data opt_data;
    Status retval;

    switch (option) {
      // Socket-level options.
      case SocketOptions::Linger:
        opt_level                    = SOL_SOCKET;
        opt_name                     = SO_LINGER;
        opt_data.linger_val.l_onoff  = (data.linger.enabled ? 1 : 0);
        opt_data.linger_val.l_linger = data.linger.seconds;
        opt_size                     = sizeof(struct linger);
        break;
      case SocketOptions::ReuseAddr:
        opt_level        = SOL_SOCKET;
        opt_name         = SO_REUSEADDR;
        opt_data.enabled = (data.reuse_addr ? 1 : 0);
        opt_size         = sizeof(int);
        break;
      case SocketOptions::KeepAlive:
        opt_level        = SOL_SOCKET;
        opt_name         = SO_KEEPALIVE;
        opt_data.enabled = (data.keep_alive ? 1 : 0);
        opt_size         = sizeof(int);
        break;
      case SocketOptions::RecvBufferSize:
        opt_name      = SO_RCVBUF;
        opt_level     = SOL_SOCKET;
        opt_data.size = data.recv_buffer_size;
        opt_size      = sizeof(size_t);
        break;
      case SocketOptions::SendBufferSize:
        opt_level     = SOL_SOCKET;
        opt_name      = SO_SNDBUF;
        opt_data.size = data.send_buffer_size;
        opt_size      = sizeof(size_t);
        break;

      // IP-level options.
      case SocketOptions::IpTimeToLive:
        opt_level     = IPPROTO_IP;
        opt_name      = IP_TTL;
        opt_data.size = data.ip_ttl;
        opt_size      = sizeof(size_t);
        break;
      case SocketOptions::IpTypeOfService:
        opt_level     = IPPROTO_IP;
        opt_name      = IP_TOS;

        switch (data.type_of_service) {
          case SocketOptions::LowDelay:
            opt_data.size = IPTOS_LOWDELAY;
            break;
          case SocketOptions::Throughput:
            opt_data.size = IPTOS_THROUGHPUT;
            break;
          case SocketOptions::Reliability:
            opt_data.size = IPTOS_RELIABILITY;
            break;
          case SocketOptions::LowCost:
#ifdef IPTOS_LOWCOST
            opt_data.size = IPTOS_LOWCOST;
#else
            fprintf(stderr,
                    "[vpr::SocketImplBSD] WARNING: This platform does not "
                    "support LowCost type of service!\n");
#endif
            break;
        }

        opt_size      = sizeof(size_t);
        break;
      case SocketOptions::AddMember:
        opt_level = IPPROTO_IP;
        opt_name  = IP_ADD_MEMBERSHIP;
        opt_data.mcast_req.imr_multiaddr.s_addr = data.mcast_add_member.getMulticastAddr().getAddressValue();
        opt_data.mcast_req.imr_interface.s_addr = data.mcast_add_member.getInterfaceAddr().getAddressValue();
        opt_size  = sizeof(struct ip_mreq);
        break;
      case SocketOptions::DropMember:
        opt_level          = IPPROTO_IP;
        opt_name           = IP_DROP_MEMBERSHIP;
        opt_data.mcast_req.imr_multiaddr.s_addr = data.mcast_drop_member.getMulticastAddr().getAddressValue();
        opt_data.mcast_req.imr_interface.s_addr = data.mcast_drop_member.getInterfaceAddr().getAddressValue();
        opt_size           = sizeof(struct ip_mreq);
        break;
      case SocketOptions::McastInterface:
        opt_level                = IPPROTO_IP;
        opt_name                 = IP_MULTICAST_IF;
        opt_data.mcast_if.s_addr = data.mcast_if.getAddressValue();
        opt_size                 = sizeof(struct in_addr);
        break;
      case SocketOptions::McastTimeToLive:
        opt_level          = IPPROTO_IP;
        opt_name           = IP_MULTICAST_TTL;
        opt_data.mcast_ttl = data.mcast_ttl;
        opt_size           = sizeof(Uint8);
        break;
      case SocketOptions::McastLoopback:
        opt_level           = IPPROTO_IP;
        opt_name            = IP_MULTICAST_LOOP;
        opt_data.mcast_loop = data.mcast_loopback;
        opt_size            = sizeof(Uint8);
        break;

      // TCP-level options.
      case SocketOptions::NoDelay:
        opt_level        = IPPROTO_TCP;
        opt_name         = TCP_NODELAY;
        opt_data.enabled = (data.no_delay ? 1 : 0);
        opt_size         = sizeof(int);
        break;
      case SocketOptions::MaxSegment:
        opt_level     = IPPROTO_TCP;
        opt_name      = TCP_MAXSEG;
        opt_data.size = data.max_segment;
        opt_size      = sizeof(size_t);
        break;
    }

    if ( ::setsockopt(m_handle->m_fdesc, opt_level, opt_name, &opt_data, opt_size) != 0 )
    {
        retval.setCode(Status::Failure);
    }

    return retval;
}

}; // End of vpr namespace
