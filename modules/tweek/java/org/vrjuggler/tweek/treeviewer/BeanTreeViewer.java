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
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
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
 *
 * @version $Revision$
 *
 * @see org.vrjuggler.tweek.treeviewer.BeanTree
 */
public class BeanTreeViewer extends DefaultBeanModelViewer
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
      viewer.add(m_default_bean_panel, JSplitPane.RIGHT);
      viewer.setDividerSize(5);
   }

   public void initDataModel (BeanTreeModel model)
   {
      m_bean_tree = new BeanTree(model);
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

   public void refreshDataModel (BeanTreeModel model)
   {
      /* Nothing to do. */ ;
   }

   public JComponent getViewer ()
   {
      return viewer;
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
            (DefaultMutableTreeNode) m_bean_tree.getLastSelectedPathComponent();

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

               viewer.add(m_default_bean_panel, JSplitPane.RIGHT);
            }
         }
      }

      private PanelBean m_last_panel = null;
   }

   // ========================================================================
   // Private methods.
   // ========================================================================

   /**
    * Initializes the GUI sub-components and the attributes of this component.
    * This method was initially generated by JBuilder and is needed for
    * JBuilder to extract information about the configuration of this GUI
    * component.
    */
   private void jbInit() throws Exception
   {
      m_default_bean_panel.setBackground(Color.white);
      m_default_bean_panel.setLayout(m_default_bean_panel_layout);

      // Put together the BeanTree displaying the JavaBeans.
      m_bean_tree.addTreeSelectionListener(new BeanSelectionListener());
      m_bean_tree.setScrollsOnExpand(true);
      m_bean_tree_pane.getViewport().add(m_bean_tree, null);

      m_title_panel_label.setFont(new java.awt.Font("Dialog", 1, 24));
      m_title_panel_label.setForeground(Color.white);
      m_title_panel_label.setText("Tweek!");
      m_title_panel.setBackground(Color.black);
      m_title_panel.setLayout(m_title_panel_layout);
      m_title_panel.add(m_title_panel_label, null);

      m_default_bean_panel.add(m_title_panel, BorderLayout.NORTH);

      viewer.add(m_default_bean_panel, JSplitPane.RIGHT);
      viewer.add(m_bean_tree_pane, JSplitPane.LEFT);
   }

   // ========================================================================
   // Private data members.
   // ========================================================================

   private JSplitPane  viewer = new JSplitPane();

   // The tree holding the bean hierarchy.
   private JScrollPane m_bean_tree_pane = new JScrollPane();
   private BeanTree    m_bean_tree      = null;

   // Contents of the default panel shown in the right side of the split pane.
   private JPanel       m_default_bean_panel        = new JPanel();
   private BorderLayout m_default_bean_panel_layout = new BorderLayout();
   private JPanel       m_title_panel               = new JPanel();
   private FlowLayout   m_title_panel_layout        = new FlowLayout();
   private JLabel       m_title_panel_label         = new JLabel();
}
