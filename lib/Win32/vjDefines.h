/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
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
 */


#ifndef _VJ_DEFINES_H_
#define _VJ_DEFINES_H_

/*
 * --------------------------------------------------------------------------
 * Library-wide configuration file.
 * Generated from acconfig.h.
 * --------------------------------------------------------------------------
 */


/* Define to `int' if <sys/types.h> doesn't define.  */
/* #undef pid_t */

/* Define as the return type of signal handlers (int or void).  */
#define RETSIGTYPE void

/* Define to `unsigned' if <sys/types.h> doesn't define.  */
/* #undef size_t */

/* Define if you have the ANSI C header files.  */
#define STDC_HEADERS 1

/* Define if you can safely include both <sys/time.h> and <time.h>.  */
/* #define TIME_WITH_SYS_TIME 1 */

/* Define all of the following to use POSIX threads. */
/* #undef VJ_USE_PTHREADS */
/* #undef VJ_THREAD_SCOPE */
/* #undef VJ_POSIX_C_SOURCE */
/* #undef _THREAD_SAFE */
/* #undef RWSTD_MULTI_THREAD */
/* #undef RW_MULTI_THREAD */
/* #undef _REENTRANT */

/* Define if using POSIX threads Draft 4. */
/* #undef _PTHREADS_DRAFT_4 */

/* Define if using POSIX threads Draft 10 (the "final" draft). */
/* #undef _PTHREADS_DRAFT_10 */

/* Define to use SGI IPC threading instead of POSIX threads. */
/* #define VJ_SGI_IPC */ 

/* Define to use the OpenGL API. */
#define VJ_API_OPENGL 

/* Define to use the Performer API. */
/* #define VJ_API_PERFORMER */

/* Define all of the following if building on IRIX. */
/* #define VJ_OS_SGI */
/* #define _BSD_TYPES */
/* #define _OLD_TERMIOS  */

/* Define to be either IRIX5 or IRIX6 depending on your system. */
/* #define IRIXREL "IRIX6" */

/* Define all of the following if building on HP-UX. */
/* #undef VJ_OS_HPUX */
/* #undef _HPUX_SOURCE */
/* #undef _INCLUDE_TERMIO */
/* #undef _CMA_NOWRAPPERS_ */
/* #undef notdef */

/* Define if building on Digital UNIX. */
/* #undef VJ_OS_DUX */

/* Define if building on FreeBSD. */
/* #undef VJ_OS_FreeBSD */

/* Define if building on Linux. */
/* #undef VJ_OS_Linux */

/* Define if building on a platform not specified above. */
#undef VJ_OS_GENERIC

/* Define to match the output of running 'uname -r. */
#define VJ_OS_RELEASE "WIN32"

/*
 * Define to match the numeric version number of the target operating system.
 * For example, on HP-UX 10.20:
 *     #define VJ_OS_RELEASE_MAJOR 10
 *     #define VJ_OS_RELEASE_MINOR 20
 */
#define VJ_OS_RELEASE_MAJOR 4
#define VJ_OS_RELEASE_MINOR 0

/* Define to unique values used for defining VJ_PERFORMANCE. */
#define VJ_PERF_NONE 0
#define VJ_PERF_SGI 1
#define VJ_PERF_POSIX 2

/* Define to one of the above values to enable performance testing. */
#define VJ_PERFORMANCE VJ_PERF_NONE

/* Define to `unsigned int' if the u_int type is not available. */
/* #undef u_int */

/* Define if you have the gettimeofday function.  */
#define HAVE_GETTIMEOFDAY 1

/* Define if you have the sginap function.  */
#define HAVE_SGINAP 1

/* Define if you have the socket function.  */
#define HAVE_SOCKET 1

/* Define if you have the strdup function.  */
#define HAVE_STRDUP 1

/* Define if you have the strerror function.  */
#define HAVE_STRERROR 1

/* Define if you have the strtod function.  */
#define HAVE_STRTOD 1

/* Define if you have the <fcntl.h> header file.  */
#define HAVE_FCNTL_H 1

/* Define if you have the <limits.h> header file.  */
#define HAVE_LIMITS_H 1

/* Define if you have the <strings.h> header file.  */
#define HAVE_STRINGS_H 1

/* Define if you have the <sys/file.h> header file.  */
#define HAVE_SYS_FILE_H 1

/* Define if you have the <sys/ioctl.h> header file.  */
#define HAVE_SYS_IOCTL_H 1

/* Define if you have the <sys/prctl.h> header file.  */
#define HAVE_SYS_PRCTL_H 1

/* Define if you have the <sys/stdsyms.h> header file.  */
/* #undef HAVE_SYS_STDSYMS_H */

/* Define if you have the <sys/time.h> header file.  */
#define HAVE_SYS_TIME_H 1

/* Define if you have the <sys/z8530.h> header file.  */
#define HAVE_SYS_Z8530_H 1

/* Define if you have the <termio.h> header file.  */
#define HAVE_TERMIO_H 1

/* Define if you have the <unistd.h> header file.  */
/* #define HAVE_UNISTD_H 1 */

/*
 * Define this macro to replace calls to sginap(2) if sginap(2) is not
 * avialable.
 */
/*
#ifndef HAVE_SGINAP
#   define sginap(x) (usleep(x * 333333))
#endif
*/

#endif	/* _VJ_DEFINES_H_ */
