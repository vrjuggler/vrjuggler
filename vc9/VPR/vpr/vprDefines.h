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
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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

#ifndef _VPR_DEFINES_H_
#define _VPR_DEFINES_H_

/* Define to 1 if you have the <fcntl.h> header file. */
#define HAVE_FCNTL_H 1

/* Define to 1 if you have the <hash_map> header file. */
#define HAVE_HASH_MAP 1

/* Define to 1 if you have the <hash_set> header file. */
#define HAVE_HASH_SET 1

/* Define to 1 if you have the <limits.h> header file. */
#define HAVE_LIMITS_H 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* define if the C++ compiler supports namespaces */
#define HAVE_NAMESPACE 

/* define if the C++ compiler supports RTTI */
#define HAVE_RTTI 

/* Define to 1 if you have the <signal.h> header file. */
#define HAVE_SIGNAL_H 1

/* define if the C++ compiler supports static_cast<> */
#define HAVE_STATIC_CAST 

/* define if the C++ compiler supports the standard library */
#define HAVE_STD 

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the `strerror' function. */
#define HAVE_STRERROR 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* define if the C++ compiler supports basic templates */
#define HAVE_TEMPLATES 

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "vrjuggler-devel@lists.sourceforge.net"

/* Define to the full name of this package. */
#define PACKAGE_NAME "VR Juggler Portable Runtime"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "VR Juggler Portable Runtime 2.1"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "vr-juggler-portable-runtime"

/* Define to the version of this package. */
#define PACKAGE_VERSION "2.1"

/* Define as the return type of signal handlers (`int' or `void'). */
#define RETSIGTYPE void

/* Define if building on a Windows platform. */
#define VPR_OS_Windows

/* Define if building on the Win32 platform. */
#ifdef WIN32
#  define VPR_OS_Win32 
/* Define if building on the Win64 platform. */
#elif defined(WIN64)
#  define VPR_OS_Win64
#endif

#define VPR_USE_WINTHREADS

/* Define if using the Netscape Portable Runtime. */
//#define VPR_USE_NSPR 

/* Define if using BOOST. */
#define VPR_USE_BOOST 

/* Define to unsigned int if the sigset_t type is not available. */
#define sigset_t unsigned int

#endif /* _VPR_DEFINES_H_ */
