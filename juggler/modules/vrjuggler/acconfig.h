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

#ifndef _VJ_DEFINES_H_
#define _VJ_DEFINES_H_

/*
 * --------------------------------------------------------------------------
 * Library-wide configuration file.
 * Generated from acconfig.h.
 * --------------------------------------------------------------------------
 */

@TOP@

/* Define all of the following to use POSIX threads. */
#undef VJ_USE_PTHREADS
#undef VJ_THREAD_SCOPE
#undef VJ_POSIX_C_SOURCE
#undef _THREAD_SAFE
#undef RWSTD_MULTI_THREAD
#undef RW_MULTI_THREAD
#undef _REENTRANT

/* Define if using POSIX threads Draft 4. */
#undef _PTHREADS_DRAFT_4

/* Define if using POSIX threads Draft 10 (the "final" draft). */
#undef _PTHREADS_DRAFT_10

/* Define to use POSIX semaphores. */
#undef VJ_POSIX_SEMAPHORES

/* Define to use the IRIX sproc(2) threading model instead of POSIX threads. */
#undef VJ_IRIX_SPROC

/* Define to use the NSPR subsystem. */
#undef VJ_USE_NSPR

/* Define to use the OpenGL API. */
#undef VJ_API_OPENGL

/* Define to use the Performer API. */
#undef VJ_API_PERFORMER

/* Define all of the following if building on IRIX. */
#undef VJ_OS_IRIX
#undef _BSD_TYPES

/* Define to be either IRIX5 or IRIX6 depending on your system. */
#undef IRIXREL

/* Define all of the following if building on HP-UX. */
#undef VJ_OS_HPUX
#undef VJ_HPUX_VERSION
#undef _INCLUDE_TERMIO
#undef _CMA_NOWRAPPERS_

/* Define if building on Digital UNIX. */
#undef VJ_OS_DUX

/* Define if building on FreeBSD. */
#undef VJ_OS_FreeBSD

/* Define if building on Linux. */
#undef VJ_OS_Linux

/* Define if building on Solaris. */
#undef VJ_OS_Solaris

/* Define if building on AIX. */
#undef VJ_OS_AIX

/* Define if building on a Win32 platform. */
#undef VJ_OS_Win32
#undef WIN32
#undef _MBCS

/* Define if building on Darwin (aka, Mac OS X). */
#undef VJ_OS_Darwin

/* Define if building on a platform not specified above. */
#undef VJ_OS_GENERIC

/* Define to match the output of running 'uname -r. */
#undef VJ_OS_RELEASE

/*
 * Define to match the numeric version number of the target operating system.
 * For example, on HP-UX 10.20:
 *     #define VJ_OS_RELEASE_MAJOR 10
 *     #define VJ_OS_RELEASE_MINOR 20
 */
#undef VJ_OS_RELEASE_MAJOR
#undef VJ_OS_RELEASE_MINOR

/* Define to unique values used for defining VJ_PERFORMANCE. */
#undef VJ_PERF_NONE
#undef VJ_PERF_SGI
#undef VJ_PERF_POSIX

/* Define to one of the above values to enable performance testing. */
#undef VJ_PERFORMANCE

/* Definte if DTK is installed. */
#undef VJ_HAVE_DTK

/* Define to `int' if the socklen_t type is not available.  */
#undef socklen_t

/*
 * Define to `unsigned int' if the u_int and/or u_int32_t types are not
 * available.  On Windows when using Visual C++, define to UINT.
 */
#undef u_int
#undef uint32_t

/*
 * Define to the sizeof a memory address if the type caddr_t is not available.
 * char* is most likely going to be a safe substitute.
 */
#undef caddr_t

/* 
 * On Windows when using Visual C++, define to __int32 and __int64
 * respectively if these two are not available.
 */
#undef int32_t
#undef int64_t

/* Define if you have the sched_yield function.  */
#undef HAVE_SCHED_YIELD

/* Define if you have the sinf function.  */
#undef HAVE_SINF

/* Define if you have the cosf function.  */
#undef HAVE_COSF

/* Define if you have the sqrtf function.  */
#undef HAVE_SQRTF

/* Define if you have the asinf function.  */
#undef HAVE_ASINF

/* Define if you have the acosf function.  */
#undef HAVE_ACOSF

/* Define if you have the atan2f function.  */
#undef HAVE_ATAN2F

/* Define if you have the atan2f function.  */
#undef HAVE_FABSF

@BOTTOM@

#endif	/* _VJ_DEFINES_H_ */
