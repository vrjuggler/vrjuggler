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

#include <gadget/gadgetConfig.h>
#include <vpr/Util/Debug.h>
#include <gadget/Devices/EventWindow/EventWindowDepCheckerXWin.h>


namespace gadget
{

bool EventWindowDepCheckerXWin::depSatisfied(jccl::ConfigElementPtr element)
{
   if(!jccl::DepChecker::depSatisfied(element))    // Run default check
   { return false; }

   // If we can pass normal check and we have a display system element.
   if (!haveDisplaySystemElement() )
   {
      return false;
   }

   if(!haveRemoteWindowNeeded(element))
   { return false; }

   return true;
}

bool EventWindowDepCheckerXWin::haveRemoteWindowNeeded(jccl::ConfigElementPtr element)
{
   // Check for registered window (if needed)
   std::string remote_window_name = element->getProperty<std::string>("event_source_name");

   if(remote_window_name.empty())
   { return true; }

   EventWindowXWin::WindowRegistry::WindowInfo remote_win_info;
   bool found_window = EventWindowXWin::WindowRegistry::instance()->getWindow(remote_window_name, remote_win_info);

   return found_window;
}

void EventWindowDepCheckerXWin::debugOutDependencies(jccl::ConfigElementPtr element,
                                                     int dbg_lvl)
{
   jccl::DepChecker::debugOutDependencies(element, dbg_lvl);

   vprDEBUG_NEXT_BEGIN(vprDBG_ALL,dbg_lvl)
      << "Extra Dependency for [" << element->getFullName() << "]: Dependent upon getting displaySystem element from Display Manager: "
      << vprDEBUG_FLUSH;

   if ( ! haveDisplaySystemElement() )
   {
      vprDEBUG_CONT(vprDBG_ALL,dbg_lvl) << "FAILED!!!\n" << vprDEBUG_FLUSH;
      vprDEBUG_NEXT(vprDBG_ALL,dbg_lvl) << "This is neede to get display strings.\n" << vprDEBUG_FLUSH;
   }
   else
   {
      vprDEBUG_CONT(vprDBG_ALL,dbg_lvl) << "passed.\n" << vprDEBUG_FLUSH;
   }



   vprDEBUG_NEXT_END(vprDBG_ALL,dbg_lvl) << std::endl << vprDEBUG_FLUSH;
}

} // End of gadget namespace
