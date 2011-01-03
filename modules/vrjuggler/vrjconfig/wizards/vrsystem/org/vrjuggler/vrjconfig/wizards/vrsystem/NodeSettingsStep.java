/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.vrjconfig.wizards.vrsystem;

import java.util.*;
import javax.swing.*;

import org.vrjuggler.tweek.wizard.*;

/**
 * A pane used to welcome the user to a wizard. This is generally the first
 * pane you wish to show in a wizard.
 */
public class NodeSettingsStep
   extends AbstractWizardStep
   implements WizardPane
{
   /**
    * Creates a new NodeSettingsStep with no text.
    */
   public NodeSettingsStep()
   {
      cwp = null;
   }

   /**
    * Gets the GUI component for this pane.
    */
   public JComponent getGUI()
   {
      if (cwp == null)
      {
         cwp = new NodeSettingsPanel();
      }
      cwp.init(mWhiteBoard);
      return cwp;
   }

   public boolean onExiting()
   {
     //return(cwp.saveFile());
     return true;
   }

   public List getRequiredClasses()
   {
      List classes = new ArrayList();
      classes.add(NodeSettingsPanel.class.getName());
      classes.add(NodeSettingsPanel.class.getName()+"$1");
      classes.add(NodeSettingsPanel.class.getName()+"$2");
      classes.add(NodeSettingsPanel.class.getName()+"$3");
      return classes;
   }

   public List getRequiredResources()
   {
      List res = new ArrayList();
      res.add("org/vrjuggler/vrjconfig/wizards/cluster/images/ClusterIcon.png");
      return res;
   }


   /**
    * The JPanel that makes up this pane's UI.
    */
   private transient NodeSettingsPanel cwp;
}
