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


#ifndef _JCCL_DEP_MGR_H_
#define _JCCL_DEP_MGR_H_

#include <jccl/jcclConfig.h>
#include <jccl/ConfigManager/DepChecker.h>
#include <jccl/Config/ConfigChunk.h>
#include <typeinfo>
#include <vpr/Util/Singleton.h>

namespace jccl {

//: Object used for creating devices
//!NOTE: Singleton
class DependencyManager
{
private:
   // Singleton so must be private
   DependencyManager() : mDepCheckers(), mDefaultChecker()
   {
      mDepCheckers = std::vector<DepChecker*>(0);
      vprASSERT(mDepCheckers.size() == 0);
      //debugDump();
   }

public:
   // Called to register a new checker with the system
   void registerChecker(DepChecker* checker)
   {
      vprASSERT(checker != NULL);
      mDepCheckers.push_back(checker);          // Add the checker to the list
      vprDEBUG(vprDBG_KERNEL,vprDBG_CONFIG_LVL)
              << "DependencyManager: Registered: "
              << std::setiosflags(std::ios::right) << std::setw(25) << std::setfill(' ') << checker->getCheckerName().c_str() << std::resetiosflags(std::ios::right)
              << "  type: " << typeid(*checker).name() << std::endl
              << vprDEBUG_FLUSH;
      debugDump();
   }



   //: Are the dependencies satisfied for the given chunk?
   //
   //!RETURNS: true - dependencies are currently satisifed for the chunk
   bool depSatisfied(ConfigChunk* chunk)
   {
      vprASSERT(NULL != chunk);
      DepChecker* checker = findDepChecker(chunk);
      return checker->depSatisfied(chunk);
   }

   void debugOutDependencies(ConfigChunk* chunk,int dbg_lvl)
   {
      vprASSERT(NULL != chunk);
      DepChecker* checker = findDepChecker(chunk);
      checker->debugOutDependencies(chunk,dbg_lvl);
   }

private:
   //: Returns a dependency checker for the given chunk
   //! RETURNS: If checker found, it is returned
   //+          Otherwise, it returns the default checker
   DepChecker* findDepChecker(ConfigChunk* chunk);

   void debugDump();

private:
   std::vector<DepChecker*> mDepCheckers;     //: List of the device constructors
   DepChecker               mDefaultChecker;  //: The default checker

    vprSingletonHeader(DependencyManager);

};

}; // namespace jccl

#endif

