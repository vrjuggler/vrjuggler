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

#pragma warning(disable:4244)	/* Converting double to float */
#pragma warning(disable:4800)	/* Int forcing value to bool */
#pragma warning(disable:4101)	/* unreferenced local variable */

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

#endif   /* __cplusplus */

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

/* --- Macros ---- */

#define VJ_EPS 1e-8
#define VJ_DEG2RAD(x) (x * 0.01745329252f) /* M_PI / 180.0 */
#define VJ_RAD2DEG(x) (x * 57.2957795131f) /* 180.0 / M_PI */
#define VJ_ZERO_CLAMP(x) ((vjSystem::abs(x) < VJ_EPS)? 0.0f : x)
#define VJ_IS_ZERO(x) (vjSystem::abs(x) < VJ_EPS)
#define VJ_CLAMP(x,y) ((x>y)? y : x)
#define VJ_MIN2(x,y) ((x>y)? y : x)
#define VJ_MIN3(x,y,z) VJ_MIN2(VJ_MIN2(x,y),z)
#define VJ_MIN4(w,x,y,z) VJ_MIN2(VJ_MIN2(w,x), VJ_MIN2(y,z))
#define VJ_MAX2(x,y) ((x>y)? x : y)
#define VJ_MAX3(x,y,z) VJ_MAX2(VJ_MAX2(x,y),z)
#define VJ_MAX4(w,x,y,z) VJ_MAX2(VJ_MAX2(w,x),VJ_MAX2(y,z))


#endif   /* _VJ_CONFIG_H_ */
