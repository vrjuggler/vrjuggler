/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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
package org.vrjuggler.vrjconfig.wizards.vrsystem;

import java.util.*;
import javax.swing.*;

import org.vrjuggler.tweek.wizard.*;
import org.vrjuggler.jccl.config.ConfigBroker;
import org.vrjuggler.jccl.config.ConfigBrokerProxy;
import org.vrjuggler.jccl.config.ConfigDefinition;

/**
 * A pane used to welcome the user to a wizard. This is generally the first
 * pane you wish to show in a wizard.
 */
public class WallStep
   extends AbstractWizardStep
   implements WizardPane
{
   /**
   * Creates a new CurrentDevicesStep with no text.
   */
   public WallStep()
   {
   }

   public void onEntering()
   {
      super.onEntering();
      
      if (mWallPanel == null)
      {
         mWallPanel = new WallPanel();
         mWallPanel.init(mWhiteBoard);
      }
   }

   public void onExited()
   {
      super.onExited();
   }
   /**
   * Gets the GUI component for this pane.
   */
   public JComponent getGUI()
   {
      return mWallPanel;
   }

   public List getRequiredClasses()
   {
      List classes = new ArrayList();
      return classes;
   }

   public List getRequiredResources()
   {
      List res = new ArrayList();
      return res;
   }

   /**
    * The JPanel that makes up this pane's UI.
    */
   private transient WallPanel mWallPanel = null;
}
