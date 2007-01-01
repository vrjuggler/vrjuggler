/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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

#include <jccl/jcclConfig.h>

#include <iomanip>
#include <algorithm>
#include <typeinfo>

#include <jccl/Util/Debug.h>
#include <jccl/Config/ConfigElement.h>
#include <jccl/RTRC/DependencyManager.h>


namespace jccl
{

// Initialize the singleton ptr
vprSingletonImpLifetime(DependencyManager, 11);


DependencyManager::DependencyManager(): mDepCheckers(), mDefaultChecker()
{
   mDepCheckers = std::vector<DepChecker*>(0);
   //debugDump();
}

DependencyManager::~DependencyManager()
{
   mDepCheckers.clear();
}

void DependencyManager::registerChecker(DepChecker* checker)
{
   vprASSERT(checker != NULL);
   mDepCheckers.push_back(checker);       // Add the checker to the list
   vprDEBUG(jcclDBG_RECONFIG, vprDBG_CONFIG_LVL)
      << "DependencyManager: Registered: "
      << std::setiosflags(std::ios::right) << std::setw(25)
      << std::setfill(' ') << checker->getCheckerName().c_str()
      << std::resetiosflags(std::ios::right)
      << "  type: " << typeid(*checker).name() << std::endl
      << vprDEBUG_FLUSH;
   debugDump();
}

void DependencyManager::unregisterChecker(DepChecker* checker)
{
   vprASSERT(checker != NULL);
   std::vector<DepChecker*>::iterator c = std::find(mDepCheckers.begin(),
                                                    mDepCheckers.end(),
                                                    checker);
   if ( c != mDepCheckers.end() )
   {
      mDepCheckers.erase(c);
      vprDEBUG(jcclDBG_RECONFIG, vprDBG_CONFIG_LVL)
         << "DependencyManager: Unregistered: "
         << std::setiosflags(std::ios::right) << std::setw(25)
         << std::setfill(' ') << checker->getCheckerName().c_str()
         << std::resetiosflags(std::ios::right)
         << "  type: " << typeid(*checker).name() << std::endl
         << vprDEBUG_FLUSH;
      debugDump();
   }
}

bool DependencyManager::isSatisfied(ConfigElementPtr element)
{
   vprASSERT(NULL != element.get());
   DepChecker* checker = findDepChecker(element);
   return checker->depSatisfied(element);
}


void DependencyManager::debugOutDependencies(ConfigElementPtr element,
                                             int dbg_lvl)
{
   vprASSERT(NULL != element.get());
   DepChecker* checker = findDepChecker(element);
   checker->debugOutDependencies(element,dbg_lvl);
}


DepChecker* DependencyManager::findDepChecker(ConfigElementPtr element)
{
   vprASSERT(NULL != element.get());

   //std::string element_type;
   //element_type = (std::string)element->getType();

   for(unsigned int i=0;i<mDepCheckers.size();i++)
   {
      // Get next constructor
      DepChecker* checker = mDepCheckers[i];
      vprASSERT(checker != NULL);

      if(checker->canHandle(element))
      {
         return checker;
      }
   }

   return &mDefaultChecker;
}


void DependencyManager::debugDump()
{
   //vprDEBUG_BEGIN(jcclDBG_RECONFIG, jcclDBG_CONFIG_LVL) << "DependencyManager::debugDump\n" << vprDEBUG_FLUSH;
   vprDEBUG_BEGIN(jcclDBG_RECONFIG, vprDBG_STATE_LVL)
      << "----- Current dependency checkers -----\n" << vprDEBUG_FLUSH;
   vprDEBUG_NEXT(jcclDBG_RECONFIG, vprDBG_STATE_LVL)
      << "num checkers:" << mDepCheckers.size() << "\n"
      << vprDEBUG_FLUSH;
   vprDEBUG_NEXT(jcclDBG_RECONFIG, vprDBG_STATE_LVL)
      << "-1: Checker: default   type: default checker  recog: all (this is a fallback)\n"
      << vprDEBUG_FLUSH;

   for(unsigned int cNum=0;cNum<mDepCheckers.size();cNum++)
   {
      DepChecker* checker = mDepCheckers[cNum];
      vprDEBUG_NEXT(jcclDBG_RECONFIG, vprDBG_STATE_LVL)
         << cNum << ": Checker:" << (void*)checker
         << "   type:" << typeid(*checker).name()
         << "   recog:" << checker->getCheckerName().c_str()
         << "\n" << vprDEBUG_FLUSH;
   }

   vprDEBUG_END(jcclDBG_RECONFIG, vprDBG_STATE_LVL)
      << "---------------------\n" << vprDEBUG_FLUSH;
}


} // namespace jccl
