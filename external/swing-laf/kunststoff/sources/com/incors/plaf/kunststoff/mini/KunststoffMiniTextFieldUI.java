package com.incors.plaf.kunststoff.mini;

/*
 * This code was developed by INCORS GmbH (www.incors.com).
 * It is published under the terms of the GNU Lesser General Public License.
 */

import java.awt.*;
import javax.swing.*;
import javax.swing.plaf.*;
import javax.swing.plaf.basic.*;
import com.incors.plaf.kunststoff.KunststoffUtilities;


public class KunststoffMiniTextFieldUI extends BasicTextFieldUI
{
   protected JComponent myComponent;

   public KunststoffMiniTextFieldUI()
   {
      super();
   }

   KunststoffMiniTextFieldUI(JComponent c)
   {
      super();
      myComponent = c;
   }

   public static ComponentUI createUI(JComponent c)
   {
      return new KunststoffMiniTextFieldUI(c);
   }

   protected void paintBackground(Graphics g)
   {
      super.paintBackground(g);

      // paint upper gradient
      Color colorReflection = KunststoffMiniLookAndFeel.getTextComponentGradientColorReflection();
      if (colorReflection != null)
      {
         Color colorReflectionFaded = KunststoffUtilities.getTranslucentColor(colorReflection, 0);
         Rectangle rect = new Rectangle(0, 0, myComponent.getWidth(), myComponent.getHeight()/2);
         KunststoffUtilities.drawGradient(g, colorReflection, colorReflectionFaded, rect, true);
      }

      // paint lower gradient
      Color colorShadow = KunststoffMiniLookAndFeel.getTextComponentGradientColorShadow();
      if (colorShadow != null)
      {
         Color colorShadowFaded = KunststoffUtilities.getTranslucentColor(colorShadow, 0);
         Rectangle rect = new Rectangle(0, myComponent.getHeight()/2, myComponent.getWidth(), myComponent.getHeight()/2);
         KunststoffUtilities.drawGradient(g, colorShadowFaded, colorShadow, rect, true);
      }
   }
}
