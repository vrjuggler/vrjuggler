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


#ifndef _VJCond_h_
#define _VJCond_h_

#include <vjConfig.h>

#if defined(VJ_SGI_IPC)
#    include <ulocks.h>
#    include <Sync/vjCondGeneric.h>
    
    typedef  vjCondGeneric vjCond;
#elif defined(VJ_USE_PTHREADS)
#   define _POSIX_C_SOURCE VJ_POSIX_C_SOURCE

#   include <Sync/vjCondPosix.h>
    
    typedef  vjCondPosix vjCond;
#elif defined(VJ_OS_Win32)
#    include <Sync/vjCondGeneric.h>
    
    typedef  vjCondGeneric vjCond;
#endif	/* VJ_SGI_IPC */

#endif
