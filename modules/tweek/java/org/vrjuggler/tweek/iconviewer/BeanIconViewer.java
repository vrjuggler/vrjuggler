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

package org.vrjuggler.tweek.iconviewer;

import java.awt.*;
import java.awt.event.*;
import java.util.Iterator;
import java.util.Stack;
import javax.swing.*;
import javax.swing.event.TreeModelEvent;
import javax.swing.event.TreeModelListener;
import javax.swing.tree.*;
import javax.swing.border.*;
import org.vrjuggler.tweek.beans.*;
import org.vrjuggler.tweek.beans.loader.BeanJarClassLoader;


/**
 * Presents the Bean hierarchy as icons in a style similar to a GUI's folder
 * view.
 *
 * @since 1.0
 */
public class BeanIconViewer extends DefaultBeanModelViewer
{
   public BeanIconViewer ()
   {
   }

   /**
    * Sets the data model being displayed by this viewer and constructs the
    * view of the model.  If the given model object is null, no action is
    * taken.
    *
    * @pre No data model has been provided.
    * @post m_home_panel is created, and m_cur_panel is set to m_home_panel.
    *       The tree model is traversed, and the view is created.
    */
   public void initDataModel (BeanTreeModel model)
   {
      if ( model != null )
      {
         DefaultMutableTreeNode root = (DefaultMutableTreeNode) model.getRoot();
         m_home_panel = new BeanIconPanel(root, "Home");
         handleChildren(root, m_home_panel);
         m_cur_panel = m_home_panel;
      }
   }

   /**
    * @pre setDataModel has been called.
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
    * Refreshes the data model being displayed by this viewer and reconstructs
    * the view of the model.  If the given model object is null, no action is
    * taken.
    */
   public void refreshDataModel (BeanTreeModel model)
   {
      if ( model != null )
      {
         DefaultMutableTreeNode root = (DefaultMutableTreeNode) model.getRoot();
         m_home_panel.removeAll();
         handleChildren(root, m_home_panel);
         m_home_panel.doLayout();
         m_home_panel.repaint();
      }
   }

   public JComponent getViewer ()
   {
      return viewer;
   }

   private void jbInit () throws Exception
   {
      m_home_panel.setName("Home");
      m_home_panel.setBackground(Color.white);

      viewer.setLayout(m_icon_panel_layout);

      m_home_button.setToolTipText("Home");

      String icon_name = "org/vrjuggler/tweek/iconviewer/home.gif";

      try
      {
         m_home_button.setIcon(new ImageIcon(BeanJarClassLoader.instance().getResource(icon_name)));
      }
      catch (NullPointerException e)
      {
         m_home_button.setText("Home");
         System.err.println("WARNING: Failed to load icon " + icon_name);
      }

      m_home_button.setVerticalTextPosition(JLabel.BOTTOM);
      m_home_button.setHorizontalTextPosition(JLabel.CENTER);
      m_home_button.addActionListener(new ActionListener ()
         {
            public void actionPerformed (ActionEvent e)
            {
               homeClicked();
            }
         });

      m_back_button.setEnabled(false);
      m_back_button.setToolTipText("Back");

      icon_name = "org/vrjuggler/tweek/iconviewer/back.gif";

      try
      {
         m_back_button.setIcon(new ImageIcon(BeanJarClassLoader.instance().getResource(icon_name)));
      }
      catch (NullPointerException e)
      {
         m_back_button.setText("Back");
         System.err.println("WARNING: Failed to load icon " + icon_name);
      }

      m_back_button.setVerticalTextPosition(JLabel.BOTTOM);
      m_back_button.setHorizontalTextPosition(JLabel.CENTER);
      m_back_button.addActionListener(new ActionListener ()
         {
            public void actionPerformed (ActionEvent e)
            {
               backClicked();
            }
         });

      m_forw_button.setEnabled(false);
      m_forw_button.setToolTipText("Forward");

      icon_name = "org/vrjuggler/tweek/iconviewer/forward.gif";

      try
      {
         m_forw_button.setIcon(new ImageIcon(BeanJarClassLoader.instance().getResource(icon_name)));
      }
      catch (NullPointerException e)
      {
         m_forw_button.setText("Forward");
         System.err.println("WARNING: Failed to load icon " + icon_name);
      }

      m_forw_button.setVerticalTextPosition(JLabel.BOTTOM);
      m_forw_button.setHorizontalTextPosition(JLabel.CENTER);
      m_forw_button.addActionListener(new ActionListener ()
         {
            public void actionPerformed (ActionEvent e)
            {
               forwClicked();
            }
         });

      m_multi_button.setText("Multiple Windows");
      m_multi_button.addItemListener(new ItemListener ()
         {
            public void itemStateChanged (ItemEvent e)
            {
               multiStateToggle(e);
            }
         });

      m_sep.setOrientation(SwingConstants.VERTICAL);

      m_view_container.setBorder(BorderFactory.createLoweredBevelBorder());

      m_nav_bar.setBorder(BorderFactory.createEtchedBorder());
      m_nav_bar.add(m_home_button, null);
      m_nav_bar.add(m_back_button, null);
      m_nav_bar.add(m_forw_button, null);
      m_nav_bar.add(m_sep, null);
      m_nav_bar.add(m_multi_button, null);
      viewer.add(m_nav_bar,  BorderLayout.NORTH);

      viewer.add(m_view_container,  BorderLayout.CENTER);
      m_view_container.getViewport().add(m_cur_panel, null);
   }

   private void addInternalFrame (JComponent panel)
   {
      JInternalFrame frame = new JInternalFrame();
      m_desktop_pane.add(frame);
      frame.getContentPane().setLayout(new BorderLayout());
      frame.getContentPane().add(new JScrollPane(panel), null);
      frame.setTitle(panel.getName());
      frame.setRequestFocusEnabled(true);
      frame.setClosable(true);
      frame.setIconifiable(true);
      frame.setResizable(true);
      frame.pack();
      frame.setBackground(Color.white);
      frame.setVisible(true);
   }

   private void homeClicked ()
   {
      if ( m_multi_win )
      {
         addInternalFrame(m_home_panel);
      }
      else
      {
         if ( m_cur_panel != m_home_panel )
         {
            pushCurrentBack(m_home_panel);
         }
      }
   }

   /**
    * Handles the case when the backward navigation button is clicked.
    */
   private void backClicked ()
   {
      if ( m_forw_stack.empty() )
      {
         m_forw_button.setEnabled(true);
      }

      swapPanel(m_back_stack, m_forw_stack);

      if ( m_last_panel != null )
      {
         fireBeanUnfocusEvent(m_last_panel);

         // XXX: Not exactly what we want...
         m_last_panel = null;
      }

      if ( m_back_stack.empty() )
      {
         m_back_button.setEnabled(false);
      }
   }

   /**
    * Handles the case when the forward navigation button is clicked.
    */
   private void forwClicked ()
   {
      if ( m_back_stack.empty() )
      {
         m_back_button.setEnabled(true);
      }

      swapPanel(m_forw_stack, m_back_stack);

      if ( m_last_panel != null )
      {
         fireBeanUnfocusEvent(m_last_panel);

         // XXX: Not exactly what we want...
         m_last_panel = null;
      }

      if ( m_forw_stack.empty() )
      {
         m_forw_button.setEnabled(false);
      }
   }

   /**
    * Moves the current panel to the top of to_stack and sets the current
    * panel to the top of from_stack.
    */
   private void swapPanel (Stack from_stack, Stack to_stack)
   {
      to_stack.push(m_cur_panel);
      m_view_container.getViewport().remove(m_cur_panel);
      m_cur_panel = (JComponent) from_stack.pop();

      m_view_container.getViewport().add(m_cur_panel, null);
      m_view_container.repaint();
   }

   /**
    * Toggles between single-window and multi-window views of the icon view.
    *
    * @post The view container is refreshed.
    */
   private void multiStateToggle (ItemEvent e)
   {
      if ( e.getStateChange() == ItemEvent.DESELECTED )
      {
         m_view_container.getViewport().remove(m_desktop_pane);
         m_view_container.getViewport().add(m_cur_panel, null);
         m_home_button.setEnabled(true);
         m_back_button.setEnabled(false);
         m_forw_button.setEnabled(false);
         m_back_stack.clear();
         m_forw_stack.clear();

         m_multi_win = false;
         m_desktop_pane.removeAll();
      }
      else
      {
         m_view_container.getViewport().remove(m_cur_panel);
         m_view_container.getViewport().add(m_desktop_pane, null);
         m_home_button.setEnabled(false);
         m_back_button.setEnabled(false);
         m_forw_button.setEnabled(false);

         m_multi_win = true;

         Iterator i = m_back_stack.iterator();

         while ( i.hasNext() )
         {
            addInternalFrame((JComponent) i.next());
         }

         addInternalFrame(m_cur_panel);
      }

      m_view_container.repaint();
   }

   /**
    * Pushes the current panel to the back stack and makes the given component
    * the new current panel.
    *
    * @post The view container is refreshed.
    */
   private void pushCurrentBack (JComponent new_component)
   {
      if ( m_back_stack.empty() )
      {
         m_back_button.setEnabled(true);
      }

      m_back_stack.push(m_cur_panel);

      // Clear out the forward navigation stack because what we are now viewing
      // is what will go into the forward stack if the Back button is pressed.
      m_forw_stack.clear();
      m_forw_button.setEnabled(false);

      m_view_container.getViewport().remove(m_cur_panel);
      m_cur_panel = new_component;
      m_view_container.getViewport().add(m_cur_panel, null);
      m_view_container.repaint();
   }

   private void handleChildren (TreeNode parent_data_node,
                                BeanIconPanel parent_panel)
   {
      int child_count = parent_data_node.getChildCount();

      for ( int i = 0; i < child_count; i++ )
      {
         DefaultMutableTreeNode child_data_node =
            (DefaultMutableTreeNode) parent_data_node.getChildAt(i);

         if ( child_data_node.isLeaf() )
         {
            addPanelIcon((PanelBean) child_data_node.getUserObject(),
                         parent_panel);
         }
         else
         {
            BeanIconPanel child_panel = new BeanIconPanel(child_data_node);
            DefaultMutableTreeNode child_node = new DefaultMutableTreeNode(parent_panel);
            addFolderIcon(child_panel, parent_panel);
            handleChildren(child_data_node, child_panel);
         }
      }
   }

   /**
    * Adds a folder icon to the given parent panel.  The folder "opens" to
    * contain the given child panel.
    */
   private void addFolderIcon (BeanIconPanel child_panel,
                               BeanIconPanel parent_panel)
   {
      JLabel folder_icon = new JLabel(child_panel.getName());

      String icon_name = "org/vrjuggler/tweek/iconviewer/bean_folder.gif";

      try
      {
         folder_icon.setIcon(new ImageIcon(BeanJarClassLoader.instance().getResource(icon_name)));
      }
      catch (NullPointerException e)
      {
         System.err.println("WARNING: Failed to load icon " + icon_name);
      }

      folder_icon.addMouseListener(new FolderMouseListener(child_panel));
      parent_panel.addIcon(folder_icon);
   }

   /**
    * Adds a panel (leaf) icon to the given parent panel.
    */
   private void addPanelIcon (PanelBean panel, BeanIconPanel panel_parent)
   {
      JLabel panel_icon = new JLabel(panel.toString());

      if ( panel.getIcon() == null )
      {
         String icon_name = "org/vrjuggler/tweek/iconviewer/bean_default.gif";

         try
         {
            panel_icon.setIcon(new ImageIcon(BeanJarClassLoader.instance().getResource(icon_name)));
         }
         catch (NullPointerException e)
         {
            System.err.println("WARNING: Failed to load icon " + icon_name);
         }
      }
      else
      {
         panel_icon.setIcon(panel.getIcon());
      }

      if ( panel.getToolTip() != null )
      {
         panel_icon.setToolTipText(panel.getToolTip());
      }

      panel_icon.addMouseListener(new IconMouseListener(panel));
      panel_parent.addIcon(panel_icon);
   }

   private class IconMouseListener implements MouseListener
   {
      public IconMouseListener (PanelBean p)
      {
         m_bean_panel = p;
      }

      public void mouseClicked (MouseEvent e)
      {
         System.out.println("Icon selected");
//         JLabel source = (JLabel) e.getSource();

         try
         {
            if ( ! m_bean_panel.isInstantiated() )
            {
               m_bean_panel.instantiate();
            }

            JComponent bean = m_bean_panel.getComponent();
            bean.setName(m_bean_panel.toString());

            if ( m_last_panel != null )
            {
               fireBeanUnfocusEvent(m_last_panel);
            }

            fireBeanFocusEvent(m_bean_panel);
            m_last_panel = m_bean_panel;

            if ( m_multi_win )
            {
               addInternalFrame(bean);
            }
            else
            {
               pushCurrentBack(bean);
            }
         }
         catch (org.vrjuggler.tweek.beans.loader.BeanInstantiationException inst_ex)
         {
            JOptionPane.showMessageDialog(null, inst_ex.getMessage(),
                                          "Instantiation Failure",
                                          JOptionPane.ERROR_MESSAGE);
         }
      }

      public void mouseEntered (MouseEvent e)
      {
      }

      public void mouseExited (MouseEvent e)
      {
      }

      public void mousePressed (MouseEvent e)
      {
      }

      public void mouseReleased (MouseEvent e)
      {
      }

      private PanelBean m_bean_panel = null;
   }

   private class FolderMouseListener implements MouseListener
   {
      public FolderMouseListener (BeanIconPanel icon_panel)
      {
         m_panel = icon_panel;
      }

      public void mouseClicked (MouseEvent e)
      {
         if ( m_last_panel != null )
         {
            fireBeanUnfocusEvent(m_last_panel);
            m_last_panel = null;
         }

         if ( m_multi_win )
         {
            addInternalFrame(m_panel);
         }
         else
         {
            pushCurrentBack(m_panel);
         }
      }

      public void mouseEntered (MouseEvent e)
      {
      }

      public void mouseExited (MouseEvent e)
      {
      }

      public void mousePressed (MouseEvent e)
      {
      }

      public void mouseReleased (MouseEvent e)
      {
      }

      private BeanIconPanel m_panel = null;
   }

   private JPanel       viewer           = new JPanel();
   private BorderLayout m_icon_panel_layout = new BorderLayout();

   private JToolBar           m_nav_bar      = new JToolBar();
   private JButton            m_home_button  = new JButton();
   private JButton            m_back_button  = new JButton();
   private JButton            m_forw_button  = new JButton();
   private JToolBar.Separator m_sep          = new JToolBar.Separator();
   private JCheckBox          m_multi_button = new JCheckBox();

   private JScrollPane m_view_container = new JScrollPane();

   private BeanIconPanel m_home_panel = null;
   private JComponent    m_cur_panel  = null;
   private PanelBean     m_last_panel = null;

   private JDesktopPane m_desktop_pane = new JDesktopPane();
   private boolean      m_multi_win    = false;

   private Stack m_back_stack = new Stack();
   private Stack m_forw_stack = new Stack();
}
