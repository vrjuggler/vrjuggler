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

#include <vpr/vprConfig.h>

#include <vpr/Util/Assert.h>

#include <vpr/md/SIM/Controller.h>
#include <vpr/md/SIM/SocketManager.h>
#include <vpr/md/SIM/IO/Socket/SocketStreamImplSIM.h>


namespace vpr
{

vpr::ReturnStatus SocketStreamImplSIM::listen( const int backlog )
{
   return vpr::sim::Controller::instance()->getSocketManager().listen(this, backlog);
}

vpr::ReturnStatus SocketStreamImplSIM::accept( SocketStreamImplSIM& client_sock,
                                               vpr::Interval timeout )
{
   vpr::ReturnStatus status;

   mConnectorQueueMutex.acquire();

   if ( ! mConnectorQueue.empty() )
   {
      SocketStreamImplSIM* peer_ptr;
      SocketStreamImplSIM** peer_remote_ptr;

      peer_ptr        = mConnectorQueue.front().first;
      vprASSERT(peer_ptr == this);

      peer_remote_ptr = mConnectorQueue.front().second;
      mConnectorQueue.pop();

      mConnectorQueueMutex.release();

      vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
         << "SocketStreamImplSIM::accept() [" << mLocalAddr
         << "]: Got pending connector from " << this->getLocalAddr()
         << "\n" << vprDEBUG_FLUSH;

      // -- Set known properties of the next socket
      client_sock.mRemoteAddr = this->mLocalAddr;    // Get the remote node's address (it must have called bind, so this is final)
      client_sock.mOpen       = true;
      client_sock.mBlocking   = mBlocking;
      client_sock.mPeer       = this;

      // Get an address for the new socket, bind it, and attach the socket to
      // the correct node.
      vpr::sim::SocketManager& sock_mgr =
         vpr::sim::Controller::instance()->getSocketManager();
      client_sock.mLocalAddr  = mLocalAddr;              // Start with local socket's address
      client_sock.mLocalAddr.setPort(0);                 // Clear port so that we get a unique one
      client_sock.bind();                                // Bind to a port (and assign to net node)
            
      // Now define the route for messages between the two sockets.
      sock_mgr.findRoute(this, client_sock.getHandle());

      // Finally, tell the connecting socket its peer.  It's okay to do this
      // cast since getHandle() returns 'this'.
      *peer_remote_ptr = (SocketStreamImplSIM*) client_sock.getHandle();

      // Make sure the peer's remote address has the right port number.
      // Prior to this point, it has the port of the accepting socket.
      vprASSERT(this->mRemoteAddr.getAddressValue() == client_sock.mLocalAddr.getAddressValue() && "Connector doesn't know peer's IP address");
      peer_ptr->mRemoteAddr.setPort(client_sock.mLocalAddr.getPort());
   }
   else
   {
      mConnectorQueueMutex.release();
      status.setCode(vpr::ReturnStatus::WouldBlock);
   }

   return status;
}

vpr::ReturnStatus SocketStreamImplSIM::addConnector ( vpr::SocketImplSIM* local,
                                                      vpr::SocketImplSIM** remote )
{
   vprASSERT( local == this );         // XXX: If this passes, then remove the "local" parameter all together

   SocketStreamImplSIM** stream_remote;

   vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
      << "SocketStreamImplSIM::addConnector() [" << mLocalAddr
      << "]: Adding connector from " << local->getLocalAddr() << "\n"
      << vprDEBUG_FLUSH;

   stream_remote   = (SocketStreamImplSIM**) (remote);

   vprASSERT(NULL != stream_remote && "Tried to connect to a non-stream socket!");

   mConnectorQueueMutex.acquire();
   {
      mConnectorQueue.push(queue_obj_t(this, stream_remote));
   }
   mConnectorQueueMutex.release();

   return vpr::ReturnStatus(vpr::ReturnStatus::InProgress);
}

vpr::ReturnStatus SocketStreamImplSIM::isReadReady () const
{
   vpr::ReturnStatus status(vpr::ReturnStatus::Fail);

   if ( mOpen && (NULL != mPeer) && (! mArrivedQueue.empty()) )
   {
      status.setCode(vpr::ReturnStatus::Succeed);
   }

   if ( mOpen && getConnectorCount() > 0 )
   {
      status.setCode(vpr::ReturnStatus::Succeed);
   }

   return status;
}

vpr::ReturnStatus SocketStreamImplSIM::isWriteReady () const
{
   vpr::ReturnStatus status;

   if ( !mOpen || (NULL == mPeer) )
   {
      status.setCode(vpr::ReturnStatus::Fail);
   }

   return status;
}

} // End of vpr namespace
