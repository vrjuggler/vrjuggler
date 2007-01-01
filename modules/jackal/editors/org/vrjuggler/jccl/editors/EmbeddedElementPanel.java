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

import java.awt.Color;
import java.awt.event.*;
import java.util.List;
import javax.swing.*;
import javax.swing.border.TitledBorder;

import com.sun.java.swing.plaf.windows.WindowsTreeUI;
import info.clearthought.layout.*;
import org.vrjuggler.jccl.config.ConfigContext;
import org.vrjuggler.jccl.config.ConfigElement;

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
   
   public EmbeddedElementPanel(ConfigContext ctx, ConfigElement elm, Color color)
   {
      super(color);
      
      mConfigElement = elm;

      ClassLoader loader = getClass().getClassLoader();
      mTypeIcon = new ImageIcon(loader.getResource("org/vrjuggler/jccl/editors/images/config_element.gif"));
      mTypeButton.setIcon(mTypeIcon);
      
      // Create a panel for the embedded element.
      //PropertySheet new_sheet = new PropertySheet(mConfigElement, getNextColor());
      PropertySheet new_sheet = PropertySheetFactory.instance().makeSheet(ctx, mConfigElement, getNextColor());
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

      TableLayoutConstraints c2 =
         new TableLayoutConstraints(TITLE_COLUMN, TITLE_ROW,
                                    TITLE_COLUMN, TITLE_ROW,
                                    TableLayout.FULL, TableLayout.CENTER);
      add(mNameComponent, c2);

      mNameComponent.setBorder(BorderFactory.createLoweredBevelBorder());
      // Save the initial background color for later use.
      mNameComponentBG = getBackground().brighter();
      mNameComponent.setBackground(mNameComponentBG);

      // The following two listeners allow the user to change the name of the
      // embedded element.
      mNameComponent.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            // Restore the original background color.
            mNameComponent.setBackground(mNameComponentBG);
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
            mNameComponent.setBackground(Color.white);
         }
         public void focusLost(FocusEvent evt)
         {
            // Restore the original background color.
            mNameComponent.setBackground(mNameComponentBG);
            //stopCellEditing();
            //XXX: Change name here.
            mConfigElement.setName(mNameComponent.getText());
         }
      });
   }

   /**
    * Provides package visible access to the value edited by this panel;
    *
    * @since 0.92.11
    */
   Object getValue()
   {
      return mConfigElement;
   }

   private Icon                 mTypeIcon        = null;
   private ConfigElement        mConfigElement   = null;
   private JTextField           mNameComponent   = null;
   private Color                mNameComponentBG = null;
}
