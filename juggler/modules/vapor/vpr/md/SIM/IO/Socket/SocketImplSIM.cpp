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

static const vpr::Uint32 SOCK_MAX_BUFFER_SIZE = 65536;

SocketImplSIM::~SocketImplSIM()
{
vpr::DebugOutputGuard dbg_output(vprDBG_ALL, vprDBG_STATE_LVL,
                                 std::string("SocketImplSIM destructor: Deleting socket\n"),
                                 std::string("Done deleting socket.\n"));

   if ( mOpen )
   {
      close();
   }

   vprDEBUG(vprDBG_ALL, vprDBG_HEX_LVL)
      << "Deleting memory at address " << std::hex << this << std::dec
      << std::endl << vprDEBUG_FLUSH;
}

vpr::ReturnStatus SocketImplSIM::close()
{
vpr::DebugOutputGuard dbg_output(vprDBG_ALL, vprDBG_STATE_LVL,
                                 std::string("SocketImplSIM::close(): Closing socket ...\n"),
                                 std::string("Done closing socket.\n"));
   vpr::ReturnStatus status;

   vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
      << "SocketImplSIM::close: " << mLocalAddr << std::endl << vprDEBUG_FLUSH;

   if ( isConnected() )
   {
      vprASSERT(mPeer != NULL && "I am connected to a NULL peer");

      // We tell our peer that we are disconnecting.  This is a little
      // different than how real sockets work, but since we communicate
      // directly with our peer, this is a reasonable thing to do.
      mPeer->disconnect();
      disconnect();
      vpr::sim::Controller::instance()->flushPath(this, mPathToPeer);
   }

   vprASSERT(mPeer == NULL && "I should not have a peer at this point");

   if ( mBound )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
         << "SocketImplSIM::close: Unbinding: " << mLocalAddr << std::endl
         << vprDEBUG_FLUSH;
      // Release the node to which we were bound.
      status = vpr::sim::Controller::instance()->getSocketManager().unbind(this);
      mBound = false;
   }

   mOpen = false;

   return status;
}

vpr::ReturnStatus SocketImplSIM::bind()
{
   vpr::ReturnStatus status;
   vpr::sim::SocketManager& sock_mgr =
      vpr::sim::Controller::instance()->getSocketManager();

   status = sock_mgr.bind(this);
   mBound = status.success();

   return status;
}

vpr::ReturnStatus SocketImplSIM::connect(vpr::Interval timeout)
{
   vpr::ReturnStatus status;
   vprASSERT(mOpen && "An unopened socket cannot connect");
   vpr::sim::SocketManager& sock_mgr =
      vpr::sim::Controller::instance()->getSocketManager();

   // If not bound, then bind us
   if (!mBound)
   {
      bind();
   }

   vprASSERT(mBound && "We must be bound first");

   status = sock_mgr.connect(this, mRemoteAddr, mPathToPeer, timeout);
   // NOTE: We are not connected until the other side says so

   return status;
}

// Complete the connection
// - Set the peer address and ptr
// - Set to connected state.
vpr::ReturnStatus SocketImplSIM::completeConnection(SocketImplSIM* connectedPeer)
{
   setConnectState(connectedPeer);
   mRemoteAddr = connectedPeer->getLocalAddr();

   return vpr::ReturnStatus::Succeed;
}

vpr::ReturnStatus SocketImplSIM::setLocalAddr(const vpr::InetAddr& addr)
{
   vpr::ReturnStatus status;

   if ( mBound )
   {
      vprASSERT(false && "Can't change address of a bound socket");
      status.setCode(vpr::ReturnStatus::Fail);
   }
   else
   {
      vprDEBUG(vprDBG_ALL, vprDBG_HVERB_LVL)
         << "SocketImplSIM::setLocalAddr(): Changing local address from "
         << mLocalAddr << " to " << addr << std::endl << vprDEBUG_FLUSH;
      mLocalAddr = addr;
   }

   return status;
}

vpr::ReturnStatus SocketImplSIM::setRemoteAddr(const vpr::InetAddr& addr)
{
   vpr::ReturnStatus status;

   // If we are connected, we cannot change our remote address.
   if ( mConnected )
   {
      status.setCode(vpr::ReturnStatus::Fail);
   }
   else
   {
      vprDEBUG(vprDBG_ALL, vprDBG_HVERB_LVL)
         << "SocketImplSIM::setLocalAddr(): Changing remote address from "
         << mRemoteAddr << " to " << addr << std::endl << vprDEBUG_FLUSH;
      mRemoteAddr = addr;
   }

   return status;
}

vpr::Uint32 SocketImplSIM::availableBytes()
{
   vprASSERT(mBound && "We must be bound first");

   vpr::Guard<vpr::Mutex> guard(mArrivedQueueMutex);
   vpr::Uint32 bytes = 0;

   if ( ! mArrivedQueue.empty() )
   {
      bytes = mArrivedQueue.front()->getSize();
   }

   return bytes;
}

vpr::ReturnStatus SocketImplSIM::read_i(void* buffer,
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
         message = mArrivedQueue.front();

         msg_size = message->getSize();

         // Use the smaller of length and msg_size for the actual amount of
         // data to copy.
         copy_len = (length > msg_size) ? msg_size : length;

         // Complete the read operation.
         memcpy(buffer, message->getBody(), copy_len);
         data_read = copy_len;

         // If there was no resizing performed on the message (the resize value
         // is 0), then we have read the entire message into our buffer.
         if ( message->resize(copy_len) == 0 )
         {
            mArrivedQueue.pop_front();
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
vpr::ReturnStatus SocketImplSIM::read_i(vpr::sim::Message::MessageDataPtr& msgData,
                                        vpr::Uint32& data_read,
                                        vpr::Interval timeout)
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
         msgData = mArrivedQueue.front()->getMessageData();
         data_read = msgData->size();
         mArrivedQueue.pop_front();
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

vpr::ReturnStatus SocketImplSIM::write_i(const void* buffer,
                                         const vpr::Uint32 length,
                                         vpr::Uint32& data_written,
                                         vpr::Interval timeout)
{
   vpr::ReturnStatus status;
   vprASSERT(mBound && "We must be bound first");
   vprASSERT(mOpen && "We must be open first");

   if ( mPeer == NULL )
   {
      vprASSERT(false && "Trying to write to NULL peer");      // XXX: This may not be a good way to do this
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

      vprASSERT(!mPathToPeer->empty() && "Path not set");
      vpr::sim::MessagePtr msg(new vpr::sim::Message(buffer, length));
      msg->setPath(mPathToPeer, this, mPeer);
      vpr::sim::Controller::instance()->getSocketManager().sendMessage(msg);
   }

   return status;
}

/** Exactly like write_i except takes MessageDataPtr directly for zero copy networking */
vpr::ReturnStatus SocketImplSIM::write_i(vpr::sim::Message::MessageDataPtr msgData,
                                         vpr::Uint32& data_written,
                                         vpr::Interval timeout)
{
   vprASSERT(mBound && "We must be bound first");
   vprASSERT(mOpen && "We must be open first");

   vpr::ReturnStatus status;

   if ( mPeer == NULL )
   {
      vprASSERT(false && "Trying to write to NULL peer");      // XXX: This may not be a good way to do this
      status.setCode(vpr::ReturnStatus::Fail);
      data_written = 0;
   }
   else
   {
      vprASSERT(!mPathToPeer->empty() && "Path not set");
      data_written = msgData->size();
      vpr::sim::MessagePtr msg(new vpr::sim::Message(msgData));
      msg->setPath(mPathToPeer, this, mPeer);
      vpr::sim::Controller::instance()->getSocketManager().sendMessage(msg);
   }

   return status;
}


vpr::ReturnStatus SocketImplSIM::getOption(const vpr::SocketOptions::Types option,
                                           struct vpr::SocketOptions::Data& data)
{
   vpr::ReturnStatus status;

   switch ( option )
   {
      // Socket-level options.
      case vpr::SocketOptions::Linger:
         status.setCode(vpr::ReturnStatus::Fail);
         break;
      case vpr::SocketOptions::ReuseAddr:
         data.reuse_addr = mReuseAddr;
         break;
      case vpr::SocketOptions::KeepAlive:
         status.setCode(vpr::ReturnStatus::Fail);
         break;
      case vpr::SocketOptions::RecvBufferSize:
         data.recv_buffer_size = SOCK_MAX_BUFFER_SIZE;
         break;
      case vpr::SocketOptions::SendBufferSize:
         data.send_buffer_size = SOCK_MAX_BUFFER_SIZE;
         break;

      // IP-level options.
      case vpr::SocketOptions::IpTimeToLive:
         status.setCode(vpr::ReturnStatus::Fail);
         break;
      case vpr::SocketOptions::IpTypeOfService:
         data.type_of_service = vpr::SocketOptions::LowDelay;
         break;
      case vpr::SocketOptions::AddMember:
         status.setCode(vpr::ReturnStatus::Fail);
         break;
      case vpr::SocketOptions::DropMember:
         status.setCode(vpr::ReturnStatus::Fail);
         break;
      case vpr::SocketOptions::McastInterface:
         status.setCode(vpr::ReturnStatus::Fail);
         break;
      case vpr::SocketOptions::McastTimeToLive:
         status.setCode(vpr::ReturnStatus::Fail);
         break;
      case vpr::SocketOptions::McastLoopback:
         status.setCode(vpr::ReturnStatus::Fail);
         break;

      // TCP-level options.
      case vpr::SocketOptions::NoDelay:
         status.setCode(vpr::ReturnStatus::Fail);
         break;
      case vpr::SocketOptions::MaxSegment:
         status.setCode(vpr::ReturnStatus::Fail);
         break;
    }

   return status;
}

vpr::ReturnStatus SocketImplSIM::setOption(const vpr::SocketOptions::Types option,
                                           const struct vpr::SocketOptions::Data& data)
{
   vpr::ReturnStatus status;

   switch ( option )
   {
      // Socket-level options.
      case vpr::SocketOptions::Linger:
         status.setCode(vpr::ReturnStatus::Fail);
         break;
      case vpr::SocketOptions::ReuseAddr:
         mReuseAddr = data.reuse_addr;
         break;
      case vpr::SocketOptions::KeepAlive:
         status.setCode(vpr::ReturnStatus::Fail);
         break;
      case vpr::SocketOptions::RecvBufferSize:
         status.setCode(vpr::ReturnStatus::Fail);
         break;
      case vpr::SocketOptions::SendBufferSize:
         status.setCode(vpr::ReturnStatus::Fail);
         break;

      // IP-level options.
      case vpr::SocketOptions::IpTimeToLive:
         status.setCode(vpr::ReturnStatus::Fail);
         break;
      case vpr::SocketOptions::IpTypeOfService:
         status.setCode(vpr::ReturnStatus::Fail);
         break;
      case vpr::SocketOptions::AddMember:
         status.setCode(vpr::ReturnStatus::Fail);
         break;
      case vpr::SocketOptions::DropMember:
         status.setCode(vpr::ReturnStatus::Fail);
         break;
      case vpr::SocketOptions::McastInterface:
         status.setCode(vpr::ReturnStatus::Fail);
         break;
      case vpr::SocketOptions::McastTimeToLive:
         status.setCode(vpr::ReturnStatus::Fail);
         break;
      case vpr::SocketOptions::McastLoopback:
         status.setCode(vpr::ReturnStatus::Fail);
         break;

      // TCP-level options.
      case vpr::SocketOptions::NoDelay:
         status.setCode(vpr::ReturnStatus::Fail);
         break;
      case vpr::SocketOptions::MaxSegment:
         status.setCode(vpr::ReturnStatus::Fail);
         break;
    }

   return status;
}

vpr::ReturnStatus SocketImplSIM::inExceptState()
{
   // XXX: For now, we never go into an exceptional state.
   return vpr::ReturnStatus(vpr::ReturnStatus::Fail);
}

// ============================================================================
// Protected methods.
// ============================================================================

SocketImplSIM::SocketImplSIM(const vpr::SocketTypes::Type sock_type)
   : mOpen(false), mBound(false), mOpenBlocking(false), mBlocking(false),
     mType(sock_type), mReuseAddr(false), mConnected(false), mPeer(NULL)
{
   /* Do nothing. */ ;
}

SocketImplSIM::SocketImplSIM(const vpr::InetAddr& local_addr,
                             const vpr::InetAddr& remote_addr,
                             const vpr::SocketTypes::Type sock_type)
   : mOpen(false), mBound(false), mOpenBlocking(false), mBlocking(false),
     mLocalAddr(local_addr), mRemoteAddr(remote_addr), mType(sock_type),
     mConnected(false), mPeer(NULL)
{
   /* Do nothing. */ ;
}

void SocketImplSIM::disconnect()
{
   // XXX: This is potentially not the best way to disconnect, but it's the
   // best I have come up with so far.
   setConnectState(NULL);
}

} // End of vpr namespace
