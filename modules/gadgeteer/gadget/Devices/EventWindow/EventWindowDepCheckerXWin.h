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

#ifndef _GADGET_EVENT_WINDOW_DEP_CHECKER_XWIN_H_
#define _GADGET_EVENT_WINDOW_DEP_CHECKER_XWIN_H_

#include <gadget/gadgetConfig.h>

// Dependency checker includes
#include <gadget/InputManager.h>
#include <jccl/RTRC/DependencyManager.h>
#include <jccl/RTRC/DepChecker.h>

namespace gadget
{

/**
 * Dependency checker for X Window System event source.
 * Implement the basic stuff plus a check for wether the system knows about
 * the system display windows yet.
 */
class EventWindowDepCheckerXWin : public jccl::DepChecker
{
public:
   EventWindowDepCheckerXWin()
   {;}

   /**
    * Return a string name of the checker.
    * Used to output messages in checkerlistings.
    */
   virtual std::string getCheckerName()
   {
      return std::string("gadget::EventWindowXWin Checker");
   }

   /// We can handle only EventWindow configuration information.
   virtual bool canHandle(jccl::ConfigElementPtr element)
   {
      const std::string element_type = element->getID();
      // Return true if we have a EventWindowXWin element type.
      return (element_type == EventWindowXWin::getElementType());
   }

   /**
    * Are the dependencies satisfied?
    * Check wether the display system element is in the active config.
    *
    * @return true is returned if default dependencies are satisfied and the
    *         Display Manager has display system element.
    */
   virtual bool depSatisfied(jccl::ConfigElementPtr element)
   {
      bool pass = jccl::DepChecker::depSatisfied(element);   // Run default check

      // If we can pass normal check and we have a display system element.
      if ( haveDisplaySystemElement() )
      {
         return pass;
      }
      else
      {
         return false;
      }
   }

   bool haveDisplaySystemElement() const
   {
      return (gadget::InputManager::instance()->getDisplaySystemElement().get() != NULL);
   }

   /// Write out the dependencies to the vprDEBUG macro.
   virtual void debugOutDependencies(jccl::ConfigElementPtr element,
                                     int dbg_lvl=vprDBG_WARNING_LVL)
   {
      jccl::DepChecker::debugOutDependencies(element, dbg_lvl);

      vprDEBUG_NEXT_BEGIN(vprDBG_ALL,dbg_lvl)
         << "Extra Dependency: Dependent upon getting displaySystem element from Display Manager: "
         << vprDEBUG_FLUSH;

      if ( ! haveDisplaySystemElement() )
      {
         vprDEBUG_CONT(vprDBG_ALL,dbg_lvl) << "FAILED!!!\n" << vprDEBUG_FLUSH;
      }
      else
      {
         vprDEBUG_CONT(vprDBG_ALL,dbg_lvl) << "passed.\n" << vprDEBUG_FLUSH;
      }

      vprDEBUG_NEXT(vprDBG_ALL,dbg_lvl)
         << "Extra Dependencies for: item: " << element->getFullName()
         << " type: " << element->getID() << std::endl
         << "   Dependent upon displaySystem element in Display Manager. (Needs it to find display strings)"
         << vprDEBUG_FLUSH;

      vprDEBUG_NEXT_END(vprDBG_ALL,dbg_lvl) << std::endl << vprDEBUG_FLUSH;
   }
};

} // End of gadget namespace

#endif
