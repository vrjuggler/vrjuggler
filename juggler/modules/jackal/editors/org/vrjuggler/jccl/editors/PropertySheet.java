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
//import java.util.HashMap;
//import java.util.Map;
//import java.util.Set;
import javax.swing.*;

//import new_layout.*;
import info.clearthought.layout.*;
import org.vrjuggler.jccl.editors.*;
import org.vrjuggler.jccl.config.*;
//import com.sun.java.swing.plaf.windows.WindowsTreeUI;

public class PropertySheet extends PropertyComponent
{
   public PropertySheet()
   {
      PropertyEditorPanelManager.registerEditor(Boolean.class, BooleanEditorPanel.class);
      PropertyEditorPanelManager.registerEditor(String.class, StringEditorPanel.class);
      PropertyEditorPanelManager.registerEditor(Integer.class, IntegerEditorPanel.class);
      PropertyEditorPanelManager.registerEditor(Float.class, FloatEditorPanel.class);
      PropertyEditorPanelManager.registerEditor(ConfigElementPointer.class, StringEditorPanel.class);
   }
   public void setElement(ConfigElement elm)
   {
      mConfigElement = elm;
      mColor = new Color(160, 160, 180);
      init();
      //addHeadings(); 
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
            List props = elm.getPropertyValues(prop_def.getToken());
            addVarList(elm, props, prop_def, row);
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
               addNormalEditor(value, prop_def, prop_def.getToken(), row, 0);
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
      //addHeadings(); 
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
            List props = elm.getPropertyValues(prop_def.getToken());
            addVarList(elm, props, prop_def, row);
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
               addNormalEditor(value, prop_def, prop_def.getToken(), row, 0);
               ++row;
            }
         }
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
         {{p, f, 16},    // Columns
          {p, p}};   // Rows

      this.setLayout(new TableLayout(size)); 
   }
   
   private void addHeadings()
   {
      JLabel name_label = new JLabel("Name");
      //name_label.setBorder(BorderFactory.createLineBorder(Color.black));
      name_label.setBorder(BorderFactory.createEtchedBorder());
      name_label.setOpaque(true);
      
      JLabel value_label = new JLabel("Value");
      value_label.setBorder(BorderFactory.createEtchedBorder());
      value_label.setOpaque(true);
      
      add(name_label, "0, 0, F, F");
      add(value_label, "1, 0, F, F");
   }
   
   public PropertySheet(ConfigElement elm, PropertyDefinition prop_def, List props, Color color)
   {
      mConfigElement = elm;
      mColor = color;
      
      init();
      
      int row = 1;
      int list_number = 0;
      
      if(prop_def.getType() != ConfigElement.class)
      {
         //addHeadings();       
      }
      // Insert Add Icon
      ClassLoader loader = getClass().getClassLoader();
      mAddIcon = new ImageIcon(loader.getResource("images/New16.gif"));
      mRemoveIcon = new ImageIcon(loader.getResource("images/Delete16.gif"));
 
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
               addNewEmbeddedElement(temp_elm, temp_prop_def);
            }
         });
         
         TableLayoutConstraints c3 = new TableLayoutConstraints(1, row, 1, row, TableLayout.CENTER, TableLayout.TOP);
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
         else
         {
            // Normal Property.
            String label =
               prop_def.getPropertyValueDefinition(list_number).getLabel();
            ++list_number;
            
            addNormalEditor(value, prop_def, label, row, list_number);
         }
         ++row;
      }
   }
   
   private void addVarList(ConfigElement elm, List props, PropertyDefinition prop_def, int row)
   {
      // Variable Property List.
      PropertySheet new_sheet = new PropertySheet(elm, prop_def, props, getNextColor());
      
      new_sheet.setColor(getNextColor());
      new_sheet.setBorder(BorderFactory.createEtchedBorder());
      // Adding a List
      ExpandablePanel editor_list = new ExpandablePanel(new_sheet, "NONE", prop_def.getToken(), mColor, prop_def.isVariable(), false);
      
      ((TableLayout)this.getLayout()).insertRow(row, TableLayout.PREFERRED);
      
      TableLayoutConstraints c = new TableLayoutConstraints(0, row, 1, row, TableLayout.FULL, TableLayout.FULL);
      this.add(editor_list, c);
      
      this.refresh();
      editor_list.refresh();
      new_sheet.refresh();
   }
   
   private void addNormalEditor(Object value, PropertyDefinition prop_def, String label, int row, int list_num)
   {
      //XXX: Editors might change.
      PropertyEditorPanel editor = PropertyEditorPanelManager.findEditor(value.getClass());
      
      if(null != editor)
      {
         editor.set(value, prop_def, mConfigElement, list_num);
               
         ((TableLayout)this.getLayout()).insertRow(row, TableLayout.PREFERRED);
      
         // Add both columns to this row.
         TableLayoutConstraints c = new TableLayoutConstraints(1, row, 1, row, TableLayout.FULL, TableLayout.FULL);
         this.add(editor, c);
         this.add(new JLabel(label), "0," + Integer.toString(row) + ",F,F");

         // If this property has variable length we must allow the user to
         // add/remove them.
         if(prop_def.isVariable())
         {
            JButton remove_button = new JButton();
            remove_button.setIcon(mRemoveIcon);
            remove_button.setMargin(new Insets(0,0,0,0));
            remove_button.setBorderPainted(false);
            remove_button.setFocusPainted(false);
            remove_button.setContentAreaFilled(false);
           
            final ConfigElement temp_elm = mConfigElement;
            final Object temp_value = value;
            final String temp_string = prop_def.getToken();
            
            remove_button.addActionListener(new ActionListener()
            {
               public void actionPerformed(ActionEvent evt)
               {
                  System.out.println("1");
                  PropertyComponent temp = (PropertyComponent)((Component)evt.getSource()).getParent();
                  temp_elm.removeProperty(temp_string, temp_value);
                  
                  if(temp.getLayout() instanceof TableLayout)
                  {
                     System.out.println("2");
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

            TableLayoutConstraints c4 = new TableLayoutConstraints(2, row, 2, row, TableLayout.LEFT, TableLayout.TOP);
            add(remove_button, c4);
         }

         //We must refresh things in the following order:
         // 1) This control.
         // 2) All contained containers recursively.
         this.refresh();
         editor.refresh();
      }
      else
      {
         System.out.println("Could not find an editor for class: " + value.getClass().toString());
      }
   }
   
   private void addEmbeddedElement(Object value, PropertyDefinition prop_def, int row)
   {
      // Embedded Element
      // Adding a List
      PropertySheet new_sheet = new PropertySheet((ConfigElement)value, getNextColor());
      new_sheet.setColor(getNextColor());
      new_sheet.setBorder(BorderFactory.createCompoundBorder(BorderFactory.createRaisedBevelBorder(),
                  BorderFactory.createLoweredBevelBorder()));
      ExpandablePanel editor_list = new ExpandablePanel(new_sheet, prop_def.getToken(), ((ConfigElement)value).getName(), mColor, false, true);
      
      ((TableLayout)this.getLayout()).insertRow(row, TableLayout.PREFERRED);
      
      TableLayoutConstraints c = new TableLayoutConstraints(0, row, 1, row, TableLayout.FULL, TableLayout.FULL);
      this.add(editor_list, c);
      
      if(prop_def.isVariable())
      {
         JButton remove_button = new JButton();
         remove_button.setIcon(mRemoveIcon);
         remove_button.setMargin(new Insets(0,0,0,0));
         remove_button.setBorderPainted(false);
         remove_button.setFocusPainted(false);
         remove_button.setContentAreaFilled(false);
         
            final ConfigElement temp_elm = mConfigElement;
            final Object temp_value = value;
            final String temp_string = prop_def.getToken();
            
            remove_button.addActionListener(new ActionListener()
            {
               public void actionPerformed(ActionEvent evt)
               {
                  System.out.println("1");
                  PropertyComponent temp = (PropertyComponent)((Component)evt.getSource()).getParent();
                  temp_elm.removeProperty(temp_string, temp_value);
                  
                  if(temp.getLayout() instanceof TableLayout)
                  {
                     System.out.println("2");
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

         TableLayoutConstraints c4 = new TableLayoutConstraints(2, row, 2, row, TableLayout.LEFT, TableLayout.TOP);
         add(remove_button, c4);
      }
      
      this.refresh();
      editor_list.refresh();
      new_sheet.refresh();
   }

   public void addNewEmbeddedElement(ConfigElement elm, PropertyDefinition prop_def)
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
      
      // Ask the user to choose a base ChunkDesc
      ConfigDefinitionChooser chooser = new ConfigDefinitionChooser();
      chooser.setDefinitions(allowed_types);
      int result = chooser.showDialog(this);

      // If the user did not cancel their choice, make a new ConfigChunk for
      // the chose ChunkDesc
      if (result == ConfigDefinitionChooser.APPROVE_OPTION)
      {
         ConfigElementFactory temp_factory = new ConfigElementFactory(allowed_types);
         
         // TODO: Compute a unique name
         new_value = temp_factory.create("CHANGEME", chooser.getSelectedDefinition());
         
         elm.addProperty(prop_def.getToken(), new_value);
         addEmbeddedElement(new_value, prop_def, 2);
      }
   }
      

   private Icon               mAddIcon  = null;
   private Icon               mRemoveIcon = null; 
   private JButton            mAddButton  = new JButton();   
   private ConfigElement      mConfigElement = null;
}
