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
      McastInterface,   /**< Multicast interface option */
      McastTimeToLive,  /**< Multicast time to live (TTL) option */
      McastLoopback,    /**< Multicast loopback option */
      NoDelay,          /**< TCP "no delay" option (Nagle Algorithm) */
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

/** \class SocketOptionWrapper SocketOptions.h vpr/IO/Socket/SocketOptions.h
 *
 * Simple interface for setting and querying socket options.
 */
class VPR_CLASS_API SocketOptionWrapper
{
public:
   virtual ~SocketOptionWrapper()
   {
      /* Do nothing. */ ;
   }

protected:
   /**
    * Retrieves the value for the given option as set on the socket.
    *
    * @param option The option to be queried.
    * @param data   A data buffer that will be used to store the value of the
    *               given option.
    *
    * @throws vpr::SocketException if the value for the given option
    *         could not be retrieved.
    */
   virtual void getOption(const vpr::SocketOptions::Types option,
                          struct vpr::SocketOptions::Data& data) const
      throw (SocketException) = 0;

   /**
    * Sets a value for the given option on the socket using the given data
    * block.
    *
    * @param option The option whose value will be set.
    * @param data   A data buffer containing the value to be used in setting
    *               the socket option.
    *
    * @throws vpr::SocketException if the value for the given option
    *         could not be set.
    */
   virtual void setOption(const vpr::SocketOptions::Types option,
                          const struct vpr::SocketOptions::Data& data)
      throw (SocketException) = 0;
};

} // End of vpr namespace


#endif  /* _VPR_SOCKET_OPTIONS_H_ */
