package org.vrjuggler.jccl.editors;

import javax.swing.JPanel;
import java.awt.Color;
import java.awt.Dimension;
import info.clearthought.layout.*;

public class PropertyComponent extends JPanel
{
   public void refresh()
   {
      doLayout();
      repaint();
      
      //JComponent parent = (JComponent)SwingUtilities.getAncestorOfClass(JScrollPane.class, getParent());
      //parent.doLayout();
      //parent.repaint();
   }

   public void updateMyRow()
   {
      if(getParent() != null && getParent() instanceof PropertyComponent)
      {
         PropertyComponent temp = (PropertyComponent)getParent();
         
         if(temp.getParent() != null && temp.getParent() instanceof ExpandablePanel)
         {
            ExpandablePanel temp_list = (ExpandablePanel)temp.getParent();
            temp_list.updateMyRow();
            System.out.println("Updating recursively.");
         }
         if(temp.getLayout() instanceof TableLayout)
         {
            TableLayout tl = (TableLayout)temp.getLayout();
            // Get the row that this panel is in.
            TableLayoutConstraints tlc = tl.getConstraints(this);
            int row = tlc.row1;
         
            Dimension pref_size = this.getPreferredSize();
            int height = (int)pref_size.getHeight();
         
            tl.setRow(row, height);
            temp.doLayout();
            temp.repaint();
            //refresh();
            //System.out.println("Adjusting row: " + row + " to " + height);
         
            //temp.doLayout();
            //temp.repaint();
         }
         temp.invalidate();
         temp.validate();
      }
   }
   
   public Color getNextColor()
   {
      boolean inc = false;

      int r = mColor.getRed();
      int g = mColor.getGreen();
      int b = mColor.getBlue();
      float hsb[] = {0.0f,0.0f,0.0f};
      
      Color.RGBtoHSB(r,g,b,hsb);

      if(hsb[1] > 0.7)
      {
         inc = false;
      }
      else
      {
         inc = true;
      }

      if(inc)
      {
         hsb[1] += 0.20;
      }
      else
      {
         hsb[1] -= 0.20;
      }

      Color new_color = Color.getHSBColor(hsb[0], hsb[1], hsb[2]);
      return new_color;
      
      /*
      int red_val = mColor.getRed();
      if(red_val == 85)
      {
         return(new Color(124, 100, 140));
      }
      else if(red_val == 124)
      {
         return(new Color(242, 211, 118));
      }
      else if(red_val == 242)
      {
         return(new Color(85, 140, 77));
      }
      */
      //return mColor;
   }
   public void setColor(Color new_color)
   {
      mColor = new_color;
   }
   public Color getColor()
   {
      return mColor;
   }
   protected Color mColor;
}
