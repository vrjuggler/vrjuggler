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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.vrjconfig.controlpanel;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Frame;
import java.awt.event.*;
import java.io.*;
import java.util.*;
import javax.swing.*;
import javax.swing.tree.DefaultMutableTreeNode;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.tweek.beans.BeanRegistry;
import org.vrjuggler.tweek.beans.loader.BeanJarClassLoader;
import org.vrjuggler.tweek.services.EnvironmentServiceProxy;
import org.vrjuggler.tweek.services.GlobalPreferencesService;
import org.vrjuggler.tweek.services.GlobalPreferencesServiceProxy;
import org.vrjuggler.tweek.wizard.*;
import org.vrjuggler.vrjconfig.ui.*;
import org.vrjuggler.jccl.editors.*;

/**
 * Provides a control panel view into a config element collection.
 */
public class ControlPanelView
   extends JPanel
{
   public ControlPanelView()
   {
      try
      {
         jbInit();
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }

      try
      {
         GlobalPreferencesService prefs = new GlobalPreferencesServiceProxy();

         // Using the global user preferences from Tweek, set the start
         // directory for mFileChooser.
         File f = new File(prefs.getChooserStartDir());
         mFileChooser.setCurrentDirectory(f);
      }
      catch(Exception ex)
      {
         System.err.println("ControlPanelView(): WARNING: Failed to set file chooser start directory: " +
                            ex.getMessage());
      }

      // Add forward and back buttons to the toolbar
      mToolbar.addToToolbar(Box.createHorizontalStrut(8));
      mBackBtn.setText("Back");
      mBackBtn.setEnabled(false);
      mBackBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            backClicked();
         }
      });
      mForwardBtn.setText("Forward");
      mForwardBtn.setEnabled(false);
      mForwardBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            forwClicked();
         }
      });
      mToolbar.addToToolbar(mBackBtn);
      mToolbar.addToToolbar(mForwardBtn);

      // Make sure the toolbar is using our context whenever we become active.
      addComponentListener(new ComponentAdapter()
      {
         public void componentShown(ComponentEvent evt)
         {
            mToolbar.setConfigContext(mContext);
         }
      });

      // Load the ControlPanel model from XML file.
      EnvironmentServiceProxy env_service = new EnvironmentServiceProxy();
      String controlpanel_path = env_service.expandEnvVars("${VJ_BASE_DIR}/share/vrjuggler/data/ControlPanel.xml");
      ControlPanelViewModel model = new ControlPanelViewModel(controlpanel_path);
      showCategoryPanel((CategoryNode)model.getRoot());
   }

   /**
    * Helper class for getting the config manager serivce.
    */
   private ConfigBroker getConfigBroker()
   {
      return new ConfigBrokerProxy();
   }

   private void showTypePanel(TypeNode root)
   {
      ControlPanel ctl = createPanelWithElementsOfType(root.getToken());
      ctl.setTitle(root.getTitle());
   
      ctl.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            JDialog dlg = new JDialog(
               (Frame)SwingUtilities.getAncestorOfClass(Frame.class, ControlPanelView.this),
               "Positional Device Editor",
               true);
            
            ControlPanel control = (ControlPanel)evt.getSource();
            Object object = control.getModel().getUserObjectAt(evt.getID());
            if(null != object && object instanceof ConfigElement)
            {
               ConfigElement element = (ConfigElement)object;
               String token = element.getDefinition().getToken();
               
               List list = CustomEditorRegistry.findEditors(token);
               
               //XXX:This will be used after making findEditors -> findEditor
               //if(null != editor)
               if(null != list && list.size() > 0)
               {
                  CustomEditor editor = (CustomEditor)list.get(0);
                  dlg.getContentPane().add(editor.getPanel(), BorderLayout.CENTER);
                  
                  // Make sure to set the context first.
                  editor.setContext(mContext);
                  editor.setConfigElement(element);
                  dlg.setTitle(editor.getTitle());
                  dlg.pack();
                  dlg.setVisible(true);
               }
            }
         }
      });
      pushCurrentBack(ctl);
   }
   private void showEditorPanel(EditorNode root)
   {
      System.out.println("Editor Node");
   }

   private void showWizardPanel(WizardNode root)
   {
      System.out.println("Wizard Node");
      
      final JDialog dialog = new JDialog();
      dialog.setSize(590, 430);
      dialog.setModal(true);
      dialog.setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);
      dialog.getContentPane().setLayout(new BorderLayout());

      // Load wizard
      Wizard wizard = null;
      JFileChooser chooser = mFileChooser;
      int result = chooser.showOpenDialog(this);
      if (result == JFileChooser.CANCEL_OPTION)
      {
         return;
      }
      File file = chooser.getSelectedFile();
      String newFilename = file.getAbsolutePath();
      try
      {
         WizardInputStream in = new WizardInputStream(newFilename);
         wizard = in.readWizard();
         in.close();
      }
      catch (IOException ioe)
      {
         ioe.printStackTrace();
         JOptionPane.showMessageDialog(this,
                        "Open failed: "+ioe.getMessage(),
                        "Error", JOptionPane.ERROR_MESSAGE);
         return;
      }

      
      WizardViewerBean viewer = new WizardViewerBean();
      viewer.setWizard(wizard);
      viewer.setSidebarImage(new ImageIcon(this.getClass().getClassLoader().getResource("org/vrjuggler/tweek/wizard/images/juggler_sidebar.png")));
      viewer.addWizardViewListener(new WizardViewListener()
      {
         public void wizardStarted(WizardViewEvent evt) {}

         public void wizardCancelled(WizardViewEvent evt)
         {
            dialog.setVisible(false);
            dialog.dispose();
         }

         public void wizardFinished(WizardViewEvent evt)
         {
            dialog.setVisible(false);
            dialog.dispose();
         }
      });
      dialog.getContentPane().add(viewer, BorderLayout.CENTER);
      dialog.setVisible(true);
   }
   
   private void showCategoryPanel(CategoryNode root)
   {
      DefaultControlPanelModel model = new DefaultControlPanelModel();
      if(!root.getAllowsChildren())
      {
         System.out.println("ERROR...");
         System.out.println(root.getClass());
      }
      for(Enumeration e = root.children() ; e.hasMoreElements() ; )
      {
         ControlPanelNode node = (ControlPanelNode)e.nextElement();
         model.add(node);
      }
      ControlPanel new_control = new ControlPanel();
      new_control.setModel(model);
      new_control.setTitle(root.getTitle());
      new_control.addActionListener(new ActionListener()
            {
               public void actionPerformed(ActionEvent evt)
               {
                  ControlPanel control = (ControlPanel)evt.getSource();
                  Object value = control.getModel().getUserObjectAt(evt.getID());
                  if(value instanceof CategoryNode)
                  {
                     showCategoryPanel((CategoryNode)value);
                  }
                  else if(value instanceof TypeNode)
                  {
                     showTypePanel((TypeNode)value);
                  }
                  else if(value instanceof EditorNode)
                  {
                     showEditorPanel((EditorNode)value);
                  }
                  else if(value instanceof WizardNode)
                  {
                     showWizardPanel((WizardNode)value);
                  }
               }
            });
      pushCurrentBack(new_control);
   }

   /**
    * Displays a panel containing all of the elements that have the given
    * token.
    */
   private ControlPanel createPanelWithElementsOfType(String token)
   {
      ClassLoader loader = BeanJarClassLoader.instance();
      DefaultControlPanelModel model = new DefaultControlPanelModel();
      /*
      List devices = ConfigUtilities.getElementsWithDefinition(
                           getConfigBroker().getElements(mContext), token);
      */
      List devices = getElementsWithDefinitionAndDerived(token);
      for (Iterator itr = devices.iterator(); itr.hasNext(); )
      {
         ConfigElement element = (ConfigElement)itr.next();
         String icon_location = element.getDefinition().getIconLocation();
         icon_location = (new EnvironmentServiceProxy()).expandEnvVars(icon_location);
         try
         {
            java.net.URL url = new java.net.URL(icon_location);
            Icon device_icon = new ImageIcon(url);
            model.add(element.getName(), device_icon, element);
         }
         catch(Exception ex)
         {
            ex.printStackTrace();
         }
      }
      ControlPanel new_control = new ControlPanel();
      new_control.setModel(model);

      return new_control;
   }

   private List getElementsWithDefinitionAndDerived(String token)
   {
      List result = new ArrayList();
      List elts = getConfigBroker().getElements(mContext);
      ConfigDefinitionRepository repos = getConfigBroker().getRepository();
      List sub_defs = repos.getSubDefinitions(token); 
      for(Iterator itr = sub_defs.iterator() ; itr.hasNext() ; )
      {
         List elements = ConfigUtilities.getElementsWithDefinition(elts, (String)itr.next());
         result.addAll(elements);
      }
      return result;
   }

   /**
    * Handles the case when the backward navigation button is clicked.
    */
   private void backClicked()
   {
      if (mForwardStack.empty())
      {
         mForwardBtn.setEnabled(true);
      }

      // Change the current component to the one at the top of the back stack.
      swapCurComponent(mBackStack, mForwardStack);

      if (mBackStack.empty())
      {
         mBackBtn.setEnabled(false);
      }
   }

   /**
    * Handles the case when the forward navigation button is clicked.
    */
   private void forwClicked()
   {
      if (mBackStack.empty())
      {
         mBackBtn.setEnabled(true);
      }

      // Change the current component to the next panel in the forward stack.
      swapCurComponent(mForwardStack, mBackStack);

      if (mForwardStack.empty())
      {
         mForwardBtn.setEnabled(false);
      }
   }

   /**
    * Moves the current component to the top of toStack and sets the current
    * component to the top of fromStack.
    */
   private void swapCurComponent(Stack fromStack, Stack toStack)
   {
      toStack.push(mCurrentView);

      this.remove(mCurrentView);
      mCurrentView = (Component)fromStack.pop();
      this.add(mCurrentView, BorderLayout.CENTER);
      this.repaint();
   }

   /**
    * Pushes the current component to the back stack and makes the given
    * component the new current component.
    */
   private void pushCurrentBack(Component newComp)
   {
      if (mBackStack.empty())
      {
         mBackBtn.setEnabled(true);
      }

      mBackStack.push(mCurrentView);

      // Clear out the forward navigation stack because what we are now viewing
      // is what will go into the forward stack if the back button is pressed.
      mForwardStack.clear();
      mForwardBtn.setEnabled(false);

      this.remove(mCurrentView);
      mCurrentView = newComp;
      this.add(mCurrentView, BorderLayout.CENTER);
      updateUI();
      newComp.invalidate();
   }

   /**
    * Responding to notification that from the toolbar that a configuration has
    * been opened or a new configuration has been created, this method creates
    * a new main panel to hold that configuration.
    */
   private void toolbarContextChanged()
   {
      mContext = mToolbar.getConfigContext();

      EnvironmentServiceProxy env_service = new EnvironmentServiceProxy();
      String controlpanel_path = env_service.expandEnvVars("${VJ_BASE_DIR}/share/vrjuggler/data/ControlPanel.xml");
      ControlPanelViewModel model = new ControlPanelViewModel(controlpanel_path);
      showCategoryPanel((CategoryNode)model.getRoot());
   }

   /**
    * Autogenerated code for the JBuilder GUI.
    */
   private void jbInit()
      throws Exception
   {
      this.setLayout(mBaseLayout);
      mToolbar.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            String cmd = evt.getActionCommand();
            if (cmd.equals("New") || cmd.equals("Open"))
            {
               toolbarContextChanged();
            }
         }
      });
      this.add(mToolbar,  BorderLayout.NORTH);
   }

   //--- JBuilder GUI variables ---//
   private BorderLayout mBaseLayout = new BorderLayout();
   private ConfigToolbar mToolbar = new ConfigToolbar();
   private JFileChooser mFileChooser = new JFileChooser();

   /**
    * The context providing a view into the current configuration.
    */
   private ConfigContext mContext = new ConfigContext();

   /**
    * The currently viewed component.
    */
   private Component mCurrentView = new ControlPanel();

   private JButton mBackBtn = new JButton();
   private JButton mForwardBtn = new JButton();

   private Stack mBackStack = new Stack();
   private Stack mForwardStack = new Stack();
}
