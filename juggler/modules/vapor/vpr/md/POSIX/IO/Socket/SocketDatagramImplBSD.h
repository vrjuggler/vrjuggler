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

#ifndef _VPR_SOCKET_DATAGRAM_IMP_BSD_H_
#define _VPR_SOCKET_DATAGRAM_IMP_BSD_H_

#include <string>

#include <md/POSIX/SocketImpBSD.h>
#include <IO/Socket/SocketDatagramOpt.h>


namespace vpr {

class SocketDatagramImpBSD : public SocketDatagramOpt, public SocketImpBSD {
public:
    typedef SocketImpBSD Parent;

    // ========================================================================
    // vpr::SocketDatagram implementation.
    // ========================================================================

    // ------------------------------------------------------------------------
    //: Default constructor.  This does nothing.
    //
    //! PRE: None.
    //! POST: None.
    // ------------------------------------------------------------------------
    SocketDatagramImpBSD (void)
        : SocketImpBSD()
    {
        /* Do nothing. */ ;
    }

    // ------------------------------------------------------------------------
    //
    //! PRE: None.
    //! POST: The member variables are initialized with the type in particular
    //+       set to vpr::SocketTypes::DATAGRAM.
    //
    // ------------------------------------------------------------------------
    SocketDatagramImpBSD (const InetAddr& local_addr,
                          const InetAddr& remote_addr)
        : SocketImpBSD(local_addr, remote_addr, SocketTypes::DATAGRAM)
    {
        /* Do nothing. */ ;
    }

    // ------------------------------------------------------------------------
    // Copy constructor.
    // ------------------------------------------------------------------------
    SocketDatagramImpBSD (const SocketDatagramImpBSD& sock) {
        m_local_addr      = sock.m_local_addr;
        m_remote_addr     = sock.m_remote_addr;
        m_handle          = new FileHandleUNIX(sock.m_handle->getName());
        m_handle->m_fdesc = sock.m_handle->m_fdesc;
    }

    // ------------------------------------------------------------------------
    //: Destructor.  This currently does nothing.
    //
    //! PRE: None.
    //! POST: None.
    // ------------------------------------------------------------------------
    virtual ~SocketDatagramImpBSD (void) {
        /* Do nothing. */ ;
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual ssize_t recvfrom(void* msg, const size_t len, const int flags,
                             InetAddr& from);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual ssize_t recvfrom(std::vector<char>& msg, const int flags,
                             InetAddr& from);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual ssize_t recvfrom(std::vector<char>& msg, const size_t len,
                             const int flags, InetAddr& from);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual ssize_t sendto(const void* msg, const size_t len, const int flags,
                           const InetAddr& to);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual ssize_t sendto(const std::vector<char>& msg, const int flags,
                           const InetAddr& to);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual ssize_t sendto(const std::vector<char>& msg, const size_t len,
                           const int flags, const InetAddr& to);

protected:
    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual bool
    getOption (const SocketOptions::Types option,
               struct SocketOptions::Data& data)
    {
        return SocketImpBSD::getOption(option, data);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual bool
    setOption (const SocketOptions::Types option,
               const struct SocketOptions::Data& data)
    {
        return SocketImpBSD::setOption(option, data);
    }
};

}; // End of namespace


#endif	/* _VPR_SOCKET_DATAGRAM_IMP_BSD_H_ */
