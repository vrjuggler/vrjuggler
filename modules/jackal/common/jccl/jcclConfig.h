/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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
 * This file (jcclConfig.h) includes header files common to most, if not all,
 * files in the JCCL source tree.  It must be included at the top of every .h,
 * and .cpp file before any other headers because it includes system headers.
 * ----------------------------------------------------------------------------
 */

/* This should always be included first. */
#include <jccl/jcclDefines.h>

/* Get rid of symbols added by Autoconf 2.5x. */
#undef PACKAGE_BUGREPORT
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_VERSION

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

#endif   /* WIN32 */

/*
 * ----------------------------------------------------------------------------
 * DLL-related macros.  These are based on the macros used by NSPR.  Use
 * JCCL_EXTERN for the prototype and JCCL_IMPLEMENT for the implementation.
 * ----------------------------------------------------------------------------
 */
#ifdef WIN32

#   if defined(__GNUC__)
#       undef _declspec
#       define _declspec(x) __declspec(x)
#   endif

#   define JCCL_EXPORT(__type)      _declspec(dllexport) __type
#   define JCCL_EXPORT_CLASS        _declspec(dllexport)
#   define JCCL_EXPORT_DATA(__type) _declspec(dllexport) __type
#   define JCCL_IMPORT(__type)      _declspec(dllimport) __type
#   define JCCL_IMPORT_DATA(__type) _declspec(dllimport) __type
#   define JCCL_IMPORT_CLASS        _declspec(dllimport)

#   define JCCL_EXTERN(__type)         extern _declspec(dllexport) __type
#   define JCCL_IMPLEMENT(__type)      _declspec(dllexport) __type
#   define JCCL_EXTERN_DATA(__type)    extern _declspec(dllexport) __type
#   define JCCL_IMPLEMENT_DATA(__type) _declspec(dllexport) __type

#   define JCCL_CALLBACK
#   define JCCL_CALLBACK_DECL
#   define JCCL_STATIC_CALLBACK(__x) static __x

#else   /* UNIX (where this stuff is simple!) */

#   define JCCL_EXPORT(__type)      __type
#   define JCCL_EXPORT_CLASS
#   define JCCL_EXPORT_DATA(__type) __type
#   define JCCL_IMPORT(__type)      __type
#   define JCCL_IMPORT_CLASS
#   define JCCL_IMPORT_DATA(__type) __type

#   define JCCL_EXTERN(__type)         extern __type
#   define JCCL_IMPLEMENT(__type)      __type
#   define JCCL_EXTERN_DATA(__type)    extern __type
#   define JCCL_IMPLEMENT_DATA(__type) __type

#   define JCCL_CALLBACK
#   define JCCL_CALLBACK_DECL
#   define JCCL_STATIC_CALLBACK(__x) static __x

#endif	/* WIN32 */

#ifdef _JCCL_BUILD_
#   define JCCL_API(__type)	JCCL_EXPORT(__type)
#   define JCCL_CLASS_API		JCCL_EXPORT_CLASS
#   define JCCL_DATA_API(__type)	JCCL_EXPORT_DATA(__type)
#else
#   define JCCL_API(__type)	JCCL_IMPORT(__type)
#   define JCCL_CLASS_API		JCCL_IMPORT_CLASS
#   define JCCL_DATA_API(__type)	JCCL_IMPORT_DATA(__type)
#endif

#endif   /* _JCCL_CONFIG_H_ */
