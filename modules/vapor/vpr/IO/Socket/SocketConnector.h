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

#ifndef _VPR_SOCK_CONNECTOR_H_
#define _VPR_SOCK_CONNECTOR_H_

#include <vpr/vprConfig.h>
#include <vpr/IO/Socket/InetAddr.h>
#include <vpr/IO/Socket/SocketStream.h>
#include <vpr/IO/Selector.h>
#include <vpr/Util/Assert.h>
#include <vpr/Util/ReturnStatus.h>
#include <vpr/Util/Debug.h>


namespace vpr
{

/**
 * @class SocketConnector
 *
 * Defines a factory for creating new connections both synchronously and asynchronously
 *
 * The socket factory creates a new socket for the user that connects
 * to the given remote destination.  It allows for synchronous and async connection
 * establishment
 *
 * This means that there is no state associated with the connector
 */
class SocketConnector
{
public:
  SocketConnector()
  {;}

  ~SocketConnector()
  {;}

  /**
   * Actively connect to the remote address and set the newStream
   * to the new connection.
   *
   * ARGS: new_stream - The stream to connect
   *       remoteAddr - The remote address to connect to
   *       timeout    - The num msecs to wait (0 - NonBlocking)
   */
  inline vpr::ReturnStatus connect (vpr::SocketStream& newStream,
                              const vpr::InetAddr& remoteAddr,
                              vpr::Interval timeout = vpr::Interval::NoTimeout,
                              const vpr::InetAddr& localAddr = vpr::InetAddr::AnyAddr);

  /**
   * Complete a non-blocking connection
   * Try to complete a non-blocking connection.
   */
  inline vpr::ReturnStatus complete (vpr::SocketStream &newStream,
                               const vpr::Interval timeout = vpr::Interval::NoTimeout);

protected:
  // Make sure we have opened the socket
  // If not, then open it with the given params
  bool checkOpen (SocketStream& newStream)
  {
      vpr::ReturnStatus status;

      if (!newStream.isOpen())
      {
          status = newStream.open();

          if(!status.success())
          {
             vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "vpr::Connector:CheckOpen: Failed to open socket\n" << vprDEBUG_FLUSH;
          }
      }

      return status.success();
  }


  // Do preconnection rituals
  // - If not bound, then bind to local addr
  // - If timeout == NoWait, then try to set nonblocking
  inline bool connectStart (vpr::SocketStream& newStream,
                            vpr::Interval timeout = vpr::Interval::NoTimeout,
                            const vpr::InetAddr& localAddr = vpr::InetAddr::AnyAddr);
};

inline vpr::ReturnStatus SocketConnector::connect(SocketStream& newStream,
                                            const vpr::InetAddr& remoteAddr,
                                            vpr::Interval timeout,
                                            const vpr::InetAddr& localAddr)
{
    vpr::ReturnStatus ret_val;
    //vpr::InetAddr remote_addr;

    // Open the socket
    if(!checkOpen(newStream))
        return vpr::ReturnStatus(vpr::ReturnStatus::Fail);

    if ( localAddr != vpr::InetAddr::AnyAddr )
    {
       vpr::ReturnStatus status;
       newStream.setLocalAddr(localAddr);
       status = newStream.bind();
       vprASSERT(status.success() && "Failed to bind local address");
    }

    // Start the connection
    if(!connectStart(newStream, timeout, localAddr))
        return vpr::ReturnStatus(vpr::ReturnStatus::Fail);

    newStream.setRemoteAddr(remoteAddr);

    // Attempt the connection
    ret_val = newStream.connect(timeout);

    /*
    // If the connect call did not return success, it may be the result of
    // using non-blocking sockets.
    if ( ! ret_val.success() )
    {
       // If connect() gave us a status saying that the connection is in
       // progress, try to complete the connection after the timeout period.
       // If there is no timeout period, simply return immediately.
       if ( ret_val == vpr::ReturnStatus::InProgress ||
            ret_val == vpr::ReturnStatus::WouldBlock )
       {
          if ( timeout != vpr::Interval::NoWait ) {
             ret_val = complete(newStream, &remote_addr, timeout);
          }
       }
    }
    // Finish up successful connection.
    else if(vpr::Interval::NoWait != timeout) {
       ret_val = complete(newStream, &remote_addr, timeout);
    }
    */

    /*
    ** Since complete doesn't do anything really we don't need this
    if(ret_val.success())
    {
       ret_val = complete(newStream, timeout);
    }
    */

    return ret_val;
}

 /**
   * Complete a non-blocking connection
   * Try to complete a non-blocking connection.
   *
   * @pre
   * @param newStream  The connected stream.
   * @param remoteAddr returns the address of the remote connection.
   */
inline vpr::ReturnStatus SocketConnector::complete (SocketStream &newStream,
                                              const vpr::Interval timeout)
{
   vpr::ReturnStatus status;

   if( newStream.isConnected() )
   {
      // XXX: Should this actually be a failure
      return vpr::ReturnStatus::Succeed;
   }

   // If non-blocking, then we can only wait as long as the timeout
   if ( newStream.getNonBlocking() )
   {
      vpr::IOSys::Handle handle;
      vpr::Selector selector;
      vpr::Uint16 num_events;

      // Use the selector to be informed when the SocketStream object is ready
      // to be used.  That is, when the object is connected.
      handle = newStream.getHandle();
      selector.addHandle(handle);
      selector.setIn(handle, vpr::Selector::Read | vpr::Selector::Write);
      status = selector.select(num_events, timeout);

      // If the selector told us that our handle is ready, we are successfully
      // connected.
      if ( selector.getOut(handle) & (vpr::Selector::Read | vpr::Selector::Write) )
      {
         status = vpr::ReturnStatus::Succeed;

         /*
         if ( remoteAddr != NULL ) {
            (*remoteAddr) = newStream.getRemoteAddr();
         }
         */
      }
      // else Use the status from the selector
   }
   else     // Not a non-blocking socket
   {
      vprASSERT(false && "Should not call complete on a non-blocking socket");
      /*
      if ( remoteAddr != NULL ) {
         (*remoteAddr) = newStream.getRemoteAddr();
      }
      */
   }

   return status;
}

// Do preconnection rituals
// - If not bound, then bind to local addr
// - If timeout == 0, then set nonblocking
inline bool SocketConnector::connectStart (SocketStream& newStream,
                                           vpr::Interval timeout,
                                           const vpr::InetAddr& localAddr)
{
   vprASSERT(newStream.isOpen());

   // If timeout is 0, then we are non-blocking
   if(vpr::Interval::NoWait == timeout)
   {
      newStream.enableNonBlocking();
   }

  // Check to bind to local addr
   if(localAddr != InetAddr::AnyAddr)
   {
      if(!newStream.setLocalAddr(localAddr).success())
         return false;

      if(!newStream.bind().success())
        return false;
   }

   return true;
}

}  // namespace vpr


#endif /* _VPR_SOCK_CONNECTOR_H_ */
