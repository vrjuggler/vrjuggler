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

#ifndef _VPR_SOCKET_IP_OPT_H_
#define _VPR_SOCKET_IP_OPT_H_

#include <vpr/IO/Socket/SocketBasicOpt.h>


namespace vpr {

/**
 * IP-level options for sockets.
 *
 * @author Patrick Hartling
 */
class VPR_CLASS_API SocketIpOpt : public SocketBasicOpt {
public:
    /**
     *
     */
    inline vpr::Status
    getTypeOfService (vpr::SocketOptions::TypeOfService& tos) {
        vpr::SocketOptions::Data option;
        vpr::Status retval;

        retval = getOption(vpr::SocketOptions::IpTypeOfService, option);

        if ( retval.success() ) {
            tos = option.type_of_service;
        }

        return retval;
    }

    /**
     *
     */
    inline vpr::Status
    setTypeOfService (const vpr::SocketOptions::TypeOfService& tos) {
        vpr::SocketOptions::Data option;
        option.type_of_service = tos;
        return setOption(vpr::SocketOptions::IpTypeOfService, option);
    }

    /**
     *
     */
    inline vpr::Status
    getTimeToLive (vpr::Int32& ttl) {
        vpr::SocketOptions::Data option;
        vpr::Status retval;

        retval = getOption(vpr::SocketOptions::IpTimeToLive, option);

        if ( retval.success() ) {
            ttl = option.ip_ttl;
        }

        return retval;
    }

    /**
     *
     */
    inline vpr::Status
    setTimeToLive (const vpr::Int32 ttl) {
        vpr::SocketOptions::Data option;
        option.ip_ttl = ttl;
        return setOption(vpr::SocketOptions::IpTimeToLive, option);
    }
};

}; // End of vpr namespace


#endif	/* _VPR_SOCKET_IP_OPT_H_ */
