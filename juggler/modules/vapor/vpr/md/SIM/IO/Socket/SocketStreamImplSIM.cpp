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
      peer_remote_ptr = mConnectorQueue.front().second;
      mConnectorQueue.pop();

      mConnectorQueueMutex.release();

      vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
         << "SocketStreamImplSIM::accept() [" << mLocalAddr
         << "]: Got pending connector from " << peer_ptr->getLocalAddr()
         << "\n" << vprDEBUG_FLUSH;

      client_sock.mRemoteAddr = peer_ptr->mLocalAddr;
      client_sock.mOpen       = true;
      client_sock.mBlocking   = mBlocking;
      client_sock.mType       = vpr::SocketTypes::STREAM;
      client_sock.mPeer       = peer_ptr;

      // Get an address for the new socket, bind it, and attach the socket to
      // the correct node.
      vpr::sim::SocketManager& sock_mgr =
         vpr::sim::Controller::instance()->getSocketManager();
      sock_mgr.bindUnusedPort(client_sock.getHandle(), client_sock.mLocalAddr);
      sock_mgr.assignToNode(client_sock.getHandle(), client_sock.mLocalAddr);

      // Now define the route for messages between the two sockets.
      sock_mgr.findRoute(peer_ptr, client_sock.getHandle());

      // Finally, tell the connecting socket its peer.  It's okay to do this
      // cast since getHandle() returns 'this'.
      *peer_remote_ptr = (SocketStreamImplSIM*) client_sock.getHandle();

      // Make sure the peer's remote address has the right port number.
      // Prior to this point, it has the port of the accepting socket.
      peer_ptr->mRemoteAddr.setPort(client_sock.mLocalAddr.getPort());
   }
   else
   {
      mConnectorQueueMutex.release();
      status.setCode(vpr::ReturnStatus::WouldBlock);
   }

   return status;
}

vpr::ReturnStatus SocketStreamImplSIM::addConnector ( const vpr::SocketImplSIM* local,
                                                      vpr::SocketImplSIM** remote )
{
   vpr::SocketImplSIM* non_const_local;
   SocketStreamImplSIM* stream_local;
   SocketStreamImplSIM** stream_remote;

   vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
      << "SocketStreamImplSIM::addConnector() [" << mLocalAddr
      << "]: Adding connector from " << local->getLocalAddr() << "\n"
      << vprDEBUG_FLUSH;

   non_const_local = const_cast<vpr::SocketImplSIM*>(local);
   stream_local    = dynamic_cast<SocketStreamImplSIM*>(non_const_local);
   stream_remote   = (SocketStreamImplSIM**) (remote);

   vprASSERT(NULL != stream_local && "Tried to connect from a non-stream socket!");
   vprASSERT(NULL != stream_remote && "Tried to connect to a non-stream socket!");

   mConnectorQueueMutex.acquire();
   {
      mConnectorQueue.push(queue_obj_t(stream_local, stream_remote));
   }
   mConnectorQueueMutex.release();

   return vpr::ReturnStatus(vpr::ReturnStatus::InProgress);
}

vpr::ReturnStatus SocketStreamImplSIM::isReadReady (const vpr::Interval timeout)
   const
{
   vpr::ReturnStatus status(vpr::ReturnStatus::Fail);

   if ( vpr::Interval::NoWait == timeout )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << "SocketStreamImplSIM::isReadReady(): Timeouts not supported "
         << "with sim sockets--yet\n" << vprDEBUG_FLUSH;
   }

//   if ( vpr::Interval::NoWait == timeout )
//   {
      if ( mOpen && NULL != mPeer && (! mArrivedQueue.empty()) )
      {
         status.setCode(vpr::ReturnStatus::Succeed);
      }

      if ( mOpen && getConnectorCount() > 0 )
      {
         status.setCode(vpr::ReturnStatus::Succeed);
      }
//   }
/*
   XXX: Will there be a way to handle waiting until the timeout expires?
   else
   {
   }
*/

   return status;
}

vpr::ReturnStatus SocketStreamImplSIM::isWriteReady (const vpr::Interval timeout)
   const
{
   vpr::ReturnStatus status;

   if ( vpr::Interval::NoWait == timeout )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << "SocketStreamImplSIM::isWriteReady(): Timeouts not supported with "
         << "sim sockets--yet\n" << vprDEBUG_FLUSH;
   }

//   if ( vpr::Interval::NoWait == timeout )
//   {
      if ( ! mOpen || NULL == mPeer )
      {
         status.setCode(vpr::ReturnStatus::Fail);
      }
//   }
/*
   XXX: Will there be a way to handle waiting until the timeout expires?
   else
   {
   }
*/

   return status;
}

} // End of vpr namespace
