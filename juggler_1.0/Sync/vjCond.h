#ifndef _VJCond_h_
#define _VJCond_h_

#include <config.h>

#ifdef VJ_SGI_IPC
#    include <ulocks.h>
#    include <Sync/vjCondGeneric.h>
    
    typedef  vjCondGeneric vjCond;
#else
#ifdef VJ_USE_PTHREADS
#   define _POSIX_C_SOURCE 2

#   include <Sync/vjCondPosix.h>
    
    typedef  vjCondPosix vjCond;
#endif	/* VJ_USE_PTHREADS */
#endif	/* VJ_SGI_IPC */

#endif
