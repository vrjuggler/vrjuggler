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

#ifndef _VPR_SOCKET_BASIC_OPT_H_
#define _VPR_SOCKET_BASIC_OPT_H_

#include <IO/Socket/SocketOptions.h>
#include <vprStatus.h>


namespace vpr {

class SocketBasicOpt {
public:
    /**
     *
     */
    inline Status
    getKeepAlive (bool& enabled) {
        SocketOptions::Data option;
        Status retval;

        retval = getOption(SocketOptions::KeepAlive, option);

        if ( retval.success() ) {
            enabled = option.keep_alive;
        }

        return retval;
    }

    /**
     *
     */
    inline Status
    setKeepAlive (const bool enable_val) {
        SocketOptions::Data option;
        option.keep_alive = enable_val;
        return setOption(SocketOptions::KeepAlive, option);
    }

    /**
     *
     */
    inline Status
    getLingerOnClose (bool& enabled, int& linger_sec) {
        SocketOptions::Data opt;
        Status retval;

        retval = getOption(SocketOptions::Linger, opt);

        if ( retval.success() ) {
            enabled    = opt.linger.enabled;
            linger_sec = opt.linger.seconds;
        }

        return retval;
    }

    /**
     *
     */
    inline Status
    setLingerOnClose (const bool enable_val, const int linger_sec) {
        SocketOptions::Data opt;

        opt.linger.enabled = enable_val;
        opt.linger.seconds = linger_sec;

        return setOption(SocketOptions::Linger, opt);
    }

    /**
     *
     */
    inline Status
    getRecvBufferSize (Int32& size) {
        SocketOptions::Data opt;
        Status retval;

        retval = getOption(SocketOptions::RecvBufferSize, opt);

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
    inline Status
    setRecvBufferSize (const Int32 size) {
        SocketOptions::Data opt;

        opt.recv_buffer_size = size;

        return setOption(SocketOptions::RecvBufferSize, opt);
    }

    /**
     *
     */
    inline Status
    getSendBufferSize (int& size) {
        SocketOptions::Data opt;
        Status retval;

        retval = getOption(SocketOptions::SendBufferSize, opt);

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
    inline Status
    setSendBufferSize (const Int32 size) {
        SocketOptions::Data opt;

        opt.send_buffer_size = size;

        return setOption(SocketOptions::SendBufferSize, opt);
    }

    /**
     *
     */
    inline Status
    getReuseAddr (bool& enabled) {
        SocketOptions::Data option;
        Status retval;

        retval = getOption(SocketOptions::ReuseAddr, option);

        if ( retval.success() ) {
            enabled = option.reuse_addr;
        }

        return retval;
    }

    /**
     * Enable reuse of the address that will be bound by this socket.
     *
     * PRE: The socket has been opened, but bind() has not been called.
     */
    inline Status
    setReuseAddr (const bool enable_val) {
        SocketOptions::Data option;
        option.reuse_addr = enable_val;
        return setOption(SocketOptions::ReuseAddr, option);
    }

protected:
    /**
     *
     */
    virtual Status getOption(const SocketOptions::Types option,
                             struct SocketOptions::Data& data) = 0;

    /**
     *
     */
    virtual Status setOption(const SocketOptions::Types option,
                             const struct SocketOptions::Data& data) = 0;
};

}; // End of vpr namespace


#endif	/* _VPR_SOCKET_OPT_H_ */
