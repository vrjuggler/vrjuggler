/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2002 by Iowa State University
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

#include <gadget/gadgetConfig.h>

#include <jccl/Util/Debug.h>
#include <gadget/RemoteInputManager/RemoteInputManager.h>
#include <gadget/Util/Debug.h>
#include <gadget/InputManager.h>
#include <gadget/RemoteInputManager/RmtMgrDepChecker.h>
#include <jccl/RTRC/ConfigManager.h>


namespace gadget
{

bool RmtMgrDepChecker::depSatisfied(jccl::ConfigChunkPtr chunk)
{
   
   static int last_pending = 0;
      // If we have a RIMChunk then we have to make sure that all machine chunks are loaded
   //if (chunk->getDescToken() == RemoteInputManager::getChunkType())
   //{
      // Return TRUE if standard dependencies are satisfied and there 
      // are NOT any pending machine specific chunks
   jccl::ConfigManager* cfg_mgr = jccl::ConfigManager::instance();
   
   if (cfg_mgr->getNumPending() == last_pending)
   {
      return true;
   }
   else
   {
      last_pending = cfg_mgr->getNumPending();
      return false;
   }
   //return(jccl::DepChecker::depSatisfied(chunk) &&
   //       !cfg_mgr->isChunkTypeInPendingList(RemoteInputManager::getMachineSpecificChunkType()));
   //}
   /*
   else  // We must have a device
   {
      return(jccl::DepChecker::depSatisfied(chunk) &&
            !jccl::ConfigManager::instance()->isChunkTypeInPendingList(RemoteInputManager::getChunkType()));
   }
   */
}


// We can handle any device and the RIMChunk
bool RmtMgrDepChecker::canHandle(jccl::ConfigChunkPtr chunk)
{
   // return (DeviceFactory::instance()->recognizeDevice(chunk) ||
   return (chunk->getDescToken() == RemoteInputManager::getChunkType());
   
}

void RmtMgrDepChecker::debugOutDependencies(jccl::ConfigChunkPtr chunk,int dbg_lvl)
{
   jccl::DepChecker::debugOutDependencies(chunk,dbg_lvl);

   vprDEBUG_NEXT_BEGIN(vprDBG_ALL,dbg_lvl) << "Extra Dependency[RIM]: Dependent "
      << "upon the Pending list becoming stable: " << vprDEBUG_FLUSH;
   if (jccl::DepChecker::depSatisfied(chunk) &&
       !jccl::ConfigManager::instance()->isChunkTypeInPendingList(RemoteInputManager::getMachineSpecificChunkType()))
   {
      vprDEBUG_CONT(vprDBG_ALL,dbg_lvl) << "passed.\n" << vprDEBUG_FLUSH;      
   }
   else
   {
      vprDEBUG_CONT(vprDBG_ALL,dbg_lvl) << "FAILED!!!\n" << vprDEBUG_FLUSH;
   }

   vprDEBUG_NEXT(vprDBG_ALL,dbg_lvl)
      << "Extra Dependencies for: " << chunk->getFullName()
      << " type: " << chunk->getDescToken() << std::endl
      << vprDEBUG_FLUSH;

   vprDEBUG_NEXT_END(vprDBG_ALL,dbg_lvl) << std::endl << vprDEBUG_FLUSH;
}

}  // end namespace gadget
