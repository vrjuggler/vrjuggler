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

#ifndef _JCCL_PLUGIN_CONFIG_H_
#define _JCCL_PLUGIN_CONFIG_H_

/* Get rid of symbols added by Autoconf 2.5x. */
#undef PACKAGE_BUGREPORT
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_VERSION
 
#include <jccl/jcclConfig.h>

#if defined(WIN32) || defined(WIN64)

#   if defined(__GNUC__)
#       undef _declspec
#       define _declspec(x) __declspec(x)
#   endif

#   define JCCL_PLUGIN_EXPORT        _declspec(dllexport)
#   define JCCL_PLUGIN_IMPORT        _declspec(dllimport)
#   define JCCL_PLUGIN_EXTERN_EXPORT extern _declspec(dllexport)
#   define JCCL_PLUGIN_EXTERN_IMPORT extern _declspec(dllimport)

#elif defined(JCCL_PLUGIN_HAVE_GCC_VISIBILITY)
#   define JCCL_PLUGIN_EXPORT        __attribute__ ((visibility("default")))
#   define JCCL_PLUGIN_IMPORT
#   define JCCL_PLUGIN_EXTERN_EXPORT extern __attribute__ ((visibility("default")))
#   define JCCL_PLUGIN_EXTERN_IMPORT extern
#else
#   define JCCL_PLUGIN_EXPORT
#   define JCCL_PLUGIN_IMPORT
#   define JCCL_PLUGIN_EXTERN_EXPORT extern
#   define JCCL_PLUGIN_EXTERN_IMPORT extern
#endif  /* WIN32 || WIN64 */

#ifdef _JCCL_PLUGIN_BUILD_
#   define JCCL_PLUGIN_API    JCCL_PLUGIN_EXPORT
#   define JCCL_PLUGIN_EXTERN JCCL_PLUGIN_EXTERN_EXPORT
#else
#   define JCCL_PLUGIN_API    JCCL_PLUGIN_IMPORT
#   define JCCL_PLUGIN_EXTERN JCCL_PLUGIN_EXTERN_IMPORT
#endif

#endif /* _JCCL_PLUGIN_CONFIG_H_ */
