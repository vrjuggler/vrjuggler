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
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
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

#include <vpr/vprConfig.h>

#include <vpr/md/SPROC/SharedMem/MemPool.h>


usptr_t* vpr::MemPoolSGI::arenaForMemPools      = NULL;
char* vpr::MemPoolSGI::arenaForMemPoolsFileName = NULL;


vpr::MemPoolSGI::MemPoolSGI (size_t initialSize, int numProcs,
                             char* staticTempName)
{
   std::cerr.setf(std::ios::showbase);
   std::cerr << "\nMemPoolSGI: Allocating arena ("
             << initialSize << " bytes, "
             << numProcs  << " procs, "
             << std::hex  << this << std::dec
             << ")\n" << std::flush;

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
      perror("ERROR: PoolSGI::MemPoolSGI");
   }

   std::cerr << "  " << arenaFileName << ", "
             << "arena: " << std::hex << arena << std::dec << std::endl;
   std::cerr.unsetf(std::ios::showbase);
}

void
vpr::MemPoolSGI::init (size_t initialSize, int numProcs, char* staticTempName)
{
   if (arenaForMemPools == NULL)
   {
      std::cerr << "\nMemPoolSGI: Allocating Base Arena for ALL MemPoolSGI's.\n"
                << initialSize << " bytes, "
                << numProcs << " procs"
                << "\n" << std::flush;
   
      usconfig(CONF_INITUSERS, numProcs);
      usconfig(CONF_INITSIZE, initialSize);
      usconfig(CONF_AUTOGROW, 1);   // Default, but we set anyway
   
      char* tempName = strdup (staticTempName); // make mutable copy for mktemp
   
      arenaForMemPools = usinit(mktemp(tempName));
      unlink(tempName);
   
      if (arenaForMemPools == NULL)
      {
         perror("ERROR: MemPoolSGI::init. Was not able to get an arena!!!!");
      }
      arenaForMemPoolsFileName = (char*)usmalloc(strlen(staticTempName)+1, arenaForMemPools);
      strcpy(arenaForMemPoolsFileName, tempName);
      free (tempName);
   
      std::cerr.setf(std::ios::showbase);
      std::cerr << "  " << arenaForMemPoolsFileName << ", "
                << "arena: " << std::hex << arenaForMemPools << std::dec
                << std::endl;
      std::cerr.unsetf(std::ios::showbase);
   } else {
      std::cerr << "Tried to re-init the Base Arena for ALL MemPoolSGI's"
                << std::endl;
   }
}
