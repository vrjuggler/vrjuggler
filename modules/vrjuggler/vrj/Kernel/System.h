#ifndef _VJSYSTEM_H_
#define _VJSYSTEM_H_

#include <config.h>
#include <Sync/vjMutex.h>

//------------------------------------------------------
//:  This class holds the state of the library system
//
//-------------------------------------------------------
class vjSystem : public vjMemory
{
// ----------- Base vjMemPool Stuff    --------- //
//  Since we need to put the singleton in Shared memory 
// Nothing in this section is safe after any forking !!! 
// Must be set BEFORE instance is created.
public:
    static setBaseSize(size_t size)
    { baseMemPoolSize = size; }
    
    static setBaseNumProcs(int numProcs)
    { baseNumProcs = numProcs; }
    
private:
    static vjSharedPool*  baseMemPool;
    static size_t baseMemPoolSize;
    static int    baseNumProcs;
    
    
//---------------------------------------------//
//------------  Singleton Stuff ---------------//
//---------------------------------------------//


// -------- Pure Singleton Stuff ------------- //
public:
    static void init()	// Just makes sure that we have been allocated
    { getInstance(); }
    
    static vjSystem* getInstance()
    {
	if (_instance == NULL) {
	    // Called automatically --- vjSharedPool::init();   // Initialize the vjMemPoolStuff
	    baseMemPool = new vjSharedPool(baseMemPoolSize, baseNumProcs);
	    _instance = new vjSystem(baseMemPool);
	}
	return _instance;
    }
  
protected:
	// -- Constructor protected so it cannot be called except by getInstance --- //
    vjSystem()
    {
	
    }
private:
    static vjSystem* _instance;
};

static vjSystem::_instance = NULL;
static vjSystem::baseMemPool

#endif  /* _VJSYSTEM_H_ */



  
