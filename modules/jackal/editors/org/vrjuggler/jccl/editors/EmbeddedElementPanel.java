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
   public void finalize()
   {
      ActionListener[] listeners = mNameComponent.getActionListeners();
      
      for (int i = 0 ; i < listeners.length ; ++i)
      {
         mNameComponent.removeActionListener(listeners[i]);
      }
      
      FocusListener[] focus_listeners = mNameComponent.getFocusListeners();
      
      for (int i = 0 ; i < focus_listeners.length ; ++i)
      {
         mNameComponent.removeFocusListener(focus_listeners[i]);
      }
      super.finalize();
   }
   
   public EmbeddedElementPanel(ConfigElement elm, Color color)
   {
      super(color);
      
      mConfigElement = elm;

      ClassLoader loader = getClass().getClassLoader();
      mTypeIcon = new ImageIcon(loader.getResource("org/vrjuggler/jccl/editors/images/config_element.gif"));
      mTypeButton.setIcon(mTypeIcon);
      
      // Create a panel for the embedded element.
      //PropertySheet new_sheet = new PropertySheet(mConfigElement, getNextColor());
      PropertySheet new_sheet = PropertySheetFactory.instance().makeSheet(mConfigElement, getNextColor());
      mComponent = new_sheet;
      
      // Set the border for an embedded element.
      new_sheet.setBorder(BorderFactory.createCompoundBorder(BorderFactory.createRaisedBevelBorder(),
                  BorderFactory.createLoweredBevelBorder()));

      //setBorder(BorderFactory.createEtchedBorder());
      setBorder(BorderFactory.createEmptyBorder(5,0,5,0));

      // Add title for the list.
      String title = mConfigElement.getName();
      //JLabel name = new JLabel(title);
      mNameComponent = new JTextField(title);

      TableLayoutConstraints c2 = new TableLayoutConstraints(2, 0, 2, 0, TableLayout.LEFT, TableLayout.TOP);
      add(mNameComponent, c2);

      mNameComponent.setBorder(BorderFactory.createLoweredBevelBorder());
      mNameComponent.setBackground(getBackground());
     
      // The following two listeners allow the user to change the name of the
      // embedded element.
      mNameComponent.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            mNameComponent.setBackground(getParent().getBackground());
            // Force the focus to be lost.
            mNameComponent.transferFocusUpCycle();
            // Force the focus to be transfered to the next component.
            //mNameComponent.transferFocus();

            // This is no longer needed since the above line will force a
            // focusLostEvent. But I have choosen to leave this line here in
            // case it might become useful later.
            //stopCellEditing();
         }
      });
      mNameComponent.addFocusListener(new FocusAdapter()
      {
         public void focusGained(FocusEvent evt)
         {
            mNameComponent.setBackground(java.awt.Color.white);
         }
         public void focusLost(FocusEvent evt)
         {
            mNameComponent.setBackground(getParent().getBackground());
            //stopCellEditing();
            //XXX: Change name here.
            mConfigElement.setName(mNameComponent.getText());
         }
      });


      
   }

   private Icon                 mTypeIcon      = null;
   private ConfigElement        mConfigElement = null;
   private JTextField           mNameComponent = null;
}
