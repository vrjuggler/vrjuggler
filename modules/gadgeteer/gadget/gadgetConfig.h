/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _GADGET_CONFIG_H_
#define _GADGET_CONFIG_H_
/* #pragma once */

/*
 * ----------------------------------------------------------------------------
 * This file (gadgetConfig.h) includes header files common to most, if not
 * all, files in the Gadgeteer source tree.  It must be included at the top of
 * every .h and .cpp file before any other headers because it includes system
 * headers.
 * ----------------------------------------------------------------------------
 */

/* This should always be included first. */
#include <gadget/gadgetDefines.h>

/* Get rid of symbols added by Autoconf 2.5x. */
#undef PACKAGE_BUGREPORT
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_VERSION

/*
 * If either JUGGLER_DEBUG or _DEBUG is defined, that implies the need for
 * GADGET_DEBUG.
 */
#if ! defined(GADGET_DEBUG) && (defined(JUGGLER_DEBUG) || defined(_DEBUG))
#   define GADGET_DEBUG
/* If JUGGLER_OPT is defined, make sure that GADGET_OPT is also defined. */
#elif defined(JUGGLER_OPT) && ! defined(GADGET_OPT)
#   define GADGET_OPT
#endif

#if defined(WIN32) || defined(WIN64)

#ifndef WIN32_LEAN_AND_MEAN
/* Exclude rarely-used stuff from Windows headers */
#   define WIN32_LEAN_AND_MEAN
#endif

/* identifier truncated to 255 characters in the debug information */
#pragma warning(disable:4786)

#include <windows.h>

#endif   /* WIN32 || WIN64 */

#if !defined(WIN32) && !defined(WIN64)          \
      && defined(__GNUC__) && __GNUC__ >= 4     \
      && !defined(GADGET_HAVE_GCC_VISIBILITY)
#  define GADGET_HAVE_GCC_VISIBILITY
#endif

/*
 * ----------------------------------------------------------------------------
 * DLL-related macros.  These are based on the macros used by NSPR.  Use
 * GADGET_EXTERN for the prototype and GADGET_IMPLEMENT for the
 * implementation.
 * ----------------------------------------------------------------------------
 */
#if defined(WIN32) || defined(WIN64)

#   if defined(__GNUC__)
#       undef _declspec
#       define _declspec(x) __declspec(x)
#   endif

#   define GADGET_EXPORT(__type)      _declspec(dllexport) __type
#   define GADGET_EXPORT_CLASS        _declspec(dllexport)
#   define GADGET_EXPORT_DATA(__type) _declspec(dllexport) __type
#   define GADGET_IMPORT(__type)      _declspec(dllimport) __type
#   define GADGET_IMPORT_DATA(__type) _declspec(dllimport) __type
#   define GADGET_IMPORT_CLASS        _declspec(dllimport)

#   define GADGET_EXTERN(__type)         extern _declspec(dllexport) __type
#   define GADGET_IMPLEMENT(__type)      _declspec(dllexport) __type
#   define GADGET_EXTERN_DATA(__type)    extern _declspec(dllexport) __type
#   define GADGET_IMPLEMENT_DATA(__type) _declspec(dllexport) __type

#   define GADGET_CALLBACK
#   define GADGET_CALLBACK_DECL
#   define GADGET_STATIC_CALLBACK(__x) static __x

#elif defined(GADGET_HAVE_GCC_VISIBILITY)

#   define GADGET_EXPORT(__type)      __attribute__ ((visibility("default"))) __type
#   define GADGET_EXPORT_CLASS        __attribute__ ((visibility("default")))
#   define GADGET_EXPORT_DATA(__type) __attribute__ ((visibility("default"))) __type
#   define GADGET_IMPORT(__type)      __type
#   define GADGET_IMPORT_DATA(__type) __type
#   define GADGET_IMPORT_CLASS        

#   define GADGET_EXTERN(__type)         extern __attribute__ ((visibility("default"))) __type
#   define GADGET_IMPLEMENT(__type)      __attribute__ ((visibility("default"))) __type
#   define GADGET_EXTERN_DATA(__type)    extern __attribute__ ((visibility("default"))) __type
#   define GADGET_IMPLEMENT_DATA(__type) __attribute__ ((visibility("default"))) __type

#   define GADGET_CALLBACK
#   define GADGET_CALLBACK_DECL
#   define GADGET_STATIC_CALLBACK(__x) static __x

#else   /* UNIX (where this stuff is simple!) */

#   define GADGET_EXPORT(__type)      __type
#   define GADGET_EXPORT_CLASS
#   define GADGET_EXPORT_DATA(__type) __type
#   define GADGET_IMPORT(__type)      __type
#   define GADGET_IMPORT_CLASS
#   define GADGET_IMPORT_DATA(__type) __type

#   define GADGET_EXTERN(__type)         extern __type
#   define GADGET_IMPLEMENT(__type)      __type
#   define GADGET_EXTERN_DATA(__type)    extern __type
#   define GADGET_IMPLEMENT_DATA(__type) __type

#   define GADGET_CALLBACK
#   define GADGET_CALLBACK_DECL
#   define GADGET_STATIC_CALLBACK(__x) static __x

#endif	/* WIN32 || WIN64 */

#ifdef _GADGET_BUILD_
#   define GADGET_API(__type)	GADGET_EXPORT(__type)
#   define GADGET_CLASS_API	GADGET_EXPORT_CLASS
#   define GADGET_DATA_API(__type)	GADGET_EXPORT_DATA(__type)
#else
#   define GADGET_API(__type)	GADGET_IMPORT(__type)
#   define GADGET_CLASS_API	GADGET_IMPORT_CLASS
#   define GADGET_DATA_API(__type)	GADGET_IMPORT_DATA(__type)

#   include <gadget/AutoLink.h>
#endif

// #define GADGET_USING_RIM_SYNC

#endif   /* _GADGET_CONFIG_H_ */
