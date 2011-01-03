/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.vrjconfig.customeditors.display_window;

import javax.swing.*;
import java.awt.*;


public class DesktopBackgroundImage extends JComponent
{
   public void setImageFile(String imageFile)
   {
      setImageFile(getClass().getResource(imageFile));
   }

   public void setImageFile(java.net.URL url)
   {
      if (url != null)
      {
         mImage = Toolkit.getDefaultToolkit().getImage(url);
         if ( mImage != null )
         {
            MediaTracker tracker = new MediaTracker(this);
            tracker.addImage(mImage, 0);
            try
            {
               tracker.waitForAll();
            }
            catch (InterruptedException e)
            {
               System.out.println(e);
               e.printStackTrace();
            }
         }
      }
   }

   protected void drawInnerBorder(Graphics g, int x, int y, int w, int h)
   {
      Color oldColor = g.getColor();

      g.translate(x, y);

      Color highlightOuterColor = getBackground().brighter().brighter();
      Color highlightInnerColor = getBackground().brighter();
      Color shadowOuterColor = getBackground().darker().darker();
      Color shadowInnerColor = getBackground().darker();

      g.setColor(shadowInnerColor);
      g.drawLine(0, 0, 0, h-1);
      g.drawLine(1, 0, w-1, 0);

      g.setColor(shadowOuterColor);
      g.drawLine(1, 1, 1, h-2);
      g.drawLine(2, 1, w-2, 1);

      g.setColor(highlightOuterColor);
      g.drawLine(0, h-1, w-1, h-1);
      g.drawLine(w-1, 1, w-1, h-2);

      g.setColor(highlightInnerColor);
      g.drawLine(2, h-2, w-2, h-2);
      g.drawLine(w-2, 2, w-2, h-3);

      g.translate(-x, -y);
      g.setColor(oldColor);
   }

   protected void drawBar(Graphics g, int x, int y, int w, int h)
   {
      Color oldColor = g.getColor();

      g.translate(x, y);

      Color highlightOuterColor = getBackground().brighter().brighter();
      Color highlightInnerColor = getBackground().brighter();
      Color shadowOuterColor = getBackground().darker().darker();
      Color shadowInnerColor = getBackground().darker();

      g.setColor(shadowInnerColor);
      g.drawLine(1, 0, w-1, 0);

      g.setColor(shadowOuterColor);
      g.drawLine(2, 1, w-2, 1);

      g.setColor(highlightOuterColor);
      g.drawLine(0, -1, w-1, -1);

      g.setColor(highlightInnerColor);
      g.drawLine(2, -2, w-2, -2);

      g.translate(-x, -y);
      g.setColor(oldColor);
   }

   protected void paintComponent(Graphics g)
   {
      if ( ! (g instanceof Graphics2D) )
      {
         return;
      }

      Insets insets = getInsets();

      Dimension d;
      if(getParent() != null)
      {
         d = getParent().getSize();
         setSize(d);
      }
      else
      {
         d = getSize();
      }

      if ( mImage != null )
      {
         int current_width = d.width - insets.left - insets.right;
         int current_height = d.height - insets.top - insets.bottom;

         g.drawImage(mImage, insets.left, insets.right, current_width,
                     current_height, this);
      }
   }

   private Image mImage = null;
}
