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

#if defined(VJ_IRIX_SPROC)
#   include <ulocks.h>
#   include <Sync/vjMutexSGI.h>    
    
    typedef  vjMutexSGI vjMutex;
#elif defined(VJ_OS_Win32)
#	include <Sync/vjMutexWin32.h>
	typedef vjMutexWin32 vjMutex;
#elif defined(VJ_USE_PTHREADS)
#   include <Sync/vjMutexPosix.h>

    typedef  vjMutexPosix vjMutex;
#endif	/* VJ_IRIX_SPROC */

#include <Sync/vjNullMutex.h>


#endif	/* _VJ_MUTEX_H_ */
