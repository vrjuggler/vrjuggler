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

#ifndef _VPR_SOCKET_STREAM_H_
#define _VPR_SOCKET_STREAM_H_

#include <vprConfig.h>

#include <IO/Socket/Socket.h>
#include <IO/Socket/InetAddr.h>


// XXX: Need to add methods for setsockopt(2) stuff.

namespace vpr {

// ----------------------------------------------------------------------------
//: Cross-platform stream socket class.
// ----------------------------------------------------------------------------
//! PUBLIC_API:
template<class RealSocketStreamImp, class RealSocketStreamImpParent>
class SocketStream_t : public Socket_t<RealSocketStreamImpParent> {
public:
    // ------------------------------------------------------------------------
    //: Default constructor.
    // ------------------------------------------------------------------------
    SocketStream_t (void)
        : Socket_t<RealSocketStreamImpParent>(), m_socket_stream_imp()
    {
        m_socket_imp = &m_socket_stream_imp;
    }

    // ------------------------------------------------------------------------
    //: Constructor.  This takes a reference to a vpr::InetAddr object giving
    //+ the local socket address and a reference to a vpr::InetAddr object
    //+ giving the remote address.
    //
    //! PRE: addr is a reference to a valid vpr::InetAddr object.
    //! POST: A socket is created using the contents of addr.
    //
    //! ARGS: local_addr  - A reference to a vpr::InetAddr object for the
    //+                     local socket address.
    //! ARGS: remote_addr - A reference to a vpr::InetAddr object for the
    //+                     remote socket address.
    // ------------------------------------------------------------------------
    SocketStream_t (const InetAddr& local_addr, const InetAddr& remote_addr)
        : Socket_t<RealSocketStreamImpParent>(),
          m_socket_stream_imp(local_addr, remote_addr)
    {
        m_socket_imp = &m_socket_stream_imp;
    }

    // ------------------------------------------------------------------------
    //: Destructor.  This currently does nothing.
    //
    //! PRE: None.
    //! POST: None.
    // ------------------------------------------------------------------------
    virtual ~SocketStream_t (void) {
        /* Do nothing. */ ;
    }

    // ------------------------------------------------------------------------
    //: Listen on the socket for incoming connection requests.
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
    inline virtual bool
    listen (const int backlog) {
        return m_socket_stream_imp.listen(backlog);
    }

    // ------------------------------------------------------------------------
    //: Accept an incoming connection request.
    //
    //! PRE: The socket is open and is in a listening state.
    //! POST: When a connection is established, a new Socket object will be
    //+       created that can be used for further communication with the
    //+       remote site.
    //
    //! RETURNS: Non-NULL - A new vpr::SocketStream object that can be used to
    //+                     communicate with the remote site.
    //! RETURNS: NULL     - A socket could not be created to establish
    //+                     communication with the remote site.  An error
    //+                     message is printed explaining what went wrong.
    //
    //! NOTE: This is a blocking call and will block until a connection is
    //+       established.
    // ------------------------------------------------------------------------
    inline virtual SocketStream_t*
    accept (void) {
        SocketStream_t* new_socket;
        RealSocketStreamImp* sock_imp = m_socket_stream_imp.accept();
        new_socket = new SocketStream_t(sock_imp);
        delete sock_imp;

        return new_socket;
    }

    // ------------------------------------------------------------------------
    //: Open a socket to be used for the server side communications.  This is
    //+ provided to automate the repeated tasks executed when a server is
    //+ set up.  It handles opening the socket, binding the address and going
    //+ into listening mode.
    //
    //! PRE: The member variables have been initialized properly.
    //! POST: The socket is in the listening state waiting for incoming
    //+       connection requests.
    //
    //! ARGS: backlog - The maximum length of the pending connection queue.
    //
    //! RETURNS: true  - The server socket is in the listening state.
    //! RETURNS: false - The server socket could not be set up.  An error
    //+                  message is printed explaining what went wrong.
    // ------------------------------------------------------------------------
    inline virtual bool
    openServer (const int backlog = 5) {
        bool retval;

        // First, open the socket.
        if ( retval = open() ) {
            // If that succeeded, bind to the internal address.
            if ( retval = bind() ) {
                // Finally, if that succeeded, go into listening mode.
                retval = listen(backlog);
            }
        }

        return retval;
    }

protected:
    // ------------------------------------------------------------------------
    //: Constructor.  Create a vpr::SocketStream object using the given
    //+ vpr::SocketStreamImp object pointer.  This is needed by accept().
    //
    //! PRE: sock_imp points to a valid vpr::SocketStreamImp object.
    //! POST: sock_imp is copied into m_socket_stream_imp.
    //
    //! ARGS: sock_imp - A pointer to a vpr::SocketStreamImp object.
    // ------------------------------------------------------------------------
    SocketStream_t (RealSocketStreamImp* sock_imp)
        : Socket_t<RealSocketStreamImpParent>(), m_socket_stream_imp(*sock_imp)
    {
        m_socket_imp = &m_socket_stream_imp;
    }

    RealSocketStreamImp m_socket_stream_imp; //: Platform-specific stream
                                             //+ socket implementation
};

}; // End of vpr namespace

#if defined(VPR_USE_NSPR)
#   include <md/NSPR/SocketStreamImpNSPR.h>

namespace vpr {
    typedef SocketStream_t<SocketStreamImpNSPR, SocketImpNSPR> SocketStream;
};
#elif defined(VPR_USE_WIN32)
#   include <md/WIN32/SocketStreamImpWinSock.h>

namespace vpr {
    typedef SocketStream_t<SocketStreamImpWinSock, SocketImpWinSock> SocketStream;
};
#else
#   include <md/POSIX/SocketStreamImpBSD.h>

namespace vpr {
    typedef SocketStream_t<SocketStreamImpBSD, SocketImpBSD> SocketStream;
};
#endif


#endif	/* _VPR_SOCKET_STREAM_H_ */
