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

#ifndef _VJ_SOCKET_STREAM_IMP_H_
#define _VJ_SOCKET_STREAM_IMP_H_

#include <vjConfig.h>

#include <string>

#include <VPR/IO/Socket/vjSocketImp.h>
#include <VPR/IO/Socket/vjSocketTypes.h>


// XXX: Need to add methods for setsockopt(2) stuff.

// ----------------------------------------------------------------------------
//: Stream socket implementation interface.
// ----------------------------------------------------------------------------
class VJ_CLASS_API vjSocketStreamImp : virtual public vjSocketImp {
public:
    // ------------------------------------------------------------------------
    //: Default constructor.  This does nothing.
    //
    //! PRE: None.
    //! POST: None.
    // ------------------------------------------------------------------------
    vjSocketStreamImp (void) {
fprintf(stderr, "vjSocketStreamImp default constructor\n");
        /* Do nothing. */ ;
    }

    // ------------------------------------------------------------------------
    //: Constructor.  This takes the address (either hostname or IP address)
    //+ of a remote site and a port and stores the values for later use in the
    //+ member variables of the object.
    //
    //! PRE: None.
    //! POST: The member variables are initialized with the m_type variable in
    //+       particular set to vjSocketTypes::STREAM.
    //
    //! ARGS: address - The hostname or IP address of the site to which we
    //+                 will connect.
    //! ARGS: port    - The port on the remote site to which we will connect.
    // ------------------------------------------------------------------------
    vjSocketStreamImp (const std::string& address, const unsigned short port,
                       const vjSocketTypes::Domain domain)
        : vjSocketImp(address, port, domain, vjSocketTypes::STREAM)
    {
        m_name = address;
        m_addr.setPort(port);
        m_addr.setFamily(domain);
        m_type = vjSocketTypes::STREAM;
fprintf(stderr, "vjSocketStreamImp constructor\n");
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
    virtual ~vjSocketStreamImp (void) {
        /* Do nothing. */ ;
    }

    // ------------------------------------------------------------------------
    // Listen on the socket for incoming connection requests.
    //
    //! PRE: The socket has been opened and bound to the address in m_addr.
    //! POST: The socket is in a listening state waiting for incoming
    //+       connection requests.
    //
    //! ARGS: backlog - The maximum length of th queue of pending connections.
    //
    //! RETURNS: true  - The socket is in a listening state.
    //! RETURNS: false - The socket could not be put into a listening state.
    //+                  An error message is printed explaining what went
    //+                  wrong.
    // ------------------------------------------------------------------------
    virtual bool listen(const int backlog) = 0;

    // ------------------------------------------------------------------------
    //: Accept an incoming connection request.
    //
    //! PRE: The socket is open and is in a listening state.
    //! POST: When a connection is established, a new Socket object will be
    //+       created that can be used for further communication with the
    //+       remote site.
    //
    //! RETURNS: Non-NULL - A new vjSocketStreamImp object that can be used to
    //+                     communicate with the remote site.
    //! RETURNS: NULL     - A socket could not be created to establish
    //+                     communication with the remote site.  An error
    //+                     message is printed explaining what went wrong.
    //
    //! NOTE: This is a blocking call and will block until a connection is
    //+       established.
    // ------------------------------------------------------------------------
    virtual vjSocketStreamImp* accept(void) = 0;
};


#endif  /* _VJ_SOCKET_STREAM_IMP_H_ */
