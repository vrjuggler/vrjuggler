package com.incors.plaf.kunststoff.mini;

/*
 * This code was developed by Jerason Banes (jbanes@techie.com).
 * It is published under the terms of the GNU Lesser General Public License.
 */

import java.awt.*;

import javax.swing.*;
import javax.swing.plaf.metal.*;
import com.incors.plaf.kunststoff.KunststoffUtilities;


public class KunststoffMiniInternalFrameTitlePane extends MetalInternalFrameTitlePane
{
   public KunststoffMiniInternalFrameTitlePane(JInternalFrame frame)
   {
      super(frame);
   }

   public void paintComponent(Graphics g)
   {
      super.paintComponent(g);

      Graphics2D g2D = (Graphics2D) g;

      // paint reflection
      Color colorReflection = KunststoffMiniLookAndFeel.getComponentGradientColorReflection();
      Color colorReflectionFaded = KunststoffUtilities.getTranslucentColor(colorReflection, 0);
      Rectangle rectReflection = new Rectangle(0, 1, this.getWidth(), this.getHeight()/2);;
      KunststoffUtilities.drawGradient(g, colorReflection, colorReflectionFaded, rectReflection, true);

      // paint shadow
      Color colorShadow = KunststoffMiniLookAndFeel.getComponentGradientColorShadow();
      Color colorShadowFaded = KunststoffUtilities.getTranslucentColor(colorShadow, 0);
      Rectangle rectShadow = new Rectangle(0, this.getHeight()/2, this.getWidth(), this.getHeight()/2+1);
      KunststoffUtilities.drawGradient(g, colorShadowFaded, colorShadow, rectShadow, true);
   }
}
