#ifndef _VJMemory_h_
#define _VJMemory_h_

//class vjMemory;

#include <config.h>
#include <sys/types.h>
#include <stdlib.h>

#include <SharedMem/vjMemPool.h>

class vjMemory;

// -- Just allocate to much memory
// -- Then move pointer over by siezeof(memPool)

/**
 * vjMemory
 *
 * Purpose:
 *	Base Class for all objects that want memory abilities
 *	This class allows object to be allocated/deallocated
 *      from memory "pools".  This class takes care of the new
 *	and delete as well as tracking the pool.
 *
 * Author:
 *	Allen Bierbaum
 *
 * Date: 1-9-97
 */
class vjMemory {
public:
    vjMemory()
    {
	;
    }
public:
    /** operator new(size)
     *
     *  allows access to be allocate on default heap
     *  rather than the memPool
     */
    void* operator new(size_t sz);
    
    /** operator new(size, memPool)
     *
     *  allocates memory of size from the given memPool
     *
     */
    void* operator new(size_t sz, vjMemPool* mp);
    
    /** localDelete(ptr)
     *
     *   same as operator delete, reverses effects of new
     */
    void localDelete(void* ptr)
    {    	    
	if (memPool != NULL)
	    memPool->deallocate(ptr);
	else
#ifdef VJ_OS_HPUX
            free(ptr);
#else
	    delete ptr;
#endif
    }
    
    /** delete(ptr)
     *
     *  reverses the effects of either new operator
     */
    void operator delete(void* ptr)
    { 
        static_cast<vjMemory*>(ptr)->localDelete(ptr);
    }

    /** getMyMemPool()
     *
     *    Return the memPool that the derived object is living,
     *  this MUST be used for vjMemory derived classes to dynamically
     *  create other vjMemory derivced classes in the form:
     *
     *     myClass *mc = new ( getMyMemPool() ) myClass;
     */	
    vjMemPool* getMyMemPool()
    {
	return memPool;
    }

    /** allocate(size)--> to be used in classes when new (getMyMemPool())
     *                --> is wierd or you're converting from malloc
     *
     *  An object should use allocate to get more memory from its
     *  memPool.
     */
    void* allocate(size_t size)
    {
	return getMyMemPool()->allocate(size);
    }
    
    /** deallocate(ptr)
     *  pre: ptr was allocated with allocate(size) by the same
     *       object that making this call
     *  modifies: memPool
     *
     *  Memory allocated using allocate should be deallocated with
     *  this function.
     */
    void deallocate(void* ptr)
    {
	getMyMemPool()->deallocate(ptr);
    }
    
private:
    vjMemPool* memPool;
};

#endif
