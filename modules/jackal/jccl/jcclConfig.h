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

#ifndef _JCCL_CONFIG_H_
#define _JCCL_CONFIG_H_
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
#include <jccl/jcclDefines.h>

#ifdef _DEBUG
#   define JCCL_DEBUG
#else
#   define JCCL_OPT
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

/*
 * ----------------------------------------------------------------------------
 * DLL-related macros.  These are based on the macros used by NSPR.  Use
 * VJ_EXTERN for the prototype and VJ_IMPLEMENT for the implementation.
 * ----------------------------------------------------------------------------
 */
#ifdef VJ_OS_Win32

#   if defined(__GNUC__)
#       undef _declspec
#       define _declspec(x) __declspec(x)
#   endif

#   define VJ_EXPORT(__type)      _declspec(dllexport) __type
#   define VJ_EXPORT_CLASS        _declspec(dllexport)
#   define VJ_EXPORT_DATA(__type) _declspec(dllexport) __type
#   define VJ_IMPORT(__type)      _declspec(dllimport) __type
#   define VJ_IMPORT_DATA(__type) _declspec(dllimport) __type
#   define VJ_IMPORT_CLASS        _declspec(dllimport)

#   define VJ_EXTERN(__type)         extern _declspec(dllexport) __type
#   define VJ_IMPLEMENT(__type)      _declspec(dllexport) __type
#   define VJ_EXTERN_DATA(__type)    extern _declspec(dllexport) __type
#   define VJ_IMPLEMENT_DATA(__type) _declspec(dllexport) __type

#   define VJ_CALLBACK
#   define VJ_CALLBACK_DECL
#   define VJ_STATIC_CALLBACK(__x) static __x

#else   /* UNIX (where this stuff is simple!) */

#   define VJ_EXPORT(__type)      __type
#   define VJ_EXPORT_CLASS
#   define VJ_EXPORT_DATA(__type) __type
#   define VJ_IMPORT(__type)      __type
#   define VJ_IMPORT_CLASS
#   define VJ_IMPORT_DATA(__type) __type

#   define VJ_EXTERN(__type)         extern __type
#   define VJ_IMPLEMENT(__type)      __type
#   define VJ_EXTERN_DATA(__type)    extern __type
#   define VJ_IMPLEMENT_DATA(__type) __type

#   define VJ_CALLBACK
#   define VJ_CALLBACK_DECL
#   define VJ_STATIC_CALLBACK(__x) static __x

#endif	/* VJ_OS_Win32 */

#ifdef _VRJ_BUILD_
#   define VJ_API(__type)	VJ_EXPORT(__type)
#   define VJ_CLASS_API		VJ_EXPORT_CLASS
#   define VJ_DATA_API(__type)	VJ_EXPORT_DATA(__type)
#else
#   define VJ_API(__type)	VJ_IMPORT(__type)
#   define VJ_CLASS_API		VJ_IMPORT_CLASS
#   define VJ_DATA_API(__type)	VJ_IMPORT_DATA(__type)
#endif

#endif   /* _VJ_CONFIG_H_ */
