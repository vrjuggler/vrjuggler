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
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.table.*;
import javax.swing.tree.*;
import org.vrjuggler.jccl.config.ConfigChunk;
import org.vrjuggler.jccl.config.DescEnum;
import org.vrjuggler.jccl.config.PropertyDesc;
import org.vrjuggler.jccl.config.ValType;
import org.vrjuggler.tweek.beans.loader.BeanJarClassLoader;
import org.vrjuggler.tweek.ui.JTreeTable;
import org.vrjuggler.tweek.ui.treetable.TreeTableModel;

/**
 * A property sheet for a config chunk object.
 */
public class ConfigChunkPropertySheet
   extends JTreeTable
{
   public ConfigChunkPropertySheet()
   {
      // Populate the PropertyEditorManager's list of valid editors
      PropertyEditorManager.registerEditor(Boolean.class, BooleanEditor.class);
      PropertyEditorManager.registerEditor(String.class, StringEditor.class);
      PropertyEditorManager.registerEditor(Integer.class, IntegerEditor.class);
      PropertyEditorManager.registerEditor(Float.class, FloatEditor.class);

      // Make resizing work intuitively
      setAutoResizeMode(JTable.AUTO_RESIZE_ALL_COLUMNS);

      // Setup the table defaults
      setShowGrid(true);
      setIntercellSpacing(new Dimension(1,1));
      setBackground(UIManager.getColor("Menu"));
      setRowHeight(getRowHeight()+4);
      getSelectionModel().setSelectionMode(ListSelectionModel.SINGLE_SELECTION);

      // Setup the tree
      getTree().setCellRenderer(new ChunkTreeCellRenderer());
      getTree().putClientProperty("JTree.lineStyle", "Angled");
      getTree().getSelectionModel().setSelectionMode(
                     TreeSelectionModel.SINGLE_TREE_SELECTION);
      getTree().setRootVisible(false);
      getTree().setToggleClickCount(1);

      // Setup the table model
      tableModel = new ConfigChunkTreeTableModel();
      setModel(tableModel);

      // Make sure we know when the selection changes
      ListSelectionModel rowSM = getSelectionModel();
      rowSM.addListSelectionListener(new ListSelectionListener()
      {
         public void valueChanged(ListSelectionEvent e)
         {
            if (! e.getValueIsAdjusting())
            {
               fireActionEvent();
            }
         }
      });
   }

   /**
    * Gets the TreeTableModel this property sheet is using.
    */
   public TreeTableModel getTreeTableModel()
   {
      return tableModel;
   }

   /**
    * Gets the cell renderer for the cell at (col, row). This is overridden such
    * that parent nodes (that print the summary value list) for multi-valued
    * properties do not render the summary when they have been expanded.
    */
   public TableCellRenderer getCellRenderer(int row, int col)
   {
      if (col == 1)
      {
         TreePath path = getTree().getPathForRow(row);
         DefaultMutableTreeNode node = (DefaultMutableTreeNode)path.getLastPathComponent();
         Object value = node.getUserObject();

         // First row is the chunk name, second is chunk type
         if (row == 0 || row == 1)
         {
            return new DefaultTableCellRenderer();
         }
         // Nodes containing a PropertyDesc do NOT render the summary
         else if (value instanceof PropertyDesc)
         {
            if (getTree().isExpanded(path))
            {
               return new DefaultTableCellRenderer()
               {
                  public Component getTableCellRendererComponent(JTable table,
                                                                 Object value,
                                                                 boolean selected,
                                                                 boolean focused,
                                                                 int row, int col)
                  {
                     super.getTableCellRendererComponent(table, value, selected,
                                                         focused, row, col);
                     setText("");
                     return this;
                  }
               };
            }
         }
         else
         {
            // Make sure we render the enumerated name when available
            PropertyDesc prop_desc = getPropertyDescForRow(row);
            if ((prop_desc != null)
               && (prop_desc.getNumEnums() > 0)
               && (prop_desc.getValType() != ValType.CHUNK)
               && (prop_desc.getValType() != ValType.EMBEDDEDCHUNK))
            {
               for (int i=0; i<prop_desc.getNumEnums(); ++i)
               {
                  final DescEnum de = prop_desc.getEnumAt(i);
                  if (de.getValue().equals(value))
                  {
                     return new DefaultTableCellRenderer()
                     {
                        public Component getTableCellRendererComponent(JTable table,
                                                                       Object value,
                                                                       boolean selected,
                                                                       boolean focused,
                                                                       int row, int col)
                        {
                           super.getTableCellRendererComponent(table, value, selected,
                                                               focused, row, col);
                           setText(de.getName());
                           return this;
                        }
                     };
                  }
               }
            }
         }
      }

      return super.getCellRenderer(row, col);
   }

   /**
    * Gets the cell editor for the cell at (col, row).
    */
   public TableCellEditor getCellEditor(int row, int col)
   {
      // The first column is the tree; use the default editor
      if (col == 0)
      {
         return super.getCellEditor(row, col);
      }

      // First row is always the chunk name
      if (row == 0)
      {
         chunkCellEditor.setPropertyDesc(null);
         return chunkCellEditor;
      }

      // Get the property desc for the row in question
      PropertyDesc prop_desc = getPropertyDescForRow(row);
      if (prop_desc == null)
      {
         return super.getCellEditor(row, col);
      }

      chunkCellEditor.setPropertyDesc(prop_desc);

      return chunkCellEditor;
   }

   /**
    * Tests if the cell at (col, row) is editable. This is overridden such that
    * parent nodes (that print the summary value list) for multi-valued
    * properties are not editable when they have been expanded.
    */
   public boolean isCellEditable(int row, int col)
   {
      // The first column is the tree; let the parent class decide
      if (col == 0)
      {
         return super.isCellEditable(row, col);
      }

      // The second row is always the chunk type ... and is not editable
      if (row == 1)
      {
         return false;
      }

      // Get the value stored in the node for the row in question
      TreePath path = getTree().getPathForRow(row);
      DefaultMutableTreeNode node = (DefaultMutableTreeNode)path.getLastPathComponent();
      Object value = node.getUserObject();

      if (value instanceof PropertyDesc)
      {
         if (((PropertyDesc)value).getValType() != ValType.EMBEDDEDCHUNK)
         {
            // Property descs are only editable if they are not expanded
            return ! getTree().isExpanded(path);
         }
         else
         {
            return false;
         }
      }
      else
      {
         return super.isCellEditable(row, col);
      }
   }

   public void addActionListener(ActionListener listener)
   {
      listenerList.add(ActionListener.class, listener);
   }

   public void removeActionListener(ActionListener listener)
   {
      listenerList.remove(ActionListener.class, listener);
   }

   private void fireActionEvent()
   {
      Object[] listeners = listenerList.getListenerList();
      ActionEvent e = new ActionEvent(this, 0, "PropertySelectionChanged");
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == ActionListener.class)
         {
            ((ActionListener)listeners[i+1]).actionPerformed(e);
         }
      }
   }

   /**
    * Gets the currently selected object in the tree that serves as the basis for
    * this property sheet.
    */
   public Object getSelectedObject()
   {
      ListSelectionModel lsm = getSelectionModel();
      if (lsm.isSelectionEmpty())
      {
         return null;
      }
      else
      {
         int row = lsm.getMinSelectionIndex();
         TreePath path = getTree().getPathForRow(row);
         DefaultMutableTreeNode node = (DefaultMutableTreeNode)path.getLastPathComponent();
         return node.getUserObject();
      }
   }

   /**
    * Gets the PropertyDesc most appropriate for the currently selected object
    * in this table.
    */
   public PropertyDesc getSelectedProperty()
   {
      ListSelectionModel lsm = getSelectionModel();
      if (lsm.isSelectionEmpty())
      {
         return null;
      }
      else
      {
         int row = lsm.getMinSelectionIndex();
         return getPropertyDescForRow(row);
      }
   }

   /**
    * Gets the PropertyDesc most approriate for the object at the given row.
    */
   private PropertyDesc getPropertyDescForRow(int row)
   {
      TreePath path = getTree().getPathForRow(row);
      DefaultMutableTreeNode node = (DefaultMutableTreeNode)path.getLastPathComponent();
      Object value = node.getUserObject();

      // If the node contains the property desc, return it
      if (value instanceof PropertyDesc)
      {
         return (PropertyDesc)value;
      }
      // If the node contains a config chunk, return the parent
      if (value instanceof ConfigChunk)
      {
         DefaultMutableTreeNode parent_node = (DefaultMutableTreeNode)node.getParent();
         // If the node doesn't have a parent, there is no property desc
         if (parent_node == null)
         {
            return null;
         }
         else
         {
            return (PropertyDesc)parent_node.getUserObject();
         }
      }
      // Otherwise it's a property value
      else
      {
         DefaultMutableTreeNode parent_node = (DefaultMutableTreeNode)node.getParent();
         Object parent = parent_node.getUserObject();
         if (parent instanceof PropertyDesc)
         {
            return (PropertyDesc)parent;
         }
         else if (parent instanceof ConfigChunk)
         {
            ConfigChunk parent_chunk = (ConfigChunk)parent;
            int idx = parent_node.getIndex(node);

            // If the parent is the root, take into account the first two
            // special child nodes for the name and type of the chunk
            if (parent_node == tableModel.getRoot())
            {
               idx -= 2;
            }
            // Make sure the index is out of bounds
            if ((idx >= 0) && (idx < parent_chunk.getDesc().propertyDescsSize()))
            {
               return parent_chunk.getDesc().getPropertyDesc(idx);
            }
            return null;
         }
         return null;
      }
   }

   /**
    * Sets the ConfigChunk that this property sheet is editing. This will cause
    * the property sheet to update itself to reflect the change in the data that
    * it is editing.
    *
    * @param chunk      the ConfigChunk to begin editing
    */
   public void setConfigChunk(ConfigChunk chunk)
   {
      // If the user is currently editing a property, force the editor to stop
      // editing first.
      TableCellEditor editor = getCellEditor();
      if (editor != null)
      {
         editor.stopCellEditing();
      }

      // Modify the config chunk in the data model
      tableModel.setConfigChunk(chunk);

      // Ensure that all the top-level properties are visible
      getTree().expandPath(new TreePath(tableModel.getRoot()));
   }

   /**
    * Gets the ConfigChunk that this property sheet is currently editing.
    *
    * @return  the ConfigChunk being edited
    */
   public ConfigChunk getConfigChunk()
   {
      return tableModel.getConfigChunk();
   }

   /**
    * This method is called by the EmbeddedChunkCellEditor to let the property
    * sheet know that an embedded ConfigChunk hyperlink has been selected.
    */
   public void embeddedChunkSelected(ConfigChunk chunk)
   {
      fireEmbeddedChunkSelected(chunk);
   }

   /**
    * Notifies all listeners that the given embedded ConfigChunk has been
    * selected.
    */
   protected void fireEmbeddedChunkSelected(ConfigChunk chunk)
   {
      Object[] listeners = listenerList.getListenerList();
      ChunkSelectionEvent e = new ChunkSelectionEvent(this, chunk);
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == ChunkSelectionListener.class)
         {
            ((ChunkSelectionListener)listeners[i+1]).chunkSelected(e);
         }
      }
   }

   public void addChunkSelectionListener(ChunkSelectionListener listener)
   {
      listenerList.add(ChunkSelectionListener.class, listener);
   }

   public void removeChunkSelectionListener(ChunkSelectionListener listener)
   {
      listenerList.remove(ChunkSelectionListener.class, listener);
   }

   /**
    * The table model for the property sheet.
    */
   private ConfigChunkTreeTableModel tableModel;

   /**
    * All listeners interested in this model.
    */
   private EventListenerList listenerList = new EventListenerList();

   /**
    * The cell editor that is reused each time a value cell is edited.
    */
   private ConfigChunkCellEditor chunkCellEditor = new ConfigChunkCellEditor();

   /**
    * Specialized renderer for a ConfigChunk tree.
    */
   class ChunkTreeCellRenderer
      extends DefaultTreeCellRenderer
   {
      public ChunkTreeCellRenderer()
      {
         ClassLoader loader = BeanJarClassLoader.instance();
         mExpandIcon = new ImageIcon(loader.getResource("org/vrjuggler/jccl/editors/images/expand_property.gif"));
         mCollapseIcon = new ImageIcon(loader.getResource("org/vrjuggler/jccl/editors/images/collapse_property.gif"));
      }

      public Component getTreeCellRendererComponent(
                           JTree tree, Object node, boolean selected,
                           boolean expanded, boolean leaf, int row,
                           boolean focused)
      {
         // Get the default settings from the UI LAF
         super.getTreeCellRendererComponent(tree, node, selected,
                                            expanded, leaf, row, focused);
         setFont(tree.getFont());

         setBackgroundNonSelectionColor(tree.getBackground());

         // Pull out the data from the tree node
         DefaultMutableTreeNode tree_node = (DefaultMutableTreeNode)node;
         DefaultMutableTreeNode parent_node = (DefaultMutableTreeNode)tree_node.getParent();
         Object value = ((DefaultMutableTreeNode)node).getUserObject();

         // Check if this is the first or second child of the root (special cases)
         if (parent_node == tree.getModel().getRoot())
         {
            int idx = parent_node.getIndex(tree_node);
            if (idx == 0)
            {
               setFont(tree.getFont().deriveFont(Font.BOLD));
               setText("Name");
               setIcon(null);
               return this;
            }
            else if (idx == 1)
            {
               setFont(tree.getFont().deriveFont(Font.BOLD));
               setText("Type");
               setIcon(null);
               return this;
            }
         }

         // Display the name of ConfigChunks
         if (value instanceof ConfigChunk)
         {
            setText(((ConfigChunk)value).getName());
            setHorizontalTextPosition(JLabel.LEFT);
            if (expanded)
            {
               setIcon(mCollapseIcon);
            }
            else
            {
               setIcon(mExpandIcon);
            }
         }
         // Display the name of PropertyDescs
         else if (value instanceof PropertyDesc)
         {
            setText(((PropertyDesc)value).getName());
            setHorizontalTextPosition(JLabel.LEFT);
            if (expanded)
            {
               setIcon(mCollapseIcon);
            }
            else
            {
               setIcon(mExpandIcon);
            }
         }
         else
         {
            setIcon(null);
            if (parent_node != null)
            {
               Object parent = parent_node.getUserObject();
               int idx = parent_node.getIndex(tree_node);

               // If the parent node is the root node, we need to account for
               // there being the two extra nodes at the beginning of this tree
               if (parent_node == tree.getModel().getRoot())
               {
                  idx -= 2;
               }

               if (parent instanceof ConfigChunk)
               {
                  String name = ((ConfigChunk)parent).getDesc().getPropertyDesc(idx).getName();
                  setText(name);
               }
               else if (parent instanceof PropertyDesc)
               {
                  PropertyDesc prop_desc = (PropertyDesc)parent;

                  if (prop_desc.hasVariableNumberOfValues() &&
                     (idx >= prop_desc.getItemsSize()))
                  {
                     // We're looking at a value past the number of defined
                     // items. Just use the last defined one by default.
                     idx = prop_desc.getItemsSize() - 1;
                  }

                  PropertyDesc.Item item = (PropertyDesc.Item)prop_desc.getItems().get(idx);
                  setText(item.getLabel());
               }
               else
               {
                  setText(value.toString());
               }
            }
         }
         return this;
      }

      private Icon mExpandIcon;
      private Icon mCollapseIcon;
   }
}
