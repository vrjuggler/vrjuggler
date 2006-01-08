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
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

#include <vpr/vprConfig.h>

#include <boost/concept_check.hpp>

#include <vpr/Util/Assert.h>
#include <vpr/Util/Debug.h>

#include <vpr/md/SIM/Controller.h>
#include <vpr/md/SIM/SocketManager.h>
#include <vpr/md/SIM/IO/Socket/SocketStreamImplSIM.h>


namespace vpr
{

vpr::ReturnStatus SocketStreamImplSIM::listen(const int backlog)
{
   return vpr::sim::Controller::instance()->getSocketManager().listen(this, backlog);
}

vpr::ReturnStatus SocketStreamImplSIM::accept(SocketStreamImplSIM& clientSock,
                                              vpr::Interval timeout)
{
   boost::ignore_unused_variable_warning(timeout);
   vpr::ReturnStatus status;

   mConnectorQueueMutex.acquire();

   if ( ! mConnectorQueue.empty() )
   {
      SocketStreamImplSIM* peer_ptr = mConnectorQueue.front();    // The peer requesting the connection
      mConnectorQueue.pop();

      mConnectorQueueMutex.release();

      vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
         << "SocketStreamImplSIM::accept() [" << mLocalAddr
         << "]: Got pending connector from " << peer_ptr->getLocalAddr()
         << "\n" << vprDEBUG_FLUSH;

      // -- Set known properties of the next socket
      clientSock.mRemoteAddr = peer_ptr->mLocalAddr;    // Get the remote node's address (it must have called bind, so this is final)
      clientSock.mOpen       = true;
      clientSock.mBlocking   = mBlocking;
      clientSock.setConnectState(peer_ptr);

      // Get an address for the new socket, bind it, and attach the socket to
      // the correct node.
      vpr::sim::Controller* controller = vpr::sim::Controller::instance();
      vpr::sim::SocketManager& sock_mgr = controller->getSocketManager();
      clientSock.mLocalAddr  = mLocalAddr;              // Start with local socket's address
      clientSock.mLocalAddr.setPort(0);                 // Clear port so that we get a unique one
      clientSock.bind();                                // Bind to a port (and assign to net node)

      // Now define the route for messages between the two sockets.
      // - Sets the path inside both sockets (path to the other socket)
      sock_mgr.findRoute(peer_ptr, clientSock.getHandle());

      peer_ptr->completeConnection(clientSock.getHandle());

      controller->addConnectionCompletionEvent(controller->getClock().getCurrentTime(),
                                               peer_ptr);

      // Make sure the peer's remote address has the right address.
      // Prior to this point, it has the port of the accepting socket.
      vprASSERT(peer_ptr->mRemoteAddr == clientSock.mLocalAddr && "Connector doesn't know peer's IP address");
   }
   else
   {
      mConnectorQueueMutex.release();
      status.setCode(vpr::ReturnStatus::WouldBlock);
   }

   return status;
}

vpr::ReturnStatus SocketStreamImplSIM::addConnector(vpr::SocketImplSIM* peerSock)
{
   SocketStreamImplSIM* stream_remote;

   vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
      << "SocketStreamImplSIM::addConnector() [" << mLocalAddr
      << "]: Adding connector from " << peerSock->getLocalAddr() << "\n"
      << vprDEBUG_FLUSH;

   stream_remote   = dynamic_cast<SocketStreamImplSIM*>(peerSock);

   vprASSERT(NULL != stream_remote && "Tried to connect to a non-stream socket!");

   mConnectorQueueMutex.acquire();
   {
      mConnectorQueue.push(stream_remote);
   }
   mConnectorQueueMutex.release();

   return vpr::ReturnStatus(vpr::ReturnStatus::InProgress);
}

vpr::ReturnStatus SocketStreamImplSIM::isReadReady() const
{
   vpr::ReturnStatus status(vpr::ReturnStatus::Fail);

   // To be able to read, we must be open, connected, and have a non-empty
   // queue of arrived messages.
   if ( isOpen() && isConnected() && (! mArrivedQueue.empty()) )
   {
      status.setCode(vpr::ReturnStatus::Succeed);
   }

   if ( isOpen() && getConnectorCount() > 0 )
   {
      status.setCode(vpr::ReturnStatus::Succeed);
   }

   return status;
}

vpr::ReturnStatus SocketStreamImplSIM::isWriteReady() const
{
   vpr::ReturnStatus status;

   // We cannot write if we are not open or we not are connected.
   if ( ! isOpen() || ! isConnected() )
   {
      status.setCode(vpr::ReturnStatus::Fail);
   }

   return status;
}

} // End of vpr namespace
