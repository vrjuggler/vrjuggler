#include <vjConfig.h>
#include <SharedMem/vjMemPoolWin32.h>


void*  vjMemPoolWin32::arenaForMemPools = NULL;
char* vjMemPoolWin32::arenaForMemPoolsFileName = NULL;
