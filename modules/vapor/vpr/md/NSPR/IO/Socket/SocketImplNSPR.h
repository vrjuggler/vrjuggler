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
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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

namespace vpr
{

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
   const std::string& getName (void)
   {
      return mName;
   }

   /**
    * Sets the blocking flags so that the socket is opened in blocking mode.
    *
    * @pre None.
    * @post The open flags are updated so that when the socket is opened, it
    *       is opened in blocking mode.  If the socket is already open, this
    *       has no effect.
    */
   void setOpenBlocking (void)
   {
      mOpenBlocking = true;
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
   void setOpenNonBlocking (void)
   {
      mOpenBlocking = false;
   }

   /**
    * Opens the socket.  This creates a new socket using the domain and type
    * options set through member variables.
    *
    * @pre mDomain and mType have been set to values recognized.
    * @post A new socket is created.
    *
    * Returns:
    *     true  - The socket was opened successfully.
    *     false - The socket could not be opened for some reason (an error
    *             message is printed explaining why).
    */
   vpr::ReturnStatus open(void);

   /**
    * Closes the socket.
    *
    * @pre The socket is open.
    * @post An attempt is made to close the socket.  The resulting status is
    *        returned to the caller.  If the socket is closed, mOpen is set
    *        to false.
    *
    * @return true if the socket was closed successfully.
    * @return false if the socket could not be closed for some reason.
    */
   vpr::ReturnStatus close(void);

   /**
    * Gets the open state of this socket.
    *
    * @pre None.
    * @post The boolean value giving the open state is returned to the
    *       caller.
    *
    * @return true is returned if this socket is open; false otherwise.
    */
   bool isOpen (void)
   {
      return mOpen;
   }

   bool isBound() const
   {
      return mBound;
   }

   /**
    * Binds this socket to the address in the host address member variable.
    *
    * @pre The socket is open, and mLocalAddr has been initialized properly.
    * @post The socket is bound to the address in mLocalAddr.
    *
    * Returns:
    *     true  - The socket was bound to the address successfully.
    *     false - The socket could not be bound to the address in
    *             mLocalAddr.  An error message is printed explaining what
    *             went wrong.
    */
   vpr::ReturnStatus bind(void);

   /**
    *  Return the contained handle
    */
   vpr::IOSys::Handle getHandle()
   {
      return mHandle;
   }

   /**
    * Queries if the blocking state for this socket is fixed and can no
    * longer be changed.
    */
   bool isBlockingFixed (void)
   {
      return mBlockingFixed;
   }

   vpr::ReturnStatus enableBlocking(void);

   vpr::ReturnStatus enableNonBlocking(void);

   /**
    * Gets the current blocking state for the socket.
    *
    * @return <code>true</code> is returned if the socket is in blocking
    *         mode.  Otherwise, <code>false</code> is returned.
    */
   bool getBlocking (void) const
   {
      return mBlocking;
   }

   /**
    * Gets the current non-blocking state for the socket.
    *
    * @return <code>true</code> is returned if the socket is in non-blocking
    *         mode.  Otherwise, <code>false</code> is returned.
    */
   bool getNonBlocking (void) const
   {
      return ! mBlocking;
   }

   // ========================================================================
   // vpr::SocketImp interface implementation.
   // ========================================================================

   /**
    * Connects the socket on the client side to the server side.  For a
    * datagram socket, this makes the address given to the constructor the
    * default destination for all packets.  For a stream socket, this has
    * the effect of establishing a connection with the destination.
    *
    * @pre The socket is open.
    * @post The socket is connected to the address in mLocalAddr.  For a
    *       stream socket, this means that a connection for future
    *       communication has been established.  For a datagram socket, the
    *       default destination for all packets is now mLocalAddr.
    *
    * Returns:
    *     true  - The connection was made.
    *     false - The connect could not be made.  An error message is
    *             printed explaining what happened.
    */
   vpr::ReturnStatus connect(const vpr::Interval timeout = vpr::Interval::NoTimeout);

   /**
    * Gets the status of a possibly connected socket.
    *
    * @pre None
    * @return true if the socket is connected to a remote addr.
    * @return false if the socket is not currently connect (the other side
    *         may have disconnected).
    */
   bool isConnected()
   {
      if ( mConnected )        // If it is not open, then it can't be connected
      {
         int num_avail = PR_Available(mHandle);
         if ( num_avail == 0 )
         {
            PRPollDesc poll_desc;
            poll_desc.fd = mHandle;
            poll_desc.in_flags = PR_POLL_READ;

            PR_Poll(&poll_desc, 1, PR_INTERVAL_NO_WAIT);
            if ( poll_desc.out_flags & PR_POLL_READ )
               return false;                           // Opened, but not connected
         }

         return true;        // Either have data, or are waiting for it
      }
      else
         return false;           // Not open --> not connected
   }

   /**
    * Gets the type of this socket (e.g., vpr::SocketTypes::STREAM).
    *
    * @pre The socket implementation pointer is valid.
    * @post The socket type for this socket is returned to the caller.
    *
    * @return A vpr::SocketTypes::Type value giving the socket type for
    *           this socket.
    */
   const SocketTypes::Type& getType (void) const
   {
      return mType;
   }

   const vpr::InetAddr& getLocalAddr (void) const
   {
      return mLocalAddr;
   }

   vpr::ReturnStatus setLocalAddr(const vpr::InetAddr& addr)
   {
      vpr::ReturnStatus status;

      if ( mBound )
      {
         vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL) << "SocketImplNSPR::setLocalAddr: Cant' set address of bound socket.\n" << vprDEBUG_FLUSH;
         status.setCode(ReturnStatus::Fail);
      }
      else
         mLocalAddr = addr;

      return status;
   }

   const vpr::InetAddr& getRemoteAddr (void) const
   {
      return mRemoteAddr;
   }

   vpr::ReturnStatus setRemoteAddr(const vpr::InetAddr& addr)
   {
      vpr::ReturnStatus status;

      if ( mConnected )
      {
         vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL) << "SocketImplNSPR::setRemoteAddr: Cant' set address of bound socket.\n" << vprDEBUG_FLUSH;
         status.setCode(vpr::ReturnStatus::Fail);
      }
      else
      {
         mRemoteAddr = addr;
      }

      return status;
   }

   vpr::ReturnStatus read_i(void* buffer, const vpr::Uint32 length,
                            vpr::Uint32& bytes_read,
                            const vpr::Interval timeout = vpr::Interval::NoTimeout);

   vpr::ReturnStatus readn_i(void* buffer, const vpr::Uint32 length,
                             vpr::Uint32& bytes_read,
                             const vpr::Interval timeout = vpr::Interval::NoTimeout);

   vpr::ReturnStatus write_i(const void* buffer, const vpr::Uint32 length,
                             vpr::Uint32& bytes_written,
                             const vpr::Interval timeout = vpr::Interval::NoTimeout);

   vpr::Uint32 availableBytes()
   {
      return PR_Available( mHandle );
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

   /**
    * Destructor.
    *
    * @pre None.
    * @post Closes the socket, and deallocates and resources associated with
    *       the socket.
    */
   ~SocketImplNSPR(void);

protected:
   /**
    * Default constructor.  This just initializes member variables to
    * reasonable defaults.
    *
    * @pre None.
    * @post The member variables are initialized accordingly to reasonable
    *       defaults.
    */
   SocketImplNSPR(const SocketTypes::Type sock_type);

   /**
    * Standard constructor.  This takes two InetAddr objects, a local address
    * and a remote address.
    *
    * @pre None.
    * @post The member variables are initialized with the given values.
    *
    * @param local_addr  The local address for the socket.
    * @param remote_addr The remote address for the socket.
    */
   SocketImplNSPR(const InetAddr& local_addr, const InetAddr& remote_addr,
                  const SocketTypes::Type sock_type);

   /**
    * Copy constructor.
    * XXX: We need to have a reference count here
    */
   SocketImplNSPR (const SocketImplNSPR& sock)
   {
      mLocalAddr      = sock.mLocalAddr;
      mRemoteAddr     = sock.mRemoteAddr;
      mHandle         = sock.mHandle;
      mType           = sock.mType;
      mOpen           = sock.mOpen;
      mOpenBlocking   = sock.mOpenBlocking;
      mBound          = sock.mBound;
      mConnected      = sock.mConnected;
      mBlocking       = sock.mBlocking;
      mBlockingFixed  = sock.mBlockingFixed;
   }

   std::string       mName;
   PRFileDesc*       mHandle;     /**< Handle to the socket */
   vpr::InetAddr     mLocalAddr;  /**< The local site's address structure */
   vpr::InetAddr     mRemoteAddr; /**< The remote site's address structure */

   vpr::SocketTypes::Type mType;  /**< Socket type */

   bool mOpen;
   bool mOpenBlocking;
   bool mBound;          /**< Is the socket bound to a port yet (connect
                               and bind do this) */
   bool mConnected;
   bool mBlocking;
   bool mBlockingFixed;
};

} // End of vpr namespace


#endif   /* _VPR_SOCKET_IMPL_NSPR_H_ */
