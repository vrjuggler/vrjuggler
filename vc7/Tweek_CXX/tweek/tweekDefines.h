/***************** <Tweek heading BEGIN do not edit this line> ****************
 * Tweek
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 ***************** <Tweek heading END do not edit this line> *****************/

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

#ifndef _TWEEK_DEFINES_H_
#define _TWEEK_DEFINES_H_

/* define if the C++ compiler supports exceptions */
#define HAVE_EXCEPTIONS 

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* define if the C++ compiler supports namespaces */
#define HAVE_NAMESPACE 

/* define if the C++ compiler supports the standard library */
#define HAVE_STD 

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* define if the C++ compiler supports basic templates */
#define HAVE_TEMPLATES 

/* If using omniORB, define to the major version number (3, 4, etc.). */
#define OMNIORB_VER 4

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "vrjuggler-devel@lists.sourceforge.net"

/* Define to the full name of this package. */
#define PACKAGE_NAME "Tweek"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "Tweek 1.1"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "tweek"

/* Define to the version of this package. */
#define PACKAGE_VERSION "1.1"

/* Define to include the main CORBA header file -- for example:
   <omniORB3/CORBA.h> */
#define TWEEK_INCLUDE_CORBA_H <omniORB4/CORBA.h>

/* Define to an ORB identifier string used to initialized the ORB -- for
   example: "omniORB3" */
#define TWEEK_ORB_VER_STRING "omniORB4"

/* Define if using omniORB as the C++ CORBA implementation. */
#define TWEEK_USE_OMNIORB 

/* Define only on Windows NT/2000/XP. */
#define __NT__ 1

/* Define to the major number of the target operating system. */
#define __OSVERSION__ 5

/* Define to 1 if building on a Win32 platform. */
#define __WIN32__ 1

/* Define to 1 if building on Intel x86 architecture. */
#define __x86__ 1

#endif /* _TWEEK_DEFINES_H_ */
