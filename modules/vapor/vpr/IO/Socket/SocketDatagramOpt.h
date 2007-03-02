/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 ****************** <VPR heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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

#ifndef _VPR_SOCKET_DATAGRAM_OPT_H_
#define _VPR_SOCKET_DATAGRAM_OPT_H_

#include <vpr/vprConfig.h>

#include <vpr/vprTypes.h>
#include <vpr/IO/Socket/SocketOptions.h>
#include <vpr/IO/Socket/SocketIpOpt.h>
#include <vpr/IO/Socket/InetAddr.h>


namespace vpr
{

/** \class SocketDatagramOpt SocketDatagramOpt.h vpr/IO/Socket/SocketDatagramOpt.h
 *
 * Options for datagram sockets.
 */
class VPR_CLASS_API SocketDatagramOpt : public vpr::SocketOptionWrapper
{
public:
   /**
    * Default constructor.  This does nothing and exists only so that we can
    * build a DLL on Win32.
    */
   SocketDatagramOpt();

   /**
    * Default destructor.  This does nothing and exists only so that we can
    * build a DLL on Win32.
    */
   virtual ~SocketDatagramOpt();

   /**
    * Gets the multicast interface for this datagram socket.
    * @throws vpr::IOException if the operation failed.
    */
   const vpr::InetAddr getMcastInterface() const
   {
      vpr::SocketOptions::Data option;

      getOption(SocketOptions::McastInterface, option);
      return option.mcast_if;
   }

   /**
    * Sets the multicast interface for this datagram socket.
    * @throws vpr::IOException if the operation failed.
    */
   void setMcastInterface(const vpr::InetAddr& mcastIf)
   {
      vpr::SocketOptions::Data option;
      option.mcast_if = mcastIf;
      setOption(SocketOptions::McastInterface, option);
   }

   /**
    * Gets the multicast time-to-live parameter for packets sent on this
    * socket.
    * @throws vpr::IOException if the operation failed.
    */
   vpr::Uint8 getMcastTimeToLive() const
   {
      vpr::SocketOptions::Data option;

      getOption(SocketOptions::McastTimeToLive, option);
      return option.mcast_ttl;
   }

   /**
    * Sets the multicast time-to-live parameter for packets sent on this
    * socket.
    * @throws vpr::IOException if the operation failed.
    */
   void setMcastTimeToLive(const vpr::Uint8 ttl)
   {
      vpr::SocketOptions::Data option;
      option.mcast_ttl = ttl;
      setOption(SocketOptions::McastTimeToLive, option);
   }

   /**
    *@throws vpr::IOException if the operation failed.
    */
   vpr::Uint8 getMcastLoopback() const
   {
      vpr::SocketOptions::Data option;

      getOption(SocketOptions::McastLoopback, option);
      return option.mcast_loopback;
   }

   /**
    *@throws vpr::IOException if the operation failed.
    */
   void setMcastLoopback(const vpr::Uint8 loop)
   {
      vpr::SocketOptions::Data option;
      option.mcast_loopback = loop;
      setOption(SocketOptions::McastLoopback, option);
   }

   /**
    *@throws vpr::IOException if the operation failed.
    */
   void addMcastMember(const vpr::McastReq& request)
   {
      vpr::SocketOptions::Data option;
      option.mcast_add_member = request;
      setOption(SocketOptions::AddMember, option);
   }

   /**
    *@throws vpr::IOException if the operation failed.
    */
   void dropMcastMember(const vpr::McastReq& request)
   {
      vpr::SocketOptions::Data option;
      option.mcast_drop_member = request;
      setOption(SocketOptions::DropMember, option);
   }

   /**
    * @throws vpr::IOException if the operation failed.
    * @since 1.1.17
    */
   void setBroadcast(const bool val)
   {
      vpr::SocketOptions::Data option;
      option.broadcast = val;
      setOption(SocketOptions::Broadcast, option);
   }
};

} // End of vpr namespace


#endif  /* _VPR_SOCKET_DATAGRAM_OPT_H_ */
