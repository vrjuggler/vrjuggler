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
   vjMemPoolSGI(size_t initialSize = 65536,  int numProcs = 8, char* staticTempName = "/var/tmp/memPoolSGIXXXXXX") {
      cerr << "\n\nMemPoolSGI: Allocating Arena."
      << "\n\tSize: " << initialSize
      << "\n\tnProcs: " << numProcs << "\n" << flush;
      usconfig(CONF_INITUSERS, numProcs);
      usconfig(CONF_INITSIZE, initialSize);
      usconfig(CONF_AUTOGROW, 1);   // Default, but we set anyway
//#ifdef DEBUG_VJ
      usconfig(CONF_LOCKTYPE, US_DEBUGPLUS);    // what type of lock information
//#endif

      //static char* staticTempName = "/var/tmp/memPoolSGIXXXXXX";  // Do it this way because mktemp overwrite's the variable
      char* tempName = new char[strlen(staticTempName)+1];      // Therefore we need to use a non-static variable
      strcpy(tempName, staticTempName);

      arena = usinit(mktemp(tempName));   // Allocate the arena

      arenaFileName = tempName;      // So we know where the file is
      //delete tempName;		    // Delete the temporary file name

      if (arena == NULL)
      {
         perror("ERROR: vjMemPoolSGI::MemPoolSGI");
      }

      cerr << "\tfile: " << arenaFileName << endl;
      cerr << "\tpool: " << this << endl;
      cerr << "\tarena: " << arena << endl;
   }

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
   // Automatically called by the first new with default values if not called previously
   static void init(size_t initialSize = 32768, int numProcs = 64, char* staticTempName = "/var/tmp/memPoolsArenaXXXXXX")     // Function to initialize any STATIC data structures
   {
      if (arenaForMemPools == NULL)
      {
         cerr << "\n\nMemPoolSGI: Allocating Base Arena for ALL vjMemPoolSGI's."
         << "\n\tSize: " << initialSize
         << "\n\tnProcs: " << numProcs << "\n" << flush;
   
         usconfig(CONF_INITUSERS, numProcs);
         usconfig(CONF_INITSIZE, initialSize);
         usconfig(CONF_AUTOGROW, 1);   // Default, but we set anyway
   
         char* tempName = new char[strlen(staticTempName)+1];      // Therefore we need to use a non-static variable
         strcpy(tempName, staticTempName);
   
         arenaForMemPools = usinit(mktemp(tempName));
         unlink(tempName);
   
         if (arenaForMemPools == NULL)
         {
            perror("ERROR: vjMemPoolSGI::init. Was not able to get an arena!!!!");
         }
   
         arenaForMemPoolsFileName = (char*)usmalloc(strlen(staticTempName+1), arenaForMemPools);
         strcpy(arenaForMemPoolsFileName, tempName);
   
         cerr << "\tfile: " << arenaForMemPoolsFileName << endl;
         cerr << "\tarena: " << arenaForMemPools << endl;
      } else {
         cerr << "Tried to re-init the Base Arena for ALL vjMemPoolSGI's" << endl;
      }
   }

   void* operator new(size_t sz)
   {   
      cerr << "MemPoolSGI::new called.\n";
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
