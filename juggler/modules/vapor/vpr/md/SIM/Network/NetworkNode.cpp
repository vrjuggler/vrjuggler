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
   vprASSERT(! hasSocket(sock->getLocalAddr().getPort(), sock->getType()) && "Tried to overwrite an existing socket");

   switch (sock->getType())
   {
      case vpr::SocketTypes::DATAGRAM:
         mDgramSocketMap[sock->getLocalAddr().getPort()] = sock;
         break;
      case vpr::SocketTypes::STREAM:
         mStreamSocketMap[sock->getLocalAddr().getPort()] = sock;
         break;
   }
}

void NetworkNode::deliverMessage (vpr::sim::MessagePtr msg)
{
//   vpr::sim::MessagePtr msg_copy(new vpr::sim::Message(*msg));

   const vpr::SocketImplSIM* dest_sock = msg->getDestinationSocket();

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
