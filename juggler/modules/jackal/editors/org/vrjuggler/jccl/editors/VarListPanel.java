package org.vrjuggler.jccl.editors;

import java.awt.Color;
import java.util.List;
import javax.swing.*;

import info.clearthought.layout.*;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.config.event.*;

/**
 * Provides a panel that contains a variable number of property values.
 */
public class VarListPanel extends ExpandablePanel implements ConfigElementListener
{
   /**
    * Construct a panel to hold the sheet that actually contains the list of
    * properties.
    */
   public VarListPanel(ConfigElement elm, PropertyDefinition prop_def, Color color)
   {
      super(color);
   
      elm.addConfigElementListener(this);
            
      // Create the new property list which is actually just a variable list of
      // property values.
      PropertySheet new_sheet = PropertySheetFactory.instance().makeVarSheet(elm, prop_def, getNextColor());

      mComponent = new_sheet;
      new_sheet.setBorder(BorderFactory.createEtchedBorder());

      setBorder(BorderFactory.createEtchedBorder());

      // Add title for the list.
      String title = prop_def.getToken();
      JLabel name = new JLabel(title);
      TableLayoutConstraints c2 = new TableLayoutConstraints(2, 0, 2, 0, TableLayout.LEFT, TableLayout.TOP);
      add(name, c2);
   }

   /**
    * A property value was added so we must show this additional property.
    */
   public void propertyValueAdded(ConfigElementEvent evt)
   {
      System.out.println("Property value added...");
      ConfigElement elm = (ConfigElement)evt.getSource();
      PropertyDefinition prop_def =
         elm.getDefinition().getPropertyDefinition(evt.getProperty());
      if(ConfigElement.class == prop_def.getType())
      {
         // Use the PropertySheetFactory to add an additional embedded element.
         PropertySheetFactory.instance().addEmbeddedElement((PropertySheet)mComponent, elm, evt.getValue(), prop_def, 2);
      }
      else
      {
         // Use the PropertySheetFactory to add an aditional normal editor.
         String label = prop_def.getPropertyValueDefinition(0).getLabel();
         PropertySheetFactory.instance().addNormalEditor((PropertySheet)mComponent, elm, 
                                                         evt.getValue(), prop_def, label, 2,
                                                         elm.getPropertyValueCount(prop_def.getToken()) - 1);
      }
   }
   
   public void nameChanged(ConfigElementEvent evt)
   {
      // Do Nothing...
   }
   public void propertyValueChanged(ConfigElementEvent evt)
   { 
      // Do Nothing...
   }
   public void propertyValueRemoved(ConfigElementEvent evt)
   {
      System.out.println("Property value removed...");
   }
}
