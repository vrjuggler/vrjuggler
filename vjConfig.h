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

#ifndef _VJ_CONFIG_H_
#define _VJ_CONFIG_H_
/* #pragma once */

/*
 * ----------------------------------------------------------------------------
 * This file (vjConfig.h) includes header files common to most, if not all,
 * files in the VR Juggler source tree.  It must be included at the top of
 * every .h and .cpp file before any other headers because it includes system
 * headers.
 * ----------------------------------------------------------------------------
 */

/* This should always be included first. */
#include <vjDefines.h>

#ifdef _DEBUG
#   define VJ_DEBUG
#else
#   define VJ_OPT
#endif

#ifdef WIN32
/* Exclude rarely-used stuff from Windows headers */
#define WIN32_LEAN_AND_MEAN

/* identifier truncated to 255 characters in the debug information */
#pragma warning(disable:4786)

#include <windows.h>

#ifndef HAVE_STRCASECMP
#define strcasecmp _stricmp
#endif

#define M_PI   3.14159265358979323846
#define M_PI_2 1.57079632679489661923
#define M_PI_4 0.78539816339744830962

/* extern HINSTANCE g_hInst = NULL; */
#endif   /* WIN32 */


/* Common C++ includes. */
/* Put here for pre-compiled headers. */
#ifdef __cplusplus
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <math.h>

//: Linear Interpolation
inline void VJ_LERP( float& result, const float& lerp, const float& a, 
                  const float& b )
{
    float size = b - a;
    result = a + (size * lerp);
}
/* #include <Kernel/vjDebug.h> */
#endif   /* __cplusplus */

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

/* --- Macros ---- */

/* Define this macro to replace calls to sleep(3) if it is not avialable. */
#ifndef HAVE_SLEEP
#   define sleep(x) (Sleep(x * 1000))        /* Win32-specific */
#endif

/* Define this macro to replace calls to usleep(3) if it is not avialable. */
#ifndef HAVE_USLEEP
#   define usleep(x) (Sleep(x / 1000))       /* Win32-specific */
#endif

#ifndef HAVE_SINF
#   define sinf(x) ((float) sin(x))
#endif

#ifndef HAVE_ASINF
#   define asinf(x) ((float) asin(x))
#endif

#ifndef HAVE_COSF
#   define cosf(x) ((float) cos(x))
#endif

#ifndef HAVE_ACOSF
#   define acosf(x) ((float) acos(x))
#endif

#ifndef HAVE_ATAN2F
#   define atan2f(x, y) ((float) atan2(x, y))
#endif

#ifndef HAVE_SQRTF
#   define sqrtf(x) ((float) sqrt(x))
#endif

#ifndef HAVE_FABSF
#   define fabsf(x) ((float) fabs(x))
#endif

#define VJ_EPS 1e-8
#define VJ_DEG2RAD(x) (x * 0.01745329252f) /* M_PI / 180.0 */
#define VJ_RAD2DEG(x) (x * 57.2957795131f) /* 180.0 / M_PI */
#define VJ_ZERO_CLAMP(x) ((fabs(x) < VJ_EPS)? 0.0f : x)
#define VJ_IS_ZERO(x) (fabs(x) < VJ_EPS)
#define VJ_CLAMP(x,y) ((x>y)? y : x)
#define VJ_MIN2(x,y) ((x>y)? y : x)
#define VJ_MIN3(x,y,z) VJ_MIN2(VJ_MIN2(x,y),z)
#define VJ_MIN4(w,x,y,z) VJ_MIN2(VJ_MIN2(w,x), VJ_MIN2(y,z))
#define VJ_MAX2(x,y) ((x>y)? x : y)
#define VJ_MAX3(x,y,z) VJ_MAX2(VJ_MAX2(x,y),z)
#define VJ_MAX4(w,x,y,z) VJ_MAX2(VJ_MAX2(w,x),VJ_MAX2(y,z))


#endif   /* _VJ_CONFIG_H_ */
