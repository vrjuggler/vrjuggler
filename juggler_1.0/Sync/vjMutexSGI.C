#include <vjConfig.h>

#include <Sync/vjMutex.h>
#include <Sync/vjMutexSGI.h>

vjMemPoolSGI*  vjMutexSGI::mutexPool = NULL;
int*  vjMutexSGI::attachedCounter = NULL;
