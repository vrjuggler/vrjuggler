#include <vjConfig.h>

#include <SharedMem/vjMemPool.h>
#include <SharedMem/vjMemPoolSGI.h>

usptr_t*  vjMemPoolSGI::arenaForMemPools = NULL;
char* vjMemPoolSGI::arenaForMemPoolsFileName = NULL;
