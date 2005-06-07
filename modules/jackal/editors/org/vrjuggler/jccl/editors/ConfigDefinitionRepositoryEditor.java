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
import org.vrjuggler.jccl.config.io.ConfigDefinitionWriter;


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
   
   private ConfigDefinition mCurConfigDef = null;
   private ConfigDefinition mOrigConfigDef = null;
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
            ConfigDefinition def = mDefEditor.getDefinition();
            int result = JOptionPane.showConfirmDialog(
               SwingUtilities.getAncestorOfClass(Frame.class, this),
               "ConfigDefinition \"" + def.getName() +
               "\" has changed. Do you want to save the changes?",
               "Config Definition Editor", JOptionPane.YES_NO_OPTION);

            if(JOptionPane.YES_OPTION == result)
            {
               try
               {
                  (new ConfigBrokerProxy()).saveDefinition(def);
               }
               catch(Exception ex)
               {
                  JOptionPane.showMessageDialog(this,
                     "ERROR: Failed to save definition '" + def.getName() +
                        "' version " + def.getVersion() + ": " +
                        ex.getMessage() + "!",
                     "Definition Save Failure", JOptionPane.ERROR_MESSAGE);
               }
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
               mDefinitionChanged = true;

               System.out.println("Config Definition Name Changed: " +
                     evt.getValue() + " --> " + mCurConfigDef.getName());
            }
            public void tokenChanged(ConfigDefinitionEvent evt)
            {
               JOptionPane.showMessageDialog(null,
                                             "Changing the token name is not currently supported",
                                             "Config Definition Editor Error",
                                             JOptionPane.ERROR_MESSAGE);
            }
            public void abstractChanged(ConfigDefinitionEvent evt)
            {
               mDefinitionChanged = true;

               System.out.println("Config Definition Abstract Changed..." +
                     ((ConfigDefinition)evt.getSource()).getToken() +
                     " " + mCurConfigDef.getToken());
            }
            public void helpChanged(ConfigDefinitionEvent evt)
            {
               mDefinitionChanged = true;

               System.out.println("Config Definition Help Changed..." +
                     ((ConfigDefinition)evt.getSource()).getToken() +
                     " " + mCurConfigDef.getToken());
            }
            public void parentAdded(ConfigDefinitionEvent evt)
            {
               mDefinitionChanged = true;

               System.out.println("Config Definition Parent Changed..." +
                     ((ConfigDefinition)evt.getSource()).getToken() +
                     " " + mCurConfigDef.getToken());
            }
            public void parentRemoved(ConfigDefinitionEvent evt)
            {
               mDefinitionChanged = true;

               System.out.println("Config Definition Parent Changed..." +
                     ((ConfigDefinition)evt.getSource()).getToken() +
                     " " + mCurConfigDef.getToken());
            }
            public void categoryAdded(ConfigDefinitionEvent evt)
            {
               mDefinitionChanged = true;

               System.out.println("Config Definition Category Changed..." +
                     ((ConfigDefinition)evt.getSource()).getToken() +
                     " " + mCurConfigDef.getToken());
            }
            public void categoryRemoved(ConfigDefinitionEvent evt)
            {
               mDefinitionChanged = true;

               System.out.println("Config Definition Category Changed..." +
                     ((ConfigDefinition)evt.getSource()).getToken() +
                     " " + mCurConfigDef.getToken());
            }
            public void propertyDefinitionAdded(ConfigDefinitionEvent evt)
            {
               mDefinitionChanged = true;

               System.out.println("Config Definition Prop Definition Added..." +
                     ((ConfigDefinition)evt.getSource()).getToken() +
                     " " + mCurConfigDef.getToken());
            }
            public void propertyDefinitionRemoved(ConfigDefinitionEvent evt)
            {
               mDefinitionChanged = true;

               System.out.println("Config Definition Prop Definition Removed..." +
                     ((ConfigDefinition)evt.getSource()).getToken() +
                     " " + mCurConfigDef.getToken());
            }
         };
      pdl = new PropertyDefinitionListener()
         {
            public void tokenChanged(PropertyDefinitionEvent evt)
            {
               mDefinitionChanged = true;

               System.out.println("Property Definition Changed..." +
                     " " + mCurConfigDef.getToken());
            }
            public void typeChanged(PropertyDefinitionEvent evt)
            {
               mDefinitionChanged = true;

               System.out.println("Property Definition Changed..." +
                     " " + mCurConfigDef.getToken());
            }
            public void helpChanged(PropertyDefinitionEvent evt)
            {
               mDefinitionChanged = true;

               System.out.println("Property Definition Changed..." +
                     " " + mCurConfigDef.getToken());
            }
            public void propertyValueDefinitionAdded(PropertyDefinitionEvent evt)
            {
               mDefinitionChanged = true;

               System.out.println("Property Definition Changed..." +
                     " " + mCurConfigDef.getToken());
            }
            public void propertyValueDefinitionRemoved(PropertyDefinitionEvent evt)
            {
               mDefinitionChanged = true;

               System.out.println("Property Definition Changed..." +
                     " " + mCurConfigDef.getToken());
            }
            public void enumEditableChanged(PropertyDefinitionEvent evt)
            {
               mDefinitionChanged = true;
            }
            public void enumAdded(PropertyDefinitionEvent evt)
            {
               mDefinitionChanged = true;

               System.out.println("Property Definition Changed..." +
                     " " + mCurConfigDef.getToken());
            }
            public void enumRemoved(PropertyDefinitionEvent evt)
            {
               mDefinitionChanged = true;

               System.out.println("Property Definition Changed..." +
                     " " + mCurConfigDef.getToken());
            }
            public void allowedTypeAdded(PropertyDefinitionEvent evt)
            {
               mDefinitionChanged = true;

               System.out.println("Property Definition Changed..." +
                     " " + mCurConfigDef.getToken());
            }
            public void allowedTypeRemoved(PropertyDefinitionEvent evt)
            {
               mDefinitionChanged = true;

               System.out.println("Property Definition Changed..." +
                     " " + mCurConfigDef.getToken());
            }
            public void variableChanged(PropertyDefinitionEvent evt)
            {
               mDefinitionChanged = true;

               System.out.println("Property Definition Changed..." +
                     " " + mCurConfigDef.getToken());
            }
         };

   }

   private void updateListeners(Object value)
   {
      DefaultMutableTreeNode node = (DefaultMutableTreeNode)
                     mDefPropTree.getLastSelectedPathComponent();

      if(null != mCurConfigDef)
      {
         mCurConfigDef.removeConfigDefinitionListener(cdl);
      }
      if(null != mPropDef)
      {
         mPropDef.removePropertyDefinitionListener(pdl);
      }

      if(null != value && value instanceof ConfigDefinition)
      {
         mCurConfigDef = (ConfigDefinition)value;
         mCurConfigDef.addConfigDefinitionListener(cdl);

         // Only change mOrigConfigDef if is not already set or we have
         // changed definition types (not just versions of the same type).
         // This is needed so that we can determine if and when a new version
         // of the definition stored in mOrigConfigDef is required.
         if ( mOrigConfigDef == null ||
              ! mOrigConfigDef.getToken().equals(mCurConfigDef.getToken()) )
         {
            mOrigConfigDef = mCurConfigDef;
         }
      }
      if(null != value && value instanceof PropertyDefinition)
      {
         TreePath path = mDefPropTree.getSelectionPath();
         int index = path.getPathCount() - 2;
         DefaultMutableTreeNode def_node =
            (DefaultMutableTreeNode)path.getPathComponent(index);
         mCurConfigDef = (ConfigDefinition)def_node.getUserObject();
         mCurConfigDef.addConfigDefinitionListener(cdl);
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
         ConfigBroker broker = new ConfigBrokerProxy();
         String new_def_name =
            JOptionPane.showInputDialog(this,
                                        "Enter the name for the new definition");

         java.util.List categories = new ArrayList();
         categories.add(node.getUserObject());

         String token =
            new_def_name.trim().toLowerCase().replaceAll("\\s+", "_");
         ConfigDefinition def = new ConfigDefinition(new_def_name,
                                                     token,
                                                     "",
                                                     1,
                                                     false,
                                                     new ArrayList(),
                                                     "",
                                                     categories,
                                                     new ArrayList(),
                                                     null);

         try
         {
            broker.saveDefinition(def);

            // Only store the new definition in the repository if it was
            // saved successfully.
            getRepository().add(def);
         }
         catch(Exception ex)
         {
            JOptionPane.showMessageDialog(this,
                                          "Failed to store new definition: " + 
                                             ex.getMessage(),
                                          "Config Definition Editor Error",
                                          JOptionPane.ERROR_MESSAGE);
         }
      }
      // Check if the user wants to add a new property definition
      else if (node.getUserObject() instanceof ConfigDefinition)
      {
         PropertyDefinition prop_def = new PropertyDefinition("untitled",
                                                              String.class,
                                                              "",
                                                              new ArrayList(),
                                                              false,
                                                              new TreeMap(),
                                                              new ArrayList(),
                                                              false);
         ConfigDefinition def = (ConfigDefinition)node.getUserObject();

         if ( isNewVersionNeeded(def) )
         {
            def = addNewDefinitionVersion(def);

            // This is needed to prevent checkForChanges() from thinking that
            // we need to save the new definition version prematurely.
            // XXX: This feels like a hack.  -PH 11/12/2004
            mDefEditor.setDefinition(def);
         }

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

         if ( isNewVersionNeeded(def) )
         {
            def = addNewDefinitionVersion(def);

            // This is needed to prevent checkForChanges() from thinking that
            // we need to save the new definition version prematurely.
            // XXX: This feels like a hack.  -PH 11/12/2004
            mDefEditor.setDefinition(def);
         }

         def.removePropertyDefinition(prop_def);
      }
   }

   protected boolean isNewVersionNeeded(ConfigDefinition def)
   {
      // If we are already working with a version newer than what we started
      // with, we do not need to make another new version.
      if ( def.getVersion() > mOrigConfigDef.getVersion() )
      {
         return false;
      }
      // If we started with an empty config definition, we do not need to
      // create a new version.
      else if ( mOrigConfigDef.getVersion() == 1 &&
                mOrigConfigDef.getPropertyDefinitions().size() == 0 )
      {
         return false;
      }
      // At this point, we know that def and mOrigConfigDef are the same
      // version and that mOrigConfigDef may still be a new, unsaved
      // definition having more than zero property definitions.  This tests
      // to see if mOrigConfigDef and def are the same object and we simply
      // have unsaved changes.  In this case, we are working on a new
      // definition that has not had any version saved yet.
      else if ( mOrigConfigDef == def && mDefinitionChanged )
      {
         return false;
      }

      // Otherwise, we need to make a new version.
      return true;
   }

   protected ConfigDefinition addNewDefinitionVersion(ConfigDefinition oldDef)
   {
      System.out.println("Creating a new definition version (" +
                         (oldDef.getVersion() + 1) + ")");
      ConfigDefinition new_def = makeNewDefinitionVersion(oldDef);
      System.out.println("new_def => " + new_def);
      getRepository().add(new_def);
      updateListeners(new_def);

      return new_def;
   }

   protected ConfigDefinition makeNewDefinitionVersion(ConfigDefinition oldDef)
   {
      return new ConfigDefinition(oldDef.getName(), oldDef.getToken(),
                                  oldDef.getIconLocation(),
                                  oldDef.getVersion() + 1,
                                  oldDef.isAbstract(),
                                  oldDef.getParents(),
                                  oldDef.getHelp(), oldDef.getCategories(),
                                  oldDef.getPropertyDefinitions(), null);
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
            setText(((PropertyDefinition)value).getToken());
            setFont(tree.getFont().deriveFont(Font.ITALIC));
         }

         return this;
      }

      private Icon mConfigDefIcon = null;
      private Icon mCategoryIcon = null;
   }
}
