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
package org.vrjuggler.vrjconfig.controlpanel;

import javax.swing.Icon;
import javax.swing.tree.DefaultMutableTreeNode;

/**
 * Contains information about a ControlPanel type which creates a 
 * ControlPanel listing all elements with a given token.
 */
public class TypeNode extends ControlPanelNode
{
   /**
    * Constructs a ControlPanel with a list of ConfigElements with a given token.
    *
    * @param label      label shown under icon
    * @param title      title printed at top of control panel
    * @param token      token of all ConfigElements to list
    * @param icon       icon displayed on button
    *
    * @see ControlPanel
    */
   public TypeNode(String label, String title, String token, Icon icon)
   {
      super(label, icon);
      setAllowsChildren(false);
      mTitle = title;
      mToken = token;
   }

   public String getTitle()
   {
      return mTitle;
   }
   
   public String getToken()
   {
      return mToken;
   }

   public String toString()
   {
      return("TypeNode[mLabel=" + mLabel + ", mTitle=" + mTitle + ", mToken=" + mToken + ", mIcon=" + mIcon +"]");
   }

   /** Title to be displayed on the top of the control panel. */
   private String mTitle = null;

   /** Token used to search for ConfigElements of a given type. */
   private String mToken = null;
}
