/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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

package org.vrjuggler.vrjconfig.customeditors.cave;

import java.awt.Graphics;
import java.awt.Insets;
import java.awt.Rectangle;
import java.awt.Color;
import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.Point;
import java.awt.Toolkit;
import java.awt.Component;
import java.awt.Dimension;

import javax.swing.border.*;
import javax.swing.Icon;
import javax.swing.UIManager;

public class TitledIconBorder extends TitledBorder
{
   private Point textLoc = new Point();
   protected Icon mIcon;
   
   /**
    * Creates a TitledBorder instance.
    * 
    * @param title  the title the border should display
    */
   public TitledIconBorder(String title, Icon icon)
   {
      super(null, title, LEADING, TOP, null, null);
      mIcon = icon;
   }

   /**
    * Paints the border for the specified component with the 
    * specified position and size.
    * @param c the component for which this border is being painted
    * @param g the paint graphics
    * @param x the x position of the painted border
    * @param y the y position of the painted border
    * @param width the width of the painted border
    * @param height the height of the painted border
    */
   public void paintBorder(Component c, Graphics g, int x, int y, int width, int height)
   {
      Border border = getBorder();

      if (getTitle() == null || getTitle().equals(""))
      {
         if (border != null)
         {
            border.paintBorder(c, g, x, y, width, height);
         }
         return;
      }

      Rectangle grooveRect = new Rectangle(x + EDGE_SPACING, y + EDGE_SPACING,
                                           width - (EDGE_SPACING * 2),
                                           height - (EDGE_SPACING * 2));

      int icon_width = mIcon.getIconWidth();
      int icon_height = mIcon.getIconHeight();

      Font font = g.getFont();
      Color color = g.getColor();

      g.setFont(getFont(c));

      FontMetrics fm = g.getFontMetrics();
      int         fontHeight = fm.getHeight();
      int         descent = fm.getDescent();
      int         ascent = fm.getAscent();
      int         diff;
      int         stringWidth = fm.stringWidth(getTitle());
      Insets      insets;

      int         real_width = stringWidth + icon_width;

      if (border != null)
      {
         insets = border.getBorderInsets(c);
      }
      else
      {
         insets = new Insets(0, 0, 0, 0);
      }

      int titlePos = getTitlePosition();
      switch (titlePos)
      {
      case ABOVE_TOP:
         diff = ascent + descent + (Math.max(EDGE_SPACING,
                           TEXT_SPACING*2) - EDGE_SPACING);
         grooveRect.y += diff;
         grooveRect.height -= diff;
         textLoc.y = grooveRect.y - (descent + TEXT_SPACING);
         break;
      case TOP:
      case DEFAULT_POSITION:
         diff = Math.max(0, ((ascent/2) + TEXT_SPACING) - EDGE_SPACING);
         grooveRect.y += diff;
         grooveRect.height -= diff;
         textLoc.y = (grooveRect.y - descent) +
                     (insets.top + ascent + descent)/2;
         break;
      case BELOW_TOP:
         textLoc.y = grooveRect.y + insets.top + ascent + TEXT_SPACING;
         break;
      case ABOVE_BOTTOM:
         textLoc.y = (grooveRect.y + grooveRect.height) -
                     (insets.bottom + descent + TEXT_SPACING);
         break;
      case BOTTOM:
         grooveRect.height -= fontHeight/2;
         textLoc.y = ((grooveRect.y + grooveRect.height) - descent) +
                     ((ascent + descent) - insets.bottom)/2;
         break;
      case BELOW_BOTTOM:
         grooveRect.height -= fontHeight;
         textLoc.y = grooveRect.y + grooveRect.height + ascent +
                     TEXT_SPACING;
         break;
      }

      int justification = getTitleJustification();
      if(isLeftToRight(c))
      {
         if(justification==LEADING || 
            justification==DEFAULT_JUSTIFICATION)
         {
            justification = LEFT;
         }
         else if(justification==TRAILING)
         {
            justification = RIGHT;
         }
      }
      else
      {
         if(justification==LEADING ||
            justification==DEFAULT_JUSTIFICATION)
         {
            justification = RIGHT;
         }
         else if(justification==TRAILING)
         {
            justification = LEFT;
         }
      }

      switch (justification)
      {
      case LEFT:
         textLoc.x = grooveRect.x + TEXT_INSET_H + insets.left;
         break;
      case RIGHT:
         textLoc.x = (grooveRect.x + grooveRect.width) -
                     (real_width + TEXT_INSET_H + insets.right);
         break;
      case CENTER:
         textLoc.x = grooveRect.x + ((grooveRect.width - real_width) / 2);
         break;
      }

        // If title is positioned in middle of border AND its fontsize
	// is greater than the border's thickness, we'll need to paint 
	// the border in sections to leave space for the component's background 
	// to show through the title.
        //
        if (border != null) {
            if (((titlePos == TOP || titlePos == DEFAULT_POSITION) &&
		  (grooveRect.y > textLoc.y - ascent)) ||
		 (titlePos == BOTTOM && 
		  (grooveRect.y + grooveRect.height < textLoc.y + descent))) {
		  
                Rectangle clipRect = new Rectangle();
                
                // save original clip
                Rectangle saveClip = g.getClipBounds();

                // paint strip left of text
                clipRect.setBounds(saveClip);
                if (computeIntersection(clipRect, x, y, textLoc.x-1-x, height)) {
                    g.setClip(clipRect);
                    border.paintBorder(c, g, grooveRect.x, grooveRect.y,
                                  grooveRect.width, grooveRect.height);
                }

                // paint strip right of text
                clipRect.setBounds(saveClip);
                if (computeIntersection(clipRect, textLoc.x + real_width + 1, y,
                               x+width-(textLoc.x + real_width + 1), height)) {
                    g.setClip(clipRect);
                    border.paintBorder(c, g, grooveRect.x, grooveRect.y,
                                  grooveRect.width, grooveRect.height);
                }

                if (titlePos == TOP || titlePos == DEFAULT_POSITION) {
                    // paint strip below text
                    clipRect.setBounds(saveClip);
                    if (computeIntersection(clipRect, textLoc.x-1, textLoc.y+descent, 
                                        real_width + 2, y+height-textLoc.y-descent)) {
                        g.setClip(clipRect);
                        border.paintBorder(c, g, grooveRect.x, grooveRect.y,
                                  grooveRect.width, grooveRect.height);
                    }

                } else { // titlePos == BOTTOM
		  // paint strip above text
                    clipRect.setBounds(saveClip);
                    if (computeIntersection(clipRect, textLoc.x-1, y, 
                          real_width + 2, textLoc.y - ascent - y)) {
                        g.setClip(clipRect); 
                        border.paintBorder(c, g, grooveRect.x, grooveRect.y,
                                  grooveRect.width, grooveRect.height);
                    }
                }

                // restore clip
                g.setClip(saveClip);   

            } else {
                border.paintBorder(c, g, grooveRect.x, grooveRect.y,
                                  grooveRect.width, grooveRect.height);
            }
        }

        g.setColor(getTitleColor());
        g.drawString(getTitle(), textLoc.x + icon_width, textLoc.y);

        g.setFont(font);
        g.setColor(color);

         int icon_xpos, icon_ypos;

         icon_xpos = textLoc.x;
         icon_ypos = textLoc.y - (icon_height/2);

         // Paint icon
         mIcon.paintIcon(c, g, icon_xpos, icon_ypos);
   }

    /** 
     * Reinitialize the insets parameter with this Border's current Insets. 
     * @param c the component for which this border insets value applies
     * @param insets the object to be reinitialized
     */
    public Insets getBorderInsets(Component c, Insets insets) {
        FontMetrics fm;
        int         descent = 0;
        int         ascent = 16;
	int         height = 16;

        Border border = getBorder();
        if (border != null) {
            if (border instanceof AbstractBorder) {
                ((AbstractBorder)border).getBorderInsets(c, insets);
            } else {
                // Can't reuse border insets because the Border interface
                // can't be enhanced.
                Insets i = border.getBorderInsets(c);
                insets.top = i.top;
                insets.right = i.right;
                insets.bottom = i.bottom;
                insets.left = i.left;
            }
        } else {
            insets.left = insets.top = insets.right = insets.bottom = 0;
        }

        insets.left += EDGE_SPACING + TEXT_SPACING;
        insets.right += EDGE_SPACING + TEXT_SPACING;
        insets.top += EDGE_SPACING + TEXT_SPACING;
        insets.bottom += EDGE_SPACING + TEXT_SPACING;

        if(c == null || getTitle() == null || getTitle().equals(""))    {
            return insets;
        }

        Font font = getFont(c);

        fm = c.getFontMetrics(font);

	if(fm != null) {
  	   descent = fm.getDescent();
	   ascent = fm.getAscent();
	   height = fm.getHeight();
	}

        switch (getTitlePosition()) {
          case ABOVE_TOP:
              insets.top += ascent + descent
                            + (Math.max(EDGE_SPACING, TEXT_SPACING*2)
                            - EDGE_SPACING);
              break;
          case TOP:
          case DEFAULT_POSITION:
              insets.top += ascent + descent;

              if (insets.top < mIcon.getIconHeight())
              {
                 insets.top = mIcon.getIconHeight();
              }
              break;
          case BELOW_TOP:
              insets.top += ascent + descent + TEXT_SPACING;
              break;
          case ABOVE_BOTTOM:
              insets.bottom += ascent + descent + TEXT_SPACING;
              break;
          case BOTTOM:
              insets.bottom += ascent + descent;
              break;
          case BELOW_BOTTOM:
              insets.bottom += height;
              break;
        }
        return insets;
    }
   
   /**
   * Returns the icon used for tiling the border or null
   * if a solid color is being used.
   */
   public Icon getIcon()
   {
      return mIcon;
   }
   
   public void setIcon(Icon icon)
   {
      mIcon = icon;
   }
   
   private static boolean computeIntersection(Rectangle dest, 
                                              int rx, int ry,
                                              int rw, int rh)
   {
      int x1 = Math.max(rx, dest.x);
      int x2 = Math.min(rx + rw, dest.x + dest.width);
      int y1 = Math.max(ry, dest.y);
      int y2 = Math.min(ry + rh, dest.y + dest.height);
      dest.x = x1;
      dest.y = y1;
      dest.width = x2 - x1;
      dest.height = y2 - y1;

      if (dest.width <= 0 || dest.height <= 0)
      {
         return false;
      }
      return true;
   }
   
   /*
   * Convenience function for determining ComponentOrientation.  
   * Helps us avoid having Munge directives throughout the code.
   */
   static boolean isLeftToRight( Component c )
   {
      return c.getComponentOrientation().isLeftToRight();
   }
}
