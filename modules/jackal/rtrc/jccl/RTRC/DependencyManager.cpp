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
 *                  Copyright (C) - 1997, 1998, 1999, 2000
 *              Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
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

   for(int i=0;i<mDepCheckers.size();i++)
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

   for(int cNum=0;cNum<mDepCheckers.size();cNum++)
   {
      vjDepChecker* checker = mDepCheckers[cNum];
      vjDEBUG(vjDBG_KERNEL,0) << cNum << ": Checker:" << (void*)checker
                 << "   type:" << typeid(*checker).name() << "\n" << vjDEBUG_FLUSH;
      vjDEBUG(vjDBG_KERNEL,0) << "   recog:" << checker->getCheckerName().c_str() << "\n" << vjDEBUG_FLUSH;
   }

   vjDEBUG_END(vjDBG_KERNEL,0) << "------ END DUMP ------\n" << vjDEBUG_FLUSH;
}

