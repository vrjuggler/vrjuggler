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

#ifdef VJ_SGI_IPC
#    include <ulocks.h>
#    include <Sync/vjCondGeneric.h>
    
    typedef  vjCondGeneric vjCond;
#endif
#ifdef VJ_USE_PTHREADS
#   define _POSIX_C_SOURCE VJ_POSIX_C_SOURCE

#   include <Sync/vjCondPosix.h>
    
    typedef  vjCondPosix vjCond;
#endif	/* VJ_USE_PTHREADS */
#ifdef WIN32
#    include <Sync/vjCondGeneric.h>
    
    typedef  vjCondGeneric vjCond;
#endif

#endif
