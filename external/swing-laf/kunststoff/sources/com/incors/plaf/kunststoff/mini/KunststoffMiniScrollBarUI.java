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


public class KunststoffMiniScrollBarUI extends MetalScrollBarUI
{
   private int scrollBarWidth = 9;

   public KunststoffMiniScrollBarUI()
   {
      super();
   }

   public static ComponentUI createUI(JComponent c)
   {
      return new KunststoffMiniScrollBarUI();
   }

   protected void configureScrollBarColors()
   {
      super.configureScrollBarColors();

      // The following is a bit of a hack to force the size of the scrollbar.
      boolean isVertical = (this.scrollbar.getOrientation() == NORTH ||
                            this.scrollbar.getOrientation() == SOUTH);
      Dimension size = this.scrollbar.getPreferredSize();

      if ( isVertical )
      {
         size.width = this.scrollBarWidth;
      }
      else
      {
         size.height = this.scrollBarWidth;
      }

      this.scrollbar.setPreferredSize(size);
   }

   protected JButton createDecreaseButton(int orientation)
   {
      decreaseButton = new KunststoffMiniScrollButton(orientation,
                                                      scrollBarWidth,
                                                      isFreeStanding);
      return decreaseButton;
   }

   protected JButton createIncreaseButton(int orientation)
   {
      increaseButton =  new KunststoffMiniScrollButton(orientation,
                                                       scrollBarWidth,
                                                       isFreeStanding);
      return increaseButton;
   }

   /**
    * Calls the super class' paint(Graphics g) method and then paints two
    * gradients.  The direction of the gradients depends on the direction of
    * the scrollbar.
    */
   protected void paintThumb(Graphics g, JComponent c, Rectangle thumbBounds)
   {
      super.paintThumb(g, c, thumbBounds);

      // colors for the reflection gradient
      Color colorReflection = KunststoffMiniLookAndFeel.getComponentGradientColorReflection();
      Color colorReflectionFaded = KunststoffUtilities.getTranslucentColor(colorReflection, 0);
      // colors for the shadow gradient
      Color colorShadow = KunststoffMiniLookAndFeel.getComponentGradientColorShadow();
      Color colorShadowFaded = KunststoffUtilities.getTranslucentColor(colorShadow, 0);

      Rectangle rectReflection;  // rectangle for the reflection gradient
      Rectangle rectShadow;  // rectangle for the shadow gradient
      if (scrollbar.getOrientation() == JScrollBar.VERTICAL)
      {
         rectReflection = new Rectangle(thumbBounds.x+1, thumbBounds.y, thumbBounds.width/2, thumbBounds.height);
         rectShadow = new Rectangle(thumbBounds.x + thumbBounds.width/2, thumbBounds.y, thumbBounds.width/2+1, thumbBounds.height);
      }
      else
      {
         rectReflection = new Rectangle(thumbBounds.x, thumbBounds.y+1, thumbBounds.width, thumbBounds.height/2);
         rectShadow = new Rectangle(thumbBounds.x, thumbBounds.y + thumbBounds.height/2, thumbBounds.width, thumbBounds.height/2+1);
      }

      // the direction of the gradient is orthogonal to the direction of the scrollbar
      boolean isVertical = (scrollbar.getOrientation() == JScrollBar.HORIZONTAL);
      KunststoffUtilities.drawGradient(g, colorReflection, colorReflectionFaded, rectReflection, isVertical);
      KunststoffUtilities.drawGradient(g, colorShadowFaded, colorShadow, rectShadow, isVertical);
   }
}
