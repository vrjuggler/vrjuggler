
// 
// vjSyncWriter.h

#ifndef _VJ_SYNC_WRITER_
#define _VJ_SYNC_WRITER_

#include <vjConfig.h>
#include <iostream.h>
#include <fstream.h>
#include <Sync/vjMutex.h>

//: an output fstream with an associated mutex, so multiple
//+ procs can write to it w/out getting munged.
class vjSyncWriter: public ofstream {

private:

    vjMutex    mutex;

public:

    vjSyncWriter();

    int lock();

    int unlock();


};



#endif
