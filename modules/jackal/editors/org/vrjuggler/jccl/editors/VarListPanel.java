/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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
public class VarListPanel
   extends ExpandablePanel
   implements ConfigElementListener
{
   public void finalize()
   {
      mConfigElement.removeConfigElementListener(this);
      super.finalize();
   }

   /**
    * Construct a panel to hold the sheet that actually contains the list of
    * properties.
    */
   public VarListPanel(ConfigContext ctx, ConfigElement elm,
                       PropertyDefinition prop_def, Color color)
   {
      super(color);

      mConfigElement = elm;
      mConfigContext = ctx;

      mConfigElement.addConfigElementListener(this);

      // Create the new property list which is actually just a variable list of
      // property values.
      PropertySheet new_sheet =
         PropertySheetFactory.instance().makeVarSheet(mConfigElement, ctx,
                                                      prop_def,
                                                      getNextColor());

      mComponent = new_sheet;
      mPropDef = prop_def;
      new_sheet.setBorder(BorderFactory.createEtchedBorder());

      setBorder(BorderFactory.createEtchedBorder());

      // Add title for the list.
      String title = prop_def.getToken();
      JLabel name = new JLabel(title);
      TableLayoutConstraints c2 = new TableLayoutConstraints(2, 0, 2, 0,
                                                             TableLayout.LEFT,
                                                             TableLayout.TOP);
      add(name, c2);
   }

   /**
    * A property value was added so we must show this additional property.
    */
   public void propertyValueAdded(ConfigElementEvent evt)
   {
      // XXX: We could also test against mConfigElement == elm
      ConfigElement elm = (ConfigElement)evt.getSource();
      PropertyDefinition prop_def =
         elm.getDefinition().getPropertyDefinition(evt.getProperty());

      if(!prop_def.equals(mPropDef))
      {
         return;
      }

      PropertySheetFactory f = PropertySheetFactory.instance();

      // Add the value index to the starting row for a variable-valued property
      // list.  This gives the row in the property sheet's table layout where
      // this new property value will be inserted.
      // XXX: Is there a better way to keep track of this?
      int row = evt.getIndex() + PropertySheet.VAR_LIST_VALUE_START_ROW;
      int prop_num = evt.getIndex();

      if(ConfigElement.class == prop_def.getType())
      {
         // Use the PropertySheetFactory to add an additional embedded element.
         f.addEmbeddedElement((PropertySheet) mComponent, mConfigContext,
                              elm, evt.getValue(), prop_def, row);
      }
      else
      {
         // Use the PropertySheetFactory to add an aditional normal editor.
         String label = prop_def.getPropertyValueDefinition(0).getLabel();
         f.addNormalEditor((PropertySheet) mComponent, mConfigContext, elm,
                           evt.getValue(), prop_def, label, row, prop_num);
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

   /**
    * @since 0.92.11
    */
   public void propertyValueOrderChanged(ConfigElementEvent evt)
   {
      // XXX: We could also test against mConfigElement == elm
      ConfigElement elm = (ConfigElement)evt.getSource();
      PropertyDefinition prop_def =
         elm.getDefinition().getPropertyDefinition(evt.getProperty());

      PropertySheetFactory f = PropertySheetFactory.instance();
      f.updateValueOrdering((PropertySheet) mComponent, elm, prop_def,
                            Math.min(evt.getIndex0(), evt.getIndex1()),
                            Math.max(evt.getIndex0(), evt.getIndex1()));

      revalidate();
      repaint();
   }

   public void propertyValueRemoved(ConfigElementEvent evt)
   {
      revalidate();
      repaint();
   }

   private PropertyDefinition   mPropDef = null;
   private ConfigElement        mConfigElement = null;
   private ConfigContext        mConfigContext = null;
}
