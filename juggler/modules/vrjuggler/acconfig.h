#ifndef _VJ_DEFINES_H_
#define _VJ_DEFINES_H_

/*
 * --------------------------------------------------------------------------
 * Library-wide configuration file.
 * Generated from acconfig.h ($Revision$)
 * --------------------------------------------------------------------------
 */

@TOP@

/* Define to enable compile-time debugging blocks. */
#undef VJ_DEBUG

/* Define all of the following to use POSIX threads. */
#undef VJ_USE_PTHREADS
#undef _THREAD_SAFE
#undef RWSTD_MULTI_THREAD
#undef RW_MULTI_THREAD
#undef _REENTRANT

/* Define if using POSIX threads Draft 4. */
#undef _PTHREADS_DRAFT_4

/* Define if using POSIX threads Draft 10 (the "final" draft). */
#undef _PTHREADS_DRAFT_10

/* Define to use SGI IPC threading instead of POSIX threads. */
#undef VJ_SGI_IPC

/* Define to use the OpenGL API. */
#undef VJ_API_OPENGL

/* Define to use the Performer API. */
#undef VJ_API_PERFORMER

/* Define all of the following if building on IRIX. */
#undef VJ_OS_SGI
#undef _BSD_TYPES

/* Define to be either IRIX5 or IRIX6 depending on your system. */
#undef IRIXREL

/* Define all of the following if building on HP-UX. */
#undef VJ_OS_HPUX
#undef _HPUX_SOURCE
#undef _INCLUDE_TERMIO
#undef _CMA_NOWRAPPERS_
#undef notdef

/* Define if building on Digital UNIX. */
#undef VJ_OS_DUX

/* Define if building on FreeBSD. */
#undef VJ_OS_FreeBSD

/* Define if building on a platform not specified above. */
#undef VJ_OS_GENERIC

/* Define to `unsigned int' if the u_int type is not available. */
#undef u_int

@BOTTOM@

/* Define this macro to replace calls to sginap(2) if sginap(2) is not
  avialable. */
#ifndef HAVE_SGINAP
#   define sginap(x) (usleep(x * 333333))
#endif

#endif	/* _VJ_DEFINES_H_ */
