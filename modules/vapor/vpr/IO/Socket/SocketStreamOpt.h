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

#ifndef _VPR_SOCKET_STREAM_OPT_H_
#define _VPR_SOCKET_STREAM_OPT_H_

#include <IO/Socket/SocketIpOpt.h>


namespace vpr {

class SocketStreamOpt : public SocketIpOpt {
public:
    /**
     *
     */
    inline int
    getMaxSegmentSize (Int32& size) {
        SocketOptions::Data option;
        int retval;

        retval = getOption(SocketOptions::MaxSegment, option);

        if ( retval == 0 ) {
            size = option.max_segment;
        }

        return retval;
    }

    /**
     *
     */
    inline int
    setMaxSegmentSize (const Int32 size) {
        SocketOptions::Data option;
        option.max_segment = size;
        return setOption(SocketOptions::MaxSegment, option);
    }

    /**
     *
     */
    inline int
    getNoDelay (bool& enabled) {
        SocketOptions::Data option;
        int retval;

        retval = getOption(SocketOptions::NoDelay, option);

        if ( retval == 0 ) {
            enabled = option.no_delay;
        }

        return retval;
    }

    /**
     *
     */
    inline int
    setNoDelay (const bool enable_val) {
        SocketOptions::Data option;
        option.no_delay = enable_val;
        return setOption(SocketOptions::NoDelay, option);
    }
};

}; // End of vpr namespace


#endif	/* _VPR_SOCKET_STREAM_OPT_H_ */
