/*
 *  File:	    $RCSfile$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                             Patrick Hartling
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                  Copyright (C) - 1997, 1998, 1999, 2000
 *              Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#ifndef _VJMemory_h_
#define _VJMemory_h_

//class vjMemory;

#include <vjConfig.h>
#include <sys/types.h>
#include <stdlib.h>

#include <SharedMem/vjMemPool.h>

class vjMemory;

// -- Just allocate to much memory
// -- Then move pointer over by siezeof(memPool)

// vjMemory
//
// Purpose:
//:	Base Class for all objects that want memory abilities.
//	This class allows object to be allocated/deallocated
//      from memory "pools".  It takes care of the new and delete
//	as well as tracking the pool.
//
// Author:
//	Allen Bierbaum
//
// Date: 1-9-97
class vjMemory {
public:
    vjMemory()
    {
	memPool = NULL;
    }
public:
    // -----------------------------------------------------------------------
    //: Allows access to be allocate on default heap rather than the memPool.
    // -----------------------------------------------------------------------
    void* operator new(size_t sz);
    
    // -----------------------------------------------------------------------
    //: Allocates memory of size from the given memPool.
    // -----------------------------------------------------------------------
    void* operator new(size_t sz, vjMemPool* mp);
    
    // -----------------------------------------------------------------------
    //: Same as operator delete, reverses effects of new.
    // -----------------------------------------------------------------------
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
    
    // -----------------------------------------------------------------------
    //: Reverses the effects of either new operator.
    // -----------------------------------------------------------------------
    void operator delete(void* ptr)
    { 
        static_cast<vjMemory*>(ptr)->localDelete(ptr);
    }

    // -----------------------------------------------------------------------
    //: Return the memPool that the derived object is living.
    //  This MUST be used for vjMemory derived classes to dynamically create
    //  other vjMemory derivced classes in the form:
    //
    //     myClass *mc = new ( getMyMemPool() ) myClass;
    // -----------------------------------------------------------------------
    vjMemPool* getMyMemPool()
    {
	return memPool;
    }

    // -----------------------------------------------------------------------
    //: An object should use allocate to get more memory from its memPool.
    // -----------------------------------------------------------------------
    void* allocate(size_t size)
    {
	return getMyMemPool()->allocate(size);
    }
    
    // -----------------------------------------------------------------------
    //: Memory allocated using allocate should be deallocated with this
    //+ function.
    //
    //! PRE: ptr was allocated with allocate(size) by the same object making
    //+      this call.
    // -----------------------------------------------------------------------
    void deallocate(void* ptr)
    {
	getMyMemPool()->deallocate(ptr);
    }
    
private:
    vjMemPool* memPool;
};

#endif
