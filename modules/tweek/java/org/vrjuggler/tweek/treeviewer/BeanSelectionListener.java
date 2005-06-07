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
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.tweek.treeviewer;

import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JSplitPane;
import javax.swing.event.TreeSelectionEvent;
import javax.swing.event.TreeSelectionListener;
import javax.swing.tree.DefaultMutableTreeNode;
import org.vrjuggler.tweek.beans.PanelBean;


/**
 * Extension of javax.swing.event.TreeSelectionListener used with BeanTree
 * objects.  This class assumes that the BeanTree leaves contain instances of
 * PanelBean objects.  It knows how to instantiate JavaBeans and thus allows
 * delayed loading of Beans.  In addition, it requires that a JSplitPane is
 * used as the container for the BeanTree where the BeanTree is the left
 * member of the split pane.  The Beans are displayed in the right member.
 *
 * @see org.vrjuggler.tweek.treeviewer.BeanTree
 * @see org.vrjuggler.tweek.beans.PanelBean
 */
public class BeanSelectionListener implements TreeSelectionListener
{
   public BeanSelectionListener (JSplitPane pane, BeanTree tree,
                                 JPanel defaultPanel)
   {
      mPane         = pane;
      mTree         = tree;
      mDefaultPanel = defaultPanel;
   }

   /**
    * Implementation of the TreeSelectionListener.valueChanged() method.  This
    * is called when the user clicks on an element of the BeanTree object
    * associated with this object.  If the element is a branch, the default
    * panel is displayed.
    */
   public void valueChanged (TreeSelectionEvent e)
   {
      DefaultMutableTreeNode node =
         (DefaultMutableTreeNode) mTree.getLastSelectedPathComponent();

      if ( node != null )
      {
         Object node_info = node.getUserObject();

         // The selected node is a leaf, so the contained JavaBean will be
         // displayed.
         if ( node.isLeaf() )
         {
            PanelBean bp = (PanelBean) node_info;

            try
            {
               // Check if the selected Bean has been instantiated yet.  If
               // not, get an instance so that it may be displayed.
               if ( ! bp.isInstantiated() )
               {
                  bp.instantiate();
               }

               // Add the Bean to the right element of the split pane.
               mPane.add(bp.getComponent(), JSplitPane.RIGHT);
            }
            catch (org.vrjuggler.tweek.beans.loader.BeanInstantiationException inst_ex)
            {
               JOptionPane.showMessageDialog(null, inst_ex.getMessage(),
                                             "Instantiation Failure",
                                             JOptionPane.ERROR_MESSAGE);
            }
         }
         // The selected element is a branch, so display the default panel in
         // the right side of the split pane.
         else
         {
            mPane.add(mDefaultPanel, JSplitPane.RIGHT);
         }
      }
   }

   private JSplitPane mPane         = null;
   private BeanTree   mTree         = null;
   private JPanel     mDefaultPanel = null;
}
