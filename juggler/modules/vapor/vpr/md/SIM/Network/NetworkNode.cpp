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
#include <vpr/Util/Assert.h>
#include <vpr/Util/Debug.h>
#include <vpr/System.h>

#include <vpr/md/SIM/IO/Socket/SocketImplSIM.h>
#include <vpr/md/SIM/Network/Message.h>
#include <vpr/md/SIM/Network/MessagePtr.h>
#include <vpr/md/SIM/Network/NetworkNode.h>


namespace vpr
{

namespace sim
{

NetworkNode::NetworkNode (const vpr::Uint32 index, const vpr::Uint8 type,
                          const std::string& ip_str)
   : mIndex(index), mType(type), mIpStr(ip_str), mIpAddr(0)
{
   std::string::size_type cur_pos, next_dot;
   std::string cur_val;

   union
   {
      char bytes[4];
      vpr::Uint32 value;
   } ip_addr;

   cur_pos = 0;

   for ( vpr::Uint8 i = 0; i < 4; i++ )
   {
      next_dot = ip_str.find(".", cur_pos);
      cur_val  = ip_str.substr(cur_pos, next_dot - cur_pos);

      ip_addr.bytes[i] = atoi(cur_val.c_str());

      cur_pos = next_dot + 1;
   }

   mIpAddr = vpr::System::Ntohl(ip_addr.value);
}

bool NetworkNode::hasSocket (const vpr::Uint32 port,
                             const vpr::SocketTypes::Type type)
   const
{
   bool result = false;

   switch (type)
   {
      case vpr::SocketTypes::DATAGRAM:
         result = mDgramSocketMap.count(port) > 0;
         break;
      case vpr::SocketTypes::STREAM:
         result = mStreamSocketMap.count(port) > 0;
         break;
   }

   return  result;
}

void NetworkNode::addSocket (vpr::SocketImplSIM* sock)
{
   vpr::Uint32 port = sock->getLocalAddr().getPort();
   vprASSERT(! hasSocket(port, sock->getType()) && "Tried to overwrite an existing socket");
   vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
      << "NetworkNode::addSocket() [" << mIpStr << "]: Adding socket (" << sock
      << ") with local address " << sock->getLocalAddr() << " to node "
      << mIpStr << "\n" << vprDEBUG_FLUSH;

   switch (sock->getType())
   {
      case vpr::SocketTypes::DATAGRAM:
         mDgramSocketMap[port] = sock;
         break;
      case vpr::SocketTypes::STREAM:
         mStreamSocketMap[port] = sock;
         break;
   }
}

vpr::ReturnStatus NetworkNode::removeSocket (const vpr::SocketImplSIM* sock)
{
   vpr::ReturnStatus status(vpr::ReturnStatus::Fail);
   std::map<vpr::Uint32, vpr::SocketImplSIM*>::iterator i;
   vpr::Uint32 port;

   port = sock->getLocalAddr().getPort();

   vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
      << "NetworkNode::removeSocket() [" << mIpStr
      << "]: Removing socket bound to " << sock->getLocalAddr() << "\n"
      << vprDEBUG_FLUSH;

   switch (sock->getType())
   {
      case vpr::SocketTypes::DATAGRAM:
         i = mDgramSocketMap.find(port);

         if ( i != mDgramSocketMap.end() )
         {
            mDgramSocketMap.erase(i);
            status.setCode(vpr::ReturnStatus::Succeed);
            vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)
               << "NetworkNode::removeSocket() [" << mIpStr
               << "]: Removed datagram socket\n" << vprDEBUG_FLUSH;
            vprASSERT(mDgramSocketMap.count(port) == 0 && "Erasing did nothing");
         }
         break;
      case vpr::SocketTypes::STREAM:
         i = mStreamSocketMap.find(port);

         if ( i != mStreamSocketMap.end() )
         {
            mStreamSocketMap.erase(i);
            status.setCode(vpr::ReturnStatus::Succeed);
            vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)
               << "NetworkNode::removeSocket() [" << mIpStr
               << "]: Removed stream socket\n" << vprDEBUG_FLUSH;
            vprASSERT(mStreamSocketMap.count(port) == 0 && "Erasing did nothing");
         }
         break;
   }

   return status;
}

void NetworkNode::deliverMessage (vpr::sim::MessagePtr msg)
{
//   vpr::sim::MessagePtr msg_copy(new vpr::sim::Message(*msg));

   const vpr::SocketImplSIM* dest_sock = msg->getDestinationSocket();

   vprDEBUG(vprDBG_ALL, vprDBG_HVERB_LVL)
      << "NetworkNode::deliverMessage() [" << mIpStr
      << "]: Delivering message to socket (" << dest_sock
      << ") with local address " << dest_sock->getLocalAddr() << "\n"
      << vprDEBUG_FLUSH;

   // Pass message to the destination socket.
   switch (dest_sock->getType())
   {
      case vpr::SocketTypes::DATAGRAM:
         vprASSERT(mDgramSocketMap.count(dest_sock->getLocalAddr().getPort()) > 0 && "Message delivered to wrong node");
         mDgramSocketMap[dest_sock->getLocalAddr().getPort()]->addArrivedMessage(msg);
         break;
      case vpr::SocketTypes::STREAM:
         vprASSERT(mStreamSocketMap.count(dest_sock->getLocalAddr().getPort()) > 0 && "Message delivered to wrong node");
         mStreamSocketMap[dest_sock->getLocalAddr().getPort()]->addArrivedMessage(msg);
         break;
   }
}

} // End of sim namespace

} // End of vpr namespace
