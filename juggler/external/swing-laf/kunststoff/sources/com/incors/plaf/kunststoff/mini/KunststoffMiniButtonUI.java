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


public class KunststoffMiniButtonUI extends MetalButtonUI
{
   private final static KunststoffMiniButtonUI buttonUI =
      new KunststoffMiniButtonUI();

   public static ComponentUI createUI(JComponent c)
   {
      JButton button = (JButton) c;
      button.setMargin(new Insets(1, 1, 1, 1));
      return buttonUI;
   }

   public void paint(Graphics g, JComponent c)
   {
      super.paint(g, c);

      if (!c.isOpaque())
      {
         return;  // we only paint the gradients if the button is opaque.
         // Thanks to Christoph Wilhelms for proposing this.
      }

      Component parent = c.getParent();
      if(parent instanceof JToolBar)
      {
         int orientation = ((JToolBar) parent).getOrientation();
         Point loc = c.getLocation();
         Rectangle rectReflection;
         Rectangle rectShadow;
         Color colorReflection = KunststoffMiniLookAndFeel.getComponentGradientColorReflection();
         Color colorShadow = KunststoffMiniLookAndFeel.getComponentGradientColorShadow();

         if(orientation == SwingConstants.HORIZONTAL)  // if tool bar orientation is horizontal
         {
            // paint upper gradient
            if (colorReflection != null)
            {
               Color colorReflectionFaded = KunststoffUtilities.getTranslucentColor(colorReflection, 0);
               rectReflection = new Rectangle(0, -loc.y, parent.getWidth(), parent.getHeight()/2);
               KunststoffUtilities.drawGradient(g, colorReflection, colorReflectionFaded, rectReflection, true);
            }

            // paint lower gradient
            if (colorShadow != null)
            {
               Color colorShadowFaded = KunststoffUtilities.getTranslucentColor(colorShadow, 0);
               rectShadow = new Rectangle(0, parent.getHeight()/2 - loc.y, parent.getWidth(), parent.getHeight()/2);
               KunststoffUtilities.drawGradient(g, colorShadowFaded, colorShadow, rectShadow, true);
            }
         }
         else  // if tool bar orientation is vertical
         {
            // paint left gradient
            if (colorReflection != null)
            {
               Color colorReflectionFaded = KunststoffUtilities.getTranslucentColor(colorReflection, 0);
               rectReflection = new Rectangle(0, 0, parent.getWidth()/2, parent.getHeight());
               KunststoffUtilities.drawGradient(g, colorReflection, colorReflectionFaded, rectReflection, false);
            }

            // paint right gradient
            if (colorShadow != null)
            {
               Color colorShadowFaded = KunststoffUtilities.getTranslucentColor(colorShadow, 0);
               rectShadow = new Rectangle(parent.getWidth()/2 - loc.x, 0, parent.getWidth(), parent.getHeight());
               KunststoffUtilities.drawGradient(g, colorShadowFaded, colorShadow, rectShadow, false);
            }
         }
      }
      else // if not in toolbar
      {
         // Paint upper gradient.
         Color colorReflection = KunststoffMiniLookAndFeel.getComponentGradientColorReflection();
         if (colorReflection != null)
         {
            Color colorReflectionFaded = KunststoffUtilities.getTranslucentColor(colorReflection, 0);
            Rectangle rect = new Rectangle(0, 0, c.getWidth(), c.getHeight()/2);
            KunststoffUtilities.drawGradient(g, colorReflection, colorReflectionFaded, rect, true);
         }

         // Paint lower gradient.
         Color colorShadow = KunststoffMiniLookAndFeel.getComponentGradientColorShadow();
         if (colorShadow != null)
         {
            Color colorShadowFaded = KunststoffUtilities.getTranslucentColor(colorShadow, 0);
            Rectangle rect = new Rectangle(0, c.getHeight()/2, c.getWidth(), c.getHeight()/2);
            KunststoffUtilities.drawGradient(g, colorShadowFaded, colorShadow, rect, true);
         }
      }
   }
}
