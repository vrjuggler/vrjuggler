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

#ifndef _VPR_SOCKET_DATAGRAM_BRIDGE_H_
#define _VPR_SOCKET_DATAGRAM_BRIDGE_H_
// NOTE: this is the bridge class for use with SocketDatagram.h

#include <vprConfig.h>

#include <IO/Socket/Socket_t.h>


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
    inline bool
    getMcastInterface (InetAddr& mcast_if) {
        return m_socket_imp->getMcastInterface(mcast_if);
    }

    /**
     *
     */
    inline bool
    setMcastInterface (const InetAddr& mcast_if) {
        return m_socket_imp->setMcastInterface(mcast_if);
    }

    /**
     *
     */
    inline bool
    getMcastTimeToLive (Uint8& ttl) {
        return m_socket_imp->getMcastTimeToLive(ttl);
    }

    /**
     *
     */
    inline bool
    setMcastTimeToLive (const Uint8 ttl) {
        return m_socket_imp->setMcastTimeToLive(ttl);
    }

    /**
     *
     */
    inline bool
    getMcastLoopback (Uint8& loop) {
        return m_socket_imp->getMcastLoopback(loop);
    }

    /**
     *
     */
    inline bool
    setMcastLoopback (const Uint8 loop) {
        return m_socket_imp->setMcastLoopback(loop);
    }

    /**
     *
     */
    inline bool
    addMcastMember (const McastReq& request) {
        return m_socket_imp->addMcastMember(request);
    }

    /**
     *
     */
    inline bool
    dropMcastMember (const McastReq& request) {
        return m_socket_imp->dropMcastMember(request);
    }

protected:
    RealSocketDatagramImp m_socket_dgram_imp;  //: Platform-specific datagram
                                               //+ socket implementation object
};

}; // End of vpr namespace


#endif	/* _VJ_SOCKET_DATAGRAM_BRIDGE_H_ */
