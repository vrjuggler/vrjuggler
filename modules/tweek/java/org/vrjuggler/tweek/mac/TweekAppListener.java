/***************** <Tweek heading BEGIN do not edit this line> ****************
 * Tweek
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 ***************** <Tweek heading END do not edit this line> *****************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

package org.vrjuggler.tweek.mac;

import com.apple.eawt.*;
import org.vrjuggler.tweek.services.GlobalPreferencesServiceProxy;
import org.vrjuggler.tweek.gui.TweekFrame;


public class TweekAppListener extends ApplicationAdapter
{
   public TweekAppListener(TweekFrame f)
   {
      mFrame = f;
      Application app = Application.getApplication();
      app.addApplicationListener(this);

      // Only enable the "Preferences" menu if the GlobalPreferences Service
      // Bean is available.
      try
      {
         new GlobalPreferencesServiceProxy();
         app.setEnabledPreferencesMenu(true);
      }
      catch(Exception ex)
      {
         app.setEnabledPreferencesMenu(false);
      }
   }

   public void handleAbout(ApplicationEvent e)
   {
      mFrame.handleAbout();
      e.setHandled(true);
   }

   public void handlePreferences(ApplicationEvent e)
   {
      mFrame.handlePrefs();
      e.setHandled(true);
   }

   public void handleOpenFile(ApplicationEvent e)
   {
      mFrame.handleOpenFile();
      e.setHandled(true);
   }

   public void handleQuit(ApplicationEvent e)
   {
      e.setHandled(mFrame.handleQuit());
   }

   private TweekFrame mFrame = null;
}

