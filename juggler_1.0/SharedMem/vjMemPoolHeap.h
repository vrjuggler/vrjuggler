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


#ifndef _MemPoolHeap_h_
#define _MemPoolHeap_h_
//#pragma once

//----------------------------------------------
// vjMemPoolHeap
//
// Purpose:
//    Simple pool based on heap memory
//
// Author:
//	Allen Bierbaum
//
// Date: 1-9-97
//-----------------------------------------------

#include <vjConfig.h>
#include <stdlib.h>
#include <SharedMem/vjMemPool.h>


class vjMemPoolHeap : public vjMemPool {

public:
   static void init()
   {;}

   vjMemPoolHeap(size_t initialSize = 65536,  int numProcs = 8, char* staticTempName = "memPoolHeapXXXXXX")
   {;}

    virtual void* allocate(size_t size)
    {
	return malloc(size); 	
    }

    virtual void deallocate(void* ptr)
    {
	free(ptr);
    }

    virtual void* reallocate(void *ptr, size_t new_sz)
    {
	return realloc(ptr, new_sz);
    }
};

#endif
