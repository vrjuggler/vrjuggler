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

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Insets;
//import java.awt.Graphics;
import java.awt.Color;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import javax.swing.*;
import java.beans.*;
import info.clearthought.layout.*;
import org.vrjuggler.jccl.editors.*;
import org.vrjuggler.jccl.config.*;
//import com.sun.java.swing.plaf.windows.WindowsTreeUI;

public class PropertySheet extends PropertyComponent
{
   public PropertySheet()
   {
      // Register the editors.
      PropertyEditorManager.registerEditor(Boolean.class, BooleanEditor.class);
      PropertyEditorManager.registerEditor(String.class, StringEditor.class);
      PropertyEditorManager.registerEditor(Integer.class, IntegerEditor.class);
      PropertyEditorManager.registerEditor(Float.class, FloatEditor.class);
   }

   public void setElement(ConfigElement elm)
   {
      mConfigElement = elm;
      mColor = new Color(160, 160, 180);
      
      Object color = UIManager.get( "desktop" );
      if(null != color && color instanceof Color)
      {
         mColor = (Color)color;
      }
      else
      {
         System.out.println("Could not get the desktop color from the  UIManager.");
      }
      
      init();
      int row = 1;

      // Loop over all properties.
      ConfigDefinition def = elm.getDefinition();
      java.util.List prop_defs = def.getPropertyDefinitions();
      for(Iterator prop_def_itr = prop_defs.iterator() ; prop_def_itr.hasNext() ; )
      {
         PropertyDefinition prop_def = (PropertyDefinition)prop_def_itr.next();
         System.out.println("Property: " + prop_def.getToken());
         
         if (prop_def.isVariable() || (elm.getPropertyValueCount(prop_def.getToken()) > 1))
         {
            System.out.println("Variable Property List.");
            addVarList(elm, prop_def, row);
            ++row;
         }
         else
         {
            Object value = elm.getProperty(prop_def.getToken(), 0);
            
            if(value.getClass() == ConfigElement.class)
            {
               // Embedded Element.
               addEmbeddedElement(value, prop_def, row);
               ++row;
            }
            else
            {
               // Normal Property.
               String label = prop_def.getPropertyValueDefinition(0).getLabel();
               addNormalEditor(value, prop_def, label, row, 0);
               ++row;
            }
         }
      }
   }
   
   public PropertySheet(ConfigElement elm, Color color)
   {
      mConfigElement = elm;
      mColor = color;
      init();
      int row = 1;

      // Loop over all properties.
      ConfigDefinition def = elm.getDefinition();
      java.util.List prop_defs = def.getPropertyDefinitions();
      for(Iterator prop_def_itr = prop_defs.iterator() ; prop_def_itr.hasNext() ; )
      {
         PropertyDefinition prop_def = (PropertyDefinition)prop_def_itr.next();
         System.out.println("Property: " + prop_def.getToken());
         
         if (prop_def.isVariable() || (elm.getPropertyValueCount(prop_def.getToken()) > 1))
         {
            System.out.println("Variable Property List.");
            addVarList(elm, prop_def, row);
            ++row;
         }
         else
         {
            Object value = elm.getProperty(prop_def.getToken(), 0);
            
            if(value.getClass() == ConfigElement.class)
            {
               // Embedded Element.
               addEmbeddedElement(value, prop_def, row);
               ++row;
            }
            else
            {
               // Normal Property.
               String label = prop_def.getPropertyValueDefinition(0).getLabel();
               addNormalEditor(value, prop_def, label, row, 0);
               ++row;
            }
         }
      }
   }
   
   // This is the special case where we actually have a Variable list of values.
   public PropertySheet(ConfigElement elm, PropertyDefinition prop_def, Color color)
   {
      System.out.println("Variable Property List.");
      List props = elm.getPropertyValues(prop_def.getToken());
      
      mConfigElement = elm;
      mColor = color;
      
      init();
      
      int row = 1;
      int list_number = 0;
      
      // Insert Add Icon
      ClassLoader loader = getClass().getClassLoader();
      mAddIcon = new ImageIcon(loader.getResource("org/vrjuggler/jccl/editors/images/New16.gif"));
      mRemoveIcon = new ImageIcon(loader.getResource("org/vrjuggler/jccl/editors/images/Delete16.gif"));

      // If we have a variable list then create the buttons to add new values.
      if(prop_def.isVariable())
      {
         mAddButton.setIcon(mAddIcon);
         mAddButton.setMargin(new Insets(0,0,0,0));
         mAddButton.setBorderPainted(false);
         mAddButton.setFocusPainted(false);
         mAddButton.setContentAreaFilled(false);

         final ConfigElement temp_elm = elm;
         final PropertyDefinition temp_prop_def = prop_def;
         
         mAddButton.addActionListener(new ActionListener()
         {
            public void actionPerformed(ActionEvent evt)
            {
               if(ConfigElement.class == temp_prop_def.getType())
               {
                  addNewEmbeddedElement(temp_elm, temp_prop_def);
               }
               else
               {
                  addNewNormalEditor(temp_elm, temp_prop_def);
               }
            }
         });
         
         TableLayoutConstraints c3 = new TableLayoutConstraints(0, row, 2, row, TableLayout.CENTER, TableLayout.TOP);
         add(mAddButton, c3);
         ++row;
      }
      
      for(Iterator itr = props.iterator() ; itr.hasNext() ; )
      {
         Object value = itr.next();

         if(value.getClass() == ConfigElement.class)
         {
            // Embedded Element.
            addEmbeddedElement(value, prop_def, row);
         }
         else // List of normal values.
         {
            String label = null;
            // If it is variable then there will only be one label for the
            // property. Otherwise there will be one for each value.
            if(prop_def.isVariable())
            {
               label = prop_def.getPropertyValueDefinition(0).getLabel();
            }
            else
            {
               label = prop_def.getPropertyValueDefinition(list_number).getLabel();
            }
            
            addNormalEditor(value, prop_def, label, row, list_number);
            ++list_number;
         }

         ++row;
      }
   }
   
   private void init()
   {
      setBackground(mColor);
           // Create a frame
      setBounds (100, 100, 700, 300);

      // Create a TableLayout for the frame
      double border = 10;
      double f = TableLayout.FILL;
      double p = TableLayout.PREFERRED;
      double size[][] =
         {{f, f, 16},    // Columns
          {p, p}};   // Rows

      this.setLayout(new TableLayout(size)); 
   }

   /**
    * Adds a delete icon next to the given property. When the Icon is clicked it
    * will remove the property from the ConfigElement.
    */
   private void addDeleteButton(PropertyDefinition prop_def, Object value, int row)
   {
      JButton remove_button = new JButton();
      remove_button.setIcon(mRemoveIcon);
      remove_button.setMargin(new Insets(0,0,0,0));
      remove_button.setBorderPainted(false);
      remove_button.setFocusPainted(false);
      remove_button.setContentAreaFilled(false);
      
      // Verify that the property is variable.
      if(prop_def.isVariable())
      {
         remove_button.setEnabled(true);
        
         final Object temp_value = value;
         final String temp_string = prop_def.getToken();
         
         remove_button.addActionListener(new ActionListener()
         {
            public void actionPerformed(ActionEvent evt)
            {
               PropertyComponent temp = (PropertyComponent)((Component)evt.getSource()).getParent();
               mConfigElement.removeProperty(temp_string, temp_value);
               
               if(temp.getLayout() instanceof TableLayout)
               {
                  TableLayout tl = (TableLayout)temp.getLayout();
                  // Get the row that this panel is in.
                  TableLayoutConstraints tlc = tl.getConstraints((Component)evt.getSource());
                  int row = tlc.row1;
                  temp.remove((Component)evt.getSource()); 
                  tl.deleteRow(row);
               }
               temp.doLayout();
               temp.repaint();
               temp.invalidate();
               temp.validate();
            }
         });
      }
      else
      {
         remove_button.setEnabled(false);
      }
      TableLayoutConstraints c4 = new TableLayoutConstraints(2, row, 2, row, TableLayout.LEFT, TableLayout.TOP);
      add(remove_button, c4);
   }
   
   private void addVarList(ConfigElement elm, PropertyDefinition prop_def, int row)
   {
      VarListPanel editor_list = new VarListPanel(elm, prop_def, mColor);
      
      ((TableLayout)this.getLayout()).insertRow(row, TableLayout.PREFERRED);
      
      TableLayoutConstraints c = new TableLayoutConstraints(0, row, 1, row, TableLayout.FULL, TableLayout.FULL);
      this.add(editor_list, c);
      
      this.refresh();
      editor_list.refresh();
   }
   
   private void addNormalEditor(Object value, PropertyDefinition prop_def, String label, int row, int list_num)
   {
      PropertyEditorPanel editor = new PropertyEditorPanel(value, prop_def, mConfigElement, list_num, mColor);
      
      ((TableLayout)this.getLayout()).insertRow(row, TableLayout.PREFERRED);
   
      // Add both columns to this row.
      TableLayoutConstraints c = new TableLayoutConstraints(1, row, 1, row, TableLayout.FULL, TableLayout.FULL);
      this.add(editor, c);
      this.add(new JLabel(label), "0," + Integer.toString(row) + ",F,F");
      
      addDeleteButton(prop_def, value, row);

      //We must refresh things in the following order:
      // 1) This control.
      // 2) All contained containers recursively.
      this.refresh();
      editor.refresh();
   }
   
   private void addEmbeddedElement(Object value, PropertyDefinition prop_def, int row)
   {
      // Embedded Element
      // Adding a List
      EmbeddedElementPanel editor_list = new EmbeddedElementPanel((ConfigElement)value, mColor);
      
      ((TableLayout)this.getLayout()).insertRow(row, TableLayout.PREFERRED);
      
      TableLayoutConstraints c = new TableLayoutConstraints(0, row, 1, row, TableLayout.FULL, TableLayout.FULL);
      this.add(editor_list, c);
      
      addDeleteButton(prop_def, value, row);
      
      this.refresh();
      editor_list.refresh();
   }

   private void addNewNormalEditor(ConfigElement elm, PropertyDefinition prop_def)
   {
      // We know that we want the default value for the first
      // PropertyValueDefinition since this is a variable list.
      PropertyValueDefinition pvd = prop_def.getPropertyValueDefinition(0);
      Object default_value = pvd.getDefaultValue();
      elm.addProperty(prop_def.getToken(), default_value);
      
      // We select 2 here because we want to add it to the top of the list.
      addNormalEditor(default_value, prop_def, pvd.getLabel(), 2,
            elm.getPropertyValueCount(prop_def.getToken()) - 1);
   }
   
   private void addNewEmbeddedElement(ConfigElement elm, PropertyDefinition prop_def)
   {
      Object new_value = null;
      
      // Pick which type of embedded chunk to add
      List string_allowed_types = prop_def.getAllowedTypes();
      List allowed_types = new ArrayList();
      
      ConfigBroker broker = new ConfigBrokerProxy();
      ConfigDefinitionRepository def_repos = broker.getRepository();
      
      for(Iterator itr = string_allowed_types.iterator() ; itr.hasNext() ; )
      {
         String type = (String)itr.next();
         ConfigDefinition def = def_repos.get(type);
         if(null != def)
         {
            allowed_types.add(def);
         }
      }
      
      // Ask the user to choose a base ConfigDefinition.
      ConfigDefinitionChooser chooser = new ConfigDefinitionChooser();
      chooser.setDefinitions(allowed_types);
      int result = chooser.showDialog(this);

      // If the user did not cancel their choice, make a new ConfigChunk for
      // the chosen ConfigDefinition.
      if (result == ConfigDefinitionChooser.APPROVE_OPTION)
      {
         // Get a list of all known ConfigDefinitions
         java.util.List defs = def_repos.getAllLatest();
      
         ConfigElementFactory temp_factory = new ConfigElementFactory(defs);
         
         // TODO: Compute a unique name
         new_value = temp_factory.create("CHANGEME", chooser.getSelectedDefinition());
         
         elm.addProperty(prop_def.getToken(), new_value);
         // We select 2 here because we want to add it to the top of the list.
         addEmbeddedElement(new_value, prop_def, 2);
      }
   }
      

   private Icon               mAddIcon  = null;
   private Icon               mRemoveIcon = null; 
   private JButton            mAddButton  = new JButton();   
   private ConfigElement      mConfigElement = null;
}
