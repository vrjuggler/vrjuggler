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



/* vjTimeStamp... a simple class to enclose a timestamp */


#ifndef _VJ_TIMESTAMP_
#define _VJ_TIMESTAMP_

#include <vjConfig.h>

#if VJ_PERFORMANCE == VJ_PERF_SGI

#include <Performance/vjTimeStampSGI.h>
typedef class vjTimeStampSGI vjTimeStamp;

#elif VJ_PERFORMANCE == VJ_PERF_POSIX

#include <Performance/vjTimeStampPosix.h>
typedef class vjTimeStampPosix vjTimeStamp;

#else

#include <Performance/vjTimeStampNone.h>
typedef class vjTimeStampNone vjTimeStamp;

#endif


#endif
