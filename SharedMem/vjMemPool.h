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


#ifndef _MemPool_h_
#define _MemPool_h_
//#pragma once

#include <vjConfig.h>
#include <stdlib.h>

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
class vjMemPool {

public:
    // ---> This is a prototype for what all derived constructors should take as params
    // vjMemPool(size_t initialSize,  int numProcs, char* staticTempName) {
    virtual ~vjMemPool() {;}     	

public:
    virtual void* allocate(size_t size) = 0;
    virtual void  deallocate(void* ptr) = 0;
    virtual void* reallocate(void *ptr, size_t new_sz) = 0;

};

#if defined(VJ_IRIX_SPROC)
#   include <SharedMem/vjMemPoolSGI.h>
#   include <SharedMem/vjMemPoolHeap.h>

    typedef vjMemPoolSGI vjSharedPool;
    typedef vjMemPoolHeap vjHeapPool;
    typedef vjMemPoolSGI vjSyncPool;
#elif defined(VJ_OS_Win32)
#	include <SharedMem/vjMemPoolWin32.h>
#	include <SharedMem/vjMemPoolHeap.h>
                                             // Everything is heap!!!
	typedef vjMemPoolHeap vjSharedPool;
	typedef vjMemPoolHeap  vjHeapPool;
	typedef vjMemPoolHeap vjSyncPool;
#elif defined(VJ_OS_HPUX) || defined(VJ_USE_PTHREADS)
#   include <SharedMem/vjMemPool_malloc.h>
#   include <SharedMem/vjMemPoolHeap.h>

    typedef vjMemPool_malloc	vjSharedPool;
    typedef vjMemPoolHeap	vjHeapPool;
    typedef vjMemPool_malloc	vjSyncPool;
#endif	/* VJ_IRIX_SPROC */

#endif // ifdef vjMemPool_h
