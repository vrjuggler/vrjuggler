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

#ifndef _VRJ_JCCL_CONFIG_H_
#define _VRJ_JCCL_CONFIG_H_
/**
 ** ----------------------------------------------------------------------------
 ** This file (jcclConfig.h) includes header files common to most, if not all,
 ** files in the JCCL source tree.  It must be included at the top of every .h,
 ** and .cpp file before any other headers because it includes system headers.
 ** ----------------------------------------------------------------------------
 **/
/* Must get defines first. */
#include <jccl/jcclDefines.h>          // Defines for jccl

/* Get rid of symbols added by Autoconf 2.5x. */
#undef PACKAGE_BUGREPORT
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_VERSION

/*
 * If either JUGGLER_DEBUG or _DEBUG is defined, that implies the need for
 * JCCL_DEBUG.
 */
#if ! defined(JCCL_DEBUG) && (defined(JUGGLER_DEBUG) || defined(_DEBUG))
#   define JCCL_DEBUG
/* If JUGGLER_OPT is defined, make sure that JCCL_OPT is also defined. */
#elif defined(JUGGLER_OPT) && ! defined(JCCL_OPT)
#   define JCCL_OPT
#endif

// Windows
#if defined(WIN32) || defined(WIN64)

#ifndef WIN32_LEAN_AND_MEAN
/* Exclude rarely-used stuff from Windows headers */
#   define WIN32_LEAN_AND_MEAN
#endif

/* Turn off: identifier truncated to 255 characters in the debug information */
#   pragma warning(disable:4786)
#   include <windows.h>
#endif /* WIN32 || WIN64 */

#if ! defined(WIN32) && ! defined(WIN64)        \
      && defined(__GNUC__) && __GNUC__ >= 4     \
      && ! defined(JCCL_HAVE_GCC_VISIBILITY)
#  define JCCL_HAVE_GCC_VISIBILITY
#endif

#if defined(WIN32) || defined(WIN64)

#   if defined(__GNUC__)
#       undef _declspec
#       define _declspec(x) __declspec(x)
#   endif

#   define JCCL_EXPORT        _declspec(dllexport)
#   define JCCL_IMPORT        _declspec(dllimport)
#   define JCCL_EXTERN_EXPORT extern _declspec(dllexport)
#   define JCCL_EXTERN_IMPORT extern _declspec(dllimport)

#elif defined(JCCL_HAVE_GCC_VISIBILITY)
#   define JCCL_EXPORT        __attribute__ ((visibility("default")))
#   define JCCL_IMPORT
#   define JCCL_EXTERN_EXPORT extern __attribute__ ((visibility("default")))
#   define JCCL_EXTERN_IMPORT extern
#else
#   define JCCL_EXPORT
#   define JCCL_IMPORT
#   define JCCL_EXTERN_EXPORT extern
#   define JCCL_EXTERN_IMPORT extern
#endif  /* WIN32 || WIN64 */

#ifdef _JCCL_BUILD_
#   define JCCL_API    JCCL_EXPORT
#   define JCCL_EXTERN JCCL_EXTERN_EXPORT
#else
#   define JCCL_API    JCCL_IMPORT
#   define JCCL_EXTERN JCCL_EXTERN_IMPORT
#endif

#if ! defined(_JCCL_BUILD_)
#   include <jccl/AutoLink.h>
#endif

#endif   /* _VRJ_JCCL_CONFIG_H_ */
