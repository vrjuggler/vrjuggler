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

#ifdef VJ_SGI_IPC
#   include <SharedMem/vjMemPoolSGI.h>
#   include <SharedMem/vjMemPoolHeap.h>

    typedef vjMemPoolSGI vjSharedPool;
    typedef vjMemPoolHeap vjHeapPool;
    typedef vjMemPoolSGI vjSyncPool;
#else
#ifdef WIN32
#	include <SharedMem/vjMemPoolWin32.h>
#	include <SharedMem/vjMemPoolHeap.h>
                                             // Everything is heap!!!
	typedef vjMemPoolHeap vjSharedPool;
	typedef vjMemPoolHeap  vjHeapPool;
	typedef vjMemPoolHeap vjSyncPool;
#else
#if defined(VJ_OS_HPUX) || defined(VJ_USE_PTHREADS)
#   include <SharedMem/vjMemPool_malloc.h>
#   include <SharedMem/vjMemPoolHeap.h>

    typedef vjMemPool_malloc	vjSharedPool;
    typedef vjMemPoolHeap	vjHeapPool;
    typedef vjMemPool_malloc	vjSyncPool;
#endif	// if defined(VJ_OS_HPUX) || defined(VJ_USE_PTHREADS)
#endif // ifdef WIN32
#endif // if defined(VJ_OS_SGI) && ! defined(VJ_USE_PTHREADS)

#endif // ifdef vjMemPool_h
