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
#include <vpr/Util/Status.h>


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
class VPR_CLASS_API SocketConnector
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
  Status connect (SocketStream& newStream,
               const InetAddr& remoteAddr,
               vpr::Interval timeout = vpr::Interval::NoTimeout,
               const InetAddr& localAddr = InetAddr::AnyAddr);

  /**
   * Complete a non-blocking connection
   * Try to complete a non-blocking connection.
   */
  Status complete (SocketStream &newStream,
                InetAddr* remoteAddr = NULL,
                const vpr::Interval timeout = vpr::Interval::NoTimeout);

protected:
  // Make sure we have opened the socket
  // If not, then open it with the given params
  bool checkOpen (SocketStream& newStream)
  {
      vpr::Status status;

      if (!newStream.isOpen())
      {
          status = newStream.open();
      }

      return status.success();
  }


  // Do preconnection rituals
  // - If not bound, then bind to local addr
  // - If timeout == 0, then set nonblocking
  bool connectStart (SocketStream& newStream,
                      vpr::Interval timeout = vpr::Interval::NoTimeout,
                      const InetAddr& localAddr = InetAddr::AnyAddr);
};

inline vpr::Status SocketConnector::connect(SocketStream& newStream,
                                            const vpr::InetAddr& remoteAddr,
                                            vpr::Interval timeout,
                                            const vpr::InetAddr& localAddr)
{
    vpr::Status ret_val;
    vpr::InetAddr remote_addr;

    if(!checkOpen(newStream))
        return vpr::Status(vpr::Status::Failure);

    if(!connectStart(newStream, timeout, localAddr))
        return vpr::Status(vpr::Status::Failure);

    newStream.setRemoteAddr(remoteAddr);

    ret_val = newStream.connect();

    // If the connect call did not return success, it may be the result of
    // using non-blocking sockets.
    if ( ! ret_val.success() ) {
       // If connect() gave us a status saying that the connection is in
       // progress, try to complete the connection after the timeout period.
       // If there is no timeout period, simply return immediately.
       if ( ret_val == vpr::Status::InProgress ||
            ret_val == vpr::Status::WouldBlock )
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

    return ret_val;
}

 /**
   * Complete a non-blocking connection
   * Try to complete a non-blocking connection.
   *
   * @param newStream  The connected stream.
   * @param remoteAddr returns the address of the remote connection.
   */
inline vpr::Status SocketConnector::complete (SocketStream &newStream,
                                              vpr::InetAddr* remoteAddr,
                                              const vpr::Interval timeout)
{
   vpr::Status status;

   // If this is a non-blocking socket, we have some extra work to do to
   // complete the connection and complete the creation of the connector.
   if ( newStream.getNonBlocking() ) {
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
      if ( num_events == 1 ) {
         if ( remoteAddr != NULL ) {
            (*remoteAddr) = newStream.getRemoteAddr();
         }
      }
   }
   else {
      if ( remoteAddr != NULL ) {
         (*remoteAddr) = newStream.getRemoteAddr();
      }
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
      vprASSERT(false && "vpr does not support non-blocking connections yet");
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
