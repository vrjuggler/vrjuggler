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


#ifndef SOCK_CONNECTOR_H
#define SOCK_CONNECTOR_H

#include <vprConfig.h>
#include <IO/Socket/InetAddr.h>
#include <IO/Socket/SocketStream.h>
#include <Utils/Assert.h>


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
  bool connect (SocketStream& newStream,
               const InetAddr& remoteAddr,
               vpr::Uint16 timeout = 0,
               const InetAddr& localAddr = InetAddr::AnyAddr);

  /**
   * Complete a non-blocking connection
   * Try to complete a non-blocking connection.
   */
  bool complete (SocketStream &newStream,
                InetAddr* remoteAddr = NULL,
                vpr::Uint16 timeout = 0);

protected:
  // Make sure we have opened the socket
  // If not, then open it with the given params
  bool checkOpen (SocketStream& newStream)
  {
      if (!newStream.isOpen())
      {
          newStream.open();
      }

      return true;
  }

  
  // Do preconnection rituals
  // - If not bound, then bind to local addr
  // - If timeout == 0, then set nonblocking
  bool connectStart (SocketStream& newStream,
                      const vpr::Uint16 timeout = 0,
                      const InetAddr& localAddr = InetAddr::AnyAddr);
};

bool SocketConnector::connect(SocketStream& newStream, const InetAddr& remoteAddr, 
                              vpr::Uint16 timeout, const InetAddr& localAddr)
{
    //bool ret_val(true);

    if(!checkOpen(newStream))
        return false;

    if(!connectStart(newStream, timeout, localAddr))
        return false;

    newStream.setRemoteAddr(remoteAddr);
    
    // XXX: This needs to return a value once we fix up non-blocking
    newStream.connect();

    InetAddr remote_addr;

    // Finish up connection
    if(timeout > 0)
        if(!complete(newStream, &remote_addr, timeout))
            return false;

    return true;
}

 /**
   * Complete a non-blocking connection
   * Try to complete a non-blocking connection.
   *
   * ARGS: newStream - The connected stream
   *       remoteAddr - returns the address of the remote connection 
   */
bool SocketConnector::complete (SocketStream &newStream, InetAddr* remoteAddr, vpr::Uint16 timeout)
{
    (*remoteAddr) = newStream.getRemoteAddr();
    return true;
}

// Do preconnection rituals
// - If not bound, then bind to local addr
// - If timeout == 0, then set nonblocking
bool SocketConnector::connectStart (SocketStream& newStream,
                  const vpr::Uint16 timeout,
                  const vpr::InetAddr& localAddr)
{
  vprASSERT(newStream.isOpen());
  
  // If timeout is 0, then we are non-blocking
  if(timeout == 0)
      newStream.enableNonBlocking();

  // Check to bind to local addr
  if(localAddr != InetAddr::AnyAddr)
  {
      if(!newStream.setLocalAddr(localAddr))
        return false;
      if(!newStream.bind())
        return false;
  }
  
  return true;
}


}  // namespace vpr

#endif //SOCK_CONNECTOR_H

