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

#ifndef _VPR_SOCKET_DATAGRAM_H_
#define _VPR_SOCKET_DATAGRAM_H_

#include <vprConfig.h>

#include <IO/Socket/Socket.h>
#include <IO/Socket/InetAddr.h>


namespace vpr {

// ----------------------------------------------------------------------------
//: Datagram socket interface.
// ----------------------------------------------------------------------------
//! PUBLIC_API:
template<class RealSocketDatagramImp, class RealSocketDatagramImpParent>
class SocketDatagram_t : public Socket_t<RealSocketDatagramImpParent> {
public:
    // ------------------------------------------------------------------------
    //: Default constructor.
    // ------------------------------------------------------------------------
    SocketDatagram_t (void)
        : m_socket_dgram_imp()
    {
        m_socket_imp = &m_socket_dgram_imp;
    }

    // ------------------------------------------------------------------------
    //: Constructor.  This takes a reference to a vpr::InetAddr object giving
    //+ the local socket address and a reference to a vpr::InetAddr object
    //+ giving the remote address.
    //
    //! PRE: addr is a reference to a valid vpr::InetAddr object.
    //! POST: A socket is created using the contents of addr.
    //
    //! ARGS: addr - A reference to a vpr::InetAddr object.
    // ------------------------------------------------------------------------
    SocketDatagram_t (const InetAddr& local_addr, const InetAddr& remote_addr)
        : Socket_t<RealSocketDatagramImpParent>(),
          m_socket_dgram_imp(local_addr, remote_addr)
    {
        m_socket_imp = &m_socket_dgram_imp;
    }

    // ------------------------------------------------------------------------
    //: Destructor.  This currently does nothing.
    //
    //! PRE: None.
    //! POST: None.
    // ------------------------------------------------------------------------
    virtual ~SocketDatagram_t (void) {
        /* Do nothing. */ ;
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline ssize_t
    recvfrom (void* msg, const size_t len, const int flags, InetAddr& from) {
        return m_socket_dgram_imp.recvfrom(msg, len, flags, from);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline ssize_t
    recvfrom (std::vector<char>& msg, const int flags, InetAddr& from) {
        return m_socket_dgram_imp.recvfrom(msg, flags, from);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline ssize_t
    recvfrom (std::vector<char>& msg, const size_t len, const int flags,
              InetAddr& from)
    {
        return m_socket_dgram_imp.recvfrom(msg, len, flags, from);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline ssize_t
    sendto (const void* msg, const size_t len, const int flags,
            const InetAddr& to)
    {
        return m_socket_dgram_imp.sendto(msg, len, flags, to);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline ssize_t
    sendto (const std::vector<char>& msg, const int flags,
            const InetAddr& to)
    {
        return m_socket_dgram_imp.sendto(msg, flags, to);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline ssize_t
    sendto (const std::vector<char>& msg, const size_t len, const int flags,
            const InetAddr& to)
    {
        return m_socket_dgram_imp.sendto(msg, len, flags, to);
    }

    /**
     *
     */
    inline int
    getMcastInterface (InetAddr& mcast_if) {
        SocketOptions::Data option;
        int retval;

        reval = m_socket_imp->getOption(SocketOptions::McastInterface, option);

        if ( retval == 0 ) {
            mcast_if = option.mcast_if;
        }

        return retval;
    }

    /**
     *
     */
    inline int
    setMcastInterface (const InetAddr& mcast_if) {
        SocketOptions::Data option;
        option.mcast_if = mcast_if;
        return m_socket_imp->setOption(SocketOptions::McastInterface, option);
    }

    /**
     *
     */
    inline int
    getMcastTimeToLive (Uint8& ttl) {
        SocketOptions::Data option;
        int retval;

        reval = m_socket_imp->getOption(SocketOptions::McastTimeToLive, option);

        if ( retval == 0 ) {
            ttl = option.mcast_ttl;
        }

        return retval;
    }

    /**
     *
     */
    inline int
    setMcastTimeToLive (const Uint8 ttl) {
        SocketOptions::Data option;
        option.mcast_ttl = ttl;
        return m_socket_imp->setOption(SocketOptions::McastTimeToLive, option);
    }

    /**
     *
     */
    inline int
    getMcastLoopback (Uint8& ttl) {
        SocketOptions::Data option;
        int retval;

        reval = m_socket_imp->getOption(SocketOptions::McastLoopback, option);

        if ( retval == 0 ) {
            loop = option.mcast_loopback;
        }

        return retval;
    }

    /**
     *
     */
    inline int
    setMcastLoopback (const Uint8 loop) {
        SocketOptions::Data option;
        option.mcast_loopback = loop;
        return m_socket_imp->setOption(SocketOptions::McastLoopback, option);
    }

    /**
     *
     */
    inline int
    addMcastMember (const McastReq& request) {
        SocketOptions::Data option;
        option.mcast_add_member = request;
        return m_socket_imp->setOption(SocketOptions::AddMember, option);
    }

    /**
     *
     */
    inline int
    dropMcastMember (const McastReq& request) {
        SocketOptions::Data option;
        option.mcast_drop_member = request;
        return m_socket_imp->setOption(SocketOptions::DropMember, option);
    }

protected:
    RealSocketDatagramImp m_socket_dgram_imp;  //: Platform-specific datagram
                                               //+ socket implementation object
};

}; // End of vpr namespace

#if defined(VPR_USE_NSPR)
#   include <md/NSPR/SocketDatagramImpNSPR.h>

namespace vpr {
  typedef SocketDatagram_t<SocketDatagramImpNSPR, SocketImpNSPR> SocketDatagram;
};
#elif defined(VPR_USE_WIN32)
#   include <md/WIN32/SocketDatagramImpWinSock.h>

namespace vpr {
  typedef SocketDatagram_t<SocketDatagramImpWinSock, SocketImpWinSock> SocketDatagram;
};
#else
#   include <md/POSIX/SocketDatagramImpBSD.h>

namespace vpr {
  typedef SocketDatagram_t<SocketDatagramImpBSD, SocketImpBSD> SocketDatagram;
};
#endif


#endif	/* _VJ_SOCKET_DATAGRAM_H_ */
