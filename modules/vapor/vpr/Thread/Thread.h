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

#ifndef _VPR_THREAD_H_
#define _VPR_THREAD_H_
//#pragma once

#include <vpr/vprConfig.h>
//#include <unistd.h>

#include <vpr/Thread/BaseThread.h>


namespace vpr {
    // Typedef to help with cross-platform abilities.
    typedef void (*thread_func_t)(void *);
};

/* IRIX SPROC */
#if defined(VPR_USE_IRIX_SPROC)
namespace vpr {
    typedef struct cancel_state cancel_state_t;

    typedef class ThreadSGI    Thread;
    typedef class ThreadKeySGI KeyId;
};

#   include <vpr/md/SPROC/Thread/ThreadSGI.h>
#   include <vpr/md/SPROC/Thread/ThreadKeySGI.h>

/* POSIX */
#elif defined(VPR_USE_PTHREADS)
namespace vpr {
    typedef int cancel_state_t;
    typedef class ThreadPosix    Thread;
    typedef class ThreadKeyPosix KeyId;
};

#ifndef _POSIX_C_SOURCE
#   define _POSIX_C_SOURCE VPR_POSIX_C_SOURCE
#endif

#   include <vpr/md/POSIX/Thread/ThreadPosix.h>
#   include <vpr/md/POSIX/Thread/ThreadKeyPosix.h>

/* NSPR */
#else
#   include <vpr/md/NSPR/Thread/ThreadNSPR.h>

namespace vpr {
    typedef class ThreadNSPR    Thread;
//    typedef class ThreadKeyNSPR KeyId;
};
#endif  /* VPR_USE_IRIX_SPROC */


#endif  /* _VPR_THREAD_H_ */
