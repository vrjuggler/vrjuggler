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

package org.vrjuggler.tweek.iconviewer;

import java.awt.*;
import java.awt.event.*;
import java.util.Iterator;
import java.util.Stack;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.tree.*;
import javax.swing.border.*;
import org.vrjuggler.tweek.beans.*;
import org.vrjuggler.tweek.beans.loader.BeanJarClassLoader;


/**
 * Presents the Bean hierarchy as icons in a style similar to a GUI's folder
 * view.
 */
public class BeanIconViewer
   extends DefaultBeanModelViewer
   implements BeanRegistrationListener, InternalFrameListener, BeanPreferences,
              PrefsEventListener
{
   public BeanIconViewer ()
   {
      mEditorPanel.addPrefsEventListener(this);
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

   public BeanTreeModel getModel()
   {
      return mModel;
   }

   /**
    * @pre setDataModel has been called.
    */
   public void initGUI()
   {
      // Make sure that our preferences are loaded before the GUI is set up.
      load();

      try
      {
         jbInit();
         setNavigationIcons(mEditorPanel.isSmallGui());
      }
      catch (Exception e)
      {
         e.printStackTrace();
      }
   }

   public JComponent getViewer ()
   {
      return viewer;
   }

   /**
    * Programmatically focuses the given panel bean in this viewer.
    *
    * @param bean       the bean to focus
    */
   public void focusBean(PanelBean bean)
   {
      System.err.println("BeanIconViewer.focusBean(PanelBean) not implemented");
   }

   public void beanRegistered (BeanRegistrationEvent e)
   {
      if ( e.getBean() instanceof PanelBean )
      {
         refreshDataModel();
      }
   }

   /**
    * This does nothing.  initGUI() handles all the interface configuration.
    */
   public void initPrefsGUI()
   {
   }

   public boolean canCommit()
   {
      return true;
   }

   public JComponent getEditor()
   {
      return mEditorPanel;
   }

   public String getEditorName()
   {
      return mEditorPanel.getName();
   }

   public void load()
   {
      mEditorPanel.load();
   }

   public void save()
   {
      mEditorPanel.save();
   }

   public void prefsChanged(PrefsEvent e)
   {
      setNavigationIcons(e.isSmallGui());
   }

   /**
    * Implementation of InternalFrameListener interface.  If the newly
    * activated internal frame contains a Panel Bean, a BeanFocusChangeEvent
    * is fired to indicate that the Bean has been focused.
    *
    * @post mCurPanel is set to the newly selected icon panel.
    */
   public void internalFrameActivated (InternalFrameEvent e)
   {
      activateFrame((InternalBeanFrame) e.getInternalFrame());
   }

   /**
    * Implementation of InternalFrameListener interface.  If the newly
    * closed internal frame contains a Panel Bean, a BeanFocusChangeEvent
    * is fired to indicate that the Bean has been unfocused.
    *
    * @post mCurPanel is set to null.
    */
   public void internalFrameClosed (InternalFrameEvent e)
   {
      deactivateFrame((InternalBeanFrame) e.getInternalFrame());
   }

   /**
    * Implementation of InternalFrameListener interface.  This does nothing.
    */
   public void internalFrameClosing (InternalFrameEvent e)
   {
   }

   public void internalFrameDeiconified (InternalFrameEvent e)
   {
   }

   public void internalFrameIconified (InternalFrameEvent e)
   {
   }

   /**
    * Implementation of InternalFrameListener interface.  If the newly
    * deactivated internal frame contains a Panel Bean, a BeanFocusChangeEvent
    * is fired to indicate that the Bean has been unfocused.
    *
    * @post mCurPanel is set to null.
    */
   public void internalFrameDeactivated (InternalFrameEvent e)
   {
      deactivateFrame((InternalBeanFrame) e.getInternalFrame());
   }

   /**
    * Implementation of InternalFrameListener interface.  This does nothing.
    */
   public void internalFrameOpened (InternalFrameEvent e)
   {
   }

   /**
    * Helper for activating an InternalBeanFrame object.
    *
    * @post mCurPanel is set to null.
    */
   private void activateFrame (InternalBeanFrame frame)
   {
      BeanIconPanel icon_panel = frame.getIconPanel();

      if ( icon_panel.getBean() != null )
      {
         this.fireBeanFocusEvent(icon_panel.getBean());
      }

      mCurPanel = icon_panel;
   }

   /**
    * Helper for deactivating the active InternalBeanFrame object.
    *
    * @post mCurPanel is set to null.
    */
   private void deactivateFrame (InternalBeanFrame frame)
   {
      BeanIconPanel icon_panel = frame.getIconPanel();

      if ( icon_panel.getBean() != null )
      {
         this.fireBeanUnfocusEvent(icon_panel.getBean());
      }

//      mCurPanel = null;
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
      mHomePanel.getComponent().removeAll();
      handleChildren(root, mHomePanel);
      mHomePanel.getComponent().doLayout();
      mHomePanel.getComponent().repaint();
   }

   private void jbInit () throws Exception
   {
      mHomePanel.getComponent().setName("Home");
      mHomePanel.getComponent().setBackground(Color.white);

      viewer.setLayout(mIconPanelLayout);

      mHomeButton.setToolTipText("Home");
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
      mViewContainer.getViewport().add(mCurPanel.getComponent(), null);
   }

   private void setNavigationIcons(boolean smallGui)
   {
      String icon_name = "org/vrjuggler/tweek/iconviewer/";

      if ( smallGui )
      {
         icon_name += "home-small.gif";
      }
      else
      {
         icon_name += "home.gif";
      }

      try
      {
         mHomeButton.setIcon(new ImageIcon(BeanJarClassLoader.instance().getResource(icon_name)));
      }
      catch (NullPointerException e)
      {
         mHomeButton.setText("Home");
         System.err.println("WARNING: Failed to load icon " + icon_name);
      }

      icon_name = "org/vrjuggler/tweek/iconviewer/";

      if ( smallGui )
      {
         icon_name += "back-small.gif";
      }
      else
      {
         icon_name += "back.gif";
      }

      try
      {
         mBackButton.setIcon(new ImageIcon(BeanJarClassLoader.instance().getResource(icon_name)));
      }
      catch (NullPointerException e)
      {
         mBackButton.setText("Back");
         System.err.println("WARNING: Failed to load icon " + icon_name);
      }

      icon_name = "org/vrjuggler/tweek/iconviewer/";

      if ( smallGui )
      {
         icon_name += "forward-small.gif";
      }
      else
      {
         icon_name += "forward.gif";
      }

      try
      {
         mForwButton.setIcon(new ImageIcon(BeanJarClassLoader.instance().getResource(icon_name)));
      }
      catch (NullPointerException e)
      {
         mForwButton.setText("Forward");
         System.err.println("WARNING: Failed to load icon " + icon_name);
      }
   }

   /**
    * Adds the given BeanIconPanel object as an internal frame for the
    * multi-window view.
    */
   private void addInternalFrame (BeanIconPanel panel)
   {
      InternalBeanFrame frame = new InternalBeanFrame();
      frame.addInternalFrameListener(this);
      frame.setIconPanel(panel);
      mDesktopPane.add(frame);
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
    * Handles the case when the backward navigation button is clicked.  This
    * may go from a Bean icon panel to a folder icon panel, but it will never
    * go from a Bean icon panel to a Bean icon panel.
    */
   private void backClicked ()
   {
      if ( mForwStack.empty() )
      {
         mForwButton.setEnabled(true);
      }

      // Signal listeners that the currently visible Bean is losing focus.
      if ( mCurPanel.getBean() != null )
      {
         fireBeanUnfocusEvent(mCurPanel.getBean());
      }

      // Change the current panel to the one at the top of the back stack.
      swapCurPanel(mBackStack, mForwStack);

      if ( mBackStack.empty() )
      {
         mBackButton.setEnabled(false);
      }
   }

   /**
    * Handles the case when the forward navigation button is clicked.  This
    * may go from a folder icon panel to a Bean icon panel, but it will never
    * go from a Bean icon panel to a Bean icon panel.
    */
   private void forwClicked ()
   {
      if ( mBackStack.empty() )
      {
         mBackButton.setEnabled(true);
      }

      // Change the current panel to the next panel in the forward stack.
      swapCurPanel(mForwStack, mBackStack);

      // Signal listeners that a new Bean is getting focus.
      if ( mCurPanel.getBean() != null )
      {
         fireBeanFocusEvent(mCurPanel.getBean());
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
   private void swapCurPanel (Stack fromStack, Stack toStack)
   {
      toStack.push(mCurPanel);
      mViewContainer.getViewport().remove(mCurPanel.getComponent());
      mCurPanel = (BeanIconPanel) fromStack.pop();

      mViewContainer.getViewport().add(mCurPanel.getComponent(), null);
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
         mViewContainer.getViewport().add(mCurPanel.getComponent(), null);
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
         mViewContainer.getViewport().remove(mCurPanel.getComponent());
         mViewContainer.getViewport().add(mDesktopPane, null);
         mHomeButton.setEnabled(false);
         mBackButton.setEnabled(false);
         mForwButton.setEnabled(false);

         mMultiWin = true;

         Iterator i = mBackStack.iterator();

         while ( i.hasNext() )
         {
            addInternalFrame((BeanIconPanel) i.next());
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
   private void pushCurrentBack (BeanIconPanel newComponent)
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

      mViewContainer.getViewport().remove(mCurPanel.getComponent());
      mCurPanel = newComponent;
      mViewContainer.getViewport().add(mCurPanel.getComponent(), null);
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
         BeanIconPanel child_panel = new BeanIconPanel(child_data_node);

         if ( child_data_node.isLeaf() )
         {
            addPanelIcon(child_panel, parentPanel);
         }
         else
         {
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
   private void addPanelIcon (BeanIconPanel childPanel,
                              BeanIconPanel panelParent)
   {
      PanelBean panel = childPanel.getBean();
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

      panel_icon.addMouseListener(new IconMouseListener(childPanel));
      panelParent.addIcon(panel_icon);
   }

   private class IconMouseListener implements MouseListener
   {
      public IconMouseListener (BeanIconPanel p)
      {
         mIconPanel = p;
      }

      public void mouseClicked (MouseEvent e)
      {
         System.out.println("Icon selected");
//         JLabel source = (JLabel) e.getSource();

         try
         {
            // Get the Panel Bean associated with the icon that was clicked.
            PanelBean bean_panel = mIconPanel.getBean();

            // If the Bean has not been instantiated, do so now.  (This is the
            // last possible moment when instantiation can occur.)
            if ( ! bean_panel.isInstantiated() )
            {
               bean_panel.instantiate();
            }

            // Retrieve the GUI component from the Panel Bean.
            JComponent bean = bean_panel.getComponent();
            bean.setName(bean_panel.toString());

            // If the currently visible panel has a Bean, it is going out of
            // focus now.
            if ( null != mCurPanel && mCurPanel.getBean() != null )
            {
               fireBeanUnfocusEvent(mCurPanel.getBean());
            }

            // The clicked icon's Bean is getting focus.
            fireBeanFocusEvent(bean_panel);

            if ( mMultiWin )
            {
               addInternalFrame(mIconPanel);
            }
            else
            {
               // This has the side effect of making mIconPanel visible and
               // setting mIconPanel to be the current panel.
               pushCurrentBack(mIconPanel);
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

      private BeanIconPanel mIconPanel = null;
   }

   private class FolderMouseListener implements MouseListener
   {
      public FolderMouseListener (BeanIconPanel icon_panel)
      {
         mPanel = icon_panel;
      }

      public void mouseClicked (MouseEvent e)
      {
         if ( mCurPanel.getBean() != null )
         {
            fireBeanUnfocusEvent(mCurPanel.getBean());
         }

         if ( mMultiWin )
         {
            addInternalFrame(mPanel);
         }
         else
         {
            // This has the side effect of making mPanel visible.
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
   private BeanIconPanel mCurPanel  = null;

   private JDesktopPane mDesktopPane = new JDesktopPane();
   private boolean      mMultiWin    = false;

   private Stack mBackStack = new Stack();
   private Stack mForwStack = new Stack();

   private PrefsEditor mEditorPanel = new PrefsEditor();
}
