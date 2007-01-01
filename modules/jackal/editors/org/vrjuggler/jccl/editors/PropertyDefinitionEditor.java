/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Frame;
import java.awt.Rectangle;
import java.awt.event.*;
import java.beans.*;
import java.util.*;
import javax.swing.*;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import javax.swing.table.*;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.config.event.*;
import org.vrjuggler.jccl.config.event.PropertyDefinitionEvent;

/**
 * Describes an editor component bean useful for editing property definitions.
 */
public class PropertyDefinitionEditor
   extends JTabbedPane
{
   /**
    * Create a new editor for property definitions. This editor has no property
    * definition to edit by default.
    */
   public PropertyDefinitionEditor()
   {
      // JBuilder can't handle BoxLayout ...
      setupGeneralTab();
      setupEnumTab();
      setupAllowedTypesTab();

      try
      {
         jbInit();
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }

      PropertyEditorManager.registerEditor(Class.class, ValuetypeEditor.class);

      // Setup the basic table
      basicTable.setModel(basicTableModel);
      basicTable.setBackground(UIManager.getColor("Menu"));
      basicTable.getColumnModel().getColumn(0).
            setCellRenderer(new BoldCellRenderer());
      basicTable.getColumnModel().getColumn(1).
            setCellRenderer(new BasicTypeRenderer());
      basicTable.getColumnModel().getColumn(1).
            setCellEditor(new BasicCellEditor());

      // Setup the items table
      itemsTable.setModel(itemsTableModel);
      itemsTable.setBackground(UIManager.getColor("Menu"));
      itemsTable.getColumnModel().getColumn(0).
            setCellRenderer(new BoldCellRenderer());
      itemsTable.getColumnModel().getColumn(1).
            setCellRenderer(new ItemsDefaultValueCellRenderer());

      // Setup the enums table
      enumTable.setModel(enumsTableModel);
      enumTable.setBackground(UIManager.getColor("Menu"));

      // Setup the allowed types list
      allowedTypesList.setModel(allowedTypesListModel);
      allowedTypesList.addListSelectionListener(
         new ListSelectionListener()
         {
            public void valueChanged(ListSelectionEvent e)
            {
               allowedTypeSelectionChanged(e);
            }
         }
      );

      // Setup the inital property definition to null
      setPropertyDefinition(null);

      // Listener support on the text area
      helpTextArea.addFocusListener(new FocusListener()
      {
         public void focusGained(FocusEvent evt)
         {
         }

         public void focusLost(FocusEvent evt)
         {
            mPropDef.setHelp(helpTextArea.getText());
            helpPreviewPanel.setText(mPropDef.getHelp());
            System.out.println("Updating property desc help");
         }
      });
   }

   public void setPropertyDefinition(PropertyDefinition propDef)
   {
      if (mPropDef != null)
      {
         mPropDef.removePropertyDefinitionListener(defChangeListener);
      }
      mPropDef = propDef;
      if (mPropDef != null)
      {
         mPropDef.addPropertyDefinitionListener(defChangeListener);
      }
      basicTableModel.setPropertyDefinition(mPropDef);
      itemsTableModel.setPropertyDefinition(mPropDef);
      enumsTableModel.setPropertyDefinition(mPropDef);
      allowedTypesListModel.setPropertyDefinition(mPropDef);
      if (mPropDef != null)
      {
         helpTextArea.setText(mPropDef.getHelp());
         helpPreviewPanel.setText(mPropDef.getHelp());
      }
      else
      {
         helpTextArea.setText("");
         helpPreviewPanel.setText("");
      }

      // Enable the enum tab only if the type is not element pointer,
      // embedded element, or boolean.
      boolean enable_enum = false;
      if (mPropDef != null)
      {
         Class type = mPropDef.getType();
         if (type != ConfigElementPointer.class &&
             type != ConfigElement.class &&
             type != Boolean.class)
         {
            enable_enum = true;
         }
      }
      setEnabledAt(indexOfTab("Enumerations"), enable_enum);

      // Enable the allowed types tab only if the type is a element pointer or
      // embedded element.
      boolean enable_allowed_types = false;
      if (mPropDef != null)
      {
         Class type = mPropDef.getType();
         if (type == ConfigElementPointer.class ||
             type == ConfigElement.class)
         {
            enable_allowed_types = true;
         }
      }
      setEnabledAt(indexOfTab("Allowed Types"), enable_allowed_types);

      revalidate();
   }

   public void revalidate()
   {
      // Tweak the max sizes of the components

      if (basicTable != null)
      {
         Dimension maxSize = basicTable.getPreferredSize();
         maxSize.width = Integer.MAX_VALUE;
         basicTable.setPreferredScrollableViewportSize(maxSize);
         basicScrollPane.setMaximumSize(maxSize);
      }

      if (itemsTable != null)
      {
         Dimension maxSize = itemsTable.getPreferredSize();
         maxSize.width = Integer.MAX_VALUE;
         itemsTable.setPreferredScrollableViewportSize(maxSize);
         itemsScrollPane.setMaximumSize(maxSize);
      }

      if (enumTable != null)
      {
         Dimension maxSize = enumTable.getPreferredSize();
         maxSize.width = Integer.MAX_VALUE;
         enumTable.setPreferredScrollableViewportSize(maxSize);
         enumScrollPane.setMaximumSize(maxSize);
      }

/*
      if ( allowedTypesList != null )
      {
         Dimension maxSize = allowedTypesList.getPreferredSize();
         maxSize.width = Integer.MAX_VALUE;
         allowedTypesList.setPreferredScrollableViewportSize(maxSize);
         allowedTypesScrollPane.setMaximumSize(maxSize);
      }
*/

      super.revalidate();
   }

   public PropertyDefinition getPropertyDefinition()
   {
      return mPropDef;
   }

   /**
    * JBuilder can't handle BoxLayout which the General tab requires, so we set
    * it up here for JBuilder.
    */
   private void setupGeneralTab()
   {
      basicScrollPane.setViewportView(basicTable);
      basicScrollPane.setAlignmentX(LEFT_ALIGNMENT);
      itemsScrollPane.setViewportView(itemsTable);
      itemsScrollPane.setAlignmentX(LEFT_ALIGNMENT);

      JLabel basic_lbl = new JLabel("Basic Properties:");
      basic_lbl.setAlignmentX(LEFT_ALIGNMENT);
      JLabel items_lbl = new JLabel("Items:");
      items_lbl.setAlignmentX(LEFT_ALIGNMENT);

      // Setup the items button panel
      addItemBtn.setText("Add");
      removeItemBtn.setText("Remove");
      addItemBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            addValueDefinition();
         }
      });
      removeItemBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            removeValueDefinition();
         }
      });
      JPanel itemsButtonPane = new JPanel();
      itemsButtonPane.setAlignmentX(LEFT_ALIGNMENT);
      itemsButtonPane.setBorder(BorderFactory.createEmptyBorder(0, 5, 0, 5));
      itemsButtonPane.setLayout(new BoxLayout(itemsButtonPane, BoxLayout.X_AXIS));
      itemsButtonPane.add(Box.createHorizontalGlue());
      itemsButtonPane.add(addItemBtn);
      itemsButtonPane.add(Box.createHorizontalStrut(10));
      itemsButtonPane.add(removeItemBtn);

      Box box = new Box(BoxLayout.Y_AXIS);
      generalTab.setViewportView(box);
      box.add(basic_lbl);
      box.add(basicScrollPane);
      box.add(Box.createVerticalStrut(12));
      box.add(items_lbl);
      box.add(itemsScrollPane);
      box.add(itemsButtonPane);
      box.add(Box.createVerticalGlue());
   }

   /**
    * JBuilder can't handle BoxLayout which the Enums tab requires, so we set it
    * up here for JBuilder.
    */
   private void setupEnumTab()
   {
      enumEditableButton.setText("Editable");
      enumEditableButton.setSelected(false);
      enumEditableButton.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            changeEnumEditableState(enumEditableButton.isSelected());
         }
      });

      // Setup the enum table
      enumScrollPane.setViewportView(enumTable);
      enumScrollPane.setAlignmentX(LEFT_ALIGNMENT);

      // Setup the enum button panel
      addEnumBtn.setText("Add");
      removeEnumBtn.setText("Remove");
      addEnumBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            addEnum();
         }
      });
      removeEnumBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            removeEnum();
         }
      });
      JPanel enumButtonPane = new JPanel();
      enumButtonPane.setAlignmentX(LEFT_ALIGNMENT);
      enumButtonPane.setBorder(BorderFactory.createEmptyBorder(0, 5, 0, 5));
      enumButtonPane.setLayout(new BoxLayout(enumButtonPane, BoxLayout.X_AXIS));
      enumButtonPane.add(Box.createHorizontalGlue());
      enumButtonPane.add(addEnumBtn);
      enumButtonPane.add(Box.createHorizontalStrut(10));
      enumButtonPane.add(removeEnumBtn);

      // Setup the base part of this panel
      Box box = new Box(BoxLayout.Y_AXIS);
      enumTab.setViewportView(box);
      box.add(enumEditableButton);
      box.add(enumScrollPane);
      box.add(enumButtonPane);
      box.add(Box.createVerticalGlue());
   }

   /**
    * JBuilder can't handle BoxLayout which the Allowed Types tab requires, so
    * we set it up here for JBuilder.
    */
   private void setupAllowedTypesTab()
   {
      // Setup the allowed types table
      allowedTypesScrollPane.setViewportView(allowedTypesList);
      allowedTypesScrollPane.setAlignmentX(LEFT_ALIGNMENT);

      // Setup the allowed types button panel
      addAllowedTypeBtn.setText("Add");
      removeAllowedTypeBtn.setEnabled(false);
      removeAllowedTypeBtn.setText("Remove");
      addAllowedTypeBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            addAllowedType();
         }
      });
      removeAllowedTypeBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            removeAllowedType();
         }
      });
      JPanel allowedTypesButtonPane = new JPanel();
      allowedTypesButtonPane.setAlignmentX(LEFT_ALIGNMENT);
      allowedTypesButtonPane.setBorder(BorderFactory.createEmptyBorder(0, 5, 0, 5));
      allowedTypesButtonPane.setLayout(new BoxLayout(allowedTypesButtonPane, BoxLayout.X_AXIS));
      allowedTypesButtonPane.add(Box.createHorizontalGlue());
      allowedTypesButtonPane.add(addAllowedTypeBtn);
      allowedTypesButtonPane.add(Box.createHorizontalStrut(10));
      allowedTypesButtonPane.add(removeAllowedTypeBtn);

      // Setup the base part of this panel
      Box box = new Box(BoxLayout.Y_AXIS);
      allowedTypesTab.setViewportView(box);
      box.add(allowedTypesScrollPane);
      box.add(allowedTypesButtonPane);
      box.add(Box.createVerticalGlue());
   }

   /**
    * Adds a new property value to the current property definition.
    */
   protected void addValueDefinition()
   {
      PropertyValueDefinition value_def = new PropertyValueDefinition("New Value");
      mPropDef.addPropertyValueDefinition(value_def);
   }

   /**
    * Removes the currently selected property value from the current property
    * definition.
    */
   protected void removeValueDefinition()
   {
      // Get the currently selected item
      int row = itemsTable.getSelectedRow();
      if (row != -1)
      {
         mPropDef.removePropertyValueDefinition(row);
      }
   }

   protected void changeEnumEditableState(boolean editable)
   {
      mPropDef.setEnumEditable(editable);
   }

   /**
    * Adds a new enumeration to the current property definition.
    */
   protected void addEnum()
   {
      Object value = null;

      // Init the default value
      if (mPropDef.getType() == Boolean.class)
      {
         value = new Boolean(false);
      }
      else if (mPropDef.getType() == Integer.class)
      {
         value = new Integer(0);
      }
      else if (mPropDef.getType() == Float.class)
      {
         value = new Float(0.0);
      }
      else if (mPropDef.getType() == String.class)
      {
         value = new String();
      }
      else if (mPropDef.getType() == ConfigElement.class)
      {
         value = null;
      }
      else if (mPropDef.getType() == ConfigElementPointer.class)
      {
         value = new ConfigElementPointer("<Please choose a target>");
      }

      // add the enumeration
      mPropDef.addEnum("New Enumeration", value);
   }

   /**
    * Removes the currently selected enumeration from the current property
    * definition.
    */
   protected void removeEnum()
   {
      int row = enumTable.getSelectedRow();
      if (row != -1)
      {
         String label = enumsTableModel.getLabelAt(row);
         mPropDef.removeEnum(label);
      }
   }

   protected void allowedTypeSelectionChanged(ListSelectionEvent e)
   {
      removeAllowedTypeBtn.setEnabled(allowedTypesList.getSelectedIndex() > -1);
   }

   /**
    * Adds a new allowed type to the current property definition.
    */
   protected void addAllowedType()
   {
      ConfigBroker broker = new ConfigBrokerProxy();
      ConfigDefinitionRepository repos = broker.getRepository();
      java.util.List all_defs = repos.getAllLatest();
      ConfigDefinitionChooser chooser = new ConfigDefinitionChooser();
      chooser.setDefinitions(all_defs);
      Frame parent = (Frame) SwingUtilities.getAncestorOfClass(Frame.class,
                                                               this);

      if ( chooser.showDialog(parent) == ConfigDefinitionChooser.APPROVE_OPTION )
      {
         ConfigDefinition new_type = chooser.getSelectedDefinition();
         if ( new_type != null )
         {
            mPropDef.addAllowedType(new_type.getToken());
         }
      }
   }

   /**
    * Removes the currently selected allowed type from the current property
    * definition.
    */
   protected void removeAllowedType()
   {
      mPropDef.removeAllowedType((String) allowedTypesList.getSelectedValue());
   }

   /**
    * Let Jbuilder init the UI.
    */
   private void jbInit()
      throws Exception
   {
      this.setEnabled(true);
      this.add(generalTab, "General");
      this.add(enumTab, "Enumerations");
      this.add(allowedTypesTab, "Allowed Types");
      this.add(mHelpPanel,  "Help");
      mHelpEditorScrollPane.setViewportView(helpTextArea);
      mHelpPanel.add(mHelpEditorScrollPane, "Editor");
      mHelpPreviewScrollPane.setViewportView(helpPreviewPanel);
      mHelpPanel.add(mHelpPreviewScrollPane, "Preview");
   }

   /**
    * The table model for the basic table.
    */
   BasicTableModel basicTableModel = new BasicTableModel();

   /**
    * The table model for the items table.
    */
   ItemsTableModel itemsTableModel = new ItemsTableModel();

   /**
    * The table model for the enumerations table.
    */
   EnumTableModel enumsTableModel = new EnumTableModel();

   /**
    * The table model for the allowed types table.
    */
   AllowedTypesListModel allowedTypesListModel = new AllowedTypesListModel();

   /**
    * The property definition for this editor.
    */
   PropertyDefinition mPropDef = null;

   /**
    * Listens for changes to the currently active property desc.
    */
   PropertyDefChangeListener defChangeListener = new PropertyDefChangeListener();

   //--- JBuilder generated GUI variables ---//
   private JScrollPane enumTab = new JScrollPane();
   private JScrollPane enumScrollPane = new JScrollPane();
   private JCheckBox enumEditableButton = new JCheckBox();
   private JTable enumTable = new JTable();
   private JScrollPane allowedTypesTab = new JScrollPane();
   private JScrollPane allowedTypesScrollPane = new JScrollPane();
   private JList allowedTypesList = new JList();
   private JTabbedPane mHelpPanel = new JTabbedPane();
   private JScrollPane mHelpEditorScrollPane = new JScrollPane();
   private JTextArea helpTextArea = new JTextArea();
   private JScrollPane mHelpPreviewScrollPane = new JScrollPane();
   private JEditorPane helpPreviewPanel = new JEditorPane("text/html", "");
   private JScrollPane generalTab = new JScrollPane();
   private JScrollPane basicScrollPane = new JScrollPane();
   private JTable basicTable = new JTable();
   private JScrollPane itemsScrollPane = new JScrollPane();
   private JTable itemsTable = new JTable();
   private JButton addItemBtn = new JButton();
   private JButton removeItemBtn = new JButton();
   private JButton addEnumBtn = new JButton();
   private JButton removeEnumBtn = new JButton();
   private JButton addAllowedTypeBtn = new JButton();
   private JButton removeAllowedTypeBtn = new JButton();

   /**
    * Listens to changes to the current property description.
    */
   class PropertyDefChangeListener
      implements PropertyDefinitionListener
   {
      public void tokenChanged(PropertyDefinitionEvent evt)
      {
         revalidate();
      }

      public void typeChanged(PropertyDefinitionEvent evt)
      {
         revalidate();
      }

      public void helpChanged(PropertyDefinitionEvent evt)
      {
         revalidate();
      }

      public void propertyValueDefinitionAdded(PropertyDefinitionEvent evt)
      {
         revalidate();
      }

      public void propertyValueDefinitionRemoved(PropertyDefinitionEvent evt)
      {
         revalidate();
      }

      public void enumEditableChanged(PropertyDefinitionEvent evt)
      {
         revalidate();
      }

      public void enumAdded(PropertyDefinitionEvent evt)
      {
         revalidate();
      }

      public void enumRemoved(PropertyDefinitionEvent evt)
      {
         revalidate();
      }

      public void allowedTypeAdded(PropertyDefinitionEvent evt)
      {
         revalidate();
      }

      public void allowedTypeRemoved(PropertyDefinitionEvent evt)
      {
         revalidate();
      }

      public void variableChanged(PropertyDefinitionEvent evt)
      {
         revalidate();
      }
   }

   /**
    * A table model for the Basic table.
    */
   class BasicTableModel
      extends AbstractTableModel
   {
      public void setPropertyDefinition(PropertyDefinition propDef)
      {
         mPropDef = propDef;
         fireTableDataChanged();
      }

      public PropertyDefinition getPropertyDefinition()
      {
         return mPropDef;
      }

      public String getColumnName(int col)
      {
         return columnNames[col].toString();
      }

      public int getRowCount()
      {
         // We only have rows if we have a property def to model
         if (mPropDef != null)
         {
            return rowNames.length;
         }
         else
         {
            return 0;
         }
      }

      public int getColumnCount() { return columnNames.length; }

      public Object getValueAt(int row, int col)
      {
         // Asking for the row name
         if (col == 0)
         {
            return rowNames[row];
         }
         else
         {
            switch (row)
            {
            case 0: // name
               return mPropDef.getToken();
            case 1: // type
               return mPropDef.getType();
            case 2: // variable
               return new Boolean(mPropDef.isVariable());
            }
         }
         return null;
      }

      public boolean isCellEditable(int row, int col)
      {
         // Only the value column is editable
         return (col == 1);
      }

      public void setValueAt(Object value, int row, int col)
      {
         // Assuming they mean column 1 ...
         switch (row)
         {
         case 0: // name
            String name = (String) value;
            String token = name.trim().toLowerCase().replaceAll("\\s+", "_");
            mPropDef.setToken(token);
            break;
         case 1: // type
            mPropDef.setType((Class)value);
            break;
         case 2: // variable
            mPropDef.setVariable(((Boolean)value).booleanValue());
            break;
         default:
            System.err.println("Bad row: "+row);
         }

         fireTableCellUpdated(row, col);
      }

      /**
       * The names of the columns.
       */
      private String[] columnNames = { "Property", "Value" };

      /**
       * The names of the rows.
       */
      private String[] rowNames = { "Name"
                                  , "Type"
                                  , "Variable"
                                  };

      /**
       * The PropertyDefinition this table model represents.
       */
      private PropertyDefinition mPropDef = null;
   }

   /**
    * A table model for the items table.
    */
   class ItemsTableModel
      extends AbstractTableModel
   {
      public void setPropertyDefinition(PropertyDefinition propDef)
      {
         // Update our listener status
         if (propertyDesc != null)
         {
            propertyDesc.removePropertyDefinitionListener(mChangeListener);
         }
         propertyDesc = propDef;
         if (propertyDesc != null)
         {
            propertyDesc.addPropertyDefinitionListener(mChangeListener);
         }
         fireTableDataChanged();
      }

      public PropertyDefinition getPropertyDefinition()
      {
         return propertyDesc;
      }

      public String getColumnName(int col)
      {
         return columnNames[col].toString();
      }

      public int getRowCount()
      {
         // We only have rows if we have a property desc to model
         if (propertyDesc != null)
         {
            return propertyDesc.getPropertyValueDefinitions().size();
         }
         else
         {
            return 0;
         }
      }

      public int getColumnCount() { return columnNames.length; }

      public Object getValueAt(int row, int col)
      {
         PropertyValueDefinition value_def = getValueDefAt(row);
         if (col == 0)
         {
            return value_def.getLabel();
         }
         else
         {
            return value_def.getDefaultValue();
         }
      }

      public PropertyValueDefinition getValueDefAt(int row)
      {
         List value_defs = propertyDesc.getPropertyValueDefinitions();
         return (PropertyValueDefinition)value_defs.get(row);
      }

      public boolean isCellEditable(int row, int col)
      {
         // Embedded elements cannot have direct default values
         if (propertyDesc.getType() == ConfigElement.class)
         {
            return false;
         }
         return true;
      }

      public void setValueAt(Object value, int row, int col)
      {
         PropertyValueDefinition value_def = getValueDefAt(row);
         if (col == 0)
         {
            value_def.setLabel((String)value);
         }
         else
         {
            value_def.setDefaultValue(value);
         }

         fireTableCellUpdated(row, col);
      }

      /**
       * The names of the columns.
       */
      private String[] columnNames = { "Label", "Default Value" };

      /**
       * The property definition this table model represents.
       */
      private PropertyDefinition propertyDesc = null;

      /** Custom listener for changes to the property definition. */
      private PropertyDefinitionListener mChangeListener = new PropertyDefinitionAdapter()
      {
         public void propertyValueDefinitionAdded(PropertyDefinitionEvent evt)
         {
            // TODO: Someday, fire off an event saying that only the items that
            // actually changed have really changed
            fireTableDataChanged();
         }

         public void propertyValueDefinitionRemoved(PropertyDefinitionEvent evt)
         {
            // TODO: Someday, fire off an event saying that only the items that
            // actually changed have really changed
            fireTableDataChanged();
         }
      };
   }

   /**
    * A table model for the enumerations table.
    */
   class EnumTableModel
      extends AbstractTableModel
   {
      public void setPropertyDefinition(PropertyDefinition propDef)
      {
         // Update our listener status
         if (propertyDesc != null)
         {
            propertyDesc.removePropertyDefinitionListener(mChangeListener);
         }
         propertyDesc = propDef;
         if (propertyDesc != null)
         {
            propertyDesc.addPropertyDefinitionListener(mChangeListener);
         }
         fireTableDataChanged();
      }

      public PropertyDefinition getPropertyDefinition()
      {
         return propertyDesc;
      }

      public String getColumnName(int col)
      {
         return columnNames[col].toString();
      }

      public int getRowCount()
      {
         // We only have rows if we have a property definition to model
         if (propertyDesc != null)
         {
            return propertyDesc.getEnumsCount();
         }
         else
         {
            return 0;
         }
      }

      public int getColumnCount() { return columnNames.length; }

      public Object getValueAt(int row, int col)
      {
         String label = getLabelAt(row);
         if (col == 0)
         {
            return label;
         }
         else
         {
            return propertyDesc.getEnums().get(label);
         }
      }

      public String getLabelAt(int row)
      {
         List labels = new ArrayList(propertyDesc.getEnums().keySet());
         return (String)labels.get(row);
      }

      public boolean isCellEditable(int row, int col)
      {
         return true;
      }

      public void setValueAt(Object value, int row, int col)
      {
         String label = getLabelAt(row);
         Object enum_value = propertyDesc.getEnums().get(label);
         if (col == 0)
         {
            propertyDesc.removeEnum(label);
            propertyDesc.addEnum((String)value, enum_value);
         }
         else
         {
            propertyDesc.removeEnum(label);
            propertyDesc.addEnum(label, value);
         }
         fireTableCellUpdated(row, col);
      }

      /**
       * The names of the columns.
       */
      private String[] columnNames = { "Name", "Value" };

      /**
       * The property definition this table model represents.
       */
      private PropertyDefinition propertyDesc = null;

      /** Custom listener for changes to the property definition. */
      private PropertyDefinitionListener mChangeListener = new PropertyDefinitionAdapter()
      {
         public void enumAdded(PropertyDefinitionEvent evt)
         {
            // TODO: Someday, fire off a more specific event with information
            // about only the enumerations that have actually changed
            fireTableDataChanged();
         }

         public void enumRemoved(PropertyDefinitionEvent evt)
         {
            // TODO: Someday, fire off a more specific event with information
            // about only the enumerations that have actually changed
            fireTableDataChanged();
         }
      };
   }

   /**
    * A list model for the allowed types list.
    */
   private static class AllowedTypesListModel
      extends AbstractListModel
   {
      public Object getElementAt(int index)
      {
         if ( propertyDesc != null )
         {
            return propertyDesc.getAllowedTypes().get(index);
         }
         return null;
      }

      public int getSize()
      {
         if ( propertyDesc != null )
         {
            return propertyDesc.getAllowedTypesCount();
         }
         return 0;
      }

      public void setPropertyDefinition(PropertyDefinition propDef)
      {
         // Update our listener status
         if (propertyDesc != null)
         {
            propertyDesc.removePropertyDefinitionListener(mChangeListener);
            fireContentsChanged(this, 0, propertyDesc.getAllowedTypesCount());
         }

         propertyDesc = propDef;

         if (propertyDesc != null)
         {
            propertyDesc.addPropertyDefinitionListener(mChangeListener);
            fireContentsChanged(this, 0, propertyDesc.getAllowedTypesCount());
         }
      }

      /**
       * The property definition this table model represents.
       */
      private PropertyDefinition propertyDesc = null;

      /** Custom listener for changes to the property definition. */
      private PropertyDefinitionListener mChangeListener =
         new PropertyDefinitionAdapter()
         {
            public void allowedTypeAdded(PropertyDefinitionEvent evt)
            {
               PropertyDefinition def = (PropertyDefinition) evt.getSource();
               int index = def.getAllowedTypes().indexOf(evt.getValue());
               fireContentsChanged(this, index, def.getAllowedTypesCount());
            }

            public void allowedTypeRemoved(PropertyDefinitionEvent evt)
            {
               PropertyDefinition def = (PropertyDefinition) evt.getSource();
               fireContentsChanged(this, 0, def.getAllowedTypesCount());
            }
         };
   }

   /**
    * Cell renderer that makes the cell's font bold.
    */
   class BoldCellRenderer
      extends DefaultTableCellRenderer
   {
      public Component getTableCellRendererComponent(JTable table,
                                                     Object value,
                                                     boolean selected,
                                                     boolean focus,
                                                     int row,
                                                     int col)
      {
         Component comp = super.getTableCellRendererComponent(table, value,
                                                              selected, focus,
                                                              row, col);
         comp.setFont(comp.getFont().deriveFont(Font.BOLD));
         return comp;
      }
   }
   
   /**
    * Cell renderer that renders the name of the type instead of the object itself.
    */
   class BasicTypeRenderer
      extends DefaultTableCellRenderer
   {
      public Component getTableCellRendererComponent(JTable table,
                                                     Object value,
                                                     boolean selected,
                                                     boolean focus,
                                                     int row,
                                                     int col)
      {
         // If we are trying to render a cell that contains a property type,
         // make sure to render the name, not the class name.
         if (value instanceof Class)
         {
            mValuetypeEditor.setValue(value);
            value = mValuetypeEditor.getAsText();
         }

         Component comp = super.getTableCellRendererComponent(table, value,
                                                              selected, focus,
                                                              row, col);
         return comp;
      }

      private ValuetypeEditor mValuetypeEditor = new ValuetypeEditor();
   }

   /**
    * Cell renderer for the items table default value column.
    */
   class ItemsDefaultValueCellRenderer
      extends DefaultTableCellRenderer
   {
      public Component getTableCellRendererComponent(JTable table,
                                                     Object value,
                                                     boolean selected,
                                                     boolean focus,
                                                     int row,
                                                     int col)
      {
         Component comp = super.getTableCellRendererComponent(table, value,
                                                              selected, focus,
                                                              row, col);
         if ( value == null )
         {
            setFont(getFont().deriveFont(Font.ITALIC));
            setText("Click here to insert default value");
         }
         else
         {
            setFont(getFont().deriveFont(Font.PLAIN));
         }

         return comp;
      }
   }

   /**
    * Cell editor for the basic table.
    */
   class BasicCellEditor
      extends AbstractCellEditor
      implements TableCellEditor
   {
      public Object getCellEditorValue()
      {
         return editor.getValue();
      }

      public boolean stopCellEditing()
      {
         try
         {
            if (editorComponent instanceof JComboBox)
            {
               JComboBox combo = (JComboBox)editorComponent;
               editor.setAsText((String)combo.getSelectedItem());
            }
            else if (editorComponent instanceof JTextField)
            {
               JTextField txt_field = (JTextField)editorComponent;
               editor.setAsText(txt_field.getText());
            }
            return super.stopCellEditing();
         }
         catch (IllegalArgumentException iae)
         {
            // TODO: Print this out to the Tweek console
            iae.printStackTrace();
//            System.out.println(iae.getMessage());
         }
         return false;
      }

      public Component getTableCellEditorComponent(JTable table, Object value,
                                                   boolean selected,
                                                   int row, int col)
      {
         editor = PropertyEditorManager.findEditor(value.getClass());
         editor.setValue(value);
         if (editor.getTags() != null)
         {
            JComboBox box = new JComboBox(editor.getTags());
            box.setSelectedItem(editor.getAsText());
            box.setFont(box.getFont().deriveFont(Font.PLAIN));
            box.setBackground(table.getBackground());
            box.addActionListener(new ActionListener()
            {
               public void actionPerformed(ActionEvent evt)
               {
                  stopCellEditing();
               }
            });
            editorComponent = box;
         }
         else
         {
            JTextField txt = new JTextField(editor.getAsText());
            txt.setBorder(BorderFactory.createEmptyBorder(0,0,0,0));
            txt.addActionListener(new ActionListener()
            {
               public void actionPerformed(ActionEvent evt)
               {
                  stopCellEditing();
               }
            });
            editorComponent = txt;
         }

         editorComponent.setForeground(table.getForeground());

         return editorComponent;
      }

      private PropertyEditor editor = null;
      private Component editorComponent = null;
   }
}
