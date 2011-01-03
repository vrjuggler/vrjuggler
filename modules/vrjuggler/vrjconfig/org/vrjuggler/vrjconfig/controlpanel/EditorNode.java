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
 * Contains information about a ControlPanel editor.
 */
public class EditorNode extends ControlPanelNode
{
   /**
    * Constructs a ControlPanel editor.
    *
    * @param label      label shown under icon
    * @param class_name class of the editor to be loaded
    * @param icon       icon displayed on button
    *
    * @see ControlPanel
    */
   public EditorNode(String label, String class_name, Icon icon)
   {
      super(label, icon);
      setAllowsChildren(false);
      mClassName = class_name;
   }

   public String getClassName()
   {
      return mClassName;
   }
   
   public String toString()
   {
      return("EditorNode[mLabel=" + mLabel + ", mClassName=" + mClassName + ", mIcon=" + mIcon +"]");
   }

   /** Custom editor class. */
   private String mClassName = null;
}
