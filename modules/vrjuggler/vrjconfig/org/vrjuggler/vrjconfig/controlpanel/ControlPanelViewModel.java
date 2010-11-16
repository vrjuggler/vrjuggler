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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.vrjconfig.controlpanel;

import java.io.File;
import java.util.Enumeration;
import javax.swing.tree.DefaultMutableTreeNode;
import org.vrjuggler.vrjconfig.controlpanel.io.*;

/**
 * Models the structure of a ControlPanelView which includes all panels
 * that are accessible from the ControlPanel.
 */
public class ControlPanelViewModel
{
   /** Root of tree structure. */
   private DefaultMutableTreeNode mRoot = null;
  
   /**
    * Load models structure from a given XML file.
    */
   public ControlPanelViewModel(String filename)
   {
      try
      {
         ControlPanelReader reader = new ControlPanelReader(new File(filename));
         mRoot = reader.readControlPanel();
      }
      catch(org.vrjuggler.vrjconfig.controlpanel.io.ParseException ex)
      {
         System.out.println(ex);
         ex.printStackTrace();
      }
      catch(java.io.IOException ex)
      {
         System.out.println(ex);
         ex.printStackTrace();
      }
   }

   /**
    * Return the root of this model.
    */
   public DefaultMutableTreeNode getRoot()
   {
      return mRoot;
   }
}
