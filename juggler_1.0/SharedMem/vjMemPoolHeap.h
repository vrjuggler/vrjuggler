#ifndef _MemPoolHeap_h_
#define _MemPoolHeap_h_
//----------------------------------------------
// vjMemPoolHeap
//
// Purpose:
//    First try at a pool implemenation
//
// Author:
//	Allen Bierbaum
//
// Date: 1-9-97
//-----------------------------------------------

#include <vjConfig.h>
#include <stdlib.h>
#include <iostream.h>
#include <SharedMem/vjMemPool.h>


class vjMemPoolHeap : public vjMemPool {

public:
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
