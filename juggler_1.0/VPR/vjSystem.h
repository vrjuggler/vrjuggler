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


//: Linear Interpolation
// use double or float only...
template <class dataType>
inline void VJ_LERP( dataType& result, const dataType& lerp, const dataType& a,
                  const dataType& b )
{
    dataType size = b - a;
    result = a + (size * lerp);
}
/* #include <Utils/vjDebug.h> */

//: Is almost equal?
// test for equality with tolerance...
template <class dataType>
inline bool VJ_ISEQUAL( const dataType& a, const dataType& b, const dataType& tolerance )
{
   return vjSystem::abs( a - b ) <= tolerance;
}


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
    inline static double atan( const double x )
    {
        return ::atan( x );
    }

    // -----------------------------------------------------------------------
    // -----------------------------------------------------------------------
    inline static float atan( const float x )
    {
#ifdef HAVE_ATAN2F
        return ::atanf( x );
#else
        return (float) ::atan( x );
#endif
    }

    // -----------------------------------------------------------------------
    // -----------------------------------------------------------------------
    inline static double tan( const double x )
    {
        return ::tan( x );
    }

    // -----------------------------------------------------------------------
    // -----------------------------------------------------------------------
    inline static float tan( const float x )
    {
#ifdef HAVE_ATAN2F
        return ::tanf( x );
#else
        return (float) ::tan( x );
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
    abs (const double x) {
        return ::fabs(x);
    }

    // -----------------------------------------------------------------------
    // -----------------------------------------------------------------------
    inline static float
    abs (const float x) {
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

    // -----------------------------------------------------------------------
    //: return a random number between 0.0f and 1.0f
    //
    //! RETURNS: random number between 0 and 1
    // -----------------------------------------------------------------------
    inline float random()
    {
       // rand returns int from  0 - RAND_MAX
       const float rand_max( RAND_MAX );
       float r = static_cast<float>( rand() );
       r /= rand_max;
       return r;
    }

    //: return a random number between x1 and x2
    //! RETURNS: random number between x1 and x2
    inline float random( float x1, float x2 )
    {
       float r = vjSystemBase::random();
       float size = x2 - x1;
       return r * size + x1;
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
