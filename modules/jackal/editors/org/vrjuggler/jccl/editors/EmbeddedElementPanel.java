package org.vrjuggler.jccl.editors;

import java.awt.Color;
import java.awt.event.*;
import java.util.List;
import javax.swing.*;
import javax.swing.border.TitledBorder;

import info.clearthought.layout.*;
//TODO: Remove these import
import org.vrjuggler.jccl.config.ConfigElement;

import com.sun.java.swing.plaf.windows.WindowsTreeUI;

public class EmbeddedElementPanel extends ExpandablePanel
{       
   public EmbeddedElementPanel(ConfigElement elm, Color color)
   {
      super(color);

      ClassLoader loader = getClass().getClassLoader();
      mTypeIcon = new ImageIcon(loader.getResource("org/vrjuggler/jccl/editors/images/config_element.gif"));
      mTypeButton.setIcon(mTypeIcon);
      
      // Create a panel for the embedded element.
      PropertySheet new_sheet = new PropertySheet(elm, getNextColor());
      mComponent = new_sheet;
      
      // Set the border for an embedded element.
      new_sheet.setBorder(BorderFactory.createCompoundBorder(BorderFactory.createRaisedBevelBorder(),
                  BorderFactory.createLoweredBevelBorder()));

      setBorder(BorderFactory.createEtchedBorder());

      // Add title for the list.
      String title = elm.getName();
      //JLabel name = new JLabel(title);
      final JTextField name = new JTextField(title);

      TableLayoutConstraints c2 = new TableLayoutConstraints(2, 0, 2, 0, TableLayout.LEFT, TableLayout.TOP);
      add(name, c2);

      name.setBorder(BorderFactory.createLoweredBevelBorder());
      name.setBackground(getBackground());
     
      // We can get away with this since the ConfigElement reference will not
      // change for the life of this component.
      final ConfigElement element = elm;
      
      // The following two listeners allow the user to change the name of the
      // embedded element.
      name.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            name.setBackground(getParent().getBackground());
            // Force the focus to be lost.
            name.transferFocusUpCycle();
            // Force the focus to be transfered to the next component.
            //name.transferFocus();

            // This is no longer needed since the above line will force a
            // focusLostEvent. But I have choosen to leave this line here in
            // case it might become useful later.
            //stopCellEditing();
         }
      });
      name.addFocusListener(new FocusAdapter()
      {
         public void focusGained(FocusEvent evt)
         {
            name.setBackground(java.awt.Color.white);
         }
         public void focusLost(FocusEvent evt)
         {
            name.setBackground(getParent().getBackground());
            //stopCellEditing();
            //XXX: Change name here.
            element.setName(name.getText());
         }
      });


      
   }

   private     Icon              mTypeIcon      = null;
}
