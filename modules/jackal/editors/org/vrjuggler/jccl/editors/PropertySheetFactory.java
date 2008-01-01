/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Insets;
import java.awt.Color;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import javax.swing.*;
import java.beans.*;
import info.clearthought.layout.*;
import org.vrjuggler.jccl.editors.*;
import org.vrjuggler.jccl.config.*;


public class PropertySheetFactory extends PropertyComponent
{
   private static final String IMG_ROOT = "org/vrjuggler/jccl/editors/images";

   public PropertySheetFactory()
   {
      // Register the editors.
      PropertyEditorManager.registerEditor(Boolean.class, BooleanEditor.class);
      PropertyEditorManager.registerEditor(String.class, StringEditor.class);
      PropertyEditorManager.registerEditor(Integer.class, IntegerEditor.class);
      PropertyEditorManager.registerEditor(Float.class, FloatEditor.class);
   }

   /**
    * Gets the singleton instance of this class. This implementation is thread
    * safe.
    */
   public static PropertySheetFactory instance()
   {
      synchronized (PropertySheetFactory.class)
      {
         if ( mInstance == null )
         {
            mInstance = new PropertySheetFactory();
         }
      }
      return mInstance;
   }

   /**
    * The singleton instance of this class.
    *
    * @link
    * @shapeType PatternLink
    * @pattern Singleton
    * @supplierRole Singleton factory
    */
   /*# private PropertySheetFactory _propertySheetFactory; */
   private static PropertySheetFactory mInstance = null;

   public PropertySheet makeSheet(ConfigContext ctx, ConfigElement elm,
                                  Color color)
   {
      PropertySheet sheet = new PropertySheet();

      sheet.setConfigElement(elm);
      sheet.setColor(color);
      sheet.init();

      int row = PropertySheet.START_ROW;

      // Loop over all properties.
      ConfigDefinition def = elm.getDefinition();
      java.util.List prop_defs = def.getPropertyDefinitions();
      for ( Iterator prop_def_itr = prop_defs.iterator();
            prop_def_itr.hasNext(); )
      {
         PropertyDefinition prop_def = (PropertyDefinition)prop_def_itr.next();

         if ( prop_def.isVariable() ||
              elm.getPropertyValueCount(prop_def.getToken()) > 1)
         {
            addVarList(sheet, ctx, elm, prop_def, row);
            ++row;
         }
         else
         {
            Object value = elm.getProperty(prop_def.getToken(), 0);

            if(value.getClass() == ConfigElement.class)
            {
               // Embedded Element.
               addEmbeddedElement(sheet, ctx, elm, value, prop_def, row);
               ++row;
            }
            else
            {
               // Normal Property.
               String label = prop_def.getPropertyValueDefinition(0).getLabel();
               addNormalEditor(sheet, ctx, elm, value, prop_def, label, row, 0);
               ++row;
            }
         }
      }
      return sheet;
   }

   public PropertySheet makeVarSheet(ConfigElement elm, ConfigContext ctx,
                                     PropertyDefinition propDef, Color color)
   {
      PropertySheet sheet = new PropertySheet();

      sheet.setConfigElement(elm);
      sheet.setColor(color);
      sheet.init();

      int row = PropertySheet.START_ROW;

      // If we have a variable list then create the buttons to add new values.
      if(propDef.isVariable())
      {
         // Insert Add Icon
         ClassLoader loader = getClass().getClassLoader();

         JButton add_button = new JButton();

         PropertyValueDefinition value_def =
            propDef.getPropertyValueDefinition(0);
         String label = value_def.getLabel();
         String text = (label == null || label.equals("")) ? "Value" : label;
         add_button.setText("New " + text);

         try
         {
            Icon add_icon =
               new ImageIcon(loader.getResource(IMG_ROOT + "/New16.gif"));
            add_button.setIcon(add_icon);
            add_button.setToolTipText("Add a new value of type " +
                                      propDef.getToken());
         }
         catch(Exception ex)
         {
         }

         add_button.setMargin(new Insets(0,0,0,0));
         add_button.setBorderPainted(false);
         add_button.setFocusPainted(false);
         add_button.setContentAreaFilled(false);

         final ConfigElement temp_elm = elm;
         final ConfigContext temp_ctx = ctx;
         final PropertyDefinition temp_prop_def = propDef;

         add_button.addActionListener(new ActionListener()
         {
            public void actionPerformed(ActionEvent evt)
            {
               if(ConfigElement.class == temp_prop_def.getType())
               {
                  addNewEmbeddedElement(temp_ctx, temp_elm, temp_prop_def);
               }
               else
               {
                  addNewNormalEditor(temp_ctx, temp_elm, temp_prop_def);
               }
            }
         });

         // The "add" button spans the label, editor, and delete icon columns.
         TableLayoutConstraints c3 =
            new TableLayoutConstraints(PropertySheet.SPAN_START_COLUMN, row,
                                       PropertySheet.SPAN_END_COLUMN, row,
                                       TableLayout.CENTER, TableLayout.TOP);
         sheet.add(add_button, c3);
         ++row;
      }

      int list_number = 0;
      List props = elm.getPropertyValues(propDef.getToken());

      for(Iterator itr = props.iterator() ; itr.hasNext() ; )
      {
         Object value = itr.next();

         if(value.getClass() == ConfigElement.class)
         {
            // Embedded Element.
            addEmbeddedElement(sheet, ctx, elm, value, propDef, row);
         }
         else // List of normal values.
         {
            String label = null;
            // If it is variable then there will only be one label for the
            // property. Otherwise there will be one for each value.
            if(propDef.isVariable())
            {
               label = propDef.getPropertyValueDefinition(0).getLabel();
            }
            else
            {
               label =
                  propDef.getPropertyValueDefinition(list_number).getLabel();
            }

            addNormalEditor(sheet, ctx, elm, value, propDef, label, row,
                            list_number);
         }

         ++row;
         ++list_number;
      }
      return sheet;
   }

   private void addOrderingButtons(PropertySheet sheet,
                                   final ConfigContext ctx,
                                   final ConfigElement elm,
                                   PropertyDefinition propDef, int row)
   {
      ClassLoader loader = getClass().getClassLoader();
      Icon up_icon = new ImageIcon(loader.getResource(IMG_ROOT + "/Up16.gif"));
      Icon down_icon = new ImageIcon(loader.getResource(IMG_ROOT +
                                                        "/Down16.gif"));

      JButton up_button = new JButton();
      up_button.setIcon(up_icon);
      up_button.setMargin(new Insets(0,0,0,0));
      up_button.setBorderPainted(false);
      up_button.setFocusPainted(false);
      up_button.setContentAreaFilled(false);
      up_button.setEnabled(false);
      up_button.setToolTipText("Move this value up one");

      JButton down_button = new JButton();
      down_button.setIcon(down_icon);
      down_button.setMargin(new Insets(0,0,0,0));
      down_button.setBorderPainted(false);
      down_button.setFocusPainted(false);
      down_button.setContentAreaFilled(false);
      down_button.setEnabled(false);
      down_button.setToolTipText("Move this value down one");

      final String prop_token = propDef.getToken();

      // Create an action listener that decrements the property value by 1.
      // The notion of "up" relates to how this information is presented in
      // the GUI.
      up_button.addActionListener(new ActionListener()
         {
            public void actionPerformed(ActionEvent evt)
            {
               Component source = (Component) evt.getSource();
               PropertyComponent sheet =
                  (PropertyComponent) source.getParent();
               TableLayout tl = (TableLayout) sheet.getLayout();

               // Get the row where this button is located.
               TableLayoutConstraints tlc = tl.getConstraints(source);
               int row = tlc.row1;

               // Calculate the property index on the fly based on our row.
               int old_index = row - PropertySheet.VAR_LIST_VALUE_START_ROW;
               int new_index = old_index - 1;
               elm.setPropertyValueIndex(prop_token, new_index, old_index,
                                         ctx);
            }
         }
      );

      // Create an action listener that increments the proeprty value by 1.
      // The notion of "down" relates to how this information is presented
      // in the GUI.
      down_button.addActionListener(new ActionListener()
         {
            public void actionPerformed(ActionEvent evt)
            {
               Component source = (Component) evt.getSource();
               PropertyComponent sheet =
                  (PropertyComponent) source.getParent();
               TableLayout tl = (TableLayout) sheet.getLayout();

               // Get the row where this button is located.
               TableLayoutConstraints tlc = tl.getConstraints(source);
               int row = tlc.row1;

               // Calculate the property index on the fly based on our row.
               int old_index = row - PropertySheet.VAR_LIST_VALUE_START_ROW;
               int new_index = old_index + 1;
               elm.setPropertyValueIndex(prop_token, new_index, old_index,
                                         ctx);
            }
         }
      );

      sheet.add(up_button,
                new TableLayoutConstraints(PropertySheet.UP_ICON_COLUMN, row,
                                           PropertySheet.UP_ICON_COLUMN, row,
                                           TableLayout.LEFT, TableLayout.TOP));
      sheet.add(down_button,
                new TableLayoutConstraints(PropertySheet.DOWN_ICON_COLUMN, row,
                                           PropertySheet.DOWN_ICON_COLUMN, row,
                                           TableLayout.LEFT, TableLayout.TOP));

      refreshOrderingButtons(sheet, PropertySheet.VAR_LIST_VALUE_START_ROW,
                             row);
   }

   /**
    * Adds a delete icon next to the given property. When the Icon is clicked
    * it will remove the property from the ConfigElement.
    */
   private void addDeleteButton(PropertySheet sheet, ConfigContext ctx,
                                ConfigElement elm, PropertyDefinition propDef,
                                int row)
   {
      ClassLoader loader = getClass().getClassLoader();
      Icon remove_icon =
         new ImageIcon(loader.getResource(IMG_ROOT + "/Delete16.gif"));

      JButton remove_button = new JButton();
      remove_button.setIcon(remove_icon);
      remove_button.setMargin(new Insets(0,0,0,0));
      remove_button.setBorderPainted(false);
      remove_button.setFocusPainted(false);
      remove_button.setContentAreaFilled(false);

      remove_button.setEnabled(true);
      remove_button.setToolTipText("Delete this property value");

      final String temp_string = propDef.getToken();
      final ConfigContext temp_ctx = ctx;
      final ConfigElement temp_elm = elm;

      remove_button.addActionListener(new ActionListener()
         {
            public void actionPerformed(ActionEvent evt)
            {
               Component source = (Component) evt.getSource();
               PropertyComponent sheet = (PropertyComponent) source.getParent();
               TableLayout tl = (TableLayout) sheet.getLayout();

               // Get the row where this button is located.
               TableLayoutConstraints tlc = tl.getConstraints(source);
               int row = tlc.row1;

               Component[] components = sheet.getComponents();

               List row_components = new ArrayList();
               for ( int i = 0; i < components.length; ++i )
               {
                  if ( components[i] != source )
                  {
                     TableLayoutConstraints cur_tlc =
                        tl.getConstraints(components[i]);
                     if ( cur_tlc.row1 == row )
                     {
                        row_components.add(components[i]);
                     }
                  }
               }

               // Calculate the property index on the fly based on our row.
               int value_index = row - PropertySheet.VAR_LIST_VALUE_START_ROW;
               temp_elm.removeProperty(temp_string, value_index, temp_ctx);

               // The components in this row have to be removed from the
               // container.
               for ( Iterator c = row_components.iterator(); c.hasNext(); )
               {
                  sheet.remove((Component) c.next());
               }

               sheet.remove(source);

               // Remove the row that is now empty from the layout.
               tl.deleteRow(row);

               refreshOrderingButtons(sheet, row, tl.getNumRow());

               sheet.revalidate();
               sheet.repaint();
            }
         }
      );

      TableLayoutConstraints c4 =
         new TableLayoutConstraints(PropertySheet.DELETE_ICON_COLUMN, row,
                                    PropertySheet.DELETE_ICON_COLUMN, row,
                                    TableLayout.LEFT, TableLayout.TOP);
      sheet.add(remove_button, c4);
   }

   private void refreshOrderingButtons(PropertyComponent sheet, int startRow,
                                       int endRow)
   {
      if ( startRow <= endRow )
      {
         Component[] components = sheet.getComponents();
         TableLayout tl = (TableLayout) sheet.getLayout();
         int last_row = tl.getNumRow() - 1;

         for ( int i = 0; i < components.length; ++i )
         {
            TableLayoutConstraints tlc = tl.getConstraints(components[i]);
            if ( tlc.row1 <= endRow )
            {
               // The "Up" button must be disabled when it is in the starting
               // row for a variable list of values.  Otherwise, it can be
               // enabled.
               if ( tlc.col1 == PropertySheet.UP_ICON_COLUMN )
               {
                  components[i].setEnabled(
                     tlc.row1 != PropertySheet.VAR_LIST_VALUE_START_ROW
                  );
               }
               // The "Down" button must be disabled when it is in the last
               // row of the layout for a variable list of values.  Otherwise,
               // it can be enabled.
               else if ( tlc.col1 == PropertySheet.DOWN_ICON_COLUMN )
               {
                  components[i].setEnabled(tlc.row1 != last_row);
               }
            }
         }
      }
   }

   private void addVarList(PropertySheet sheet, ConfigContext ctx,
                           ConfigElement elm, PropertyDefinition propDef,
                           int row)
   {
      // Use the same color for the list panel.
      VarListPanel editor_list = new VarListPanel(ctx, elm, propDef,
                                                  sheet.getColor());

      ((TableLayout)sheet.getLayout()).insertRow(row, TableLayout.PREFERRED);

      // Variable list panels span all the columns.
      TableLayoutConstraints c =
         new TableLayoutConstraints(PropertySheet.SPAN_START_COLUMN, row,
                                    PropertySheet.SPAN_END_COLUMN, row,
                                    TableLayout.FULL, TableLayout.FULL);
      sheet.add(editor_list, c);

      revalidate();
      repaint();
   }

   public void addNormalEditor(PropertySheet sheet, ConfigContext ctx,
                               ConfigElement elm, Object value,
                               PropertyDefinition propDef, String labelText,
                               int row, int propValueIndex)
   {
      PropertyEditorPanel editor = new PropertyEditorPanel(ctx, value,
                                                           propDef, elm,
                                                           propValueIndex,
                                                           sheet.getColor());

      ((TableLayout)sheet.getLayout()).insertRow(row, TableLayout.PREFERRED);

      // If the property definition does not allow for a variable number of
      // values, span from the editor start column to the delete icon column.
      int end_col = (propDef.isVariable() ? PropertySheet.EDITOR_COLUMN
                                          : PropertySheet.SPAN_END_COLUMN);

      // Add both columns to this row.
      TableLayoutConstraints c =
         new TableLayoutConstraints(PropertySheet.EDITOR_COLUMN, row,
                                    end_col, row, TableLayout.FULL,
                                    TableLayout.FULL);
      sheet.add(editor, c);
      JLabel label = new JLabel(labelText);
      sheet.add(label, PropertySheet.LABEL_COLUMN + "," + row + ",F,F");

      if ( propDef.isVariable() )
      {
         addOrderingButtons(sheet, ctx, elm, propDef, row);
         addDeleteButton(sheet, ctx, elm, propDef, row);
      }

      revalidate();
      repaint();
   }

   public void addEmbeddedElement(PropertySheet sheet, ConfigContext ctx,
                                  ConfigElement elm, Object value,
                                  PropertyDefinition propDef, int row)
   {
      // Embedded Element
      // Adding a List
      EmbeddedElementPanel editor_list =
         new EmbeddedElementPanel(ctx, (ConfigElement) value,
                                  sheet.getColor());

      ((TableLayout)sheet.getLayout()).insertRow(row, TableLayout.PREFERRED);

      // If the property definition does not allow for a variable number of
      // values, span from the editor start column to the delete icon column.
      int end_col = (propDef.isVariable() ? PropertySheet.EDITOR_COLUMN
                                          : PropertySheet.SPAN_END_COLUMN);

      // Embedded element panels span the label and editor columns.
      TableLayoutConstraints c =
         new TableLayoutConstraints(PropertySheet.LABEL_COLUMN, row,
                                    end_col, row, TableLayout.FULL,
                                    TableLayout.FULL);
      sheet.add(editor_list, c);

      if ( propDef.isVariable() )
      {
         addOrderingButtons(sheet, ctx, elm, propDef, row);
         addDeleteButton(sheet, ctx, elm, propDef, row);
      }

      revalidate();
      repaint();
   }

   /**
    * Handles any reordering of editor components needed based on a change
    * in the order of property values in the given property
    * (as identified by <code>propDef</code>) in the given config element.
    * The given property sheet is revalidated and repainted at the end of
    * this operation.
    *
    * @param sheet      the property sheet to update
    * @param elt        the element containing the property whose values have
    *                   been reordered
    * @param rangeStart the index of the first property value that has been
    *                   reordered
    * @param rangeEnd   the index of the last property value that has been
    *                   reordered
    *
    * @since 0.92.11
    */
   public void updateValueOrdering(PropertySheet sheet, ConfigElement elt,
                                   PropertyDefinition propDef, int rangeStart,
                                   int rangeEnd)
   {
      String prop_token = propDef.getToken();
      Component[] components = sheet.getComponents();
      TableLayout tl = (TableLayout) sheet.getLayout();

      int start_row = rangeStart + PropertySheet.VAR_LIST_VALUE_START_ROW;
      int end_row   = rangeEnd + PropertySheet.VAR_LIST_VALUE_START_ROW;

      List prop_values = elt.getPropertyValues(prop_token);

      for ( int i = 0; i < components.length; ++i )
      {
         TableLayoutConstraints cur_tlc =
            tl.getConstraints(components[i]);

         if ( cur_tlc.row1 >= start_row && cur_tlc.row1 <= end_row )
         {
            Object value = null;

            if ( components[i] instanceof EmbeddedElementPanel )
            {
               value = ((EmbeddedElementPanel) components[i]).getValue();
            }
            else if ( components[i] instanceof PropertyEditorPanel )
            {
               value = ((PropertyEditorPanel) components[i]).getValue();
            }

            if ( value != null )
            {
               int value_index = elt.getPropertyValueIndex(prop_token, value);

               if ( value_index > -1 )
               {
                  int expected_row = value_index +
                                        PropertySheet.VAR_LIST_VALUE_START_ROW;

                  if ( expected_row != cur_tlc.row1 )
                  {
                     cur_tlc.row1 = expected_row;
                     cur_tlc.row2 = expected_row;
                     tl.setConstraints(components[i], cur_tlc);
                  }
               }
            }
         }
      }

      sheet.revalidate();
      sheet.repaint();
   }

   private void addNewNormalEditor(ConfigContext ctx, ConfigElement elm,
                                   PropertyDefinition propDef)
   {
      // We know that we want the default value for the first
      // PropertyValueDefinition since this is a variable list.
      PropertyValueDefinition pvd = propDef.getPropertyValueDefinition(0);
      Object default_value = null;

      // NOTE: This fixed a rather large bug that caused
      //       ConfigElementPointerEditors that were added at run time to all
      //       edit the same ConfigElementPointer.  By default the
      //       PropertyValueDefinition returns a default value that is always
      //       the same, bad idea.
      if (propDef.getType() == ConfigElementPointer.class)
      {
         default_value = new ConfigElementPointer("");
      }
      else
      {
         default_value = pvd.getDefaultValue();
      }

      elm.addProperty(propDef.getToken(), default_value, ctx);
   }

   private void addNewEmbeddedElement(ConfigContext ctx, ConfigElement elm,
                                      PropertyDefinition propDef)
   {
      Object new_value = null;

      // Pick which type of embedded element to add
      List string_allowed_types = propDef.getAllowedAndDerivedTypes();
      List allowed_types = new ArrayList();

      ConfigBroker broker = new ConfigBrokerProxy();
      ConfigDefinitionRepository def_repos = broker.getRepository();

      for(Iterator itr = string_allowed_types.iterator() ; itr.hasNext() ; )
      {
         String type = (String)itr.next();
         ConfigDefinition def = def_repos.get(type);
         if ( null != def && ! def.isAbstract() )
         {
            allowed_types.add(def);
         }
      }

      ConfigDefinition selected_definition = null;

      // Only use ConfigDefinitionChooser when allowed_types has more than one
      // choice.  When there is only one choice, we can make it for the user.
      if ( allowed_types.size() > 1 )
      {
         // Ask the user to choose a base ConfigDefinition.
         ConfigDefinitionChooser chooser = new ConfigDefinitionChooser();
         chooser.setDefinitions(allowed_types);
         int result = chooser.showDialog(this);

         // If the user did not cancel their choice, make a new ConfigElement
         // for the chosen ConfigDefinition.
         if (result == ConfigDefinitionChooser.APPROVE_OPTION)
         {
            selected_definition = chooser.getSelectedDefinition();
         }
      }
      else
      {
         selected_definition = (ConfigDefinition) allowed_types.get(0);
      }

      if ( selected_definition != null )
      {
         ConfigElementFactory temp_factory =
            new ConfigElementFactory(def_repos.getAllLatest());

         // TODO: Compute a unique name
         new_value = temp_factory.create("CHANGEME", selected_definition);

         elm.addProperty(propDef.getToken(), new_value, ctx);
      }
   }
}
