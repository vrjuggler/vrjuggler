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
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.vrjconfig.controlpanel;

import java.awt.*;

/**
 * Layout which forces control panel buttons to appear in
 * similar locations to a WinXP control panel.
 *
 * @see LayoutManager
 */
public class ControlPanelLayout
   implements LayoutManager
{
   /**
    * Sets the horizontal gap between components.
    */
   public void setHorizontalGap(int horiz)
   {
      hgap = horiz;
   }

   public int getHorizontalGap()
   {
      return hgap;
   }

   /**
    * Sets the vertical gap between components.
    */
   public void setVerticalGap(int vert)
   {
      vgap = vert;
   }

   public int getVerticalGap()
   {
      return vgap;
   }

   private void calcSizes(Container parent)
   {
      int num_comps = parent.getComponentCount();

      maxPrefSize.width = 0;
      maxPrefSize.height = 0;
      maxMinSize.width = 0;
      maxMinSize.height = 0;

      // Compute the cell size to the biggest component.
      for (int i=0; i<num_comps; ++i)
      {
         Component c = parent.getComponent(i);
         if (c.isVisible())
         {
            // Check preferred size
            Dimension pref = c.getPreferredSize();

            if (pref.width > maxPrefSize.width)
            {
               maxPrefSize.width = pref.width;
            }
            if (pref.height > maxPrefSize.height)
            {
               maxPrefSize.height = pref.height;
            }

            // Check minimum size
            Dimension min = c.getMinimumSize();

            if (min.width > maxMinSize.width)
            {
               maxMinSize.width = min.width;
            }
            if (min.height > maxMinSize.height)
            {
               maxMinSize.height = min.height;
            }
         }
      }

      // Let layoutContainer() know that that it doesn't need to call this
      // method the next time the container asks to be laid out.
      needCalcSizes = false;
   }

   public void addLayoutComponent(String name, Component comp)
   {
   }

   public void removeLayoutComponent(Component comp)
   {
   }

   public Dimension preferredLayoutSize(Container parent)
   {
      calcSizes(parent);

      int num_comps = parent.getComponentCount();

      // Take into account the parent's insets
      Insets insets = parent.getInsets();
      int cols = 2;
      int rows = (num_comps / 2) + 1;

      Dimension pref_size = new Dimension();
      pref_size.width   = cols * maxPrefSize.width
                        + hgap
                        + insets.left + insets.right;
      pref_size.height  = rows * maxPrefSize.height
                        + (rows-1) * vgap
                        + insets.top + insets.bottom;

      return pref_size;
   }

   public Dimension minimumLayoutSize(Container parent)
   {
      calcSizes(parent);

      int num_comps = parent.getComponentCount();

      // Take into account the parent's insets
      Insets insets = parent.getInsets();
      int cols = 2;
      int rows = (num_comps / 2) + 1;

      Dimension min_size = new Dimension();
      min_size.width    = cols * maxMinSize.width
                        + hgap
                        + insets.left + insets.right;
      min_size.height   = rows * maxMinSize.height
                        + (rows-1) * vgap
                        + insets.top + insets.bottom;

      return min_size;
   }

   public void layoutContainer(Container parent)
   {
      Insets insets = parent.getInsets();
      int max_width = parent.getSize().width - (insets.left + insets.right);
      int max_height = parent.getSize().height - (insets.top + insets.bottom);

      int num_comps = parent.getComponentCount();
      int x = insets.left;
      int y = insets.top;

      // Calculate the components' sizes if it hasn't been done already.
      if (needCalcSizes)
      {
         calcSizes(parent);
      }

      for (int i=0; i<num_comps; ++i)
      {
         Component c = parent.getComponent(i);
         if (c.isVisible())
         {
            Dimension d = c.getPreferredSize();
            c.setBounds(x, y, d.width, d.height);

            // Calculate the position of the next component ...
            if (i % 2 == 0)
            {
               // Should be in the right column
               x += maxPrefSize.width + hgap;
//               x += Math.max(maxPrefSize.width, max_width/2-(hgap/2)) + hgap;
            }
            else
            {
               // Should be in left column, next line
               x = insets.left;
               y += maxPrefSize.height + vgap;
            }
         }
      }
   }

   /**
    * The biggest minimum size specified by a component.
    */
   private Dimension maxMinSize = new Dimension();

   /**
    * The biggest preferred size specified by a component.
    */
   private Dimension maxPrefSize = new Dimension();

   /**
    * Flag for whether the component sizes have been calculated.
    */
   private boolean needCalcSizes = true;

   private int hgap = 5;
   private int vgap = 5;
}
