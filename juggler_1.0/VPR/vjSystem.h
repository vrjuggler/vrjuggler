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

#ifndef _VJ_SYSTEM_H_
#define _VJ_SYSTEM_H_

#include <vjConfig.h>

#include <math.h>

#include <VPR/Threads/vjThread.h>


class vjSystemBase {
public:
    // -----------------------------------------------------------------------
    // -----------------------------------------------------------------------
    inline static double
    sin (const double x) {
        return ::sin(x);
    }

    // -----------------------------------------------------------------------
    // -----------------------------------------------------------------------
    inline static float
    sin (const float x) {
#ifdef HAVE_SINF
        return ::sinf(x);
#else
        return (float) ::sin(x);
#endif
    }

    // -----------------------------------------------------------------------
    // -----------------------------------------------------------------------
    inline static double
    asin (const double x) {
        return ::asin(x);
    }

    // -----------------------------------------------------------------------
    // -----------------------------------------------------------------------
    inline static float
    asin (const float x) {
#ifdef HAVE_ASINF
        return ::asinf(x);
#else
        return (float) ::asin(x);
#endif
    }

    // -----------------------------------------------------------------------
    // -----------------------------------------------------------------------
    inline static double
    cos (const double x) {
        return ::cos(x);
    }

    // -----------------------------------------------------------------------
    // -----------------------------------------------------------------------
    inline static float
    cos (const float x) {
#ifdef HAVE_COSF
        return ::cosf(x);
#else
        return (float) ::cos(x);
#endif
    }

    // -----------------------------------------------------------------------
    // -----------------------------------------------------------------------
    inline static double
    acos (const double x) {
        return ::acos(x);
    }

    // -----------------------------------------------------------------------
    // -----------------------------------------------------------------------
    inline static float
    acos (const float x) {
#ifdef HAVE_ACOSF
        return ::acosf(x);
#else
        return (float) ::acos(x);
#endif
    }

    // -----------------------------------------------------------------------
    // -----------------------------------------------------------------------
    inline static double
    atan2 (const double x, const double y) {
        return ::atan2(x, y);
    }

    // -----------------------------------------------------------------------
    // -----------------------------------------------------------------------
    inline static float
    atan2 (const float x, const float y) {
#ifdef HAVE_ATAN2F
        return ::atan2f(x, y);
#else
        return (float) ::atan2(x, y);
#endif
    }

    // -----------------------------------------------------------------------
    // -----------------------------------------------------------------------
    inline static double
    sqrt (const double x) {
        return ::sqrt(x);
    }

    // -----------------------------------------------------------------------
    // -----------------------------------------------------------------------
    inline static float
    sqrt (const float x) {
#ifdef HAVE_SQRTF
        return ::sqrtf(x);
#else
        return (float) ::sqrt(x);
#endif
    }

    // -----------------------------------------------------------------------
    // -----------------------------------------------------------------------
    inline static double
    fabs (const double x) {
        return ::fabs(x);
    }

    // -----------------------------------------------------------------------
    // -----------------------------------------------------------------------
    inline static float
    fabs (const float x) {
#ifdef HAVE_FABSF
        return ::fabsf(x);
#else
        return (float) ::fabs(x);
#endif
    }

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
        return vjThread::usleep(milli);
    }

    // -----------------------------------------------------------------------
    // -----------------------------------------------------------------------
    inline static int
    sleep (u_int32_t seconds) {
        return vjThread::sleep(seconds);
    }
};


#if defined(VJ_IRIX_SPROC) || defined(VJ_USE_PTHREADS)
#   include <VPR/md/POSIX/vjSystemPosix.h>

    typedef class vjSystemPosix vjSystem;
#elif defined(VJ_OS_Win32)
#   include <VPR/md/WIN32/vjSystemWin32.h>

    typedef class vjSystemWin32 vjSystem;
#else
#   include <VPR/md/NSPR/vjSystemNSPR.h>

    typedef class vjSystemNSPR vjSystem;
#endif  /* VJ_IRIX_SPROC || VJ_USE_PTHREADS */

#endif   /* _VJ_SYSTEM_H_ */
