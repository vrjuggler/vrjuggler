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

#ifndef _MemPool_h_
#define _MemPool_h_
//#pragma once

#include <vprConfig.h>
#include <stdlib.h>

namespace  {

//: Shared memory pool.
//
//    Base class that defines how all memory pools
// should behave.
//
// Author:
//	Allen Bierbaum
//
// Date: 1-9-97
//
class MemPool {

public:
    // ---> This is a prototype for what all derived constructors should take
    // as params
    // MemPool(size_t initialSize,  int numProcs, char* staticTempName)
    virtual ~MemPool() {;}

public:
    virtual void* allocate(size_t size) = 0;
    virtual void  deallocate(void* ptr) = 0;
    virtual void* reallocate(void *ptr, size_t new_sz) = 0;

};

}; // End of vpr namespace

#if defined(VPR_IRIX_SPROC)
#   include <md/SPROC/MemPoolSGI.h>
#   include <SharedMem/MemPoolHeap.h>

namespace vpr {
    typedef MemPoolSGI  SharedPool;
    typedef MemPoolHeap HeapPool;
    typedef MemPoolSGI  SyncPool;
};
#elif defined(VPR_OS_Win32)
#   include <md/WIN32/MemPoolWin32.h>
#   include <SharedMem/MemPoolHeap.h>  // Everything is heap!!!

namespace vpr {
    typedef MemPoolHeap SharedPool;
    typedef MemPoolHeap HeapPool;
    typedef MemPoolHeap SyncPool;
};
#elif defined(VPR_USE_PTHREADS)
#   include <md/POSIX/MemPoolPosix.h>
#   include <SharedMem/MemPoolHeap.h>

namespace vpr {
    typedef MemPoolPosix SharedPool;
    typedef MemPoolHeap  HeapPool;
    typedef MemPoolPosix SyncPool;
};
#else
#   include <md/NSPR/MemPoolNSPR.h>
#   include <SharedMem/MemPoolHeap.h>

namespace vpr {
    typedef MemPoolNSPR SharedPool;
    typedef MemPoolHeap HeapPool;
    typedef MemPoolNSPR SyncPool;
};
#endif	/* VPR_IRIX_SPROC */

#endif	/* _MemPool_h_ */
