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

#if ! defined(WIN32) && ! defined(WIN64)        \
      && defined(__GNUC__) && __GNUC__ >= 4     \
      && ! defined(GADGET_HAVE_GCC_VISIBILITY)
#  define GADGET_HAVE_GCC_VISIBILITY
#endif

#if defined(WIN32) || defined(WIN64)

#   if defined(__GNUC__)
#       undef _declspec
#       define _declspec(x) __declspec(x)
#   endif

#   define GADGET_EXPORT        _declspec(dllexport)
#   define GADGET_IMPORT        _declspec(dllimport)
#   define GADGET_EXTERN_EXPORT extern _declspec(dllexport)
#   define GADGET_EXTERN_IMPORT extern _declspec(dllimport)

#elif defined(GADGET_HAVE_GCC_VISIBILITY)
#   define GADGET_EXPORT        __attribute__ ((visibility("default")))
#   define GADGET_IMPORT
#   define GADGET_EXTERN_EXPORT extern __attribute__ ((visibility("default")))
#   define GADGET_EXTERN_IMPORT extern
#else
#   define GADGET_EXPORT
#   define GADGET_IMPORT
#   define GADGET_EXTERN_EXPORT extern
#   define GADGET_EXTERN_IMPORT extern
#endif  /* WIN32 || WIN64 */

#ifdef _GADGET_BUILD_
#   define GADGET_API    GADGET_EXPORT
#   define GADGET_EXTERN GADGET_EXTERN_EXPORT
#else
#   define GADGET_API    GADGET_IMPORT
#   define GADGET_EXTERN GADGET_EXTERN_IMPORT
#endif

#if ! defined(_GADGET_BUILD_)
#   include <gadget/AutoLink.h>
#endif

// #define GADGET_USING_RIM_SYNC

#endif   /* _GADGET_CONFIG_H_ */
