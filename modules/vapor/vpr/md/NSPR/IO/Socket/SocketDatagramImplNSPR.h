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

#ifndef _VPR_SOCKET_DATAGRAM_IMP_NSPR_H_
#define _VPR_SOCKET_DATAGRAM_IMP_NSPR_H_

#include <string>

#include <md/NSPR/SocketImpNSPR.h>
#include <IO/Socket/SocketDatagramOpt.h>


namespace vpr {

class SocketDatagramImpNSPR : public SocketDatagramOpt, public SocketImpNSPR {
public:
    typedef SocketImpNSPR Parent;

    // ========================================================================
    // vpr::SocketDatagram implementation.
    // ========================================================================

    // ------------------------------------------------------------------------
    //: Default constructor.  This does nothing.
    //
    //! PRE: None.
    //! POST: None.
    // ------------------------------------------------------------------------
    SocketDatagramImpNSPR (void)
        : SocketImpNSPR(SocketTypes::DATAGRAM)
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
    SocketDatagramImpNSPR (const InetAddr& local_addr,
                           const InetAddr& remote_addr)
        : SocketImpNSPR(local_addr, remote_addr, SocketTypes::DATAGRAM)
    {
        /* Do nothing. */ ;
    }

    // ------------------------------------------------------------------------
    // Copy constructor.
    // ------------------------------------------------------------------------
    SocketDatagramImpNSPR (const SocketDatagramImpNSPR& sock)
        : SocketImpNSPR(SocketTypes::DATAGRAM)
    {
        m_local_addr      = sock.m_local_addr;
        m_remote_addr     = sock.m_remote_addr;
    }

    // ------------------------------------------------------------------------
    //: Destructor.  This currently does nothing.
    //
    //! PRE: None.
    //! POST: None.
    // ------------------------------------------------------------------------
    virtual ~SocketDatagramImpNSPR (void) {
        /* Do nothing. */ ;
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual Status recvfrom(void* msg, const size_t length, const int flags,
                            InetAddr& from, ssize_t& bytes_read);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual Status recvfrom(std::string& msg, const size_t length,
                            const int flags, InetAddr& from,
                            ssize_t& bytes_read);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual Status recvfrom(std::vector<vpr::Uint8>& msg, const size_t length,
                            const int flags, InetAddr& from,
                            ssize_t& bytes_read);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual Status sendto(const void* msg, const size_t length,
                          const int flags, const InetAddr& to,
                          ssize_t& bytes_sent);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual Status sendto(const std::string& msg, const size_t length,
                          const int flags, const InetAddr& to,
                          ssize_t& bytes_sent);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual Status sendto(const std::vector<vpr::Uint8>& msg,
                          const size_t length, const int flags,
                          const InetAddr& to, ssize_t& bytes_sent);

protected:
    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual Status
    getOption (const SocketOptions::Types option,
               struct SocketOptions::Data& data)
    {
        return SocketImpNSPR::getOption(option, data);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual Status
    setOption (const SocketOptions::Types option,
               const struct SocketOptions::Data& data)
    {
        return SocketImpNSPR::setOption(option, data);
    }
};

}; // End of namespace


#endif	/* _VPR_SOCKET_DATAGRAM_IMP_NSPR_H_ */
