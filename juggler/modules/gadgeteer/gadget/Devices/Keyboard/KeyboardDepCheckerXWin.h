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

#ifndef _VRJ_KEYBOARD_DEP_CHECKER_XWIN_H_
#define _VRJ_KEYBOARD_DEP_CHECKER_XWIN_H_

#include <vrj/vrjConfig.h>

// Dependency checker includes
#include <vrj/Kernel/DependencyManager.h>
#include <vrj/Display/DisplayManager.h>
#include <vrj/Kernel/DepChecker.h>

namespace vrj
{
   //: Dependency checker for XWin keyboard
// Implement the basic stuff plus a check for wether
// the system knows about the system display windows yet
class KeyboardDepCheckerXWin : public DepChecker
{
public:
   KeyboardDepCheckerXWin()
   {;}

   //: Return a string name of the checker
   // Used to output messages in checker listings
   virtual std::string getCheckerName()
   { return std::string("vrj::KeyboardXWin Checker"); }

   // We can handle only keyboard configuration information
   virtual bool canHandle(ConfigChunk* chunk)
   {
      std::string chunk_type = (std::string)chunk->getType();
      return (chunk_type == KeyboardXWin::getChunkType());      // Return true if we have a KeyboardXWin chunk type
   }

   //: Are the dependencies satisfied?
   //! RETURNS: true - default dependencies are satisfied && display manager has display system chunk
   // Check wether the display system chunk is in the active config
   virtual bool depSatisfied(ConfigChunk* chunk)
   {
      bool pass = DepChecker::depSatisfied(chunk);   // Run default check

      // If we can pass normal check and we have a display system chunk
      if(DisplayManager::instance()->getDisplaySystemChunk() != NULL)
         return pass;
      else
         return false;
   }

   // Write out the dependencies to the vprDEBUG macro
   virtual void debugOutDependencies(ConfigChunk* chunk,int dbg_lvl=vprDBG_WARNING_LVL)
   {
      DepChecker::debugOutDependencies(chunk,dbg_lvl);

      vprDEBUG_NEXT_BEGIN(vprDBG_ALL,dbg_lvl) << "Extra Dependency: Dependent upon getting DisplaySystemChunk from displayManager: " << vprDEBUG_FLUSH;
      if(depSatisfied(chunk) == false)
         vprDEBUG_CONT(vprDBG_ALL,dbg_lvl) << "FAILED!!!\n" << vprDEBUG_FLUSH;
      else
         vprDEBUG_CONT(vprDBG_ALL,dbg_lvl) << "passed.\n" << vprDEBUG_FLUSH;


      vprDEBUG_NEXT(vprDBG_ALL,dbg_lvl) << "Extra Dependencies for: item: "
                                      << chunk->getProperty("name")
                                      << " type: "
                                      << ((std::string)chunk->getType()).c_str()
                                      << std::endl
                                      << "   Dependant upon displaySystemChunk in display Manager. (Needs it to find display strings)"
                                      << vprDEBUG_FLUSH;

      vprDEBUG_NEXT_END(vprDBG_ALL,dbg_lvl) << std::endl << vprDEBUG_FLUSH;
   }
};

};

#endif
