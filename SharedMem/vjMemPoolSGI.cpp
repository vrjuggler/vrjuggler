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
