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

#ifndef _VRJ_XWIN_KB_DEP_CHECKER_H_
#define _VRJ_XWIN_KB_DEP_CHECKER_H_

#include <vrj/vjConfig.h>

// Dependency checker includes
#include <vrj/Kernel/DependencyManager.h>
#include <vrj/Display/DisplayManager.h>
#include <vrj/Kernel/DepChecker.h>

namespace vrj
{
   //: Dependency checker for XWin keyboard
// Implement the basic stuff plus a check for wether
// the system knows about the system display windows yet
class XWinKBDepChecker : public DepChecker
{
public:
   XWinKBDepChecker()
   {;}

   //: Return a string name of the checker
   // Used to output messages in checker listings
   virtual std::string getCheckerName()
   { return std::string("vjXWinKeyboard Checker"); }

   // We can handle only keyboard configuration information
   virtual bool canHandle(ConfigChunk* chunk)
   {
      std::string chunk_type = (std::string)chunk->getType();
      return (chunk_type == XWinKeyboard::getChunkType());      // Return true if we have a XWinKeyboard chunk type
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

   // Write out the dependencies to the vjDEBUG macro
   virtual void debugOutDependencies(ConfigChunk* chunk,int dbg_lvl=vjDBG_WARNING_LVL)
   {
      DepChecker::debugOutDependencies(chunk,dbg_lvl);

      vjDEBUG_NEXT_BEGIN(vjDBG_ALL,dbg_lvl) << "Extra Dependency: Dependent upon getting DisplaySystemChunk from displayManager: " << vjDEBUG_FLUSH;
      if(depSatisfied(chunk) == false)
         vjDEBUG_CONT(vjDBG_ALL,dbg_lvl) << "FAILED!!!\n" << vjDEBUG_FLUSH;
      else
         vjDEBUG_CONT(vjDBG_ALL,dbg_lvl) << "passed.\n" << vjDEBUG_FLUSH;


      vjDEBUG_NEXT(vjDBG_ALL,dbg_lvl) << "Extra Dependencies for: item: "
                                      << chunk->getProperty("name")
                                      << " type: "
                                      << ((std::string)chunk->getType()).c_str()
                                      << std::endl
                                      << "   Dependant upon displaySystemChunk in display Manager. (Needs it to find display strings)"
                                      << vjDEBUG_FLUSH;

      vjDEBUG_NEXT_END(vjDBG_ALL,dbg_lvl) << std::endl << vjDEBUG_FLUSH;
   }
};

};

#endif
