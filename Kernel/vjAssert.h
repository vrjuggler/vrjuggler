#ifndef VJ_ASSERT_INCLUDED
#define VJ_ASSERT_INCLUDED

// -- ASSERT -- //
#ifdef VJ_DEBUG
#   define vjASSERT(val) assert((val))
#else
#   define vjASSERT(val) ((void)0)
#endif

#endif
