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


#ifndef _VJ_MUTEX_H_
#define _VJ_MUTEX_H_
//----------------------------------------------
// vjMutex
//
// Purpose:
//    Defines a mutex wrapper.  No base class because
//   we don't want to have to use virtual functions.
//
// Author:
//	Allen Bierbaum
//
// Date: 1-20-97
//-----------------------------------------------

#include <vjConfig.h>

#if defined(VJ_SGI_IPC)
#   include <ulocks.h>
#   include <Sync/vjMutexSGI.h>    
    
    typedef  vjMutexSGI vjMutex;
#elif defined(VJ_OS_Win32)
#	include <Sync/vjMutexWin32.h>
	typedef vjMutexWin32 vjMutex;
#elif defined(VJ_USE_PTHREADS)
#   include <Sync/vjMutexPosix.h>

    typedef  vjMutexPosix vjMutex;
#endif	/* VJ_SGI_IPC */

#include <Sync/vjNullMutex.h>


#endif	/* _VJ_MUTEX_H_ */
