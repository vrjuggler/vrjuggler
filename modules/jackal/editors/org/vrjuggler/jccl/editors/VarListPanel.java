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

      revalidate();
      repaint();
   }
   
   public void nameChanged(ConfigElementEvent evt)
   {
      revalidate();
      repaint();
   }

   public void propertyValueChanged(ConfigElementEvent evt)
   { 
      revalidate();
      repaint();
   }

   public void propertyValueRemoved(ConfigElementEvent evt)
   {
      System.out.println("Property value removed...");

      revalidate();
      repaint();
   }
}
