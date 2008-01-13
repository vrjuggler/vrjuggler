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
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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

#ifndef _VPR_SOCKET_OPTIONS_H_
#define _VPR_SOCKET_OPTIONS_H_

#include <vpr/vprConfig.h>

#include <vpr/IO/Socket/InetAddr.h>
#include <vpr/IO/Socket/McastReq.h>
#include <vpr/IO/Socket/SocketException.h>


namespace vpr
{

/** Socket-level options. */
namespace SocketOptions
{
   /**
    * Socket option types.  These are used to identify what option is being
    * set or requested.
    *
    * @see vpr::SocketOptions::Data, vpr::SocketOptionsWrapper
    */
   enum Types
   {
      Linger,           /**< IP linger option */
      ReuseAddr,        /**< IP resuse address option */
      KeepAlive,        /**< IP keep-alive option */
      RecvBufferSize,   /**< Receive buffer size option */
      SendBufferSize,   /**< Send buffer size option */
      IpTimeToLive,     /**< IP time to live (TTL) option */
      IpTypeOfService,  /**< IP type of service option */
      AddMember,        /**< Multicast "add member" option */
      DropMember,       /**< Multicast "remove member" option */
      Broadcast,        /**< Establish permission to send broadcast datagrams */
      McastInterface,   /**< Multicast interface option */
      McastTimeToLive,  /**< Multicast time to live (TTL) option */
      McastLoopback,    /**< Multicast loopback option */
      NoDelay,          /**< TCP "no delay" option (Nagle Algorithm) */
      NoPush,           /**< TCP "corking" option. @since 2.1.9 */
      MaxSegment,       /**< TCP maximum segment size option */
            /** Implementation specific */
      Error             /**< Current socket error */
   };

   /** Socket type of service options. */
   enum TypeOfService
   {
      LowDelay,     /**< Low delay service */
      Throughput,   /**< Optimal throughput service */
      Reliability,  /**< Optimal reliability service */
      LowCost       /**< Low cost service */
   };

   /** \struct Linger SocketOptions.h vpr/IO/Socket/SocketOptions.h
    *
    * IP linger data structure.
    */
   struct Linger
   {
      bool       enabled;
      vpr::Int32 seconds;
   };

   /** \struct Data SocketOptions.h vpr/IO/Socket/SocketOptions.h
    *
    * Socket options data block.
    *
    * @see vpr::SocketOptions::Types, vpr::SocketOptionWrapper
    */
   struct Data
   {
      Uint32        ip_ttl;
      TypeOfService type_of_service;
      bool          reuse_addr;
      bool          keep_alive;
      bool          no_delay;
      bool          no_push;            /**< @since 2.1.9 */
      bool          broadcast;
      size_t        max_segment;
      size_t        recv_buffer_size;
      size_t        send_buffer_size;
      struct Linger linger;
      InetAddr      mcast_if;
      Uint8         mcast_ttl;
      Uint8         mcast_loopback;
      McastReq      mcast_add_member;
      McastReq      mcast_drop_member;
      Int32         error;
   };
}

} // End of vpr namespace


#endif  /* _VPR_SOCKET_OPTIONS_H_ */
