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
public class BeanIconViewer
   extends DefaultBeanModelViewer
   implements BeanRegistrationListener
{
   public BeanIconViewer ()
   {
   }

   public void setModel (BeanTreeModel model)
   {
      // We only want to perform these steps the first time our model is set.
      if ( mModel == null )
      {
         BeanRegistry.instance().addBeanRegistrationListener(this);
         initDataModel(model);
      }
      else
      {
         mModel = model;
         refreshDataModel();
      }

      mModel = model;
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

   // =========================================================================
   // Private methods.
   // =========================================================================

   /**
    * Sets the data model being displayed by this viewer and constructs the
    * view of the model.  If the given model object is null, no action is
    * taken.
    *
    * @pre No data model has been provided.
    * @post mHomePanel is created, and mCurPanel is set to mHomePanel.
    *       The tree model is traversed, and the view is created.
    */
   private void initDataModel (BeanTreeModel model)
   {
      DefaultMutableTreeNode root = (DefaultMutableTreeNode) model.getRoot();
      mHomePanel = new BeanIconPanel(root, "Home");
      handleChildren(root, mHomePanel);
      mCurPanel = mHomePanel;
   }

   /**
    * Refreshes the data model being displayed by this viewer and reconstructs
    * the view of the model.  If the given model object is null, no action is
    * taken.
    */
   private void refreshDataModel ()
   {
      DefaultMutableTreeNode root = (DefaultMutableTreeNode) mModel.getRoot();
      mHomePanel.removeAll();
      handleChildren(root, mHomePanel);
      mHomePanel.doLayout();
      mHomePanel.repaint();
   }

   private void jbInit () throws Exception
   {
      mHomePanel.setName("Home");
      mHomePanel.setBackground(Color.white);

      viewer.setLayout(mIconPanelLayout);

      mHomeButton.setToolTipText("Home");

      String icon_name = "org/vrjuggler/tweek/iconviewer/home.gif";

      try
      {
         mHomeButton.setIcon(new ImageIcon(BeanJarClassLoader.instance().getResource(icon_name)));
      }
      catch (NullPointerException e)
      {
         mHomeButton.setText("Home");
         System.err.println("WARNING: Failed to load icon " + icon_name);
      }

      mHomeButton.setVerticalTextPosition(JLabel.BOTTOM);
      mHomeButton.setHorizontalTextPosition(JLabel.CENTER);
      mHomeButton.addActionListener(new ActionListener ()
         {
            public void actionPerformed (ActionEvent e)
            {
               homeClicked();
            }
         });

      mBackButton.setEnabled(false);
      mBackButton.setToolTipText("Back");

      icon_name = "org/vrjuggler/tweek/iconviewer/back.gif";

      try
      {
         mBackButton.setIcon(new ImageIcon(BeanJarClassLoader.instance().getResource(icon_name)));
      }
      catch (NullPointerException e)
      {
         mBackButton.setText("Back");
         System.err.println("WARNING: Failed to load icon " + icon_name);
      }

      mBackButton.setVerticalTextPosition(JLabel.BOTTOM);
      mBackButton.setHorizontalTextPosition(JLabel.CENTER);
      mBackButton.addActionListener(new ActionListener ()
         {
            public void actionPerformed (ActionEvent e)
            {
               backClicked();
            }
         });

      mForwButton.setEnabled(false);
      mForwButton.setToolTipText("Forward");

      icon_name = "org/vrjuggler/tweek/iconviewer/forward.gif";

      try
      {
         mForwButton.setIcon(new ImageIcon(BeanJarClassLoader.instance().getResource(icon_name)));
      }
      catch (NullPointerException e)
      {
         mForwButton.setText("Forward");
         System.err.println("WARNING: Failed to load icon " + icon_name);
      }

      mForwButton.setVerticalTextPosition(JLabel.BOTTOM);
      mForwButton.setHorizontalTextPosition(JLabel.CENTER);
      mForwButton.addActionListener(new ActionListener ()
         {
            public void actionPerformed (ActionEvent e)
            {
               forwClicked();
            }
         });

      mMultiButton.setText("Multiple Windows");
      mMultiButton.addItemListener(new ItemListener ()
         {
            public void itemStateChanged (ItemEvent e)
            {
               multiStateToggle(e);
            }
         });

      mSep.setOrientation(SwingConstants.VERTICAL);

      mViewContainer.setBorder(BorderFactory.createLoweredBevelBorder());

      mNavBar.setBorder(BorderFactory.createEtchedBorder());
      mNavBar.add(mHomeButton, null);
      mNavBar.add(mBackButton, null);
      mNavBar.add(mForwButton, null);
      mNavBar.add(mSep, null);
      mNavBar.add(mMultiButton, null);
      viewer.add(mNavBar,  BorderLayout.NORTH);

      viewer.add(mViewContainer,  BorderLayout.CENTER);
      mViewContainer.getViewport().add(mCurPanel, null);
   }

   private void addInternalFrame (JComponent panel)
   {
      JInternalFrame frame = new JInternalFrame();
      mDesktopPane.add(frame);
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
      if ( mMultiWin )
      {
         addInternalFrame(mHomePanel);
      }
      else
      {
         if ( mCurPanel != mHomePanel )
         {
            pushCurrentBack(mHomePanel);
         }
      }
   }

   /**
    * Handles the case when the backward navigation button is clicked.
    */
   private void backClicked ()
   {
      if ( mForwStack.empty() )
      {
         mForwButton.setEnabled(true);
      }

      swapPanel(mBackStack, mForwStack);

      if ( mLastPanel != null )
      {
         fireBeanUnfocusEvent(mLastPanel);

         // XXX: Not exactly what we want...
         mLastPanel = null;
      }

      if ( mBackStack.empty() )
      {
         mBackButton.setEnabled(false);
      }
   }

   /**
    * Handles the case when the forward navigation button is clicked.
    */
   private void forwClicked ()
   {
      if ( mBackStack.empty() )
      {
         mBackButton.setEnabled(true);
      }

      swapPanel(mForwStack, mBackStack);

      if ( mLastPanel != null )
      {
         fireBeanUnfocusEvent(mLastPanel);

         // XXX: Not exactly what we want...
         mLastPanel = null;
      }

      if ( mForwStack.empty() )
      {
         mForwButton.setEnabled(false);
      }
   }

   /**
    * Moves the current panel to the top of toStack and sets the current
    * panel to the top of fromStack.
    */
   private void swapPanel (Stack fromStack, Stack toStack)
   {
      toStack.push(mCurPanel);
      mViewContainer.getViewport().remove(mCurPanel);
      mCurPanel = (JComponent) fromStack.pop();

      mViewContainer.getViewport().add(mCurPanel, null);
      mViewContainer.repaint();
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
         mViewContainer.getViewport().remove(mDesktopPane);
         mViewContainer.getViewport().add(mCurPanel, null);
         mHomeButton.setEnabled(true);
         mBackButton.setEnabled(false);
         mForwButton.setEnabled(false);
         mBackStack.clear();
         mForwStack.clear();

         mMultiWin = false;
         mDesktopPane.removeAll();
      }
      else
      {
         mViewContainer.getViewport().remove(mCurPanel);
         mViewContainer.getViewport().add(mDesktopPane, null);
         mHomeButton.setEnabled(false);
         mBackButton.setEnabled(false);
         mForwButton.setEnabled(false);

         mMultiWin = true;

         Iterator i = mBackStack.iterator();

         while ( i.hasNext() )
         {
            addInternalFrame((JComponent) i.next());
         }

         addInternalFrame(mCurPanel);
      }

      mViewContainer.repaint();
   }

   /**
    * Pushes the current panel to the back stack and makes the given component
    * the new current panel.
    *
    * @post The view container is refreshed.
    */
   private void pushCurrentBack (JComponent newComponent)
   {
      if ( mBackStack.empty() )
      {
         mBackButton.setEnabled(true);
      }

      mBackStack.push(mCurPanel);

      // Clear out the forward navigation stack because what we are now viewing
      // is what will go into the forward stack if the Back button is pressed.
      mForwStack.clear();
      mForwButton.setEnabled(false);

      mViewContainer.getViewport().remove(mCurPanel);
      mCurPanel = newComponent;
      mViewContainer.getViewport().add(mCurPanel, null);
      mViewContainer.repaint();
   }

   private void handleChildren (TreeNode parentDataNode,
                                BeanIconPanel parentPanel)
   {
      int child_count = parentDataNode.getChildCount();

      for ( int i = 0; i < child_count; i++ )
      {
         DefaultMutableTreeNode child_data_node =
            (DefaultMutableTreeNode) parentDataNode.getChildAt(i);

         if ( child_data_node.isLeaf() )
         {
            addPanelIcon((PanelBean) child_data_node.getUserObject(),
                         parentPanel);
         }
         else
         {
            BeanIconPanel child_panel = new BeanIconPanel(child_data_node);
            DefaultMutableTreeNode child_node = new DefaultMutableTreeNode(parentPanel);
            addFolderIcon(child_panel, parentPanel);
            handleChildren(child_data_node, child_panel);
         }
      }
   }

   /**
    * Adds a folder icon to the given parent panel.  The folder "opens" to
    * contain the given child panel.
    */
   private void addFolderIcon (BeanIconPanel childPanel,
                               BeanIconPanel parentPanel)
   {
      JButton folder_icon = new JButton(childPanel.getName());

      String icon_name = "org/vrjuggler/tweek/iconviewer/bean_folder.gif";

      try
      {
         folder_icon.setIcon(new ImageIcon(BeanJarClassLoader.instance().getResource(icon_name)));
      }
      catch (NullPointerException e)
      {
         System.err.println("WARNING: Failed to load icon " + icon_name);
      }

      folder_icon.setToolTipText(childPanel.getName());
      folder_icon.addMouseListener(new FolderMouseListener(childPanel));
      parentPanel.addIcon(folder_icon);
   }

   /**
    * Adds a panel (leaf) icon to the given parent panel.
    */
   private void addPanelIcon (PanelBean panel, BeanIconPanel panelParent)
   {
      JButton panel_icon = new JButton(panel.toString());

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
      else
      {
         panel_icon.setToolTipText(panel.toString());
      }

      panel_icon.addMouseListener(new IconMouseListener(panel));
      panelParent.addIcon(panel_icon);
   }

   private class IconMouseListener implements MouseListener
   {
      public IconMouseListener (PanelBean p)
      {
         mBeanPanel = p;
      }

      public void mouseClicked (MouseEvent e)
      {
         System.out.println("Icon selected");
//         JLabel source = (JLabel) e.getSource();

         try
         {
            if ( ! mBeanPanel.isInstantiated() )
            {
               mBeanPanel.instantiate();
            }

            JComponent bean = mBeanPanel.getComponent();
            bean.setName(mBeanPanel.toString());

            if ( mLastPanel != null )
            {
               fireBeanUnfocusEvent(mLastPanel);
            }

            fireBeanFocusEvent(mBeanPanel);
            mLastPanel = mBeanPanel;

            if ( mMultiWin )
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

      private PanelBean mBeanPanel = null;
   }

   private class FolderMouseListener implements MouseListener
   {
      public FolderMouseListener (BeanIconPanel icon_panel)
      {
         mPanel = icon_panel;
      }

      public void mouseClicked (MouseEvent e)
      {
         if ( mLastPanel != null )
         {
            fireBeanUnfocusEvent(mLastPanel);
            mLastPanel = null;
         }

         if ( mMultiWin )
         {
            addInternalFrame(mPanel);
         }
         else
         {
            pushCurrentBack(mPanel);
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

      private BeanIconPanel mPanel = null;
   }

   private BeanTreeModel mModel = null;

   private JPanel       viewer           = new JPanel();
   private BorderLayout mIconPanelLayout = new BorderLayout();

   private JToolBar           mNavBar      = new JToolBar();
   private JButton            mHomeButton  = new JButton();
   private JButton            mBackButton  = new JButton();
   private JButton            mForwButton  = new JButton();
   private JToolBar.Separator mSep         = new JToolBar.Separator();
   private JCheckBox          mMultiButton = new JCheckBox();

   private JScrollPane mViewContainer = new JScrollPane();

   private BeanIconPanel mHomePanel = null;
   private JComponent    mCurPanel  = null;
   private PanelBean     mLastPanel = null;

   private JDesktopPane mDesktopPane = new JDesktopPane();
   private boolean      mMultiWin    = false;

   private Stack mBackStack = new Stack();
   private Stack mForwStack = new Stack();
}
