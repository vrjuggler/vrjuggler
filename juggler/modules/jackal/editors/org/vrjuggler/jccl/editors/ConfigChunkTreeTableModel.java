/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2002 by Iowa State University
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

import java.util.*;
import javax.swing.tree.*;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.tweek.ui.JTreeTable;
import org.vrjuggler.tweek.ui.treetable.*;

/**
 * A table model for config chunks. This two-column model will return property
 * names for the first column and property values for the second column.
 */
public class ConfigChunkTreeTableModel
   extends AbstractTreeTableModel
{
   /**
    * Creates a new table model with no config chunk for its data.
    */
   public ConfigChunkTreeTableModel()
   {
      super(new DefaultMutableTreeNode(""));
   }

   /**
    * Creates a new table model with the given config chunk for its data.
    */
   public ConfigChunkTreeTableModel(ConfigChunk chunk)
   {
      this();
      setConfigChunk(chunk);
   }

   /**
    * Sets the config chunk this table model represents.
    */
   public void setConfigChunk(ConfigChunk chunk)
   {
      // First clear out all the old nodes from the tree.
      ((DefaultMutableTreeNode)getRoot()).removeAllChildren();

      this.chunk = chunk;
      setRoot(new DefaultMutableTreeNode(this.chunk));

      // First node is name
      DefaultMutableTreeNode name_node = new DefaultMutableTreeNode(chunk.getName());
      insertNodeInto(name_node, (MutableTreeNode)getRoot(), 0);

      // Second node is type
      DefaultMutableTreeNode type_node = new DefaultMutableTreeNode(
                                                chunk.getDesc().getName());
      insertNodeInto(type_node, (MutableTreeNode)getRoot(), 1);

      for (Iterator itr = this.chunk.getDesc().getPropertyDescs().iterator(); itr.hasNext(); )
      {
         PropertyDesc prop_desc = (PropertyDesc)itr.next();
         addProperty(chunk, prop_desc);
      }
   }

   /**
    * Gets the config chunk this table model represents.
    */
   public ConfigChunk getConfigChunk()
   {
      return chunk;
   }

   /**
    * Adds the nodes necessary to display the property values for the given
    * config chunk described by the given property description.
    */
   private void addProperty(ConfigChunk chunk, PropertyDesc propDesc)
   {
      DefaultMutableTreeNode parent = getNodeFor(chunk);

      // This property has only one value, add it in
      if ((propDesc.getItemsSize() == 1) && (! propDesc.hasVariableNumberOfValues())
         && (propDesc.getValType() != ValType.EMBEDDEDCHUNK))
      {
         int idx = parent.getChildCount();
//         System.out.println("Adding property node for single-valued property: "+
//                            propDesc.getToken()+"["+idx+"]");
         Object value = chunk.getProperty(propDesc.getToken()).get();
         DefaultMutableTreeNode value_node = new DefaultMutableTreeNode(value);
         insertNodeInto(value_node, parent, idx);
      }
      // This property has (or can have) more than one value, add a node that is
      // the parent for all its values.
      else
      {
         int idx = parent.getChildCount();
//         System.out.println("Adding property node for multi-valued property: "+
//                            propDesc.getToken()+"["+idx+"]");
         DefaultMutableTreeNode prop_desc_node = new DefaultMutableTreeNode(propDesc);
         insertNodeInto(prop_desc_node, parent, idx);
         parent = prop_desc_node;

         int num_props = chunk.getNumPropertyValues(propDesc.getToken());
         for (int i=0; i<num_props; ++i)
         {
            Object value = chunk.getProperty(propDesc.getToken(), i).get();
            if (! (value instanceof ConfigChunk))
            {
//               System.out.println("Adding property value: "+propDesc.getToken()+"["+i+"]");
               DefaultMutableTreeNode child = new DefaultMutableTreeNode(value);
               int prop_idx = parent.getChildCount();
               insertNodeInto(child, parent, prop_idx);
            }
            else
            {
               addEmbeddedChunk(parent, (ConfigChunk)value);
            }
         }
      }
   }

   /**
    * Adds a new embedded config chunk to the given parent node.
    */
   private void addEmbeddedChunk(DefaultMutableTreeNode parent, ConfigChunk chunk)
   {
      int idx = parent.getChildCount();
//      System.out.println("Adding embedded chunk node for chunk: "+
//                         chunk.getName()+"["+idx+"]");
      DefaultMutableTreeNode chunk_node = new DefaultMutableTreeNode(chunk);
      insertNodeInto(chunk_node, parent, idx);

      for (Iterator itr = chunk.getDesc().getPropertyDescs().iterator(); itr.hasNext(); )
      {
         PropertyDesc prop_desc = (PropertyDesc)itr.next();
         addProperty(chunk, prop_desc);
      }
   }

   /**
    * Sets the root node to the given object.
    */
   protected void setRoot(Object value)
   {
      this.root = value;
      fireTreeStructureChanged(this, getPathToRoot((TreeNode)getRoot()), null, null);
   }

   /**
    * Gets the number of columns.
    */
   public int getColumnCount()
   {
      return columnNames.length;
   }

   /**
    * Gets the name of the given column.
    */
   public String getColumnName(int col)
   {
      return columnNames[col];
   }

   /**
    * Gets the class of the data in the given column.
    */
   public Class getColumnClass(int col)
   {
      return columnClasses[col];
   }

   /**
    * Gets the value for the given node at the given column.
    */
   public Object getValueAt(Object node, int col)
   {
      DefaultMutableTreeNode tree_node = (DefaultMutableTreeNode)node;
      Object value = tree_node.getUserObject();
      switch (col)
      {
      // Name
      case 0:
         return value;
      // Value
      case 1:
         if (value instanceof ConfigChunk)
         {
            return null;
         }
         else if (value instanceof PropertyDesc)
         {
            if (((PropertyDesc)value).getValType() != ValType.EMBEDDEDCHUNK)
            {
               StringBuffer buffer = new StringBuffer();
               for (Enumeration e = tree_node.children(); e.hasMoreElements(); )
               {
                  DefaultMutableTreeNode child_node = (DefaultMutableTreeNode)e.nextElement();
                  buffer.append(child_node.getUserObject());
                  if (e.hasMoreElements())
                  {
                     buffer.append(", ");
                  }
               }
               return buffer.toString();
            }
            return null;
         }
         return value;
      }
      return null;
   }

   /**
    * Sets the value for the given node at the given column to the given value.
    */
   public void setValueAt(Object value, Object node, int col)
   {
      DefaultMutableTreeNode tree_node = (DefaultMutableTreeNode)node;
      Object node_value = tree_node.getUserObject();

      switch (col)
      {
      // Name (not supported)
      case 0:
         break;
      // Value
      case 1:
         DefaultMutableTreeNode parent_node = (DefaultMutableTreeNode)tree_node.getParent();

         // First child of root is always the chunk name
         if (parent_node == getRoot() && parent_node.getIndex((TreeNode)node) == 0)
         {
            ConfigChunk chunk = (ConfigChunk)parent_node.getUserObject();
            chunk.setName((String)value);
            tree_node.setUserObject(value);
         }

         else if (node_value instanceof PropertyDesc)
         {
            // Hey, we're editing a property desc. If it's not of an embedded
            // chunk type, we're probably editing a summary list of the values
            // of the children.
            if (((PropertyDesc)node_value).getValType() != ValType.EMBEDDEDCHUNK)
            {
               ConfigChunk chunk = (ConfigChunk)parent_node.getUserObject();
               PropertyDesc prop_desc = (PropertyDesc)node_value;
               StringTokenizer tokenizer = new StringTokenizer((String)value,
                                                               ", ");
               int idx = 0;
               while (tokenizer.hasMoreTokens())
               {
                  String token = tokenizer.nextToken();

                  // Make sure we don't overrun the property values
                  if ((idx >= prop_desc.getItemsSize())
                      && (!prop_desc.hasVariableNumberOfValues()))
                  {
                     break;
                  }

                  // Convert the value to the appropriate type
                  Object new_value = null;
                  ValType type = prop_desc.getValType();
                  if (type == ValType.BOOL)
                  {
                     new_value = new Boolean(token);
                  }
                  else if (type == ValType.INT)
                  {
                     new_value = new Integer(token);
                  }
                  else if (type == ValType.FLOAT)
                  {
                     new_value = new Float(token);
                  }
                  else if (type == ValType.STRING || type == ValType.CHUNK)
                  {
                     new_value = new String(token);
                  }

                  setProperty(new_value, chunk, prop_desc.getToken(), idx);

                  // Get the node for the current property value and update it
                  if (idx < tree_node.getChildCount())
                  {
                     DefaultMutableTreeNode child_node = (DefaultMutableTreeNode)tree_node.getChildAt(idx);
                     child_node.setUserObject(new_value);
                  }
                  else
                  {
                     // Insert the new property
                     DefaultMutableTreeNode new_node = new DefaultMutableTreeNode(new_value);
                     tree_node.add(new_node);
                     fireTreeNodesInserted(this,
                                           new Object[] { getPathToRoot(tree_node) },
                                           new int[] { tree_node.getIndex(new_node) },
                                           new Object[] { new_node });
                  }
                  ++idx;
               }
            }
         }
         else
         {
            // Parent is a ConfigChunk ... must be a single-valued property
            if (parent_node.getUserObject() instanceof ConfigChunk)
            {
               ConfigChunk chunk = (ConfigChunk)parent_node.getUserObject();
               int desc_idx = parent_node.getIndex(tree_node);

               // If the parent is the root, take into account the extra name
               // and type nodes
               if (parent_node == getRoot())
               {
                  desc_idx -= 2;
               }
               PropertyDesc prop_desc = chunk.getDesc().getPropertyDesc(desc_idx);
               setProperty(value, chunk, prop_desc.getToken(), 0);
               tree_node.setUserObject(value);
            }
            else
            {
               // Parent must be a PropertyDesc
               PropertyDesc prop_desc = (PropertyDesc)parent_node.getUserObject();
               int value_idx = parent_node.getIndex(tree_node);
               DefaultMutableTreeNode chunk_node = (DefaultMutableTreeNode)parent_node.getParent();
               ConfigChunk chunk = (ConfigChunk)chunk_node.getUserObject();
               setProperty(value, chunk, prop_desc.getToken(), value_idx);
               tree_node.setUserObject(value);
            }
         }
         fireTreeNodesChanged(this,
                              new Object[] { getPathToRoot(parent_node) },
                              new int[] { parent_node.getIndex(tree_node) },
                              new Object[] { tree_node });
         break;
      default:
         throw new IllegalArgumentException("Invalid column: "+col);
      }
   }

   /**
    * Helper method for setValueAt() that sets the value of a specific property
    * value within a chunk to the given value.
    */
   private void setProperty(Object value, ConfigChunk chunk, String propToken,
                            int value_idx)
   {
      VarValue varVal;
      if (value instanceof Boolean)
      {
         varVal = new VarValue(((Boolean)value).booleanValue());
      }
      else if (value instanceof Integer)
      {
         varVal = new VarValue(((Integer)value).intValue());
      }
      else if (value instanceof Float)
      {
         varVal = new VarValue(((Float)value).floatValue());
      }
      else if (value instanceof String)
      {
         varVal = new VarValue(value.toString());
      }
      else if (value instanceof ConfigChunk)
      {
         varVal = new VarValue((ConfigChunk)value);
      }
      else
      {
         // ACK!
         varVal = null;
      }

      chunk.setProperty(propToken, value_idx, varVal);
   }

   /**
    * Indicates whether the value for the given node at the given column is
    * editable.
    */
   public boolean isCellEditable(Object node, int col)
   {
      // The tree column has to be editable so it can get the events
      if (col == 0)
      {
         return true;
      }

      DefaultMutableTreeNode tree_node = (DefaultMutableTreeNode)node;
      Object value = tree_node.getUserObject();
      if (value instanceof ConfigChunk)
      {
         return false;
      }
      else
      {
         return true;
      }
   }

   /**
    * Gets the number of child nodes for the given parent node.
    */
   public int getChildCount(Object parent)
   {
      return ((DefaultMutableTreeNode)parent).getChildCount();
   }

   /**
    * Gets the child node at the given index to the parent node.
    */
   public Object getChild(Object parent, int idx)
   {
      return ((DefaultMutableTreeNode)parent).getChildAt(idx);
   }

   /**
    * Inserts the given child node into the given parent at the given index.
    */
   public void insertNodeInto(MutableTreeNode child, MutableTreeNode parent, int idx)
   {
      parent.insert(child, idx);
      this.fireTreeNodesInserted(this, this.getPathToRoot(parent),
                                 new int[] { idx },
                                 new Object[] { child });
   }

   /**
    * Builds the parents of node up to and including the root node,
    * where the original node is the last element in the returned array.
    * The length of the returned array gives the node's depth in the
    * tree.
    *
    * @param aNode the TreeNode to get the path for
    * @param an array of TreeNodes giving the path from the root to the
    *        specified node.
    */
   public TreeNode[] getPathToRoot(TreeNode aNode)
   {
       return getPathToRoot(aNode, 0);
   }

   /**
    * Builds the parents of node up to and including the root node,
    * where the original node is the last element in the returned array.
    * The length of the returned array gives the node's depth in the
    * tree.
    *
    * @param aNode  the TreeNode to get the path for
    * @param depth  an int giving the number of steps already taken towards
    *        the root (on recursive calls), used to size the returned array
    * @return an array of TreeNodes giving the path from the root to the
    *         specified node
    */
   protected TreeNode[] getPathToRoot(TreeNode aNode, int depth)
   {
      TreeNode[]              retNodes;
      // This method recurses, traversing towards the root in order
      // size the array. On the way back, it fills in the nodes,
      // starting from the root and working back to the original node.

      /* Check for null, in case someone passed in a null node, or
         they passed in an element that isn't rooted at root. */
      if (aNode == null)
      {
         if (depth == 0)
         {
            return null;
         }
         else
         {
            retNodes = new TreeNode[depth];
         }
      }
      else
      {
         depth++;
         if (aNode == getRoot())
         {
            retNodes = new TreeNode[depth];
         }
         else
         {
            retNodes = getPathToRoot(aNode.getParent(), depth);
         }
         retNodes[retNodes.length - depth] = aNode;
      }
      return retNodes;
   }

   /**
    * Gets the node for the given object.
    */
   public DefaultMutableTreeNode getNodeFor(Object value)
   {
      return getNodeFor(value, (DefaultMutableTreeNode)getRoot());
   }

   /**
    * Gets the node for the given object
    */
   private DefaultMutableTreeNode getNodeFor(Object obj, DefaultMutableTreeNode node)
   {
      // Check if we found a match
      if (node.getUserObject() == obj)
      {
         return node;
      }

      // Check all children of the current node
      for (Enumeration e = node.children(); e.hasMoreElements(); )
      {
         DefaultMutableTreeNode child = (DefaultMutableTreeNode)e.nextElement();
         DefaultMutableTreeNode result = getNodeFor(obj, child);
         if (result != null)
         {
            return result;
         }
      }

      // Didn't find anything :(
      return null;
   }

   private ConfigChunk chunk = null;

   private String[] columnNames = new String[] { "Property", "Value" };

   private Class[] columnClasses = new Class[] { TreeTableModel.class
                                               , Object.class };
}
