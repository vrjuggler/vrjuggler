/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ****************** <VPR heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VPR_MemPoolSGI_h_
#define _VPR_MemPoolSGI_h_

//----------------------------------------------
// vpr::MemPoolSGI
//
// Purpose:
//    Shared Memory pool on the SGI systems
//    Used by vprMemory (Base class) to control allocation
//    and deallocation from a "memory pool"
//
// Use:
//    Clients should create memory pools as needed
//    Then when objects are created, they can pass
//    a pool as a parameter to the new (if the object
//    is a derived from vprMemory)
//
// NOTE:
//    The static function 'init' MUST be called before
//    any forks or other process splitting take place.
//    This is because it sets static data that must
//    be shared across processes.
//
// Author:
//  Allen Bierbaum
//
// Date: 1-9-97
//-----------------------------------------------

#include <vpr/vprConfig.h>
#include <stdio.h>
#include <unistd.h>
#include <ulocks.h>


// - Call usinit in new processes created.  Try to overcome limitation on number of users.
// otherwise wet to a "big" number.


namespace vpr
{

class MemPoolSGI : public MemPool
{
public:
   MemPoolSGI(size_t initialSize = 65536, int numProcs = 8,
              char* staticTempName = "/var/tmp/memPoolSGIXXXXXX");

   virtual ~MemPoolSGI()
   {
      usdetach(arena);
      unlink(arenaFileName);
      std::cerr << "\nUnlinking: " << arenaFileName << std::endl;
   }

public:
   virtual void* allocate(size_t size)
   {
      void* retval;
      retval = usmalloc(size, arena);

      if ( retval == NULL )
      {
         std::cerr << "MemPoolSGI: Out of memory!!!" << std::endl;
      }

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
   {
      return arena;
   }

public:
   // Function must be called before any vpr::MemPools are created.
   // Automatically called by the first new with default values if not called
   // previously.
   // Function to initialize any STATIC data structures.
   static void init(size_t initialSize = 32768, int numProcs = 64,
                    char* staticTempName = "/var/tmp/memPoolsArenaXXXXXX");

   void* operator new(size_t sz)
   {
      std::cerr << "MemPoolSGI::new called. sz:" << sz << "\n";
      if ( arenaForMemPools == NULL )
      {
         init(); // Make sure that we are initialized already.
      }

      return usmalloc(sizeof(MemPoolSGI), arenaForMemPools);
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

} // End of vpr namespace


#endif
