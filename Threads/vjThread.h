/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/


#ifndef _VJ_THREAD_H_
#define _VJ_THREAD_H_
//#pragma once

#include <vjConfig.h>
//#include <unistd.h>


// Typedefs to help with cross-platform abilities
typedef void (*vj_thread_func_t)(void *);

// For backwards compatability with older versions...
typedef vj_thread_func_t THREAD_FUNC;

#include <Threads/vjBaseThread.h>


#if defined(VJ_IRIX_SPROC)      /* ---- SGI IPC Barrier ------ */
    typedef struct cancel_state cancel_state_t;

#   include <Threads/vjThreadSGI.h>
#   include <Threads/vjThreadKeySGI.h>

    typedef class vjThreadSGI    vjThread;
    typedef class vjThreadKeySGI vjKeyId;
#elif defined(VJ_OS_Win32)
#   include <windows.h>
#   include <process.h>
#   include <Threads/vjThreadWin32.h>
#   include <Threads/vjThreadKeyWin32.h>
    typedef vjThreadWin32 vjThread;
    typedef vjThreadKeyWin32 vjKeyId;
#elif defined(VJ_USE_PTHREADS)
    typedef int cancel_state_t;

#ifndef _POSIX_C_SOURCE
#   define _POSIX_C_SOURCE VJ_POSIX_C_SOURCE
#endif

#   include <Threads/vjThreadPosix.h>
#   include <Threads/vjThreadKeyPosix.h>

    typedef class vjThreadPosix    vjThread;
    typedef class vjThreadKeyPosix vjKeyId;
#else
#   include <Threads/vjThreadFunctor.h>
#endif  /* VJ_IRIX_SPROC */


#endif  /* _VJ_THREAD_H_ */
