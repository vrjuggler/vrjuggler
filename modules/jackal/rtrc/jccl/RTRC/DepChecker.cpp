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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/


#include <jccl/jcclConfig.h>
#include <jccl/Config/ConfigChunk.h>
#include <jccl/Config/ChunkDesc.h>
#include <jccl/RTRC/ConfigManager.h>
#include <jccl/RTRC/DepChecker.h>
#include <boost/concept_check.hpp>

namespace jccl
{

DepChecker::DepChecker ()
{;}


/*virtual*/ std::string DepChecker::getCheckerName ()
{
   return std::string("Default Checker");
}


/*virtual*/ bool DepChecker::canHandle (ConfigChunkPtr chunk)
{
   boost::ignore_unused_variable_warning(chunk);
   return true;
}


/*virtual*/ bool DepChecker::depSatisfied (ConfigChunkPtr chunk)
{
   bool pass=true;

   ConfigManager* cfg_mgr = ConfigManager::instance();

   // Get the list of dependencies
   std::vector<std::string> dependencies =
      chunk->getChunkPtrDependencies();

   // Check to see if they are loaded already
   for(unsigned int i=0;i<dependencies.size();i++)
   {
      if(!cfg_mgr->isChunkInActiveList(dependencies[i]))
         pass = false;
   }
   return pass;
}


// Write out the dependencies to the vprDEBUG macro
/*virtual*/ void DepChecker::debugOutDependencies (ConfigChunkPtr chunk,
                                                   int dbg_lvl)
{
   vprDEBUG_NEXT_BEGIN(vprDBG_ALL,dbg_lvl)
      << "---- Dependencies for: item: " << chunk->getName()
      << " type: " << chunk->getDescToken()
      << "-------\n" << vprDEBUG_FLUSH;

   ConfigManager* cfg_mgr = ConfigManager::instance();

   // Get the list of dependencies
   std::vector<std::string> dependencies = chunk->getChunkPtrDependencies();

   // Check to see if they are loaded already
   for(unsigned int i=0;i<dependencies.size();i++)
   {
      vprDEBUG_NEXT(vprDBG_ALL,dbg_lvl) << i << ": "
                                        << dependencies[i].c_str()
                                        << " ==> " << vprDEBUG_FLUSH;
      if(!cfg_mgr->isChunkInActiveList(dependencies[i]))
      {
         vprDEBUG_CONT(vprDBG_ALL,dbg_lvl) << "not available.\n" << vprDEBUG_FLUSH;
      }
      else
      {
         vprDEBUG_CONT(vprDBG_ALL,dbg_lvl) << "passed.\n" << vprDEBUG_FLUSH;
      }
   }

   vprDEBUG_CONT_END(vprDBG_ALL,dbg_lvl) << std::endl << vprDEBUG_FLUSH;

}


} //namespace jccl
