/*
 *  File:       $RCSfile$
 *  Date modified:  $Date$
 *  Version:       $Revision$
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

#if defined(VJ_IRIX_SPROC)
#   include <ulocks.h>
#   include <Sync/vjSemaphoreSGI.h>
    
    typedef vjSemaphoreSGI vjSemaphore;
#elif defined(VJ_OS_Win32)
#  include <Sync/vjSemaphoreWin32.h>
   typedef vjSemaphoreWin32 vjSemaphore;
#elif defined(VJ_USE_PTHREADS) || defined(VJ_USE_POSIX)
#ifndef _POSIX_C_SOURCE
#   define _POSIX_C_SOURCE VJ_POSIX_C_SOURCE
#endif

#   include <Sync/vjSemaphorePosix.h>

    typedef vjSemaphorePosix vjSemaphore;
#endif   /* VJ_IRIX_SPROC */

#endif
