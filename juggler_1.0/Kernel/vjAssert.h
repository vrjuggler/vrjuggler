#ifndef VJ_ASSERT_INCLUDED
#define VJ_ASSERT_INCLUDED

#include <vjConfig.h>

// -- ASSERT -- //
#ifdef VJ_DEBUG
#   include <assert.h>
#   define vjASSERT(val) assert((val))
#else
#   define vjASSERT(val) ((void)0)
#endif

#endif
