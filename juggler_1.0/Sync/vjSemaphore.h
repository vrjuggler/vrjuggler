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
 *                             Patrick Hartling
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                  Copyright (C) - 1997, 1998, 1999, 2000
 *              Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#ifndef _VJSemaphore_h_
#define _VJSemaphore_h_

#include <vjConfig.h>

#ifdef VJ_SGI_IPC
#   include <ulocks.h>
#   include <Sync/vjSemaphoreSGI.h>
    
    typedef vjSemaphoreSGI vjSemaphore;
#else
#ifdef WIN32
#	include <Sync/vjSemaphoreWin32.h>
	typedef vjSemaphoreWin32 vjSemaphore;
#else
#if defined(VJ_USE_PTHREADS) || defined(VJ_USE_POSIX)
#   define _POSIX_C_SOURCE VJ_POSIX_C_SOURCE

#   include <Sync/vjSemaphorePosix.h>

    typedef vjSemaphorePosix vjSemaphore;
#endif	/* VJ_USE_PTHREADS || VJ_USE_POSIX */
#endif	/* WIN32 */
#endif	/* VJ_SGI_IPC */

#endif
