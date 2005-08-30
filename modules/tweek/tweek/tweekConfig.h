/***************** <Tweek heading BEGIN do not edit this line> ****************
 * Tweek
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 ***************** <Tweek heading END do not edit this line> *****************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

#ifndef _TWEEK_CONFIG_H_
#define _TWEEK_CONFIG_H_
/* #pragma once */

/*
 * ----------------------------------------------------------------------------
 * This file (tweekConfig.h) includes header files common to most, if not all,
 * files in the VR Juggler source tree.  It must be included at the top of
 * every .h and .cpp file before any other headers because it includes system
 * headers.
 * ----------------------------------------------------------------------------
 */

/* This should always be included first. */
#include <tweek/tweekDefines.h>

/* Get rid of symbols added by Autoconf 2.5x. */
#undef PACKAGE_BUGREPORT
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_VERSION

#ifdef _DEBUG
#   define TWEEK_DEBUG
#else
#   define TWEEK_OPT
#endif

#if defined(WIN32) || defined(WIN64)
/* Exclude rarely-used stuff from Windows headers */
#define WIN32_LEAN_AND_MEAN

/* identifier truncated to 255 characters in the debug information */
#pragma warning(disable:4786)

#include <windows.h>

/* extern HINSTANCE g_hInst = NULL; */
#endif   /* WIN32 || WIN64 */

#if !defined(WIN32) && !defined(WIN64)          \
      && defined(__GNUC__) && __GNUC__ >= 4     \
      && !defined(TWEEK_HAVE_GCC_VISIBILITY)
#  define TWEEK_HAVE_GCC_VISIBILITY
#endif

/*
 * ----------------------------------------------------------------------------
 * DLL-related macros.  These are based on the macros used by NSPR.  Use
 * TWEEK_EXTERN for the prototype and TWEEK_IMPLEMENT for the implementation.
 * ----------------------------------------------------------------------------
 */
#if defined(WIN32) || defined(WIN64)

#   if defined(__GNUC__)
#       undef _declspec
#       define _declspec(x) __declspec(x)
#   endif

#   define TWEEK_EXPORT(__type)      _declspec(dllexport) __type
#   define TWEEK_EXPORT_CLASS        _declspec(dllexport)
#   define TWEEK_EXPORT_DATA(__type) _declspec(dllexport) __type
#   define TWEEK_IMPORT(__type)      _declspec(dllimport) __type
#   define TWEEK_IMPORT_DATA(__type) _declspec(dllimport) __type
#   define TWEEK_IMPORT_CLASS        _declspec(dllimport)

#   define TWEEK_EXTERN(__type)         extern _declspec(dllexport) __type
#   define TWEEK_IMPLEMENT(__type)      _declspec(dllexport) __type
#   define TWEEK_EXTERN_DATA(__type)    extern _declspec(dllexport) __type
#   define TWEEK_IMPLEMENT_DATA(__type) _declspec(dllexport) __type

#   define TWEEK_CALLBACK
#   define TWEEK_CALLBACK_DECL
#   define TWEEK_STATIC_CALLBACK(__x) static __x

#elif defined(TWEEK_HAVE_GCC_VISIBILITY)

#   define TWEEK_EXPORT(__type)      __attribute__ ((visibility("default"))) __type
#   define TWEEK_EXPORT_CLASS        __attribute__ ((visibility("default")))
#   define TWEEK_EXPORT_DATA(__type) __attribute__ ((visibility("default"))) __type
#   define TWEEK_IMPORT(__type)      __type
#   define TWEEK_IMPORT_DATA(__type) __type
#   define TWEEK_IMPORT_CLASS        

#   define TWEEK_EXTERN(__type)         extern __attribute__ ((visibility("default"))) __type
#   define TWEEK_IMPLEMENT(__type)      __attribute__ ((visibility("default"))) __type
#   define TWEEK_EXTERN_DATA(__type)    extern __attribute__ ((visibility("default"))) __type
#   define TWEEK_IMPLEMENT_DATA(__type) __attribute__ ((visibility("default"))) __type

#   define TWEEK_CALLBACK
#   define TWEEK_CALLBACK_DECL
#   define TWEEK_STATIC_CALLBACK(__x) static __x

#else   /* UNIX (where this stuff is simple!) */

#   define TWEEK_EXPORT(__type)      __type
#   define TWEEK_EXPORT_CLASS
#   define TWEEK_EXPORT_DATA(__type) __type
#   define TWEEK_IMPORT(__type)      __type
#   define TWEEK_IMPORT_CLASS
#   define TWEEK_IMPORT_DATA(__type) __type

#   define TWEEK_EXTERN(__type)         extern __type
#   define TWEEK_IMPLEMENT(__type)      __type
#   define TWEEK_EXTERN_DATA(__type)    extern __type
#   define TWEEK_IMPLEMENT_DATA(__type) __type

#   define TWEEK_CALLBACK
#   define TWEEK_CALLBACK_DECL
#   define TWEEK_STATIC_CALLBACK(__x) static __x

#endif	/* WIN32 || WIN64 */

#ifdef _TWEEK_BUILD_
#   define TWEEK_API(__type)		TWEEK_EXPORT(__type)
#   define TWEEK_CLASS_API		TWEEK_EXPORT_CLASS
#   define TWEEK_DATA_API(__type)	TWEEK_EXPORT_DATA(__type)
#else
#   define TWEEK_API(__type)		TWEEK_IMPORT(__type)
#   define TWEEK_CLASS_API		TWEEK_IMPORT_CLASS
#   define TWEEK_DATA_API(__type)	TWEEK_IMPORT_DATA(__type)
#endif

#endif   /* _TWEEK_CONFIG_H_ */
