/*
 *  File:	    $Name$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


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
