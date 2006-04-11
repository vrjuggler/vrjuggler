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