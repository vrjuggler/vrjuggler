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
#include <Kernel/vjDependencyManager.h>

// Initialize the singleton ptr
vjDependencyManager* vjDependencyManager::mInstance = NULL;


vjDepChecker* vjDependencyManager::findDepChecker(vjConfigChunk* chunk)
{
   vjASSERT(NULL != chunk);

   //std::string chunk_type;
   //chunk_type = (std::string)chunk->getType();

   for(unsigned int i=0;i<mDepCheckers.size();i++)
   {
      // Get next constructor
      vjDepChecker* checker = mDepCheckers[i];
      vjASSERT(checker != NULL);

      if(checker->canHandle(chunk))
         return checker;
   }

   return &mDefaultChecker;
}


void vjDependencyManager::debugDump()
{
   vjDEBUG_BEGIN(vjDBG_KERNEL,0) << "vjDepChecker::debugDump\n" << vjDEBUG_FLUSH;
   vjDEBUG(vjDBG_KERNEL,0) << "num checkers:" << mDepCheckers.size() << "\n" << vjDEBUG_FLUSH;

   for(unsigned int cNum=0;cNum<mDepCheckers.size();cNum++)
   {
      vjDepChecker* checker = mDepCheckers[cNum];
      vjDEBUG(vjDBG_KERNEL,0) << cNum << ": Checker:" << (void*)checker
                 << "   type:" << typeid(*checker).name() << "\n" << vjDEBUG_FLUSH;
      vjDEBUG(vjDBG_KERNEL,0) << "   recog:" << checker->getCheckerName().c_str() << "\n" << vjDEBUG_FLUSH;
   }

   vjDEBUG_END(vjDBG_KERNEL,0) << "------ END DUMP ------\n" << vjDEBUG_FLUSH;
}

