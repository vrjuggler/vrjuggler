/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VPR_SOCKET_DATAGRAM_OPT_H_
#define _VPR_SOCKET_DATAGRAM_OPT_H_

#include <vpr/IO/Socket/SocketIpOpt.h>


namespace vpr {

/**
 * Options for datagram sockets.
 *
 * @author Patrick Hartling
 */
class VPR_CLASS_API SocketDatagramOpt : public SocketIpOpt {
public:
    /**
     * Gets the multicast interface for this datagram socket.
     */
    inline Status
    getMcastInterface (InetAddr& mcast_if) {
        SocketOptions::Data option;
        Status retval;

        retval = getOption(SocketOptions::McastInterface, option);

        if ( retval.success() ) {
            mcast_if = option.mcast_if;
        }

        return retval;
    }

    /**
     * Sets the multicast interface for this datagram socket.
     */
    inline Status
    setMcastInterface (const InetAddr& mcast_if) {
        SocketOptions::Data option;
        option.mcast_if = mcast_if;
        return setOption(SocketOptions::McastInterface, option);
    }

    /**
     * Gets the multicast time-to-live parameter for packets sent on this
     * socket.
     */
    inline Status
    getMcastTimeToLive (Uint8& ttl) {
        SocketOptions::Data option;
        Status retval;

        retval = getOption(SocketOptions::McastTimeToLive, option);

        if ( retval.success() ) {
            ttl = option.mcast_ttl;
        }

        return retval;
    }

    /**
     *
     */
    inline Status
    setMcastTimeToLive (const Uint8 ttl) {
        SocketOptions::Data option;
        option.mcast_ttl = ttl;
        return setOption(SocketOptions::McastTimeToLive, option);
    }

    /**
     *
     */
    inline Status
    getMcastLoopback (Uint8& loop) {
        SocketOptions::Data option;
        Status retval;

        retval = getOption(SocketOptions::McastLoopback, option);

        if ( retval.success() ) {
            loop = option.mcast_loopback;
        }

        return retval;
    }

    /**
     *
     */
    inline Status
    setMcastLoopback (const Uint8 loop) {
        SocketOptions::Data option;
        option.mcast_loopback = loop;
        return setOption(SocketOptions::McastLoopback, option);
    }

    /**
     *
     */
    inline Status
    addMcastMember (const McastReq& request) {
        SocketOptions::Data option;
        option.mcast_add_member = request;
        return setOption(SocketOptions::AddMember, option);
    }

    /**
     *
     */
    inline Status
    dropMcastMember (const McastReq& request) {
        SocketOptions::Data option;
        option.mcast_drop_member = request;
        return setOption(SocketOptions::DropMember, option);
    }
};

}; // End of vpr namespace


#endif	/* _VPR_SOCKET_DATAGRAM_OPT_H_ */
