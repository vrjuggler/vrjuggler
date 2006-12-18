/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

#ifndef _VRJ_DRAW_D3D_CONFIG_H_
#define _VRJ_DRAW_D3D_CONFIG_H_
/* #pragma once */

/**
 * \file
 *
 * This file is to be used by code in the Direct3D Draw Manager for ensuring
 * that platform-specific preprocessor symbols are defined correctly. In
 * particular, this file handles the Windows DLL and automatic linking
 * features for the Direct3D Draw Manager shared library. It must be included
 * at the top of every .h and .cpp file in the Direct3D Draw Manager source
 * before any other headers.
 */

/* This should always be included first. */
#include <vrj/vrjConfig.h>

/* Get rid of symbols added by Autoconf 2.5x. */
#undef PACKAGE_BUGREPORT
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_VERSION

#if !defined(WIN32) && !defined(WIN64)          \
      && defined(__GNUC__) && __GNUC__ >= 4     \
      && !defined(VJ_D3D_HAVE_GCC_VISIBILITY)
#  define VJ_D3D_HAVE_GCC_VISIBILITY
#endif

/*
 * ----------------------------------------------------------------------------
 * DLL-related macros.  These are based on the macros used by NSPR.  Use
 * VJ_D3D_EXTERN for the prototype and VJ_D3D_IMPLEMENT for the implementation.
 * ----------------------------------------------------------------------------
 */
#if defined(WIN32) || defined(WIN64)

#   if defined(__GNUC__)
#       undef _declspec
#       define _declspec(x) __declspec(x)
#   endif

#   define VJ_D3D_EXPORT(__type)      _declspec(dllexport) __type
#   define VJ_D3D_EXPORT_CLASS        _declspec(dllexport)
#   define VJ_D3D_EXPORT_DATA(__type) _declspec(dllexport) __type
#   define VJ_D3D_IMPORT(__type)      _declspec(dllimport) __type
#   define VJ_D3D_IMPORT_DATA(__type) _declspec(dllimport) __type
#   define VJ_D3D_IMPORT_CLASS        _declspec(dllimport)

#   define VJ_D3D_EXTERN(__type)         extern _declspec(dllexport) __type
#   define VJ_D3D_IMPLEMENT(__type)      _declspec(dllexport) __type
#   define VJ_D3D_EXTERN_DATA(__type)    extern _declspec(dllexport) __type
#   define VJ_D3D_IMPLEMENT_DATA(__type) _declspec(dllexport) __type

#   define VJ_D3D_CALLBACK
#   define VJ_D3D_CALLBACK_DECL
#   define VJ_D3D_STATIC_CALLBACK(__x) static __x

#elif defined(VJ_D3D_HAVE_GCC_VISIBILITY)

#   define VJ_D3D_EXPORT(__type)      __attribute__ ((visibility("default"))) __type
#   define VJ_D3D_EXPORT_CLASS        __attribute__ ((visibility("default")))
#   define VJ_D3D_EXPORT_DATA(__type) __attribute__ ((visibility("default"))) __type
#   define VJ_D3D_IMPORT(__type)      __type
#   define VJ_D3D_IMPORT_DATA(__type) __type
#   define VJ_D3D_IMPORT_CLASS        

#   define VJ_D3D_EXTERN(__type)         extern __attribute__ ((visibility("default"))) __type
#   define VJ_D3D_IMPLEMENT(__type)      __attribute__ ((visibility("default"))) __type
#   define VJ_D3D_EXTERN_DATA(__type)    extern __attribute__ ((visibility("default"))) __type
#   define VJ_D3D_IMPLEMENT_DATA(__type) __attribute__ ((visibility("default"))) __type

#   define VJ_D3D_CALLBACK
#   define VJ_D3D_CALLBACK_DECL
#   define VJ_D3D_STATIC_CALLBACK(__x) static __x

#else   /* UNIX (where this stuff is simple!) */

#   define VJ_D3D_EXPORT(__type)      __type
#   define VJ_D3D_EXPORT_CLASS
#   define VJ_D3D_EXPORT_DATA(__type) __type
#   define VJ_D3D_IMPORT(__type)      __type
#   define VJ_D3D_IMPORT_CLASS
#   define VJ_D3D_IMPORT_DATA(__type) __type

#   define VJ_D3D_EXTERN(__type)         extern __type
#   define VJ_D3D_IMPLEMENT(__type)      __type
#   define VJ_D3D_EXTERN_DATA(__type)    extern __type
#   define VJ_D3D_IMPLEMENT_DATA(__type) __type

#   define VJ_D3D_CALLBACK
#   define VJ_D3D_CALLBACK_DECL
#   define VJ_D3D_STATIC_CALLBACK(__x) static __x

#endif  /* WIN32 || WIN64 */

#ifdef _VRJ_D3D_BUILD_
#   define VJ_D3D_API(__type)           VJ_D3D_EXPORT(__type)
#   define VJ_D3D_CLASS_API             VJ_D3D_EXPORT_CLASS
#   define VJ_D3D_DATA_API(__type)      VJ_D3D_EXPORT_DATA(__type)
#else
#   define VJ_D3D_API(__type)           VJ_D3D_IMPORT(__type)
#   define VJ_D3D_CLASS_API             VJ_D3D_IMPORT_CLASS
#   define VJ_D3D_DATA_API(__type)      VJ_D3D_IMPORT_DATA(__type)

#   define VJ_LIB_NAME "vrj_d3d"
#   include <vrj/AutoLink.h>
#   undef VJ_LIB_NAME
#endif

#endif   /* _VRJ_DRAW_D3D_CONFIG_H_ */
