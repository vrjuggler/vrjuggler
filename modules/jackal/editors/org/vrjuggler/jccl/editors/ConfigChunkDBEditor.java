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

import java.awt.BorderLayout;
import java.awt.CardLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.event.*;
import java.beans.*;
import java.util.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.tree.*;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.tweek.beans.BeanRegistry;
import org.vrjuggler.tweek.beans.loader.BeanJarClassLoader;

/**
 * A specialized panel that knows how to edit ConfigChunks within a
 * ConfigChunkDB.
 */
public class ConfigChunkDBEditor
   extends JPanel
   implements ContextListener
            , ConfigChunkListener
{
   /**
    * The ID used to reference the generic ConfigChunk editor panel in the
    * CardLayout for the editorPane.
    */
   final static String GENERIC         = "GenericPane";

   /**
    * The ID used to reference the custom ConfigChunk editor panel in the
    * CardLayout for the editorPane.
    */
   final static String CUSTOM          = "CustomPane";

   /**
    * The ID used to reference the Category panel in the CardLayout for the
    * editorPane.
    */
   final static String CATEGORY        = "CategoryPane";

   private final static String ADD_VALUE_ACTION       = "Add Value";
   private final static String REMOVE_VALUE_ACTION    = "Remove Value";

   /**
    * Creates a new ConfigChunkDBEditor with no chunk to edit.
    */
   public ConfigChunkDBEditor()
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
            ConfigChunkDBEditor.this.propsSplitPane.removeComponentListener(this);
         }
      });

      // Display the help for properties as they get selected
      genericEditor.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            ConfigChunkPropertySheet ps = (ConfigChunkPropertySheet)evt.getSource();
            PropertyDesc prop_desc = ps.getSelectedProperty();
            if (prop_desc != null)
            {
               displayHelp(ps.getSelectedProperty());
            }
         }
      });

      // Update the action panel as things get selected
      genericEditor.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            ConfigChunkPropertySheet ps = (ConfigChunkPropertySheet)evt.getSource();
            PropertyDesc prop_desc = ps.getSelectedProperty();
            Object sel_object = ps.getSelectedObject();

            // Initially make the pane invisible ...
            propActionPane.setVisible(false);
            if (prop_desc != null)
            {
               // Only tell the user they can add/remove values when it is
               // actually possible to do so.
               if (prop_desc.hasVariableNumberOfValues())
               {
                  // If the selected object is actually a property desc, we want to
                  // provide the user with the add action.
                  if (sel_object instanceof PropertyDesc)
                  {
                     propActionBtn.setActionCommand(ADD_VALUE_ACTION);
                     if (prop_desc.getValType() == ValType.EMBEDDEDCHUNK)
                     {
                        propActionBtn.setText("Add New Embedded Chunk");
                     }
                     else
                     {
                        propActionBtn.setText("Add New Value");
                     }
                     propActionPane.setVisible(true);
                  }
                  // Otherwise, we want to provide the remove action
                  else
                  {
                     propActionBtn.setActionCommand(REMOVE_VALUE_ACTION);
                     if (prop_desc.getValType() == ValType.EMBEDDEDCHUNK)
                     {
                        propActionBtn.setText("Remove Embedded Chunk");
                     }
                     else
                     {
                        propActionBtn.setText("Remove Value");
                     }
                     propActionPane.setVisible(true);
                  }
               }
            }
            propActionPane.getParent().invalidate();
         }
      });
      genericEditor.addChunkSelectionListener(new ChunkSelectionListener()
      {
         public void chunkSelected(ChunkSelectionEvent evt)
         {
            ConfigChunk sel_chunk = evt.getChunk();
            select(sel_chunk);
         }
      });
      genericEditorScrollPane.setViewportView(genericEditor);
      genericEditorScrollPane.setMinimumSize(new Dimension(0, 0));
      genericEditorScrollPane.setPreferredSize(genericEditor.getPreferredSize());
      editorPane.add(genericEditorScrollPane, GENERIC);

      categoryEditorScrollPane.setViewportView(categoryEditor);
      categoryEditorScrollPane.setMinimumSize(new Dimension(0, 0));
      categoryEditorScrollPane.setPreferredSize(categoryEditor.getPreferredSize());
      editorPane.add(categoryEditorScrollPane, CATEGORY);

      editorPaneLayout.show(editorPane, CATEGORY);

      // Init the ConfigChunkDB tree
      DefaultMutableTreeNode root = new DefaultMutableTreeNode("Config");
      treeModel = new DefaultTreeModel(root);
      chunkPropTree.setModel(treeModel);
      chunkPropTree.setCellRenderer(new ConfigChunkDBTreeCellRenderer());
      chunkPropTree.putClientProperty("JTree.lineStyle", "Angled");
      chunkPropTree.getSelectionModel().setSelectionMode(
                              TreeSelectionModel.SINGLE_TREE_SELECTION);
      chunkPropTree.addTreeSelectionListener(new TreeSelectionListener()
      {
         public void valueChanged(TreeSelectionEvent evt)
         {
            DefaultMutableTreeNode node = (DefaultMutableTreeNode)
                           chunkPropTree.getLastSelectedPathComponent();
            // Show basic help if nothing is selected
            if (node == null)
            {
               // Set enabled status on the add/remove buttons
               addBtn.setEnabled(true);
               removeBtn.setEnabled(false);

               // TODO: Show the basic help. For now show nothing.
               editorPaneLayout.show(editorPane, CATEGORY);
               return;
            }

            // Edit an entire chunk
            else if (node.getUserObject() instanceof ConfigChunk)
            {
               // If the parent does not contain a property desc, the selected
               // chunk is not an embedded chunk
               DefaultMutableTreeNode parent = (DefaultMutableTreeNode)node.getParent();
               boolean is_emb_chunk = parent.getUserObject() instanceof PropertyDesc;

               // Set enabled status on the add/remove buttons
               addBtn.setEnabled(true);
               removeBtn.setEnabled(! is_emb_chunk);

               // Show an editor for the given chunk
               ConfigChunk chunk = (ConfigChunk)node.getUserObject();
               genericEditor.setConfigChunk(chunk);

               // Compute the string version of the path to the chunk
               String path = convertFullNameToTreePath(chunk.getFullName());
               locationLbl.setText(path);
               editorPaneLayout.show(editorPane, GENERIC);
               propsPane.updateUI();
            }

            // Selected a category
            else if (node.getUserObject() instanceof String)
            {
               // Set enabled status on the add/remove buttons
               addBtn.setEnabled(true);
               removeBtn.setEnabled(false);

               // TODO: Switch to a category panel. Clear the editing panel.
               editorPaneLayout.show(editorPane, CATEGORY);
            }

            // Selected a property desc (for an embedded chunk)
            else if (node.getUserObject() instanceof PropertyDesc)
            {
               // Set enabled status on the add/remove buttons
               addBtn.setEnabled(true);
               removeBtn.setEnabled(false);

               //TODO: Switch to a property desc panel.
               editorPaneLayout.show(editorPane, CATEGORY);
            }
         }
      });
   }

   protected String convertFullNameToTreePath(String fullname)
   {
      StringTokenizer tokenizer = new StringTokenizer(fullname, "/");
      List chunks = getConfigBroker().getChunks(context);
      List matches = ConfigUtilities.getChunksWithName(chunks, tokenizer.nextToken());
      ConfigChunk root_chunk = (ConfigChunk)matches.get(0);
      ConfigChunk chunk = root_chunk;

      StringBuffer path = new StringBuffer(chunk.getName());
      boolean property_token = true;
      while (tokenizer.hasMoreTokens())
      {
         String token = tokenizer.nextToken();
         if (property_token)
         {
            path.append("/").append(chunk.getDescName());
         }
         else
         {
            List emb_chunks = chunk.getEmbeddedChunks();
            for (Iterator itr = emb_chunks.iterator(); itr.hasNext(); )
            {
               ConfigChunk child = (ConfigChunk)itr.next();
               if (child.getName().equals(token))
               {
                  chunk = child;
                  path.append("/").append(chunk.getName());
               }
            }
         }
         property_token = ! property_token;
      }

      return path.toString();
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
    * Displays the help for the given property description.
    */
   protected void displayHelp(PropertyDesc desc)
   {
      if (desc != null)
      {
         helpPane.setText(desc.getHelp());
      }
      else
      {
         helpPane.setText("");
      }
   }

   /**
    * Programmatically selects the given ConfigChunk in the tree. If the chunk
    * is in more than one category, its location in the first category will be
    * selected.
    */
   protected void select(ConfigChunk chunk)
   {
      String path = chunk.getFullName();

      // Get the root chunk.
      String name = (new StringTokenizer(path, "/")).nextToken();
      List chunks = getConfigBroker().getChunks(context);
      List matches = ConfigUtilities.getChunksWithName(chunks, name);
      if (matches.size() == 0)
      {
         System.out.println("ERROR: Can't find the root ConfigChunk");
         return;
      }
      ConfigChunk root = (ConfigChunk)matches.get(0);

      // Get the category name in which the root chunk belongs
      ChunkDesc desc = root.getDesc();

      // Fail silently if this chunk is not in any categories
      if (desc.getNumCategories() == 0)
      {
         return;
      }

      // Get the category in which the ConfigChunk belongs
      String category = desc.getCategory(0);
      DefaultMutableTreeNode node = getCategoryNode(category);

      StringTokenizer tokenizer = new StringTokenizer(path, "/");
      while (tokenizer.hasMoreTokens())
      {
         String token = tokenizer.nextToken();

         // Find the child of this node whose value matches the current token
         for (int i=0; i<node.getChildCount(); ++i)
         {
            DefaultMutableTreeNode child = (DefaultMutableTreeNode)node.getChildAt(i);
            Object value = child.getUserObject();
            if (value instanceof ConfigChunk)
            {
               if (((ConfigChunk)value).getName().equals(token))
               {
                  node = child;
                  break;
               }
            }
            if (value instanceof PropertyDesc)
            {
               if (((PropertyDesc)value).getToken().equals(token))
               {
                  node = child;
                  break;
               }
            }
         }

         // Check if we found the next child
         if (node == null)
         {
            // We have failed to find the Chunk in the tree
            System.out.println("Can't find Chunk "+path);
            return;
         }
      }

      // Select the node in the tree.
      chunkPropTree.setSelectionPath(new TreePath(node.getPath()));
   }

   /**
    * Rebuilds the config chunk DB tree.
    */
   private void rebuildTree()
   {
      // First clear out all the old nodes from the tree.
      ((DefaultMutableTreeNode)treeModel.getRoot()).removeAllChildren();

      // Run through all chunk descs in the context
      List chunks = getConfigBroker().getChunks(context);
      addChunks(chunks);
   }

   /**
    * Adds all of the config chunks in the given list into the tree in all of
    * their categories.
    */
   private void addChunks(List chunks)
   {
      for (Iterator itr = chunks.iterator(); itr.hasNext(); )
      {
         addChunk((ConfigChunk)itr.next());
      }

      // Make sure the root node is expanded
      TreePath path = new TreePath(treeModel.getPathToRoot((TreeNode)treeModel.getRoot()));
      chunkPropTree.expandPath(path);
   }

   /**
    * Adds the given config chunk to the tree in all of its categories.
    */
   private void addChunk(ConfigChunk chunk)
   {
      // Make ourselves a listener to the chunk
      chunk.addConfigChunkListener(this);
      for (Iterator itr = chunk.getEmbeddedChunks().iterator(); itr.hasNext(); )
      {
         ConfigChunk emb_chunk = (ConfigChunk)itr.next();
         System.out.println("Listening to emb chunk: "+emb_chunk.getName());
         emb_chunk.addConfigChunkListener(this);
      }

      // Add the chunk to each of its categories
      for (Iterator itr = chunk.getDesc().getCategories(); itr.hasNext(); )
      {
         // Get the node for the chunk's category
         String category = (String)itr.next();
         DefaultMutableTreeNode categoryNode = getCategoryNode(category);

         // Add the chunk to the category
         addChunk(categoryNode, chunk);
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
    * Adds a new ConfigChunk to the given parent node. This method does
    * <b>not</b> fire off any events.
    *
    * @param parent     the node that will be the chunk's parent
    * @param chunk      the ConfigChunk to intert into the tree
    */
   private DefaultMutableTreeNode addChunk(DefaultMutableTreeNode parent,
                                           ConfigChunk chunk)
   {
      // Create the node for the chunk and add it to the parent
      DefaultMutableTreeNode chunkNode = new DefaultMutableTreeNode(chunk);
      addNode(parent, chunkNode);

      // Add each property of the chunk as a child node of the chunk
      ChunkDesc desc = chunk.getDesc();
      for (Iterator itr = desc.getPropertyDescs().iterator(); itr.hasNext(); )
      {
         DefaultMutableTreeNode descNode;
         PropertyDesc propDesc = (PropertyDesc)itr.next();
         descNode = new DefaultMutableTreeNode(propDesc);

         // If this property desc describes an embedded chunk recurse
         if (propDesc.getValType() == ValType.EMBEDDEDCHUNK)
         {
            // Add in the property only if it contains embedded chunks
            addNode(chunkNode, descNode);
            int numPropValues = chunk.getNumPropertyValues(propDesc.getToken());
            for (int i=0; i<numPropValues; ++i)
            {
               // Add the embedded chunk as a child of the current property
               ConfigChunk emb_chunk = (ConfigChunk)chunk.getProperty(propDesc.getToken(), i);
               addChunk(descNode, emb_chunk);
            }
         }
      }

      return chunkNode;
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

         // If child is a ConfigChunk, it should be sorted against other config
         // chunks, but come after categories
         else if (childData instanceof ConfigChunk)
         {
            // If the current node is ConfigChunk, we can do our comparision.
            // Otherwise, we just continue through the loop
            ConfigChunk chunk = (ConfigChunk)childData;
            if (data instanceof ConfigChunk)
            {
               // Does the new chunk belong before the current chunk?
               ConfigChunk chk = (ConfigChunk)data;
               if (chunk.getName().compareTo(chk.getName()) <= 0)
               {
                  // Insert the new chunk before the current node
                  --insert_idx;
                  break;
               }
            }
         }

         // If a  child is a PropertyDesc, it should be sorted against other
         // property descs. A property desc's siblings are always other property
         // descs, but just in case, we always put them at the end.
         else if (childData instanceof PropertyDesc)
         {
            // If the current node is a PropertyDesc, we can do our comparison
            PropertyDesc prop_desc = (PropertyDesc)childData;
            if (data instanceof PropertyDesc)
            {
               // Does the new desc belong before the current desc?
               PropertyDesc desc = (PropertyDesc)data;
               if (prop_desc.getName().compareTo(desc.getName()) <= 0)
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

      treeModel.insertNodeInto(parent, catNode, insert_idx);
      return catNode;
   }

   /**
    * Removes all of the config chunks in the given list from all of their
    * locations in the tree.
    */
   private void removeChunks(List chunks)
   {
      for (Iterator itr = chunks.iterator(); itr.hasNext(); )
      {
         removeChunk((ConfigChunk)itr.next());
      }
   }

   /**
    * Removes the given config chunk from all of its locations in the tree.
    */
   private void removeChunk(ConfigChunk chunk)
   {
      // Don't listen to the chunk anymore
      for (Iterator itr = chunk.getEmbeddedChunks().iterator(); itr.hasNext(); )
      {
         ((ConfigChunk)itr.next()).removeConfigChunkListener(this);
      }
      chunk.removeConfigChunkListener(this);

      // Get the nodes for the chunk
      List chunk_nodes = getNodesFor(chunk);
      for (Iterator itr = chunk_nodes.iterator(); itr.hasNext(); )
      {
         // Remove the config chunk node from its parent. In this case, the
         // tree model will automatically fire off the removed event for us.
         DefaultMutableTreeNode chunk_node = (DefaultMutableTreeNode)itr.next();
         MutableTreeNode parent = (MutableTreeNode)chunk_node.getParent();
         treeModel.removeNodeFromParent(chunk_node);

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
    * Called by the current context when a resource has been added to it.
    */
   public void resourceAdded(ContextEvent evt)
   {
      String resource = evt.getResource();
      addChunks(getConfigBroker().getChunksIn(resource));
   }

   /**
    * Called by the current context when a resource has been removed from it.
    */
   public void resourceRemoved(ContextEvent evt)
   {
      String resource = evt.getResource();
      removeChunks(getConfigBroker().getChunksIn(resource));
   }

   /**
    * This gets called whenever one of the ConfigChunks we are editing has
    * changed its name.
    */
   public void nameChanged(ConfigChunkEvent evt)
   {
      ConfigChunk src = (ConfigChunk)evt.getSource();
      for (Iterator itr = getNodesFor(src).iterator(); itr.hasNext(); )
      {
         treeModel.nodeChanged((TreeNode)itr.next());
      }
   }

   /**
    * This gets called whenever one of the ConfigChunks we are editing has one
    * of its property values change.
    */
   public void propertyValueChanged(ConfigChunkEvent evt)
   {
      ConfigChunk src = (ConfigChunk)evt.getSource();

      // If the property is not an embedded chunk, we don't care about it
      if (src.getPropertyDesc(evt.getProperty()).getValType() != ValType.EMBEDDEDCHUNK)
      {
         return;
      }

      // Update each chunk node in the tree
      for (Iterator itr = getNodesFor(src).iterator(); itr.hasNext(); )
      {
         DefaultMutableTreeNode node = (DefaultMutableTreeNode)itr.next();

         // Go through the children of this node looking for the property that
         // matches
         for (Enumeration e = node.children(); e.hasMoreElements(); )
         {
            DefaultMutableTreeNode prop_node = (DefaultMutableTreeNode)e.nextElement();
            if (prop_node.getUserObject() == evt.getValue())
            {
               treeModel.nodeChanged(prop_node);
            }
         }
      }
   }

   /**
    * This gets called whenever one of the ConfigChunks we are editing adds a
    * new property value.
    */
   public void propertyValueAdded(ConfigChunkEvent evt)
   {
      ConfigChunk src = (ConfigChunk)evt.getSource();
      // If the property is not an embedded chunk, we don't care about it
      if (src.getPropertyDesc(evt.getProperty()).getValType() != ValType.EMBEDDEDCHUNK)
      {
         return;
      }

      // Update each chunk node in the tree
      for (Iterator itr = getNodesFor(src).iterator(); itr.hasNext(); )
      {
         DefaultMutableTreeNode chunk_node = (DefaultMutableTreeNode)itr.next();

         // Find the node for the property description
         for (Enumeration e = chunk_node.children(); e.hasMoreElements(); )
         {
            DefaultMutableTreeNode child = (DefaultMutableTreeNode)e.nextElement();
            PropertyDesc prop_desc = src.getPropertyDesc(evt.getProperty());
            if (prop_desc.equals(child.getUserObject()))
            {
               // Create a new node for the new property value
               ConfigChunk new_chunk = (ConfigChunk)evt.getValue();
               DefaultMutableTreeNode new_node = new DefaultMutableTreeNode(new_chunk);

               // Add a listener to the newly added embedded chunk
               System.out.println("Listening to emb chunk: "+new_chunk.getName());
               new_chunk.addConfigChunkListener(this);

               // Add the new node into the tree
               treeModel.insertNodeInto(new_node, child, evt.getIndex());
            }
         }
      }
   }

   /**
    * This gets called whenever one of the ConfigChunks we are editing removes a
    * property value.
    */
   public void propertyValueRemoved(ConfigChunkEvent evt)
   {
      ConfigChunk src = (ConfigChunk)evt.getSource();

      // If the property is not an embedded chunk, we don't care about it
      if (src.getPropertyDesc(evt.getProperty()).getValType() != ValType.EMBEDDEDCHUNK)
      {
         return;
      }

      // Update each chunk node in the tree
      for (Iterator itr = getNodesFor(src).iterator(); itr.hasNext(); )
      {
         DefaultMutableTreeNode chunk_node = (DefaultMutableTreeNode)itr.next();

         // Find the node for the property description
         for (Enumeration e = chunk_node.children(); e.hasMoreElements(); )
         {
            DefaultMutableTreeNode child = (DefaultMutableTreeNode)e.nextElement();
            PropertyDesc prop_desc = src.getPropertyDesc(evt.getProperty());
            if (prop_desc.equals(child.getUserObject()))
            {
               // Get the node for the property value to remove
               MutableTreeNode prop_node = (MutableTreeNode)child.getChildAt(evt.getIndex());

               // Stop listening to the embedded chunk
               ConfigChunk removed_chunk = (ConfigChunk)evt.getValue();
               removed_chunk.removeConfigChunkListener(this);

               // Remove the node from the tree
               treeModel.removeNodeFromParent(prop_node);
            }
         }
      }
   }

   /**
    * Gets a list of all the tree nodes that contain the given Object.
    */
   protected List getNodesFor(Object obj)
   {
      return getNodesFor(obj, (DefaultMutableTreeNode)treeModel.getRoot());
   }

   /**
    * Recursive helper for getNodesFor(Object).
    *
    * @see #getNodesFor(Object)
    */
   private List getNodesFor(Object obj, DefaultMutableTreeNode node)
   {
      List results = new ArrayList();

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
    * Adds a new config chunk to the current ConfigChunkDB.
    */
   protected void addAction(ActionEvent evt)
   {
      DefaultMutableTreeNode node = (DefaultMutableTreeNode)
                     chunkPropTree.getLastSelectedPathComponent();

      // Get a DB of all known ChunkDescs
      List descs = getConfigBroker().getDescs(context);

      // Ask the user to choose a base ChunkDesc
      ChunkDescChooser chooser = new ChunkDescChooser();
      chooser.setDescs(descs);
      int result = chooser.showDialog(this);

      // If the user did not cancel their choice, make a new ConfigChunk for
      // the chose ChunkDesc
      if (result == ChunkDescChooser.APPROVE_OPTION)
      {
         ConfigChunk chunk = new ConfigChunk(chooser.getSelectedChunkDesc());
//         chunk.setName(configChunkDB.getNewName(chunk.getDesc().getName()));
         chunk.setName(chunk.getDesc().getName()); // TODO: Compute a unique name

         // Make sure this add goes through successfully
         if (! getConfigBroker().add(context, chunk))
         {
            JOptionPane.showMessageDialog(this,
                                          "There are no configuration files active.",
                                          "Error",
                                          JOptionPane.ERROR_MESSAGE);
            return;
         }

         // Make sure the new node gets selected
         List chunk_nodes = getNodesFor(chunk);
         if (chunk_nodes.size() > 0)
         {
            TreeNode chunk_node = (TreeNode)chunk_nodes.get(0);
            TreePath path = new TreePath(treeModel.getPathToRoot(chunk_node));
            chunkPropTree.setSelectionPath(path);
         }
      }
   }

   /**
    * Removes the currently selected ConfigChunk from the current ConfigChunkDB.
    */
   protected void removeAction(ActionEvent evt)
   {
      DefaultMutableTreeNode node = (DefaultMutableTreeNode)
                     chunkPropTree.getLastSelectedPathComponent();

      if (node.getUserObject() instanceof ConfigChunk)
      {
         getConfigBroker().remove(context, (ConfigChunk)node.getUserObject());
      }
   }

   /**
    * The generic action that applies to the currently selected property value.
    */
   protected void propertyValueAction(ActionEvent evt)
   {
      String cmd = evt.getActionCommand();
      ConfigChunkTreeTableModel model =
            (ConfigChunkTreeTableModel)genericEditor.getTreeTableModel();

      if (cmd.equals(ADD_VALUE_ACTION))
      {
         // The currently selected object MUST be a PropertyDesc
         PropertyDesc prop_desc = (PropertyDesc)genericEditor.getSelectedObject();
         DefaultMutableTreeNode node = model.getNodeFor(prop_desc);
         DefaultMutableTreeNode parent_node = (DefaultMutableTreeNode)node.getParent();
         ConfigChunk parent_chunk = (ConfigChunk)parent_node.getUserObject();

         // Append a new value into the property
         int insert_idx = parent_chunk.getNumPropertyValues(prop_desc.getToken());
         Object default_value = prop_desc.getDefaultValue(insert_idx);
         parent_chunk.setProperty(prop_desc.getToken(), insert_idx, default_value);
      }
      else if (cmd.equals(REMOVE_VALUE_ACTION))
      {
         // The parent to the currently selected object MUST be a PropertyDesc
         Object sel_value = genericEditor.getSelectedObject();
         DefaultMutableTreeNode node = model.getNodeFor(sel_value);
         DefaultMutableTreeNode parent_node = (DefaultMutableTreeNode)node.getParent();
         PropertyDesc prop_desc = (PropertyDesc)parent_node.getUserObject();

         // The parent to the PropertyDesc MUST be a ConfigChunk
         DefaultMutableTreeNode chunk_node = (DefaultMutableTreeNode)parent_node.getParent();
         ConfigChunk chunk = (ConfigChunk)chunk_node.getUserObject();

         // Remove the currently selected value from the property
         int remove_idx = parent_node.getIndex(node);
         chunk.removeProperty(prop_desc.getToken(), remove_idx);
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
      treeScrollPane.setViewportView(chunkPropTree);
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
      propsPane.setLayout(propsLayout);
      locationLbl.setText("");
      locationLbl.setFont(new Font("serif", Font.BOLD, 16));
      locationLbl.setBorder(BorderFactory.createRaisedBevelBorder());
      locationLbl.setMinimumSize(new Dimension(0, 0));
      editorPane.setLayout(editorPaneLayout);
      propActionPane.setLayout(new BoxLayout(propActionPane, BoxLayout.X_AXIS));
      propActionPane.setVisible(false);
      propActionBtn.setText("Add Embedded Chunk");
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
      propActionBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            propertyValueAction(evt);
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
      propsSplitPane.add(propsPane, JSplitPane.TOP);
      propsPane.add(locationLbl, BorderLayout.NORTH);
      propsPane.add(editorPane, BorderLayout.CENTER);
      propsPane.add(propActionPane, BorderLayout.SOUTH);
      propActionPane.add(Box.createHorizontalGlue(), null);
      propActionPane.add(propActionBtn, null);
   }

   /**
    * The data model for the tree displaying the ConfigChunk.
    */
   private DefaultTreeModel treeModel;

   /**
    * The context that provides our view into the active configuration.
    */
   private ConfigContext context = new ConfigContext();

   /**
    * The listener for changes to the configuration.
    */
   private ConfigListener configListener = new ConfigChunkConfigListener();

   /**
    * We cache the config broker proxy for speed.
    */
   private ConfigBroker broker;

   /**
    * The generic property sheet editor for ConfigChunks.
    */
   ConfigChunkPropertySheet genericEditor = new ConfigChunkPropertySheet();

   /**
    * ScrollPane for the generic editor component.
    */
   JScrollPane genericEditorScrollPane = new JScrollPane();

   /**
    * The category editor.
    */
   JPanel categoryEditor = new JPanel();

   /**
    * ScrollPane for the category editor component.
    */
   JScrollPane categoryEditorScrollPane = new JScrollPane();

   //--- JBuilder generated GUI variables ---//
   private BorderLayout baseLayout = new BorderLayout();
   private JSplitPane baseSplitPane = new JSplitPane();
   private BorderLayout treeLayout = new BorderLayout();
   private JPanel treePane = new JPanel();
   private JToolBar treeToolbar = new JToolBar();
   private JButton addBtn = new JButton();
   private JButton removeBtn = new JButton();
   private JScrollPane treeScrollPane = new JScrollPane();
   private JTree chunkPropTree = new JTree();
   private JSplitPane propsSplitPane = new JSplitPane();
   private JScrollPane helpScrollPane = new JScrollPane();
   private JEditorPane helpPane = new JEditorPane()
   {
      public boolean getScrollableTracksViewportWidth() { return true; }
      public boolean getScrollableTracksViewportHeight() { return true; }
   };
   private JPanel propsPane = new JPanel();
   private JLabel locationLbl = new JLabel();
   private JPanel editorPane = new JPanel();
   private JPanel propActionPane = new JPanel();
   private JButton propActionBtn = new JButton();
   private BorderLayout propsLayout = new BorderLayout();
   private CardLayout editorPaneLayout = new CardLayout();

   /**
    * Specialized listener for changes to the configuration.
    */
   class ConfigChunkConfigListener
      implements ConfigListener
   {
      public void configChunkAdded(ConfigEvent evt)
      {
         if (getConfigContext().contains(evt.getResource()))
         {
            addChunk(evt.getConfigChunk());
         }
      }

      public void configChunkRemoved(ConfigEvent evt)
      {
         if (getConfigContext().contains(evt.getResource()))
         {
            removeChunk(evt.getConfigChunk());
         }
      }

      public void chunkDescAdded(ConfigEvent evt) {}
      public void chunkDescRemoved(ConfigEvent evt) {}
   }

   /**
    * Specialized renderer for a ConfigChunk tree.
    */
   class ConfigChunkDBTreeCellRenderer
      extends DefaultTreeCellRenderer
   {
      public ConfigChunkDBTreeCellRenderer()
      {
         try
         {
            // Get all the icons we'll ever need now so we can reuse them later
            mConfigChunkIcon = new ImageIcon(BeanJarClassLoader.instance().getResource(
                                       "org/vrjuggler/jccl/editors/images/configchunk.gif"));
            mPropertyIcon = new ImageIcon(BeanJarClassLoader.instance().getResource(
                                       "org/vrjuggler/jccl/editors/images/property.gif"));
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

         // Display the name of ConfigChunks
         if (value instanceof ConfigChunk)
         {
            setText(((ConfigChunk)value).getName());
            setFont(null);
            setIcon(mConfigChunkIcon);
         }
         // Display the name of PropertyDescs
         else if (value instanceof PropertyDesc)
         {
            setText(((PropertyDesc)value).getName());
            setFont(tree.getFont().deriveFont(Font.ITALIC));
            setIcon(mPropertyIcon);
         }
         // Strings are always categories
         else if (value instanceof String)
         {
            setText((String)value);
            setFont(tree.getFont().deriveFont(Font.BOLD));
            setIcon(mCategoryIcon);
         }

         return this;
      }

      private Icon mConfigChunkIcon = null;
      private Icon mPropertyIcon = null;
      private Icon mCategoryIcon = null;
   }
}
