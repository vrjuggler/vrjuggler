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

#ifndef _VPR_SOCKET_IMP_BSD_H_
#define _VPR_SOCKET_IMP_BSD_H_

#include <sys/types.h>
#include <netinet/in.h>
#include <string>
#include <vector>

#include <IO/BlockIO.h>
#include <md/POSIX/FileHandleUNIX.h>
#include <IO/Socket/InetAddr.h>
#include <IO/Socket/SocketIpOpt.h>

/*
#if defined(sgi) && defined(host_mips) && !defined(socklen_t)
typedef int socklen_t;
#endif
*/


namespace vpr {

class SocketImpBSD : public BlockIO, public SocketIpOpt
{
public:
    // ========================================================================
    // vpr::BlockIO overrides.
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
    // vpr::SocketImp interface implementation.
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
    //: Get the type of this socket (e.g., vpr::SocketTypes::STREAM).
    //
    //! PRE: The socket implementation pointer is valid.
    //! POST: The socket type for m_socket_imp is returned to the caller.
    //
    //! RETURNS: A vpr::SocketTypes::Type value giving the socket type for
    //+          this socket.
    // ------------------------------------------------------------------------
    inline const SocketTypes::Type&
    getType (void) const {
        return m_type;
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline const InetAddr&
    getLocalAddr (void) const {
        return m_local_addr;
    }

    bool setLocalAddr(const InetAddr& addr)
    {
      if (this->m_connected)
       { return false; }
       else
          m_local_addr = addr;

       return true;
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline const InetAddr&
    getRemoteAddr (void) const {
        return m_remote_addr;
    }

    bool setRemoteAddr(const InetAddr& addr)
    {
       if (this->m_connected)
       { return false; }
       else
          m_remote_addr = addr;

       return true;
    }

protected:
    // ------------------------------------------------------------------------
    // Default constructor.  This just initializes member variables to
    // reasonable defaults.
    //
    // PRE: None.
    // POST: The member variables are initialized accordingly to reasonable
    //       defaults.
    // ------------------------------------------------------------------------
    SocketImpBSD (const SocketTypes::Type sock_type)
        : BlockIO(std::string("INADDR_ANY")), m_connected(false),
          m_handle(NULL), m_type(sock_type)
    {
        m_handle = new FileHandleUNIX("INADDR_ANY");
    }

    // ------------------------------------------------------------------------
    // Standard constructor.  This takes two InetAddr objects, a local address
    // and a remote address.
    //
    // PRE: None.
    // POST: The member variables are initialized with the given values.
    //
    // Arguments:
    //     local_addr  - The local address for the socket.
    //     remote_addr - The remote address for the socket.
    // ------------------------------------------------------------------------
    SocketImpBSD (const InetAddr& local_addr, const InetAddr& remote_addr,
                  const SocketTypes::Type sock_type)
        : BlockIO(std::string("INADDR_ANY")), m_connected(false),
          m_handle(NULL), m_local_addr(local_addr), m_remote_addr(remote_addr),
          m_type(sock_type)
    {
        m_handle = new FileHandleUNIX(m_name);
    }

    // ------------------------------------------------------------------------
    // Destructor.  This currently does nothing.
    //
    // PRE: None.
    // POST: None.
    // ------------------------------------------------------------------------
    virtual ~SocketImpBSD(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual ssize_t
    read_i (void* buffer, const size_t length) {
        return m_handle->read(buffer, length);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual ssize_t
    readn_i (void* buffer, const size_t length) {
        return m_handle->readn(buffer, length);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual ssize_t
    write_i (const void* buffer, const size_t length) {
        return m_handle->write(buffer, length);
    }

    /**
     *
     */
    virtual bool getOption(const SocketOptions::Types option,
                           struct SocketOptions::Data& data);

    /**
     *
     */
    virtual bool setOption(const SocketOptions::Types option,
                           const struct SocketOptions::Data& data);

    bool              m_connected;
    FileHandleUNIX*   m_handle;      //:
    InetAddr          m_local_addr;  //: The local site's address structure
    InetAddr          m_remote_addr; //: The remote site's address structure
    SocketTypes::Type m_type;        //:
};

}; // End of vpr namespace


#endif   /* _VPR_SOCKET_IMP_BSD_H_ */
