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
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.Iterator;
import java.util.List;
import java.util.StringTokenizer;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.tree.*;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.tweek.beans.loader.BeanJarClassLoader;

/**
 * A specialized panel that knows how to edit ChunkDescs within a
 * ChunkDescDB.
 */
public class ChunkDescDBEditor
   extends JPanel
   implements PropertyChangeListener
            , ChunkDescListener
            , ContextListener
{
   /**
    * The ID used to reference the ChunkDesc editor panel in the CardLayout
    * for the editorPane.
    */
   final static String CHUNK_DESC      = "ChunkDescPane";

   /**
    * The ID used to reference the PropertyDesc editor panel in the CardLayout
    * for the editorPane.
    */
   final static String PROPERTY_DESC   = "PropertyDescPane";

   final static String EMPTY           = "Empty";

   /**
    * Creates a new ChunkDescDBEditor with no desc to edit.
    */
   public ChunkDescDBEditor()
   {
      try
      {
         jbInit();
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }

      // Hack around JSplitPane bug where it can't handle setting the divider
      // location until it's visible. JDC Bug #4182558.
      propsSplitPane.addComponentListener(new ComponentAdapter()
      {
         public void componentResized(ComponentEvent evt)
         {
            propsSplitPane.setDividerLocation(0.80);
            ChunkDescDBEditor.this.propsSplitPane.removeComponentListener(this);
         }
      });

      // Init the ChunkDesc editor pane
      descEditorScrollPane.setViewportView(descEditor);
      descEditorScrollPane.setMinimumSize(new Dimension(0, 0));
      descEditorScrollPane.setPreferredSize(descEditor.getPreferredSize());
      editorPane.add(descEditorScrollPane, CHUNK_DESC);

      // Init the PropertyDesc editor pane
      editorPane.add(propertyDescEditor, PROPERTY_DESC);

      // Init the empty editor pane
      editorPane.add(emptyPane, EMPTY);
      editorPaneLayout.show(editorPane, EMPTY);

      // Init the ChunkDescDB tree
      DefaultMutableTreeNode root = new DefaultMutableTreeNode("Desc");
      treeModel = new DefaultTreeModel(root);
      descPropTree.setModel(treeModel);
//      descPropTree.setEditable(true);
      descPropTree.setCellRenderer(new ChunkDescDBTreeCellRenderer());
      descPropTree.putClientProperty("JTree.lineStyle", "Angled");
      descPropTree.getSelectionModel().setSelectionMode(
                              TreeSelectionModel.SINGLE_TREE_SELECTION);
      descPropTree.addTreeSelectionListener(new TreeSelectionListener()
      {
         public void valueChanged(TreeSelectionEvent evt)
         {
            DefaultMutableTreeNode node = (DefaultMutableTreeNode)
                           descPropTree.getLastSelectedPathComponent();
            // Show basic help if nothing is selected
            if (node == null)
            {
               // Disable the add/remove buttons
               addBtn.setEnabled(false);
               removeBtn.setEnabled(false);

               // TODO: Show the basic help
               editorPaneLayout.show(editorPane, EMPTY);
               return;
            }

            // Edit an entire desc
            else if (node.getUserObject() instanceof ChunkDesc)
            {
               addBtn.setEnabled(true);
               removeBtn.setEnabled(true);

               ChunkDesc desc = (ChunkDesc)node.getUserObject();
               descEditor.setDefinition(desc);
               editorPaneLayout.show(editorPane, CHUNK_DESC);
               return;
            }
            // Edit a specific property desc
            else if (node.getUserObject() instanceof PropertyDesc)
            {
               addBtn.setEnabled(false);
               removeBtn.setEnabled(true);

               PropertyDesc desc = (PropertyDesc)node.getUserObject();
               propertyDescEditor.setPropertyDesc(desc);
               editorPaneLayout.show(editorPane, PROPERTY_DESC);
               return;
            }
            // Edit a category
            else if (node.getUserObject() instanceof String)
            {
               addBtn.setEnabled(true);
               removeBtn.setEnabled(false);

               editorPaneLayout.show(editorPane, EMPTY);
               return;
            }
         }
      });
   }

   /**
    * Sets the context in which this panel is editing descriptions.
    */
   public void setConfigContext(ConfigContext context)
   {
      if (this.context != null)
      {
         this.context.removeContextListener(this);
         getConfigBroker().removeConfigListener(configListener);
      }
      this.context = context;
      if (this.context != null)
      {
         this.context.addContextListener(this);
         getConfigBroker().addConfigListener(configListener);
      }

      // rebuild the tree
      rebuildTree();
   }

   /**
    * Gets the context that this panel is using to edit descriptions.
    */
   public ConfigContext getConfigContext()
   {
      return context;
   }

   /**
    * Called by the current context when a resource has been added to it.
    */
   public void resourceAdded(ContextEvent evt)
   {
      String resource = evt.getResource();
      addDescs(getConfigBroker().getDescsIn(resource));
   }

   /**
    * Called by the current context when a resource has been removed from it.
    */
   public void resourceRemoved(ContextEvent evt)
   {
      String resource = evt.getResource();
      removeDescs(getConfigBroker().getDescsIn(resource));
   }

   /**
    * Called by a ChunkDesc when its name has been changed.
    */
   public void nameChanged(ChunkDescEvent evt)
   {
      ChunkDesc src = (ChunkDesc)evt.getSource();
      java.util.List desc_nodes = getNodesFor(src);
      for (Iterator itr = desc_nodes.iterator(); itr.hasNext(); )
      {
         // Let the tree know that the node has changed
         treeModel.nodeChanged((TreeNode)itr.next());
      }
   }

   /**
    * Called by a ChunkDesc when its token has been changed.
    */
   public void tokenChanged(ChunkDescEvent evt)
   {
   }

   /**
    * Called by a ChunkDesc when its help text has changed.
    */
   public void helpChanged(ChunkDescEvent evt)
   {
   }

   /**
    * Called by a ChunkDesc when it has added a category that it belongs to.
    */
   public void categoryAdded(ChunkDescEvent evt)
   {
      ChunkDesc src = (ChunkDesc)evt.getSource();
      String category = (String)evt.getValue();

      // Add the desc to the new category
      DefaultMutableTreeNode categoryNode = getCategoryNode(category);
      addDescTo(categoryNode, src);
   }

   /**
    * Called by a ChunkDesc when it has removed a category that it belongs to.
    */
   public void categoryRemoved(ChunkDescEvent evt)
   {
      ChunkDesc src = (ChunkDesc)evt.getSource();
      String category = (String)evt.getValue();

      java.util.List desc_nodes = getNodesFor(src);
      for (Iterator itr = desc_nodes.iterator(); itr.hasNext(); )
      {
         DefaultMutableTreeNode node = (DefaultMutableTreeNode)itr.next();
         DefaultMutableTreeNode parent_node = (DefaultMutableTreeNode)node.getParent();

         // Check if we found the node that's in the search category
         if (parent_node.getUserObject().equals(category))
         {
            // Remove the desc node from its parent. In this case, the tree model will
            // automatically fire off the removed event for us.
            treeModel.removeNodeFromParent(node);
            break;
         }
      }
   }

   /**
    * Called by a ChunkDesc when one of its properties has been modified.
    */
   public void propertyDescChanged(ChunkDescEvent evt)
   {
      ChunkDesc src = (ChunkDesc)evt.getSource();
      PropertyDesc prop_desc = evt.getPropertyDesc();
      java.util.List desc_nodes = getNodesFor(prop_desc);
      for (Iterator itr = desc_nodes.iterator(); itr.hasNext(); )
      {
         // Let the tree know that the node has changed
         treeModel.nodeChanged((TreeNode)itr.next());
      }
   }

   /**
    * Called by a ChunkDesc when a new property desc has been added to it.
    */
   public void propertyDescAdded(ChunkDescEvent evt)
   {
      ChunkDesc src = (ChunkDesc)evt.getSource();
      PropertyDesc prop_desc = evt.getPropertyDesc();
      java.util.List desc_nodes = getNodesFor(src);
      for (Iterator itr = desc_nodes.iterator(); itr.hasNext(); )
      {
         // Add the property desc node as a child of the chunk desc node
         DefaultMutableTreeNode desc_node = (DefaultMutableTreeNode)itr.next();
         DefaultMutableTreeNode prop_node = new DefaultMutableTreeNode(prop_desc);
         addNode(desc_node, prop_node);
      }
   }

   /**
    * Called by a ChunkDesc when a property desc has been removed from it.
    */
   public void propertyDescRemoved(ChunkDescEvent evt)
   {
      ChunkDesc src = (ChunkDesc)evt.getSource();
      PropertyDesc prop_desc = evt.getPropertyDesc();
      java.util.List prop_nodes = getNodesFor(prop_desc);
      for (Iterator itr = prop_nodes.iterator(); itr.hasNext(); )
      {
         // Remove the property desc node from its parent. In this case, the
         // tree model will automatically fire off the removed event for us.
         DefaultMutableTreeNode prop_node = (DefaultMutableTreeNode)itr.next();
         treeModel.removeNodeFromParent(prop_node);
      }
   }

   /**
    * This gets called whenver one of the ChunkDescs in the ChunkDescDB we are
    * editing has been modified.
    */
   public void propertyChange(PropertyChangeEvent evt)
   {
      System.out.println("ChunkDescDBEditor.propertyChange for property: "+evt.getPropertyName()+" from desc "+((ChunkDesc)evt.getSource()).getName());
      ChunkDesc src = (ChunkDesc)evt.getSource();
      java.util.List desc_nodes = getNodesFor(src);
      System.out.println("\tFound "+desc_nodes.size()+" matches");
      for (Iterator itr = getNodesFor(src).iterator(); itr.hasNext(); )
      {
         TreeNode desc_node = (TreeNode)itr.next();
         // Look through the desc's children and find the node that changed
         for (Enumeration e = desc_node.children(); e.hasMoreElements(); )
         {
            DefaultMutableTreeNode child = (DefaultMutableTreeNode)e.nextElement();
            if (child.getUserObject() instanceof PropertyDesc)
            {
               PropertyDesc prop_desc = (PropertyDesc)child.getUserObject();
               System.out.println("\tToken: "+prop_desc.getToken());
               if (prop_desc.getToken().equals(evt.getPropertyName()))
               {
                  System.out.println("\tUpdating property for prop with token "+prop_desc.getToken());
                  treeModel.nodeChanged(child);
               }
            }
         }
      }
   }

   /**
    * Gets a list of all the tree nodes that contain the given Object.
    */
   protected java.util.List getNodesFor(Object obj)
   {
      return getNodesFor(obj, (DefaultMutableTreeNode)treeModel.getRoot());
   }

   /**
    * Recursive helper for getNodesFor(Object).
    *
    * @see #getNodesFor(Object)
    */
   private java.util.List getNodesFor(Object obj, DefaultMutableTreeNode node)
   {
      java.util.List results = new ArrayList();

      // Check if we found a match
      if (node.getUserObject().equals(obj))
      {
            results.add(node);
      }

      // Check all children of the current node
      for (Enumeration e = node.children(); e.hasMoreElements(); )
      {
         DefaultMutableTreeNode child = (DefaultMutableTreeNode)e.nextElement();
         results.addAll(getNodesFor(obj, child));
      }

      return results;
   }

   /**
    * Displays the help for the given property description.
    */
   protected void displayHelp(PropertyDesc desc)
   {
      // Make sure desc is not null
      if (desc != null)
      {
         helpPane.setText(desc.getHelp());
      }
      else
      {
         // Default to an empty help window
         helpPane.setText("");
      }
   }

   /**
    * Rebuilds the chunk desc DB tree. This forces the table model to fire off
    * a nodeStructureChanged event on the root node.
    */
   private void rebuildTree()
   {
      // First clear out all the old nodes from the tree.
      ((DefaultMutableTreeNode)treeModel.getRoot()).removeAllChildren();

      // Run through all chunk descs in the context
      List descs = getConfigBroker().getDescs(context);
      addDescs(descs);
   }

   /**
    * Adds all the given chunk descriptions in the given list into the tree in
    * all of their categories.
    */
   private void addDescs(List descs)
   {
      for (Iterator itr = descs.iterator(); itr.hasNext(); )
      {
         ChunkDesc desc = (ChunkDesc)itr.next();
         addDesc(desc);
      }

      // Make sure the root node is expanded
      TreePath path = new TreePath(treeModel.getPathToRoot((TreeNode)treeModel.getRoot()));
      descPropTree.expandPath(path);
  }

   /**
    * Adds the given chunk description into the tree in all of its categories.
    */
   private void addDesc(ChunkDesc desc)
   {
      // Make ourselves a listener to changes on the desc
      desc.addPropertyChangeListener(this);
      desc.addChunkDescListener(this);

      // Add the desc to each of its categories
      for (Iterator itr = desc.getCategories(); itr.hasNext(); )
      {
         // Get the node for the desc's category
         String category = (String)itr.next();
         DefaultMutableTreeNode categoryNode = getCategoryNode(category);

         // Add the desc to the category
         addDescTo(categoryNode, desc);
      }
   }

   /**
    * Gets the node for the given category, creating nodes for it as needed if
    * they don't exist. A category is heirarchical, meaning that the category
    * string "Devices/Simulator" refers to the "Simulator" category under the
    * "Devices" parent category.
    *
    * @param category      the name of the category for which to get the node
    *
    * @see ChunkDesc.getCategories()
    */
   private DefaultMutableTreeNode getCategoryNode(String category)
   {
      // Initially our best guess is the root node
      DefaultMutableTreeNode catNode = (DefaultMutableTreeNode)treeModel.getRoot();

      // Step through each part of the category looking for its node
      StringTokenizer tokenizer = new StringTokenizer(category, "/");
      while (tokenizer.hasMoreTokens())
      {
         // Get and clean up the next sub-category
         String nextCategory = tokenizer.nextToken();
         nextCategory = nextCategory.trim();

         // Update our best guess reference to the found node
         catNode = getCategoryNode(catNode, nextCategory);
      }

      return catNode;
   }

   /**
    * Gets the node for the given subcategory within the given category. This is
    * a helper method for getCategorynode(String). If the category doesn't
    * exist, it is lazily created and handed back.
    *
    * @param parent     the parent category node
    * @param category   the category to look for
    *
    * @see #getCategoryNode(String)
    */
   private DefaultMutableTreeNode getCategoryNode(DefaultMutableTreeNode parent,
                                                  String category)
   {
      for (Enumeration e = parent.children(); e.hasMoreElements(); )
      {
         DefaultMutableTreeNode node = (DefaultMutableTreeNode)e.nextElement();
         // If the node contains a String, it must be a category
         if (node.getUserObject() instanceof String)
         {
            // Aha! We have a category. Check if it matches the search category
            String catName = (String)node.getUserObject();
            if (catName.equals(category))
            {
               // Match! Return our great find
               return node;
            }
         }
      }

      // Hmm ... no matches. Lazily init the category
      DefaultMutableTreeNode node = new DefaultMutableTreeNode(category);
      addNode(parent, node);
      return node;
   }

   /**
    * Adds a new ChunkDesc to the given parent node. This method does
    * <b>not</b> fire off any events.
    *
    * @param parent     the node that will be the desc's parent
    * @param desc       the ChunkDesc to insert into the tree
    */
   private DefaultMutableTreeNode addDescTo(DefaultMutableTreeNode parent,
                                            ChunkDesc desc)
   {
      // Create the node for the desc and add it to the parent
      DefaultMutableTreeNode descNode = new DefaultMutableTreeNode(desc);
      addNode(parent, descNode);

      // Add all the property descs to the desc node
      for (Iterator prop_itr = desc.getPropertyDescs().iterator(); prop_itr.hasNext(); )
      {
         PropertyDesc prop_desc = (PropertyDesc)prop_itr.next();
         addNode(descNode, new DefaultMutableTreeNode(prop_desc));
      }

      return descNode;
   }

   /**
    * Adds the given child node to the given parent node. It is preferrable to
    * add nodes to the tree this way since order is maintained.
    * <p>
    * <b>This excessively long method needs to be refactored out later with
    * support for pluggable filters/sorters that can be applied to the tree.</b>
    *
    * @param parent     the parent node in which the child will be inserted
    * @param child      the node to insert into the parent
    */
   private void addNode(DefaultMutableTreeNode parent,
                        DefaultMutableTreeNode child)
   {
      Object childData = child.getUserObject();

      // Loop through the children of the node, comparing against them
      int insert_idx = 0;
      while (insert_idx < parent.getChildCount())
      {
         DefaultMutableTreeNode node =
                        (DefaultMutableTreeNode)parent.getChildAt(insert_idx);
         Object data = node.getUserObject();

         // First assume that we need to insert after the current child. If we
         // decide we need to insert ourselves before such child, we'll back up
         // the insertion index.
         ++insert_idx;

         // If child is a category, it needs to be sorted against the other
         // categories, but come before all other nodes
         if (childData instanceof String)
         {
            // Check if the current node is also a category
            String category = (String)childData;
            if (data instanceof String)
            {
               // Does the new category belong before the current category
               String cat = (String)data;
               if (category.compareTo(cat) <= 0)
               {
                  // Insert the new category before the current node
                  --insert_idx;
                  break;
               }
            }
            // If the current node is not a category, category goes before it
            else
            {
               --insert_idx;
               break;
            }
         }

         // If child is a ChunkDesc, it should be sorted against other chunk
         // descs, but come after categories
         else if (childData instanceof ChunkDesc)
         {
            // If the current node is ChunkDesc, we can do our comparision.
            // Otherwise, we just continue through the loop
            ChunkDesc desc = (ChunkDesc)childData;
            if (data instanceof ChunkDesc)
            {
               // Does the new desc belong before the current desc?
               ChunkDesc dsc = (ChunkDesc)data;
               if (desc.getName().compareTo(dsc.getName()) <= 0)
               {
                  // Insert the new desc before the current node
                  --insert_idx;
                  break;
               }
            }
         }

         // If child is a PropertyDesc, it should be sorted against other
         // property descs
         else if (childData instanceof PropertyDesc)
         {
            // If the curent node is a PropertyDesc, we can do our comparison.
            // Otherwise, we jus continue through the loop
            PropertyDesc desc = (PropertyDesc)childData;
            if (data instanceof PropertyDesc)
            {
               // Does the new desc belong before the current desc?
               PropertyDesc dsc = (PropertyDesc)data;
               if (desc.getName().compareTo(dsc.getName()) <= 0)
               {
                  // Insert the new desc before the current node
                  --insert_idx;
                  break;
               }
            }
         }
      }

      treeModel.insertNodeInto(child, parent, insert_idx);
   }

   /**
    * Adds the given category to the given parent node. It is preferable to add
    * categories this way since sorted order will be kept.
    */
   private DefaultMutableTreeNode addCategory(DefaultMutableTreeNode parent,
                                              String category)
   {
      DefaultMutableTreeNode catNode = new DefaultMutableTreeNode(category);

      // Loop through the children of the node, comparing against them
      int insert_idx = 0;
      while (insert_idx < parent.getChildCount())
      {
         DefaultMutableTreeNode child =
                        (DefaultMutableTreeNode)parent.getChildAt(insert_idx);
         Object data = child.getUserObject();

         // First assume that we need to insert after the current child. If we
         // decide we need to insert ourselves before such child, we'll back up
         // the insertion index.
         ++insert_idx;

         // If the child is a category, do lexographic comparison against it
         if (data instanceof String)
         {
            // Check if the new category belongs before this child category
            String cat = (String)data;
            if (category.compareTo(cat) <= 0)
            {
               // Insert the new category before this child node
               --insert_idx;
               break;
            }
         }
         // If the child is not a category, the new category should go before it
         else
         {
            // Insert the new category before this child node
            --insert_idx;
            break;
         }
      }

      parent.insert(catNode, insert_idx);
      return catNode;
   }

   /**
    * Removes all of the chunk descs in the given list from all their locations
    * in the tree.
    */
   private void removeDescs(List descs)
   {
      for (Iterator itr = descs.iterator(); itr.hasNext(); )
      {
         removeDesc((ChunkDesc)itr.next());
      }
   }

   /**
    * Removes the given chunk desc from all of its locations in the tree.
    */
   private void removeDesc(ChunkDesc desc)
   {
      // Stop listening to changes on the desc
      desc.removePropertyChangeListener(this);
      desc.removeChunkDescListener(this);

      for (Iterator itr = getNodesFor(desc).iterator(); itr.hasNext(); )
      {
         MutableTreeNode desc_node = (MutableTreeNode)itr.next();
         MutableTreeNode parent = (MutableTreeNode)desc_node.getParent();
         treeModel.removeNodeFromParent(desc_node);

         // Walk our way up the tree cleaning up empty parents
         while ((parent.getParent() != null) && (parent.getChildCount() == 0))
         {
            MutableTreeNode next_parent = (MutableTreeNode)parent.getParent();
            treeModel.removeNodeFromParent(parent);
            parent = next_parent;
         }
      }
   }

   /**
    * Called when the user has clicked the generic add button positioned above
    * the tree.
    */
   protected void addAction(ActionEvent evt)
   {
      DefaultMutableTreeNode node = (DefaultMutableTreeNode)
                     descPropTree.getLastSelectedPathComponent();
      // Check if the user wants to add a new ChunkDesc
      if (node.getUserObject() instanceof String)
      {
         ChunkDesc chunk_desc = new ChunkDesc();
         chunk_desc.setName("New Chunk Description");
         chunk_desc.setToken("undefined");
         chunk_desc.addCategory((String)node.getUserObject());
         getConfigBroker().add(context, chunk_desc);
      }
      // Check if the user wants to add a new PropertyDesc
      else if (node.getUserObject() instanceof ChunkDesc)
      {
         PropertyDesc new_desc = new PropertyDesc();
         new_desc.setValType(ValType.STRING);
         new_desc.setName("New Property Desc");
         new_desc.setToken("undefined");
         ChunkDesc chunk_desc = (ChunkDesc)node.getUserObject();
         chunk_desc.addPropertyDesc(new_desc);
      }
   }

   /**
    * Called when the user has clicked the generic remove button positioned
    * above the tree.
    */
   protected void removeAction(ActionEvent evt)
   {
      DefaultMutableTreeNode node = (DefaultMutableTreeNode)
                     descPropTree.getLastSelectedPathComponent();
      // Check if the user wants to remove a ChunkDesc
      if (node.getUserObject() instanceof ChunkDesc)
      {
         ChunkDesc chunk_desc = (ChunkDesc)node.getUserObject();
         getConfigBroker().remove(context, chunk_desc);
      }
      // Check if the user wants to remove a PropertyDesc
      else if (node.getUserObject() instanceof PropertyDesc)
      {
         DefaultMutableTreeNode parent = (DefaultMutableTreeNode)node.getParent();
         ChunkDesc chunk_desc = (ChunkDesc)parent.getUserObject();
         PropertyDesc prop_desc = (PropertyDesc)node.getUserObject();
         chunk_desc.removePropertyDesc(prop_desc);
      }
   }

   /**
    * Gets the cached config broker proxy instance.
    */
   private ConfigBroker getConfigBroker()
   {
      if (broker == null)
      {
         broker = new ConfigBrokerProxy();
      }
      return broker;
   }

   /**
    * Builds up the GUI elements. This method is auto-generated by JBuilder.
    */
   private void jbInit()
      throws Exception
   {
      this.setLayout(baseLayout);
      baseSplitPane.setLeftComponent(treeScrollPane);
      treePane.setLayout(treeLayout);
      treeScrollPane.setViewportView(descPropTree);
      propsSplitPane.setOrientation(JSplitPane.VERTICAL_SPLIT);
      propsSplitPane.setOneTouchExpandable(true);
      treeToolbar.setFloatable(false);
      addBtn.setText("Add");
      addBtn.setEnabled(false);
      removeBtn.setText("Remove");
      removeBtn.setEnabled(false);
      helpScrollPane.setMinimumSize(new Dimension(0, 0));
      helpPane.setBackground(new Color(255, 253, 181));
      helpPane.setBorder(null);
      helpPane.setEditable(false);
      helpPane.setMinimumSize(new Dimension(0, 0));
      editorPane.setLayout(editorPaneLayout);
      editorPane.setMinimumSize(new Dimension(0, 0));
      addBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            addAction(evt);
         }
      });
      removeBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            removeAction(evt);
         }
      });
      this.add(baseSplitPane, BorderLayout.CENTER);
      baseSplitPane.add(treePane, JSplitPane.LEFT);
      baseSplitPane.add(propsSplitPane, JSplitPane.RIGHT);
      treePane.add(treeToolbar, BorderLayout.NORTH);
      treePane.add(treeScrollPane, BorderLayout.CENTER);
      treeToolbar.add(addBtn);
      treeToolbar.add(removeBtn);
      propsSplitPane.add(helpScrollPane, JSplitPane.BOTTOM);
      helpScrollPane.setViewportView(helpPane);
      propsSplitPane.add(editorPane, JSplitPane.TOP);
   }

   /**
    * The data model for the tree displaying the ChunkDescDB.
    */
   private DefaultTreeModel treeModel;

   /**
    * The context that provides our view into the active configuration.
    */
   private ConfigContext context = new ConfigContext();

   /**
    * The listener for changes to the configuration.
    */
   private ConfigListener configListener = new ChunkDescConfigListener();

   /**
    * We cache the config broker proxy for speed.
    */
   private ConfigBroker broker;

   /**
    * The overall editor for the ChunkDesc.
    */
   private ChunkDescEditor descEditor = new ChunkDescEditor();

   /**
    * ScrollPane for the ChunkDesc property sheet.
    */
   private JScrollPane descEditorScrollPane = new JScrollPane();

   /**
    * The editor for PropertyDesc editing pane.
    */
   private PropertyDescEditor propertyDescEditor = new PropertyDescEditor();

   private BorderLayout baseLayout = new BorderLayout();
   private JSplitPane baseSplitPane = new JSplitPane();
   private BorderLayout treeLayout = new BorderLayout();
   private JPanel treePane = new JPanel();
   private JToolBar treeToolbar = new JToolBar();
   private JButton addBtn = new JButton();
   private JButton removeBtn = new JButton();
   private JScrollPane treeScrollPane = new JScrollPane();
   private JTree descPropTree = new JTree();
   private JSplitPane propsSplitPane = new JSplitPane();
   private JScrollPane helpScrollPane = new JScrollPane();
   private JPanel editorPane = new JPanel();
   private JEditorPane helpPane = new JEditorPane()
   {
      public boolean getScrollableTracksViewportWidth() { return true; }
      public boolean getScrollableTracksViewportHeight() { return true; }
   };
   private CardLayout editorPaneLayout = new CardLayout();
   private JPanel emptyPane = new JPanel();

   /**
    * Specialized listener for changes to the configuration.
    */
   class ChunkDescConfigListener
      implements ConfigListener
   {
      public void configChunkAdded(ConfigEvent evt) {}
      public void configChunkRemoved(ConfigEvent evt) {}

      public void chunkDescAdded(ConfigEvent evt)
      {
         if (getConfigContext().contains(evt.getResource()))
         {
            addDesc(evt.getChunkDesc());
         }
      }
      public void chunkDescRemoved(ConfigEvent evt)
      {
         if (getConfigContext().contains(evt.getResource()))
         {
            removeDesc(evt.getChunkDesc());
         }
      }
   }

   /**
    * Specialized renderer for a ChunkDesc tree.
    */
   class ChunkDescDBTreeCellRenderer
      extends DefaultTreeCellRenderer
   {
      public ChunkDescDBTreeCellRenderer()
      {
         try
         {
            // Get all the icons we'll ever need now so we can reuse them later
            mChunkDescIcon = new ImageIcon(BeanJarClassLoader.instance().getResource(
                                       "org/vrjuggler/jccl/editors/images/configchunk.gif"));
            mCategoryIcon = new ImageIcon(BeanJarClassLoader.instance().getResource(
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

         // Display the name of ChunkDescs
         if (value instanceof ChunkDesc)
         {
            setText(((ChunkDesc)value).getName());
            setFont(null);
            setIcon(mChunkDescIcon);
         }
         // Strings are always categories
         else if (value instanceof String)
         {
            setText((String)value);
            setFont(tree.getFont().deriveFont(Font.BOLD));
            setIcon(mCategoryIcon);
         }
         // Display the name of PropertyDescs
         else if (value instanceof PropertyDesc)
         {
            setText(((PropertyDesc)value).getName());
            setFont(tree.getFont().deriveFont(Font.ITALIC));
         }

         return this;
      }

      private Icon mChunkDescIcon = null;
      private Icon mCategoryIcon = null;
   }
}
