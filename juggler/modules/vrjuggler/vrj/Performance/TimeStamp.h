
/* vjTimeStamp... a simple class to enclose a timestamp */


#ifndef _VJ_TIMESTAMP_
#define _VJ_TIMESTAMP_

#if VJ_PERFORMANCE == VJ_PERF_SGI

#include <Performance/vjTimeStampSGI.h>
typedef class vjTimeStampSGI vjTimeStamp;

#elif VJ_PERFORMANCE == VJ_PERF_POSIX

#include <Performance/vjTimeStampPosix.h>
typedef class vjTimeStampPosix vjTimeStamp;

#else

#include <Performance/vjTimeStampNone.h>
typedef class vjTimeStampNone vjTimeStamp;

#endif


#endif
