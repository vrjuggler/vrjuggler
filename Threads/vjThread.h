/*
 *  File:	    $RCSfile$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#ifndef _VJ_THREAD_H_
#define _VJ_THREAD_H_
#pragma once

#include <vjConfig.h>
#include <unistd.h>


// Typedefs to help with cross-platform abilities
typedef void (*THREAD_FUNC)(void *);

#include <Threads/vjBaseThread.h>


#ifdef VJ_SGI_IPC    // ---- SGI IPC Barrier ------ //
    typedef struct cancel_state	cancel_state_t;

#   include <Threads/vjThreadSGI.h>
#   include <Threads/vjThreadKeySGI.h>

    typedef class vjThreadSGI	vjThread;
    typedef class vjThreadKeySGI	vjKeyId;
#else
#ifdef WIN32
#   include <windows.h>
#   include <process.h>
#   include <Threads/vjThreadWin32.h>
#   include <Threads/vjThreadKeyWin32.h>
    typedef vjThreadWin32 vjThread;
	typedef vjThreadKeyWin32 vjKeyId;
#else
#ifdef VJ_USE_PTHREADS
    typedef int		cancel_state_t;

#   define _POSIX_C_SOURCE VJ_POSIX_C_SOURCE

#   include <Threads/vjThreadPosix.h>
#   include <Threads/vjThreadKeyPosix.h>

    typedef class vjThreadPosix		vjThread;
    typedef class vjThreadKeyPosix	vjKeyId;
#else
#   include <Threads/vjThreadFunctor.h>
#endif	/* VJ_USE_PTHREADS */
#endif	/* WIN32 */
#endif	/* VJ_SGI_IPC */

   // Other thread related classes //
#include <Threads/vjThreadManager.h>


#endif	/* _VJ_THREAD_H_ */
