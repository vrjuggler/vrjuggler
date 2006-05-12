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

#ifndef _CLUSTER_PLUGIN_CONFIG_H_
#define _CLUSTER_PLUGIN_CONFIG_H_

/* Get rid of symbols added by Autoconf 2.5x. */
#undef PACKAGE_BUGREPORT
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_VERSION
 
#include <gadget/gadgetConfig.h>

/*
 * ----------------------------------------------------------------------------
 * DLL-related macros.  These are based on the macros used by NSPR.  Use
 * GADGET_CLUSTER_PLUGIN_EXTERN for the prototype and GADGET_CLUSTER_PLUGIN_IMPLEMENT for the
 * implementation.
 * ----------------------------------------------------------------------------
 */
#if defined(WIN32) || defined(WIN64)

#   if defined(__GNUC__)
#       undef _declspec
#       define _declspec(x) __declspec(x)
#   endif

#   define GADGET_CLUSTER_PLUGIN_EXPORT(__type)      _declspec(dllexport) __type
#   define GADGET_CLUSTER_PLUGIN_EXPORT_CLASS        _declspec(dllexport)
#   define GADGET_CLUSTER_PLUGIN_EXPORT_DATA(__type) _declspec(dllexport) __type
#   define GADGET_CLUSTER_PLUGIN_IMPORT(__type)      _declspec(dllimport) __type
#   define GADGET_CLUSTER_PLUGIN_IMPORT_DATA(__type) _declspec(dllimport) __type
#   define GADGET_CLUSTER_PLUGIN_IMPORT_CLASS        _declspec(dllimport)

#   define GADGET_CLUSTER_PLUGIN_EXTERN(__type)         extern _declspec(dllexport) __type
#   define GADGET_CLUSTER_PLUGIN_IMPLEMENT(__type)      _declspec(dllexport) __type
#   define GADGET_CLUSTER_PLUGIN_EXTERN_DATA(__type)    extern _declspec(dllexport) __type
#   define GADGET_CLUSTER_PLUGIN_IMPLEMENT_DATA(__type) _declspec(dllexport) __type

#   define GADGET_CLUSTER_PLUGIN_CALLBACK
#   define GADGET_CLUSTER_PLUGIN_CALLBACK_DECL
#   define GADGET_CLUSTER_PLUGIN_STATIC_CALLBACK(__x) static __x

#elif defined(GADGET_HAVE_GCC_VISIBILITY)

#   define GADGET_CLUSTER_PLUGIN_EXPORT(__type)      __attribute__ ((visibility("default"))) __type
#   define GADGET_CLUSTER_PLUGIN_EXPORT_CLASS        __attribute__ ((visibility("default")))
#   define GADGET_CLUSTER_PLUGIN_EXPORT_DATA(__type) __attribute__ ((visibility("default"))) __type
#   define GADGET_CLUSTER_PLUGIN_IMPORT(__type)      __type
#   define GADGET_CLUSTER_PLUGIN_IMPORT_DATA(__type) __type
#   define GADGET_CLUSTER_PLUGIN_IMPORT_CLASS        

#   define GADGET_CLUSTER_PLUGIN_EXTERN(__type)         extern __attribute__ ((visibility("default"))) __type
#   define GADGET_CLUSTER_PLUGIN_IMPLEMENT(__type)      __attribute__ ((visibility("default"))) __type
#   define GADGET_CLUSTER_PLUGIN_EXTERN_DATA(__type)    extern __attribute__ ((visibility("default"))) __type
#   define GADGET_CLUSTER_PLUGIN_IMPLEMENT_DATA(__type) __attribute__ ((visibility("default"))) __type

#   define GADGET_CLUSTER_PLUGIN_CALLBACK
#   define GADGET_CLUSTER_PLUGIN_CALLBACK_DECL
#   define GADGET_CLUSTER_PLUGIN_STATIC_CALLBACK(__x) static __x

#else   /* UNIX (where this stuff is simple!) */

#   define GADGET_CLUSTER_PLUGIN_EXPORT(__type)      __type
#   define GADGET_CLUSTER_PLUGIN_EXPORT_CLASS
#   define GADGET_CLUSTER_PLUGIN_EXPORT_DATA(__type) __type
#   define GADGET_CLUSTER_PLUGIN_IMPORT(__type)      __type
#   define GADGET_CLUSTER_PLUGIN_IMPORT_CLASS
#   define GADGET_CLUSTER_PLUGIN_IMPORT_DATA(__type) __type

#   define GADGET_CLUSTER_PLUGIN_EXTERN(__type)         extern __type
#   define GADGET_CLUSTER_PLUGIN_IMPLEMENT(__type)      __type
#   define GADGET_CLUSTER_PLUGIN_EXTERN_DATA(__type)    extern __type
#   define GADGET_CLUSTER_PLUGIN_IMPLEMENT_DATA(__type) __type

#   define GADGET_CLUSTER_PLUGIN_CALLBACK
#   define GADGET_CLUSTER_PLUGIN_CALLBACK_DECL
#   define GADGET_CLUSTER_PLUGIN_STATIC_CALLBACK(__x) static __x

#endif	/* WIN32 || WIN64 */

#ifdef _GADGET_CLUSTER_PLUGIN_BUILD_
#   define GADGET_CLUSTER_PLUGIN_API(__type)	GADGET_CLUSTER_PLUGIN_EXPORT(__type)
#   define GADGET_CLUSTER_PLUGIN_CLASS_API	GADGET_CLUSTER_PLUGIN_EXPORT_CLASS
#   define GADGET_CLUSTER_PLUGIN_DATA_API(__type)	GADGET_CLUSTER_PLUGIN_EXPORT_DATA(__type)
#else
#   define GADGET_CLUSTER_PLUGIN_API(__type)	GADGET_CLUSTER_PLUGIN_IMPORT(__type)
#   define GADGET_CLUSTER_PLUGIN_CLASS_API	GADGET_CLUSTER_PLUGIN_IMPORT_CLASS
#   define GADGET_CLUSTER_PLUGIN_DATA_API(__type)	GADGET_CLUSTER_PLUGIN_IMPORT_DATA(__type)
#endif

#endif /* _GADGET_CLUSTER_PLUGIN_CONFIG_H_ */
