
#include <Kernel/vjStreamLock.h>


ostream& operator<<(ostream& s, const vjStreamLock& streamLock)
{
   streamLock.mMutex.acquire();
   
   return s;
}


ostream& operator<<(ostream& s, const vjStreamUnLock& streamUnLock)
{
   streamUnLock.mMutex.release();
   
   return s;
}
