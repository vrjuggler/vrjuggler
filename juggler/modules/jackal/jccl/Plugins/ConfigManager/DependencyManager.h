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


#ifndef _VJ_DEP_MGR_H_
#define _VJ_DEP_MGR_H_
//#pragma once

#include <vjConfig.h>
#include <Kernel/vjDepChecker.h>
#include <Config/vjConfigChunk.h>
#include <typeinfo>

//: Object used for creating devices
//!NOTE: Singleton
class vjDependencyManager
{
private:
   // Singleton so must be private
   vjDependencyManager()
   {
      mDepCheckers = std::vector<vjDepChecker*>(0);
      vjASSERT(mDepCheckers.size() == 0);
   }

public:
   void registerChecker(vjDepChecker* checker)
   {
      vjASSERT(checker != NULL);
      mDepCheckers.push_back(checker);     // Add the constructor to the list
      vjDEBUG(vjDBG_KERNEL,1) << "vjDependencyManager::registerChecker: Checker registered: "
              << checker->getCheckerName().c_str()
              << "   :" << (void*)checker
              << " type:" << typeid(*checker).name() << endl << vjDEBUG_FLUSH;
   }

   //: Are the dependencies satisfied for the given chunk?
   //
   //!RETURNS: true - dependencies are currently satisifed for the chunk
   bool depSatisfied(vjConfigChunk* chunk)
   {
      vjASSERT(NULL != chunk);
      vjDepChecker* checker = findDepChecker(chunk);
      return checker->depSatisfied(chunk);
   }

   void debugOutDependencies(vjConfigChunk* chunk,int dbg_lvl)
   {
      vjASSERT(NULL != chunk);
      vjDepChecker* checker = findDepChecker(chunk);
      checker->debugOutDependencies(chunk,dbg_lvl);
   }

private:
   //: Returns a dependency checker for the given chunk
   //! RETURNS: If checker found, it is returned
   //+          Otherwise, it returns the default checker
   vjDepChecker* findDepChecker(vjConfigChunk* chunk);

   void debugDump();


private:
   std::vector<vjDepChecker*> mDepCheckers;     //: List of the device constructors
   vjDepChecker               mDefaultChecker;  //: The default checker

public:     // ------ SINGLETON ----- ///
   //: Return singleton instance of the class
   static vjDependencyManager* instance()
   {
      if(mInstance == NULL)
      {
         mInstance = new vjDependencyManager();
      }

      return mInstance;
   }

private:
   static vjDependencyManager* mInstance;
};

#endif

