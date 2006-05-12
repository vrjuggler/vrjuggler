/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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
package org.vrjuggler.jccl.editors;

import java.awt.*;
import java.awt.event.*;
import java.beans.*;
import java.util.Iterator;
import java.util.Map;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.table.*;
import javax.swing.tree.*;
import org.vrjuggler.tweek.ui.JTreeTable;
import org.vrjuggler.tweek.ui.treetable.TreeTableModel;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.editors.event.*;

/**
 * A property sheet for a configuration element.
 */
public class ConfigElementPropertySheet
   extends JTreeTable
{
   public ConfigElementPropertySheet()
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
      getTree().setCellRenderer(new ElementTreeCellRenderer());
      getTree().putClientProperty("JTree.lineStyle", "Angled");
      getTree().getSelectionModel().setSelectionMode(
                     TreeSelectionModel.SINGLE_TREE_SELECTION);
      getTree().setRootVisible(false);
      getTree().setToggleClickCount(1);

      // Setup the table model
      mTableModel = new ConfigElementTreeTableModel();
      setModel(mTableModel);

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
      return mTableModel;
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

         // First row is the element name, second is element type
         if (row == 0 || row == 1)
         {
            return new DefaultTableCellRenderer();
         }
         // Nodes containing a PropertyDefinition do NOT render the summary
         else if (value instanceof PropertyDefinition)
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
            PropertyDefinition prop_def = getPropertyDefinitionForRow(row);
            if ((prop_def != null)
               && (prop_def.getEnumsCount() > 0)
               && (prop_def.getType() != ConfigElementPointer.class)
               && (prop_def.getType() != ConfigElement.class))
            {
               Map enums = prop_def.getEnums();
               for (Iterator itr = enums.keySet().iterator(); itr.hasNext(); )
               {
                  final String label = (String)itr.next();
                  if (enums.get(label).equals(value))
                  {
                     // We found an enumeration for the value; create a renderer
                     // that will use the label rather than the value.
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
                           setText(label);
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

      // First row is always the element name
      if (row == 0)
      {
         mElementCellEditor.setPropertyDefinition(null);
         return mElementCellEditor;
      }

      // Get the property definition for the row in question
      PropertyDefinition prop_def = getPropertyDefinitionForRow(row);
      if (prop_def == null)
      {
         return super.getCellEditor(row, col);
      }

      mElementCellEditor.setPropertyDefinition(prop_def);

      return mElementCellEditor;
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

      // The second row is always the element type ... and is not editable
      if (row == 1)
      {
         return false;
      }

      // Get the value stored in the node for the row in question
      TreePath path = getTree().getPathForRow(row);
      DefaultMutableTreeNode node = (DefaultMutableTreeNode)path.getLastPathComponent();
      Object value = node.getUserObject();

      if (value instanceof PropertyDefinition)
      {
         if (((PropertyDefinition)value).getType() != ConfigElement.class)
         {
            // Property definitions are only editable if they are not expanded
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
      mListenerList.add(ActionListener.class, listener);
   }

   public void removeActionListener(ActionListener listener)
   {
      mListenerList.remove(ActionListener.class, listener);
   }

   private void fireActionEvent()
   {
      Object[] listeners = mListenerList.getListenerList();
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
    * Gets the PropertyDefinition most appropriate for the currently selected
    * object in this table.
    */
   public PropertyDefinition getSelectedProperty()
   {
      ListSelectionModel lsm = getSelectionModel();
      if (lsm.isSelectionEmpty())
      {
         return null;
      }
      else
      {
         int row = lsm.getMinSelectionIndex();
         return getPropertyDefinitionForRow(row);
      }
   }

   /**
    * Gets the PropertyDefinition most approriate for the object at the given
    * row.
    */
   private PropertyDefinition getPropertyDefinitionForRow(int row)
   {
      TreePath path = getTree().getPathForRow(row);
      DefaultMutableTreeNode node = (DefaultMutableTreeNode)path.getLastPathComponent();
      Object value = node.getUserObject();

      // If the node contains the property definition, return it
      if (value instanceof PropertyDefinition)
      {
         return (PropertyDefinition)value;
      }
      // If the node contains a config element, return the parent
      if (value instanceof ConfigElement)
      {
         DefaultMutableTreeNode parent_node = (DefaultMutableTreeNode)node.getParent();
         // If the node doesn't have a parent, there is no property definition
         if (parent_node == null)
         {
            return null;
         }
         else
         {
            return (PropertyDefinition)parent_node.getUserObject();
         }
      }
      // Otherwise it's a property value
      else
      {
         DefaultMutableTreeNode parent_node = (DefaultMutableTreeNode)node.getParent();
         Object parent = parent_node.getUserObject();
         if (parent instanceof PropertyDefinition)
         {
            return (PropertyDefinition)parent;
         }
         else if (parent instanceof ConfigElement)
         {
            ConfigElement parent_elt = (ConfigElement)parent;
            int idx = parent_node.getIndex(node);

            // If the parent is the root, take into account the first two
            // special child nodes for the name and type of the element
            if (parent_node == mTableModel.getRoot())
            {
               idx -= 2;
            }
            // Make sure the index is out of bounds
            if ((idx >= 0) && (idx < parent_elt.getDefinition().getPropertyDefinitionsCount()))
            {
               return (PropertyDefinition)parent_elt.getDefinition().getPropertyDefinitions().get(idx);
            }
            return null;
         }
         return null;
      }
   }

   /**
    * Sets the ConfigElement that this property sheet is editing. This will
    * cause the property sheet to update itself to reflect the change in the
    * data that it is editing.
    *
    * @param elt        the ConfigElement to begin editing
    */
   public void setElement(ConfigElement elt)
   {
      // If the user is currently editing a property, force the editor to stop
      // editing first.
      TableCellEditor editor = getCellEditor();
      if (editor != null)
      {
         editor.stopCellEditing();
      }

      // Modify the config element in the data model
      mTableModel.setElement(elt);

      // Ensure that all the top-level properties are visible
      getTree().expandPath(new TreePath(mTableModel.getRoot()));
   }

   /**
    * Gets the ConfigElement that this property sheet is currently editing.
    *
    * @return  the ConfigElement being edited
    */
   public ConfigElement getElement()
   {
      return mTableModel.getElement();
   }

   /**
    * This method is called by the EmbeddedElementCellEditor to let the property
    * sheet know that an embedded ConfigElement hyperlink has been selected.
    */
   public void embeddedElementSelected(ConfigElement elt)
   {
      fireEmbeddedElementSelected(elt);
   }

   /**
    * Notifies all listeners that the given embedded ConfigElement has been
    * selected.
    */
   protected void fireEmbeddedElementSelected(ConfigElement elt)
   {
      Object[] listeners = mListenerList.getListenerList();
      ConfigElementSelectionEvent e = new ConfigElementSelectionEvent(this, elt);
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == ConfigElementSelectionListener.class)
         {
            ((ConfigElementSelectionListener)listeners[i+1]).configElementSelected(e);
         }
      }
   }

   public void addConfigElementSelectionListener(ConfigElementSelectionListener listener)
   {
      mListenerList.add(ConfigElementSelectionListener.class, listener);
   }

   public void removeConfigElementSelectionListener(ConfigElementSelectionListener listener)
   {
      mListenerList.remove(ConfigElementSelectionListener.class, listener);
   }

   /**
    * The table model for the property sheet.
    */
   private ConfigElementTreeTableModel mTableModel;

   /**
    * All listeners interested in this model.
    */
   private EventListenerList mListenerList = new EventListenerList();

   /**
    * The cell editor that is reused each time a value cell is edited.
    */
   private ConfigElementCellEditor mElementCellEditor = new ConfigElementCellEditor();

   /**
    * Specialized renderer for a ConfigElement tree.
    */
   class ElementTreeCellRenderer
      extends DefaultTreeCellRenderer
   {
      public ElementTreeCellRenderer()
      {
         ClassLoader loader = getClass().getClassLoader();
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

         // Display the name of ConfigElements
         if (value instanceof ConfigElement)
         {
            setText(((ConfigElement)value).getName());
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
         // Display the name of PropertyDefinitions
         else if (value instanceof PropertyDefinition)
         {
            setText(((PropertyDefinition)value).getName());
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

               if (parent instanceof ConfigElement)
               {
                  ConfigDefinition def = ((ConfigElement)parent).getDefinition();
                  PropertyDefinition prop_def = (PropertyDefinition)def.getPropertyDefinitions().get(idx);
                  setText(prop_def.getName());
               }
               else if (parent instanceof PropertyDefinition)
               {
                  PropertyDefinition prop_def = (PropertyDefinition)parent;
                  int num_values = prop_def.getPropertyValueDefinitionCount();

                  if (prop_def.isVariable() &&
                     (idx >= num_values))
                  {
                     // We're looking at a value past the number of defined
                     // items. Just use the last defined one by default.
                     idx = num_values - 1;
                  }

                  PropertyValueDefinition value_def =
                     (PropertyValueDefinition)prop_def.getPropertyValueDefinitions().get(idx);
                  setText(value_def.getLabel());
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

   public static void main(String[] args)
      throws Exception
   {
      java.io.File file = new java.io.File(args[0]);
      org.vrjuggler.jccl.config.io.ConfigurationReader reader =
         new org.vrjuggler.jccl.config.io.ConfigurationReader(file, new ConfigDefinitionRepository());
      Configuration config = reader.readConfiguration();


      ConfigElementPropertySheet sheet = new ConfigElementPropertySheet();
      sheet.setElement((ConfigElement)config.getElements().get(0));

      JFrame frame = new JFrame("Test");
      frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
      frame.getContentPane().setLayout(new BorderLayout());
      frame.getContentPane().add(sheet, BorderLayout.CENTER);

      frame.pack();
      frame.setVisible(true);
   }
}
