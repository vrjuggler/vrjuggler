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

package org.vrjuggler.vrjconfig.controlpanel;

import javax.swing.Icon;
import javax.swing.tree.DefaultMutableTreeNode;

/**
 * Abstract base class of all control panel nodes which contains 
 * data that is present in all nodes.
 */
public abstract class ControlPanelNode extends DefaultMutableTreeNode
{
   /**
    * Construct a ControlPanel node.
    *
    * @param label      text to be shown below icon
    * @param icon       Icon to be displayed on button
    */
   public ControlPanelNode(String label, Icon icon)
   {
      mLabel = label;
      mIcon = icon;
   }
      
   public String getLabel()
   {
      return mLabel;
   }
   
   public Icon getIcon()
   {
      return mIcon;
   }
   
   /** Label to be shown below icon. */
   protected String mLabel = null;

   /** Icon to be displayed on button. */
   protected Icon mIcon = null;
}
