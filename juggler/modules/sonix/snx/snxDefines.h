/* gadget/gadgetDefines.h.  Generated automatically by configure.  */
/* gadget/gadgetDefines.h.in.  Generated automatically from configure.in by autoheader.  */
/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2002 by Iowa State University
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

#ifndef _SNX_DEFINES_H_
#define _SNX_DEFINES_H_

/*
 * --------------------------------------------------------------------------
 * Library-wide configuration file.
 * Generated from acconfig.h.
 * --------------------------------------------------------------------------
 */


/* Define to `unsigned' if <sys/types.h> doesn't define.  */
/* #undef size_t */

/* Define if you have the ANSI C header files.  */
#define STDC_HEADERS 1

/* Define the following if building on IRIX. */
/* #undef _BSD_TYPES */

/* Define all of the following if building on HP-UX. */
/* #undef SNX_HPUX_VERSION */
/* #undef _INCLUDE_TERMIO */
/* #undef _CMA_NOWRAPPERS_ */

/* Definte if DTK is installed. */
/* #undef SNX_HAVE_DTK */

/* Define if you have the strdup function.  */
#define HAVE_STRDUP 1

/* Define if you have the strerror function.  */
#define HAVE_STRERROR 1

/* Define if you have the <fcntl.h> header file.  */
#define HAVE_FCNTL_H 1

/* Define if you have the <sys/ioctl.h> header file.  */
#define HAVE_SYS_IOCTL_H 1

/* Define if you have the <sys/stdsyms.h> header file.  */
/* #undef HAVE_SYS_STDSYMS_H */

/* Define if you have the <sys/time.h> header file.  */
#define HAVE_SYS_TIME_H 1

/* Define if you have the <sys/z8530.h> header file.  */
/* #undef HAVE_SYS_Z8530_H */

/* Define if you have the <termios.h> header file.  */
#define HAVE_TERMIOS_H 1

/* Define if you have the <unistd.h> header file.  */
#define HAVE_UNISTD_H 1

/* define to "IRIX5" or "IRIX6" depending on IRIX version */
/* #undef IRIXREL */

/* define if compiling on a Win32 platform */
/* #undef WIN32 */

/* define if compiling on a Win32 platform */
/* #undef _MBCS */

/* define if the C++ compiler supports basic templates */
#define HAVE_TEMPLATES 

/* define if the C++ compiler supports namespaces */
#define HAVE_NAMESPACE 

/* define if the C++ compiler supports the standard library */
#define HAVE_STD 

/* define if the C++ compiler supports RTTI */
#define HAVE_RTTI 

/* define if the C++ compiler supports static_cast<> */
#define HAVE_STATIC_CAST 

/* define if the C++ compiler supports dynamic_cast<> */
#define HAVE_DYNAMIC_CAST 


#endif  /* _SNX_DEFINES_H_ */
