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

#include <SharedMem/vjMemPool.h>
#include <SharedMem/vjMemPoolSGI.h>

usptr_t*  vjMemPoolSGI::arenaForMemPools = NULL;
char* vjMemPoolSGI::arenaForMemPoolsFileName = NULL;


vjMemPoolSGI::vjMemPoolSGI (size_t initialSize, int numProcs,
                            char* staticTempName)
{
   cerr << "\n\nMemPoolSGI: Allocating Arena." << "\n\tSize: " << initialSize
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

void
vjMemPoolSGI::init (size_t initialSize, int numProcs, char* staticTempName) {
   if (arenaForMemPools == NULL)
   {
      cerr << "\n\nMemPoolSGI: Allocating Base Arena for ALL vjMemPoolSGI's."
           << "\n\tSize: " << initialSize << "\n\tnProcs: " << numProcs
           << "\n" << flush;
   
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
