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

#include <vpr/vprConfig.h>

#include <vpr/Sync/Guard.h>
#include <vpr/md/SIM/Network/Message.h>
#include <vpr/md/SIM/Network/MessagePtr.h>
#include <vpr/md/SIM/Controller.h>
#include <vpr/md/SIM/SocketManager.h>
#include <vpr/md/SIM/IO/Socket/SocketDatagramImplSIM.h>


namespace vpr
{

vpr::ReturnStatus SocketDatagramImplSIM::recvfrom (void* msg,
                                                   const vpr::Uint32 length,
                                                   vpr::InetAddr& from,
                                                   vpr::Uint32& bytes_read,
                                                   const vpr::Interval timeout)
{
   vpr::ReturnStatus status;
   vprASSERT(mBound && "Can recvfrom if we are not bound so they can send to us");

   vpr::Guard<vpr::Mutex> guard(mArrivedQueueMutex);

   if ( ! mArrivedQueue.empty() )
   {
      vpr::sim::MessagePtr msg_ptr = mArrivedQueue.front();
      vpr::Uint32 msg_size, copy_len;

      msg_size = msg_ptr->getSize();

      // Use the smaller of length and msg_size for the actual amount of
      // data to copy.
      copy_len = (length > msg_size) ? msg_size : length;

      // Complete the read operation.
      memcpy(msg, msg_ptr->getBody(), copy_len);
      bytes_read = copy_len;

      from = msg_ptr->getSourceSocket()->getLocalAddr();

      if ( msg_ptr->resize(copy_len) == 0 )
      {
         mArrivedQueue.pop_front();
      }
   }
   else
   {
      status.setCode(vpr::ReturnStatus::WouldBlock);
      bytes_read = 0;
   }

   return status;
}

vpr::ReturnStatus SocketDatagramImplSIM::sendto (const void* msg,
                                                 const vpr::Uint32 length,
                                                 const vpr::InetAddr& to,
                                                 vpr::Uint32& bytes_sent,
                                                 const vpr::Interval timeout)
{
   vpr::ReturnStatus status;
   vpr::sim::SocketManager& sock_mgr =
      vpr::sim::Controller::instance()->getSocketManager();

   if ( ! mBound )
   {
      bind();      
   }

   vprASSERT( mBound && "Must be bound before sending");

   bytes_sent = length;
   vpr::sim::MessagePtr net_msg(new vpr::sim::Message(msg, length));
   sock_mgr.sendMessageTo(net_msg, this, to);

   return status;
}

vpr::ReturnStatus SocketDatagramImplSIM::isReadReady () const
{
   vpr::ReturnStatus status;

   if ( ! mOpen || mArrivedQueue.empty() )
   {
      status.setCode(vpr::ReturnStatus::Fail);
   }

   return status;
}

vpr::ReturnStatus SocketDatagramImplSIM::isWriteReady () const
{
   vpr::ReturnStatus status;

   if ( ! mOpen )
   {
      status.setCode(vpr::ReturnStatus::Fail);
   }

   return status;
}

} // End of vpr namespace
