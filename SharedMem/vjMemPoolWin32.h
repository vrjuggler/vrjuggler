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
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#ifndef _MemPoolWin32_h_
#define _MemPoolWin32_h_
//----------------------------------------------
// vjMemPoolWin32
//
// Purpose:
//    Shared Memory pool on the win32 systems
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
//	Andy Himberger
//
// Date: 11-6-97
//-----------------------------------------------

#include <iostream.h>
#include <stdio.h>
#include <windows.h>
#include <SharedMem/vjMemPool.h>

// - Call usinit in new processes created.  Try to overcome limitation on number of users.
// otherwise wet to a "big" number.


class vjMemPoolWin32 : public vjMemPool
{
public:    
   vjMemPoolWin32(size_t initialSize = 65536,  int numProcs = 8, char* staticTempName = "memPoolWin32XXXXXX") {
      cerr << "\n\nMemPoolWin32: Allocating Arena."
      << "\n\tSize: " << initialSize
      << "\n\tnProcs: " << numProcs << "\n" << flush;  

   }

   virtual ~vjMemPoolWin32() {
      cerr << "\nUnlinking: " << endl;
   }       

public:
   virtual void* allocate(size_t size)
   {
      void* retval;
      retval = malloc(size);

      if (retval == NULL)
         cerr << "MemPoolWin32: Out of memory!!!" << endl;

      return retval;    
   }

   virtual void* reallocate(void* ptr, size_t size)
   {
      void* retval;
      retval = realloc(ptr,size);
      cerr << "MemPoolWin32: realloc failure" << endl;

      return retval;
   }

   virtual void deallocate(void* ptr)
   {
      free(ptr);
   }

public:      // Non-virtual functions


   public:
   // Function must be called before any vjMemPools are created.
   // Automatically called by the first new with default values if not called previously
   static void init(size_t initialSize = 32768, int numProcs = 64, char* staticTempName = "/var/tmp/memPoolsArenaXXXXXX")     // Function to initialize any STATIC data structures
   {
      cerr << "\n\nMemPoolWin32: Allocating Base Arena for ALL vjMemPoolSGI's."
      << "\n\tSize: " << initialSize
      << "\n\tnProcs: " << numProcs << "\n" << flush;

   }

   void* operator new(size_t sz)
   {   
      cerr << "MemPoolWin32::new called.\n";
      //if (arenaForMemPools == NULL)
      //  init();	// Make sure that we are initialized already.

      return malloc(sizeof(vjMemPoolWin32));   
   }

   void operator delete(void* ptr)
   { 
      free(ptr);
   } 
private:
   void*   arena;
   char*   arenaFileName;

private:    // Static data for all members -- Must be set before forks!!!!
   static void* arenaForMemPools;
   static char* arenaForMemPoolsFileName;
};

void*  vjMemPoolWin32::arenaForMemPools = NULL;
char* vjMemPoolWin32::arenaForMemPoolsFileName = NULL;
#endif
