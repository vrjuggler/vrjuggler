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


#ifndef _MemPoolSGI_h_
#define _MemPoolSGI_h_
//----------------------------------------------
// vjMemPoolSGI
//
// Purpose:
//    Shared Memory pool on the SGI systems
//    Used by vjMemory (Base class) to control allocation
//    and deallocation from a "memory pool"
//
// Use:
//    Clients should create memory pools as needed
//    Then when objects are created, they can pass
//    a pool as a parameter to the new (if the object
//    is a derived from vjMemory)
//
// NOTE:
//    The static function 'init' MUST be called before
//    any forks or other process splitting take place.
//    This is because it sets static data that must
//    be shared across processes.
//
// Author:
//	Allen Bierbaum
//
// Date: 1-9-97
//-----------------------------------------------

#include <vjConfig.h>
#include <stdio.h>
#include <unistd.h>
#include <ulocks.h>
//#include <Kernel/vjDebug.h>

// - Call usinit in new processes created.  Try to overcome limitation on number of users.
// otherwise wet to a "big" number.


class vjMemPoolSGI : public vjMemPool
{
public:
   vjMemPoolSGI(size_t initialSize = 65536, int numProcs = 8,
                char* staticTempName = "/var/tmp/memPoolSGIXXXXXX");

   virtual ~vjMemPoolSGI() {
      usdetach(arena);
      unlink(arenaFileName);
      cerr << "\nUnlinking: " << arenaFileName << endl;
   }

public:
   virtual void* allocate(size_t size)
   {
      void* retval;
      retval = usmalloc(size, arena);

      if (retval == NULL)
         cerr << "MemPoolSGI: Out of memory!!!" << endl;

      return retval;
   }

   virtual void deallocate(void* ptr)
   {
      usfree(ptr, arena);
   }

   virtual void* reallocate(void *ptr, size_t new_sz)
   {
      return usrealloc(ptr, new_sz, arena);
   }

public:      // Non-virtual functions
   usptr_t*    getArena() // Use with extreme caution  NOTE: Possibly use "friend" stuff
   { return arena;}

public:
   // Function must be called before any vjMemPools are created.
   // Automatically called by the first new with default values if not called
   // previously.
   // Function to initialize any STATIC data structures.
   static void init(size_t initialSize = 32768, int numProcs = 64,
                    char* staticTempName = "/var/tmp/memPoolsArenaXXXXXX");

   void* operator new(size_t sz)
   {
      cerr << "MemPoolSGI::new called. sz:" << sz << "\n";
      if (arenaForMemPools == NULL)
         init(); // Make sure that we are initialized already.

      return usmalloc(sizeof(vjMemPoolSGI), arenaForMemPools);
   }

   void operator delete(void* ptr)
   {
      usfree(ptr, arenaForMemPools);
   }

private:
   usptr_t*   arena;
   char*   arenaFileName;

private:    // Static data for all members -- Must be set before forks!!!!
   static usptr_t* arenaForMemPools;
   static char* arenaForMemPoolsFileName;
};

#endif
