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

#include <vpr/vprConfig.h>

#include <vpr/vprTypes.h>
#include <vpr/IO/Selector.h>
#include <vpr/IO/BlockIO.h>


namespace vpr {

/**
 * Tests if reading from this I/O device will block.
 */
bool
BlockIO::isReadBlocked (const vpr::Interval& timeout) {
    bool is_blocked;
    vpr::Selector selector;
    vpr::IOSys::Handle handle;
    vpr::Uint16 num_events;
    vpr::Status status;

    handle = getHandle();
    selector.addHandle(handle);
    selector.setIn(handle, vpr::Selector::Read);

    // Test the handle to get its read state.
    status = selector.select(num_events, timeout);

    if ( num_events == 1 ) {
        is_blocked = false;
    }
    else {
        is_blocked = true;
    }

    return is_blocked;
}

/**
 * Tests if writing to this I/O device will block.
 */
bool
BlockIO::isWriteBlocked (const vpr::Interval& timeout) {
    bool is_blocked;
    vpr::Selector selector;
    vpr::IOSys::Handle handle;
    vpr::Uint16 num_events;
    vpr::Status status;

    handle = getHandle();
    selector.addHandle(handle);
    selector.setIn(handle, vpr::Selector::Write);

    // Test the handle to get its write state.
    status = selector.select(num_events, timeout);

    if ( num_events == 1 ) {
        is_blocked = false;
    }
    else {
        is_blocked = true;
    }

    return is_blocked;
}

} // End of vpr namespace
