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
package org.vrjuggler.jccl.editors;

import java.awt.*;
import java.awt.event.*;
import java.beans.*;
import java.util.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.tree.*;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.config.event.*;

/**
 * A specialized panel that knows how to edit configuration definitions within a
 * configuration definition repository.
 */
public class ConfigDefinitionRepositoryEditor
   extends JPanel
{
   /**
    * The ID used to reference the config definition editor panel in the
    * CardLayout for the editorPane.
    */
   final static String CONFIG_DEFINITION      = "ConfigDefinitionPane";

   /**
    * The ID used to reference the property definition editor panel in the
    * CardLayout for the editorPane.
    */
   final static String PROPERTY_DEFINITION   = "PropertyDefinitionPane";

   final static String EMPTY           = "Empty";

   /**
    * Creates a new configuration definition repository editor with no
    * definition to edit.
    */
   public ConfigDefinitionRepositoryEditor()
   {
      try
      {
         jbInit();
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }

      setupListeners();

      // Init the configuration definition editor pane
      mDefEditorScrollPane.setViewportView(mDefEditor);
      mDefEditorScrollPane.setMinimumSize(new Dimension(0, 0));
      mDefEditorScrollPane.setPreferredSize(mDefEditor.getPreferredSize());
      mEditorPane.add(mDefEditorScrollPane, CONFIG_DEFINITION);

      // Init the property definition editor pane
      mEditorPane.add(mPropertyDefEditor, PROPERTY_DEFINITION);

      // Init the empty editor pane
      mEditorPane.add(mEmptyPane, EMPTY);
      mEditorPaneLayout.show(mEditorPane, EMPTY);

      // Init the config definition tree
      mReposModel = new ConfigDefinitionRepositoryModel();
      mReposModel.setRepository((new ConfigBrokerProxy()).getRepository());
      mDefPropTree.setModel(mReposModel);
//      descPropTree.setEditable(true);
      mDefPropTree.setCellRenderer(new DefinitionRepositoryTreeCellRenderer());
      mDefPropTree.putClientProperty("JTree.lineStyle", "Angled");
      mDefPropTree.getSelectionModel().setSelectionMode(
                              TreeSelectionModel.SINGLE_TREE_SELECTION);
      mDefPropTree.addTreeSelectionListener(new TreeSelectionListener()
      {
         public void valueChanged(TreeSelectionEvent evt)
         {
            DefaultMutableTreeNode node = (DefaultMutableTreeNode)
                           mDefPropTree.getLastSelectedPathComponent();

            // Show basic help if nothing is selected
            if (node == null)
            {
               // Disable the add/remove buttons
               mAddBtn.setEnabled(false);
               mRemoveBtn.setEnabled(false);
               
               updateListeners(null);
               checkForChanges(null);

               // TODO: Show the basic help
               mEditorPaneLayout.show(mEditorPane, EMPTY);
               return;
            }

            // Edit an entire config definition
            else if (node.getUserObject() instanceof ConfigDefinition)
            {
               mAddBtn.setEnabled(true);
               mRemoveBtn.setEnabled(true);
               
               updateListeners(node.getUserObject());

               ConfigDefinition def = (ConfigDefinition)node.getUserObject();
               checkForChanges(def);
               mEditorPaneLayout.show(mEditorPane, CONFIG_DEFINITION);
               return;
            }
            // Edit a specific property definition
            else if (node.getUserObject() instanceof PropertyDefinition)
            {
               mAddBtn.setEnabled(false);
               mRemoveBtn.setEnabled(true);
               
               updateListeners(node.getUserObject());

               // Get the first ancestor that is a ConfigDefinition.
               TreePath path = mDefPropTree.getSelectionPath();
               int index = path.getPathCount() - 2;
               DefaultMutableTreeNode def_node =
                  (DefaultMutableTreeNode)path.getPathComponent(index);
               ConfigDefinition config_def = (ConfigDefinition)def_node.getUserObject();

               checkForChanges(config_def);
               
               PropertyDefinition prop_def = (PropertyDefinition)node.getUserObject();
               mPropertyDefEditor.setPropertyDefinition(prop_def);
               mEditorPaneLayout.show(mEditorPane, PROPERTY_DEFINITION);
               return;
            }
            // Edit a category
            else if (node.getUserObject() instanceof Category)
            {
               mAddBtn.setEnabled(true);
               mRemoveBtn.setEnabled(false);
               
               updateListeners(node.getUserObject());
               checkForChanges(null);

               mEditorPaneLayout.show(mEditorPane, EMPTY);
               return;
            }
         }
      });
   }
   
   private ConfigDefinition mConfigDef = null;
   private PropertyDefinition mPropDef = null;
   private ConfigDefinitionListener cdl = null; 
   private PropertyDefinitionListener pdl = null;
   private boolean mDefinitionChanged = false;

   private void checkForChanges(ConfigDefinition config_def)
   {
      if(config_def != mDefEditor.getDefinition())
      {
         if(mDefinitionChanged)
         {
            int result = JOptionPane.showConfirmDialog(this,
               "ConfigDefinition \"" + mDefEditor.getDefinition().getName() +
               "\" has changed. Do you want to save the changes?", "VRJConfig",
               JOptionPane.YES_NO_OPTION);
            
            if(JOptionPane.YES_OPTION == result)
            {
               (new ConfigBrokerProxy()).saveDefinition(mDefEditor.getDefinition().getToken());
            }
            
                     
            mDefinitionChanged = false;
         }
         mDefEditor.setDefinition(config_def);
      }
   }
   private void setupListeners()
   {
      cdl = new ConfigDefinitionListener()
         {
            public void nameChanged(ConfigDefinitionEvent evt)
            {
               System.out.println("Config Definition Changed..." +
                     ((ConfigDefinition)evt.getSource()).getToken() +
                     " " + mConfigDef.getToken());
            }
            public void tokenChanged(ConfigDefinitionEvent evt)
            {
               System.out.println("ERROR: Can not change token..." +
                     ((ConfigDefinition)evt.getSource()).getToken() +
                     " " + mConfigDef.getToken());
            }
            public void helpChanged(ConfigDefinitionEvent evt)
            {
               mDefinitionChanged = true;

               System.out.println("Config Definition Help Changed..." +
                     ((ConfigDefinition)evt.getSource()).getToken() +
                     " " + mConfigDef.getToken());
            }
            public void categoryAdded(ConfigDefinitionEvent evt)
            {
               mDefinitionChanged = true;

               System.out.println("Config Definition Category Changed..." +
                     ((ConfigDefinition)evt.getSource()).getToken() +
                     " " + mConfigDef.getToken());
            }
            public void categoryRemoved(ConfigDefinitionEvent evt)
            {
               mDefinitionChanged = true;

               System.out.println("Config Definition Category Changed..." +
                     ((ConfigDefinition)evt.getSource()).getToken() +
                     " " + mConfigDef.getToken());
            }
            public void propertyDefinitionAdded(ConfigDefinitionEvent evt)
            {
               mDefinitionChanged = true;

               System.out.println("Config Definition Prop Definition Added..." +
                     ((ConfigDefinition)evt.getSource()).getToken() +
                     " " + mConfigDef.getToken());
            }
            public void propertyDefinitionRemoved(ConfigDefinitionEvent evt)
            {
               mDefinitionChanged = true;

               System.out.println("Config Definition Prop Definition Removed..." +
                     ((ConfigDefinition)evt.getSource()).getToken() +
                     " " + mConfigDef.getToken());
            }
         };
      pdl = new PropertyDefinitionListener()
         {
            public void nameChanged(PropertyDefinitionEvent evt)
            {
               mDefinitionChanged = true;

               System.out.println("Property Definition Changed..." +
                     " " + mConfigDef.getToken());
            }
            public void tokenChanged(PropertyDefinitionEvent evt)
            {
               mDefinitionChanged = true;

               System.out.println("Property Definition Changed..." +
                     " " + mConfigDef.getToken());
            }
            public void typeChanged(PropertyDefinitionEvent evt)
            {
               mDefinitionChanged = true;

               System.out.println("Property Definition Changed..." +
                     " " + mConfigDef.getToken());
            }
            public void helpChanged(PropertyDefinitionEvent evt)
            {
               mDefinitionChanged = true;

               System.out.println("Property Definition Changed..." +
                     " " + mConfigDef.getToken());
            }
            public void propertyValueDefinitionAdded(PropertyDefinitionEvent evt)
            {
               mDefinitionChanged = true;

               System.out.println("Property Definition Changed..." +
                     " " + mConfigDef.getToken());
            }
            public void propertyValueDefinitionRemoved(PropertyDefinitionEvent evt)
            {
               mDefinitionChanged = true;

               System.out.println("Property Definition Changed..." +
                     " " + mConfigDef.getToken());
            }
            public void enumAdded(PropertyDefinitionEvent evt)
            {
               mDefinitionChanged = true;

               System.out.println("Property Definition Changed..." +
                     " " + mConfigDef.getToken());
            }
            public void enumRemoved(PropertyDefinitionEvent evt)
            {
               mDefinitionChanged = true;

               System.out.println("Property Definition Changed..." +
                     " " + mConfigDef.getToken());
            }
            public void allowedTypeAdded(PropertyDefinitionEvent evt)
            {
               mDefinitionChanged = true;

               System.out.println("Property Definition Changed..." +
                     " " + mConfigDef.getToken());
            }
            public void allowedTypeRemoved(PropertyDefinitionEvent evt)
            {
               mDefinitionChanged = true;

               System.out.println("Property Definition Changed..." +
                     " " + mConfigDef.getToken());
            }
            public void variableChanged(PropertyDefinitionEvent evt)
            {
               mDefinitionChanged = true;

               System.out.println("Property Definition Changed..." +
                     " " + mConfigDef.getToken());
            }
         };

   }
   private void updateListeners(Object value)
   {
      DefaultMutableTreeNode node = (DefaultMutableTreeNode)
                     mDefPropTree.getLastSelectedPathComponent();

      if(null != mConfigDef)
      {
         mConfigDef.removeConfigDefinitionListener(cdl);
      }
      if(null != mPropDef)
      {
         mPropDef.removePropertyDefinitionListener(pdl);
      }

      if(null != value && value instanceof ConfigDefinition)
      {
         mConfigDef = (ConfigDefinition)value;
         mConfigDef.addConfigDefinitionListener(cdl);
      }
      if(null != value && value instanceof PropertyDefinition)
      {
         TreePath path = mDefPropTree.getSelectionPath();
         int index = path.getPathCount() - 2;
         DefaultMutableTreeNode def_node =
            (DefaultMutableTreeNode)path.getPathComponent(index);
         mConfigDef = (ConfigDefinition)def_node.getUserObject();
         mConfigDef.addConfigDefinitionListener(cdl);
         mPropDef = (PropertyDefinition)value;
         mPropDef.addPropertyDefinitionListener(pdl);
      }

   }
   

   /**
    * Sets the repository that should be edited.
    */
   public void setRepository(ConfigDefinitionRepository repos)
   {
      mReposModel.setRepository(repos);
   }

   /**
    * Gets the repository that this editor is using.
    */
   public ConfigDefinitionRepository getRepository()
   {
      return mReposModel.getRepository();
   }

   /**
    * Called when the user has clicked the generic add button positioned above
    * the tree.
    */
   protected void addAction(ActionEvent evt)
   {
      DefaultMutableTreeNode node = (DefaultMutableTreeNode)
                     mDefPropTree.getLastSelectedPathComponent();
      // Check if the user wants to add a new config definition
      if (node.getUserObject() instanceof Category)
      {
         java.util.List categories = new ArrayList();
         categories.add(node.getUserObject());
         ConfigDefinition def = new ConfigDefinition("Untitled",
                                                     "untitled",
                                                     "",
                                                     1,
                                                     new ArrayList(),
                                                     "",
                                                     categories,
                                                     new ArrayList(),
                                                     null);
         getRepository().add(def);
      }
      // Check if the user wants to add a new property definition
      else if (node.getUserObject() instanceof ConfigDefinition)
      {
         PropertyDefinition prop_def = new PropertyDefinition("Untitled",
                                                              "untitled",
                                                              String.class,
                                                              "",
                                                              new ArrayList(),
                                                              new TreeMap(),
                                                              new ArrayList(),
                                                              false);
         ConfigDefinition def = (ConfigDefinition)node.getUserObject();
         def.addPropertyDefinition(prop_def);
      }
   }

   /**
    * Called when the user has clicked the generic remove button positioned
    * above the tree.
    */
   protected void removeAction(ActionEvent evt)
   {
      DefaultMutableTreeNode node = (DefaultMutableTreeNode)
                     mDefPropTree.getLastSelectedPathComponent();
      // Check if the user wants to remove a config definition
      if (node.getUserObject() instanceof ConfigDefinition)
      {
         ConfigDefinition def = (ConfigDefinition)node.getUserObject();
//XXX         getRepository().remove(def);
      }
      // Check if the user wants to remove a property definition
      else if (node.getUserObject() instanceof PropertyDefinition)
      {
         DefaultMutableTreeNode parent = (DefaultMutableTreeNode)node.getParent();
         ConfigDefinition def = (ConfigDefinition)parent.getUserObject();
         PropertyDefinition prop_def = (PropertyDefinition)node.getUserObject();
         def.removePropertyDefinition(prop_def);
      }
   }

   /**
    * Builds up the GUI elements. This method is auto-generated by JBuilder.
    */
   private void jbInit()
      throws Exception
   {
      this.setLayout(mBaseLayout);
      mBaseSplitPane.setLeftComponent(mTreeScrollPane);
      mTreePane.setLayout(mTreeLayout);
      mTreeScrollPane.setViewportView(mDefPropTree);
      mTreeToolbar.setFloatable(false);
      mAddBtn.setText("Add");
      mAddBtn.setEnabled(false);
      mRemoveBtn.setText("Remove");
      mRemoveBtn.setEnabled(false);
      mEditorPane.setLayout(mEditorPaneLayout);
      mEditorPane.setMinimumSize(new Dimension(0, 0));
      mAddBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            addAction(evt);
         }
      });
      mRemoveBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            removeAction(evt);
         }
      });
      this.add(mBaseSplitPane, BorderLayout.CENTER);
      mBaseSplitPane.add(mTreePane, JSplitPane.LEFT);
      mBaseSplitPane.add(mEditorPane, JSplitPane.RIGHT);
      mTreePane.add(mTreeToolbar, BorderLayout.NORTH);
      mTreePane.add(mTreeScrollPane, BorderLayout.CENTER);
      mTreeToolbar.add(mAddBtn);
      mTreeToolbar.add(mRemoveBtn);
   }

   /**
    * The data model for the repository being edited.
    */
   private ConfigDefinitionRepositoryModel mReposModel;

   /**
    * The overall editor for the selected configuration definition.
    */
   private ConfigDefinitionEditor mDefEditor = new ConfigDefinitionEditor();

   /**
    * ScrollPane for the configuration definition property sheet.
    */
   private JScrollPane mDefEditorScrollPane = new JScrollPane();

   /**
    * The editor for property definition editing pane.
    */
   private PropertyDefinitionEditor mPropertyDefEditor = new PropertyDefinitionEditor();

   private BorderLayout mBaseLayout = new BorderLayout();
   private JSplitPane mBaseSplitPane = new JSplitPane();
   private BorderLayout mTreeLayout = new BorderLayout();
   private JPanel mTreePane = new JPanel();
   private JToolBar mTreeToolbar = new JToolBar();
   private JButton mAddBtn = new JButton();
   private JButton mRemoveBtn = new JButton();
   private JScrollPane mTreeScrollPane = new JScrollPane();
   private JTree mDefPropTree = new JTree();
   private JPanel mEditorPane = new JPanel();
   private CardLayout mEditorPaneLayout = new CardLayout();
   private JPanel mEmptyPane = new JPanel();

   /**
    * Specialized renderer for a config definition tree.
    */
   class DefinitionRepositoryTreeCellRenderer
      extends DefaultTreeCellRenderer
   {
      public DefinitionRepositoryTreeCellRenderer()
      {
         try
         {
            // Get all the icons we'll ever need now so we can reuse them later
            ClassLoader loader = getClass().getClassLoader();
            mConfigDefIcon = new ImageIcon(loader.getResource(
                                       "org/vrjuggler/jccl/editors/images/configchunk.gif"));
            mCategoryIcon = new ImageIcon(loader.getResource(
                                       "org/vrjuggler/jccl/editors/images/category.gif"));
         }
         catch (NullPointerException npe)
         {
            // Images aren't available ... oh well
         }
      }

      public Component getTreeCellRendererComponent(
                           JTree tree, Object node, boolean selected,
                           boolean expanded, boolean leaf, int row,
                           boolean focused)
      {
         // Get the default settings from the UI LAF
         super.getTreeCellRendererComponent(tree, node, selected,
                                            expanded, leaf, row, focused);

         // Pull out the data from the tree node
         Object value = ((DefaultMutableTreeNode)node).getUserObject();

         // Display the name of configuration definitions
         if (value instanceof ConfigDefinition)
         {
            setText(((ConfigDefinition)value).getName());
            setFont(null);
            setIcon(mConfigDefIcon);
         }
         // Display the name of a category
         else if (value instanceof Category)
         {
            setText(((Category)value).getName());
            setFont(tree.getFont().deriveFont(Font.BOLD));
            setIcon(null);
         }
         // Display the name of property definitions
         else if (value instanceof PropertyDefinition)
         {
            setText(((PropertyDefinition)value).getName());
            setFont(tree.getFont().deriveFont(Font.ITALIC));
         }

         return this;
      }

      private Icon mConfigDefIcon = null;
      private Icon mCategoryIcon = null;
   }
}
