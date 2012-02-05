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

#ifndef _VRJ_DRAW_PF_CONFIG_H_
#define _VRJ_DRAW_PF_CONFIG_H_
/* #pragma once */

/**
 * \file
 *
 * This file is to be used by code in the OpenGL Performer Draw Manager for
 * ensuring that platform-specific preprocessor symbols are defined correctly.
 * In particular, this file handles the Windows DLL and automatic linking
 * features for the OpenGL Performer Draw Manager shared library. It must be
 * included at the top of every .h and .cpp file in the OpenGL Performer Draw
 * Manager source before any other headers.
 */

/* This should always be included first. */
#include <vrj/vrjConfig.h>

#if defined(WIN32) || defined(WIN64)

#   if defined(__GNUC__)
#       undef _declspec
#       define _declspec(x) __declspec(x)
#   endif

#   define VJ_PF_EXPORT        _declspec(dllexport)
#   define VJ_PF_IMPORT        _declspec(dllimport)
#   define VJ_PF_EXTERN_EXPORT extern _declspec(dllexport)
#   define VJ_PF_EXTERN_IMPORT extern _declspec(dllimport)

#elif defined(VJ_HAVE_GCC_VISIBILITY)
#   define VJ_PF_EXPORT        __attribute__ ((visibility("default")))
#   define VJ_PF_IMPORT
#   define VJ_PF_EXTERN_EXPORT extern __attribute__ ((visibility("default")))
#   define VJ_PF_EXTERN_IMPORT extern
#else
#   define VJ_PF_EXPORT
#   define VJ_PF_IMPORT
#   define VJ_PF_EXTERN_EXPORT extern
#   define VJ_PF_EXTERN_IMPORT extern
#endif  /* WIN32 || WIN64 */

#ifdef _VRJ_PF_BUILD_
#   define VJ_PF_API    VJ_PF_EXPORT
#   define VJ_PF_EXTERN VJ_PF_EXTERN_EXPORT
#else
#   define VJ_PF_API    VJ_PF_IMPORT
#   define VJ_PF_EXTERN VJ_PF_EXTERN_IMPORT
#endif

#if ! defined(_VRJ_PF_BUILD_)
#   define VJ_LIB_NAME "vrj_pf"
#   include <vrj/AutoLink.h>
#   undef VJ_LIB_NAME
#endif

#endif   /* _VRJ_DRAW_PF_CONFIG_H_ */
