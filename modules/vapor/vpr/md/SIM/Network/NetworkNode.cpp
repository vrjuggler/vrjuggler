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
#include <vpr/md/SIM/IO/Socket/SocketStreamImplSIM.h>
#include <vpr/md/SIM/IO/Socket/SocketDatagramImplSIM.h>
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

   for ( vpr::Uint8 i = 0; i < 4; ++i )
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
         result = (mDgramSocketMap.find(port) != mDgramSocketMap.end());
         break;
      case vpr::SocketTypes::STREAM:
         result = (mStreamSocketMap.find(port) != mStreamSocketMap.end());;
         break;
   }

   return  result;
}

vpr::SocketImplSIM* NetworkNode::getSocket(const vpr::Uint32 port,
                              const vpr::SocketTypes::Type type)
{
   vprASSERT( hasSocket(port,type) && "getSocket doesn't have a socket of the given type");

   switch (type)
   {
      case vpr::SocketTypes::DATAGRAM:
         return mDgramSocketMap[port];
         break;
      case vpr::SocketTypes::STREAM:
         return mStreamSocketMap[port];
         break;
   }

   return NULL;
}

void NetworkNode::addSocket (vpr::SocketImplSIM* sock)
{
   vpr::Uint32 port = sock->getLocalAddr().getPort();
   vprASSERT( mIpAddr == sock->getLocalAddr().getAddressValue() && "Trying to add socket to node of wrong ip addr");
   if(! hasSocket(port, sock->getType()))
      vprDEBUG(vprDBG_ALL, 0) << "NetworkNode::addSocket: Tried to overwrite existing socket: " << sock->getLocalAddr() << std::endl << vprDEBUG_FLUSH;
   vprASSERT(! hasSocket(port, sock->getType()) && "Tried to overwrite an existing socket");

   vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
      << "NetworkNode::addSocket() [" << mIpStr << "]: Adding socket (" << sock
      << ") with local address " << sock->getLocalAddr() << " to node "
      << mIpStr << "\n" << vprDEBUG_FLUSH;

   switch (sock->getType())
   {
      case vpr::SocketTypes::DATAGRAM:
         vprASSERT( dynamic_cast<vpr::SocketDatagramImplSIM*>(sock) != NULL);
         mDgramSocketMap[port] = sock;
         break;
      case vpr::SocketTypes::STREAM:
         vprASSERT( dynamic_cast<vpr::SocketStreamImplSIM*>(sock) != NULL);
         mStreamSocketMap[port] = sock;
         break;
   }
}

vpr::ReturnStatus NetworkNode::removeSocket (const vpr::SocketImplSIM* sock)
{
   vpr::ReturnStatus status(vpr::ReturnStatus::Fail);
   socket_map_t::iterator i;

   vpr::Uint32 port( sock->getLocalAddr().getPort() );
   vpr::Uint32 ip_addr( sock->getLocalAddr().getAddressValue() );

   vprASSERT( hasSocket(port, sock->getType())  && "Trying to remove a socket that isn't registered with node");
   vprASSERT( mIpAddr == ip_addr && "Removing socket from wrong node [ip addresses differ]");

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
            vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL) << "NetworkNode::removeSocket() [" << mIpStr
                                                  << "]: Removed datagram socket\n" << vprDEBUG_FLUSH;
            vprASSERT(!hasSocket(port, sock->getType()) && "Erasing did nothing");
         }
         break;
      case vpr::SocketTypes::STREAM:
         i = mStreamSocketMap.find(port);

         if ( i != mStreamSocketMap.end() )
         {
            mStreamSocketMap.erase(i);
            status.setCode(vpr::ReturnStatus::Succeed);
            vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL) << "NetworkNode::removeSocket() [" << mIpStr
                                                  << "]: Removed stream socket\n" << vprDEBUG_FLUSH;
            vprASSERT(!hasSocket(port, sock->getType()) && "Erasing did nothing");
         }
         break;
   }

   return status;
}

/** Get an unused TCP port number */
vpr::Uint32 NetworkNode::getUnassignedTcpPortNumber()
{
   vpr::Uint32 ret_val(1);
   bool        found_one(false);
   for(socket_map_t::iterator i = mStreamSocketMap.begin();
       (!found_one) && (i != mStreamSocketMap.end()); ++i)
   {
      socket_map_t::iterator next = i; ++next;
      if(next != mStreamSocketMap.end())
      {
         if((*next).first != ((*i).first + 1) )     // If not sequential
         {
            ret_val = (*i).first + 1;
            found_one = true;
         }
      }
      else // There is no next, so use the one after the current
      {
         ret_val = (*i).first +1;
         found_one = true;
      }

   }

   vprASSERT( mStreamSocketMap.find(ret_val) == mStreamSocketMap.end() && "Returned a value that already exists");
   return ret_val;
}

/** Get an unused TCP port number */
vpr::Uint32 NetworkNode::getUnassignedUdpPortNumber()
{
   vpr::Uint32 ret_val(1);
   bool        found_one(false);
   for(socket_map_t::iterator i = mDgramSocketMap.begin();
       (!found_one) && (i != mDgramSocketMap.end()); ++i)
   {
      socket_map_t::iterator next = i; ++next;
      if(next != mDgramSocketMap.end())
      {
         if((*next).first != ((*i).first + 1) )     // If not sequential
         {
            ret_val = (*i).first + 1;
            found_one = true;
         }
      }
      else // There is no next, so use the one after the current
      {
         ret_val = (*i).first +1;
         found_one = true;
      }
   }

   vprASSERT( mDgramSocketMap.find(ret_val) == mDgramSocketMap.end() && "Returned a value that already exists");
   return ret_val;
}

} // End of sim namespace

} // End of vpr namespace
