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

#include <vpr/vprConfig.h>

#include <vpr/vprTypes.h>
#include <vpr/IO/Socket/SocketOptions.h>
#include <vpr/IO/Socket/SocketIpOpt.h>
#include <vpr/IO/Socket/InetAddr.h>
#include <vpr/Util/Status.h>


namespace vpr {

/**
 * Options for datagram sockets.
 *
 * @author Patrick Hartling
 */
class VPR_CLASS_API SocketDatagramOpt : public vpr::SocketOptionWrapper {
public:
    /**
     * Default constructor.  This does nothing and exists only so that we can
     * build a DLL on Win32.
     */
    SocketDatagramOpt(void);

    /**
     * Default destructor.  This does nothing and exists only so that we can
     * build a DLL on Win32.
     */
    virtual ~SocketDatagramOpt(void);

    /**
     * Gets the multicast interface for this datagram socket.
     */
    inline vpr::Status
    getMcastInterface (vpr::InetAddr& mcast_if) {
        vpr::SocketOptions::Data option;
        vpr::Status retval;

        retval = getOption(SocketOptions::McastInterface, option);

        if ( retval.success() ) {
            mcast_if = option.mcast_if;
        }

        return retval;
    }

    /**
     * Sets the multicast interface for this datagram socket.
     */
    inline vpr::Status
    setMcastInterface (const vpr::InetAddr& mcast_if) {
        vpr::SocketOptions::Data option;
        option.mcast_if = mcast_if;
        return setOption(SocketOptions::McastInterface, option);
    }

    /**
     * Gets the multicast time-to-live parameter for packets sent on this
     * socket.
     */
    inline vpr::Status
    getMcastTimeToLive (vpr::Uint8& ttl) {
        vpr::SocketOptions::Data option;
        vpr::Status retval;

        retval = getOption(SocketOptions::McastTimeToLive, option);

        if ( retval.success() ) {
            ttl = option.mcast_ttl;
        }

        return retval;
    }

    /**
     * Sets the multicast time-to-live parameter for packets sent on this
     * socket.
     */
    inline vpr::Status
    setMcastTimeToLive (const vpr::Uint8 ttl) {
        vpr::SocketOptions::Data option;
        option.mcast_ttl = ttl;
        return setOption(SocketOptions::McastTimeToLive, option);
    }

    /**
     *
     */
    inline vpr::Status
    getMcastLoopback (vpr::Uint8& loop) {
        vpr::SocketOptions::Data option;
        vpr::Status retval;

        retval = getOption(SocketOptions::McastLoopback, option);

        if ( retval.success() ) {
            loop = option.mcast_loopback;
        }

        return retval;
    }

    /**
     *
     */
    inline vpr::Status
    setMcastLoopback (const vpr::Uint8 loop) {
        vpr::SocketOptions::Data option;
        option.mcast_loopback = loop;
        return setOption(SocketOptions::McastLoopback, option);
    }

    /**
     *
     */
    inline vpr::Status
    addMcastMember (const vpr::McastReq& request) {
        vpr::SocketOptions::Data option;
        option.mcast_add_member = request;
        return setOption(SocketOptions::AddMember, option);
    }

    /**
     *
     */
    inline vpr::Status
    dropMcastMember (const vpr::McastReq& request) {
        vpr::SocketOptions::Data option;
        option.mcast_drop_member = request;
        return setOption(SocketOptions::DropMember, option);
    }
};

}; // End of vpr namespace


#endif	/* _VPR_SOCKET_DATAGRAM_OPT_H_ */
