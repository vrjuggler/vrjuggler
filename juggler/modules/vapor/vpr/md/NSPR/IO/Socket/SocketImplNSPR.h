/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ****************** <VPR heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VPR_SOCKET_IMPL_NSPR_H_
#define _VPR_SOCKET_IMPL_NSPR_H_

#include <vpr/vprConfig.h>

#include <string>
#include <vector>
#include <prio.h>
#include <prerror.h>

#include <vpr/IO/Socket/InetAddr.h>
#include <vpr/md/NSPR/NSPRHelpers.h>
#include <vpr/IO/Socket/SocketTypes.h>
#include <vpr/IO/Socket/SocketOptions.h>
#include <vpr/IO/IOSys.h>
#include <vpr/Util/ReturnStatus.h>

#include <vpr/Util/Debug.h>

namespace vpr {

class VPR_CLASS_API SocketImplNSPR
{
public:
    // ========================================================================
    // vpr::BlockIO basics.
    // ========================================================================

    /**
     * Gets the "name" of this socket.  It is typically the address of the
     * peer host.
     *
     * @pre None.
     * @post
     *
     * @return An object containing the "name" of this socket.
     */
    inline const std::string&
    getName (void) {
        return m_name;
    }

    /**
     * Sets the blocking flags so that the socket is opened in blocking mode.
     *
     * @pre None.
     * @post The open flags are updated so that when the socket is opened, it
     *       is opened in blocking mode.  If the socket is already open, this
     *       has no effect.
     */
    inline void
    setOpenBlocking (void) {
        m_open_blocking = true;
    }

    /**
     * Sets the blocking flags so that the socket is opened in non-blocking
     * mode.
     *
     * @pre None.
     * @post The open flags are updated so that when the socket is opened, it
     *       is opened in non-blocking mode.  If the socket is already open,
     *       this has no effect.
     */
    inline void
    setOpenNonBlocking (void) {
        m_open_blocking = false;
    }

    // ------------------------------------------------------------------------
    // Open the socket.  This creates a new socket using the domain and type
    // options set through member variables.
    //
    // PRE: m_domain and m_type have been set to values recognized
    // POST: A new socket is created
    //
    // Returns:
    //     true  - The socket was opened successfully.
    //     false - The socket could not be opened for some reason (an error
    //             message is printed explaining why).
    // ------------------------------------------------------------------------
    vpr::ReturnStatus open(void);

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
    vpr::ReturnStatus close(void);

    /**
     * Gets the open state of this socket.
     *
     * @pre None.
     * @post The boolean value giving the open state is returned to the
     *       caller.
     *
     * @return <code>true</code> is returned if this socket is open;
     *         <code>false</code> otherwise.
     */
    inline bool
    isOpen (void) {
        return m_open;
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
    vpr::ReturnStatus bind(void);

    // ---------------------------------------
    //: Return the contained handle
    // --------------------------------------
    vpr::IOSys::Handle getHandle()
    {
       return m_handle;
    }

    /**
     * Queries if the blocking state for this socket is fixed and can no
     * longer be changed.
     */
    inline bool
    isBlockingFixed (void) {
        return m_blocking_fixed;
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    vpr::ReturnStatus enableBlocking(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    vpr::ReturnStatus enableNonBlocking(void);

    /**
     * Gets the current blocking state for the socket.
     *
     * @return <code>true</code> is returned if the socket is in blocking
     *         mode.  Otherwise, <code>false</code> is returned.
     */
    inline bool
    getBlocking (void) const {
        return m_blocking;
    }

    /**
     * Gets the current non-blocking state for the socket.
     *
     * @return <code>true</code> is returned if the socket is in non-blocking
     *         mode.  Otherwise, <code>false</code> is returned.
     */
    inline bool
    getNonBlocking (void) const {
        return ! m_blocking;
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
    vpr::ReturnStatus connect(const vpr::Interval timeout = vpr::Interval::NoTimeout);

    // ------------------------------------------------------------------
    //: Get the status of a possibly connected socket
    //
    //! PRE: None
    //! RETURNS: true - The socket is connected to a remote addr
    //! RETURNS: false - The socket is not currently connect (the other side may have disconnected)
    // -----------------------------------------------------------------
    bool isConnected()
    {
        if(m_connected)        // If it is not open, then it can't be connected
        {
            int num_avail = PR_Available(m_handle);
            if(num_avail == 0)
            {
                PRPollDesc poll_desc;
                poll_desc.fd = m_handle;
                poll_desc.in_flags = PR_POLL_READ;

                PR_Poll(&poll_desc, 1, PR_INTERVAL_NO_WAIT);
                if(poll_desc.out_flags & PR_POLL_READ)
                    return false;                           // Opened, but not connected
            }

            return true;        // Either have data, or are waiting for it
        }
        else
            return false;           // Not open --> not connected
    }

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

    ReturnStatus setLocalAddr(const InetAddr& addr)
    {
       ReturnStatus status;

      if (m_bound)
       {
          vprDEBUG(vprDBG_ALL,0) << "SocketImplNSPR::setLocalAddr: Cant' set address of bound socket.\n" << vprDEBUG_FLUSH;
          status.setCode(ReturnStatus::Fail);
       }
       else
          m_local_addr = addr;

       return status;
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline const InetAddr&
    getRemoteAddr (void) const {
        return m_remote_addr;
    }

    ReturnStatus setRemoteAddr(const InetAddr& addr)
    {
       ReturnStatus status;

       if (m_connected)
       {
           vprDEBUG(vprDBG_ALL,0) << "SocketImplNSPR::setRemoteAddr: Cant' set address of bound socket.\n" << vprDEBUG_FLUSH;
           status.setCode(ReturnStatus::Fail);
       }
       else
          m_remote_addr = addr;

       return status;
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    vpr::ReturnStatus read_i(void* buffer, const vpr::Uint32 length,
                       vpr::Uint32& bytes_read,
                       const vpr::Interval timeout = vpr::Interval::NoTimeout);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    vpr::ReturnStatus readn_i(void* buffer, const vpr::Uint32 length,
                        vpr::Uint32& bytes_read,
                        const vpr::Interval timeout = vpr::Interval::NoTimeout);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    vpr::ReturnStatus write_i(const void* buffer, const vpr::Uint32 length,
                        vpr::Uint32& bytes_written,
                        const vpr::Interval timeout = vpr::Interval::NoTimeout);

    vpr::Uint32 availableBytes()
    {
      return PR_Available( m_handle );
    }

    /**
     * Retrieves the value for the given option as set on the socket.
     *
     * @param option The option to be queried.
     * @param data   A data buffer that will be used to store the value of the
     *               given option.
     *
     * @return vpr::ReturnStatus::Succeed is returned if the value for the given
     *         option was retrieved successfully.<br>
     *         vpr::ReturnStatus;:Fail is returned otherwise.
     */
    vpr::ReturnStatus getOption(const vpr::SocketOptions::Types option,
                          struct vpr::SocketOptions::Data& data);

    /**
     * Sets a value for the given option on the socket using the given data
     * block.
     *
     * @param option The option whose value will be set.
     * @param data   A data buffer containing the value to be used in setting
     *               the socket option.
     */
    vpr::ReturnStatus setOption(const vpr::SocketOptions::Types option,
                          const struct vpr::SocketOptions::Data& data);


    // ------------------------------------------------------------------------
    // Destructor.
    //
    // PRE: None.
    // POST: Closes the socket, and deallocates and resources associated with the socket.
    // ------------------------------------------------------------------------
    ~SocketImplNSPR(void);

protected:
    // ------------------------------------------------------------------------
    // Default constructor.  This just initializes member variables to
    // reasonable defaults.
    //
    // PRE: None.
    // POST: The member variables are initialized accordingly to reasonable
    //       defaults.
    // ------------------------------------------------------------------------
    SocketImplNSPR(const SocketTypes::Type sock_type);

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
    SocketImplNSPR(const InetAddr& local_addr, const InetAddr& remote_addr,
                   const SocketTypes::Type sock_type);

    // ------------------------------------------------------------------------
    // Copy constructor.
    // XXX: We need to have a reference count here
    // ------------------------------------------------------------------------
    SocketImplNSPR (const SocketImplNSPR& sock)
    {
        m_local_addr      = sock.m_local_addr;
        m_remote_addr     = sock.m_remote_addr;
        m_handle          = sock.m_handle;
        m_type            = sock.m_type;
        m_open            = sock.m_open;
        m_open_blocking   = sock.m_open_blocking;
        m_bound           = sock.m_bound;
        m_connected       = sock.m_connected;
        m_blocking        = sock.m_blocking;
        m_blocking_fixed  = sock.m_blocking_fixed;
    }

    std::string       m_name;
    PRFileDesc*       m_handle;      //: Handle to the socket
    vpr::InetAddr     m_local_addr;  //: The local site's address structure
    vpr::InetAddr     m_remote_addr; //: The remote site's address structure

    vpr::SocketTypes::Type m_type;        //:

    bool m_open;
    bool m_open_blocking;
    bool m_bound;          /**< Is the socket bound to a port yet (connect
                                and bind do this */
    bool m_connected;
    bool m_blocking;
    bool m_blocking_fixed;
};

}; // End of vpr namespace


#endif   /* _VPR_SOCKET_IMPL_BSD_H_ */
