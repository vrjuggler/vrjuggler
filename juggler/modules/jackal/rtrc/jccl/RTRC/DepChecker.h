/*
 *  File:       $RCSfile$
 *  Date modified:  $Date$
 *  Version:       $Revision$
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


#ifndef _VJ_DEP_CHECKER_H_
#define _VJ_DEP_CHECKER_H_
//#pragma once

#include <vjConfig.h>
#include <Input/vjInput/vjInput.h>
#include <Config/vjConfigChunk.h>
#include <Kernel/vjConfigManager.h>

//: Base class for dependency checkers
// 
// A dependency checker is responsible for figuring out
// if the system has all the required dependencies filled
// for a given configChunk
//<br>
// This class also implements a default behavior for
// dependency checkers, that just looks at the chunk
// ptrs in the given chunk and returns true if
// all chunk ptrs have those corresponding chunks
// loaded in the current configuration.
//
class vjDepChecker
{
public:
   vjDepChecker()
   {;}

   //: Return a string name of the checker
   // Used to output messages in checker listings
   virtual std::string getCheckerName()
   { return std::string("Default Checker"); }

   // Can we handle the given chunk type?
   // Default to true, because the default checker can check anything
   virtual bool canHandle(vjConfigChunk* chunk)
   {
      return true;  
   }

   //: Are the dependencies satisfied?
   //! RETURNS: true - dependencies are satisfied
   virtual bool depSatisfied(vjConfigChunk* chunk)
   {
      bool pass=true;

      vjConfigManager* cfg_mgr = vjConfigManager::instance();
      
      // Get the list of dependencies
      std::vector<std::string> dependencies = chunk->getChunkPtrDependencies();

      // Check to see if they are loaded already
      for(int i=0;i<dependencies.size();i++)
      {
         if(!cfg_mgr->isChunkInActiveList(dependencies[i]));
            pass = false;      
      }
      return pass;
   }
};

#endif

