/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2009 by Iowa State University
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
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.vrjconfig.customeditors.cave;

import java.awt.Graphics;
import java.awt.Insets;
import java.awt.Rectangle;
import java.awt.Color;
import java.awt.Component;
import javax.swing.border.AbstractBorder;
import javax.swing.border.Border;

/**
 * A class which implements a computer screen border.
 */
public class ComputerScreenBorder extends AbstractBorder
{
   protected int mThickness;
   protected int mTotalStandHeight;
   protected Color lineColor;

   /** 
    * Creates a line border with the specified color and a 
    * thickness = 1.
    * @param color the color for the border
    */
   public ComputerScreenBorder(Color color)
   {
      this(color, 1);
   }

   /**
    * Creates a line border with the specified color and thickness.
    * @param color the color of the border
    * @param thickness the thickness of the border
    */
   public ComputerScreenBorder(Color color, int thickness)
   {
      lineColor = color;
      this.mThickness = thickness;
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
      Color oldColor = g.getColor();
      int i;

      int diam = 20;
      int center = x + (width/2);

      int arc_width = width/4;

      int middle_width = width/6;
      int middle_height = height/24;

      int bottom_width = width/4;
      int bottom_height = height/24;

      int middle_start_x = (center-(middle_width/2));
      int middle_start_y = (height-middle_height-bottom_height);

      int bottom_start_x = (center-(bottom_width/2));
      int bottom_start_y = (height-bottom_height);

      mTotalStandHeight = middle_height + bottom_height;

      g.setColor(lineColor);
      paintRaisedBevel(c, g, x, y, width, height-(mTotalStandHeight));
      paintLoweredBevel(c, g, x+mThickness, y+mThickness, width-(mThickness*2), height-(mThickness*2)-(mTotalStandHeight));

      paintRaisedBevel(c, g, middle_start_x, middle_start_y, middle_width, middle_height);
      paintRaisedBevel(c, g, bottom_start_x, bottom_start_y, bottom_width, bottom_height);
      //paintLoweredArcBevel(c, g, arc_start_x, arc_start_y, arc_width, arc_height);
      //g.fillArc(arc_start_x, arc_start_y, arc_width, arc_height, 0, 180);
      g.setColor(oldColor);
   }

   /**
    * Returns the insets of the border.
    * @param c the component for which this border insets value applies
    */
   public Insets getBorderInsets(Component c)
   {
      return new Insets(mThickness+4-1, mThickness+4-1, (mThickness+4-1)+(mTotalStandHeight+8-2), mThickness+4-1);
   }

   /** 
    * Reinitialize the insets parameter with this Border's current Insets. 
    * @param c the component for which this border insets value applies
    * @param insets the object to be reinitialized
    */
   public Insets getBorderInsets(Component c, Insets insets)
   {
      insets.left = insets.top = insets.right = mThickness+4-1;
      insets.bottom = (mThickness+4-1) + (mTotalStandHeight+8-2);
      return insets;
   }

   /**
   * Returns the color of the border.
   */
   public Color getLineColor()
   {
      return lineColor;
   }

   /**
     * Returns the thickness of the border.
     */
   public int getThickness()
   {
      return mThickness;
   }

   protected Color highlightOuter;
   protected Color highlightInner;
   protected Color shadowInner;
   protected Color shadowOuter;

   /**
    * Returns the outer highlight color of the bevel border
    * when rendered on the specified component.  If no highlight
    * color was specified at instantiation, the highlight color
    * is derived from the specified component's background color.
    * @param c the component for which the highlight may be derived
    */
   public Color getHighlightOuterColor(Component c)
   {
      Color highlight = getHighlightOuterColor();
      return ((highlight != null) ? highlight : c.getBackground().brighter().brighter());
   }

   /**
    * Returns the inner highlight color of the bevel border
    * when rendered on the specified component.  If no highlight
    * color was specified at instantiation, the highlight color
    * is derived from the specified component's background color.
    * @param c the component for which the highlight may be derived
    */
   public Color getHighlightInnerColor(Component c)
   {
      Color highlight = getHighlightInnerColor();
      return ((highlight != null) ? highlight : c.getBackground().brighter());
   }

   /**
    * Returns the inner shadow color of the bevel border
    * when rendered on the specified component.  If no shadow
    * color was specified at instantiation, the shadow color
    * is derived from the specified component's background color.
    * 
    * @param c the component for which the shadow may be derived
    */
   public Color getShadowInnerColor(Component c)
   {
      Color shadow = getShadowInnerColor();
      return ((shadow != null) ? shadow : c.getBackground().darker());
   }

   /**
    * Returns the outer shadow color of the bevel border
    * when rendered on the specified component.  If no shadow
    * color was specified at instantiation, the shadow color
    * is derived from the specified component's background color.
    * 
    * @param c the component for which the shadow may be derived
    */
   public Color getShadowOuterColor(Component c)
   {
      Color shadow = getShadowOuterColor();
      return ((shadow != null) ? shadow : c.getBackground().darker().darker());
   }

   /**
    * Returns the outer highlight color of the bevel border.
    * Will return null if no highlight color was specified
    * at instantiation.
    */
   public Color getHighlightOuterColor()
   {
      return highlightOuter;
   }

   /**
    * Returns the inner highlight color of the bevel border.
    * Will return null if no highlight color was specified
    * at instantiation.
    */
   public Color getHighlightInnerColor()
   {
      return highlightInner;
   }

   /**
    * Returns the inner shadow color of the bevel border.
    * Will return null if no shadow color was specified
    * at instantiation.
    */
   public Color getShadowInnerColor()
   {
      return shadowInner;
   }

   /**
    * Returns the outer shadow color of the bevel border.
    * Will return null if no shadow color was specified
    * at instantiation.
    */
   public Color getShadowOuterColor()
   {
      return shadowOuter;
   }

   /**
    * Returns whether or not the border is opaque.
    */
   protected void paintRaisedBevel(Component c, Graphics g, int x, int y,
                                   int width, int height)
   {
      Color oldColor = g.getColor();
      int h = height;
      int w = width;

      g.translate(x, y);

      g.setColor(getHighlightOuterColor(c));
      g.drawLine(0, 0, 0, h-2);
      g.drawLine(1, 0, w-2, 0);

      g.setColor(getHighlightInnerColor(c));
      g.drawLine(1, 1, 1, h-3);
      g.drawLine(2, 1, w-3, 1);

      g.setColor(getShadowOuterColor(c));
      g.drawLine(0, h-1, w-1, h-1);
      g.drawLine(w-1, 0, w-1, h-2);

      g.setColor(getShadowInnerColor(c));
      g.drawLine(1, h-2, w-2, h-2);
      g.drawLine(w-2, 1, w-2, h-3);

      g.translate(-x, -y);
      g.setColor(oldColor);
   }

   protected void paintLoweredBevel(Component c, Graphics g, int x, int y,
                                    int width, int height)
   {
      Color oldColor = g.getColor();
      int h = height;
      int w = width;

      g.translate(x, y);

      g.setColor(getShadowInnerColor(c));
      g.drawLine(0, 0, 0, h-1);
      g.drawLine(1, 0, w-1, 0);

      g.setColor(getShadowOuterColor(c));
      g.drawLine(1, 1, 1, h-2);
      g.drawLine(2, 1, w-2, 1);

      g.setColor(getHighlightOuterColor(c));
      g.drawLine(1, h-1, w-1, h-1);
      g.drawLine(w-1, 1, w-1, h-2);

      g.setColor(getHighlightInnerColor(c));
      g.drawLine(2, h-2, w-2, h-2);
      g.drawLine(w-2, 2, w-2, h-3);

      g.translate(-x, -y);
      g.setColor(oldColor);
   }

   protected void paintLoweredArcBevel(Component c, Graphics g, int arc_start_x, int arc_start_y,
                                       int arc_width, int arc_height)
   {
      Color oldColor = g.getColor();
      g.setColor(getShadowInnerColor(c));
      g.drawArc(arc_start_x, arc_start_y, arc_width-1, arc_height-1, 0, 180);
      g.setColor(getShadowOuterColor(c));
      g.drawArc(arc_start_x+1, arc_start_y+1, arc_width-2, arc_height-2, 0, 180);

      g.setColor(getHighlightOuterColor(c));
      g.drawLine(arc_start_x, arc_start_y, arc_width-1, arc_start_y);
      g.setColor(getHighlightInnerColor(c));
      g.drawLine(arc_start_x, arc_start_y, arc_width-1, arc_start_y+10);

      g.setColor(oldColor);
   }
}
