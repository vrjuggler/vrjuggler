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

#ifndef _GADGET_DRIVER_CONFIG_H_
#define _GADGET_DRIVER_CONFIG_H_

/* Get rid of symbols added by Autoconf 2.5x. */
#undef PACKAGE_BUGREPORT
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_VERSION
 
#include <gadget/gadgetConfig.h>

#if defined(WIN32) || defined(WIN64)

#   if defined(__GNUC__)
#       undef _declspec
#       define _declspec(x) __declspec(x)
#   endif

#   define GADGET_DRIVER_EXPORT        _declspec(dllexport)
#   define GADGET_DRIVER_IMPORT        _declspec(dllimport)
#   define GADGET_DRIVER_EXTERN_EXPORT extern _declspec(dllexport)
#   define GADGET_DRIVER_EXTERN_IMPORT extern _declspec(dllimport)

#elif defined(GADGET_DRIVER_HAVE_GCC_VISIBILITY)
#   define GADGET_DRIVER_EXPORT        __attribute__ ((visibility("default")))
#   define GADGET_DRIVER_IMPORT
#   define GADGET_DRIVER_EXTERN_EXPORT extern __attribute__ ((visibility("default")))
#   define GADGET_DRIVER_EXTERN_IMPORT extern
#else
#   define GADGET_DRIVER_EXPORT
#   define GADGET_DRIVER_IMPORT
#   define GADGET_DRIVER_EXTERN_EXPORT extern
#   define GADGET_DRIVER_EXTERN_IMPORT extern
#endif  /* WIN32 || WIN64 */

#ifdef _GADGET_DRIVER_BUILD_
#   define GADGET_DRIVER_API    GADGET_DRIVER_EXPORT
#   define GADGET_DRIVER_EXTERN GADGET_DRIVER_EXTERN_EXPORT
#else
#   define GADGET_DRIVER_API    GADGET_DRIVER_IMPORT
#   define GADGET_DRIVER_EXTERN GADGET_DRIVER_EXTERN_IMPORT
#endif

#endif /* _GADGET_DRIVER_CONFIG_H_ */
