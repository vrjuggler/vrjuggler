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
                                                   const int flags,
                                                   vpr::InetAddr& from,
                                                   vpr::Uint32& bytes_read,
                                                   const vpr::Interval timeout)
{
   vpr::ReturnStatus status;
   vpr::Guard<vpr::Mutex> guard(mArrivedQueueMutex);

   if ( ! mArrivedQueue.empty() )
   {
      vpr::sim::MessagePtr msg_ptr = mArrivedQueue[0];
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
         mArrivedQueue.erase(mArrivedQueue.begin());
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
                                                 const int flags,
                                                 const vpr::InetAddr& to,
                                                 vpr::Uint32& bytes_sent,
                                                 const vpr::Interval timeout)
{
   vpr::ReturnStatus status;

   if ( ! mNodeAssigned )
   {
      vpr::sim::Controller::instance()->getSocketManager().assignToNode(this, mLocalAddr);
   }

   bytes_sent = length;
   vpr::sim::MessagePtr net_msg(new vpr::sim::Message(msg, length));
   vpr::sim::Controller::instance()->getSocketManager().sendMessageTo(net_msg, this, to);

   return status;
}

vpr::ReturnStatus SocketDatagramImplSIM::isReadReady (const vpr::Interval timeout)
   const
{
   vpr::ReturnStatus status;

   if ( vpr::Interval::NoWait == timeout )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << "SocketDatagramImplSIM::isReadReady(): Timeouts not supported "
         << "with sim sockets--yet\n" << vprDEBUG_FLUSH;
   }

//   if ( vpr::Interval::NoWait == timeout )
//   {
      if ( ! mOpen || mArrivedQueue.empty() )
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

vpr::ReturnStatus SocketDatagramImplSIM::isWriteReady (const vpr::Interval timeout)
   const
{
   vpr::ReturnStatus status;

   if ( vpr::Interval::NoWait == timeout )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << "SocketDatagramImplSIM::isWriteReady(): Timeouts not supported with "
         << "sim sockets--yet\n" << vprDEBUG_FLUSH;
   }

//   if ( vpr::Interval::NoWait == timeout )
//   {
      if ( ! mOpen )
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
