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

#ifndef _VPR_SOCKET_OPTIONS_H_
#define _VPR_SOCKET_OPTIONS_H_

#include <vpr/vprConfig.h>

#include <vpr/IO/Socket/InetAddr.h>
#include <vpr/IO/Socket/McastReq.h>
#include <vpr/Util/Status.h>


namespace vpr {

namespace SocketOptions {
    enum Types {
        Linger,
        ReuseAddr,
        KeepAlive,
        RecvBufferSize,
        SendBufferSize,
        IpTimeToLive,
        IpTypeOfService,
        AddMember,
        DropMember,
        McastInterface,
        McastTimeToLive,
        McastLoopback,
        NoDelay,
        MaxSegment
    };

    enum TypeOfService {
        LowDelay,
        Throughput,
        Reliability,
        LowCost
    };

    struct Linger {
        bool  enabled;
        Int32 seconds;
    };

    // I really wish this could be a union...
    struct Data {
        Uint32        ip_ttl;
        TypeOfService type_of_service;
        bool          reuse_addr;
        bool          keep_alive;
        bool          no_delay;
        Uint32        max_segment;
        Uint32        recv_buffer_size;
        Uint32        send_buffer_size;
        struct Linger linger;
        InetAddr      mcast_if;
        Uint8         mcast_ttl;
        Uint8         mcast_loopback;
        McastReq      mcast_add_member;
        McastReq      mcast_drop_member;
    };
};

class VPR_CLASS_API SocketOptionWrapper {
protected:
    /**
     * Retrieves the value for the given option as set on the socket.
     *
     * @param option The option to be queried.
     * @param data   A data buffer that will be used to store the value of the
     *               given option.
     *
     * @return vpr::Status::Success is returned if the value for the given
     *         option was retrieved successfully.<br>
     *         vpr::Status;:Failure is returned otherwise.
     */
    virtual vpr::Status getOption(const vpr::SocketOptions::Types option,
                                  struct vpr::SocketOptions::Data& data) = 0;

    /**
     * Sets a value for the given option on the socket using the given data
     * block.
     *
     * @param option The option whose value will be set.
     * @param data   A data buffer containing the value to be used in setting
     *               the socket option.
     */
    virtual vpr::Status setOption(const vpr::SocketOptions::Types option,
                                  const struct vpr::SocketOptions::Data& data) = 0;
};

}; // End of vpr namespace


#endif	/* _VPR_SOCKET_OPTIONS_H_ */
