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

#ifndef _VJ_SOCKET_DATAGRAM_IMP_H_
#define _VJ_SOCKET_DATAGRAM_IMP_H_

#include <vjConfig.h>

#include <string>

#include <VPR/IO/Socket/vjSocketImp.h>
#include <VPR/IO/Socket/vjInetAddr.h>


// ----------------------------------------------------------------------------
//: Datagram socket implementation interface.
// ----------------------------------------------------------------------------
class VJ_CLASS_API vjSocketDatagramImp : virtual public vjSocketImp {
public:
    // ------------------------------------------------------------------------
    //: Default constructor.  This does nothing.
    //
    //! PRE: None.
    //! POST: None.
    // ------------------------------------------------------------------------
    vjSocketDatagramImp (void) {
fprintf(stderr, "vjSocketDatagramImp default constructor\n");
        /* Do nothing. */ ;
    }

    // ------------------------------------------------------------------------
    //: Constructor.  This takes the address (either hostname or IP address)
    //+ of a remote site and a port and stores the values for later use in the
    //+ member variables of the object.
    //
    //! PRE: None.
    //! POST: The member variables are initialized with the m_type variable in
    //+       particular set to vjSocketTypes::DATAGRAM.
    //
    //! ARGS: address - The hostname or IP address of the site to which we
    //+                 will connect.
    //! ARGS: port    - The port on the remote site to which we will connect.
    //! ARGS: domain  - The protocol family (domain) for this socket.
    // ------------------------------------------------------------------------
    vjSocketDatagramImp (const std::string& address, const unsigned short port,
                         const vjSocketTypes::Domain domain)
        : vjSocketImp(address, port, domain, vjSocketTypes::DATAGRAM)
    {
        m_name = address;
        m_addr.setPort(port);
        m_addr.setFamily(domain);
        m_type = vjSocketTypes::DATAGRAM;
fprintf(stderr, "vjSocketDatagramImp constructor\n");
fprintf(stderr, "    Address: %s\n", address.c_str());
fprintf(stderr, "    Port: %hu\n", port);
fprintf(stderr, "    Domain: %d\n", domain);
    }

    // ------------------------------------------------------------------------
    //: Destructor.  This currently does nothing.
    //
    //! PRE: None.
    //! POST: None.
    // ------------------------------------------------------------------------
    virtual ~vjSocketDatagramImp (void) {
        /* Do nothing. */ ;
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual ssize_t recvfrom(void* msg, const size_t len, const int flags,
                             vjInetAddr& from) = 0;

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual ssize_t recvfrom(unsigned char* msg, const size_t len,
                             const int flags, vjInetAddr& from) = 0;

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual ssize_t recvfrom(char* msg, const size_t len, const int flags,
                             vjInetAddr& from) = 0;

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual ssize_t recvfrom(std::vector<char>& msg, const int flags,
                             vjInetAddr& from) = 0;

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual ssize_t recvfrom(std::vector<char>& msg, const size_t len,
                             const int flags, vjInetAddr& from) = 0;

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual ssize_t sendto(const void* msg, const size_t len, const int flags,
                           const vjInetAddr& to) = 0;

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual ssize_t sendto(const unsigned char* msg, const size_t len,
                           const int flags, const vjInetAddr& to) = 0;

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual ssize_t sendto(const char* msg, const size_t len, const int flags,
                           const vjInetAddr& to) = 0;

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual ssize_t sendto(const std::vector<char>& msg, const int flags,
                           const vjInetAddr& to) = 0;

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual ssize_t sendto(const std::vector<char>& msg, const size_t len,
                           const int flags, const vjInetAddr& to) = 0;
};


#endif  /* _VJ_SOCKET_DATAGRAM_IMP_H_ */
