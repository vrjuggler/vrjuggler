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

vpr::Uint32 SocketImplSIM::availableBytes ()
{
   vpr::Guard<vpr::Mutex> guard(mArrivedQueueMutex);
   vpr::Uint32 bytes = 0;

   if ( ! mArrivedQueue.empty() )
   {
      std::vector<vpr::sim::MessagePtr>::iterator i = mArrivedQueue.begin();
      bytes = (*i)->getSize();
   }

   return bytes;
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
      if ( ! mArrivedQueue.empty() )
      {
         vpr::sim::MessagePtr message;
         size_t copy_len, msg_size;

         // Remove the message from the arrival queue.
         message = mArrivedQueue[0];

         msg_size = message->getSize();

         // Use the smaller of length and msg_size for the actual amount of
         // data to copy.
         copy_len = (length > msg_size) ? msg_size : length;

         // Complete the read operation.
         memcpy(buffer, message->getBody(), copy_len);
         data_read = copy_len;

         if ( message->resize(copy_len) == 0 )
         {
            mArrivedQueue.erase(mArrivedQueue.begin());
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

/** Exactly like read_i except takes MessageDataPtr directly for zero copy networking
* Updates msgData to point at the new message data.
*/
vpr::ReturnStatus SocketImplSIM::read_i( vpr::sim::Message::MessageDataPtr& msgData,
                          vpr::Uint32& data_read,
                          vpr::Interval timeout = vpr::Interval::NoTimeout )
{
   vpr:: ReturnStatus status;
   vprASSERT(mOpen && "Cannot read on an unopened socket");

   // This is a rather long critical sectino, but we need to be sure that the
   // arrvied queue stays in a consistent state while we are working.
   mArrivedQueueMutex.acquire();
   {
      if ( ! mArrivedQueue.empty() )
      {
         // Get copy of message data, then
         // Remove the message from the arrival queue.
         msgData = mArrivedQueue[0]->getMessageData();
         data_read = msgData->size();
         mArrivedQueue.erase(mArrivedQueue.begin());
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

#ifdef VPR_DEBUG
      vpr::Uint32 remainder;
      const char* msg_ptr = (const char*) buffer;
      remainder = length % 4;

      vprDEBUG(vprDBG_ALL, vprDBG_HEX_LVL)
         << "SocketImplSIM::write_i(): Message (" << length
         << " bytes in 4-byte hex blocks):" << vprDEBUG_FLUSH;

      for ( vpr::Uint32 i = 0; i < length; i += 4 )
      {
         if ( i % 16 == 0 )
         {
            vprDEBUG_NEXT(vprDBG_ALL, vprDBG_HEX_LVL)
               << "\n" << std::hex << std::setfill('0') << std::setw(8)
               << *((vpr::Uint32*) (&msg_ptr[i])) << " " << vprDEBUG_FLUSH;
         }
         else
         {
            vprDEBUG_CONT(vprDBG_ALL, vprDBG_HEX_LVL)
               << std::hex << std::setfill('0') << std::setw(8)
               << *((vpr::Uint32*) (&msg_ptr[i])) << " " << vprDEBUG_FLUSH;
         }
      }

      for ( vpr::Uint32 i = 0; i < remainder; i++ )
      {
         vprDEBUG_CONT(vprDBG_ALL, vprDBG_HEX_LVL) << "00" << vprDEBUG_FLUSH;
      }

      vprDEBUG_NEXT(vprDBG_ALL, vprDBG_HEX_LVL)
         << "\n---------------------------\n" << std::dec << vprDEBUG_FLUSH;
#endif

      vpr::sim::MessagePtr msg(new vpr::sim::Message(buffer, length));
      msg->setPath(mPathToPeer, this, mPeer);
      vpr::sim::Controller::instance()->getSocketManager().sendMessage(msg);
   }

   return status;
}

/** Exactly like write_i except takes MessageDataPtr directly for zero copy networking */
vpr::ReturnStatus SocketImplSIM::write_i( vpr::sim::Message::MessageDataPtr msgData,
                           vpr::Uint32& data_written,
                           vpr::Interval timeout = vpr::Interval::NoTimeout )
{
   vpr::ReturnStatus status;

   if ( mPeer == NULL )
   {
      status.setCode(vpr::ReturnStatus::Fail);
      data_written = 0;
   }
   else
   {
      data_written = msgData->size();
      vpr::sim::MessagePtr msg(new vpr::sim::Message(msgData));
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
