/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

#include <vrj/Draw/Pf/PfInputHandler.h>

namespace vrj
{

PfInputHandler::PfInputHandler(pfPipeWindow* pWin, const std::string& displayName)
{
#ifdef VPR_OS_Win32
   mWinHandle = pWin->getWSWindow();
#else
   // Get the XWindow from that we are going to recieve events from.
   mXWindow = pWin->getWSWindow();

   // Create a new display connection to the X server.
   mXDisplay = XOpenDisplay( displayName.c_str());
#endif
}

#ifdef VPR_OS_Win32
void PfInputHandler::handlePerformerEvent(MSG message)
{
   // If we have a valid KeyboardMouseDevice, process
   // all keyboard/mouse events
   if ( NULL != mKeyboardMouseDevice )
   {
      // Forward events on to subclass. The magic of inheritance :)
      InputAreaWin32::updKeys( message );
   }
}
#else
void PfInputHandler::handlePerformerEvent(::XEvent& event)
{
   // If we have a valid KeyboardMouseDevice, process
   // all keyboard/mouse events
   if ( NULL != mKeyboardMouseDevice )
   {
      // Forward events on to subclass. The magic of inheritance :)
      InputAreaXWin::handleEvent( event );
   }
}
#endif

} // End of vrj namespace
