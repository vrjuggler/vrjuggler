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

#ifndef _VPR_SOCKET_BASIC_OPT_H_
#define _VPR_SOCKET_BASIC_OPT_H_

#include <vpr/vprConfig.h>

#include <vpr/vprTypes.h>
#include <vpr/IO/Socket/SocketOptions.h>
#include <vpr/Util/ReturnStatus.h>


namespace vpr {


/**
 * Options common to all types of sockets.
 */
class VPR_CLASS_API SocketBasicOpt : public vpr::SocketOptionWrapper {
public:
    /**
     *
     */
    inline vpr::ReturnStatus
    getKeepAlive (bool& enabled) {
        vpr::SocketOptions::Data option;
        vpr::ReturnStatus retval;

        retval = getOption(vpr::SocketOptions::KeepAlive, option);

        if ( retval.success() ) {
            enabled = option.keep_alive;
        }

        return retval;
    }

    /**
     *
     */
    inline vpr::ReturnStatus
    setKeepAlive (const bool enable_val) {
        vpr::SocketOptions::Data option;
        option.keep_alive = enable_val;
        return setOption(vpr::SocketOptions::KeepAlive, option);
    }

    /**
     *
     */
    inline vpr::ReturnStatus
    getLingerOnClose (bool& enabled, int& linger_sec) {
        vpr::SocketOptions::Data opt;
        vpr::ReturnStatus retval;

        retval = getOption(vpr::SocketOptions::Linger, opt);

        if ( retval.success() ) {
            enabled    = opt.linger.enabled;
            linger_sec = opt.linger.seconds;
        }

        return retval;
    }

    /**
     *
     */
    inline vpr::ReturnStatus
    setLingerOnClose (const bool enable_val, const int linger_sec) {
        vpr::SocketOptions::Data opt;

        opt.linger.enabled = enable_val;
        opt.linger.seconds = linger_sec;

        return setOption(vpr::SocketOptions::Linger, opt);
    }

    /**
     *
     */
    inline vpr::ReturnStatus
    getRecvBufferSize (Int32& size) {
        vpr::SocketOptions::Data opt;
        vpr::ReturnStatus retval;

        retval = getOption(vpr::SocketOptions::RecvBufferSize, opt);

        if ( retval.success() ) {
            size = opt.recv_buffer_size;
        }
        else {
            size = -1;
        }

        return retval;
    }

    /**
     *
     */
    inline vpr::ReturnStatus
    setRecvBufferSize (const Int32 size) {
        vpr::SocketOptions::Data opt;

        opt.recv_buffer_size = size;

        return setOption(vpr::SocketOptions::RecvBufferSize, opt);
    }

    /**
     *
     */
    inline vpr::ReturnStatus
    getSendBufferSize (int& size) {
        vpr::SocketOptions::Data opt;
        vpr::ReturnStatus retval;

        retval = getOption(vpr::SocketOptions::SendBufferSize, opt);

        if ( retval.success() ) {
            size = opt.send_buffer_size;
        }
        else {
            size = -1;
        }

        return retval;
    }

    /**
     *
     */
    inline vpr::ReturnStatus
    setSendBufferSize (const Int32 size) {
        vpr::SocketOptions::Data opt;

        opt.send_buffer_size = size;

        return setOption(vpr::SocketOptions::SendBufferSize, opt);
    }

    /**
     *
     */
    inline vpr::ReturnStatus
    getReuseAddr (bool& enabled) {
        vpr::SocketOptions::Data option;
        vpr::ReturnStatus retval;

        retval = getOption(vpr::SocketOptions::ReuseAddr, option);

        if ( retval.success() ) {
            enabled = option.reuse_addr;
        }

        return retval;
    }

    /**
     * Enables reuse of the address that will be bound by the socket.
     *
     * @pre The socket has been opened, but bind() has not been called.
     */
    inline vpr::ReturnStatus
    setReuseAddr (const bool enable_val) {
        vpr::SocketOptions::Data option;
        option.reuse_addr = enable_val;
        return setOption(vpr::SocketOptions::ReuseAddr, option);
    }
};

}; // End of vpr namespace


#endif	/* _VPR_SOCKET_OPT_H_ */
