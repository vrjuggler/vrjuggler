package org.vrjuggler.jccl.editors;

import java.awt.Color;
import java.util.List;
import javax.swing.*;

import info.clearthought.layout.*;
//TODO: Remove these import
import org.vrjuggler.jccl.config.ConfigElement;
import org.vrjuggler.jccl.config.PropertyDefinition;

public class VarListPanel extends ExpandablePanel
{       
   public VarListPanel(ConfigElement elm, PropertyDefinition prop_def, Color color)
   {
      super(color);

      // XXX: Move this out to its own class soon.
      // Create the new property list which is actually just a variable list of
      // property values.
      PropertySheet new_sheet = new PropertySheet(elm, prop_def, getNextColor());
      mComponent = new_sheet;
      new_sheet.setBorder(BorderFactory.createEtchedBorder());

      setBorder(BorderFactory.createEtchedBorder());

      // Add title for the list.
      String title = prop_def.getToken();
      JLabel name = new JLabel(title);
      TableLayoutConstraints c2 = new TableLayoutConstraints(2, 0, 2, 0, TableLayout.LEFT, TableLayout.TOP);
      add(name, c2);
   }
}
