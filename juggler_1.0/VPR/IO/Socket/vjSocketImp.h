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

#ifndef _VJ_SOCKET_IMP_H_
#define _VJ_SOCKET_IMP_H_

#include <vjConfig.h>

#include <stdio.h>	/* XXX: Debugging only */
#include <string>

#include <VPR/IO/vjBlockIO.h>
#include <VPR/IO/Socket/vjSocketTypes.h>
#include <VPR/IO/Socket/vjInetAddr.h>


// ----------------------------------------------------------------------------
//: Socket interface.
// ----------------------------------------------------------------------------
class VJ_CLASS_API vjSocketImp : public vjBlockIO {
public:
    // ------------------------------------------------------------------------
    //: Destructor.  This currently does nothing.
    //
    //! PRE: None.
    //! POST: None.
    // ------------------------------------------------------------------------
    virtual ~vjSocketImp (void) {
        /* Do nothing. */ ;
    }

    // ------------------------------------------------------------------------
    //: Bind this socket to the address in the host address member variable.
    //
    //! PRE: The socket is open, and m_addr has been initialized properly.
    //! POST: The socket is bound to the address in m_addr.
    //
    //! RETURNS: true  - The socket was bound to the address successfully.
    //! RETURNS: false - The socket could not be bound to the address in
    //+                  m_addr.  An error message is printed explaining what
    //+                  went wrong.
    // ------------------------------------------------------------------------
    virtual bool bind(void) = 0;

    // ------------------------------------------------------------------------
    //: Connect the socket on the client side to the server side.  For a
    //+ datagram socket, this makes the address given to the constructor the
    //+ default destination for all packets.  For a stream socket, this has
    //+ the effect of establishing a connection with the destination.
    //
    //! PRE: The socket is open.
    //! POST: The socket is connected to the address in m_addr.  For a stream
    //+       socket, this means that a connection for future communication
    //+       has been established.  For a datagram socket, the default
    //+       destination for all packets is now m_addr.
    //
    //! RETURNS: true  - The connection was made.
    //! RETURNS: false - The connect could not be made.  An error message is
    //+                  printed explaining what happened.
    // ------------------------------------------------------------------------
    virtual bool connect(void) = 0;

    // ------------------------------------------------------------------------
    //: Receive the specified number of bytes from the remote site to which
    //+ the local side is connected.
    //
    //! PRE: The socket is open and connect() has been called.  buffer is at
    //+      least length bytes in size.
    //! POST: length bytes are read from the remote site and stored in the
    //+       given buffer.  The number of bytes read is returned to the
    //+       caller.
    //
    //! ARGS: buffer - A pointer to the buffer used for storing the bytes
    //+                received from the remote site.
    //! ARGS: length - The number of bytes to receive from the remote site.
    //! ARGS: flags  - Flags to use when receiving the data.  This is optional
    //                 and defaults to 0.
    //
    //! RETURNS: >-1 - The number of bytes received.
    //! RETURNS:  -1 - Something went wrong when trying to receive the data.
    // ------------------------------------------------------------------------
    virtual ssize_t recv(void* buffer, const size_t length,
                         const int flags = 0) = 0;

    // ------------------------------------------------------------------------
    //: Receive the specified number of bytes from the remote site to which
    //+ the local side is connected.
    //
    //! PRE: The socket is open and connect() has been called.  buffer is at
    //+      least length bytes in size.
    //! POST: length bytes are read from the remote site and stored in the
    //+       given buffer.  The number of bytes read is returned to the
    //+       caller.
    //
    //! ARGS: buffer - A pointer to the buffer (an array of unsigned chars)
    //+                used for storing the bytes received from the remote
    //+                site.
    //! ARGS: length - The number of bytes to receive from the remote site.
    //! ARGS: flags  - Flags to use when receiving the data.  This is optional
    //+                and defaults to 0.
    //
    //! RETURNS: >-1 - The number of bytes received.
    //! RETURNS:  -1 - Something went wrong when trying to receive the data.
    // ------------------------------------------------------------------------
    virtual ssize_t recv(unsigned char* buffer, const size_t length,
                         const int flags = 0) = 0;

    // ------------------------------------------------------------------------
    //: Receive the specified number of bytes from the remote site to which
    //+ the local side is connected.
    //
    //! PRE: The socket is open and connect() has been called.  buffer is at
    //+      least length bytes in size.
    //! POST: length bytes are read from the remote site and stored in the
    //+       given buffer.  The number of bytes read is returned to the
    //+       caller.
    //
    //! ARGS: buffer - A pointer to the buffer (an array of chars) used for
    //+                storing the bytes received from the remote site.
    //! ARGS: length - The number of bytes to receive from the remote site.
    //! ARGS: flags  - Flags to use when receiving the data.  This is optional
    //+                and defaults to 0.
    //
    //! RETURNS: >-1 - The number of bytes received.
    //! RETURNS:  -1 - Something went wrong when trying to receive the data.
    // ------------------------------------------------------------------------
    virtual ssize_t recv(char* buffer, const size_t length,
                         const int flags = 0) = 0;

    // ------------------------------------------------------------------------
    //: Receive the specified number of bytes from the remote site to which
    //+ the local side is connected.
    //
    //! PRE: The socket is open and connect() has been called.  buffer is at
    //+      least length bytes in size.
    //! POST: length bytes are read from the remote site and stored in the
    //+       given buffer.  The number of bytes read is returned to the
    //+       caller.
    //
    //! ARGS: buffer - A reference to the buffer (a std::string object) used
    //+                for storing the bytes received from the remote site.
    //! ARGS: length - The number of bytes to receive from the remote site.
    //+                If the length is 0, the value is determined from the
    //+                size of the string object.
    //! ARGS: flags  - Flags to use when receiving the data.  This is optional
    //+                and defaults to 0.
    //
    //! RETURNS: >-1 - The number of bytes received.
    //! RETURNS:  -1 - Something went wrong when trying to receive the data.
    // ------------------------------------------------------------------------
    virtual ssize_t recv(std::string& buffer, const size_t length,
                         const int flags = 0) = 0;

    // ------------------------------------------------------------------------
    //: Receive the specified number of bytes from the remote site to which
    //+ the local side is connected.
    //
    //! PRE: The socket is open and connect() has been called.  buffer is at
    //+      least length bytes in size.
    //! POST: length bytes are read from the remote site and stored in the
    //+       given buffer.  The number of bytes read is returned to the
    //+       caller.
    //
    //! ARGS: buffer - A pointer to the buffer (a vector of chars) used for
    //+                storing the bytes received from the remote site.
    //! ARGS: length - The number of bytes to receive from the remote site.
    //+                If the length is 0, the value is determined from the
    //+                size of the vector.
    //! ARGS: flags  - Flags to use when receiving the data.  This is optional
    //+                and defaults to 0.
    //
    //! RETURNS: >-1 - The number of bytes received.
    //! RETURNS:  -1 - Something went wrong when trying to receive the data.
    // ------------------------------------------------------------------------
    virtual ssize_t recv(std::vector<char>& buffer, const size_t length = 0,
                         const int flags = 0) = 0;

    // ------------------------------------------------------------------------
    //: Send the specified number of bytes contained in the given buffer from
    //+ the local side to the remote site to which we are connected.
    //
    //! PRE: The socket is open and connect() has been called.  buffer is at
    //+      least length bytes in size.
    //! POST: length bytes are sent from the buffer on the local site to the
    //+       remote site.  The number of bytes read is returned to the
    //+       caller.
    //
    //! ARGS: buffer - A pointer to the buffer containing the bytes to be
    //+                sent.
    //! ARGS: length - The number of bytes to sent to the remote site.
    //! ARGS: flags  - Flags to use when sending the data.  This is optional
    //+                and defaults to 0.
    //
    //! RETURNS: >-1 - The number of bytes received.
    //! RETURNS:  -1 - Something went wrong when trying to receive the data.
    // ------------------------------------------------------------------------
    virtual ssize_t send(const void* buffer, const size_t length,
                         const int flags = 0) = 0;

    // ------------------------------------------------------------------------
    //: Send the specified number of bytes contained in the given buffer from
    //+ the local side to the remote site to which we are connected.
    //
    //! PRE: The socket is open and connect() has been called.  buffer is at
    //+      least length bytes in size.
    //! POST: length bytes are sent from the buffer on the local site to the
    //+       remote site.  The number of bytes read is returned to the
    //+       caller.
    //
    //! ARGS: buffer - A pointer to the buffer (an array of unsigned chars)
    //+                containing the bytes to be sent.
    //! ARGS: length - The number of bytes to sent to the remote site.
    //! ARGS: flags  - Flags to use when sending the data.  This is optional
    //+                and defaults to 0.
    //
    //! RETURNS: >-1 - The number of bytes received.
    //! RETURNS:  -1 - Something went wrong when trying to receive the data.
    // ------------------------------------------------------------------------
    virtual ssize_t send(const unsigned char* buffer, const size_t length,
                         const int flags = 0) = 0;

    // ------------------------------------------------------------------------
    //: Send the specified number of bytes contained in the given buffer from
    //+ the local side to the remote site to which we are connected.
    //
    //! PRE: The socket is open and connect() has been called.  buffer is at
    //+      least length bytes in size.
    //! POST: length bytes are sent from the buffer on the local site to the
    //+       remote site.  The number of bytes read is returned to the
    //+       caller.
    //
    //! ARGS: buffer - A pointer to the buffer (an array of chars) containing
    //+                the bytes to be sent.
    //! ARGS: length - The number of bytes to sent to the remote site.
    //! ARGS: flags  - Flags to use when sending the data.  This is optional
    //+                and defaults to 0.
    //
    //! RETURNS: >-1 - The number of bytes received.
    //! RETURNS:  -1 - Something went wrong when trying to receive the data.
    // ------------------------------------------------------------------------
    virtual ssize_t send(const char* buffer, const size_t length,
                         const int flags = 0) = 0;

    // ------------------------------------------------------------------------
    //: Send the specified number of bytes contained in the given buffer from
    //+ the local side to the remote site to which we are connected.
    //
    //! PRE: The socket is open and connect() has been called.  buffer is at
    //+      least length bytes in size.
    //! POST: length bytes are sent from the buffer on the local site to the
    //+       remote site.  The number of bytes read is returned to the
    //+       caller.
    //
    //! ARGS: buffer - A reference to the buffer (a std::string object)
    //+                containing the bytes to be sent.
    //! ARGS: length - The number of bytes to sent to the remote site.  If the
    //+                length is 0, the value is determined from the size of
    //+                the string object.
    //! ARGS: flags  - Flags to use when sending the data.  This is optional
    //+                and defaults to 0.
    //
    //! RETURNS: >-1 - The number of bytes received.
    //! RETURNS:  -1 - Something went wrong when trying to receive the data.
    // ------------------------------------------------------------------------
    virtual ssize_t send(const std::string& buffer, const size_t length,
                         const int flags = 0) = 0;

    // ------------------------------------------------------------------------
    //: Send the specified number of bytes contained in the given buffer from
    //+ the local side to the remote site to which we are connected.
    //
    //! PRE: The socket is open and connect() has been called.  buffer is at
    //+      least length bytes in size.
    //! POST: length bytes are sent from the buffer on the local site to the
    //+       remote site.  The number of bytes read is returned to the
    //+       caller.
    //
    //! ARGS: buffer - A pointer to the buffer (a vector of chars) containing
    //+                the bytes to be sent.
    //! ARGS: length - The number of bytes to sent to the remote site.  If the
    //+                length is 0, the value is determined from the size of
    //+                the vector.
    //! ARGS: flags  - Flags to use when sending the data.  This is optional
    //+                and defaults to 0.
    //
    //! RETURNS: >-1 - The number of bytes received.
    //! RETURNS:  -1 - Something went wrong when trying to receive the data.
    // ------------------------------------------------------------------------
    virtual ssize_t send(const std::vector<char>& buffer, const size_t length,
                         const int flags = 0) = 0;

    // ------------------------------------------------------------------------
    //: Get the port for this socket.
    //
    //! PRE: The socket implementation pointer is valid.
    //! POST: The port number in m_addr is returned to the caller.
    //
    //! RETURNS: An unsigned short (16-bit value) giving the port number for
    //+          this socket.
    // ------------------------------------------------------------------------
    inline unsigned short
    getPort (void) const {
        return m_addr.getPort();
    }

    // ------------------------------------------------------------------------
    //: Get the type of this socket (e.g., vjSocketTypes::STREAM).
    //
    //! PRE: The socket implementation pointer is valid.
    //! POST: The value in m_type is returned to the caller.
    //
    //! RETURNS: A vjSocketTypes::Type value giving the socket type for this
    //+          socket.
    // ------------------------------------------------------------------------
    inline const vjSocketTypes::Type&
    getType (void) const {
        return m_type;
    }

protected:
    // ------------------------------------------------------------------------
    //: Default constructor.  This passes "INADDRY_ANY" on to the vjBlockIO
    //+ constructor.
    //
    //! PRE: None.
    //! POST: "INADDRY_ANY" is passed to the vjBlockIO constructor for this
    //+       socket's address.
    // ------------------------------------------------------------------------
    vjSocketImp (void)
        : vjBlockIO(std::string("INADDR_ANY"))
    {
fprintf(stderr, "vjSocketImp default constructor\n");
fprintf(stderr, "    Address: %s\n", m_name.c_str());
fprintf(stderr, "    Port: %hu\n", m_addr.getPort());
fprintf(stderr, "    Domain: %d\n", m_addr.getFamily());
fprintf(stderr, "    Type: %d\n", m_type);
        /* Do nothing. */ ;
    }

    // ------------------------------------------------------------------------
    //: Constructor.  This takes the address (either hostname or IP address)
    //+ of a remote site and a port and stores the values for later use in the
    //+ member variables of the object.
    //
    //! PRE: None.
    //! POST: The member variables are initialized.
    //
    //! ARGS: address - The hostname or IP address of the site to which we
    //+                 will connect.
    //! ARGS: port    - The port on the remote site to which we will connect.
    // ------------------------------------------------------------------------
    vjSocketImp (const std::string& address, const unsigned short port,
                 const vjSocketTypes::Domain domain,
                 const vjSocketTypes::Type type)
        : vjBlockIO(address), m_type(type)
    {
        m_addr.setPort(port);
        m_addr.setFamily(domain);
fprintf(stderr, "vjSocketImp(address, port, domain, type) constructor\n");
fprintf(stderr, "    Address: %s\n", m_name.c_str());
fprintf(stderr, "    Port: %hu\n", m_addr.getPort());
fprintf(stderr, "    Domain: %d\n", m_addr.getFamily());
fprintf(stderr, "    Type: %d\n", m_type);
    }

    vjInetAddr            m_addr;    //: The remote site's address structure
    vjSocketTypes::Type   m_type;    //:
};


#endif	/* _VJ_SOCKET_IMP_H_ */
