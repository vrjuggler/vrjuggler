#include <vjConfig.h>
#include <Sync/vjBarrierSGI.h>

vjMemPoolSGI* vjBarrierSGI::barrierPool = NULL;
int* vjBarrierSGI::attachedCounter      = NULL;
