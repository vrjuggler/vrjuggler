#include <vjConfig.h>

#include <SharedMem/vjMemory.h>

    // --- Allocate from the given vjMemPool --- //
void* vjMemory::operator new(size_t sz, vjMemPool* mp)
    {   
      void* retVal;
      vjMemory* me;
      retVal = mp->allocate(sz);
      me = (vjMemory*)retVal;
      me->memPool = mp;
      return retVal;
    } 

void* vjMemory::operator new(size_t sz)
    { 
	void* retVal;
	vjMemory* me;
	retVal = ::operator new(sz);
	me = (vjMemory*)retVal;
	me->memPool = NULL;	    // This means - Allocated normally
	return retVal;
    }
