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

#include <IO/Socket/SocketIpOpt.h>


namespace vpr {

class SocketDatagramOpt : public SocketIpOpt {
public:
    /**
     *
     */
    inline int
    getMcastInterface (InetAddr& mcast_if) {
        SocketOptions::Data option;
        int retval;

        retval = getOption(SocketOptions::McastInterface, option);

        if ( retval == 0 ) {
            mcast_if = option.mcast_if;
        }

        return retval;
    }

    /**
     *
     */
    inline int
    setMcastInterface (const InetAddr& mcast_if) {
        SocketOptions::Data option;
        option.mcast_if = mcast_if;
        return setOption(SocketOptions::McastInterface, option);
    }

    /**
     *
     */
    inline int
    getMcastTimeToLive (Uint8& ttl) {
        SocketOptions::Data option;
        int retval;

        retval = getOption(SocketOptions::McastTimeToLive, option);

        if ( retval == 0 ) {
            ttl = option.mcast_ttl;
        }

        return retval;
    }

    /**
     *
     */
    inline int
    setMcastTimeToLive (const Uint8 ttl) {
        SocketOptions::Data option;
        option.mcast_ttl = ttl;
        return setOption(SocketOptions::McastTimeToLive, option);
    }

    /**
     *
     */
    inline int
    getMcastLoopback (Uint8& loop) {
        SocketOptions::Data option;
        int retval;

        retval = getOption(SocketOptions::McastLoopback, option);

        if ( retval == 0 ) {
            loop = option.mcast_loopback;
        }

        return retval;
    }

    /**
     *
     */
    inline int
    setMcastLoopback (const Uint8 loop) {
        SocketOptions::Data option;
        option.mcast_loopback = loop;
        return setOption(SocketOptions::McastLoopback, option);
    }

    /**
     *
     */
    inline int
    addMcastMember (const McastReq& request) {
        SocketOptions::Data option;
        option.mcast_add_member = request;
        return setOption(SocketOptions::AddMember, option);
    }

    /**
     *
     */
    inline int
    dropMcastMember (const McastReq& request) {
        SocketOptions::Data option;
        option.mcast_drop_member = request;
        return setOption(SocketOptions::DropMember, option);
    }
};

}; // End of vpr namespace


#endif	/* _VPR_SOCKET_DATAGRAM_OPT_H_ */
