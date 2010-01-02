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

package org.vrjuggler.tweek.treeviewer;

import javax.swing.tree.DefaultTreeModel;
import javax.swing.tree.TreeSelectionModel;
import org.vrjuggler.tweek.beans.BeanInstantiationEvent;
import org.vrjuggler.tweek.beans.BeanInstantiationListener;


/**
 * Simple extension of javax.swing.JTree that holds a collection of objects
 * listening for BeanInstantiationEvents.  Instances of this class always
 * use javax.swing.tree.DefaultTreeModel as their internal tree model.
 *
 * @see javax.swing.tree.DefaultTreeModel
 */
public class BeanTree extends javax.swing.JTree
{
/*
   // XXX: The value of the default constructor for this class is
   // debatable.  JBuilder may find it useful in its component designers, but
   // I personally feel that it breaks the ability to assume that the tree
   // model is always an instance of DefaultTreeModel.
   public BeanTree ()
   {
      super(new DefaultTreeModel(null));
   }
*/

   public BeanTree (javax.swing.tree.TreeModel model)
   {
      super(model);
      jbInit();
   }

   /**
    * Initializes this Swing component.  This method is so named to aid in the
    * use of this class with JBuilder.
    */
   private void jbInit ()
   {
      this.setRootVisible(false);
      this.setShowsRootHandles(true);
      this.putClientProperty("JTree.lineStyle", "Angled");
      this.getSelectionModel().setSelectionMode(TreeSelectionModel.SINGLE_TREE_SELECTION);
   }
}
