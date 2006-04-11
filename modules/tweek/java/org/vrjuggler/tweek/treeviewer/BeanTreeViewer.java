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
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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

import java.awt.*;
import javax.swing.*;
import javax.swing.event.TreeSelectionEvent;
import javax.swing.event.TreeSelectionListener;
import javax.swing.tree.*;
import org.vrjuggler.tweek.beans.*;


/**
 * A Tweek Viewer Bean that implements a tree-based view of the loaded Panel
 * Beans.  This can be loaded dynamically by the Tweek GUI to extend the
 * available viewers.
 *
 * @version $Revision$
 *
 * @see org.vrjuggler.tweek.treeviewer.BeanTree
 */
public class BeanTreeViewer
   extends DefaultBeanModelViewer
   implements BeanRegistrationListener
{
   // ========================================================================
   // Public methods.
   // ========================================================================

   /**
    * Constructs the split pane.  This just fills in panels for the left and
    * right sides of this JSplitPane.  For the left pane, a blank JPanel is
    * For the right pane, the default (no-Bean-selected) JPanel is shown.
    * used.  This is done primarily to keep the GUI from looking nasty before
    * the full GUI initialization is done.
    */
   public BeanTreeViewer ()
   {
      JPanel empty_left = new JPanel();
      empty_left.setBackground(Color.white);
      empty_left.setMinimumSize(new Dimension(125, 200));
      viewer.add(empty_left, JSplitPane.LEFT);
      viewer.add(mDefaultBeanPanel, JSplitPane.RIGHT);
   }

   public void setModel (BeanTreeModel model)
   {
      if ( mBeanTree == null )
      {
         mBeanTree = new BeanTree(model);
         BeanRegistry.instance().addBeanRegistrationListener(this);
      }
      else
      {
         mBeanTree.setModel(model);
      }

      this.model = model;
   }

   public BeanTreeModel getModel()
   {
      return model;
   }

   /**
    * Component initialization.
    */
   public void initGUI ()
   {
      try
      {
         jbInit();
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }
   }

   /**
    * Programmatically focuses the given panel bean in this viewer.
    *
    * @param bean       the bean to focus
    */
   public void focusBean(PanelBean bean)
   {
      java.util.List paths = bean.getPaths();

      // Can't select it if it doesn't have a path
      if (paths.size() == 0)
      {
         return;
      }

      // Just use the first path found and add the bean onto the end of the path
      String path = (String)paths.get(0);
      if (! path.endsWith("/"))
      {
         path = path + "/";
      }
      path = path + bean.getName();

      // Get the TreePath to the bean's node
      TreeNode node = model.getNode(path);
      if (node == null)
      {
         // Can't select an invalid node
         System.err.println("Invalid node for path '" + path + "'");
         return;
      }
      // Select the bean's node
      mBeanTree.setSelectionPath(new TreePath(model.getPathToRoot(node)));
   }

   public JComponent getViewer ()
   {
      return viewer;
   }

   public void beanRegistered (BeanRegistrationEvent e)
   {
      if ( e.getBean() instanceof PanelBean )
      {
         refreshDataModel();
      }
   }

   /**
    * Extension of javax.swing.event.TreeSelectionListener used with BeanTree
    * objects.  This class assumes that the BeanTree leaves contain instances
    * of PanelBean objects.  It knows how to instantiate JavaBeans and thus
    * allows delayed loading of Beans.  In addition, it requires that a
    * JSplitPane is used as the container for the BeanTree where the BeanTree
    * is the left member of the split pane.  The Beans are displayed in the
    * right member.
    */
   protected class BeanSelectionListener implements TreeSelectionListener
   {
      /**
       * Implementation of the TreeSelectionListener.valueChanged() method.
       * This is called when the user clicks on an element of the BeanTree
       * object associated with this object.  If the element is a branch, the
       * default panel is displayed.
       */
      public void valueChanged (TreeSelectionEvent e)
      {
         DefaultMutableTreeNode node =
            (DefaultMutableTreeNode) mBeanTree.getLastSelectedPathComponent();

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
                  if ( m_last_panel != null )
                  {
                     fireBeanUnfocusEvent(m_last_panel);
                  }

                  fireBeanFocusEvent(bp);
                  m_last_panel = bp;
                  viewer.add(bp.getComponent(), JSplitPane.RIGHT);
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
               if ( m_last_panel != null )
               {
                  fireBeanUnfocusEvent(m_last_panel);
                  m_last_panel = null;
               }

               viewer.add(mDefaultBeanPanel, JSplitPane.RIGHT);
            }
         }
      }

      private PanelBean m_last_panel = null;
   }

   // ========================================================================
   // Private methods.
   // ========================================================================

   private void refreshDataModel ()
   {
      model.nodeStructureChanged((TreeNode) model.getRoot());
   }

   /**
    * Initializes the GUI sub-components and the attributes of this component.
    * This method was initially generated by JBuilder and is needed for
    * JBuilder to extract information about the configuration of this GUI
    * component.
    */
   private void jbInit() throws Exception
   {
      mDefaultBeanPanel.setBackground(Color.white);
      mDefaultBeanPanel.setLayout(mDefaultBeanPanelLayout);

      // Put together the BeanTree displaying the JavaBeans.
      mBeanTree.addTreeSelectionListener(new BeanSelectionListener());
      mBeanTree.setScrollsOnExpand(true);
      viewer.setDividerSize(8);
      viewer.setOneTouchExpandable(true);
      mBeanTreePane.getViewport().add(mBeanTree, null);

      mTitlePanelLabel.setFont(new java.awt.Font("Dialog", 1, 24));
      mTitlePanelLabel.setForeground(Color.white);
      mTitlePanelLabel.setText("Tweek!");
      mTitlePanel.setBackground(Color.black);
      mTitlePanel.setLayout(mTitlePanelLayout);
      mTitlePanel.add(mTitlePanelLabel, null);

      mDefaultBeanPanel.add(mTitlePanel, BorderLayout.NORTH);

      viewer.add(mDefaultBeanPanel, JSplitPane.RIGHT);
      viewer.add(mBeanTreePane, JSplitPane.LEFT);
   }

   // ========================================================================
   // Private data members.
   // ========================================================================

   private BeanTreeModel model = null;

   private JSplitPane  viewer = new JSplitPane();

   // The tree holding the bean hierarchy.
   private JScrollPane mBeanTreePane = new JScrollPane();
   private BeanTree    mBeanTree     = null;

   // Contents of the default panel shown in the right side of the split pane.
   private JPanel       mDefaultBeanPanel       = new JPanel();
   private BorderLayout mDefaultBeanPanelLayout = new BorderLayout();
   private JPanel       mTitlePanel             = new JPanel();
   private FlowLayout   mTitlePanelLayout       = new FlowLayout();
   private JLabel       mTitlePanelLabel        = new JLabel();
}
