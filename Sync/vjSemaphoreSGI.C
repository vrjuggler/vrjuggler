#include <vjConfig.h>

#include <Sync/vjSemaphore.h>
#include <Sync/vjSemaphoreSGI.h>

vjMemPoolSGI* vjSemaphoreSGI::semaphorePool = NULL;
int* vjSemaphoreSGI::attachedCounter = NULL;
