#include <config.h>

#include <Sync/vjMutex.h>
#include <Kernel/vjDebug.h>

vjMutex DebugLock;
vjDebug* vjDebug::_instance = NULL;
