package org.vrjuggler.jccl.editors;

import javax.swing.JPanel;
import java.awt.Color;

public class PropertyComponent extends JPanel
{
   public void refresh()
   {
      doLayout();
      repaint();
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
   protected Color mColor;
}
