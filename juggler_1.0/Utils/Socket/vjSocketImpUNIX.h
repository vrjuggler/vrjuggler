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

#ifndef _VJ_SOCKET_IMP_UNIX_H_
#define _VJ_SOCKET_IMP_UNIX_H_

#include <sys/types.h>
#include <netinet/in.h>
#include <string>

#include <Utils/Socket/vjSocketImp.h>
#include <Utils/vjFileHandleUNIX.h>
#include <Utils/Socket/vjInetAddr.h>

#if defined(sgi) && defined(host_mips) && !defined(socklen_t)
typedef int socklen_t;
#endif

class vjSocketImpUNIX : virtual public vjSocketImp {
public:
    // ========================================================================
    // vjBlockIO overrides.
    // ========================================================================

    // ------------------------------------------------------------------------
    // Open the socket.  This creates a new socket using the domain and type
    // options set through member variables.
    //
    // PRE: m_domain and m_type have been set to values recognized by the
    //      socket(2) system call.
    // POST: A new socket is created with its file handle stored in the
    //       m_fdesc member variable.
    //
    // Returns:
    //     true  - The socket was opened successfully.
    //     false - The socket could not be opened for some reason (an error
    //             message is printed explaining why).
    // ------------------------------------------------------------------------
    virtual bool open(void);

    // ------------------------------------------------------------------------
    //: Close the socket.
    //
    //! PRE: The socket is open.
    //! POST: An attempt is made to close the socket.  The resulting status is
    //+       returned to the caller.  If the socket is closed, m_open is set
    //+       to false.
    //
    //! RETURNS: true  - The socket was closed successfully.
    //! RETURNS: false - The socket could not be closed for some reason.
    // ------------------------------------------------------------------------
    inline virtual bool
    close (void) {
        bool retval;

        if ( (retval = m_handle->close()) ) {
            m_open = false;
        }

        return retval;
    }

    // ------------------------------------------------------------------------
    // Bind this socket to the address in the host address member variable.
    //
    // PRE: The socket is open, and m_host_addr has been initialized properly.
    // POST: The socket is bound to the address in m_host_addr.
    //
    // Returns:
    //     true  - The socket was bound to the address successfully.
    //     false - The socket could not be bound to the address in
    //             m_host_addr.  An error message is printed explaining what
    //             went wrong.
    // ------------------------------------------------------------------------
    virtual bool bind(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    read (void* buffer, const size_t length) {
        return m_handle->read(buffer, length);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    read (unsigned char* buffer, const size_t length) {
        return m_handle->read(buffer, length);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    read (char* buffer, const size_t length) {
        return m_handle->read(buffer, length);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    read (std::vector<char>& buffer, const size_t length = 0) {
        return m_handle->read(buffer, length);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    readn (void* buffer, const size_t length) {
        return m_handle->readn(buffer, length);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    readn (unsigned char* buffer, const size_t length) {
        return m_handle->readn(buffer, length);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    readn (char* buffer, const size_t length) {
        return m_handle->readn(buffer, length);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    readn (std::vector<char>& buffer, const size_t length = 0) {
        return m_handle->readn(buffer, length);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    write (void* buffer, const size_t length) {
        return m_handle->write(buffer, length);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    write (unsigned char* buffer, const size_t length) {
        return m_handle->write(buffer, length);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    write (char* buffer, const size_t length) {
        return m_handle->write(buffer, length);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    write (std::vector<char>& buffer, const size_t length = 0) {
        return m_handle->write(buffer, length);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual bool
    enableBlocking (void) {
        return m_handle->enableBlocking();
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual bool
    enableNonBlocking (void) {
        return m_handle->enableNonBlocking();
    }

    // ========================================================================
    // vjSocketImp interface implementation.
    // ========================================================================

    // ------------------------------------------------------------------------
    // Connect the socket on the client side to the server side.  For a
    // datagram socket, this makes the address given to the constructor the
    // default destination for all packets.  For a stream socket, this has
    // the effect of establishing a connection with the destination.
    //
    // PRE: The socket is open.
    // POST: The socket is connected to the address in m_host_addr.  For a
    //       stream socket, this means that a connection for future
    //       communication has been established.  For a datagram socket, the
    //       default destination for all packets is now m_host_addr.
    //
    // Returns:
    //     true  - The connection was made.
    //     false - The connect could not be made.  An error message is
    //             printed explaining what happened.
    // ------------------------------------------------------------------------
    virtual bool connect(void);

    // ------------------------------------------------------------------------
    // Receive the specified number of bytes from the remote site to which the
    // local side is connected.
    //
    // PRE: The socket is open and connect() has been called.  buffer is at
    //      least length bytes in size.
    // POST: length bytes are read from the remote site and stored in the
    //       given buffer.  The number of bytes read is returned to the
    //       caller.
    //
    // Arguments:
    //     buffer - A pointer to the buffer used for storing the bytes
    //              received from the remote site.
    //     length - The number of bytes to receive from the remote site.
    //     flags  - Flags to use when receiving the data.  This is optional
    //              and defaults to 0.
    //
    // Returns:
    //     >-1 - The number of bytes received.
    //      -1 - Something went wrong when trying to receive the data.
    // ------------------------------------------------------------------------
    virtual ssize_t recv(void* buffer, const size_t length,
                         const int flags = 0);

    // ------------------------------------------------------------------------
    // Receive the specified number of bytes from the remote site to which the
    // local side is connected.
    //
    // PRE: The socket is open and connect() has been called.  buffer is at
    //      least length bytes in size.
    // POST: length bytes are read from the remote site and stored in the
    //       given buffer.  The number of bytes read is returned to the
    //       caller.
    //
    // Arguments:
    //     buffer - A pointer to the buffer (an array of unsigned chars) used
    //              for storing the bytes received from the remote site.
    //     length - The number of bytes to receive from the remote site.
    //     flags  - Flags to use when receiving the data.  This is optional
    //              and defaults to 0.
    //
    // Returns:
    //     >-1 - The number of bytes received.
    //      -1 - Something went wrong when trying to receive the data.
    // ------------------------------------------------------------------------
    virtual ssize_t recv(unsigned char* buffer, const size_t length,
                         const int flags = 0);

    // ------------------------------------------------------------------------
    // Receive the specified number of bytes from the remote site to which the
    // local side is connected.
    //
    // PRE: The socket is open and connect() has been called.  buffer is at
    //      least length bytes in size.
    // POST: length bytes are read from the remote site and stored in the
    //       given buffer.  The number of bytes read is returned to the
    //       caller.
    //
    // Arguments:
    //     buffer - A pointer to the buffer (an array of chars) used for
    //              storing the bytes received from the remote site.
    //     length - The number of bytes to receive from the remote site.
    //     flags  - Flags to use when receiving the data.  This is optional
    //              and defaults to 0.
    //
    // Returns:
    //     >-1 - The number of bytes received.
    //      -1 - Something went wrong when trying to receive the data.
    // ------------------------------------------------------------------------
    virtual ssize_t recv(char* buffer, const size_t length,
                         const int flags = 0);

    // ------------------------------------------------------------------------
    // Receive the specified number of bytes from the remote site to which the
    // local side is connected.
    //
    // PRE: The socket is open and connect() has been called.  buffer is at
    //      least length bytes in size.
    // POST: length bytes are read from the remote site and stored in the
    //       given buffer.  The number of bytes read is returned to the
    //       caller.
    //
    // Arguments:
    //     buffer - A pointer to the buffer (a vector of chars) used for
    //              storing the bytes received from the remote site.
    //     length - The number of bytes to receive from the remote site.  If
    //              the length is 0, the value is determined from the size of
    //              the vector.
    //     flags  - Flags to use when receiving the data.  This is optional
    //              and defaults to 0.
    //
    // Returns:
    //     >-1 - The number of bytes received.
    //      -1 - Something went wrong when trying to receive the data.
    // ------------------------------------------------------------------------
    virtual ssize_t recv(std::vector<char>& buffer, const size_t length,
                         const int flags = 0);

    // ------------------------------------------------------------------------
    // Send the specified number of bytes contained in the given buffer from
    // the local side to the remote site to which we are connected.
    //
    // PRE: The socket is open and connect() has been called.  buffer is at
    //      least length bytes in size.
    // POST: length bytes are sent from the buffer on the local site to the
    //       remote site.  The number of bytes read is returned to the caller.
    //
    // Arguments:
    //     buffer - A pointer to the buffer containing the bytes to be sent.
    //     length - The number of bytes to sent to the remote site.
    //     flags  - Flags to use when sending the data.  This is optional and
    //              defaults to 0.
    //
    // Returns:
    //     >-1 - The number of bytes received.
    //      -1 - Something went wrong when trying to receive the data.
    // ------------------------------------------------------------------------
    virtual ssize_t send(const void* buffer, const size_t length,
                         const int flags = 0);

    // ------------------------------------------------------------------------
    // Send the specified number of bytes contained in the given buffer from
    // the local side to the remote site to which we are connected.
    //
    // PRE: The socket is open and connect() has been called.  buffer is at
    //      least length bytes in size.
    // POST: length bytes are sent from the buffer on the local site to the
    //       remote site.  The number of bytes read is returned to the caller.
    //
    // Arguments:
    //     buffer - A pointer to the buffer (an array of unsigned chars)
    //              containing the bytes to be sent.
    //     length - The number of bytes to sent to the remote site.
    //     flags  - Flags to use when sending the data.  This is optional and
    //              defaults to 0.
    //
    // Returns:
    //     >-1 - The number of bytes received.
    //      -1 - Something went wrong when trying to receive the data.
    // ------------------------------------------------------------------------
    virtual ssize_t send(const unsigned char* buffer, const size_t length,
                         const int flags = 0);

    // ------------------------------------------------------------------------
    // Send the specified number of bytes contained in the given buffer from
    // the local side to the remote site to which we are connected.
    //
    // PRE: The socket is open and connect() has been called.  buffer is at
    //      least length bytes in size.
    // POST: length bytes are sent from the buffer on the local site to the
    //       remote site.  The number of bytes read is returned to the caller.
    //
    // Arguments:
    //     buffer - A pointer to the buffer (an array of chars) containing the
    //              bytes to be sent.
    //     length - The number of bytes to sent to the remote site.
    //     flags  - Flags to use when sending the data.  This is optional and
    //              defaults to 0.
    //
    // Returns:
    //     >-1 - The number of bytes received.
    //      -1 - Something went wrong when trying to receive the data.
    // ------------------------------------------------------------------------
    virtual ssize_t send(const char* buffer, const size_t length,
                         const int flags = 0);

    // ------------------------------------------------------------------------
    // Send the specified number of bytes contained in the given buffer from
    // the local side to the remote site to which we are connected.
    //
    // PRE: The socket is open and connect() has been called.  buffer is at
    //      least length bytes in size.
    // POST: length bytes are sent from the buffer on the local site to the
    //       remote site.  The number of bytes read is returned to the caller.
    //
    // Arguments:
    //     buffer - A pointer to the buffer (a vector of chars) containing the
    //              bytes to be sent.
    //     length - The number of bytes to sent to the remote site.  If the
    //              length is 0, the value is determined from the size of the
    //              vector.
    //     flags  - Flags to use when sending the data.  This is optional and
    //              defaults to 0.
    //
    // Returns:
    //     >-1 - The number of bytes received.
    //      -1 - Something went wrong when trying to receive the data.
    // ------------------------------------------------------------------------
    virtual ssize_t send(const std::vector<char>& buffer, const size_t length,
                         const int flags = 0);

protected:
    // ------------------------------------------------------------------------
    // Default constructor.  This just initializes member variables to
    // reasonable defaults.
    //
    // PRE: None.
    // POST: The member variables are initialized accordingly to reasonable
    //       defaults.
    // ------------------------------------------------------------------------
    vjSocketImpUNIX(void);

    // ------------------------------------------------------------------------
    // Standard constructor.  This takes the given address (a string
    // containing a hostname or an IP address), port, domain and type and
    // stores the values in the member variables for use when opening the
    // socket and performing communications.
    // 
    // PRE: None.
    // POST: The member variables are initialized with the given values.
    //
    // Arguments:
    //     address - The hostname or IP address of the remote site with which
    //               communication will be performed.
    //     port    - The port on the remote site to which we will send data.
    //     domain  - The communications domain for the socket.
    //     type    - The communications type of the socket (either
    //               vjSocket::STREAM or vjSocket::DATAGRAM).
    //
    // Note:
    //     The m_file_name member variable is used to store the remote site's
    //     address string.
    // ------------------------------------------------------------------------
    vjSocketImpUNIX(const std::string& address, const unsigned short port,
                    const vjSocketTypes::Domain domain,
                    const vjSocketTypes::Type type);

    // ------------------------------------------------------------------------
    // Destructor.  This currently does nothing.
    //
    // PRE: None.
    // POST: None.
    // ------------------------------------------------------------------------
    virtual ~vjSocketImpUNIX(void);

    // ------------------------------------------------------------------------
    // Look up the address in m_file_name and store the address in the
    // m_host_addr structure's address field.
    //
    // PRE: None.
    // POST: The address string is converted into a 32-bit INET address, and
    //       the m_host_addr structure is populated accordingly.
    //
    // Returns:
    //     true  - The address lookup was successful.
    //     false - The address could not be looked up.  An error message is
    //             printed to stderr explaining what went wrong.
    // ------------------------------------------------------------------------
    bool lookupAddress(void);

    vjFileHandleUNIX* m_handle;       //:
};


#endif	/* _VJ_SOCKET_IMP_UNIX_H_ */
