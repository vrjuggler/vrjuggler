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
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

/** \class SocketImplNSPR SocketImplNSPR.h vpr/IO/Socket/Socket.h
 *
 * NSPR implementation of the base socket interface.  This is used in
 * conjunction with vpr::SocketConfiguration to create the typedef
 * vpr::Socket.
 */
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
    * @return An object containing the "name" of this socket.
    */
   const std::string& getName() const
   {
      return mName;
   }

   /**
    * Opens the socket.  This creates a new socket using the domain and type
    * options set through member variables.
    *
    * @pre \c mDomain and \c mType have been set to values recognized.
    * @post A new socket is created.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the socket was opened
    *         successfully.
    * @return vpr::ReturnStatus::Fail is returned if the socket could not be
    *         opened for some reason (an error message is printed explaining
    *         why).
    */
   vpr::ReturnStatus open();

   /**
    * Closes the socket.
    *
    * @pre The socket is open.
    * @post An attempt is made to close the socket.  The resulting status is
    *       returned to the caller.  If the socket is closed, \c mOpen is set
    *       to \c false.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the socket was closed
    *         successfully.
    * @return vpr::ReturnStatus::Fail is returned if the socket could not be
    *         closed for some reason.
    */
   vpr::ReturnStatus close();

   /**
    * Gets the open state of this socket.
    *
    * @post The boolean value giving the open state is returned to the caller.
    *
    * @return \c true is returned if this socket is open; \c false otherwise.
    */
   bool isOpen() const
   {
      return mOpen;
   }

   /**
    * Gets the bound state of this socket.
    *
    * @post The boolean value giving the bound state is returned to the caller.
    *
    * @return \c true is returned if this socket is bound; \c false otherwise.
    */
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
    * @return vpr::ReturnStatus::Succeed is returned if this socket was bound
    *         to its local address successfully.
    * @return vpr::ReturnStatus::Fail is returned if this socket could not be
    *         bound to the address in \c mLocalAddr.  An error message is
    *         printed explaining what went wrong.
    */
   vpr::ReturnStatus bind();

   /**
    * Returns the contained handle.
    */
   vpr::IOSys::Handle getHandle() const
   {
      return mHandle;
   }

   /**
    * Queries if the blocking state for this socket is fixed and can no
    * longer be changed.
    */
   bool isBlockingFixed() const
   {
      return mBlockingFixed;
   }

   /**
    * Sets the blocking state for this socket.
    *
    * @pre This socket is not already open (isOpen() returns \c false).
    * @post Processes will block (or not) when accessing the socket.
    *
    * @param blocking The new blocking state.  A value of \c true indicates
    *                 that this will be a blocking socket.  A value of \c false
    *                 indicates that it will be a non-blocking socket.
    * 
    * @return vpr::ReturnStatus::Succeed is returned if the blocking state was
    *         changed successfully.
    * @return vpr::ReturnStatus::Fail is returned if the blocking state was not
    *         changed.  This will happen if this socket is already open.
    *
    * @see isOpen, open
    */
   vpr::ReturnStatus setBlocking(bool blocking);

   /**
    * Gets the current blocking state for this socket.
    *
    * @return \c true is returned if the socket is in blocking mode.
    *         Otherwise, \c false is returned.
    */
   bool isBlocking() const
   {
      return mBlocking;
   }

   /**
    * Connects the socket on the client side to the server side.  For a
    * datagram socket, this makes the address given to the constructor the
    * default destination for all packets.  For a stream socket, this has
    * the effect of establishing a connection with the destination.
    *
    * @pre This socket is open.
    * @post This socket is connected to the address in \c mLocalAddr.  For a
    *       stream socket, this means that a connection for future
    *       communication has been established.  For a datagram socket, the
    *       default destination for all packets is now \c mLocalAddr.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the connection was
    *         made.
    * @return vpr::ReturnStatus::Fail is returned if the connection could not
    *         be made.  An error message is printed explaining what happened.
    */
   vpr::ReturnStatus connect(const vpr::Interval timeout = vpr::Interval::NoTimeout);

   /**
    * Gets the status of a possibly connected socket.
    *
    * @return \c true if the socket is connected to a remote address.
    * @return \c false if the socket is not currently connect (the other side
    *         may have disconnected).
    */
   bool isConnected() const
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
            {
               return false;             // Opened, but not connected
            }
         }

         return true;        // Either have data, or are waiting for it
      }
      else
      {
         return false;           // Not open --> not connected
      }
   }

   /**
    * Gets the type of this socket (for example, vpr::SocketTypes::STREAM).
    *
    * @pre The socket implementation pointer is valid.
    * @post The socket type for this socket is returned to the caller.
    *
    * @return A vpr::SocketTypes::Type value giving the socket type for
    *         this socket.
    */
   const SocketTypes::Type& getType() const
   {
      return mType;
   }

   /**
    * Returns the local address to which this socket will be or is bound.
    */
   const vpr::InetAddr& getLocalAddr() const
   {
      return mLocalAddr;
   }

   /**
    * Changes the local address for this socket.
    *
    * @pre This socket is not already bound (isBound() returns \c false).
    * @post On successful completion, \c mLocalAddr is updated to use the given
    *       vpr::InetAddr object.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the local address was
    *         changed successfully.
    * @return vpr::ReturnStatus::Fail is returned if the local address could
    *         not be changed.  This occurs when this socket is already bound.
    *
    * @see isBound, bind
    */
   vpr::ReturnStatus setLocalAddr(const vpr::InetAddr& addr)
   {
      vpr::ReturnStatus status;

      if ( mBound )
      {
         vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
            << "[vpr::SocketImplNSPR::setLocalAddr()] "
            << "Can't set address of bound socket.\n" << vprDEBUG_FLUSH;
         status.setCode(ReturnStatus::Fail);
      }
      else
      {
         mLocalAddr = addr;
      }

      return status;
   }

   /**
    * Returns the remote address with which this socket will communicate.
    */
   const vpr::InetAddr& getRemoteAddr() const
   {
      return mRemoteAddr;
   }

   /**
    * Changes the remote address for this socket.
    *
    * @pre This socket is not already connected (isConnected() returns
    *      \c false).
    * @post On successful completion, \c mRemoteAddr is updated to use the
    *       given vpr::InetAddr object.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the remote address was
    *         changed successfully.
    * @return vpr::ReturnStatus::Fail is returned if the remote address could
    *         not be changed.  This occurs when this socket is already
    *         connected.
    *
    * @see isConnected, connect
    */
   vpr::ReturnStatus setRemoteAddr(const vpr::InetAddr& addr)
   {
      vpr::ReturnStatus status;

      if ( mConnected )
      {
         vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
            << "[vpr::SocketImplNSPR::setRemoteAddr()] "
            << "Can't set address of bound socket.\n" << vprDEBUG_FLUSH;
         status.setCode(vpr::ReturnStatus::Fail);
      }
      else
      {
         mRemoteAddr = addr;
      }

      return status;
   }

   vpr::ReturnStatus read_i(void* buffer, const vpr::Uint32 length,
                            vpr::Uint32& bytesRead,
                            const vpr::Interval timeout = vpr::Interval::NoTimeout);

   vpr::ReturnStatus readn_i(void* buffer, const vpr::Uint32 length,
                             vpr::Uint32& bytesRead,
                             const vpr::Interval timeout = vpr::Interval::NoTimeout);

   vpr::ReturnStatus write_i(const void* buffer, const vpr::Uint32 length,
                             vpr::Uint32& bytesWritten,
                             const vpr::Interval timeout = vpr::Interval::NoTimeout);

   /**
    * Returns the number of bytes available to be read from this socket's
    * receive buffer.
    */
   vpr::Uint32 availableBytes() const
   {
      return PR_Available(mHandle);
   }

   /**
    * Retrieves the value for the given option as set on the socket.
    *
    * @param option The option to be queried.
    * @param data   A data buffer that will be used to store the value of the
    *               given option.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the value for the given
    *         option was retrieved successfully.
    * @return vpr::ReturnStatus::Fail is returned otherwise.
    */
   vpr::ReturnStatus getOption(const vpr::SocketOptions::Types option,
                               vpr::SocketOptions::Data& data) const;

   /**
    * Sets a value for the given option on the socket using the given data
    * block.
    *
    * @param option The option whose value will be set.
    * @param data   A data buffer containing the value to be used in setting
    *               the socket option.
    */
   vpr::ReturnStatus setOption(const vpr::SocketOptions::Types option,
                               const vpr::SocketOptions::Data& data);

   /**
    * Destructor.
    *
    * @post Closes the socket, and deallocates and resources associated with
    *       the socket.
    */
   ~SocketImplNSPR();

protected:
   /**
    * Default constructor.  This just initializes member variables to
    * reasonable defaults.
    *
    * @post The member variables are initialized accordingly to reasonable
    *       defaults.
    *
    * @param sockType The type of socket.
    */
   SocketImplNSPR(const SocketTypes::Type sockType);

   /**
    * Standard constructor.  This takes two vpr::InetAddr objects, a local
    * address and a remote address.
    *
    * @post The member variables are initialized with the given values.
    *
    * @param localAddr  The local address for the socket.
    * @param remoteAddr The remote address for the socket.
    * @param sockType   The type of socket.
    */
   SocketImplNSPR(const InetAddr& localAddr, const InetAddr& remoteAddr,
                  const SocketTypes::Type sockType);

   /**
    * Copy constructor.
    * XXX: We need to have a reference count here
    */
   SocketImplNSPR(const SocketImplNSPR& sock)
      : mName(sock.mName)
      , mHandle(sock.mHandle)
      , mLocalAddr(sock.mLocalAddr)
      , mRemoteAddr(sock.mRemoteAddr)
      , mType(sock.mType)
      , mOpen(sock.mOpen)
      , mBound(sock.mBound)
      , mConnected(sock.mConnected)
      , mOpenBlocking(sock.mOpenBlocking)
      , mBlocking(sock.mBlocking)
      , mBlockingFixed(sock.mBlockingFixed)
   {
   }

   std::string       mName;
   PRFileDesc*       mHandle;     /**< Handle to the socket */
   vpr::InetAddr     mLocalAddr;  /**< The local site's address structure */
   vpr::InetAddr     mRemoteAddr; /**< The remote site's address structure */

   vpr::SocketTypes::Type mType;  /**< Socket type */

   bool mOpen;
   bool mBound;          /**< Is the socket bound to a port yet (connect
                               and bind do this) */
   bool mConnected;
   bool mOpenBlocking;
   bool mBlocking;
   bool mBlockingFixed;
};

} // End of vpr namespace


#endif   /* _VPR_SOCKET_IMPL_NSPR_H_ */
