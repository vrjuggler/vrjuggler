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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
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

/*
 * ----------------------------------------------------------------------------
 * DLL-related macros.  These are based on the macros used by NSPR.  Use
 * JCCL_PLUGIN_EXTERN for the prototype and JCCL_PLUGIN_IMPLEMENT for the
 * implementation.
 * ----------------------------------------------------------------------------
 */
#if defined(WIN32) || defined(WIN64)

#   if defined(__GNUC__)
#       undef _declspec
#       define _declspec(x) __declspec(x)
#   endif

#   define JCCL_PLUGIN_EXPORT(__type)      _declspec(dllexport) __type
#   define JCCL_PLUGIN_EXPORT_CLASS        _declspec(dllexport)
#   define JCCL_PLUGIN_EXPORT_DATA(__type) _declspec(dllexport) __type
#   define JCCL_PLUGIN_IMPORT(__type)      _declspec(dllimport) __type
#   define JCCL_PLUGIN_IMPORT_DATA(__type) _declspec(dllimport) __type
#   define JCCL_PLUGIN_IMPORT_CLASS        _declspec(dllimport)

#   define JCCL_PLUGIN_EXTERN(__type)         extern _declspec(dllexport) __type
#   define JCCL_PLUGIN_IMPLEMENT(__type)      _declspec(dllexport) __type
#   define JCCL_PLUGIN_EXTERN_DATA(__type)    extern _declspec(dllexport) __type
#   define JCCL_PLUGIN_IMPLEMENT_DATA(__type) _declspec(dllexport) __type

#   define JCCL_PLUGIN_CALLBACK
#   define JCCL_PLUGIN_CALLBACK_DECL
#   define JCCL_PLUGIN_STATIC_CALLBACK(__x) static __x

#else   /* UNIX (where this stuff is simple!) */

#   define JCCL_PLUGIN_EXPORT(__type)      __type
#   define JCCL_PLUGIN_EXPORT_CLASS
#   define JCCL_PLUGIN_EXPORT_DATA(__type) __type
#   define JCCL_PLUGIN_IMPORT(__type)      __type
#   define JCCL_PLUGIN_IMPORT_CLASS
#   define JCCL_PLUGIN_IMPORT_DATA(__type) __type

#   define JCCL_PLUGIN_EXTERN(__type)         extern __type
#   define JCCL_PLUGIN_IMPLEMENT(__type)      __type
#   define JCCL_PLUGIN_EXTERN_DATA(__type)    extern __type
#   define JCCL_PLUGIN_IMPLEMENT_DATA(__type) __type

#   define JCCL_PLUGIN_CALLBACK
#   define JCCL_PLUGIN_CALLBACK_DECL
#   define JCCL_PLUGIN_STATIC_CALLBACK(__x) static __x

#endif	/* WIN32 || WIN64 */

#ifdef _JCCL_PLUGIN_BUILD_
#   define JCCL_PLUGIN_API(__type)	JCCL_PLUGIN_EXPORT(__type)
#   define JCCL_PLUGIN_CLASS_API	JCCL_PLUGIN_EXPORT_CLASS
#   define JCCL_PLUGIN_DATA_API(__type)	JCCL_PLUGIN_EXPORT_DATA(__type)
#else
#   define JCCL_PLUGIN_API(__type)	JCCL_PLUGIN_IMPORT(__type)
#   define JCCL_PLUGIN_CLASS_API	JCCL_PLUGIN_IMPORT_CLASS
#   define JCCL_PLUGIN_DATA_API(__type)	JCCL_PLUGIN_IMPORT_DATA(__type)
#endif

#endif /* _JCCL_PLUGIN_CONFIG_H_ */
