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

#ifndef _VJ_SOCKET_STREAM_H_
#define _VJ_SOCKET_STREAM_H_

#include <vjConfig.h>

#include <VPR/IO/Socket/vjSocket.h>
#include <VPR/IO/Socket/vjSocketStreamImp.h>
#include <VPR/IO/Socket/vjInetAddr.h>


// XXX: Need to add methods for setsockopt(2) stuff.

// ----------------------------------------------------------------------------
//: Stream socket interface.
// ----------------------------------------------------------------------------
//! PUBLIC_API:
class VJ_CLASS_API vjSocketStream : public vjSocket {
public:
    // ------------------------------------------------------------------------
    //: Default constructor.
    // ------------------------------------------------------------------------
    vjSocketStream(void);

    // ------------------------------------------------------------------------
    //: Constructor.  This takes a port number and stores the value for later
    //+ use in the member variables of the object.  The address defaults to
    //+ "INADDR_ANY" which is interpreted properly when the socket is created.
    //
    //! PRE: None.
    //! POST: The member variables are initialized with the m_type variable in
    //+       particular set to vjSocketTypes::STREAM.
    //
    //! ARGS: port - The port on the remote site to which we will connect.
    // ------------------------------------------------------------------------
    vjSocketStream(const unsigned short port);

    // ------------------------------------------------------------------------
    // Constructor.  This takes the address (either hostname or IP address) of
    // a remote site and a port and stores the values for later use in the
    // member variables of the object.
    //
    // PRE: None.
    // POST: The member variables are initialized with the m_type variable in
    //       particular set to vjSocketTypes::STREAM.
    //
    // Arguments:
    //     address - The hostname or IP address of the site to which we will
    //               connect.
    //     port    - The port on the remote site to which we will connect.
    // ------------------------------------------------------------------------
    vjSocketStream(const std::string& address, const unsigned short port);

    // ------------------------------------------------------------------------
    // Constructor.  This takes the address (either hostname or IP address) of
    // a remote site and a port and stores the values for later use in the
    // member variables of the object.
    //
    // PRE: None.
    // POST: The member variables are initialized with the m_type variable in
    //       particular set to vjSocketTypes::STREAM.
    //
    // Arguments:
    //     address - The hostname or IP address of the site to which we will
    //               connect.
    //     port    - The port on the remote site to which we will connect.
    //     domain  -
    // ------------------------------------------------------------------------
    vjSocketStream(const std::string& address, const unsigned short port,
                   const vjSocketTypes::Domain domain);

    // ------------------------------------------------------------------------
    //: Constructor.  This takes an address of the form
    //+ <host addr>:<port number> and creates a socket.
    //
    //! PRE: The given address is of the form <host addr>:<port number> where
    //+      <host addr> is either a hostname or a dotted-decimal IPv4
    //+      address and <port number> is a valid port number.
    //! POST: The host address and port number are extracted, and a socket is
    //+       created using those values.
    //
    //! ARGS: An address of the form <host addr>:<port number>.
    // ------------------------------------------------------------------------
    vjSocketStream(const std::string& address);

    // ------------------------------------------------------------------------
    //: Constructor.  This takes a reference to a vjInetAddr object and
    //+ creates a socket using its address value and port number.
    //
    //! PRE: addr is a reference to a valid vjInetAddr object.
    //! POST: A socket is created using the contents of addr.
    //
    //! ARGS: addr - A reference to a vjInetAddr object.
    // ------------------------------------------------------------------------
    vjSocketStream(const vjInetAddr& addr);

    // ------------------------------------------------------------------------
    //: Destructor.  This currently does nothing.
    //
    //! PRE: None.
    //! POST: None.
    // ------------------------------------------------------------------------
    virtual ~vjSocketStream (void) {
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
        return m_socket_stream_imp->listen(backlog);
    }

    // ------------------------------------------------------------------------
    //: Accept an incoming connection request.
    //
    //! PRE: The socket is open and is in a listening state.
    //! POST: When a connection is established, a new Socket object will be
    //+       created that can be used for further communication with the
    //+       remote site.
    //
    //! RETURNS: Non-NULL - A new vjSocketStream object that can be used to
    //+                     communicate with the remote site.
    //! RETURNS: NULL     - A socket could not be created to establish
    //+                     communication with the remote site.  An error
    //+                     message is printed explaining what went wrong.
    //
    //! NOTE: This is a blocking call and will block until a connection is
    //+       established.
    // ------------------------------------------------------------------------
    inline virtual vjSocketStream*
    accept (void) {
        vjSocketStreamImp* sock_imp = m_socket_stream_imp->accept();
        return new vjSocketStream(sock_imp);
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
    virtual bool openServer(const int backlog = 5);

protected:
    // ------------------------------------------------------------------------
    //: Constructor.  Create a vjSocketStream object using the given
    //+ vjSocketStreamImp object pointer.  This is needed by accept().
    //
    //! PRE: sock_imp points to a valid vjSocketStreamImp object.
    //! POST: sock_imp is passed on to the vjSocket constructor, and
    //+       m_socket_stream_imp is assigned the value of sock_imp.
    //
    //! ARGS: sock_imp - A pointer to a vjSocketStreamImp object.
    // ------------------------------------------------------------------------
    vjSocketStream (vjSocketStreamImp* sock_imp)
        : vjSocket(sock_imp), m_socket_stream_imp(sock_imp)
    {
        /* Do nothing. */ ;
    }

    vjSocketStreamImp* m_socket_stream_imp; //: Platform-specific stream socket
                                            //+ implementation
};


#endif	/* _VJ_SOCKET_STREAM_H_ */
