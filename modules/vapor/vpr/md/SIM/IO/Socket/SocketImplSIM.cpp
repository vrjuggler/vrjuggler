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

#include <stdlib.h>
#include <string.h>

#include <vpr/Util/Assert.h>
#include <vpr/Util/Debug.h>
#include <vpr/md/SIM/Controller.h>
#include <vpr/md/SIM/SocketManager.h>
#include <vpr/md/SIM/IO/Socket/SocketImplSIM.h>


namespace vpr
{

SocketImplSIM::~SocketImplSIM ()
{
   if ( mNodeAssigned )
   {
      // Release the node to which we were bound.
      vpr::sim::Controller::instance()->getSocketManager().unbind(this);
      mNodeAssigned = false;
   }

   // cant do this because accept retuns an imp, which gets deleted
   // after constructing the returned Socket_t
/*
   if (this->isOpen())
   {
      std::cout<<"~SocketImplSIM(), closing...\n"<<std::flush;
      this->close();
   }
*/
}

vpr::ReturnStatus SocketImplSIM::close ()
{
   vpr::ReturnStatus status;

   if ( mPeer != NULL )
   {
      // We tell our peer that we are disconnecting.  This is a little
      // different than how real sockets work, but since we communicate
      // directly with our peer, this is a reasonable thing to do.
      mPeer->disconnect();
      mPeer = NULL;
   }

   status = vpr::sim::Controller::instance()->getSocketManager().unbind(this);

   if ( status.success() )
   {
      mNodeAssigned = false;
   }

   mOpen  = false;
   mBound = false;

   return status;
}

vpr::ReturnStatus SocketImplSIM::bind ()
{
   vpr::ReturnStatus status;

   if ( vpr::InetAddr::AnyAddr == mLocalAddr )
   {
      status =
         vpr::sim::Controller::instance()->getSocketManager().bindUnusedPort(this,
                                                                             mLocalAddr);
      vprASSERT(status.success() && "Failed to assign port number to socket");
   }

   vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
      << "SocketImplSIM::bind(): Assigning socket to a node in the graph\n"
      << vprDEBUG_FLUSH;
   status =
      vpr::sim::Controller::instance()->getSocketManager().assignToNode(this,
                                                                        mLocalAddr);
   vprASSERT(status.success() && "Failed to assign socket to a node");

   status = vpr::sim::Controller::instance()->getSocketManager().bind(this, mLocalAddr);
   mBound = status.success();

   return status;
}

vpr::ReturnStatus SocketImplSIM::connect (vpr::Interval timeout)
{
   vpr::ReturnStatus status;
   vprASSERT(mOpen && "An unopened socket cannot connect");

   if ( ! mNodeAssigned )
   {
      if ( vpr::InetAddr::AnyAddr == mLocalAddr )
      {
         status =
            vpr::sim::Controller::instance()->getSocketManager().bindUnusedPort(this,
                                                                                mLocalAddr);
         vprASSERT(status.success() && "Failed to assign port number to socket");
      }

      vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
         << "SocketImplSIM::connect(): Assigning socket to a node in the "
         << "graph\n" << vprDEBUG_FLUSH;
      status =
         vpr::sim::Controller::instance()->getSocketManager().assignToNode(this,
                                                                           mLocalAddr);
   }

   vprASSERT(mNodeAssigned && "A node-less socket cannot connect");
   status =
      vpr::sim::Controller::instance()->getSocketManager().connect(this, &mPeer,
                                                                   mRemoteAddr,
                                                                   mPathToPeer,
                                                                   timeout);
   mBound = mConnected = status.success();

   // Now that we are connected (or queued to get connected), we do not have
   // to manipulate our local address as is done in the real VPR sockets.
   // In a real OS, the local address would be set automatically, so that's
   // what the Sim Socket Manager does.

   return status;
}

vpr::ReturnStatus SocketImplSIM::read_i (void* buffer,
                                         const vpr::Uint32 length,
                                         vpr::Uint32& data_read,
                                         vpr::Interval timeout )
{
   vpr:: ReturnStatus status;
   vprASSERT(mOpen && "Cannot read on an unopened socket");

   // This is a rather long critical sectino, but we need to be sure that the
   // arrvied queue stays in a consistent state while we are working.
   mArrivedQueueMutex.acquire();
   {
      if ( mArrivedQueue.size() > 0 )
      {
         vpr::sim::MessagePtr message;
         size_t copy_len, msg_size;

         // Remove the message from the arrival queue.
         message = mArrivedQueue.front();

         msg_size = message->getSize();

         // Use the smaller of length and msg_size for the actual amount of
         // data to copy.
         copy_len = (length > msg_size) ? msg_size : length;

         // Complete the read operation.
         memcpy(buffer, message->getBody(), copy_len);
         data_read = copy_len;

         if ( message->resize(copy_len) == 0 )
         {
            mArrivedQueue.pop();
//            delete message;
         }
      }
      // Nothing is in the queue, so we tell the caller that the operation is
      // in progress.
      else
      {
         status.setCode(vpr::ReturnStatus::WouldBlock);
         data_read = 0;
      }
   }
   mArrivedQueueMutex.release();

   return status;
}

vpr::ReturnStatus SocketImplSIM::write_i (const void* buffer,
                                          const vpr::Uint32 length,
                                          vpr::Uint32& data_written,
                                          vpr::Interval timeout)
{
   vpr::ReturnStatus status;

   if ( mPeer == NULL )
   {
      status.setCode(vpr::ReturnStatus::Fail);
      data_written = 0;
   }
   else
   {
      data_written = length;

      vprDEBUG(vprDBG_ALL, vprDBG_HEX_LVL)
         << "Sending message from " << mLocalAddr << " to " << mRemoteAddr
         << ".  Message (size " << length << "): " << std::hex << buffer
         << std::dec << std::endl << vprDEBUG_FLUSH;

      vpr::sim::MessagePtr msg(new vpr::sim::Message(buffer, length));
      msg->setPath(mPathToPeer, this, mPeer);
      vpr::sim::Controller::instance()->getSocketManager().sendMessage(msg);
   }

   return status;
}

vpr::ReturnStatus SocketImplSIM::getOption (const vpr::SocketOptions::Types option,
                                            struct vpr::SocketOptions::Data& data)
{
   return vpr::ReturnStatus();
}

vpr::ReturnStatus SocketImplSIM::setOption (const vpr::SocketOptions::Types option,
                                            const struct vpr::SocketOptions::Data& data)
{
   return vpr::ReturnStatus();
}

vpr::ReturnStatus SocketImplSIM::isReadReady (const vpr::Interval timeout)
{
   vpr::ReturnStatus status;

   if ( vpr::Interval::NoWait == timeout )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << "Timeouts not supported with sim sockets yet!\n" << vprDEBUG_FLUSH;
   }

//   if ( vpr::Interval::NoWait == timeout )
//   {
      if ( ! mOpen || NULL == mPeer || mArrivedQueue.size() == 0 )
      {
         status.setCode(vpr::ReturnStatus::Fail);
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

vpr::ReturnStatus SocketImplSIM::isWriteReady (const vpr::Interval timeout)
{
   vpr::ReturnStatus status;

   if ( vpr::Interval::NoWait == timeout )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << "Timeouts not supported with sim sockets yet!\n" << vprDEBUG_FLUSH;
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

vpr::ReturnStatus SocketImplSIM::inExceptState ()
{
   // XXX: For now, we never go into an exceptional state.
   return vpr::ReturnStatus(vpr::ReturnStatus::Fail);
}

// ============================================================================
// Protected methods.
// ============================================================================

SocketImplSIM::SocketImplSIM (const vpr::SocketTypes::Type sock_type)
   : mOpen(false), mBound(false), mConnected(false), mOpenBlocking(false),
     mBlocking(false), mType(sock_type), mReuseAddr(false), mPeer(NULL),
     mNodeAssigned(false)
{
   /* Do nothing. */ ;
//   vpr::ReturnStatus status;
//   status = vpr::sim::Controller::instance()->getSocketManager().assignToNode(this);
//
//   vprASSERT(status.success() && "Failed to assign new socket to a node");
}

SocketImplSIM::SocketImplSIM (const vpr::InetAddr& local_addr,
                              const vpr::InetAddr& remote_addr,
                              const vpr::SocketTypes::Type sock_type)
   : mOpen(false), mBound(false), mConnected(false), mOpenBlocking(false),
     mBlocking(false), mLocalAddr(local_addr), mRemoteAddr(remote_addr),
     mType(sock_type), mPeer(NULL), mNodeAssigned(false)
{
   /* Do nothing. */ ;
//   vpr::ReturnStatus status;
//   status = vpr::sim::Controller::instance()->getSocketManager().assignToNode(this, local_addr);
//
//   vprASSERT(status.success() && "Failed to assign new socket to a node");
}

void SocketImplSIM::disconnect ()
{
   // XXX: This is potentially not the best way to disconnect, but it's the
   // best I have come up with so far.
   mPeer = NULL;
}

} // End of vpr namespace
