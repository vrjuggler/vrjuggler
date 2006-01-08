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
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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
    */
   void getMcastInterface(vpr::InetAddr& mcastIf) throw (IOException)
   {
      vpr::SocketOptions::Data option;

      getOption(SocketOptions::McastInterface, option);
      mcastIf = option.mcast_if;
   }

   /**
    * Sets the multicast interface for this datagram socket.
    */
   void setMcastInterface(const vpr::InetAddr& mcastIf) throw (IOException)
   {
      vpr::SocketOptions::Data option;
      option.mcast_if = mcastIf;
      setOption(SocketOptions::McastInterface, option);
   }

   /**
    * Gets the multicast time-to-live parameter for packets sent on this
    * socket.
    */
   void getMcastTimeToLive(vpr::Uint8& ttl) throw (IOException)
   {
      vpr::SocketOptions::Data option;

      getOption(SocketOptions::McastTimeToLive, option);
      ttl = option.mcast_ttl;
   }

   /**
    * Sets the multicast time-to-live parameter for packets sent on this
    * socket.
    */
   void setMcastTimeToLive(const vpr::Uint8 ttl) throw (IOException)
   {
      vpr::SocketOptions::Data option;
      option.mcast_ttl = ttl;
      setOption(SocketOptions::McastTimeToLive, option);
   }

   /**
    *
    */
   void getMcastLoopback(vpr::Uint8& loop) throw (IOException)
   {
      vpr::SocketOptions::Data option;

      getOption(SocketOptions::McastLoopback, option);
      loop = option.mcast_loopback;
   }

   /**
    *
    */
   void setMcastLoopback(const vpr::Uint8 loop) throw (IOException)
   {
      vpr::SocketOptions::Data option;
      option.mcast_loopback = loop;
      setOption(SocketOptions::McastLoopback, option);
   }

   /**
    *
    */
   void addMcastMember(const vpr::McastReq& request) throw (IOException)
   {
      vpr::SocketOptions::Data option;
      option.mcast_add_member = request;
      setOption(SocketOptions::AddMember, option);
   }

   /**
    *
    */
   void dropMcastMember(const vpr::McastReq& request) throw (IOException)
   {
      vpr::SocketOptions::Data option;
      option.mcast_drop_member = request;
      setOption(SocketOptions::DropMember, option);
   }
};

} // End of vpr namespace


#endif  /* _VPR_SOCKET_DATAGRAM_OPT_H_ */
