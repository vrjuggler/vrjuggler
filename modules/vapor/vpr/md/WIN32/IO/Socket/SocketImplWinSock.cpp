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
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include <vpr/md/WIN32/IO/Socket/SocketImplWinSock.h>


namespace vpr {

// ============================================================================
// Public methods.
// ============================================================================

// ----------------------------------------------------------------------------
// Open the socket.  This creates a new socket using the domain and type
// options set through member variables.
// ----------------------------------------------------------------------------
Status
SocketImplWinSock::open () {
    Status retval;
    int domain, type;

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
        domain = PF_INET6;
        break;
/*
      case SocketTypes::LINK:
        domain = PF_LINK;
        break;
*/
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
    }

    m_sockfd = socket(domain, type, 0);

    if ( m_sockfd == INVALID_SOCKET ) {
        fprintf(stderr,
                "[vpr::SocketImplWinSock] Could not create socket (%s): %s\n",
                m_name.c_str(), strerror(errno));
        retval.setCode(Status::Failure);
    }

    return retval;
}

// ----------------------------------------------------------------------------
// Close the socket.
// ----------------------------------------------------------------------------
Status
SocketImplWinSock::close () {
    Status retval;

    if ( ::closesocket(m_sockfd) == 0 ) {
        m_open = false;
    }
    else {
        fprintf(stderr,
                "[vpr::SocketImplWinSock] Could not close socket (%s): %s\n",
                m_name.c_str(), strerror(errno));
        retval.setCode(Status::Failure);
    }

    return retval;
}

// ----------------------------------------------------------------------------
// Bind this socket to the address in the host address member variable.
// ----------------------------------------------------------------------------
Status
SocketImplWinSock::bind () {
    Status retval;
    int status;

    // Bind the socket to the address in m_addr.
    status = ::bind(m_sockfd, (struct sockaddr*) &m_local_addr.m_addr,
                    m_local_addr.size());

    // If that fails, print an error and return error status.
    if ( status == -1 ) {
        fprintf(stderr,
                "[vpr::SocketImplWinSock] Cannot bind socket to address: %s\n",
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
SocketImplWinSock::connect () {
    Status retval;
    int status;

    // Attempt to connect to the address in m_addr.
    status = ::connect(m_sockfd, (struct sockaddr*) &m_remote_addr.m_addr,
                       m_remote_addr.size());

    // If connect(2) failed, print an error message explaining why and return
    // error status.
    if ( status == -1 ) {
        fprintf(stderr, "[vpr::SocketImplWinSock] Error connecting to %s: %s\n",
                m_name.c_str(), strerror(errno));
        retval.setCode(Status::Failure);
    }
    else {
        m_connected = true;
    }

    return retval;
}

// ============================================================================
// Protected methods.
// ============================================================================

// ----------------------------------------------------------------------------
// Default constructor.  This just initializes member variables to reasonable
// defaults.
// ----------------------------------------------------------------------------
SocketImplWinSock::SocketImplWinSock ()
    : BlockIO(std::string("INADDR_ANY")), m_sockfd(-1), m_bound(false),
      m_connected(false)
{
    init();
}

// ----------------------------------------------------------------------------
// Standard constructor.  This takes the given address (a string containing
// a hostname or an IP address), port, domain and type and stores the values
// in the member variables for use when opening the socket and performing
// communications.
// ----------------------------------------------------------------------------
SocketImplWinSock::SocketImplWinSock (const InetAddr& local_addr,
                                      const InetAddr& remote_addr,
                                      const SocketTypes::Type type)
    : BlockIO(std::string("INADDR_ANY")), m_sockfd(-1), m_bound(false),
      m_connected(false), m_local_addr(local_addr),
      m_remote_addr(remote_addr), m_type(type)
{
    init();
}

// ----------------------------------------------------------------------------
// Destructor.  This currently does nothing.
// ----------------------------------------------------------------------------
SocketImplWinSock::~SocketImplWinSock () {
    init();
}

// ----------------------------------------------------------------------------
// Do the WinSock initialization required before any socket stuff can happen.
// This is copied from a similar function given on page 279 of _Effective
// TCP/IP Programming_ by Jon C. Snader.
// ----------------------------------------------------------------------------
void
SocketImplWinSock::init () {
    WSADATA wsadata;
    WSAStartup(MAKEWORD(2, 2), &wsadata);
}

// ----------------------------------------------------------------------------
// Receive the specified number of bytes from the remote site to which the
// local side is connected.
// ----------------------------------------------------------------------------
Status
SocketImplWinSock::recv (void* buffer, const size_t length, const int flags,
                         ssize_t& bytes_read)
{
    Status retval;

    bytes_read = ::recv(m_sockfd, (char*) buffer, length, flags);

    if ( bytes_read < 0 ) {
        retval.setCode(Status::Failure);
    }

    return retval;
}

// ----------------------------------------------------------------------------
// Read exactly the specified number of bytes from the file handle into the
// given buffer.  This is baesd on the readn() function given on pages 51-2 of
// _Effective TCP/IP Programming_ by Jon D. Snader.
// ----------------------------------------------------------------------------
Status
SocketImplWinSock::recvn (void* buffer, const size_t length, const int flags,
                          ssize_t& bytes_read)
{
    size_t count;
    Status retval;

    count = length;

    while ( count > 0 ) {
        bytes_read = ::recv(m_sockfd, (char*) buffer, length, flags);

        // Read error.
        if ( bytes_read < 0 ) {
            retval.setCode(Status::Failure);
            break;
        }
        // May have read EOF, so return bytes read so far.
        else if ( bytes_read == 0 ) {
            bytes_read = length - count;
        }
        else {
            buffer = (void*) ((char*) buffer + bytes_read);
            count  -= bytes_read;
        }
    }

    return retval;
}

// ----------------------------------------------------------------------------
// Send the specified number of bytes contained in the given buffer from the
// local side to the remote site to which we are connected.
// ----------------------------------------------------------------------------
Status
SocketImplWinSock::send (const void* buffer, const size_t length,
                         const int flags, ssize_t& bytes_sent)
{
    Status retval;

    bytes_sent = ::send(m_sockfd, (char*) buffer, length, flags);

    if ( bytes_sent < 0 ) {
        retval.setCode(Status::Failure);
    }

    return retval;
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

Status
SocketImplWinSock::getOption (const SocketOptions::Types option,
                              struct SocketOptions::Data& data)
{
    int opt_name, opt_level;
    Status retval;
    socklen_t opt_size;
    union sockopt_data opt_data;
    bool do_get;

    do_get = true;

    switch (option) {
      // Socket-level options.
      case SocketOptions::Linger:
        opt_level = SOL_SOCKET;
        opt_name  = SO_LINGER;
        break;
      case SocketOptions::ReuseAddr:
        opt_level = SOL_SOCKET;
        opt_name  = SO_REUSEADDR;
        break;
      case SocketOptions::KeepAlive:
        opt_level = SOL_SOCKET;
        opt_name  = SO_KEEPALIVE;
        break;
      case SocketOptions::RecvBufferSize:
        opt_level = SOL_SOCKET;
        opt_name  = SO_RCVBUF;
        break;
      case SocketOptions::SendBufferSize:
        opt_level = SOL_SOCKET;
        opt_name  = SO_SNDBUF;
        break;

      // IP-level options.
      case SocketOptions::IpTimeToLive:
        opt_level = IPPROTO_IP;
        opt_name  = IP_TTL;
        break;
      case SocketOptions::IpTypeOfService:
        opt_level = IPPROTO_IP;
        opt_name  = IP_TOS;
        break;
      case SocketOptions::AddMember:
        opt_level = IPPROTO_IP;
        opt_name  = IP_ADD_MEMBERSHIP;
        break;
      case SocketOptions::DropMember:
        opt_level = IPPROTO_IP;
        opt_name  = IP_DROP_MEMBERSHIP;
        break;
      case SocketOptions::McastInterface:
        opt_level = IPPROTO_IP;
        opt_name  = IP_MULTICAST_IF;
        break;
      case SocketOptions::McastTimeToLive:
        opt_level = IPPROTO_IP;
        opt_name  = IP_MULTICAST_TTL;
        break;
      case SocketOptions::McastLoopback:
        opt_level = IPPROTO_IP;
        opt_name  = IP_MULTICAST_LOOP;
        break;

      // TCP-level options.
      case SocketOptions::NoDelay:
        opt_level = IPPROTO_TCP;
        opt_name  = TCP_NODELAY;
        break;
      case SocketOptions::MaxSegment:
        fprintf(stderr,
                "[vpr::SocketImplWinSock] WARNING: This platform does not "
                "support the TCP max segment option!\n");
        do_get = false;
        break;
    }

    if ( do_get ) {
        int status;

        status = getsockopt(m_sockfd, opt_level, opt_name, (char*) &opt_data,
                            &opt_size);

        if ( status == 0 ) {
            retval.setCode(Status::Failure);

            // This extracts the information from the union passed to
            // getsockopt(2) and puts it in our friendly SocketOptions::Data
            // object.
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
/*
                switch (opt_data.size) {
#ifdef IPTOS_LOWDELAY
                  case IPTOS_LOWDELAY:
                    data.type_of_service = SocketOptions::LowDelay;
                    break;
#endif
#ifdef IPTOS_THROUGHPUT
                  case IPTOS_THROUGHPUT:
                    data.type_of_service = SocketOptions::Throughput;
                    break;
#endif
#ifdef IPTOS_RELIABILITY
                  case IPTOS_RELIABILITY:
                    data.type_of_service = SocketOptions::Reliability;
                    break;
#endif
#ifdef IPTOS_LOWCOST
                  case IPTOS_LOWCOST:
                    data.type_of_service = SocketOptions::LowCost;
                    break;
#endif
                }
*/

                break;
              case SocketOptions::McastInterface:
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
                    "[vpr::SocketImplWinSock] ERROR: Could not get socket "
                    "option for socket %s: %s\n", getName().c_str(),
                    strerror(errno));
        }
    }
    else {
        retval.setCode(Status::Failure);
    }

    return retval;
}

Status
SocketImplWinSock::setOption (const SocketOptions::Types option,
                              const struct SocketOptions::Data& data)
{
    int opt_name, opt_level;
    socklen_t opt_size;
    union sockopt_data opt_data;
    bool do_set;
    Status retval;

    do_set = true;

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
#ifdef IPTOS_LOWDELAY
            opt_data.size = IPTOS_LOWDELAY;
#else
            fprintf(stderr,
                    "[vpr::SocketImplWinSock] WARNING: This platform does not "
                    "support LowDelay type of service!\n");
#endif
            break;
          case SocketOptions::Throughput:
#ifdef IPTOS_THROUGHPUT
            opt_data.size = IPTOS_THROUGHPUT;
#else
            fprintf(stderr,
                    "[vpr::SocketImplWinSock] WARNING: This platform does not "
                    "support LowDelay type of service!\n");
#endif
            break;
          case SocketOptions::Reliability:
#ifdef IPTOS_RELIABILITY
            opt_data.size = IPTOS_RELIABILITY;
#else
            fprintf(stderr,
                    "[vpr::SocketImplWinSock] WARNING: This platform does not "
                    "support LowDelay type of service!\n");
#endif
            break;
          case SocketOptions::LowCost:
#ifdef IPTOS_LOWCOST
            opt_data.size = IPTOS_LOWCOST;
#else
            fprintf(stderr,
                    "[vpr::SocketImplWinSock] WARNING: This platform does not "
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
        fprintf(stderr,
                "[vpr::SocketImplWinSock] WARNING: This platform does not "
                "support the TCP max segment option!\n");
        do_set = false;
        break;
    }

    if ( do_set ) {
        int set_status;

        set_status = setsockopt(m_sockfd, opt_level, opt_name,
                                (char*) &opt_data, opt_size);

        if ( set_status < 0 ) {
            retval.setCode(Status::Failure);
        }
    }
    else {
        retval.setCode(Status::Failure);
    }

    return retval;
}

}; // End of vpr namespace
