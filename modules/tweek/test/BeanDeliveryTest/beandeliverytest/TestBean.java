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

package beandeliverytest;

import javax.swing.*;
import java.awt.event.*;


public class TestBean extends JPanel
{
   public TestBean()
   {
      try
      {
         jbInit();
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }
   }

   private void jbInit() throws Exception
   {
      mButton.setText("Click Me");
      mButton.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            buttonClicked(e);
         }
      });
      this.add(mButtonPanel, null);
      mButtonPanel.add(mButton, null);
   }

   private void buttonClicked(ActionEvent e)
   {
      JOptionPane.showMessageDialog(null, "The downloaded Bean works!",
                                    "Happy Dialog",
                                    JOptionPane.INFORMATION_MESSAGE);

   }

   private JPanel mButtonPanel = new JPanel();
   private JButton mButton = new JButton();
}
