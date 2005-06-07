package com.incors.plaf.kunststoff.mini;

/*
 * This code was developed by INCORS GmbH (www.incors.com).
 * It is published under the terms of the GNU Lesser General Public License.
 */

import java.awt.*;
import javax.swing.*;
import javax.swing.plaf.*;
import javax.swing.plaf.metal.*;
import com.incors.plaf.kunststoff.KunststoffUtilities;


public class KunststoffMiniScrollButton extends MetalScrollButton
{
   public KunststoffMiniScrollButton(int direction, int width,
                                     boolean freeStanding)
   {
      super(direction, width, freeStanding);
   }

   /**
    * Calls the super classes paint(Graphics g) method and then paints two gradients.
    * The direction of the gradients depends on the direction of the scrollbar.
    */
   public void paint(Graphics g)
   {
      super.paint(g);
      int width = getWidth();
      int height = getHeight();
      Rectangle rectReflection;
      Rectangle rectShadow;
      boolean isVertical = (getDirection() == EAST || getDirection() == WEST);

      if (isVertical)
      {
         rectReflection = new Rectangle(1, 1, width, height/2);
         rectShadow = new Rectangle(1, height/2, width, height/2+1);
      }
      else
      {
         rectReflection = new Rectangle(1, 1, width/2, height);
         rectShadow = new Rectangle(width/2, 1, width/2+1, height);
      }

      // Paint reflection gradient.
      Color colorReflection =
         KunststoffMiniLookAndFeel.getComponentGradientColorReflection();

      if (colorReflection != null)
      {
         Color colorReflectionFaded =
            KunststoffUtilities.getTranslucentColor(colorReflection, 0);
         KunststoffUtilities.drawGradient(g, colorReflection,
                                          colorReflectionFaded, rectReflection,
                                          isVertical);
      }

      // Paint shadow gradient.
      Color colorShadow =
         KunststoffMiniLookAndFeel.getComponentGradientColorShadow();

      if (colorShadow != null)
      {
         Color colorShadowFaded =
            KunststoffUtilities.getTranslucentColor(colorShadow, 0);
         KunststoffUtilities.drawGradient(g, colorShadowFaded, colorShadow,
                                          rectShadow, isVertical);
      }
   }
}
