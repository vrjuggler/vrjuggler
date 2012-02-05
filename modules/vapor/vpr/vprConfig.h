/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 ****************** <VPR heading END do not edit this line> ******************/

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

#ifndef _VPR_CONFIG_H_
#define _VPR_CONFIG_H_
/* #pragma once */

/**
 * \file
 *
 * This file (vprConfig.h) includes header files common to most, if not all,
 * files in the VPR source tree.  It must be included at the top of every .h
 * and .cpp file before any other headers because it includes system headers.
 *
 * Include this file to get the basic declarations of VPR types.  This includes
 * forward declarations of wrapper classes for platform-specific features.
 */

/*
 * Protect against redefinition of symbols added by Autoconf 2.5x in
 * vprDefines.h.
 */
#undef PACKAGE_BUGREPORT
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_VERSION

/* This should always be included first. */
#include <vpr/vprDefines.h>

/* Get rid of symbols added by Autoconf 2.5x in vprDefines.h. */
#undef PACKAGE_BUGREPORT
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_VERSION

/*
 * If either JUGGLER_DEBUG or _DEBUG is defined, that implies the need for
 * VPR_DEBUG.
 */
#if ! defined(VPR_DEBUG) && (defined(JUGGLER_DEBUG) || defined(_DEBUG))
#   define VPR_DEBUG
/* If JUGGLER_OPT is defined, make sure that VPR_OPT is also defined. */
#elif defined(JUGGLER_OPT) && ! defined(VPR_OPT)
#   define VPR_OPT
#endif

#if defined(WIN32) || defined(WIN64)

#ifndef WIN32_LEAN_AND_MEAN
/* Exclude rarely-used stuff from Windows headers */
#   define WIN32_LEAN_AND_MEAN
#endif

/* identifier truncated to 255 characters in the debug information */
#pragma warning(disable:4786)

#include <windows.h>

/* extern HINSTANCE g_hInst = NULL; */
#endif   /* WIN32 || WIN64 */


/* Common C++ includes. */
/* Put here for pre-compiled headers. */
#ifdef __cplusplus
#include <iostream>

/*
 * GCC 3.1 and beyond have non-standard STL stuff in the __gnu_cxx namespace.
 * The code below works around that behavior by bringing the __gnu_cxx
 * namespace into the std namespace.
 *
 * NOTE: The Intel C++ compiler masquerades as GCC by defining __GNUC__.
 * Versions prior to 8.1 did not make use of any GCC headers, so we have to
 * make sure we are not using a version of the Intel compiler older than 8.1
 * when performing this operation.
 *
 * The EKOPath/PathScale compiler similarly lies.
 */
#if (! defined(__INTEL_COMPILER) && !defined(__PATHCC__) && defined(__GNUC__) && \
     ((__GNUC__ == 3 && __GNUC_MINOR__ >= 1) || __GNUC__ > 3)) || \
    (defined(__INTEL_COMPILER) && __INTEL_COMPILER >= 810 && defined(__GNUC__))
namespace std
{
   using namespace __gnu_cxx;
}

/*
 * Visual C++ 8.0 and 9.0 have non-standard STL stuff in the stdext namespace.
 * The code below works around that behavior by bringing the stdext namespace
 * into the std namespace.
 */
#elif defined(_MSC_VER) && (_MSC_VER == 1400 || _MSC_VER == 1500)
namespace std
{
   using namespace stdext;
}
#endif

#endif   /* __cplusplus */

/*
 * This is used to deal with various locations for the header that defines
 * std::hash_map and std::hash_set.  Files that need the header can test 
 * for the definition of VPR_HASH_MAP_INCLUDE to determine if any std::hash_map 
 * is available.
 */
#if defined(HAVE_HASH_MAP)
#  define VPR_HASH_MAP_INCLUDE <hash_map>
#elif defined(HAVE_EXT_HASH_MAP)
#  define VPR_HASH_MAP_INCLUDE <ext/hash_map>
#elif defined(HAVE_HASH_MAP_H)
#  define VPR_HASH_MAP_INCLUDE <hash_map.h>
#endif

#if defined(HAVE_HASH_MAP) || defined(HAVE_EXT_HASH_MAP) || defined(HAVE_HASH_MAP_H)
#   define VPR_HAVE_HASH_MAP 1
#endif

#if defined(HAVE_HASH_SET)
#  define VPR_HASH_SET_INCLUDE <hash_set>
#elif defined(HAVE_EXT_HASH_SET)
#  define VPR_HASH_SET_INCLUDE <ext/hash_set>
#elif defined(HAVE_HASH_SET_H)
#  define VPR_HASH_SET_INCLUDE <hash_set.h>
#endif

#if defined(HAVE_HASH_SET) || defined(HAVE_EXT_HASH_SET) || defined(HAVE_HASH_SET_H)
#   define VPR_HAVE_HASH_SET  1
#endif

#if ! defined(WIN32) && ! defined(WIN64)        \
      && defined(__GNUC__) && __GNUC__ >= 4     \
      && ! defined(VPR_HAVE_GCC_VISIBILITY)
#  define VPR_HAVE_GCC_VISIBILITY
#endif

#if defined(VPR_OS_Windows)

#   if defined(__GNUC__)
#       undef _declspec
#       define _declspec(x) __declspec(x)
#   endif

#   define VPR_EXPORT        _declspec(dllexport)
#   define VPR_IMPORT        _declspec(dllimport)
#   define VPR_EXTERN_EXPORT extern _declspec(dllexport)
#   define VPR_EXTERN_IMPORT extern _declspec(dllimport)

#elif defined(VPR_HAVE_GCC_VISIBILITY)
#   define VPR_EXPORT        __attribute__ ((visibility("default")))
#   define VPR_IMPORT
#   define VPR_EXTERN_EXPORT extern __attribute__ ((visibility("default")))
#   define VPR_EXTERN_IMPORT extern
#else
#   define VPR_EXPORT
#   define VPR_IMPORT
#   define VPR_EXTERN_EXPORT extern
#   define VPR_EXTERN_IMPORT extern
#endif  /* WIN32 || WIN64 */

#ifdef _VPR_BUILD_
#   define VPR_API    VPR_EXPORT
#   define VPR_EXTERN VPR_EXTERN_EXPORT
#else
#   define VPR_API    VPR_IMPORT
#   define VPR_EXTERN VPR_EXTERN_IMPORT
#endif

#include <vpr/vprDomain.h>

#if ! defined(_VPR_BUILD_)
#  include <vpr/AutoLink.h>
#endif

#endif   /* _VPR_CONFIG_H_ */
