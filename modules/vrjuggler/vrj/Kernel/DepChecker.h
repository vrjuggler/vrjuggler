/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VRJ_DEP_CHECKER_H_
#define _VRJ_DEP_CHECKER_H_
//#pragma once

#include <vrj/vjConfig.h>
#include <vrj/Input/Type/Input.h>
#include <vrj/Config/ConfigChunk.h>
#include <vrj/Kernel/ConfigManager.h>

namespace vrj
{
   
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
// <br>
// Configuration information with special
// dependency requirements should implement
// a specialization of this class and register
// it with the dependency checker.
// <br>
// NOTE: It must be registered BEFORE
// a chunk of the given type is checked for dependencies.
//!PUBLIC_API
class DepChecker
{
public:
   DepChecker()
   {;}

   //: Return a string name of the checker
   // Used to output messages in checker listings
   virtual std::string getCheckerName()
   { return std::string("Default Checker"); }

   // Can we handle the given chunk type?
   // Default to true, because the default checker can check anything
   virtual bool canHandle(ConfigChunk* chunk)
   {
      return true;
   }

   //: Are the dependencies satisfied?
   //! RETURNS: true - dependencies are satisfied
   virtual bool depSatisfied(ConfigChunk* chunk)
   {
      bool pass=true;

      ConfigManager* cfg_mgr = ConfigManager::instance();

      // Get the list of dependencies
      std::vector<std::string> dependencies = chunk->getChunkPtrDependencies();

      // Check to see if they are loaded already
      for(unsigned int i=0;i<dependencies.size();i++)
      {
         if(!cfg_mgr->isChunkInActiveList(dependencies[i]))
            pass = false;
      }
      return pass;
   }

   // Write out the dependencies to the vjDEBUG macro
   virtual void debugOutDependencies(ConfigChunk* chunk,int dbg_lvl=vjDBG_WARNING_LVL)
   {
      vjDEBUG_NEXT_BEGIN(vjDBG_ALL,dbg_lvl) << "---- Dependencies for: item: "
                                            << chunk->getProperty("name")
                                            << " type: " << ((std::string)chunk->getType()).c_str()
                                            << "-------\n" << vjDEBUG_FLUSH;

      ConfigManager* cfg_mgr = ConfigManager::instance();

      // Get the list of dependencies
      std::vector<std::string> dependencies = chunk->getChunkPtrDependencies();

      // Check to see if they are loaded already
      for(unsigned int i=0;i<dependencies.size();i++)
      {
         vjDEBUG_NEXT(vjDBG_ALL,dbg_lvl) << i << ": "
                                         << dependencies[i].c_str()
                                         << " ==> " << vjDEBUG_FLUSH;
         if(!cfg_mgr->isChunkInActiveList(dependencies[i]))
         {
            vjDEBUG_CONT(vjDBG_ALL,dbg_lvl) << "not available.\n" << vjDEBUG_FLUSH;
         }
         else
         {
            vjDEBUG_CONT(vjDBG_ALL,dbg_lvl) << "passed.\n" << vjDEBUG_FLUSH;
         }
      }

      vjDEBUG_CONT_END(vjDBG_ALL,dbg_lvl) << std::endl << vjDEBUG_FLUSH;

   }
};

};
#endif

