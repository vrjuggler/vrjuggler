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
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#ifndef _MemPoolHeap_h_
#define _MemPoolHeap_h_
//#pragma once

//----------------------------------------------
// vjMemPoolHeap
//
// Purpose:
//    Simple pool based on heap memory
//
// Author:
//	Allen Bierbaum
//
// Date: 1-9-97
//-----------------------------------------------

#include <vjConfig.h>
#include <stdlib.h>
#include <SharedMem/vjMemPool.h>


class vjMemPoolHeap : public vjMemPool {

public:
   static void init()
   {;}

   vjMemPoolHeap(size_t initialSize = 65536,  int numProcs = 8, char* staticTempName = "memPoolHeapXXXXXX")
   {;}

    virtual void* allocate(size_t size)
    {
	return malloc(size); 	
    }

    virtual void deallocate(void* ptr)
    {
	free(ptr);
    }

    virtual void* reallocate(void *ptr, size_t new_sz)
    {
	return realloc(ptr, new_sz);
    }
};

#endif
