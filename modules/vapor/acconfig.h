/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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

#ifndef _VPR_DEFINES_H_
#define _VPR_DEFINES_H_

/*
 * --------------------------------------------------------------------------
 * Library-wide configuration file.
 * Generated from acconfig.h.
 * --------------------------------------------------------------------------
 */

@TOP@

/* Define all of the following to use POSIX threads. */
#undef VPR_USE_PTHREADS
#undef VPR_THREAD_SCOPE
#undef VPR_POSIX_C_SOURCE
#undef _THREAD_SAFE
#undef RWSTD_MULTI_THREAD
#undef RW_MULTI_THREAD
#undef _REENTRANT

/* Define to use POSIX semaphores. */
#undef VPR_POSIX_SEMAPHORES

/* Define to use the IRIX sproc(2) threading model instead of POSIX threads. */
#undef VPR_USE_IRIX_SPROC

/* Define to use the NSPR subsystem. */
#undef VPR_USE_NSPR

/* Define to use the Win32 subsystem. */
#undef VPR_USE_WIN32

/* Define all of the following if building on IRIX. */
#undef VPR_OS_IRIX
#undef _BSD_TYPES

/* Define all of the following if building on HP-UX. */
#undef VPR_OS_HPUX
#undef _INCLUDE_TERMIO
#undef _CMA_NOWRAPPERS_

/* Define if building on Digital UNIX. */
#undef VPR_OS_DUX

/* Define if building on FreeBSD. */
#undef VPR_OS_FreeBSD

/* Define if building on Linux. */
#undef VPR_OS_Linux

/* Define if building on Solaris. */
#undef VPR_OS_Solaris

/* Define if building on AIX. */
#undef VPR_OS_AIX

/* Define if building on a Win32 platform. */
#undef VPR_OS_Win32

/* Define if building on Darwin. */
#undef VPR_OS_Darwin

/* Define if building on a platform not specified above. */
#undef VPR_OS_GENERIC

/* Define to match the output of running 'uname -r. */
#undef VPR_OS_RELEASE

/*
 * Define to match the numeric version number of the target operating system.
 * For example, on HP-UX 10.20:
 *     #define VPR_OS_RELEASE_MAJOR 10
 *     #define VPR_OS_RELEASE_MINOR 20
 */
#undef VPR_OS_RELEASE_MAJOR
#undef VPR_OS_RELEASE_MINOR

/* Define to `int' if the socklen_t type is not available.  */
#undef socklen_t

/* Define to `int' if the in_addr_t type is not available.  */
#undef in_addr_t

/* Define to the appropriate type if the ssize_t type is not available.  */
#undef ssize_t

/*
 * Define to the sizeof a memory address if the type caddr_t is not available.
 * char* is most likely going to be a safe substitute.
 */
#undef caddr_t

/* Define if you have the sched_yield function.  */
#undef HAVE_SCHED_YIELD

@BOTTOM@

#endif	/* _VPR_DEFINES_H_ */
