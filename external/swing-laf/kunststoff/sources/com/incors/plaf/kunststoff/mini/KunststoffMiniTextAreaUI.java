package com.incors.plaf.kunststoff.mini;

/*
 * This code was developed by INCORS GmbH (www.incors.com)
 * based on a contribution by Timo Haberkern.
 * It is published under the terms of the Lesser GNU Public License.
 */

import java.awt.*;
import javax.swing.*;
import javax.swing.plaf.*;
import javax.swing.plaf.basic.*;
import com.incors.plaf.kunststoff.KunststoffUtilities;


public class KunststoffMiniTextAreaUI extends BasicTextAreaUI
{
   protected JComponent myComponent;

   public KunststoffMiniTextAreaUI(JComponent c)
   {
      super();
      myComponent = c;
   }

   public static ComponentUI createUI(JComponent c)
   {
      return new KunststoffMiniTextAreaUI(c);
   }

   protected void paintBackground(Graphics g)
   {
      super.paintBackground(g);
      Rectangle editorRect = getVisibleEditorRect();

      // paint upper gradient
      Color colorReflection = KunststoffMiniLookAndFeel.getTextComponentGradientColorReflection();
      if (colorReflection != null)
      {
         Color colorReflectionFaded = KunststoffUtilities.getTranslucentColor(colorReflection, 0);
         Rectangle rect = new Rectangle(editorRect.x, editorRect.y, editorRect.width, editorRect.height/2);
         KunststoffUtilities.drawGradient(g, colorReflection, colorReflectionFaded, rect, true);
      }

      // paint lower gradient
      Color colorShadow = KunststoffMiniLookAndFeel.getTextComponentGradientColorShadow();
      if (colorShadow != null)
      {
         Color colorShadowFaded = KunststoffUtilities.getTranslucentColor(colorShadow, 0);
         Rectangle rect = new Rectangle(editorRect.x, editorRect.y+editorRect.height/2, editorRect.width, editorRect.height/2);
         KunststoffUtilities.drawGradient(g, colorShadowFaded, colorShadow, rect, true);
      }
   }
}
