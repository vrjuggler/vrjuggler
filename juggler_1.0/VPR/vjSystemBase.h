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

#ifndef _VJ_SYSTEM_BASE_H_
#define _VJ_SYSTEM_BASE_H_

#include <vjConfig.h>

#include <math.h>

#include <VPR/Threads/vjThread.h>

// vjSystemBase is a base class for vjSystem
// so, for xplatform system functions, use vjSystem (don't use vjSystemBase)
// see the end of this (VPR/vjSystemBase.h) file for details...
class vjSystemBase {
public:
    // -----------------------------------------------------------------------
    // -----------------------------------------------------------------------
    inline static int
    usleep (u_int32_t micro) {
        return vjThread::usleep(micro);
    }

    // -----------------------------------------------------------------------
    // -----------------------------------------------------------------------
    inline static int
    msleep (u_int32_t milli) {
        return vjThread::msleep(milli);
    }

    // -----------------------------------------------------------------------
    // -----------------------------------------------------------------------
    inline static int
    sleep (u_int32_t seconds) {
        return vjThread::sleep(seconds);
    }

    // -----------------------------------------------------------------------
    //: Determine the endianness of the host platform.  A return nvalue of 0
    //+ means that the host uses little-endian byte order.  A return value of
    //+ 1 means that the host uses big-endian byte order.
    //
    //! RETURNS: 0 - Little endian
    //! RETURNS: 1 - Big endian
    // -----------------------------------------------------------------------
    inline static int
    getEndian (void) {
        union {
            char   c[sizeof(short)];
            short  value;
        } endian;

        // The way this works is that we access the first byte of endian.value
        // directly.  If it is 1, the host treats that as the high-order byte.
        // Otherwise, it is the low-order byte.
        endian.value = 256;

        return endian.c[0];
    }

    // -----------------------------------------------------------------------
    //: Tells if the host uses little-endian byte order or not.
    //
    //! RETURNS: true  - Little-endian host.
    //! RETURNS: false - Big-endian host.
    // -----------------------------------------------------------------------
    inline static bool
    isLittleEndian (void) {
        return (getEndian() == 0);
    }

    // -----------------------------------------------------------------------
    //: Tells if the host uses big-endian byte order or not.
    //
    //! RETURNS: true  - Big-endian host.
    //! RETURNS: false - Little-endian host.
    // -----------------------------------------------------------------------
    inline static bool
    isBigEndian (void) {
        return (getEndian() == 1);
    }
};

#endif   /* _VJ_SYSTEM_BASE_H_ */
