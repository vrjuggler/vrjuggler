/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
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
