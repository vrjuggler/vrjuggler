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

#ifndef _VPR_CONFIG_H_
#define _VPR_CONFIG_H_
/* #pragma once */

/*
 * ----------------------------------------------------------------------------
 * This file (vprConfig.h) includes header files common to most, if not all,
 * files in the VPR source tree.  It must be included at the top of every .h
 * and .cpp file before any other headers because it includes system headers.
 * ----------------------------------------------------------------------------
 */

/* This should always be included first. */
#include <vpr/vprDefines.h>
#include <vpr/vprTypes.h>

#ifdef _DEBUG
#   define VPR_DEBUG
#else
#   define VPR_OPT
#endif

#ifdef WIN32
/* Exclude rarely-used stuff from Windows headers */
#define WIN32_LEAN_AND_MEAN

/* identifier truncated to 255 characters in the debug information */
#pragma warning(disable:4786)

#include <windows.h>

/* extern HINSTANCE g_hInst = NULL; */
#endif   /* WIN32 */


/* Common C++ includes. */
/* Put here for pre-compiled headers. */
#ifdef __cplusplus
#include <iostream>
#include <iomanip>
#include <fstream>

#endif   /* __cplusplus */

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

/*
 * ----------------------------------------------------------------------------
 * DLL-related macros.  These are based on the macros used by NSPR.  Use
 * VPR_EXTERN for the prototype and VPR_IMPLEMENT for the implementation.
 * ----------------------------------------------------------------------------
 */
#ifdef VPR_OS_Win32

#   if defined(__GNUC__)
#       undef _declspec
#       define _declspec(x) __declspec(x)
#   endif

#   define VPR_EXPORT(__type)      _declspec(dllexport) __type
#   define VPR_EXPORT_CLASS        _declspec(dllexport)
#   define VPR_EXPORT_DATA(__type) _declspec(dllexport) __type
#   define VPR_IMPORT(__type)      _declspec(dllimport) __type
#   define VPR_IMPORT_DATA(__type) _declspec(dllimport) __type
#   define VPR_IMPORT_CLASS        _declspec(dllimport)

#   define VPR_EXTERN(__type)         extern _declspec(dllexport) __type
#   define VPR_IMPLEMENT(__type)      _declspec(dllexport) __type
#   define VPR_EXTERN_DATA(__type)    extern _declspec(dllexport) __type
#   define VPR_IMPLEMENT_DATA(__type) _declspec(dllexport) __type

#   define VPR_CALLBACK
#   define VPR_CALLBACK_DECL
#   define VPR_STATIC_CALLBACK(__x) static __x

#elif defined(VPR_OS_Mac)

#   define VPR_EXPORT(__type)		__declspec(export) __type
#   define VPR_EXPORT_CLASS		__declspec(export)
#   define VPR_EXPORT_DATA(__type)	__declspec(export) __type
#   define VPR_IMPORT(__type)		__declspec(export) __type
#   define VPR_IMPORT_CLASS		__declspec(export)
#   define VPR_IMPORT_DATA(__type)	__declspec(export) __type

#   define VPR_EXTERN(__type)		extern __declspec(export) __type
#   define VPR_IMPLEMENT(__type)	__declspec(export) __type
#   define VPR_EXTERN_DATA(__type)	extern __declspec(export) __type
#   define VPR_IMPLEMENT_DATA(__type)	__declspec(export) __type

#   define VPR_CALLBACK
#   define VPR_CALLBACK_DECL
#   define VPR_STATIC_CALLBACK(__x) static __x

#else   /* UNIX (where this stuff is simple!) */

#   define VPR_EXPORT(__type)      __type
#   define VPR_EXPORT_CLASS
#   define VPR_EXPORT_DATA(__type) __type
#   define VPR_IMPORT(__type)      __type
#   define VPR_IMPORT_DATA(__type) __type
#   define VPR_IMPORT_CLASS

#   define VPR_EXTERN(__type)         extern __type
#   define VPR_IMPLEMENT(__type)      __type
#   define VPR_EXTERN_DATA(__type)    extern __type
#   define VPR_IMPLEMENT_DATA(__type) __type

#   define VPR_CALLBACK
#   define VPR_CALLBACK_DECL
#   define VPR_STATIC_CALLBACK(__x) static __x

#endif	/* VPR_OS_Win32 */

#ifdef _VPR_BUILD_
#   define VPR_API(__type)	VPR_EXPORT(__type)
#   define VPR_CLASS_API	VPR_EXPORT_CLASS
#   define VPR_DATA_API(__type)	VPR_EXPORT_DATA(__type)
#else
#   define VPR_API(__type)	VPR_IMPORT(__type)
#   define VPR_CLASS_API	VPR_IMPORT_CLASS
#   define VPR_DATA_API(__type)	VPR_IMPORT_DATA(__type)
#endif

#include <vpr/vprDomain.h>

#endif   /* _VPR_CONFIG_H_ */
