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

#ifndef _VRJ_DRAW_OGL_CONFIG_H_
#define _VRJ_DRAW_OGL_CONFIG_H_
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
#include <vrj/vrjConfig.h>

/* Get rid of symbols added by Autoconf 2.5x. */
#undef PACKAGE_BUGREPORT
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_VERSION

#if !defined(WIN32) && !defined(WIN64)          \
      && defined(__GNUC__) && __GNUC__ >= 4     \
      && !defined(VJ_OGL_HAVE_GCC_VISIBILITY)
#  define VJ_OGL_HAVE_GCC_VISIBILITY
#endif

/*
 * ----------------------------------------------------------------------------
 * DLL-related macros.  These are based on the macros used by NSPR.  Use
 * VJ_OGL_EXTERN for the prototype and VJ_OGL_IMPLEMENT for the implementation.
 * ----------------------------------------------------------------------------
 */
#if defined(WIN32) || defined(WIN64)

#   if defined(__GNUC__)
#       undef _declspec
#       define _declspec(x) __declspec(x)
#   endif

#   define VJ_OGL_EXPORT(__type)      _declspec(dllexport) __type
#   define VJ_OGL_EXPORT_CLASS        _declspec(dllexport)
#   define VJ_OGL_EXPORT_DATA(__type) _declspec(dllexport) __type
#   define VJ_OGL_IMPORT(__type)      _declspec(dllimport) __type
#   define VJ_OGL_IMPORT_DATA(__type) _declspec(dllimport) __type
#   define VJ_OGL_IMPORT_CLASS        _declspec(dllimport)

#   define VJ_OGL_EXTERN(__type)         extern _declspec(dllexport) __type
#   define VJ_OGL_IMPLEMENT(__type)      _declspec(dllexport) __type
#   define VJ_OGL_EXTERN_DATA(__type)    extern _declspec(dllexport) __type
#   define VJ_OGL_IMPLEMENT_DATA(__type) _declspec(dllexport) __type

#   define VJ_OGL_CALLBACK
#   define VJ_OGL_CALLBACK_DECL
#   define VJ_OGL_STATIC_CALLBACK(__x) static __x

#elif defined(VJ_OGL_HAVE_GCC_VISIBILITY)

#   define VJ_OGL_EXPORT(__type)      __attribute__ ((visibility("default"))) __type
#   define VJ_OGL_EXPORT_CLASS        __attribute__ ((visibility("default")))
#   define VJ_OGL_EXPORT_DATA(__type) __attribute__ ((visibility("default"))) __type
#   define VJ_OGL_IMPORT(__type)      __type
#   define VJ_OGL_IMPORT_DATA(__type) __type
#   define VJ_OGL_IMPORT_CLASS        

#   define VJ_OGL_EXTERN(__type)         extern __attribute__ ((visibility("default"))) __type
#   define VJ_OGL_IMPLEMENT(__type)      __attribute__ ((visibility("default"))) __type
#   define VJ_OGL_EXTERN_DATA(__type)    extern __attribute__ ((visibility("default"))) __type
#   define VJ_OGL_IMPLEMENT_DATA(__type) __attribute__ ((visibility("default"))) __type

#   define VJ_OGL_CALLBACK
#   define VJ_OGL_CALLBACK_DECL
#   define VJ_OGL_STATIC_CALLBACK(__x) static __x

#else   /* UNIX (where this stuff is simple!) */

#   define VJ_OGL_EXPORT(__type)      __type
#   define VJ_OGL_EXPORT_CLASS
#   define VJ_OGL_EXPORT_DATA(__type) __type
#   define VJ_OGL_IMPORT(__type)      __type
#   define VJ_OGL_IMPORT_CLASS
#   define VJ_OGL_IMPORT_DATA(__type) __type

#   define VJ_OGL_EXTERN(__type)         extern __type
#   define VJ_OGL_IMPLEMENT(__type)      __type
#   define VJ_OGL_EXTERN_DATA(__type)    extern __type
#   define VJ_OGL_IMPLEMENT_DATA(__type) __type

#   define VJ_OGL_CALLBACK
#   define VJ_OGL_CALLBACK_DECL
#   define VJ_OGL_STATIC_CALLBACK(__x) static __x

#endif  /* WIN32 || WIN64 */

#ifdef _VRJ_OGL_BUILD_
#   define VJ_OGL_API(__type)           VJ_OGL_EXPORT(__type)
#   define VJ_OGL_CLASS_API             VJ_OGL_EXPORT_CLASS
#   define VJ_OGL_DATA_API(__type)      VJ_OGL_EXPORT_DATA(__type)
#else
#   define VJ_OGL_API(__type)           VJ_OGL_IMPORT(__type)
#   define VJ_OGL_CLASS_API             VJ_OGL_IMPORT_CLASS
#   define VJ_OGL_DATA_API(__type)      VJ_OGL_IMPORT_DATA(__type)

#   if defined(WIN32) || defined(WIN64)
#      if defined(VJ_DEBUG) && ! defined(_USE_RELEASE_RUNTIME)
#         pragma comment(lib, "vrj_ogl_d.lib")
#      else
#         pragma comment(lib, "vrj_ogl.lib")
#      endif
#   endif
#endif

#endif   /* _VRJ_DRAW_OGL_CONFIG_H_ */
