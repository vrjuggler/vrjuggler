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

#ifndef _VPR_SOCKET_STREAM_OPT_H_
#define _VPR_SOCKET_STREAM_OPT_H_

#include <vpr/vprConfig.h>

#include <vpr/vprTypes.h>
#include <vpr/IO/Socket/SocketOptions.h>
#include <vpr/IO/Socket/SocketIpOpt.h>
#include <vpr/Util/ReturnStatus.h>


namespace vpr {

/**
 * Options for stream sockets.
 *
 * @author Patrick Hartling
 */
class VPR_CLASS_API SocketStreamOpt : public vpr::SocketOptionWrapper {
public:
    /**
     * Default constructor.  This does nothing and exists only so that we can
     * build a DLL on Win32.
     */
    SocketStreamOpt(void);

    /**
     * Default destructor.  This does nothing and exists only so that we can
     * build a DLL on Win32.
     */
    virtual ~SocketStreamOpt(void);

    /**
     *
     */
    inline vpr::ReturnStatus
    getMaxSegmentSize (vpr::Int32& size) {
        vpr::SocketOptions::Data option;
        vpr::ReturnStatus retval;

        retval = getOption(vpr::SocketOptions::MaxSegment, option);

        if ( retval.success() ) {
            size = option.max_segment;
        }

        return retval;
    }

    /**
     *
     */
    inline vpr::ReturnStatus
    setMaxSegmentSize (const vpr::Int32 size) {
        vpr::SocketOptions::Data option;
        option.max_segment = size;
        return setOption(vpr::SocketOptions::MaxSegment, option);
    }

    /**
     * Gets the current no-delay status for this socket.  If no-delay is true,
     * then the Nagel algorithm has been disabled.
     *
     * @param enabled A reference to a <code>bool</code> variable to be used as
     *                storage for the current no-delay enable state.  If the
     *                value is <code>true</code>, the Nagel algorithm is
     *                <i>not</i> being used to delay the transmission of TCP
     *                segements.  Otherwise, the Nagel alorithm is delaying
     *                the transmission.
     */
    inline vpr::ReturnStatus
    getNoDelay (bool& enabled) {
        vpr::SocketOptions::Data option;
        vpr::ReturnStatus retval;

        retval = getOption(vpr::SocketOptions::NoDelay, option);

        if ( retval.success() ) {
            enabled = option.no_delay;
        }

        return retval;
    }

    /**
     * Sets the current no-delay status for this socket.  If no-delay is true,
     * then the Nagel algorithm will be disabled.
     *
     * @param enable_val The Boolean enable/disable state for no-delay on this
     *                   socket.
     */
    inline vpr::ReturnStatus
    setNoDelay (const bool enable_val) {
        vpr::SocketOptions::Data option;
        option.no_delay = enable_val;
        return setOption(vpr::SocketOptions::NoDelay, option);
    }
};

}; // End of vpr namespace


#endif	/* _VPR_SOCKET_STREAM_OPT_H_ */
