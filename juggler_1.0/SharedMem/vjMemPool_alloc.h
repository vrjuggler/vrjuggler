/*
 *  File:	    $Name$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


// This file defines an STL allocator that works with
// vjMemPools.

#ifndef _MEMPOOL_ALLOC_H_
#define _MEMPOOL_ALLOC_H_

#include <vjConfig.h>
#include <alloc.h>
#include <SharedMem/vjMemPool.h>


// Malloc-based allocator.  Typically slower than default alloc below.
// Typically thread-safe and more storage efficient.
#ifdef __STL_STATIC_TEMPLATE_MEMBER_BUG
# ifdef __DECLARE_GLOBALS_HERE
    void (* __mempool_alloc_oom_handler)() = 0;
    // g++ 2.7.2 does not handle static template data members.
# else
    extern void (* __mempool_alloc_oom_handler)();
# endif
#endif

template <int inst>
class __vj_mempool_alloc_template
{

private:

   static void *oom_malloc(size_t);

   static void *oom_realloc(void *, size_t);

   #ifndef __STL_STATIC_TEMPLATE_MEMBER_BUG
   static void (* __mempool_alloc_oom_handler)();
   #endif

public:

   static void * allocate(size_t n)
   {
      verifyMempoolAllocated();
      void *result = STLMemPool->allocate(n);
      if (0 == result) result = oom_malloc(n);
      return result;
   }

   static void deallocate(void *p, size_t /* n */)
   {
      STLMemPool->deallocate(p);
   }

   static void * reallocate(void *p, size_t /* old_sz */, size_t new_sz)
   {
      void * result = STLMemPool->reallocate(p, new_sz);
      if (0 == result) result = oom_realloc(p, new_sz);
      return result;
   }

   static void (* set_malloc_handler(void (*f)()))()
   {
      void (* old)() = __mempool_alloc_oom_handler;
      __mempool_alloc_oom_handler = f;
      return (old);
   }

public:

   static void verifyMempoolAllocated()
   {
      if (STLMemPool == NULL)      // ASSERT: No allocated
      {
         STLMemPool = new vjSharedPool(1024*1024);
      }
   }

   static vjMemPool* STLMemPool;   
};

MemPool* __vj_mempool_alloc_template<0>::STLMemPool = NULL;

// mempool_alloc out-of-memory handling

#ifndef __STL_STATIC_TEMPLATE_MEMBER_BUG
template <int inst>
void (* __vj_mempool_alloc_template<inst>::__mempool_alloc_oom_handler)() = 0;
#endif

template <int inst>
void * __vj_mempool_alloc_template<inst>::oom_malloc(size_t n)
{
    void (* my_malloc_handler)();
    void *result;

    for (;;) {
        my_malloc_handler = __mempool_alloc_oom_handler;
        if (0 == my_malloc_handler) { __THROW_BAD_ALLOC; }
        (*my_malloc_handler)();
        result = malloc(n);
        if (result) return(result);
    }
}

template <int inst>
void * __vj_mempool_alloc_template<inst>::oom_realloc(void *p, size_t n)
{
    void (* my_malloc_handler)();
    void *result;

    for (;;) {
        my_malloc_handler = __mempool_alloc_oom_handler;
        if (0 == my_malloc_handler) { __THROW_BAD_ALLOC; }
        (*my_malloc_handler)();
        result = realloc(p, n);
        if (result) return(result);
    }
}

typedef __vj_mempool_alloc_template<0> mempool_alloc;

#endif  // _MEMPOOL_ALLOC_H_
